#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <time.h>

// Реализация SYRC для одинарной точности (float)
void syrc_single(int n, float alpha, float* x, float* A, int lda) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            A[i * lda + j] += alpha * x[i] * x[j];
            // Симметричность: A[j][i] = A[i][j]
            if (i != j) {
                A[j * lda + i] = A[i * lda + j];
            }
        }
    }
}

// Реализация SYRC для двойной точности (double)
void syrc_double(int n, double alpha, double* x, double* A, int lda) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            A[i * lda + j] += alpha * x[i] * x[j];
            if (i != j) {
                A[j * lda + i] = A[i * lda + j];
            }
        }
    }
}

// Функция для замера времени (в секундах)
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}