/* The C the case of defect 2673 calls: a lock of `hero_ipc.h` acquired by a
 * thread that then ends, and asked of by a thread started after it, which on
 * Darwin is given the ended thread's `pthread_t` at once and on Windows may
 * be given its id. The header's promise, *held by one thread of one process
 * at a time, and only that thread releases it*, is asked of the later
 * thread: its release is refused `HERO_IPC_NOT_HELD`, and its acquire is
 * never refused as by a thread that holds the lock (`HERO_IPC_INVALID`): it
 * waits as any other thread does, until its bound on POSIX, where the ended
 * thread's hold stands, or takes the lock its holder abandoned on Windows.
 * Each function answers 1 where every step held, else the negative number
 * of the step that did not. Threads are C's, which a Heroes thread cannot
 * be told apart from by the layer's lock (it carries one `i64`). */
#ifndef FIXEDBUGS_2673_HOLDERS_H
#define FIXEDBUGS_2673_HOLDERS_H

#include "hero_ipc.h"

#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

typedef struct {
    hero_ipc_lock *lock;
    /* 0 to acquire, 1 to release, 2 to acquire with no wait. */
    int act;
    int64_t answer;
    int64_t kind;
} holders_call;

static void holders_act(holders_call *call) {
    if (call->act == 0) {
        call->answer = hero_ipc_lock_acquire(call->lock, 1000);
    } else if (call->act == 1) {
        call->answer = hero_ipc_lock_release(call->lock);
    } else {
        call->answer = hero_ipc_lock_acquire(call->lock, 0);
    }
    call->kind = hero_ipc_last_error();
}

#if defined(_WIN32)
static DWORD WINAPI holders_thread(LPVOID arg) {
    holders_act((holders_call *)arg);
    return 0;
}

/* `call` made on a thread of its own, which has ended when this returns. */
static int holders_on_a_thread(holders_call *call) {
    HANDLE t = CreateThread(NULL, 0, holders_thread, call, 0, NULL);

    if (t == NULL) {
        return 0;
    }
    (void)WaitForSingleObject(t, INFINITE);
    (void)CloseHandle(t);
    return 1;
}
#else
static void *holders_thread(void *arg) {
    holders_act((holders_call *)arg);
    return NULL;
}

/* `call` made on a thread of its own, which has ended when this returns. */
static int holders_on_a_thread(holders_call *call) {
    pthread_t t;

    if (pthread_create(&t, NULL, holders_thread, call) != 0) {
        return 0;
    }
    (void)pthread_join(t, NULL);
    return 1;
}
#endif

/* A lock's name this process alone uses. */
static void holders_name(char *out, size_t room, const char *what) {
#if defined(_WIN32)
    (void)snprintf(out, room, "f2673-%s-%lu", what, (unsigned long)GetCurrentProcessId());
#else
    (void)snprintf(out, room, "f2673-%s-%ld", what, (long)getpid());
#endif
}

/* A thread acquires the lock and ends; a thread started after it releases
 * it, and is refused. */
static int64_t release_after_the_holder_ended(void) {
    char name[32];

    holders_name(name, sizeof name, "release");
    hero_ipc_lock *lock = hero_ipc_lock_open(name, 0600);

    if (lock == NULL) {
        return -1;
    }
    holders_call took = { lock, 0, -1, 0 };
    holders_call gave = { lock, 1, -1, 0 };
    int64_t answer = 1;

    if (!holders_on_a_thread(&took) || took.answer != 0) {
        answer = -2;
    } else if (!holders_on_a_thread(&gave)) {
        answer = -3;
    } else if (gave.answer != -1 || gave.kind != HERO_IPC_NOT_HELD) {
        answer = -4;
    }
    (void)hero_ipc_lock_close(lock);
    (void)hero_ipc_lock_remove(name);
    return answer;
}

/* A thread acquires the lock and ends; a thread started after it acquires
 * it with no wait, and is told the lock is taken (POSIX) or given it as
 * abandoned (Windows), never that it holds it already. */
static int64_t acquire_after_the_holder_ended(void) {
    char name[32];

    holders_name(name, sizeof name, "acquire");
    hero_ipc_lock *lock = hero_ipc_lock_open(name, 0600);

    if (lock == NULL) {
        return -1;
    }
    holders_call took = { lock, 0, -1, 0 };
    holders_call again = { lock, 2, -1, 0 };
    int64_t answer = 1;

    if (!holders_on_a_thread(&took) || took.answer != 0) {
        answer = -2;
    } else if (!holders_on_a_thread(&again)) {
        answer = -3;
    } else if (again.answer == -1 && again.kind == HERO_IPC_INVALID) {
        answer = -4;
#if defined(_WIN32)
    } else if (again.answer != 1) {
        answer = -5;
#else
    } else if (again.answer != -1 || again.kind != HERO_IPC_TIMED_OUT) {
        answer = -5;
#endif
    }
    (void)hero_ipc_lock_close(lock);
    (void)hero_ipc_lock_remove(name);
    return answer;
}

#endif
