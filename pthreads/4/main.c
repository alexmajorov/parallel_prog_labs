#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <math.h> 

#ifndef N 
#define N 10000000 
#endif 

double integral = 0.0; 
double buffer[N]; 
int buffer_len = 0; 
int buffer_full = 0; 
 
pthread_mutex_t mutex;
pthread_cond_t buffer_full_cond;

double my_rand(double a, double b)  
{ 
    double range = b - a;  
    double div = RAND_MAX / range; 
    return a + rand() / div; 
} 
 
void *calculate_integral(void *arg) 
{ 
    pthread_mutex_lock(&mutex); 
    while (!buffer_full) 
        pthread_cond_wait(&buffer_full_cond, &mutex); 
 
    for (int i = 0; i < N; ++i) 
        integral += exp(1.0 / buffer[i]); 
 
    integral /= N; 
    printf("Integral value: %f\n", integral); 
    pthread_mutex_unlock(&mutex); 
    return NULL; 
} 
 
void *generate_random_numbers(void *arg) 
{ 
    for (int i = 0; i < N; ++i) 
    { 
        buffer[buffer_len++] = my_rand(2.0, 3.0); 
        if (buffer_len == N) 
        { 
            pthread_mutex_lock(&mutex); 
            buffer_full = 1; 
            buffer_len = 0; 
            pthread_cond_signal(&buffer_full_cond); 
            pthread_mutex_unlock(&mutex); 
        } 
    } 
    return NULL; 
} 
 
int main(void) 
{ 
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&buffer_full_cond, NULL);
    
    pthread_t threads[5]; 
 
    pthread_create(&threads[4], NULL, calculate_integral, NULL); 
    for (int i = 0; i < 4; i++) 
        pthread_create(&threads[i], NULL, generate_random_numbers, NULL);
        
    for (int i = 0; i < 5; i++) 
        pthread_join(threads[i], NULL);
     
    pthread_mutex_destroy(&mutex); 
    pthread_cond_destroy(&buffer_full_cond);
 
    return 0; 
}
