/* Beside defect 428's cases: functions with two `char` pointers, both written
   by the first, one read and one written by the second. */
#include <stdint.h>

static inline int32_t w_two(char *a, unsigned char *b) { a[0] = 'Y'; b[0] = 'Z'; return 1; }
static inline int32_t w_second(const char *a, unsigned char *b) { b[0] = (unsigned char)a[0]; return 1; }
