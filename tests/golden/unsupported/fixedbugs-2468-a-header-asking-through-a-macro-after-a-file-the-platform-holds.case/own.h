/* Defect 2468: asks through a macro whether `<stdio.h>` exists. */
#define OWN_STDIO <stdio.h>
#if __has_include(OWN_STDIO)
#define OWN_CFG 1
#else
#define OWN_CFG 0
#endif
