/* Defect 2554: on Windows a listener the platform layer closed went on
 * listening after the close, so its port was still held; a connect to it
 * succeeded 16 times in 80 on the box (the issue's comment of 2026-11-15).
 * The layer's poller holds each socket it watches as one of its users, and
 * the last user out closes the socket: a close nudged the waiting `accept`,
 * which left first and answered, and the socket stayed open until the
 * poller's round ended. A second road to the same hold: an `accept` whose
 * deadline passed leaves without waking the poller, which goes on holding
 * the listener, and a close then found only the poller's hold and returned.
 *
 * Each function runs its shape `rounds` times and asks, each time, whether
 * the port is free: a listen on it succeeds only where no socket holds it
 * (the layer listens with SO_EXCLUSIVEADDRUSE on Windows, SO_REUSEADDR on
 * POSIX, neither of which takes a port a listener holds). 1 where every round
 * held, else the negative number of the step that did not. The waiting
 * thread is C's own, calling the layer and nothing of Heroes. */
#include "hero_net.h"

#include <stdatomic.h>
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

static hero_net_socket *port_listener;
static hero_net_socket *port_took;
static int64_t port_kind;
static _Atomic int port_done;

static void port_accept(void) {
    port_took = hero_net_accept(port_listener);
    port_kind = hero_net_last_error();
    atomic_store(&port_done, 1);
}

static void port_pause(int ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec t = { ms / 1000, (long)(ms % 1000) * 1000000L };
    nanosleep(&t, NULL);
#endif
}

/* Whether the waiting thread finished within `ms`. */
static int port_finished(int ms) {
    for (int waited = 0; waited < ms && !atomic_load(&port_done); waited += 10) {
        port_pause(10);
    }
    return atomic_load(&port_done);
}

#if defined(_WIN32)
typedef HANDLE port_thread;

static DWORD WINAPI port_accept_body(LPVOID unused) {
    (void)unused;
    port_accept();
    return 0;
}

static int port_start(port_thread *t) {
    *t = CreateThread(NULL, 0, port_accept_body, NULL, 0, NULL);
    return *t != NULL;
}

static void port_join(port_thread t) {
    WaitForSingleObject(t, INFINITE);
    CloseHandle(t);
}
#else
typedef pthread_t port_thread;

static void *port_accept_body(void *unused) {
    (void)unused;
    port_accept();
    return NULL;
}

static int port_start(port_thread *t) {
    return pthread_create(t, NULL, port_accept_body, NULL) == 0;
}

static void port_join(port_thread t) {
    pthread_join(t, NULL);
}
#endif

/* Whether no socket holds `port`: a listener on it is made, and closed. */
static int port_free(int64_t port) {
    hero_net_socket *again = hero_net_listen("127.0.0.1", port);

    if (again == NULL) return 0;
    hero_net_close(again);
    return 1;
}

/* An `accept` waiting on a listener in a thread of C's, the listener
 * closed, the `accept`'s answer read once its thread has ended: the port is
 * free then. */
static int64_t port_after_waiting_accept(int64_t rounds) {
    for (int64_t r = 0; r < rounds; r += 1) {
        port_thread t;
        hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);

        if (listener == NULL) return -1;
        int64_t port = hero_net_port(listener);

        port_listener = listener;
        port_took = NULL;
        atomic_store(&port_done, 0);
        if (!port_start(&t)) {
            hero_net_close(listener);
            return -2;
        }
        port_pause(10);
        if (hero_net_close(listener) != 0) {
            port_join(t);
            return -3;
        }
        if (!port_finished(3000)) {
            hero_net_socket *freeing = hero_net_connect("127.0.0.1", port);

            port_join(t);
            if (port_took != NULL) hero_net_close(port_took);
            if (freeing != NULL) hero_net_close(freeing);
            return -4;
        }
        port_join(t);
        if (port_took != NULL) {
            hero_net_close(port_took);
            return -5;
        }
        if (port_kind != HERO_NET_NOT_OPEN) return -6;
        if (!port_free(port)) return -7;
    }
    return 1;
}

/* An `accept` on a listener whose deadline passes, the listener closed at
 * once: the port is free once the close has returned. */
static int64_t port_after_timed_out_accept(int64_t rounds) {
    for (int64_t r = 0; r < rounds; r += 1) {
        hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);

        if (listener == NULL) return -1;
        int64_t port = hero_net_port(listener);

        if (hero_net_set_deadline(listener, 10) != 0) {
            hero_net_close(listener);
            return -2;
        }
        hero_net_socket *took = hero_net_accept(listener);

        if (took != NULL) {
            hero_net_close(took);
            hero_net_close(listener);
            return -3;
        }
        if (hero_net_last_error() != HERO_NET_TIMED_OUT) {
            hero_net_close(listener);
            return -4;
        }
        if (hero_net_close(listener) != 0) return -5;
        if (!port_free(port)) return -7;
    }
    return 1;
}
