#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 矩阵维数
    const MKL_INT n = 4;
    
    // 定义一个4x4的厄米矩阵（上三角部分）
    MKL_Complex16 A[4*4] = {
        {4.0, 0.0}, {1.0, 1.0}, {0.0, 0.0}, {0.0, 0.0},
        {1.0, -1.0}, {5.0, 0.0}, {1.0, -1.0}, {0.0, 0.0},
        {0.0, 0.0}, {1.0, 1.0}, {6.0, 0.0}, {1.0, 2.0},
        {0.0, 0.0}, {0.0, 0.0}, {1.0, -2.0}, {7.0, 0.0}
    };

    // 特征值
    double w[n];
    
    // LAPACK参数
    MKL_INT info;

    // 调用LAPACKE_zheev求解器
    info = LAPACKE_zheev(LAPACK_ROW_MAJOR, 'V', 'U', n, A, n, w);

    // 检查info值
    if (info == 0) {
        printf("LAPACKE_zheev收敛\n");
        printf("特征值的数量 n=%d\n", n);
        for (MKL_INT i = 0; i < n; i++) {
            printf("特征值 %d: %.6f\n", i + 1, w[i]);
        }
    } else {
        printf("LAPACKE_zheev未收敛，info=%d\n", info);
    }

    return 0;
}

