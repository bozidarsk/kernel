BUILD = build

CC = x86_64-linux-gnu-gcc
ASM = nasm
CFLAGS = -ffreestanding -fno-stack-protector -maccumulate-outgoing-args -fno-stack-check -fno-pic -fno-pie -finline -fshort-wchar -masm=intel -mno-red-zone -msse4.2 -mno-avx -nostdinc -Wall -Wextra -std=c23 -I include
ASMFLAGS = -f elf64
LDFLAGS = -nostdlib -znocombreloc -static -no-pie -L $(BUILD)

QEMUFLAGS = \
\
	-d int \
	-no-reboot \
	-no-shutdown \
	-serial stdio \
\
	-nodefaults \
	-no-user-config \
	-machine q35 \
	-cpu Skylake-Client-v2 \
	-rtc clock=host,base=utc \
	-smp sockets=1,cores=4,threads=2 \
	-m 6G \
\
	-drive if=pflash,format=raw,readonly=on,file=$(BUILD)/OVMF_CODE.4m.fd \
	-drive if=pflash,format=raw,file=$(BUILD)/OVMF_VARS.4m.fd \
	-smbios type=0,vendor="American Megatrends Inc.",version="1.0.0",date="05/01/2023" \
	-smbios type=1,manufacturer="Dell Inc.",product="XPS 8940",version="1.0",serial="ABC123456" \
	-smbios type=2,manufacturer="Dell Inc.",product="0XG7RK",version="",serial="",asset="12345" \
\
	-netdev user,id=net0 \
	-device e1000e,netdev=net0 \
\
	-device ich9-usb-ehci1,id=ehci \
	-device ich9-usb-uhci1,masterbus=ehci.0,firstport=0 \
	-device ich9-usb-uhci2,masterbus=ehci.0,firstport=2 \
	-device ich9-usb-uhci3,masterbus=ehci.0,firstport=4 \
	-device usb-kbd,bus=ehci.0 \
	-device usb-mouse,bus=ehci.0 \
\
	-device ich9-intel-hda,id=HDA \
	-audiodev pipewire,id=audio0 \
	-device hda-micro,audiodev=audio0 \
\
	-device ahci,id=ahci0 \
	-device ide-cd,drive=boot,bus=ahci0.0 \
	-drive if=none,id=boot,media=cdrom,readonly=on,format=raw,file=$(BUILD)/kernel.iso \
	-device ide-hd,drive=drive,bus=ahci0.1,serial="S64ANF0R123456P",model="Samsung SSD 980" \
	-drive if=none,id=drive,format=raw,file=$(BUILD)/drive.disk \
\
	-vga none \
	-display none \
	-nographic \
\

cdeps = $(patsubst src/%.c,$(BUILD)/%.c.o,$(shell find src/$(1) -type f -path '*.c'))
asmdeps = $(patsubst src/%.asm,$(BUILD)/%.asm.o,$(shell find src/$(1) -type f -path '*.asm'))
deps = $(call cdeps,$(1)) $(call asmdeps,$(1))
exists = $(shell [ -$(1) $(2) ] && echo true || echo false)

$(BUILD)/%.c.o: $(patsubst $(BUILD)/%.c.o,src/%.c,$(BUILD)/%.c.o)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c '$<' -o '$@'

$(BUILD)/%.asm.o: $(patsubst $(BUILD)/%.asm.o,src/%.asm,$(BUILD)/%.asm.o)
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) '$<' -o '$@'

stdlib: $(call deps,stdlib)
	ar rcs $(BUILD)/libstdlib.a $^

kernel: $(call deps,kernel) | stdlib
	$(CC) $(LDFLAGS) $^ -o $(BUILD)/kernel.bin -lstdlib -T linker.ld

boot: kernel src/boot/efi.c
	@mkdir -p $(BUILD)/boot
	$(CC) $(CFLAGS) -c src/boot/efi.c -o $(BUILD)/boot/efi.o -fpic -I /usr/include/efi -D DEFI_FUNCTION_WRAPPER -D ELF_PATH=L\"kernel.bin\" -D ELF_SIZE=$(shell du -b $(BUILD)/kernel.bin | sed -E 's/([0-9]+)\s+$(BUILD)\/kernel.bin/\1/')
	ld $(LDFLAGS) $(BUILD)/boot/efi.o /usr/lib/crt0-efi-x86_64.o -o $(BUILD)/boot/efi.so -m elf_x86_64 -shared -Bsymbolic -T /usr/lib/elf_x86_64_efi.lds -L /usr/lib -lgnuefi -lefi
	objcopy --output-target efi-app-x86_64 --subsystem=10 $(BUILD)/boot/efi.so $(BUILD)/BOOTX64.EFI -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym -j .rel -j .rela -j '.rel.*' -j '.rela.*' -j .reloc

iso: kernel boot
	@mkdir -p $(BUILD)/iso
ifeq ($(call exists,f,$(BUILD)/iso/kernel.disk),false)
	dd if=/dev/zero of=$(BUILD)/iso/kernel.disk bs=1K count=1440 2> /dev/null
	mformat -i $(BUILD)/iso/kernel.disk -f 1440 '::'
	mmd -i $(BUILD)/iso/kernel.disk '::/EFI'
	mmd -i $(BUILD)/iso/kernel.disk '::/EFI/BOOT'
endif
	mcopy -o -i $(BUILD)/iso/kernel.disk $(BUILD)/kernel.bin '::/'
	mcopy -o -i $(BUILD)/iso/kernel.disk $(BUILD)/BOOTX64.EFI '::/EFI/BOOT'
	xorriso -as mkisofs -R -f -e kernel.disk -no-emul-boot -o $(BUILD)/kernel.iso $(BUILD)/iso 2> /dev/null

bios:
	@mkdir -p $(BUILD)
ifeq ($(call exists,f,$(BUILD)/OVMF_CODE.4m.fd),false)
	cp /usr/share/edk2/x64/OVMF_CODE.4m.fd $(BUILD)
endif
ifeq ($(call exists,f,$(BUILD)/OVMF_VARS.4m.fd),false)
	cp /usr/share/edk2/x64/OVMF_VARS.4m.fd $(BUILD)
endif

disk:
	@mkdir -p $(BUILD)
ifeq ($(call exists,f,$(BUILD)/drive.disk),false)
	dd if=/dev/zero of=$(BUILD)/drive.disk bs=512 count=20480 2> /dev/null
endif

run: kernel boot bios
	qemu-system-x86_64 $(QEMUFLAGS)

debug: kernel boot bios
	qemu-system-x86_64 $(QEMUFLAGS) -s -S

all: stdlib kernel boot iso bios disk

clean:
	rm -rf $(BUILD)

.SILENT:
