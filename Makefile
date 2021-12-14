CC=gcc
FLAGS=-Wall -Wextra --pedantic -O0 -g
LINK:=-lm
SRC=cachesim/src/
INC=cachesim/include/
OBJ=obj/
BIN=bin/

all:${BIN}cachesim

${BIN}cachesim:${OBJ}cachesim.o ${OBJ}cache.o
	${CC} $^ ${FLAGS} ${LINK} -largp -o $@

linux:${OBJ}cachesim.o ${OBJ}cache.o
	${CC} $^ ${FLAGS} ${LINK} -o ${BIN}cachesim

${OBJ}cache.o:${SRC}cache.c
	$(CC) $^ ${FLAGS} -I${INC} -c -o $@

${OBJ}cachesim.o:${SRC}cachesim.c
	${CC} $^ ${FLAGS} -I${INC} -c -o $@

clean:
	rm -f ${OBJ}cachesim.o ${OBJ}cache.o ${BIN}cachesim
