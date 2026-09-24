/* Same-size nodes from malloc, the shape the llm-ergonomist named at panel 176:
 * a node given back, a new one made, the old one given back again. */
#include <stdlib.h>
#include <stdio.h>
typedef struct node { int v; } node;
static __attribute__((noinline)) node *node_new(void) { node *n = malloc(sizeof *n); return n; }
static __attribute__((noinline)) void node_free(node *n) { free(n); }
