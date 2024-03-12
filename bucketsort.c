#include <stdio.h>
#include <stdlib.h>

#define NUM_BUCKETS 10

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

void merge_sorted_buckets(int buckets[][NUM_BUCKETS], int arr[], int n) {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < n; j++) {
            if (buckets[i][j] != 0) {
                arr[index++] = buckets[i][j];
            }
        }
    }
}

void bucket_sort(int arr[], int n) {
    int buckets[NUM_BUCKETS][n];

    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < n; j++) {
            buckets[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        int bucket_index = arr[i] / NUM_BUCKETS;
        int pos = buckets[bucket_index][0]++;
        buckets[bucket_index][pos + 1] = arr[i];
    }

    for (int i = 0; i < NUM_BUCKETS; i++) {
        insertion_sort(buckets[i], n);
    }

    merge_sorted_buckets(buckets, arr, n);
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[] = {29, 25, 3, 49, 9, 37, 21, 43};
    int n = sizeof(arr) / sizeof(arr[0]);


    bucket_sort(arr, n);

    printf("Sorted Array:\n");
    print_array(arr, n);

    return 0;
}
