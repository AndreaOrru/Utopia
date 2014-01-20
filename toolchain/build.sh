#!/bin/sh

if [ "$(uname)" == "Darwin" ]; then
    export CC=/usr/local/bin/gcc-4.8
    export CXX=/usr/local/bin/g++-4.8
    export CPP=/usr/local/bin/cpp-4.8
    export LD=/usr/local/bin/gcc-4.8

    BINUTILS_CFLAGS="-Wno-error=deprecated-declarations"
    NEWLIB_AUTOMAKE="automake112"
    NEWLIB_ACLOCAL="aclocal112"
else
    NEWLIB_AUTOMAKE="automake-1.12"
    NEWLIB_ACLOCAL="aclocal-1.12"
fi

BINUTILS_VER=2.24
GMP_VER=5.1.3
MPFR_VER=3.1.2
MPC_VER=1.0.2
ICONV_VER=1.14
GCC_VER=4.8.2
NEWLIB_VER=2.1.0

BINUTILS_URL=http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.bz2
GMP_URL=https://gmplib.org/download/gmp/gmp-$GMP_VER.tar.bz2
MPFR_URL=http://www.mpfr.org/mpfr-current/mpfr-$MPFR_VER.tar.bz2
MPC_URL=http://multiprecision.org/mpc/download/mpc-$MPC_VER.tar.gz
ICONV_URL=http://ftp.gnu.org/gnu/libiconv/libiconv-$ICONV_VER.tar.gz
GCC_URL=http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2
NEWLIB_URL=ftp://sourceware.org/pub/newlib/newlib-$NEWLIB_VER.tar.gz

export TARGET=i586-pc-utopia
export PREFIX=/usr/local/utopia
export PATH="$PREFIX/bin:$PATH"

mkdir packages
cd packages


if [ "$1" != "-n" ]; then
    wget -c $BINUTILS_URL
    tar xjf binutils-$BINUTILS_VER.tar.bz2

    pushd binutils-$BINUTILS_VER
        patch -p1 < ../../patches/binutils-$BINUTILS_VER.patch
    popd

    mkdir build-binutils
    pushd build-binutils
        CFLAGS="$BINUTILS_CFLAGS" ../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls
        make -j4
        sudo make install
    popd

    if [ "$1" = "-b" ]; then
        exit 0
    fi



    wget -c $GMP_URL
    wget -c $MPFR_URL
    wget -c $MPC_URL
    wget -c $ICONV_URL
    wget -c $GCC_URL

    tar xjf gmp-$GMP_VER.tar.bz2
    tar xjf mpfr-$MPFR_VER.tar.bz2
    tar xzf mpc-$MPC_VER.tar.gz
    tar xzf libiconv-$ICONV_VER.tar.gz
    tar xjf gcc-$GCC_VER.tar.bz2

    pushd gcc-$GCC_VER
        mv ../gmp-$GMP_VER gmp
        mv ../mpfr-$MPFR_VER mpfr
        mv ../mpc-$MPC_VER mpc
        mv ../libiconv-$ICONV_VER libiconv
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

    if [ "$1" = "-g" ]; then
        exit 0
    fi
fi



wget -c $NEWLIB_URL
tar xzf newlib-$NEWLIB_VER.tar.gz

pushd newlib-$NEWLIB_VER
    patch -p1 < ../../patches/newlib-$NEWLIB_VER.patch
    cp -a ../../newlib-utopia newlib/libc/sys/utopia
    pushd newlib/libc/sys
        autoconf
        pushd utopia
            AUTOMAKE="$NEWLIB_AUTOMAKE" ACLOCAL="$NEWLIB_ACLOCAL" autoreconf
        popd
    popd
popd

mkdir build-newlib
pushd build-newlib
    ../newlib-$NEWLIB_VER/configure --target=$TARGET --prefix="$PREFIX"
    make -j4
    sudo make install
popd
