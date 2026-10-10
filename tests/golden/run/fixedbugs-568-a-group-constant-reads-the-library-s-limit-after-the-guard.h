/* Defect 568, 2026-10-10: constants whose macros name the library's own limits. */
#include <stdint.h>
#include <math.h>
#define CASE_LIMIT INT32_MAX
#define CASE_HUGE INFINITY
#define CASE_LOW INT64_MIN
