/* A header of the program's own, beside its `main.hero`, named like the one
 * the package `rec` binds and does not ship (defect 2636). */
#ifndef REC2636_H
#define REC2636_H
#include <stdint.h>

typedef struct rec_pair {
    int64_t a;
    int64_t b;
} rec_pair;

#define REC_SCALE 100
#endif
