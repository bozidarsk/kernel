target remote localhost:1234
set disassembly-flavor intel
lay asm
ni
lay regs | asm
