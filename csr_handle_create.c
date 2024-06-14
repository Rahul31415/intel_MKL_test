#include <stdio.h>
#include <mkl.h>

int main() {
    // 示例数据：创建一个 3x3 的 CSR 矩阵
    MKL_INT nrows = 3;
    MKL_INT ncols = 3;
    //MKL_INT ia[4] = {0, 2, 3, 5};   // 行指针数组
    //MKL_INT ja[5] = {0, 2, 1, 0, 2}; // 列索引数组
    MKL_INT ia[4] = {1, 3, 4, 6};   // 行指针数组
    MKL_INT ja[5] = {1, 3, 2, 1, 3}; // 列索引数组
    double a[5] = {1.0, 2.0, 3.0, 4.0, 5.0}; // 非零值数组

    sparse_matrix_t csrA;
    sparse_status_t status;

    // 创建 CSR 矩阵
    status = mkl_sparse_d_create_csr(&csrA, SPARSE_INDEX_BASE_ONE, nrows, ncols, ia, ia + 1, ja, a);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error creating CSR matrix\n");
        return 1;
    }

    // 导出 CSR 矩阵
    sparse_index_base_t indexing;
    MKL_INT rows, cols;
    MKL_INT *rows_start, *rows_end, *col_indx;
    double *values;

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
    for (int i = 0; i < rows_end[rows - 1]; i++) {
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

