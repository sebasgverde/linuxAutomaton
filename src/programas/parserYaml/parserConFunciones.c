/*
 * Author: Juan Francisco Cardona Mc'Cormick
 * Date:   20/09/2013
 *
 * Purpose: To show how to write an parser for YAML file
 *
 * History of Modifications:
 * 27/09/2013 - Correcting some errors and adding a procesing
 *              for each invoice.
 */




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


struct Estado
{
  //char * nomAutomata;
  char* nomNodo;
  GSList* transiciones;//Transicion_t transiciones [0];
  //tuberia entrada
  //tuberia salida [] esto debe ir pero dentro del proceso no aqui

};
typedef struct Estado Estado_t;

struct Automata
{
   char* nombre; 
   char * alpha [0];
   char * states[0];
   GSList* estados;//Estado_t estados[0];
   Estado_t inicial;
   GSList* final;//Estado_t final [1];
};
typedef struct Automata Automata_t;

// Automata_t automatasPrograma [];




//------------------------------------




struct item {
  int id;
  float valueUnit;
  int units;
};

typedef struct item item_t;
typedef struct item* pitem_t;

struct invoice {
  int id;
  char* name;
  GSList* items;
};

typedef struct invoice invoice_t;
typedef struct invoice* pinvoice_t;

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
  FILE *infile;
  int cont = TRUE;
  pitem_t pitem = NULL;
  pinvoice_t pinvoice = NULL;
  GSList* invoices = NULL;
  state_t state = start;



void asignarCosas()
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

void asignarCosas2()
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


void siguienteEvento(yaml_parser_t * parse, yaml_event_t * even)
{
  //antes la tenia recibiento el struct y no el *, pero parece que eso clona 
  //el struct, con esto de paso de parametros y eso es mejor siempre pasar parametros
  //referencia y no por valor
          yaml_event_delete(even);
        yaml_parser_parse(parse, even);
}






GSList* parsingInvoicesFile(const char* filename) {
 

  infile = fopen(filename, "r");

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
      case YAML_MAPPING_START_EVENT:
  state = four;
  pinvoice = (pinvoice_t) malloc(sizeof(invoice_t));
  invoices = g_slist_append(invoices, pinvoice);
  break;
      case YAML_SEQUENCE_END_EVENT:
  state = two;
  break;
      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;

    case four:
      switch(event.type) {
      case YAML_SCALAR_EVENT:

   // yaml_event_delete(&event);
     //   yaml_parser_parse(&parser, &event);
siguienteEvento(&parser,&event);
        asignarCosas();


    state = five;
  break;

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;

    case five:
      switch(event.type) {
      case YAML_SEQUENCE_START_EVENT:
  state = six;
  break;
      case YAML_MAPPING_END_EVENT:
  state = three;
  break;

      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;

    case six:
      switch(event.type) {
      case YAML_MAPPING_START_EVENT:
  pitem = (pitem_t) malloc(sizeof(item_t));
  pinvoice->items = g_slist_append(pinvoice->items, pitem);
  state = seven;
  break;
      case YAML_SEQUENCE_END_EVENT:
  state = five;
  break;
      default:
  state = showErrorState(state, event.type);
  break;
      }
      break;

    case seven:
      switch(event.type) {
      case YAML_SCALAR_EVENT:
      asignarCosas2();

  state = seven;
  break;
      case YAML_MAPPING_END_EVENT:
  state = six;
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
