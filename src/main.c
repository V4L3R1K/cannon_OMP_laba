#include "cannon.h"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h> //omp_get_wtime()

int allocate_square_matrix(int n, int ***matrix)
{
    if (n <= 0 || !matrix)
        return 0;

    *matrix = (int **)malloc(n * sizeof(int *));
    if (!*matrix)
    {
        *matrix = 0;
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        (*matrix)[i] = (int *)malloc(n * sizeof(int));
        if (!(*matrix)[i])
        {
            for (int k = 0; k < i; k++)
                free((*matrix)[k]);
            free(*matrix);
            *matrix = 0;
            return 0;
        }
    }

    return 1;
}

int generate_zero_square_matrix(int n, int ***matrix)
{
    if (!allocate_square_matrix(n, matrix))
        return 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            (*matrix)[i][j] = 0;

    return 1;
}

int generate_zero_square_matrix_stack(int n, int matrix[n][n]){
    for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                (matrix)[i][j] = 0;
    return 1;
}

int generate_square_matrix(int n, int ***matrix)
{
    if (!allocate_square_matrix(n, matrix))
        return 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            (*matrix)[i][j] = rand() % 10;

    return 1;
}

int generate_square_matrix_stack(int n, int matrix[n][n]){
    for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                (matrix)[i][j] = rand() % 10;
    return 1;
}

void free_matrix(int n, int **matrix)
{
    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
}

void print_matrix(int n, int **matrix)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%6d\t", matrix[i][j]);
        printf("\n");
    }
}

double experiment_seq(int n, int blocks_per_dim, int repeats, int random_seed)
{
    srand(random_seed);

    int **A = 0, **B = 0, **C = 0;
    double time_start = 0., time_end = 0., time_sum = 0.;

    for (int i = 0; i < repeats; i++)
    {
        if (!generate_square_matrix(n, &A) || !generate_square_matrix(n, &B) || !generate_zero_square_matrix(n, &C))
        {
            fprintf(stderr, "Unable to generate matrix\n");
            return -1;
        }

        time_start = omp_get_wtime();
        multiply_matrix_cannon_seq(A, B, C, n, blocks_per_dim);
        time_end = omp_get_wtime();

        time_sum += time_end - time_start;

        free_matrix(n, A);
        free_matrix(n, B);
        free_matrix(n, C);
    }

    return time_sum / repeats;
}

double experiment_par(int n, int blocks_per_dim, int threads, int repeats, int random_seed)
{
    srand(random_seed);

    int **A = 0, **B = 0, **C = 0;
    double time_start = 0., time_end = 0., time_sum = 0.;

    for (int i = 0; i < repeats; i++)
    {
        if (!generate_square_matrix(n, &A) || !generate_square_matrix(n, &B) || !generate_zero_square_matrix(n, &C))
        {
            fprintf(stderr, "Unable to generate matrix\n");
            return -1;
        }

        time_start = omp_get_wtime();
        multiply_matrix_cannon_par(A, B, C, n, blocks_per_dim, threads);
        time_end = omp_get_wtime();

        time_sum += time_end - time_start;

        free_matrix(n, A);
        free_matrix(n, B);
        free_matrix(n, C);
    }

    return time_sum / repeats;
}

double experiment_seq_stack(int n, int blocks_per_dim, int repeats, int random_seed)
{
    srand(random_seed);

    int A[n][n];
    int B[n][n];
    int C[n][n];
    double time_start = 0., time_end = 0., time_sum = 0.;

    for (int i = 0; i < repeats; i++)
    {
        if (!generate_square_matrix_stack(n, A) || !generate_square_matrix_stack(n, B) || !generate_zero_square_matrix_stack(n, C))
        {
            fprintf(stderr, "Unable to generate matrix\n");
            return -1;
        }

        time_start = omp_get_wtime();
        multiply_matrix_cannon_seq_stack(n, A, B, C, blocks_per_dim);
        time_end = omp_get_wtime();

        time_sum += time_end - time_start;
    }

    return time_sum / repeats;
}

double experiment_par_stack(int n, int blocks_per_dim, int threads, int repeats, int random_seed)
{
    srand(random_seed);
    
    int A[n][n];
    int B[n][n];
    int C[n][n];

    double time_start = 0., time_end = 0., time_sum = 0.;

    for (int i = 0; i < repeats; i++)
    {
        if (!generate_square_matrix_stack(n, A) || !generate_square_matrix_stack(n, B) || !generate_zero_square_matrix_stack(n, C))
        {
            fprintf(stderr, "Unable to generate matrix\n");
            return -1;
        }

        time_start = omp_get_wtime();
        multiply_matrix_cannon_par_stack(n, A, B, C, blocks_per_dim, threads);
        time_end = omp_get_wtime();

        time_sum += time_end - time_start;
    }

    return time_sum / repeats;
}


int main(int argc, char **argv)
{
    const int random_seed = 69420;

    /* seq*/
    // int blocks_per_dim = 1, threads = 16, repeats = 1, n = 144 * 9;
    // double time_avg = 0.;
    // printf("blocks");
    // for (n = 8; n <= 1024+8; n += 8)
    // {
    //     printf(" %d", n);
    // }
    // n = 1024;
    // printf("\ntime");
    // // printf("\n%d", blocks_per_dim);
    // for (blocks_per_dim = 8; blocks_per_dim <= 1024+8; blocks_per_dim+=8 )
    // {
    //     threads = blocks_per_dim * blocks_per_dim;
    //     // printf("\n%d", blocks_per_dim);
    //     // for (n = 16; n <= 1024; n += 16)
    //     // {
    //     time_avg = experiment_seq(n, blocks_per_dim, repeats, random_seed);
    //     printf("\t%.9f", time_avg);
    //     fflush(stdout);
    //     // }
    // }
    // puts("");
    /* par */
    int blocks_per_dim = 1, threads = 16, repeats = 30, n = 144 * 9;
    double time_avg = 0.;
    
    puts("Расчет зависимости времени от размера матриц и от количества потоков.");
    puts("От 36 до 1 потока, и от 64 до 1024 размеры матриц.");

    printf("threads");
    for (n = 64; n <= 1024; n += 64)
    {
        printf(" %d", n);
    }
    for (blocks_per_dim = 6; blocks_per_dim >= 1; blocks_per_dim--)
    {
        threads = blocks_per_dim * blocks_per_dim;
        printf("\n%d", threads);
        for (n = 64; n <= 1024; n += 64)
        {
            time_avg = experiment_par_stack(n, blocks_per_dim, threads, repeats, random_seed);
            printf(" %.9f", time_avg);
            fflush(stdout);
        }
    }           
    puts("");
    return 0;
}



