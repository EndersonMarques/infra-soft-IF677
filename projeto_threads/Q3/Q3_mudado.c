#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define DESCONHECIDOS 4 //Numero de Incognitas
#define ITERACOES 5 //Numero de Execuções
#define N_THREADS 4 //Numero de Threads

int MATRIZ[DESCONHECIDOS][DESCONHECIDOS] = {
  {1, 2, -1,  3},
  {3, -1, 2, -4},
  {2, 3, -4,  1},
  {-1, 1, 5, -2}
};
int VETOR[DESCONHECIDOS] = {10, -5, 12, 8};
float SOLUCAO[DESCONHECIDOS] = {1, 1, 1, 1};
float NOVA_SOLUCAO[ITERACOES][DESCONHECIDOS];
pthread_barrier_t barreira;

void calcular(float *SOLUCAO, float *NOVA_SOLUCAO, int *MATRIZ, int *VETOR, int i){
  float SOMA = 0; //Parenteses
  for(int j = 0; j < DESCONHECIDOS; j++){
      if(j != i){
          SOMA += MATRIZ[j]*SOLUCAO[j];
      }
  }
  SOMA = VETOR[i] - SOMA;
  NOVA_SOLUCAO[i] = SOMA/MATRIZ[i];
}

void *Jacobi(void *ID){
  int id = *(int *)ID;
  for(int i = 0; i < ITERACOES; i++){
      for(int j = id; j < DESCONHECIDOS; j += N_THREADS){
          calcular(SOLUCAO, NOVA_SOLUCAO[i], MATRIZ[j], VETOR, j);
      }
      pthread_barrier_wait(&barreira); //Sincronização das threads (Posiciona barreira)
      for(int j = id; j < DESCONHECIDOS; j += N_THREADS){ //Passa os valores do novo X para o X
          SOLUCAO[j] = NOVA_SOLUCAO[i][j];
      }
      pthread_barrier_wait(&barreira); //Sincronização das threads
  }
  pthread_exit(NULL);
}

int main(){
  pthread_t THREADS[N_THREADS];
  pthread_barrier_init(&barreira, NULL, N_THREADS); //Cria a barreira
  int ID[N_THREADS];
  for(int i = 0; i < N_THREADS; i++){ //Cria as threads
      ID[i] = i;
      pthread_create(&THREADS[i], NULL, Jacobi, (void *)&ID[i]);
  }
  for(int i = 0; i < N_THREADS; i++){ //Espera todas as threads terminarem de execcutar
      pthread_join(THREADS[i], NULL);
  }
  for(int i = 0; i < ITERACOES; i++){ //Printa os resultados obtidos
      printf("Resultado após %d iterações: \n", i+1);
      for(int j = 0; j < DESCONHECIDOS; j++){
          printf("X%d = %.4f\n", j, NOVA_SOLUCAO[i][j]);
      }
      printf("\n");
  }
  return 0;
}
