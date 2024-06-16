#include <mkl.h>
#include <stdio.h>
#include <stdlib.h>

// 手动移除CSR矩阵中的零元素
void remove_zeros_from_csr(const sparse_matrix_t A, sparse_matrix_t *new_A) {
    // 导出CSR格式数据
    sparse_index_base_t indexing = SPARSE_INDEX_BASE_ZERO;
    MKL_INT rows, cols, *rows_start, *rows_end, *col_indx;
    double *values;

    mkl_sparse_d_export_csr(A, &indexing, &rows, &cols, &rows_start, &rows_end, &col_indx, &values);

    // 新的CSR数组
    MKL_INT* new_rows_start = (MKL_INT *)malloc((rows + 1) * sizeof(MKL_INT));
    MKL_INT* new_col_indx = (MKL_INT *)malloc((rows_end[rows - 1] - rows_start[0]) * sizeof(MKL_INT));
    double* new_values = (double *)malloc((rows_end[rows - 1] - rows_start[0]) * sizeof(double));

    MKL_INT nnz = 0; // 非零元素计数

    // 遍历每一行
    for (MKL_INT i = 0; i < rows; i++) {
        new_rows_start[i] = nnz;
        for (MKL_INT j = rows_start[i]; j < rows_end[i]; j++) {
            if (values[j] != 0.0) {
                new_col_indx[nnz] = col_indx[j];
                new_values[nnz] = values[j];
                nnz++;
            }
        }
    }
    new_rows_start[rows] = nnz;

    // 创建新的稀疏矩阵
    mkl_sparse_d_create_csr(new_A, SPARSE_INDEX_BASE_ZERO, rows, cols, new_rows_start, new_rows_start + 1, new_col_indx, new_values);
}

int main() {
    // 示例矩阵数据
    MKL_INT rows = 3, cols = 3;
    MKL_INT rows_start[4] = {0, 3, 6, 9};
    MKL_INT col_indx[9] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
    double values[9] = {1.0, 1.0, 0.0, 4.0, 3.0, 8.0, 4.0, 9.0, 6.0};

    // 创建CSR矩阵A
    sparse_matrix_t A;
    mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, rows, cols, rows_start, rows_start + 1, col_indx, values);

    // 创建一个新的稀疏矩阵B用于存储压缩后的结果
    sparse_matrix_t B;
    remove_zeros_from_csr(A, &B);

    // 导出并打印新的稀疏矩阵B的数据
    sparse_index_base_t indexing = SPARSE_INDEX_BASE_ZERO;
    MKL_INT new_rows, new_cols, *new_rows_start, *new_rows_end, *new_col_indx;
    double *new_values;

    mkl_sparse_d_export_csr(B, &indexing, &new_rows, &new_cols, &new_rows_start, &new_rows_end, &new_col_indx, &new_values);

    printf("Indexing: %d\n", indexing);
    printf("Rows: %d, Cols: %d\n", new_rows,new_cols);
    printf("Row pointers:\n");
    for (int i = 0; i <= new_rows; i++) {
        printf("%d ", new_rows_start[i]);
    }
    printf("\nColumn indices:\n");
    for (int i = 0; i < new_rows_start[new_rows]; i++) {
        printf("%d ", new_col_indx[i]);
    }
    printf("\nValues:\n");
    for (int i = 0; i < new_rows_start[new_rows]; i++) {
        printf("%f ", new_values[i]);
    }
    printf("\n");

    // 释放分配的内存
    free(new_rows_start);
    free(new_col_indx);
    free(new_values);

    // 清理
    mkl_sparse_destroy(A);
    mkl_sparse_destroy(B);
    return 0;
}

