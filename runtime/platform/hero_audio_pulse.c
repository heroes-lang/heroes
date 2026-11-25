/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_audio_pulse.c — the arm of sound (`platform/hero_audio_arm.h`)
 * by PulseAudio's library, on every system neither Apple's nor Windows
 * (M-rebel-library, 2026-11-23). Empty elsewhere.
 *
 * THE LIBRARY is opened by `dlopen` at the first call, `libpulse.so.0` and
 * `libpulse-simple.so.0`, as `hero_keyring.c` opens libsecret, so a program
 * that never plays links none of it and a machine without it answers
 * `HERO_AUDIO_UNSUPPORTED` rather than failing to start. Its functions are
 * found ONCE inside `pthread_once` and only read after it. No header of the
 * library is read: the functions are declared here by their documented
 * prototypes, and of `pa_sink_info`, `pa_source_info` and `pa_server_info`
 * only the leading fields are read, which every release since 0.9 has kept
 * in their place. PipeWire's own server answers this library as PulseAudio's
 * does (`pipewire-pulse`), and names itself in its server name, which
 * `hero_audio_arm_system` reads.
 *
 * THE DEVICES are the server's sinks (outputs) and sources (inputs), each
 * named by its name, described by its description, at its sample spec; the
 * defaults are the server's. A source that monitors a sink is a source like
 * any other. Asked of a context on a main loop of this arm's own, iterated
 * without blocking for at most `HERO_AUDIO_PULSE_ASK_NS`, so a server that
 * does not answer is told rather than waited for.
 *
 * A STREAM is a `pa_simple` connection of interleaved 32-bit little-endian
 * floats, and one thread of this arm's own: an output's pulls a fiftieth of a
 * second from the core and writes it, the library blocking until the server
 * has room, and once the core has handed the last frame of a finished output
 * waits for the server to play it out (`pa_simple_drain`); an input's reads a
 * fiftieth of a second and pushes it. A pause plays silence (the core hands
 * none), so the server's buffer, 60 ms at most, is the pause's delay. A call
 * of the library that fails while the stream runs has lost its device. The
 * thread alone calls the connection, which `pa_simple` asks: a stop is a flag
 * it reads, and flushes the server's buffer itself.
 *
 * THE BEEP is libcanberra's event `bell` where `libcanberra.so.0` is there,
 * else a tenth of a second at 880 Hz played on the default output by a
 * thread of its own, so the call answers at once either way.
 */

#include "hero_audio_pulse.h"

#if !defined(_WIN32) && !defined(__APPLE__)
#include <dlfcn.h>
#include <math.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "hero_layer_alloc.h"
#include "hero_layer_maths.h"

/* How long a question to the server may take before it is told. */
#define HERO_AUDIO_PULSE_ASK_NS INT64_C(3000000000)
/* The library's numbers this arm uses (its `def.h`, `sample.h`). */
#define HERO_AUDIO_PULSE_PLAYBACK 1
#define HERO_AUDIO_PULSE_RECORD 2
#define HERO_AUDIO_PULSE_FLOAT32LE 5
#define HERO_AUDIO_PULSE_READY 4
#define HERO_AUDIO_PULSE_FAILED 5
#define HERO_AUDIO_PULSE_TERMINATED 6
#define HERO_AUDIO_PULSE_DONE 1
#define HERO_AUDIO_PULSE_NOAUTOSPAWN 1

typedef struct {
    int format;
    uint32_t rate;
    uint8_t channels;
} hero_pa_spec;

typedef struct {
    uint32_t maxlength;
    uint32_t tlength;
    uint32_t prebuf;
    uint32_t minreq;
    uint32_t fragsize;
} hero_pa_attr;

/* The leading fields of `pa_sink_info` and `pa_source_info`, the same four. */
typedef struct {
    const char *name;
    uint32_t index;
    const char *description;
    hero_pa_spec spec;
} hero_pa_device;

/* The leading fields of `pa_server_info`. */
typedef struct {
    const char *user_name;
    const char *host_name;
    const char *server_version;
    const char *server_name;
    hero_pa_spec spec;
    const char *default_sink_name;
    const char *default_source_name;
} hero_pa_server;

typedef void (*hero_pa_device_cb)(void *context, const hero_pa_device *device, int eol, void *given);
typedef void (*hero_pa_server_cb)(void *context, const hero_pa_server *server, void *given);

/* The library's functions, found once. */
typedef struct {
    void *(*simple_new)(const char *server, const char *name, int dir, const char *dev, const char *stream, const hero_pa_spec *spec, const void *map, const hero_pa_attr *attr, int *error);
    int (*simple_write)(void *s, const void *data, size_t bytes, int *error);
    int (*simple_read)(void *s, void *data, size_t bytes, int *error);
    int (*simple_drain)(void *s, int *error);
    int (*simple_flush)(void *s, int *error);
    void (*simple_free)(void *s);
    const char *(*strerror)(int error);
    void *(*mainloop_new)(void);
    void *(*mainloop_get_api)(void *m);
    int (*mainloop_iterate)(void *m, int block, int *retval);
    void (*mainloop_free)(void *m);
    void *(*context_new)(void *api, const char *name);
    int (*context_connect)(void *c, const char *server, int flags, const void *spawn);
    int (*context_get_state)(void *c);
    void (*context_disconnect)(void *c);
    void (*context_unref)(void *c);
    void *(*get_sinks)(void *c, hero_pa_device_cb cb, void *given);
    void *(*get_sources)(void *c, hero_pa_device_cb cb, void *given);
    void *(*get_server)(void *c, hero_pa_server_cb cb, void *given);
    int (*operation_get_state)(void *o);
    void (*operation_unref)(void *o);
} hero_audio_pulse_functions;

static hero_audio_pulse_functions hero_audio_pulse_fn;
static bool hero_audio_pulse_found;
static pthread_once_t hero_audio_pulse_once = PTHREAD_ONCE_INIT;

static void hero_audio_pulse_find(void) {
    void *pulse = dlopen("libpulse.so.0", RTLD_NOW | RTLD_LOCAL);
    void *simple = pulse == NULL ? NULL : dlopen("libpulse-simple.so.0", RTLD_NOW | RTLD_LOCAL);

    if (simple == NULL) {
        return;
    }
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;
    bool all = true;

#define HERO_PA_FIND(field, lib, symbol)                         \
    do {                                                         \
        *(void **)(&f->field) = dlsym((lib), (symbol));          \
        all = all && f->field != NULL;                           \
    } while (0)
    HERO_PA_FIND(simple_new, simple, "pa_simple_new");
    HERO_PA_FIND(simple_write, simple, "pa_simple_write");
    HERO_PA_FIND(simple_read, simple, "pa_simple_read");
    HERO_PA_FIND(simple_drain, simple, "pa_simple_drain");
    HERO_PA_FIND(simple_flush, simple, "pa_simple_flush");
    HERO_PA_FIND(simple_free, simple, "pa_simple_free");
    HERO_PA_FIND(strerror, pulse, "pa_strerror");
    HERO_PA_FIND(mainloop_new, pulse, "pa_mainloop_new");
    HERO_PA_FIND(mainloop_get_api, pulse, "pa_mainloop_get_api");
    HERO_PA_FIND(mainloop_iterate, pulse, "pa_mainloop_iterate");
    HERO_PA_FIND(mainloop_free, pulse, "pa_mainloop_free");
    HERO_PA_FIND(context_new, pulse, "pa_context_new");
    HERO_PA_FIND(context_connect, pulse, "pa_context_connect");
    HERO_PA_FIND(context_get_state, pulse, "pa_context_get_state");
    HERO_PA_FIND(context_disconnect, pulse, "pa_context_disconnect");
    HERO_PA_FIND(context_unref, pulse, "pa_context_unref");
    HERO_PA_FIND(get_sinks, pulse, "pa_context_get_sink_info_list");
    HERO_PA_FIND(get_sources, pulse, "pa_context_get_source_info_list");
    HERO_PA_FIND(get_server, pulse, "pa_context_get_server_info");
    HERO_PA_FIND(operation_get_state, pulse, "pa_operation_get_state");
    HERO_PA_FIND(operation_unref, pulse, "pa_operation_unref");
#undef HERO_PA_FIND
    hero_audio_pulse_found = all;
}

/* Whether the library is there, failing `UNSUPPORTED` where it is not. */
static bool hero_audio_pulse_ready(void) {
    (void)pthread_once(&hero_audio_pulse_once, hero_audio_pulse_find);
    if (!hero_audio_pulse_found) {
        (void)hero_audio_arm_fail(HERO_AUDIO_UNSUPPORTED, 0, "this machine has neither libpulse.so.0 nor libpulse-simple.so.0, which PulseAudio and PipeWire's pipewire-pulse install");
        return false;
    }
    return true;
}

static int64_t hero_audio_pulse_fail(int64_t kind, const char *asked, int error) {
    char words[256];

    (void)snprintf(words, sizeof words, "%s: %s", asked, hero_audio_pulse_fn.strerror(error));
    return hero_audio_arm_fail(kind, error, words);
}

static void hero_audio_pulse_nap(void) {
    struct timespec step = {0, 5000000};

    (void)nanosleep(&step, NULL);
}

/* A context connected to the server on a main loop of its own, or false where
 * none answered within `HERO_AUDIO_PULSE_ASK_NS`. */
typedef struct {
    void *loop;
    void *context;
} hero_audio_pulse_link;

static bool hero_audio_pulse_connect(hero_audio_pulse_link *link) {
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;

    link->loop = f->mainloop_new();
    link->context = link->loop == NULL ? NULL : f->context_new(f->mainloop_get_api(link->loop), "heroes");
    if (link->context == NULL || f->context_connect(link->context, NULL, HERO_AUDIO_PULSE_NOAUTOSPAWN, NULL) < 0) {
        (void)hero_audio_arm_fail(HERO_AUDIO_UNSUPPORTED, 0, "no PulseAudio or PipeWire server answers this user (its socket under XDG_RUNTIME_DIR, or PULSE_SERVER)");
        return false;
    }
    for (int64_t waited = 0; waited < HERO_AUDIO_PULSE_ASK_NS; waited += 5000000) {
        (void)f->mainloop_iterate(link->loop, 0, NULL);
        int state = f->context_get_state(link->context);

        if (state == HERO_AUDIO_PULSE_READY) {
            return true;
        }
        if (state == HERO_AUDIO_PULSE_FAILED || state == HERO_AUDIO_PULSE_TERMINATED) {
            break;
        }
        hero_audio_pulse_nap();
    }
    (void)hero_audio_arm_fail(HERO_AUDIO_UNSUPPORTED, 0, "no PulseAudio or PipeWire server answers this user (its socket under XDG_RUNTIME_DIR, or PULSE_SERVER)");
    return false;
}

static void hero_audio_pulse_unlink(hero_audio_pulse_link *link) {
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;

    if (link->context != NULL) {
        f->context_disconnect(link->context);
        f->context_unref(link->context);
    }
    if (link->loop != NULL) {
        f->mainloop_free(link->loop);
    }
}

/* Iterates `link`'s loop until `op` is done; false past the bound. */
static bool hero_audio_pulse_await(hero_audio_pulse_link *link, void *op) {
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;
    bool done = false;

    for (int64_t waited = 0; op != NULL && waited < HERO_AUDIO_PULSE_ASK_NS && !done; waited += 5000000) {
        (void)f->mainloop_iterate(link->loop, 0, NULL);
        done = f->operation_get_state(op) == HERO_AUDIO_PULSE_DONE;
        if (!done) {
            hero_audio_pulse_nap();
        }
    }
    if (op != NULL) {
        f->operation_unref(op);
    }
    if (!done) {
        (void)hero_audio_arm_fail(HERO_AUDIO_OTHER, 0, "the PulseAudio server did not answer within 3 seconds");
    }
    return done;
}

/* What the server answered of itself, copied out of the callback. */
typedef struct {
    char name[128];
    char sink[512];
    char source[512];
    bool answered;
} hero_audio_pulse_about;

static void hero_audio_pulse_copy(char *to, size_t room, const char *from) {
    size_t n = from == NULL ? 0 : strlen(from);

    if (n >= room) {
        n = 0;
    }
    if (n > 0) {
        memcpy(to, from, n);
    }
    to[n] = 0;
}

static void hero_audio_pulse_on_server(void *context, const hero_pa_server *server, void *given) {
    hero_audio_pulse_about *about = given;

    (void)context;
    if (server != NULL) {
        hero_audio_pulse_copy(about->name, sizeof about->name, server->server_name);
        hero_audio_pulse_copy(about->sink, sizeof about->sink, server->default_sink_name);
        hero_audio_pulse_copy(about->source, sizeof about->source, server->default_source_name);
        about->answered = true;
    }
}

static bool hero_audio_pulse_ask_server(hero_audio_pulse_link *link, hero_audio_pulse_about *about) {
    memset(about, 0, sizeof *about);
    return hero_audio_pulse_await(link, hero_audio_pulse_fn.get_server(link->context, hero_audio_pulse_on_server, about)) && about->answered;
}

int64_t hero_audio_arm_system(char *buffer, size_t count) {
    hero_audio_pulse_link link = {NULL, NULL};
    hero_audio_pulse_about about;

    if (!hero_audio_pulse_ready()) {
        return -1;
    }
    bool asked = hero_audio_pulse_connect(&link) && hero_audio_pulse_ask_server(&link, &about);

    hero_audio_pulse_unlink(&link);
    if (!asked) {
        return -1;
    }
    const char *name = strstr(about.name, "PipeWire") != NULL ? "PipeWire" : "PulseAudio";
    size_t n = strlen(name);

    if (n <= count) {
        memcpy(buffer, name, n);
    }
    return (int64_t)n;
}

/* The devices' callback: one sink or source added to the list. */
typedef struct {
    hero_audio_list *list;
    hero_audio_pulse_about *about;
    int64_t direction;
    int64_t answer;
} hero_audio_pulse_listing;

/* The encoding of a sample format of the library's (`sample.h`): the little
 * and big-endian forms alike, since the server converts; a format this layer
 * names none of is told as the floats it opens streams in. */
static const char *hero_audio_pulse_encoding(int format) {
    if (format == 0) {
        return "pcm8";
    }
    if (format == 3 || format == 4) {
        return "pcm16";
    }
    if (format == 7 || format == 8 || format == 11 || format == 12) {
        return "pcm32";
    }
    if (format == 9 || format == 10) {
        return "pcm24";
    }
    return "float32";
}

static void hero_audio_pulse_on_device(void *context, const hero_pa_device *device, int eol, void *given) {
    hero_audio_pulse_listing *listing = given;

    (void)context;
    if (eol != 0 || device == NULL || listing->answer < 0 || device->name == NULL) {
        return;
    }
    const char *chosen = listing->direction == HERO_AUDIO_OUTPUT ? listing->about->sink : listing->about->source;
    const char *encoding = hero_audio_pulse_encoding(device->spec.format);

    listing->answer = hero_audio_list_add(listing->list, listing->direction, strcmp(chosen, device->name) == 0, device->name,
                                          device->description == NULL ? device->name : device->description, (int64_t)device->spec.rate,
                                          (int64_t)device->spec.channels, encoding);
}

int64_t hero_audio_arm_devices(hero_audio_list *list) {
    hero_audio_pulse_link link = {NULL, NULL};
    hero_audio_pulse_about about;

    if (!hero_audio_pulse_ready()) {
        return -1;
    }
    if (!hero_audio_pulse_connect(&link) || !hero_audio_pulse_ask_server(&link, &about)) {
        hero_audio_pulse_unlink(&link);
        return -1;
    }
    hero_audio_pulse_listing sinks = {list, &about, HERO_AUDIO_OUTPUT, 0};
    hero_audio_pulse_listing sources = {list, &about, HERO_AUDIO_INPUT, 0};
    bool done = hero_audio_pulse_await(&link, hero_audio_pulse_fn.get_sinks(link.context, hero_audio_pulse_on_device, &sinks)) &&
                hero_audio_pulse_await(&link, hero_audio_pulse_fn.get_sources(link.context, hero_audio_pulse_on_device, &sources));

    hero_audio_pulse_unlink(&link);
    if (!done) {
        return -1;
    }
    return sinks.answer < 0 || sources.answer < 0 ? -1 : 0;
}

/* Whether `name` is a sink (an output) or a source (an input) of the server. */
typedef struct {
    const char *name;
    bool found;
} hero_audio_pulse_seek;

static void hero_audio_pulse_on_seek(void *context, const hero_pa_device *device, int eol, void *given) {
    hero_audio_pulse_seek *seek = given;

    (void)context;
    if (eol == 0 && device != NULL && device->name != NULL && strcmp(device->name, seek->name) == 0) {
        seek->found = true;
    }
}

/* The stream of this arm: its connection, its thread and what it is told. */
typedef struct {
    hero_audio_core *st;
    void *simple;
    pthread_t thread;
    int64_t direction;
    size_t frames;
    size_t frame_bytes;
    _Atomic bool closing;
    _Atomic bool stopping;
} hero_audio_pulse_stream;

static void *hero_audio_pulse_run(void *given) {
    hero_audio_pulse_stream *ps = given;
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;
    float *buffer = hero_layer_alloc(ps->frames * ps->frame_bytes);
    bool stopped = false;
    bool ended = false;

    while (buffer != NULL && !atomic_load(&ps->closing)) {
        int error = 0;

        if (atomic_load(&ps->stopping)) {
            if (!stopped && ps->direction == HERO_AUDIO_OUTPUT) {
                (void)f->simple_flush(ps->simple, &error);
            }
            stopped = true;
            hero_audio_pulse_nap();
            continue;
        }
        if (ended) {
            hero_audio_pulse_nap();
            continue;
        }
        if (ps->direction == HERO_AUDIO_OUTPUT) {
            (void)hero_audio_pull(ps->st, buffer, ps->frames);
            if (f->simple_write(ps->simple, buffer, ps->frames * ps->frame_bytes, &error) < 0) {
                hero_audio_lost(ps->st, error, f->strerror(error));
                ended = true;
            } else if (hero_audio_drained(ps->st)) {
                if (f->simple_drain(ps->simple, &error) < 0) {
                    hero_audio_lost(ps->st, error, f->strerror(error));
                } else {
                    hero_audio_played_out(ps->st);
                }
                ended = true;
            }
        } else if (f->simple_read(ps->simple, buffer, ps->frames * ps->frame_bytes, &error) < 0) {
            hero_audio_lost(ps->st, error, f->strerror(error));
            ended = true;
        } else {
            (void)hero_audio_push(ps->st, buffer, ps->frames);
        }
    }
    hero_layer_free(buffer);
    return NULL;
}

int64_t hero_audio_arm_open(hero_audio_core *st, const char *device, int64_t direction, int64_t rate, int64_t channels, void **arm) {
    if (!hero_audio_pulse_ready()) {
        return -1;
    }
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;

    if (device != NULL) {
        hero_audio_pulse_link link = {NULL, NULL};
        hero_audio_pulse_seek seek = {device, false};
        bool asked = hero_audio_pulse_connect(&link) &&
                     hero_audio_pulse_await(&link, direction == HERO_AUDIO_OUTPUT ? f->get_sinks(link.context, hero_audio_pulse_on_seek, &seek)
                                                                                 : f->get_sources(link.context, hero_audio_pulse_on_seek, &seek));

        hero_audio_pulse_unlink(&link);
        if (!asked) {
            return -1;
        }
        if (!seek.found) {
            return hero_audio_arm_fail(HERO_AUDIO_NOT_FOUND, 0, direction == HERO_AUDIO_OUTPUT ? "the server has no sink of that name" : "the server has no source of that name");
        }
    }
    hero_audio_pulse_stream *ps = hero_layer_alloc_zero(1, sizeof *ps);

    if (ps == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no memory for the stream");
    }
    ps->st = st;
    ps->direction = direction;
    ps->frame_bytes = (size_t)channels * sizeof(float);
    ps->frames = (size_t)((rate + 49) / 50);
    hero_pa_spec spec = {HERO_AUDIO_PULSE_FLOAT32LE, (uint32_t)rate, (uint8_t)channels};
    uint32_t chunk = (uint32_t)(ps->frames * ps->frame_bytes);
    hero_pa_attr attr = {UINT32_MAX, 3 * chunk, UINT32_MAX, UINT32_MAX, chunk};
    int error = 0;

    ps->simple = f->simple_new(NULL, "heroes", direction == HERO_AUDIO_OUTPUT ? HERO_AUDIO_PULSE_PLAYBACK : HERO_AUDIO_PULSE_RECORD, device,
                               direction == HERO_AUDIO_OUTPUT ? "playback" : "recording", &spec, NULL, &attr, &error);
    if (ps->simple == NULL) {
        hero_layer_free(ps);
        return hero_audio_pulse_fail(HERO_AUDIO_OTHER, "connecting a stream to the server", error);
    }
    if (pthread_create(&ps->thread, NULL, hero_audio_pulse_run, ps) != 0) {
        f->simple_free(ps->simple);
        hero_layer_free(ps);
        return hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no thread for the stream");
    }
    *arm = ps;
    return 0;
}

int64_t hero_audio_arm_pause(void *arm) {
    (void)arm;
    return 0;
}

int64_t hero_audio_arm_resume(void *arm) {
    (void)arm;
    return 0;
}

void hero_audio_arm_stop(void *arm) {
    hero_audio_pulse_stream *ps = arm;

    atomic_store(&ps->stopping, true);
}

void hero_audio_arm_close(void *arm) {
    hero_audio_pulse_stream *ps = arm;

    atomic_store(&ps->closing, true);
    (void)pthread_join(ps->thread, NULL);
    hero_audio_pulse_fn.simple_free(ps->simple);
    hero_layer_free(ps);
}

int64_t hero_audio_arm_permission(int64_t limit_ns) {
    (void)limit_ns;
    return 1;
}

/* libcanberra's context, made once and kept, since a sound it plays stops
 * with its context; and its function to play an event. */
static pthread_once_t hero_audio_pulse_bell_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t hero_audio_pulse_bell_lock = PTHREAD_MUTEX_INITIALIZER;
static void *hero_audio_pulse_bell_context;
static int (*hero_audio_pulse_bell_play)(void *context, uint32_t id, ...);

static void hero_audio_pulse_bell_find(void) {
    void *canberra = dlopen("libcanberra.so.0", RTLD_NOW | RTLD_LOCAL);
    int (*create)(void **context) = NULL;

    if (canberra == NULL) {
        return;
    }
    *(void **)(&create) = dlsym(canberra, "ca_context_create");
    *(void **)(&hero_audio_pulse_bell_play) = dlsym(canberra, "ca_context_play");
    if (create == NULL || hero_audio_pulse_bell_play == NULL || create(&hero_audio_pulse_bell_context) != 0) {
        hero_audio_pulse_bell_context = NULL;
    }
}

/* The tone the beep plays where libcanberra is not there, on a thread of its
 * own, which ends when the tone has played. */
static void *hero_audio_pulse_tone(void *given) {
    void *simple = given;
    hero_audio_pulse_functions *f = &hero_audio_pulse_fn;
    float tone[4410];
    int error = 0;

    for (size_t k = 0; k < 4410; k += 1) {
        /* 880 Hz at 44,100 frames a second: the turn of frame k is exact. */
        tone[k] = (float)(0.3 * hero_layer_sin_of_turn((double)((880 * k) % 44100) / 44100.0));
    }
    if (f->simple_write(simple, tone, sizeof tone, &error) == 0) {
        (void)f->simple_drain(simple, &error);
    }
    f->simple_free(simple);
    return NULL;
}

int64_t hero_audio_arm_beep(void) {
    (void)pthread_once(&hero_audio_pulse_bell_once, hero_audio_pulse_bell_find);
    if (hero_audio_pulse_bell_context != NULL) {
        (void)pthread_mutex_lock(&hero_audio_pulse_bell_lock);
        int played = hero_audio_pulse_bell_play(hero_audio_pulse_bell_context, 0, "event.id", "bell", NULL);

        (void)pthread_mutex_unlock(&hero_audio_pulse_bell_lock);
        if (played == 0) {
            return 0;
        }
    }
    if (!hero_audio_pulse_ready()) {
        return -1;
    }
    hero_pa_spec spec = {HERO_AUDIO_PULSE_FLOAT32LE, 44100, 1};
    int error = 0;
    void *simple = hero_audio_pulse_fn.simple_new(NULL, "heroes", HERO_AUDIO_PULSE_PLAYBACK, NULL, "beep", &spec, NULL, NULL, &error);

    if (simple == NULL) {
        return hero_audio_pulse_fail(HERO_AUDIO_UNSUPPORTED, "playing the beep on the default output", error);
    }
    pthread_t thread;
    pthread_attr_t detached;

    (void)pthread_attr_init(&detached);
    (void)pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED);
    int made = pthread_create(&thread, &detached, hero_audio_pulse_tone, simple);

    (void)pthread_attr_destroy(&detached);
    if (made != 0) {
        hero_audio_pulse_fn.simple_free(simple);
        return hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no thread for the beep");
    }
    return 0;
}

#endif
