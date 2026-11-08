/* Defect 2334's record: one fixed array of 8,192 bytes, not `partial`, every
 * byte set to its index modulo 251 so each read has a known value. */
#include <stdint.h>

typedef struct fixedbugs_2334_big {
    uint8_t f0[8192];
} fixedbugs_2334_big;

static fixedbugs_2334_big fixedbugs_2334_make(void) {
    fixedbugs_2334_big b = {{0}};
    for (int i = 0; i < 8192; i++) b.f0[i] = (uint8_t)(i % 251);
    return b;
}
