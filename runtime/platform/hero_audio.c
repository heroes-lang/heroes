/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_audio.c — the platform layer's sound, `hero_audio.h`'s
 * functions on every system: the table of streams, the ring of frames between
 * the program and the system, the waits, the bell, the null device, and the
 * calls into this system's ARM (`platform/hero_audio_arm.h`), which moves the
 * frames between the ring and the device (M-rebel-library, 2026-11-23).
 *
 * ONE STREAM, TWO SIDES. The program writes into the ring (an output) or reads
 * from it (an input) through the functions of `hero_audio.h`, on any thread;
 * the system's thread, or the null device's, pulls from it or pushes into it
 * through the core functions of `hero_audio_arm.h`. Both sides take the
 * stream's own lock, briefly, and the program's side waits on the stream's
 * condition, which every move of frames and every change of state wakes. A
 * wait sleeps in slices of `HERO_AUDIO_SLICE_NS` and asks the program's stop
 * between them, outside the lock (`hero_net_stop_asked`).
 *
 * THE TABLE. A stream's handle is a slot and a life (`hero_handle_tag.h`), so
 * a closed one, or one this layer never issued, is refused rather than read.
 * A call holds a reference to the state while it uses it, under the table's
 * lock, so `hero_audio_close` from another thread wakes the call and frees the
 * state only once the last reference is given back, after the arm's stream is
 * closed and no system thread reaches it.
 *
 * NO CONDITIONAL BUT THE GUARD, in the header; here the two families of
 * systems differ in their locks, threads and bells alone.
 */

#include "hero_audio.h"

#include <math.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"
#include "platform/hero_audio_arm.h"

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#endif

/* The bits of a handle's slot: 1023 streams open at once. */
#define HERO_AUDIO_INDEX_BITS 10
#define HERO_AUDIO_SLOTS 1023
#define HERO_AUDIO_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_AUDIO_INDEX_BITS)
_Static_assert((HERO_AUDIO_LIFE_MOST << HERO_AUDIO_INDEX_BITS) < (UINT64_C(1) << HERO_HANDLE_TAG_SHIFT), "a life stays below the tag");
_Static_assert(HERO_AUDIO_SLOTS < (1 << HERO_AUDIO_INDEX_BITS), "a slot's number and one fit its bits");

/* The longest a wait sleeps before it asks the stop again. */
#define HERO_AUDIO_SLICE_NS INT64_C(50000000)
/* The null device's step, and the most time one step makes up for. */
#define HERO_AUDIO_STEP_NS INT64_C(10000000)
#define HERO_AUDIO_CATCH_NS INT64_C(100000000)
/* The frames the null device moves at most at once, and the most samples its
 * step's buffer holds: `HERO_AUDIO_CATCH_NS` at the highest rate, every
 * channel. */
#define HERO_AUDIO_NULL_FRAMES 76800
/* The words a failure keeps. */
#define HERO_AUDIO_WORDS 512

#if defined(_WIN32)
typedef SRWLOCK hero_audio_mutex;
typedef CONDITION_VARIABLE hero_audio_cond;
typedef HANDLE hero_audio_thread;
#else
typedef pthread_mutex_t hero_audio_mutex;
typedef pthread_cond_t hero_audio_cond;
typedef pthread_t hero_audio_thread;
#endif

struct hero_audio_core {
    hero_audio_mutex lock;
    hero_audio_cond moved;
    int64_t direction;
    int64_t rate;
    int64_t channels;
    /* The ring: `room` frames of `channels` floats. `written` and `taken`
     * count frames since the stream opened; `written - taken` wait. */
    float *ring;
    uint64_t room;
    uint64_t written;
    uint64_t taken;
    uint64_t position;
    uint64_t dropped;
    double volume;
    bool paused;
    bool finished;
    bool done;
    bool gone;
    bool closing;
    int64_t gone_os;
    char gone_words[HERO_AUDIO_WORDS];
    /* The arm's stream, or none for the null device, which a thread of this
     * member's own runs. */
    void *arm;
    bool null_device;
    bool null_running;
    hero_audio_thread null_thread;
    uint64_t null_frame;
    /* The references calls hold, and whether `hero_audio_close` is done
     * with the stream, both under the table's lock. */
    int64_t refs;
    bool closed;
    /* The bell, made at the first ask. */
    bool bell;
    bool rung;
#if defined(_WIN32)
    hero_net_socket *bell_socket;
    int64_t bell_hold;
    SOCKET bell_fd;
    struct sockaddr_in bell_at;
#else
    hero_stream *bell_in;
    hero_stream *bell_out;
    int bell_in_fd;
    int bell_out_fd;
#endif
};

struct hero_audio_list {
    char *bytes;
    size_t len;
    size_t cap;
};

typedef struct {
    hero_audio_core *state;
    uint64_t life;
} hero_audio_slot;

/* The table of streams: slots, the slots ever used, and their lock. */
static hero_audio_slot hero_audio_slots[HERO_AUDIO_SLOTS];
static size_t hero_audio_fresh;
#if defined(_WIN32)
static SRWLOCK hero_audio_table_lock = SRWLOCK_INIT;
#else
static pthread_mutex_t hero_audio_table_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

/* The calling thread's last failure. */
static _Thread_local int64_t hero_audio_kind;
static _Thread_local int64_t hero_audio_os;
static _Thread_local char hero_audio_words[HERO_AUDIO_WORDS];
static _Thread_local size_t hero_audio_words_len;

/* LOCKS, CONDITIONS, THREADS AND THE CLOCK. */

static void hero_audio_mutex_init(hero_audio_mutex *m) {
#if defined(_WIN32)
    InitializeSRWLock(m);
#else
    (void)pthread_mutex_init(m, NULL);
#endif
}

static void hero_audio_mutex_drop(hero_audio_mutex *m) {
#if defined(_WIN32)
    (void)m;
#else
    (void)pthread_mutex_destroy(m);
#endif
}

static void hero_audio_enter(hero_audio_mutex *m) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(m);
#else
    (void)pthread_mutex_lock(m);
#endif
}

static void hero_audio_leave(hero_audio_mutex *m) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(m);
#else
    (void)pthread_mutex_unlock(m);
#endif
}

static void hero_audio_cond_init(hero_audio_cond *c) {
#if defined(_WIN32)
    InitializeConditionVariable(c);
#else
    (void)pthread_cond_init(c, NULL);
#endif
}

static void hero_audio_cond_drop(hero_audio_cond *c) {
#if defined(_WIN32)
    (void)c;
#else
    (void)pthread_cond_destroy(c);
#endif
}

static void hero_audio_wake(hero_audio_cond *c) {
#if defined(_WIN32)
    WakeAllConditionVariable(c);
#else
    (void)pthread_cond_broadcast(c);
#endif
}

/* Waits on `c` under `m` for at most `span` nanoseconds, waking early where
 * the condition is signalled or the system wakes it for nothing. */
static void hero_audio_sleep_on(hero_audio_cond *c, hero_audio_mutex *m, int64_t span) {
    if (span <= 0) {
        return;
    }
#if defined(_WIN32)
    DWORD ms = (DWORD)((span + 999999) / 1000000);

    (void)SleepConditionVariableSRW(c, m, ms == 0 ? 1 : ms, 0);
#else
    struct timespec at;

    (void)clock_gettime(CLOCK_REALTIME, &at);
    int64_t nanos = (int64_t)at.tv_nsec + span % INT64_C(1000000000);

    at.tv_sec += (time_t)(span / INT64_C(1000000000) + nanos / INT64_C(1000000000));
    at.tv_nsec = (long)(nanos % INT64_C(1000000000));
    (void)pthread_cond_timedwait(c, m, &at);
#endif
}

/* The monotonic clock in nanoseconds. */
static int64_t hero_audio_now(void) {
#if defined(_WIN32)
    LARGE_INTEGER count;
    LARGE_INTEGER rate;

    (void)QueryPerformanceCounter(&count);
    (void)QueryPerformanceFrequency(&rate);
    return (int64_t)((double)count.QuadPart * 1e9 / (double)rate.QuadPart);
#else
    struct timespec at;

    (void)clock_gettime(CLOCK_MONOTONIC, &at);
    return (int64_t)at.tv_sec * INT64_C(1000000000) + (int64_t)at.tv_nsec;
#endif
}

/* The moment `limit_ns` from now, the largest where it would pass it. */
static int64_t hero_audio_deadline(int64_t limit_ns) {
    int64_t now = hero_audio_now();

    return limit_ns > INT64_MAX - now ? INT64_MAX : now + limit_ns;
}

/* FAILURE. */

int64_t hero_audio_arm_fail(int64_t kind, int64_t os_error, const char *words) {
    hero_audio_kind = kind;
    hero_audio_os = os_error;
    hero_audio_words_len = 0;
    if (words != NULL) {
        size_t n = strlen(words);

        if (n >= sizeof hero_audio_words) {
            n = sizeof hero_audio_words - 1;
        }
        memcpy(hero_audio_words, words, n);
        hero_audio_words_len = n;
    }
    return -1;
}

static void hero_audio_clear(void) {
    hero_audio_kind = HERO_AUDIO_OK;
    hero_audio_os = 0;
    hero_audio_words_len = 0;
}

int64_t hero_audio_last_error(void) {
    return hero_audio_kind;
}

int64_t hero_audio_last_os_error(void) {
    return hero_audio_os;
}

int64_t hero_audio_last_message(void *buffer, uint64_t count) {
    if (buffer == NULL || count == 0) {
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "no room for the message");
    }
    size_t n = hero_audio_words_len;

    if (n > count) {
        n = (size_t)count;
        /* Cut where a character ends: never before a continuation byte. */
        while (n > 0 && ((unsigned char)hero_audio_words[n] & 0xC0) == 0x80) {
            n -= 1;
        }
    }
    memcpy(buffer, hero_audio_words, n);
    return (int64_t)n;
}

/* Whether the `n` bytes of `s` are UTF-8: no overlong form, no surrogate,
 * nothing past U+10FFFF. */
static bool hero_audio_utf8(const unsigned char *s, size_t n) {
    size_t i = 0;

    while (i < n) {
        unsigned char c = s[i];
        size_t more;
        uint32_t code;

        if (c < 0x80) {
            i += 1;
            continue;
        } else if (c >= 0xC2 && c <= 0xDF) {
            more = 1;
            code = c & 0x1F;
        } else if (c >= 0xE0 && c <= 0xEF) {
            more = 2;
            code = c & 0x0F;
        } else if (c >= 0xF0 && c <= 0xF4) {
            more = 3;
            code = c & 0x07;
        } else {
            return false;
        }
        if (n - i <= more) {
            return false;
        }
        for (size_t k = 1; k <= more; k += 1) {
            if ((s[i + k] & 0xC0) != 0x80) {
                return false;
            }
            code = (code << 6) | (uint32_t)(s[i + k] & 0x3F);
        }
        if ((more == 2 && (code < 0x800 || (code >= 0xD800 && code <= 0xDFFF))) || (more == 3 && (code < 0x10000 || code > 0x10FFFF))) {
            return false;
        }
        i += more + 1;
    }
    return true;
}

/* THE LIST OF DEVICES. */

static bool hero_audio_list_put(hero_audio_list *list, const char *field) {
    size_t n = strlen(field) + 1;

    if (list->len + n > list->cap) {
        size_t cap = list->cap == 0 ? 1024 : list->cap;

        while (cap < list->len + n) {
            cap *= 2;
        }
        char *grown = hero_layer_grow(list->bytes, cap);

        if (grown == NULL) {
            return false;
        }
        list->bytes = grown;
        list->cap = cap;
    }
    memcpy(list->bytes + list->len, field, n);
    list->len += n;
    return true;
}

int64_t hero_audio_list_add(hero_audio_list *list, int64_t direction, int is_default, const char *id, const char *name, int64_t rate, int64_t channels, const char *encoding) {
    if (id == NULL || name == NULL || !hero_audio_utf8((const unsigned char *)id, strlen(id)) || !hero_audio_utf8((const unsigned char *)name, strlen(name))) {
        return 0;
    }
    char numbers[2][24];

    (void)snprintf(numbers[0], sizeof numbers[0], "%lld", (long long)rate);
    (void)snprintf(numbers[1], sizeof numbers[1], "%lld", (long long)channels);
    size_t was = list->len;
    const char *fields[7] = {
        direction == HERO_AUDIO_INPUT ? "input" : "output", is_default ? "default" : "", id, name, numbers[0], numbers[1], encoding};

    for (int i = 0; i < 7; i += 1) {
        if (!hero_audio_list_put(list, fields[i])) {
            list->len = was;
            return hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no memory for the list of devices");
        }
    }
    return 0;
}

int64_t hero_audio_devices(void *buffer, uint64_t count) {
    hero_audio_list list = {NULL, 0, 0};

    if (hero_audio_arm_devices(&list) < 0) {
        hero_layer_free(list.bytes);
        return -1;
    }
    if (list.len <= count && list.len > 0) {
        if (buffer == NULL) {
            hero_layer_free(list.bytes);
            return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a count of bytes with no buffer behind it");
        }
        memcpy(buffer, list.bytes, list.len);
    }
    hero_layer_free(list.bytes);
    hero_audio_clear();
    return (int64_t)list.len;
}

int64_t hero_audio_system(void *buffer, uint64_t count) {
    char name[64];
    int64_t n = hero_audio_arm_system(name, sizeof name);

    if (n < 0) {
        return -1;
    }
    if ((uint64_t)n <= count) {
        if (buffer == NULL && n > 0) {
            return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a count of bytes with no buffer behind it");
        }
        memcpy(buffer, name, (size_t)n);
    }
    hero_audio_clear();
    return n;
}

/* THE BELL. Under the stream's lock. */

static bool hero_audio_ready(hero_audio_core *st) {
    uint64_t waiting = st->written - st->taken;

    if (st->done || st->gone) {
        return true;
    }
    return st->direction == HERO_AUDIO_OUTPUT ? waiting < st->room : waiting > 0;
}

static void hero_audio_bell_sync(hero_audio_core *st) {
    if (!st->bell) {
        return;
    }
    bool ready = hero_audio_ready(st);

    if (ready && !st->rung) {
#if defined(_WIN32)
        if (sendto(st->bell_fd, "\1", 1, 0, (const struct sockaddr *)&st->bell_at, (int)sizeof st->bell_at) == 1) {
            st->rung = true;
        }
#else
        char one = 1;

        if (write(st->bell_out_fd, &one, 1) == 1) {
            st->rung = true;
        }
#endif
    } else if (!ready) {
#if defined(_WIN32)
        /* Every datagram there, a stranger's among them: the socket is the
         * layer's, non-blocking. */
        char drop[64];

        while (recv(st->bell_fd, drop, (int)sizeof drop, 0) >= 0) {
        }
        st->rung = false;
#else
        if (st->rung) {
            char drop[8];

            while (read(st->bell_in_fd, drop, sizeof drop) > 0) {
            }
            st->rung = false;
        }
#endif
    }
}

/* Wakes every waiter and keeps the bell true. Under the stream's lock. */
static void hero_audio_moved(hero_audio_core *st) {
    hero_audio_wake(&st->moved);
    hero_audio_bell_sync(st);
}

/* THE CORE, for the arms. */

size_t hero_audio_pull(hero_audio_core *st, float *out, size_t frames) {
    size_t channels = (size_t)st->channels;
    size_t got = 0;

    hero_audio_enter(&st->lock);
    if (!st->paused && !st->done && !st->closing) {
        uint64_t waiting = st->written - st->taken;

        got = waiting < frames ? (size_t)waiting : frames;
        float gain = (float)st->volume;

        for (size_t f = 0; f < got; f += 1) {
            size_t at = (size_t)((st->taken + f) % st->room) * channels;

            for (size_t c = 0; c < channels; c += 1) {
                out[f * channels + c] = st->ring[at + c] * gain;
            }
        }
        st->taken += got;
        st->position += got;
        if (got < frames && !st->finished && st->written > 0) {
            st->dropped += frames - got;
        }
        if (got > 0) {
            hero_audio_moved(st);
        }
    }
    hero_audio_leave(&st->lock);
    memset(out + got * channels, 0, (frames - got) * channels * sizeof(float));
    return got;
}

int hero_audio_drained(hero_audio_core *st) {
    hero_audio_enter(&st->lock);
    int drained = st->finished && st->written == st->taken && !st->done;

    hero_audio_leave(&st->lock);
    return drained;
}

void hero_audio_played_out(hero_audio_core *st) {
    hero_audio_enter(&st->lock);
    st->done = true;
    hero_audio_moved(st);
    hero_audio_leave(&st->lock);
}

size_t hero_audio_push(hero_audio_core *st, const float *in, size_t frames) {
    size_t channels = (size_t)st->channels;
    size_t kept = 0;

    hero_audio_enter(&st->lock);
    if (!st->paused && !st->done && !st->closing) {
        uint64_t room = st->room - (st->written - st->taken);

        kept = room < frames ? (size_t)room : frames;
        float gain = (float)st->volume;

        for (size_t f = 0; f < kept; f += 1) {
            size_t at = (size_t)((st->written + f) % st->room) * channels;

            for (size_t c = 0; c < channels; c += 1) {
                st->ring[at + c] = in[f * channels + c] * gain;
            }
        }
        st->written += kept;
        st->position += frames;
        st->dropped += frames - kept;
        if (kept > 0) {
            hero_audio_moved(st);
        }
    }
    hero_audio_leave(&st->lock);
    return kept;
}

void hero_audio_lost(hero_audio_core *st, int64_t os_error, const char *words) {
    hero_audio_enter(&st->lock);
    if (!st->gone) {
        st->gone = true;
        st->gone_os = os_error;
        size_t n = words == NULL ? 0 : strlen(words);

        if (n >= sizeof st->gone_words) {
            n = sizeof st->gone_words - 1;
        }
        if (n > 0) {
            memcpy(st->gone_words, words, n);
        }
        st->gone_words[n] = 0;
    }
    hero_audio_moved(st);
    hero_audio_leave(&st->lock);
}

/* THE NULL DEVICE. Its thread moves the frames a device at the stream's rate
 * would have moved since its last step, then sleeps a step on the stream's
 * condition, which `hero_audio_close` wakes. */

#if defined(_WIN32)
static DWORD WINAPI hero_audio_null_run(LPVOID given)
#else
static void *hero_audio_null_run(void *given)
#endif
{
    hero_audio_core *st = given;
    size_t channels = (size_t)st->channels;
    float *frames = hero_layer_alloc((size_t)HERO_AUDIO_NULL_FRAMES * channels * sizeof(float));
    int64_t last = hero_audio_now();
    /* Elapsed time times the rate, not yet made frames: so no frame is lost
     * to rounding, whatever the step. */
    int64_t owed = 0;

    hero_audio_enter(&st->lock);
    while (frames != NULL && !st->closing) {
        bool running = !st->paused && !st->done;

        hero_audio_sleep_on(&st->moved, &st->lock, HERO_AUDIO_STEP_NS);
        int64_t now = hero_audio_now();
        int64_t span = now - last;

        last = now;
        if (!running || st->closing) {
            continue;
        }
        if (span > HERO_AUDIO_CATCH_NS) {
            span = HERO_AUDIO_CATCH_NS;
        }
        owed += span * st->rate;
        size_t due = (size_t)(owed / INT64_C(1000000000));

        owed -= (int64_t)due * INT64_C(1000000000);
        hero_audio_leave(&st->lock);
        while (due > 0) {
            size_t n = due < HERO_AUDIO_NULL_FRAMES ? due : HERO_AUDIO_NULL_FRAMES;

            if (st->direction == HERO_AUDIO_OUTPUT) {
                (void)hero_audio_pull(st, frames, n);
                if (hero_audio_drained(st)) {
                    hero_audio_played_out(st);
                }
            } else {
                for (size_t f = 0; f < n; f += 1) {
                    uint64_t k = st->null_frame + f;
                    double turn = (double)((k * 440) % (uint64_t)st->rate) / (double)st->rate;
                    float x = (float)(0.5 * sin(2.0 * 3.14159265358979323846 * turn));

                    for (size_t c = 0; c < channels; c += 1) {
                        frames[f * channels + c] = x;
                    }
                }
                st->null_frame += n;
                (void)hero_audio_push(st, frames, n);
            }
            due -= n;
        }
        hero_audio_enter(&st->lock);
    }
    hero_audio_leave(&st->lock);
    hero_layer_free(frames);
#if defined(_WIN32)
    return 0;
#else
    return NULL;
#endif
}

static bool hero_audio_null_start(hero_audio_core *st) {
#if defined(_WIN32)
    st->null_thread = CreateThread(NULL, 0, hero_audio_null_run, st, 0, NULL);
    st->null_running = st->null_thread != NULL;
#else
    st->null_running = pthread_create(&st->null_thread, NULL, hero_audio_null_run, st) == 0;
#endif
    return st->null_running;
}

static void hero_audio_null_join(hero_audio_core *st) {
    if (!st->null_running) {
        return;
    }
#if defined(_WIN32)
    (void)WaitForSingleObject(st->null_thread, INFINITE);
    (void)CloseHandle(st->null_thread);
#else
    (void)pthread_join(st->null_thread, NULL);
#endif
    st->null_running = false;
}

/* THE TABLE. */

static void hero_audio_table_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_audio_table_lock);
#else
    (void)pthread_mutex_lock(&hero_audio_table_lock);
#endif
}

static void hero_audio_table_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_audio_table_lock);
#else
    (void)pthread_mutex_unlock(&hero_audio_table_lock);
#endif
}

/* The state `s` names, a reference to it taken, or NULL where `s` is no open
 * stream of this table. */
static hero_audio_core *hero_audio_take(hero_audio_stream *s) {
    uint64_t value = (uint64_t)(uintptr_t)s;
    uint64_t low = value & ((UINT64_C(1) << HERO_AUDIO_INDEX_BITS) - 1);
    hero_audio_core *st = NULL;

    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_AUDIO) || low == 0 || low > HERO_AUDIO_SLOTS) {
        return NULL;
    }
    hero_audio_table_enter();
    hero_audio_slot *slot = &hero_audio_slots[low - 1];

    if (low <= hero_audio_fresh && slot->state != NULL && slot->life == (value & ~HERO_HANDLE_TAG_MASK) >> HERO_AUDIO_INDEX_BITS) {
        st = slot->state;
        st->refs += 1;
    }
    hero_audio_table_leave();
    return st;
}

static void hero_audio_free(hero_audio_core *st) {
#if defined(_WIN32)
    if (st->bell_socket != NULL) {
        hero_net_unhold(st->bell_hold);
        (void)hero_net_close(st->bell_socket);
    }
#else
    if (st->bell_in != NULL) {
        (void)hero_stream_release(st->bell_in);
        (void)hero_stream_close(st->bell_in);
    }
    if (st->bell_out != NULL) {
        (void)hero_stream_release(st->bell_out);
        (void)hero_stream_close(st->bell_out);
    }
#endif
    hero_audio_cond_drop(&st->moved);
    hero_audio_mutex_drop(&st->lock);
    hero_layer_free(st->ring);
    hero_layer_free(st);
}

/* Gives back the reference `hero_audio_take` took, freeing the state where it
 * was the last of a closed stream. */
static void hero_audio_give(hero_audio_core *st) {
    hero_audio_table_enter();
    st->refs -= 1;
    bool last = st->refs == 0 && st->closed;

    hero_audio_table_leave();
    if (last) {
        hero_audio_free(st);
    }
}

/* Puts `st` in a free slot and answers its handle, or NULL where none is. */
static hero_audio_stream *hero_audio_issue(hero_audio_core *st) {
    hero_audio_stream *made = NULL;

    hero_audio_table_enter();
    for (size_t i = 0; i < HERO_AUDIO_SLOTS; i += 1) {
        hero_audio_slot *slot = &hero_audio_slots[i];

        if (slot->state == NULL && (i < hero_audio_fresh || i == hero_audio_fresh) && slot->life < HERO_AUDIO_LIFE_MOST) {
            if (i == hero_audio_fresh) {
                hero_audio_fresh += 1;
            }
            slot->life += 1;
            slot->state = st;
            uint64_t value = HERO_HANDLE_TAG(HERO_HANDLE_TAG_AUDIO) | (slot->life << HERO_AUDIO_INDEX_BITS) | (uint64_t)(i + 1);

            made = (hero_audio_stream *)(uintptr_t)value;
            break;
        }
    }
    hero_audio_table_leave();
    return made;
}

/* THE PROGRAM'S SIDE. */

hero_audio_stream *hero_audio_open(const char *device, int64_t direction, int64_t rate, int64_t channels, int64_t room) {
    if (direction != HERO_AUDIO_OUTPUT && direction != HERO_AUDIO_INPUT) {
        (void)hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a direction is HERO_AUDIO_OUTPUT or HERO_AUDIO_INPUT");
        return NULL;
    }
    if (rate < 1 || rate > HERO_AUDIO_RATE_MOST || channels < 1 || channels > HERO_AUDIO_CHANNELS_MOST) {
        (void)hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a stream runs at 1 to 768000 frames a second and 1 to 64 channels");
        return NULL;
    }
    if (room < 1 || room > HERO_AUDIO_ROOM_MOST || room * channels > HERO_AUDIO_SAMPLES_MOST) {
        (void)hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a stream's room holds 1 to 268435456 frames and at most 536870912 samples");
        return NULL;
    }
    bool null_device = device != NULL && strcmp(device, HERO_AUDIO_NULL) == 0;

    if (device != NULL && device[0] == 0) {
        device = NULL;
    }
    hero_audio_core *st = hero_layer_alloc_zero(1, sizeof *st);
    float *ring = st == NULL ? NULL : hero_layer_alloc((size_t)room * (size_t)channels * sizeof(float));

    if (ring == NULL) {
        hero_layer_free(st);
        (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no memory for the stream's room");
        return NULL;
    }
    hero_audio_mutex_init(&st->lock);
    hero_audio_cond_init(&st->moved);
    st->direction = direction;
    st->rate = rate;
    st->channels = channels;
    st->ring = ring;
    st->room = (uint64_t)room;
    st->volume = 1.0;
    st->null_device = null_device;
#if defined(_WIN32)
    st->bell_fd = INVALID_SOCKET;
#else
    st->bell_in_fd = -1;
    st->bell_out_fd = -1;
#endif
    bool started = null_device ? hero_audio_null_start(st) : hero_audio_arm_open(st, device, direction, rate, channels, &st->arm) == 0;

    if (!started) {
        if (null_device) {
            (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no thread for the null device");
        }
        hero_audio_free(st);
        return NULL;
    }
    hero_audio_stream *made = hero_audio_issue(st);

    if (made == NULL) {
        hero_audio_enter(&st->lock);
        st->closing = true;
        hero_audio_moved(st);
        hero_audio_leave(&st->lock);
        if (null_device) {
            hero_audio_null_join(st);
        } else {
            hero_audio_arm_close(st->arm);
        }
        hero_audio_free(st);
        (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "1023 streams are open, the most this layer holds");
        return NULL;
    }
    hero_audio_clear();
    return made;
}

/* What a wait does between its slices: gives the lock back, asks the stop,
 * takes the lock again; answers whether the stop was asked. */
static bool hero_audio_stop_between(hero_audio_core *st) {
    hero_audio_leave(&st->lock);
    bool asked = hero_net_stop_asked() != 0;

    hero_audio_enter(&st->lock);
    return asked;
}

/* Sleeps on the stream until `ends`, a slice at most; answers false where
 * `ends` has passed. Under the stream's lock. */
static bool hero_audio_slice(hero_audio_core *st, int64_t ends) {
    int64_t now = hero_audio_now();

    if (now >= ends) {
        return false;
    }
    int64_t span = ends - now;

    hero_audio_sleep_on(&st->moved, &st->lock, span < HERO_AUDIO_SLICE_NS ? span : HERO_AUDIO_SLICE_NS);
    return true;
}

int64_t hero_audio_write(hero_audio_stream *s, const float *samples, uint64_t count, int64_t limit_ns) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    if (st->direction != HERO_AUDIO_OUTPUT || limit_ns < 0 || count % (uint64_t)st->channels != 0 || (samples == NULL && count > 0)) {
        hero_audio_give(st);
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, st->direction != HERO_AUDIO_OUTPUT ? "an input is read, never written" : "a write takes whole frames and a limit of zero or more");
    }
    const float *from = samples;
    uint64_t frames = count / (uint64_t)st->channels;
    uint64_t queued = 0;
    size_t channels = (size_t)st->channels;
    int64_t ends = hero_audio_deadline(limit_ns);
    int64_t kind = HERO_AUDIO_OK;

    hero_audio_enter(&st->lock);
    for (;;) {
        if (st->closing) {
            kind = HERO_AUDIO_NOT_OPEN;
            break;
        }
        if (st->gone) {
            kind = HERO_AUDIO_LOST;
            break;
        }
        if (st->done || st->finished) {
            kind = HERO_AUDIO_ENDED;
            break;
        }
        uint64_t room = st->room - (st->written - st->taken);
        uint64_t n = frames - queued < room ? frames - queued : room;

        for (uint64_t f = 0; f < n; f += 1) {
            size_t at = (size_t)((st->written + f) % st->room) * channels;

            memcpy(st->ring + at, from + (size_t)(queued + f) * channels, channels * sizeof(float));
        }
        st->written += n;
        queued += n;
        if (n > 0) {
            hero_audio_moved(st);
        }
        if (queued == frames || !hero_audio_slice(st, ends)) {
            break;
        }
        if (hero_audio_stop_between(st)) {
            kind = queued > 0 ? HERO_AUDIO_OK : HERO_AUDIO_STOPPED;
            break;
        }
    }
    hero_audio_leave(&st->lock);
    hero_audio_give(st);
    if (kind != HERO_AUDIO_OK && !(queued > 0 && kind != HERO_AUDIO_NOT_OPEN)) {
        return hero_audio_arm_fail(kind, 0, kind == HERO_AUDIO_ENDED ? "the stream has ended: finished or stopped" : kind == HERO_AUDIO_LOST ? "the device went away" : kind == HERO_AUDIO_STOPPED ? "the program was asked to stop" : "the stream is closed");
    }
    hero_audio_clear();
    return (int64_t)queued;
}

int64_t hero_audio_read(hero_audio_stream *s, float *buffer, uint64_t count, int64_t limit_ns) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    uint64_t most = count / (uint64_t)st->channels;

    if (st->direction != HERO_AUDIO_INPUT || limit_ns < 0 || most == 0 || buffer == NULL) {
        hero_audio_give(st);
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, st->direction != HERO_AUDIO_INPUT ? "an output is written, never read" : "a read takes room for a frame and a limit of zero or more");
    }
    float *into = buffer;
    size_t channels = (size_t)st->channels;
    int64_t ends = hero_audio_deadline(limit_ns);
    int64_t kind = HERO_AUDIO_OK;
    uint64_t moved = 0;

    hero_audio_enter(&st->lock);
    for (;;) {
        if (st->closing) {
            kind = HERO_AUDIO_NOT_OPEN;
            break;
        }
        uint64_t waiting = st->written - st->taken;

        if (waiting > 0) {
            moved = waiting < most ? waiting : most;
            for (uint64_t f = 0; f < moved; f += 1) {
                size_t at = (size_t)((st->taken + f) % st->room) * channels;

                memcpy(into + (size_t)f * channels, st->ring + at, channels * sizeof(float));
            }
            st->taken += moved;
            hero_audio_moved(st);
            break;
        }
        if (st->gone) {
            kind = HERO_AUDIO_LOST;
            break;
        }
        if (st->done) {
            kind = HERO_AUDIO_ENDED;
            break;
        }
        if (!hero_audio_slice(st, ends)) {
            break;
        }
        if (hero_audio_stop_between(st)) {
            kind = HERO_AUDIO_STOPPED;
            break;
        }
    }
    hero_audio_leave(&st->lock);
    hero_audio_give(st);
    if (kind != HERO_AUDIO_OK) {
        return hero_audio_arm_fail(kind, 0, kind == HERO_AUDIO_ENDED ? "the stream has ended and every frame was read" : kind == HERO_AUDIO_LOST ? "the device went away" : kind == HERO_AUDIO_STOPPED ? "the program was asked to stop" : "the stream is closed");
    }
    hero_audio_clear();
    return (int64_t)moved;
}

int64_t hero_audio_finish(hero_audio_stream *s) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    if (st->direction != HERO_AUDIO_OUTPUT) {
        hero_audio_give(st);
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "an input is stopped, never finished");
    }
    hero_audio_enter(&st->lock);
    st->finished = true;
    hero_audio_moved(st);
    hero_audio_leave(&st->lock);
    hero_audio_give(st);
    hero_audio_clear();
    return 0;
}

int64_t hero_audio_wait(hero_audio_stream *s, int64_t limit_ns) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    if (limit_ns < 0) {
        hero_audio_give(st);
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a limit is zero or more");
    }
    int64_t ends = hero_audio_deadline(limit_ns);
    int64_t kind = HERO_AUDIO_OK;

    hero_audio_enter(&st->lock);
    for (;;) {
        if (st->closing) {
            kind = HERO_AUDIO_NOT_OPEN;
            break;
        }
        if (st->done) {
            break;
        }
        if (st->gone) {
            kind = HERO_AUDIO_LOST;
            break;
        }
        if (!hero_audio_slice(st, ends)) {
            kind = HERO_AUDIO_TIMEOUT;
            break;
        }
        if (hero_audio_stop_between(st)) {
            kind = HERO_AUDIO_STOPPED;
            break;
        }
    }
    hero_audio_leave(&st->lock);
    hero_audio_give(st);
    if (kind != HERO_AUDIO_OK) {
        return hero_audio_arm_fail(kind, 0, kind == HERO_AUDIO_TIMEOUT ? "the stream had not ended when the limit passed" : kind == HERO_AUDIO_LOST ? "the device went away" : kind == HERO_AUDIO_STOPPED ? "the program was asked to stop" : "the stream is closed");
    }
    hero_audio_clear();
    return 0;
}

/* Pauses or resumes `s`: the flag first, so the system's next pull or push
 * sees it, then the system's own stream. */
static int64_t hero_audio_hold(hero_audio_stream *s, bool pause) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    hero_audio_enter(&st->lock);
    int64_t kind = st->gone ? HERO_AUDIO_LOST : st->done ? HERO_AUDIO_ENDED : HERO_AUDIO_OK;
    bool change = kind == HERO_AUDIO_OK && st->paused != pause;

    if (change) {
        st->paused = pause;
        hero_audio_moved(st);
    }
    hero_audio_leave(&st->lock);
    int64_t answer = 0;

    if (kind != HERO_AUDIO_OK) {
        answer = hero_audio_arm_fail(kind, 0, kind == HERO_AUDIO_LOST ? "the device went away" : "the stream has ended: finished or stopped");
    } else if (change && !st->null_device) {
        answer = pause ? hero_audio_arm_pause(st->arm) : hero_audio_arm_resume(st->arm);
        if (answer < 0) {
            hero_audio_enter(&st->lock);
            st->paused = !pause;
            hero_audio_moved(st);
            hero_audio_leave(&st->lock);
        }
    }
    hero_audio_give(st);
    if (answer == 0) {
        hero_audio_clear();
    }
    return answer;
}

int64_t hero_audio_pause(hero_audio_stream *s) {
    return hero_audio_hold(s, true);
}

int64_t hero_audio_resume(hero_audio_stream *s) {
    return hero_audio_hold(s, false);
}

int64_t hero_audio_stop(hero_audio_stream *s) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    hero_audio_enter(&st->lock);
    bool was = st->done;

    if (!was) {
        st->done = true;
        if (st->direction == HERO_AUDIO_OUTPUT) {
            st->taken = st->written;
        }
        hero_audio_moved(st);
    }
    hero_audio_leave(&st->lock);
    if (!was && !st->null_device) {
        hero_audio_arm_stop(st->arm);
    }
    hero_audio_give(st);
    hero_audio_clear();
    return 0;
}

int64_t hero_audio_set_volume(hero_audio_stream *s, double volume) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    if (!(volume >= 0.0 && volume <= 1.0)) {
        hero_audio_give(st);
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a volume is 0 to 1");
    }
    hero_audio_enter(&st->lock);
    st->volume = volume;
    hero_audio_leave(&st->lock);
    hero_audio_give(st);
    hero_audio_clear();
    return 0;
}

/* One number of the stream read under its lock: 0 the state, 1 the
 * position, 2 the frames waiting, 3 the frames dropped. */
static int64_t hero_audio_read_number(hero_audio_stream *s, int which) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    hero_audio_enter(&st->lock);
    int64_t answer = which == 0   ? (st->gone ? HERO_AUDIO_GONE : st->done ? HERO_AUDIO_DONE : st->paused ? HERO_AUDIO_PAUSED : HERO_AUDIO_RUNNING)
                     : which == 1 ? (int64_t)st->position
                     : which == 2 ? (int64_t)(st->written - st->taken)
                                  : (int64_t)st->dropped;

    hero_audio_leave(&st->lock);
    hero_audio_give(st);
    hero_audio_clear();
    return answer;
}

int64_t hero_audio_state(hero_audio_stream *s) {
    return hero_audio_read_number(s, 0);
}

int64_t hero_audio_position(hero_audio_stream *s) {
    return hero_audio_read_number(s, 1);
}

int64_t hero_audio_waiting(hero_audio_stream *s) {
    return hero_audio_read_number(s, 2);
}

int64_t hero_audio_dropped(hero_audio_stream *s) {
    return hero_audio_read_number(s, 3);
}

hero_stream *hero_audio_bell_stream(hero_audio_stream *s) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        (void)hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
        return NULL;
    }
#if defined(_WIN32)
    hero_audio_give(st);
    (void)hero_audio_arm_fail(HERO_AUDIO_UNSUPPORTED, 0, "on Windows the bell is a socket: hero_audio_bell_socket");
    return NULL;
#else
    hero_audio_enter(&st->lock);
    hero_stream *made = st->bell ? st->bell_in : NULL;

    hero_audio_leave(&st->lock);
    if (made == NULL) {
        /* Made outside the lock: no other member is called under it. */
        hero_stream *in = NULL;
        hero_stream *out = NULL;

        if (hero_stream_pipe(&in, &out) != 0) {
            hero_audio_give(st);
            (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, hero_stream_last_os_error(), "no pipe for the bell");
            return NULL;
        }
        int64_t in_fd = hero_stream_hold(in);
        int64_t out_fd = in_fd < 0 ? -1 : hero_stream_hold(out);

        if (in_fd < 0 || out_fd < 0 || in_fd > INT32_MAX || out_fd > INT32_MAX) {
            if (in_fd >= 0) {
                (void)hero_stream_release(in);
            }
            if (out_fd >= 0) {
                (void)hero_stream_release(out);
            }
            (void)hero_stream_close(in);
            (void)hero_stream_close(out);
            hero_audio_give(st);
            (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no descriptor for the bell");
            return NULL;
        }
        /* The pipe is the stream's alone: neither end ever waits. */
        for (int i = 0; i < 2; i += 1) {
            int fd = (int)(i == 0 ? in_fd : out_fd);
            int flags = fcntl(fd, F_GETFL);

            if (flags >= 0) {
                (void)fcntl(fd, F_SETFL, flags | O_NONBLOCK);
            }
        }
        hero_audio_enter(&st->lock);
        bool kept = !st->bell;

        if (kept) {
            st->bell_in = in;
            st->bell_out = out;
            st->bell_in_fd = (int)in_fd;
            st->bell_out_fd = (int)out_fd;
            st->bell = true;
            hero_audio_bell_sync(st);
        }
        made = st->bell_in;
        hero_audio_leave(&st->lock);
        if (!kept) {
            (void)hero_stream_release(in);
            (void)hero_stream_release(out);
            (void)hero_stream_close(in);
            (void)hero_stream_close(out);
        }
    }
    hero_audio_give(st);
    hero_audio_clear();
    return made;
#endif
}

hero_net_socket *hero_audio_bell_socket(hero_audio_stream *s) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        (void)hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
        return NULL;
    }
#if !defined(_WIN32)
    hero_audio_give(st);
    (void)hero_audio_arm_fail(HERO_AUDIO_UNSUPPORTED, 0, "off Windows the bell is a pipe: hero_audio_bell_stream");
    return NULL;
#else
    hero_audio_enter(&st->lock);
    hero_net_socket *made = st->bell ? st->bell_socket : NULL;

    hero_audio_leave(&st->lock);
    if (made == NULL) {
        hero_net_socket *u = hero_net_udp("127.0.0.1", 0);

        if (u == NULL) {
            hero_audio_give(st);
            (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, hero_net_last_os_error(), "no socket for the bell");
            return NULL;
        }
        int64_t port = hero_net_port(u);
        int64_t fd = -1;
        int64_t hold = port <= 0 ? -1 : hero_net_hold(u, &fd);

        if (hold < 0 || port > 65535) {
            if (hold >= 0) {
                hero_net_unhold(hold);
            }
            (void)hero_net_close(u);
            hero_audio_give(st);
            (void)hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no descriptor for the bell");
            return NULL;
        }
        hero_audio_enter(&st->lock);
        bool kept = !st->bell;

        if (kept) {
            st->bell_socket = u;
            st->bell_hold = hold;
            st->bell_fd = (SOCKET)(uint64_t)fd;
            memset(&st->bell_at, 0, sizeof st->bell_at);
            st->bell_at.sin_family = AF_INET;
            st->bell_at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            st->bell_at.sin_port = htons((u_short)port);
            st->bell = true;
            hero_audio_bell_sync(st);
        }
        made = st->bell_socket;
        hero_audio_leave(&st->lock);
        if (!kept) {
            hero_net_unhold(hold);
            (void)hero_net_close(u);
        }
    }
    hero_audio_give(st);
    hero_audio_clear();
    return made;
#endif
}

int64_t hero_audio_close(hero_audio_stream *s) {
    hero_audio_core *st = hero_audio_take(s);

    if (st == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    /* The slot first, so no call finds the stream again. */
    uint64_t value = (uint64_t)(uintptr_t)s;
    size_t index = (size_t)(value & ((UINT64_C(1) << HERO_AUDIO_INDEX_BITS) - 1)) - 1;

    hero_audio_table_enter();
    bool first = hero_audio_slots[index].state == st;

    if (first) {
        hero_audio_slots[index].state = NULL;
    }
    hero_audio_table_leave();
    if (!first) {
        hero_audio_give(st);
        return hero_audio_arm_fail(HERO_AUDIO_NOT_OPEN, 0, "the stream is closed");
    }
    hero_audio_enter(&st->lock);
    st->closing = true;
    hero_audio_moved(st);
    hero_audio_leave(&st->lock);
    if (st->null_device) {
        hero_audio_null_join(st);
    } else {
        hero_audio_arm_close(st->arm);
    }
    hero_audio_table_enter();
    st->closed = true;
    hero_audio_table_leave();
    hero_audio_give(st);
    hero_audio_clear();
    return 0;
}

int64_t hero_audio_permission(int64_t limit_ns) {
    if (limit_ns < 0) {
        return hero_audio_arm_fail(HERO_AUDIO_INVALID, 0, "a limit is zero or more");
    }
    int64_t answer = hero_audio_arm_permission(limit_ns);

    if (answer >= 0) {
        hero_audio_clear();
    }
    return answer;
}

int64_t hero_audio_beep(void) {
    int64_t answer = hero_audio_arm_beep();

    if (answer >= 0) {
        hero_audio_clear();
    }
    return answer;
}
