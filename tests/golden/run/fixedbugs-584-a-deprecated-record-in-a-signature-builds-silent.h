/* Defect 584, 2026-10-10: a deprecated struct, a parameter's type and main's temporary. */
#include <stdint.h>
struct __attribute__((deprecated("use new_pair"))) old_pair { int64_t a; };
