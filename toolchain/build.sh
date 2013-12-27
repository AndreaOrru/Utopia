#!/bin/sh

BINUTILS_VER=2.24
BINUTILS_URL=http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.bz2

GCC_VER=4.8.2
GCC_URL=http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2

NEWLIB_VER=2.1.0
NEWLIB_URL=ftp://sourceware.org/pub/newlib/newlib-$NEWLIB_VER.tar.gz

export TARGET=i586-pc-utopia
export PREFIX=/usr/local/utopia
export PATH="$PREFIX/bin:$PATH"

mkdir packages
cd packages



wget -c $BINUTILS_URL
tar xjf binutils-$BINUTILS_VER.tar.bz2

pushd binutils-$BINUTILS_VER
    patch -p1 < ../../patches/binutils-$BINUTILS_VER.patch
popd

mkdir build-binutils
pushd build-binutils
    ../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls
    make -j4
    sudo make install
popd



wget -c $GCC_URL
tar xjf gcc-$GCC_VER.tar.bz2

pushd gcc-$GCC_VER
    patch -p1 < ../../patches/gcc-$GCC_VER.patch
popd

mkdir build-gcc
pushd build-gcc
    ../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls \
        --enable-languages=c --disable-libssp --with-newlib
    make all-gcc -j4
    make all-target-libgcc -j4
    sudo make install-gcc
    sudo make install-target-libgcc
popd



wget -c $NEWLIB_URL
tar xzf newlib-$NEWLIB_VER.tar.gz

pushd newlib-$NEWLIB_VER
    patch -p1 < ../../patches/newlib-$NEWLIB_VER.patch
    cp -a ../../newlib-utopia newlib/libc/sys/utopia
    pushd newlib/libc/sys
        autoconf
        pushd utopia
            AUTOMAKE=automake-1.12 ACLOCAL=aclocal-1.12 autoreconf
        popd
    popd
popd

mkdir build-newlib
pushd build-newlib
    ../newlib-$NEWLIB_VER/configure --target=$TARGET --prefix="$PREFIX"
    make -j4
    sudo make install
popd
