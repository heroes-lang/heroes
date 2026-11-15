/* The package `pk`'s own header, embedding its own data file (defect 2555). */
#include <stdint.h>

static const unsigned char pk_blob[] = {
#embed "data.bin"
};

static inline int64_t pk_first(void) {
    return (int64_t)pk_blob[0];
}
