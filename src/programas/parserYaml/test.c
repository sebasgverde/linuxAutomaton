/*
Pointers to struct[edit]

Pointers can be used to refer to a struct by its address. This is particularly useful for passing structs to a function by reference or to refer to another instance of the struct type as a field. The pointer can be dereferenced just like any other pointer in C — using the * operator. There is also a -> operator in C which dereferences the pointer to struct (left operand) and then accesses the value of a member of the struct (right operand).
struct point {
   int x;
   int y;
};
struct point my_point = { 3, 7 };
struct point *p = &my_point;   To declare and define p as a pointer of type struct point,
                                 and initialize it with the address of my_point. 
 
(*p).x = 8;                    To access the first member of the struct 
p->x = 8;    
*/


/*
variables importantes pra el debuger
event.data.scalar.value
event.type
*/

#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


#define BUFFER_MAXIMO 256


char* readinput(FILE* inp)
{
   char* input = NULL;
   char tempbuf[BUFFER_MAXIMO];
   size_t inputlen = 0, templen = 0;
   do {
       fgets(tempbuf, BUFFER_MAXIMO, inp);
       templen = strlen(tempbuf);
       inputlen += templen;
       input = realloc(input, inputlen+1);
       strcat(input, tempbuf);
    } while (templen==BUFFER_MAXIMO-1 && tempbuf[BUFFER_MAXIMO-2]!='\n');
    input[strlen(input)-1] = '\0';
    return input;
}



int leerEntradaDeDescriptor(char* cadenaEntrada, int inputDescr)
{
    int tam = 0;
  /*
  char apuntador[BUFFER_MAXIMO];
  char cadenaEntrada[BUFFER_MAXIMO];
  //int tam = read(inputDescr, apuntador, BUFFER_MAXIMO);

tam = scanf("%s", cadenaEntrada);
printf("el tamaño leido es %d\n", tam);
printf("%s\n", cadenaEntrada);
*/
  int pipeline[2];
  pipe(pipeline);


  dprintf(pipeline[1],"%s\n", cadenaEntrada);

    FILE *file = fdopen(pipeline[0],"r");

  char* apuntador = readinput(file);

printf("lo del archivo : \n\n\n\n\n\n%s\n", apuntador);
  /*

  printf("%s\n", apuntador);
  if(tam <= 0)
    return tam;

  cadenaEntrada = (char*)malloc(sizeof(char) * (tam+1));  
  strncpy(cadenaEntrada,(char*) apuntador,tam-2);
  cadenaEntrada[tam] = '\0';
    printf("%s\n", cadenaEntrada);
*/

  return tam;



  /*
    printf("%s", apuntador);
    cadenaEntrada = (char*)malloc(sizeof(char) * (tam+1);

    while(apuntador[tam-1] != '}' && apuntador[tam-1] != '\n')
    {
      tam = read(inputDescr, apuntador, 6);
      printf("%s", apuntador);
    }*/
    /*char * apuntador =(char *) malloc(sizeof(char) *
       (event.data.scalar.length + 1));
    strcpy(apuntador,(char*) event.data.scalar.value);
    apuntador[event.data.scalar.length] = '\0';
    return apuntador;
    */
}




int
main(int argc, char *argv[]) {

      char* result = readinput(stdin);
    printf("And the result is [%s]\n", result);
  int tam = leerEntradaDeDescriptor(result, 0);

    free(result);


  //printf("%s\n", a);
  //printf("tam %d\n", tam);

  return 0;
}