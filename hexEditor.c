#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>

/* Variable global para mejor legibilidad */
int fd; // Archivo a leer

char *hazLinea(char *base, int dir) {
	char linea[100]; // La linea es mas pequeña
	int o=0;
	// Muestra 16 caracteres por cada linea
	o += sprintf(linea,"%08x ",dir); // offset en hexadecimal
	for(int i=0; i < 4; i++) {
		unsigned char a,b,c,d;
		a = base[dir+4*i+0];
		b = base[dir+4*i+1];
		c = base[dir+4*i+2];
		d = base[dir+4*i+3];
		o += sprintf(&linea[o],"%02x %02x %02x %02x ", a, b, c, d);
	}
	for(int i=0; i < 16; i++) {
		if (isprint(base[dir+i])) {
			o += sprintf(&linea[o],"%c",base[dir+i]);
		}
		else {
			o += sprintf(&linea[o],".");
		}
	}
	sprintf(&linea[o],"\n");

	return(strdup(linea));
}

char *mapFile(char *filePath) {
    /* Abre archivo */
    fd = open(filePath, O_RDWR);
    if (fd == -1) {
    	perror("Error abriendo el archivo");
	    return(NULL);
    }

    /* Mapea archivo */
    struct stat st;
    fstat(fd,&st);
    int fs = st.st_size;

    char *map = mmap(0, fs, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
    	close(fd);
	    perror("Error mapeando el archivo");
	    return(NULL);
    }

  return map;
}

void pantalla(char *map, int base){
	for(int i= 0; i<25; i++) {
		// Haz linea, base y offset
		char *l = hazLinea(map,base+i*16);
		move(i,0);
		addstr(l);
	}
	refresh();
}

void modifica(char *map,int c, int x, int y, int offset){
	char c1,n1;
	if(x<16){
		char n = tolower(c);
		if((n>='0' && n<='9') || (n>='a' && n<='f')){
			c1 = leeChar();
			char n1 = tolower(c1);
			if((n1>='0' && n1<='9') || (n1>='a' && n1<='f')){
				char hd[3];
				hd[0]=n;
				hd[1]=n1;
				hd[2]=0;
				long l = strtol(hd,NULL,16);
				map[(offset+y)*16+x]=l;
				pantalla(map, offset*16);
			}
		}
	}
	else{
		char n = c;
		if(isprint(n)){
			map[(offset+y-1)*16+x]=n;
			pantalla(map, offset*16);
		}
	}
}

int edita(char *filename) {
	int i,c,x=0,y=0,px=0,offset=0,tamanol=16;
	/* Lee archivo */
	char *map = mapFile(filename);
	if (map == NULL) {
		exit(EXIT_FAILURE);
	}
	
	pantalla(map,0);

	do{
		px= (x<16) ? x*3: 32+x;
		move(0+y,9+px);
		c = leeChar();
		switch(c){
			case 0x1B5B41: // Arriba
				if(y>0){
					y -= 1;
				}
				else if(y==0 && offset==0){
					y=0;
				}
				else{
					offset -= 1;
					for(i= 0; i<25; i++) {
						// Haz linea, base y offset
						char *p = hazLinea(map,i*16 + offset*tamanol);
						move(i,0);
						addstr(p);
					}
				}
				break;
			case 0x1B5B42: // Abajo
				if(y<24){
					y += 1;
				}
				else{
					offset += 1;
					for(i= 0; i<25; i++) {
						// Haz linea, base y offset
						char *p = hazLinea(map,i*16 + offset*tamanol);
						move(i,0);
						addstr(p);
					}
				}
				break;
			case 0x1B5B43: // Derecha
				if(x<31){
					x += 1;
				}
				else if(x==31){
					y += 1;
					x=0;
				}
				break;
			case 0x1B5B44: // Izquierda
				if(x>0){
					x -= 1;
				}
				else if(x == 0 && (y!=0 || offset!=0)){
					y -= 1;
					x = 31;
				}
				break;
			default:
				modifica(map,c,x,y,offset);
				// if(x<16){
				// 	char n = tolower(c);
				// 	if((n>='0' && n<='9') || (n>='a' && n<='f')){
				// 		c1 = leeChar();
				// 		char n1 = tolower(c1);
				// 		if((n1>='0' && n1<='9') || (n1>='a' && n1<='f')){
				// 			char hd[3];
				// 			hd[0]=n;
				// 			hd[1]=n1;
				// 			hd[2]=0;
				// 			long l = strtol(hd,NULL,16);
				// 			map[(offset+y)*16+x]=l;
				// 			pantalla(map, offset*16);
				// 		}
				// 	}
				// }
				// else{
				// 	char n = tolower(c);
				// 	if(isprint(n)){
				// 		map[(offset+y-1)*16+x]=n;
				// 		pantalla(map, offset*16);
				// 	}
				// }
				break;
		}
		move(28,10);
    printw("Estoy en x=%d y=%d, Offset %02i", x, y, offset);
    clrtoeol();
	}while(c != 24);

	if (munmap(map, fd) == -1) {
		perror("Error al desmapear");
	}
	close(fd);
	
	return 0;

}
    


