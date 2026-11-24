/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_syslog.h — the platform layer's way into the system's own log
 * (M-rebel-library, 2026-11-24; spec § 13): the three logs a daemon's lines
 * belong in, each reached the way its system documents.
 *
 * LINUX: the journal's native protocol, one datagram of fields to
 * `/run/systemd/journal/socket` (systemd's `systemd-journald.service(8)` and
 * the protocol page `JOURNAL_NATIVE_PROTOCOL`), and the classic syslog socket
 * `/dev/log` where no journal runs. The bytes are the package's to write;
 * this layer sends them, and passes a journal datagram too long for one
 * through a sealed memory file, as the protocol asks. Every POSIX system has
 * the datagram half, so a test sends to a socket of its own on any of them.
 *
 * MACOS: the unified log, `os_log_create` and `os_log_with_type` with the
 * text public, since a program's line is not a secret the system should
 * redact.
 *
 * WINDOWS: the Event Log, `RegisterEventSourceW` and `ReportEventW` with the
 * line as the event's one string, and the source registered under the
 * Application log with `EventCreate.exe` as its message file, as Go's
 * `eventlog.InstallAsEventCreate` does, so the line shows whole.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answers -1 and
 * `hero_syslog_last_error` names the kind, `hero_syslog_last_os_error` the
 * system's number and `hero_syslog_last_message` the layer's own words.
 * Nothing here is held between calls: no handle, no open log, so no value of
 * the program can outlive what it names.
 */

#ifndef HERO_SYSLOG_H
#define HERO_SYSLOG_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform. */
#define HERO_SYSLOG_OK 0
/* An argument the layer refuses: an empty name, a byte that may not stand
 * in it, a level out of range. */
#define HERO_SYSLOG_INVALID 1
/* This system has no such log. */
#define HERO_SYSLOG_UNSUPPORTED 2
/* Nothing answers at the socket, or no source of that name is registered. */
#define HERO_SYSLOG_NOT_FOUND 3
/* The system refuses the caller: a socket's permission, the registry's. */
#define HERO_SYSLOG_DENIED 4
/* The log's queue is full, or the line is longer than it takes. */
#define HERO_SYSLOG_LIMIT 5
/* A source of that name is registered already. */
#define HERO_SYSLOG_EXISTS 6
/* A wait for a datagram passed. */
#define HERO_SYSLOG_TIMEOUT 7
#define HERO_SYSLOG_OTHER 8

/* What `hero_syslog_has` is asked. */
#define HERO_SYSLOG_HAS_DATAGRAM 0
#define HERO_SYSLOG_HAS_UNIFIED 1
#define HERO_SYSLOG_HAS_EVENT_LOG 2

/* The levels, RFC 5424's severities: 0 emergency to 7 debug. */
#define HERO_SYSLOG_LEVEL_MOST 7

/* The most bytes of one line the unified log and the Event Log are given:
 * `ReportEventW` takes a string of at most 31,839 characters. */
#define HERO_SYSLOG_LINE_MOST 31839

/* 1 where this system has `what`, 0 where it has not. Never fails. */
int64_t hero_syslog_has(int64_t what);

/* Sends the `count` bytes of `data`, 1 or more, as one datagram to the local
 * datagram socket at `path`, never waiting: 0, or -1 with
 * `HERO_SYSLOG_NOT_FOUND` where nothing answers there, `HERO_SYSLOG_DENIED`
 * where the system refuses the sender, `HERO_SYSLOG_LIMIT` where the
 * receiver's queue is full or, `sealed` being 0, the datagram is longer
 * than the system carries; `HERO_SYSLOG_INVALID` for a path longer than an
 * address holds, `HERO_SYSLOG_UNSUPPORTED` on Windows. Where `sealed` is 1
 * and the datagram is too long, Linux sends it as the journal's protocol
 * asks, an empty datagram carrying a sealed memory file of the bytes; off
 * Linux that is `HERO_SYSLOG_LIMIT` as well. */
int64_t hero_syslog_send(const char *path, const void *data, uint64_t count, int64_t sealed);

/* One line into the unified log under `subsystem` and `category`, at the
 * level `level` (0 to 7, mapped to the log's five types: 0 to 2 a fault, 3
 * an error, 4 and 5 the default, 6 info, 7 debug): 0, or -1 with
 * `HERO_SYSLOG_INVALID` for an empty subsystem, a level out of range or a
 * line longer than `HERO_SYSLOG_LINE_MOST`, `HERO_SYSLOG_UNSUPPORTED` off
 * macOS. The log keeps no answer to give: a line it drops is not told. */
int64_t hero_syslog_unified(const char *subsystem, const char *category, int64_t level, const char *line);

/* One event into the Event Log from the source `source`, of the type the
 * level gives (0 to 3 an error, 4 a warning, 5 to 7 information) and the
 * event id `id` (1 to 1000, the ids `EventCreate.exe`'s messages show
 * whole), the line its one string: 0, or -1 with `HERO_SYSLOG_INVALID`,
 * `HERO_SYSLOG_DENIED`, `HERO_SYSLOG_UNSUPPORTED` off Windows. */
int64_t hero_syslog_event(const char *source, int64_t level, int64_t id, const char *line);

/* Registers `source` under the Application log with `EventCreate.exe` as its
 * message file and the three types it writes: 0, or -1 with
 * `HERO_SYSLOG_EXISTS` where it is registered, `HERO_SYSLOG_DENIED` without
 * the administrator's rights, `HERO_SYSLOG_UNSUPPORTED` off Windows. */
int64_t hero_syslog_event_install(const char *source);

/* Removes what `hero_syslog_event_install` registered: 0, or -1 with
 * `HERO_SYSLOG_NOT_FOUND` where nothing is, `HERO_SYSLOG_DENIED`. */
int64_t hero_syslog_event_remove(const char *source);

int64_t hero_syslog_last_error(void);
int64_t hero_syslog_last_os_error(void);

/* The layer's own words for the last failure, UTF-8, written into `buffer`,
 * `n` long, where they fit; their length, 0 where there are none. */
int64_t hero_syslog_last_message(void *buffer, uint64_t n);

#endif
