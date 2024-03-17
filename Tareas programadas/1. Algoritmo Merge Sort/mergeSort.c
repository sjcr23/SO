void merge(long arr[], int inicio, int medio, int fin) {
    int i, j, k;
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    // Crear arreglos temporales
    int L[n1], R[n2];

    // Copiar datos a los arreglos temporales L[] y R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[inicio + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[medio + 1 + j];

    // Combinar los arreglos temporales de vuelta en arr[]
    i = 0;
    j = 0;
    k = inicio;
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

    // Copiar los elementos restantes de L[], si los hay
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si los hay
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(long arr[], int inicio, int fin) {
    if (inicio < fin) {
        // Encuentra el punto medio del arreglo
        int medio = inicio + (fin - inicio) / 2;

        // Ordena la primera mitad y la segunda mitad
        mergeSort(arr, inicio, medio);
        mergeSort(arr, medio + 1, fin);

        // Combina las dos mitades ordenadas
        merge(arr, inicio, medio, fin);
    }
}


long* clonarVector(long* original, int size) {
    long* clon = (long*)malloc(size * sizeof(long));
    if (clon == NULL) {
        printf("Error: No se pudo asignar memoria para el vector clonado.\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        clon[i] = original[i];
    }
    return clon;
}

void imprimirVector(long arr[], int size) {
    for (int i = 0; i < size; i++){
        printf("%ld ", arr[i]);
    }
    printf("\n");
}