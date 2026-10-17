/* Defect 2112: a C library that keeps the address of a record it is handed,
   and reads its padding later, through that address, with no call between. */
#include <stdint.h>
typedef struct { uint8_t c; int64_t n; } Kept;   /* bytes 1 to 7 are padding */
static Kept *kept_at;
static inline int64_t keep_it(Kept *k) { kept_at = k; return 0; }
static inline int64_t padding_set(void) {
    const unsigned char *b = (const unsigned char *)kept_at;
    int64_t set = 0;
    for (int i = 1; i < 8; i++) set += b[i] != 0;
    return set;
}
