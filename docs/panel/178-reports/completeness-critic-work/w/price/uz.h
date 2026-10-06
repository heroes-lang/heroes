#include <string.h>
#include <sys/utsname.h>
static inline struct utsname utsname_zero(void) { struct utsname u; memset(&u, 0, sizeof u); return u; }
