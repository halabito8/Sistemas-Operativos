#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void *runner(void *param);

int *arreglo;
int main(int argc, char *argv[]){
  pthread_t tid;
  pthread_attr_t attr;
  srand(time(NULL));
  int i, j, temp, upper=20;
  double diferencia;
  clock_t empieza, acaba;
  printf("De cuanto el arreglo?\n");
  scanf("%i",&upper);
  printf("\n");
  arreglo = (int *)malloc(sizeof(int)*upper);
  for(i=0;i<upper;i++)
    arreglo[i]=rand()%100;
  pthread_attr_init(&attr);

  int lim1[] = {0, upper/2};

  int lim2[] = {upper/2, upper};

  empieza=clock();

  pthread_create(&tid, &attr, runner, &lim1);

  pthread_create(&tid, &attr, runner, &lim2);

  pthread_join(tid, NULL);

  for(i=0;i<upper;i++){
    for(j=0;j<upper-1;j++){
      if(arreglo[j] > arreglo[j+1]){
        temp=arreglo[j];
        arreglo[j]=arreglo[j+1];
        arreglo[j+1]=temp;
      }
    }
  }
  acaba=clock();
  for(i=0;i<upper;i++)
    printf("%i\n",arreglo[i]);

  diferencia=(double)(acaba-empieza)/CLOCKS_PER_SEC;
  printf("El ordenamiento tardo %lf segundos\n", diferencia);
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
