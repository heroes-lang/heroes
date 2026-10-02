#include <stdint.h>
/* a header macro that is not a member of the struct beside it */
#define size 4
typedef struct { int32_t len; int32_t cap; } BUF;
static inline BUF make_buf(void) { BUF b = {2, 8}; return b; }
