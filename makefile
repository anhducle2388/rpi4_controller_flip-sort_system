CC	= gcc
CFLAGS	= -Wall -I. -lwiringPi -lpthread -lmodbus
OBJ	= bin/app
LINUX_OBJ = /tmp/app
HEADER	= lib/libLogHandler.c src/app_threading.c src/app_timing_control.c
SOURCE  = src/tmProgram.c

app: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ) $(SOURCE) $(HEADER) $(CFLAGS)

# limux-app: $(SOURCE) $(HEADER)
# 	$(CC) -o $(LINUX_OBJ) $(SOURCE) $(HEADER) $(CFLAGS)