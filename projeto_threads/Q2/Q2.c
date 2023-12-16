#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_FILES 100
#define MAX_PRODUCTS 100

pthread_mutex_t mutex[MAX_PRODUCTS];
int product_count[MAX_PRODUCTS] = {0};
int total_products = 0;

typedef struct {
    int total_files;
    int total_products;
} ThreadData;

void *count_products(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    while (1) {
        pthread_mutex_lock(&mutex[data->total_files]);
        int current_file = total_products;
        total_products++;
        pthread_mutex_unlock(&mutex[data->total_files]);

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
        }

        fclose(file);
    }

    pthread_exit(NULL);
}

int main() {
    int num_files, num_threads, num_products;

    printf("Enter the number of files (N): ");
    scanf("%d", &num_files);

    printf("Enter the number of threads (T): ");
    scanf("%d", &num_threads);

    printf("Enter the number of products (P): ");
    scanf("%d", &num_products);

    if (num_threads > num_files) {
        num_threads = num_files;
        printf("Adjusting the number of threads to be equal to the number of files.\n");
    }

    pthread_t threads[num_threads];
    ThreadData thread_data = {num_files, num_products};

    for (int i = 0; i < num_products; i++) {
        pthread_mutex_init(&mutex[i], NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, count_products, &thread_data);
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
