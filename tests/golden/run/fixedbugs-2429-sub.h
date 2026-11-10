/* Declares `sub_extra` only where `WANT_SUB` is defined before it; read by
 * `fixedbugs-2429-earlier.h` and `fixedbugs-2429-top.h` both (defect 2429). */
#ifndef FIXEDBUGS_2429_SUB_H
#define FIXEDBUGS_2429_SUB_H
#ifdef WANT_SUB
static inline long long sub_extra(void) { return 3; }
#define SUB_MAX 4
struct sub_rec { long long a; };
#endif
static inline long long sub_plain(void) { return 1; }
#endif
