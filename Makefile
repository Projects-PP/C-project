CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude $(shell pkg-config --cflags sdl2 sdl2_image sdl2_ttf) #add paths to library declarations
LDFLAGS = $(shell pkg-config --libs sdl2 sdl2_image sdl2_ttf) #add paths to binary libraries 

SRC = src/main.c src/game_logic.c src/file_operations.c src/gui.c
OBJ = $(SRC:.c=.o)
TARGET = chess

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

#compilation of separate files
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f src/*.o $(TARGET)
	rm -rf *.dSYM

debug: CFLAGS += -g -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: clean all

run: all
	./$(TARGET)

test: all
	$(CC) $(CFLAGS) tests/test_logic.c src/game_logic.o src/file_operations.o -o test_logic $(LDFLAGS)
	$(CC) $(CFLAGS) tests/test_gui.c src/gui.o src/game_logic.o src/file_operations.o -o test_gui $(LDFLAGS)
	./test_logic
	./test_gui
	rm -f test_logic test_gui


#makes compilator sure that words bellow are commands, not files
.PHONY: all clean debug run test
