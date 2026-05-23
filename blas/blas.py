def saxpy(alpha, x, y):
    """SAXPY: y = alpha*x + y"""
    return [alpha * xi + yi for xi, yi in zip(x, y)]

def ddot(x, y):
    """Скалярное произведение векторов"""
    return sum(xi * yi for xi, yi in zip(x, y))

def dgemm(A, B):
    """Умножение матриц"""
    n = len(A)
    m = len(B[0])
    p = len(B)
    C = [[0 for _ in range(m)] for _ in range(n)]
    for i in range(n):
        for j in range(m):
            for k in range(p):
                C[i][j] += A[i][k] * B[k][j]
    return C

def benchmark_dgemm(n=50):
    """Тест производительности"""
    import time
    A = [[1.0 for _ in range(n)] for _ in range(n)]
    B = [[2.0 for _ in range(n)] for _ in range(n)]
    start = time.time()
    C = dgemm(A, B)
    end = time.time()
    return end - start