/* Defect 2445: declares x2445 only where t2445.h was not read first. */
#ifndef T2445_READ
#include <stdint.h>
int64_t x2445(void);
#endif
#define D2445_LIMIT 4
