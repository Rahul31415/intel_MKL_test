#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

void print_csr_matrix(const char *name, sparse_index_base_t indexing, MKL_INT rows, MKL_INT cols, MKL_INT *rows_start, MKL_INT *rows_end, MKL_INT *col_indx, double *values) {
    printf("Matrix %s (indexing: %d)\n", name, indexing);
    printf("Rows: %d, Cols: %d\n", rows, cols);

    printf("Row pointers:\n");
    for (MKL_INT i = 0; i <= rows; i++) {
        printf("%d ", rows_start[i]);
    }
    printf("\n");

    printf("Column indices:\n");
    for (MKL_INT i = 0; i < rows_end[rows - 1]; i++) {
        printf("%d ", col_indx[i]);
    }
    printf("\n");

    printf("Values:\n");
    for (MKL_INT i = 0; i < rows_end[rows - 1]; i++) {
        printf("%f ", values[i]);
    }
    printf("\n");
}

int main() {
    // Matrix A (3x3)
    MKL_INT nrows = 3;
    MKL_INT ncols = 3;
    MKL_INT ia[4] = {0, 2, 3, 6}; // Row pointers (0-based)
    MKL_INT ja[6] = {0, 2, 1, 0, 1, 2}; // Column indices (0-based)
    double a[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}; // Values

    // Matrix B (3x3)
    MKL_INT ib[4] = {0, 1, 3, 4}; // Row pointers (0-based)
    MKL_INT jb[4] = {0, 0, 1, 2}; // Column indices (0-based)
    double b[4] = {1.0, 2.0, 3.0, 4.0}; // Values

    sparse_matrix_t csrA, csrB, csrC;
    sparse_status_t status;

    // Create CSR matrices
    status = mkl_sparse_d_create_csr(&csrA, SPARSE_INDEX_BASE_ZERO, nrows, ncols, ia, ia + 1, ja, a);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error creating CSR matrix A\n");
        return 1;
    }

    status = mkl_sparse_d_create_csr(&csrB, SPARSE_INDEX_BASE_ZERO, nrows, ncols, ib, ib + 1, jb, b);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error creating CSR matrix B\n");
        return 1;
    }

    // Perform matrix addition C = A + B
    status = mkl_sparse_d_add(SPARSE_OPERATION_NON_TRANSPOSE, csrA, 1.0, csrB, &csrC);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error adding CSR matrices A and B\n");
        return 1;
    }

    // Export result matrix C
    sparse_index_base_t indexing;
    MKL_INT rows, cols;
    MKL_INT *rows_start = NULL, *rows_end = NULL, *col_indx = NULL;
    double *values = NULL;

    status = mkl_sparse_d_export_csr(csrC, &indexing, &rows, &cols, &rows_start, &rows_end, &col_indx, &values);
    if (status != SPARSE_STATUS_SUCCESS) {
        printf("Error exporting CSR matrix C\n");
        return 1;
    }

    // Print result matrix C
    print_csr_matrix("C", indexing, rows, cols, rows_start, rows_end, col_indx, values);

    // Destroy matrices
    mkl_sparse_destroy(csrA);
    mkl_sparse_destroy(csrB);
    mkl_sparse_destroy(csrC);

    return 0;
}

