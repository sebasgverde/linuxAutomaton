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





#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

struct Transicion
{
  char* entrada;
  char* next;
};
typedef struct Transicion Transicion_t;
typedef struct Transicion * Ptransicion_t;


struct Estado
{
  //char * nomAutomata;
  char* nomNodo;
  GSList* transiciones;//Transicion_t transiciones [0];
  //tuberia entrada
  //tuberia salida [] esto debe ir pero dentro del proceso no aqui

};
typedef struct Estado Estado_t;
typedef struct Estado * Pestado_t;


struct Automata
{
   char* nombre; 
   char * alpha [50];
   char * states[20];
   GSList* estados;//Estado_t estados[0];
   char inicial;
   GSList* final;//Estado_t final [1];
};
typedef struct Automata Automata_t;
typedef struct Automata * Pautomata_t;


// Automata_t automatasPrograma [];




//------------------------------------

static void
usage(char *progname) {
  fprintf(stderr, "Usage: %s <file>\n", progname);
  exit(1);
}

// I guess this is too pedantic, but to drop all warnings requerided that
// level.
enum state { start = 0, one, two, three, four, five, six, seven,
       end = 8, error = 9 };

typedef enum state state_t;

float valueItems(pitem_t item) {
  return (float) item->units * item->valueUnit;
}

void showItem(pitem_t item) {
  fprintf(stdout, "%5d\t%8.2f\t%5d\t%8.2f\n",
    item->id, item->valueUnit, item->units,
    valueItems(item));
}

void showInvoice(pinvoice_t invoice) {
  GList* node = NULL;
  fprintf(stdout, "ID: %d\nNAME: %s\n", invoice->id, invoice->name);
  float total = 0.0f;
  for (node = invoice->items; node; node = node->next) {
    total += valueItems((pitem_t) node->data);
    showItem((pitem_t) node->data);
  }
  fprintf(stdout, "\t\t\t\t%8.2f\n", total);
}

enum state showErrorState(enum state curr_state, 
        yaml_event_type_t event_type) {
  fprintf(stderr, "Error at state %d found event: %d\n", 
    curr_state, event_type);
  return error;
}











yaml_parser_t parser;
yaml_event_t event;


void siguienteEvento(yaml_parser_t * parse, yaml_event_t * even)
{
  //antes la tenia recibiento el struct y no el *, pero parece que eso clona 
  //el struct, con esto de paso de parametros y eso es mejor siempre pasar parametros
  //referencia y no por valor
          yaml_event_delete(even);
        yaml_parser_parse(parse, even);
}



void asignarCosas(invoice_t * pinvoice)
{
  pinvoice->id = atoi((char*)event.data.scalar.value);
  siguienteEvento(&parser,&event);
  siguienteEvento(&parser,&event);

  pinvoice->name = (char *) malloc(sizeof(char) *
     (event.data.scalar.length + 1));
  strcpy(pinvoice->name, (char*) event.data.scalar.value);
  pinvoice->name[event.data.scalar.length] = '\0';
  siguienteEvento(&parser,&event);

  pinvoice->items = NULL;
}

char* mallocAString()
{
  char * apuntador =(char *) malloc(sizeof(char) *
     (event.data.scalar.length + 1));
  strcpy(apuntador,(char*) event.data.scalar.value);
  apuntador[event.data.scalar.length] = '\0';
  return apuntador;
}

void asignarAutomata(Pautomata_t pautom)
{
  siguienteEvento(&parser,&event);//automata valor
  pautom->nombre = mallocAString();

    siguienteEvento(&parser,&event);//descripcion
  siguienteEvento(&parser,&event);//descripcion valor
  siguienteEvento(&parser,&event);//alpha
  siguienteEvento(&parser,&event);//secuencia alpha

  siguienteEvento(&parser,&event);//valor
 // int i = 0;
  while(event.type != YAML_SEQUENCE_END_EVENT )
  {
      siguienteEvento(&parser,&event);//secuencia alpha valor
      //pautom->alpha[i] = mallocAString();
      //i++;
  }

  siguienteEvento(&parser,&event);//estados
  siguienteEvento(&parser,&event);//secuencia estados
  siguienteEvento(&parser,&event);//valor estado
  while(event.type != YAML_SEQUENCE_END_EVENT )
  {
      siguienteEvento(&parser,&event);//secuencia estado valor
  }

  siguienteEvento(&parser,&event);//start
  siguienteEvento(&parser,&event);//start valor
  pautom->inicial = mallocAString();

  siguienteEvento(&parser,&event);//final
  siguienteEvento(&parser,&event);//valor finales
  while(event.type != YAML_SEQUENCE_END_EVENT )
  {
      siguienteEvento(&parser,&event);//secuencia estado valor
  }

  siguienteEvento(&parser,&event);//delta

}

void asignarEstados(Pestado_t pestado)
{
    siguienteEvento(&parser,&event);//node valor
    pestado->nomNodo = mallocAString();

    siguienteEvento(&parser,&event);//trans


}

void asiganarTransiciones()
{}

void asignarCosas2(pitem_t pitem)
{
  siguienteEvento(&parser,&event);//item valor
  pitem->id = atoi((char*)event.data.scalar.value);

  siguienteEvento(&parser,&event);//unitvalue
  siguienteEvento(&parser,&event);//unitvalue valor
  pitem->valueUnit = atof((char *) event.data.scalar.value);

  siguienteEvento(&parser,&event);//units
  siguienteEvento(&parser,&event);//units valor
  pitem->units = atoi((char*)event.data.scalar.value);

}








GSList* parsingInvoicesFile(const char* filename) {
 
  FILE *infile;
  int cont = TRUE;
  infile = fopen(filename, "r");
  pitem_t pitem = NULL;
  pinvoice_t pinvoice = NULL;
  GSList* invoices = NULL;
  state_t state = start;

  GSList* listaAutomatas;
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
      asignarEstados(pitem);

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
  pautom->estados->transiciones = g_slist_append( pautom->estados->transiciones, ptrans);
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
      asiganarTransiciones(pitem);

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
  return invoices;
}

int
main(int argc, char *argv[]) {
  GSList *invoices, *node;

  if (argc != 2) {
    usage(argv[0]);
  }

  invoices = parsingInvoicesFile(argv[1]);

  for (node = invoices; node; node = node->next) {
    showInvoice((pinvoice_t) node->data);
  }
  return 0;
}
