/* The library's folder holds a stdio.h that reads the C library's and
   changes it: the ffi-pragmatist's `pcstdio` (panel 243). */
#include_next <stdio.h>
#undef BUFSIZ
#define BUFSIZ 7
