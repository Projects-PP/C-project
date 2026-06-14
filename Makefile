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

.PHONY: all clean debug run
