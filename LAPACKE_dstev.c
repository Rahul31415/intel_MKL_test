#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 定义三对角矩阵的尺寸
    MKL_INT n = 4;

    // 定义对称三对角矩阵
    double d[4] = {1.0, 2.0, 3.0, 4.0}; // 对角线元素
    double e[3] = {0.5, 0.5, 0.5};      // 次对角线元素

    // 用于存储特征值
    double w[4];

    // 用于存储特征向量
    double z[16];
    MKL_INT ldz = 4;

    // 调用 LAPACKE_dstev 计算特征值和特征向量
    MKL_INT info = LAPACKE_dstev(LAPACK_ROW_MAJOR, 'V', n, d, e, z, ldz);

    // 检查计算结果
    if (info == 0) {
        printf("特征值:\n");
        for (MKL_INT i = 0; i < n; i++) {
            printf("%f\n", d[i]);
        }

        printf("特征向量:\n");
        for (MKL_INT i = 0; i < n; i++) {
            for (MKL_INT j = 0; j < n; j++) {
                printf("%f ", z[i * n + j]);
            }
            printf("\n");
        }
    } else {
        printf("LAPACKE_dstev 计算失败，info=%d\n", info);
    }

    return 0;
}

