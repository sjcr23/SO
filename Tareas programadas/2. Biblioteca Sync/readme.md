# Biblioteca Sync

Esta biblioteca hace uso de Hilos, Mutex y los operadores Wait/Notify para  incorporar las estructuras de sincronización: Semáforo, Barrera y Monitor.

## Desarollado por
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
- `pid` : id del proceso
- `next` : nodo al que apunta un nodo.
- `head` : inicio de la cola.
- `tail` : final de la cola.

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
- `count` : tamaño del semáforo, es decir, la cantidad de hilos que puede antender.
- `queue` : cola de hilos que están siendo atendidos.
- `blocked_queue` : cola de hilos que están en espera.


### Funciones implementadas
- `acquire_semaphore()` : Valida si hay campo en la cola de atendidos, de haber espacio encola el hilo solicitante, si no caso, lo encola en la lista de los que están en espera.
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
- `release_semaphore()` : Valida que no hay campo en la cola de atendidos, cuando sucede suelta al primero de la cola atendida e inmediatamente encola al siguiente de la cola de espera.
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
Una estructura de barrera es una herramienta de sincronización, la idea básica de una barrera es permitir que un grupo de hilos o procesos se ejecuten de manera independiente hasta cierto punto en su ejecución, y luego los detiene hasta que todos lleguen a una "barrera" designada. Una vez que todos los hilos o procesos han llegado a la barrera, se liberan todos al mismo tiempo, lo que les permite continuar ejecutándose.

```C
typedef struct {
    int count;
    int max_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Barrier;
```

**Atributos:**
- `count` : Número actual de hilos o procesos que han llegado a la barrera. Se incrementa cada vez que un hilo o proceso llega a la barrera. Inicialmente, cuando la barrera se crea, este valor suele establecerse en 0.

- `max_count` : Tamaño máximo esperado de la barrera, es decir, el número total de hilos o procesos que se esperan que lleguen antes de que la barrera se abra y permita que todos continúen su ejecución.

- `mutex` : Mutex utilizado para garantizar que las operaciones en la barrera, como la actualización del contador (count), sean seguras y no estén sujetas a condiciones de carrera. El mutex se bloquea antes de acceder o modificar los datos de la barrera y se desbloquea después de que se completan estas operaciones.

- `cond` : Variable de condición utilizada para bloquear y desbloquear los hilos que esperan en la barrera. Cuando un hilo llega a la barrera y descubre que no es el último, se bloquea en esta variable de condición hasta que todos los hilos hayan llegado y la barrera se abra. La variable de condición se utiliza junto con el mutex para realizar la espera y la señalización de manera segura.

### Funciones implementadas
- `barrier_wait()` : Asegura que los procesos se sincronicen en un punto de barrera, esperando a que todos los procesos hayan llegado antes de continuar. Una vez que todos los procesos han llegado, se les permite continuar su ejecución en orden controlado.
    ```C
    void barrier_wait(Barrier *bar, pid_t pid) {
        // Siempre admite
        pthread_mutex_lock(&bar->mutex);
        bar->count++;
        
        // Cuando está lleno avisa
        if (bar->count >= bar->max_count) {
            pthread_cond_broadcast(&bar->cond);
        }
        // Manda a esperar cuando supera el límite
        else if (bar->count < bar->max_count && bar->count > 0) {
            pthread_cond_wait(&bar->cond, &bar->mutex);
        }
        // Los suelta mientras haya
        if(bar->count > 0){
            bar->count--;
        }
        pthread_mutex_unlock(&bar->mutex);
    }
    ```
> Nota: Es necesario implementar una espera con el método `sleep()` cada vez que un hilo adquiera o suelte el recurso. Para ver en detalle las funciones visite el archivo: [barrier.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/structs/barrier.c)

### Ejemplo de uso
El siguiente ejemplo de uso es parte del archivo de pruebas de la biblioteca, para ver el ejemplo en detalle no dude en visitar el archivo: [sync_test.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/sync_test.c)
```C
Barrier *bar;

void *acquire_barr(void *arg) {
    pid_t pid = *(pid_t *)arg;
    barrier_wait(bar, pid);
    pthread_exit(NULL);
}

void main(){
    int NUM_THREADS = 5;
    bar = create_barrier(NUM_THREADS);

    int attends = NUM_THREADS*2;
    int tids[attends];
    pthread_t threads[attends];
    
    // Crear hilos e IDs
    for (int i = 0; i < attends; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, acquire_barr, &tids[i]);
    }
    // Esperar a que terminen
    for (int i = 0; i < attends; i++) {
        pthread_join(threads[i], NULL);
    }
    exit(EXIT_SUCCESS);
}
```

## Monitor

Un monitor es una estructura de programación concurrente utilizada para coordinar el acceso a recursos compartidos entre múltiples hilos o procesos de manera segura y sincronizada. La idea principal detrás de un monitor es que el acceso a los datos compartidos y la lógica de sincronización están encapsulados dentro de un solo objeto o estructura. Esto facilita la implementación de la exclusión mutua y la sincronización entre múltiples hilos o procesos, ya que todos los accesos a los datos compartidos deben pasar a través de métodos o funciones proporcionadas por el monitor.

```C
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Monitor;
```

**Atributos:**
- `buffer[BUFFER_SIZE]` : Es un arreglo de enteros que representa el buffer compartido. Este es el área de memoria donde se almacenan los elementos que los productores producen y los consumidores consumen.

- `count` : Entero que indica el número de elementos actualmente en el buffer. Permite realizar un seguimiento del número de elementos presentes en el buffer en un momento dado.

- `in` : Entero que indica la posición del índice de inserción en el buffer. Se utiliza para rastrear la próxima posición disponible para insertar un nuevo elemento en el buffer.

- `out` : Entero que indica la posición del índice de extracción en el buffer. Se utiliza para rastrear la próxima posición desde la cual extraer un elemento del buffer.

- `mutex` : Mutex que se utiliza para lograr exclusión mutua en el acceso a los datos compartidos del monitor, como buffer, count, in y out. Garantiza que solo un hilo pueda acceder a estos datos compartidos a la vez, evitando condiciones de carrera.

- `not_full` : Variable de condición que se utiliza para señalar a los productores que el buffer no está lleno y que pueden continuar produciendo elementos. Los productores esperan en esta variable de condición cuando el buffer está lleno.

- `not_empty` : Variable de condición que se utiliza para señalar a los consumidores que el buffer no está vacío y que pueden continuar consumiendo elementos. Los consumidores esperan en esta variable de condición cuando el buffer está vacío.

### Funciones implementadas
- `produce()` : Agrega un dato al buffer de un monitor de forma segura, utilizando exclusión mutua y variables de condición para garantizar que la producción de datos sea sincronizada y que no se produzcan condiciones de carrera.
    ```C
    void produce(Monitor *mon, int data) {
        pthread_mutex_lock(&mon->mutex);
        
        // Mientras el buffer esté lleno, esperamos
        while (mon->count == BUFFER_SIZE) { 
            pthread_cond_wait(&mon->not_full, &mon->mutex);
        }

        // Insertamos el dato en el buffer
        mon->buffer[mon->in] = data; 

        // Actualizamos el índice de inserción
        mon->in = (mon->in + 1) % BUFFER_SIZE; 
        
        // Incremento del contador de elementos
        mon->count++;
        
        // Señalizamos que el buffer no está vacío
        pthread_cond_signal(&mon->not_empty); 
        pthread_mutex_unlock(&mon->mutex);
    }
    ```
- `consume()` : Extrae un dato del buffer de un monitor de forma segura, utilizando exclusión mutua y variables de condición para garantizar que el consumo de datos sea sincronizado y que no se produzcan condiciones de carrera.
    ```C
    int consume(Monitor *mon) {
        int data;
        pthread_mutex_lock(&mon->mutex);

        // Mientras el buffer esté vacío, esperamos
        while (mon->count == 0) { 
            pthread_cond_wait(&mon->not_empty, &mon->mutex);
        }

        // Extraemos el dato del buffer
        data = mon->buffer[mon->out]; 

        // Actualizamos el índice de extracción
        mon->out = (mon->out + 1) % BUFFER_SIZE; 

        // Disminuimos el contador de elementos en el buffer
        mon->count--; 
        
        // Señalizamos que el buffer no está lleno
        pthread_cond_signal(&mon->not_full); 
        pthread_mutex_unlock(&mon->mutex);
        return data;
    }
    ```

> Nota: Para ver en detalle las funciones visite el archivo: [monitor.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/structs/monitor.c)

### Ejemplo de uso
El siguiente ejemplo de uso es parte del archivo de pruebas de la biblioteca, para ver el ejemplo en detalle no dude en visitar el archivo: [sync_test.c](https://github.com/sjcr23/SO/blob/main/Tareas%20programadas/2.%20Biblioteca%20Sync/sync_test.c)
```C
Monitor *mon;

// Función de hilo para el productor
void *producer_thread(void *arg) {
    // Produce datos en el monitor
    for (int i = 0; i < DATA_SIZE; i++) {
        produce(mon, i);
        // Imprime el dato producido
        printf("• [PRODUCED %d]\n", i);
    }
    pthread_exit(NULL);
}

// Función de hilo para el consumidor
void *consumer_thread(void *arg) {
    // Consume datos del monitor
    for (int i = 0; i < DATA_SIZE; i++) {
        int data = consume(mon);
        // Imprime el dato consumido
        printf("- [CONSUMED %d]\n", data);
    }
    pthread_exit(NULL);
}

void main(){
    pthread_t producer, consumer;
    mon = create_monitor();

    // Crear hilos para productor y consumidor
    pthread_create(&producer, NULL, producer_thread, NULL);
    pthread_create(&consumer, NULL, consumer_thread, NULL);

    // Esperar a que los hilos terminen
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    exit(EXIT_SUCCESS);
}
```