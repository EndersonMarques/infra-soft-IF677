#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 8

// int array[SIZE] = {38, 27, 43, 3, 9, 82, 10, 19};
int array[SIZE] = {4, 5, 1, 6, 7, 0, 2, 10};
typedef struct {
    int left;
    int right;
} Range;

void merge(int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void *merge_sort(void *arg) {
    Range *range = (Range *)arg;
    int left = range->left;
    int right = range->right;

    if (left < right) {
        int mid = left + (right - left) / 2;

        // Criando duas threads para resolver as partes esquerda e direita em paralelo
        pthread_t thread_left, thread_right;

        Range range_left = {left, mid};
        Range range_right = {mid + 1, right};

        pthread_create(&thread_left, NULL, merge_sort, &range_left);
        pthread_create(&thread_right, NULL, merge_sort, &range_right);

        pthread_join(thread_left, NULL);
        pthread_join(thread_right, NULL);


        // feito assim porque não tem como garantir qual thread vai terminar primeiro
        merge(left, mid, right);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t main_thread;
    Range initial_range = {0, SIZE - 1};

    pthread_create(&main_thread, NULL, merge_sort, &initial_range);
    pthread_join(main_thread, NULL);

    printf("Vetor ordenado: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}

// [4, 5, 1, 6, 7, 0, 2, 10]
