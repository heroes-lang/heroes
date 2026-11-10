/* Defect 2421's route: a header of the program's own whose functions, each
   of a new name with a fixed prototype, call the variadic functions of
   `fixedbugs-2421-slots.h` through that header's own prototypes, handing
   each argument at the C type the callee reads. The program binds these, and
   each prototype holds its argument as any prototype does. */
#include "fixedbugs-2421-slots.h"

static inline long slot_read_long(int count, long value) {
    return slot_read(count, value);
}

static inline double slot_double_of(int count, double value) {
    return slot_double(count, value);
}

static inline int64_t slot_pair_of(int count, Pair p) {
    return slot_pair(count, p);
}

static inline int64_t slot_length_of(int count, const char *s) {
    return slot_length(count, s);
}

static inline int64_t slot_pointer_of(int count, void *p) {
    return slot_pointer(count, p);
}
