/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_procs.c — the platform layer's list of the machine's
 * processes, one member of the archive the compiler puts on every program's
 * link line (M-rebel-library, gap G20). Its header, `hero_procs.h`, says what
 * the answer holds; this file says how each platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * NOTHING SURVIVES BETWEEN CALLS BUT THE LAST ERROR, which is per thread: no
 * table and no lock. What one call needs (a list of ids, a short run of the
 * names accounts have) it takes through `parts/alloc.c`, the single
 * allocation point (the net's `runtime` suite, rule 1), or on the stack, and
 * gives back before it returns.
 *
 * ONE ENTRY IS BUILT WHOLE BEFORE IT IS WRITTEN: a process the platform lists
 * and then will not tell its start time (it ended, it is hidden from this
 * caller) is left out and never half-written, so the fill form's answer is
 * a whole number of entries.
 */

#include "hero_procs.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <sddl.h>
#include <wchar.h>
/* The libraries the member needs, named by the member, as `hero_sys.c` names
 * its own: the request travels in the object and the linker reads it only
 * where it takes this member. `advapi32` holds a process's token, the SID it
 * names and the account that SID is; MSVC's word, a MinGW target links by
 * `-l`. */
#if defined(_MSC_VER)
#pragma comment(lib, "advapi32.lib")
#endif
#else
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <libproc.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/sysctl.h>
#endif
#if defined(__linux__)
#include <dirent.h>
#include <fcntl.h>
#endif
#endif

/* The most a program's name, an owner's name and an owner's id are written
 * from: a Windows name is at most 260 units, three bytes each, and a SID's
 * text at most 187 characters. */
#define HERO_PROCS_TEXT 1024

static _Thread_local int64_t hero_procs_kind;
static _Thread_local int64_t hero_procs_os;

static void hero_procs_fine(void) {
    hero_procs_kind = HERO_PROCS_OK;
    hero_procs_os = 0;
}

static int64_t hero_procs_fail(int64_t kind, int64_t os) {
    hero_procs_kind = kind;
    hero_procs_os = os;
    return -1;
}

int64_t hero_procs_last_error(void) {
    return hero_procs_kind;
}

int64_t hero_procs_last_os_error(void) {
    return hero_procs_os;
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_procs_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_ACCESS_DENIED:
        return HERO_PROCS_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
        return HERO_PROCS_LIMIT;
    case ERROR_NOT_SUPPORTED:
    case ERROR_CALL_NOT_IMPLEMENTED:
        return HERO_PROCS_UNSUPPORTED;
#else
    case EACCES:
    case EPERM:
        return HERO_PROCS_DENIED;
    case ENOMEM:
    case EMFILE:
    case ENFILE:
    case EAGAIN:
        return HERO_PROCS_LIMIT;
    case ENOSYS:
    case ENOTSUP:
        return HERO_PROCS_UNSUPPORTED;
#endif
    default:
        return HERO_PROCS_OTHER;
    }
}

/* An answer on its way into the caller's buffer: the buffer and its bytes,
 * and how many bytes the answer has so far, written or not. */
typedef struct {
    unsigned char *room;
    uint64_t count;
    uint64_t used;
    /* The answer's count no longer fits 64 bits. */
    bool over;
} hero_procs_fill;

static hero_procs_fill hero_procs_into(void *buffer, uint64_t count) {
    hero_procs_fill f;

    f.room = buffer;
    f.count = count;
    f.used = 0;
    f.over = false;
    return f;
}

/* `n` more bytes of the answer: written where all of them fit behind the ones
 * before, counted either way. Once one piece did not fit, `used` is past
 * `count` and no later piece is written, so nothing lands past the buffer. */
static void hero_procs_put(hero_procs_fill *f, const void *bytes, size_t n) {
    if (n > 0 && f->used <= f->count && (uint64_t)n <= f->count - f->used) {
        memcpy(f->room + f->used, bytes, n);
    }
    if (__builtin_add_overflow(f->used, (uint64_t)n, &f->used)) {
        f->over = true;
    }
}

/* What a fill form answers: the bytes of the whole answer. */
static int64_t hero_procs_filled(const hero_procs_fill *f) {
    if (f->over || f->used > (uint64_t)INT64_MAX) {
        return hero_procs_fail(HERO_PROCS_LIMIT, 0);
    }
    hero_procs_fine();
    return (int64_t)f->used;
}

/* One field: `n` bytes and the zero byte that ends them. An empty field is
 * `n` of 0, the zero byte alone. */
static void hero_procs_field(hero_procs_fill *f, const char *bytes, size_t n) {
    hero_procs_put(f, bytes, n);
    hero_procs_put(f, "", 1);
}

static void hero_procs_decimal(hero_procs_fill *f, uint64_t value) {
    char digits[21];
    size_t n = 0;

    do {
        digits[sizeof digits - 2 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    digits[sizeof digits - 1] = '\0';
    hero_procs_put(f, digits + sizeof digits - 1 - n, n + 1);
}

/* `value` times `numerator` over `denominator`, without a product that does
 * not fit: false where the result is past what an `int64_t` holds. */
static bool hero_procs_scaled(uint64_t value, uint64_t numerator, uint64_t denominator, int64_t *out) {
    uint64_t whole = 0;
    uint64_t part = 0;
    uint64_t sum = 0;

    if (denominator == 0) {
        return false;
    }
    if (__builtin_mul_overflow(value / denominator, numerator, &whole) || __builtin_mul_overflow(value % denominator, numerator, &part)) {
        return false;
    }
    if (__builtin_add_overflow(whole, part / denominator, &sum) || sum > (uint64_t)INT64_MAX) {
        return false;
    }
    *out = (int64_t)sum;
    return true;
}

/* One process, whole: what the entry's fields hold, before any is written.
 * A text is a C string of at most `HERO_PROCS_TEXT` - 1 bytes; empty owner
 * text means the platform did not tell it, and `usage` false that fields 5
 * to 8 are empty. */
typedef struct {
    int64_t id;
    int64_t parent;
    int64_t started_ms;
    char owner_id[HERO_PROCS_TEXT];
    char owner_name[HERO_PROCS_TEXT];
    bool usage;
    int64_t resident;
    int64_t virtual_bytes;
    int64_t user_ns;
    int64_t system_ns;
    char name[HERO_PROCS_TEXT];
} hero_procs_row;

static void hero_procs_row_clear(hero_procs_row *row) {
    memset(row, 0, sizeof *row);
}

static void hero_procs_emit(hero_procs_fill *f, const hero_procs_row *row) {
    hero_procs_decimal(f, (uint64_t)row->id);
    hero_procs_decimal(f, (uint64_t)row->parent);
    hero_procs_decimal(f, (uint64_t)row->started_ms);
    hero_procs_field(f, row->owner_id, strlen(row->owner_id));
    hero_procs_field(f, row->owner_name, strlen(row->owner_name));
    if (row->usage) {
        hero_procs_decimal(f, (uint64_t)row->resident);
        hero_procs_decimal(f, (uint64_t)row->virtual_bytes);
        hero_procs_decimal(f, (uint64_t)row->user_ns);
        hero_procs_decimal(f, (uint64_t)row->system_ns);
    } else {
        for (int i = 0; i < 4; i += 1) {
            hero_procs_field(f, "", 0);
        }
    }
    hero_procs_field(f, row->name, strlen(row->name));
}

/* `text` into `out` as a C string, cut at `HERO_PROCS_TEXT` - 1 bytes where
 * longer (a name no platform lets longer than that). */
static void hero_procs_copy(char *out, const char *text, size_t length) {
    size_t n = length < HERO_PROCS_TEXT - 1 ? length : HERO_PROCS_TEXT - 1;

    memcpy(out, text, n);
    out[n] = '\0';
}

#if defined(__APPLE__) || defined(__linux__)

/* `value` as decimal digits in `out`, a C string. */
static void hero_procs_digits(char *out, uint64_t value) {
    char digits[21];
    size_t n = 0;

    do {
        digits[sizeof digits - 2 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    memcpy(out, digits + sizeof digits - 1 - n, n);
    out[n] = '\0';
}

/* How many owners one call remembers the name of: a machine has a handful of
 * accounts running processes, and a lookup may go to a directory service. */
#define HERO_PROCS_OWNERS 32

typedef struct {
    uid_t uid;
    bool named;
    char name[256];
} hero_procs_owner;

typedef struct {
    hero_procs_owner seen[HERO_PROCS_OWNERS];
    size_t used;
} hero_procs_owners;

/* The owner of `row` set from `uid`: its number as decimal digits, and the
 * name the system's record of that user holds, asked once for each user in a
 * call and left empty where no record names one. */
static void hero_procs_owned(hero_procs_owners *owners, uid_t uid, hero_procs_row *row) {
    hero_procs_owner *found = NULL;

    hero_procs_digits(row->owner_id, (uint64_t)uid);
    for (size_t i = 0; i < owners->used; i += 1) {
        if (owners->seen[i].uid == uid) {
            found = &owners->seen[i];
            break;
        }
    }
    if (found == NULL) {
        struct passwd record;
        struct passwd *result = NULL;
        char scratch[4096];
        hero_procs_owner fresh;

        fresh.uid = uid;
        fresh.named = false;
        fresh.name[0] = '\0';
        if (getpwuid_r(uid, &record, scratch, sizeof scratch, &result) == 0 && result != NULL && record.pw_name != NULL) {
            size_t n = strlen(record.pw_name);

            if (n > 0 && n < sizeof fresh.name) {
                memcpy(fresh.name, record.pw_name, n + 1);
                fresh.named = true;
            }
        }
        if (owners->used < HERO_PROCS_OWNERS) {
            owners->seen[owners->used] = fresh;
            found = &owners->seen[owners->used];
            owners->used += 1;
        } else {
            hero_procs_copy(row->owner_name, fresh.name, strlen(fresh.name));
            return;
        }
    }
    if (found->named) {
        hero_procs_copy(row->owner_name, found->name, strlen(found->name));
    }
}

/* The last component of `path`, which is all of it where it holds no slash. */
static const char *hero_procs_base(const char *path) {
    const char *slash = strrchr(path, '/');

    return slash == NULL ? path : slash + 1;
}

/* A name cut where the platform keeps only so many bytes of it, given back
 * whole: `candidate`, where it begins with `name` and is longer. Only a name
 * as long as the platform's limit is asked, because a shorter one was not
 * cut, and an executable that is called something else than the name its
 * process bears is not the name's longer form. */
static void hero_procs_longer(char *name, const char *candidate) {
    size_t have = strlen(name);
    size_t more = strlen(candidate);

    if (more > have && more < HERO_PROCS_TEXT && strncmp(candidate, name, have) == 0) {
        memcpy(name, candidate, more + 1);
    }
}

#endif

#if defined(_WIN32)

/* `units` UTF-16 units as UTF-8 into `out`, a C string cut at
 * `HERO_PROCS_TEXT` - 1 bytes on a whole character: a pair as its one
 * character, and a unit that is half of no pair as the three bytes its own
 * number takes, so nothing is replaced and the caller's own validation
 * refuses exactly that text (as `hero_sys.c` writes its own). */
static void hero_procs_wide(char *out, const WCHAR *wide, size_t units) {
    size_t at = 0;

    for (size_t i = 0; i < units; i += 1) {
        uint32_t c = (uint32_t)wide[i];
        unsigned char b[4];
        size_t n = 0;

        if (c >= 0xD800u && c <= 0xDBFFu && i + 1 < units) {
            uint32_t low = (uint32_t)wide[i + 1];

            if (low >= 0xDC00u && low <= 0xDFFFu) {
                c = 0x10000u + ((c - 0xD800u) << 10) + (low - 0xDC00u);
                i += 1;
            }
        }
        if (c < 0x80u) {
            b[n++] = (unsigned char)c;
        } else if (c < 0x800u) {
            b[n++] = (unsigned char)(0xC0u | (c >> 6));
            b[n++] = (unsigned char)(0x80u | (c & 0x3Fu));
        } else if (c < 0x10000u) {
            b[n++] = (unsigned char)(0xE0u | (c >> 12));
            b[n++] = (unsigned char)(0x80u | ((c >> 6) & 0x3Fu));
            b[n++] = (unsigned char)(0x80u | (c & 0x3Fu));
        } else {
            b[n++] = (unsigned char)(0xF0u | (c >> 18));
            b[n++] = (unsigned char)(0x80u | ((c >> 12) & 0x3Fu));
            b[n++] = (unsigned char)(0x80u | ((c >> 6) & 0x3Fu));
            b[n++] = (unsigned char)(0x80u | (c & 0x3Fu));
        }
        if (at + n > HERO_PROCS_TEXT - 1) {
            break;
        }
        memcpy(out + at, b, n);
        at += n;
    }
    out[at] = '\0';
}

/* 100-nanosecond units from 1601-01-01 to 1970-01-01. */
#define HERO_PROCS_EPOCH_UNITS ((uint64_t)116444736000000000ull)

/* A `FILETIME` as milliseconds since 1970: false before 1970. */
static bool hero_procs_unix_ms(const FILETIME *time, int64_t *out) {
    uint64_t units = ((uint64_t)time->dwHighDateTime << 32) | (uint64_t)time->dwLowDateTime;

    if (units < HERO_PROCS_EPOCH_UNITS) {
        return false;
    }
    *out = (int64_t)((units - HERO_PROCS_EPOCH_UNITS) / 10000u);
    return true;
}

/* A `FILETIME` span as nanoseconds: false where it does not fit. */
static bool hero_procs_span_ns(const FILETIME *time, int64_t *out) {
    uint64_t units = ((uint64_t)time->dwHighDateTime << 32) | (uint64_t)time->dwLowDateTime;

    return hero_procs_scaled(units, 100, 1, out);
}

#define HERO_PROCS_OWNERS 32

/* An account a SID names, remembered for the rest of the call: a lookup of an
 * account of a domain may go over the network. */
typedef struct {
    BYTE sid[SECURITY_MAX_SID_SIZE];
    bool named;
    char name[HERO_PROCS_TEXT];
} hero_procs_owner;

typedef struct {
    hero_procs_owner seen[HERO_PROCS_OWNERS];
    size_t used;
} hero_procs_owners;

static int hero_procs_entries_order(const void *left, const void *right) {
    DWORD a = ((const PROCESSENTRY32W *)left)->th32ProcessID;
    DWORD b = ((const PROCESSENTRY32W *)right)->th32ProcessID;

    return (a > b) - (a < b);
}

/* The owner of `row` read from the token of `process`: the SID's text, and the
 * name of the account it is, without its domain. Left empty where the token
 * is not readable by this caller. The cache holds the accounts looked up
 * already in this call. */
static void hero_procs_owned(hero_procs_owners *owners, HANDLE process, hero_procs_row *row) {
    HANDLE token = NULL;
    union {
        TOKEN_USER user;
        BYTE bytes[sizeof(TOKEN_USER) + SECURITY_MAX_SID_SIZE + 16];
    } held;
    DWORD length = 0;
    PSID sid = NULL;
    WCHAR *text = NULL;
    hero_procs_owner *found = NULL;

    if (!OpenProcessToken(process, TOKEN_QUERY, &token)) {
        return;
    }
    memset(&held, 0, sizeof held);
    if (!GetTokenInformation(token, TokenUser, &held, (DWORD)sizeof held, &length)) {
        (void)CloseHandle(token);
        return;
    }
    (void)CloseHandle(token);
    sid = held.user.User.Sid;
    if (sid == NULL || !IsValidSid(sid) || GetLengthSid(sid) > SECURITY_MAX_SID_SIZE) {
        return;
    }
    if (!ConvertSidToStringSidW(sid, &text)) {
        return;
    }
    hero_procs_wide(row->owner_id, text, wcslen(text));
    (void)LocalFree(text);
    for (size_t i = 0; i < owners->used; i += 1) {
        if (EqualSid((PSID)owners->seen[i].sid, sid)) {
            found = &owners->seen[i];
            break;
        }
    }
    if (found == NULL) {
        hero_procs_owner fresh;
        WCHAR name[512];
        WCHAR domain[512];
        DWORD name_units = 512;
        DWORD domain_units = 512;
        SID_NAME_USE use;

        memset(&fresh, 0, sizeof fresh);
        (void)CopySid((DWORD)sizeof fresh.sid, (PSID)fresh.sid, sid);
        if (LookupAccountSidW(NULL, sid, name, &name_units, domain, &domain_units, &use) && name_units > 0) {
            hero_procs_wide(fresh.name, name, name_units);
            fresh.named = fresh.name[0] != '\0';
        }
        if (owners->used < HERO_PROCS_OWNERS) {
            owners->seen[owners->used] = fresh;
            found = &owners->seen[owners->used];
            owners->used += 1;
        } else {
            hero_procs_copy(row->owner_name, fresh.name, strlen(fresh.name));
            return;
        }
    }
    if (found->named) {
        hero_procs_copy(row->owner_name, found->name, strlen(found->name));
    }
}

/* The process of `entry` read into `row`: 1 where it is whole, 0 where the
 * process is not told to this caller or has ended, -1 with the kind set where
 * the machine has no memory left. */
static int hero_procs_read(const PROCESSENTRY32W *entry, hero_procs_owners *owners, hero_procs_row *row) {
    HANDLE process = NULL;
    FILETIME created;
    FILETIME ended;
    FILETIME kernel;
    FILETIME user;
    PROCESS_MEMORY_COUNTERS counters;
    int64_t in_user = 0;
    int64_t in_system = 0;
    DWORD code = 0;

    hero_procs_row_clear(row);
    process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, entry->th32ProcessID);
    if (process == NULL) {
        code = GetLastError();
        return hero_procs_kind_of(code) == HERO_PROCS_LIMIT ? (int)hero_procs_fail(HERO_PROCS_LIMIT, code) : 0;
    }
    if (!GetProcessTimes(process, &created, &ended, &kernel, &user) || !hero_procs_unix_ms(&created, &row->started_ms)) {
        (void)CloseHandle(process);
        return 0;
    }
    row->id = (int64_t)entry->th32ProcessID;
    row->parent = (int64_t)entry->th32ParentProcessID;
    hero_procs_wide(row->name, entry->szExeFile, wcsnlen(entry->szExeFile, sizeof entry->szExeFile / sizeof entry->szExeFile[0]));
    memset(&counters, 0, sizeof counters);
    counters.cb = (DWORD)sizeof counters;
    if (K32GetProcessMemoryInfo(process, &counters, (DWORD)sizeof counters) && hero_procs_span_ns(&user, &in_user) && hero_procs_span_ns(&kernel, &in_system) && (uint64_t)counters.WorkingSetSize <= (uint64_t)INT64_MAX && (uint64_t)counters.PagefileUsage <= (uint64_t)INT64_MAX) {
        row->usage = true;
        row->resident = (int64_t)counters.WorkingSetSize;
        row->virtual_bytes = (int64_t)counters.PagefileUsage;
        row->user_ns = in_user;
        row->system_ns = in_system;
    }
    hero_procs_owned(owners, process, row);
    (void)CloseHandle(process);
    return 1;
}

/* `CreateToolhelp32Snapshot` lists every process, whoever owns it, with its
 * parent as recorded when it was made and the name of its executable. Its
 * entries are kept and sorted by id, then each process is opened for
 * `PROCESS_QUERY_LIMITED_INFORMATION`, the least right that reads a start
 * time, memory and a token. */
static int64_t hero_procs_walk(hero_procs_fill *f) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W *entries = NULL;
    size_t used = 0;
    size_t room = 0;
    PROCESSENTRY32W one;
    hero_procs_owners *owners = NULL;
    hero_procs_row *row = NULL;
    int64_t answer = 0;

    if (snapshot == INVALID_HANDLE_VALUE) {
        DWORD code = GetLastError();

        return hero_procs_fail(hero_procs_kind_of(code), code);
    }
    memset(&one, 0, sizeof one);
    one.dwSize = (DWORD)sizeof one;
    for (BOOL more = Process32FirstW(snapshot, &one); more; more = Process32NextW(snapshot, &one)) {
        if (used == room) {
            size_t bigger = room == 0 ? 256 : room * 2;
            PROCESSENTRY32W *larger = NULL;

            if (bigger > SIZE_MAX / sizeof *entries) {
                answer = hero_procs_fail(HERO_PROCS_LIMIT, 0);
                break;
            }
            larger = entries == NULL ? hero_layer_alloc(bigger * sizeof *entries) : hero_layer_grow(entries, bigger * sizeof *entries);
            if (larger == NULL) {
                answer = hero_procs_fail(HERO_PROCS_LIMIT, ERROR_NOT_ENOUGH_MEMORY);
                break;
            }
            entries = larger;
            room = bigger;
        }
        entries[used] = one;
        used += 1;
    }
    if (answer == 0 && GetLastError() != ERROR_NO_MORE_FILES && used == 0) {
        DWORD code = GetLastError();

        answer = hero_procs_fail(hero_procs_kind_of(code), code);
    }
    (void)CloseHandle(snapshot);
    owners = hero_layer_alloc_zero(1, sizeof *owners);
    row = hero_layer_alloc(sizeof *row);
    if (answer == 0 && (owners == NULL || row == NULL)) {
        answer = hero_procs_fail(HERO_PROCS_LIMIT, ERROR_NOT_ENOUGH_MEMORY);
    }
    if (answer == 0 && used > 1) {
        qsort(entries, used, sizeof *entries, hero_procs_entries_order);
    }
    for (size_t i = 0; answer == 0 && i < used; i += 1) {
        int read = hero_procs_read(&entries[i], owners, row);

        if (read < 0) {
            answer = -1;
        } else if (read > 0) {
            hero_procs_emit(f, row);
        }
    }
    if (owners != NULL) {
        hero_layer_free(owners);
    }
    if (row != NULL) {
        hero_layer_free(row);
    }
    if (entries != NULL) {
        hero_layer_free(entries);
    }
    return answer;
}

#elif defined(__APPLE__)

/* The longest name `p_comm` holds is sixteen bytes. */
#define HERO_PROCS_NAME_CUT 15

/* Every process the kernel keeps, `sysctl`'s `KERN_PROC_ALL`: the list an
 * ordinary process may read whoever owns the processes, where `proc_pidinfo`
 * answers `EPERM` for every process of another user (measured on this Mac:
 * 464 of 622 read that way, `launchd` among the ones left out). The list is
 * asked for its size and then read with room to spare, and asked again where
 * it grew past that; the memory is the call's and is given back by it. */
static int64_t hero_procs_kernel(struct kinfo_proc **list, size_t *count) {
    int names[3] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL};

    for (int tries = 0; tries < 16; tries += 1) {
        size_t size = 0;
        struct kinfo_proc *found = NULL;

        if (sysctl(names, 3, NULL, &size, NULL, 0) != 0) {
            int code = errno;

            return hero_procs_fail(hero_procs_kind_of(code), code);
        }
        if (size > SIZE_MAX - size / 4 - 4096) {
            return hero_procs_fail(HERO_PROCS_LIMIT, 0);
        }
        size += size / 4 + 4096;
        found = hero_layer_alloc(size);
        if (found == NULL) {
            return hero_procs_fail(HERO_PROCS_LIMIT, ENOMEM);
        }
        if (sysctl(names, 3, found, &size, NULL, 0) == 0) {
            *list = found;
            *count = size / sizeof *found;
            return 0;
        }
        {
            int code = errno;

            hero_layer_free(found);
            if (code != ENOMEM) {
                return hero_procs_fail(hero_procs_kind_of(code), code);
            }
        }
    }
    return hero_procs_fail(HERO_PROCS_OTHER, 0);
}

static int hero_procs_kernel_order(const void *left, const void *right) {
    int a = ((const struct kinfo_proc *)left)->kp_proc.p_pid;
    int b = ((const struct kinfo_proc *)right)->kp_proc.p_pid;

    return (a > b) - (a < b);
}

/* The process `kernel` describes read into `row`: 1 where it is whole, 0 where
 * its start time is not a time. Its id, parent, start, owner and a name of
 * sixteen bytes at most are the kernel's own list's, told for every process;
 * the name is given whole from `proc_name` or the executable's path where the
 * system lets this caller read either. Its memory and time are its task's,
 * which the system tells only for a process of the caller's own user, so
 * another's is listed without usage. */
static int hero_procs_read(const struct kinfo_proc *kernel, mach_timebase_info_data_t base, hero_procs_owners *owners, hero_procs_row *row) {
    struct proc_taskinfo task;
    char longer[PROC_PIDPATHINFO_MAXSIZE];
    int pid = kernel->kp_proc.p_pid;
    int64_t seconds = (int64_t)kernel->kp_proc.p_un.__p_starttime.tv_sec;
    int64_t micros = (int64_t)kernel->kp_proc.p_un.__p_starttime.tv_usec;
    int64_t millis = 0;
    int64_t in_user = 0;
    int64_t in_system = 0;

    hero_procs_row_clear(row);
    if (pid < 0 || seconds <= 0 || micros < 0 || __builtin_mul_overflow(seconds, (int64_t)1000, &millis) || __builtin_add_overflow(millis, micros / 1000, &millis)) {
        return 0;
    }
    row->id = (int64_t)pid;
    row->parent = (int64_t)kernel->kp_eproc.e_ppid;
    row->started_ms = millis;
    hero_procs_copy(row->name, kernel->kp_proc.p_comm, strnlen(kernel->kp_proc.p_comm, sizeof kernel->kp_proc.p_comm));
    if (strlen(row->name) >= HERO_PROCS_NAME_CUT) {
        int length = proc_name(pid, longer, (uint32_t)sizeof longer);

        if (length > 0 && (size_t)length < sizeof longer) {
            longer[length] = '\0';
            hero_procs_longer(row->name, longer);
        }
        length = proc_pidpath(pid, longer, (uint32_t)sizeof longer);
        if (length > 0 && (size_t)length < sizeof longer) {
            longer[length] = '\0';
            hero_procs_longer(row->name, hero_procs_base(longer));
        }
    }
    hero_procs_owned(owners, (uid_t)kernel->kp_eproc.e_ucred.cr_uid, row);
    memset(&task, 0, sizeof task);
    if (proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &task, (int)sizeof task) == (int)sizeof task && hero_procs_scaled(task.pti_total_user, base.numer, base.denom, &in_user) && hero_procs_scaled(task.pti_total_system, base.numer, base.denom, &in_system) && task.pti_resident_size <= (uint64_t)INT64_MAX && task.pti_virtual_size <= (uint64_t)INT64_MAX) {
        row->usage = true;
        row->resident = (int64_t)task.pti_resident_size;
        row->virtual_bytes = (int64_t)task.pti_virtual_size;
        row->user_ns = in_user;
        row->system_ns = in_system;
    }
    return 1;
}

static int64_t hero_procs_walk(hero_procs_fill *f) {
    struct kinfo_proc *list = NULL;
    size_t count = 0;
    mach_timebase_info_data_t base;
    hero_procs_owners *owners = NULL;
    hero_procs_row *row = NULL;
    int64_t answer = 0;

    memset(&base, 0, sizeof base);
    if (mach_timebase_info(&base) != KERN_SUCCESS || base.denom == 0) {
        return hero_procs_fail(HERO_PROCS_OTHER, 0);
    }
    if (hero_procs_kernel(&list, &count) != 0) {
        return -1;
    }
    owners = hero_layer_alloc_zero(1, sizeof *owners);
    row = hero_layer_alloc(sizeof *row);
    if (owners == NULL || row == NULL) {
        answer = hero_procs_fail(HERO_PROCS_LIMIT, ENOMEM);
    }
    if (answer == 0 && count > 1) {
        qsort(list, count, sizeof *list, hero_procs_kernel_order);
    }
    for (size_t i = 0; answer == 0 && i < count; i += 1) {
        /* A process the kernel lists twice, while it forks, is one entry. */
        if (i > 0 && list[i].kp_proc.p_pid == list[i - 1].kp_proc.p_pid) {
            continue;
        }
        if (hero_procs_read(&list[i], base, owners, row) > 0) {
            hero_procs_emit(f, row);
        }
    }
    if (owners != NULL) {
        hero_layer_free(owners);
    }
    if (row != NULL) {
        hero_layer_free(row);
    }
    hero_layer_free(list);
    return answer;
}

#elif defined(__linux__)

/* The ids of the processes found in `/proc`, collected and then sorted
 * ascending, so the list's order is the same on every system. */
typedef struct {
    int32_t *items;
    size_t used;
    size_t room;
} hero_procs_ids;

static bool hero_procs_ids_push(hero_procs_ids *ids, int32_t id) {
    if (ids->used == ids->room) {
        size_t room = ids->room == 0 ? 512 : ids->room * 2;
        int32_t *larger = NULL;

        if (room > SIZE_MAX / sizeof(int32_t)) {
            return false;
        }
        larger = ids->items == NULL ? hero_layer_alloc(room * sizeof(int32_t)) : hero_layer_grow(ids->items, room * sizeof(int32_t));
        if (larger == NULL) {
            return false;
        }
        ids->items = larger;
        ids->room = room;
    }
    ids->items[ids->used] = id;
    ids->used += 1;
    return true;
}

static int hero_procs_ids_order(const void *left, const void *right) {
    int32_t a = *(const int32_t *)left;
    int32_t b = *(const int32_t *)right;

    return (a > b) - (a < b);
}

static void hero_procs_ids_free(hero_procs_ids *ids) {
    if (ids->items != NULL) {
        hero_layer_free(ids->items);
    }
    ids->items = NULL;
    ids->used = 0;
    ids->room = 0;
}

/* The most a file of one process is read into: its `stat` and its `status`
 * hold under a kilobyte and two. */
#define HERO_PROCS_FILE 16384

/* The most `/proc/stat` is read into, whose lines hold a processor each and
 * whose `intr` line one count for each interrupt. */
#define HERO_PROCS_STAT_MOST ((size_t)16 << 20)

/* The most a process's `smaps_rollup` or `statm` is read into: the first
 * holds some twenty lines of under forty bytes, the second one line. */
#define HERO_PROCS_ROLLUP 4096

/* A file of `/proc` read into `buffer` (`size` bytes, a zero byte after what
 * is read) and its length into `*length`: 1 where it was read whole, 0 where
 * the process is gone or this caller may not read it, -1 with the kind set
 * where the file is longer than the buffer or the read failed otherwise. */
static int hero_procs_small(const char *path, char *buffer, size_t size, size_t *length) {
    int file = open(path, O_RDONLY | O_CLOEXEC);
    size_t got = 0;

    if (file < 0) {
        int code = errno;

        if (code == ENOENT || code == ESRCH || code == EACCES || code == EPERM) {
            return 0;
        }
        return (int)hero_procs_fail(hero_procs_kind_of(code), code);
    }
    for (;;) {
        ssize_t now = 0;

        if (got + 1 >= size) {
            (void)close(file);
            return (int)hero_procs_fail(HERO_PROCS_OTHER, 0);
        }
        now = read(file, buffer + got, size - 1 - got);
        if (now < 0 && errno == EINTR) {
            continue;
        }
        if (now < 0) {
            int code = errno;

            (void)close(file);
            /* A process that ends while its file is read answers ESRCH. */
            if (code == ESRCH || code == ENOENT) {
                return 0;
            }
            return (int)hero_procs_fail(hero_procs_kind_of(code), code);
        }
        if (now == 0) {
            break;
        }
        got += (size_t)now;
    }
    (void)close(file);
    buffer[got] = '\0';
    *length = got;
    return 1;
}

/* The boot's time in whole seconds since 1970, from the `btime` line of
 * `/proc/stat`, which the kernel changes only where the clock is set. The
 * file is read whole into memory the call gives back. */
static int64_t hero_procs_boot(int64_t *seconds) {
    int file = open("/proc/stat", O_RDONLY | O_CLOEXEC);
    size_t room = 8192;
    size_t got = 0;
    char *bytes = NULL;
    const char *line = NULL;
    uint64_t value = 0;
    int64_t answer = 0;

    if (file < 0) {
        int code = errno;

        return hero_procs_fail(hero_procs_kind_of(code), code);
    }
    bytes = hero_layer_alloc(room);
    if (bytes == NULL) {
        (void)close(file);
        return hero_procs_fail(HERO_PROCS_LIMIT, ENOMEM);
    }
    for (;;) {
        ssize_t now = 0;

        if (got + 1 >= room) {
            char *larger = NULL;

            if (room >= HERO_PROCS_STAT_MOST) {
                answer = hero_procs_fail(HERO_PROCS_LIMIT, 0);
                break;
            }
            larger = hero_layer_grow(bytes, room * 2);
            if (larger == NULL) {
                answer = hero_procs_fail(HERO_PROCS_LIMIT, ENOMEM);
                break;
            }
            bytes = larger;
            room *= 2;
        }
        now = read(file, bytes + got, room - 1 - got);
        if (now < 0 && errno == EINTR) {
            continue;
        }
        if (now < 0) {
            int code = errno;

            answer = hero_procs_fail(hero_procs_kind_of(code), code);
            break;
        }
        if (now == 0) {
            break;
        }
        got += (size_t)now;
    }
    (void)close(file);
    if (answer == 0) {
        bytes[got] = '\0';
        line = bytes;
        while (line != NULL && *line != '\0') {
            if (strncmp(line, "btime ", 6) == 0) {
                char *end = NULL;

                errno = 0;
                value = strtoull(line + 6, &end, 10);
                if (end == line + 6 || errno != 0 || value > (uint64_t)INT64_MAX / 1000u) {
                    answer = hero_procs_fail(HERO_PROCS_OTHER, 0);
                } else {
                    *seconds = (int64_t)value;
                }
                break;
            }
            line = strchr(line, '\n');
            if (line != NULL) {
                line += 1;
            }
        }
        if (answer == 0 && line == NULL) {
            answer = hero_procs_fail(HERO_PROCS_OTHER, 0);
        }
    }
    hero_layer_free(bytes);
    return answer;
}

/* The `n`th field (3 for the state) of a `stat` line, whose fields after the
 * name are separated by one space each, as the number it is: false where the
 * line holds no such field or it is not digits. `from` is what stands right
 * after the name's closing parenthesis and the space. */
static bool hero_procs_stat_field(const char *from, int n, uint64_t *out) {
    const char *at = from;

    for (int field = 3; field < n; field += 1) {
        at = strchr(at, ' ');
        if (at == NULL) {
            return false;
        }
        at += 1;
    }
    if (*at < '0' || *at > '9') {
        return false;
    }
    errno = 0;
    {
        char *end = NULL;

        *out = strtoull(at, &end, 10);
        return end != at && errno == 0;
    }
}

/* The effective user of a process, the second number of the `Uid:` line of
 * its `status`: unlike the owner of its folder in `/proc`, which is root for
 * a process that cannot be dumped. */
static bool hero_procs_effective(const char *status, uint64_t *uid) {
    const char *line = status;

    while (line != NULL && *line != '\0') {
        if (strncmp(line, "Uid:", 4) == 0) {
            char *end = NULL;
            const char *at = line + 4;

            errno = 0;
            (void)strtoull(at, &end, 10);
            if (end == at || errno != 0) {
                return false;
            }
            *uid = strtoull(end, &end, 10);
            return errno == 0;
        }
        line = strchr(line, '\n');
        if (line != NULL) {
            line += 1;
        }
    }
    return false;
}

/* The resident memory of the process `pid` in bytes, into `*bytes`, read as
 * `hero_sys_process_usage` reads it (`hero_sys.c`, `hero_sys_proc_resident`,
 * which says why never the 24th field of `stat`): the `Rss:` line of its
 * `smaps_rollup`, in KiB, where this caller may read that file and it holds
 * one; else the second field of its `statm`, in pages. 1 where one was read,
 * 0 where the process is gone or hidden from this caller, -1 with the kind
 * set. */
static int hero_procs_resident(int32_t pid, uint64_t page, uint64_t *bytes) {
    char path[64];
    char text[HERO_PROCS_ROLLUP];
    size_t length = 0;
    int read = 0;

    /* A rollup this buffer cannot hold, or one whose reading fails, is
     * passed over for `statm`, whose answer the listing then carries. */
    (void)snprintf(path, sizeof path, "/proc/%d/smaps_rollup", (int)pid);
    read = hero_procs_small(path, text, sizeof text, &length);
    if (read > 0) {
        const char *line = text;

        while (line != NULL && *line != '\0') {
            if (strncmp(line, "Rss:", 4) == 0) {
                char *end = NULL;
                uint64_t kib = 0;

                errno = 0;
                kib = strtoull(line + 4, &end, 10);
                if (end != line + 4 && errno == 0 && !__builtin_mul_overflow(kib, (uint64_t)1024, bytes)) {
                    return 1;
                }
                break;
            }
            line = strchr(line, '\n');
            if (line != NULL) {
                line += 1;
            }
        }
    }
    (void)snprintf(path, sizeof path, "/proc/%d/statm", (int)pid);
    read = hero_procs_small(path, text, sizeof text, &length);
    if (read <= 0) {
        return read;
    }
    {
        char *end = NULL;
        const char *at = text;
        uint64_t pages = 0;

        errno = 0;
        (void)strtoull(at, &end, 10);
        if (end == at || errno != 0) {
            return 0;
        }
        at = end;
        pages = strtoull(at, &end, 10);
        if (end == at || errno != 0 || __builtin_mul_overflow(pages, page, bytes)) {
            return 0;
        }
    }
    return 1;
}

/* What `/proc/<pid>` tells about the process, read into `row`: 1 where it is
 * whole, 0 where the process ended or is hidden from this caller, -1 with the
 * kind set. */
static int hero_procs_read(int32_t pid, int64_t boot_seconds, uint64_t ticks_per_second, uint64_t page, hero_procs_owners *owners, hero_procs_row *row) {
    char path[64];
    char text[HERO_PROCS_FILE];
    size_t length = 0;
    const char *open_paren = NULL;
    const char *close_paren = NULL;
    uint64_t parent = 0;
    uint64_t user_ticks = 0;
    uint64_t system_ticks = 0;
    uint64_t start_ticks = 0;
    uint64_t virtual_bytes = 0;
    uint64_t uid = 0;
    uint64_t resident = 0;
    int64_t started = 0;
    int64_t in_user = 0;
    int64_t in_system = 0;
    int read = 0;
    size_t comm = 0;

    hero_procs_row_clear(row);
    (void)snprintf(path, sizeof path, "/proc/%d/stat", (int)pid);
    read = hero_procs_small(path, text, sizeof text, &length);
    if (read <= 0) {
        return read;
    }
    open_paren = strchr(text, '(');
    close_paren = strrchr(text, ')');
    if (open_paren == NULL || close_paren == NULL || close_paren < open_paren || close_paren[1] != ' ') {
        return 0;
    }
    if (!hero_procs_stat_field(close_paren + 2, 4, &parent) || !hero_procs_stat_field(close_paren + 2, 14, &user_ticks) || !hero_procs_stat_field(close_paren + 2, 15, &system_ticks) || !hero_procs_stat_field(close_paren + 2, 22, &start_ticks) || !hero_procs_stat_field(close_paren + 2, 23, &virtual_bytes)) {
        return 0;
    }
    read = hero_procs_resident(pid, page, &resident);
    if (read <= 0) {
        return read;
    }
    if (parent > (uint64_t)INT64_MAX || virtual_bytes > (uint64_t)INT64_MAX || resident > (uint64_t)INT64_MAX) {
        return 0;
    }
    if (!hero_procs_scaled(start_ticks, 1000, ticks_per_second, &started) || __builtin_add_overflow(started, boot_seconds * 1000, &started) || !hero_procs_scaled(user_ticks, 1000000000u, ticks_per_second, &in_user) || !hero_procs_scaled(system_ticks, 1000000000u, ticks_per_second, &in_system)) {
        return 0;
    }
    row->id = (int64_t)pid;
    row->parent = (int64_t)parent;
    row->started_ms = started;
    row->usage = true;
    row->resident = (int64_t)resident;
    row->virtual_bytes = (int64_t)virtual_bytes;
    row->user_ns = in_user;
    row->system_ns = in_system;
    comm = (size_t)(close_paren - open_paren - 1);
    hero_procs_copy(row->name, open_paren + 1, comm);
    /* The kernel keeps fifteen bytes of the name an executable was started
     * under: a name that long may be cut, and the file's own name says. */
    if (comm >= 15) {
        char target[HERO_PROCS_TEXT];
        ssize_t linked = 0;

        (void)snprintf(path, sizeof path, "/proc/%d/exe", (int)pid);
        linked = readlink(path, target, sizeof target - 1);
        if (linked > 0) {
            static const char deleted[] = " (deleted)";

            target[linked] = '\0';
            if ((size_t)linked > sizeof deleted - 1 && strcmp(target + linked - (sizeof deleted - 1), deleted) == 0) {
                target[linked - (ssize_t)(sizeof deleted - 1)] = '\0';
            }
            hero_procs_longer(row->name, hero_procs_base(target));
        }
        if (strlen(row->name) == comm) {
            (void)snprintf(path, sizeof path, "/proc/%d/cmdline", (int)pid);
            read = hero_procs_small(path, text, sizeof text, &length);
            if (read < 0) {
                return -1;
            }
            if (read > 0 && length > 0) {
                text[length] = '\0';
                hero_procs_longer(row->name, hero_procs_base(text));
            }
        }
    }
    (void)snprintf(path, sizeof path, "/proc/%d/status", (int)pid);
    read = hero_procs_small(path, text, sizeof text, &length);
    if (read < 0) {
        return -1;
    }
    if (read == 0) {
        /* The process ended between its two files. */
        return 0;
    }
    if (hero_procs_effective(text, &uid) && uid <= (uint64_t)UINT32_MAX) {
        hero_procs_owned(owners, (uid_t)uid, row);
    }
    return 1;
}

/* `/proc`'s folders named for an id, sorted. A process of another user is not
 * listed where `/proc` is mounted with `hidepid=2`, and listed but not read
 * under `hidepid=1`. */
static int64_t hero_procs_collect(hero_procs_ids *ids) {
    DIR *proc = opendir("/proc");

    if (proc == NULL) {
        int code = errno;

        return hero_procs_fail(hero_procs_kind_of(code), code);
    }
    for (;;) {
        struct dirent *entry = NULL;
        const char *name = NULL;
        uint64_t id = 0;
        size_t digits = 0;

        errno = 0;
        entry = readdir(proc);
        if (entry == NULL) {
            int code = errno;

            (void)closedir(proc);
            return code == 0 ? 0 : hero_procs_fail(hero_procs_kind_of(code), code);
        }
        name = entry->d_name;
        while (name[digits] >= '0' && name[digits] <= '9') {
            digits += 1;
        }
        if (digits == 0 || name[digits] != '\0' || digits > 10 || name[0] == '0') {
            continue;
        }
        for (size_t i = 0; i < digits; i += 1) {
            id = id * 10u + (uint64_t)(name[i] - '0');
        }
        if (id > (uint64_t)INT32_MAX) {
            continue;
        }
        if (!hero_procs_ids_push(ids, (int32_t)id)) {
            (void)closedir(proc);
            return hero_procs_fail(HERO_PROCS_LIMIT, ENOMEM);
        }
    }
}

static int64_t hero_procs_walk(hero_procs_fill *f) {
    hero_procs_ids ids = {NULL, 0, 0};
    hero_procs_owners *owners = NULL;
    hero_procs_row *row = NULL;
    int64_t boot = 0;
    long ticks = sysconf(_SC_CLK_TCK);
    long page = sysconf(_SC_PAGESIZE);
    int64_t answer = 0;

    if (ticks <= 0 || page <= 0) {
        return hero_procs_fail(HERO_PROCS_OTHER, 0);
    }
    if (hero_procs_boot(&boot) != 0) {
        return -1;
    }
    if (hero_procs_collect(&ids) != 0) {
        hero_procs_ids_free(&ids);
        return -1;
    }
    owners = hero_layer_alloc_zero(1, sizeof *owners);
    row = hero_layer_alloc(sizeof *row);
    if (owners == NULL || row == NULL) {
        answer = hero_procs_fail(HERO_PROCS_LIMIT, ENOMEM);
    }
    if (answer == 0 && ids.used > 1) {
        qsort(ids.items, ids.used, sizeof(int32_t), hero_procs_ids_order);
    }
    for (size_t i = 0; answer == 0 && i < ids.used; i += 1) {
        int read = hero_procs_read(ids.items[i], boot, (uint64_t)ticks, (uint64_t)page, owners, row);

        if (read < 0) {
            answer = -1;
        } else if (read > 0) {
            hero_procs_emit(f, row);
        }
    }
    if (owners != NULL) {
        hero_layer_free(owners);
    }
    if (row != NULL) {
        hero_layer_free(row);
    }
    hero_procs_ids_free(&ids);
    return answer;
}

#else

static int64_t hero_procs_walk(hero_procs_fill *f) {
    (void)f;
    return hero_procs_fail(HERO_PROCS_UNSUPPORTED, 0);
}

#endif

int64_t hero_procs_list(void *buffer, uint64_t count) {
    hero_procs_fill f = hero_procs_into(buffer, count);

    if (buffer == NULL && count != 0) {
        return hero_procs_fail(HERO_PROCS_INVALID, 0);
    }
    if (hero_procs_walk(&f) != 0) {
        return -1;
    }
    return hero_procs_filled(&f);
}
