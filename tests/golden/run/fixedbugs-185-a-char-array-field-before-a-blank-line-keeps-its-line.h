/* Defect 185's first shape: a tagged struct whose last member is an array of
   a signed byte, `int8_t` so the element's sign is the same on every
   platform (a plain `char` is not). */
#include <stdint.h>

struct label {
    int32_t id;
    int8_t name[8];
};

static inline int32_t label_id(struct label l) { return l.id; }
