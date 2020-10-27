#include <stdio.h> 
#include <unistd.h> 
#include <pthread.h> 

#define N_CADEIRAS 5 

pthread_mutex_t mutex; 
pthread_cond_t condition; 

int clientes = 0; 
int clientesAtendidos = 0; 

enum {DORMINDO, TRABALHANDO} state_barbeiro; 
int phil_num[N_PHILOSOPHERS]; 

void *philosophing (void *arg); 
void pickup_forks(int ph_num); 
void return_forks(int ph_num); 
void test(int ph_num); 
void dormindo (); 
void trabalhando (); 
void contarCliente(); 

int main(int argc, char *argv[]) { 
  /* Setup */ 
  pthread_t ph_thread_barbeiro; 
  pthread_t ph_thread_clientes; 
  pthread_mutex_init(&mutex, NULL); 
  pthread_create(&ph_thread_barbeiro, NULL, dormindo, &clientes); 
  pthread_create(&ph_thread_clientes, NULL, contarCliente, &clientes); 
  pthread_join(ph_thread_barbeiro, NULL);

  pthread_join(ph_thread_clientes, NULL); 
  pthread_mutex_destroy(&mutex); 
  return(0); 
} 

void dormindo (void *arg){ 
  pthread_mutex_unlock(&mutex); 
  int *_clientes = arg; 
  state_barbeiro = DORMINDO; 
  printf("Dormindo - Tem %d Clientes \n", *_clientes); 
  while(*_clientes==0){ 
    sleep(1); 
  }
  trabalhando(_clientes); 
}

void trabalhando(void *arg){ 
  pthread_mutex_lock(&mutex); 
  int *_clientes = arg; 
  state_barbeiro = TRABALHANDO; 
  *_clientes = *_clientes - 1; 
  printf("Trabalhando - Restam %d Clientes \n", *_clientes); 

  sleep(tempoAleatorioTrabalho()); 
  printf("Total de Clientes atendidos: %d \n", ++clientesAtendidos); 
  if(*_clientes > 0) 
  trabalhando(_clientes); 
  else 
  dormindo(_clientes); 
} 
 
void contarCliente(void *arg){ 
  int *_clientes = arg; 
  sleep(tempoAleatorioChegadaCliente()); 
  if(*_clientes<= N_CADEIRAS){ 
    *_clientes +=1; 
    printf("Chegou cliente - Restam %d Clientes \n", *_clientes); 
  }
  contarCliente(_clientes); 
} 

int tempoAleatorioChegadaCliente(){ 
  int valor = rand() % 100;

  if (valor < 33) 
    return 25; 
  else if (valor < 66) 
    return 30; 
  else 
    return 35; 
} 

int tempoAleatorioTrabalho(){ 
  int valor = rand() % 100; 
  if (valor < 33) 
    return 20; 
  else if (valor < 66) 
    return 25; 
  else 
    return 30; 
}
