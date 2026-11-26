/* Lane p261-beside, 2026-11-26 (panel 245's R3): a header of the case's own
 * declaring a name the platform's zlib defines, standing for another
 * release's header, and writing the name by pasting it, as glibc's math.h
 * writes every float function: clang places the name in its own buffer,
 * `<scratch space>`, and the header is the file that expanded the macro. */
#define HERO_PASTED(stem, tail) const char *stem##tail(void);
HERO_PASTED(zlib, Version)
