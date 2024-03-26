#ifndef SRC_S21_MATRIX_H
#define SRC_S21_MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

#define SUCCESS 1
#define FAILURE 0

// Все операции (кроме сравнения матриц) должны возвращать результирующий код:
// - 0 - OK
// - 1 - Ошибка, некорректная матрица
// - 2 - Ошибка вычисления (несовпадающие размеры матриц; матрица, для которой
// нельзя провести вычисления и т.д.)

int s21_create_matrix(int rows, int columns,
                      matrix_t *result);      // создание матрицы
void s21_remove_matrix(matrix_t *A);          // очистка матрицы
int s21_eq_matrix(matrix_t *A, matrix_t *B);  // сравнение матриц
int s21_sum_matrix(matrix_t *A, matrix_t *B,
                   matrix_t *result);  // сложение матриц
int s21_sub_matrix(matrix_t *A, matrix_t *B,
                   matrix_t *result);  // вычитание матриц
int s21_mult_number(matrix_t *A, double number,
                    matrix_t *result);  // умножение матрицы на число
int s21_mult_matrix(matrix_t *A, matrix_t *B,
                    matrix_t *result);  // умножение двух матриц
int s21_transpose(matrix_t *A, matrix_t *result);  //транспортирование матрицы
int s21_calc_complements(
    matrix_t *A,
    matrix_t *result);  //Минор матрицы и матрица алгебраических дополнений
int s21_determinant(matrix_t *A, double *result);  //определитель матрицы
int s21_inverse_matrix(matrix_t *A, matrix_t *result);  //обратная матрица

int s21_correct_mat(matrix_t *A);  //проверить корректность матрицы
double s21_calc_minor(
    int i, int j, matrix_t *A);  //вычисление минора для s21_calc_complements
void s21_perevorot(matrix_t *A);
double s21_minor_for_complements(matrix_t tmp);

#endif  // SRC_S21_MATRIX_H