#ifndef SERIAL_STACK_H
#define SERIAL_STACK_H

#include "common_stack.h"

// Funzione per inserire un elemento nello stack (push)
static  void push(Node** top, int data) {
    Node* newNode = createNode(data);
    newNode->next = *top;
    *top = newNode;
}

// Funzione per rimuovere un elemento dallo stack (pop)
static int pop(Node** top) {
    if (*top == NULL) {
        return -1;
    }
    Node* temp = *top;
    int poppedData = temp->data;
    *top = (*top)->next;
    free(temp);
    return poppedData;
}

#endif // SERIAL_STACK_H
