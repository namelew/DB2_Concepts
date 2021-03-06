#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define N 10

// métodos avulsos
int *generateSample(int type){
    int *array = malloc(sizeof(int) * N);
    switch (type)
    {
    case 0:;
        for(int i = 0; i < N; i++)
            array[i] = i;
        return array;
    case 1:;
        int value = N - 1;
        for(int i = 0; i < N; i++){
            array[i] = value;
            value--;
        }
        return array;
    case 2:;
        srand(getpid());
        for(int i = 0; i < N; i++)
            array[i] = rand()%N;
        return array;
    default:
        return NULL;
    }
}

void printArray(int *array){
    for(int i = 0; i < N; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int isSorted(int *array){
    for(int i = 0; i < N - 1; i++)
        if(array[i] > array[i+1])
            return 0;
    return 1;
}

int getMax(int *array){
    int greater = array[0];

    for(int i = 1; i < N; i++)
        if (array[i] > greater)
            greater = array[i];
    
    return greater;
}

// funções auxiliares
int partition(int *array, int start, int end)
{
    int pivo = array[end];
    int k = start;
 
    for (int j = start; j <= end - 1; j++)
        if (array[j] < pivo)    
            swap(&array[k++], &array[j]);
    
    swap(&array[k], &array[end]);
    return k;
}

int partition_rp(int *array, int start, int end){
    int rp = start + rand() % (end - start);

    swap(&array[rp], &array[end]);

    return partition(array, start, end);
}

void merge(int *array, int start, int mid, int end){
    int *aux = malloc(sizeof(int) * N);
    int left = start;
    int right = mid + 1;
    int k = 0;

    while(left <= mid && right <= end){
        if(array[left] <= array[right])
            aux[k] = array[left++];
        else
            aux[k] = array[right++];
        k++;
    }

    while(left <= mid)
        aux[k++] = array[left++];

    while(right <= end)
        aux[k++] = array[right++];

    for(k = start; k <= end; k++)
        array[k] = aux[k - start];
    
    free(aux);
}

void heaptify(int *array, int position, int tam){
    int greater = position;
    int left = position * 2 + 1;
    int right = position * 2 + 2;

    if (left < tam && array[left] > array[position])
        greater = left;
    if (right < tam && array[right] > array[greater])
        greater = right;
    
    if (greater != position){
        swap(&array[position], &array[greater]);
        heaptify(array, greater, tam);
    }

}

void countingSortRadix(int *array, int pos){
    int *aux = malloc(sizeof(int) * N);
    int *counting = calloc(10, sizeof(int));

    for(int i = 0; i < N; i++)
        counting[(array[i]/pos)%10]++;
    
    for(int i = 1; i < 10; i++)
        counting[i] += counting[i-1];
    
    for(int i = N - 1; i >= 0; i--){
        counting[(array[i]/pos)%10]--;
        aux[counting[(array[i]/pos)%10]] = array[i];
    }

    for(int i = 0; i < N; i++)
        array[i] = aux[i];
}

// métodos quadráticos 0(n^2)
void bubbleSort(int *array){
    int i,j;

    for(i = N - 1; i >= 0; i--){
        for(j = 0; j < i; j++){
            if(array[j] > array[j + 1])
                swap(&array[j], &array[j + 1]);
        }
    }
}

void insertionSort(int *array){
    int i,j,p;

    for(i = 1; i < N;i++){
        p = array[i];
        for(j = i; j >= 1 && p < array[j-1]; j--)
            array[j] = array[j - 1];
        array[j] = p;
    }
}

void selectionSort(int *array){
    int i,j, menor;

    for(i = 0; i < N; i++){
        menor = i;
        for(j = i; j < N; j++){
            if (array[j] < array[menor]){
                menor = j;
            }
        }
        swap(&array[i], &array[menor]);
    }
}

// métodos log lineares 0(n log n)
void quickSort(int *array, int start, int end){
    if(end > start){
        int pt = partition_rp(array, start, end);

        quickSort(array, start, pt - 1);
        quickSort(array, pt + 1, end);
    }
}

void mergeSort(int *array, int start, int end){
    if(end > start){
        int mid = (start + end)/2;

        mergeSort(array, start, mid);
        mergeSort(array, mid + 1, end);
        merge(array, start, mid, end);
    }
}

void heapSort(int *array){
    for(int k = N / 2 - 1; k >= 0; k--)
        heaptify(array, k, N);    

    for(int k = N - 1; k >= 1; k--){
        swap(&array[0], &array[k]);
        heaptify(array, 0, k);
    }
}

// métodos derivados de contagem 0(n * k)

void countingSort(int *array){
    int greater = getMax(array);
    int *counting = calloc(greater+1, sizeof(int));
    int *aux = malloc(sizeof(int) * N);
    
    for(int i = 0; i < N; i++)
        counting[array[i]]++;
    
    for(int i = 1; i <= greater; i++)
        counting[i] += counting[i - 1];

    for(int i = N -1; i >= 0; i--){
        counting[array[i]]--;
        aux[counting[array[i]]] = array[i];
    }
    
    for(int i = 0; i < N;i++)
        array[i] = aux[i];
}

void radixSort(int *array){
    int greater = getMax(array);

    for(int pos = 1; greater/pos > 0; pos*=10){
        countingSortRadix(array, pos);
    }
}

int main(int argc, char const *argv[])
{
    int sampleType = 2;
    if (argc > 1){
        sampleType = atoi(argv[1]);
    }
    int *array = generateSample(sampleType);

    radixSort(array);

    printArray(array);

    return 0;
}