CC = gcc
CFLAGS = -Wall

all: programa

programa: main.o matrices.o vectores.o
	$(CC) $(CFLAGS) -o programa main.o matrices.o vectores.o

main.o: main.c matrices.h vectores.h
	$(CC) $(CFLAGS) -c main.c

matrices.o: matrices.c matrices.h
	$(CC) $(CFLAGS) -c matrices.c

vectores.o: vectores.c vectores.h
	$(CC) $(CFLAGS) -c vectores.c

clean:
	rm -f *.o programa
