DEBUG	= -g
CC	= gcc -c
LD	= gcc
CFLAGS	= $(DEBUG) -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align \
		  -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security \
		  -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long  \
		  -Wcast-qual -Wno-suggest-attribute=format -Wpedantic -Werror=declaration-after-statement \
		  -Wmissing-declarations
LIBS	= -lpthread -lm
LDFLAGS = $(DEBUG)

OBJS = main.o thread.o utils.o fileinfo.o

all : a.out

a.out  : $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

main.o : main.c thread.h fileinfo.h utils.h
	$(CC) $(CFLAGS) $< -o $@

thread.o : thread.c thread.h fileinfo.h utils.h
	$(CC) $(CFLAGS) $< -o $@

fileinfo.o : fileinfo.c fileinfo.h
	$(CC) $(CFLAGS) $< -o $@

utils.o : utils.c utils.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) a.out