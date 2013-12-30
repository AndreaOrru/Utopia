#!/bin/sh

SERVERS="servers/test/test.elf,servers/test2/test2.elf"

if [ "$1" = "-g" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.elf -initrd $SERVERS &
    sleep 0.5
    gdb
elif [ "$1" = "-w" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.elf -initrd $SERVERS &
else
    qemu-system-i386 -kernel kernel/kernel.elf -initrd $SERVERS
fi
