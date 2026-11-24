/* The C the case of defect 2667 calls: streams another member of the layer
 * makes over a table of its own (`hero_stream_make`), whose readiness is a
 * descriptor's, waited on by a set of `hero_poll.h`. A serial port, a
 * pseudo-terminal's controlling side and a local channel of `hero_ipc.h`
 * each stand on a descriptor they read directly, so each joins a set and is
 * told READ once its other side writes, and not before. Each function
 * answers 1 where every step held, or where the platform has no such stream
 * or waits on none (Windows, where `hero_poll.h` waits on sockets alone and
 * there is no pseudo-terminal), else the negative number of the step that
 * did not. A wait lasts at most 5 s, so the program ends either way. */
#ifndef FIXEDBUGS_2667_READY_H
#define FIXEDBUGS_2667_READY_H

#include "hero_ipc.h"
#include "hero_poll.h"
#include "hero_serial.h"
#include "hero_stream.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* The process's number names the channel's file: `_getpid` from the C
 * library's `<process.h>` on Windows, never `<windows.h>`, whose `near` and
 * `far` macros would erase two names below. */
#if defined(_WIN32)
#include <process.h>
#else
#include <unistd.h>
#endif

#define READY_PATIENCE_NS INT64_C(5000000000)

/* Whether one wait of at most `limit_ns` tells the entry `key` READ. */
static int ready_told(hero_poll_set *set, int64_t key, int64_t limit_ns) {
    int64_t events[3 * 8];
    int64_t count = hero_poll_wait(set, limit_ns, events, sizeof events / sizeof events[0]);

    for (int64_t at = 0; at < count; at += 1) {
        if (events[at * HERO_POLL_EVENT_NUMBERS] == key && (events[at * HERO_POLL_EVENT_NUMBERS + 1] & HERO_POLL_READ) != 0) {
            return 1;
        }
    }
    return 0;
}

/* The two sides of a pseudo-terminal, the port side opened as a port: each
 * joins one set, neither is ready, and each is told READ once the other
 * writes to it. */
static int64_t pseudo_sides_join_a_set(void) {
    hero_stream *controller = hero_serial_pseudo();

    if (controller == NULL) {
        return hero_serial_last_error() == HERO_SERIAL_UNSUPPORTED ? 1 : -1;
    }
    char path[129];
    int64_t length = hero_serial_pseudo_path(controller, path, sizeof path - 1);
    int64_t answer = 1;

    if (length <= 0 || length >= (int64_t)sizeof path) {
        (void)hero_stream_close(controller);
        return -2;
    }
    path[length] = '\0';
    hero_stream *port = hero_serial_open(path);
    hero_poll_set *set = port != NULL ? hero_poll_make(4) : NULL;

    if (set == NULL) {
        answer = port == NULL ? -3 : -4;
    } else {
        int64_t at_port = hero_poll_add_stream(set, port, HERO_POLL_READ);
        int64_t at_controller = hero_poll_add_stream(set, controller, HERO_POLL_READ);
        unsigned char bytes[64];

        if (at_port < 0 || at_controller < 0) {
            answer = -5;
        } else if (ready_told(set, at_port, 0) || ready_told(set, at_controller, 0)) {
            answer = -6;
        } else if (hero_stream_write(controller, "to the port", 11) != 11 || !ready_told(set, at_port, READY_PATIENCE_NS)) {
            answer = -7;
        } else if (ready_told(set, at_controller, 0) || hero_stream_read(port, bytes, sizeof bytes) <= 0) {
            answer = -8;
        } else if (hero_stream_write(port, "to the device", 13) != 13 || !ready_told(set, at_controller, READY_PATIENCE_NS)) {
            answer = -9;
        }
        (void)hero_poll_close(set);
    }
    if (port != NULL) {
        (void)hero_stream_close(port);
    }
    (void)hero_stream_close(controller);
    return answer;
}

/* A local channel's two ends, a connection and the one it was accepted as:
 * each joins one set, neither is ready, and each is told READ once the other
 * writes to it. */
static int64_t channel_ends_join_a_set(void) {
    char path[64];
    int64_t answer = 1;

#if defined(_WIN32)
    (void)snprintf(path, sizeof path, "build/ready-2667-%ld.sock", (long)_getpid());
#else
    (void)snprintf(path, sizeof path, "build/ready-2667-%ld.sock", (long)getpid());
#endif
    hero_ipc_listener *listener = hero_ipc_listen(path, HERO_IPC_SPACE_PATH, 0600);

    if (listener == NULL) {
        return -1;
    }
    hero_stream *near = hero_ipc_connect(path, HERO_IPC_SPACE_PATH, 1000);
    hero_stream *far = near != NULL ? hero_ipc_accept(listener, 1000) : NULL;
    hero_poll_set *set = far != NULL ? hero_poll_make(4) : NULL;

    if (set == NULL) {
        answer = -2;
    } else {
        int64_t at_far = hero_poll_add_stream(set, far, HERO_POLL_READ);
        int64_t at_near = at_far < 0 ? -1 : hero_poll_add_stream(set, near, HERO_POLL_READ);
        unsigned char bytes[64];

        if (at_far < 0 && hero_poll_last_error() == HERO_POLL_UNSUPPORTED) {
#if !defined(_WIN32)
            answer = -3;
#endif
        } else if (at_far < 0 || at_near < 0) {
            answer = -4;
        } else if (ready_told(set, at_far, 0) || ready_told(set, at_near, 0)) {
            answer = -5;
        } else if (hero_stream_write(near, "ping", 4) != 4 || !ready_told(set, at_far, READY_PATIENCE_NS)) {
            answer = -6;
        } else if (ready_told(set, at_near, 0) || hero_stream_read(far, bytes, sizeof bytes) != 4 || memcmp(bytes, "ping", 4) != 0) {
            answer = -7;
        } else if (hero_stream_write(far, "pong", 4) != 4 || !ready_told(set, at_near, READY_PATIENCE_NS)) {
            answer = -8;
        }
        (void)hero_poll_close(set);
    }
    if (far != NULL) {
        (void)hero_stream_close(far);
    }
    if (near != NULL) {
        (void)hero_stream_close(near);
    }
    (void)hero_ipc_listener_close(listener);
    return answer;
}

#endif
