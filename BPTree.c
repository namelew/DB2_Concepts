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
    int size;
}Leafs;

typedef struct node{
    int size;
    int *refs;
    Leafs **leafs;
    struct node **nodes;
}Node;

Leafs *initLeafs(){
    Leafs *new = malloc(sizeof(Leafs));
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
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
    list->size++;
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

Node *initBPTree(){
    Node *root = malloc(sizeof(Node));
    root->leafs = (Leafs **)malloc(sizeof(Leafs) * (K - 1));
    root->nodes = NULL;
    for(int i = 0; i < K - 1; i++)
        root->leafs[i] = initLeafs();
    root->refs = malloc(sizeof(int) * (K - 1));
    root->size = 0;

    return root;
}

int main(){
    Node *root = initBPTree();
    return 0;
}