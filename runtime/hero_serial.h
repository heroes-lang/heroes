/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_serial.h — the platform layer's serial ports: the ports listed, a port
 * opened as a stream, its settings set and read, its timeouts, its modem
 * lines, a break sent, its buffers emptied, and a pseudo-terminal that stands
 * for a port where no hardware is (M-rebel-library, 2026-11-18; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_serial.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_serial.h"`, with no `link` word and no flag.
 *
 * WHY C OF THE PROJECT'S OWN: a port's settings are `termios` on POSIX, a
 * structure whose flags and speeds differ between Darwin and Linux, and the
 * `DCB` and `COMMTIMEOUTS` on Windows, where a port is opened by
 * `CreateFileW` and read by overlapped calls. So no platform type reaches
 * this header, and no conditional but its guard.
 *
 * A PORT IS A STREAM, `hero_stream.h`'s one handle, made by `hero_stream_make`
 * over a block of the member's own: read and written by `hero_stream_read`
 * and `hero_stream_write`, given back by `hero_stream_close`, and every other
 * call below takes that stream. A stream this member did not make (a file, a
 * pipe, a socket) is refused `HERO_SERIAL_NOT_A_PORT`; one closed already, or
 * no stream at all, `HERO_SERIAL_NOT_OPEN`. A call holds the port's block
 * while it runs, so a close on another thread meanwhile releases it when the
 * call ends, never under it.
 *
 * A READ waits for one byte at most the port's read timeout, then answers
 * every byte the port holds, up to the count; with a gap set it goes on
 * taking bytes while each comes within the gap of the one before. A WRITE
 * waits at most the write timeout for the port to take a byte, then answers
 * how many it took. Past a timeout the stream's failure is
 * `HERO_STREAM_TIMED_OUT`; the program asking to stop (`hero_net_ask_stop`, or
 * Ctrl-C after `hero_net_catch_stop`) ends a waiting read or write within
 * about 50 ms with `HERO_STREAM_STOPPED`, which is why this member takes
 * `hero_net`'s into a program. A port whose device is gone (a USB adapter
 * pulled out) reads its end, 0, and refuses a write `HERO_STREAM_CLOSED_PEER`.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answering a stream answers
 * NULL, one answering a number -1, and `hero_serial_last_error()` then names
 * the kind; a read or a write of the stream names its failure as every
 * stream does, `hero_stream_last_error()`.
 */

#ifndef HERO_SERIAL_H
#define HERO_SERIAL_H

#include <stdint.h>

#include "hero_stream.h"

/* The kinds of failure, the same numbers on every platform and the stream's
 * numbers where `hero_stream.h` has the kind; the platform's own number for
 * the last failure is `hero_serial_last_os_error()`. */
#define HERO_SERIAL_OK 0
/* An argument the layer refuses: no path, a speed, a size of character, a
 * parity, a stop, a flow, a line, a timeout or a buffer outside what the call
 * takes, or a count with no bytes behind it. */
#define HERO_SERIAL_INVALID 1
/* The stream handed is none `hero_stream.h` holds open: closed already, NULL,
 * or a value it never issued. */
#define HERO_SERIAL_NOT_OPEN 2
/* The path names nothing. */
#define HERO_SERIAL_NOT_FOUND 3
/* The system refuses the port to this program: its permissions, or, on
 * Windows, a port another program holds, which Windows tells the same way. */
#define HERO_SERIAL_DENIED 5
/* The process or the machine has no descriptor, handle or memory left, or
 * the layer as many streams as it holds. */
#define HERO_SERIAL_LIMIT 8
/* The port's device is gone, a USB adapter pulled out. */
#define HERO_SERIAL_GONE 10
/* The platform or the port has no such thing: a speed, a parity, a stop or a
 * flow it does not take (a pseudo-terminal keeps 8 bits and no parity on
 * Linux), the modem lines of a pseudo-terminal, a pseudo-terminal on
 * Windows. */
#define HERO_SERIAL_UNSUPPORTED 11
#define HERO_SERIAL_OTHER 12
/* Another program has the port open exclusively (POSIX's `TIOCEXCL`, which
 * this member takes at every open). */
#define HERO_SERIAL_BUSY 13
/* What the path names, or the stream handed, is no port: a file, a folder,
 * the null device, a pipe, a socket, a stream another member made; or the
 * controlling side of a pseudo-terminal, handed where a port is asked. */
#define HERO_SERIAL_NOT_A_PORT 14
/* A drain or a break waited past its time. */
#define HERO_SERIAL_TIMED_OUT 16
/* The program asked to stop while the call waited. */
#define HERO_SERIAL_STOPPED 17

/* What a parity is, for `hero_serial_configure` and `hero_serial_setting`. */
#define HERO_SERIAL_PARITY_NONE 0
#define HERO_SERIAL_PARITY_ODD 1
#define HERO_SERIAL_PARITY_EVEN 2
/* The parity bit always 1, or always 0: Linux's `CMSPAR` and Windows'; Darwin
 * has neither, `HERO_SERIAL_UNSUPPORTED`. */
#define HERO_SERIAL_PARITY_MARK 3
#define HERO_SERIAL_PARITY_SPACE 4

/* How many stop bits end a character. One and a half is Windows' alone (POSIX
 * has no word for it), `HERO_SERIAL_UNSUPPORTED` elsewhere. */
#define HERO_SERIAL_STOP_ONE 1
#define HERO_SERIAL_STOP_TWO 2
#define HERO_SERIAL_STOP_ONE_AND_HALF 3

/* The flow control, bits that combine; 0 is none. RTS/CTS by the hardware's
 * lines, XON/XOFF by the bytes 17 and 19 in both directions, DSR/DTR by the
 * other pair of lines, Windows' alone (`HERO_SERIAL_UNSUPPORTED` on POSIX). */
#define HERO_SERIAL_FLOW_RTS_CTS 1
#define HERO_SERIAL_FLOW_XON_XOFF 2
#define HERO_SERIAL_FLOW_DSR_DTR 4

/* Which setting `hero_serial_setting` answers. */
#define HERO_SERIAL_SETTING_BAUD 1
#define HERO_SERIAL_SETTING_DATA_BITS 2
#define HERO_SERIAL_SETTING_PARITY 3
#define HERO_SERIAL_SETTING_STOP_BITS 4
#define HERO_SERIAL_SETTING_FLOW 5

/* The modem lines, bits that combine: DTR and RTS this side sets, CTS, DSR,
 * CD (the carrier) and RI (the ring) the other side does. */
#define HERO_SERIAL_LINE_DTR 1
#define HERO_SERIAL_LINE_RTS 2
#define HERO_SERIAL_LINE_CTS 4
#define HERO_SERIAL_LINE_DSR 8
#define HERO_SERIAL_LINE_CD 16
#define HERO_SERIAL_LINE_RI 32

/* A read or a write timeout that never ends a wait. */
#define HERO_SERIAL_FOREVER (-1)

/* Which buffer `hero_serial_discard` empties and `hero_serial_waiting` counts:
 * what was received and not read, what was written and not sent. */
#define HERO_SERIAL_INPUT 1
#define HERO_SERIAL_OUTPUT 2

/* The fastest speed this header takes, in bits a second; a port takes fewer. */
#define HERO_SERIAL_BAUD_MOST 2147483647

/* The ports this machine has, each name followed by a zero byte, written into
 * `buffer` as far as `count` reaches, and answers how many bytes the whole
 * list takes, so a caller whose buffer was short asks again with that many.
 * POSIX: the names under `/dev` a port's driver makes, `/dev/tty.*` and
 * `/dev/cu.*` on Darwin, `/dev/ttyS*`, `/dev/ttyUSB*` and `/dev/ttyACM*` on
 * Linux, a `ttyS` one only where `/sys/class/tty/<name>/device` stands (the
 * kernel makes 4 to 32 of them whatever the machine has). Windows: the values
 * of the registry's `HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM` key, as
 * `COM3`, which is no list where the machine has no port. In no order. -1:
 * `HERO_SERIAL_INVALID` for a count with no buffer, `HERO_SERIAL_OTHER` where
 * the system refused the folder or the key. */
int64_t hero_serial_ports(void *buffer, uint64_t count);

/* The port at `path` opened to read and to write, as a stream given back by
 * `hero_stream_close`, or NULL. `path` is UTF-8: `/dev/ttyUSB0`, or on
 * Windows `COM3` (which this member writes `\\.\COM3`, the form every port
 * number takes) or the whole `\\.\` form. The port is this process's alone
 * while it is open: Windows opens no port twice, and on POSIX the open takes
 * `TIOCEXCL`, which Linux keeps (a second open fails `HERO_SERIAL_BUSY`) and
 * Darwin's pseudo-terminals do not. Its bytes pass as they are: nothing is
 * echoed, translated or taken as a signal. Its speed, size of character,
 * parity, stops and flow stay what they were, so a caller sets them with
 * `hero_serial_configure`; DTR and RTS are raised, as every platform does at
 * an open. Its timeouts start as `HERO_SERIAL_FOREVER` and no gap. NULL:
 * `HERO_SERIAL_INVALID` for a NULL or empty path or one that is not UTF-8,
 * `HERO_SERIAL_NOT_FOUND`, `HERO_SERIAL_DENIED`, `HERO_SERIAL_BUSY`,
 * `HERO_SERIAL_NOT_A_PORT` where the path names no terminal device,
 * `HERO_SERIAL_LIMIT`. */
hero_stream *hero_serial_open(const char *path);

/* Sets the port's speed `baud` in bits a second, its size of character
 * `data_bits`, 5 to 8, its `parity` (`HERO_SERIAL_PARITY_*`), its `stop_bits`
 * (`HERO_SERIAL_STOP_*`) and its `flow` (`HERO_SERIAL_FLOW_*` bits, 0 for
 * none), all at once, and answers 0 once the port holds every one; or -1,
 * with the port as it was where the system refused them all at once.
 * `HERO_SERIAL_INVALID` for a value outside those, or a speed below 1 or past
 * `HERO_SERIAL_BAUD_MOST`; `HERO_SERIAL_UNSUPPORTED` for one the platform has
 * no word for, or one the port did not keep (read back after it is set: a
 * Linux pseudo-terminal keeps 8 bits and no parity whatever it is asked).
 * SPEEDS: Darwin takes any, through `IOSSIOSPEED` where `termios` refuses it;
 * Linux the speeds `termios` names, 50 to 4,000,000, others
 * `HERO_SERIAL_UNSUPPORTED`; Windows any its driver takes. */
int64_t hero_serial_configure(hero_stream *s, int64_t baud, int64_t data_bits, int64_t parity, int64_t stop_bits, int64_t flow);

/* One setting of the port as the system holds it now, `which` one of
 * `HERO_SERIAL_SETTING_*`, in `hero_serial_configure`'s numbers; or -1.
 * Answers what the platform says even for a value it set itself and this
 * header has no word for: a speed POSIX names by no number is
 * `HERO_SERIAL_UNSUPPORTED`. XON/XOFF is said where either direction has it. */
int64_t hero_serial_setting(hero_stream *s, int64_t which);

/* Sets the port's three timeouts in milliseconds and answers 0, or -1 with
 * `HERO_SERIAL_INVALID` for a value outside them: `read_ms` how long a read
 * waits for its first byte, `write_ms` how long a write waits for the port to
 * take one, each 0 or more or `HERO_SERIAL_FOREVER`, 0 not waiting at all;
 * `gap_ms`, 0 or more, how long a read that has bytes waits for the next one
 * before it answers, 0 for no wait. The controlling side of a
 * pseudo-terminal takes them too. A read or a write already waiting keeps
 * the timeouts it began with. */
int64_t hero_serial_set_timeouts(hero_stream *s, int64_t read_ms, int64_t write_ms, int64_t gap_ms);

/* The port's three timeouts, as `hero_serial_set_timeouts` took them, into
 * `*read_ms`, `*write_ms` and `*gap_ms`, and answers 0; or -1 with nothing
 * written. */
int64_t hero_serial_timeouts(hero_stream *s, int64_t *read_ms, int64_t *write_ms, int64_t *gap_ms);

/* The modem lines as bits, `HERO_SERIAL_LINE_*` for each line that is on; or
 * -1. DTR and RTS are what this side holds them at (on Windows what this
 * member set them to last, which no call reads back), the other four what
 * the other side holds. `HERO_SERIAL_UNSUPPORTED` for a port with no lines,
 * a pseudo-terminal among them. */
int64_t hero_serial_lines(hero_stream *s);

/* Raises (`on` 1) or drops (`on` 0) the line `line`, `HERO_SERIAL_LINE_DTR`
 * or `HERO_SERIAL_LINE_RTS`, and answers 0; or -1: `HERO_SERIAL_INVALID` for
 * another line or another `on`, `HERO_SERIAL_UNSUPPORTED` as for
 * `hero_serial_lines`. A line the flow control drives may be refused by the
 * system, or taken back by the flow at its next turn. */
int64_t hero_serial_set_line(hero_stream *s, int64_t line, int64_t on);

/* Holds the line in break, the transmit line at 0, for `ms` milliseconds, 0
 * or more, then lets it go, and answers 0; or -1: `HERO_SERIAL_INVALID` for a
 * negative time, `HERO_SERIAL_STOPPED` where the program asked to stop
 * meanwhile (the break is let go first). */
int64_t hero_serial_send_break(hero_stream *s, int64_t ms);

/* Holds the line in break (`on` 1) until it is let go (`on` 0), and answers
 * 0; or -1, `HERO_SERIAL_INVALID` for another `on`. */
int64_t hero_serial_set_break(hero_stream *s, int64_t on);

/* Throws away what the port holds of `which`, `HERO_SERIAL_INPUT`,
 * `HERO_SERIAL_OUTPUT` or both added, and answers 0; or -1,
 * `HERO_SERIAL_INVALID` for another `which`. */
int64_t hero_serial_discard(hero_stream *s, int64_t which);

/* Waits until every byte written to the port has been sent, at most the
 * port's write timeout, and answers 0; or -1: `HERO_SERIAL_TIMED_OUT` past
 * the timeout, `HERO_SERIAL_STOPPED` where the program asked to stop. A byte
 * a pseudo-terminal holds is sent once its other side reads it. */
int64_t hero_serial_drain(hero_stream *s);

/* How many bytes the port holds of `which`, `HERO_SERIAL_INPUT` (received,
 * not read) or `HERO_SERIAL_OUTPUT` (written, not sent); or -1,
 * `HERO_SERIAL_INVALID` for another `which`. */
int64_t hero_serial_waiting(hero_stream *s, int64_t which);

/* A pseudo-terminal, a pair of ends that stands for a port and a device on
 * it: answers its CONTROLLING side as a stream, given back by
 * `hero_stream_close`, whose port side `hero_serial_pseudo_path` names and
 * `hero_serial_open` opens. What one side writes the other reads, nothing
 * echoed or translated. The controlling side takes timeouts, discards,
 * drains and counts; settings, lines and breaks are refused it,
 * `HERO_SERIAL_NOT_A_PORT`. Where the port side is not open its reads answer
 * the end (Linux) or wait (Darwin). NULL: `HERO_SERIAL_UNSUPPORTED` on
 * Windows, `HERO_SERIAL_LIMIT` where the system has no pseudo-terminal
 * left. */
hero_stream *hero_serial_pseudo(void);

/* The path of the port side of the pseudo-terminal whose controlling side is
 * `s`, written into `buffer` as far as `count` reaches, no zero after it, and
 * answers how many bytes it takes; or -1, `HERO_SERIAL_INVALID` for a stream
 * this member made that is no controlling side, or a count with no buffer. */
int64_t hero_serial_pseudo_path(hero_stream *s, void *buffer, uint64_t count);

/* The kind of the last call's failure on this thread, `HERO_SERIAL_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_serial_last_error(void);
int64_t hero_serial_last_os_error(void);

#endif
