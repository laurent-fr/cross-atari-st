APP=png2sprite

CC=gcc
CFLAGS=-O2 -g
LIBS=-lpng

OBJS=png2sprite.o 

$(APP): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

png2sprite.o: png2sprite.c
	$(CC) -c $< $(CFLAGS)

install: $(APP)
	cp $(APP) /usr/local/bin/

clean:
	rm -f $(APP)
	rm -f *.o
