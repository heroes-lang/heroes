/* A file of the library `aa2636`'s folder named like the header the fetched
 * package `fq` binds and does not ship, with another layout (defect 2636). */
#ifndef FQ2636_H
#define FQ2636_H
#include <stdint.h>

typedef struct fq_pair {
    int64_t b;
    int64_t a;
    int64_t spare;
} fq_pair;

#define FQ_SCALE 10
#endif
