/* Defect 2468: the program's own `stdio.h`, which only `own.h` asks after. */
#include_next <stdio.h>
#undef SEEK_END
#define SEEK_END 7
