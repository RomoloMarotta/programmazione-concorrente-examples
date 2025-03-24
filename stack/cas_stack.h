#ifndef SPIN_STACK_H
#define SPIN_STACK_H

#include "common_stack.h"

// Funzione per inserire un elemento nello stack (push)
static void push(Node** top, int data) {
    Node* newNode = createNode(data);
    Node* old_val = NULL;
    do{
        old_val = *top;
        newNode->next = old_val;
    }while(!__sync_bool_compare_and_swap(top, old_val, newNode));
}

// Funzione per rimuovere un elemento dallo stack (pop)
static int pop(Node** top) {
    Node* old_val = NULL;
    int poppedData = -1;


    do{
        old_val = *top;
        if (old_val == NULL) goto out;
    }while(!__sync_bool_compare_and_swap(top, old_val, old_val->next));
    poppedData = old_val->data;
out:
    return poppedData;

}

#endif // SPIN_STACK_H
