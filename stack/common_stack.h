#ifndef COMMON_STACK_H
#define COMMON_STACK_H

#include <stdio.h>
#include <stdlib.h>

// Definizione della struttura per il nodo dello stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Funzione per creare un nuovo nodo
static inline Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Errore di allocazione di memoria\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Funzione per liberare la memoria allocata dello stack
static inline void freeStack(Node** top) {
    while (*top != NULL) {
        Node* temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

#endif // COMMON_STACK_H