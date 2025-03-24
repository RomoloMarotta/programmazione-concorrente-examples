#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "common_stack.h"

pthread_spinlock_t slock;  
pthread_mutex_t mlock;     

#define RAND_R

#if   VERSION==0
#include "serial_stack.h"
#elif VERSION==1
#include "spinlock_stack.h"
#elif VERSION==2
#include "mutex_stack.h"
#elif VERSION==3
#include "cas_stack.h"
#elif VERSION==4
#include "backoff_stack.h"
#elif VERSION==5
#include "cas_impr_stack.h"
#elif VERSION==6
#include "backoff_impr_stack.h"
#endif

// Variabili globali
volatile int stop = 0;
int num_ops = 0;
Node* stack = NULL;


#include "timer.h"

__thread struct drand48_data rand_state;

// Funzione eseguita dal thread
void* thread_function(void* args) {
    srand48_r(time(NULL), &rand_state);
    int operations = 0;
    
    while (!stop) {
        //usleep(1);
        long rand_value;
        long rand_num;
        unsigned long long start = RAW_CLOCK_READ();
        lrand48_r(&rand_state, &rand_value);
        lrand48_r(&rand_state, &rand_num);
        __sync_fetch_and_add(&num_ops, 1);
        while((RAW_CLOCK_READ() - start) < CLOCKS_PER_US);
        if (rand_value % 2 == 0) {
            int value = rand_num % 100;
            push(&stack, value);
        } else {
            pop(&stack);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numero_thread>\n", argv[0]);
        return EXIT_FAILURE;
    }
    pthread_spin_init(&slock, PTHREAD_PROCESS_PRIVATE);
    pthread_mutex_init(&mlock, NULL);
    
    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Il numero di thread deve essere maggiore di zero.\n");
        return EXIT_FAILURE;
    }
    
    pthread_t threads[num_threads];
    
    for (int i = 0; i < 100; i++) 
        push(&stack, i);

    // Creazione dei thread
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }
    
    // Thread A si addormenta per 10 secondi
    sleep(5);
    
    // Dopo il risveglio, A imposta stop = 1
    stop = 1;
    
    // Attende la terminazione di tutti i thread
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Libera la memoria dello stack
    freeStack(&stack);
    printf("%f\n", num_ops / (10 * 1000000.0));
    
    return 0;
}