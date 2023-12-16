#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_FILES 100
#define MAX_PRODUCTS 100

pthread_mutex_t mutex[MAX_PRODUCTS];
pthread_mutex_t file_selection_mutex = PTHREAD_MUTEX_INITIALIZER;

int product_count[MAX_PRODUCTS] = {0};
int total_products = 0;

typedef struct {
    int total_files;
    int total_products;
} ThreadData;

int arquivos_visitados[MAX_FILES] = {0};

void *count_products(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    while (1) {
        int current_file;

        pthread_mutex_lock(&file_selection_mutex);

        for (current_file = 0; current_file < data->total_files; current_file++) {
            if (arquivos_visitados[current_file] == 0) {
                arquivos_visitados[current_file] = 1;
                break;
            }
        }

        pthread_mutex_unlock(&file_selection_mutex);

        if (current_file >= data->total_files) {
            break; // All files processed
        }

        char filename[10];
        sprintf(filename, "%d.in", current_file + 1);

        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        int product;
        while (fscanf(file, "%d", &product) == 1) {
            pthread_mutex_lock(&mutex[product]);
            product_count[product]++;
            pthread_mutex_unlock(&mutex[product]);
            __sync_fetch_and_add(&total_products, 1);
        }

        fclose(file);
    }

    pthread_exit(NULL);
}

int main() {
    int num_files, num_threads, num_products;

    num_files = 5;
    num_threads = 3;
    num_products = 5;

    if (num_threads > num_files) {
        num_threads = num_files;
        printf("O numero de threads deve ser menor ou igual ao numero de arquivos lidos\n");
        exit(1);
    }

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    for (int i = 0; i < num_products; i++) {
        pthread_mutex_init(&mutex[i], NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].total_files = num_files;
        thread_data[i].total_products = num_products;
        pthread_create(&threads[i], NULL, count_products, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nTotal products: %d\n", total_products);
    printf("Product percentages:\n");

    for (int i = 0; i < num_products; i++) {
        printf("Product %d: %.2f%%\n", i, (float)product_count[i] / total_products * 100);
    }

    for (int i = 0; i < num_products; i++) {
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}
