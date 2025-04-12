
#include <stdio.h>
#include <unistd.h>

void bubble_sort(int* arr, size_t n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - i - 1; j++){
            if(arr[j] > arr[j + 1]){
                int aux = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = aux;
            }
        }
    }
}

void binary_search(int* arr, size_t n){
    int x = 5;
    int l = 0;
    int r = n - 1;
    int m = 0;
    while(l <= r){
        m = l + (r - l) / 2;
        if(arr[m] == x){
            printf("Element found at index %d\n", m);
            return;
        }
        if(arr[m] < x){
            l = m + 1;
        }
        else{
            r = m - 1;
        }
    }
    printf("Element not found\n");
}

void print_array(int* arr, size_t n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}
int arr[] = {1, 5, 4, 10, 20, 6, 7, 8, 9};

int main(){

    pid_t t = fork();

    if (t < 0){
        printf("Error\n");
    }
    else if (t == 0){
        binary_search(arr, sizeof(arr) / sizeof(arr[0]));
        printf("Pid: %d\n", getpid());
    }
    else{
        bubble_sort(arr, sizeof(arr) / sizeof(arr[0])); 
        print_array(arr, sizeof(arr) / sizeof(arr[0]));
        printf("Pid: %d\n", getpid());
    }
    return 0;
}