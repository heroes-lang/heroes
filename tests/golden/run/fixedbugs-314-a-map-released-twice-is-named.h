/* The C side of defect 314's map shape: a map this C makes through the
 * runtime and releases twice, as a binding that gives back what it does not
 * hold would. A map does not cross to C at all, so the block is the C's own;
 * what is judged is the runtime's answer to the second release. noinline, so
 * clang cannot fold the two calls. */
#include <stdint.h>
static __attribute__((noinline)) void release_twice(int64_t entries) {
    HeroMapHeader *m = hero_map_new(&hero_desc_int, &hero_desc_int, entries);
    hero_map_decref(m);
    hero_map_decref(m);
}
