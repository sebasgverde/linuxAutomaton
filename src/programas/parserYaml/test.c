#include <yaml.h>
#include <stdio.h>
#include <glib.h>

struct Estado
{


};
typedef struct Estado Estado_t

struct Automata
{
   char nombre []; 
   Estado_t estados[];

};




void process_layer(yaml_parser_t *parser, GNode *data);
gboolean dump(GNode *n, gpointer data);



int main (int argc, char **argv) {
    char *file_path = "automatas.yaml";
    GNode *cfg = g_node_new(file_path);
    yaml_parser_t parser;

    FILE *source = fopen(file_path, "rb");
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, source);
    process_layer(&parser, cfg); // Recursive parsing
    yaml_parser_delete(&parser);
    fclose(source);

    printf("Results iteration:\n");
   // g_node_traverse(cfg, G_PRE_ORDER, G_TRAVERSE_ALL, -1, dump, NULL);
   // g_node_destroy(cfg);

    return(0);
}



enum storage_flags { VAR, VAL, SEQ }; // "Store as" switch

void process_layer(yaml_parser_t *parser, GNode *data) {
    GNode *last_leaf = data;
    yaml_event_t event;
    int storage = VAR; // mapping cannot start with VAL definition w/o VAR key

    while (1) {
      yaml_parser_parse(parser, &event);

      // Parse value either as a new leaf in the mapping
      //  or as a leaf value (one of them, in case it's a sequence)
  //printf("%s\n", event.type);
      if (event.type == YAML_SCALAR_EVENT) {
printf("%s \n", event.data.scalar.value);
        if (storage) g_node_append_data(last_leaf, g_strdup((gchar*) event.data.scalar.value));
        else last_leaf = g_node_append(data, g_node_new(g_strdup((gchar*) event.data.scalar.value)));
        storage ^= VAL; // Flip VAR/VAL switch for the next event
      }

      // Sequence - all the following scalars will be appended to the last_leaf
      else if (event.type == YAML_SEQUENCE_START_EVENT) {storage = SEQ; puts("<b>Start Block (Sequence)</b>");}
      else if (event.type == YAML_SEQUENCE_END_EVENT) {storage = VAR; puts("<b>end Block (Sequence)</b>");}

      // depth += 1
      else if (event.type == YAML_MAPPING_START_EVENT) {
puts("<b>MAPPING_START_EVENT</b>");
        process_layer(parser, last_leaf);
        storage ^= VAL; // Flip VAR/VAL, w/o touching SEQ
      }

      // depth -= 1
      else if (
        event.type == YAML_MAPPING_END_EVENT
        || event.type == YAML_STREAM_END_EVENT
      ){puts("<b>MAPPING,stream_end_EVENT</b>"); break;}



      yaml_event_delete(&event);
    }
}


gboolean dump(GNode *node, gpointer data) {
    int i = g_node_depth(node);
    while (--i) printf(" ");
    printf("%s\n", (char*) node->data);
    return(FALSE);
}
