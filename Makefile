CC := gcc
C_FLAGS := -Wall -Wextra -O2 -ggdb

C_SRC := pet.c main.c ini.c

all: build

build:
	$(CC) $(C_SRC) $(C_FLAGS) -o pet

.PHONY: clean
clean:
	rm pet
