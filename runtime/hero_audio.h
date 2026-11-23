/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_audio.h — the platform layer's sound: the output and input devices
 * listed with their formats, frames played on one and recorded from one as
 * streams of 32-bit floats, a stream paused, resumed, stopped, its volume set
 * and its end told, the microphone's permission asked, and the system's beep
 * (M-rebel-library, 2026-11-23; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_watch.h`
 * is: its functions are defined in `platform/hero_audio.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_audio.h"`, with no `link` word and no flag. That member calls
 * each system's ARM (`platform/hero_audio_arm.h`): CoreAudio's AudioQueue on
 * macOS (`platform/hero_audio_coreaudio.c`), WASAPI on Windows
 * (`platform/hero_audio_wasapi.c`), and PulseAudio's library elsewhere
 * (`platform/hero_audio_pulse.c`), opened at the first call as `hero_keyring.c`
 * opens libsecret, which PipeWire's own server answers as PulseAudio's does;
 * where neither the library nor a server answers, every call that reaches a
 * system device answers -1 with `HERO_AUDIO_UNSUPPORTED`, and
 * `hero_audio_last_message` says which. It calls `hero_stream.h` (the bell
 * on Darwin and Linux) and `hero_net.h` (the stop, and the bell on Windows).
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: a group sees `int64_t`,
 * `uint64_t`, `double`, `void *`, `const char *`, the stream below, and the two
 * handles of the headers this one includes for their names.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a stream
 * answers NULL, one answering a count -1, and `hero_audio_last_error()` then
 * names the kind. A stream is a handle the layer issues and checks, never an
 * address it reads through, so one closed already, NULL, or one this layer
 * never issued is `HERO_AUDIO_NOT_OPEN`.
 *
 * WHAT CROSSES: frames of 32-bit IEEE floats, interleaved, little-endian as
 * every system this layer runs on keeps them, full scale at -1 and 1; a count
 * of samples, which is a whole number of frames. A stream is opened at the
 * rate and the channels the program gives; the system converts where its
 * device runs at others (AudioQueue, PulseAudio and WASAPI's
 * `AUTOCONVERTPCM` all do), and `audio` gives a device's own so that none is
 * needed.
 *
 * THE NULL DEVICE, `HERO_AUDIO_NULL`, on every system and with no hardware:
 * an output that plays nothing at the stream's rate, so its frames are taken
 * as a device would take them, and an input that records, in every channel,
 * frame `k` as `0.5 * sin(2 * pi * 440 * k / rate)`, computed in `double` and
 * rounded to `float`; a thread of the layer's own keeps its time, 10 ms a
 * step. Tests play and record through it the same on every system.
 *
 * WHEN THINGS HAPPEN. A stream starts at once. An output plays the frames
 * written in order and silence where none waits; `hero_audio_finish` says no
 * more will come, and its end is told once the system has played the last of
 * them. An input keeps what it records until it is read, and the frames that
 * arrive while its room is full are dropped and counted. A wait is bounded by
 * a limit in nanoseconds, 0 asking without waiting, and answers stopped once
 * the program's stop is asked (`hero_net.h`). The system's own thread or one
 * of the layer's moves the frames; the calls are safe from any thread.
 */

#ifndef HERO_AUDIO_H
#define HERO_AUDIO_H

#include <stdint.h>

#include "hero_net.h"
#include "hero_stream.h"

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_audio_last_os_error()`. */
#define HERO_AUDIO_OK 0
/* An argument the layer refuses: a direction, rate, channel count, room or
 * volume out of its range, a count of samples that is no whole number of
 * frames or has no bytes behind it, a write to an input or a read from an
 * output. */
#define HERO_AUDIO_INVALID 1
/* The stream is closed, NULL, or none this layer issued. */
#define HERO_AUDIO_NOT_OPEN 2
/* No device of that identifier is there, or no default of that direction. */
#define HERO_AUDIO_NOT_FOUND 3
/* The system refuses this program the device: the person refused the
 * microphone, or has not answered yet, or a policy forbids it. */
#define HERO_AUDIO_DENIED 4
/* The system or the process has no memory, thread or descriptor left. */
#define HERO_AUDIO_LIMIT 5
/* This machine has no sound system this layer reaches (above). */
#define HERO_AUDIO_UNSUPPORTED 6
/* The limit passed before what was waited for came. */
#define HERO_AUDIO_TIMEOUT 7
/* The program's stop was asked while the call waited. */
#define HERO_AUDIO_STOPPED 8
/* The stream has ended: finished and played out, or stopped. */
#define HERO_AUDIO_ENDED 9
/* The device went away while the stream ran: unplugged, or taken by another
 * program. */
#define HERO_AUDIO_LOST 10
/* The system refused for a reason none of the above names; the message
 * carries its words. */
#define HERO_AUDIO_OTHER 11

/* The two directions of a device and of a stream. */
#define HERO_AUDIO_OUTPUT 1
#define HERO_AUDIO_INPUT 2

/* The states of a stream, `hero_audio_state`'s answers. */
#define HERO_AUDIO_RUNNING 1
#define HERO_AUDIO_PAUSED 2
#define HERO_AUDIO_DONE 3
#define HERO_AUDIO_GONE 4

/* The identifier of the null device (above), in either direction. */
#define HERO_AUDIO_NULL "heroes:null"

/* The most frames a second, and the most channels, a stream is opened at. */
#define HERO_AUDIO_RATE_MOST 768000
#define HERO_AUDIO_CHANNELS_MOST 64
/* The most frames a stream's room holds, and the most samples (frames times
 * channels): a 2 GiB ring of floats at most. */
#define HERO_AUDIO_ROOM_MOST 268435456
#define HERO_AUDIO_SAMPLES_MOST 536870912

typedef struct hero_audio_stream hero_audio_stream;

/* The name of the sound system this machine's devices are reached through,
 * written into `buffer` as UTF-8 with no zero byte, and answers its length:
 * "CoreAudio", "WASAPI", "PulseAudio" or "PipeWire" (the server PulseAudio's
 * library reaches, as it names itself). Where the name is longer than
 * `count`, nothing is written and the length is answered all the same. -1:
 * `UNSUPPORTED`, `OTHER`. */
int64_t hero_audio_system(void *buffer, uint64_t count);

/* Writes the devices into `buffer`, each as seven fields, each field's UTF-8
 * followed by one zero byte: its direction, `output` or `input`; `default`
 * where it is that direction's default, else nothing; its identifier, which
 * `hero_audio_open` takes; its name for a person; the frames a second, the
 * channels and the encoding it runs at (`float32`, `pcm16`, `pcm24`, `pcm32`,
 * `float64`, `pcm8`) as the system mixes for it, the numbers in decimal.
 * Answers how many bytes that takes, 0 where there is no device; where it
 * takes more than `count`, nothing is written and the bytes are answered all
 * the same. A device whose name or identifier is no UTF-8 or holds a zero
 * byte is left out. The null device is not listed. -1: `UNSUPPORTED`,
 * `DENIED`, `LIMIT`, `OTHER`. */
int64_t hero_audio_devices(void *buffer, uint64_t count);

/* Opens a stream on `device` (an identifier `hero_audio_devices` wrote, NULL
 * or "" for the default of `direction`, or `HERO_AUDIO_NULL`) in `direction`,
 * at `rate` frames a second and `channels` channels, with room for `room`
 * frames between the program and the system, and starts it. NULL: `INVALID`,
 * `NOT_FOUND`, `DENIED` (an input whose permission the person refused or has
 * not given: `hero_audio_permission` asks it), `LIMIT`, `UNSUPPORTED`,
 * `OTHER`. */
hero_audio_stream *hero_audio_open(const char *device, int64_t direction, int64_t rate, int64_t channels, int64_t room);

/* Queues the `count` samples of `samples`, whole frames, on the output `s`,
 * waiting up to `limit_ns` for room as the device plays, and answers how many
 * frames it queued: all of them, or fewer where the limit passed or the stop
 * was asked with some queued, 0 where none fitted. -1: `NOT_OPEN`, `INVALID`,
 * `ENDED` (finished or stopped), `LOST`, `STOPPED` (asked before any was
 * queued). */
int64_t hero_audio_write(hero_audio_stream *s, const float *samples, uint64_t count, int64_t limit_ns);

/* Moves the frames the input `s` has recorded into `buffer`, at most `count`
 * samples' worth of whole frames, waiting up to `limit_ns` for the first, and
 * answers how many frames it moved, 0 where the limit passed with none. -1:
 * `NOT_OPEN`, `INVALID`, `ENDED` (stopped, every frame read), `LOST` (every
 * frame read), `STOPPED`. */
int64_t hero_audio_read(hero_audio_stream *s, float *buffer, uint64_t count, int64_t limit_ns);

/* Says that no more frames will be written to the output `s`: its end is
 * told once the system has played the last of them. Answers 0. -1:
 * `NOT_OPEN`, `INVALID` (an input). */
int64_t hero_audio_finish(hero_audio_stream *s);

/* Waits up to `limit_ns` for `s` to end and answers 0 once it has: an output
 * finished and played out, or either stopped. -1: `NOT_OPEN`, `TIMEOUT`,
 * `LOST`, `STOPPED`. */
int64_t hero_audio_wait(hero_audio_stream *s, int64_t limit_ns);

/* Pauses `s`, which keeps its frames, and resumes it; each answers 0, also
 * where it already was so. -1: `NOT_OPEN`, `ENDED`, `LOST`, `OTHER`. */
int64_t hero_audio_pause(hero_audio_stream *s);
int64_t hero_audio_resume(hero_audio_stream *s);

/* Stops `s` at once: an output's queued frames are dropped and an input
 * records no more, its frames still read; it has then ended. Answers 0, also
 * where it had. -1: `NOT_OPEN`. */
int64_t hero_audio_stop(hero_audio_stream *s);

/* Sets the gain every sample of `s` is multiplied by on its way, 0 to 1, 1
 * being as written or recorded. Answers 0. -1: `NOT_OPEN`, `INVALID`. */
int64_t hero_audio_set_volume(hero_audio_stream *s, double volume);

/* The state of `s`, one of the four above. -1: `NOT_OPEN`. */
int64_t hero_audio_state(hero_audio_stream *s);

/* The frames the system has taken from the output `s`, or given to the input
 * `s`, since it opened, silence it played for want of frames not counted.
 * -1: `NOT_OPEN`. */
int64_t hero_audio_position(hero_audio_stream *s);

/* The frames waiting in `s`: queued and not yet taken by the system for an
 * output, recorded and not yet read for an input. -1: `NOT_OPEN`. */
int64_t hero_audio_waiting(hero_audio_stream *s);

/* The frames the input `s` dropped while its room was full, and the frames of
 * silence the output `s` played while running with none queued and not
 * finished. -1: `NOT_OPEN`. */
int64_t hero_audio_dropped(hero_audio_stream *s);

/* THE BELL, for `hero_poll.h`: a stream that is ready to read while the output
 * `s` has room for a frame or has ended, or the input `s` holds a frame or
 * has ended, and not otherwise. BORROWED: the stream's own, the same at every
 * call and closed by `hero_audio_close`; the program never reads, writes or
 * closes it. Darwin and Linux only, a pipe: NULL with `HERO_AUDIO_UNSUPPORTED`
 * on Windows, whose readiness waits on sockets alone, `NOT_OPEN`, or `LIMIT`
 * where no pipe can be made. */
hero_stream *hero_audio_bell_stream(hero_audio_stream *s);

/* The bell as a datagram socket of this layer bound to the loopback, which a
 * datagram reaches while the stream is ready as above: Windows only, NULL
 * with `HERO_AUDIO_UNSUPPORTED` elsewhere. BORROWED as that one is. A
 * datagram a stranger sends makes it ready while the stream is not. */
hero_net_socket *hero_audio_bell_socket(hero_audio_stream *s);

/* Closes `s` and gives its handle back: the system's stream ended, the
 * layer's thread joined, the room and the bell given back. An output's
 * queued frames are dropped. Answers 0. -1: `NOT_OPEN`. */
int64_t hero_audio_close(hero_audio_stream *s);

/* Whether this program may record from a microphone: 1 where it may. Where
 * the person has not been asked yet (macOS), asks them, the system showing
 * its own dialog, and waits up to `limit_ns` for the answer; 0 asks nothing.
 * -1: `DENIED` (refused, forbidden by a policy, or not answered: the message
 * says which), `TIMEOUT` (asked and not answered within the limit),
 * `STOPPED`, `OTHER`. Windows and Linux ask nothing here: their refusal comes
 * from `hero_audio_open`. */
int64_t hero_audio_permission(int64_t limit_ns);

/* Sounds the system's alert, as the person set it, and answers 0 without
 * waiting for it to end. -1: `UNSUPPORTED`, `OTHER`. */
int64_t hero_audio_beep(void);

/* The kind of the last call's failure on this thread, `HERO_AUDIO_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_audio_last_error(void);
int64_t hero_audio_last_os_error(void);

/* The words of the last call's failure on this thread, the system's where it
 * gave some, written into `buffer` as UTF-8 with no zero byte, cut where a
 * character ends to fit `count`; answers the bytes written, 0 after a call
 * that did not fail. -1 with `INVALID` where `buffer` is NULL or `count` is
 * 0. */
int64_t hero_audio_last_message(void *buffer, uint64_t count);

#endif
