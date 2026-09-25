/* Panel 175's critic: cJSON's ownership contract in miniature, the header
 * panel 177's reproducers ran over (`docs/panel/177-briefs/cj.h`).
 * AddItemToObject TRANSFERS item into object; Delete frees a node and every
 * child it owns. Shipped beside the case so the extern block reads against a
 * header that exists; `check` never opens it. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
typedef struct cJSON { struct cJSON *child; struct cJSON *next; } cJSON;
static inline __attribute__((noinline)) cJSON *cJSON_CreateObject(void) { return (cJSON *)calloc(1, sizeof(cJSON)); }
static inline __attribute__((noinline)) int64_t cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item) {
    (void)string; item->next = object->child; object->child = item; return 1; }
static inline __attribute__((noinline)) void cJSON_Delete(cJSON *item) {
    while (item != NULL) { cJSON *next = item->next; if (item->child) cJSON_Delete(item->child); free(item); item = next; } }
