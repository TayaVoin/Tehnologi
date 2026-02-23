#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <complex.h>
#include "cblas.h"

#define MAX_N 10
#define EPSILON 1e-6

typedef struct {
    char test_name[50];
    int passed;
    char message[100];
} TestResult;

TestResult results[100];
int test_count = 0;

int assert_float_eq(float a, float b, float eps) {
    return fabsf(a - b) < eps;
}

int assert_double_eq(double a, double b, double eps) {
    return fabs(a - b) < eps;
}

void record_result(const char* name, int passed, const char* message) {
    strcpy(results[test_count].test_name, name);
    results[test_count].passed = passed;
    strcpy(results[test_count].message, message);
    test_count++;
    printf("%s: %s\n", name, passed ? "PASS" : "FAIL");
    if (!passed) printf("  Message: %s\n", message);
}

// Тест 1: scopy - копирование вектора float
void test_scopy(void) {
    float x[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    float y[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float expected[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    int n = 5;
    
    cblas_scopy(n, x, 1, y, 1);
    
    int passed = 1;
    for (int i = 0; i < n; i++) {
        if (!assert_float_eq(y[i], expected[i], EPSILON)) {
            passed = 0;
            break;
        }
    }
    record_result("scopy basic", passed, "Копирование вектора float");
}

// Тест 2: scopy с ненулевым шагом
void test_scopy_stride(void) {
    float x[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f};
    float y[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float expected[] = {1.0f, 0.0f, 3.0f, 0.0f, 5.0f, 0.0f, 7.0f};
    int n = 4;  // берем каждый второй элемент
    
    cblas_scopy(n, x, 2, y, 2);
    
    int passed = 1;
    for (int i = 0; i < 7; i++) {
        if (!assert_float_eq(y[i], expected[i], EPSILON)) {
            passed = 0;
            break;
        }
    }
    record_result("scopy stride", passed, "Копирование с шагом");
}

// Тест 3: saxpy - y = a*x + y
void test_saxpy(void) {
    float x[] = {1.0f, 2.0f, 3.0f};
    float y[] = {1.0f, 1.0f, 1.0f};
    float expected[] = {3.5f, 6.0f, 8.5f};  // 2.5*x + y
    int n = 3;
    float alpha = 2.5f;
    
    cblas_saxpy(n, alpha, x, 1, y, 1);
    
    int passed = 1;
    for (int i = 0; i < n; i++) {
        if (!assert_float_eq(y[i], expected[i], EPSILON)) {
            passed = 0;
            break;
        }
    }
    record_result("saxpy alpha=2.5", passed, "y = 2.5*x + y");
}

// Тест 4: sdot - скалярное произведение
void test_sdot(void) {
    float x[] = {1.0f, 2.0f, 3.0f};
    float y[] = {4.0f, 5.0f, 6.0f};
    float expected = 1.0f*4.0f + 2.0f*5.0f + 3.0f*6.0f;  // = 32.0
    int n = 3;
    
    float result = cblas_sdot(n, x, 1, y, 1);
    
    int passed = assert_float_eq(result, expected, EPSILON);
    char msg[100];
    sprintf(msg, "Ожидалось: %f, Получено: %f", expected, result);
    record_result("sdot basic", passed, msg);
}

// Тест 5: snrm2 - евклидова норма
void test_snrm2(void) {
    float x[] = {3.0f, 4.0f};
    float expected = 5.0f;  // sqrt(3^2 + 4^2)
    int n = 2;
    
    float result = cblas_snrm2(n, x, 1);
    
    int passed = assert_float_eq(result, expected, EPSILON);
    char msg[100];
    sprintf(msg, "Ожидалось: %f, Получено: %f", expected, result);
    record_result("snrm2 basic", passed, msg);
}

// Тест 6: isamax - индекс максимального элемента
void test_isamax(void) {
    float x[] = {-5.0f, 1.0f, 3.0f, -2.0f, 4.0f};
    int expected = 0;  // индекс первого элемента с макс. модулем (|-5|=5)
    int n = 5;
    
    int result = cblas_isamax(n, x, 1);
    
    int passed = (result == expected);
    char msg[100];
    sprintf(msg, "Ожидалось: %d, Получено: %d", expected, result);
    record_result("isamax basic", passed, msg);
}

// Тест 7: sswap - обмен векторов
void test_sswap(void) {
    float x[] = {1.0f, 2.0f, 3.0f};
    float y[] = {4.0f, 5.0f, 6.0f};
    float expected_x[] = {4.0f, 5.0f, 6.0f};
    float expected_y[] = {1.0f, 2.0f, 3.0f};
    int n = 3;
    
    cblas_sswap(n, x, 1, y, 1);
    
    int passed = 1;
    for (int i = 0; i < n; i++) {
        if (!assert_float_eq(x[i], expected_x[i], EPSILON) ||
            !assert_float_eq(y[i], expected_y[i], EPSILON)) {
            passed = 0;
            break;
        }
    }
    record_result("sswap basic", passed, "Обмен векторов");
}

int main(int argc, char** argv) {
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("CBLAS Level 1 Interface Tests\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    
    // Установка количества потоков
    #ifdef OPENBLAS_NUM_THREADS
    openblas_set_num_threads(OPENBLAS_NUM_THREADS);
    printf("Установлено потоков: %d\n", OPENBLAS_NUM_THREADS);
    #endif
    
    test_scopy();
    test_scopy_stride();
    test_saxpy();
    test_sdot();
    test_snrm2();
    test_isamax();
    test_sswap();
    
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Итоги тестирования\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    int total_passed = 0;
    for (int i = 0; i < test_count; i++) {
        printf("%-25s: %s\n", results[i].test_name, 
               results[i].passed ? "PASS" : "FAIL");
        if (results[i].passed) total_passed++;
    }
    
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Пройдено: %d/%d тестов (%.1f%%)\n", 
           total_passed, test_count, 
           100.0 * total_passed / test_count);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    return (total_passed == test_count) ? 0 : 1;
}