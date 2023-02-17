CC	= gcc

CFLAGS	= -Wall -w -lwiringPi -lpthread -lmodbus -ljson-c -lsoem -lzmq
CFLAGS  += -I. -I/src/ -L/src/ -I/lib/ -L/lib 
CFLAGS  += -I./lib/soem -L./lib/soem

OBJ-APP = bin/app

HEADER  = src/app_threading.c src/app_timing_control.c 
HEADER += lib/libLogHandler.c lib/libJsonConfig.c lib/libSoemBeckhoff.c lib/libZeroMqComm.c

SOURCE  = src/app.c

app: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ-APP) $(SOURCE) $(HEADER) $(CFLAGS)