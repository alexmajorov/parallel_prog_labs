#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include <time.h>

#define N 6

int main(int argc, char *argv[]) 
{
    double c[N][N];
    double d[N][N];
    int rank, size;

    srand(time(NULL));
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            c[i][j] = (double)(rand() % 10000) / 100;
        
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    for (int i = rank; i < N; i += size)
        for (int j = 0; j < N; ++j)
        {
            if      (j > i) d[i][j] = c[i][j] * c[i][j];
            else if (j < i) d[i][j] = c[i][j] / 3;
            else            d[i][j] = c[i][j];
        }
            
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0)
    {
        printf("Matrix c: \n");
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j) 
                printf("%.2lf ", c[i][j]);
            printf("\n");
        }

        printf("\nMatrix d: \n");
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j) 
                printf("%.2lf ", d[i][j]);
            printf("\n");
        }
    }
    
    MPI_Finalize();

    return 0;
}
