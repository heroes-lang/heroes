/* The package `pk`'s own header, asking whether a file of the program's exists by a climb out of its root (defect 2351). */
#if __has_include("../../outside.h")
#define PK_V 4
#else
#define PK_V 3
#endif
