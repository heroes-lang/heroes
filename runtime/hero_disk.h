/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_disk.h — the platform layer's doors on the file system that are
 * reached BY PATH (M-rebel-library; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * is: its functions are defined in `platform/hero_disk.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_disk.h"`, with no `link` word and no flag, and no unit reads
 * it unless one of its groups names it.
 *
 * WHY THESE DOORS, BESIDE `hero_os.h`'s: that header's file system is the
 * compiler's, each door the shell utility's meaning (an absent file is a
 * removal that worked, a directory is made with every parent and no
 * permission, a listing is kept by the runtime and read back by index) and
 * each answering done or failed. A library's is the system call's meaning,
 * strictly, with the kind of every failure: nothing here is `hero_os.h`'s
 * changed, and nothing here opens, reads or writes a file's bytes, which is
 * `hero_stream.h`'s.
 *
 * WHY C OF THE PROJECT'S OWN, AND NOT A BINDING OF THE PLATFORM'S: `struct
 * stat`'s `st_mode` is two bytes on Darwin and four on Linux and its times
 * are `st_mtimespec` there and `st_mtim` here, `struct dirent` has another
 * layout on each, Darwin's `statvfs` counts blocks in 32 bits, and Windows
 * has none of them: its paths are UTF-16, its times count from 1601 and its
 * links are reparse points. So no platform type reaches this header: a
 * group sees `int64_t`, `uint64_t`, `void *` and `const char *`.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function answers -1 and
 * `hero_disk_last_error()` then names the kind, the same number on every
 * platform, `hero_disk_last_os_error()` the platform's own. No door keeps
 * anything between calls but those two, which are each thread's own: there
 * is no handle here, and no listing a later call overwrites.
 *
 * A PATH IS BYTES ENDED BY A ZERO, at most `HERO_DISK_PATH_MOST` of them:
 * UTF-8, which on Windows the layer hands the system as UTF-16. A name the
 * system gives back, an entry's or a link's target, is the bytes that name
 * it: UTF-8 wherever the system holds text, and where it does not (a POSIX
 * name of other bytes; a Windows name holding half a pair, given as the
 * three bytes its value would take) bytes that are not UTF-8, whole, for the
 * caller's own check of text to find. None is cut and none replaced, and
 * Windows takes such a name back as it gave it.
 *
 * BYTES AND NUMBERS COME BACK BY THE FILL FORM: the caller hands a buffer
 * and its count, the door fills it and answers how many the whole answer
 * is. An answer greater than the count says the buffer was short: nothing
 * in it is then to be read, and the caller asks again with at least that
 * many, as often as it takes, since a folder may grow between two calls.
 */

#ifndef HERO_DISK_H
#define HERO_DISK_H

#include <stdint.h>

/* For `hero_disk_lock` alone (round 3): the stream a lock is held through. */
#include "hero_stream.h"

/* The kinds of failure, the same numbers on every platform and the layer's
 * own where its headers share a kind (2 and 9 are a handle's and a wait's,
 * which no door by path answers); the platform's own number for the last
 * failure is `hero_disk_last_os_error()`. */
#define HERO_DISK_OK 0
/* An argument the layer or the system refuses: no path, one longer than
 * `HERO_DISK_PATH_MOST` or than the system takes, a count with no buffer
 * behind it, a number outside what its door says, or a path that names no
 * link where a link is asked. */
#define HERO_DISK_INVALID 1
/* Nothing is at the path, or at a folder on the way to it; the empty path
 * names nothing. */
#define HERO_DISK_NOT_FOUND 3
#define HERO_DISK_EXISTS 4
/* The system refuses this user, or the volume is read-only. */
#define HERO_DISK_DENIED 5
/* Something that is no directory stands where one is needed: the path a
 * scan or `hero_disk_enter` is handed, on every platform; and on POSIX a
 * file standing on the way to the path, which Windows' own numbers are not
 * known to tell from a folder that is not there (unrun). */
#define HERO_DISK_NOT_A_DIRECTORY 6
#define HERO_DISK_IS_A_DIRECTORY 7
#define HERO_DISK_NOT_EMPTY 8
/* No room is left: on the volume or in a quota, in memory, among the
 * process's descriptors, or in what this layer carries of one name. */
#define HERO_DISK_LIMIT 10
/* The platform has no such thing: an owner, on Windows. */
#define HERO_DISK_UNSUPPORTED 11
#define HERO_DISK_OTHER 12
/* Windows' alone: the path's bytes are neither UTF-8 nor a name this layer
 * gave, so the system cannot be handed them. */
#define HERO_DISK_NOT_TEXT 13
/* A rename between two volumes, which moves nothing. */
#define HERO_DISK_CROSS_DEVICE 14
/* Another process, or the system, holds what the call would change. */
#define HERO_DISK_BUSY 15

/* The longest path a door takes, in bytes before its zero. */
#define HERO_DISK_PATH_MOST 4095

/* What a path names. A LINK is a symbolic link and, on Windows, a junction
 * too; OTHER a device, a pipe, a socket; UNKNOWN is `hero_disk_scan`'s
 * alone, an entry whose kind the system would not say (a folder that may be
 * listed and not entered), which `hero_disk_facts` on its path explains. */
#define HERO_DISK_KIND_FILE 1
#define HERO_DISK_KIND_DIRECTORY 2
#define HERO_DISK_KIND_LINK 3
#define HERO_DISK_KIND_OTHER 4
#define HERO_DISK_KIND_UNKNOWN 5

/* The facts `hero_disk_facts` fills, by their place in its buffer, and how
 * many there are.
 *
 * KIND is one of the kinds above. SIZE is a file's length in bytes, and for
 * anything else what the system says (a link's is its target's length on
 * POSIX, 0 on Windows). PERMISSION is the low twelve bits of a POSIX mode;
 * Windows keeps one of them, read-only: 0444 or 0666 there, as `hero_os.h`'s
 * `hero_fs_mode` answers. WRITTEN and READ are the times of the last write
 * and the last read, in nanoseconds since 1970-01-01 UTC, negative before
 * it, as fine as the volume keeps them (and READ only where the volume
 * keeps it at all); a time 64 bits of nanoseconds cannot hold, before 1678
 * or after 2262, is given as the nearest they can. LINKS is how many names
 * the file has. */
#define HERO_DISK_FACT_KIND 0
#define HERO_DISK_FACT_SIZE 1
#define HERO_DISK_FACT_PERMISSION 2
#define HERO_DISK_FACT_WRITTEN 3
#define HERO_DISK_FACT_READ 4
#define HERO_DISK_FACT_LINKS 5
#define HERO_DISK_FACTS 6

/* What `path` names, the link itself where it is one: fills the first
 * `count` of the facts above into `facts`, as many as there are at most,
 * and answers how many there are, `HERO_DISK_FACTS`; -1 with
 * `HERO_DISK_NOT_FOUND` where nothing is there. */
int64_t hero_disk_facts(const char *path, int64_t *facts, uint64_t count);

/* The same of what `path` names once every link is followed; a link whose
 * target is not there is `HERO_DISK_NOT_FOUND`. */
int64_t hero_disk_facts_through(const char *path, int64_t *facts, uint64_t count);

/* 1 where the two paths name one file, every link followed, 0 where they
 * name two, -1 where either cannot be asked. One file is one device and
 * inode; on Windows one volume serial number and file index. */
int64_t hero_disk_same(const char *first, const char *second);

/* Makes ONE directory, with the permission `permission`, 0 to 07777, less
 * what the process's mask takes away: 0, or -1 with `HERO_DISK_EXISTS`
 * where anything is at `path` already and `HERO_DISK_NOT_FOUND` where its
 * parent is not there. Windows has no permission bits: the number is
 * checked and not used, and the directory takes its parent's access. */
int64_t hero_disk_make_folder(const char *path, int64_t permission);

/* Removes a file, a link (never what it names) or an EMPTY directory,
 * strictly: 0, or -1 with `HERO_DISK_NOT_FOUND` where nothing is there and
 * `HERO_DISK_NOT_EMPTY` for a directory that holds something. A file marked
 * read-only is removed on POSIX, where the folder decides, and is
 * `HERO_DISK_DENIED` on Windows, where the file does. */
int64_t hero_disk_remove(const char *path);

/* Renames `from` to `to`, strictly, and both arms are made to answer alike:
 * 0, or -1 with `HERO_DISK_NOT_FOUND` where `from` names nothing or the
 * folder of `to` is not there, and `HERO_DISK_CROSS_DEVICE` where the two
 * stand on two volumes, nothing moved.
 *
 * WHERE `to` EXISTS. A file, or anything that is no directory, takes the
 * place of a file, the old one gone: in one step on POSIX (`rename`), and
 * by `MoveFileExW` with `MOVEFILE_REPLACE_EXISTING` on Windows, of which no
 * page promises one step. It never takes a directory's place:
 * `HERO_DISK_IS_A_DIRECTORY`. A directory never takes the place of
 * anything: `HERO_DISK_EXISTS`, where POSIX alone would have replaced an
 * empty directory and Windows nothing; the layer looks before it renames,
 * so a directory made at `to` between the look and the rename is replaced
 * on POSIX if it is empty. `to` naming `from` itself, another case of its
 * letters included, is a rename and no failure. A link is what the name
 * itself is, never what it names: a file on POSIX, and on Windows a file or
 * a directory as it was made. */
int64_t hero_disk_rename(const char *from, const char *to);

/* The entries of the directory `path`, each with its kind, no link
 * followed, as ONE scan into `buffer`: for each entry one byte holding its
 * kind (`HERO_DISK_KIND_FILE` to `HERO_DISK_KIND_UNKNOWN`), then its name's bytes,
 * then a zero byte, so a name may hold any byte but zero and `/`. The
 * entries stand in the system's order, which is none, and `.` and `..` are
 * never among them. Answers how many bytes the whole listing is, 0 for an
 * empty directory; an answer greater than `count` says the buffer was
 * short, and the caller asks again as the opening comment says. -1 with
 * `HERO_DISK_NOT_FOUND` where nothing is there and
 * `HERO_DISK_NOT_A_DIRECTORY` where it is no directory. */
int64_t hero_disk_scan(const char *path, void *buffer, uint64_t count);

/* The target of the symbolic link `path`, as the link holds it, into
 * `buffer`, with no zero after it: answers its length in bytes, an answer
 * greater than `count` saying the buffer was short. -1 with
 * `HERO_DISK_INVALID` where `path` names something that is no link, and
 * `HERO_DISK_LIMIT` for a target longer than `HERO_DISK_PATH_MOST`. On
 * Windows a junction's target is answered too, and a target is the text
 * the link shows, or where it shows none the one it resolves by. */
int64_t hero_disk_link_target(const char *path, void *buffer, uint64_t count);

/* Sets the times of the last write and the last read of what `path` names,
 * links followed, each in nanoseconds since 1970-01-01 UTC as the facts
 * give them, kept as fine as the volume keeps them: 0 or -1. Windows keeps
 * no time at or before 1601-01-01: `HERO_DISK_INVALID` there. */
int64_t hero_disk_set_times(const char *path, int64_t written, int64_t read);

/* Makes the file `path` names, links followed, `length` bytes long: 0 or
 * -1, `HERO_DISK_INVALID` for a negative length and
 * `HERO_DISK_IS_A_DIRECTORY` for a directory. Made shorter it loses its
 * end; made longer, the new bytes read as zero on POSIX, and on Windows on
 * NTFS, whose own page leaves them undefined elsewhere. */
int64_t hero_disk_set_length(const char *path, int64_t length);

/* Makes `path` the working folder, which every relative path is read in
 * from then on: 0 or -1. The folder is the PROCESS's, one for all its
 * threads. */
int64_t hero_disk_enter(const char *path);

/* The system's folder for temporary files, into `buffer`, with no zero
 * after it and no separator at its end unless it is a root: answers its
 * length in bytes, an answer greater than `count` saying the buffer was
 * short. On POSIX it is the environment's `TMPDIR` where that is set and
 * not empty, else the system's own, `/tmp`; on Windows `GetTempPathW`'s.
 * The folder is named, never made nor asked for. */
int64_t hero_disk_temp_folder(void *buffer, uint64_t count);

/* The bytes of the volume `path` stands on: into `*free_bytes` those this
 * user may still write, into `*total_bytes` the volume's size. 0, or -1
 * with nothing written. */
int64_t hero_disk_space(const char *path, int64_t *free_bytes, int64_t *total_bytes);

/* Changes the owner of what `path` names, links followed, to the user and
 * the group of those numbers, -1 leaving either as it is: 0 or -1. Windows
 * has no such numbers: `HERO_DISK_UNSUPPORTED` there, always. */
int64_t hero_disk_set_owner(const char *path, int64_t user, int64_t group);

/* The same of the link itself where `path` names one. */
int64_t hero_disk_set_link_owner(const char *path, int64_t user, int64_t group);

/* The kind of the last call's failure on this thread, `HERO_DISK_OK` after
 * a call that did not fail, and the platform's own number for it (0 where
 * the layer itself refused). */
int64_t hero_disk_last_error(void);
int64_t hero_disk_last_os_error(void);

/* -- ROUND 3: LINKS MADE, PERMISSION SET, WHAT THE FACTS DO NOT GIVE, AND A
 * FILE LOCKED (M-rebel-library, 2026-11-16) ---------------------------------
 *
 * Added for Rebel's `fs`. Everything above stands as it was, its names, its
 * signatures and what it answers.
 *
 * WHY A LINK, A SYMBOLIC LINK AND A PERMISSION HERE, BESIDE `hero_os.h`'s
 * `hero_fs_link`, `hero_fs_symlink` and `hero_fs_set_mode`: those answer done
 * or failed with no kind, and their Windows arms hand the path to the ANSI
 * calls, which read UTF-8 bytes in the machine's code page and name another
 * file. These take the path as every door above does.
 *
 * `through`, WHERE A DOOR TAKES IT, says what is asked about: 0 the link
 * itself where `path` names one, 1 what it names once every link is
 * followed, as `hero_disk_facts` and `hero_disk_facts_through` read; any
 * other number is `HERO_DISK_INVALID`. */

/* The kind of the failure of a lock door handed what is no open stream:
 * NULL, closed already, or never one (`hero_stream.h`'s own words). */
#define HERO_DISK_NOT_OPEN 2

/* What an OTHER is, as `hero_disk_other_kind` answers it. NONE where the
 * path names a file, a directory or a link, which are no other. */
#define HERO_DISK_OTHER_NONE 0
#define HERO_DISK_OTHER_PIPE 1
#define HERO_DISK_OTHER_SOCKET 2
#define HERO_DISK_OTHER_CHARACTER_DEVICE 3
#define HERO_DISK_OTHER_BLOCK_DEVICE 4
/* An other the system names and this list does not: Darwin's whiteout, a
 * Windows handle of a type the system calls unknown or remote. */
#define HERO_DISK_OTHER_UNKNOWN 5

/* Which other `path` names, one of the numbers above, or -1. POSIX tells all
 * four; Windows tells a pipe and a character device (`NUL`, a console) by
 * `GetFileType`, and calls a socket's file what `hero_disk_facts` calls it,
 * a file, so it answers neither SOCKET nor BLOCK_DEVICE. */
int64_t hero_disk_other_kind(const char *path, int64_t through);

/* The time what `path` names was made, into `*made` in nanoseconds since
 * 1970-01-01 UTC, as the facts' times are: 0, or -1 with nothing written.
 * Darwin keeps it in `stat` (`st_birthtimespec`), Linux in `statx`
 * (`stx_btime`) where the volume keeps one, Windows in a file's facts (its
 * creation time); `HERO_DISK_UNSUPPORTED` where the system or the volume
 * keeps none: a Linux whose kernel has no `statx`, a volume `statx` says
 * keeps no such time, a Windows time of 0, and any other POSIX system. */
int64_t hero_disk_made(const char *path, int64_t through, int64_t *made);

/* The numbers of the user and of the group that own what `path` names, into
 * `*user` and `*group`: 0, or -1 with nothing written. Windows names an
 * owner by a security identifier and has no such numbers:
 * `HERO_DISK_UNSUPPORTED` there, always, as `hero_disk_set_owner`. */
int64_t hero_disk_owner(const char *path, int64_t through, int64_t *user, int64_t *group);

/* Makes `name` a second name of the file `existing` names, a hard link:
 * 0, or -1 with `HERO_DISK_EXISTS` where anything is at `name`,
 * `HERO_DISK_NOT_FOUND` where `existing` or the folder of `name` is not
 * there, `HERO_DISK_IS_A_DIRECTORY` where `existing` is a directory (asked
 * first, since the systems refuse one with three numbers), and
 * `HERO_DISK_CROSS_DEVICE` across two volumes. A link `existing` names is
 * itself given the second name, never what it names (`linkat` with no
 * flag; `CreateHardLinkW`). */
int64_t hero_disk_link(const char *existing, const char *name);

/* Makes `name` a symbolic link holding `target`, 1 to `HERO_DISK_PATH_MOST`
 * bytes, which is not looked for: 0, or -1 with `HERO_DISK_EXISTS` where
 * anything is at `name` and `HERO_DISK_INVALID` for an empty target. Windows
 * makes one only for an administrator or, in developer mode, for anybody
 * (`SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE`), `HERO_DISK_DENIED`
 * elsewhere; and gives a link a kind when it is made, as `hero_os.h`'s
 * `hero_fs_symlink` does: every `/` of the target is written `\`, and the
 * link is a directory's where the target, read from the folder of `name`
 * unless it is absolute, names a directory then, else a file's. */
int64_t hero_disk_symlink(const char *target, const char *name);

/* Sets the permission bits of what `path` names, links followed, to
 * `permission`, 0 to 07777, no mask taking any away: 0, or -1. Windows
 * keeps one bit, read-only: set where `permission` grants nobody writing
 * (no bit of 0222), cleared where it grants somebody, so the facts then read
 * 0444 or 0666 back. */
int64_t hero_disk_set_permission(const char *path, int64_t permission);

/* The same of the link itself where `path` names one, and of what it names
 * where it names no link. Darwin keeps a link's own bits; Linux keeps none,
 * `HERO_DISK_UNSUPPORTED` for a link there; Windows the link's own
 * read-only mark. */
int64_t hero_disk_set_link_permission(const char *path, int64_t permission);

/* Sets the times of the last write and the last read of the link itself
 * where `path` names one, else of what it names, as `hero_disk_set_times`
 * does (`utimensat` with `AT_SYMLINK_NOFOLLOW`; the link opened itself on
 * Windows). */
int64_t hero_disk_set_link_times(const char *path, int64_t written, int64_t read);

/* A LOCK ON A WHOLE FILE, held through an open stream (`hero_stream.h`):
 * `flock` on POSIX, `LockFileEx` over every byte on Windows.
 *
 * `how` is SHARED, which other shared locks stand beside, or EXCLUSIVE,
 * which nothing stands beside; `wait` 1 waits until the lock is given and 0
 * answers at once, `HERO_DISK_BUSY` where another holds a lock that excludes
 * it. Answers 0, or -1 with `HERO_DISK_NOT_OPEN` where `s` is no open stream,
 * `HERO_DISK_UNSUPPORTED` where it stands on no descriptor (one another
 * member made, over a socket), and `HERO_DISK_INVALID` for a `how` or a
 * `wait` that is none of these.
 *
 * WHO IS EXCLUDED: every other stream on the file, of this process as of
 * another, since a lock is the stream's own (POSIX's open file, Windows'
 * handle). A second lock on the same stream replaces the first, which is
 * let go before the second is asked, so another may take the file between
 * the two, on every platform (flock(2) says so of POSIX's). It ends at
 * `hero_disk_unlock` or when the stream closes.
 *
 * ADVISORY ON POSIX, ENFORCED ON WINDOWS: on POSIX a lock excludes only
 * another lock, never a read or a write; on Windows, while a lock is held,
 * another stream's read of the file fails where it is exclusive, and a write
 * fails through every stream but the one holding an exclusive lock: a shared
 * lock refuses a write through its own stream too (LockFileEx's page; the
 * last two unrun until the box runs them).
 *
 * A WAIT ENDS ONLY WHEN THE LOCK IS GIVEN: no deadline and no stop reach it,
 * and a signal that interrupts it is waited through. A program that cannot
 * wait asks with `wait` 0, or waits on a thread of its own. */
#define HERO_DISK_LOCK_SHARED 1
#define HERO_DISK_LOCK_EXCLUSIVE 2
int64_t hero_disk_lock(hero_stream *s, int64_t how, int64_t wait);

/* Lets go the lock `s` holds: 0, also where it holds none, or -1 with the
 * kinds of `hero_disk_lock`. */
int64_t hero_disk_unlock(hero_stream *s);

#endif
