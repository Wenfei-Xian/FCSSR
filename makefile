SHELL = /bin/sh

OBJS= main.o fuzzy.o perfect.o sequence.format.o 
CXXFLAGS = -O3 -Wall
CC = g++
LIBS = -lm

FCSSR: ${OBJS}
	${CC} ${CXXFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}
clean:
	-rm -rf *.o FCSSR
