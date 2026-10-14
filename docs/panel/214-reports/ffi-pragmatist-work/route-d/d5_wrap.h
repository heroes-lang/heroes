/* A header of the program's own reaching a function-like macro (design.md
 * §4.19, panel 185 R1), and a header constant. */
#include <sys/wait.h>
#include <sqlite3.h>
static inline int exit_status(int status) { return WEXITSTATUS(status); }
