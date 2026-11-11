/* Defect 2468: asks in quotes whether its own `cfg.h` beside it exists. */
/* Corrected 2026-11-11, defect 2482: the file is `cfg2482.h`, since a folder
   of the CI's Windows leg holds a `cfg.h`. */
#if __has_include("cfg2482.h")
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
