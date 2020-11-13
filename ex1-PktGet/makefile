TARGET=main

INCLUDE_DIR=include
SRC_DIR=src

CC=gcc
CFLAGS= -g -O0 -Wall -I$(INCLUDE_DIR)
LDFLAGS=-lpcap
TARGET_DIR=build

DEPS=$(patsubst $(SRC_DIR)/%.c, $(TARGET_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))

all: $(TARGET_DIR) $(TARGET)

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(TARGET): $(DEPS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ $^ -c $(CFLAGS)

clean:
	rm -rf $(TARGET_DIR)
	rm -rf $(TARGET)