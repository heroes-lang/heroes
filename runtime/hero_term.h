/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_term.h — the platform layer's terminal: its keys read one at a time
 * with nothing echoed, written to directly, and whether a stream takes the
 * terminal's escape sequences (M-rebel-library, 2026-11-16; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_term.c`, a member of the archive
 * on every program's link line, which the linker takes only into a program
 * that calls one; a group binds them by naming this header,
 * `extern "hero_term.h"`, with no `link` word and no flag.
 *
 * WHY C OF THE PROJECT'S OWN: what a terminal's modes are is `termios`, a
 * structure whose fields and flags differ between Darwin and Linux, on POSIX,
 * and two console handles with their own mode bits on Windows; and putting a
 * terminal back as it was when the program ends, by a signal or by `exit`, is
 * C that runs where no Heroes function may. So no platform type reaches this
 * header, and no conditional but its guard.
 *
 * THE TERMINAL IS THE PROCESS'S CONTROLLING ONE, opened by this member at its
 * first use and kept for the process: `/dev/tty` on POSIX, so keys are read
 * from the person at the keyboard even where the program's input is a pipe,
 * as a reader of passwords does; where it cannot be opened (no controlling
 * terminal), the process's input and its error output when they are
 * terminals; and on Windows the console's own `CONIN$` and `CONOUT$`. With
 * none of these, every call below but `hero_term_sequences` answers -1 with
 * `HERO_TERM_NOT_A_TERMINAL`.
 *
 * THE KEYS MODE, AND HOW IT IS UNDONE. `hero_term_enter_keys` makes the
 * terminal hand each key as it is pressed, nothing echoed, no key ending the
 * program (Ctrl-C is the byte 3, Ctrl-D the byte 4, Enter the byte 13), and on
 * Windows a key with no character (an arrow, Home, a function key) as the same
 * escape sequence a POSIX terminal sends. `hero_term_leave` puts the terminal
 * back as it was before the first enter. And so does the end of the program,
 * WHEREVER THIS MEMBER CAN RUN: when `main` returns or `exit` is called (an
 * `atexit` function), and on POSIX at SIGINT, SIGTERM, SIGHUP and SIGQUIT,
 * each of which, where the program left it to its default end, is taken
 * while the mode stands, puts the terminal back, and then ends the program as
 * the signal would have. WHAT NO MEMBER CAN UNDO: a program the runtime ends
 * at a panic (`abort`, its handler set back to the default first), a memory
 * fault, and SIGKILL end the process with the mode standing; `stty sane` puts
 * a POSIX terminal back by hand. A signal the program or `hero_net.h`'s stop
 * took is left to them: the stop lets the program go on, to `hero_term_leave`.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answering a count -1, and
 * `hero_term_last_error()` then names the kind. The state is the process's,
 * one terminal, under a lock on both platforms.
 */

#ifndef HERO_TERM_H
#define HERO_TERM_H

#include <stdint.h>

#include "hero_stream.h"

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_term_last_os_error()`. */
#define HERO_TERM_OK 0
/* An argument the layer refuses: a count below what the call needs, a time
 * below -1. */
#define HERO_TERM_INVALID 1
/* The process has no terminal (THE TERMINAL, above). */
#define HERO_TERM_NOT_A_TERMINAL 2
/* `hero_term_read_key`'s time passed with no key. */
#define HERO_TERM_TIMED_OUT 3
#define HERO_TERM_OTHER 4
/* The stream handed is none `hero_stream.h` holds open: closed already, NULL,
 * or a value it never issued (a handle of another table, whose tag is not
 * the streams'), as that header words it. */
#define HERO_TERM_NOT_OPEN 5

/* The terminal put in the keys mode (THE KEYS MODE, above). Answers 0 where
 * this call put it there, and 1 where the mode stood already and nothing
 * changed, so a caller that reads one key leaves only a mode it entered; or
 * -1: `HERO_TERM_NOT_A_TERMINAL`, `HERO_TERM_OTHER` with the system's number
 * where it refused the mode. */
int64_t hero_term_enter_keys(void);

/* The terminal put back as it was before the first enter, and the signals this
 * member took given back to their default. Answers 0, also where no mode
 * stands; or -1 with `HERO_TERM_OTHER` where the system refused. */
int64_t hero_term_leave(void);

/* The most bytes one key takes: a UTF-8 character is 4, an escape sequence
 * this member hands whole at most this many. */
#define HERO_TERM_KEY_MOST 32

/* Reads ONE KEY into `buffer` and answers how many bytes it takes, waiting at
 * most `ms` milliseconds for it (0 asks without waiting, -1 waits with no
 * limit): one UTF-8 character, a control byte (Enter 13, Tab 9, Backspace 127
 * or 8, Ctrl-C 3), or one escape sequence whole (`ESC [ ... final`, `ESC O
 * x`), Escape alone where nothing follows it within about 50 ms. Bytes typed
 * ahead are kept for the next call, never lost; bytes that are no UTF-8 are
 * handed one key at a time as they came. 0 where the terminal is gone (hung
 * up, or its input ended). `count` is `HERO_TERM_KEY_MOST` or more, else
 * `HERO_TERM_INVALID`. -1: `HERO_TERM_TIMED_OUT`, `HERO_TERM_INVALID` for a
 * time below -1, `HERO_TERM_NOT_A_TERMINAL`, `HERO_TERM_OTHER`. */
int64_t hero_term_read_key(void *buffer, uint64_t count, int64_t ms);

/* Writes the `count` bytes of `data`, UTF-8, to the terminal, every one, and
 * answers `count`: a prompt the person at the keyboard sees even where the
 * program's outputs are files. -1: `HERO_TERM_NOT_A_TERMINAL`,
 * `HERO_TERM_INVALID` for UTF-8 Windows cannot convert, `HERO_TERM_OTHER`. */
int64_t hero_term_write(const void *data, uint64_t count);

/* Whether the stream `s` is a terminal that takes escape sequences (colours,
 * the cursor, clearing): 1, or 0 for a stream that is no terminal, a file or a
 * pipe or a socket, or a Windows console too old to take them. On Windows the
 * console's own processing of sequences is turned on where it is off, and put
 * back as it was when the program ends, as the keys mode is. -1 with
 * `HERO_TERM_NOT_OPEN` where `s` is no open stream. */
int64_t hero_term_sequences(hero_stream *s);

/* The kind of the last call's failure on this thread, `HERO_TERM_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_term_last_error(void);
int64_t hero_term_last_os_error(void);

#endif
