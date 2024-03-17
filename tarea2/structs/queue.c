#include <stdio.h>
#include <stdlib.h>
#include "sync.h"

// Función para inicializar una cola vacía
Queue* createQueue() {
    Queue *queue = calloc(1, sizeof(Queue));
    if (queue == NULL) {
        printf("Error: No se pudo asignar memoria para la cola.\n");
        exit(EXIT_FAILURE);
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

// Función para encolar un elemento en la cola
void enqueue(Queue *queue, pid_t pid) {
    // Crear el nodo
    Node *newNode = (Node*)calloc(1, sizeof(Node));
    if (newNode == NULL) {
        printf("Error: No se pudo asignar memoria para el nodo.\n");
        exit(EXIT_FAILURE);
    }
    newNode->pid = pid;
    newNode->next = NULL;

    // Añadir a la cola
    if (queue->tail == NULL) {
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
}

// Función para desencolar un elemento de la cola
pid_t dequeue(Queue *queue) {
    // Cola vacía
    if (queue->head == NULL) {
        printf("La cola está vacía.\n");
        exit(EXIT_SUCCESS);
    }
    
    // Sacar de la cola
    Node *temp = queue->head;
    int pid = temp->pid;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(temp);
    return pid;
}

// Función para verificar si la cola está vacía
int isEmpty(Queue *queue) {
    return queue->head == NULL;
}