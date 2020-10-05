#include <curses.h>
#include <dirent.h>
#include<string.h>
#include<unistd.h>
#include"hexEditor.c"

typedef struct s_dir {
    int tipo;
    char *nombre;
  }s_dir;

int leeChar();
void leerdir(char *cwd, s_dir res[128], char lista[100][100], int *max);
void Arriba();

int main(){
  s_dir res[128];

  char lista[100][100], temp[100], cwd[256], arch[50];
  int i=0, j, k, max, offset=0, tam=20;
  int c;
  getcwd(cwd, 256);
  leerdir(cwd,res,lista,&max);

  initscr();
  raw();
  noecho(); /* No muestres el caracter leido */
  cbreak(); /* Haz que los caracteres se le pasen al usuario */
  do {
    for (k=0; (k+offset) < max && k < tam; k++) {
      if (k == i) {
        attron(A_REVERSE);
      }
      char *nom = lista[k+offset];
      move(5+k,5);
      addstr(nom);
      clrtoeol();
      attroff(A_REVERSE);
    }
    move(5+i,5);
    refresh();
    c = leeChar();
    switch(c) {
      case 0x1B5B41: // Arriba
        if(i==0){
          if(offset > 0){
            offset -= 1;
          }
          else{
            i = tam;
            offset = (max - i);
          }
        }
        else{
          i -= 1;
        }
        break;
      case 0x1B5B42: //Abajo
        if(  i < (tam-1) && (i<max)){
          i += 1;
        }
        else{
          if( (offset + i) < (max - 1)){
            offset += 1;
          }
          else{
            i = offset = 0;
            clear();
          }
        }
        break;
      case 0xa: //enter
        if( strcmp(lista[i + offset],"D ..") == 0 ){
          char *p=strrchr(cwd,'/');
          *p=0;
          leerdir(cwd,res,lista,&max);
          clear();
        }
        else if( lista[i + offset][0] == 'D' ){
          strcat(cwd,"/");
          strcat(cwd,lista[i + offset]+2);
          printf("Hola %s\n", cwd);
          leerdir(cwd,res,lista,&max);
          clear();
        }
        else if( lista[i + offset][0] == 'F'){
          strcat(arch,lista[i + offset]+2);
          edita(arch);
          clear();
        }
        break;
      default:
        // Nothing 
        break;
    }
    move(2,10);
    printw("Estoy en %d: Lei 0x%08x, Offset %02i, dir: %s",i, c, offset, cwd);
    clrtoeol();
  } while (c != 0x1b);
  clear();
  endwin();
  return 0;
}

int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido */
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}

void leerdir(char *cwd, s_dir res[128], char lista[100][100], int *max){
  int i=0,j=0;
  char temp[100];
  DIR *dir = opendir(cwd);
  struct dirent *dp;
  while((dp=readdir(dir)) != NULL) {
    res[i].tipo = dp->d_type;
    res[i].nombre=dp->d_name;
    i++;
  }
  
  for(j=0; j<i; j++) {
    if (res[j].tipo == DT_DIR) {
      strcpy(temp,"D ");
    }
    else {
      strcpy(temp,"F ");
    }
    strcat(temp,res[j].nombre);
    strcpy(lista[j],temp);
  }

  closedir(dir);
  *max=j;
}