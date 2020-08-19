#include<dirent.h>
#include<curses.h>
#include<string.h>
#include<stdlib.h>

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
    for(int j=0;j<i;j++){
      res <<=8;
      res |= chars[j];
    }
  return res;
}

int main(){
  char lista[20][100];
  int i=0,k=0;
  int c;
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d){
    while ((dir = readdir(d)) != NULL) {
      strcpy(lista[k],dir->d_name);
      k++;
      // printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  initscr();
  raw();
  noecho(); /* No muestres el caracter leido */
  cbreak(); /* Haz que los caracteres se le pasen al usuario */
  do{
    for(j=0; j < k; j++){
      if (j == i) {
        attron(A_REVERSE);
      }
      mvprintw(5+j,5,lista[j]);
      attroff(A_REVERSE);
    }
    move(5+i,5);
    refresh();
    c = leeChar();
    switch(c) {
      case 0x1B5B41:
        i = (i>0) ? i - 1 : k;
        break;
      case 0x1B5B42:
        i = (i<k) ? i + 1 : 0;
        break;
      default:
        // Nothing 
        break;
    }
    move(2,5);
    printw("Estoy en %d: Lei 0x%08x",i,c);
  }while (c != 0x1b);
  endwin();
  return 0;
}
