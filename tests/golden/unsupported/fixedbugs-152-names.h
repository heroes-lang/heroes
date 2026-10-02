/* Beside the cases of defect 152, lane ffi-macro, 2026-10-02: every kind of
   name a header can hold that is not a function, one of each, so each case
   can declare one as a function. Beside its cases rather than a system
   header, so no platform lacks it and every platform spells it alike. */
#include <stdint.h>

/* a value: an enumerator, and a number by #define */
enum { RED = 1, BLUE = 2 };
#define LIMIT 100

/* an object: a variable of a struct, and one of an integer */
struct pair { int32_t a; int32_t b; };
static struct pair the_pair = { 1, 2 };
static int32_t counter = 7;

/* a type */
typedef int32_t my_int;
