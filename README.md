# Cross Atari ST

A script to setup the tools you need to code in C & ASM for the Atari ST !

## Install the tools

The script will install in $HOME/cross-atari-st : 
* GCC M68K from https://tho-otto.de/crossmint.php
* VASM from http://sun.hasenbraten.de/vasm/ (Motorola syntax)
* LibcMini from Freemint project https://github.com/freemint/libcmini

```
cd install
./cross-atari-st-install.sh
```

You need to add ~/cross-atari-st/usr/bin to your PATH (.bashrc , .bash_profile , ...)

## Png2sprite

This program does 2 things : 
* convert a sprite in .png format into Atari ST Low display format encoding
* extract the palette values from the .png

The .png must be color indexed, only the 16 first colors will be used. To manipulate an existing .png I found that AseSprite ( https://github.com/aseprite/aseprite ) was simpler to use than Gimp.

To install, first ensure that you have libpng devel packages on your system, then : 

```
cd png2sprite
make
sudo make install
```

To run the program : 

```
$ png2sprite 
png2sprite -i file.png [-x N] [-y N] [-w N] [-h N] [-s] [-p]
         -s : print sprite data
         -p : print palette data

$ png2sprite -i donuts.png -x 16 -y 0 -w 16 -h 16 -s
0x0f800f80, 0x0f800f80,
0x3ff03fe0, 0x3ff03fe0,
0x7fe87fe0, 0x7ff87ff0,
0xf7fcf7f0, 0xfffcfff0,
0xebfcebf8, 0xfffcfff8,
0xf7fef1f8, 0xfffef9f8,
0xeff6e0f0, 0xfffef0f8,
0xeefae0f8, 0xfefef0fc,
0xfcf6f0f0, 0xfcfef0f8,
0xf9def9d8, 0xf9fef9f8,
0x7fbe7fb0, 0xfffefff0,
0x3f4e3f40, 0xfffefff0,
0x97941780, 0xfffc7fe8,
0xca6c0a00, 0xfffc3f90,
0x7fb80000, 0x7ff80040,
0x3ff00000, 0x3ff00000

$ png2sprite -i donuts.png -p
 0x000,
 0x81a,
 0xb1a,
 0xf02,
 0xaa2,
 0x5a9,
 0x04a,
 0x4bc,
 0x15f,
 0xfb5,
 0x079,
 0xf50,
 0x666,
 0xf65,
 0xf71,
 0xff7
```
## Examples

### Hello

A simple Hello World : 

```
cd examples/hello
make
```

will produce hello.tos , you can either transfer the program to a real Atari ST or use an emulator (Hatari for instance).

Once you have installed hatari and you have setup a TOS ROM (You can use EmuTOS https://emutos.sourceforge.io/ ) , you can run the program like this : 

```
hatari hello.tos
```

### Donuts

An array of donuts scrolling on the screen.

Ensure that png2sprite is installed, then : 

```
cd examples/donuts
make
hatari donuts.tos
```
