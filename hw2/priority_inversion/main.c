// Originated from
// https://github.com/qwe661234/MuThreadPackage/blob/main/Tests/test-04-priority-inversion.c

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "atomic.h"
#include "futex.h"
#include "mutex.h"


#define THREADCOUNT 3

mutex_t mutex_normal, mutex_normal_2;

void *TASK1(void *arg)
{
    mutex_lock(&mutex_normal);
    printf("1\n");
    mutex_unlock(&mutex_normal);
    return NULL;
}

void *TASK2(void *arg)
{
    mutex_lock(&mutex_normal_2);
    sleep(1);
    printf("2\n");
    mutex_unlock(&mutex_normal_2);
    return NULL;
}

void *TASK3(void *arg)
{
    mutex_lock(&mutex_normal);
    sleep(1);
    printf("3\n");
    mutex_unlock(&mutex_normal);
    return NULL;
}

static void *(*TASKS[])() = {
    TASK1,
    TASK2,
    TASK3,
};

int main()
{
    int st;
    pthread_t th[THREADCOUNT];
    pthread_attr_t attr;

    struct sched_param param;
    mutex_init(&mutex_normal);
    mutex_init(&mutex_normal_2);

    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    for (int i = THREADCOUNT - 1; i >= 0; i--) {
        param.sched_priority = (THREADCOUNT - i) * 10;
        pthread_attr_setschedparam(&attr, &param);
        st = pthread_create(&th[i], NULL, TASKS[i], NULL);
        if (st != 0) {
            printf("Failed to spawn thread %d: %s\n", i, strerror(-st));
            return 1;
        }
    }

    void *result;
    for (int i = 0; i < THREADCOUNT; i++)
        pthread_join(th[i], &result);
    return 0;
}