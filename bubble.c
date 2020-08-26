#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(){
  int arreglo[20], i, j, temp, upper=20;
  clock_t empieza, acaba;
  double diferencia;
  srand(time(NULL));
  for(i=0;i<upper;i++)
    arreglo[i]=rand()%100;

  empieza=clock();
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
  printf("El programa tardo %lf en ordenar el arreglo\n", diferencia);
}
