/* The package `pk`'s own header, embedding a file of the program's by a climb out of its root (defect 2555). */
#include <stdint.h>

static const unsigned char pk_blob[] = {
#embed "../../outside.bin"
};

static inline int64_t pk_first(void) {
    return (int64_t)pk_blob[0];
}
