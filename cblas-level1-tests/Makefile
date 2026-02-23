CC = gcc
CFLAGS = -Wall -O2 -g -std=c99
INCLUDES = -I$(OPENBLAS_HOME)/include
LIBS = -L$(OPENBLAS_HOME)/lib -lopenblas -lm
DEFINES = 

OPENBLAS_HOME ?= /Users/taya/openblas_test/OpenBLAS/install

TARGET = test_cblas_level1
SOURCES = test_cblas_level1.c
OBJECTS = $(SOURCES:.c=.o)

all: CFLAGS += -DOPENBLAS_NUM_THREADS=1
all: $(TARGET)

multithread: CFLAGS += -DOPENBLAS_NUM_THREADS=4
multithread: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

run-multithread: multithread
	./$(TARGET)

coverage: CFLAGS += -fprofile-arcs -ftest-coverage
coverage: clean $(TARGET)
	./$(TARGET)
	gcov $(SOURCES)

.PHONY: all clean run run-multithread coverage