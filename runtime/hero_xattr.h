/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_xattr.h — the platform layer's extended attributes (M-rebel-library,
 * gap G07; spec § 13): the named values a file or a folder carries beside its
 * bytes, listed, read, written and removed, so a backup keeps a macOS file's
 * tags and quarantine, a Linux file's SELinux label, and what a program
 * stored beside a file of its own.
 *
 * On Darwin and Linux an attribute is the system's own (`getxattr`,
 * `setxattr`, `listxattr`, `removexattr`); a Linux name carries its
 * namespace, `user.`, `security.`, `trusted.` or `system.`, and Darwin's has
 * none (`com.apple.quarantine`). On Windows an attribute is a named data
 * stream of NTFS, the file's own `file:name` (`FindFirstStreamW`'s kind):
 * its name is the stream's, compared without regard to case, and its value
 * the stream's bytes; `Zone.Identifier` is Windows' quarantine.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_procs.h`
 * is: its functions are defined in `platform/hero_xattr.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_xattr.h"`, with no `link` word and no flag.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function answers -1 and
 * `hero_xattr_last_error()` then names the kind.
 *
 * NOTHING IS KEPT BETWEEN CALLS but the last error, which is per thread. A
 * path and a name are UTF-8 on Windows, any bytes but zero on POSIX.
 */

#ifndef HERO_XATTR_H
#define HERO_XATTR_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform, and the numbers
 * `hero_disk.h` gives the kinds of the same names. */
#define HERO_XATTR_OK 0
/* A name the system takes no such: empty, too long, or on Windows holding
 * `:`, `\` or `/`; or a count with no buffer behind it. */
#define HERO_XATTR_INVALID 1
/* The path names nothing. */
#define HERO_XATTR_NOT_FOUND 3
/* The attribute is there, and the write was asked only of one that is not. */
#define HERO_XATTR_EXISTS 4
/* The system refuses this to the program: the file's permission, a
 * namespace this user may not write (Linux's `trusted.`, `user.` on a link),
 * a name Darwin keeps for itself. */
#define HERO_XATTR_DENIED 5
/* A part of the path on its way is not a folder. */
#define HERO_XATTR_NOT_A_DIRECTORY 6
/* The system has no room for it: no memory, no space on the volume, or the
 * user's quota spent. */
#define HERO_XATTR_LIMIT 10
/* The volume keeps no such attribute (a file system with none, a Linux
 * namespace it does not keep, a Windows volume with no named streams), or
 * the system has none: a POSIX system neither Darwin nor Linux. */
#define HERO_XATTR_UNSUPPORTED 11
#define HERO_XATTR_OTHER 12
/* On Windows, a path or a name that is not UTF-8. */
#define HERO_XATTR_NOT_TEXT 13
/* The path is there and holds no attribute of that name. */
#define HERO_XATTR_ABSENT 16
/* The value is larger than the volume keeps for one attribute. */
#define HERO_XATTR_TOO_LARGE 17

/* What `hero_xattr_write` does where the attribute is there, or not. */
#define HERO_XATTR_EITHER 0
/* Only where it is not there yet; `HERO_XATTR_EXISTS` where it is. */
#define HERO_XATTR_ONLY_NEW 1
/* Only where it is there already; `HERO_XATTR_ABSENT` where it is not. */
#define HERO_XATTR_ONLY_EXISTING 2

/* Every function that names a path follows a link at its end where `follow`
 * is not 0, and works on the link itself where it is 0 (Darwin's
 * `XATTR_NOFOLLOW`, Linux's `l` calls, Windows' `FILE_FLAG_OPEN_REPARSE_POINT`).
 * Linux keeps no `user.` attribute on a link, so writing one there is
 * `HERO_XATTR_DENIED`.
 *
 * THE FILL FORM, as `hero_sys.h` says it, for the two that answer bytes: up
 * to `count` bytes are written into `buffer`, and the answer is how many
 * bytes the WHOLE answer has; where that is above `count` the buffer was too
 * short, what it holds is not the answer, and the caller asks again with at
 * least that many (a count of 0 only measures, and `buffer` may then be
 * NULL). One call is one reading: an attribute may change between two. */

/* The names of `path`'s attributes, each followed by one zero byte, in the
 * order the system lists them. A Linux name this user may not read (a
 * `trusted.` one, for a user who is not root) is not listed; on Windows the
 * file's unnamed stream, its bytes, is not an attribute and is not listed. */
int64_t hero_xattr_names(const char *path, int64_t follow, void *buffer, uint64_t count);

/* The value of `path`'s attribute `name`, its bytes, which may be none. */
int64_t hero_xattr_read(const char *path, const char *name, int64_t follow, void *buffer, uint64_t count);

/* `size` bytes of `value` as `path`'s attribute `name`, whole: on Darwin and
 * Linux the system writes it at once or not at all; on Windows the stream is
 * written and then cut to `size`, so a failure part way may leave it short.
 * `mode` is `HERO_XATTR_EITHER`, `HERO_XATTR_ONLY_NEW` or
 * `HERO_XATTR_ONLY_EXISTING`. 0, or -1. */
int64_t hero_xattr_write(const char *path, const char *name, const void *value, uint64_t size, int64_t follow,
                         int64_t mode);

/* `path`'s attribute `name` removed; `HERO_XATTR_ABSENT` where it has none of
 * that name. 0, or -1. */
int64_t hero_xattr_remove(const char *path, const char *name, int64_t follow);

/* The kind of the last call's failure on this thread, `HERO_XATTR_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused): `errno` on POSIX, `GetLastError` on Windows. */
int64_t hero_xattr_last_error(void);
int64_t hero_xattr_last_os_error(void);

#endif
