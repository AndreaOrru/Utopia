#!/bin/sh

if [ "$1" = "-g" ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.elf &
    sleep 0.5
    i686-elf-gdb
    killall qemu-system-i386
else
    qemu-system-i386 -kernel kernel/kernel.elf
fi
