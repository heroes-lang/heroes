/* The library's own header, which the package `rec` binds and does not ship
 * (defect 2636): a folder `--include` names supplies it. */
#ifndef REC2636_H
#define REC2636_H
#include <stdint.h>

typedef struct rec_pair {
    int64_t a;
    int64_t b;
} rec_pair;

#define REC_SCALE 10
#endif
