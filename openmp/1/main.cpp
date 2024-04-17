#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <chrono>

using namespace std::chrono;

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define NUM_THREADS 10

int main(void)
{
    int rows = 0, cols = 0;
    printf("rows: "); if (scanf("%d", &rows) == 0 || rows < 1) return 1;
    printf("cols: "); if (scanf("%d", &cols) == 0 || cols < 1) return 1;
    
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; ++i) 
        matrix[i] = (int *)malloc(cols * sizeof(int));

    srand(time(NULL));
    
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix[i][j] = rand() % 10;
    
    /*
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    */

    auto beg = high_resolution_clock::now();
    omp_set_num_threads(NUM_THREADS);
    int ans = 0;
    #pragma omp parallel reduction(+:ans)
    {
        int cur_thread = omp_get_thread_num();
        for (int i = cur_thread; i < rows; i += NUM_THREADS)
        {
            for (int j = 1; j < cols - 1; ++j)
                if (matrix[i][j] < matrix[i][j - 1] && matrix[i][j] < matrix[i][j + 1])
                    ++ans;
            if (matrix[i][0] < matrix[i][1]) ++ans;
            if (matrix[i][cols - 1] < matrix[i][cols - 2]) ++ans;
        }
    }
    auto end = high_resolution_clock::now();
    duration<double, std::milli> ms = end - beg;
    //printf("openmp ans : %d\n", ans);
    printf("openmp time: %lf ms\n", ms.count());

    for (int i = 0; i < rows; ++i) 
        free(matrix[i]);
    free(matrix);
    return 0;
}

