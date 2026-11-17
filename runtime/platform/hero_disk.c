/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_disk.c — the platform layer's doors on the file system that
 * are reached by path, one member of the archive the compiler puts on every
 * program's link line (M-rebel-library; `selfhost/cli/layer.hero` builds,
 * keys and links it). Its header, `hero_disk.h`, says what each function
 * answers; this file says how each platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * NOTHING IS KEPT BETWEEN CALLS BUT THE LAST FAILURE, WHICH IS EACH
 * THREAD'S OWN. There is no handle here, so no table and no lock: a scan
 * opens its directory, fills the caller's buffer and closes it before it
 * answers, and a link's target is read into the caller's buffer. What the
 * process shares is the system's, and said where it is met: the working
 * folder `hero_disk_enter` changes, and the environment `TMPDIR` is read
 * from, which C lets no thread read while another writes it.
 *
 * NO ALLOCATION (the runtime allocates in `parts/alloc.c` alone, the same
 * suite's rule 1): every buffer is on the caller's stack and of a fixed
 * size, which is why a path has a longest length, `HERO_DISK_PATH_MOST`,
 * the same on every platform, refused before any system call. The most a
 * door holds is two paths as Windows takes them and one reparse buffer,
 * under 40 KB.
 *
 * WHAT A FAILURE IS: the system's number, read at once after the call that
 * failed, and the kind it maps to in `hero_disk_kind_of`, one table with an
 * arm per platform. Where two platforms' numbers would name one event two
 * ways, the door looks instead of trusting the number, as `hero_os.h`'s
 * comment says of `mkdir`: `hero_disk_remove` asks what is there before it
 * chooses `unlink` or `rmdir`, since `unlink` on a directory is `EPERM` on
 * Darwin, `EISDIR` on Linux and, for the super-user on some systems, a
 * directory unlinked; `hero_disk_rename` asks what both names are, since
 * POSIX replaces an empty directory and Windows none; `hero_disk_scan` on
 * Windows asks whether the path is a directory where the search would not
 * start, since a file and a missing folder fail it alike.
 *
 * WINDOWS' PATHS ARE UTF-16, AND A NAME IS CARRIED BOTH WAYS WHOLE. In:
 * UTF-8 by this file's own reader, strict (no overlong form, nothing past
 * U+10FFFF), with one widening, the runtime's own carriage of a listing's
 * name (`parts/codepage.c`): a surrogate standing alone as the three bytes
 * its value would take, which is how such a name comes OUT of
 * `hero_disk_scan`, so a file the system can list is a file the layer can
 * be handed back; a pair spelled as two such halves is refused, being no
 * listing's. Anything else is `HERO_DISK_NOT_TEXT`. `MultiByteToWideChar`
 * is not asked: with its strict flag it refuses those three bytes, and
 * without it, it replaces what it cannot read. Every call is the wide one,
 * `...W`. A path is handed over as written, so one longer than `MAX_PATH`
 * is the system's to refuse unless the process is made long-path aware:
 * this file adds no `\\?\`.
 *
 * WHAT IS UNRUN: the `_WIN32` arm was compiled on no Windows machine by its
 * writer (2026-11-15); its calls are `CreateFileW`,
 * `GetFileInformationByHandle`, `GetFileInformationByHandleEx`,
 * `GetFileType`, `GetFileAttributesW`, `CreateDirectoryW`, `DeleteFileW`,
 * `RemoveDirectoryW`, `MoveFileExW`, `FindFirstFileW`, `FindNextFileW`,
 * `FindClose`, `DeviceIoControl` with `FSCTL_GET_REPARSE_POINT`,
 * `SetFileTime`, `SetFilePointerEx`, `SetEndOfFile`, `SetCurrentDirectoryW`,
 * `GetTempPathW`, `GetVolumePathNameW`, `GetDiskFreeSpaceExW`,
 * `CloseHandle` and `GetLastError`. (It ran on the box on 2026-11-15:
 * `rebel-scratch/windows/hero_disk.md`.)
 *
 * ROUND 3 (2026-11-16) adds, on Windows, `GetFileType` after
 * `SetLastError`, `GetFileInformationByHandle`'s creation time,
 * `CreateHardLinkW`, `CreateSymbolicLinkW`, `GetFileInformationByHandleEx`
 * and `SetFileInformationByHandle` with `FileBasicInfo`, `SetFileTime` on a
 * link opened itself, and `LockFileEx` and `UnlockFileEx` on a stream's
 * `HANDLE` held through `hero_stream_hold`; its arm ran on the box the same
 * day (`rebel-scratch/windows/c-round3.md`).
 */

#include "hero_disk.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <winioctl.h>
#include <wchar.h>
#else
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#if defined(__APPLE__)
/* Darwin's `statvfs` counts a volume's blocks in 32 bits; its `statfs`
 * counts them in 64. */
#include <sys/mount.h>
#include <sys/param.h>
#else
#include <sys/statvfs.h>
#endif
#if defined(__linux__)
/* `statx`, for the time a file was made (round 3): asked by its number, the
 * C library declaring its wrapper only under `_GNU_SOURCE`, which a member
 * cannot define above its own header; the record and its mask are the
 * kernel's own header's. */
#include <linux/stat.h>
#include <sys/syscall.h>
#endif
#endif

static _Thread_local int64_t hero_disk_kind;
static _Thread_local int64_t hero_disk_os;

static void hero_disk_fail(int64_t kind, int64_t os) {
    hero_disk_kind = kind;
    hero_disk_os = os;
}

static void hero_disk_clear(void) {
    hero_disk_fail(HERO_DISK_OK, 0);
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_disk_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:
    case ERROR_BAD_NETPATH:
    case ERROR_BAD_NET_NAME:
        return HERO_DISK_NOT_FOUND;
    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
        return HERO_DISK_EXISTS;
    case ERROR_ACCESS_DENIED:
    case ERROR_WRITE_PROTECT:
    case ERROR_PRIVILEGE_NOT_HELD:
        return HERO_DISK_DENIED;
    case ERROR_DIRECTORY:
        return HERO_DISK_NOT_A_DIRECTORY;
    case ERROR_DIR_NOT_EMPTY:
        return HERO_DISK_NOT_EMPTY;
    case ERROR_DISK_FULL:
    case ERROR_HANDLE_DISK_FULL:
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_TOO_MANY_LINKS:
        return HERO_DISK_LIMIT;
    case ERROR_NOT_SAME_DEVICE:
        return HERO_DISK_CROSS_DEVICE;
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
    case ERROR_BUSY:
        return HERO_DISK_BUSY;
    case ERROR_INVALID_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_INVALID_PARAMETER:
    case ERROR_NOT_A_REPARSE_POINT:
        return HERO_DISK_INVALID;
    case ERROR_NO_UNICODE_TRANSLATION:
        return HERO_DISK_NOT_TEXT;
    case ERROR_NOT_SUPPORTED:
    case ERROR_CALL_NOT_IMPLEMENTED:
    case ERROR_INVALID_FUNCTION:
        return HERO_DISK_UNSUPPORTED;
#else
    case ENOENT:
        return HERO_DISK_NOT_FOUND;
    case EEXIST:
        return HERO_DISK_EXISTS;
    case EACCES:
    case EPERM:
    case EROFS:
        return HERO_DISK_DENIED;
    case ENOTDIR:
        return HERO_DISK_NOT_A_DIRECTORY;
    case EISDIR:
        return HERO_DISK_IS_A_DIRECTORY;
    case ENOTEMPTY:
        return HERO_DISK_NOT_EMPTY;
    case ENOSPC:
    case EDQUOT:
    case ENOMEM:
    case EMFILE:
    case ENFILE:
    case EMLINK:
    case EFBIG:
        return HERO_DISK_LIMIT;
    case EXDEV:
        return HERO_DISK_CROSS_DEVICE;
    case EBUSY:
    case ETXTBSY:
        return HERO_DISK_BUSY;
    case EINVAL:
    case ENAMETOOLONG:
        return HERO_DISK_INVALID;
    case ENOTSUP:
    case ENOSYS:
        return HERO_DISK_UNSUPPORTED;
#endif
    default:
        return HERO_DISK_OTHER;
    }
}

/* The failure a platform's number is, as the -1 its door answers. */
static int64_t hero_disk_failed_with(int64_t code) {
    hero_disk_fail(hero_disk_kind_of(code), code);
    return -1;
}

/* The failure the last system call reported. */
static int64_t hero_disk_failed(void) {
#if defined(_WIN32)
    return hero_disk_failed_with((int64_t)GetLastError());
#else
    return hero_disk_failed_with((int64_t)errno);
#endif
}

/* The failure the layer itself finds, with no number of the system's. */
static int64_t hero_disk_refused(int64_t kind) {
    hero_disk_fail(kind, 0);
    return -1;
}

/* Whether `path` is one a door takes: there, not empty, and at most
 * `HERO_DISK_PATH_MOST` bytes, read no further than that. */
static bool hero_disk_path_ok(const char *path) {
    size_t length = 0;

    if (path == NULL) {
        hero_disk_fail(HERO_DISK_INVALID, 0);
        return false;
    }
    while (length <= HERO_DISK_PATH_MOST && path[length] != '\0') {
        length += 1;
    }
    if (length == 0) {
        hero_disk_fail(HERO_DISK_NOT_FOUND, 0);
        return false;
    }
    if (length > HERO_DISK_PATH_MOST) {
        hero_disk_fail(HERO_DISK_INVALID, 0);
        return false;
    }
    return true;
}

/* Whether a count has a buffer behind it. */
static bool hero_disk_room_ok(const void *buffer, uint64_t count) {
    if (buffer == NULL && count > 0) {
        hero_disk_fail(HERO_DISK_INVALID, 0);
        return false;
    }
    return true;
}

/* The first `count` facts to the caller, and how many there are. */
static int64_t hero_disk_give(const int64_t *all, int64_t *facts, uint64_t count) {
    uint64_t at;

    for (at = 0; at < count && at < HERO_DISK_FACTS; at += 1) {
        facts[at] = all[at];
    }
    return HERO_DISK_FACTS;
}

/* A whole answer of `length` bytes into the caller's buffer where it fits,
 * and its length either way. */
static int64_t hero_disk_fill(void *buffer, uint64_t count, const char *bytes, size_t length) {
    if (length > 0 && (uint64_t)length <= count) {
        memcpy(buffer, bytes, length);
    }
    return (int64_t)length;
}

/* One entry into a listing that needs `needed` bytes so far: its kind, its
 * name's bytes and a zero, where the whole of it still fits, and the bytes
 * the listing needs with it either way. Once an entry did not fit, none
 * after it is written, so the buffer holds whole entries alone. */
static uint64_t hero_disk_put(void *buffer, uint64_t count, uint64_t needed, int64_t kind, const char *name,
                              size_t length) {
    unsigned char *out = buffer;
    uint64_t entry = (uint64_t)length + 2;

    if (needed <= count && entry <= count - needed) {
        out[needed] = (unsigned char)kind;
        memcpy(out + needed + 1, name, length);
        out[needed + 1 + length] = 0;
    }
    return needed + entry;
}

#if defined(_WIN32)

/* A path as the wide API takes it, with a listing's `\*` and its zero. */
#define HERO_DISK_WIDE (HERO_DISK_PATH_MOST + 4)
/* A name is asked about while others read, write, rename or remove it. */
#define HERO_DISK_SHARE (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE)
/* 1970-01-01 in Windows' count of 100 ns since 1601-01-01. */
#define HERO_DISK_EPOCH INT64_C(116444736000000000)

/* The character that begins at `s[*at]`, `*at` moved past it, or -1 where
 * no well-formed UTF-8 sequence begins there: a continuation alone, an
 * overlong form, a value past U+10FFFF, a sequence cut short. A surrogate's
 * three bytes are read as its value, for `hero_disk_wide` to judge. The
 * path's own zero is no continuation, so no read passes it. */
static int32_t hero_disk_char_at(const unsigned char *s, size_t *at) {
    unsigned char lead = s[*at];
    uint32_t c;
    uint32_t least;
    int length;
    int k;

    if (lead < 0x80) {
        *at += 1;
        return (int32_t)lead;
    }
    if (lead >= 0xC2 && lead <= 0xDF) {
        length = 2;
        c = lead & 0x1Fu;
        least = 0x80u;
    } else if (lead >= 0xE0 && lead <= 0xEF) {
        length = 3;
        c = lead & 0x0Fu;
        least = 0x800u;
    } else if (lead >= 0xF0 && lead <= 0xF4) {
        length = 4;
        c = lead & 0x07u;
        least = 0x10000u;
    } else {
        return -1;
    }
    for (k = 1; k < length; k += 1) {
        unsigned char next = s[*at + (size_t)k];

        if ((next & 0xC0u) != 0x80u) {
            return -1;
        }
        c = (c << 6) | (next & 0x3Fu);
    }
    if (c < least || c > 0x10FFFFu) {
        return -1;
    }
    *at += (size_t)length;
    return (int32_t)c;
}

/* `path` into `out`, `HERO_DISK_WIDE` units, as the wide API takes it, with
 * `suffix` after it; false with the failure set where it is no path or its
 * bytes are no text (the opening comment says which bytes are). A path of
 * n bytes is at most n units, so it fits with a suffix of two. */
static bool hero_disk_wide(const char *path, const wchar_t *suffix, wchar_t *out) {
    const unsigned char *s = (const unsigned char *)path;
    size_t at = 0;
    size_t put = 0;
    bool after_high = false;

    if (!hero_disk_path_ok(path)) {
        return false;
    }
    while (s[at] != 0) {
        int32_t c = hero_disk_char_at(s, &at);

        if (c < 0 || (after_high && c >= 0xDC00 && c <= 0xDFFF)) {
            hero_disk_fail(HERO_DISK_NOT_TEXT, 0);
            return false;
        }
        after_high = c >= 0xD800 && c <= 0xDBFF;
        if (c >= 0x10000) {
            out[put++] = (wchar_t)(0xD800 + ((c - 0x10000) >> 10));
            out[put++] = (wchar_t)(0xDC00 + ((c - 0x10000) & 0x3FF));
        } else {
            out[put++] = (wchar_t)c;
        }
    }
    while (suffix != NULL && *suffix != 0) {
        out[put++] = *suffix++;
    }
    out[put] = 0;
    return true;
}

/* `units` of a wide name as bytes into `out`: UTF-8, and a surrogate
 * standing alone as the three bytes its value would take, which are not
 * UTF-8 and so are told by the caller's check of text. How many bytes, or
 * -1 where `room` is short. */
static int64_t hero_disk_narrow(const wchar_t *wide, size_t units, char *out, size_t room) {
    size_t at = 0;
    size_t i;

    for (i = 0; i < units; i += 1) {
        uint32_t c = (uint32_t)wide[i];

        if (c >= 0xD800u && c <= 0xDBFFu && i + 1 < units && (uint32_t)wide[i + 1] >= 0xDC00u &&
            (uint32_t)wide[i + 1] <= 0xDFFFu) {
            c = 0x10000u + ((c - 0xD800u) << 10) + ((uint32_t)wide[i + 1] - 0xDC00u);
            i += 1;
        }
        if (room - at < 4) {
            return -1;
        }
        if (c < 0x80u) {
            out[at++] = (char)c;
        } else if (c < 0x800u) {
            out[at++] = (char)(0xC0u | (c >> 6));
            out[at++] = (char)(0x80u | (c & 0x3Fu));
        } else if (c < 0x10000u) {
            out[at++] = (char)(0xE0u | (c >> 12));
            out[at++] = (char)(0x80u | ((c >> 6) & 0x3Fu));
            out[at++] = (char)(0x80u | (c & 0x3Fu));
        } else {
            out[at++] = (char)(0xF0u | (c >> 18));
            out[at++] = (char)(0x80u | ((c >> 12) & 0x3Fu));
            out[at++] = (char)(0x80u | ((c >> 6) & 0x3Fu));
            out[at++] = (char)(0x80u | (c & 0x3Fu));
        }
    }
    return (int64_t)at;
}

/* Whether a reparse point of this tag is a link: a symbolic link or a
 * junction. Any other, a cloud file's placeholder among them, IS the file. */
static bool hero_disk_link_tag(DWORD tag) {
    return tag == IO_REPARSE_TAG_SYMLINK || tag == IO_REPARSE_TAG_MOUNT_POINT;
}

/* What `wide` names, opened to be asked about and never read: a directory
 * too (`FILE_FLAG_BACKUP_SEMANTICS`), and the link itself, not what it
 * names, unless `follow` (`FILE_FLAG_OPEN_REPARSE_POINT`). */
static HANDLE hero_disk_open(const wchar_t *wide, DWORD access, bool follow) {
    DWORD flags = FILE_FLAG_BACKUP_SEMANTICS;

    if (!follow) {
        flags |= FILE_FLAG_OPEN_REPARSE_POINT;
    }
    return CreateFileW(wide, access, HERO_DISK_SHARE, NULL, OPEN_EXISTING, flags, NULL);
}

/* The failure the last system call reported, its handle closed after the
 * number is read. */
static int64_t hero_disk_failed_closing(HANDLE h) {
    DWORD code = GetLastError();

    (void)CloseHandle(h);
    return hero_disk_failed_with((int64_t)code);
}

/* A time of the system's as nanoseconds since 1970, or the nearest that 64
 * bits of them hold. */
static int64_t hero_disk_ns_of(FILETIME t) {
    uint64_t ticks = ((uint64_t)t.dwHighDateTime << 32) | (uint64_t)t.dwLowDateTime;
    int64_t ns = 0;

    if (ticks > (uint64_t)INT64_MAX) {
        return INT64_MAX;
    }
    if (__builtin_mul_overflow((int64_t)ticks - HERO_DISK_EPOCH, INT64_C(100), &ns)) {
        return (int64_t)ticks < HERO_DISK_EPOCH ? INT64_MIN : INT64_MAX;
    }
    return ns;
}

/* Nanoseconds since 1970 as a time of the system's, rounded down to its
 * 100 ns; false at or before 1601-01-01, where the system keeps none (0
 * there asks `SetFileTime` to leave the time as it is). */
static bool hero_disk_filetime(int64_t ns, FILETIME *out) {
    int64_t ticks = ns / 100;

    if (ns % 100 < 0) {
        ticks -= 1;
    }
    ticks += HERO_DISK_EPOCH;
    if (ticks <= 0) {
        return false;
    }
    out->dwLowDateTime = (DWORD)((uint64_t)ticks & 0xFFFFFFFFu);
    out->dwHighDateTime = (DWORD)((uint64_t)ticks >> 32);
    return true;
}

static int64_t hero_disk_facts_of(const char *path, bool follow, int64_t *facts, uint64_t count) {
    wchar_t wide[HERO_DISK_WIDE];
    BY_HANDLE_FILE_INFORMATION info;
    FILE_ATTRIBUTE_TAG_INFO tag;
    int64_t all[HERO_DISK_FACTS];
    uint64_t size;
    bool link = false;
    HANDLE h;

    hero_disk_clear();
    if (!hero_disk_room_ok(facts, count) || !hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    h = hero_disk_open(wide, FILE_READ_ATTRIBUTES, follow);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    if (!GetFileInformationByHandle(h, &info)) {
        return hero_disk_failed_closing(h);
    }
    if (!follow && (info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
        if (!GetFileInformationByHandleEx(h, FileAttributeTagInfo, &tag, sizeof tag)) {
            return hero_disk_failed_closing(h);
        }
        link = hero_disk_link_tag(tag.ReparseTag);
    }
    if (link) {
        all[HERO_DISK_FACT_KIND] = HERO_DISK_KIND_LINK;
    } else if ((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        all[HERO_DISK_FACT_KIND] = HERO_DISK_KIND_DIRECTORY;
    } else if (GetFileType(h) == FILE_TYPE_DISK) {
        all[HERO_DISK_FACT_KIND] = HERO_DISK_KIND_FILE;
    } else {
        all[HERO_DISK_FACT_KIND] = HERO_DISK_KIND_OTHER;
    }
    (void)CloseHandle(h);
    size = ((uint64_t)info.nFileSizeHigh << 32) | (uint64_t)info.nFileSizeLow;
    all[HERO_DISK_FACT_SIZE] = size > (uint64_t)INT64_MAX ? INT64_MAX : (int64_t)size;
    all[HERO_DISK_FACT_PERMISSION] = (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0 ? 0444 : 0666;
    all[HERO_DISK_FACT_WRITTEN] = hero_disk_ns_of(info.ftLastWriteTime);
    all[HERO_DISK_FACT_READ] = hero_disk_ns_of(info.ftLastAccessTime);
    all[HERO_DISK_FACT_LINKS] = (int64_t)info.nNumberOfLinks;
    return hero_disk_give(all, facts, count);
}

/* 1 where the two name one file, 0 where two, -1 with the failure set: one
 * volume's serial number and one file index, each asked while both are
 * held open, since an index names its file only while something holds it. */
static int64_t hero_disk_same_wide(const wchar_t *first, const wchar_t *second, bool follow) {
    BY_HANDLE_FILE_INFORMATION a;
    BY_HANDLE_FILE_INFORMATION b;
    HANDLE ha;
    HANDLE hb;
    BOOL asked;
    DWORD code;

    ha = hero_disk_open(first, FILE_READ_ATTRIBUTES, follow);
    if (ha == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    hb = hero_disk_open(second, FILE_READ_ATTRIBUTES, follow);
    if (hb == INVALID_HANDLE_VALUE) {
        return hero_disk_failed_closing(ha);
    }
    asked = GetFileInformationByHandle(ha, &a) && GetFileInformationByHandle(hb, &b);
    code = GetLastError();
    (void)CloseHandle(ha);
    (void)CloseHandle(hb);
    if (!asked) {
        return hero_disk_failed_with((int64_t)code);
    }
    return a.dwVolumeSerialNumber == b.dwVolumeSerialNumber && a.nFileIndexHigh == b.nFileIndexHigh &&
                   a.nFileIndexLow == b.nFileIndexLow
               ? 1
               : 0;
}

int64_t hero_disk_same(const char *first, const char *second) {
    wchar_t one[HERO_DISK_WIDE];
    wchar_t other[HERO_DISK_WIDE];

    hero_disk_clear();
    if (!hero_disk_wide(first, NULL, one) || !hero_disk_wide(second, NULL, other)) {
        return -1;
    }
    return hero_disk_same_wide(one, other, true);
}

int64_t hero_disk_make_folder(const char *path, int64_t permission) {
    wchar_t wide[HERO_DISK_WIDE];

    hero_disk_clear();
    if (permission < 0 || permission > 07777) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    if (!CreateDirectoryW(wide, NULL)) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_remove(const char *path) {
    wchar_t wide[HERO_DISK_WIDE];
    DWORD attributes;
    BOOL removed;

    hero_disk_clear();
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    /* The name's own attributes, a link's and not its target's: a link made
     * to a directory is removed as a directory is, and its target stays. */
    attributes = GetFileAttributesW(wide);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return hero_disk_failed();
    }
    removed = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? RemoveDirectoryW(wide) : DeleteFileW(wide);
    if (!removed) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_rename(const char *from, const char *to) {
    wchar_t source[HERO_DISK_WIDE];
    wchar_t target[HERO_DISK_WIDE];
    DWORD was;
    DWORD there;
    bool directory;

    hero_disk_clear();
    if (!hero_disk_wide(from, NULL, source) || !hero_disk_wide(to, NULL, target)) {
        return -1;
    }
    was = GetFileAttributesW(source);
    if (was == INVALID_FILE_ATTRIBUTES) {
        return hero_disk_failed();
    }
    /* The same spelling twice is the rename POSIX defines as done with
     * nothing moved; no page says what `MoveFileExW` answers for it, so it
     * is not asked. */
    if (wcscmp(source, target) == 0) {
        return 0;
    }
    directory = (was & FILE_ATTRIBUTE_DIRECTORY) != 0;
    there = GetFileAttributesW(target);
    if (there != INVALID_FILE_ATTRIBUTES) {
        if (directory) {
            /* Anything there but the directory itself under another case of
             * its letters. */
            if (hero_disk_same_wide(source, target, false) != 1) {
                return hero_disk_refused(HERO_DISK_EXISTS);
            }
        } else if ((there & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            return hero_disk_refused(HERO_DISK_IS_A_DIRECTORY);
        }
    }
    /* No `MOVEFILE_COPY_ALLOWED`: a move between volumes fails and moves
     * nothing. A directory is moved without the replacing flag, which its
     * page refuses for one. */
    if (!MoveFileExW(source, target, directory ? 0 : MOVEFILE_REPLACE_EXISTING)) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_scan(const char *path, void *buffer, uint64_t count) {
    wchar_t pattern[HERO_DISK_WIDE];
    wchar_t bare[HERO_DISK_WIDE];
    char name[3 * MAX_PATH + 4];
    WIN32_FIND_DATAW found;
    HANDLE search;
    DWORD code;
    DWORD attributes;
    uint64_t needed = 0;
    int64_t kind;
    int64_t length;
    char last;

    hero_disk_clear();
    if (!hero_disk_room_ok(buffer, count) || !hero_disk_path_ok(path)) {
        return -1;
    }
    /* Every entry of the folder: `\*` after its name, or `*` alone where
     * the name ends in a separator or names a drive (`C:`). */
    last = path[strlen(path) - 1];
    if (!hero_disk_wide(path, last == '\\' || last == '/' || last == ':' ? L"*" : L"\\*", pattern)) {
        return -1;
    }
    search = FindFirstFileW(pattern, &found);
    if (search == INVALID_HANDLE_VALUE) {
        /* The search's number does not say what stands at the path, so the
         * path is asked: nothing, something that is no directory, or a
         * directory, which with `ERROR_FILE_NOT_FOUND` is a root holding
         * nothing (any other holds `.` and `..`). */
        code = GetLastError();
        if (!hero_disk_wide(path, NULL, bare)) {
            return -1;
        }
        attributes = GetFileAttributesW(bare);
        if (attributes == INVALID_FILE_ATTRIBUTES) {
            return hero_disk_failed();
        }
        if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            hero_disk_fail(HERO_DISK_NOT_A_DIRECTORY, (int64_t)code);
            return -1;
        }
        if (code == ERROR_FILE_NOT_FOUND) {
            return 0;
        }
        return hero_disk_failed_with((int64_t)code);
    }
    do {
        const wchar_t *w = found.cFileName;

        if (w[0] == L'.' && (w[1] == 0 || (w[1] == L'.' && w[2] == 0))) {
            continue;
        }
        if ((found.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0 && hero_disk_link_tag(found.dwReserved0)) {
            kind = HERO_DISK_KIND_LINK;
        } else if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            kind = HERO_DISK_KIND_DIRECTORY;
        } else {
            kind = HERO_DISK_KIND_FILE;
        }
        length = hero_disk_narrow(w, wcslen(w), name, sizeof name);
        if (length < 0) {
            (void)FindClose(search);
            return hero_disk_refused(HERO_DISK_LIMIT);
        }
        needed = hero_disk_put(buffer, count, needed, kind, name, (size_t)length);
    } while (FindNextFileW(search, &found));
    code = GetLastError();
    (void)FindClose(search);
    if (code != ERROR_NO_MORE_FILES) {
        return hero_disk_failed_with((int64_t)code);
    }
    return (int64_t)needed;
}

int64_t hero_disk_link_target(const char *path, void *buffer, uint64_t count) {
    wchar_t wide[HERO_DISK_WIDE];
    /* The reparse data, in units so that the names inside it, which stand
     * at even offsets, are read where they lie. Its layout is the one
     * Microsoft's page of the reparse data buffer gives: a tag of four
     * bytes, a length and a reserved field of two each, then for a symbolic
     * link and a junction alike the substitute name's offset and length and
     * the print name's, two bytes each, a symbolic link's flags of four,
     * and the names. */
    WCHAR data[MAXIMUM_REPARSE_DATA_BUFFER_SIZE / sizeof(WCHAR)];
    char target[HERO_DISK_PATH_MOST + 1];
    const unsigned char *raw = (const unsigned char *)data;
    const WCHAR *text;
    DWORD got = 0;
    DWORD tag = 0;
    USHORT offset = 0;
    USHORT bytes = 0;
    USHORT print_offset = 0;
    USHORT print_bytes = 0;
    size_t names;
    size_t units;
    int64_t length;
    HANDLE h;

    hero_disk_clear();
    if (!hero_disk_room_ok(buffer, count) || !hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    h = hero_disk_open(wide, FILE_READ_ATTRIBUTES, false);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    if (!DeviceIoControl(h, FSCTL_GET_REPARSE_POINT, NULL, 0, data, (DWORD)sizeof data, &got, NULL)) {
        return hero_disk_failed_closing(h);
    }
    (void)CloseHandle(h);
    if (got < 16) {
        return hero_disk_refused(HERO_DISK_OTHER);
    }
    memcpy(&tag, raw, sizeof tag);
    if (tag == IO_REPARSE_TAG_SYMLINK) {
        names = 20;
    } else if (tag == IO_REPARSE_TAG_MOUNT_POINT) {
        names = 16;
    } else {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    memcpy(&offset, raw + 8, sizeof offset);
    memcpy(&bytes, raw + 10, sizeof bytes);
    memcpy(&print_offset, raw + 12, sizeof print_offset);
    memcpy(&print_bytes, raw + 14, sizeof print_bytes);
    if (print_bytes != 0) {
        offset = print_offset;
        bytes = print_bytes;
    }
    if (offset % 2 != 0 || bytes % 2 != 0 || names + (size_t)offset + (size_t)bytes > (size_t)got) {
        return hero_disk_refused(HERO_DISK_OTHER);
    }
    text = data + (names + (size_t)offset) / 2;
    units = (size_t)bytes / 2;
    /* A substitute name is the system's own spelling, `\??\C:\folder`; the
     * prefix is the system's and no part of the target. */
    if (print_bytes == 0 && units >= 4 && text[0] == L'\\' && text[1] == L'?' && text[2] == L'?' && text[3] == L'\\') {
        text += 4;
        units -= 4;
    }
    length = hero_disk_narrow(text, units, target, sizeof target);
    if (length < 0) {
        return hero_disk_refused(HERO_DISK_LIMIT);
    }
    return hero_disk_fill(buffer, count, target, (size_t)length);
}

int64_t hero_disk_set_times(const char *path, int64_t written, int64_t read) {
    wchar_t wide[HERO_DISK_WIDE];
    FILETIME written_at;
    FILETIME read_at;
    HANDLE h;

    hero_disk_clear();
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    if (!hero_disk_filetime(written, &written_at) || !hero_disk_filetime(read, &read_at)) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    h = hero_disk_open(wide, FILE_WRITE_ATTRIBUTES, true);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    if (!SetFileTime(h, NULL, &read_at, &written_at)) {
        return hero_disk_failed_closing(h);
    }
    (void)CloseHandle(h);
    return 0;
}

int64_t hero_disk_set_length(const char *path, int64_t length) {
    wchar_t wide[HERO_DISK_WIDE];
    LARGE_INTEGER end;
    DWORD attributes;
    HANDLE h;

    hero_disk_clear();
    if (length < 0) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    /* A directory opened to be written is `ERROR_ACCESS_DENIED`, which
     * would read as this user refused: it is asked first. */
    attributes = GetFileAttributesW(wide);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return hero_disk_failed();
    }
    if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (attributes & FILE_ATTRIBUTE_REPARSE_POINT) == 0) {
        return hero_disk_refused(HERO_DISK_IS_A_DIRECTORY);
    }
    h = CreateFileW(wide, GENERIC_WRITE, HERO_DISK_SHARE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    end.QuadPart = length;
    if (!SetFilePointerEx(h, end, NULL, FILE_BEGIN) || !SetEndOfFile(h)) {
        return hero_disk_failed_closing(h);
    }
    (void)CloseHandle(h);
    return 0;
}

int64_t hero_disk_enter(const char *path) {
    wchar_t wide[HERO_DISK_WIDE];

    hero_disk_clear();
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    if (!SetCurrentDirectoryW(wide)) {
        /* What stands at the path says whether it was no directory, rather
         * than the number, as in `hero_disk_scan`. */
        DWORD code = GetLastError();
        DWORD attributes = GetFileAttributesW(wide);

        if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            hero_disk_fail(HERO_DISK_NOT_A_DIRECTORY, (int64_t)code);
            return -1;
        }
        return hero_disk_failed_with((int64_t)code);
    }
    return 0;
}

int64_t hero_disk_temp_folder(void *buffer, uint64_t count) {
    WCHAR wide[MAX_PATH + 2];
    char folder[3 * (MAX_PATH + 2) + 4];
    DWORD room = (DWORD)(sizeof wide / sizeof wide[0]);
    DWORD units;
    int64_t length;

    hero_disk_clear();
    if (!hero_disk_room_ok(buffer, count)) {
        return -1;
    }
    units = GetTempPathW(room, wide);
    if (units == 0) {
        return hero_disk_failed();
    }
    if (units >= room) {
        return hero_disk_refused(HERO_DISK_LIMIT);
    }
    /* The system's answer ends in a separator: it goes, unless what is left
     * would be a drive alone (`C:`), which names another folder. */
    while (units > 1 && (wide[units - 1] == L'\\' || wide[units - 1] == L'/') && wide[units - 2] != L':') {
        units -= 1;
    }
    length = hero_disk_narrow(wide, (size_t)units, folder, sizeof folder);
    if (length < 0) {
        return hero_disk_refused(HERO_DISK_LIMIT);
    }
    return hero_disk_fill(buffer, count, folder, (size_t)length);
}

int64_t hero_disk_space(const char *path, int64_t *free_bytes, int64_t *total_bytes) {
    wchar_t wide[HERO_DISK_WIDE];
    wchar_t root[HERO_DISK_WIDE];
    ULARGE_INTEGER available;
    ULARGE_INTEGER whole;

    hero_disk_clear();
    if (free_bytes == NULL || total_bytes == NULL) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    /* `GetDiskFreeSpaceExW` takes a directory, and a path may name a file:
     * the path is asked to be there, then its volume's own folder is the
     * one asked about. */
    if (GetFileAttributesW(wide) == INVALID_FILE_ATTRIBUTES) {
        return hero_disk_failed();
    }
    if (!GetVolumePathNameW(wide, root, (DWORD)HERO_DISK_WIDE)) {
        return hero_disk_failed();
    }
    if (!GetDiskFreeSpaceExW(root, &available, &whole, NULL)) {
        return hero_disk_failed();
    }
    if (available.QuadPart > (ULONGLONG)INT64_MAX || whole.QuadPart > (ULONGLONG)INT64_MAX) {
        return hero_disk_refused(HERO_DISK_OTHER);
    }
    *free_bytes = (int64_t)available.QuadPart;
    *total_bytes = (int64_t)whole.QuadPart;
    return 0;
}

/* Windows names an owner by a security identifier, never by two numbers:
 * the door has nothing to hand them to, and says so. */
int64_t hero_disk_set_owner(const char *path, int64_t user, int64_t group) {
    (void)path;
    (void)user;
    (void)group;
    return hero_disk_refused(HERO_DISK_UNSUPPORTED);
}

int64_t hero_disk_set_link_owner(const char *path, int64_t user, int64_t group) {
    (void)path;
    (void)user;
    (void)group;
    return hero_disk_refused(HERO_DISK_UNSUPPORTED);
}

#else

/* The times of a file, which POSIX names `st_mtim` and `st_atim` and Darwin
 * `st_mtimespec` and `st_atimespec`. */
#if defined(__APPLE__)
#define HERO_DISK_WRITTEN_AT(info) ((info).st_mtimespec)
#define HERO_DISK_READ_AT(info) ((info).st_atimespec)
#else
#define HERO_DISK_WRITTEN_AT(info) ((info).st_mtim)
#define HERO_DISK_READ_AT(info) ((info).st_atim)
#endif

#define HERO_DISK_NS_PER_S INT64_C(1000000000)

/* An owner's number is unsigned on Darwin and on Linux, which
 * `hero_disk_id_ok` rests on: where it is not, this refuses to compile. */
_Static_assert((uid_t)-1 > 0 && (gid_t)-1 > 0, "an owner's number is unsigned, so -1 is its last value");

/* Seconds and the part of one as nanoseconds since 1970, or the nearest
 * that 64 bits of them hold. The part is never negative, before 1970 too. */
static int64_t hero_disk_ns(int64_t seconds, int64_t part) {
    int64_t whole = 0;
    int64_t ns = 0;

    if (__builtin_mul_overflow(seconds, HERO_DISK_NS_PER_S, &whole) || __builtin_add_overflow(whole, part, &ns)) {
        return seconds < 0 ? INT64_MIN : INT64_MAX;
    }
    return ns;
}

/* Nanoseconds since 1970 as seconds and the part of one, the part never
 * negative. */
static struct timespec hero_disk_spec(int64_t ns) {
    struct timespec out;
    int64_t seconds = ns / HERO_DISK_NS_PER_S;
    int64_t part = ns % HERO_DISK_NS_PER_S;

    if (part < 0) {
        part += HERO_DISK_NS_PER_S;
        seconds -= 1;
    }
    out.tv_sec = (time_t)seconds;
    out.tv_nsec = (long)part;
    return out;
}

static int64_t hero_disk_kind_of_mode(mode_t mode) {
    if (S_ISREG(mode)) {
        return HERO_DISK_KIND_FILE;
    }
    if (S_ISDIR(mode)) {
        return HERO_DISK_KIND_DIRECTORY;
    }
    if (S_ISLNK(mode)) {
        return HERO_DISK_KIND_LINK;
    }
    return HERO_DISK_KIND_OTHER;
}

static int64_t hero_disk_facts_of(const char *path, bool follow, int64_t *facts, uint64_t count) {
    struct stat info;
    int64_t all[HERO_DISK_FACTS];

    hero_disk_clear();
    if (!hero_disk_room_ok(facts, count) || !hero_disk_path_ok(path)) {
        return -1;
    }
    if ((follow ? stat(path, &info) : lstat(path, &info)) != 0) {
        return hero_disk_failed();
    }
    all[HERO_DISK_FACT_KIND] = hero_disk_kind_of_mode(info.st_mode);
    all[HERO_DISK_FACT_SIZE] = (int64_t)info.st_size;
    all[HERO_DISK_FACT_PERMISSION] = (int64_t)(info.st_mode & 07777);
    all[HERO_DISK_FACT_WRITTEN] =
        hero_disk_ns((int64_t)HERO_DISK_WRITTEN_AT(info).tv_sec, (int64_t)HERO_DISK_WRITTEN_AT(info).tv_nsec);
    all[HERO_DISK_FACT_READ] =
        hero_disk_ns((int64_t)HERO_DISK_READ_AT(info).tv_sec, (int64_t)HERO_DISK_READ_AT(info).tv_nsec);
    all[HERO_DISK_FACT_LINKS] = (int64_t)info.st_nlink;
    return hero_disk_give(all, facts, count);
}

int64_t hero_disk_same(const char *first, const char *second) {
    struct stat a;
    struct stat b;

    hero_disk_clear();
    if (!hero_disk_path_ok(first) || !hero_disk_path_ok(second)) {
        return -1;
    }
    if (stat(first, &a) != 0 || stat(second, &b) != 0) {
        return hero_disk_failed();
    }
    return a.st_dev == b.st_dev && a.st_ino == b.st_ino ? 1 : 0;
}

int64_t hero_disk_make_folder(const char *path, int64_t permission) {
    hero_disk_clear();
    if (permission < 0 || permission > 07777) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    if (mkdir(path, (mode_t)permission) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_remove(const char *path) {
    struct stat info;
    int code;

    hero_disk_clear();
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    /* What the name itself is decides the call, so `unlink` is never handed
     * a directory (the opening comment says what it would do with one) and
     * a link is removed, never what it names. */
    if (lstat(path, &info) != 0) {
        return hero_disk_failed();
    }
    if (!S_ISDIR(info.st_mode)) {
        if (unlink(path) != 0) {
            return hero_disk_failed();
        }
        return 0;
    }
    if (rmdir(path) != 0) {
        /* POSIX lets a directory that holds something be either number. */
        code = errno;
        hero_disk_fail(code == EEXIST ? HERO_DISK_NOT_EMPTY : hero_disk_kind_of((int64_t)code), (int64_t)code);
        return -1;
    }
    return 0;
}

int64_t hero_disk_rename(const char *from, const char *to) {
    struct stat source;
    struct stat target;

    hero_disk_clear();
    if (!hero_disk_path_ok(from) || !hero_disk_path_ok(to)) {
        return -1;
    }
    if (lstat(from, &source) != 0) {
        return hero_disk_failed();
    }
    /* A directory takes the place of nothing, where `rename` alone would
     * replace an empty one: anything at `to` but the directory itself,
     * under another case of its letters, refuses it. A file over a
     * directory is `rename`'s own `EISDIR`. */
    if (S_ISDIR(source.st_mode) && lstat(to, &target) == 0 &&
        !(target.st_dev == source.st_dev && target.st_ino == source.st_ino)) {
        return hero_disk_refused(HERO_DISK_EXISTS);
    }
    if (rename(from, to) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

/* The kind of one entry of an open directory, 0 where it is gone already.
 * The system's own word for it where the entry carries one (`d_type`, on
 * Darwin, Linux and the BSDs; a volume may still answer that it does not
 * know), else the entry is asked by name inside its directory, no link
 * followed; one that cannot be asked is `HERO_DISK_KIND_UNKNOWN`. */
static int64_t hero_disk_entry_kind(DIR *folder, const struct dirent *entry) {
    struct stat info;

#if defined(DT_UNKNOWN)
    switch (entry->d_type) {
    case DT_REG:
        return HERO_DISK_KIND_FILE;
    case DT_DIR:
        return HERO_DISK_KIND_DIRECTORY;
    case DT_LNK:
        return HERO_DISK_KIND_LINK;
    case DT_UNKNOWN:
        break;
    default:
        return HERO_DISK_KIND_OTHER;
    }
#endif
    if (fstatat(dirfd(folder), entry->d_name, &info, AT_SYMLINK_NOFOLLOW) != 0) {
        return errno == ENOENT ? 0 : HERO_DISK_KIND_UNKNOWN;
    }
    return hero_disk_kind_of_mode(info.st_mode);
}

int64_t hero_disk_scan(const char *path, void *buffer, uint64_t count) {
    DIR *folder;
    struct dirent *entry;
    uint64_t needed = 0;
    int64_t kind;
    int code;

    hero_disk_clear();
    if (!hero_disk_room_ok(buffer, count) || !hero_disk_path_ok(path)) {
        return -1;
    }
    folder = opendir(path);
    if (folder == NULL) {
        return hero_disk_failed();
    }
    /* `readdir` on a stream of this call's own: no other thread holds it,
     * which is all the thread safety POSIX asks of the call. Its NULL is
     * the end or a failure, told apart by an `errno` cleared before it. */
    for (;;) {
        const char *name;

        errno = 0;
        entry = readdir(folder);
        if (entry == NULL) {
            code = errno;
            break;
        }
        name = entry->d_name;
        if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'))) {
            continue;
        }
        kind = hero_disk_entry_kind(folder, entry);
        if (kind == 0) {
            continue;
        }
        needed = hero_disk_put(buffer, count, needed, kind, name, strlen(name));
    }
    (void)closedir(folder);
    if (code != 0) {
        return hero_disk_failed_with((int64_t)code);
    }
    return (int64_t)needed;
}

int64_t hero_disk_link_target(const char *path, void *buffer, uint64_t count) {
    /* One byte more than the longest target the layer answers: `readlink`
     * cuts in silence at its buffer's size, so a buffer it fills whole is a
     * target that may be longer, and is refused. */
    char target[HERO_DISK_PATH_MOST + 1];
    ssize_t got;

    hero_disk_clear();
    if (!hero_disk_room_ok(buffer, count) || !hero_disk_path_ok(path)) {
        return -1;
    }
    got = readlink(path, target, sizeof target);
    if (got < 0) {
        return hero_disk_failed();
    }
    if ((size_t)got >= sizeof target) {
        return hero_disk_refused(HERO_DISK_LIMIT);
    }
    return hero_disk_fill(buffer, count, target, (size_t)got);
}

int64_t hero_disk_set_times(const char *path, int64_t written, int64_t read) {
    struct timespec times[2];

    hero_disk_clear();
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    /* The last read first, then the last write, as `utimensat` takes them. */
    times[0] = hero_disk_spec(read);
    times[1] = hero_disk_spec(written);
    if (utimensat(AT_FDCWD, path, times, 0) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_set_length(const char *path, int64_t length) {
    hero_disk_clear();
    if (length < 0) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    if (truncate(path, (off_t)length) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_enter(const char *path) {
    hero_disk_clear();
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    if (chdir(path) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_temp_folder(void *buffer, uint64_t count) {
    const char *folder;
    size_t length;

    hero_disk_clear();
    if (!hero_disk_room_ok(buffer, count)) {
        return -1;
    }
    /* POSIX's own variable for it, then the system's own folder. */
    folder = getenv("TMPDIR");
    if (folder == NULL || folder[0] == '\0') {
        folder = "/tmp";
    }
    length = strlen(folder);
    while (length > 1 && folder[length - 1] == '/') {
        length -= 1;
    }
    return hero_disk_fill(buffer, count, folder, length);
}

int64_t hero_disk_space(const char *path, int64_t *free_bytes, int64_t *total_bytes) {
    int64_t available = 0;
    int64_t whole = 0;
#if defined(__APPLE__)
    struct statfs info;
#else
    struct statvfs info;
    uint64_t unit;
#endif

    hero_disk_clear();
    if (free_bytes == NULL || total_bytes == NULL) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
#if defined(__APPLE__)
    if (statfs(path, &info) != 0) {
        return hero_disk_failed();
    }
    if (__builtin_mul_overflow((uint64_t)info.f_bavail, (uint64_t)info.f_bsize, &available) ||
        __builtin_mul_overflow((uint64_t)info.f_blocks, (uint64_t)info.f_bsize, &whole)) {
        return hero_disk_refused(HERO_DISK_OTHER);
    }
#else
    if (statvfs(path, &info) != 0) {
        return hero_disk_failed();
    }
    /* Blocks are counted in the fragment size, the block size where a
     * volume reports no fragment. */
    unit = info.f_frsize != 0 ? (uint64_t)info.f_frsize : (uint64_t)info.f_bsize;
    if (__builtin_mul_overflow((uint64_t)info.f_bavail, unit, &available) ||
        __builtin_mul_overflow((uint64_t)info.f_blocks, unit, &whole)) {
        return hero_disk_refused(HERO_DISK_OTHER);
    }
#endif
    *free_bytes = available;
    *total_bytes = whole;
    return 0;
}

/* Whether `number` is one an owner's type of `width` bytes can say: -1,
 * which leaves the owner as it is, or a number of its range but the last,
 * which the system reads as -1. */
static bool hero_disk_id_ok(int64_t number, size_t width) {
    uint64_t last = width >= 8 ? UINT64_MAX : (UINT64_C(1) << (width * 8)) - 1;

    return number == -1 || (number >= 0 && (uint64_t)number < last);
}

static int64_t hero_disk_own(const char *path, int64_t user, int64_t group, bool follow) {
    uid_t owner = user == -1 ? (uid_t)-1 : (uid_t)user;
    gid_t owners = group == -1 ? (gid_t)-1 : (gid_t)group;

    hero_disk_clear();
    if (!hero_disk_id_ok(user, sizeof(uid_t)) || !hero_disk_id_ok(group, sizeof(gid_t))) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    if ((follow ? chown(path, owner, owners) : lchown(path, owner, owners)) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_set_owner(const char *path, int64_t user, int64_t group) {
    return hero_disk_own(path, user, group, true);
}

int64_t hero_disk_set_link_owner(const char *path, int64_t user, int64_t group) {
    return hero_disk_own(path, user, group, false);
}

#endif

int64_t hero_disk_facts(const char *path, int64_t *facts, uint64_t count) {
    return hero_disk_facts_of(path, false, facts, count);
}

int64_t hero_disk_facts_through(const char *path, int64_t *facts, uint64_t count) {
    return hero_disk_facts_of(path, true, facts, count);
}

int64_t hero_disk_last_error(void) {
    return hero_disk_kind;
}

int64_t hero_disk_last_os_error(void) {
    return hero_disk_os;
}

/* -- round 3 (M-rebel-library, 2026-11-16) -------------------------------
 *
 * Links made, permission set, the facts `hero_disk_facts` does not give, and
 * a file locked through a stream. Each arm below uses the helpers of its
 * arm above; what is common stands first. The lock is the one door here
 * that takes a handle, and it holds the stream's descriptor through
 * `hero_stream_hold` for as long as it uses it, so a close on another thread
 * never hands the number to another file meanwhile. */

/* Whether `through` is one a door takes, 0 or 1; false with the failure
 * set. */
static bool hero_disk_through_ok(int64_t through) {
    if (through != 0 && through != 1) {
        hero_disk_fail(HERO_DISK_INVALID, 0);
        return false;
    }
    return true;
}

/* Whether a symbolic link's target is one the door takes: there, 1 to
 * `HERO_DISK_PATH_MOST` bytes; false with `HERO_DISK_INVALID`, the empty
 * target included, which `hero_disk_path_ok` would call not found. */
static bool hero_disk_target_ok(const char *target) {
    if (target == NULL || target[0] == '\0') {
        hero_disk_fail(HERO_DISK_INVALID, 0);
        return false;
    }
    return hero_disk_path_ok(target);
}

/* The descriptor of the stream `s`, held until `hero_stream_release`, as
 * the stream's number; -1 with this layer's kind where none is held. */
static int64_t hero_disk_held(hero_stream *s) {
    int64_t number = hero_stream_hold(s);

    if (number < 0) {
        int64_t why = hero_stream_last_error();

        hero_disk_fail(why == HERO_STREAM_NOT_OPEN    ? HERO_DISK_NOT_OPEN
                       : why == HERO_STREAM_UNSUPPORTED ? HERO_DISK_UNSUPPORTED
                                                        : HERO_DISK_OTHER,
                       hero_stream_last_os_error());
        return -1;
    }
    return number;
}

#if defined(_WIN32)

#if !defined(SYMBOLIC_LINK_FLAG_DIRECTORY)
#define SYMBOLIC_LINK_FLAG_DIRECTORY 0x1
#endif
#if !defined(SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)
#define SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE 0x2
#endif

int64_t hero_disk_other_kind(const char *path, int64_t through) {
    wchar_t wide[HERO_DISK_WIDE];
    HANDLE h;
    DWORD type;
    DWORD code;

    hero_disk_clear();
    if (!hero_disk_through_ok(through) || !hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    h = hero_disk_open(wide, FILE_READ_ATTRIBUTES, through == 1);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    /* `FILE_TYPE_UNKNOWN` is a failure only where the system then names
     * one: the number is cleared first, so a stale one is not read. */
    SetLastError(NO_ERROR);
    type = GetFileType(h);
    code = GetLastError();
    (void)CloseHandle(h);
    if (type == FILE_TYPE_UNKNOWN && code != NO_ERROR) {
        return hero_disk_failed_with((int64_t)code);
    }
    if (type == FILE_TYPE_DISK) {
        return HERO_DISK_OTHER_NONE;
    }
    if (type == FILE_TYPE_PIPE) {
        return HERO_DISK_OTHER_PIPE;
    }
    if (type == FILE_TYPE_CHAR) {
        return HERO_DISK_OTHER_CHARACTER_DEVICE;
    }
    return HERO_DISK_OTHER_UNKNOWN;
}

int64_t hero_disk_made(const char *path, int64_t through, int64_t *made) {
    wchar_t wide[HERO_DISK_WIDE];
    BY_HANDLE_FILE_INFORMATION info;
    HANDLE h;

    hero_disk_clear();
    if (made == NULL) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_through_ok(through) || !hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    h = hero_disk_open(wide, FILE_READ_ATTRIBUTES, through == 1);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    if (!GetFileInformationByHandle(h, &info)) {
        return hero_disk_failed_closing(h);
    }
    (void)CloseHandle(h);
    /* 1601-01-01 exactly is the system's "no time": a volume that keeps
     * none. */
    if (info.ftCreationTime.dwLowDateTime == 0 && info.ftCreationTime.dwHighDateTime == 0) {
        return hero_disk_refused(HERO_DISK_UNSUPPORTED);
    }
    *made = hero_disk_ns_of(info.ftCreationTime);
    return 0;
}

/* Windows names an owner by a security identifier, never by two numbers. */
int64_t hero_disk_owner(const char *path, int64_t through, int64_t *user, int64_t *group) {
    (void)path;
    (void)through;
    hero_disk_clear();
    if (user == NULL || group == NULL) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    return hero_disk_refused(HERO_DISK_UNSUPPORTED);
}

int64_t hero_disk_link(const char *existing, const char *name) {
    wchar_t from[HERO_DISK_WIDE];
    wchar_t to[HERO_DISK_WIDE];
    DWORD attributes;

    hero_disk_clear();
    if (!hero_disk_wide(existing, NULL, from) || !hero_disk_wide(name, NULL, to)) {
        return -1;
    }
    /* The attributes of the name itself, a link's own where it is one. */
    attributes = GetFileAttributesW(from);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return hero_disk_failed();
    }
    if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (attributes & FILE_ATTRIBUTE_REPARSE_POINT) == 0) {
        return hero_disk_refused(HERO_DISK_IS_A_DIRECTORY);
    }
    if (!CreateHardLinkW(to, from, NULL)) {
        return hero_disk_failed();
    }
    return 0;
}

/* Whether `target`, as a link at `name` will read it, names a directory
 * now, every link on the way followed: a relative target is read from the
 * folder holding `name`, that folder being `name`'s part up to its last
 * separator, or its drive's `X:` where it names a drive and no folder. */
static bool hero_disk_names_directory(const wchar_t *target, const wchar_t *name) {
    wchar_t probe[2 * HERO_DISK_WIDE];
    BY_HANDLE_FILE_INFORMATION info;
    size_t folder = 0;
    size_t length = wcslen(target);
    bool absolute = target[0] == L'\\' || (target[0] != 0 && target[1] == L':');
    HANDLE h;
    BOOL asked;

    if (!absolute) {
        for (size_t i = 0; name[i] != 0; i += 1) {
            if (name[i] == L'\\' || name[i] == L'/') {
                folder = i + 1;
            }
        }
        if (folder == 0 && name[0] != 0 && name[1] == L':') {
            folder = 2;
        }
    }
    if (folder + length + 1 > sizeof probe / sizeof probe[0]) {
        return false;
    }
    memcpy(probe, name, folder * sizeof(wchar_t));
    memcpy(probe + folder, target, (length + 1) * sizeof(wchar_t));
    h = hero_disk_open(probe, FILE_READ_ATTRIBUTES, true);
    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }
    asked = GetFileInformationByHandle(h, &info);
    (void)CloseHandle(h);
    return asked && (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int64_t hero_disk_symlink(const char *target, const char *name) {
    wchar_t text[HERO_DISK_WIDE];
    wchar_t to[HERO_DISK_WIDE];
    DWORD flags = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

    hero_disk_clear();
    if (!hero_disk_target_ok(target) || !hero_disk_wide(target, NULL, text) || !hero_disk_wide(name, NULL, to)) {
        return -1;
    }
    for (size_t i = 0; text[i] != 0; i += 1) {
        if (text[i] == L'/') {
            text[i] = L'\\';
        }
    }
    if (hero_disk_names_directory(text, to)) {
        flags |= SYMBOLIC_LINK_FLAG_DIRECTORY;
    }
    if (!CreateSymbolicLinkW(to, text, flags)) {
        return hero_disk_failed();
    }
    return 0;
}

/* The read-only mark of what `path` names, or of the link itself unless
 * `follow`, set from `permission` as `hero_disk_set_permission` says: the
 * attributes read and written through one handle, every time left as it
 * is (a time of 0 in `FILE_BASIC_INFO`). */
static int64_t hero_disk_mark(const char *path, int64_t permission, bool follow) {
    wchar_t wide[HERO_DISK_WIDE];
    FILE_BASIC_INFO basic;
    DWORD wanted;
    HANDLE h;

    hero_disk_clear();
    if (permission < 0 || permission > 07777) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    h = hero_disk_open(wide, FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, follow);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    if (!GetFileInformationByHandleEx(h, FileBasicInfo, &basic, sizeof basic)) {
        return hero_disk_failed_closing(h);
    }
    wanted = (permission & 0222) == 0 ? (basic.FileAttributes | FILE_ATTRIBUTE_READONLY)
                                      : (basic.FileAttributes & ~(DWORD)FILE_ATTRIBUTE_READONLY);
    basic.FileAttributes = wanted == 0 ? FILE_ATTRIBUTE_NORMAL : wanted;
    basic.CreationTime.QuadPart = 0;
    basic.LastAccessTime.QuadPart = 0;
    basic.LastWriteTime.QuadPart = 0;
    basic.ChangeTime.QuadPart = 0;
    if (!SetFileInformationByHandle(h, FileBasicInfo, &basic, sizeof basic)) {
        return hero_disk_failed_closing(h);
    }
    (void)CloseHandle(h);
    return 0;
}

int64_t hero_disk_set_permission(const char *path, int64_t permission) {
    return hero_disk_mark(path, permission, true);
}

int64_t hero_disk_set_link_permission(const char *path, int64_t permission) {
    return hero_disk_mark(path, permission, false);
}

int64_t hero_disk_set_link_times(const char *path, int64_t written, int64_t read) {
    wchar_t wide[HERO_DISK_WIDE];
    FILETIME written_at;
    FILETIME read_at;
    HANDLE h;

    hero_disk_clear();
    if (!hero_disk_wide(path, NULL, wide)) {
        return -1;
    }
    if (!hero_disk_filetime(written, &written_at) || !hero_disk_filetime(read, &read_at)) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    h = hero_disk_open(wide, FILE_WRITE_ATTRIBUTES, false);
    if (h == INVALID_HANDLE_VALUE) {
        return hero_disk_failed();
    }
    if (!SetFileTime(h, NULL, &read_at, &written_at)) {
        return hero_disk_failed_closing(h);
    }
    (void)CloseHandle(h);
    return 0;
}

int64_t hero_disk_lock(hero_stream *s, int64_t how, int64_t wait) {
    OVERLAPPED whole;
    DWORD flags = 0;
    DWORD code = 0;
    BOOL locked;
    int64_t number;
    HANDLE h;

    hero_disk_clear();
    if ((how != HERO_DISK_LOCK_SHARED && how != HERO_DISK_LOCK_EXCLUSIVE) || (wait != 0 && wait != 1)) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    number = hero_disk_held(s);
    if (number < 0) {
        return -1;
    }
    h = (HANDLE)(intptr_t)number;
    /* A lock the stream holds is let go first, as POSIX's `flock` does
     * before it gives another kind: a handle's second lock would otherwise
     * stand beside its first, or wait on it for ever. None held is
     * `ERROR_NOT_LOCKED`, which is no failure here. */
    memset(&whole, 0, sizeof whole);
    (void)UnlockFileEx(h, 0, MAXDWORD, MAXDWORD, &whole);
    if (how == HERO_DISK_LOCK_EXCLUSIVE) {
        flags |= LOCKFILE_EXCLUSIVE_LOCK;
    }
    if (wait == 0) {
        flags |= LOCKFILE_FAIL_IMMEDIATELY;
    }
    memset(&whole, 0, sizeof whole);
    locked = LockFileEx(h, flags, 0, MAXDWORD, MAXDWORD, &whole);
    code = locked ? 0 : GetLastError();
    (void)hero_stream_release(s);
    if (!locked) {
        return hero_disk_failed_with((int64_t)code);
    }
    return 0;
}

int64_t hero_disk_unlock(hero_stream *s) {
    OVERLAPPED whole;
    DWORD code = 0;
    BOOL unlocked;
    int64_t number;

    hero_disk_clear();
    number = hero_disk_held(s);
    if (number < 0) {
        return -1;
    }
    memset(&whole, 0, sizeof whole);
    unlocked = UnlockFileEx((HANDLE)(intptr_t)number, 0, MAXDWORD, MAXDWORD, &whole);
    code = unlocked ? 0 : GetLastError();
    (void)hero_stream_release(s);
    if (!unlocked && code != ERROR_NOT_LOCKED) {
        return hero_disk_failed_with((int64_t)code);
    }
    return 0;
}

#else

/* The facts of what `path` names, the link itself unless `through`. */
static int hero_disk_stat_of(const char *path, int64_t through, struct stat *info) {
    return through == 1 ? stat(path, info) : lstat(path, info);
}

int64_t hero_disk_other_kind(const char *path, int64_t through) {
    struct stat info;

    hero_disk_clear();
    if (!hero_disk_through_ok(through) || !hero_disk_path_ok(path)) {
        return -1;
    }
    if (hero_disk_stat_of(path, through, &info) != 0) {
        return hero_disk_failed();
    }
    if (S_ISREG(info.st_mode) || S_ISDIR(info.st_mode) || S_ISLNK(info.st_mode)) {
        return HERO_DISK_OTHER_NONE;
    }
    if (S_ISFIFO(info.st_mode)) {
        return HERO_DISK_OTHER_PIPE;
    }
    if (S_ISSOCK(info.st_mode)) {
        return HERO_DISK_OTHER_SOCKET;
    }
    if (S_ISCHR(info.st_mode)) {
        return HERO_DISK_OTHER_CHARACTER_DEVICE;
    }
    if (S_ISBLK(info.st_mode)) {
        return HERO_DISK_OTHER_BLOCK_DEVICE;
    }
    return HERO_DISK_OTHER_UNKNOWN;
}

int64_t hero_disk_made(const char *path, int64_t through, int64_t *made) {
    hero_disk_clear();
    if (made == NULL) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_through_ok(through) || !hero_disk_path_ok(path)) {
        return -1;
    }
#if defined(__APPLE__)
    struct stat info;

    if (hero_disk_stat_of(path, through, &info) != 0) {
        return hero_disk_failed();
    }
    *made = hero_disk_ns((int64_t)info.st_birthtimespec.tv_sec, (int64_t)info.st_birthtimespec.tv_nsec);
    return 0;
#elif defined(__linux__) && defined(SYS_statx) && defined(STATX_BTIME)
    struct statx info;

    memset(&info, 0, sizeof info);
    if (syscall(SYS_statx, AT_FDCWD, path, through == 1 ? 0 : AT_SYMLINK_NOFOLLOW, STATX_BTIME, &info) != 0) {
        return hero_disk_failed();
    }
    if ((info.stx_mask & STATX_BTIME) == 0) {
        return hero_disk_refused(HERO_DISK_UNSUPPORTED);
    }
    *made = hero_disk_ns((int64_t)info.stx_btime.tv_sec, (int64_t)info.stx_btime.tv_nsec);
    return 0;
#else
    return hero_disk_refused(HERO_DISK_UNSUPPORTED);
#endif
}

int64_t hero_disk_owner(const char *path, int64_t through, int64_t *user, int64_t *group) {
    struct stat info;

    hero_disk_clear();
    if (user == NULL || group == NULL) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_through_ok(through) || !hero_disk_path_ok(path)) {
        return -1;
    }
    if (hero_disk_stat_of(path, through, &info) != 0) {
        return hero_disk_failed();
    }
    *user = (int64_t)info.st_uid;
    *group = (int64_t)info.st_gid;
    return 0;
}

int64_t hero_disk_link(const char *existing, const char *name) {
    struct stat info;

    hero_disk_clear();
    if (!hero_disk_path_ok(existing) || !hero_disk_path_ok(name)) {
        return -1;
    }
    if (lstat(existing, &info) != 0) {
        return hero_disk_failed();
    }
    if (S_ISDIR(info.st_mode)) {
        return hero_disk_refused(HERO_DISK_IS_A_DIRECTORY);
    }
    if (linkat(AT_FDCWD, existing, AT_FDCWD, name, 0) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_symlink(const char *target, const char *name) {
    hero_disk_clear();
    if (!hero_disk_target_ok(target) || !hero_disk_path_ok(name)) {
        return -1;
    }
    if (symlink(target, name) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_set_permission(const char *path, int64_t permission) {
    hero_disk_clear();
    if (permission < 0 || permission > 07777) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    if (chmod(path, (mode_t)permission) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

int64_t hero_disk_set_link_permission(const char *path, int64_t permission) {
    hero_disk_clear();
    if (permission < 0 || permission > 07777) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    if (fchmodat(AT_FDCWD, path, (mode_t)permission, AT_SYMLINK_NOFOLLOW) != 0) {
        int code = errno;

        /* Linux keeps no bits of a link's own, and says so with
         * `EOPNOTSUPP`, which Darwin numbers apart from `ENOTSUP`. */
        if (code == EOPNOTSUPP) {
            hero_disk_fail(HERO_DISK_UNSUPPORTED, (int64_t)code);
            return -1;
        }
        return hero_disk_failed_with((int64_t)code);
    }
    return 0;
}

int64_t hero_disk_set_link_times(const char *path, int64_t written, int64_t read) {
    struct timespec times[2];

    hero_disk_clear();
    if (!hero_disk_path_ok(path)) {
        return -1;
    }
    times[0] = hero_disk_spec(read);
    times[1] = hero_disk_spec(written);
    if (utimensat(AT_FDCWD, path, times, AT_SYMLINK_NOFOLLOW) != 0) {
        return hero_disk_failed();
    }
    return 0;
}

/* `flock` with `operation`, a signal waited through. 0, or the system's
 * number. */
static int hero_disk_flock(int64_t number, int operation) {
    for (;;) {
        if (flock((int)number, operation) == 0) {
            return 0;
        }
        if (errno != EINTR) {
            return errno;
        }
    }
}

int64_t hero_disk_lock(hero_stream *s, int64_t how, int64_t wait) {
    int operation;
    int64_t number;
    int code;

    hero_disk_clear();
    if ((how != HERO_DISK_LOCK_SHARED && how != HERO_DISK_LOCK_EXCLUSIVE) || (wait != 0 && wait != 1)) {
        return hero_disk_refused(HERO_DISK_INVALID);
    }
    number = hero_disk_held(s);
    if (number < 0) {
        return -1;
    }
    operation = (how == HERO_DISK_LOCK_EXCLUSIVE ? LOCK_EX : LOCK_SH) | (wait == 0 ? LOCK_NB : 0);
    code = hero_disk_flock(number, operation);
    (void)hero_stream_release(s);
    if (code == EWOULDBLOCK) {
        hero_disk_fail(HERO_DISK_BUSY, (int64_t)code);
        return -1;
    }
    if (code != 0) {
        return hero_disk_failed_with((int64_t)code);
    }
    return 0;
}

int64_t hero_disk_unlock(hero_stream *s) {
    int64_t number;
    int code;

    hero_disk_clear();
    number = hero_disk_held(s);
    if (number < 0) {
        return -1;
    }
    code = hero_disk_flock(number, LOCK_UN);
    (void)hero_stream_release(s);
    if (code != 0) {
        return hero_disk_failed_with((int64_t)code);
    }
    return 0;
}

#endif
