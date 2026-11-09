/* Defect 2388: group records whose `const` member lies inside an ELEMENT of
 * an array member, which C11 makes no modifiable lvalue (6.3.2.1p1) and
 * clang 21 and 22 assign without a word; each also handed back by value. */
#include <stdint.h>

struct fixedbugs_2388_inner { const int32_t k; int32_t v; };

/* The reproducer: a `const` member in the element of an array member. */
typedef struct fixedbugs_2388_outer { struct fixedbugs_2388_inner items[2]; int32_t n; } fixedbugs_2388_outer;

/* Two arrays deep: the element holds an array whose element holds it. */
struct fixedbugs_2388_mid { int32_t m; struct fixedbugs_2388_inner leaves[2]; };
typedef struct fixedbugs_2388_deep { int32_t n; struct fixedbugs_2388_mid two[2]; } fixedbugs_2388_deep;

/* An array in an arm of an anonymous union. */
typedef struct fixedbugs_2388_arm { int32_t n; union { int32_t x; struct fixedbugs_2388_inner u[2]; }; } fixedbugs_2388_arm;

/* A 2-D array, its rows a typedef, its element an anonymous struct. */
typedef struct { const int32_t k; int32_t v; } fixedbugs_2388_cell;
typedef fixedbugs_2388_cell fixedbugs_2388_row[3];
typedef struct fixedbugs_2388_grid { int32_t n; fixedbugs_2388_row rows[2]; } fixedbugs_2388_grid;

static inline fixedbugs_2388_outer fixedbugs_2388_outer_make(void) { fixedbugs_2388_outer v = { { { 1, 2 }, { 3, 4 } }, 5 }; return v; }
static inline fixedbugs_2388_deep fixedbugs_2388_deep_make(void) { fixedbugs_2388_deep v = { 1 }; return v; }
static inline fixedbugs_2388_arm fixedbugs_2388_arm_make(void) { fixedbugs_2388_arm v = { 1 }; return v; }
static inline fixedbugs_2388_grid fixedbugs_2388_grid_make(void) { fixedbugs_2388_grid v = { 1 }; return v; }
