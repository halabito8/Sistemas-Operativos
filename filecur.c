#include <curses.h>
#include <dirent.h>
#include<string.h>

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

int main(){
  struct s_dir {
    int tipo;
    char *nombre;
  } res[128];

  char lista[100][100], temp[100];
  int i=0,j,k;
  int c;

  DIR *dir = opendir(".");
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

  initscr();
  raw();
  noecho(); /* No muestres el caracter leido */
  cbreak(); /* Haz que los caracteres se le pasen al usuario */
  do {
    for (k=0; k < j; k++) {
      if (k == i) {
        attron(A_REVERSE);
      }
      mvprintw(5+k,5,lista[k]);
      attroff(A_REVERSE);
    }
    move(5+i,5);
    refresh();
    c = leeChar();
    switch(c) {
      case 0x1B5B41:
        i = (i>0) ? i - 1 : j;
        break;
      case 0x1B5B42:
        i = (i<k) ? i + 1 : 0;
        break;
      default:
        // Nothing 
        break;
    }
    move(10,20);
    printw("Estoy en %d: Lei 0x%08x",i,c);
  } while (c != 0x1b);
  endwin();
  return 0;
}
