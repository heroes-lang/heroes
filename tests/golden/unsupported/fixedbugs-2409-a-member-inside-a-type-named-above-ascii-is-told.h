/* Defect 2409's shape beside it: the bound field `fd` stands inside the
   member `via`, whose type is a typedef of a union named `unioné`; the round
   that finds the member asked the type's first word by a cut at `union `'s
   length, inside `é`. */
#include <stdint.h>
typedef union { int32_t fd; void *p; } unioné;
struct tagged { int32_t kind; unioné via; };
