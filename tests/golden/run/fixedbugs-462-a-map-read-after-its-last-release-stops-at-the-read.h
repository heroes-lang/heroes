/* The C side of defect 462's map shape: a map this C makes through the
 * runtime, gives back, and then asks its length, as a release written once
 * too often would leave a reader holding it. A map does not cross to C at
 * all, so the block is the C's own; what is judged is the runtime's answer to
 * the read. noinline, so clang cannot fold it. */
#include <stdint.h>
static __attribute__((noinline)) int64_t read_after_release(int64_t entries) {
    HeroMapHeader *m = hero_map_new(&hero_desc_int, &hero_desc_int, entries);
    hero_map_decref(m);
    return hero_map_len(m);
}
