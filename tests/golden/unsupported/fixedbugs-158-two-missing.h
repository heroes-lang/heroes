/* Defect 158, lane h158, 2026-10-02: two includes no machine has. */
#include <stdint.h>
#include <first_missing_here.h>
#include <second_missing_here.h>

static inline int32_t seven(void) { return 7; }
