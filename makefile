
CC = /home/arunkumar/SHMEM/bin/oshCC

RUNNER = /home/arunkumar/SHMEM/bin/oshrun

SOURCE ?= pi_collective.cpp

EXE = ./launch.out

N ?= 100

code:
	$(CC) $(SOURCE) -o $(EXE)

default: code

all: code

run: code
	 GASNET_BACKTRACE=1 $(RUNNER) -np 8 $(EXE) $(N)