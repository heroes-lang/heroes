/* Panel 178 ffi-pragmatist: the one piece of C every sockaddr binding needs
   under EVERY route, because bind/connect/getsockname take `struct sockaddr *`
   and a Heroes record is `struct sockaddr_un *`: Heroes has no cast, and the
   compiler refuses the direct binding (ffi_parameter_type, measured). */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
static inline int hero_bind_un(int fd, const struct sockaddr_un *a) { return bind(fd, (const struct sockaddr *)a, (socklen_t)sizeof *a); }
static inline int hero_connect_un(int fd, const struct sockaddr_un *a) { return connect(fd, (const struct sockaddr *)a, (socklen_t)sizeof *a); }
static inline int hero_getsockname_un(int fd, struct sockaddr_un *a) { socklen_t n = sizeof *a; return getsockname(fd, (struct sockaddr *)a, &n); }
static inline int hero_getsockname_ss(int fd, struct sockaddr_storage *s) { socklen_t n = sizeof *s; return getsockname(fd, (struct sockaddr *)s, &n); }
