/* Defect 2468: asks in quotes after a name its own folder does not hold. */
#if __has_include("stdio.h")
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
