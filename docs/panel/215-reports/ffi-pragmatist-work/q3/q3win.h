/* ffi-pragmatist, panel 215, Q3 on Windows: Winsock's sockaddr and fd_set. */
#include <winsock2.h>
static inline int wsa_start(void) { WSADATA d; return WSAStartup(MAKEWORD(2, 2), &d); }
static inline void fds_add(SOCKET s, fd_set *set) { FD_SET(s, set); }
static inline int fds_count(const fd_set *set) { return (int)set->fd_count; }
