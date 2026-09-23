/* The C side of defect 076's second shape: a function handed a lease that
 * writes over the first eight of the sixteen bytes before its pointer, which is
 * where the lease's block keeps its mark. Inside the runtime's own block, so
 * AddressSanitizer says nothing. noinline for the reason the string case's
 * header gives. */
#include <string.h>
#include <stdint.h>
static __attribute__((noinline)) void clobber_held(const char *s) {
    memset((char *)(uintptr_t)s - 16, 0, 8);
}
