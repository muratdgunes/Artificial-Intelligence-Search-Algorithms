#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int count_no_seat(int arr[], int size);
float percentage_aisle(int arr[], int size);
int main() {
    srand(time(NULL));
    int variable;
    printf("Enter the number of students: ");
    scanf("%d", &variable);
    int arr[variable];
    printf("Seat distribution: ");
    for(int i = 0; i < variable; i++){
        arr[variable] = rand() % 3 + 1;
        printf("%d ",arr[variable]);
    }
    printf("\nNumber of students without a seat: %d", count_no_seat(arr, variable));
    printf("\nPercentage of aisle seats: %.2f", percentage_aisle(arr, variable));
    return 0;
}
int count_no_seat(int arr[], int size){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(arr[i] == 3){
            count++;
        }
    }
    return count;
}
float percentage_aisle(int arr[], int size){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(arr[i] == 2){
            count++;
        }
    }
    return ((float)count / size) * 100;
}
