/* Defect 158, lane h158, 2026-10-02: the second header, the macro again,
   then the include clang cannot open. */
#define FIXEDBUGS_158_TWICE 2
#include <no_such_header_here.h>
