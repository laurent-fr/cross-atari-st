APP=hello.tos

CC=m68k-atari-mint-gcc
LIBCMINI=$(HOME)/cross-atari-st/libcmini
INCLUDE=-I$(LIBCMINI)/include
CFLAGS=-fomit-frame-pointer -Os $(INCLUDE)
LIBS= -L$(LIBCMINI) -lcmini -lgcc -lcmini

OBJS=hello.o

$(APP): $(OBJS)
	m68k-atari-mint-gcc -nostdlib $(LIBCMINI)/crt0.o $(OBJS) -o $@ -s $(LIBS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(OBJS)
	rm -f $(APP)
