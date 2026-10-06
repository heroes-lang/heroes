#include <string.h>
#include <sys/un.h>
static inline struct sockaddr_un sockaddr_un_zero(void) { struct sockaddr_un a; memset(&a, 0, sizeof a); return a; }
