#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
  int id = fork();
  int x=10;
  if (id!=0){
    sleep(15);
    x+=5;
    printf("Hola Mundo! No, soy tu padre x=%d\n",x);
  }
  else{
    sleep(10);
    x-=3;
    printf("Hola Mundo! Me han forkeado! x=%d\n",x);
  }
}

