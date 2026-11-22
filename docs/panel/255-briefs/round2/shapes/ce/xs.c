/* A library loaded after main: a static buffer of its own. */
#include <setjmp.h>
static sigjmp_buf xs_static;
sigjmp_buf *xs_buf(void) { return &xs_static; }
