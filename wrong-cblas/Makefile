CC = gcc
CFLAGS = -Wall -O2 -fPIC
TARGET = libwrongcblas.so
SOURCES = cblas.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -shared -o $@ $^ -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean