# # gcc -Wall -o bin/tmProgram src/tmProgram.c -lwiringPi

CC	= gcc
CFLAGS	= -Wall -lwiringPi -I. -lpthread
OBJ	= bin/app
HEADER	= lib/libLogHandler.c lib/libThreadControl.c
SOURCE  = src/tmProgram.c
app: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ) $(SOURCE) $(HEADER) $(CFLAGS)