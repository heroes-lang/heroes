/* Defect 361, 2026-10-06: a header whose macros take names the C this
 * compiler writes uses: a runtime function, the integer literal macro of
 * <stdint.h> and the runtime's stamp, each undefined first as a header that
 * means it does, and `main`. Each rewrote the program's own C until the
 * repair, from exit 2 to a wrong value at exit 0. `LIMIT` is a macro the
 * program binds, and keeps its value. */
#include <stdint.h>
#define hero_print_int(x) nothing
#undef INT64_C
#define INT64_C(c) 0
#undef HERO_RUNTIME_ABI
#define HERO_RUNTIME_ABI 99
#define main other_main
#define LIMIT 7
static inline int64_t twice(int64_t x) { return 2 * x; }
