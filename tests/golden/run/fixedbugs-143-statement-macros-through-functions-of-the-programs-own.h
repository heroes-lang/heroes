/* Beside defect 143's cases, lane ffimsg, 2026-10-03: the shapes the
   system cases bind from `sys/wait.h` and `sys/select.h`, on a header of
   the tree's own so every platform reads it, Windows included, which has
   neither: a macro over a status's bits, as `WEXITSTATUS` is, and macros
   over a struct of bits, the clearing one a STATEMENT, `do { ... } while
   (0)`, as glibc's `FD_ZERO` is, which only a function body can hold. The
   functions of the program's own are the repair `ffi_macro_name` names. */
#include <stdint.h>
#include <stdlib.h>

#define EXIT_STATUS_OF(s) (((s) >> 8) & 0xff)

typedef struct { uint64_t bits[2]; } bit_set;
#define BITS_ZERO(p) do { (p)->bits[0] = 0; (p)->bits[1] = 0; } while (0)
#define BITS_SET(n, p) ((p)->bits[(n) / 64] |= (uint64_t)1 << ((n) % 64))
#define BITS_ISSET(n, p) (((p)->bits[(n) / 64] >> ((n) % 64)) & 1)

static inline int hero_exit_status_of(int status) { return EXIT_STATUS_OF(status); }
static inline bit_set *hero_bits_new(void) { return calloc(1, sizeof(bit_set)); }
static inline void hero_bits_free(bit_set *p) { free(p); }
static inline void hero_bits_zero(bit_set *p) { BITS_ZERO(p); }
static inline void hero_bits_set(int n, bit_set *p) { BITS_SET(n, p); }
static inline int hero_bits_isset(int n, const bit_set *p) { return BITS_ISSET(n, p) != 0; }
