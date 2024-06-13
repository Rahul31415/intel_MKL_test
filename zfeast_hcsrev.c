#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>


#define N 4
#define NNZ 9

int main() {
    // 矩阵维度和非零元素个数
    // const MKL_INT N = 4;
    // const MKL_INT NNZ = 8;

    // CSR 格式的厄米矩阵
    MKL_INT rows_start[N+1] = {1, 4, 6, 7, 9};
    MKL_INT columns[NNZ] = {1, 2, 4, 1, 2, 3, 1, 4};
    MKL_Complex16 values[NNZ] = {
        {1.0, 0.0}, {1.0, 1.0},
        {2.0, 1}, {1.0, -1.0}, {2.0, 0.0},
        {3.0, 0.0}, {2.0, -1.0},
        {4.0, 0.0}
    };

    // FEAST 参数
    MKL_INT fpm[128];
    double epsout;
    MKL_INT loop;
    double emin = -5.0, emax = 10.0;
    MKL_INT M0 = N; // 期望的特征值数
    MKL_INT M, info;
    MKL_INT n = N;
    
    double *E = (double *)malloc(M0 * sizeof(double));
    MKL_Complex16 *X = (MKL_Complex16 *)malloc(N * M0 * sizeof(MKL_Complex16));
    double *res = (double *)malloc(M0 * sizeof(double));

    // 初始化 FEAST 参数
    feastinit(fpm);
    fpm[0] = 1; // 启用输出信息

    // 调用 FEAST 函数求解特征值问题
    zfeast_hcsrev("F", &n, values, rows_start, columns, fpm, &epsout, &loop, &emin, &emax, &M0, E, X, &M, res, &info);

    // 检查计算结果
    if (info == 0) {
        printf("FEAST 收敛于 %d 次迭代\n", loop);
        printf("找到 %d 个特征值\n", M);
        printf("特征值:\n");
        for (MKL_INT i = 0; i < M; i++) {
            printf("%.4f\n", E[i]);
        }

        printf("\n特征向量:\n");
        for (MKL_INT i = 0; i < N; i++) {
            for (MKL_INT j = 0; j < M; j++) {
                printf("(%.4f, %.4f) ", X[i * M + j].real, X[i * M + j].imag);
            }
            printf("\n");
        }
    } else {
        printf("计算失败，info = %d\n", info);
    }

    // 释放内存
    free(E);
    free(X);
    free(res);

    return 0;
}

