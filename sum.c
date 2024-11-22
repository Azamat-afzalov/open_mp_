#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>
#include<stdlib.h>

#define NUM_THREADS 10
#define ITERATIONS 100000000 // 100 million

long long int sum = 0;
pthread_mutex_t lock;

void *calculate_sum(void *arg) {
    int thread_id = *(int*)arg;
    long int partial_sum = 0;

    int start = thread_id * (ITERATIONS / NUM_THREADS);
    int end = start + (ITERATIONS / NUM_THREADS);


    for (int i = start + 1; i <= end; i++) {
        partial_sum += i;
    }

    // lock the mutex to change global sum;
    pthread_mutex_lock(&lock);
    sum += partial_sum;
    // unlock the mutext after changing global sum;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&lock, NULL);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;

        if (pthread_create(&threads[i], NULL, calculate_sum, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    long long int actual = (long long)ITERATIONS * (((long long)ITERATIONS + 1) / 2);


    printf("Calculated Sum = %lld \n", sum);
    printf("Actual = %lld \n", actual);
    
    pthread_mutex_destroy(&lock);

    return 0;
}