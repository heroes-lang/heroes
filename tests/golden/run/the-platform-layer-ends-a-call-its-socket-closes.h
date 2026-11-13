/* A close of a socket another thread waits on ends the wait, failing
 * HERO_NET_NOT_OPEN, and nothing reaches the socket after it (feature 2295,
 * 2026-11-12; sitting 246's ffi-pragmatist found the listener's half broken
 * on Darwin, where `accept` went on waiting and took a connection made after
 * the close, and the Windows box the `recv`'s, where a shutdown did not end
 * it). Three waits: an `accept` for a connection, a `recv` for bytes, and a
 * `send` for room in a buffer a peer that never reads has filled. The
 * waiting thread is C's own, POSIX's or Windows', calling the
 * layer and nothing of Heroes. Each function answers 1 where every step held,
 * else the negative number of the step that did not; a thread still waiting
 * after 3 s is freed (a connection, a byte) so the program ends either way. */
#include "hero_net.h"

#include <stdatomic.h>
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

static hero_net_socket *waiter_on;
static hero_net_socket *waiter_took;
static int64_t waiter_answer;
static int64_t waiter_kind;
static _Atomic int waiter_done;

static void waiter_accept(void) {
    waiter_took = hero_net_accept(waiter_on);
    waiter_kind = hero_net_last_error();
    atomic_store(&waiter_done, 1);
}

static void waiter_recv(void) {
    unsigned char buffer[16];

    waiter_answer = hero_net_recv(waiter_on, buffer, sizeof buffer);
    waiter_kind = hero_net_last_error();
    atomic_store(&waiter_done, 1);
}

static unsigned char waiter_bytes[65536];
static int64_t waiter_sent;

/* Sends until a send fails: the peer reads nothing, so the buffer fills and
 * a send waits for room, until the close ends it. */
static void waiter_send(void) {
    int64_t n = 0;

    waiter_sent = 0;
    while ((n = hero_net_send(waiter_on, waiter_bytes, sizeof waiter_bytes)) > 0) {
        waiter_sent += n;
    }
    waiter_answer = n;
    waiter_kind = hero_net_last_error();
    atomic_store(&waiter_done, 1);
}

static void waiter_pause(int ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec t = { ms / 1000, (long)(ms % 1000) * 1000000L };
    nanosleep(&t, NULL);
#endif
}

/* Whether the thread finished within `ms`. */
static int waiter_finished(int ms) {
    for (int waited = 0; waited < ms && !atomic_load(&waiter_done); waited += 10) {
        waiter_pause(10);
    }
    return atomic_load(&waiter_done);
}

#if defined(_WIN32)
typedef HANDLE waiter_thread;

static DWORD WINAPI waiter_accept_body(LPVOID unused) { (void)unused; waiter_accept(); return 0; }
static DWORD WINAPI waiter_recv_body(LPVOID unused) { (void)unused; waiter_recv(); return 0; }
static DWORD WINAPI waiter_send_body(LPVOID unused) { (void)unused; waiter_send(); return 0; }

/* `which`: 1 an accept, 0 a recv, 2 a send. */
static int waiter_start(waiter_thread *t, int which) {
    *t = CreateThread(NULL, 0, which == 1 ? waiter_accept_body : which == 2 ? waiter_send_body : waiter_recv_body, NULL, 0, NULL);
    return *t != NULL;
}

static void waiter_join(waiter_thread t) {
    WaitForSingleObject(t, INFINITE);
    CloseHandle(t);
}
#else
typedef pthread_t waiter_thread;

static void *waiter_accept_body(void *unused) { (void)unused; waiter_accept(); return NULL; }
static void *waiter_recv_body(void *unused) { (void)unused; waiter_recv(); return NULL; }
static void *waiter_send_body(void *unused) { (void)unused; waiter_send(); return NULL; }

/* `which`: 1 an accept, 0 a recv, 2 a send. */
static int waiter_start(waiter_thread *t, int which) {
    return pthread_create(t, NULL, which == 1 ? waiter_accept_body : which == 2 ? waiter_send_body : waiter_recv_body, NULL) == 0;
}

static void waiter_join(waiter_thread t) {
    pthread_join(t, NULL);
}
#endif

/* TWO CALLS WAITING ON ONE SOCKET AT ONCE, each with its own answer: the
 * shape beside the three above (lane b34-layer, 2026-11-13), where the close
 * must end both, and the last one out closes the descriptor. `which`: 1 an
 * accept, 0 a recv, 2 a send waiting for room. */
typedef struct {
    int which;
    int64_t answer;
    int64_t kind;
} pair_waiter;

static pair_waiter pair[2];
static _Atomic int pair_done;

static void pair_wait(pair_waiter *w) {
    if (w->which == 1) {
        hero_net_socket *got = hero_net_accept(waiter_on);

        w->answer = got == NULL ? -1 : 1;
        w->kind = hero_net_last_error();
        if (got != NULL) hero_net_close(got);
    } else if (w->which == 0) {
        unsigned char buffer[16];

        w->answer = hero_net_recv(waiter_on, buffer, sizeof buffer);
        w->kind = hero_net_last_error();
    } else {
        int64_t n = 0;

        while ((n = hero_net_send(waiter_on, waiter_bytes, sizeof waiter_bytes)) > 0) {
        }
        w->answer = n;
        w->kind = hero_net_last_error();
    }
    atomic_fetch_add(&pair_done, 1);
}

#if defined(_WIN32)
static DWORD WINAPI pair_body(LPVOID w) { pair_wait((pair_waiter *)w); return 0; }

static int pair_start(waiter_thread *t, pair_waiter *w) {
    *t = CreateThread(NULL, 0, pair_body, w, 0, NULL);
    return *t != NULL;
}
#else
static void *pair_body(void *w) { pair_wait((pair_waiter *)w); return NULL; }

static int pair_start(waiter_thread *t, pair_waiter *w) {
    return pthread_create(t, NULL, pair_body, w) == 0;
}
#endif

/* Whether both finished within `ms`. */
static int pair_finished(int ms) {
    for (int waited = 0; waited < ms && atomic_load(&pair_done) < 2; waited += 10) {
        waiter_pause(10);
    }
    return atomic_load(&pair_done) >= 2;
}

/* Both waiters started on `on`; 1 where each failed `HERO_NET_NOT_OPEN`
 * after `on` was closed. Where one still waits after 3 s, `free_them` is
 * called so both return and the program ends either way. */
static int64_t pair_closed(hero_net_socket *on, int first, int second, int pause_ms, void (*free_them)(void)) {
    waiter_thread t[2];

    waiter_on = on;
    pair[0] = (pair_waiter){ first, 0, 0 };
    pair[1] = (pair_waiter){ second, 0, 0 };
    atomic_store(&pair_done, 0);
    if (!pair_start(&t[0], &pair[0])) return -2;
    if (!pair_start(&t[1], &pair[1])) {
        hero_net_close(on);
        waiter_join(t[0]);
        return -2;
    }
    waiter_pause(pause_ms);
    int64_t step = hero_net_close(on) == 0 ? 1 : -3;

    if (!pair_finished(3000)) {
        free_them();
        step = -4;
    }
    waiter_join(t[0]);
    waiter_join(t[1]);
    for (int i = 0; i < 2 && step == 1; i += 1) {
        if (pair[i].answer != -1 || pair[i].kind != HERO_NET_NOT_OPEN) step = -5;
    }
    return step;
}

static int64_t pair_port;
static hero_net_socket *pair_peer;

/* Two connections, one for each accept still waiting. */
static void pair_connect_twice(void) {
    for (int i = 0; i < 2; i += 1) {
        hero_net_socket *c = hero_net_connect("127.0.0.1", pair_port);

        if (c != NULL) hero_net_close(c);
    }
}

/* The peer closed, which ends a recv and a send still waiting. */
static void pair_close_peer(void) {
    hero_net_close(pair_peer);
    pair_peer = NULL;
}

static inline int64_t close_ends_two_waits(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);

    if (listener == NULL) return -1;
    pair_port = hero_net_port(listener);
    int64_t accepts = pair_closed(listener, 1, 1, 200, pair_connect_twice);

    if (accepts != 1) return accepts;
    listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *client = listener ? hero_net_connect("127.0.0.1", hero_net_port(listener)) : NULL;

    pair_peer = client ? hero_net_accept(listener) : NULL;
    int64_t moves = pair_peer == NULL ? -6 : pair_closed(client, 0, 2, 500, pair_close_peer);

    if (moves == -6 && client != NULL) hero_net_close(client);
    if (pair_peer != NULL) hero_net_close(pair_peer);
    if (listener != NULL) hero_net_close(listener);
    return moves == 1 ? 1 : moves - 10;
}

/* An `accept` waiting on a listener, then the listener closed. */
static inline int64_t close_ends_accept(void) {
    waiter_thread t;
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);

    if (listener == NULL) return -1;
    int64_t port = hero_net_port(listener);

    waiter_on = listener;
    waiter_took = NULL;
    atomic_store(&waiter_done, 0);
    if (!waiter_start(&t, 1)) return -2;
    waiter_pause(200);
    if (hero_net_close(listener) != 0) return -3;
    if (!waiter_finished(3000)) {
        hero_net_socket *freeing = hero_net_connect("127.0.0.1", port);

        waiter_join(t);
        if (waiter_took != NULL) hero_net_close(waiter_took);
        if (freeing != NULL) hero_net_close(freeing);
        return -4;
    }
    waiter_join(t);
    if (waiter_took != NULL) {
        hero_net_close(waiter_took);
        return -5;
    }
    if (waiter_kind != HERO_NET_NOT_OPEN) return -6;
    hero_net_socket *late = hero_net_connect("127.0.0.1", port);

    if (late != NULL) {
        hero_net_close(late);
        return -7;
    }
    if (hero_net_accept(listener) != NULL || hero_net_last_error() != HERO_NET_NOT_OPEN) return -8;
    return 1;
}

/* A `recv` waiting on a connection, then that socket closed. */
static inline int64_t close_ends_recv(void) {
    waiter_thread t;
    unsigned char buffer[4];
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *client = listener ? hero_net_connect("127.0.0.1", hero_net_port(listener)) : NULL;
    hero_net_socket *server = client ? hero_net_accept(listener) : NULL;
    int64_t step = 1;

    if (server == NULL) {
        step = -1;
    } else {
        waiter_on = server;
        atomic_store(&waiter_done, 0);
        if (!waiter_start(&t, 0)) {
            step = -2;
            hero_net_close(server);
        } else {
            waiter_pause(200);
            if (hero_net_close(server) != 0) step = -3;
            if (!waiter_finished(3000)) {
                hero_net_send(client, "x", 1);
                step = -4;
            }
            waiter_join(t);
            if (step == 1 && (waiter_answer != -1 || waiter_kind != HERO_NET_NOT_OPEN)) step = -5;
            if (step == 1 && (hero_net_recv(server, buffer, sizeof buffer) != -1 || hero_net_last_error() != HERO_NET_NOT_OPEN)) step = -6;
        }
    }
    if (client != NULL) hero_net_close(client);
    if (listener != NULL) hero_net_close(listener);
    return step;
}

/* A `send` waiting for room in a buffer a peer that never reads has filled,
 * then that socket closed. */
static inline int64_t close_ends_send(void) {
    waiter_thread t;
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *client = listener ? hero_net_connect("127.0.0.1", hero_net_port(listener)) : NULL;
    hero_net_socket *server = client ? hero_net_accept(listener) : NULL;
    int64_t step = 1;

    if (server == NULL) {
        step = -1;
    } else {
        waiter_on = client;
        atomic_store(&waiter_done, 0);
        if (!waiter_start(&t, 2)) {
            step = -2;
        } else {
            waiter_pause(500);
            if (hero_net_close(client) != 0) step = -3;
            if (!waiter_finished(3000)) {
                hero_net_close(server);
                server = NULL;
                step = -4;
            }
            waiter_join(t);
            if (step == 1 && waiter_sent <= 0) step = -5;
            if (step == 1 && (waiter_answer != -1 || waiter_kind != HERO_NET_NOT_OPEN)) step = -6;
            if (step == 1 && (hero_net_send(client, "x", 1) != -1 || hero_net_last_error() != HERO_NET_NOT_OPEN)) step = -7;
            client = NULL;
        }
    }
    if (client != NULL) hero_net_close(client);
    if (server != NULL) hero_net_close(server);
    if (listener != NULL) hero_net_close(listener);
    return step;
}
