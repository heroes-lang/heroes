/* Defect 2409's case: a struct whose tag holds a character above ASCII,
   which C11 admits in an identifier and clang reads as UTF-8, held by the
   bound record's field `c`, declared here at another type. */
#include <stdint.h>
struct café { int32_t a; };
struct holder { int32_t kind; struct café c; };
