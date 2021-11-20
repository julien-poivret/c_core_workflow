CC = gcc
FLAGS = -std=c2x -Wall -lpthread -Os 
BIN = out
$(BIN):main.c
	$(CC) main.c $(FLAGS) -o $(BIN) && strip $(BIN) && ./$(BIN)

.PHONY:clean
clean:
	$(RM) ./out
	@echo "Clean done."
