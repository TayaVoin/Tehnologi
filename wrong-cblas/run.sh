#!/bin/bash
echo "Запуск тестов с НЕПРАВИЛЬНОЙ библиотекой"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

make -f Makefile

gcc -Wall -O2 -I$(OPENBLAS_HOME)/include -c test_cblas_level1.c -o test_wrong.o
gcc -o test_wrong test_wrong.o -L. -lwrongcblas -lm

LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test_wrong