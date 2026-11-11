/* Defect 2464, 2026-11-10: defect 584's door, with fortification off, whose
 * `sprintf` this Mac's SDK marks deprecated. */
#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <stdlib.h>
static inline int print_number(char *buf, long long n) {
    return sprintf(buf, "%lld", n);
}
