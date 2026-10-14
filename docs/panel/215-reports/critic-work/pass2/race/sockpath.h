/* The critic's stand-in for the experiment's sockpath.h, panel 215 pass 1. */
#include <string.h>
struct sock_path {
    unsigned short family;
    char path[108];
    union { int fd; void *handle; } via;
};
static inline int sp_send(const struct sock_path *p) { return (int)strnlen(p->path, sizeof p->path); }
