#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "parallel_mergeSort.h"

// Structure for passing arguments to the sorting function
typedef struct {
    long *arr;
    long left;
    long right;
} MergeSortArgs;

// Function to merge two subarrays of arr[]
void parallel_merge(long arr[], long l, long m, long r) {
    long i, j, k;
    long n1 = m - l + 1;
    long n2 = r - m;

    // Crear arreglos temporales
    long L[n1], R[n2];

    // Copiar datos a los arreglos temporales L[] y R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    i = 0;
    j = 0;
    k = l;
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

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Parallel merge sort function
void* parallel_mergeSort(void* args) {
    MergeSortArgs* myArgs = (MergeSortArgs*) args;
    long* arr = myArgs->arr;
    long left = myArgs->left;
    long right = myArgs->right;

    if (left < right) {
        // Encuentra el punto medio del arreglo
        long mid = left + (right - left) / 2;

        // Crea los hilos que para ordenar la izquierda y la derecha
        pthread_t tid1, tid2;
        MergeSortArgs args1 = {arr, left, mid};
        MergeSortArgs args2 = {arr, mid + 1, right};

        pthread_create(&tid1, NULL, parallel_mergeSort, &args1);
        pthread_create(&tid2, NULL, parallel_mergeSort, &args2);

        // Wait for the threads to finish
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        // Merge the sorted subarrays
        parallel_merge(arr, left, mid, right);
    }

    pthread_exit(NULL);
}