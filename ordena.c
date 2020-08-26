#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/sysinfo.h>

void *runner(void *param);
int num_cpus();

int *arreglo;
int main(int argc, char *argv[]){
  pthread_t tid;
  pthread_attr_t attr;
  srand(time(NULL));
  int i, j, temp, upper=20, hilos;
  double diferencia;
  clock_t empieza, acaba;
  
  int num=num_cpus();
	
	printf("Encontre %d cpus \n",num);

  printf("De cuanto el arreglo?\n");
  scanf("%i",&upper);
  printf("\n");
  arreglo = (int *)malloc(sizeof(int)*upper);
  for(i=0;i<upper;i++)
    arreglo[i]=rand()%100;
  pthread_attr_init(&attr);

  printf("En cuantos hilos lo quieres dividir?\n");
  scanf("%i",&hilos);

  empieza=clock();
  for(i=0;i<hilos;i++){
    int lim[] = {i*(upper/hilos), (upper/hilos) * (i+1)};
    pthread_create(&tid, &attr, runner, &lim);
  }

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
