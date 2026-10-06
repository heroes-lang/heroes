/* Critic, panel 178: route C, the binding's own header returns the value.
   The zero is written in C, where the struct is; so is the library's own
   initialiser, where one exists. No Heroes form, no compiler line. */
#include <string.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
static inline struct utsname hero_utsname_zero(void) { struct utsname u; memset(&u, 0, sizeof u); return u; }
static inline struct sockaddr_un hero_un_zero(void) { struct sockaddr_un a; memset(&a, 0, sizeof a); a.sun_family = AF_UNIX; return a; }
static inline pthread_mutex_t hero_mutex_new(void) { return (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; }
static inline int hero_bind_un(int fd, const struct sockaddr_un *a) { return bind(fd, (const struct sockaddr *)a, (socklen_t)sizeof *a); }
static inline int hero_connect_un(int fd, const struct sockaddr_un *a) { return connect(fd, (const struct sockaddr *)a, (socklen_t)sizeof *a); }
