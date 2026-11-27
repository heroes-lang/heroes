/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_procs.h — the platform layer's list of the machine's processes
 * (M-rebel-library, gap G20; spec § 13): every process the system shows this
 * program, with its id, its parent's id, its start time, its owner, its name
 * and what it holds and has spent, so a monitor's table needs no `ps`.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_sys.h` is:
 * its function is defined in `platform/hero_procs.c`, a member of the archive
 * on every program's link line, which the linker takes only into a program
 * that calls it; a group binds it by naming this header,
 * `extern "hero_procs.h"`, with no `link` word and no flag. On Windows the
 * member, and only a program that takes it, links `advapi32` (a process's
 * token and the account that owns it) and `kernel32` (the snapshot).
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: the function answers -1 and
 * `hero_procs_last_error()` then names the kind.
 *
 * NOTHING IS KEPT BETWEEN CALLS but the last error, which is per thread: the
 * member holds no table and no lock, so the function may be called from any
 * thread at once.
 */

#ifndef HERO_PROCS_H
#define HERO_PROCS_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform, and the numbers
 * `hero_sys.h` and `hero_stream.h` give the kinds of the same names. */
#define HERO_PROCS_OK 0
/* An argument the layer refuses: a count with no buffer behind it. */
#define HERO_PROCS_INVALID 1
/* The platform keeps no list the caller may read. */
#define HERO_PROCS_DENIED 5
/* The process or the machine has no memory left, or the answer is larger
 * than 64 bits can count. */
#define HERO_PROCS_LIMIT 8
/* The platform has no such list: a POSIX system that is neither Darwin nor
 * Linux. */
#define HERO_PROCS_UNSUPPORTED 11
#define HERO_PROCS_OTHER 12

/* The fields of one entry of `hero_procs_list`'s answer, each written and
 * followed by one zero byte, in this order. A field the platform did not
 * tell this caller is written empty, its zero byte alone:
 *
 *   0 the process's id, in decimal digits;
 *   1 its parent's id, in decimal digits: on POSIX the process that has the
 *     process now (1, or a reaper, where its parent ended), on Windows the id
 *     recorded when the process was made, which may by now name another
 *     process or none;
 *   2 its start time, in decimal digits: milliseconds since 1970-01-01
 *     00:00:00 UTC, which does not change while the process lives, so an id
 *     and a start time together name one process for as long as the system
 *     lives (on Linux the boot's time is known in seconds, so the figure is
 *     within a second of the true one and equal in every reading while the
 *     clock is not set);
 *   3 the id of its owner, as the system writes it: on POSIX the effective
 *     user's number in decimal digits, on Windows the SID of the token's user
 *     as `S-1-5-...`; empty where the system does not let this caller read
 *     it (a Windows process of another user);
 *   4 the owner's name, without a domain, UTF-8; empty where the system
 *     names none for that id, or field 3 is empty;
 *   5 its resident memory in bytes, in decimal digits;
 *   6 its virtual memory in bytes, in decimal digits;
 *   7 the processor time it has spent in its own code, in nanoseconds;
 *   8 the processor time the system has spent on its behalf, in nanoseconds:
 *     fields 5 to 8 are empty together, where the system does not let this
 *     caller read that process's usage (on Darwin a process of another user,
 *     or one that has ended and is not yet waited for), and mean what
 *     `hero_sys_process_usage` answers for the same process;
 *   9 the program's name: the name of its executable file, UTF-8 on Windows,
 *     the bytes the system holds on POSIX, which need not be valid UTF-8, and
 *     never empty text but where a process set its own name to nothing. */
#define HERO_PROCS_FIELDS 10

/* Every process of the machine this program may read, by the fill form, in
 * ascending order of id, `HERO_PROCS_FIELDS` zero-terminated fields each (the
 * list above). THE FILL FORM, as `hero_sys.h` says it: up to `count` bytes
 * are written into `buffer`, and the answer is how many bytes the WHOLE
 * answer has; where that is above `count` the buffer was too short, what it
 * holds is not the answer, and the caller asks again with at least that many
 * (a count of 0 only measures, and `buffer` may then be NULL). One call is
 * one reading, and two calls are two: processes begin and end between them.
 *
 * A process the system hides from this caller is not in the answer, and so is
 * none whose start time cannot be read, the start time being what tells one
 * process from the next that is given its id: a process that ended while the
 * list was read, a Linux process of another user where `/proc` is mounted
 * with `hidepid`, and on Windows the idle process (id 0) and a process the
 * caller may not open. Never an entry is made up for one. On Darwin the list
 * is `proc_listallpids` and each process is read with `proc_pidinfo`; on
 * Linux it is `/proc`, read as `ps` reads it but for the resident memory,
 * which is read as `hero_sys_process_usage` reads it, a name cut at fifteen bytes
 * given back whole from the executable's own name where that begins with it;
 * on Windows `CreateToolhelp32Snapshot`, a process opened for
 * `PROCESS_QUERY_LIMITED_INFORMATION`, its times and memory counted as
 * `hero_sys_process_usage` counts them. On a POSIX system neither Darwin nor
 * Linux, -1 with `HERO_PROCS_UNSUPPORTED`. */
int64_t hero_procs_list(void *buffer, uint64_t count);

/* The kind of the last call's failure on this thread, `HERO_PROCS_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused): `errno` on POSIX, `GetLastError` on Windows. */
int64_t hero_procs_last_error(void);
int64_t hero_procs_last_os_error(void);
#endif
