


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




#include <stdio.h>
#include <stdlib.h>

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}


int leerEntradaDeDescriptor(int inputDescr,int outDesc)
{
    int tam = 0;

  FILE *file = fdopen(inputDescr,"r");
    while(1)
    {

      char* apuntador = inputString(file,10);
      if(strlen(apuntador)>0)
      {
      printf("soy %d lo del archivo : \n\n%s\n",getpid(), apuntador);
      //printf("tamaño %d\n", strlen(apuntador));      }
      free(apuntador);
    }
  
  return tam;
}




int
main(int argc, char *argv[]) {


  int pipeline[2];
  pipe(pipeline);
if(fork()==0){
  int tam = leerEntradaDeDescriptor(pipeline[0],pipeline[1]);
}
else
{
  while(1)
  {
    char *m;

    printf("soy %d input string : ",getpid());
    m = inputString(stdin, 10);
    //printf("%s\n", m);
    dprintf(pipeline[1],"%s\n", m);

    //printf("tamaño %d\n", strlen(m));

    free(m);
    sleep(1);
  }
}
    return 0;
}