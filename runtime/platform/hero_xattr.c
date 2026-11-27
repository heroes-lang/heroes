/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_xattr.c — the platform layer's extended attributes, one
 * member of the archive the compiler puts on every program's link line
 * (M-rebel-library, gap G07). Its header, `hero_xattr.h`, says what each
 * function answers; this file says how each platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * NOTHING SURVIVES BETWEEN CALLS BUT THE LAST ERROR, which is per thread. What
 * one call needs on Windows (a wide path, a stream listing) it takes through
 * `parts/alloc.c`, the single allocation point (the net's `runtime` suite,
 * rule 1), and gives back before it returns.
 *
 * A READING THAT CHANGES UNDER IT IS READ AGAIN: on POSIX a list or a value
 * is measured, then read into the caller's room, and a value that grew in
 * between (`ERANGE`) is measured again, at most `HERO_XATTR_TRIES` times.
 *
 * ON WINDOWS THE FILE IS HELD WHILE ITS STREAM IS ASKED: the path is opened
 * first, with no `FILE_SHARE_DELETE`, so a stream is never written on a file
 * made in its place, since opening `file:name` to write makes `file` where
 * nothing is there.
 */

#include "hero_xattr.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <wchar.h>
#else
#include <errno.h>
#include <limits.h>
#if defined(__APPLE__) || defined(__linux__)
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#endif
#endif

static _Thread_local int64_t hero_xattr_kind;
static _Thread_local int64_t hero_xattr_os;

static int64_t hero_xattr_fail(int64_t kind, int64_t os) {
    hero_xattr_kind = kind;
    hero_xattr_os = os;
    return -1;
}

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
static void hero_xattr_fine(void) {
    hero_xattr_kind = HERO_XATTR_OK;
    hero_xattr_os = 0;
}
#endif

int64_t hero_xattr_last_error(void) {
    return hero_xattr_kind;
}

int64_t hero_xattr_last_os_error(void) {
    return hero_xattr_os;
}

/* What every platform refuses before asking the system: a path or a name
 * that is not there or is empty, room counted and not given, a value
 * counted and not given, a mode the header does not name. */
static bool hero_xattr_args(const char *path, const char *name, bool named, const void *bytes, uint64_t count) {
    if (path == NULL || path[0] == 0 || (named && (name == NULL || name[0] == 0)) || (bytes == NULL && count > 0)) {
        hero_xattr_fail(HERO_XATTR_INVALID, 0);
        return false;
    }
    return true;
}

#if defined(__APPLE__) || defined(__linux__)

/* How many times a list or a value that changes between its measure and its
 * reading is measured again before the call fails. */
#define HERO_XATTR_TRIES 8

/* The kind a platform's number is, the same kind on every platform. Darwin
 * tells a missing attribute `ENOATTR` and Linux `ENODATA`; Darwin's
 * `ENOTSUP` and `EOPNOTSUPP` are two numbers and Linux's one. */
static int64_t hero_xattr_kind_of(int code) {
#if defined(__APPLE__)
    if (code == ENOATTR) {
        return HERO_XATTR_ABSENT;
    }
#else
    if (code == ENODATA) {
        return HERO_XATTR_ABSENT;
    }
#endif
    if (code == ENOTSUP || code == EOPNOTSUPP) {
        return HERO_XATTR_UNSUPPORTED;
    }
    switch (code) {
    case ENOENT:
        return HERO_XATTR_NOT_FOUND;
    case ENOTDIR:
        return HERO_XATTR_NOT_A_DIRECTORY;
    case EACCES:
    case EPERM:
    case EROFS:
        return HERO_XATTR_DENIED;
    case EEXIST:
        return HERO_XATTR_EXISTS;
    case E2BIG:
        return HERO_XATTR_TOO_LARGE;
    case ENOSPC:
    case EDQUOT:
    case ENOMEM:
        return HERO_XATTR_LIMIT;
    case ENAMETOOLONG:
    case EINVAL:
        return HERO_XATTR_INVALID;
    default:
        return HERO_XATTR_OTHER;
    }
}

static int64_t hero_xattr_failed(void) {
    int code = errno;

    return hero_xattr_fail(hero_xattr_kind_of(code), (int64_t)code);
}

/* The room a call may write, at most what `size_t` and `ssize_t` count. */
static size_t hero_xattr_room(uint64_t count) {
    uint64_t most = (uint64_t)SSIZE_MAX;

    return (size_t)(count < most ? count : most);
}

static ssize_t hero_xattr_list_call(const char *path, char *buffer, size_t size, bool follow) {
#if defined(__APPLE__)
    return listxattr(path, buffer, size, follow ? 0 : XATTR_NOFOLLOW);
#else
    return follow ? listxattr(path, buffer, size) : llistxattr(path, buffer, size);
#endif
}

static ssize_t hero_xattr_get_call(const char *path, const char *name, void *buffer, size_t size, bool follow) {
#if defined(__APPLE__)
    return getxattr(path, name, buffer, size, 0, follow ? 0 : XATTR_NOFOLLOW);
#else
    return follow ? getxattr(path, name, buffer, size) : lgetxattr(path, name, buffer, size);
#endif
}

/* The fill form over a call that measures with no room and fails `ERANGE`
 * where the room it is given is short: measured, read where it fits, and
 * measured again where it grew in between. `name` is NULL for the list. */
static int64_t hero_xattr_fill(const char *path, const char *name, bool follow, void *buffer, uint64_t count) {
    size_t room = hero_xattr_room(count);
    int tries;

    for (tries = 0; tries < HERO_XATTR_TRIES; tries += 1) {
        ssize_t need = name == NULL ? hero_xattr_list_call(path, NULL, 0, follow)
                                    : hero_xattr_get_call(path, name, NULL, 0, follow);
        ssize_t got;

        if (need < 0) {
            return hero_xattr_failed();
        }
        if ((uint64_t)need > (uint64_t)room || need == 0) {
            hero_xattr_fine();
            return (int64_t)need;
        }
        got = name == NULL ? hero_xattr_list_call(path, (char *)buffer, room, follow)
                           : hero_xattr_get_call(path, name, buffer, room, follow);
        if (got >= 0) {
            hero_xattr_fine();
            return (int64_t)got;
        }
        if (errno != ERANGE) {
            return hero_xattr_failed();
        }
    }
    return hero_xattr_fail(HERO_XATTR_OTHER, (int64_t)ERANGE);
}

int64_t hero_xattr_names(const char *path, int64_t follow, void *buffer, uint64_t count) {
    if (!hero_xattr_args(path, NULL, false, buffer, count)) {
        return -1;
    }
    return hero_xattr_fill(path, NULL, follow != 0, buffer, count);
}

int64_t hero_xattr_read(const char *path, const char *name, int64_t follow, void *buffer, uint64_t count) {
    if (!hero_xattr_args(path, name, true, buffer, count)) {
        return -1;
    }
    return hero_xattr_fill(path, name, follow != 0, buffer, count);
}

int64_t hero_xattr_write(const char *path, const char *name, const void *value, uint64_t size, int64_t follow,
                         int64_t mode) {
    int flags;
    int done;

    if (!hero_xattr_args(path, name, true, value, size)) {
        return -1;
    }
    if (mode == HERO_XATTR_EITHER) {
        flags = 0;
    } else if (mode == HERO_XATTR_ONLY_NEW) {
        flags = XATTR_CREATE;
    } else if (mode == HERO_XATTR_ONLY_EXISTING) {
        flags = XATTR_REPLACE;
    } else {
        return hero_xattr_fail(HERO_XATTR_INVALID, 0);
    }
    if (size > (uint64_t)SSIZE_MAX) {
        return hero_xattr_fail(HERO_XATTR_TOO_LARGE, 0);
    }
#if defined(__linux__)
    /* Linux's `XATTR_NAME_MAX`: a longer name is `ERANGE`, which a write
     * otherwise reads as a value past the volume's limit. */
    if (strlen(name) > 255) {
        return hero_xattr_fail(HERO_XATTR_INVALID, 0);
    }
#endif
#if defined(__APPLE__)
    done = setxattr(path, name, value, (size_t)size, 0, flags | (follow != 0 ? 0 : XATTR_NOFOLLOW));
#else
    done = follow != 0 ? setxattr(path, name, value, (size_t)size, flags)
                       : lsetxattr(path, name, value, (size_t)size, flags);
#endif
    if (done != 0) {
        /* A write's `ERANGE` is the value's size: on Darwin one a name keeps
         * for itself refuses (`com.apple.FinderInfo` is 32 bytes), on Linux
         * one past what the volume keeps for an attribute. */
        if (errno == ERANGE) {
#if defined(__APPLE__)
            return hero_xattr_fail(HERO_XATTR_INVALID, (int64_t)ERANGE);
#else
            return hero_xattr_fail(HERO_XATTR_TOO_LARGE, (int64_t)ERANGE);
#endif
        }
        return hero_xattr_failed();
    }
    hero_xattr_fine();
    return 0;
}

#if defined(__linux__)
/* Whether Linux refused the removal of `name` from `path` only because the
 * object there can hold no `user.` attribute: Linux keeps `user.` names on
 * regular files and folders alone, and answers a write or a removal of one on
 * anything else with `EPERM` where a reading of the same name answers
 * `ENODATA` (measured in the Linux arm64 image, kernel 7.0.14, on overlayfs
 * and tmpfs, as root and as a user, 2026-11-27: `lremovexattr` and
 * `lsetxattr` of a link, `removexattr` of a pipe and of `/dev/null`), so such
 * an object never holds one. Asked of the object the call named, the link
 * itself where `follow` is 0; any other `EPERM` (another namespace, a sticky
 * folder of another user's, an immutable file) stays a refusal. */
static bool hero_xattr_none_kept(const char *path, const char *name, int64_t follow, int code) {
    struct stat seen;

    if (code != EPERM || strncmp(name, "user.", 5) != 0) {
        return false;
    }
    if ((follow != 0 ? stat(path, &seen) : lstat(path, &seen)) != 0) {
        return false;
    }
    return !S_ISREG(seen.st_mode) && !S_ISDIR(seen.st_mode);
}
#endif

int64_t hero_xattr_remove(const char *path, const char *name, int64_t follow) {
    int done;

    if (!hero_xattr_args(path, name, true, NULL, 0)) {
        return -1;
    }
#if defined(__APPLE__)
    done = removexattr(path, name, follow != 0 ? 0 : XATTR_NOFOLLOW);
#else
    done = follow != 0 ? removexattr(path, name) : lremovexattr(path, name);
#endif
    if (done != 0) {
#if defined(__linux__)
        int code = errno;

        if (hero_xattr_none_kept(path, name, follow, code)) {
            return hero_xattr_fail(HERO_XATTR_ABSENT, (int64_t)code);
        }
        errno = code;
#endif
        return hero_xattr_failed();
    }
    hero_xattr_fine();
    return 0;
}

#elif defined(_WIN32)

/* Every share but deletion: the file a stream belongs to cannot be removed
 * or renamed away while the call holds it. */
#define HERO_XATTR_HOLD (FILE_SHARE_READ | FILE_SHARE_WRITE)
#define HERO_XATTR_SHARE (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE)

/* The largest listing of streams read, past which the call fails `LIMIT`. */
#define HERO_XATTR_LISTING_MOST (64u * 1024u * 1024u)

static int64_t hero_xattr_kind_of(DWORD code) {
    switch (code) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return HERO_XATTR_NOT_FOUND;
    case ERROR_DIRECTORY:
        return HERO_XATTR_NOT_A_DIRECTORY;
    case ERROR_ACCESS_DENIED:
    case ERROR_WRITE_PROTECT:
    case ERROR_PRIVILEGE_NOT_HELD:
        return HERO_XATTR_DENIED;
    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
        return HERO_XATTR_EXISTS;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_DISK_FULL:
    case ERROR_HANDLE_DISK_FULL:
    case ERROR_DISK_QUOTA_EXCEEDED:
    case ERROR_TOO_MANY_OPEN_FILES:
        return HERO_XATTR_LIMIT;
    case ERROR_NOT_SUPPORTED:
    case ERROR_INVALID_FUNCTION:
        return HERO_XATTR_UNSUPPORTED;
    case ERROR_INVALID_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_FILENAME_EXCED_RANGE:
        return HERO_XATTR_INVALID;
    default:
        return HERO_XATTR_OTHER;
    }
}

static int64_t hero_xattr_failed_with(DWORD code) {
    return hero_xattr_fail(hero_xattr_kind_of(code), (int64_t)code);
}

/* The failure the last system call reported, `h` closed after the number is
 * read. */
static int64_t hero_xattr_failed_closing(HANDLE h) {
    DWORD code = GetLastError();

    (void)CloseHandle(h);
    return hero_xattr_failed_with(code);
}

/* `text`, UTF-8, appended to `out` from `*put` as UTF-16; false where it is
 * not text. A surrogate standing alone may be spelled as the three bytes its
 * value would take, as a name the system lists comes out of
 * `hero_xattr_names`; a pair spelled so is refused. Every byte is at most
 * one unit, so `out` holds a text of n bytes in n units. */
static bool hero_xattr_widen(const char *text, wchar_t *out, size_t *put) {
    const unsigned char *s = (const unsigned char *)text;
    size_t at = 0;
    bool after_high = false;

    while (s[at] != 0) {
        uint32_t c = s[at];
        uint32_t least = 0;
        int length = 0;
        int k;

        if (c >= 0x80u) {
            if ((c & 0xE0u) == 0xC0u) {
                c &= 0x1Fu;
                length = 1;
                least = 0x80u;
            } else if ((c & 0xF0u) == 0xE0u) {
                c &= 0x0Fu;
                length = 2;
                least = 0x800u;
            } else if ((c & 0xF8u) == 0xF0u) {
                c &= 0x07u;
                length = 3;
                least = 0x10000u;
            } else {
                return false;
            }
        }
        for (k = 1; k <= length; k += 1) {
            unsigned char next = s[at + (size_t)k];

            if ((next & 0xC0u) != 0x80u) {
                return false;
            }
            c = (c << 6) | (next & 0x3Fu);
        }
        if (c < least || c > 0x10FFFFu || (after_high && c >= 0xDC00u && c <= 0xDFFFu)) {
            return false;
        }
        at += 1 + (size_t)length;
        after_high = c >= 0xD800u && c <= 0xDBFFu;
        if (c >= 0x10000u) {
            out[(*put)++] = (wchar_t)(0xD800u + ((c - 0x10000u) >> 10));
            out[(*put)++] = (wchar_t)(0xDC00u + ((c - 0x10000u) & 0x3FFu));
        } else {
            out[(*put)++] = (wchar_t)c;
        }
    }
    return true;
}

/* `path` as the wide API takes it, and `path:name` where `name` is given, in
 * one block the caller frees; NULL with the failure set. A name holding `:`,
 * `\` or `/` names no stream, so it is refused before the system reads it as
 * a stream's type or a path. */
static wchar_t *hero_xattr_wide(const char *path, const char *name, wchar_t **stream) {
    /* The path and its zero, then the path again, `:`, the name and its
     * zero: a byte is at most one unit. */
    size_t length = strlen(path) + 1 + (name == NULL ? 0 : strlen(path) + 1 + strlen(name) + 1);
    wchar_t *out;
    size_t put = 0;

    if (name != NULL && strpbrk(name, ":\\/") != NULL) {
        hero_xattr_fail(HERO_XATTR_INVALID, 0);
        return NULL;
    }
    out = (wchar_t *)hero_layer_alloc_zero(length + 1, sizeof(wchar_t));
    if (out == NULL) {
        hero_xattr_fail(HERO_XATTR_LIMIT, 0);
        return NULL;
    }
    if (!hero_xattr_widen(path, out, &put)) {
        hero_layer_free(out);
        hero_xattr_fail(HERO_XATTR_NOT_TEXT, 0);
        return NULL;
    }
    out[put++] = 0;
    *stream = NULL;
    if (name != NULL) {
        size_t from = put;

        (void)wmemcpy(out + put, out, from - 1);
        put += from - 1;
        out[put++] = L':';
        if (!hero_xattr_widen(name, out, &put)) {
            hero_layer_free(out);
            hero_xattr_fail(HERO_XATTR_NOT_TEXT, 0);
            return NULL;
        }
        out[put] = 0;
        *stream = out + from;
    }
    return out;
}

static DWORD hero_xattr_flags(bool follow) {
    return FILE_FLAG_BACKUP_SEMANTICS | (follow ? 0 : FILE_FLAG_OPEN_REPARSE_POINT);
}

/* The file `wide` names, held so it cannot be removed while the call works
 * on its streams, after its volume is asked whether it keeps named streams.
 * `INVALID_HANDLE_VALUE` with the failure set. */
static HANDLE hero_xattr_hold(const wchar_t *wide, bool follow) {
    HANDLE h = CreateFileW(wide, FILE_READ_ATTRIBUTES, HERO_XATTR_HOLD, NULL, OPEN_EXISTING, hero_xattr_flags(follow),
                           NULL);
    DWORD flags = 0;

    if (h == INVALID_HANDLE_VALUE) {
        hero_xattr_failed_with(GetLastError());
        return INVALID_HANDLE_VALUE;
    }
    if (!GetVolumeInformationByHandleW(h, NULL, 0, NULL, NULL, &flags, NULL, 0)) {
        hero_xattr_failed_closing(h);
        return INVALID_HANDLE_VALUE;
    }
    if ((flags & FILE_NAMED_STREAMS) == 0) {
        (void)CloseHandle(h);
        hero_xattr_fail(HERO_XATTR_UNSUPPORTED, 0);
        return INVALID_HANDLE_VALUE;
    }
    return h;
}

/* The stream `stream` of a file held, opened for `access` as `disposition`
 * says; a stream that is not there is `HERO_XATTR_ABSENT`, the file being. */
static HANDLE hero_xattr_stream(const wchar_t *stream, DWORD access, DWORD disposition, bool follow) {
    HANDLE h = CreateFileW(stream, access, HERO_XATTR_SHARE, NULL, disposition, hero_xattr_flags(follow), NULL);

    if (h == INVALID_HANDLE_VALUE) {
        DWORD code = GetLastError();

        if (code == ERROR_FILE_NOT_FOUND) {
            hero_xattr_fail(HERO_XATTR_ABSENT, (int64_t)code);
        } else {
            hero_xattr_failed_with(code);
        }
    }
    return h;
}

/* An answer on its way into the caller's room, counted whole. */
typedef struct {
    unsigned char *room;
    uint64_t count;
    uint64_t used;
} hero_xattr_answer;

static void hero_xattr_put(hero_xattr_answer *a, const unsigned char *bytes, size_t n) {
    if (a->used <= a->count && (uint64_t)n <= a->count - a->used) {
        memcpy(a->room + a->used, bytes, n);
    }
    a->used += (uint64_t)n;
}

/* `units` of a stream's name, UTF-8 and a lone surrogate as the three bytes
 * its value would take, then a zero byte. */
static void hero_xattr_narrow(hero_xattr_answer *a, const wchar_t *wide, size_t units) {
    size_t i;

    for (i = 0; i < units; i += 1) {
        uint32_t c = (uint32_t)wide[i];
        unsigned char bytes[4];
        size_t n;

        if (c >= 0xD800u && c <= 0xDBFFu && i + 1 < units && (uint32_t)wide[i + 1] >= 0xDC00u &&
            (uint32_t)wide[i + 1] <= 0xDFFFu) {
            c = 0x10000u + ((c - 0xD800u) << 10) + ((uint32_t)wide[i + 1] - 0xDC00u);
            i += 1;
        }
        if (c < 0x80u) {
            bytes[0] = (unsigned char)c;
            n = 1;
        } else if (c < 0x800u) {
            bytes[0] = (unsigned char)(0xC0u | (c >> 6));
            bytes[1] = (unsigned char)(0x80u | (c & 0x3Fu));
            n = 2;
        } else if (c < 0x10000u) {
            bytes[0] = (unsigned char)(0xE0u | (c >> 12));
            bytes[1] = (unsigned char)(0x80u | ((c >> 6) & 0x3Fu));
            bytes[2] = (unsigned char)(0x80u | (c & 0x3Fu));
            n = 3;
        } else {
            bytes[0] = (unsigned char)(0xF0u | (c >> 18));
            bytes[1] = (unsigned char)(0x80u | ((c >> 12) & 0x3Fu));
            bytes[2] = (unsigned char)(0x80u | ((c >> 6) & 0x3Fu));
            bytes[3] = (unsigned char)(0x80u | (c & 0x3Fu));
            n = 4;
        }
        hero_xattr_put(a, bytes, n);
    }
    hero_xattr_put(a, (const unsigned char *)"", 1);
}

/* Each named data stream of one entry of the listing, `:name:$DATA`, its
 * name written; the unnamed one, `::$DATA`, and a stream of another type are
 * not attributes. */
static void hero_xattr_entry(hero_xattr_answer *a, const wchar_t *name, size_t units) {
    static const wchar_t type[] = L":$DATA";
    size_t type_units = (sizeof type / sizeof type[0]) - 1;

    if (units <= 1 + type_units || name[0] != L':' ||
        wmemcmp(name + units - type_units, type, type_units) != 0) {
        return;
    }
    hero_xattr_narrow(a, name + 1, units - 1 - type_units);
}

int64_t hero_xattr_names(const char *path, int64_t follow, void *buffer, uint64_t count) {
    wchar_t *stream = NULL;
    wchar_t *wide;
    HANDLE h;
    DWORD size = 4096;
    unsigned char *listing = NULL;
    hero_xattr_answer a = {(unsigned char *)buffer, count, 0};
    size_t at = 0;

    if (!hero_xattr_args(path, NULL, false, buffer, count)) {
        return -1;
    }
    wide = hero_xattr_wide(path, NULL, &stream);
    if (wide == NULL) {
        return -1;
    }
    h = hero_xattr_hold(wide, follow != 0);
    hero_layer_free(wide);
    if (h == INVALID_HANDLE_VALUE) {
        return -1;
    }
    for (;;) {
        listing = (unsigned char *)hero_layer_alloc(size);
        if (listing == NULL) {
            (void)CloseHandle(h);
            return hero_xattr_fail(HERO_XATTR_LIMIT, 0);
        }
        if (GetFileInformationByHandleEx(h, FileStreamInfo, listing, size)) {
            break;
        }
        {
            DWORD code = GetLastError();

            hero_layer_free(listing);
            listing = NULL;
            if (code == ERROR_HANDLE_EOF) {
                /* No stream at all: a folder with no named stream. */
                (void)CloseHandle(h);
                hero_xattr_fine();
                return 0;
            }
            if (code != ERROR_MORE_DATA || size >= HERO_XATTR_LISTING_MOST) {
                (void)CloseHandle(h);
                return code == ERROR_MORE_DATA ? hero_xattr_fail(HERO_XATTR_LIMIT, (int64_t)code)
                                               : hero_xattr_failed_with(code);
            }
            size *= 2;
        }
    }
    (void)CloseHandle(h);
    for (;;) {
        const FILE_STREAM_INFO *info = (const FILE_STREAM_INFO *)(const void *)(listing + at);
        size_t fixed = offsetof(FILE_STREAM_INFO, StreamName);

        /* The system's own listing, its lengths checked against the block
         * all the same: an entry past it ends the reading. */
        if (size - at < fixed || info->StreamNameLength > size - at - fixed) {
            break;
        }
        hero_xattr_entry(&a, info->StreamName, info->StreamNameLength / sizeof(wchar_t));
        if (info->NextEntryOffset == 0 || info->NextEntryOffset > size - at) {
            break;
        }
        at += info->NextEntryOffset;
    }
    hero_layer_free(listing);
    hero_xattr_fine();
    return (int64_t)a.used;
}

int64_t hero_xattr_read(const char *path, const char *name, int64_t follow, void *buffer, uint64_t count) {
    wchar_t *stream = NULL;
    wchar_t *wide;
    HANDLE file;
    HANDLE h;
    LARGE_INTEGER length;
    uint64_t got = 0;
    unsigned char more;
    DWORD one = 0;

    if (!hero_xattr_args(path, name, true, buffer, count)) {
        return -1;
    }
    wide = hero_xattr_wide(path, name, &stream);
    if (wide == NULL) {
        return -1;
    }
    file = hero_xattr_hold(wide, follow != 0);
    if (file == INVALID_HANDLE_VALUE) {
        hero_layer_free(wide);
        return -1;
    }
    h = hero_xattr_stream(stream, GENERIC_READ, OPEN_EXISTING, follow != 0);
    hero_layer_free(wide);
    (void)CloseHandle(file);
    if (h == INVALID_HANDLE_VALUE) {
        return -1;
    }
    if (!GetFileSizeEx(h, &length)) {
        return hero_xattr_failed_closing(h);
    }
    if ((uint64_t)length.QuadPart > count) {
        (void)CloseHandle(h);
        hero_xattr_fine();
        return (int64_t)length.QuadPart;
    }
    while (got < count) {
        uint64_t left = count - got;
        DWORD ask = left > (uint64_t)(1u << 30) ? (DWORD)(1u << 30) : (DWORD)left;
        DWORD read = 0;

        if (!ReadFile(h, (unsigned char *)buffer + got, ask, &read, NULL)) {
            return hero_xattr_failed_closing(h);
        }
        if (read == 0) {
            break;
        }
        got += read;
    }
    /* A stream that grew past the room since its length was read: the
     * caller asks again with more. */
    if (got == count && ReadFile(h, &more, 1, &one, NULL) && one == 1) {
        (void)CloseHandle(h);
        hero_xattr_fine();
        return (int64_t)count + 1;
    }
    (void)CloseHandle(h);
    hero_xattr_fine();
    return (int64_t)got;
}

int64_t hero_xattr_write(const char *path, const char *name, const void *value, uint64_t size, int64_t follow,
                         int64_t mode) {
    wchar_t *stream = NULL;
    wchar_t *wide;
    HANDLE file;
    HANDLE h;
    DWORD disposition;
    uint64_t put = 0;

    if (!hero_xattr_args(path, name, true, value, size)) {
        return -1;
    }
    if (mode == HERO_XATTR_EITHER) {
        disposition = OPEN_ALWAYS;
    } else if (mode == HERO_XATTR_ONLY_NEW) {
        disposition = CREATE_NEW;
    } else if (mode == HERO_XATTR_ONLY_EXISTING) {
        disposition = OPEN_EXISTING;
    } else {
        return hero_xattr_fail(HERO_XATTR_INVALID, 0);
    }
    if (size > (uint64_t)INT64_MAX) {
        return hero_xattr_fail(HERO_XATTR_TOO_LARGE, 0);
    }
    wide = hero_xattr_wide(path, name, &stream);
    if (wide == NULL) {
        return -1;
    }
    file = hero_xattr_hold(wide, follow != 0);
    if (file == INVALID_HANDLE_VALUE) {
        hero_layer_free(wide);
        return -1;
    }
    h = hero_xattr_stream(stream, GENERIC_WRITE, disposition, follow != 0);
    hero_layer_free(wide);
    if (h == INVALID_HANDLE_VALUE) {
        (void)CloseHandle(file);
        return -1;
    }
    while (put < size) {
        uint64_t left = size - put;
        DWORD ask = left > (uint64_t)(1u << 30) ? (DWORD)(1u << 30) : (DWORD)left;
        DWORD wrote = 0;

        if (!WriteFile(h, (const unsigned char *)value + put, ask, &wrote, NULL) || wrote == 0) {
            DWORD code = GetLastError();

            (void)CloseHandle(h);
            (void)CloseHandle(file);
            return hero_xattr_failed_with(code);
        }
        put += wrote;
    }
    if (!SetEndOfFile(h)) {
        DWORD code = GetLastError();

        (void)CloseHandle(h);
        (void)CloseHandle(file);
        return hero_xattr_failed_with(code);
    }
    (void)CloseHandle(h);
    (void)CloseHandle(file);
    hero_xattr_fine();
    return 0;
}

int64_t hero_xattr_remove(const char *path, const char *name, int64_t follow) {
    wchar_t *stream = NULL;
    wchar_t *wide;
    HANDLE file;
    HANDLE h;
    FILE_DISPOSITION_INFO gone;

    if (!hero_xattr_args(path, name, true, NULL, 0)) {
        return -1;
    }
    wide = hero_xattr_wide(path, name, &stream);
    if (wide == NULL) {
        return -1;
    }
    file = hero_xattr_hold(wide, follow != 0);
    if (file == INVALID_HANDLE_VALUE) {
        hero_layer_free(wide);
        return -1;
    }
    h = hero_xattr_stream(stream, DELETE, OPEN_EXISTING, follow != 0);
    hero_layer_free(wide);
    (void)CloseHandle(file);
    if (h == INVALID_HANDLE_VALUE) {
        return -1;
    }
    gone.DeleteFile = TRUE;
    if (!SetFileInformationByHandle(h, FileDispositionInfo, &gone, sizeof gone)) {
        return hero_xattr_failed_closing(h);
    }
    (void)CloseHandle(h);
    hero_xattr_fine();
    return 0;
}

#else

/* A POSIX system that is neither Darwin nor Linux: its attributes, where it
 * has them (the BSDs' `extattr_*`), are not read here. */

int64_t hero_xattr_names(const char *path, int64_t follow, void *buffer, uint64_t count) {
    (void)follow;
    if (!hero_xattr_args(path, NULL, false, buffer, count)) {
        return -1;
    }
    return hero_xattr_fail(HERO_XATTR_UNSUPPORTED, 0);
}

int64_t hero_xattr_read(const char *path, const char *name, int64_t follow, void *buffer, uint64_t count) {
    (void)follow;
    if (!hero_xattr_args(path, name, true, buffer, count)) {
        return -1;
    }
    return hero_xattr_fail(HERO_XATTR_UNSUPPORTED, 0);
}

int64_t hero_xattr_write(const char *path, const char *name, const void *value, uint64_t size, int64_t follow,
                         int64_t mode) {
    (void)follow;
    (void)mode;
    if (!hero_xattr_args(path, name, true, value, size)) {
        return -1;
    }
    return hero_xattr_fail(HERO_XATTR_UNSUPPORTED, 0);
}

int64_t hero_xattr_remove(const char *path, const char *name, int64_t follow) {
    (void)follow;
    if (!hero_xattr_args(path, name, true, NULL, 0)) {
        return -1;
    }
    return hero_xattr_fail(HERO_XATTR_UNSUPPORTED, 0);
}

#endif
