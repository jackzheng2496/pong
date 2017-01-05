CC = gcc
TARGET = ./bin/pong

all: $(OBJ)
	mkdir -p ./bin
	$(CC) -o $(TARGET) ./src/random.c -lncurses

clean:
	rm -rf ./bin ./build

