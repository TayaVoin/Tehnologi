#!/bin/bash
echo "Запуск тестов с НЕПРАВИЛЬНОЙ библиотекой"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

export OPENBLAS_HOME=~/openblas_test/OpenBLAS/install

echo "OPENBLAS_HOME = $OPENBLAS_HOME"
ls -la $OPENBLAS_HOME/include/cblas.h || echo "WARNING: cblas.h not found!"

make -f Makefile clean
make -f Makefile

if [ ! -f test_cblas_level1.c ]; then
    echo "Файл тестов не найден, копируем..."
    cp ../cblas-level1-tests/level1.c test_cblas_level1.c 2>/dev/null
    if [ ! -f test_cblas_level1.c ]; then
        echo "ОШИБКА: не удалось скопировать файл тестов!"
        exit 1
    fi
fi

echo "Компиляция тестов..."
gcc -Wall -O2 -I${OPENBLAS_HOME}/include -c test_cblas_level1.c -o test_wrong.o
if [ $? -ne 0 ]; then
    echo "ОШИБКА: компиляция не удалась!"
    exit 1
fi

gcc -o test_wrong test_wrong.o -L. -lwrongcblas -lm
if [ $? -ne 0 ]; then
    echo "ОШИБКА: линковка не удалась!"
    exit 1
fi

echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "Запуск тестов:"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test_wrong