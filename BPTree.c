#include <stdlib.h>
#include <stdio.h>

#define K 4

typedef struct{
    int value;
}Data;

typedef struct leaf{
    Data data;
    struct leaf *right;
    struct leaf *left;
}Leaf;

typedef struct{
    Leaf *head;
    Leaf *tail;
}Leafs;

typedef struct{
    int size;
    int *refs;
}Nodes;

Leafs *initLeafs(){
    Leafs *new = malloc(sizeof(Leafs));
    new->head = NULL;
    new->tail = NULL;
    return new;
}

Leaf *createLeaf(Data value){
    Leaf *new = malloc(sizeof(Leaf));
    new->data = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void insertLeafAt(Leaf *prev, Leaf *new, Leaf *next){
    new->left = prev;
    new->right = next;

    if(!prev){
        next->left = new;   
        return;
    }
    if(!next){
        prev->right = new;
        return;
    }

    prev->right = new;
    next->left = new;
}

void insertLeaf(Leafs *list, Leaf *new){
    if (!list->head && !list->tail){
        list->head = new;
        list->tail = new;
        return;
    }

    insertLeafAt(list->tail, new, (Leaf *)NULL);
    list->tail = new;
}

void printLeafs(Leafs *list){
    for(Leaf *aux = list->head; aux != NULL; aux = aux->right){
        printf("%d ", aux->data.value);
    }
    printf("\n");
}

int main(){
    return 0;
}