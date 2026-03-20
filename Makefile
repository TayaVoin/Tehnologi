CC = gcc

# Путь к libomp
OMP_PREFIX = /opt/homebrew/Cellar/libomp/22.1.1
CFLAGS = -Wall -O2 -Xpreprocessor -fopenmp -I$(OMP_PREFIX)/include -std=c99
LDFLAGS = -L$(OMP_PREFIX)/lib -lomp

# Путь к OpenBLAS
OPENBLAS_HOME = /Users/taya/openblas_perf_test/OpenBLAS/install
INCLUDES = -I$(OPENBLAS_HOME)/include
LIBS = -L$(OPENBLAS_HOME)/lib -lopenblas -lm

# Файлы
TARGET = perf_test
SOURCES = syrc_implementation.c performance_test.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	export DYLD_LIBRARY_PATH=$(OPENBLAS_HOME)/lib:$$DYLD_LIBRARY_PATH && \
	./$(TARGET)

.PHONY: all clean run