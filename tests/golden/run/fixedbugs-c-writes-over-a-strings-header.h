/* The C side of defect 076's deterministic shape: a function that writes over
 * the eight bytes before the text it is lent. A `str`'s header ends there, so
 * its mark is what this destroys. The write stays inside the runtime's own
 * block, which is why AddressSanitizer says nothing and the check is the only
 * thing that notices. noinline, so clang cannot see the write from the call. */
#include <string.h>
#include <stdint.h>
static __attribute__((noinline)) void clobber_before(const char *s) {
    memset((char *)(uintptr_t)s - 8, 0, 8);
}
