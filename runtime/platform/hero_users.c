/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_users.c — the platform layer's users and groups, one member of
 * the archive the compiler puts on every program's link line (M-rebel-library,
 * gap G12). Its header, `hero_users.h`, says what each function answers; this
 * file says how each platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * NOTHING SURVIVES BETWEEN CALLS BUT THE LAST ERROR, which is per thread: no
 * table and no lock. What one call needs (the room a system's record of a user
 * is read into, which a directory service may make large) it takes through
 * `parts/alloc.c`, the single allocation point (the net's `runtime` suite,
 * rule 1), and gives back before it returns. An answer is built whole in the
 * fill form before it is reported, so a failure leaves no half of one.
 */

#include "hero_users.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <lm.h>
#include <sddl.h>
#include <wchar.h>
/* The libraries the member needs, named by the member, as `hero_sys.c` names
 * its own: `advapi32` holds the account lookups, the profile list's registry
 * key and the SID text, `netapi32` a user's primary group and local groups.
 * MSVC's word, a MinGW target links by `-l`. */
#if defined(_MSC_VER)
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")
#endif
#else
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

static _Thread_local int64_t hero_users_kind;
static _Thread_local int64_t hero_users_os;

static void hero_users_fine(void) {
    hero_users_kind = HERO_USERS_OK;
    hero_users_os = 0;
}

static int64_t hero_users_fail(int64_t kind, int64_t os) {
    hero_users_kind = kind;
    hero_users_os = os;
    return -1;
}

int64_t hero_users_last_error(void) {
    return hero_users_kind;
}

int64_t hero_users_last_os_error(void) {
    return hero_users_os;
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_users_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_ACCESS_DENIED:
        return HERO_USERS_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case NERR_BufTooSmall:
        return HERO_USERS_LIMIT;
    case ERROR_NONE_MAPPED:
    case NERR_UserNotFound:
        return HERO_USERS_NOT_FOUND;
    case ERROR_NOT_SUPPORTED:
        return HERO_USERS_UNSUPPORTED;
#else
    case EACCES:
    case EPERM:
        return HERO_USERS_DENIED;
    case ENOMEM:
    case EMFILE:
    case ENFILE:
    case EAGAIN:
        return HERO_USERS_LIMIT;
    case ENOSYS:
    case ENOTSUP:
        return HERO_USERS_UNSUPPORTED;
#endif
    default:
        return HERO_USERS_OTHER;
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
} hero_users_fill;

static hero_users_fill hero_users_into(void *buffer, uint64_t count) {
    hero_users_fill f;

    f.room = buffer;
    f.count = count;
    f.used = 0;
    f.over = false;
    return f;
}

/* `n` more bytes of the answer: written where all of them fit behind the ones
 * before, counted either way, so nothing lands past the buffer. */
static void hero_users_put(hero_users_fill *f, const void *bytes, size_t n) {
    if (n > 0 && f->used <= f->count && (uint64_t)n <= f->count - f->used) {
        memcpy(f->room + f->used, bytes, n);
    }
    if (__builtin_add_overflow(f->used, (uint64_t)n, &f->used)) {
        f->over = true;
    }
}

/* What a fill form answers: the bytes of the whole answer. */
static int64_t hero_users_filled(const hero_users_fill *f) {
    if (f->over || f->used > (uint64_t)INT64_MAX) {
        return hero_users_fail(HERO_USERS_LIMIT, 0);
    }
    hero_users_fine();
    return (int64_t)f->used;
}

/* One field: `n` bytes and the zero byte that ends them; an empty field is
 * `n` of 0, the zero byte alone. */
static void hero_users_field(hero_users_fill *f, const char *bytes, size_t n) {
    hero_users_put(f, bytes, n);
    hero_users_put(f, "", 1);
}

/* What a call needs of its arguments before it asks the system anything. */
static bool hero_users_room(const char *key, const void *buffer, uint64_t count) {
    return key != NULL && key[0] != '\0' && (buffer != NULL || count == 0);
}

#if !defined(_WIN32)

static void hero_users_decimal(hero_users_fill *f, uint64_t value) {
    char digits[21];
    size_t n = 0;

    do {
        digits[sizeof digits - 2 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    digits[sizeof digits - 1] = '\0';
    hero_users_put(f, digits + sizeof digits - 1 - n, n + 1);
}

/* The most room a record of a user or a group is read into: the system tells
 * `ERANGE` while the room is short, and a group with every user of a large
 * directory in it is some megabytes. */
#define HERO_USERS_RECORD_MOST ((size_t)16 << 20)

typedef struct {
    char *room;
    size_t size;
} hero_users_scratch;

/* The room doubled (4096 bytes the first time), the old bytes not kept: the
 * record is read again. False where it would pass the most or the allocation
 * fails. */
static bool hero_users_widen(hero_users_scratch *s) {
    size_t size = s->size == 0 ? 4096 : s->size * 2;

    if (size > HERO_USERS_RECORD_MOST) {
        return false;
    }
    if (s->room != NULL) {
        hero_layer_free(s->room);
        s->room = NULL;
        s->size = 0;
    }
    s->room = hero_layer_alloc(size);
    if (s->room == NULL) {
        return false;
    }
    s->size = size;
    return true;
}

static void hero_users_scratch_free(hero_users_scratch *s) {
    if (s->room != NULL) {
        hero_layer_free(s->room);
    }
    s->room = NULL;
    s->size = 0;
}

/* The id `text` writes, digits alone, as the 32-bit number POSIX ids are:
 * false for anything else. */
static bool hero_users_number(const char *text, uint64_t *out) {
    uint64_t value = 0;
    size_t n = 0;

    for (; text[n] >= '0' && text[n] <= '9'; n += 1) {
        value = value * 10u + (uint64_t)(text[n] - '0');
        if (n >= 10) {
            return false;
        }
    }
    if (n == 0 || text[n] != '\0' || value > (uint64_t)UINT32_MAX) {
        return false;
    }
    *out = value;
    return true;
}

/* Whether the number a failed lookup left says that the user is not there:
 * POSIX lets `getpwnam_r` and its kin tell a missing record by these as well
 * as by no result. */
static bool hero_users_absent(int code) {
    return code == 0 || code == ENOENT || code == ESRCH || code == EBADF || code == EPERM;
}

/* The record of the user a lookup names, found and copied: its name, home and
 * ids. Answers 1 where found, 0 where there is none, -1 with the kind set. The
 * name and the home are copied into `name` and `home`, `size` bytes each, and
 * cut at that. */
typedef struct {
    uint32_t uid;
    uint32_t gid;
    char name[1024];
    char home[4096];
} hero_users_user;

static int hero_users_find_user(const char *key, bool by_id, hero_users_user *out) {
    uint64_t number = 0;
    hero_users_scratch s = {NULL, 0};

    if (by_id && !hero_users_number(key, &number)) {
        return (int)hero_users_fail(HERO_USERS_INVALID, 0);
    }
    for (;;) {
        struct passwd record;
        struct passwd *result = NULL;
        int code = 0;

        if (!hero_users_widen(&s)) {
            hero_users_scratch_free(&s);
            return (int)hero_users_fail(HERO_USERS_LIMIT, ENOMEM);
        }
        memset(&record, 0, sizeof record);
        code = by_id ? getpwuid_r((uid_t)number, &record, s.room, s.size, &result) : getpwnam_r(key, &record, s.room, s.size, &result);
        if (code == ERANGE) {
            continue;
        }
        if (code != 0 && !hero_users_absent(code)) {
            hero_users_scratch_free(&s);
            return (int)hero_users_fail(hero_users_kind_of(code), code);
        }
        if (result == NULL) {
            hero_users_scratch_free(&s);
            return 0;
        }
        out->uid = (uint32_t)record.pw_uid;
        out->gid = (uint32_t)record.pw_gid;
        out->name[0] = '\0';
        out->home[0] = '\0';
        if (record.pw_name != NULL) {
            strncat(out->name, record.pw_name, sizeof out->name - 1);
        }
        if (record.pw_dir != NULL) {
            strncat(out->home, record.pw_dir, sizeof out->home - 1);
        }
        hero_users_scratch_free(&s);
        return 1;
    }
}

typedef struct {
    uint32_t gid;
    char name[1024];
} hero_users_group;

/* The record of the group a lookup names, as above. */
static int hero_users_find_group(const char *key, bool by_id, hero_users_group *out) {
    uint64_t number = 0;
    hero_users_scratch s = {NULL, 0};

    if (by_id && !hero_users_number(key, &number)) {
        return (int)hero_users_fail(HERO_USERS_INVALID, 0);
    }
    for (;;) {
        struct group record;
        struct group *result = NULL;
        int code = 0;

        if (!hero_users_widen(&s)) {
            hero_users_scratch_free(&s);
            return (int)hero_users_fail(HERO_USERS_LIMIT, ENOMEM);
        }
        memset(&record, 0, sizeof record);
        code = by_id ? getgrgid_r((gid_t)number, &record, s.room, s.size, &result) : getgrnam_r(key, &record, s.room, s.size, &result);
        if (code == ERANGE) {
            continue;
        }
        if (code != 0 && !hero_users_absent(code)) {
            hero_users_scratch_free(&s);
            return (int)hero_users_fail(hero_users_kind_of(code), code);
        }
        if (result == NULL) {
            hero_users_scratch_free(&s);
            return 0;
        }
        out->gid = (uint32_t)record.gr_gid;
        out->name[0] = '\0';
        if (record.gr_name != NULL) {
            strncat(out->name, record.gr_name, sizeof out->name - 1);
        }
        hero_users_scratch_free(&s);
        return 1;
    }
}

static int64_t hero_users_user_of(const char *key, bool by_id, void *buffer, uint64_t count) {
    hero_users_fill f = hero_users_into(buffer, count);
    hero_users_user found;
    int read = 0;

    if (!hero_users_room(key, buffer, count)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    read = hero_users_find_user(key, by_id, &found);
    if (read < 0) {
        return -1;
    }
    if (read == 0) {
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    hero_users_decimal(&f, found.uid);
    hero_users_field(&f, found.name, strlen(found.name));
    hero_users_decimal(&f, found.gid);
    hero_users_field(&f, found.home, strlen(found.home));
    return hero_users_filled(&f);
}

static int64_t hero_users_group_of(const char *key, bool by_id, void *buffer, uint64_t count) {
    hero_users_fill f = hero_users_into(buffer, count);
    hero_users_group found;
    int read = 0;

    if (!hero_users_room(key, buffer, count)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    read = hero_users_find_group(key, by_id, &found);
    if (read < 0) {
        return -1;
    }
    if (read == 0) {
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    hero_users_decimal(&f, found.gid);
    hero_users_field(&f, found.name, strlen(found.name));
    return hero_users_filled(&f);
}

int64_t hero_users_user_named(const char *name, void *buffer, uint64_t count) {
    return hero_users_user_of(name, false, buffer, count);
}

int64_t hero_users_user_identified(const char *id, void *buffer, uint64_t count) {
    return hero_users_user_of(id, true, buffer, count);
}

int64_t hero_users_group_named(const char *name, void *buffer, uint64_t count) {
    return hero_users_group_of(name, false, buffer, count);
}

int64_t hero_users_group_identified(const char *id, void *buffer, uint64_t count) {
    return hero_users_group_of(id, true, buffer, count);
}

/* The system's own slot for a group id in `getgrouplist`: Darwin's is an
 * `int`, every other system's a `gid_t`. */
#if defined(__APPLE__)
typedef int hero_users_slot;
#else
typedef gid_t hero_users_slot;
#endif

/* The most groups one user is read with. Linux allows 65,536 a process. */
#define HERO_USERS_GROUPS_MOST ((size_t)1 << 20)

typedef struct {
    uint32_t gid;
    size_t at;
} hero_users_ranked;

static int hero_users_ranked_order(const void *left, const void *right) {
    const hero_users_ranked *a = left;
    const hero_users_ranked *b = right;

    if (a->gid != b->gid) {
        return a->gid < b->gid ? -1 : 1;
    }
    return (a->at > b->at) - (a->at < b->at);
}

/* The ids of the groups of the user named `name` whose primary group is
 * `base`, in the system's order and each once: `getgrouplist` into room that
 * is doubled until it holds them all. The ids are in `*ids`, counted in `*n`,
 * the caller's to give back with `hero_layer_free`. */
static int64_t hero_users_group_ids(const char *name, uint32_t base, uint32_t **ids, size_t *n) {
    size_t room = 64;
    hero_users_slot *slots = NULL;
    uint32_t *kept = NULL;
    hero_users_ranked *ranked = NULL;
    unsigned char *skip = NULL;
    size_t total = 0;
    size_t used = 0;
    int64_t answer = 0;

    for (;;) {
        int count = (int)room;
        int got = 0;

        if (room > HERO_USERS_GROUPS_MOST || room > (size_t)INT32_MAX || room > SIZE_MAX / sizeof *slots) {
            return hero_users_fail(HERO_USERS_LIMIT, 0);
        }
        slots = hero_layer_alloc(room * sizeof *slots);
        if (slots == NULL) {
            return hero_users_fail(HERO_USERS_LIMIT, ENOMEM);
        }
        got = getgrouplist(name, (hero_users_slot)base, slots, &count);
        if (got >= 0 && count >= 0 && (size_t)count <= room) {
            total = (size_t)count;
            break;
        }
        hero_layer_free(slots);
        slots = NULL;
        room = (count > 0 && (size_t)count > room) ? (size_t)count : room * 2;
    }
    kept = hero_layer_alloc((total == 0 ? 1 : total) * sizeof *kept);
    ranked = hero_layer_alloc((total == 0 ? 1 : total) * sizeof *ranked);
    skip = hero_layer_alloc_zero(total == 0 ? 1 : total, 1);
    if (kept == NULL || ranked == NULL || skip == NULL) {
        answer = hero_users_fail(HERO_USERS_LIMIT, ENOMEM);
    } else {
        for (size_t i = 0; i < total; i += 1) {
            ranked[i].gid = (uint32_t)slots[i];
            ranked[i].at = i;
        }
        qsort(ranked, total, sizeof *ranked, hero_users_ranked_order);
        /* Of the entries of one id, the first in the system's order is kept. */
        for (size_t i = 1; i < total; i += 1) {
            if (ranked[i].gid == ranked[i - 1].gid) {
                skip[ranked[i].at] = 1;
            }
        }
        for (size_t i = 0; i < total; i += 1) {
            if (skip[i] == 0) {
                kept[used] = (uint32_t)slots[i];
                used += 1;
            }
        }
    }
    if (slots != NULL) {
        hero_layer_free(slots);
    }
    if (ranked != NULL) {
        hero_layer_free(ranked);
    }
    if (skip != NULL) {
        hero_layer_free(skip);
    }
    if (answer != 0) {
        if (kept != NULL) {
            hero_layer_free(kept);
        }
        return -1;
    }
    *ids = kept;
    *n = used;
    return 0;
}

/* `value` as decimal digits in `out`, 11 bytes at least, a C string. */
static void hero_users_gid_text(uint32_t value, char *out) {
    char reversed[11];
    size_t r = 0;
    size_t at = 0;

    do {
        reversed[r++] = (char)('0' + (int)(value % 10u));
        value /= 10u;
    } while (value > 0);
    while (r > 0) {
        out[at++] = reversed[--r];
    }
    out[at] = '\0';
}

int64_t hero_users_groups_of(const char *name, void *buffer, uint64_t count) {
    hero_users_fill f = hero_users_into(buffer, count);
    hero_users_user user;
    uint32_t *ids = NULL;
    size_t n = 0;
    int read = 0;

    if (!hero_users_room(name, buffer, count)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    read = hero_users_find_user(name, false, &user);
    if (read < 0) {
        return -1;
    }
    if (read == 0) {
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    if (hero_users_group_ids(name, user.gid, &ids, &n) != 0) {
        return -1;
    }
    for (size_t i = 0; i < n; i += 1) {
        hero_users_group group;
        char text[16];
        int found = 0;

        hero_users_gid_text(ids[i], text);
        found = hero_users_find_group(text, true, &group);
        if (found < 0) {
            hero_layer_free(ids);
            return -1;
        }
        hero_users_decimal(&f, ids[i]);
        hero_users_field(&f, found > 0 ? group.name : "", found > 0 ? strlen(group.name) : 0);
    }
    hero_layer_free(ids);
    return hero_users_filled(&f);
}

int64_t hero_users_change(const char *name) {
    hero_users_user user;
    int read = 0;

    if (!hero_users_room(name, NULL, 0)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    read = hero_users_find_user(name, false, &user);
    if (read < 0) {
        return -1;
    }
    if (read == 0) {
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    if (geteuid() != 0) {
        /* Nothing to give up and nothing the system lets this process take. */
        if ((uint32_t)getuid() == user.uid && (uint32_t)geteuid() == user.uid && (uint32_t)getgid() == user.gid && (uint32_t)getegid() == user.gid) {
            hero_users_fine();
            return 0;
        }
        return hero_users_fail(HERO_USERS_DENIED, EPERM);
    }
    /* Groups first and the user last: a process that has given up root cannot
     * set its groups. */
    if (initgroups(name, (hero_users_slot)user.gid) != 0) {
        int code = errno;

        return hero_users_fail(hero_users_kind_of(code), code);
    }
    if (setgid((gid_t)user.gid) != 0) {
        int code = errno;

        return hero_users_fail(hero_users_kind_of(code), code);
    }
    if (setuid((uid_t)user.uid) != 0) {
        int code = errno;

        return hero_users_fail(hero_users_kind_of(code), code);
    }
    if ((uint32_t)getuid() != user.uid || (uint32_t)geteuid() != user.uid || (uint32_t)getgid() != user.gid || (uint32_t)getegid() != user.gid) {
        return hero_users_fail(HERO_USERS_OTHER, 0);
    }
    /* A process whose saved id is still root could take it back: where the
     * user is not root, asking for root must be refused, or the drop did not
     * hold. */
    if (user.uid != 0 && (setuid(0) == 0 || seteuid(0) == 0)) {
        return hero_users_fail(HERO_USERS_OTHER, 0);
    }
    hero_users_fine();
    return 0;
}

#else

/* The units `text` holds as a C string of UTF-16, strictly: false, with the
 * kind set, for bytes that are not UTF-8. The memory is the caller's to give
 * back with `hero_layer_free`. */
static WCHAR *hero_users_utf16(const char *text) {
    int units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);
    WCHAR *wide = NULL;

    if (units <= 0) {
        hero_users_fail(HERO_USERS_INVALID, (int64_t)GetLastError());
        return NULL;
    }
    wide = hero_layer_alloc((size_t)units * sizeof *wide);
    if (wide == NULL) {
        hero_users_fail(HERO_USERS_LIMIT, ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, wide, units) != units) {
        hero_layer_free(wide);
        hero_users_fail(HERO_USERS_OTHER, (int64_t)GetLastError());
        return NULL;
    }
    return wide;
}

/* `units` UTF-16 units written as one field: a pair as its one character, and
 * a unit that is half of no pair as the three bytes its own number takes, so
 * nothing is replaced (as `hero_sys.c` writes its own). */
static void hero_users_wide(hero_users_fill *f, const WCHAR *wide, size_t units) {
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
        hero_users_put(f, b, n);
    }
    hero_users_put(f, "", 1);
}

/* An account: its SID, in memory the caller gives back, what the system says
 * it is, and its name without a domain. */
typedef struct {
    PSID sid;
    SID_NAME_USE use;
    WCHAR name[1024];
    size_t name_units;
} hero_users_account;

static void hero_users_account_free(hero_users_account *a) {
    if (a->sid != NULL) {
        hero_layer_free(a->sid);
    }
    a->sid = NULL;
}

/* The name of the account `a.sid` is, read into `a`, and what kind of account
 * it is; false with the kind set where the system gives none. */
static bool hero_users_account_named(hero_users_account *a) {
    WCHAR domain[1024];
    DWORD name_units = (DWORD)(sizeof a->name / sizeof a->name[0]);
    DWORD domain_units = (DWORD)(sizeof domain / sizeof domain[0]);

    if (!LookupAccountSidW(NULL, a->sid, a->name, &name_units, domain, &domain_units, &a->use)) {
        DWORD code = GetLastError();

        hero_users_fail(hero_users_kind_of((int64_t)code), (int64_t)code);
        return false;
    }
    a->name_units = (size_t)name_units;
    return true;
}

/* The account `wname` names (`user`, `DOMAIN\user`, `user@domain`): 1 where
 * found and read into `a`, 0 where none has the name, -1 with the kind set. */
static int hero_users_find_by_name(const WCHAR *wname, hero_users_account *a) {
    DWORD sid_bytes = 0;
    DWORD domain_units = 0;
    WCHAR *domain = NULL;
    BOOL got = FALSE;
    DWORD code = 0;

    memset(a, 0, sizeof *a);
    (void)LookupAccountNameW(NULL, wname, NULL, &sid_bytes, NULL, &domain_units, &a->use);
    code = GetLastError();
    if (code == ERROR_NONE_MAPPED) {
        return 0;
    }
    if (code != ERROR_INSUFFICIENT_BUFFER || sid_bytes == 0) {
        return (int)hero_users_fail(hero_users_kind_of((int64_t)code), (int64_t)code);
    }
    a->sid = hero_layer_alloc(sid_bytes);
    domain = hero_layer_alloc(((size_t)domain_units + 1) * sizeof *domain);
    if (a->sid == NULL || domain == NULL) {
        hero_users_account_free(a);
        if (domain != NULL) {
            hero_layer_free(domain);
        }
        return (int)hero_users_fail(HERO_USERS_LIMIT, ERROR_NOT_ENOUGH_MEMORY);
    }
    got = LookupAccountNameW(NULL, wname, a->sid, &sid_bytes, domain, &domain_units, &a->use);
    code = GetLastError();
    hero_layer_free(domain);
    if (!got) {
        hero_users_account_free(a);
        return code == ERROR_NONE_MAPPED ? 0 : (int)hero_users_fail(hero_users_kind_of((int64_t)code), (int64_t)code);
    }
    if (!hero_users_account_named(a)) {
        hero_users_account_free(a);
        return -1;
    }
    return 1;
}

/* The account whose security identifier `text` writes: 1, 0 or -1 as above;
 * `HERO_USERS_INVALID` for text that is no SID. */
static int hero_users_find_by_id(const char *text, hero_users_account *a) {
    WCHAR *wide = hero_users_utf16(text);
    PSID given = NULL;
    DWORD bytes = 0;

    memset(a, 0, sizeof *a);
    if (wide == NULL) {
        return -1;
    }
    if (!ConvertStringSidToSidW(wide, &given)) {
        hero_layer_free(wide);
        return (int)hero_users_fail(HERO_USERS_INVALID, (int64_t)GetLastError());
    }
    hero_layer_free(wide);
    bytes = GetLengthSid(given);
    a->sid = hero_layer_alloc(bytes);
    if (a->sid == NULL || !CopySid(bytes, a->sid, given)) {
        hero_users_account_free(a);
        (void)LocalFree(given);
        return (int)hero_users_fail(HERO_USERS_LIMIT, ERROR_NOT_ENOUGH_MEMORY);
    }
    (void)LocalFree(given);
    if (!hero_users_account_named(a)) {
        DWORD code = (DWORD)hero_users_os;
        int none = code == ERROR_NONE_MAPPED;

        hero_users_account_free(a);
        return none ? 0 : -1;
    }
    return 1;
}

/* Whether `a` is a user: an account of kind user, or one of the service
 * accounts (`S-1-5-18`, `-19`, `-20`) the system counts as well-known groups
 * and that own processes. */
static bool hero_users_is_user(const hero_users_account *a) {
    SID_IDENTIFIER_AUTHORITY nt = SECURITY_NT_AUTHORITY;

    if (a->use == SidTypeUser) {
        return true;
    }
    if (a->use == SidTypeWellKnownGroup && memcmp(GetSidIdentifierAuthority(a->sid), &nt, sizeof nt) == 0 && *GetSidSubAuthorityCount(a->sid) == 1) {
        DWORD rid = *GetSidSubAuthority(a->sid, 0);

        return rid == 18 || rid == 19 || rid == 20;
    }
    return false;
}

static bool hero_users_is_group(const hero_users_account *a) {
    return a->use == SidTypeGroup || a->use == SidTypeAlias || a->use == SidTypeWellKnownGroup;
}

/* The text of `sid` as one field: `S-1-5-...`. */
static void hero_users_sid_field(hero_users_fill *f, PSID sid) {
    WCHAR *text = NULL;

    if (!ConvertSidToStringSidW(sid, &text)) {
        hero_users_field(f, "", 0);
        return;
    }
    hero_users_wide(f, text, wcslen(text));
    (void)LocalFree(text);
}

/* The primary group of the user `a` as one field, empty where the machine's own
 * accounts do not hold it: the group's relative id, which `NetUserGetInfo` tells,
 * in the place of the user's own in the user's SID. */
static void hero_users_group_field(hero_users_fill *f, const hero_users_account *a) {
    USER_INFO_4 *info = NULL;
    PSID group = NULL;
    DWORD bytes = 0;
    UCHAR parts = 0;

    if (a->use != SidTypeUser || NetUserGetInfo(NULL, a->name, 4, (LPBYTE *)&info) != NERR_Success || info == NULL) {
        hero_users_field(f, "", 0);
        return;
    }
    bytes = GetLengthSid(a->sid);
    parts = *GetSidSubAuthorityCount(a->sid);
    group = hero_layer_alloc(bytes);
    if (group == NULL || parts == 0 || !CopySid(bytes, group, a->sid)) {
        if (group != NULL) {
            hero_layer_free(group);
        }
        (void)NetApiBufferFree(info);
        hero_users_field(f, "", 0);
        return;
    }
    *GetSidSubAuthority(group, (DWORD)parts - 1) = info->usri4_primary_group_id;
    (void)NetApiBufferFree(info);
    hero_users_sid_field(f, group);
    hero_layer_free(group);
}

/* The user's home as one field: the folder of the profile the machine keeps
 * for the SID, empty where it keeps none (the user has never logged on). */
static void hero_users_home_field(hero_users_fill *f, PSID sid) {
    static const WCHAR base[] = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\";
    WCHAR key[512];
    WCHAR home[4096];
    DWORD bytes = (DWORD)sizeof home;
    WCHAR *text = NULL;

    if (!ConvertSidToStringSidW(sid, &text)) {
        hero_users_field(f, "", 0);
        return;
    }
    if (wcslen(text) + sizeof base / sizeof base[0] >= sizeof key / sizeof key[0]) {
        (void)LocalFree(text);
        hero_users_field(f, "", 0);
        return;
    }
    wcscpy(key, base);
    wcscat(key, text);
    (void)LocalFree(text);
    if (RegGetValueW(HKEY_LOCAL_MACHINE, key, L"ProfileImagePath", RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ, NULL, home, &bytes) != ERROR_SUCCESS || bytes < sizeof(WCHAR)) {
        hero_users_field(f, "", 0);
        return;
    }
    hero_users_wide(f, home, wcsnlen(home, sizeof home / sizeof home[0]));
}

static int hero_users_find(const char *key, bool by_id, hero_users_account *a) {
    WCHAR *wide = NULL;
    int found = 0;

    if (by_id) {
        return hero_users_find_by_id(key, a);
    }
    wide = hero_users_utf16(key);
    if (wide == NULL) {
        return -1;
    }
    found = hero_users_find_by_name(wide, a);
    hero_layer_free(wide);
    return found;
}

static int64_t hero_users_user_of(const char *key, bool by_id, void *buffer, uint64_t count) {
    hero_users_fill f = hero_users_into(buffer, count);
    hero_users_account a;
    int found = 0;

    if (!hero_users_room(key, buffer, count)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    found = hero_users_find(key, by_id, &a);
    if (found < 0) {
        return -1;
    }
    if (found == 0) {
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    if (!hero_users_is_user(&a)) {
        hero_users_account_free(&a);
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    hero_users_sid_field(&f, a.sid);
    hero_users_wide(&f, a.name, a.name_units);
    hero_users_group_field(&f, &a);
    hero_users_home_field(&f, a.sid);
    hero_users_account_free(&a);
    return hero_users_filled(&f);
}

static int64_t hero_users_group_of(const char *key, bool by_id, void *buffer, uint64_t count) {
    hero_users_fill f = hero_users_into(buffer, count);
    hero_users_account a;
    int found = 0;

    if (!hero_users_room(key, buffer, count)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    found = hero_users_find(key, by_id, &a);
    if (found < 0) {
        return -1;
    }
    if (found == 0) {
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    if (!hero_users_is_group(&a)) {
        hero_users_account_free(&a);
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    hero_users_sid_field(&f, a.sid);
    hero_users_wide(&f, a.name, a.name_units);
    hero_users_account_free(&a);
    return hero_users_filled(&f);
}

int64_t hero_users_user_named(const char *name, void *buffer, uint64_t count) {
    return hero_users_user_of(name, false, buffer, count);
}

int64_t hero_users_user_identified(const char *id, void *buffer, uint64_t count) {
    return hero_users_user_of(id, true, buffer, count);
}

int64_t hero_users_group_named(const char *name, void *buffer, uint64_t count) {
    return hero_users_group_of(name, false, buffer, count);
}

int64_t hero_users_group_identified(const char *id, void *buffer, uint64_t count) {
    return hero_users_group_of(id, true, buffer, count);
}

/* `NetUserGetLocalGroups` names the machine's groups the user belongs to,
 * through a group of a domain as well (`LG_INCLUDE_INDIRECT`); each name is
 * turned into its SID by the account lookup. A group that goes between the two
 * calls is left out. */
int64_t hero_users_groups_of(const char *name, void *buffer, uint64_t count) {
    hero_users_fill f = hero_users_into(buffer, count);
    hero_users_account user;
    LOCALGROUP_USERS_INFO_0 *groups = NULL;
    DWORD read = 0;
    DWORD total = 0;
    NET_API_STATUS status = 0;
    WCHAR *wide = NULL;
    int found = 0;

    if (!hero_users_room(name, buffer, count)) {
        return hero_users_fail(HERO_USERS_INVALID, 0);
    }
    wide = hero_users_utf16(name);
    if (wide == NULL) {
        return -1;
    }
    found = hero_users_find_by_name(wide, &user);
    if (found < 0) {
        hero_layer_free(wide);
        return -1;
    }
    if (found == 0 || !hero_users_is_user(&user)) {
        if (found > 0) {
            hero_users_account_free(&user);
        }
        hero_layer_free(wide);
        return hero_users_fail(HERO_USERS_NOT_FOUND, 0);
    }
    hero_users_account_free(&user);
    status = NetUserGetLocalGroups(NULL, wide, 0, LG_INCLUDE_INDIRECT, (LPBYTE *)&groups, MAX_PREFERRED_LENGTH, &read, &total);
    hero_layer_free(wide);
    if (status != NERR_Success) {
        return hero_users_fail(hero_users_kind_of((int64_t)status), (int64_t)status);
    }
    for (DWORD i = 0; i < read; i += 1) {
        hero_users_account group;
        int member = hero_users_find_by_name(groups[i].lgrui0_name, &group);

        if (member < 0) {
            (void)NetApiBufferFree(groups);
            return -1;
        }
        if (member == 0) {
            continue;
        }
        hero_users_sid_field(&f, group.sid);
        hero_users_wide(&f, group.name, group.name_units);
        hero_users_account_free(&group);
    }
    (void)NetApiBufferFree(groups);
    return hero_users_filled(&f);
}

int64_t hero_users_change(const char *name) {
    (void)name;
    return hero_users_fail(HERO_USERS_UNSUPPORTED, 0);
}

#endif
