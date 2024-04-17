#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <math.h>

#define EPS 0.001

#ifndef M 
#define M 10
#endif 
 
pthread_mutex_t mutex;
static pthread_barrier_t barrier;

void *task(void *data)
{
    double *x = (double *)data;
    double term = 1.0;
    double s = 1.0;
    int i = 1;
    while (fabs(term) >= EPS)
    {
        term = (1 + 2.0 / (2 * i - 1)) * (*x) / i * term;
        s += term;
        ++i;
    }
    pthread_barrier_wait(&barrier);
    *x = s;
    return NULL;
}

int main(void) 
{ 
    double a = 0.1, b = 0.8;
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, M);
    
    pthread_t threads[M]; 
    double ans[M];

    for (int i = 0; i < M; ++i)
        ans[i] = a + i * (b - a) / (M - 1);
    
    for (int i = 0; i < M; i++) 
        pthread_create(&threads[i], NULL, task, (void *)&ans[i]);
        
    for (int i = 0; i < M; i++) 
        pthread_join(threads[i], NULL);
    
    for (int i = 0; i < M; ++i)
        printf("S(%lf) = %lf\n", a + i * (b - a) / (M - 1), ans[i]);
    
    pthread_mutex_destroy(&mutex); 
    pthread_barrier_destroy(&barrier);
 
    return 0; 
}
