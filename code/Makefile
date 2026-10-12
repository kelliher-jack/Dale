# Makefile for the Dale interpreter (64-bit port)

CC = gcc
CFLAGS = -Wall -Wno-extra

# Source files for the interpreter (test.c is excluded — it's a standalone test)
SRCS = ctype.c dale.c dale1.c dale2.c dale3.c dale4.c \
       daleprm.c dump.c eval.c expr.c hash.c lex.c \
       lists.c make.c objprim.c preproc.c symtbl.c xtdio.c

OBJS = $(SRCS:.c=.o)

dale: $(OBJS)
	$(CC) $(CFLAGS) -o dale $(OBJS)

clean:
	rm -f $(OBJS) dale

# test.c is a standalone unit test for lists.h — build separately
test_lists: test.c
	$(CC) $(CFLAGS) -o test_lists test.c
