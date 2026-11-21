/* The package `duo`'s own header, which it ships beside its module
 * (defect 2636). */
#ifndef DUO2636_H
#define DUO2636_H
#include <stdint.h>

typedef struct duo_tally {
    int64_t n;
} duo_tally;

#define DUO_STEP 2
#endif
