CC=gcc
CFLAGS=-g -Wall -Werror -Wpedantic

all: compile

clean:
	rm compile

compile: parse.c
	gcc $(CFLAGS) -o $@ $^
