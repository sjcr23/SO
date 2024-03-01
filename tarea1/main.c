#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "mergeSort.c"


#define MIN_NUM -1000
#define MAX_NUM 1000
#define NUM_THREADS 2001


// Memoria compartida por el programa
long VECTOR_SIZE;

int moda = -1;
int frecuencia_maxima = 0;
pthread_mutex_t mutex;

typedef struct {
    long *arr;
    int id;
} CalcularModaArgs;

// Structure for passing arguments to the sorting function
typedef struct {
    long *arr;
    long left;
    long right;
} MergeSortArgs;


void *calcularModa(void *args) {
    CalcularModaArgs* myArgs = (CalcularModaArgs*) args;
    long* vector = myArgs->arr;
    int id_hilo = myArgs->id;

    // Encontrar el elemento con la frecuencia más alta en la sección
    int moda_local = -1;
    int frecuencia_maxima_local = 0;
    
    int i = 0;
    while (i < VECTOR_SIZE){
        if (vector[i] ==  id_hilo) {
            moda_local = vector[i];
            frecuencia_maxima_local++;
        }
        // No es necesario validar valores mayores al id del nodo
        if (vector[i] > id_hilo) {
            break;
        }

        // Increases
        int increase = 1;
        if (i+10 < VECTOR_SIZE && vector[i+10] < id_hilo){
            increase = 10;
        }
        if (i+100 < VECTOR_SIZE && vector[i+100] < id_hilo) {
            increase = 100;
        }
        if (i+1000 < VECTOR_SIZE && vector[i+1000] < id_hilo) {
            increase = 1000;
        }
        i = i+increase;
    }
   
    // Actualizar la moda global si se encuentra una moda local con una frecuencia más alta
    pthread_mutex_lock(&mutex);
    if (frecuencia_maxima_local > frecuencia_maxima) {
        moda = moda_local;
        frecuencia_maxima = frecuencia_maxima_local;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


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
        pthread_mutex_unlock(&mutex);
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        pthread_mutex_lock(&mutex);
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        pthread_mutex_unlock(&mutex);
        arr[k] = L[i];
        i++;
        k++;
        pthread_mutex_lock(&mutex);
    }
    

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        pthread_mutex_lock(&mutex);
        arr[k] = R[j];
        j++;
        k++;
        pthread_mutex_lock(&mutex);
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
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}

int main() {
    long N;
    long* copia;
    
    // 0. Inicializar el mutex y definir tamaño de lista
    pthread_mutex_init(&mutex, NULL);
    
    printf("Ingrese el tamaño de la lista: ");
    scanf("%ld", &N);

    VECTOR_SIZE = N;
    long vector[VECTOR_SIZE];
   
    // 1. Generar N números enteros aleatorios
    //    añadirlos a la lista con la misma semilla
    srand(time(0));
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        vector[i] = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
    }

    copia = clonarVector(vector, VECTOR_SIZE);
    

    // 2.A Ordenar la lista y calcular el tiempo que demora (mergeSort)
    clock_t start_time = clock();
    mergeSort(vector, 0, VECTOR_SIZE - 1);
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos. (mergeSort)\n", time_taken);



    // 2.B Ordenar la lista y calcular el tiempo que demora (parallel_mergeSort)
    clock_t p_start_time = clock();
    pthread_t tid;
    MergeSortArgs args = {copia, 0, VECTOR_SIZE - 1};
        
    // Crea el trhead para hacer el merge sort
    pthread_create(&tid, NULL, parallel_mergeSort, &args);   

    // Espera a que termine
    pthread_join(tid, NULL);
    clock_t p_end_time = clock();

    double p_time_taken = ((double)(p_end_time - p_start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos. (parallel_mergeSort)\n", p_time_taken);



    // 3. Crear los hilos e inicializar las frecuencias en cero.
    int ids[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++){
        ids[i] = MIN_NUM + i;
    }
    
    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        CalcularModaArgs args = {vector, ids[i]};
        pthread_create(&threads[i], NULL, calcularModa, &args);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    // Imprimir la moda y su frecuencia
    printf("La moda es: %d\n", moda);
    printf("La frecuencia de la moda es: %d\n", frecuencia_maxima);
    return 0;
}
