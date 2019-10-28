#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int particion(int *array, int inicio, int final){
    int i = inicio;
    int j = final;
    int pivote = array[(inicio+final)/2];
    while (1) {
        while (array[i] < pivote) {
            i++;
        }
        while (array[j] > pivote) {
            j--;
        }
        if(i >= j){
            return j;
        }
        else{
            int temp;
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

}
void qsortt(int *array, int inicio, int final){
    if(inicio < final){
        int p = particion(array, inicio, final);
        qsortt(array, inicio, p);
        qsortt(array, p+1, final);
    }
}
void quicksort(int *array, int n){
    qsortt(array, 0, n-1);
}
typedef unsigned int uint;
int main()
{
    srand((uint) time(NULL));
    int n = 15;
    int array[n];
    for(int i = 0; i < n; i++){
        array[i] = rand() % 50;
        printf("%d ", array[i]);
    }
    printf("\n");
    quicksort(array, n);
    for(int i = 0; i < n; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
