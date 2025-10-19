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

int generate_square_matrix(int n, int ***matrix)
{
    if (!allocate_square_matrix(n, matrix))
        return 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            (*matrix)[i][j] = rand() % 10;

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

int main(int argc, char **argv)
{
    const int random_seed = 69420;

    /* seq
    int blocks_per_dim = 1, repeats = 50;
    double time_avg = 0.;

    printf("blocks_per_dim=%d repeats=%d seed=%d\n", blocks_per_dim, repeats, random_seed);
    printf("n\ttime_avg\n");

    for (int n = 10; n <= 250; n += 10)
    {
        time_avg = experiment_seq(n, blocks_per_dim, repeats, random_seed);
        printf("%d\t%.9f\n", n, time_avg);
    }
    */

    /* par */
    int blocks_per_dim = 1, repeats = 1, n = 144 * 9;
    double time_avg = 0.;

    printf("n=%d repeats=%d seed=%d\n", n, repeats, random_seed);
    printf("threads\ttime_avg\n");

    for (int threads = 1; threads <= 16; threads += 1)
    {
        blocks_per_dim = 16 * 9;
        time_avg = experiment_par(n, blocks_per_dim, threads, repeats, random_seed);
        printf("%d\t%.9f\n", threads, time_avg);
    }

    return 0;
}