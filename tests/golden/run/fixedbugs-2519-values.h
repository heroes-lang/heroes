/* Defect 2519, 2026-11-13: functions taken as values by a program whose
 * spelling of a parameter is another C type of the same meaning: a typed
 * pointer bound as `ptr`, and `size_t` bound as `u64`, two types on Darwin
 * (`unsigned long` and `unsigned long long`) and one on glibc. */
#include <stddef.h>

static inline int set_two2519(unsigned *p) {
    *p = 2u;
    return 0;
}

static inline unsigned read2519(const unsigned *p) {
    return *p;
}

static inline size_t twice2519(size_t n) {
    return n * 2u;
}

static inline int add_one2519(int a) {
    return a + 1;
}

/* Whether the value a program handed C is the function itself. */
static inline int is_add_one2519(int (*f)(int)) {
    return f == add_one2519;
}
