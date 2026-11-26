/* Lane p261-beside, 2026-11-26: the case's own header declares C's `fdimf`
 * by pasting the `f` onto the double function's name, as glibc's math.h
 * declares every float function, so clang places the name in its own
 * buffer, `<scratch space>`, and this file is the one that expanded it. */
#define HERO_PASTED(stem, tail) extern float stem##tail(float, float);
HERO_PASTED(fdim, f)
