#!/bin/sh

if [ $1 ]; then
    qemu-system-i386 -s -S -kernel kernel/kernel.bin
else
    qemu-system-i386 -s -kernel kernel/kernel.bin
fi
