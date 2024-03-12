#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define NUM_ELEMENTS 100

pthread_t threads[NUM_THREADS];
int data[NUM_ELEMENTS];

void insertion_sort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge_sorted_buckets(int buckets[][NUM_ELEMENTS], int arr[], int n) {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < n; j++) {
            if (buckets[i][j] != 0) {
                arr[index++] = buckets[i][j];
            }
        }
    }
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void* sort_chunk(void* arg) {
    int thread_id = *(int*)arg;

    int chunk_size = NUM_ELEMENTS / NUM_THREADS;
    int start_index = thread_id * chunk_size;
    int end_index = start_index + chunk_size - 1;

    insertion_sort(&data[start_index], chunk_size);

    return NULL;
}

int main() {
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        data[i] = rand() % 100;
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, sort_chunk, thread_id);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    merge_sorted_chunks(data, NUM_ELEMENTS, NUM_THREADS);

    printf("Sorted Array:\n");
    print_array(data, NUM_ELEMENTS);

    return 0;
}
