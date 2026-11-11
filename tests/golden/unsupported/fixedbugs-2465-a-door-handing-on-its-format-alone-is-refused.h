/* Defect 2465, panel 245's R2, 2026-11-11: a door handing on the format it
 * was given, with no argument after it. */
#include <stdio.h>
static inline int say(const char *f) { int n = printf(f); fflush(stdout); return n; }
