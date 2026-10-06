/* Defect 094, lane ffi13, 2026-10-06: the values the run cases of
   `fixedbugs-094-*` bind, each spelled as a header spells a struct's value,
   a brace list or an expression, on a header of the program's own so every
   platform runs them. */
#include <stdint.h>

struct pt { int32_t x; int32_t y; };
struct line { struct pt a; struct pt b; };

/* Shaped as Darwin's mutex: a signature, then bytes no binding names. */
struct big { int64_t sig; char opaque[56]; };

typedef union { int32_t i; float f; } Num;

static inline struct pt make_pt(void) { struct pt p = {5, 6}; return p; }
static const struct pt pt_origin = {7, 8};

static inline int64_t big_rest(struct big b) {
    int64_t sum = 0;
    for (int i = 0; i < 56; i++) sum += b.opaque[i];
    return sum;
}

#define PT_INIT {1, 2}
#define LINE_INIT {{1, 2}, {3, 4}}
#define LINE_FLAT {5, 6, 7, 8}
#define PT_Y { .y = 2 }
#define PT_TWICE { .x = 1, .y = 3, .x = 2 }
#define BIG_INIT {0x32AAABA7, {0}}
#define NUM_INIT {5}
#define NUM_F { .f = 1.5f }
#define PT_LIT (struct pt){ 3, 4 }
#define PT_CALL make_pt()
#define PT_OBJ pt_origin
#define PT_EMPTY {}
#define PT_ZERO {0}
