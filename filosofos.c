/*
  ============================================================================
  Name        : prueba_simple.c
  Author      : Rene
  Version     : 0.1
  Copyright   : Your copyright notice
  Description : ejemplo con pthreads
  ============================================================================
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int dato=0; /* dato compartido por los hilos */
sem_t sem;
void* resta(); /* hilo 1 */
void* suma(); /* hilo 2 */
void* comer();
int main(int argc, char *argv[]) {
  pthread_attr_t attr; /* set of attributes for the thread */
  int i;
  if (argc != 2) {
    fprintf(stderr, "usage: a.out <integer value>\n");
    /*exit(1);*/
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
    /*exit(1);*/
    return -1;
  }

  /* El parametro que recibo es el valor inicial de dato */
  dato = atoi(argv[1]);

  sem_init(&sem, 0, dato-1); //0 no va a ser cmpartido con hilos de otro proceso

  pthread_t workers[dato];/* the thread identifier */

  /* get the default attributes */
  pthread_attr_init(&attr);

  for(i=0;i<dato;i++)
    pthread_create(&workers[i], &attr, comer, NULL);

  /* espera a los hilos */
  for(i=0;i<dato;i++)
    pthread_join(workers[i], NULL);
}
/**
 * Este hilo resta
 */

void* comer(){
  sem_wait(&sem);
  printf("Estoy comiendo\n");
  sleep(5);
  printf("Ya acabe\n");
  sem_post(&sem);
  pthread_exit(0);
}

void* resta() {
    int t=0;
    sem_wait(&sem);
    int temp=dato;
    for (int i = 0; i <= 100000; i++) { // Haz cosas
      t++;
    }
    temp -= t/10;
    dato = temp;
    sem_post(&sem);
    pthread_exit(0);
}
/**
 * Este hilo suma
 */
void* suma() {
  int t=0;
  sem_wait(&sem);
  int temp=dato;
  for (int i = 0; i <= 500000; i++) { // Haz cosas
    t++;
  }
  temp += t/10;
  dato = temp;
  sem_post(&sem);
  pthread_exit(0);
}
