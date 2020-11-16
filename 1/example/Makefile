DEBUG	= -g
CC	= gcc -c
LD	= gcc
CFLAGS	= $(DEBUG) -W -Wall
LIBS	=
LDFLAGS = $(DEBUG)

OBJS = process.o integral.o

all : process

process  : $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

process.o : process.c integral.h
	$(CC) $(CFLAGS) $< -o $@

integral.o : integral.c integral.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) process
