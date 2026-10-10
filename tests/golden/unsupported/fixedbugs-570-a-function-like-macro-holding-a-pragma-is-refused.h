/* Defect 570, 2026-10-10: a function-like macro that holds a pragma. */
#include <stdint.h>
typedef struct {
    unsigned char nsap[8];
    int64_t id;
} Slot;
static inline Slot slot_make(void) {
    Slot s = { {72, 105, 0, 0, 0, 0, 0, 0}, 7 };
    return s;
}
static inline void slot_fill(void *p, int64_t n) {
    unsigned char *b = (unsigned char *)p;
    for (int64_t i = 0; i < n; i++) b[i] = (unsigned char)(65 + i);
}
static inline int64_t slot_sum(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}
static inline int my_twice(int x) { return 2 * x; }
#define my_twice(x) (_Pragma("clang diagnostic ignored \"-Wincompatible-pointer-types-discards-qualifiers\"") my_twice(x))
