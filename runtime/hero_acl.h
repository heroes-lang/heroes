/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_acl.h — the platform layer's access control lists (M-rebel-library,
 * gap G06; spec § 13): who may do what to a file or a folder beyond its
 * permission bits, read and written whole, so a backup restores a folder
 * with who may read it, and a program grants one user or group a right.
 *
 * THREE SYSTEMS, ONE FORM. Windows' DACL and Darwin's extended ACL are lists
 * of entries that allow or deny rights to a user or a group, with
 * inheritance flags; Darwin's rights and flags are Windows' one to one, so
 * an entry's rights and flags here are Windows' own numbers (`winnt.h`), and
 * Darwin's are carried to and from them. Linux's are POSIX.1e's draft lists,
 * as the kernel keeps them in `system.posix_acl_access` and
 * `system.posix_acl_default` (`linux/posix_acl_xattr.h`): entries that only
 * allow read, write and execute, to the owner, a user, the owning group, a
 * group, the mask and everyone else; read is `HERO_ACL_READ_DATA`, write
 * `HERO_ACL_WRITE_DATA` and execute `HERO_ACL_EXECUTE`, and a folder's
 * default entries carry `HERO_ACL_LINUX_DEFAULT`. No library is linked: the
 * kernel's own form is read and written, so a program needs no libacl.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_procs.h`
 * is: its functions are defined in `platform/hero_acl.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_acl.h"`, with no `link` word and no flag. On Windows the
 * member, and only a program that takes it, links `advapi32`.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function answers -1 and
 * `hero_acl_last_error()` then names the kind.
 *
 * NOTHING IS KEPT BETWEEN CALLS but the last error, which is per thread. A
 * path and a name are UTF-8 on Windows, any bytes but zero on POSIX.
 */

#ifndef HERO_ACL_H
#define HERO_ACL_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform, and the numbers
 * `hero_disk.h` gives the kinds of the same names. */
#define HERO_ACL_OK 0
/* An argument the layer refuses: a list that is not this header's form (a
 * field with no zero byte after it, a number that is not one, whom an entry
 * names out of range, an id the system cannot read as one), a list the
 * system refuses as a whole (a Linux list missing its owner, owning group or
 * everyone-else entry, or naming one twice), or a count with no buffer
 * behind it. */
#define HERO_ACL_INVALID 1
/* The path names nothing. */
#define HERO_ACL_NOT_FOUND 3
/* The system refuses this to the program: only the owner, or a user the
 * list lets, may change a list. */
#define HERO_ACL_DENIED 5
/* A part of the path on its way is not a folder. */
#define HERO_ACL_NOT_A_DIRECTORY 6
#define HERO_ACL_LIMIT 10
/* The volume keeps no access list (a FAT drive; a Linux file system mounted
 * without them, written anything but the permission bits), or the system
 * has none: a POSIX system neither Darwin nor Linux. */
#define HERO_ACL_UNSUPPORTED 11
#define HERO_ACL_OTHER 12
/* On Windows, a path or a name that is not UTF-8. */
#define HERO_ACL_NOT_TEXT 13
/* A user or a group the system does not know, by its name or its id. */
#define HERO_ACL_UNKNOWN 16
/* An entry this system's lists cannot hold, `hero_acl_last_os_error` naming
 * which, from 1, and the reason one of the `HERO_ACL_CANNOT_` numbers
 * below in `hero_acl_last_reason`. */
#define HERO_ACL_CANNOT 17

/* Why an entry cannot be held, `hero_acl_last_reason`'s answer after
 * `HERO_ACL_CANNOT`, or after a read of a list that holds such an entry. */
/* Darwin and Windows name a user or a group alone: no owner, owning group,
 * mask or everyone-else entry. */
#define HERO_ACL_CANNOT_WHO 1
/* Linux's entries only allow. */
#define HERO_ACL_CANNOT_DENY 2
/* A right this system's entries have no place for: on Linux any but read,
 * write and execute; on Darwin a generic right or one past synchronize. */
#define HERO_ACL_CANNOT_RIGHT 3
/* A flag this system's entries have no place for: on Linux any but the
 * default entries' whole set. */
#define HERO_ACL_CANNOT_FLAG 4
/* A list marked protected from its parent's inheritance, which Linux has no
 * place for: its lists are inherited only when a file is made. */
#define HERO_ACL_CANNOT_PROTECT 5
/* A Linux folder's default entries on what is not a folder. */
#define HERO_ACL_CANNOT_DEFAULT 6
/* Read: an entry of a kind not read here (Windows' object, callback and
 * audit entries), or a DACL that is null, which grants everyone everything. */
#define HERO_ACL_CANNOT_READ 7

/* Whom an entry names: field 0 of an entry. */
#define HERO_ACL_USER 1
#define HERO_ACL_GROUP 2
/* POSIX's own three and its mask, on Linux alone: the file's owner, its
 * owning group, everyone else, and the most a user or group entry grants. */
#define HERO_ACL_OWNER 3
#define HERO_ACL_OWNING_GROUP 4
#define HERO_ACL_OTHERS 5
#define HERO_ACL_MASK 6

/* The rights, Windows' numbers for them (`winnt.h`); the name after each is
 * Darwin's (`sys/acl.h`). */
#define HERO_ACL_READ_DATA 0x1             /* ACL_READ_DATA, ACL_LIST_DIRECTORY */
#define HERO_ACL_WRITE_DATA 0x2            /* ACL_WRITE_DATA, ACL_ADD_FILE */
#define HERO_ACL_APPEND_DATA 0x4           /* ACL_APPEND_DATA, ACL_ADD_SUBDIRECTORY */
#define HERO_ACL_READ_EXTENDED 0x8         /* ACL_READ_EXTATTRIBUTES */
#define HERO_ACL_WRITE_EXTENDED 0x10       /* ACL_WRITE_EXTATTRIBUTES */
#define HERO_ACL_EXECUTE 0x20              /* ACL_EXECUTE, ACL_SEARCH */
#define HERO_ACL_DELETE_CHILD 0x40         /* ACL_DELETE_CHILD */
#define HERO_ACL_READ_ATTRIBUTES 0x80      /* ACL_READ_ATTRIBUTES */
#define HERO_ACL_WRITE_ATTRIBUTES 0x100    /* ACL_WRITE_ATTRIBUTES */
#define HERO_ACL_DELETE 0x10000            /* ACL_DELETE */
#define HERO_ACL_READ_SECURITY 0x20000     /* ACL_READ_SECURITY */
#define HERO_ACL_WRITE_SECURITY 0x40000    /* ACL_WRITE_SECURITY */
#define HERO_ACL_CHANGE_OWNER 0x80000      /* ACL_CHANGE_OWNER */
#define HERO_ACL_SYNCHRONIZE 0x100000      /* ACL_SYNCHRONIZE */
#define HERO_ACL_GENERIC_ALL 0x10000000    /* Windows alone */
#define HERO_ACL_GENERIC_EXECUTE 0x20000000
#define HERO_ACL_GENERIC_WRITE 0x40000000
#define HERO_ACL_GENERIC_READ 0x80000000

/* The flags, Windows' numbers for them; Darwin's after each. */
#define HERO_ACL_FILE_INHERIT 0x1   /* ACL_ENTRY_FILE_INHERIT; OBJECT_INHERIT_ACE */
#define HERO_ACL_FOLDER_INHERIT 0x2 /* ACL_ENTRY_DIRECTORY_INHERIT; CONTAINER_INHERIT_ACE */
#define HERO_ACL_LIMIT_INHERIT 0x4  /* ACL_ENTRY_LIMIT_INHERIT; NO_PROPAGATE_INHERIT_ACE */
#define HERO_ACL_ONLY_INHERIT 0x8   /* ACL_ENTRY_ONLY_INHERIT; INHERIT_ONLY_ACE */
#define HERO_ACL_INHERITED 0x10     /* ACL_ENTRY_INHERITED; INHERITED_ACE */
/* A Linux folder's default entries, which every file and folder made in it
 * takes, and which do not govern the folder itself. */
#define HERO_ACL_LINUX_DEFAULT (HERO_ACL_FILE_INHERIT | HERO_ACL_FOLDER_INHERIT | HERO_ACL_ONLY_INHERIT)

/* THE LIST'S FORM, read and written: fields of text, each followed by one
 * zero byte. First one field, `1` where the list is protected from its
 * parent's inheritance (Windows' `SE_DACL_PROTECTED`, Darwin's
 * `ACL_FLAG_NO_INHERIT`), else `0`; then for each entry, in the list's order,
 * `HERO_ACL_FIELDS` fields:
 *
 *   0 whom it names, `HERO_ACL_USER` to `HERO_ACL_MASK`, in decimal digits;
 *   1 `1` where it denies, `0` where it allows;
 *   2 its rights, in decimal digits;
 *   3 its flags, in decimal digits;
 *   4 the user's or group's id, as the system writes it: on Windows a SID
 *     (`S-1-5-32-544`), on Darwin a UUID in upper case
 *     (`ABCDEFAB-CDEF-ABCD-EFAB-CDEF0000000C`), on Linux the user's or the
 *     group's number; empty for the owner, the owning group, everyone else
 *     and the mask;
 *   5 its name, as the system gives it (`BUILTIN\Administrators`,
 *     `everyone`, `staff`), empty where the system knows none; never read
 *     by a write.
 *
 * On Linux a list read holds its access entries in the kernel's order
 * (owner, users by number, owning group, groups by number, mask, everyone
 * else), then, for a folder, its default entries in the same order; one
 * written is put in that order, and where it names a user or a group and
 * has no mask, a mask is made of every right a user, group or owning group
 * entry grants, as `setfacl` makes one. On Darwin and Windows the order is
 * the system's and is kept, and Windows reads its entries in that order, so
 * a deny meant to win goes before the allow it overrides. */
#define HERO_ACL_FIELDS 6

/* `path`'s list by the fill form, as `hero_sys.h` says it: up to `count`
 * bytes written into `buffer`, the answer how many bytes the WHOLE list has;
 * where that is above `count` the buffer was too short, and the caller asks
 * again with at least that many (a count of 0 only measures, and `buffer`
 * may then be NULL). A link at the path's end is followed where `follow` is
 * not 0, and asked itself where it is 0.
 *
 * Where a file has no list, Darwin's answer is no entry; Linux's is the
 * three entries its permission bits are. A Windows list holding an entry of
 * a kind not read here, or a null DACL, is `HERO_ACL_CANNOT` with
 * `HERO_ACL_CANNOT_READ`, never a list with an entry left out. */
int64_t hero_acl_read(const char *path, int64_t follow, void *buffer, uint64_t count);

/* `path`'s list replaced by the `size` bytes of `list`, in the form above.
 * On Windows a list not protected keeps the entries its parent hands down:
 * the entries given that carry `HERO_ACL_INHERITED` are left to the system,
 * which writes its parent's again; a protected one is written whole, as
 * given. On Darwin the list is written whole, as given. On Linux the access
 * entries replace the access list, and a folder's default entries its
 * default list, removed where none is given; a volume without access lists
 * takes a list of the three entries its permission bits are as those bits.
 * 0, or -1. */
int64_t hero_acl_write(const char *path, int64_t follow, const void *list, uint64_t size);

/* The id of the user, or where `group` is not 0 the group, the system knows
 * by `name`, by the fill form, its bytes with no zero after them:
 * `LookupAccountNameW` on Windows (a SID), `mbr_identifier_to_uuid` on Darwin
 * (a UUID), `getpwnam_r` and
 * `getgrnam_r` on Linux (a number). `HERO_ACL_UNKNOWN` where it knows none. */
int64_t hero_acl_id_of(const char *name, int64_t group, void *buffer, uint64_t count);

/* The kind of the last call's failure on this thread, `HERO_ACL_OK` after a
 * call that did not fail; the platform's own number for it (0 where the
 * layer itself refused): `errno` on POSIX, `GetLastError` on Windows, and
 * after `HERO_ACL_CANNOT` the entry's place, from 1 (0 for the list's own
 * protection); and after `HERO_ACL_CANNOT` the reason, a `HERO_ACL_CANNOT_`
 * number, 0 after any other. */
int64_t hero_acl_last_error(void);
int64_t hero_acl_last_os_error(void);
int64_t hero_acl_last_reason(void);

#endif
