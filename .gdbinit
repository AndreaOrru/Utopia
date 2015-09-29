set disassembly-flavor intel

file kernel/kernel.elf
target remote localhost:1234

break main
continue

tui enable
