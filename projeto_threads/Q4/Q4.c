#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE 1017, 24, 1, 8,  15

// Tamanho da matriz (ordem do quadrado mágico)
#define N 10

int matrix[N][N] = {
    {92, 99, 1,   8,  15, 67, 74, 51, 58, 40},
    {98, 80, 7,   14, 16, 73, 55, 57, 64, 41},
    {4,  81, 88,  20, 22, 54, 56, 63, 70, 47},
    {85, 87, 19,  21, 3,  60, 62, 69, 71, 28},
    {86, 93, 25,  2,  9,  61, 68, 75, 52, 34},
    {17, 24, 76,  83, 90, 42, 49, 26, 33, 65},
    {23, 5,  82,  89, 91, 48, 30, 32, 39, 66},
    {79, 6,  13,  95, 97, 29, 31, 38, 45, 72},
    {10, 12, 94,  96, 78, 35, 37, 44, 46, 53},
    {11, 18, 100, 77, 84, 36, 43, 50, 27, 59}
};

int row_sum[N] = {0}; // Soma de cada linha
int col_sum[N] = {0}; // Soma de cada coluna
int sum_main_diag = 0; // Soma da diagonal principal
int sum_secondary_diag = 0; // Soma da diagonal secundária
int is_magic_square = 1; // Variável para verificar se é um quadrado mágico

void *check_rows(void *arg) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            row_sum[i] += matrix[i][j];
        }
    }

    pthread_exit(NULL);
}

void *check_columns(void *arg) {
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            col_sum[j] += matrix[i][j];
        }
    }

    pthread_exit(NULL);
}

void *check_diagonals(void *arg) {
    for (int i = 0; i < N; i++) {
        sum_main_diag += matrix[i][i];
        sum_secondary_diag += matrix[i][N - i - 1];
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t thread_rows, thread_columns, thread_diagonals;

    pthread_create(&thread_rows, NULL, check_rows, NULL);
    pthread_create(&thread_columns, NULL, check_columns, NULL);
    pthread_create(&thread_diagonals, NULL, check_diagonals, NULL);

    pthread_join(thread_rows, NULL);
    pthread_join(thread_columns, NULL);
    pthread_join(thread_diagonals, NULL);

    // Verifica se todas as somas são iguais
    int reference_sum = row_sum[0];

    for (int i = 0; i < N; i++) {
        if (row_sum[i] != reference_sum || col_sum[i] != reference_sum) {
            is_magic_square = 0;
            break;
        }
    }

    if (sum_main_diag != reference_sum || sum_secondary_diag != reference_sum) {
        is_magic_square = 0;
    }

    if (is_magic_square) {
        printf("A matriz é um quadrado mágico.\n");
    } else {
        printf("A matriz NÃO é um quadrado mágico.\n");
    }

    return 0;
}

