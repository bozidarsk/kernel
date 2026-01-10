#!/bin/sh

QEMUFLAGS=""

if [[ "$@" == *"gdb"* ]]; then
	QEMUFLAGS="$QEMUFLAGS -s -S"
fi

qemu-system-x86_64 $QEMUFLAGS \
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
	-drive if=pflash,format=raw,readonly=on,file=bin/OVMF_CODE.4m.fd \
	-drive if=pflash,format=raw,file=bin/OVMF_VARS.4m.fd \
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
	-drive if=none,id=boot,media=cdrom,readonly=on,format=raw,file=bin/kernel.iso \
	-device ide-hd,drive=drive,bus=ahci0.1,serial="S64ANF0R123456P",model="Samsung SSD 980" \
	-drive if=none,id=drive,format=raw,file=bin/drive.disk \
\
	-vga none \
	-display none \
	-nographic \
\
