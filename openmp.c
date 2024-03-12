#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_BUCKETS 10
#define NUM_ELEMENTS 100

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

void bucket_sort(int arr[], int n) {
    int buckets[NUM_BUCKETS][NUM_ELEMENTS] = {0};

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int bucket_index = arr[i] / NUM_BUCKETS;
        int pos = __sync_fetch_and_add(&buckets[bucket_index][0], 1);
        buckets[bucket_index][pos] = arr[i];
    }

    #pragma omp parallel for
    for (int i = 0; i < NUM_BUCKETS; i++) {
        insertion_sort(buckets[i], n);
    }

    merge_sorted_buckets(buckets, arr, n);
}

int main() {
    int arr[NUM_ELEMENTS];


    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        arr[i] = rand() % 100;
    }

    bucket_sort(arr, NUM_ELEMENTS);

    printf("Sorted Array:\n");
    print_array(arr, NUM_ELEMENTS);

    return 0;
}
