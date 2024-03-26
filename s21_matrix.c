#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = 0;
  if (result == NULL || rows <= 0 || columns <= 0) {
    res = 1;
  } else {
    double **mat = calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++) {
      mat[i] = calloc(columns, sizeof(double));
    }
    result->rows = rows;
    result->columns = columns;
    result->matrix = mat;
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (s21_correct_mat(A) == 0 || s21_correct_mat(B) == 0) {
    res = FAILURE;
  } else {
    if (A->rows == B->rows && A->columns == B->columns) {
      int count_suc = 0;
      int n = A->rows, m = A->columns;
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          if (fabs(A->matrix[i][j] - B->matrix[i][j]) < 1e-7) count_suc++;
        }
      }
      if (count_suc != n * m) res = FAILURE;
    } else {
      res = FAILURE;
    }
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || s21_correct_mat(B) == 0 || result == NULL) {
    res = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || s21_correct_mat(B) == 0 || result == NULL) {
    res = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || result == NULL) {
    res = 1;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || s21_correct_mat(B) == 0 || result == NULL) {
    res = 1;
  } else if (A->columns != B->rows) {
    res = 2;
  } else {
    int k = A->columns;
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        double sum = 0;
        for (int x = 0; x < k; x++) {
          sum += A->matrix[i][x] * B->matrix[x][j];
        }
        result->matrix[i][j] = sum;
      }
    }
  }
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || result == NULL) {
    res = 1;
  } else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || result == NULL) {
    res = 1;
  } else if (A->rows != A->columns) {
    res = 2;
  } else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = s21_calc_minor(i, j, A);  // Достаём миноры
      }
    }
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] =
            result->matrix[i][j] * pow(-1, i + j);  // Алгеброич. дополнение
      }
    }
  }
  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = 0;
  *result = 0;
  if (s21_correct_mat(A) == 0) {
    res = 1;
  } else if (A->rows != A->columns) {
    res = 2;
  } else {
    if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else {
      double arr[A->rows];
      for (int j = 0; j < A->columns; j++) {
        arr[j] = s21_calc_minor(0, j, A);
      }
      for (int i = 0; i < A->columns; i++) {
        *result += (arr[i] * A->matrix[0][i] * pow(-1, i + 0));
      }
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (s21_correct_mat(A) == 0 || result == NULL) {
    res = 1;
  } else if (A->rows != A->columns) {
    res = 2;
  } else {
    double deter = 0;
    s21_determinant(A, &deter);
    if (deter == 0) {
      res = 2;
    } else {
      s21_calc_complements(A, result);  // тут создаётся result matrix
      matrix_t tmp = {0};
      s21_transpose(result, &tmp);  // тут создается tmp matrix
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = tmp.matrix[i][j] / deter;
        }
      }
      s21_remove_matrix(&tmp);
    }
  }
  return res;
}

double s21_calc_minor(int a, int b, matrix_t *A) {
  int count = 0;
  double arr[(A->rows - 1) * (A->rows - 1)];
  double res = 0;
  for (int x = 0; x < A->rows; x++) {
    if (x != a) {
      for (int y = 0; y < A->columns; y++) {
        if (y != b) {
          arr[count++] = A->matrix[x][y];
        }
      }
    }
  }
  count = 0;
  matrix_t tmp = {0};
  s21_create_matrix(A->rows - 1, A->rows - 1, &tmp);
  for (int i = 0; i < tmp.rows; i++) {
    for (int j = 0; j < tmp.columns; j++) {
      tmp.matrix[i][j] = arr[count++];
    }
  }
  if (tmp.rows > 2) {
    s21_determinant(&tmp, &res);
  } else {
    if (tmp.rows == 1) {
      res = tmp.matrix[0][0];
    } else {
      double mul1 = s21_minor_for_complements(tmp);
      s21_perevorot(&tmp);
      double mul2 = s21_minor_for_complements(tmp);
      res = mul1 - mul2;
    }
  }
  s21_remove_matrix(&tmp);
  return res;
}

void s21_perevorot(matrix_t *A) {
  matrix_t tmp = {0};
  s21_create_matrix(A->rows, A->columns, &tmp);
  for (int i = 0; i < tmp.rows; i++) {
    for (int j = tmp.columns - 1, t = 0; j >= 0; j--, t++) {
      tmp.matrix[i][j] = A->matrix[i][t];
    }
  }
  for (int i = 0; i < tmp.rows; i++) {
    for (int j = 0; j < tmp.columns; j++) {
      A->matrix[i][j] = tmp.matrix[i][j];
    }
  }
  s21_remove_matrix(&tmp);
}

double s21_minor_for_complements(matrix_t tmp) {
  double res = 1;
  for (int i = 0; i < tmp.rows; i++) {
    for (int j = 0; j < tmp.columns; j++) {
      if (i == j) res *= tmp.matrix[i][j];
    }
  }
  return res;
}

int s21_correct_mat(matrix_t *A) {
  int res = SUCCESS;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    res = FAILURE;
  }
  return res;
}
