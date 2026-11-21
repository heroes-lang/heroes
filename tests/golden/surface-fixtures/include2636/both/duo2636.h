/* A file named like the package `duo`'s own header, with another layout
 * (defect 2636): a folder `--include` names would stand in front of the
 * package's file. */
#ifndef DUO2636_H
#define DUO2636_H
#include <stdint.h>

typedef struct duo_tally {
    int32_t n;
    int32_t spare;
    int64_t more;
} duo_tally;

#define DUO_STEP 5
#endif
