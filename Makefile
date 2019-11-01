# Makefile
# the final executable is called run

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

project: project.o ppmIO.o global.o imageManip.o menuUtil.o
	$(CC) -g -lm -o run project.o ppmIO.o global.o imageManip.o menuUtil.o

# main file
project.o: project.c
	$(CC) $(CFLAGS) -c project.c

# files for reading and writing image
ppmIO.o: ppmIO.c ppmIO.h
	$(CC) $(CFLAGS) -c ppmIO.c

# files for carrying out image manipulation
imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c

# files for helper functions
global.o: global.c global.h
	$(CC) $(CFLAGS) -c global.c

# files for menu functions, printing menu and scanning. 
menuUtil.o: menuUtil.c menuUtil.h
	$(CC) $(CFLAGS) -c menuUtil.c

clean:
	rm -f *.o run

