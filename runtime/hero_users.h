/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_users.h — the platform layer's questions about the machine's users and
 * groups (M-rebel-library, gap G12; spec § 13): a user or a group found by its
 * name or by its id, the groups a named user belongs to, and the user the
 * process runs as changed.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_sys.h` is:
 * its functions are defined in `platform/hero_users.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_users.h"`, with no `link` word and no flag. On Windows the
 * member, and only a program that takes it, links `advapi32` (the accounts)
 * and `netapi32` (a user's primary group and local groups).
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function answers -1 and
 * `hero_users_last_error()` then names the kind.
 *
 * AN ID IS TEXT, AS THE SYSTEM WRITES IT: on POSIX the number in decimal
 * digits (`501`), on Windows the security identifier (`S-1-5-21-...-500`).
 * The layer hands it back so, and takes it so, and no platform type crosses.
 *
 * THE FILL FORM, as `hero_sys.h` says it: a function that answers bytes takes
 * the caller's `buffer` and its `count` of bytes, writes nothing past them,
 * and answers how many bytes the WHOLE answer has; where that is above `count`
 * the buffer was too short and the caller asks again with at least that many.
 * A `count` of 0 only measures, and `buffer` may then be NULL. An answer is
 * fields, each text followed by one zero byte; a field the system does not
 * tell is written empty, its zero byte alone. Text is UTF-8: on POSIX the bytes
 * the system holds, which need not be valid UTF-8; on Windows converted from
 * UTF-16, a unit that is half of no pair crossing as the three bytes UTF-8's
 * rule would give its number.
 *
 * NOTHING IS KEPT BETWEEN CALLS but the last error, which is per thread: the
 * member holds no table and no lock, so every function may be called from any
 * thread at once. The system's own record of a user may be a directory
 * service's, so a lookup may wait on the network.
 */

#ifndef HERO_USERS_H
#define HERO_USERS_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform, and the numbers
 * `hero_sys.h` gives the kinds of the same names. */
#define HERO_USERS_OK 0
/* An argument the layer refuses: an empty name, an id that is not an id, a
 * count with no buffer behind it. */
#define HERO_USERS_INVALID 1
/* No user or group has that name or that id. */
#define HERO_USERS_NOT_FOUND 3
/* The system does not let this process do it: the user is changed only by one
 * with the rights to. */
#define HERO_USERS_DENIED 5
/* The process or the machine has no memory left, or the answer is larger
 * than the room the layer reads it into. */
#define HERO_USERS_LIMIT 8
/* The platform has no such thing: a process of Windows keeps the user it was
 * started as. */
#define HERO_USERS_UNSUPPORTED 11
#define HERO_USERS_OTHER 12

/* The user named `name`, by the fill form: four fields. 0 the user's id; 1
 * the user's name as the system holds it (on Windows without a domain, in the
 * case the account was made with); 2 the id of the user's primary group, empty
 * where the system does not tell it (Windows, for an account of a domain this
 * machine cannot ask, and for the service accounts); 3 the user's home folder,
 * empty where the user has none (a Windows user that has never logged on has no
 * profile). On POSIX `getpwnam_r`, whose record a directory service may be the
 * source of; on Windows `LookupAccountNameW`, which also reads `DOMAIN\user`
 * and `user@domain`, the primary group `NetUserGetInfo`'s and the home the
 * profile list's `ProfileImagePath`. A name that names an account that is not a
 * user (a group) is `HERO_USERS_NOT_FOUND`, and Windows' service accounts
 * (`SYSTEM`, `LOCAL SERVICE`, `NETWORK SERVICE`), which the system counts as
 * well-known groups, are users here since they own processes. An empty name is
 * `HERO_USERS_INVALID`. */
int64_t hero_users_user_named(const char *name, void *buffer, uint64_t count);

/* The user whose id is `id`, as `hero_users_user_named` answers. An id that is
 * not digits of a number a user id holds (POSIX) or not a security identifier
 * (Windows) is `HERO_USERS_INVALID`; one no user has, `HERO_USERS_NOT_FOUND`. */
int64_t hero_users_user_identified(const char *id, void *buffer, uint64_t count);

/* The group named `name`, by the fill form: two fields. 0 the group's id; 1 the
 * group's name as the system holds it, never empty here. On POSIX
 * `getgrnam_r`; on Windows `LookupAccountNameW`, a group, an alias or a
 * well-known group. The kinds of failure are the user's. */
int64_t hero_users_group_named(const char *name, void *buffer, uint64_t count);

/* The group whose id is `id`, as `hero_users_group_named` answers. */
int64_t hero_users_group_identified(const char *id, void *buffer, uint64_t count);

/* The groups the user named `name` belongs to, by the fill form: two fields,
 * as one group is above, for each, in the system's order and none twice. The
 * name of a group the system holds no record of is written empty. On POSIX
 * `getgrouplist`, which tells the user's primary group among them; on Darwin
 * at most what the system keeps for a user (`NGROUPS_MAX`, 16). On Windows
 * `NetUserGetLocalGroups`, the machine's groups the user is a member of
 * directly or through a group of a domain. A name no user has is
 * `HERO_USERS_NOT_FOUND`. */
int64_t hero_users_groups_of(const char *name, void *buffer, uint64_t count);

/* The user and the groups of this process changed to the user named `name`:
 * its supplementary groups, then its group, then its user, each of the real,
 * effective and saved ids at once, `initgroups`, `setgid` and `setuid`, in
 * that order since a process that has given up its user may give up nothing
 * more. Answers 0 once every id is the user's and the process, which could
 * regain the user it was, no longer can (`setuid(0)` is asked and must be
 * refused); -1 otherwise. A process that is not run by root changes nothing
 * and answers 0 only where it already runs as that user and that group, ids
 * all equal, and `HERO_USERS_DENIED` otherwise: a process takes another user's
 * rights only from one with the right to give them. Every thread of the
 * process is changed. Where the change began and failed midway the process is
 * partly changed, and a caller treats it as one that must end. On Windows -1
 * with `HERO_USERS_UNSUPPORTED`: a process keeps the user it started as, and a
 * thread that takes another user's rights does it with that user's password
 * (`LogonUser`), which this layer is never given. */
int64_t hero_users_change(const char *name);

/* The kind of the last call's failure on this thread, `HERO_USERS_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused): `errno` on POSIX, `GetLastError` or `NET_API_STATUS`
 * on Windows. */
int64_t hero_users_last_error(void);
int64_t hero_users_last_os_error(void);
#endif
