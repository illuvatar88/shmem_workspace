
CC = /home/arunkumar/SHMEM/bin/oshCC

RUNNER = /home/arunkumar/SHMEM/bin/oshrun

SOURCE ?= hello_world.cpp

EXE = ./launch.out

code:
	$(CC) $(SOURCE) -o $(EXE)

default: code

all: code

run: code
	$(RUNNER) -np 8 $(EXE)