#include <stdint.h>
typedef struct { int32_t a; int32_t b; int32_t c; } S3;
static inline S3 make_s3(void) { S3 s = {1, 2, 3}; return s; }
