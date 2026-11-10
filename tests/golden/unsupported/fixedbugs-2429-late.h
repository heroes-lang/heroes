/* Declares, read after `fixedbugs-2429-empty.h`, what a group naming that
 * header binds (defect 2429). */
#ifndef FIXEDBUGS_2429_LATE_H
#define FIXEDBUGS_2429_LATE_H
#define LATE_MAX 4
struct late { long long y; };
static inline long long late_first(void) { return 1; }
static inline long long late_count(void) { return 2; }
#endif
