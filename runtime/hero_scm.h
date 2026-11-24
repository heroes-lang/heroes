/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_scm.h — the platform layer's half of a service installed: a service of
 * the Windows Service Control Manager created, started, stopped, asked and
 * deleted by another program (M-rebel-library, 2026-11-24; spec § 13). What
 * Go's `golang.org/x/sys/windows/svc/mgr` and Rust's `windows-service`
 * `ServiceManager` give a program; `hero_service.h` is the other half, the
 * program that is the service.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_service.h`
 * is: its functions are defined in `platform/hero_scm.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_scm.h"`, with no `link` word and no flag. On Windows the
 * member links `advapi32`.
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: a group sees `int64_t`,
 * `uint64_t` and `const char *`, every text UTF-8, which the member turns into
 * the system's UTF-16 and back.
 *
 * ONLY WINDOWS HAS THE SERVICE CONTROL MANAGER. Every other system answers
 * `HERO_SCM_UNSUPPORTED` to everything but `hero_scm_supported` (0): systemd
 * and launchd are reached through their own programs, `systemctl` and
 * `launchctl`, by the package above this layer.
 *
 * WHAT A CALL WAITS FOR: nothing but the system's answer. A start or a stop
 * asks and returns; the package above asks `hero_scm_query` again until the
 * state it waits for comes or its limit passes, so no wait here is unbounded.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answers -1 and
 * `hero_scm_last_error()` names the kind, `hero_scm_last_os_error()` the
 * system's number, `hero_scm_last_message` the system's own words, all of
 * the calling thread. */

#ifndef HERO_SCM_H
#define HERO_SCM_H

#include <stdint.h>

/* The kinds of failure `hero_scm_last_error` answers. */
#define HERO_SCM_OK 0
/* An argument the layer refuses: a text that is not UTF-8 or is empty where a
 * text is owed, a number out of its range. */
#define HERO_SCM_INVALID 1
/* This system has no Service Control Manager: it is not Windows. */
#define HERO_SCM_UNSUPPORTED 2
/* A service of that name is installed already (`ERROR_SERVICE_EXISTS`, or
 * `ERROR_DUPLICATE_SERVICE_NAME` where another's display name is the name). */
#define HERO_SCM_EXISTS 3
/* No service of that name is installed (`ERROR_SERVICE_DOES_NOT_EXIST`). */
#define HERO_SCM_NOT_FOUND 4
/* The system refuses the program this (`ERROR_ACCESS_DENIED`): creating,
 * starting, stopping or deleting a service asks an administrator's rights. */
#define HERO_SCM_DENIED 5
/* The service is deleted already and goes once its last handle is closed
 * (`ERROR_SERVICE_MARKED_FOR_DELETE`): nothing of that name can be made or
 * started until then. */
#define HERO_SCM_MARKED 6
/* The service is between two states and takes no control now
 * (`ERROR_SERVICE_CANNOT_ACCEPT_CTRL`): asked again, it may. */
#define HERO_SCM_BUSY 7
/* The service did not answer its start in the time the system allows
 * (`ERROR_SERVICE_REQUEST_TIMEOUT`). */
#define HERO_SCM_TIMEOUT 8
/* No memory was left for the layer's own copies. */
#define HERO_SCM_LIMIT 9
/* The system refused for another reason: `hero_scm_last_os_error` holds its
 * number and `hero_scm_last_message` its words. */
#define HERO_SCM_OTHER 10

/* The states `hero_scm_query` answers, the system's own numbers
 * (`SERVICE_STOPPED` to `SERVICE_PAUSED`), which the member checks against
 * `winsvc.h` where it is compiled for Windows. */
#define HERO_SCM_STATE_STOPPED 1
#define HERO_SCM_STATE_START_PENDING 2
#define HERO_SCM_STATE_STOP_PENDING 3
#define HERO_SCM_STATE_RUNNING 4
#define HERO_SCM_STATE_CONTINUE_PENDING 5
#define HERO_SCM_STATE_PAUSE_PENDING 6
#define HERO_SCM_STATE_PAUSED 7

/* The longest name and description the member takes, in bytes of UTF-8: the
 * system's service name is at most 256 characters, and a description is held
 * to the same bound of the layer's so that no text is sized by its caller. */
#define HERO_SCM_NAME_MOST 256
#define HERO_SCM_TEXT_MOST 32768

/* 1 where this system has the Service Control Manager, else 0. */
int64_t hero_scm_supported(void);

/* Creates the service `name`, of its own process, run as the machine's own
 * account (LocalSystem), started by the system at boot where `at_boot` is 1
 * and only when asked where it is 0, its display name `name`, its description
 * `description` ("" for none), the command line `command_line` (the program's
 * path quoted where it holds a blank, then its arguments, as the C runtime
 * reads them back). Where `restart_delay_ms` is 0 or more the system restarts
 * the service that fails, after that many milliseconds, and counts as a
 * failure a stop the service reports with an exit code other than 0; where
 * it is -1 it restarts nothing. Answers 0, or -1 with nothing left behind: a
 * service made and then not configured is deleted again. */
int64_t hero_scm_install(const char *name, const char *description, const char *command_line, int64_t at_boot, int64_t restart_delay_ms);

/* Asks the system to start the service `name`. Answers 0 where the start was
 * asked, 1 where the service ran already, -1 on failure. */
int64_t hero_scm_start(const char *name);

/* Asks the service `name` to stop. Answers 0 where the stop was asked, 1
 * where it was stopped already, -1 on failure (`HERO_SCM_BUSY` while it is
 * starting). */
int64_t hero_scm_stop(const char *name);

/* What the service `name` is in: `*state` one of `HERO_SCM_STATE_*`, `*pid`
 * its process's id or 0, `*at_boot` 1 where the system starts it at boot,
 * `*exit_code` the code it last stopped with (its own code where it reported
 * one of its own, else the system's error number; 0 for none). Answers 0 or
 * -1; on -1 the four are left as they were. */
int64_t hero_scm_query(const char *name, int64_t *state, int64_t *pid, int64_t *at_boot, int64_t *exit_code);

/* Deletes the service `name`: the system removes it once no program holds a
 * handle of it and it has stopped. Answers 0, also where it was deleted
 * already and waits for its last handle, or -1. */
int64_t hero_scm_remove(const char *name);

/* The kind of the calling thread's last failure, one of `HERO_SCM_*`. */
int64_t hero_scm_last_error(void);

/* The system's number for the calling thread's last failure, 0 where the
 * layer refused it itself. */
int64_t hero_scm_last_os_error(void);

/* The system's words for that number, UTF-8 with no line end, written into
 * `buffer` where `n` holds them all; answers how many bytes they take, 0
 * where there are none. */
int64_t hero_scm_last_message(void *buffer, uint64_t n);

#endif
