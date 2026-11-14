/* The C the cases of a socket's deadline share (panel 248's R5,
 * `hero_net_set_deadline`): a quiet peer, a peer that sends one byte at a
 * time it is told, and a thread of C's own that sets a deadline while a call
 * waits, the waiting machinery of `fixedbugs-2502-waiters.h`. Each function
 * answers 1 where every step held, else the negative number of the step that
 * did not. The bounds on time are wide (a deadline of 300 ms met before
 * 2,300 ms), since what each asks is whether the deadline ended the wait or
 * did not, never how fast. */
#ifndef THE_PLATFORM_LAYER_DEADLINE_H
#define THE_PLATFORM_LAYER_DEADLINE_H

#include "fixedbugs-2502-waiters.h"

/* A connection made and accepted on `listener`, its two ends in `ends`. */
static int deadline_pair(hero_net_socket *listener, hero_net_socket *ends[2]) {
    ends[0] = hero_net_connect("127.0.0.1", hero_net_port(listener));
    ends[1] = ends[0] == NULL ? NULL : hero_net_accept(listener);
    return ends[1] != NULL;
}

static void deadline_close(hero_net_socket *listener, hero_net_socket *ends[2]) {
    if (ends[0] != NULL) hero_net_close(ends[0]);
    if (ends[1] != NULL) hero_net_close(ends[1]);
    if (listener != NULL) hero_net_close(listener);
}

/* A recv on a connection whose peer sends nothing, and an accept on a
 * listener nobody connects to, each with a deadline of 300 ms: 1 where each
 * answered `HERO_NET_TIMED_OUT`, no sooner than 300 ms and before 2,300. */
static inline int64_t deadline_ends_a_quiet_wait(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *ends[2] = { NULL, NULL };
    unsigned char byte = 0;

    if (listener == NULL || !deadline_pair(listener, ends)) return -1;
    if (hero_net_set_deadline(ends[1], 300) != 0) return -2;
    int64_t began = waiters_now_us();
    int64_t got = hero_net_recv(ends[1], &byte, 1);
    int64_t kind = hero_net_last_error();
    int64_t took = waiters_now_us() - began;
    int64_t step = got == -1 && kind == HERO_NET_TIMED_OUT ? 1 : -3;

    if (step == 1 && (took < 300000 || took >= 2300000)) step = -4;
    if (step == 1 && hero_net_set_deadline(listener, 300) != 0) step = -5;
    began = waiters_now_us();
    hero_net_socket *none = step == 1 ? hero_net_accept(listener) : NULL;

    kind = hero_net_last_error();
    took = waiters_now_us() - began;
    if (none != NULL) {
        hero_net_close(none);
        step = -6;
    }
    if (step == 1 && kind != HERO_NET_TIMED_OUT) step = -7;
    if (step == 1 && (took < 300000 || took >= 2300000)) step = -8;
    deadline_close(listener, ends);
    return step;
}

static hero_net_socket *sender_on;
static int sender_after_ms;

static void sender_run(void) {
    unsigned char b = 42;

    waiters_pause(sender_after_ms);
    (void)hero_net_send(sender_on, &b, 1);
}

#if defined(_WIN32)
static DWORD WINAPI sender_body(LPVOID unused) { (void)unused; sender_run(); return 0; }
static int sender_start(waiter_thread *t) {
    *t = CreateThread(NULL, 0, sender_body, NULL, 0, NULL);
    return *t != NULL;
}
#else
static void *sender_body(void *unused) { (void)unused; sender_run(); return NULL; }
static int sender_start(waiter_thread *t) {
    return pthread_create(t, NULL, sender_body, NULL) == 0;
}
#endif

/* A recv with a deadline of 1,500 ms, its peer sending one byte at 300 ms:
 * 1 where the byte was received, the deadline never met. */
static inline int64_t deadline_lets_an_early_byte_through(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *ends[2] = { NULL, NULL };
    waiter_thread sender;
    unsigned char byte = 0;

    if (listener == NULL || !deadline_pair(listener, ends)) return -1;
    if (hero_net_set_deadline(ends[1], 1500) != 0) return -2;
    sender_on = ends[0];
    sender_after_ms = 300;
    if (!sender_start(&sender)) return -3;
    int64_t got = hero_net_recv(ends[1], &byte, 1);
    int64_t step = got == 1 && byte == 42 ? 1 : -4;

    waiter_join(sender);
    deadline_close(listener, ends);
    return step;
}

static hero_net_socket *setter_on;
static int64_t setter_ms;

static void setter_run(void) {
    waiters_pause(300);
    (void)hero_net_set_deadline(setter_on, setter_ms);
}

#if defined(_WIN32)
static DWORD WINAPI setter_body(LPVOID unused) { (void)unused; setter_run(); return 0; }
static int setter_start(waiter_thread *t) {
    *t = CreateThread(NULL, 0, setter_body, NULL, 0, NULL);
    return *t != NULL;
}
#else
static void *setter_body(void *unused) { (void)unused; setter_run(); return NULL; }
static int setter_start(waiter_thread *t) {
    return pthread_create(t, NULL, setter_body, NULL) == 0;
}
#endif

/* A recv waiting with no deadline, given one of 300 ms by another thread
 * 300 ms in: 1 where the waiting call took it and answered
 * `HERO_NET_TIMED_OUT`, before 2,600 ms from its start. Go's meaning: a
 * deadline applies to a call already waiting. */
static inline int64_t deadline_reaches_a_call_already_waiting(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *ends[2] = { NULL, NULL };
    waiter_thread setter;
    unsigned char byte = 0;

    if (listener == NULL || !deadline_pair(listener, ends)) return -1;
    setter_on = ends[1];
    setter_ms = 300;
    if (!setter_start(&setter)) return -2;
    int64_t began = waiters_now_us();
    int64_t got = hero_net_recv(ends[1], &byte, 1);
    int64_t kind = hero_net_last_error();
    int64_t took = waiters_now_us() - began;
    int64_t step = got == -1 && kind == HERO_NET_TIMED_OUT ? 1 : -3;

    if (step == 1 && (took < 600000 || took >= 2600000)) step = -4;
    waiter_join(setter);
    deadline_close(listener, ends);
    return step;
}

/* A deadline of 200 ms taken away by 0, then a recv whose peer sends one
 * byte at 600 ms: 1 where the byte was received, no deadline met. */
static inline int64_t deadline_zero_takes_it_away(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *ends[2] = { NULL, NULL };
    waiter_thread sender;
    unsigned char byte = 0;

    if (listener == NULL || !deadline_pair(listener, ends)) return -1;
    if (hero_net_set_deadline(ends[1], 200) != 0 || hero_net_set_deadline(ends[1], 0) != 0) return -2;
    sender_on = ends[0];
    sender_after_ms = 600;
    if (!sender_start(&sender)) return -3;
    int64_t got = hero_net_recv(ends[1], &byte, 1);
    int64_t step = got == 1 && byte == 42 ? 1 : -4;

    waiter_join(sender);
    deadline_close(listener, ends);
    return step;
}

/* A deadline passed already does not end a call that need not wait: one
 * byte sent, a deadline of 1 ms set and passed, then a recv answers the
 * byte, and the next recv, which must wait, answers `HERO_NET_TIMED_OUT` at
 * once. And a socket given up for its number and taken back keeps its
 * deadline: 1 where both held. */
static inline int64_t deadline_spares_what_is_there_and_crosses_as_a_number(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);
    hero_net_socket *ends[2] = { NULL, NULL };
    unsigned char b = 7;
    unsigned char byte = 0;

    if (listener == NULL || !deadline_pair(listener, ends)) return -1;
    if (hero_net_send(ends[0], &b, 1) != 1) return -2;
    waiters_pause(50);
    if (hero_net_set_deadline(ends[1], 1) != 0) return -3;
    waiters_pause(50);
    int64_t step = hero_net_recv(ends[1], &byte, 1) == 1 && byte == 7 ? 1 : -4;
    int64_t began = waiters_now_us();

    if (step == 1 && (hero_net_recv(ends[1], &byte, 1) != -1 || hero_net_last_error() != HERO_NET_TIMED_OUT)) step = -5;
    if (step == 1 && waiters_now_us() - began >= 1000000) step = -6;
    if (step == 1 && hero_net_set_deadline(ends[1], 300) != 0) step = -7;
    int64_t n = step == 1 ? hero_net_number(ends[1]) : -1;
    hero_net_socket *back = n > 0 ? hero_net_of(n) : NULL;

    if (back == NULL) {
        step = step == 1 ? -8 : step;
    } else {
        ends[1] = back;
        if (step == 1 && (hero_net_recv(back, &byte, 1) != -1 || hero_net_last_error() != HERO_NET_TIMED_OUT)) step = -9;
    }
    deadline_close(listener, ends);
    return step;
}

/* What the door refuses: a negative time, `HERO_NET_INVALID`, and a socket
 * closed already, `HERO_NET_NOT_OPEN`: 1 where both were refused so. */
static inline int64_t deadline_refusals(void) {
    hero_net_socket *listener = hero_net_listen("127.0.0.1", 0);

    if (listener == NULL) return -1;
    if (hero_net_set_deadline(listener, -1) != -1 || hero_net_last_error() != HERO_NET_INVALID) return -2;
    hero_net_close(listener);
    if (hero_net_set_deadline(listener, 100) != -1 || hero_net_last_error() != HERO_NET_NOT_OPEN) return -3;
    return 1;
}

#endif
