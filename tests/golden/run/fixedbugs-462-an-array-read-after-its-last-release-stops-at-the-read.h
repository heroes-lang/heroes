/* The C side of defect 462's array shape: an array this C makes through the
 * runtime, gives back, and then asks its length, as a release written once too
 * often would leave a reader holding it. The program cannot hand C an array's
 * own block (a lend is a copy), so the block is the C's own; what is judged is
 * the runtime's answer to the read. noinline, so clang cannot fold it. */
#include <stdint.h>
static __attribute__((noinline)) int64_t read_after_release(int64_t elements) {
    HeroArrayHeader *a = hero_array_new(&hero_desc_int, elements);
    int64_t value = 5;
    for (int64_t k = 0; k < elements; k++) {
        HeroArrayHeader *longer = hero_array_push(a, &value);
        hero_array_decref(a);
        a = longer;
    }
    hero_array_decref(a);
    return hero_array_len(a);
}
