#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 矩阵维数
    const MKL_INT n = 4;
    
    // 定义一个4x4的厄米矩阵（上三角部分）
    MKL_Complex16 A[4*4] = {
        {4.0, 0.0}, {1.0, 1.0}, {0.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {5.0, 0.0}, {1.0, -1.0}, {0.0, 0.0},
        {0.0, 0.0}, {0.0, 0.0}, {6.0, 0.0}, {1.0, 2.0},
        {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {7.0, 0.0}
    };

    // 特征值和特征向量
    double w[n];    // 特征值
    MKL_Complex16 z[n*n];  // 特征向量

    // LAPACK参数
    MKL_INT m;    // 实际找到的特征值数量
    MKL_INT isuppz[2*n]; // 特征向量的支持
    
    double vl = 0.0, vu = 10.0; // 特征值范围
    MKL_INT il = 1, iu = n;     // 特征值索引范围
    double abstol = 1e-9;       // 绝对误差容忍度
    MKL_INT info;

    // 调用LAPACKE_zheevr求解器
    info = LAPACKE_zheevr(LAPACK_ROW_MAJOR, 'V', 'A', 'U', n, A, n, vl, vu, il, iu, abstol, &m, w, z, n, isuppz);

    // 检查info值
    if (info == 0) {
        printf("LAPACKE_zheevr收敛\n");
        printf("特征值的数量 m=%d\n", m);
        for (MKL_INT i = 0; i < m; i++) {
            printf("特征值 %d: %.6f\n", i + 1, w[i]);
        }
    } else {
        printf("LAPACKE_zheevr未收敛，info=%d\n", info);
    }

    return 0;
}

