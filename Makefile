
default: build run

run: build
	./bin/match

build: 
	mkdir -p bin
	gcc -Wall -Wextra -g -pedantic-errors match.c -o ./bin/match -lm -lncurses

gdb: build
	gdb ./bin/match

submit: build
	cp lab$(labnum).c lab$(labnum)_zoeymertes_ghorrati.c
