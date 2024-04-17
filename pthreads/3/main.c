#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

int b_len = 0;
int m = 0;

struct thread_data 
{
    int *from;
    int *to;
    int i, j;
};

pthread_mutex_t mutex;

void *task(void *data)
{
    struct thread_data *tdata = (struct thread_data *)data;
    for(int i = tdata->i; i < tdata->j; ++i)
        if (abs(tdata->from[i]) < m)
        {
            pthread_mutex_lock(&mutex);
            tdata->to[b_len] = tdata->from[i];
            ++b_len;
            pthread_mutex_unlock(&mutex);
        }
    return NULL;
}

int main(void)
{
    int n = 0, p = 0;
    printf("Array a len: "); if (scanf("%d", &n) == 0 || n < 1) return 1; 
    printf("Number of threads: "); if (scanf("%d", &p) == 0 || p < 1 || p > n) return 1; 
    printf("m: "); if (scanf("%d", &m) == 0 || n < 1) return 1; 
    
    pthread_mutex_init(&mutex, NULL);
    
    int *a = (int *)malloc(n * sizeof(int));
    int *b = (int *)malloc(n * sizeof(int));

    srand(time(NULL));

    for (int i = 0; i < n; ++i)
        a[i] = rand() % 200 - 100;

    printf("a: ");
    for (int i = 0; i < n; ++i)
        printf("%d ", a[i]);
    printf("\n");

    int rc = 0;
    pthread_t *threads = (pthread_t *)malloc(p * sizeof(pthread_t));
    struct thread_data *tdata = (struct thread_data *)malloc(p * sizeof(struct thread_data));
    int extra_left = n % p;
    int extra_added = 0;
    for (int k = 0; k < p; ++k)
    {
        tdata[k].from = a;
        tdata[k].to = b;
        tdata[k].i = k * (n / p) + extra_added;
        tdata[k].j = tdata[k].i + n / p; 
        if (extra_left > 0)
        {
            tdata[k].j++;
            extra_left--;
            extra_added++;
        }
        if ((rc = pthread_create(&threads[k], NULL, task, (void *) &tdata[k])))
        {
            printf("Error creating thread %d\n", rc);
            free(a);
            free(b);
            free(threads);
            free(tdata);
            return 1;
        }
    }

    for (int i = 0; i < p; ++i)
        if ((rc = pthread_join(threads[i], NULL)))
        {
            printf("Error joining thread %d\n", rc);
            free(a);
            free(b);
            free(threads);
            free(tdata);
            return 1;
        }

    printf("b: ");
    for (int i = 0; i < b_len; ++i)
        printf("%d ", b[i]);
    printf("\n");

    pthread_mutex_destroy(&mutex);
    free(a);
    free(b);
    free(threads);
    free(tdata);
    return 0;
}
