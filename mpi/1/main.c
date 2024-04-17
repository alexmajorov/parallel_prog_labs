#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include <time.h>

#define ARRAY_SIZE 15

int main(int argc, char *argv[]) 
{
    //setbuf(stdout, NULL);
    int array[ARRAY_SIZE];
    int sum = 0;
    int rank, size;

    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; ++i) 
    {
        array[i] = rand() % 100;
        sum += array[i];
    }   
    
    double average = (double)sum / ARRAY_SIZE;
    int global_closest_value;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_closest_value = array[rank];
    double local_min_diff = abs(average - local_closest_value);
    for (int i = rank + size; i < ARRAY_SIZE; i += size)     
        if (abs(average - array[i]) < local_min_diff)
        {
            local_min_diff = abs(average - array[i]);
            local_closest_value = array[i];
        }
    
    printf("Rank %d: %d\n", rank, local_closest_value);
    
    int local_closest_values[size];
    MPI_Gather(&local_closest_value, 1, MPI_INT, local_closest_values, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        global_closest_value = local_closest_values[0];
        double global_min_diff = abs(average - global_closest_value);
        for (int i = 1; i < size; ++i)
            if (abs(average - local_closest_values[i]) < global_min_diff)
            {
                global_min_diff = abs(average - local_closest_values[i]);
                global_closest_value = local_closest_values[i];
            }

        fprintf(stderr, "Array: ");
        for (int i = 0; i < ARRAY_SIZE; ++i) 
            fprintf(stderr, "%d ", array[i]);
        fprintf(stderr, "\n");

        fprintf(stderr, "Average: %lf\n", average);
        fprintf(stderr, "Closest element to average: %d\n", global_closest_value);
    }

    MPI_Finalize();

    return 0;
}
