CC	= gcc

CFLAGS	= -Wall -w -lwiringPi -lpthread -lmodbus -ljson-c -lsoem -DMG_ENABLE_LINES=1
CFLAGS  += -I. -I/src/ -L/src/ -I/lib/ -L/lib 
CFLAGS  += -I./lib/soem -L./lib/soem -I./lib/mongoose -L./lib/mongoose/

SSL = ?
ifeq "$(SSL)" "MBEDTLS"
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -lmbedtls -lmbedcrypto -lmbedx509
endif

ifeq "$(SSL)" "OPENSSL"
CFLAGS += -DMG_ENABLE_OPENSSL=1 -lssl -lcrypto
endif

OBJ-APP = bin/app
OBJ-DBG = bin/debug

HEADER  = src/app_threading.c src/app_timing_control.c 
HEADER += lib/libLogHandler.c lib/libJsonConfig.c lib/libSoemBeckhoff.c lib/mongoose/mongoose.c

SOURCE  = src/app.c

app: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ-APP) $(SOURCE) $(HEADER) $(CFLAGS)

debug: $(SOURCE) $(HEADER)
	$(CC) -o $(OBJ-DBG) $(SOURCE) $(HEADER) $(CFLAGS)