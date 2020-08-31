#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int arreglo[20];
void *runner(void *param);
int main(int argc, char *argv[]){
  pthread_t tid, workers[2];
  pthread_attr_t attr;
  srand(time(NULL));
  int i, j, temp;
  int upper=20;
  for(i=0;i<20;i++)
    arreglo[i]=rand()%100;
  pthread_attr_init(&attr);

  int lim[2][2] = {0, upper/2};

  pthread_create(&workers[0], &attr, runner, &lim[0]);

  lim[1][0] = upper/2;
  lim[1][1] = upper;

  pthread_create(&workers[1], &attr, runner, &lim[1]);

  for(i=0;i<2;i++)
    pthread_join(workers[i], NULL);
  
/*
  for(i=0;i<upper;i++){
    for(j=0;j<upper-1;j++){
      if(arreglo[j] > arreglo[j+1]){
        temp=arreglo[j];
        arreglo[j]=arreglo[j+1];
        arreglo[j+1]=temp;
      }
    }
  }*/
  for(i=0;i<upper;i++)
    printf("%i\n",arreglo[i]);
}

void *runner(void *param){
  int *par=(int *)param;
  int lower=par[0], upper=par[1], i, j, temp;
  for(i=lower;i<upper;i++){
    for(j=lower;j<upper-1;j++){
      if(arreglo[j] > arreglo[j+1]){
        temp=arreglo[j];
        arreglo[j]=arreglo[j+1];
        arreglo[j+1]=temp;
      }
    }
  }
  pthread_exit(0);
}