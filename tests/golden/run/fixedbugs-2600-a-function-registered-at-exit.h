/* Defect 2600: the program's own C for its case on the order of the end. It
 * registers with `atexit` a function that ends the process with status 42, as
 * a library's own function at exit may end it, so the status says whether it
 * ran before the end `exit(code:)` asks for. Nothing here allocates. */
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

static void fixedbugs_2600_at_exit(void) {
    _exit(42);
}

/* 1 once the function is registered, 0 where `atexit` refused it. */
static inline int64_t register_at_exit(void) {
    return atexit(fixedbugs_2600_at_exit) == 0 ? 1 : 0;
}
