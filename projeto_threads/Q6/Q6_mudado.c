Claro, aqui está o código com nomes de variáveis em português:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TAMANHO 3
#define LINHAS 3
#define COLUNAS 3
#define NUM_THREADS 3

int matriz_esparsa1_posicoes[TAMANHO][TAMANHO + 1] = {{0, 1, -1, -1}, {0, 1, 2, -1}, {1, 2, -1, -1}};
float matriz_esparsa1_valores[TAMANHO][TAMANHO] = {{2.0, -1.0}, {-1.0, 2.0, -1.0}, {-1.0, 2.0}};

int matriz_esparsa2_posicoes[TAMANHO][TAMANHO + 1] = {{0, -1, -1, -1}, {2, -1, -1, -1}, {0, 1, 2, -1}};
float matriz_esparsa2_valores[TAMANHO][TAMANHO] = {{3.0}, {5.0}, {2.0, 8.0, 1.0}};

float vetor_denso[TAMANHO] = {5, 8, 3};

float matriz_densa[TAMANHO][TAMANHO] = {{1, 2, 5}, {3, 4, 2}, {5, 6, 1}};

float resultado_esparsa_X_densa[TAMANHO][TAMANHO];
float resultado_esparsa_X_vetor[TAMANHO][TAMANHO];
float resultado_esparsa_X_esparsa[TAMANHO][TAMANHO];

void *codigoThread(void *tid)
{
    int i, j, k, x;
    int idThread = (*(int *)tid);
    int posicoes[TAMANHO + 1];
    float valores[TAMANHO + 1];

    // Multiplicando a matriz_esparsa1 por um vetor denso
    for (i = idThread; i < LINHAS; i = i + NUM_THREADS)
    {
        j = 0;
        resultado_esparsa_X_vetor[i][j] = 0;
        for (k = 0; matriz_esparsa1_posicoes[i][k] != -1; k++)
        {
            // "x" recebe a posição em que deve procurar o elemento correto do "vetor_denso"
            x = matriz_esparsa1_posicoes[i][k];
            resultado_esparsa_X_vetor[i][j] += matriz_esparsa1_valores[i][k] * vetor_denso[x];
        }
    }

    // Multiplicando uma matriz_esparsa1 pela matriz_esparsa2;
    for (i = idThread; i < LINHAS; i = i + NUM_THREADS)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            resultado_esparsa_X_esparsa[i][j] = 0;
            for (k = 0; matriz_esparsa1_posicoes[i][k] != -1; k++)
            {
                // "x" recebe a posição em que deve procurar o elemento correto da matriz_esparsa2
                x = matriz_esparsa1_posicoes[i][k];

                for (int w = 0; matriz_esparsa2_posicoes[x][w] != -1; w++)
                {
                    if (matriz_esparsa2_posicoes[x][w] == j)
                    {
                        resultado_esparsa_X_esparsa[i][j] += matriz_esparsa1_valores[i][k] * matriz_esparsa2_valores[x][w];
                    }
                }
            }
        }
    }

    // Multiplicando uma matriz esparsa por uma densa
    for (i = idThread; i < LINHAS; i = i + NUM_THREADS)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            resultado_esparsa_X_densa[i][j] = 0;

            for (k = 0; matriz_esparsa1_posicoes[i][k] != -1; k++)
            {
                // "x" recebe a posição em que deve procurar o elemento correto da matriz_densa
                x = matriz_esparsa1_posicoes[i][k];
                resultado_esparsa_X_densa[i][j] += matriz_esparsa1_valores[i][k] * matriz_densa[x][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int *idsThread;

    // Criando threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        idsThread = (int *)malloc(sizeof(int));
        *idsThread = i;
        pthread_create(&threads[i], NULL, codigoThread, (void *)idsThread);
    }

    // Esperando todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Resultado da matriz esparsa vezes o vetor denso: \n");
    for (int i = 0; i < LINHAS; i++)
    {
        printf("%.2f\t", resultado_esparsa_X_vetor[i][0]);
        printf("\n");
    }

    printf("Resultado da matriz esparsa vezes outra matriz esparsa: \n");
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            printf("%.2f\t", resultado_esparsa_X_esparsa[i][j]);
        }

        printf("\n");
    }

    printf("Resultado da matriz esparsa vezes a matriz densa: \n");
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            printf("%.2f\t", resultado_esparsa_X_densa[i][j]);
        }

        printf("\n");
    }

    // Desalocando os recursos usados no programa
    free(idsThread);

    return 0;
}
```

Espero que isso atenda às suas necessidades!
