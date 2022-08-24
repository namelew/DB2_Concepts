#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
    struct node *prev;
}Node;

typedef struct{
    Node *top;
}Stack;

typedef struct{
    Node *head;
    Node *tail;
}List;

int **createSet(int size){
    int **set = malloc(sizeof(int *) * size);

    for(int i = 0; i < size; i++)
        set[i] = calloc(size, sizeof(int));
    return set;
}

Stack *initStack(){
    Stack *stack = malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

List *initList(){
    List *new = malloc(sizeof(List));
    new->head = NULL;
    new->tail = NULL;

    return new;
}

Node *createNode(int data){
    Node *new = malloc(sizeof(Node));
    new->data = data;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void push(Stack *stack, Node *new){
    if(!stack->top){
        stack->top = new;
        return;
    }
    new->next = stack->top;
    stack->top = new;
}

int pull(Stack *stack){
    if(!stack->top)
        return -1;
    int data;
    Node *aux = stack->top;

    stack->top = stack->top->next;

    data = aux->data;

    free(aux);

    return data;
}

void insertList(List *list, Node *new){
    if(!list->head){
        list->head = new;
        list->tail = new;
        return;
    }

    list->tail->next = new;
    new->prev = list->tail;
    list->tail = new;
}

Node *isInList(List *list, int key){
    for(Node *aux = list->head; aux != NULL; aux = aux->next)
        if(key == aux->data)
            return aux;
    return NULL;
}

int deleteNodeList(List *list, int key){
    if(!list->head && !list->tail)
        return 0;

    Node *aux = isInList(list, key);

    if(!aux)
        return 0;

    if(aux == list->head && aux == list->tail){
        list->head = NULL;
        list->tail = NULL;
        free(aux);
        return 1;
    }

    if(aux == list->head){
        list->head = list->head->next;
        aux->next->prev = aux->prev;
        free(aux);

        return 1;
    }

    if(aux == list->tail){
        list->tail = list->tail->prev;
        aux->prev->next = aux->next;
        free(aux);

        return 1;
    }


    aux->prev->next = aux->next;
    aux->next->prev = aux->prev;

    free(aux);

    return 1;
}

void printList(List *list, int bl){
    for(Node *aux = list->head; aux != NULL; aux=aux->next)
        printf("%d ", aux->data);
    if(bl) printf("\n");
}

void freeList(List *list){
    Node *aux = list->head;

    while(aux != NULL){
        Node *temp = aux;
        aux = aux->next;
        free(temp);
    }
    free(list);
}

void exploreSampleSet(int **set, List *solution, List *unvisiteds, int start, int size){
    List *temp = initList();

    for(int i = 0; i < size; i++){
        if(set[start][i] && !isInList(solution, i+1)){
            deleteNodeList(unvisiteds, i);
            insertList(solution, createNode(i+1));
            insertList(temp, createNode(i));
        }
    }

    if(!isInList(solution, start+1)){
        deleteNodeList(unvisiteds, start);
        insertList(solution, createNode(start+1));
    }

    for(Node *aux = temp->head; aux != NULL; aux = aux->next){
        exploreSampleSet(set, solution, unvisiteds, aux->data, size);
    }

    freeList(temp);
}

int main(int argc, char const *argv[]){
    int sample, n, start;
    int **set;
    List *solution = initList();
    Stack *stack = initStack();
    List *unvisiteds = initList();

    printf("Conjunto: ");
    scanf("%d", &sample);

    set = createSet(sample);

    printf("Numero de subconjuntos: ");
    scanf("%d", &n);
    getchar();

    for(int i = 0; i < n; i++){
        char data;
        printf("Ciclo %d: ", i + 1);
        
        data = getchar();

        while(data != '\n'){
            if(data != ' '){
                int temp = (int)data - 49;
                push(stack, createNode(temp));

                if(!isInList(unvisiteds, temp)) insertList(unvisiteds, createNode(temp));
            }
            data = getchar();
        }

        int current = pull(stack);
        int next = pull(stack);
        start = current;

        while(next != -1){
            set[current][next]++;
            set[next][current]++;

            current = next;
            next = pull(stack);
        }
    }

    free(stack);

    exploreSampleSet(set, solution, unvisiteds, start, sample);

    printList(solution, 0);

    while(unvisiteds->head){
        printf("| ");
        freeList(solution);
        solution = initList();
        exploreSampleSet(set, solution, unvisiteds, unvisiteds->head->data, sample);
        printList(solution, 0);
    }
    printf("\n");

    for(int i = 0; i < sample; i++)
        free(set[i]);
    free(set);

    return 0;
}