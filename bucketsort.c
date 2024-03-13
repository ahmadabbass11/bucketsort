#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BUCKETS 10
#define NUM_ELEMENTS 1000

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

void merge_buckets(int buckets[][NUM_ELEMENTS], int arr[], int n) {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            if (buckets[i][j] != -1) {
                arr[index++] = buckets[i][j];
            }
        }
    }
}

void bucket_sort(int arr[], int n) {
    int buckets[NUM_BUCKETS][NUM_ELEMENTS];

    // Initialize buckets
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            buckets[i][j] = -1;
        }
    }

    // Distribute elements into buckets
    for (int i = 0; i < n; i++) {
        int bucket_index = arr[i] / (100 / NUM_BUCKETS); // Assuming elements are in the range 0-99
        int pos = 0;
        while (buckets[bucket_index][pos] != -1) {
            pos++;
        }
        buckets[bucket_index][pos] = arr[i];
    }

    // Sort each bucket
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (buckets[i][0] != -1) { // Check if the bucket is not empty
            merge_sort(buckets[i], 0, NUM_ELEMENTS - 1);
        }
    }

    // Merge sorted buckets into the original array
    merge_buckets(buckets, arr, n);
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
