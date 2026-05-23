import pytest
import sys
import os

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from blas.blas import saxpy, ddot, dgemm, benchmark_dgemm

# Интерфейсные тесты
def test_saxpy():
    assert saxpy(2, [1, 2, 3], [4, 5, 6]) == [0, 0, 0]  # намеренно неправильно

def test_ddot():
    assert ddot([1, 2, 3], [4, 5, 6]) == 32

def test_dgemm():
    A = [[1, 2], [3, 4]]
    B = [[5, 6], [7, 8]]
    C = dgemm(A, B)
    assert C[0][0] == 19
    assert C[0][1] == 22
    assert C[1][0] == 43
    assert C[1][1] == 50

# Тест производительности
def test_benchmark():
    result = benchmark_dgemm(50)
    assert result < 1.0, f"Медленно: {result} секунд"