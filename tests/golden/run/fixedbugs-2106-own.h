/* Defect 2106's route: a header of the program's own, whose function takes
   the pointer at the type the format writes and hands it to the C library's
   variadic `sscanf`. Its prototype is what holds the width. */
#include <stdint.h>
#include <stdio.h>

typedef struct {
    int32_t a;
    int32_t b;
} Pair;

static inline int32_t scan_int(const char *text, int32_t *n) {
    return (int32_t)sscanf(text, "%d", n);
}
