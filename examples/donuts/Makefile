APP=donuts.tos

CC=m68k-atari-mint-gcc
LIBCMINI=$(HOME)/cross-atari-st/libcmini
INCLUDE=-I$(LIBCMINI)/include
CFLAGS=-fomit-frame-pointer -funroll-loops -Os $(INCLUDE) 
LIBS=-L$(LIBCMINI) -lcmini -lgcc -lcmini
ASM=vasmm68k_mot
ASMFLAGS=-Faout -quiet -x -m68000 -spaces -showopt

OBJS=video.o gfx.o

$(APP): $(OBJS)
	m68k-atari-mint-gcc -nostdlib $(LIBCMINI)/crt0.o $(OBJS) -o $@ -s $(LIBS)

sprite1.dat:
	png2sprite -i donuts.png -x 64 -y 0 -w 16 -h 16 -s > sprite1.dat

sprite2.dat:
	png2sprite -i donuts.png -x 16 -y 0 -w 16 -h 16 -s > sprite2.dat

sprite3.dat:
	png2sprite -i donuts.png -x 32 -y 0 -w 16 -h 16 -s > sprite3.dat

sprite4.dat:
	png2sprite -i donuts.png -x 48 -y 0 -w 16 -h 16 -s > sprite4.dat

palette.dat:
	png2sprite -i donuts.png -p > palette.dat

video.o: video.c sprite1.dat sprite2.dat sprite3.dat sprite4.dat palette.dat
	$(CC) -c $< $(CFLAGS)

gfx.o: gfx.s
	$(ASM) $(ASMFLAGS) $< -o $@ -L $(basename $@).lst

clean:
	rm -f $(OBJS)
	rm -f $(APP)
	rm -f *.dat
	rm -f *.lst
