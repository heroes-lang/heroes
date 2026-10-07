/* The C side of defect 314's array shape: an array this C makes through the
 * runtime and releases twice, as a binding that gives back what it does not
 * hold would. The program cannot hand C an array's own block (a lend is a
 * copy), so the block is the C's own; what is judged is the runtime's answer
 * to the second release. noinline, so clang cannot fold the two calls. */
#include <stdint.h>
static __attribute__((noinline)) void release_twice(int64_t elements) {
    HeroArrayHeader *a = hero_array_new(&hero_desc_int, elements);
    hero_array_decref(a);
    hero_array_decref(a);
}
