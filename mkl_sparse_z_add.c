#include <stdio.h>
#include <mkl.h>

// Function to add two CSR matrices
void add_csr_matrices(MKL_INT nrows, MKL_INT ncols,
                      MKL_INT *ia1, MKL_INT *ja1, double *a1,
                      MKL_INT *ia2, MKL_INT *ja2, double *a2,
                      MKL_INT **ia_res, MKL_INT **ja_res, double **a_res) {
    sparse_matrix_t csrA, csrB, csrC;
   // struct matrix_descr descr;
   
   //MKL_INT* ia1e = (MKL_INT*)calloc(nrows, sizeof(MKL_INT));
    //MKL_INT* ia2e = (MKL_INT*)calloc(nrows, sizeof(MKL_INT));
    
    //for(int i = 0; i < nrows - 1; i++){
    //	ia1e[i] = ia1[i + 1];
    //	ia2e[i] = ia2[i + 1];
   // }
    //ia1e[nrows - 1] = ia1e[nrows - 2];
    //ia2e[nrows - 1] = ia2e[nrows - 2];

    // Create sparse matrix handles
    mkl_sparse_d_create_csr(&csrA, SPARSE_INDEX_BASE_ZERO, nrows, ncols, ia1, ia1 + 1, ja1, a1);
    mkl_sparse_d_create_csr(&csrB, SPARSE_INDEX_BASE_ZERO, nrows, ncols, ia2, ia2 + 1, ja2, a2);

    // Set the matrix descriptor to default
    //descr.type = SPARSE_MATRIX_TYPE_GENERAL;

    // Add the two matrices
    mkl_sparse_d_add(SPARSE_OPERATION_NON_TRANSPOSE, csrA, 1.0, csrB, &csrC);

    // Export the result matrix
    sparse_index_base_t indexing = SPARSE_INDEX_BASE_ZERO;
    MKL_INT *rows_start, *rows_end, *columns;
    double *values;
    mkl_sparse_d_export_csr(csrC, &indexing, &nrows, &ncols, &rows_start, &rows_end, &columns, &values);

    // Allocate memory for the result CSR arrays
    MKL_INT nnz = rows_end[nrows - 1] - rows_start[0];
    *ia_res = (MKL_INT *)malloc((nrows + 1) * sizeof(MKL_INT));
    *ja_res = (MKL_INT *)malloc(nnz * sizeof(MKL_INT));
    *a_res = (double *)malloc(nnz * sizeof(double));

    // Copy the result matrix to the output arrays
    for (MKL_INT i = 0; i <= nrows; i++) {
        (*ia_res)[i] = rows_start[i];
    }
    for (MKL_INT i = 0; i < nnz; i++) {
        (*ja_res)[i] = columns[i];
        (*a_res)[i] = values[i];
    }

    // Release the matrix handles
    mkl_sparse_destroy(csrA);
    mkl_sparse_destroy(csrB);
    mkl_sparse_destroy(csrC);
}

int main() {
    // Define two 3x3 CSR matrices
    MKL_INT nrows = 3, ncols = 3;

    // First matrix
    MKL_INT ia1[] = {0, 2, 4, 6};
    MKL_INT ja1[] = {0, 1, 0, 2, 1, 2};
    double a1[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    // Second matrix
    MKL_INT ia2[] = {0, 1, 3, 4};
    MKL_INT ja2[] = {0, 1, 2, 1};
    double a2[] = {7.0, 8.0, 9.0, 10.0};

    // Result matrix
    MKL_INT *ia_res, *ja_res;
    double *a_res;

    // Add the matrices
    add_csr_matrices(nrows, ncols, ia1, ja1, a1, ia2, ja2, a2, &ia_res, &ja_res, &a_res);

    // Print the result
    printf("Result matrix in CSR format:\n");
    for (int i = 0; i <= nrows; i++) {
        printf("ia_res[%d] = %d\n", i, ia_res[i]);
    }
    for (int i = 0; i < ia_res[nrows]; i++) {
        printf("ja_res[%d] = %d, a_res[%d] = %.2f\n", i, ja_res[i], i, a_res[i]);
    }

    // Free the allocated memory
    free(ia_res);
    free(ja_res);
    free(a_res);

    return 0;
}

