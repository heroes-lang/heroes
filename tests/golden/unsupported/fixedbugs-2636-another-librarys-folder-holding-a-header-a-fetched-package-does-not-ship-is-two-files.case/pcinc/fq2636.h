/* The library `fq2636`'s own header, in the folder its `.pc` names, which
 * the package `fq` binds and does not ship (defect 2636). */
#ifndef FQ2636_H
#define FQ2636_H
#include <stdint.h>

typedef struct fq_pair {
    int64_t a;
    int64_t b;
} fq_pair;

#define FQ_SCALE 10
#endif
