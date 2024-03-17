# Biblioteca Sync

Esta biblioteca hace uso de Hilos, Mutex y los operadores Wait/Notify para  incorporar las estructuras de sincronización: Semáforo, Barrera y Monitor.

## Desarollada por
- 2016101574 - Josué Castro Ramírez
- 2020065036 - Adrián Amador
- 2019061555 - Miguel Sánchez Sánchez

## Tabla de Contenidos
- [Instalación de requisitos](#instalación-de-requisitos)
- [¿Cómo compilar y ejecutar?](#compilar-y-ejecutar)
- [Estructuras implementadas](#estructuras-implementadas)
    - [Semáforo](#semáforo)
    - [Barrera](#barrera)
    - [Monitor](#monitor)


## Instalación de requisitos

A continuación se enlistan los comandos necesarios para instalar los requisitos para ejecutar este proyecto.

- Fedora 39 (Workstation)
    ```bash
    sudo dnf install gcc
    ```
- Ubuntu
    ```bash
    sudo dnf install gcc
    ```

## Compilar y ejecutar

A continuación se enlistan los comandos necesarios para ejecutar este proyecto.

1. Acceder a la carpeta `2. Biblioteca Sync`
    ```bash
    cd Tareas\ programadas/2.\ Biblioteca\ Sync/
    ```
2. Compilar el archivo `<file_name>.c` y ejecutar el archivo `<output_name>`
    ```bash
    gcc <file_name>.c -o <output_name> && ./<output_name>
    ```
**Ejemplo de uso:**
```bash
cd Tareas\ programadas/2.\ Biblioteca\ Sync/
gcc sync_test.c -o test_sync && ./test_sync
```

# Estructuras implementadas

A continuación se detalla cada estructura de sincronización implementada, así como sus funciones asociadas y ejemplos de uso.

## Queue
Una cola es una colección de elementos donde el principio de inserción (enqueue) es diferente al principio de extracción (dequeue). Es decir, el primer elemento que se inserta en la cola es el primero en ser eliminado (FIFO: First In, First Out).

```C
typedef struct Node {
    pid_t pid;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} Queue;
```

**Atributos:**
- `pid:` id del proceso
- `next:` nodo al que apunta un nodo.
- `head:` inicio de la cola.
- `tail:` final de la cola.

### Funciones implementadas
Para esta tarea fue necesario implementar una cola para llevar el control de los procesos, para ello se implementaron los siguientes métodos, los cuáles puede ver en detalle en el archivo: [queue.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/structs/queue.c)
```C
Queue* createQueue();
void enqueue(Queue *queue, pid_t data);
pid_t dequeue(Queue *queue);
int isEmpty(Queue *queue);
```

### Ejemplo de uso

```C
// Inicializar la cola
Queue *queue = createQueue();

// Agregar elementos
enqueue(queue, 10);
enqueue(queue, 20);
enqueue(queue, 30);

// Vaciar la cola
while (!isEmpty(queue)) {
    dequeue(queue);
}
```


## Semáforo

La función principal de un semáforo es permitir que un número determinado de hilos o procesos accedan simultáneamente a un recurso compartido. Por ejemplo, si un semáforo tiene un valor inicial de 5, significa que hasta cinco hilos pueden acceder al recurso al mismo tiempo. Si todos los hilos intentan acceder al recurso y el semáforo alcanza un valor de 0, cualquier hilo adicional que intente acceder al recurso se bloqueará hasta que otro hilo libere el recurso y aumente el valor del semáforo.


```C
typedef struct {
    int count;
    Queue *queue;
    Queue *blocked_queue;
} Semaphore;
```

**Atributos:**
- `count:` tamaño del semáforo, es decir, la cantidad de hilos que puede antender.
- `queue:` cola de hilos que están siendo atendidos.
- `blocked_queue:` cola de hilos que están en espera.


### Funciones implementadas
- `acquire_semaphore(): ` Valida si hay campo en la cola de atendidos, de haber espacio encola el hilo solicitante, si no caso, lo encola en la lista de los que están en espera.
    ```C
    void acquire_semaphore(Semaphore *sem, pid_t pid) {
        if(sem->count > 0){
            sem->count--;
            enqueue(sem->queue, pid);
        }
        else {
            enqueue(sem->blocked_queue, pid);
        }
    }
    ```
- `release_semaphore(): ` Valida que no hay campo en la cola de atendidos, cuando sucede suelta al primero de la cola atendida e inmediatamente encola al siguiente de la cola de espera.
    ```C
    void release_semaphore(Semaphore *sem) {
        if(sem->count == 0){
            sem->count++;
            if(!isEmpty(sem->blocked_queue)){
                pid_t first_blocked = dequeue(sem->blocked_queue);
                acquire_semaphore(sem, first_blocked);
            }
        }
    }
    ```
> Nota: Para ver en detalle las funciones visite el archivo: [semaphore.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/structs/semaphore.c)

### Ejemplo de uso
El siguiente ejemplo de uso es parte del archivo de pruebas de la biblioteca, para ver el ejemplo en detalle no dude en visitar el archivo: [sync_test.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/sync_test.c)
```C
Semaphore * sem;

void *acquire_sem(void *arg) {
    pid_t pid = *(pid_t *)arg;
    acquire_semaphore(sem, pid);
    sleep((rand() % (3 - 1 + 1)) + 1);
    release_semaphore(sem);
    pthread_exit(NULL);
}

int main(){
    // Inicializar el semáforo
    int slots = 3;
    sem = create_semaphore(slots);

    // Hilos por atender
    int attends = slots*3;
    pid_t tids[attends];
    pthread_t threads[attends];
   
    // Crear hilos
    for (int i = 0; i < attends; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, acquire_sem, &tids[i]);
    }
    // Esperar a que terminen
    for (int i = 0; i < attends; i++) {
        pthread_join(threads[i], NULL);
    }
    
    exit(EXIT_SUCCESS);
}
```



## Barrera
texto

## Monitor
texto
