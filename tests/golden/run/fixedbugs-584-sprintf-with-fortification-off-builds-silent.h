/* Defect 584, 2026-10-10: fortification off, so the program's call of
   `sprintf` names the C library's own declaration, which this Mac's SDK
   marks deprecated, rather than the fortified macro. */
#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <stdlib.h>
