#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void heap_lock(void)
{
    pthread_mutex_lock(&mutex);
}

void heap_unlock(void)
{
    pthread_mutex_unlock(&mutex);
}