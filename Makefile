# This Makefile is a part of a thread library project at NCSU
# Members: Jitesh Shah, Salil Kanitkar, Aditya Jalgaonkar

SRCS = mythread_create.c mythread_exit.c
OBJS = $(SRCS:.c=.o)
TEST_SRCS = mythread_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

CFLAGS = -Wall -Werror -I.
EXTRA_CFLAGS = -L.
LIB = libmythread.a

AR = /usr/bin/ar
CC = gcc

.PHONY: all clean tags test
all: $(LIB)

libmythread.a: $(OBJS)
	$(AR) rcs $(LIB) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(LIB) *~ mythread_test

tags:
	find . -name "*.[cChH]" | xargs ctags
	find . -name "*.[cChH]" | etags -

test:	$(TEST_OBJS) $(LIB)
	$(CC) -o mythread_test $(CFLAGS) $(EXTRA_CFLAGS) -l mythread $(TEST_OBJS)
