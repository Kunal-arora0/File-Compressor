
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude

TARGET = compressor

SOURCES = src/main.c \
          src/huffman.c \
          src/heap.c \
          src/bitio.c \
          src/huff_format.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

