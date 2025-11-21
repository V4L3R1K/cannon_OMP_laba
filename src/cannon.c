#include "cannon.h"

void multiply_matrix_cannon_seq(int **A, int **B, int **C, int n, int blocks_per_dim)
{
    int block_size = n / blocks_per_dim;

    for (int block_i = 0; block_i < blocks_per_dim; block_i++)
        for (int block_j = 0; block_j < blocks_per_dim; block_j++)
            for (int step = 0; step < blocks_per_dim; step++)
            {
                int i_base = block_i * block_size;
                int j_base = block_j * block_size;
                int k_base = ((block_i + block_j + step) % blocks_per_dim) * block_size;

                for (int i = 0; i < block_size; i++)
                    for (int j = 0; j < block_size; j++)
                        for (int k = 0; k < block_size; k++)
                            C[i_base + i][j_base + j] += A[i_base + i][k_base + k] * B[k_base + k][j_base + j];
            }
}

void multiply_matrix_cannon_par(int **A, int **B, int **C, int n, int blocks_per_dim, int threads)
{
    int block_size = n / blocks_per_dim;

#pragma omp parallel for collapse(2) num_threads(threads) default(none) shared(A, B, C, blocks_per_dim, block_size)
    for (int block_i = 0; block_i < blocks_per_dim; block_i++)
        for (int block_j = 0; block_j < blocks_per_dim; block_j++)
            for (int step = 0; step < blocks_per_dim; step++)
            {
                int i_base = block_i * block_size;
                int j_base = block_j * block_size;
                int k_base = ((block_i + block_j + step) % blocks_per_dim) * block_size;

                for (int i = 0; i < block_size; i++)
                    for (int j = 0; j < block_size; j++)
                        for (int k = 0; k < block_size; k++)
                            C[i_base + i][j_base + j] += A[i_base + i][k_base + k] * B[k_base + k][j_base + j];
            }
}

void multiply_matrix_cannon_seq_stack(int n, int A[n][n], int B[n][n], int C[n][n], int blocks_per_dim)
{
    int block_size = n / blocks_per_dim;

    for (int block_i = 0; block_i < blocks_per_dim; block_i++)
        for (int block_j = 0; block_j < blocks_per_dim; block_j++)
            for (int step = 0; step < blocks_per_dim; step++)
            {
                int i_base = block_i * block_size;
                int j_base = block_j * block_size;
                int k_base = ((block_i + block_j + step) % blocks_per_dim) * block_size;

                for (int i = 0; i < block_size; i++)
                    for (int j = 0; j < block_size; j++)
                        for (int k = 0; k < block_size; k++)
                            C[i_base + i][j_base + j] += A[i_base + i][k_base + k] * B[k_base + k][j_base + j];
            }
}

void multiply_matrix_cannon_par_stack(int n, int A[n][n], int B[n][n], int C[n][n], int blocks_per_dim, int threads)
{
    int block_size = n / blocks_per_dim;

#pragma omp parallel for collapse(2) num_threads(threads) default(none) shared(A, B, C, blocks_per_dim, block_size)
    for (int block_i = 0; block_i < blocks_per_dim; block_i++)
        for (int block_j = 0; block_j < blocks_per_dim; block_j++)
            for (int step = 0; step < blocks_per_dim; step++)
            {
                int i_base = block_i * block_size;
                int j_base = block_j * block_size;
                int k_base = ((block_i + block_j + step) % blocks_per_dim) * block_size;

                for (int i = 0; i < block_size; i++)
                    for (int j = 0; j < block_size; j++)
                        for (int k = 0; k < block_size; k++)
                            C[i_base + i][j_base + j] += A[i_base + i][k_base + k] * B[k_base + k][j_base + j];
            }
}
