/* Beside the case, and the struct is TAGGED on purpose: `struct tagged` is two
   words, and two words is exactly what broke the marker this case guards
   (defect 062). An untagged typedef would have hidden it. */
#include <stdint.h>

struct tagged {
    char name[16];
    int32_t id;
};

static inline int32_t tagged_id(struct tagged t) { return t.id; }
