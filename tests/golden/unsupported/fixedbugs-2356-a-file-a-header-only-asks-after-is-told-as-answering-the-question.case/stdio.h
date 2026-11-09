/* Defect 2356: a stand-in for the library's `<stdio.h>` that changes what it reads. */
#include_next <stdio.h>
#undef SEEK_END
#define SEEK_END 7
