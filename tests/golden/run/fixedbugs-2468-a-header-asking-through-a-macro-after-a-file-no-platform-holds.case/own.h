/* Defect 2468: asks through a macro whether the program's `cfg.h` exists. */
#define OWN_CFG_HEADER "cfg.h"
#if __has_include(OWN_CFG_HEADER)
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
