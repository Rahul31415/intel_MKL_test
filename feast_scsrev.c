#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 矩阵的尺寸和非零元素数量
    const MKL_INT n = 100;
    const MKL_INT nnz = 102;


    // 定义稀疏矩阵的CSR格式
    MKL_INT rows[101];
    rows[0] = 1;
    rows[1] = 3;
    for (int i = 2; i < 100; i++){
	    rows[i] = i + 2;
    }
    rows[100] = 103;
    MKL_INT cols[102];
    cols[0] = 1; 
    cols[1] = 100;
    for (int i = 2; i < 100; i++){
	    cols[i] = i;
    }
    cols[100] = 1;
    cols[101] = 100;

    double values[102];
    values[0] = 1;
    values[1] = 1;

    for (int i = 2; i < 100; i++){
	    values[i] = i;
    }

    values[100] = 1;
    values[101] = 100;

    // FEAST参数
    MKL_INT fpm[128];
    double Emin = -100.0, Emax = 20.0;
    double epsout;
    MKL_INT loop;
    MKL_INT M0 = n, M;
    double E[100];
    double X[10000];
    double res[100];
    MKL_INT info;
    char UPLO = 'F';

    // 初始化FEAST参数
    feastinit(fpm);
    fpm[0] = 1;  // 打开冗长模式

    // 调用FEAST求解器
    dfeast_scsrev(&UPLO, &n, values, rows, cols, fpm, &epsout, &loop, &Emin, &Emax, &M0, E, X, &M, res, &info);

    // 检查info值
    if (info == 0) {
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

