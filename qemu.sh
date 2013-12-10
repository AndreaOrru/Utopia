#!/bin/sh

if [ $1 = '-g' ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.bin &
    sleep 0.5
    gdb
elif [ $1 = '-w' ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.bin &
else
    qemu-system-i386 -kernel kernel/kernel.bin
fi
