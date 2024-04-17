#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <chrono>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace std::chrono;

struct thread_data 
{
   int **arr;
   int cols; 
   int rows;
   int result;
};

void *task(void *data)
{
    struct thread_data *tdata = (struct thread_data *)data;
    
    for (int i = 0; i < tdata->rows; ++i)
    {
        if (tdata->cols == 1) tdata->result = 0;
        else
        {
            for (int j = 1; j < tdata->cols - 1; ++j) 
                if (tdata->arr[i][j] < tdata->arr[i][j - 1] && tdata->arr[i][j] < tdata->arr[i][j + 1])
                    ++tdata->result;
            if (tdata->arr[i][0] < tdata->arr[i][1]) ++tdata->result;
            if (tdata->arr[i][tdata->cols - 1] < tdata->arr[i][tdata->cols - 2]) ++tdata->result;
        }
    }
    
    return NULL;
} 

int local_mins_parralel(int **matrix, int rows, int cols, int cores, int *ans)
{
    int rc = 0;
    pthread_t *threads;
    int n = ((rows + cores - 1) / cores);
    threads = (pthread_t *)malloc(n * sizeof(pthread_t));
    struct thread_data *tdata = (struct thread_data *)malloc(n * sizeof(struct thread_data));
    for (int i = 0; i < n; ++i)
    { 
        tdata[i].arr = matrix + i * cores;
        tdata[i].cols = cols;
        tdata[i].rows = MIN(rows - i * cores, cores);
        tdata[i].result = 0;
        if ((rc = pthread_create(&threads[i], NULL, task, (void *) &tdata[i])))
        {
            printf("Error creating thread %d\n", rc);
            free(tdata);
            free(threads);
            return 1;
        }
    }

    for (int i = 0; i < n; ++i)
        if ((rc = pthread_join(threads[i], NULL)))
        {
            printf("Error joining thread %d\n", rc);
            free(tdata);
            free(threads);
            return 1;
        }

    (*ans) = 0;
    for (int i = 0; i < n; ++i)
        (*ans) += tdata[i].result;

    free(tdata);
    free(threads);
    return 0;
}

int local_mins_consecutive(int **matrix, int rows, int cols, int *ans)
{
    (*ans) = 0;
    if (cols > 1) 
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 1; j < cols - 1; ++j)
                if (matrix[i][j] < matrix[i][j - 1] && matrix[i][j] < matrix[i][j + 1])
                    ++(*ans);
            if (matrix[i][0] < matrix[i][1]) ++(*ans);
            if (matrix[i][cols - 1] < matrix[i][cols - 2]) ++(*ans);
        }
    }
    
    return 0;
}


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

    int ans = 0;
    auto beg = high_resolution_clock::now();
    local_mins_parralel(matrix, rows, cols, 1, &ans);
    auto end = high_resolution_clock::now();
    duration<double, std::milli> ms = end - beg;
    //printf("pthreads ans  (1 row per thread): %d\n", ans);
    printf("\npthreads time (1 row per thread): %lf ms\n", ms.count());

    ans = 0;
    beg = high_resolution_clock::now();
    local_mins_parralel(matrix, rows, cols, 10, &ans);
    end = high_resolution_clock::now();
    ms = end - beg;
    //printf("pthreads ans  (10 rows per thread): %d\n", ans);
    printf("\npthreads time (10 rows per thread): %lf ms\n", ms.count());

    ans = 0;
    beg = high_resolution_clock::now();
    local_mins_consecutive(matrix, rows, cols, &ans);
    end = high_resolution_clock::now();
    ms = end - beg;
    //printf("consecutive  ans: %d\n", ans);
    printf("\nconsecutive time: %lf ms\n", ms.count());

    for (int i = 0; i < rows; ++i) 
        free(matrix[i]);
    free(matrix);
    return 0;
}

