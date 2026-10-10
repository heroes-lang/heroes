/* Defect 571, 2026-10-10: a deprecated enumerator, bound and read. */
#include <stdint.h>
enum { OLD_LIMIT __attribute__((deprecated)) = 7 };
