#!/bin/sh

SERVERS="servers/test/test,servers/test/test"

if [ "$1" = "-g" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.bin -initrd $SERVERS &
    sleep 0.5
    gdb
elif [ "$1" = "-w" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.bin -initrd $SERVERS &
else
    qemu-system-i386 -kernel kernel/kernel.bin -initrd $SERVERS
fi
