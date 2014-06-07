#!/bin/sh

SERVERS="servers/keyboard/keyboard.elf,servers/video/video.elf,servers/test/test.elf"

if [ "$1" = "-g" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.elf -initrd $SERVERS &
    sleep 0.5
    i686-elf-gdb
elif [ "$1" = "-w" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.elf -initrd $SERVERS &
else
    qemu-system-i386 -kernel kernel/kernel.elf -initrd $SERVERS
fi
