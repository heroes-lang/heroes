#include <string.h>
struct sock_path {
    unsigned short family;
    char path[108];
};
static inline int sp_send(const struct sock_path *p) { return (int)strnlen(p->path, 108); }
