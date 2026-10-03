/* Beside defect 171's cases, lane ffimsg, 2026-10-03: a function pointer
   whose pointee takes two parameters. */
#include <stdint.h>

static int64_t (*combine)(int64_t, int64_t) = 0;
