#ifndef SPIN_STACK_H
#define SPIN_STACK_H

#include "common_stack.h"

#ifdef RAND_R
extern __thread struct drand48_data rand_state;
#endif

void backoff(int attempt, long base_time, long max_time) {
    if(attempt  <= 0) return;
    long delay = base_time * (1 << (attempt - 1)); // base_time * 2^(attempt-1)
    delay = delay < 2 ? 2 : delay;
    long rand_num;
#ifdef RAND_R
//    printf("A HERE %ld %ld\n", rand_num, delay);
    lrand48_r(&rand_state, &rand_num);
//    printf("B HERE %ld %ld\n", rand_num, delay);
#else
    rand_num = rand();
#endif
    long jitter = rand_num % (delay / 2);
    delay = delay + jitter;
    if (delay > max_time)  delay = max_time;
    usleep(delay);
}

// Funzione per inserire un elemento nello stack (push)
static void push(Node** top, int data) {
    Node* newNode = createNode(data);
    Node* old_val = NULL;
    int attempt = 0;
    do{
        backoff(attempt++, 1, 10*1000);
        old_val = *top;
        newNode->next = old_val;
    }while(!__sync_bool_compare_and_swap(top, old_val, newNode));
}

// Funzione per rimuovere un elemento dallo stack (pop)
static int pop(Node** top) {
    Node* old_val = NULL;
    int poppedData = -1;

    int attempt = 0;

    do{
        backoff(attempt++, 1, 10*1000);
        old_val = *top;
        if (old_val == NULL) goto out;
    }while(!__sync_bool_compare_and_swap(top, old_val, old_val->next));
    poppedData = old_val->data;
out:
    return poppedData;

}

#endif // SPIN_STACK_H
