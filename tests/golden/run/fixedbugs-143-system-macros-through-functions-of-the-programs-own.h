/* Beside defect 143's second run case, lane ffi-macro, 2026-10-02 (panel
   185's ffi-pragmatist ran its shape on macOS and in the Linux container):
   functions of the program's own over `WEXITSTATUS` and the `fd_set` macros,
   glibc's `FD_ZERO` a statement, `do { ... } while (0)`, which only a
   function body can hold. The types are the macros' documented ones. */
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/select.h>

static inline int hero_wexitstatus(int status) { return WEXITSTATUS(status); }
static inline fd_set *hero_fd_new(void) { return calloc(1, sizeof(fd_set)); }
static inline void hero_fd_free(fd_set *p) { free(p); }
static inline void hero_fd_zero(fd_set *p) { FD_ZERO(p); }
static inline void hero_fd_set(int fd, fd_set *p) { FD_SET(fd, p); }
static inline int hero_fd_isset(int fd, const fd_set *p) { return FD_ISSET(fd, p) != 0; }
