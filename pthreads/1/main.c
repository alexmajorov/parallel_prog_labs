#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_int(void *arg)
{
    printf("Hello from thread %ld\n", atoi((char *)arg) + pthread_self());
    return NULL;
}

void *func_char(void *arg)
{
    printf("Hello from thread %s\n", (char *)arg);
    return NULL;
}

void *func_float(void *arg)
{
    printf("Hello from thread %f\n", atof((char * )arg));
    return NULL;
}

int main(int argc, char *argv[])
{
    int rc;
    pthread_t *threads;
    if (argc != 6) return 1;
    else
    {
        threads = (pthread_t *)malloc((argc - 1) * sizeof(pthread_t));
        for (int i = 1; i < argc; ++i)    
        { 
            if (i == 2) rc = pthread_create(&threads[i - 1], NULL, func_int, argv[i]);            
            else if (i == 1 || i == 3 || i == 4) rc = pthread_create(&threads[i - 1], NULL, func_char, argv[i]);
            else rc = pthread_create(&threads[i - 1], NULL, func_float, argv[i]);  
            if (rc != 0)
            {
                printf("Error creating thread %d\n", rc);
                return 1;
            }
        }
        for (int i = 1; i < argc; ++i)
            if ((rc = pthread_join(threads[i - 1], NULL)))
            {
                printf("Error joining thread %d\n", rc);
                return 1;
            }
    }
    free(threads);
    return 0;
}
