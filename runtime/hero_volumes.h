/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_volumes.h — the platform layer's list of the volumes mounted on the
 * machine (M-rebel-library, gap G08; spec § 13): where each is mounted, what
 * is mounted there, its file system, its label, its size and its free bytes,
 * and what the system says of it, so a file manager's sidebar and a
 * monitor's disk table need no `df` and no `mount`.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_procs.h`
 * is: its functions are defined in `platform/hero_volumes.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_volumes.h"`, with no `link` word and no flag. On Windows the
 * member, and only a program that takes it, links `mpr` (the share a network
 * drive names).
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: the function answers -1 and
 * `hero_volumes_last_error()` then names the kind.
 *
 * NOTHING IS KEPT BETWEEN CALLS but the last error, which is per thread: the
 * member holds no table and no lock, so the function may be called from any
 * thread at once.
 */

#ifndef HERO_VOLUMES_H
#define HERO_VOLUMES_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform, and the numbers
 * `hero_disk.h` gives the kinds of the same names. */
#define HERO_VOLUMES_OK 0
/* An argument the layer refuses: a count with no buffer behind it. */
#define HERO_VOLUMES_INVALID 1
/* The system does not let this program read its table of mounts. */
#define HERO_VOLUMES_DENIED 5
/* The process or the machine has no memory left, or the answer is larger
 * than 64 bits can count. */
#define HERO_VOLUMES_LIMIT 10
/* The platform keeps no table of mounts: a POSIX system that is neither
 * Darwin nor Linux, or a Linux whose `/proc` is not mounted. */
#define HERO_VOLUMES_UNSUPPORTED 11
#define HERO_VOLUMES_OTHER 12

/* The fields of one entry of `hero_volumes_list`'s answer, each written and
 * followed by one zero byte, in this order. A text the system does not give
 * is written empty, its zero byte alone:
 *
 *   0 the folder it is mounted on, absolute: on POSIX the bytes the system
 *     holds, which need not be valid UTF-8; on Windows UTF-8, a drive's root
 *     `C:\` or a folder a volume is mounted on, ending in `\`;
 *   1 what is mounted there, as the system names it: a device
 *     (`/dev/disk3s1`, `/dev/sda2`), a share (`server:/export`,
 *     `\\server\share`), a volume's name (`\\?\Volume{...}\`), or the name a
 *     file system that stores nothing gives itself (`proc`, `devfs`);
 *   2 the file system's name, as the system writes it: `apfs`, `ext4`,
 *     `NTFS`, `fuse.sshfs`;
 *   3 the volume's label, the name a person gave it and a file manager
 *     shows: on Darwin the volume's name (`getattrlist`'s `ATTR_VOL_NAME`), on
 *     Linux the label udev links in `/dev/disk/by-label`, on Windows
 *     `GetVolumeInformationW`'s; empty where it has none;
 *   4 the volume's size in bytes, in decimal digits;
 *   5 the bytes free on it, to any user, the system's reserve included, in
 *     decimal digits;
 *   6 the bytes this user may still write, in decimal digits: at most field
 *     5, the system's reserve and this user's quota left out;
 *   7 `1` where the volume is read only, else `0`;
 *   8 `1` where it is stored on another machine, else `0`: on Darwin a mount
 *     not marked `MNT_LOCAL`, on Linux a file system systemd v262.2 counts a
 *     network one (`fstype_is_network`), on Windows a drive `GetDriveTypeW`
 *     names `DRIVE_REMOTE`;
 *   9 `1` where its medium can be taken out of the machine, else `0`: on
 *     Darwin `MNT_REMOVABLE`, on Linux the kernel's `removable` of the disk
 *     under it (`/sys/dev/block`), on Windows `DRIVE_REMOVABLE` and
 *     `DRIVE_CDROM`;
 *  10 `1` where the system marks it as no place for a person's files, kept
 *     out of its file manager, else `0`: Darwin's `MNT_DONTBROWSE`, which
 *     marks the system's own volumes under `/System/Volumes`; Linux and
 *     Windows mark no volume so, and write `0`. */
#define HERO_VOLUMES_FIELDS 11

/* Every volume mounted on the machine, by the fill form, in the order the
 * system lists its mounts, `HERO_VOLUMES_FIELDS` zero-terminated fields each
 * (the list above). THE FILL FORM, as `hero_sys.h` says it: up to `count`
 * bytes are written into `buffer`, and the answer is how many bytes the
 * WHOLE answer has; where that is above `count` the buffer was too short,
 * what it holds is not the answer, and the caller asks again with at least
 * that many (a count of 0 only measures, and `buffer` may then be NULL). One
 * call is one reading, and two calls are two: volumes come and go between
 * them.
 *
 * A mount whose size the system will not tell (a share that does not answer
 * within the system's own wait, a drive with no medium in it, a folder this
 * user may not enter) is not in the answer: never an entry is made up for
 * one. On Darwin the list is `getfsstat` with `MNT_NOWAIT`, so a network
 * volume that does not answer holds no call, its figures the system's last;
 * on Linux it is `/proc/self/mountinfo`, each mount asked `statvfs`, which a
 * share that does not answer holds as it holds `df`; on Windows it is every
 * drive letter (`GetLogicalDriveStringsW`) and then every folder a local
 * volume is mounted on (`FindFirstVolumeW`, `GetVolumePathNamesForVolumeNameW`).
 * On a POSIX system neither Darwin nor Linux, -1 with
 * `HERO_VOLUMES_UNSUPPORTED`. */
int64_t hero_volumes_list(void *buffer, uint64_t count);

/* The kind of the last call's failure on this thread, `HERO_VOLUMES_OK` after
 * a call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused): `errno` on POSIX, `GetLastError` on Windows. */
int64_t hero_volumes_last_error(void);
int64_t hero_volumes_last_os_error(void);

#endif
