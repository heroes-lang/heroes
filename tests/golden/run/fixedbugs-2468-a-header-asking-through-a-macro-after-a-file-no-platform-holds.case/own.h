/* Defect 2468: asks through a macro whether the program's `cfg.h` exists. */
/* Corrected 2026-11-11, defect 2482: the file is `cfg2482.h`, since a folder
   of the CI's Windows leg holds a `cfg.h`. */
#define OWN_CFG_HEADER "cfg2482.h"
#if __has_include(OWN_CFG_HEADER)
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
