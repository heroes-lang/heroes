/* Defect 2468: asks in quotes whether its own `stdio.h` beside it exists. */
#if __has_include("stdio.h")
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
