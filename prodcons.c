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

sem_t vacio,lleno,mutex;
int datos[4];
void* consume(); /* hilo 1 */
void* produce(); /* hilo 2 */
int main(int argc, char *argv[]) {
  pthread_attr_t attr; /* set of attributes for the thread */
  int i;

  /* El parametro que recibo es el valor inicial de dato */

  sem_init(&vacio, 0, 2); //0 no va a ser cmpartido con hilos de otro proceso
  sem_init(&lleno, 0, 2);
  sem_init(&mutex, 0, 2);

  pthread_t workers[2];/* the thread identifier */

  /* get the default attributes */
  pthread_attr_init(&attr);

  
  pthread_create(&workers[0], &attr, produce, NULL);
  pthread_create(&workers[1], &attr, consume, NULL);

  /* espera a los hilos */
  for(i=0;i<2;i++)
    pthread_join(workers[i], NULL);
}

void* produce() {
  int cc=0,i=0;
  do{
    sem_wait(&vacio);
    sem_wait(&mutex);
    datos[cc]=random()%100;
    printf("Produje %i\n",datos[cc]);
    cc = (cc < 4) ? cc++: 0;
    sem_post(&mutex);
    sem_post(&lleno);
    i++;
  }while(i<20);
  pthread_exit(0);
}

void* consume() {
  int cc=0,i=0;
  while(i<20){
    sem_wait(&lleno);
    sem_wait(&mutex);
    printf("Dato anterior %i\n",datos[cc]);
    datos[cc]+=5;
    printf("Dato actual %i\n",datos[cc]);
    cc = (cc < 4) ? cc++: 0;
    sem_post(&mutex);
    sem_post(&vacio);
    i++;
    sleep(2);
  }
  pthread_exit(0);
}
