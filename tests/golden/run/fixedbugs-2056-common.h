/* Defect 2056, 2026-10-14: one file two groups' headers include, read once,
 * at the first; one of its macros only under the switch. */
#ifndef FIXEDBUGS_2056_COMMON_H
#define FIXEDBUGS_2056_COMMON_H
#define LIMIT 7
#ifdef WANT_EXTRA
#define EXTRA 9
#endif
#endif
