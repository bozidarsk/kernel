#!/bin/sh

set -e

CFLAGS='-ffreestanding -fno-stack-protector -maccumulate-outgoing-args -fno-stack-check -fno-pic -fno-pie -finline -masm=intel -mno-red-zone -msse4.2 -mno-avx -nostdinc -nostdlib -I include -Wall -Wextra -std=c23 -c'
ASMFLAGS='-f elf64'
LDFLAGS='-nostdlib -znocombreloc -static -no-pie -L bin'

function compile() 
{
	local src="$1"
	local dest="$2"
	local flags="$3"

	if [[ ! -d "$dest" ]]; then
		mkdir -p "$dest"
	fi

	for x in $(find src -path "$src"'/*.c' -type f); do
		x86_64-linux-gnu-gcc "$x" -o "$dest"/$(basename "$x").o $CFLAGS $flags
	done
}

function assemble() 
{
	local src="$1"
	local dest="$2"
	local flags="$3"

	if [[ ! -d "$dest" ]]; then
		mkdir -p "$dest"
	fi

	for x in $(find src -path "$src"'/*.asm' -type f); do
		nasm "$x" -o "$dest"/$(basename "$x").o $ASMFLAGS $flags
	done
}

function link() 
{
	local src="$1"
	local dest="$2"
	local flags="$3"

	x86_64-linux-gnu-gcc "$src"/*.o -o "$dest" $LDFLAGS $flags
}


# stdlib
compile src/stdlib bin/stdlib
assemble src/stdlib bin/stdlib
ar rcs bin/libstdlib.a bin/stdlib/*.o


# kernel
compile src/kernel bin/kernel
assemble src/kernel bin/kernel
link bin/kernel bin/kernel.bin "-lstdlib -T linker.ld"


# bootloader (efi program)
if [[ ! -d bin/boot ]]; then mkdir -p bin/boot; fi
if [[ ! -f bin/boot/crt0-efi-x86_64.o ]]; then cp /usr/lib/crt0-efi-x86_64.o bin/boot; fi
x86_64-linux-gnu-gcc src/boot/efi.c -o bin/boot/efi.o \
	-ffreestanding -fno-stack-protector -maccumulate-outgoing-args -fno-stack-check -finline -fpic -fshort-wchar \
	-mno-red-zone -msse4.2 -mno-avx \
	-Wall -Wextra -std=c23 -c -I . -I /usr/include/efi -D ELF_PATH=L\"kernel.bin\" -D ELF_SIZE=$(du -b bin/kernel.bin | sed -E 's/([0-9]+)\s+bin\/kernel.bin/\1/') -D DEFI_FUNCTION_WRAPPER
ld bin/boot/*.o -o bin/boot/efi.so -m elf_x86_64 -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -lgnuefi -lefi
objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym -j .rel -j .rela -j '.rel.*' -j '.rela.*' -j .reloc --target efi-app-x86_64 --subsystem=10 bin/boot/efi.so bin/BOOTX64.EFI


# iso
if [[ ! -d bin/iso ]]; then mkdir -p bin/iso; fi
if [[ ! -f bin/iso/kernel.disk ]]; then
	dd if=/dev/zero of=bin/iso/kernel.disk bs=1K count=1440 2> /dev/null
	mformat -i bin/iso/kernel.disk -f 1440 '::'
	mmd -i bin/iso/kernel.disk '::/EFI'
	mmd -i bin/iso/kernel.disk '::/EFI/BOOT'
fi
mcopy -o -i bin/iso/kernel.disk bin/kernel.bin '::/'
mcopy -o -i bin/iso/kernel.disk bin/BOOTX64.EFI '::/EFI/BOOT'
xorriso -as mkisofs -R -f -e kernel.disk -no-emul-boot -o bin/kernel.iso bin/iso


# uefi bios
if [[ ! -f bin/OVMF_CODE.4m.fd || ! -f bin/OVMF_VARS.4m.fd ]]; then
	cp /usr/share/edk2/x64/OVMF_CODE.4m.fd bin
	cp /usr/share/edk2/x64/OVMF_VARS.4m.fd bin
fi


# external disk for reads/writes from kernel
if [[ ! -f bin/drive.disk ]]; then
	dd if=/dev/zero of=bin/drive.disk bs=512 count=20480 2> /dev/null
fi
