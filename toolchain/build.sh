#!/bin/sh

if [ "$(uname)" == "Darwin" ]; then
    export CC=/usr/local/bin/gcc-4.9
    export CXX=/usr/local/bin/g++-4.9
    export CPP=/usr/local/bin/cpp-4.9
    export LD=/usr/local/bin/gcc-4.9

    BINUTILS_CFLAGS="-Wno-error=deprecated-declarations -Wno-error=unused-value"
    AUTOCONF_264="autoconf264"
    NEWLIB_AUTOMAKE="automake112"
    NEWLIB_ACLOCAL="aclocal112"
else
    AUTOCONF_264="autoconf-2.64"
    NEWLIB_AUTOMAKE="automake-1.12"
    NEWLIB_ACLOCAL="aclocal-1.12"
fi

BINUTILS_VER=2.24
GMP_VER=5.1.3
MPFR_VER=3.1.2
MPC_VER=1.0.2
ICONV_VER=1.14
GCC_VER=4.9.0
GDB_VER=7.7.1
NEWLIB_VER=2.1.0

BINUTILS_URL=http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.bz2
GMP_URL=https://gmplib.org/download/gmp/gmp-$GMP_VER.tar.bz2
MPFR_URL=http://www.mpfr.org/mpfr-current/mpfr-$MPFR_VER.tar.bz2
MPC_URL=http://multiprecision.org/mpc/download/mpc-$MPC_VER.tar.gz
ICONV_URL=http://ftp.gnu.org/gnu/libiconv/libiconv-$ICONV_VER.tar.gz
GCC_URL=http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2
GDB_URL=http://ftp.gnu.org/gnu/gdb/gdb-$GDB_VER.tar.bz2
NEWLIB_URL=ftp://sourceware.org/pub/newlib/newlib-$NEWLIB_VER.tar.gz

export TARGET=i686-pc-utopia
export PREFIX=/usr/local/utopia
export PATH="$PREFIX/bin:$PATH"

mkdir packages
cd packages


if [ "$1" != "-n" ]; then
    wget -c $BINUTILS_URL
    tar xjf binutils-$BINUTILS_VER.tar.bz2

    pushd binutils-$BINUTILS_VER
        patch -p1 < ../../patches/binutils-$BINUTILS_VER.patch || exit 1
    popd

    mkdir build-binutils
    pushd build-binutils
        CFLAGS="$BINUTILS_CFLAGS" ../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls || exit 1
        make -j4 || exit 1
        sudo make install || exit 1
    popd



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
        patch -p1 < ../../patches/gcc-$GCC_VER.patch || exit 1
        pushd libstdc++-v3
            $AUTOCONF_264 || exit 1
        popd
    popd

    mkdir build-gcc
    pushd build-gcc
        ../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls \
            --enable-languages=c,c++ --disable-libssp --with-newlib || exit 1
        make all-gcc -j4 || exit 1
        make all-target-libgcc -j4 || exit 1
        sudo make install-gcc || exit 1
        sudo make install-target-libgcc || exit 1
    popd



    wget -c $GDB_URL
    tar xjf gdb-$GDB_VER.tar.bz2

    mkdir build-gdb
    pushd build-gdb
       ../gdb-$GDB_VER/configure --target=i686-elf --prefix="$PREFIX" --disable-nls || exit 1
       make -j4 || exit 1
       sudo make install || exit 1
    popd
fi



wget -c $NEWLIB_URL
tar xzf newlib-$NEWLIB_VER.tar.gz

pushd newlib-$NEWLIB_VER
    patch -p1 < ../../patches/newlib-$NEWLIB_VER.patch || exit 1
    cp -a ../../newlib-utopia newlib/libc/sys/utopia
    pushd newlib/libc/sys
        autoconf || exit 1
        pushd utopia
            AUTOMAKE="$NEWLIB_AUTOMAKE" ACLOCAL="$NEWLIB_ACLOCAL" autoreconf || exit 1
        popd
    popd
popd

mkdir build-newlib
pushd build-newlib
    ../newlib-$NEWLIB_VER/configure --target=$TARGET --prefix="$PREFIX" || exit 1
    make -j4 || exit 1
    sudo make install || exit 1
popd

if [ "$1" != "-n" ]; then
    pushd build-gcc
        make || exit 1
        sudo make install || exit 1
    popd
fi
