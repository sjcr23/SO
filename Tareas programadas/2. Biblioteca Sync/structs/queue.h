#ifndef sync_h
#define sync_h

// Estructura para representar un nodo de la cola
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Estructura para representar la cola
typedef struct {
    Node *front;
    Node *rear;
} Queue;

Queue* createQueue();
void enqueue(Queue *queue, int data);
int dequeue(Queue *queue);
int isEmpty(Queue *queue);

#endif