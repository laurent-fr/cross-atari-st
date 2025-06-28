#!/bin/bash

DST=~/cross-atari-st
DL=$PWD/dl
SRC=$PWD/src

BINUTILS=binutils-2.44-mint-20250415-bin-linux64.tar.xz
GCC=gcc-14.2.0-mint-20240809-bin-linux64.tar.xz
MINTLIB=mintlib-0.60.1-mint-20240718-dev.tar.xz
LIBCMINI=libcmini
VASM=vasm 

# get GCC
mkdir -p $DL
cd dl
if [ ! -f $BINUTILS ] ; then wget https://tho-otto.de/download/mint/$BINUTILS ; fi
if [ ! -f $GCC ] ; then wget https://tho-otto.de/download/mint/$GCC ; fi
if [ ! -f $MINTLIB] ; then wget https://tho-otto.de/download/mint/$MINTLIB ; fi
cd -

# get LIBCMINI
mkdir -p $SRC
cd src
if [ ! -d $LIBCMINI ] ; then git clone https://github.com/freemint/libcmini.git ; fi

# get VASM
if [ ! -d $VASM ] ; then
	wget http://phoenix.owl.de/tags/vasm.tar.gz
	tar xf vasm.tar.gz
fi

cd -

# install GCC
mkdir -p $DST
cd $DST
tar xf $DL/binutils-2.44-mint-20250415-bin-linux64.tar.xz
tar xf $DL/gcc-14.2.0-mint-20240809-bin-linux64.tar.xz
tar xf $DL/mintlib-0.60.1-mint-20240718-dev.tar.xz
cd - 

export PATH=$PATH:$DST/usr/bin

# install LIBCMINI
cd $SRC/libcmini
sed -i 's/ONLY_68K=N/ONLY_68K=Y/' Makefile
sed -i 's/BUILD_CF=Y/BUILD_CF=N/' Makefile
make
cd -

mkdir -p $DST/libcmini
cp -a $SRC/libcmini/include $DST/libcmini
cp $SRC/libcmini/build/*.a $DST/libcmini
cp $SRC/libcmini/build/objs/crt0.o $DST/libcmini

# install VASM
cd $SRC/vasm
make CPU=m68k SYNTAX=mot
cp vasmm68k_mot $DST/usr/bin/
cp vobjdump $DST/usr/bin/
cd -

echo "Please add $DST/usr/bin to you PATH"
