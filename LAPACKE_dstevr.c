#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 矩阵维数
    const MKL_INT n = 5;
    
    // 对称三对角矩阵
    double d[5] = {4.0, 5.0, 6.0, 7.0, 8.0}; // 对角线元素
    double e[5] = {1.0, 1.0, 1.0, 1.0, 0.0};    // 次对角线元素

    // 特征值和特征向量
    double w[5];    // 特征值
    double z[25];  // 特征向量

    // LAPACK参数
    MKL_INT m;    // 实际找到的特征值数量
    MKL_INT isuppz[10]; // 特征向量的支持
    
    double vl = 0.0, vu = 10.0; // 特征值范围
    MKL_INT il = 1, iu = n;     // 特征值索引范围
    double abstol = 1e-9;       // 绝对误差容忍度
    MKL_INT info;

    // 调用LAPACKE_dstevr求解器
    info = LAPACKE_dstevr(LAPACK_ROW_MAJOR, 'V', 'A', n, d, e, vl, vu, il, iu, abstol, &m, w, z, n, isuppz);

    // 检查info值
    if (info == 0) {
        printf("LAPACKE_dstevr收敛\n");
        printf("特征值的数量 m=%d\n", m);
        for (MKL_INT i = 0; i < m; i++) {
            printf("特征值 %d: %.6f\n", i + 1, w[i]);
        }
    } else {
        printf("LAPACKE_dstevr未收敛，info=%d\n", info);
    }

    return 0;
}

