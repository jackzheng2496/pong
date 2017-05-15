CC = gcc
BLD = build
SRC = src
BIN = bin
INC = include

all: create_dirs $(BLD)/game $(BIN)/pong

create_dirs:
	mkdir -p $(BIN)
	mkdir -p $(BLD)

$(BLD)/game:
	$(CC) -c $(INC)/game.c -o $@

$(BIN)/pong:
	$(CC) -o $@ $(SRC)/main.c $(BLD)/game -I$(INC) -lncurses -lpthread

clean:
	rm -rf ./bin ./build

