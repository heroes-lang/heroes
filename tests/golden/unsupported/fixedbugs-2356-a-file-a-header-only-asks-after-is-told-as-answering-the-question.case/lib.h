/* Defect 2356: asks whether the library has `<stdio.h>`, and includes nothing. */
#if __has_include(<stdio.h>)
#define HAS 1
#else
#define HAS 0
#endif
