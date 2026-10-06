/* A C struct with a `char[4]` field, beside the case that binds it. */
#include <stdint.h>

typedef struct {
    char name[4];
    int32_t id;
} Tag;
