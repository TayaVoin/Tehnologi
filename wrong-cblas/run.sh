#!/bin/bash
echo "Запуск тестов с НЕПРАВИЛЬНОЙ библиотекой"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

export OPENBLAS_HOME=~/openblas_test/OpenBLAS/install

make -f Makefile clean
make -f Makefile

cp ../cblas-level1-tests/level1.c ./test_cblas_level1.c 2>/dev/null || \
echo "ОШИБКА: файл тестов не найден! Сначала переключитесь на ветку openblas-tests и скопируйте level1.c"

gcc -Wall -O2 -I$(OPENBLAS_HOME)/include -c test_cblas_level1.c -o test_wrong.o
gcc -o test_wrong test_wrong.o -L. -lwrongcblas -lm

echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "Запуск тестов:"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test_wrong