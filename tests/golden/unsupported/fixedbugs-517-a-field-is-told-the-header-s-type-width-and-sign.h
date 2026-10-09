/* Defect 517's case: a field whose name says no width (an enumeration, made
   `int` on every platform by its negative enumerator, and a typedef of an
   unsigned int), and a pointer to the struct itself, each declared at
   another type. C's `long` stays out: its width is each platform's, and this
   case's words are the same on all three. */
#include <stdint.h>
typedef enum { EV_NONE = -1, EV_A = 1 } ev_kind;
typedef uint32_t Uint32;
typedef struct ev { ev_kind kind; Uint32 stamp; struct ev *next; int32_t n; } ev;
