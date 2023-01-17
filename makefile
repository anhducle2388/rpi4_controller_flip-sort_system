# gcc -Wall -o bin/tmProgram src/tmProgram.c -lwiringPi

bin/tmProgam: src/tmProgram.c
	gcc -Wall -o bin/tmProgram src/tmProgram.c -lwiringPi