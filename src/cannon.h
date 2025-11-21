#ifndef CANNON_H
#define CANNON_H

void multiply_matrix_cannon_seq(int **A, int **B, int **C, int n, int blocks_per_dim);
void multiply_matrix_cannon_par(int **A, int **B, int **C, int n, int blocks_per_dim, int threads);

void multiply_matrix_cannon_seq_stack(int n, int A[n][n], int B[n][n], int C[n][n], int blocks_per_dim);
void multiply_matrix_cannon_par_stack(int n, int A[n][n], int B[n][n], int C[n][n], int blocks_per_dim, int threads);


#endif