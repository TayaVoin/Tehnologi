#include <stdio.h>
#include <math.h>

void cblas_scopy(const int n, const float *x, const int incx, 
                 float *y, const int incy) {
    // Копируем в обратном направлении
    for (int i = 0; i < n; i++) {
        y[(n-1-i) * incy] = x[i * incx];
    }
}

void cblas_saxpy(const int n, const float alpha, const float *x,
                 const int incx, float *y, const int incy) {
    // Неправильная формула (умножаем y вместо x)
    for (int i = 0; i < n; i++) {
        y[i * incy] = alpha * y[i * incy] + x[i * incx];
    }
}

float cblas_sdot(const int n, const float *x, const int incx,
                 const float *y, const int incy) {
    // Возвращаем сумму модулей, а не скалярное произведение
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += fabsf(x[i * incx]) + fabsf(y[i * incy]);
    }
    return sum;
}

float cblas_snrm2(const int n, const float *x, const int incx) {
    // Возвращаем сумму, а не суммы квадратов
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += fabsf(x[i * incx]);
    }
    return sum;
}

int cblas_isamax(const int n, const float *x, const int incx) {
    // Возвращаем последний, а не первый максимум
    int idx = 0;
    float max_val = fabsf(x[0]);
    for (int i = 1; i < n; i++) {
        float val = fabsf(x[i * incx]);
        if (val >= max_val) {
            max_val = val;
            idx = i;
        }
    }
    return idx;
}

void cblas_sswap(const int n, float *x, const int incx,
                 float *y, const int incy) {
    // Меняем местами неправильно
    for (int i = 0; i < n; i++) {
        float temp = x[i * incx];
        x[i * incx] = y[i * incy] * 2.0f;
        y[i * incy] = temp * 0.5f;
    }
}