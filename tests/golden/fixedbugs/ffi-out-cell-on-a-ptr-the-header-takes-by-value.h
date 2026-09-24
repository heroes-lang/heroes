/* A C function that writes eight bytes through the pointer it is handed. With
 * `@p: ptr` the program hands it the address of its own cell, and the eight
 * bytes land in the program's stack at exit 0. */
#include <string.h>
static inline __attribute__((noinline)) void wipe(void *p) { memset(p, 0, 8); }
