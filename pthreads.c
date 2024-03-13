#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_BUCKETS 10
#define NUM_ELEMENTS 1000
#define NUM_THREADS 3 // Number of threads to use

// Structure for passing arguments to the thread function
typedef struct {
    int thread_id;
    int* arr;
    int* buckets[NUM_BUCKETS];
    int bucket_size[NUM_BUCKETS];
    int max_bucket_size;
} ThreadArgs;

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void *sort_buckets(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int start_bucket = thread_args->thread_id * (NUM_BUCKETS / NUM_THREADS);
    int end_bucket = (thread_args->thread_id + 1) * (NUM_BUCKETS / NUM_THREADS);

    for (int i = start_bucket; i < end_bucket; i++) {
        if (thread_args->buckets[i] != NULL) {
            merge_sort(thread_args->buckets[i], 0, thread_args->bucket_size[i] - 1);
        }
    }

    pthread_exit(NULL);
}

void merge_buckets(ThreadArgs *thread_args) {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (thread_args->buckets[i] != NULL) {
            for (int j = 0; j < thread_args->bucket_size[i]; j++) {
                thread_args->arr[index++] = thread_args->buckets[i][j];
            }
            free(thread_args->buckets[i]); // Free memory allocated for buckets
        }
    }

    // Sort the merged array using merge sort
    merge_sort(thread_args->arr, 0, thread_args->max_bucket_size - 1);
}

void bucket_sort(int arr[], int n) {
    int* buckets[NUM_BUCKETS] = {NULL}; // Initialize buckets array with NULL pointers
    int bucket_size[NUM_BUCKETS] = {0}; // Initialize bucket sizes to 0
    int max_bucket_size = 0;

    // Distribute elements into buckets
    for (int i = 0; i < n; i++) {
        int bucket_index = arr[i] / (100 / NUM_BUCKETS); // Assuming elements are in the range 0-99
        int size = ++bucket_size[bucket_index];
        if (size > max_bucket_size) {
            max_bucket_size = size;
        }
        buckets[bucket_index] = realloc(buckets[bucket_index], size * sizeof(int));
        buckets[bucket_index][size - 1] = arr[i];
    }

    // Create threads and sort buckets
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_id = i;
        thread_args[i].arr = arr;
        for (int j = 0; j < NUM_BUCKETS; j++) {
            thread_args[i].buckets[j] = buckets[j];
            thread_args[i].bucket_size[j] = bucket_size[j];
        }
        thread_args[i].max_bucket_size = max_bucket_size;
        pthread_create(&threads[i], NULL, sort_buckets, (void *)&thread_args[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge sorted buckets into the original array
    merge_buckets(&thread_args[0]);
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    int arr[NUM_ELEMENTS];

    start = clock();
    srand(time(NULL));
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        arr[i] = rand() % 100;
    }
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Unsorted Array:\n");
    print_array(arr, n);

    bucket_sort(arr, n);

    end = clock();

    printf("Sorted Array:\n");
    print_array(arr, n);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    return 0;
}

