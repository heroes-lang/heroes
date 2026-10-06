#include <stdint.h>
#include <string.h>
struct one { int32_t a; int32_t b; };
__attribute__((noinline)) static int64_t touch(void *buf, uint64_t n) { ((unsigned char *)buf)[0] ^= (unsigned char)n; return (int64_t)n; }
