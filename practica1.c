#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/sysinfo.h>

void *runner(void *param);
int num_cpus();

int *arreglo;
int main(int argc, char *argv[]){
  pthread_attr_t attr;
  srand(time(NULL));
  int i, j, temp, upper, hilos, inf, sup;
  double diferencia;
  clock_t empieza, acaba;
  
  int num=num_cpus();
	
	printf("Encontre %d cpus \n",num);

  printf("De cuanto el arreglo?\n");
  scanf("%i",&upper);
  printf("\n");
  
  arreglo = (int *)malloc(sizeof(int)*upper);
  for(i=0;i<upper;i++)
    arreglo[i]=rand()%5000;
  pthread_attr_init(&attr);

  printf("En cuantos hilos lo quieres dividir?\n");
  scanf("%i",&hilos);

  pthread_t workers[hilos], tid;

  int lim[hilos][2];
  empieza=clock();
  for(i=0;i<hilos;i++){
    inf = i*(upper/hilos);
    sup = (upper/hilos) * (i+1);
    lim[i][0] = inf;
    lim[i][1] = sup;
    pthread_create(&workers[i], &attr, runner, &lim[i]);
  }
  printf("Por hilos\n");
  for(i=0;i<upper;i++)
    printf("%i ",arreglo[i]);

  for(i=0;i<hilos;i++)
    pthread_join(workers[i], NULL);

  printf("\n");

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
  printf("Todo ordenado\n");
  for(i=0;i<upper;i++)
    printf("%i ",arreglo[i]);

  printf("\n");
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

int num_cpus(){

	unsigned  int	eax=11,ebx=0,ecx=1,edx=0;
	asm
	volatile
	( " cpuid "
	:  "=a"  ( eax ) ,
	"=b"  ( ebx ) ,
	"=c"  ( ecx ) ,
	"=d"  ( edx )
	:  "0"  ( eax ) ,  "2"  ( ecx )
	:  ) ;
	printf("Cores :  % d\nThreads :  % d\nActual  thread :  % d\n", eax , ebx , edx);
	
	return eax*ebx;
	
}