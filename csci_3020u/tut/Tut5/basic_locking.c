
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

pthread_mutex_t mutex;

void *runner(void *arg)
{
    char *str;

    str=(char*)arg;

    // Wait the main thead to lock
    usleep(10);
    printf("%s about to lock\n",str);

    // Request the lock.
    pthread_mutex_lock (&mutex);
    printf("%s says in critical section\n",str);

    // Release the lock.
    pthread_mutex_unlock (&mutex);
    printf("%s says in finished\n",str);

    return NULL;
}

/*
 * Simple locking example that allows shows how
 * simple locking can work.
 */
int main(int argc, char *argv[]) {

    pthread_t pth;

    // Initialize the mutex.
    pthread_mutex_init(&mutex, NULL);

    // Launch the new thread
    pthread_create(&pth, 0, runner, (void *) "Thread 1");

    printf("Main Thread locking\n");

    // Request the lock.
    pthread_mutex_lock(&mutex);

    printf("Main Thread within critical section\n");

    // Release the lock.
    pthread_mutex_unlock(&mutex);
    printf("Main Thread joinning  with thread\n");

    // wait for our thread to finish before continuing
    pthread_join(pth, 0);
    printf("Main Thread finished waiting\n");

    // Distroy the mutex.
    pthread_mutex_destroy(&mutex);

    return 0;
}