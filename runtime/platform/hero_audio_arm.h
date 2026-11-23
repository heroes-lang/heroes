/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_audio_arm.h — what `hero_audio.c` and each system's arm of
 * sound say to each other (M-rebel-library, 2026-11-23): THE ARM, the
 * functions every system's member defines (`hero_audio_coreaudio.c` on
 * Apple's systems, `hero_audio_wasapi.c` on Windows, `hero_audio_pulse.c`
 * elsewhere) and `hero_audio.c` alone calls; and THE CORE, the functions
 * `hero_audio.c` defines and an arm calls from the system's thread to move
 * frames and tell what happened. Exactly one arm is compiled with content on
 * a system, the others being empty there, so each name below has one
 * definition in every link.
 *
 * A HEADER OF THE MEMBERS ALONE: no Heroes group names it, and no header a
 * group names includes it. NO CONDITIONAL BUT THE GUARD.
 *
 * THE CONTRACT. An arm never holds the core's lock and never takes it; every
 * core function below takes it for itself, briefly, and may be called from
 * any thread, the system's own included. An arm's stream is the `void *` its
 * open hands back, which the core gives to every later call and to nothing
 * else. After `hero_audio_arm_close` returns, the arm calls no core function
 * for that stream again: the system's thread is joined or its callbacks are
 * done.
 */

#ifndef HERO_AUDIO_ARM_H
#define HERO_AUDIO_ARM_H

#include <stddef.h>
#include <stdint.h>

#include "hero_audio.h"

/* The core's state of one stream, which an arm hands back to the core
 * functions and never reads. */
typedef struct hero_audio_core hero_audio_core;

/* The list `hero_audio_devices` writes, which an arm fills by
 * `hero_audio_list_add`. */
typedef struct hero_audio_list hero_audio_list;

/* THE ARM. Each answers 0 or a count, or -1 after `hero_audio_arm_fail`. */

/* The system's name into `buffer` (at most `count` bytes, no zero byte);
 * answers its length, written or not, as `hero_audio_system` does. */
int64_t hero_audio_arm_system(char *buffer, size_t count);

/* Adds every device of the system to `list`. */
int64_t hero_audio_arm_devices(hero_audio_list *list);

/* Opens the system's stream on `device` (NULL for the default of
 * `direction`), at `rate` and `channels`, and starts it; answers 0 and its
 * stream in `*arm`. From then on the system's thread pulls frames from
 * `st` (an output) or pushes them into it (an input). */
int64_t hero_audio_arm_open(hero_audio_core *st, const char *device, int64_t direction, int64_t rate, int64_t channels, void **arm);

/* Pauses and resumes the system's stream; stops it at once, dropping what
 * the system holds. */
int64_t hero_audio_arm_pause(void *arm);
int64_t hero_audio_arm_resume(void *arm);
void hero_audio_arm_stop(void *arm);

/* Ends the system's stream and gives it back; see THE CONTRACT. */
void hero_audio_arm_close(void *arm);

/* `hero_audio_permission`'s answer for this system. */
int64_t hero_audio_arm_permission(int64_t limit_ns);

/* `hero_audio_beep`'s for this system. */
int64_t hero_audio_arm_beep(void);

/* THE CORE. */

/* Sets the calling thread's failure, `kind` one of `HERO_AUDIO_*`, the
 * system's number and its words (NULL for none), and answers -1. */
int64_t hero_audio_arm_fail(int64_t kind, int64_t os_error, const char *words);

/* Adds one device to `list`; a device whose `id` or `name` is no UTF-8 is
 * left out. `encoding` is one of the names `hero_audio_devices` lists.
 * Answers 0, or -1 with `LIMIT`. */
int64_t hero_audio_list_add(hero_audio_list *list, int64_t direction, int is_default, const char *id, const char *name, int64_t rate, int64_t channels, const char *encoding);

/* For an output: writes `frames` frames into `out`, those the program queued
 * first, each times the stream's volume, and silence after them, and answers
 * how many were the program's; none while the stream is paused or has ended.
 * Silence played while running with none queued and not finished is
 * counted. */
size_t hero_audio_pull(hero_audio_core *st, float *out, size_t frames);

/* Whether the output was finished and every frame queued has been pulled:
 * the arm then lets the system play out what it holds and calls
 * `hero_audio_played_out`. */
int hero_audio_drained(hero_audio_core *st);

/* The system has played the last frame of a finished output: it has ended. */
void hero_audio_played_out(hero_audio_core *st);

/* For an input: keeps the `frames` frames of `in`, each times the stream's
 * volume, while there is room, dropping and counting the rest; nothing while
 * the stream is paused or has ended. Answers how many it kept. */
size_t hero_audio_push(hero_audio_core *st, const float *in, size_t frames);

/* The device went away: the stream has ended, told `HERO_AUDIO_LOST` with the
 * system's number and words. */
void hero_audio_lost(hero_audio_core *st, int64_t os_error, const char *words);

#endif
