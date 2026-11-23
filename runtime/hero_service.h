/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_service.h — the platform layer's service: the program as a Windows
 * service of the Service Control Manager, its start answered, its status
 * reported, and the controls the system sends it (stop, pause, continue,
 * shutdown and the rest) handed to the program as values it waits on
 * (M-rebel-library, 2026-11-23; spec § 13). What Go's `svc` package, .NET's
 * `ServiceBase` and Rust's `windows-service` give a program.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_watch.h`
 * is: its functions are defined in `platform/hero_service.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_service.h"`, with no `link` word and no flag. The member calls
 * `hero_net.h` (the stop, and the bell), so a program that takes it takes
 * that one; on Windows it links `advapi32` and `ws2_32`.
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: a group sees `int64_t`,
 * `uint64_t`, `const char *`, and the socket of `hero_net.h`, which the bell
 * is.
 *
 * ONLY WINDOWS HAS SERVICES. Every other system answers
 * `HERO_SERVICE_UNSUPPORTED` to everything but `hero_service_supported` (0)
 * and `hero_service_is_service` (0): a plain process on Linux and macOS is
 * already what systemd and launchd start.
 *
 * ONE SERVICE A PROGRAM, ONCE. The program is the service of the own-process
 * kind: the name it starts under is the one `sc create` registered, and the
 * Service Control Manager's dispatcher blocks the thread that calls it, so
 * the member runs the dispatcher on a thread of its own and `main` keeps the
 * program's work. A second `hero_service_start`, after the first ended or
 * not, answers `HERO_SERVICE_EXISTS`: the dispatcher answers one start for
 * the life of a process.
 *
 * THE CONTROLS. The system calls the member's handler on the dispatcher's
 * thread, which must return at once: the handler puts the control in a queue
 * of `HERO_SERVICE_QUEUE` entries and rings the bell; `hero_service_next` takes
 * them in order. A control the program did not say it accepts (`accepts` of
 * `hero_service_report`) is answered not implemented, never queued; the
 * interrogate control is answered by the member and never told. A queue that
 * is full drops the notification kinds (`HERO_SERVICE_PARAM_CHANGE`,
 * `HERO_SERVICE_TIME_CHANGE`, `HERO_SERVICE_POWER`, `HERO_SERVICE_SESSION`,
 * `HERO_SERVICE_USER`) before the lifecycle ones, which put the oldest
 * notification out; a program that takes its controls loses none.
 *
 * THE BELL, as `hero_watch.h`'s: a datagram socket of the loopback a
 * readiness set waits on, rung while a control waits, so one loop serves
 * connections, timers and the service's own controls. A datagram from a
 * stranger can make it readable with nothing queued, so a `hero_service_next`
 * with a limit of 0 may answer none.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answers -1 and
 * `hero_service_last_error()` names the kind, `hero_service_last_os_error()`
 * the system's number, `hero_service_last_message` the system's own words. */

#ifndef HERO_SERVICE_H
#define HERO_SERVICE_H

#include "hero_net.h"

#include <stdint.h>

/* The kinds of failure `hero_service_last_error` answers. */
#define HERO_SERVICE_OK 0
/* An argument out of its range: a name empty, over `HERO_SERVICE_NAME_MOST`
 * bytes, holding a slash or not UTF-8; a state or a bit nobody defines. */
#define HERO_SERVICE_INVALID 1
/* This system has no services. */
#define HERO_SERVICE_UNSUPPORTED 2
/* The program was not started by the Service Control Manager: run from a
 * console or by another program. */
#define HERO_SERVICE_NOT_SERVICE 3
/* A service was started by this program already. */
#define HERO_SERVICE_EXISTS 4
/* No service stands: not started, or ended. */
#define HERO_SERVICE_NOT_OPEN 5
/* The system's answer did not come in the time given. */
#define HERO_SERVICE_TIMEOUT 6
/* The program was asked to stop while the call waited. */
#define HERO_SERVICE_STOPPED 7
/* No socket, thread or memory for what the call needs. */
#define HERO_SERVICE_LIMIT 8
/* The system refused for a reason none of the above names. */
#define HERO_SERVICE_OTHER 9
/* The buffer was shorter than the answer. */
#define HERO_SERVICE_SHORT 10
/* The system refused the report: its status handle is gone, the service was
 * deleted under the program. */
#define HERO_SERVICE_DENIED 11

/* The states a service reports, the numbers of `hero_service_report`. */
#define HERO_SERVICE_STATE_STOPPED 1
#define HERO_SERVICE_STATE_START_PENDING 2
#define HERO_SERVICE_STATE_STOP_PENDING 3
#define HERO_SERVICE_STATE_RUNNING 4
#define HERO_SERVICE_STATE_CONTINUE_PENDING 5
#define HERO_SERVICE_STATE_PAUSE_PENDING 6
#define HERO_SERVICE_STATE_PAUSED 7

/* The controls a service may accept, the bits of `hero_service_report`'s
 * `accepts`. A pending state and the stopped state accept none: the member
 * clears what a state forbids, as the system asks. */
#define HERO_SERVICE_ACCEPT_STOP 1
#define HERO_SERVICE_ACCEPT_PAUSE_CONTINUE 2
#define HERO_SERVICE_ACCEPT_SHUTDOWN 4
#define HERO_SERVICE_ACCEPT_PRESHUTDOWN 8
#define HERO_SERVICE_ACCEPT_PARAM_CHANGE 16
#define HERO_SERVICE_ACCEPT_TIME_CHANGE 32
#define HERO_SERVICE_ACCEPT_POWER 64
#define HERO_SERVICE_ACCEPT_SESSION 128
#define HERO_SERVICE_ACCEPT_ALL 255

/* The kinds of control, the first of the numbers `hero_service_next` fills. */
#define HERO_SERVICE_STOP 1
#define HERO_SERVICE_PAUSE 2
#define HERO_SERVICE_CONTINUE 3
#define HERO_SERVICE_SHUTDOWN 4
#define HERO_SERVICE_PRESHUTDOWN 5
#define HERO_SERVICE_PARAM_CHANGE 6
#define HERO_SERVICE_TIME_CHANGE 7
/* A power event: the second number is the system's event (`PBT_`), the third
 * 0. */
#define HERO_SERVICE_POWER 8
/* A session change: the second number is the system's event (`WTS_`), the
 * third the session's number. */
#define HERO_SERVICE_SESSION 9
/* A control of the service's own, 128 to 255: the second number is its code. */
#define HERO_SERVICE_USER 10

/* The numbers one control takes, the entries a queue holds, and the most
 * bytes of a name. */
#define HERO_SERVICE_NUMBERS 3
#define HERO_SERVICE_QUEUE 64
#define HERO_SERVICE_NAME_MOST 256

/* 1 where this system has services, 0 where it has none. Never fails. */
int64_t hero_service_supported(void);

/* 1 where this process was started by the Service Control Manager, 0 where
 * it was started any other way and everywhere but Windows. Never fails and
 * starts nothing. */
int64_t hero_service_is_service(void);

/* Starts the dispatcher on a thread of the layer's own and waits up to
 * `limit_ms` (-1 for ever) for the system to call the program as the service
 * `name`: the service then stands, in `HERO_SERVICE_STATE_START_PENDING`, and
 * the program reports each state after it. Answers 0, or -1 with the kind. */
int64_t hero_service_start(const char *name, int64_t limit_ms);

/* The start parameters the system gave, the service's name first, each
 * UTF-8 and followed by a zero byte, written into `buffer`, `n` long, where
 * they all fit; their length in bytes, or -1 with `HERO_SERVICE_NOT_OPEN`.
 * A room shorter than that gets nothing written and the length all the same;
 * 0 for `n` asks the length alone. */
int64_t hero_service_arguments(void *buffer, uint64_t n);

/* Reports the state `state` to the Service Control Manager with the controls
 * the program `accepts`. A pending state is given the next check point and
 * the wait hint `wait_hint_ms` (0 to 3,600,000); any other state a check
 * point of 0. `exit_code` is read for `HERO_SERVICE_STATE_STOPPED` alone: 0
 * is success, any other number the service's own code, told to the system as
 * its service-specific exit code. Answers 0, or -1. */
int64_t hero_service_report(int64_t state, int64_t accepts, int64_t exit_code, int64_t wait_hint_ms);

/* Takes the oldest queued control into `numbers` (`HERO_SERVICE_NUMBERS`
 * entries at least), waiting at most `limit_ns` for one (0 not at all, -1
 * for ever). Answers 1 with the control taken, 0 where none came, -1 with
 * `HERO_SERVICE_STOPPED` where the program was asked to stop,
 * `HERO_SERVICE_NOT_OPEN` where the service ended. */
int64_t hero_service_next(int64_t limit_ns, int64_t *numbers, uint64_t numbers_count);

/* How many controls wait; -1 with `HERO_SERVICE_NOT_OPEN`. */
int64_t hero_service_count(void);

/* The bell, borrowed: the service's. NULL with the kind where no socket can
 * be made. */
hero_net_socket *hero_service_bell_socket(void);

/* Reports `HERO_SERVICE_STATE_STOPPED` with `exit_code` (as
 * `hero_service_report` reads it) and ends the service: the dispatcher's
 * thread joined, the bell closed. Answers 0, or -1. A wait another thread is
 * in answers `HERO_SERVICE_NOT_OPEN`. */
int64_t hero_service_end(int64_t exit_code);

int64_t hero_service_last_error(void);
int64_t hero_service_last_os_error(void);

/* The system's own words for the last failure's number, UTF-8, written into
 * `buffer`, `n` long, where they fit; their length, 0 where the layer itself
 * refused. */
int64_t hero_service_last_message(void *buffer, uint64_t n);

#endif
