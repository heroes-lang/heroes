/* The platform layer's handles, asked in C what Heroes' own handle marks
 * refuse before C runs (feature 2295, 2026-11-12): a socket closed twice,
 * NULL, a value the layer never issued, a socket whose slot a later one took,
 * and a count with no bytes behind it. Each answers -1 and names its kind,
 * never a read of freed memory and never another socket's descriptor. The
 * header reaches `hero_net.h` on the runtime's include path, and the linker
 * takes the member for the calls below. */
#include "hero_net.h"

#include <stdint.h>

/* The kind a refused call left, or a negative number saying which step did
 * not go as the layer promises. */
static inline int64_t closed_twice(void) {
    hero_net_socket *s = hero_net_listen("127.0.0.1", 0);

    if (s == NULL) return -100;
    if (hero_net_close(s) != 0) return -101;
    if (hero_net_close(s) != -1) return -102;
    return hero_net_last_error();
}

static inline int64_t null_handle(void) {
    if (hero_net_close(NULL) != -1) return -100;
    if (hero_net_last_error() != HERO_NET_NOT_OPEN) return -101;
    if (hero_net_port(NULL) != -1) return -102;
    if (hero_net_accept(NULL) != NULL) return -103;
    return hero_net_last_error();
}

static inline int64_t forged_handle(void) {
    hero_net_socket *forged = (hero_net_socket *)(uintptr_t)UINT64_C(0x0000000700012345);
    unsigned char byte = 0;

    if (hero_net_send(forged, &byte, 1) != -1) return -100;
    if (hero_net_recv(forged, &byte, 1) != -1) return -101;
    return hero_net_last_error();
}

/* A socket closed, then another opened, which may take its slot: the first
 * handle names a life that ended, and asks nothing of the second's. */
static inline int64_t stale_after_reuse(void) {
    hero_net_socket *first = hero_net_listen("127.0.0.1", 0);

    if (first == NULL) return -100;
    if (hero_net_close(first) != 0) return -101;
    hero_net_socket *second = hero_net_listen("127.0.0.1", 0);

    if (second == NULL) return -102;
    if (second == first) return -103;
    int64_t stale = hero_net_port(first);
    int64_t kind = hero_net_last_error();

    if (hero_net_port(second) <= 0) return -104;
    if (hero_net_close(first) != -1) return -105;
    if (hero_net_close(second) != 0) return -106;
    return stale == -1 ? kind : -107;
}

static inline int64_t bytes_not_there(void) {
    hero_net_socket *s = hero_net_listen("127.0.0.1", 0);

    if (s == NULL) return -100;
    int64_t sent = hero_net_send(s, NULL, 5);
    int64_t kind = hero_net_last_error();

    if (hero_net_close(s) != 0) return -101;
    return sent == -1 ? kind : -102;
}

/* A call that succeeds leaves no kind behind. */
static inline int64_t cleared_after_success(void) {
    (void)hero_net_close(NULL);
    hero_net_socket *s = hero_net_listen("127.0.0.1", 0);

    if (s == NULL) return -100;
    int64_t kind = hero_net_last_error();

    if (hero_net_close(s) != 0) return -101;
    return kind;
}
