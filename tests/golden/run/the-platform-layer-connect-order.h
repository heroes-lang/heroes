/* A connect the layer's poller wakes for nothing, whose failure lands between
 * the loop's two looks, is told failed, never taken for a connection made
 * (M-rebel-library step 87b). `hero_net_try_connect` read a connection's
 * pending error and then its writability; after a wake with nothing behind it
 * (the poller's own, for a slot given back and issued again, in
 * `runtime/platform/hero_net.c`), the first look read 0 while the connection
 * was still being made, and one that failed before the second look was
 * writable to `poll` (POLLHUP on Darwin, POLLOUT with POLLERR and POLLHUP on
 * Linux), answered made, its first send told RESET: measured on Darwin and
 * on Linux arm64 before the repair, every run.
 *
 * This header is the layer itself, compiled into the program with
 * `getsockopt` wrapped, so the case does what no program of the layer's doors
 * can: it sends the poller's wake, and makes the failure land between the two
 * looks. The wrapper answers what the system said at its moment; on the first
 * read of the pending error after the wake that reads none, it makes the
 * connection fail and waits, at most 10 s, until the socket shows it, so only
 * the time between the two looks is long. The far end holds the SYN
 * unanswered: on Linux a listener on the loopback whose accept queue is full,
 * which drops it, the listener then closed so the next SYN is refused; on
 * Darwin, which completes a loopback connection past a full queue (measured),
 * 127.0.0.2, an address no interface of its holds, the connection then given
 * a time of 1 s (`TCP_CONNECTIONTIMEOUT`), so the system times it out.
 *
 * Each function answers 1 where the connect was told failed with the far
 * end's kind (REFUSED on Linux, TIMED_OUT on Darwin) and, after the wake, the
 * failure landed inside the widened look; else the negative number of the
 * step that did not hold: -1 the far end not made, -2 the connect never seen
 * waiting, -3 the failure never landed inside the look, -4 taken for a
 * connection made, -5 told failed with another kind. The connect is bounded
 * at 10 s (`hero_net_connect_within`), so a far end that never fails ends the
 * case all the same.
 *
 * Windows: built, and not run (`tests/harness/platform_skips.hero`). Its
 * connect waits in slices, `hero_net_wait_slice`, and joins no waiter list,
 * so no wake reaches it but `WSAPoll`'s own; each function answers 0 there. */
#if !defined(_WIN32)

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define ORDER_FILLERS 64

static int order_listener = -1;
static int order_fillers[ORDER_FILLERS];
static int order_filled;
/* 0 nothing asked; 1 the wake sent, the next read of no error widened; 2
 * widened. */
static _Atomic int order_armed;
/* The widened look: 1 the failure landed inside it, -1 not within 10 s. */
static _Atomic int order_landed;
/* Whether the connect was seen waiting. */
static _Atomic int order_seen;
static int order_hint;

/* The connection being made fails, by the far end's way on this system. */
static void order_fail(int fd) {
#if defined(__APPLE__)
    int secs = 1;

    (void)setsockopt(fd, IPPROTO_TCP, TCP_CONNECTIONTIMEOUT, &secs, sizeof secs);
#else
    (void)fd;
    if (order_listener >= 0) {
        (void)close(order_listener);
        order_listener = -1;
    }
#endif
}

static int order_getsockopt(int fd, int level, int name, void *value, socklen_t *size) {
    int said = getsockopt(fd, level, name, value, size);
    int armed = 1;

    if (said == 0 && level == SOL_SOCKET && name == SO_ERROR && *(int *)value == 0 &&
        atomic_compare_exchange_strong(&order_armed, &armed, 2)) {
        order_fail(fd);
        struct pollfd one = { fd, POLLOUT, 0 };

        atomic_store(&order_landed, poll(&one, 1, 10000) > 0 ? 1 : -1);
    }
    return said;
}

#define getsockopt order_getsockopt
#include "platform/hero_net.c"
#undef getsockopt

/* The slot a connect waits in for a connection made, or NULL. Under the
 * lock. */
static hero_net_slot *order_waiting(void) {
    for (uint32_t i = 0; i < hero_net_fresh; i += 1) {
        if (hero_net_slots[i].waiting[1] != NULL) {
            return &hero_net_slots[i];
        }
    }
    return NULL;
}

/* Once the connect waits, and 200 ms more so it sleeps: the poller's wake,
 * or, with no wake, the connection made to fail while it waits. */
static void *order_waker(void *unused) {
    (void)unused;
    hero_net_slot *slot = NULL;

    for (int i = 0; i < 500 && slot == NULL; i += 1) {
        hero_net_enter();
        slot = order_waiting();
        hero_net_leave();
        if (slot == NULL) {
            usleep(10000);
        }
    }
    usleep(200000);
    hero_net_enter();
    slot = order_waiting();
    hero_net_fd fd = slot != NULL ? slot->fd : HERO_NET_NO_FD;

    if (slot != NULL && order_hint) {
        atomic_store(&order_armed, 1);
        hero_net_wake_ready(slot);
    }
    hero_net_leave();
    if (fd != HERO_NET_NO_FD && !order_hint) {
        order_fail(fd);
    }
    atomic_store(&order_seen, slot != NULL ? 1 : 0);
    return NULL;
}

/* Everything the far end holds, given back. */
static void order_far_end_gone(void) {
    if (order_listener >= 0) {
        (void)close(order_listener);
        order_listener = -1;
    }
    for (int i = 0; i < order_filled; i += 1) {
        (void)close(order_fillers[i]);
    }
    order_filled = 0;
}

#if !defined(__APPLE__)
/* A connect of the header's own to the listener: 1 where it stays in
 * progress past 300 ms, the queue full; 0 where it was made and is kept,
 * filling the queue; -1 where it failed. */
static int order_filler(int64_t port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in to;

    if (fd < 0) {
        return -1;
    }
    memset(&to, 0, sizeof to);
    to.sin_family = AF_INET;
    to.sin_port = htons((uint16_t)port);
    to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    (void)fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    if (connect(fd, (struct sockaddr *)&to, sizeof to) != 0) {
        if (errno != EINPROGRESS) {
            (void)close(fd);
            return -1;
        }
        struct pollfd one = { fd, POLLOUT, 0 };

        if (poll(&one, 1, 300) == 0) {
            (void)close(fd);
            return 1;
        }
        int err = 0;
        socklen_t size = sizeof err;

        if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &size) != 0 || err != 0) {
            (void)close(fd);
            return -1;
        }
    }
    order_fillers[order_filled] = fd;
    order_filled += 1;
    return 0;
}
#endif

/* The far end that holds the SYN unanswered, its address and port; false
 * where it cannot be made. */
static int order_far_end(const char **address, int64_t *port) {
#if defined(__APPLE__)
    *address = "127.0.0.2";
    *port = 9;
    return 1;
#else
    struct sockaddr_in at;
    socklen_t at_size = sizeof at;

    order_listener = socket(AF_INET, SOCK_STREAM, 0);
    memset(&at, 0, sizeof at);
    at.sin_family = AF_INET;
    at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (order_listener < 0 || bind(order_listener, (struct sockaddr *)&at, sizeof at) != 0 || listen(order_listener, 1) != 0 ||
        getsockname(order_listener, (struct sockaddr *)&at, &at_size) != 0) {
        return 0;
    }
    *address = "127.0.0.1";
    *port = ntohs(at.sin_port);
    while (order_filled < ORDER_FILLERS) {
        int got = order_filler(*port);

        if (got != 0) {
            return got == 1;
        }
    }
    return 0;
#endif
}

#if defined(__APPLE__)
#define ORDER_KIND HERO_NET_TIMED_OUT
#else
#define ORDER_KIND HERO_NET_REFUSED
#endif

static int64_t order_run(int hint) {
    const char *address = NULL;
    int64_t port = 0;
    pthread_t waker;

    atomic_store(&order_armed, 0);
    atomic_store(&order_landed, 0);
    atomic_store(&order_seen, 0);
    order_hint = hint;
    if (!order_far_end(&address, &port) || pthread_create(&waker, NULL, order_waker, NULL) != 0) {
        order_far_end_gone();
        return -1;
    }
    hero_net_socket *made = hero_net_connect_within(address, port, 10000);
    int64_t kind = hero_net_last_error();
    int64_t answer = 1;

    (void)pthread_join(waker, NULL);
    if (atomic_load(&order_seen) != 1) {
        answer = -2;
    } else if (made != NULL) {
        answer = -4;
    } else if (hint && atomic_load(&order_landed) != 1) {
        answer = -3;
    } else if (kind != ORDER_KIND) {
        answer = -5;
    }
    if (made != NULL) {
        (void)hero_net_close(made);
    }
    order_far_end_gone();
    return answer;
}

static inline int64_t connect_failed_while_it_waits(void) {
    return order_run(0);
}

static inline int64_t connect_failed_between_two_looks_after_a_wake_for_nothing(void) {
    return order_run(1);
}

#else

#include <stdint.h>

static inline int64_t connect_failed_while_it_waits(void) {
    return 0;
}

static inline int64_t connect_failed_between_two_looks_after_a_wake_for_nothing(void) {
    return 0;
}

#endif
