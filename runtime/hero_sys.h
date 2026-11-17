/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_sys.h — the platform layer's questions about the machine and about the
 * process itself (M-rebel-library; spec § 13): the host's name, who runs the
 * program, how large a page is and how many processors it may use, where the
 * user's folders are, what the environment holds, random bytes, how large
 * the terminal is, which system and processor it runs on, the system's
 * version, the user's name, the processor time spent and the load.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_sys.c`, a member of the archive
 * on every program's link line, which the linker takes only into a program
 * that calls one; a group binds them by naming this header,
 * `extern "hero_sys.h"`, with no `link` word and no flag. On Windows the
 * member, and only a program that takes it, links `bcrypt`, `shell32`,
 * `ole32`, `uuid` and `advapi32`.
 *
 * WHAT IS NOT HERE IS IN `hero_os.h` ALREADY: the process's own id
 * (`hero_os_pid`), the path of its executable and the reading of one
 * variable of the environment.
 *
 * WHY C OF THE PROJECT'S OWN, AND NOT A BINDING OF THE PLATFORM'S: a user id
 * is a `uid_t` on POSIX and nothing on Windows, the environment is a global
 * of the C library on Linux, a function's answer on Darwin and a block of
 * UTF-16 on Windows, and each platform keeps a program's folders by a
 * convention of its own. So no platform type reaches this header: a group
 * sees `int64_t`, `uint64_t` and `void *`.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function answers -1 and
 * `hero_sys_last_error()` then names the kind. Where a platform has no such
 * thing, a user id on Windows, the answer is -1 with `HERO_SYS_UNSUPPORTED`,
 * never an invented number.
 *
 * THE FILL FORM. A function that answers bytes takes the caller's `buffer`
 * and its `count` of bytes, writes nothing past them, and answers how many
 * bytes the WHOLE answer has. Where that is above `count` the buffer was too
 * short: what it holds is not the answer, and the caller asks again with at
 * least that many. A `count` of 0 only measures, and `buffer` may then be
 * NULL. No zero byte ends the answer and none is counted. Text is UTF-8: on
 * POSIX the bytes the system holds, as they are, which need not be valid
 * UTF-8; on Windows converted from UTF-16, where a unit that is half of no
 * pair, which Windows allows in a name, crosses as the three bytes UTF-8's
 * rule would give that number. Those are not valid UTF-8, so a caller that
 * validates refuses that one text, as it refuses a POSIX name that is not
 * UTF-8, and nothing is replaced or dropped.
 *
 * NO DOOR WRITES THE ENVIRONMENT (the author's ruling, M-rebel-library): in C
 * a write of it beside another thread's read is undefined, so this layer
 * only reads. A C library the program binds that writes it (`setenv`,
 * `putenv`) beside a call of this header is that undefined behaviour, which
 * no lock here could close, since the library takes none.
 *
 * NOTHING IS KEPT BETWEEN CALLS but the last error, which is per thread: the
 * member holds no table and no lock, so every function may be called from any
 * thread at once.
 */

#ifndef HERO_SYS_H
#define HERO_SYS_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform, and the numbers
 * `hero_stream.h` gives the kinds of the same names; the platform's own
 * number for the last failure is `hero_sys_last_os_error()`. */
#define HERO_SYS_OK 0
/* An argument the layer refuses: a count with no buffer behind it. */
#define HERO_SYS_INVALID 1
/* The machine names no such thing: no home folder for the user, or on
 * Windows no record of the process among the running ones. */
#define HERO_SYS_NOT_FOUND 3
#define HERO_SYS_DENIED 5
/* The process or the machine has no memory or descriptor left, or the answer
 * is larger than the room the layer reads it into. */
#define HERO_SYS_LIMIT 8
/* The platform has no such thing, or the thing asked about is not one: a
 * user id on Windows, the size of an output that is not a terminal. */
#define HERO_SYS_UNSUPPORTED 11
#define HERO_SYS_OTHER 12

/* The machine's host name, by the fill form. On POSIX what `gethostname`
 * answers, the name the kernel holds, qualified or not as the machine was
 * set up; on Windows the DNS host name of the local computer
 * (`ComputerNameDnsHostname`), which is not qualified. */
int64_t hero_sys_host_name(void *buffer, uint64_t count);

/* The id of the process that started this one. On POSIX, once that process
 * has ended, the id of the one the system gave this one to (1, or a reaper).
 * On Windows the id recorded when this process was made: the parent may have
 * ended since, and the number may by now name another process. */
int64_t hero_sys_parent_pid(void);

/* The user and the group the process runs as, real and effective: POSIX's
 * `getuid`, `geteuid`, `getgid` and `getegid`, which cannot fail. On Windows
 * -1 with `HERO_SYS_UNSUPPORTED`: it names its users otherwise. */
int64_t hero_sys_user_id(void);
int64_t hero_sys_effective_user_id(void);
int64_t hero_sys_group_id(void);
int64_t hero_sys_effective_group_id(void);

/* The groups the process belongs to beside its own, POSIX's `getgroups`, by
 * the fill form with ids in place of bytes: up to `count` ids are written
 * into `ids`, and the answer is how many there are. Whether the effective
 * group is among them is the system's choice, and Darwin tells at most
 * sixteen. On Windows -1 with `HERO_SYS_UNSUPPORTED`. */
int64_t hero_sys_groups(int64_t *ids, uint64_t count);

/* The bytes of one page of memory: on Windows the page, not the 64 KiB grain
 * it reserves addresses by. */
int64_t hero_sys_page_size(void);

/* How many processors the program may run on at once, at least 1. On Linux
 * the ones the calling thread is allowed (`sched_getaffinity`, so `taskset`
 * and a container's set of processors are heeded; a container's quota of
 * time is not read), on Darwin, which has no such set, the ones online, and
 * on Windows the ones in the process's affinity mask, or every active one
 * where the process spans more than one group of processors. */
int64_t hero_sys_processor_count(void);

/* The user's home folder, by the fill form. On POSIX `$HOME`, or where that
 * is unset or empty the folder the system's record of the user names
 * (`getpwuid_r`); on Windows the known folder `FOLDERID_Profile`
 * (`C:\Users\<name>`). `HERO_SYS_NOT_FOUND` where neither names one. The
 * path alone, here and in the two below: the layer neither makes the folder
 * nor asks whether it is there. */
int64_t hero_sys_home_folder(void *buffer, uint64_t count);

/* The folder a program keeps what it can make again in, by the fill form.
 * On Darwin `Library/Caches` under the home folder; on Linux and every other
 * POSIX `$XDG_CACHE_HOME` where it is set to an absolute path, else `.cache`
 * under the home folder; on Windows `FOLDERID_LocalAppData`
 * (`...\AppData\Local`). The folder is every program's: a program keeps its
 * own under a name of its own there. */
int64_t hero_sys_cache_folder(void *buffer, uint64_t count);

/* The folder a program keeps its settings in, by the fill form. On Darwin
 * `Library/Application Support` under the home folder; on Linux and every
 * other POSIX `$XDG_CONFIG_HOME` where it is set to an absolute path, else
 * `.config` under the home folder; on Windows `FOLDERID_RoamingAppData`
 * (`...\AppData\Roaming`). Every program's, as the one above. */
int64_t hero_sys_settings_folder(void *buffer, uint64_t count);

/* Every variable of the environment, by the fill form: each as its name, `=`
 * and its value, as the system holds it, followed by one zero byte, in the
 * system's order. An entry the system holds with no `=` is handed as it
 * stands, an empty one is left out, and so are Windows' own entries whose
 * name is empty (`=C:=C:\...`, the folder it remembers for each drive). One
 * call is one reading: on Windows a copy the system makes, on POSIX the C
 * library's own list, read as it stands (see NO DOOR WRITES THE ENVIRONMENT
 * above). */
int64_t hero_sys_environment(void *buffer, uint64_t count);

/* `count` random bytes written into `buffer`, from the system's generator
 * for keys (`getentropy`; Windows' `BCryptGenRandom`), so fit for a unique
 * name or a key. Answers `count`, every byte written, or -1, and what the
 * buffer then holds is not to be used. On Linux it may wait, early in the
 * machine's start, until the system has gathered what it needs. */
int64_t hero_sys_random(void *buffer, uint64_t count);

/* The size of the terminal the process's standard output is, in characters:
 * its columns into `*columns` and its rows into `*rows`, both above 0, in
 * one reading. Answers 0, or -1 with `HERO_SYS_UNSUPPORTED` where the output
 * is not a terminal (a file, a pipe, none at all) or is one that tells no
 * size, the two then left as they were. */
int64_t hero_sys_terminal_size(int64_t *columns, int64_t *rows);

/* The systems `hero_sys_system` tells apart, the same numbers on every
 * platform. */
#define HERO_SYS_SYSTEM_OTHER 0
#define HERO_SYS_SYSTEM_DARWIN 1
#define HERO_SYS_SYSTEM_LINUX 2
#define HERO_SYS_SYSTEM_WINDOWS 3

/* Which system the program was built for and so runs on: Darwin, Linux,
 * Windows, or `HERO_SYS_SYSTEM_OTHER` for a POSIX system none of those. The
 * member's arm knows it when it is compiled; no call is made, and none
 * fails. */
int64_t hero_sys_system(void);

/* The processor architectures `hero_sys_architecture` tells apart, the same
 * numbers on every platform. */
#define HERO_SYS_ARCH_OTHER 0
#define HERO_SYS_ARCH_X86_64 1
#define HERO_SYS_ARCH_ARM64 2

/* The processor architecture the program was built for: x86-64, arm64, or
 * `HERO_SYS_ARCH_OTHER`. Known when the member is compiled, as the system
 * is; none fails. A program built for x86-64 and run by a translator on an
 * arm64 machine answers x86-64. */
int64_t hero_sys_architecture(void);

/* The system's version, by the fill form, as the system writes it. On POSIX
 * the release `uname` answers, the kernel's, which `uname -r` prints: on
 * Darwin the kernel's number, not macOS's own, though the two may agree. On
 * Windows `major.minor.build` from
 * `RtlGetVersion`, which no manifest changes, as `GetVersionExW` is changed;
 * it is found in `ntdll` when the call is made, so a
 * program links no library for it, and where it is not there the answer is
 * -1 with `HERO_SYS_UNSUPPORTED`. */
int64_t hero_sys_system_version(void *buffer, uint64_t count);

/* The name of the user the process runs as, by the fill form. On POSIX the
 * name the system's record of the effective user holds (`getpwuid_r`), or
 * where no record names one, `$USER` where it is set and not empty;
 * `HERO_SYS_NOT_FOUND` where neither names one. On Windows the name
 * `GetUserNameW` answers, of the thread's user, without its domain. */
int64_t hero_sys_user_name(void *buffer, uint64_t count);

/* The processor time the process has spent, every thread of it, in
 * nanoseconds: in its own code into `*user_ns`, in the system's on its
 * behalf into `*system_ns`, in one reading. Answers 0, or -1 with the two
 * left as they were. On POSIX `getrusage` of the process itself, counted
 * in microseconds; on Windows `GetProcessTimes`, in units of 100
 * nanoseconds. */
int64_t hero_sys_process_times(int64_t *user_ns, int64_t *system_ns);

/* The system's load averages over the last one, five and fifteen minutes,
 * each multiplied by 1000 and rounded, so 1.5 processes waiting is 1500:
 * POSIX's `getloadavg`, in one reading. Answers 0, or -1 with the three
 * left as they were; on Windows, which keeps no such average, -1 with
 * `HERO_SYS_UNSUPPORTED`. */
int64_t hero_sys_load_average(int64_t *one, int64_t *five, int64_t *fifteen);

/* The kind of the last call's failure on this thread, `HERO_SYS_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused): `errno` on POSIX; on Windows `GetLastError`, the
 * `HRESULT` a known folder was refused with, or the `NTSTATUS` of the random
 * generator or of `RtlGetVersion`. */
int64_t hero_sys_last_error(void);
int64_t hero_sys_last_os_error(void);

#endif
