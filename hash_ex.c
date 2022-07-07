#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define K 2
#define ORDER 1

typedef struct{
    int tail;
    int *page;
    int order;
    int reference;
}Buck;

typedef struct{
    int size;
    int order;
    Buck *bucks;
}HashTable;

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int hash(int key, int size){
    return key%size;
}

Buck *createBucks(){
    Buck *bucks = malloc(sizeof(Buck) * pow(K,ORDER));

    for(int i = 0; i < K; i++){
        bucks[i].page = malloc(sizeof(int) * pow(K,ORDER));
        bucks[i].tail = 0;
        bucks[i].order = pow(K,ORDER);
        bucks[i].reference = i;

        for(int j = 0; j < bucks[i].order; j++)
            bucks[i].page[j] = 0;
    }
    return bucks;
}

HashTable *init(){
    HashTable *hashTable = malloc(sizeof(HashTable));
    hashTable->bucks = createBucks();
    hashTable->size = pow(K, ORDER);
    hashTable->order = ORDER; 
    return hashTable;
}

HashTable *rebuildTable(HashTable *hashTable){
    int old = hashTable->size; 
    hashTable->order++;
    hashTable->size = pow(K, hashTable->order);

    Buck *new = malloc(sizeof(Buck) * hashTable->size);

    for(int i = 0; i < old; i++){
        new[i].page = hashTable->bucks[i].page;
        new[i].tail = hashTable->bucks[i].tail;
        new[i].order = hashTable->bucks[i].order;
        new[i].reference = -1;
    }
    int j = 0;
    for(int i = old; i < hashTable->size; i++){
        new[i].page = hashTable->bucks[j].page;
        new[i].tail = hashTable->bucks[j].tail;
        new[i].order = hashTable->bucks[j].order;
        new[i].reference = j;
        j++;
    }

    free(hashTable->bucks);
    hashTable->bucks = new;

    return hashTable;
}

void printHash(HashTable *hashTable){
    for(int i = 0; i < hashTable->size; i++){
        printf("Slot %d: ", i);
        for(int j = 0; j < hashTable->bucks[i].tail; j++){
            printf("%d ", hashTable->bucks[i].page[j]);
        }
        printf("\n");
    }
    printf("---------------\n");
}

void insertValue(HashTable *hashTable,int key){
    int slot = hash(key, hashTable->size);

    if(hashTable->bucks[slot].tail >= pow(K,ORDER)){
        int isCompartilhado = hashTable->bucks[slot].reference == slot ? 0 : 1;

        if(isCompartilhado == 1){
            int temp = -1;
            if (hashTable->bucks[slot].reference == -1){
                for(int i = 0; i < hashTable->size; i++){
                    if(hashTable->bucks[i].reference == slot){
                        temp = slot;
                        hashTable->bucks[slot].reference = slot;
                        slot = i;
                        break;
                    }                    
                }
            }
            hashTable->bucks[slot].tail = 0;
            hashTable->bucks[slot].reference = slot;
            hashTable->bucks[slot].page = malloc(sizeof(int) * pow(K,ORDER));

            for(int i = 0; i < hashTable->bucks[slot].order; i++)
                hashTable->bucks[slot].page[i] = 0;
            
            if(temp != -1){
                hashTable->bucks[temp].tail = 0;
                for(int i = 0; i < hashTable->bucks[temp].order; i++)
                    insertValue(hashTable, hashTable->bucks[temp].page[i]);
            }
            insertValue(hashTable, key);
            return;
        } else{
            hashTable = rebuildTable(hashTable);
            hashTable->bucks[slot].tail = 0;
            for(int i = 0; i < hashTable->bucks[slot].order; i++){
                insertValue(hashTable, hashTable->bucks[slot].page[i]);
            }

            insertValue(hashTable, key);
            return;
        }
    }
    
    hashTable->bucks[slot].page[hashTable->bucks[slot].tail] = key;
    hashTable->bucks[slot].tail++;
}

int removeValue(HashTable *hashTable,int key){
    int slot = hash(key, hashTable->size);

    for(int i = 0; i < hashTable->bucks[slot].tail; i++){
        if(hashTable->bucks[slot].page[i] == key){
            swap(&hashTable->bucks[slot].page[i], &hashTable->bucks[slot].page[hashTable->bucks[slot].tail - 1]);
            hashTable->bucks[slot].tail--;
            return hashTable->bucks[slot].page[hashTable->bucks[slot].tail];
        }
    }
    return -1;
}

int main(){
    HashTable *hashTable = init();

    for(int i = 0; i < 10; i++){
        insertValue(hashTable, i);
        printHash(hashTable);
    }

    removeValue(hashTable, 8);
    removeValue(hashTable, 2);

    printHash(hashTable);
    return 0;
}