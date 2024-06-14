#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int main() {
    // 示例数据：创建一个 3x3 的 CSR 矩阵，基数为 1
    MKL_INT nrows = 5;
    MKL_INT ncols = 5;
    MKL_INT ia[6] = {0, 3, 5, 8, 11, 13};   // 行指针数组，基数为 1
    MKL_INT iae[6] = {3, 5, 8, 11, 13, };
    MKL_INT ja[13] = {0, 1, 3, 0, 1, 2, 3, 4, 0, 2, 3, 1, 4}; // 列索引数组，基数为 1
    double a[13] = {1, -1, -3, -2, 5, 4, 6, 4, -4, 2, 7, 8, -5}; // 非零值数组

    sparse_matrix_t csrA;
    sparse_status_t status;

    // 创建 CSR 矩阵，基数为 1
    status = mkl_sparse_d_create_csr(&csrA, SPARSE_INDEX_BASE_ZERO, nrows, ncols, ia, ia + 1, ja, a);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error creating CSR matrix\n");
        return 1;
    }

    // 导出 CSR 矩阵
    sparse_index_base_t indexing = SPARSE_INDEX_BASE_ZERO;
    MKL_INT rows, cols;
    MKL_INT *rows_start = NULL, *rows_end = NULL, *col_indx = NULL;
    double *values = NULL;

    status = mkl_sparse_d_export_csr(csrA, &indexing, &rows, &cols, &rows_start, &rows_end, &col_indx, &values);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error exporting CSR matrix\n");
        return 1;
    }

    // 打印导出的 CSR 矩阵数据
    printf("Indexing: %d\n", indexing);
    printf("Rows: %d, Cols: %d\n", rows, cols);

    printf("Row pointers:\n");
    for (int i = 0; i < rows + 1; i++) {
        printf("%d ", rows_start[i]);
    }
    printf("\n");

    printf("Column indices:\n");
    for (int i = 0; i < rows_end[rows - 1]; i++) { // 使用 rows_end 计算实际的非零元素数量
        printf("%d ", col_indx[i]);
    }
    printf("\n");

    printf("Values:\n");
    for (int i = 0; i < rows_end[rows - 1]; i++) {
        printf("%f ", values[i]);
    }
    printf("\n");

    // 释放资源
    mkl_sparse_destroy(csrA);

    return 0;
}


