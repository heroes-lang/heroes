/* Compiler-engineer, panel 215: a C function that returns small.h's padded
   record by value, every byte zeroed first; noinline so no level folds it. */
#include <string.h>
#include "small.h"
__attribute__((noinline)) static struct cd cd_make(void) {
    struct cd r; memset(&r, 0, sizeof r); r.c = 1; r.d = 2.0; return r;
}
