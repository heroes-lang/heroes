/* Defect 158, lane h158, 2026-10-02: the second header, the macro again,
   then the include clang cannot open; since 2026-10-10 a use of the first header's deprecated name, a macro defined again being an error (panel 204's R2). */
static inline int fixedbugs_158_use(void) { return fixedbugs_158_old; }
#include <no_such_header_here.h>
