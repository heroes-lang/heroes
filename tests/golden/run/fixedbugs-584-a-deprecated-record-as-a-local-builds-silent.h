/* Defect 584, 2026-10-10: a deprecated struct, a local of the program's own. */
#include <stdint.h>
struct __attribute__((deprecated("use new_pair"))) old_pair { int64_t a; };
