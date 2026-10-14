/* ffi-pragmatist, panel 215, Q3: the three census structs of measurement 044 and the macros a
   program reaches them through (design.md §4.19: a function-like macro through static inline). */
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
static inline void fds_add(int fd, fd_set *s) { FD_SET(fd, s); }
static inline int fds_has(int fd, const fd_set *s) { return FD_ISSET(fd, s) ? 1 : 0; }
static inline int select_now(int nfds, fd_set *r) { struct timeval tv = {0, 0}; return select(nfds, r, NULL, NULL, &tv); }
