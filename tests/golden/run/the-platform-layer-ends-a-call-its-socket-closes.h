/* A close of a socket another thread waits on ends the wait, failing
 * HERO_NET_NOT_OPEN, and nothing reaches the socket after it (feature 2295,
 * 2026-11-12; sitting 246's ffi-pragmatist found the listener's half broken
 * on Darwin, where `accept` went on waiting and took a connection made after
 * the close). The waiting thread is C's own, POSIX's or Windows', calling the
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

static int waiter_start(waiter_thread *t, int accepting) {
    *t = CreateThread(NULL, 0, accepting ? waiter_accept_body : waiter_recv_body, NULL, 0, NULL);
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

static int waiter_start(waiter_thread *t, int accepting) {
    return pthread_create(t, NULL, accepting ? waiter_accept_body : waiter_recv_body, NULL) == 0;
}

static void waiter_join(waiter_thread t) {
    pthread_join(t, NULL);
}
#endif

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
