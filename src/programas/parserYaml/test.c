#include <stdio.h>
#include <yaml.h>
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
  GSList transiciones;//Transicion_t transiciones [0];
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












yaml_event_t  event;
Automata_t* pAutom;
  GSList* automatasPrueba;
int estado = 0;

void imprimir()
{
  if(strcmp((char*)event.data.scalar.value, "automata") == 0)
  {
    pAutom = (Automata_t*)malloc(sizeof(Automata_t));
    automatasPrueba = g_slist_append(automatasPrueba, pAutom);
    estado = 1;
    return;
  }
  if(estado == 1)
  {
    pAutom -> nombre = (char *) malloc(sizeof(char) *
           (event.data.scalar.length + 1));
      printf("Got scalar (value %s)\n\n\n\n\n", event.data.scalar.value);
      estado= 0;
  }
  else
  printf("Got scalar (value %s)\n", event.data.scalar.value);
}

int main(void)
{
  FILE *fh = fopen("automatas.yaml", "r");
  yaml_parser_t parser;
  //yaml_event_t  event;   /* New variable */


  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  /* START new code */
  do {
    if (!yaml_parser_parse(&parser, &event)) {
       printf("Parser error %d\n", parser.error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */
    case YAML_STREAM_START_EVENT: puts("STREAM START"); break;
    case YAML_STREAM_END_EVENT:   puts("STREAM END");   break;
    /* Block delimeters */
    case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b>"); break;
    case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b>");   break;
    case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b>"); break;
    case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b>");   break;
    case YAML_MAPPING_START_EVENT:  puts("<b>Start Mapping</b>");  break;
    case YAML_MAPPING_END_EVENT:    puts("<b>End Mapping</b>");    break;
    /* Data */
    case YAML_ALIAS_EVENT:  printf("Got alias (anchor %s)\n", event.data.alias.anchor); break;
    case YAML_SCALAR_EVENT: imprimir();break;//printf("Got scalar (value %s)\n", event.data.scalar.value); break;
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
  fclose(fh);

  GSList* node;
  for (node = automatasPrueba; node; node = node->next) {
    fprintf(stdout, "%s\n",pAutom->nombre);
  }

  return 0;
}
