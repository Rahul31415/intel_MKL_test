#include <stdio.h>
#include <mkl.h>

int main() {
    // 定义两个复数矩阵 A 和 B
    MKL_Complex16 A[4] = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {7.0, 8.0}};
    MKL_Complex16 B[4] = {{-1.0, -1.0}, {-2.0, -2.0}, {-3.0, -3.0}, {-4.0, -4.0}};
    MKL_Complex16 C[4]; // 结果矩阵

    // 矩阵的行列数
    int m = 2, n = 2, k = 2;
    // 复数单位
    MKL_Complex16 alpha = {1.0, 0.0};
    MKL_Complex16 beta = {0.0, 0.0};

    // 调用 cblas_zgemm 进行矩阵乘法
    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, &alpha, A, k, B, n, &beta, C, n);

    // 打印结果矩阵
    printf("矩阵 C 的结果为:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("(%f, %f) ", C[i*n + j].real, C[i*n + j].imag);
        }
        printf("\n");
    }

    return 0;
}

