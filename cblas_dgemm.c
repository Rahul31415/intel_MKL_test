#include <stdio.h>
#include <mkl.h>

int main() {
    // 定义两个实数矩阵 A 和 B
    double A[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}; // 2x3 矩阵
    double B[6] = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0}; // 3x2 矩阵
    double C[4]; // 结果矩阵 2x2

    // 矩阵的行列数
    int m = 2, n = 2, k = 3;
    // 实数单位
    double alpha = 1.0;
    double beta = 0.0;

    // 调用 cblas_dgemm 进行矩阵乘法
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);

    // 打印结果矩阵
    printf("矩阵 C 的结果为:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f ", C[i*n + j]);
        }
        printf("\n");
    }

    return 0;
}

