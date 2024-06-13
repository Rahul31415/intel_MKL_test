#include <mkl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // 矩阵的尺寸和非零元素数量
    const MKL_INT n = 3;
    const MKL_INT nnz = 7;

    // 定义稀疏矩阵的CSR格式
    MKL_INT rows[4] = {1, 4, 6, 8}; // 行索引
    MKL_INT cols[7] = {1, 2, 3, 1, 2, 1, 3}; // 列索引
    double values[7] = {4.0, 1.0, 2.0, 1.0, 3.0, 2.0, 2.0}; // 非零元素

    // FEAST参数
    MKL_INT fpm[128];
    memset(fpm, 0, sizeof(fpm));
    double Emin = -10.0;
    double Emax = 10.0;
    double epsout = 0;
    MKL_INT loop = 0;
    MKL_INT M0 = n;
    MKL_INT M = 0;
    double E[3] = {0};
    double X[9] = {0};
    double res[3] = {0};
    MKL_INT info = 0;
    char UPLO = 'F';

    // 初始化FEAST参数
    feastinit(fpm);
    fpm[0] = 1;  // 打开冗长模式

    // 调用FEAST求解器
    dfeast_scsrev(&UPLO, &n, values, rows, cols, fpm, &epsout, &loop, &Emin, &Emax, &M0, E, X, &M, res, &info);

    // 检查info值
    if (info == 0) {
	printf("loop: %d \n", loop);
        printf("FEAST收敛\n");
        printf("特征值的数量 M=%d\n", M);
        for (int i = 0; i < M; i++) {
            printf("特征值 %d: %.2f\n", i + 1, E[i]);
        }
    } else {
        printf("FEAST未收敛，info=%d\n", info);
    }

    return 0;
}

