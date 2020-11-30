
OBJS = match.o card.o
CFLAGS = -Wall -Wextra -g -pedantic-errors -std=c11
CC = gcc
LIBS = -lm -lncurses
INCLUDES= -I "${PWD}"

default: build run

run: build
	./bin/match

build: ${OBJS}
	mkdir -p bin
	${CC} ${CFLAGS} ${INCLUDES} -o ./bin/match ${OBJS} ${LIBS}

gdb: build
	gdb ./bin/match

clean:
	-rm -rf bin/
	-rm -f *.o core *.core

.c.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<
