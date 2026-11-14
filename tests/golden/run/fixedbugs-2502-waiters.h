/* The C the cases of defect 2502 share (panel 248's R2): calls of the layer
 * waiting on threads of C's own, POSIX's or Windows', which a Heroes thread
 * cannot be (it carries one `i64`, a socket is a handle), and the layer's
 * promises asked of them: a close ends each waiting call at once, and one
 * poller tells many waiting calls, in both directions on one socket, when
 * their socket is ready. Each function answers 1 where every step held, else
 * the negative number of the step that did not; a call still waiting after
 * 5 s is freed (a connection, a byte, a close) so the program ends either
 * way, and that is the case's failure, never a hang. */
#include "hero_net.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

#define WAITERS_MOST 200

static void waiters_pause(int ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec t = { ms / 1000, (long)(ms % 1000) * 1000000L };
    nanosleep(&t, NULL);
#endif
}

/* Microseconds on a clock that never goes back. */
static int64_t waiters_now_us(void) {
#if defined(_WIN32)
    LARGE_INTEGER c;
    LARGE_INTEGER f;
    QueryPerformanceCounter(&c);
    QueryPerformanceFrequency(&f);
    return (int64_t)(c.QuadPart / f.QuadPart) * 1000000 + (int64_t)(c.QuadPart % f.QuadPart) * 1000000 / (int64_t)f.QuadPart;
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (int64_t)t.tv_sec * 1000000 + (int64_t)t.tv_nsec / 1000;
#endif
}

/* One waiting call: what it waits in (0 a recv of one byte, 1 an accept,
 * 2 sends until the peer's buffer is full and a send waits), on which
 * socket, and what it answered and when. */
typedef struct {
    int which;
    hero_net_socket *on;
    int64_t answer;
    int64_t kind;
    unsigned char got;
    _Atomic int64_t back_us;
    _Atomic int started;
} waiter;

static waiter waiters[WAITERS_MOST];
static unsigned char waiters_bytes[65536];

static void waiter_run(waiter *w) {
    atomic_store(&w->started, 1);
    if (w->which == 1) {
        hero_net_socket *got = hero_net_accept(w->on);

        w->answer = got == NULL ? -1 : 1;
        w->kind = hero_net_last_error();
        if (got != NULL) hero_net_close(got);
    } else if (w->which == 0) {
        w->answer = hero_net_recv(w->on, &w->got, 1);
        w->kind = hero_net_last_error();
    } else {
        int64_t n = 0;

        while ((n = hero_net_send(w->on, waiters_bytes, sizeof waiters_bytes)) > 0) {
            w->answer += n;
        }
        w->kind = hero_net_last_error();
    }
    atomic_store(&w->back_us, waiters_now_us());
}

#if defined(_WIN32)
typedef HANDLE waiter_thread;
static DWORD WINAPI waiter_body(LPVOID w) { waiter_run((waiter *)w); return 0; }
static int waiter_start(waiter_thread *t, waiter *w) {
    *t = CreateThread(NULL, 0, waiter_body, w, 0, NULL);
    return *t != NULL;
}
static void waiter_join(waiter_thread t) {
    WaitForSingleObject(t, INFINITE);
    CloseHandle(t);
}
#else
typedef pthread_t waiter_thread;
static void *waiter_body(void *w) { waiter_run((waiter *)w); return NULL; }
static int waiter_start(waiter_thread *t, waiter *w) {
    return pthread_create(t, NULL, waiter_body, w) == 0;
}
static void waiter_join(waiter_thread t) {
    pthread_join(t, NULL);
}
#endif

static waiter_thread waiter_threads[WAITERS_MOST];

/* Whether waiter `i` returned within `ms`. */
static int waiter_back(int i, int ms) {
    for (int waited = 0; waited < ms && atomic_load(&waiters[i].back_us) == 0; waited += 1) {
        waiters_pause(1);
    }
    return atomic_load(&waiters[i].back_us) != 0;
}

static int compare_i64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a;
    int64_t y = *(const int64_t *)b;
    return x < y ? -1 : x > y;
}

/* `n` accepts waiting, each on a listener of its own, then each listener
 * closed in turn and the time from the close to its accept's return kept:
 * 1 where every accept failed `HERO_NET_NOT_OPEN` and the median of those
 * times is under 10 ms. Waiting in slices of 50 ms, the median is about
 * 25 ms (the slices' half; 38 ms measured at the base on Darwin, where a
 * listener's shutdown wakes nothing), and under 10 ms for one in five
 * closes, so the median of 20 passes so with a chance near 1 in 400. */
static inline int64_t close_wakes_at_once(int64_t n) {
    int64_t took[WAITERS_MOST];

    if (n < 1 || n > WAITERS_MOST) return -1;
    for (int i = 0; i < n; i += 1) {
        waiters[i] = (waiter){ 1, hero_net_listen("127.0.0.1", 0), 0, 0, 0, 0, 0 };
        if (waiters[i].on == NULL) return -2;
        if (!waiter_start(&waiter_threads[i], &waiters[i])) return -3;
    }
    /* Each accept waiting in the layer before its listener is closed. */
    waiters_pause(300);
    int64_t step = 1;

    for (int i = 0; i < n; i += 1) {
        int64_t closed = waiters_now_us();

        if (hero_net_close(waiters[i].on) != 0) step = -4;
        if (!waiter_back(i, 5000)) step = -5;
        took[i] = atomic_load(&waiters[i].back_us) - closed;
    }
    for (int i = 0; i < n; i += 1) {
        waiter_join(waiter_threads[i]);
        if (step == 1 && (waiters[i].answer != -1 || waiters[i].kind != HERO_NET_NOT_OPEN)) step = -6;
    }
    qsort(took, (size_t)n, sizeof took[0], compare_i64);
    if (step == 1 && took[n / 2] >= 10000) step = -7;
    return step;
}

/* `n` connections, a recv of one byte waiting on each, then one byte sent to
 * each in the reverse of the order they began to wait: 1 where each received
 * exactly its own byte, a value no other connection was sent. */
static inline int64_t many_waiters_each_get_their_byte(int64_t n) {
    hero_net_socket *peers[WAITERS_MOST];
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);

    if (n < 1 || n > WAITERS_MOST || listener == NULL) return -1;
    int64_t port = hero_net_port(listener);

    for (int i = 0; i < n; i += 1) {
        peers[i] = hero_net_connect("127.0.0.1", port);
        hero_net_socket *mine = peers[i] == NULL ? NULL : hero_net_accept(listener);

        if (mine == NULL) return -2;
        waiters[i] = (waiter){ 0, mine, 0, 0, 0, 0, 0 };
        if (!waiter_start(&waiter_threads[i], &waiters[i])) return -3;
    }
    waiters_pause(300);
    int64_t step = 1;

    for (int i = (int)n - 1; i >= 0; i -= 1) {
        unsigned char b = (unsigned char)(i % 251 + 1);

        if (hero_net_send(peers[i], &b, 1) != 1) step = -4;
    }
    for (int i = 0; i < n; i += 1) {
        if (!waiter_back(i, 5000)) {
            step = -5;
            hero_net_close(peers[i]);
            peers[i] = NULL;
        }
    }
    for (int i = 0; i < n; i += 1) {
        waiter_join(waiter_threads[i]);
        if (step == 1 && (waiters[i].answer != 1 || waiters[i].got != (unsigned char)(i % 251 + 1))) step = -6;
        hero_net_close(waiters[i].on);
        if (peers[i] != NULL) hero_net_close(peers[i]);
    }
    hero_net_close(listener);
    return step;
}

/* One connection with a send waiting for room (its peer reads nothing until
 * its buffer is full) and a recv waiting for a byte, both at once on one
 * socket, then the peer reads everything sent so far and sends one byte: 1
 * where the recv received that byte and the send went on (it sent more after
 * the peer read) until the close ended it, `HERO_NET_NOT_OPEN`. The one
 * descriptor is armed for both ways at once, which Linux's poller holds as
 * one entry. */
static inline int64_t both_ways_on_one_socket(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *client = listener ? hero_net_connect("127.0.0.1", hero_net_port(listener)) : NULL;
    hero_net_socket *peer = client ? hero_net_accept(listener) : NULL;

    if (peer == NULL) return -1;
    waiters[0] = (waiter){ 2, client, 0, 0, 0, 0, 0 };
    waiters[1] = (waiter){ 0, client, 0, 0, 0, 0, 0 };
    if (!waiter_start(&waiter_threads[0], &waiters[0])) return -2;
    if (!waiter_start(&waiter_threads[1], &waiters[1])) return -2;
    /* The sender fills the buffers and waits; the receiver waits. */
    waiters_pause(500);
    int64_t step = 1;
    unsigned char sink[65536];
    int64_t drained = 0;
    int64_t got = 0;

    /* Everything the sender put in so far read, which leaves it room. */
    while (drained < 1048576 && (got = hero_net_recv(peer, sink, sizeof sink)) > 0) {
        drained += got;
        if (got < (int64_t)sizeof sink) break;
    }
    if (drained == 0) step = -3;
    unsigned char one = 77;

    if (hero_net_send(peer, &one, 1) != 1) step = -4;
    if (!waiter_back(1, 5000)) step = -5;
    /* The sender went on past what the peer read. */
    waiters_pause(300);
    hero_net_close(client);
    if (!waiter_back(0, 5000)) step = -6;
    waiter_join(waiter_threads[0]);
    waiter_join(waiter_threads[1]);
    if (step == 1 && (waiters[1].answer != 1 || waiters[1].got != 77)) step = -7;
    if (step == 1 && (waiters[0].answer <= drained || waiters[0].kind != HERO_NET_NOT_OPEN)) step = -8;
    hero_net_close(peer);
    hero_net_close(listener);
    return step;
}
