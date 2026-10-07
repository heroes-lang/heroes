/* The C side of defect 314's shape: a function that releases the string it
 * is lent, one reference the program still holds, as a C library given the
 * string under a mark that is not true of it (`consumes`, `owned`) would.
 * The program's own release then finds the block already given back. The
 * `HeroStr` is the program's own text pointer with its length, so the block
 * it names is the real one. noinline, so clang cannot see the release from
 * the call. */
#include <string.h>
#include <stdint.h>
static __attribute__((noinline)) void release_behind(const char *s) {
    HeroStr held = {s, (int64_t)strlen(s)};
    hero_str_decref(held);
}
