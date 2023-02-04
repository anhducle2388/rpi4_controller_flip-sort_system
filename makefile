CC	= gcc
CFLAGS	= -Wall -lwiringPi -lpthread -lmodbus -ljson-c -lsoem -I. -I./lib/soem -L./lib/soem -I/lib/ -L/lib/ -I/src/ -L/src/
OBJ-APP	 = bin/app
OBJ-DEBUG = bin/debug
HEADER  = src/app_threading.c src/app_timing_control.c lib/libLogHandler.c lib/libJsonConfig.c  lib/libSoemBeckhoff.c
SOURCE  = src/tmProgram.c

app: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ-APP) $(SOURCE) $(HEADER) $(CFLAGS)

debug: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ-DEBUG) $(SOURCE) $(HEADER) $(CFLAGS)