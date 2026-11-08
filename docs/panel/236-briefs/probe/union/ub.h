#include <stdint.h>
typedef struct ub {
    union {
        int64_t a[300];
        int64_t b[300];
    };
} ub;
