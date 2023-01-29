CC	= gcc
CFLAGS	= -Wall -I. -lwiringPi -lpthread -lmodbus -ljson-c
OBJ	= bin/app
HEADER	= lib/libLogHandler.c lib/libJsonConfig.c src/app_threading.c src/app_timing_control.c
SOURCE  = src/tmProgram.c

app: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ) $(SOURCE) $(HEADER) $(CFLAGS)