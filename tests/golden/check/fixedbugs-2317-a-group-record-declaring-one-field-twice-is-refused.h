/* Defect 2317's group record: one member `a`, which the binding beside this
 * header declares twice. */
#include <stdint.h>

typedef struct fixedbugs_2317_pair {
    int64_t a;
    int64_t b;
} fixedbugs_2317_pair;
