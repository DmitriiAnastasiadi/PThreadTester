#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_COUNT 10000000
#define THREADS_COUNT 4

typedef int bool;

typedef struct thread_arguments
{
    int *array;
    int start; int end;
    int result;
} thread_arguments;

bool cmp_function(int val) {
    return val > RAND_MAX/2;
}

// counts how much numbers in array fits in compare function in between [start, end)
void *thread_work(void *arguments) {
    thread_arguments *args = (thread_arguments *)arguments;
    int count = 0;
    for (int i = args->start; i < args->end; i++) {
        // printf("%d %d %d\n", i, args->start, args->end);
        if (cmp_function(args->array[i])) {
            count++;
        }
    }
    args->result = count;
    pthread_exit(NULL);
}

int calc_greater_parallel(int *nums, int nums_count, int thread_count) {
    printf("%s\n", __FUNCTION__);
    struct timespec t0, t1;
    clock_gettime(1, &t0);

    // prepare threads
    pthread_t *threads_ids = calloc(sizeof(pthread_t), thread_count);
    thread_arguments *threads_args = calloc(sizeof(thread_arguments), thread_count);
    for (int i = 0; i < thread_count; i++) {
        thread_arguments th_arg = {
            .array = nums,
            .start = nums_count / thread_count * i,
            .end = (i != thread_count - 1) ? (nums_count / thread_count * (i + 1)) : nums_count
        };
        threads_args[i] = th_arg;
        if (pthread_create(&(threads_ids[i]), NULL, &thread_work, &threads_args[i])) {
            printf("Error occured while creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    // joining threads
    for (int i = 0; i < thread_count; i++) {
        if (pthread_join(threads_ids[i], NULL)) {
            printf("Error occured while joining thread\n");
            exit(EXIT_FAILURE);
        }
    }
    int result = 0;
    for (int i = 0; i < thread_count; i++) {
        result += threads_args[i].result;
    }
    clock_gettime(1, &t1);
    printf("time = %ld\n", (t1.tv_sec - t0.tv_sec) * 1000000 + (t1.tv_nsec - t0.tv_nsec) / 1000);
    free(threads_ids);
    free(threads_args);
    return result;
}

int calc_greater(int *nums, int nums_count) {
    printf("%s\n", __FUNCTION__);
    struct timespec t0, t1;
    clock_gettime(1, &t0);
    int result = 0;
    for (int i = 0; i < nums_count; i++) {
        if (cmp_function(nums[i])) {
            result++;
        }
    }
    clock_gettime(1, &t1);
    printf("time = %ld\n", (t1.tv_sec - t0.tv_sec) * 1000000 + (t1.tv_nsec - t0.tv_nsec) / 1000);
    return result;
}

int main() {
    srand(time(NULL));
    
    int *nums = calloc(sizeof(int), ARRAY_COUNT);
    if (nums == NULL) {
        printf("memory allocation error\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < ARRAY_COUNT; i++) {
        nums[i] = rand();
    }

    printf("result = %d\n", calc_greater(nums, ARRAY_COUNT));
    printf("result = %d\n", calc_greater_parallel(nums, ARRAY_COUNT, 4));

    free(nums);
    return EXIT_SUCCESS;
}