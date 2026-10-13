/* What route E would emit if a Heroes local kept the author's name as its C name.
   Each line is a plausible Heroes binding: `stdin = 1`, `errno = 3`, `linux = 4`. */
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include "/opt/homebrew/include/raylib.h"
int64_t f_stdin(void) { int64_t stdin = 1; return stdin; }
#ifdef T_ERRNO
int64_t f_errno(void) { int64_t errno = 3; return errno; }
#endif
#ifdef T_LINUX
int64_t f_linux(void) { int64_t linux = 4; return linux; }
#endif
#ifdef T_TRUE
int64_t f_true(void) { int64_t true = 5; return true; }
#endif
int main(void) { return (int)f_stdin(); }
