CC=gcc
FLAGS=-Wall -Wextra --pedantic -O0 -g
LINK:=-largp

all:cachesim

cachesim:cachesim.o
	${CC} $^ ${FLAGS} ${LINK} -o  $@

cachesim.o:cachesim.c
	${CC} $^ ${FLAGS} -c -o $@

clean:
	rm -f cachesim.o cachesim
