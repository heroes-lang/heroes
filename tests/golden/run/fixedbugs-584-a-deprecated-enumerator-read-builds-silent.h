/* Defect 584, 2026-10-10: a deprecated enumerator, read in the program's own arithmetic. */
#include <stdint.h>
enum { OLD_LIMIT __attribute__((deprecated("use NEW_LIMIT"))) = 7 };
