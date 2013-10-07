#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/select.h>

#define MAX_BUFFER 256

void segundoHijo(int in, int out) {
  char buffer[MAX_BUFFER];
  int c;
  while((c = read(in, buffer, MAX_BUFFER))) {
    if (c == 0)
      break;
    else if (c == -1) 
      break;
    else if (c > 0) {
      int i;
      for (i = 0; i < c; i++) {
	buffer[i] = toupper(buffer[i]);
      }
      write(out, buffer, c);
    }
  }
}

void tercerHijo(int in, int out) {
  char buffer[MAX_BUFFER];
  int c;
  while((c = read(in, buffer, MAX_BUFFER))) {
    if (c == 0)
      break;
    else if (c == -1) 
      break;
    else if (c > 0) {
      int i;
      int j;
      for (i = 0, j = c - 1; i < c/2; i++, j--) {
	char tmp;
	tmp = buffer[i];
	buffer[i] = buffer[j];
	buffer[j] = tmp;
      }
      write(out, buffer, c);
    }
  }
  fprintf(stderr, "Termino tercer hijo\n");
}

void procesoPadre(int in, int out) {
  
  char buffer[MAX_BUFFER];
  int c;
  fd_set fdin;
  int rin;

  for (;;) {

    FD_ZERO(&fdin);
    FD_SET(0, &fdin);
    FD_SET(in, &fdin);
    rin = select((in + 1), &fdin, NULL, NULL, NULL);

    if (rin == 1 ) {
      if (FD_ISSET(0, &fdin)) { 
	c = read(0, buffer, MAX_BUFFER);
      }
      else if (FD_ISSET(in, &fdin)) { 
	c = read(in, buffer, MAX_BUFFER);
      }
    }
    if (c == 0)
      break;
    else if (c == -1) 
      break;
    else if (c > 0) {
      if (FD_ISSET(0, &fdin)) {
	int res;
	res = write(out, buffer, c);
	if (res < -1) {
	  fprintf(stderr, "Error en salida tuberia padre");
	}
      }
      else if (FD_ISSET(in, &fdin)) {
	int res;
	res = write(1, buffer, c);
	if (res < -1) {
	  fprintf(stderr, "Error en salida estandar padre");
	}
	else {
	  fprintf(stdout, "\n");
	}
      }
    }
  }
  fprintf(stderr, "Termino padre\n");
}

int
main() {
  int tuberia1[2];
  int tuberia2[2];
  int tuberia3[2];
  int a=0;
  char b [2] ="a";

  pipe(tuberia1);
  pipe(tuberia2);
  pipe(tuberia3);

  printf("hola 1 a es %d b es %s\n",a,b);

  if (fork() == 0) { // Segundo hijo
 a++;
 b[1] = '1';
  printf("hola 2 a es %d b es %s\n",a,b);
    close(0);
    close(1);
    //close(tuberia3[0]);
    //close(tuberia3[1]);
    segundoHijo(tuberia1[0], tuberia2[1]);
  }
  else if (fork() == 0) { // Tercer hijo
     a++;
     // b[1] = '2';

  printf("hola 3 a es %d b es %s\n",a,b);

    close(0);
    close(1);
    //close(tuberia1[0]);
    //close(tuberia1[1]);
    tercerHijo(tuberia2[0], tuberia3[1]);
  }
  else { // Padre
         a++;
         // b[1] = '3';

  printf("hola 4 a es %d b es %s\n",a,b);
   // close(tuberia2[0]);
    //close(tuberia2[1]);
    procesoPadre(tuberia3[0], tuberia1[1]);
  }
}

