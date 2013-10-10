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










struct Transicion
{
  char* entrada;
  char* siguiente;
};
typedef struct Transicion Transicion_t;
typedef struct Transicion * Ptransicion_t;


struct Estado
{
  char* nomNodo;
  GSList* transiciones;//Transicion_t transiciones [0];
  //tuberia(int) entrada
  int PidProceso;
};
typedef struct Estado Estado_t;
typedef struct Estado * Pestado_t;


struct Automata
{
   char* nombre; 
   GSList* alpha;
   GSList* states;//lista de letras
   //int* tuberiasSalida;array
   //int* tuberiasEntrada;array
   char* inicial;
   GSList* final;//Estado_t final [1];
   GSList* estados;//lista de structs

};
typedef struct Automata Automata_t;
typedef struct Automata * Pautomata_t;


struct MensajeDeUsuario
{
  char* cmd;
  char* msg;
};
typedef struct MensajeDeUsuario MensajeDeUsuario_t;
typedef struct MensajeDeUsuario * PmensajeDeUsuario_t;

struct MensajeEntreAutomatas
{
  char* recog;
  char* rest;
};
typedef struct MensajeEntreAutomatas MensajeEntreAutomatas_t;
typedef struct MensajeEntreAutomatas * PmensajeEntreAutomatas_t;


struct MensajeAutsisCtrl
{
  int codterm;
  char* recog;
  char* rest;
};
typedef struct MensajeAutsisCtrl MensajeAutsisCtrl_t;
typedef struct MensajeAutsisCtrl * PmensajeAutsisCtrl_t;

struct TuberiasAutomata
{ 
  char* nombreAut;
  int outEstEntrada;
  int inAsisCtrl;
  int outAsisCtrl;
};
typedef struct TuberiasAutomata TuberiasAutomata_t;
typedef struct TuberiasAutomata * PtuberiasAutomata_t;


static void
usage(char *progname) {
  fprintf(stderr, "Usage: %s <file>\n", progname);
  exit(1);
}

// I guess this is too pedantic, but to drop all warnings requerided that
// level.
enum state { start = 0, one, two, three, four, five, six, seven, eight, nine, ten,
       end = 11, error = 12 };

typedef enum state state_t;


enum state showErrorState(enum state curr_state, 
        yaml_event_type_t event_type) {
  fprintf(stderr, "Error at state %d found event: %d\n", 
    curr_state, event_type);
  return error;
}

yaml_parser_t parser;
yaml_event_t event;






#define BUFFER_MAXIMO 256






void siguienteEvento(yaml_parser_t * parse, yaml_event_t * even)
{
  //antes la tenia recibiento el struct y no el *, pero parece que eso clona 
  //el struct, con esto de paso de parametros y eso es mejor siempre pasar parametros
  //referencia y no por valor
          yaml_event_delete(even);
        yaml_parser_parse(parse, even);
}


char* mallocAString()
{
  char * apuntador =(char *) malloc(sizeof(char) *
     (event.data.scalar.length + 1));
  strcpy(apuntador,(char*) event.data.scalar.value);
  apuntador[event.data.scalar.length] = '\0';
  return apuntador;
}

GSList* listaDeStrings()
{
  GSList* temp = NULL;
  char* charTemp = NULL;

    while(event.type != YAML_SEQUENCE_END_EVENT )
  {
    if(event.data.scalar.length > 0)
    {
      charTemp = mallocAString();
      temp = g_slist_append(temp, charTemp);
    }
    siguienteEvento(&parser,&event);//secuencia alpha valor
    //pautom->alpha[i] = mallocAString();
    //i++;
  }
  return temp;
}












//----------------parserAutomatas-----------------

void asignarAutomata(Pautomata_t pautom)
{
  GSList* cosa;//para imprimir alpha, states y final(debug)


  siguienteEvento(&parser,&event);//automata valor
  pautom->nombre = mallocAString();

  siguienteEvento(&parser,&event);//descripcion
  siguienteEvento(&parser,&event);//descripcion valor
  siguienteEvento(&parser,&event);//alpha
  siguienteEvento(&parser,&event);//secuencia alpha

  siguienteEvento(&parser,&event);//valor

  pautom -> alpha = listaDeStrings();

  //for (cosa = pautom->alpha; cosa; cosa=cosa->next)
    //printf("alfabeto : %s\n",cosa->data);

  siguienteEvento(&parser,&event);//estados
  siguienteEvento(&parser,&event);//secuencia estados
  siguienteEvento(&parser,&event);//valor estado
  

  pautom -> states = listaDeStrings();
  
  //for (cosa = pautom->states; cosa; cosa=cosa->next)
    //printf("estados : %s\n",cosa->data);


  siguienteEvento(&parser,&event);//start
  siguienteEvento(&parser,&event);//start valor
  pautom->inicial = mallocAString();

  siguienteEvento(&parser,&event);//final
  siguienteEvento(&parser,&event);//valor finales
  
    pautom -> final = listaDeStrings();

  //for (cosa = pautom->final; cosa; cosa=cosa->next)
    //printf("estados finales : %s\n",cosa->data);

  siguienteEvento(&parser,&event);//delta

}

void asignarEstados(Pestado_t pestado)
{
    siguienteEvento(&parser,&event);//node valor
    pestado->nomNodo = mallocAString();

    siguienteEvento(&parser,&event);//trans


}

void asiganarTransiciones(Ptransicion_t ptrans)
{
    siguienteEvento(&parser,&event);//in valor
    ptrans->entrada = mallocAString();

    siguienteEvento(&parser,&event);//next
    siguienteEvento(&parser,&event);//next valor
    ptrans->siguiente = mallocAString();


}


GSList* parserArchivoAutomatas(const char* filename) {
 
  FILE *infile;
  int cont = TRUE;
  infile = fopen("automatas.yaml", "r");

  state_t state = start;

  GSList* listaAutomatas = NULL;
  Pautomata_t pautom = NULL;
  Pestado_t pestado = NULL;
  Ptransicion_t ptrans = NULL;

  yaml_parser_initialize(&parser);
  yaml_parser_set_input_file(&parser, infile);

  do {
    yaml_parser_parse(&parser, &event);
    switch (state) {
    case start:
      switch(event.type) {
          case YAML_STREAM_START_EVENT:
      state = one;
      break;
          default:
      state = showErrorState(state, event.type);
      break;
      }
      break;






    case one:
      switch (event.type) {
      case YAML_STREAM_END_EVENT:
        state = end;
        break;
            case YAML_DOCUMENT_START_EVENT:
        state = two;
        break;
            default:
        state = showErrorState(state, event.type);
        break;
      }
      break;








    case two:
      switch (event.type) {
      case YAML_SEQUENCE_START_EVENT:
      state = three;
      break;

          case YAML_DOCUMENT_END_EVENT:
      state = one;
      break;

          default:
      state = showErrorState(state, event.type);
      break;
      }
      break;







    case three:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT://inicia un automata
  state = four;
  pautom = (Pautomata_t) malloc(sizeof(Automata_t));
  listaAutomatas = g_slist_append(listaAutomatas, pautom);
  break;
      case YAML_SEQUENCE_END_EVENT://termina secuencia de automatas
  state = two;
  break;
      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;








    case four:
      switch(event.type) {
      case YAML_SCALAR_EVENT://automata

          asignarAutomata(pautom);

    state = five;
  break;

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;





    case five:
      switch(event.type) {
      case YAML_SEQUENCE_START_EVENT://inicia secuencia de estados
  state = six;
  break;
      case YAML_MAPPING_END_EVENT://termina automata 
  state = three;
  break;

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;







    case six:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT://nuevo estado
  pestado = (Pestado_t) malloc(sizeof(Estado_t));
  pautom->estados = g_slist_append(pautom->estados, pestado);
  state = seven;
  break;
      case YAML_SEQUENCE_END_EVENT://termina secuencia de estados
  state = five;
  break;
      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;










    case seven:
      switch(event.type) {
      case YAML_SCALAR_EVENT://node
      asignarEstados(pestado);

  state = eight;
  break;
    /*  case YAML_MAPPING_END_EVENT:
  state = six;
  break;*/

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;







  case eight:
       switch(event.type) {
      case YAML_SEQUENCE_START_EVENT://inicia secuencia de transiciones
  state = nine;
  break;
      case YAML_SCALAR_EVENT://no tiene transiciones
    pestado->transiciones = NULL;
    state = eight;
    break;
      case YAML_MAPPING_END_EVENT://termina nodo
  state = six;
  break;

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;




    case nine:
      switch(event.type)
      {
        case YAML_MAPPING_START_EVENT://nueva transicion

      ptrans = (Ptransicion_t) malloc(sizeof(Transicion_t));
  pestado->transiciones = g_slist_append( pestado->transiciones, ptrans);
  state = ten;
  break;
      case YAML_SEQUENCE_END_EVENT://termina secuencia de transiciones
  state = eight;
  break;
      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;






    case ten:
  switch(event.type) {
      case YAML_SCALAR_EVENT://in
      asiganarTransiciones(ptrans);

  state = ten;
  break;
      case YAML_MAPPING_END_EVENT://termina transicion
  state = nine;
  break;

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;





    case end:
      cont = FALSE;
      break;

    case error:
      cont = FALSE;
      return NULL;
      break;
    }
    yaml_event_delete(&event);
  } while (cont);

  yaml_parser_delete(&parser);
  return listaAutomatas;
}













//-------------------parser entre automatas--------------------
void parserMensajeEntreAutomatas(PmensajeEntreAutomatas_t pmensaje, char* fh)
{

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_string(&parser, (unsigned char *)fh, strlen(fh));

  /* START new code */
  do {
    if (!yaml_parser_parse(&parser, &event)) {
       printf("Parser error %d\n", parser.error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 

      case YAML_SCALAR_EVENT: 
      {
        if(strcmp(event.data.scalar.value,"recog")==0)
        {
          siguienteEvento(&parser,&event);//recog valor
          pmensaje->recog = mallocAString();
        }
        else if(strcmp(event.data.scalar.value,"rest")==0)
        {
          siguienteEvento(&parser,&event);//rest valor
          pmensaje->rest = mallocAString();
        }      }
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
}






//------------parser automatas a sisctrl-------------------


void parserMensajeAutomatasSisCtrl(PmensajeAutsisCtrl_t pmensaje)
{
  char fh [BUFFER_MAXIMO];
  

  fprintf(stdout,"ingrese entrada\n","%s");
  int tama = read(0,fh,BUFFER_MAXIMO);

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_string(&parser, (unsigned char *)fh, tama);

  /* START new code */
  do {
    if (!yaml_parser_parse(&parser, &event)) {
       printf("Parser error %d\n", parser.error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 

      case YAML_SCALAR_EVENT: 
      {
        if(strcmp(event.data.scalar.value,"codterm")==0)
        {
          siguienteEvento(&parser,&event);//codterm valor
          pmensaje->codterm = atoi((char*)event.data.scalar.value);
        }
        else if(strcmp(event.data.scalar.value,"recog")==0)
        {
          siguienteEvento(&parser,&event);//recog valor
          pmensaje->recog = mallocAString();
        }      
        else if(strcmp(event.data.scalar.value,"rest")==0)
        {
          siguienteEvento(&parser,&event);//rest valor
          pmensaje->rest = mallocAString();
        }
      }
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
}




//----------------parser mensajes de usuario-------------------------------


void parserMensajesDeUsuario(PmensajeDeUsuario_t pmensaje)
{
  char fh [BUFFER_MAXIMO];
  

  fprintf(stdout,"ingrese entrada\n","%s");
  int tama = read(0,fh,BUFFER_MAXIMO);

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_string(&parser, (unsigned char *)fh, tama);

  /* START new code */
  do {
    if (!yaml_parser_parse(&parser, &event)) {
       printf("Parser error %d\n", parser.error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 

      case YAML_SCALAR_EVENT: 
      {
        if(strcmp(event.data.scalar.value,"cmd")==0)
        {
          siguienteEvento(&parser,&event);//cmd valor
          pmensaje->cmd = mallocAString();
        }
        else if(strcmp(event.data.scalar.value,"msg")==0)
        {
          siguienteEvento(&parser,&event);//msg valor
          pmensaje->msg = mallocAString();
        }      }
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
}


















//-------------procesos---------------------------------------------
























/*

esto es lo que debe ir en sysctrl

*/

char* evaluarCadena(char* recog,char* cadena, GSList* transiciones)
{
  GSList* node;

  for(node=transiciones;node;node=node->next)
  {
    Ptransicion_t tran = (Ptransicion_t) node->data;
    if(strncmp(tran->entrada,cadena,strlen(tran->entrada))==0)
    {

      int a = strlen(cadena)-strlen(tran->entrada)+1;
      char* nuevoRest = (char*)malloc(a);
      strncpy(nuevoRest,cadena+strlen(tran->entrada),a-1);
      nuevoRest[a-1]='\0';
      //cadena = (char*)malloc(strlen(nuevoRest));
      strcpy(cadena,nuevoRest);

      nuevoRest = (char*)malloc(strlen(recog)+strlen(tran->entrada));
      sprintf(nuevoRest,"%s%s",recog,tran->entrada);
      strcpy(recog,nuevoRest);

      return(tran->siguiente);
    }
  }
  return "0";
}

int obtenerDescriptor(char* estado,int** pipes,GSList* states)
{
  GSList* node;
  int pos = 0;

  for(node=states;node;node=node->next)
  {
    if(strcmp(node->data, estado)==0)
      break;
    else
      pos++;
  }

  return pipes[pos][1];//ojo que si el estado no esta, aumenta y posicion
  //seria = a pipes.length, dando un segmentation faul, esto no importantes
  //pues nunca se ingresara un estado incorrecto
}

void procesoEstado(char* nomAut,char* nombreEst,int in, int** pipes, GSList* states, GSList* transiciones, int numEst, int esFinal)
{
  printf("aut: %s estad: %s  final: %d entrada: %d\n",nomAut ,nombreEst,esFinal,in);
  /*int i;
  for(i=0; i<numEst;i++)
  {
    printf("%d,%d ", pipes[i][0],pipes[i][1]);
  }
  printf("el pid es: %d\n", getpid());
  printf("\n");*/

  //printf("aut: %s estado %s pid: %d\n",nomAut,nombreEst, getpid() );



  char buffer[BUFFER_MAXIMO];//aqui decia char* bufre[bumaximo] meimagino que eso es lo que se usa para acumular
  int tamLeido;
  PmensajeEntreAutomatas_t pmensaje;

  setpgid(0,getpgid(getppid()));
  //printf("soy estado %s padre: %d y grupo es: %d\n", nombreEst,getppid(), getpgrp());


  while(tamLeido = read(in, buffer, BUFFER_MAXIMO))
  {
        /*while((fh[tama-2] != '}') && (fh[tama-1] != '\0'))
          {
            char tmp[80];
            tama = read(0,tmp,6);

            strcat(fh,tmp);
            //printf("%s\n", fh);

            }
          printf("%s\n",fh );
          //a[1] = '\0';
          //printf("el valor de a es %s\n", a);
          if(fh[tama-2] == '}')
            write(1,"true", 5);
          else
            write(1,"falsesdfgsdf", 6);*/
    if (tamLeido == 0)
    {
     break;
    }
    else if (tamLeido == -1) 
      break;
    else if (tamLeido > 0) 
    {
        printf("aut: %s estad: %s  %s\n",nomAut ,nombreEst,buffer);

        pmensaje = (PmensajeEntreAutomatas_t) malloc(sizeof(MensajeDeUsuario_t));
        parserMensajeEntreAutomatas(pmensaje,buffer);
        //printf("recog:%s rest: %s\n",pmensaje->recog,pmensaje->rest);

        if(strcmp(pmensaje->rest,"")==0) 
        {
          if(esFinal)
          {
            printf("cadena aceptada\n");
            return;//aceptar
          }
          else
          {
            printf("cadena rechazada por no acabar en final\n");
            return;//rechazar
          }
        }
      else
      {
        char* result = evaluarCadena(pmensaje->recog, pmensaje->rest, transiciones);
        if(strcmp(result,"0")==0)
        {
          printf("cadena rechazada por no encontrar transicion\n");
          return;//rechazar
        }
        else
        {
          int desc=obtenerDescriptor(result,pipes,states);

        
          char impresion[BUFFER_MAXIMO];
          //esta funcion es la puteria, aunque la encontre despues de darme comntra
          //los muros por mas de 3 horas tratando de concatenar eso con el write
          sprintf(impresion,"{ recog: %s, rest: %s }", pmensaje->recog,pmensaje->rest);
          write(desc, impresion, BUFFER_MAXIMO);
          //dprintf(desc,"{ recog: %s, rest: %s }", pmensaje->recog,pmensaje->rest);
          //printf("el  parse { recog: %s, rest: %s }\n", pmensaje->recog,pmensaje->rest);

        }
      }
    }
  }


}



int numeroEstadosAutomata(GSList* states)
{
  GSList* node;
  int numero = 0;
  for(node = states;node;node = node->next)//me esat
  {
    numero++;
  }
  return numero;
}

//cuando empiezo a lanzar los procesos uso esto para saber si el estado
//es final
int esFinal(char* nombreEst, GSList* finales)
{
  int resp = 0;
  GSList* node;

  for (node = finales; node; node = node->next) 
  {
    if(strcmp(node->data,nombreEst)==0)
      resp=1;
  }
  return resp;

}

void imprimirCosas(GSList* pipesAutomatas)
{
  GSList* cosa;
    for(cosa = pipesAutomatas; cosa; cosa = cosa->next)
    {
      PtuberiasAutomata_t ptuberia = (PtuberiasAutomata_t)cosa->data;
      printf("nomaut: %s outent %d insis %d outsis %d\n", ptuberia->nombreAut,ptuberia->outEstEntrada,ptuberia->inAsisCtrl,ptuberia->outAsisCtrl);
    }
}

int
main(int argc, char *argv[]) {
  GSList *node, *trans, *automatas, *estadito, *automata,*cosa;
  //PmensajeEntreAutomatas_t pmensaje;
  //PmensajeAutsisCtrl_t pmensaje;
  PmensajeDeUsuario_t pmensaje;
  PtuberiasAutomata_t ptuberias;
  int **pipes;//lista de arreglos por estado para cada automata
  GSList *pipesAutomatas;/*tuberias por cada automata para comuncarse 
  con sisctrl y saber los estados de entrada*/
  char estadoLeido[1];
  char cadenaEntrada[BUFFER_MAXIMO];
  setsid();

  //printf("soy sisctrl y mi pid es: %d y grupo: %d\n", getpid(),getpgrp());

  /*if (argc != 2) {
    usage(argv[0]);
  }*/

  automatas = parserArchivoAutomatas("automatas.yaml");//argv[1]"");

  /*pmensaje = (PmensajeEntreAutomatas_t) malloc(sizeof(MensajeEntreAutomatas_t));
  parserMensajeEntreAutomatas(pmensaje);
  printf("recog:%s rest: %s\n",pmensaje->recog,pmensaje->rest);
  */

  /*pmensaje = (PmensajeAutsisCtrl_t) malloc(sizeof(MensajeAutsisCtrl_t));
  parserMensajeAutomatasSisCtrl(pmensaje);
  printf("codterm: %d recog:%s rest: %s\n",pmensaje->codterm, pmensaje->recog,pmensaje->rest);
  */

  /*pmensaje = (PmensajeDeUsuario_t) malloc(sizeof(MensajeDeUsuario_t));
  parserMensajesDeUsuario(pmensaje);
  printf("cmd:%s msg: %s\n",pmensaje->cmd, pmensaje->msg);
  */


  /*for (node = automatas; node; node = node->next) {
    //showInvoice((pinvoice_t) node->data);
    Pautomata_t a= (Pautomata_t)node->data;
    printf("\nautomata: %s\n", a->nombre);

    for (cosa = a->alpha; cosa; cosa=cosa->next)
      printf("alfabeto : %s\n",cosa->data);   
    for (cosa = a->states; cosa; cosa=cosa->next)
      printf("Estados : %s\n",cosa->data);  
    for (cosa = a->final; cosa; cosa=cosa->next)
      printf("Estados finales : %s\n",cosa->data);  
    GSList* estaditos;
    for(estaditos = a->estados; estaditos; estaditos=estaditos->next)
    {
      Pestado_t b = (Pestado_t) estaditos->data;
      printf("Estado: %s pid: %d\n", b->nomNodo, b->PidProceso);
      for(trans=b->transiciones;trans;trans = trans->next)
      {
        Ptransicion_t c = (Ptransicion_t)trans->data;
        printf("%s\n", c->entrada);
        printf("%s\n\n", c->siguiente);
      }
    }      
  }*/


  for(automata = automatas; automata; automata = automata->next)
  {
    //cada automata
    Pautomata_t pautomata= (Pautomata_t)automata->data;

    int numeroEstados = numeroEstadosAutomata(pautomata->states);
    pipes = (int**)malloc(numeroEstados * sizeof(int*));//numero de filas
    printf("numero de estados: %d\n", numeroEstados);
    int i;
    for(i = 0; i < numeroEstados; i++)
    {
      pipes[i] = (int*)malloc(2*sizeof(int));//nuevo arreglo de 2 posiciones
      pipe(pipes[i]);//nueva tuberia

    }

/*
    for(i=0; i<numeroEstados;i++)
    {
      printf("%d  ", pipes[i][0]);
    }
    printf("\n\n");
    sleep(1);*/

    /*aqui se llena la estructura tuberias automatas*/
    ptuberias = (PtuberiasAutomata_t)malloc(sizeof(TuberiasAutomata_t));
    pipesAutomatas = g_slist_append(pipesAutomatas, ptuberias);

    ptuberias->nombreAut = (char*)malloc(strlen(pautomata->nombre));
    strcpy(ptuberias->nombreAut, pautomata->nombre);

    int tuberiaSisctrl[2];
    pipe(tuberiaSisctrl);
    ptuberias->inAsisCtrl = tuberiaSisctrl[0];
    ptuberias->outAsisCtrl = tuberiaSisctrl[1];
    printf("en la lista critica %s\n", ptuberias->nombreAut);

    i=0;
    for(estadito = pautomata->estados;estadito;estadito = estadito->next)
    {
      Pestado_t pestadito = (Pestado_t) estadito->data;
      int finalONo = esFinal(pestadito->nomNodo,pautomata -> final);

      if(strcmp(pestadito->nomNodo, pautomata->inicial)==0)//si es el de entrada ponga la tuberia en que toca escribir
            ptuberias->outEstEntrada = pipes[i][1];

      int pid = fork();
      if(pid == 0)
      {
        procesoEstado(pautomata->nombre,pestadito->nomNodo,pipes[i][0],pipes, pautomata->states, pestadito->transiciones,numeroEstados, finalONo);
        return;
      }

      pestadito -> PidProceso = pid;
      //printf("pid aut %s es %d\n",pestadito->nomNodo,pestadito->PidProceso );

      // printf("nomaut: %s outent %d insis %d outsis %d\n", ptuberias->nombreAut,ptuberias->outEstEntrada,ptuberias->inAsisCtrl,ptuberias->outAsisCtrl,);

      i++;
      //sleep(1);
    }


       /* while(1)
    {
          printf("ingrese cadena\n" );
          scanf("%s",cadenaEntrada);

          char* envio[BUFFER_MAXIMO];
          sprintf(envio,"{ recog: , rest: %s }",cadenaEntrada);
          //int desc=obtenerDescriptor(estadoLeido,pipes,pautomata->states);
          int desc=obtenerDescriptor("A",pipes,pautomata->states);          
          write(desc,envio,BUFFER_MAXIMO);
          //write(desc,"{ recog: afgdf, rest: ccaaaaaabbcc }",BUFFER_MAXIMO);

    }*/
  }
  imprimirCosas(pipesAutomatas);/*me toco poner esta funcion no se
  por que poniendo el for aca nunca paso de segmentation fault -_-*/
 // kill(getpgrp(),1);

  return 0;
}