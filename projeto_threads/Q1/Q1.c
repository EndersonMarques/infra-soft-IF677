#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 3  // Número de garçons robôs
#define M 5  // Número de mesas
#define Q 10 // Tamanho máximo da fila de pedidos

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

int fila_pedidos[Q];
int frente_fila = 0;
int fim_fila = 0;
int num_pedidos = 0;
int qtd_pedidos = 0;

// o argumento id precisa ser void
void *garcom_robo(void *arg) {
    // transforma o argumento para inteiro
    int id = *(int *)arg;

    while (1) {
        // trava o acesso de outras threads
        pthread_mutex_lock(&mutex);

        // se nao houver pedidos, essa thread fica dormindo
        while (num_pedidos == 0) {
            printf("Garçom Robô %d: Aguardando pedidos.\n", id);
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        int pedido = fila_pedidos[frente_fila];
        frente_fila = (frente_fila + 1) % Q;
        num_pedidos--;

        printf("Garçom Robô %d: Atendendo pedido %d.\n", id, pedido);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_producer);

        sleep(2); // Simula o tempo de atendimento

        printf("Garçom Robô %d: Pedido %d atendido. Retornando para a base.\n", id, pedido);
    }

    return NULL;
}

void *dispositivo_mesa(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // se o numero de pedidos for 10, essa thread fica dormindo, pois a quantidade de pedidos está cheia
        while (num_pedidos == Q) {
            printf("Dispositivo da Mesa: Fila de pedidos cheia. Aguardando espaço.\n");
            pthread_cond_wait(&cond_producer, &mutex);
        }

        ++qtd_pedidos;
        int pedido = qtd_pedidos; // Gera um pedido aleatório
        printf("Dispositivo da Mesa: Novo pedido %d adicionado à fila.\n", pedido);

        fila_pedidos[fim_fila] = pedido;
        fim_fila = (fim_fila + 1) % Q;
        num_pedidos++;

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_consumer);

        sleep(1); // Simula o tempo entre os pedidos
    }

    return NULL;
}

int main() {
    pthread_t garcons[N];
    pthread_t mesa;

    int garcon_ids[N];

    for (int i = 0; i < N; i++) {
        garcon_ids[i] = i + 1;
        // Cria uma thread pra cada garçom
        pthread_create(&garcons[i], NULL, garcom_robo, (void *)&garcon_ids[i]);
    }

    pthread_create(&mesa, NULL, dispositivo_mesa, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(garcons[i], NULL);
    }

    pthread_join(mesa, NULL);

    return 0;
}