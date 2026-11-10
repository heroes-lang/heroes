/* Defect 2409's shape beside it: a typedef whose name opens with `union`
   and goes on above ASCII, so the type text clang spells is `unioné`, seven
   bytes, and a cut at `union `'s six fell inside `é`. */
#include <stdint.h>
typedef double unioné;
struct holder { int32_t kind; unioné c; };
