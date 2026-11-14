/* The C the cases of the stop a program asks itself share (panel 248's R4,
 * `hero_net_ask_stop`): calls of the layer waiting on threads of C's own, the
 * waiting machinery of `fixedbugs-2502-waiters.h`, and the stop asked by
 * another thread of C's own, no signal sent. */
#ifndef THE_PLATFORM_LAYER_ASK_STOP_H
#define THE_PLATFORM_LAYER_ASK_STOP_H

#include "fixedbugs-2502-waiters.h"

static _Atomic int64_t ask_at_us;

static void asker_run(void) {
    waiters_pause(300);
    atomic_store(&ask_at_us, waiters_now_us());
    hero_net_ask_stop();
}

#if defined(_WIN32)
static DWORD WINAPI asker_body(LPVOID unused) { (void)unused; asker_run(); return 0; }
static int asker_start(waiter_thread *t) {
    *t = CreateThread(NULL, 0, asker_body, NULL, 0, NULL);
    return *t != NULL;
}
#else
static void *asker_body(void *unused) { (void)unused; asker_run(); return NULL; }
static int asker_start(waiter_thread *t) {
    return pthread_create(t, NULL, asker_body, NULL) == 0;
}
#endif

/* Twelve calls waiting, four accepts on listeners of their own, four recvs
 * on quiet connections and four sends for room in buffers their peers never
 * read, and the stop asked 300 ms in by a thread that is none of theirs nor
 * `main`'s: 1 where each answered `HERO_NET_STOPPED`, the median time from
 * the ask to their return was under 10 ms (slices of 50 ms give about 25),
 * and `hero_net_stop_asked` answered 0 before the ask and 1 after. A call
 * still waiting after 5 s is freed by closing its socket, the case's
 * failure. */
static inline int64_t ask_stops_every_wait(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *peers[12] = { NULL };
    int64_t took[12];
    waiter_thread asker;

    if (listener == NULL) return -1;
    if (hero_net_stop_asked() != 0) return -2;
    for (int i = 0; i < 12; i += 1) {
        int which = i % 3 == 0 ? 1 : i % 3 == 1 ? 0 : 2;
        hero_net_socket *on = NULL;

        if (which == 1) {
            on = hero_net_listen("127.0.0.1", 0);
        } else {
            peers[i] = hero_net_connect("127.0.0.1", hero_net_port(listener));
            on = peers[i] == NULL ? NULL : hero_net_accept(listener);
        }
        if (on == NULL) return -3;
        waiters[i] = (waiter){ which, on, 0, 0, 0, 0, 0 };
        if (!waiter_start(&waiter_threads[i], &waiters[i])) return -4;
    }
    if (!asker_start(&asker)) return -4;
    int64_t step = 1;

    for (int i = 0; i < 12; i += 1) {
        if (!waiter_back(i, 5000)) {
            step = -5;
            hero_net_close(waiters[i].on);
            waiters[i].on = NULL;
        }
    }
    waiter_join(asker);
    for (int i = 0; i < 12; i += 1) {
        waiter_join(waiter_threads[i]);
        took[i] = atomic_load(&waiters[i].back_us) - atomic_load(&ask_at_us);
        if (step == 1 && waiters[i].kind != HERO_NET_STOPPED) step = -6;
        if (waiters[i].on != NULL) hero_net_close(waiters[i].on);
        if (peers[i] != NULL) hero_net_close(peers[i]);
    }
    hero_net_close(listener);
    qsort(took, 12, sizeof took[0], compare_i64);
    if (step == 1 && took[6] >= 10000) step = -7;
    if (step == 1 && hero_net_stop_asked() != 1) step = -8;
    return step;
}

#endif
