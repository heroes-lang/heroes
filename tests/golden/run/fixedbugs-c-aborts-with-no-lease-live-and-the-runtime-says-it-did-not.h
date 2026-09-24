/* A C function that ends the program itself, the way a library does when it
 * catches misuse. noinline so the call stays a call. */
#include <stdlib.h>
static __attribute__((noinline)) void give_up(void) { abort(); }
