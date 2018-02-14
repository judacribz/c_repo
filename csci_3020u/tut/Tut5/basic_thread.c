
#include <stdio.h>
#include <pthread.h>

void *threadFunc(void *arg)
{
    char *str;
    int i = 0;

    str=(char*)arg;

    while(i < 10)
    {
        // Show the thread is running
        printf("On %s\n",str);
        ++i;
    }

    return NULL;
}

/**
 * Basic example that showing threading interleaving.
 */
int main(int argc, char *argv[]) {

    // Thread object
    pthread_t pth;
    pthread_t pth2;

    // Create the thread.
    pthread_create(&pth, 0, threadFunc, (void *) "Thread 1");
    pthread_create(&pth2, 0, threadFunc, (void *) "Thread 2");

    // wait for our thread to finish before continuing
    pthread_join(pth, 0);
    pthread_join(pth2, 0);

    return 0;
}