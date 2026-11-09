/* Defect 2388: the shapes beside a `const` member in an array's element that
 * C11 lets a program assign, each built and handed back by value. */
#include <stdint.h>

/* No `const` in the element. */
struct fixedbugs_2388_cell { int32_t k; int32_t v; };
typedef struct fixedbugs_2388_plain { struct fixedbugs_2388_cell items[2]; int32_t n; } fixedbugs_2388_plain;

/* `const` only in a pointer's pointee: the element's, the array's, and a
 * pointer to an array of records that hold one. */
struct fixedbugs_2388_locked { const int32_t k; int32_t v; };
typedef struct fixedbugs_2388_pointing {
    int32_t n;
    struct { const int32_t *q; int32_t z; } e[2];
    const struct fixedbugs_2388_locked *p[2];
    struct fixedbugs_2388_locked (*pa)[2];
} fixedbugs_2388_pointing;

static inline fixedbugs_2388_plain fixedbugs_2388_plain_make(void) { fixedbugs_2388_plain v = { { { 1, 2 }, { 3, 4 } }, 5 }; return v; }
static inline fixedbugs_2388_pointing fixedbugs_2388_pointing_make(void) { fixedbugs_2388_pointing v = { 6 }; return v; }
