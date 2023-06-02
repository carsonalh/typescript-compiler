CC=gcc
CFLAGS=-g -std=c99 -Wall -Werror
SOURCES=token.c parse.c main.c

all: compile

clean:
	rm compile

compile: $(SOURCES:%.c=objects/%.o)
	$(CC) -o $@ $^

objects/%.o: sources/%.c sources/compile.h | objects
	$(CC) $(CFLAGS) -c $< -o $@

objects:
	mkdir -p $@
