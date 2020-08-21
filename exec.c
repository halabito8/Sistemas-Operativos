#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
  char *args[]={"./fork.exe",NULL};
  int id=fork();

  if(id!=0){
    printf("Tu padre\n");
  }
  else{
    printf("Hijo\n");
    execvp(args[0],args);
  }

  wait(NULL);
  printf("Ya termine\n");
}
