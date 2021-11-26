CC=gcc
LINK=-largp
FLAGS=-O0 -Wall --pedantic

cachesim:main.o
	${CC} $^ ${FLAGS} ${LINK} -o $@
main.o: main.c
	${CC} $^ ${FLAGS} ${LINK} -c -o $@

