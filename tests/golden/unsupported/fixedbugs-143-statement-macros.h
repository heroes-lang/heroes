/* Beside defect 143's cases, lane ffimsg, 2026-10-03: the macros of
   `sys/wait.h` and `sys/select.h` the system cases bind, in the shapes
   those cases need, on a header of the tree's own so no platform lacks it:
   a macro over a status's bits, as `WEXITSTATUS` is, and macros over a
   struct of bits, the clearing one a STATEMENT, `do { ... } while (0)`, as
   glibc's `FD_ZERO` is. */
#include <stdint.h>

#define EXIT_STATUS_OF(s) (((s) >> 8) & 0xff)

typedef struct { uint64_t bits[2]; } bit_set;
#define BITS_ZERO(p) do { (p)->bits[0] = 0; (p)->bits[1] = 0; } while (0)
#define BITS_SET(n, p) ((p)->bits[(n) / 64] |= (uint64_t)1 << ((n) % 64))
#define BITS_ISSET(n, p) (((p)->bits[(n) / 64] >> ((n) % 64)) & 1)
