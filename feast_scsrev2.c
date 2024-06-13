#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 矩阵的尺寸和非零元素数量
    const MKL_INT n = 5;
    const MKL_INT nnz = 13;

    // 定义稀疏矩阵的CSR格式
    MKL_INT rows[6] = {1, 4, 7, 10, 12, 14};
    MKL_INT cols[13] = {1, 2, 5, 1, 3, 5, 2, 3, 4, 3, 5, 1, 1};
    double values[13] = {10, 1, 2, 1, 10, 3, 1, 10, 4, 1, 10, 2, 4};

    // FEAST参数
    MKL_INT fpm[128];
    double Emin = 1.0, Emax = 20.0;
    double epsout;
    MKL_INT loop;
    MKL_INT M0, M;
    double E[5];
    double X[25];
    double res[5];
    MKL_INT info;
    char UPLO = 'F';

    // 初始化FEAST参数
    feastinit(fpm);
    fpm[0] = 1;  // 打开冗长模式

    // 初始化M0为5
    M0 = n;

    // 调用FEAST求解器
    dfeast_scsrev(&UPLO, &n, values, rows, cols, fpm, &epsout, &loop, &Emin, &Emax, &M0, E, X, &M, res, &info);

    // 检查info值
    if (info == 0) {
        printf("FEAST收敛\n");
        printf("特征值的数量 M=%d\n", M);
        for (int i = 0; i < M; i++) {
            printf("特征值 %d: %.16e\n", i + 1, E[i]);
        }
    } else {
        printf("FEAST未收敛，info=%d\n", info);
    }

    return 0;
}

