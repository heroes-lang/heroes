/* The program's own `fq2636.h`, beside `main.hero`, which alone declares
 * `OWN_ONLY` (defect 2638). */
#ifndef FQ2636_H
#define FQ2636_H
#include <stdint.h>

typedef struct fq_pair {
    int64_t a;
    int64_t b;
} fq_pair;

#define FQ_SCALE 100
#define OWN_ONLY 5
#endif
