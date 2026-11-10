/* Declares `extra` only where `WANT_EXTRA` is defined before it (defect 2429). */
#ifndef FIXEDBUGS_2429_LIB_H
#define FIXEDBUGS_2429_LIB_H
#ifdef WANT_EXTRA
static inline long long extra(void) { return 7; }
#endif
static inline long long plain(void) { return 1; }
#endif
