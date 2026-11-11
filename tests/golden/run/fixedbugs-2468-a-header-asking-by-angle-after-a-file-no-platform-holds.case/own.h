/* Defect 2468: asks by angle whether the program's `cfg.h` exists. */
#if __has_include(<cfg.h>)
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
