CC=gcc
FLAGS=-Wall -Wextra --pedantic -O0 -g
LINK:=-lm

all:cachesim

mac:cachesim.o cache.o
	${CC} $^ ${FLAGS} ${LINK} -largp -o cachesim

cachesim:cachesim.o cache.o
	${CC} $^ ${FLAGS} ${LINK} -o  $@

cache.o:cache.c
	$(CC) $^ ${FLAGS} -c -o $@

cachesim.o:cachesim.c
	${CC} $^ ${FLAGS} -c -o $@

clean:
	rm -f cachesim.o cachesim
