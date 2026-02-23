#include <stdio.h>
#include <math.h>

// 1. Неправильный порядок аргументов в scopy
void cblas_scopy_wrong(const int n, const float *y, const int incy, 
                        float *x, const int incx) {
    for (int i = 0; i < n; i++) {
        x[i * incx] = y[i * incy];
    }
}

// 2. Неправильная формула в saxpy (изменяем x вместо y)
void cblas_saxpy_wrong(const int n, const float alpha, const float *x,
                        const int incx, float *y, const int incy) {
    for (int i = 0; i < n; i++) {
        ((float*)x)[i * incx] = alpha * y[i * incy] + x[i * incx];
    }
}

// 3. Неправильная норма (сумма вместо sqrt(суммы квадратов))
float cblas_snrm2_wrong(const int n, const float *x, const int incx) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += fabs(x[i * incx]);
    }
    return sum;
}

// 4. Неправильный индекс в isamax (последний, а не первый)
int cblas_isamax_wrong(const int n, const float *x, const int incx) {
    int idx = 0;
    float max_val = fabs(x[0]);
    for (int i = 1; i < n; i++) {
        float val = fabs(x[i * incx]);
        if (val >= max_val) {
            max_val = val;
            idx = i;
        }
    }
    return idx;
}