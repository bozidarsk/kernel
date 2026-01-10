#!/bin/sh

set -e

CFLAGS='-ffreestanding -fno-pie -fno-stack-protector -finline -msse4.2 -mno-avx -mno-red-zone -nostdlib -I include -Wall -Wextra -std=c23 -c -O1'
ASMFLAGS='-f elf64'
LDFLAGS='-T linker.ld -static -no-pie -nostdlib'


if [[ ! -d bin/iso ]]; then
	mkdir -p bin/iso
fi

if [[ ! -d bin/efi ]]; then
	mkdir -p bin/efi
fi

if [[ ! -f bin/efi/crt0-efi-x86_64.o ]]; then
	cp /usr/lib/crt0-efi-x86_64.o bin/efi
fi


for x in $(find src -path 'src/kernel/*.c' -type f); do
	x86_64-linux-gnu-gcc $CFLAGS "$x" -o bin/$(basename "$x").o
done

for x in $(find src -path '*.asm' -type f); do
	nasm $ASMFLAGS "$x" -o bin/$(basename "$x").o
done

x86_64-linux-gnu-gcc $LDFLAGS bin/*.o -o bin/kernel.bin


gcc -ffreestanding -fno-stack-protector -fno-stack-check -maccumulate-outgoing-args -fpic -fshort-wchar -mno-red-zone -Wall -Wextra -c -I include -I /usr/include/efi -D ELF_PATH="L\"kernel.bin\"" -D ELF_SIZE=$(du -b bin/kernel.bin | sed -E 's/([0-9]+)\s+bin\/kernel.bin/\1/') -D DEFI_FUNCTION_WRAPPER src/boot/efi.c -o bin/efi/efi.o
ld -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib64 bin/efi/*.o -lgnuefi -lefi -o bin/efi/efi.so
objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym -j .rel -j .rela -j '.rel.*' -j '.rela.*' -j .reloc --target efi-app-x86_64 --subsystem=10 bin/efi/efi.so bin/efi/BOOTX64.EFI

if [[ ! -f bin/kernel.disk ]]; then
	dd if=/dev/zero of=bin/kernel.disk bs=1K count=1440 2> /dev/null
	mformat -i bin/kernel.disk -f 1440 '::'
	mmd -i bin/kernel.disk '::/EFI'
	mmd -i bin/kernel.disk '::/EFI/BOOT'
fi

mcopy -o -i bin/kernel.disk bin/kernel.bin '::/'
mcopy -o -i bin/kernel.disk bin/efi/BOOTX64.EFI '::/EFI/BOOT'
cp bin/kernel.disk bin/iso
xorriso -as mkisofs -R -f -e kernel.disk -no-emul-boot -o bin/kernel.iso bin/iso


if [[ ! -f bin/OVMF_CODE.4m.fd || ! -f bin/OVMF_VARS.4m.fd ]]; then
	cp /usr/share/edk2/x64/OVMF_CODE.4m.fd bin
	cp /usr/share/edk2/x64/OVMF_VARS.4m.fd bin
fi

if [[ ! -f bin/drive.disk ]]; then
	dd if=/dev/zero of=bin/drive.disk bs=512 count=20480 2> /dev/null
fi
