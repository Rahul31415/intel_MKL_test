#include <stdio.h>
#include <mkl.h>

int main() {
  // 定义矩阵
  double A[4] = {1.0, 2.0, 3.0, 4.0};
  double B[4] = {5.0, 6.0, 7.0, 8.0};
  double C[4];

  // 使用mkl_blas_dgemm计算矩阵乘法
  mkl_blas_dgemm(100, 100, 100, 1.0, A, 100, B, 100, 0.0, C, 100);

  // 输出结果
  for (int i = 0; i < 4; i++) {
    printf("%f ", C[i]);
  }
  printf("\n");

  return 0;
}

