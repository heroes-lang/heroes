/* The C side of defect 462's string shape: a function that releases every
 * reference the program holds to the string it is lent, as a C library that
 * frees what it was handed would, so the block is given back while the
 * program still names it. The count is read once, before the first release,
 * so nothing here reads the block after it is gone; the program's next read
 * is the first. noinline, so clang cannot see the releases from the call. */
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>
static __attribute__((noinline)) void release_all(const char *s) {
    HeroStr held = {s, (int64_t)strlen(s)};
    const HeroStrHeader *h = (const HeroStrHeader *)(const void *)(s - sizeof(HeroStrHeader));
    int64_t count = atomic_load_explicit(&h->refcount, memory_order_relaxed);
    for (int64_t k = 0; k < count; k++) hero_str_decref(held);
}
