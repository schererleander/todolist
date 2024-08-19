CC = gcc
CFLAGS = -Wall -g
LIBS = -lncurses
SRC = src/main.c
BUILD_DIR = build
TARGET = $(BUILD_DIR)/todolist

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -rf $(BUILD_DIR)
