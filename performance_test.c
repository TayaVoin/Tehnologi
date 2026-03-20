#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "cblas.h"

void syrc_single(int n, float alpha, float* x, float* A, int lda);
void syrc_double(int n, double alpha, double* x, double* A, int lda);

double get_time();

// Структура для хранения результатов
typedef struct {
    int n;
    int threads;
    double my_time;
    double openblas_time;
    double ratio;
} TestResult;

TestResult results[100];
int result_count = 0;

// Запуск теста для float
void run_test_float(int n, int threads, int repeat_count) {
    printf("Testing float, n=%d, threads=%d\n", n, threads);
    
    // Устанавливаем число потоков для OpenBLAS
    openblas_set_num_threads(threads);
    
    float* x = (float*)malloc(n * sizeof(float));
    float* A1 = (float*)malloc(n * n * sizeof(float));
    float* A2 = (float*)malloc(n * n * sizeof(float));
    
    // Инициализация случайными значениями
    for (int i = 0; i < n; i++) {
        x[i] = (float)rand() / RAND_MAX;
        for (int j = 0; j < n; j++) {
            float val = (float)rand() / RAND_MAX;
            A1[i*n + j] = val;
            A2[i*n + j] = val;
        }
    }
    
    float alpha = 2.5f;
    
    // Прогрев (чтобы кэш прогрелся)
    syrc_single(n, alpha, x, A1, n);
    cblas_ssyr(CblasRowMajor, CblasLower, n, alpha, x, 1, A2, n);
    
    double my_total = 0.0;
    double openblas_total = 0.0;
    
    // Основной цикл тестирования
    for (int rep = 0; rep < repeat_count; rep++) {
        // Копируем исходную матрицу
        memcpy(A1, A2, n * n * sizeof(float));
        
        // Замер нашей реализации
        double t1 = get_time();
        syrc_single(n, alpha, x, A1, n);
        double t2 = get_time();
        my_total += (t2 - t1);
        
        // Замер OpenBLAS
        t1 = get_time();
        cblas_ssyr(CblasRowMajor, CblasLower, n, alpha, x, 1, A2, n);
        t2 = get_time();
        openblas_total += (t2 - t1);
    }
    
    double my_avg = my_total / repeat_count;
    double openblas_avg = openblas_total / repeat_count;
    
    results[result_count].n = n;
    results[result_count].threads = threads;
    results[result_count].my_time = my_avg;
    results[result_count].openblas_time = openblas_avg;
    results[result_count].ratio = openblas_avg / my_avg * 100.0;
    
    printf("  My: %.6f sec, OpenBLAS: %.6f sec, Ratio: %.2f%%\n", 
           my_avg, openblas_avg, results[result_count].ratio);
    
    result_count++;
    
    free(x);
    free(A1);
    free(A2);
}

// Запуск теста для double
void run_test_double(int n, int threads, int repeat_count) {
    printf("Testing double, n=%d, threads=%d\n", n, threads);
    
    openblas_set_num_threads(threads);
    
    double* x = (double*)malloc(n * sizeof(double));
    double* A1 = (double*)malloc(n * n * sizeof(double));
    double* A2 = (double*)malloc(n * n * sizeof(double));
    
    for (int i = 0; i < n; i++) {
        x[i] = (double)rand() / RAND_MAX;
        for (int j = 0; j < n; j++) {
            double val = (double)rand() / RAND_MAX;
            A1[i*n + j] = val;
            A2[i*n + j] = val;
        }
    }
    
    double alpha = 2.5;
    
    // Прогрев
    syrc_double(n, alpha, x, A1, n);
    cblas_dsyr(CblasRowMajor, CblasLower, n, alpha, x, 1, A2, n);
    
    double my_total = 0.0;
    double openblas_total = 0.0;
    
    for (int rep = 0; rep < repeat_count; rep++) {
        memcpy(A1, A2, n * n * sizeof(double));
        
        double t1 = get_time();
        syrc_double(n, alpha, x, A1, n);
        double t2 = get_time();
        my_total += (t2 - t1);
        
        t1 = get_time();
        cblas_dsyr(CblasRowMajor, CblasLower, n, alpha, x, 1, A2, n);
        t2 = get_time();
        openblas_total += (t2 - t1);
    }
    
    double my_avg = my_total / repeat_count;
    double openblas_avg = openblas_total / repeat_count;
    
    results[result_count].n = n;
    results[result_count].threads = threads;
    results[result_count].my_time = my_avg;
    results[result_count].openblas_time = openblas_avg;
    results[result_count].ratio = openblas_avg / my_avg * 100.0;
    
    printf("  My: %.6f sec, OpenBLAS: %.6f sec, Ratio: %.2f%%\n", 
           my_avg, openblas_avg, results[result_count].ratio);
    
    result_count++;
    
    free(x);
    free(A1);
    free(A2);
}

// Вывод сводной таблицы
void print_summary() {
    printf("\n\n");
    printf("========================================================\n");
    printf("Сводная таблица тестирования производительности\n");
    printf("========================================================\n");
    printf("%-6s %-8s %-12s %-12s %-10s\n", 
           "n", "потоки", "Мое (сек)", "OpenBLAS (сек)", "% от OpenBLAS");
    printf("--------------------------------------------------------\n");
    
    double geom_mean = 1.0;
    for (int i = 0; i < result_count; i++) {
        printf("%-6d %-8d %-12.6f %-12.6f %-10.2f\n", 
               results[i].n, results[i].threads, 
               results[i].my_time, results[i].openblas_time, 
               results[i].ratio);
        geom_mean *= results[i].ratio;
    }
    
    geom_mean = pow(geom_mean, 1.0 / result_count);
    printf("--------------------------------------------------------\n");
    printf("Среднее геометрическое: %.2f%% от OpenBLAS\n", geom_mean);
    printf("========================================================\n");
}

int main(int argc, char** argv) {
    int repeat_count = 10;
    int threads[] = {1, 2, 4, 8, 16};
    int num_thread_configs = 5;
    
    int n_float = 10000;
    printf("Подбор размера для float...\n");
    while (1) {
        printf("Пробуем n = %d\n", n_float);
        run_test_float(n_float, 1, 1);
        if (results[result_count-1].my_time > 60.0) {
            break;
        }
        n_float += 1000;
    }
    result_count = 0;
    
    int n_double = 10000;
    printf("Подбор размера для double...\n");
    while (1) {
        printf("Пробуем n = %d\n", n_double);
        run_test_double(n_double, 1, 1);
        if (results[result_count-1].my_time > 60.0) {
            break;
        }
        n_double += 1000;
    }
    result_count = 0;
    
    printf("\nПодобранные размеры: float n=%d, double n=%d\n", n_float, n_double);
    
    // Основные тесты для float
    for (int t = 0; t < num_thread_configs; t++) {
        run_test_float(n_float, threads[t], repeat_count);
    }
    
    // Основные тесты для double
    for (int t = 0; t < num_thread_configs; t++) {
        run_test_double(n_double, threads[t], repeat_count);
    }
    
    print_summary();
    
    return 0;
}