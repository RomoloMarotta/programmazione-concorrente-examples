#ifndef SPIN_STACK_H
#define SPIN_STACK_H

#include "common_stack.h"

// Funzione per inserire un elemento nello stack (push)
static void push(Node** top, int data) {
    Node* newNode = createNode(data);

    pthread_mutex_lock(&mlock);
    newNode->next = *top;
    *top = newNode;
    pthread_mutex_unlock(&mlock);
}

// Funzione per rimuovere un elemento dallo stack (pop)
static int pop(Node** top) {
    int poppedData = -1;

    pthread_mutex_lock(&mlock);
    if (*top == NULL) goto out;

    Node* temp = *top;
    poppedData = temp->data;
    *top = (*top)->next;
    free(temp);

out:
    pthread_mutex_unlock(&mlock);
    return poppedData;

}

#endif // SPIN_STACK_H
