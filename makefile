# # gcc -Wall -o bin/tmProgram src/tmProgram.c -lwiringPi

CC=gcc
CFLAGS=-Wall -lwiringPi -I.
OBJ= bin/app

app: src/tmProgram.c lib/libLogHandler.c
	$(CC) -o $(OBJ) src/tmProgram.c lib/libLogHandler.c $(CFLAGS)