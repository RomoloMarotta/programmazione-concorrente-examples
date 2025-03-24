#ifndef SPIN_STACK_H
#define SPIN_STACK_H

#include "common_stack.h"

// Funzione per inserire un elemento nello stack (push)
static void push(Node** top, int data) {
    Node* newNode = createNode(data);

    pthread_spin_lock(&slock);
    newNode->next = *top;
    *top = newNode;
    pthread_spin_unlock(&slock);
}

// Funzione per rimuovere un elemento dallo stack (pop)
static int pop(Node** top) {
    int poppedData = -1;

    pthread_spin_lock(&slock);
    if (*top == NULL) goto out;

    Node* temp = *top;
    poppedData = temp->data;
    *top = (*top)->next;
    free(temp);

out:
    pthread_spin_unlock(&slock);
    return poppedData;

}

#endif // SPIN_STACK_H
