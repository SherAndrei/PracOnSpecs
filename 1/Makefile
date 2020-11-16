DEBUG	= -g
CC	= gcc -c
LD	= gcc
CFLAGS	= $(DEBUG) -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align \
		  -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security \
		  -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long  \
		  -Wcast-qual -Wno-suggest-attribute=format -Wpedantic -Werror=declaration-after-statement \
		  -Wmissing-declarations -O3
LIBS	=
LDFLAGS = $(DEBUG)

OBJS = process.o find_incr.o

all : process

process  : $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

process.o : process.c find_incr.h
	$(CC) $(CFLAGS) $< -o $@

find_incr.o : find_incr.c find_incr.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) process