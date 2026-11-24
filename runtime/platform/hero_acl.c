/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_acl.c — the platform layer's access control lists, one
 * member of the archive the compiler puts on every program's link line
 * (M-rebel-library, gap G06). Its header, `hero_acl.h`, says what a list
 * holds and in which form; this file says how each platform reads and
 * writes one.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * NOTHING SURVIVES BETWEEN CALLS BUT THE LAST ERROR, which is per thread. What
 * one call needs (the entries parsed, a Linux list's bytes, a Windows ACL)
 * it takes through `parts/alloc.c`, the single allocation point (the net's
 * `runtime` suite, rule 1), and gives back before it returns.
 *
 * A LIST IS JUDGED WHOLE BEFORE ANYTHING IS WRITTEN: every entry is parsed
 * and every entry the system cannot hold refused, by its place, before the
 * system is asked, so a refused list never leaves half of itself behind.
 */

#include "hero_acl.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <wchar.h>
#if defined(_MSC_VER)
#pragma comment(lib, "advapi32.lib")
#endif
#else
#include <errno.h>
#include <limits.h>
#if defined(__APPLE__) || defined(__linux__)
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#if defined(__APPLE__)
#include <membership.h>
#include <sys/acl.h>
#include <uuid/uuid.h>
#endif
#if defined(__linux__)
#include <sys/xattr.h>
#endif
#endif

static _Thread_local int64_t hero_acl_kind;
static _Thread_local int64_t hero_acl_os;
static _Thread_local int64_t hero_acl_reason;

static int64_t hero_acl_fail(int64_t kind, int64_t os) {
    hero_acl_kind = kind;
    hero_acl_os = os;
    hero_acl_reason = 0;
    return -1;
}

int64_t hero_acl_last_error(void) {
    return hero_acl_kind;
}

int64_t hero_acl_last_os_error(void) {
    return hero_acl_os;
}

int64_t hero_acl_last_reason(void) {
    return hero_acl_reason;
}

/* The helpers below serve the three arms that read and write; a POSIX
 * system with no access lists compiles none of them. */
#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)

static void hero_acl_fine(void) {
    hero_acl_kind = HERO_ACL_OK;
    hero_acl_os = 0;
    hero_acl_reason = 0;
}

/* The entry at `place`, from 1, cannot be held, for `reason`. */
static int64_t hero_acl_cannot(int64_t reason, int64_t place) {
    hero_acl_kind = HERO_ACL_CANNOT;
    hero_acl_os = place;
    hero_acl_reason = reason;
    return -1;
}

/* An answer on its way into the caller's buffer, counted whole. Once one
 * piece did not fit no later piece is written, so nothing lands past it. */
typedef struct {
    unsigned char *room;
    uint64_t count;
    uint64_t used;
} hero_acl_answer;

static void hero_acl_put(hero_acl_answer *a, const void *bytes, size_t n) {
    if (n > 0 && a->used <= a->count && (uint64_t)n <= a->count - a->used) {
        memcpy(a->room + a->used, bytes, n);
    }
    a->used += (uint64_t)n;
}

static void hero_acl_field(hero_acl_answer *a, const char *text, size_t n) {
    hero_acl_put(a, text, n);
    hero_acl_put(a, "", 1);
}

static void hero_acl_text(hero_acl_answer *a, const char *text) {
    hero_acl_field(a, text, strlen(text));
}

static void hero_acl_decimal(hero_acl_answer *a, uint64_t value) {
    char digits[21];
    size_t n = 0;

    do {
        digits[sizeof digits - 1 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    hero_acl_field(a, digits + sizeof digits - n, n);
}

/* One entry, as the header's form writes it. */
static void hero_acl_entry_out(hero_acl_answer *a, int64_t who, bool deny, uint64_t rights, uint64_t flags,
                               const char *id, const char *name) {
    hero_acl_decimal(a, (uint64_t)who);
    hero_acl_text(a, deny ? "1" : "0");
    hero_acl_decimal(a, rights);
    hero_acl_decimal(a, flags);
    hero_acl_text(a, id);
    hero_acl_text(a, name);
}

/* One entry of a list handed to `hero_acl_write`. `id` points into the
 * caller's bytes and is `id_length` long, with its zero after it. */
typedef struct {
    int64_t who;
    bool deny;
    uint64_t rights;
    uint64_t flags;
    const char *id;
    size_t id_length;
} hero_acl_entry;

/* A list handed to `hero_acl_write`, parsed. */
typedef struct {
    bool protected_;
    hero_acl_entry *entries;
    size_t count;
} hero_acl_list;

/* The next field of `bytes` from `*at`: where it starts and how long it is;
 * false where no zero byte ends it. */
static bool hero_acl_next(const unsigned char *bytes, uint64_t size, uint64_t *at, const char **field,
                          size_t *length) {
    const unsigned char *end;

    if (*at >= size) {
        return false;
    }
    end = (const unsigned char *)memchr(bytes + *at, 0, (size_t)(size - *at));
    if (end == NULL) {
        return false;
    }
    *field = (const char *)(bytes + *at);
    *length = (size_t)(end - (bytes + *at));
    *at += (uint64_t)*length + 1;
    return true;
}

/* A field of decimal digits as a number at most `most`; false for anything
 * else, an empty field among them. */
static bool hero_acl_number(const char *field, size_t length, uint64_t most, uint64_t *out) {
    uint64_t value = 0;
    size_t i;

    if (length == 0 || length > 20) {
        return false;
    }
    for (i = 0; i < length; i += 1) {
        unsigned digit = (unsigned)((unsigned char)field[i] - '0');

        if (digit > 9 || value > (most - digit) / 10u) {
            return false;
        }
        value = value * 10u + digit;
    }
    *out = value;
    return true;
}

/* `bytes` as a list, its entries in a block the caller frees; false with
 * `HERO_ACL_INVALID` set where they are not the header's form. */
static bool hero_acl_parse(const void *list, uint64_t size, hero_acl_list *out) {
    const unsigned char *bytes = (const unsigned char *)list;
    uint64_t at = 0;
    uint64_t zeros = 0;
    uint64_t i;
    const char *field;
    size_t length;
    uint64_t number;

    out->entries = NULL;
    out->count = 0;
    for (i = 0; i < size; i += 1) {
        zeros += bytes[i] == 0 ? 1u : 0u;
    }
    if (zeros == 0 || (zeros - 1) % HERO_ACL_FIELDS != 0 || bytes[size - 1] != 0) {
        hero_acl_fail(HERO_ACL_INVALID, 0);
        return false;
    }
    if (!hero_acl_next(bytes, size, &at, &field, &length) || !hero_acl_number(field, length, 1, &number)) {
        hero_acl_fail(HERO_ACL_INVALID, 0);
        return false;
    }
    out->protected_ = number == 1;
    out->count = (size_t)((zeros - 1) / HERO_ACL_FIELDS);
    if (out->count == 0) {
        return true;
    }
    out->entries = (hero_acl_entry *)hero_layer_alloc_zero(out->count, sizeof(hero_acl_entry));
    if (out->entries == NULL) {
        hero_acl_fail(HERO_ACL_LIMIT, 0);
        return false;
    }
    for (i = 0; i < out->count; i += 1) {
        hero_acl_entry *e = &out->entries[i];
        bool good = hero_acl_next(bytes, size, &at, &field, &length) &&
                    hero_acl_number(field, length, HERO_ACL_MASK, &number) && number >= HERO_ACL_USER;

        e->who = (int64_t)number;
        good = good && hero_acl_next(bytes, size, &at, &field, &length) && hero_acl_number(field, length, 1, &number);
        e->deny = number == 1;
        good = good && hero_acl_next(bytes, size, &at, &field, &length) &&
               hero_acl_number(field, length, UINT64_MAX, &e->rights);
        good = good && hero_acl_next(bytes, size, &at, &field, &length) &&
               hero_acl_number(field, length, UINT64_MAX, &e->flags);
        good = good && hero_acl_next(bytes, size, &at, &e->id, &e->id_length);
        /* The name, which a write never reads. */
        good = good && hero_acl_next(bytes, size, &at, &field, &length);
        if (!good) {
            hero_layer_free(out->entries);
            out->entries = NULL;
            hero_acl_fail(HERO_ACL_INVALID, 0);
            return false;
        }
    }
    return true;
}

#endif

#if defined(__APPLE__) || defined(__linux__)

static int64_t hero_acl_kind_of(int code) {
    if (code == ENOTSUP || code == EOPNOTSUPP) {
        return HERO_ACL_UNSUPPORTED;
    }
    switch (code) {
    case ENOENT:
        return HERO_ACL_NOT_FOUND;
    case ENOTDIR:
        return HERO_ACL_NOT_A_DIRECTORY;
    case EACCES:
    case EPERM:
    case EROFS:
        return HERO_ACL_DENIED;
    case ENOMEM:
    case ENOSPC:
    case EDQUOT:
    case E2BIG:
        return HERO_ACL_LIMIT;
    case EINVAL:
    case ENAMETOOLONG:
        return HERO_ACL_INVALID;
    default:
        return HERO_ACL_OTHER;
    }
}

static int64_t hero_acl_failed(void) {
    int code = errno;

    return hero_acl_fail(hero_acl_kind_of(code), (int64_t)code);
}

static bool hero_acl_args(const char *path, const void *bytes, uint64_t count) {
    if (path == NULL || path[0] == 0 || (bytes == NULL && count > 0)) {
        hero_acl_fail(HERO_ACL_INVALID, 0);
        return false;
    }
    return true;
}

/* The name the system gives user or group `id`, written into `out`, empty
 * where it knows none: a list may name an account since removed. */
static void hero_acl_name_of(bool group, uint64_t id, char *out, size_t room) {
    char scratch[16384];
    int done;

    out[0] = 0;
    if (group) {
        struct group g;
        struct group *found = NULL;

        done = getgrgid_r((gid_t)id, &g, scratch, sizeof scratch, &found);
        if (done == 0 && found != NULL && strlen(found->gr_name) < room) {
            strcpy(out, found->gr_name);
        }
    } else {
        struct passwd p;
        struct passwd *found = NULL;

        done = getpwuid_r((uid_t)id, &p, scratch, sizeof scratch, &found);
        if (done == 0 && found != NULL && strlen(found->pw_name) < room) {
            strcpy(out, found->pw_name);
        }
    }
}

#if defined(__linux__)
/* The number of the user or group the system knows by `name`, or false. */
static bool hero_acl_number_of(const char *name, bool group, uint64_t *out) {
    char scratch[16384];

    if (group) {
        struct group g;
        struct group *found = NULL;

        if (getgrnam_r(name, &g, scratch, sizeof scratch, &found) == 0 && found != NULL) {
            *out = (uint64_t)found->gr_gid;
            return true;
        }
    } else {
        struct passwd p;
        struct passwd *found = NULL;

        if (getpwnam_r(name, &p, scratch, sizeof scratch, &found) == 0 && found != NULL) {
            *out = (uint64_t)found->pw_uid;
            return true;
        }
    }
    return false;
}
#endif

#endif

#if defined(__APPLE__)

/* Darwin's rights and flags, each beside the number Windows and the header
 * give it. Each function that reads one holds its own copy, on its stack:
 * the member keeps nothing between calls but the last error. */
typedef struct {
    acl_perm_t perm;
    uint64_t right;
} hero_acl_right;

typedef struct {
    acl_flag_t flag;
    uint64_t bit;
} hero_acl_flag;

#define HERO_ACL_RIGHTS                                                                                                \
    {                                                                                                                  \
        {ACL_READ_DATA, HERO_ACL_READ_DATA}, {ACL_WRITE_DATA, HERO_ACL_WRITE_DATA},                                    \
            {ACL_APPEND_DATA, HERO_ACL_APPEND_DATA}, {ACL_READ_EXTATTRIBUTES, HERO_ACL_READ_EXTENDED},                 \
            {ACL_WRITE_EXTATTRIBUTES, HERO_ACL_WRITE_EXTENDED}, {ACL_EXECUTE, HERO_ACL_EXECUTE},                       \
            {ACL_DELETE_CHILD, HERO_ACL_DELETE_CHILD}, {ACL_READ_ATTRIBUTES, HERO_ACL_READ_ATTRIBUTES},                \
            {ACL_WRITE_ATTRIBUTES, HERO_ACL_WRITE_ATTRIBUTES}, {ACL_DELETE, HERO_ACL_DELETE},                          \
            {ACL_READ_SECURITY, HERO_ACL_READ_SECURITY}, {ACL_WRITE_SECURITY, HERO_ACL_WRITE_SECURITY},                \
            {ACL_CHANGE_OWNER, HERO_ACL_CHANGE_OWNER}, {ACL_SYNCHRONIZE, HERO_ACL_SYNCHRONIZE},                        \
    }

#define HERO_ACL_FLAGS                                                                                                 \
    {                                                                                                                  \
        {ACL_ENTRY_FILE_INHERIT, HERO_ACL_FILE_INHERIT}, {ACL_ENTRY_DIRECTORY_INHERIT, HERO_ACL_FOLDER_INHERIT},       \
            {ACL_ENTRY_LIMIT_INHERIT, HERO_ACL_LIMIT_INHERIT}, {ACL_ENTRY_ONLY_INHERIT, HERO_ACL_ONLY_INHERIT},        \
            {ACL_ENTRY_INHERITED, HERO_ACL_INHERITED},                                                                 \
    }

#define HERO_ACL_COUNT(table) (sizeof(table) / sizeof((table)[0]))

/* Every right Darwin has a place for, and every flag. */
static uint64_t hero_acl_all(bool flags) {
    const hero_acl_right hero_acl_rights[] = HERO_ACL_RIGHTS;
    const hero_acl_flag hero_acl_flags[] = HERO_ACL_FLAGS;
    uint64_t all = 0;
    size_t i;

    if (flags) {
        for (i = 0; i < HERO_ACL_COUNT(hero_acl_flags); i += 1) {
            all |= hero_acl_flags[i].bit;
        }
    } else {
        for (i = 0; i < HERO_ACL_COUNT(hero_acl_rights); i += 1) {
            all |= hero_acl_rights[i].right;
        }
    }
    return all;
}

/* One entry of Darwin's list, as the header's form writes it. */
static bool hero_acl_entry_read(hero_acl_answer *a, acl_entry_t entry) {
    const hero_acl_right hero_acl_rights[] = HERO_ACL_RIGHTS;
    const hero_acl_flag hero_acl_flags[] = HERO_ACL_FLAGS;
    acl_tag_t tag;
    acl_permset_t perms;
    acl_flagset_t flags;
    void *qualifier;
    uint64_t rights = 0;
    uint64_t bits = 0;
    char id[37];
    char name[256];
    id_t number;
    int type = -1;
    size_t i;

    if (acl_get_tag_type(entry, &tag) != 0 || acl_get_permset(entry, &perms) != 0 ||
        acl_get_flagset_np(entry, &flags) != 0) {
        return false;
    }
    qualifier = acl_get_qualifier(entry);
    if (qualifier == NULL) {
        return false;
    }
    uuid_unparse_upper(*(const uuid_t *)qualifier, id);
    name[0] = 0;
    if (mbr_uuid_to_id(*(const uuid_t *)qualifier, &number, &type) == 0) {
        hero_acl_name_of(type == ID_TYPE_GID, (uint64_t)number, name, sizeof name);
    }
    (void)acl_free(qualifier);
    for (i = 0; i < HERO_ACL_COUNT(hero_acl_rights); i += 1) {
        if (acl_get_perm_np(perms, hero_acl_rights[i].perm) == 1) {
            rights |= hero_acl_rights[i].right;
        }
    }
    for (i = 0; i < HERO_ACL_COUNT(hero_acl_flags); i += 1) {
        if (acl_get_flag_np(flags, hero_acl_flags[i].flag) == 1) {
            bits |= hero_acl_flags[i].bit;
        }
    }
    hero_acl_entry_out(a, type == ID_TYPE_GID ? HERO_ACL_GROUP : HERO_ACL_USER, tag == ACL_EXTENDED_DENY, rights,
                       bits, id, name);
    return true;
}

int64_t hero_acl_read(const char *path, int64_t follow, void *buffer, uint64_t count) {
    hero_acl_answer a = {(unsigned char *)buffer, count, 0};
    acl_t acl;
    acl_flagset_t flags;
    acl_entry_t entry;
    int which = ACL_FIRST_ENTRY;

    if (!hero_acl_args(path, buffer, count)) {
        return -1;
    }
    acl = follow != 0 ? acl_get_file(path, ACL_TYPE_EXTENDED) : acl_get_link_np(path, ACL_TYPE_EXTENDED);
    if (acl == NULL) {
        struct stat facts;

        /* A file with no list is `ENOENT`, as a path that names nothing
         * is: the path is asked which it was. */
        if (errno == ENOENT && (follow != 0 ? stat(path, &facts) : lstat(path, &facts)) == 0) {
            hero_acl_text(&a, "0");
            hero_acl_fine();
            return (int64_t)a.used;
        }
        return hero_acl_failed();
    }
    if (acl_get_flagset_np(acl, &flags) != 0) {
        int code = errno;

        (void)acl_free(acl);
        return hero_acl_fail(HERO_ACL_OTHER, (int64_t)code);
    }
    hero_acl_text(&a, acl_get_flag_np(flags, ACL_FLAG_NO_INHERIT) == 1 ? "1" : "0");
    while (acl_get_entry(acl, which, &entry) == 0) {
        which = ACL_NEXT_ENTRY;
        if (!hero_acl_entry_read(&a, entry)) {
            int code = errno;

            (void)acl_free(acl);
            return hero_acl_fail(HERO_ACL_OTHER, (int64_t)code);
        }
    }
    (void)acl_free(acl);
    hero_acl_fine();
    return (int64_t)a.used;
}

/* Entry `e` of a list handed in, added to Darwin's `acl`; false with the
 * failure set. */
static bool hero_acl_entry_write(acl_t *acl, const hero_acl_entry *e) {
    const hero_acl_right hero_acl_rights[] = HERO_ACL_RIGHTS;
    const hero_acl_flag hero_acl_flags[] = HERO_ACL_FLAGS;
    acl_entry_t entry;
    acl_permset_t perms;
    acl_flagset_t flags;
    char text[37];
    uuid_t id;
    size_t i;

    if (e->id_length != 36) {
        hero_acl_fail(HERO_ACL_INVALID, 0);
        return false;
    }
    memcpy(text, e->id, 36);
    text[36] = 0;
    if (uuid_parse(text, id) != 0) {
        hero_acl_fail(HERO_ACL_INVALID, 0);
        return false;
    }
    if (acl_create_entry(acl, &entry) != 0 ||
        acl_set_tag_type(entry, e->deny ? ACL_EXTENDED_DENY : ACL_EXTENDED_ALLOW) != 0 ||
        acl_set_qualifier(entry, id) != 0 || acl_get_permset(entry, &perms) != 0 || acl_clear_perms(perms) != 0 ||
        acl_get_flagset_np(entry, &flags) != 0 || acl_clear_flags_np(flags) != 0) {
        hero_acl_failed();
        return false;
    }
    for (i = 0; i < HERO_ACL_COUNT(hero_acl_rights); i += 1) {
        if ((e->rights & hero_acl_rights[i].right) != 0 && acl_add_perm(perms, hero_acl_rights[i].perm) != 0) {
            hero_acl_failed();
            return false;
        }
    }
    for (i = 0; i < HERO_ACL_COUNT(hero_acl_flags); i += 1) {
        if ((e->flags & hero_acl_flags[i].bit) != 0 && acl_add_flag_np(flags, hero_acl_flags[i].flag) != 0) {
            hero_acl_failed();
            return false;
        }
    }
    if (acl_set_permset(entry, perms) != 0 || acl_set_flagset_np(entry, flags) != 0) {
        hero_acl_failed();
        return false;
    }
    return true;
}

int64_t hero_acl_write(const char *path, int64_t follow, const void *list, uint64_t size) {
    hero_acl_list parsed;
    acl_t acl;
    acl_flagset_t flags;
    uint64_t rights = hero_acl_all(false);
    uint64_t bits = hero_acl_all(true);
    size_t i;
    int done;

    if (!hero_acl_args(path, list, size) || !hero_acl_parse(list, size, &parsed)) {
        return -1;
    }
    for (i = 0; i < parsed.count; i += 1) {
        const hero_acl_entry *e = &parsed.entries[i];
        int64_t reason = e->who != HERO_ACL_USER && e->who != HERO_ACL_GROUP ? HERO_ACL_CANNOT_WHO
                         : (e->rights & ~rights) != 0                         ? HERO_ACL_CANNOT_RIGHT
                         : (e->flags & ~bits) != 0                            ? HERO_ACL_CANNOT_FLAG
                                                                              : 0;

        if (reason != 0) {
            hero_layer_free(parsed.entries);
            return hero_acl_cannot(reason, (int64_t)i + 1);
        }
    }
    acl = acl_init((int)(parsed.count < 128 ? parsed.count : 128));
    if (acl == NULL) {
        hero_layer_free(parsed.entries);
        return hero_acl_failed();
    }
    if (acl_get_flagset_np(acl, &flags) != 0 || acl_clear_flags_np(flags) != 0 ||
        (parsed.protected_ && acl_add_flag_np(flags, ACL_FLAG_NO_INHERIT) != 0) ||
        acl_set_flagset_np(acl, flags) != 0) {
        hero_layer_free(parsed.entries);
        (void)acl_free(acl);
        return hero_acl_failed();
    }
    for (i = 0; i < parsed.count; i += 1) {
        if (!hero_acl_entry_write(&acl, &parsed.entries[i])) {
            hero_layer_free(parsed.entries);
            (void)acl_free(acl);
            return -1;
        }
    }
    hero_layer_free(parsed.entries);
    done = follow != 0 ? acl_set_file(path, ACL_TYPE_EXTENDED, acl) : acl_set_link_np(path, ACL_TYPE_EXTENDED, acl);
    if (done != 0) {
        int64_t failed = hero_acl_failed();

        (void)acl_free(acl);
        return failed;
    }
    (void)acl_free(acl);
    hero_acl_fine();
    return 0;
}

int64_t hero_acl_id_of(const char *name, int64_t group, void *buffer, uint64_t count) {
    hero_acl_answer a = {(unsigned char *)buffer, count, 0};
    uuid_t id;
    char text[37];
    id_t number;
    int type = -1;
    int done;

    if (name == NULL || name[0] == 0 || (buffer == NULL && count > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    done = mbr_identifier_to_uuid(group != 0 ? ID_TYPE_GROUPNAME : ID_TYPE_USERNAME, name, strlen(name), id);
    if (done != 0) {
        return hero_acl_fail(HERO_ACL_UNKNOWN, (int64_t)done);
    }
    /* The id is the kind asked: a user's name asked as a group's is none. */
    done = mbr_uuid_to_id(id, &number, &type);
    if (done != 0 || type != (group != 0 ? ID_TYPE_GID : ID_TYPE_UID)) {
        return hero_acl_fail(HERO_ACL_UNKNOWN, (int64_t)done);
    }
    uuid_unparse_upper(id, text);
    hero_acl_put(&a, text, 36);
    hero_acl_fine();
    return (int64_t)a.used;
}

#elif defined(__linux__)

/* The kernel's form of a list (`linux/posix_acl_xattr.h`,
 * `linux/posix_acl.h`): a little-endian version, 2, then eight bytes an
 * entry, its tag and its rights in two bytes each and its id in four. */
#define HERO_ACL_VERSION 2u
#define HERO_ACL_TAG_USER_OBJ 0x01u
#define HERO_ACL_TAG_USER 0x02u
#define HERO_ACL_TAG_GROUP_OBJ 0x04u
#define HERO_ACL_TAG_GROUP 0x08u
#define HERO_ACL_TAG_MASK 0x10u
#define HERO_ACL_TAG_OTHER 0x20u
#define HERO_ACL_UNDEFINED_ID 0xFFFFFFFFu
#define HERO_ACL_ACCESS "system.posix_acl_access"
#define HERO_ACL_DEFAULT "system.posix_acl_default"
#define HERO_ACL_POSIX_RIGHTS (HERO_ACL_READ_DATA | HERO_ACL_WRITE_DATA | HERO_ACL_EXECUTE)

/* The kernel's tag for whom the header names, 0 for no such. */
static unsigned hero_acl_tag_of(int64_t who) {
    switch (who) {
    case HERO_ACL_USER:
        return HERO_ACL_TAG_USER;
    case HERO_ACL_GROUP:
        return HERO_ACL_TAG_GROUP;
    case HERO_ACL_OWNER:
        return HERO_ACL_TAG_USER_OBJ;
    case HERO_ACL_OWNING_GROUP:
        return HERO_ACL_TAG_GROUP_OBJ;
    case HERO_ACL_OTHERS:
        return HERO_ACL_TAG_OTHER;
    case HERO_ACL_MASK:
        return HERO_ACL_TAG_MASK;
    default:
        return 0;
    }
}

/* Whom the header names for the kernel's tag, 0 for a tag it has no name
 * for. */
static int64_t hero_acl_who_of(unsigned tag) {
    int64_t who;

    for (who = HERO_ACL_USER; who <= HERO_ACL_MASK; who += 1) {
        if (hero_acl_tag_of(who) == tag) {
            return who;
        }
    }
    return 0;
}

static uint64_t hero_acl_rights_of(unsigned perm) {
    return ((perm & 4u) != 0 ? HERO_ACL_READ_DATA : 0) | ((perm & 2u) != 0 ? HERO_ACL_WRITE_DATA : 0) |
           ((perm & 1u) != 0 ? HERO_ACL_EXECUTE : 0);
}

static unsigned hero_acl_perm_of(uint64_t rights) {
    return ((rights & HERO_ACL_READ_DATA) != 0 ? 4u : 0u) | ((rights & HERO_ACL_WRITE_DATA) != 0 ? 2u : 0u) |
           ((rights & HERO_ACL_EXECUTE) != 0 ? 1u : 0u);
}

static unsigned hero_acl_le(const unsigned char *at, int bytes) {
    unsigned value = 0;
    int i;

    for (i = bytes - 1; i >= 0; i -= 1) {
        value = (value << 8) | at[i];
    }
    return value;
}

/* The kernel's list `name` of `path`, in a block the caller frees: 1 where
 * it has one, 0 where it has none or the volume keeps none, -1 with the
 * failure set. */
static int hero_acl_get(const char *path, const char *name, bool follow, unsigned char **out, size_t *size) {
    int tries;

    *out = NULL;
    *size = 0;
    for (tries = 0; tries < 8; tries += 1) {
        ssize_t need = follow ? getxattr(path, name, NULL, 0) : lgetxattr(path, name, NULL, 0);
        ssize_t got;

        if (need < 0) {
            if (errno == ENODATA || errno == ENOTSUP || errno == EOPNOTSUPP) {
                return 0;
            }
            hero_acl_failed();
            return -1;
        }
        *out = (unsigned char *)hero_layer_alloc(need > 0 ? (size_t)need : 1);
        if (*out == NULL) {
            hero_acl_fail(HERO_ACL_LIMIT, 0);
            return -1;
        }
        got = follow ? getxattr(path, name, *out, (size_t)need) : lgetxattr(path, name, *out, (size_t)need);
        if (got >= 0) {
            *size = (size_t)got;
            return 1;
        }
        hero_layer_free(*out);
        *out = NULL;
        if (errno != ERANGE) {
            hero_acl_failed();
            return -1;
        }
    }
    hero_acl_fail(HERO_ACL_OTHER, (int64_t)ERANGE);
    return -1;
}

/* The kernel's list in `bytes` as entries of the header's form, each with
 * `flags`; false where the bytes are not the kernel's form. */
static bool hero_acl_entries_out(hero_acl_answer *a, const unsigned char *bytes, size_t size, uint64_t flags) {
    size_t at;

    if (size < 4 || (size - 4) % 8 != 0 || hero_acl_le(bytes, 4) != HERO_ACL_VERSION) {
        return false;
    }
    for (at = 4; at < size; at += 8) {
        int64_t who = hero_acl_who_of(hero_acl_le(bytes + at, 2));
        uint64_t rights = hero_acl_rights_of(hero_acl_le(bytes + at + 2, 2));
        unsigned id = hero_acl_le(bytes + at + 4, 4);
        char number[21] = "";
        char name[256] = "";

        if (who == 0) {
            return false;
        }
        if (who == HERO_ACL_USER || who == HERO_ACL_GROUP) {
            hero_acl_answer digits = {(unsigned char *)number, sizeof number - 1, 0};

            hero_acl_decimal(&digits, id);
            number[digits.used - 1] = 0;
            hero_acl_name_of(who == HERO_ACL_GROUP, id, name, sizeof name);
        }
        hero_acl_entry_out(a, who, false, rights, flags, number, name);
    }
    return true;
}

int64_t hero_acl_read(const char *path, int64_t follow, void *buffer, uint64_t count) {
    hero_acl_answer a = {(unsigned char *)buffer, count, 0};
    struct stat facts;
    unsigned char *bytes;
    size_t size;
    int has;

    if (!hero_acl_args(path, buffer, count)) {
        return -1;
    }
    if ((follow != 0 ? stat(path, &facts) : lstat(path, &facts)) != 0) {
        return hero_acl_failed();
    }
    hero_acl_text(&a, "0");
    has = hero_acl_get(path, HERO_ACL_ACCESS, follow != 0, &bytes, &size);
    if (has < 0) {
        return -1;
    }
    if (has == 0) {
        /* No list: the three entries the permission bits are. */
        hero_acl_entry_out(&a, HERO_ACL_OWNER, false, hero_acl_rights_of((facts.st_mode >> 6) & 7u), 0, "", "");
        hero_acl_entry_out(&a, HERO_ACL_OWNING_GROUP, false, hero_acl_rights_of((facts.st_mode >> 3) & 7u), 0, "",
                           "");
        hero_acl_entry_out(&a, HERO_ACL_OTHERS, false, hero_acl_rights_of(facts.st_mode & 7u), 0, "", "");
    } else {
        bool good = hero_acl_entries_out(&a, bytes, size, 0);

        hero_layer_free(bytes);
        if (!good) {
            return hero_acl_fail(HERO_ACL_OTHER, 0);
        }
    }
    if (S_ISDIR(facts.st_mode)) {
        has = hero_acl_get(path, HERO_ACL_DEFAULT, follow != 0, &bytes, &size);
        if (has < 0) {
            return -1;
        }
        if (has > 0) {
            bool good = hero_acl_entries_out(&a, bytes, size, HERO_ACL_LINUX_DEFAULT);

            hero_layer_free(bytes);
            if (!good) {
                return hero_acl_fail(HERO_ACL_OTHER, 0);
            }
        }
    }
    hero_acl_fine();
    return (int64_t)a.used;
}

/* One entry on its way into the kernel's form. */
typedef struct {
    unsigned tag;
    unsigned perm;
    unsigned id;
} hero_acl_kernel;

/* The kernel's form of the entries of `parsed` whose flags are `flags`, put
 * in the kernel's order, a mask made where one is needed and none given;
 * `*bytes` NULL where there are none. False with the failure set. */
static bool hero_acl_encode(const hero_acl_list *parsed, uint64_t flags, unsigned char **bytes, size_t *size) {
    hero_acl_kernel *kept;
    size_t n = 0;
    size_t i;
    bool named = false;
    bool masked = false;
    unsigned mask = 0;

    *bytes = NULL;
    *size = 0;
    kept = (hero_acl_kernel *)hero_layer_alloc_zero(parsed->count + 1, sizeof(hero_acl_kernel));
    if (kept == NULL) {
        hero_acl_fail(HERO_ACL_LIMIT, 0);
        return false;
    }
    for (i = 0; i < parsed->count; i += 1) {
        const hero_acl_entry *e = &parsed->entries[i];
        hero_acl_kernel k;
        uint64_t id = HERO_ACL_UNDEFINED_ID;
        size_t j;

        if (e->flags != flags) {
            continue;
        }
        if (e->who == HERO_ACL_USER || e->who == HERO_ACL_GROUP) {
            if (!hero_acl_number(e->id, e->id_length, HERO_ACL_UNDEFINED_ID - 1u, &id)) {
                hero_layer_free(kept);
                hero_acl_fail(HERO_ACL_INVALID, 0);
                return false;
            }
            named = true;
        }
        k.tag = hero_acl_tag_of(e->who);
        k.perm = hero_acl_perm_of(e->rights);
        k.id = (unsigned)id;
        masked = masked || e->who == HERO_ACL_MASK;
        if (e->who == HERO_ACL_USER || e->who == HERO_ACL_GROUP || e->who == HERO_ACL_OWNING_GROUP) {
            mask |= k.perm;
        }
        /* Into its place: by tag, then by id. */
        j = n;
        while (j > 0 && (kept[j - 1].tag > k.tag || (kept[j - 1].tag == k.tag && kept[j - 1].id > k.id))) {
            kept[j] = kept[j - 1];
            j -= 1;
        }
        kept[j] = k;
        n += 1;
    }
    if (n == 0) {
        hero_layer_free(kept);
        return true;
    }
    if (named && !masked) {
        hero_acl_kernel k = {HERO_ACL_TAG_MASK, mask, HERO_ACL_UNDEFINED_ID};
        size_t j = n;

        while (j > 0 && kept[j - 1].tag > k.tag) {
            kept[j] = kept[j - 1];
            j -= 1;
        }
        kept[j] = k;
        n += 1;
    }
    *size = 4 + 8 * n;
    *bytes = (unsigned char *)hero_layer_alloc(*size);
    if (*bytes == NULL) {
        hero_layer_free(kept);
        hero_acl_fail(HERO_ACL_LIMIT, 0);
        return false;
    }
    (*bytes)[0] = (unsigned char)HERO_ACL_VERSION;
    (*bytes)[1] = (*bytes)[2] = (*bytes)[3] = 0;
    for (i = 0; i < n; i += 1) {
        unsigned char *at = *bytes + 4 + 8 * i;

        at[0] = (unsigned char)(kept[i].tag & 0xFFu);
        at[1] = (unsigned char)(kept[i].tag >> 8);
        at[2] = (unsigned char)(kept[i].perm & 0xFFu);
        at[3] = (unsigned char)(kept[i].perm >> 8);
        at[4] = (unsigned char)(kept[i].id & 0xFFu);
        at[5] = (unsigned char)((kept[i].id >> 8) & 0xFFu);
        at[6] = (unsigned char)((kept[i].id >> 16) & 0xFFu);
        at[7] = (unsigned char)(kept[i].id >> 24);
    }
    hero_layer_free(kept);
    return true;
}

/* Whether the kernel's list in `bytes` is the three entries the permission
 * bits are, and the bits it gives where it is. */
static bool hero_acl_bits_only(const unsigned char *bytes, size_t size, unsigned *bits) {
    size_t at;

    *bits = 0;
    if (size != 4 + 3 * 8) {
        return false;
    }
    for (at = 4; at < size; at += 8) {
        unsigned tag = hero_acl_le(bytes + at, 2);
        unsigned perm = hero_acl_le(bytes + at + 2, 2);

        *bits |= tag == HERO_ACL_TAG_USER_OBJ ? perm << 6 : tag == HERO_ACL_TAG_GROUP_OBJ ? perm << 3 : perm;
    }
    return true;
}

int64_t hero_acl_write(const char *path, int64_t follow, const void *list, uint64_t size) {
    hero_acl_list parsed;
    struct stat facts;
    unsigned char *access = NULL;
    unsigned char *defaults = NULL;
    size_t access_size = 0;
    size_t defaults_size = 0;
    unsigned bits;
    size_t i;
    int done;

    if (!hero_acl_args(path, list, size) || !hero_acl_parse(list, size, &parsed)) {
        return -1;
    }
    if ((follow != 0 ? stat(path, &facts) : lstat(path, &facts)) != 0) {
        hero_layer_free(parsed.entries);
        return hero_acl_failed();
    }
    if (parsed.protected_) {
        hero_layer_free(parsed.entries);
        return hero_acl_cannot(HERO_ACL_CANNOT_PROTECT, 0);
    }
    for (i = 0; i < parsed.count; i += 1) {
        const hero_acl_entry *e = &parsed.entries[i];
        int64_t reason = e->deny                                                         ? HERO_ACL_CANNOT_DENY
                         : (e->rights & ~(uint64_t)HERO_ACL_POSIX_RIGHTS) != 0           ? HERO_ACL_CANNOT_RIGHT
                         : e->flags != 0 && e->flags != (uint64_t)HERO_ACL_LINUX_DEFAULT ? HERO_ACL_CANNOT_FLAG
                         : e->flags != 0 && !S_ISDIR(facts.st_mode)                      ? HERO_ACL_CANNOT_DEFAULT
                                                                                          : 0;

        if (reason != 0) {
            hero_layer_free(parsed.entries);
            return hero_acl_cannot(reason, (int64_t)i + 1);
        }
    }
    if (!hero_acl_encode(&parsed, 0, &access, &access_size) ||
        !hero_acl_encode(&parsed, HERO_ACL_LINUX_DEFAULT, &defaults, &defaults_size)) {
        hero_layer_free(parsed.entries);
        hero_layer_free(access);
        return -1;
    }
    hero_layer_free(parsed.entries);
    if (access == NULL) {
        /* No access entry: the kernel's own refusal of a list missing its
         * owner, owning group and everyone else. */
        hero_layer_free(defaults);
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    done = follow != 0 ? setxattr(path, HERO_ACL_ACCESS, access, access_size, 0)
                       : lsetxattr(path, HERO_ACL_ACCESS, access, access_size, 0);
    if (done != 0 && (errno == ENOTSUP || errno == EOPNOTSUPP) && defaults == NULL && follow != 0 &&
        hero_acl_bits_only(access, access_size, &bits)) {
        /* A volume without access lists takes a list that is its
         * permission bits as those bits, its set-id and sticky bits kept. */
        done = chmod(path, (mode_t)((facts.st_mode & 07000u) | bits));
    }
    hero_layer_free(access);
    if (done != 0) {
        hero_layer_free(defaults);
        return hero_acl_failed();
    }
    if (S_ISDIR(facts.st_mode)) {
        if (defaults != NULL) {
            done = follow != 0 ? setxattr(path, HERO_ACL_DEFAULT, defaults, defaults_size, 0)
                               : lsetxattr(path, HERO_ACL_DEFAULT, defaults, defaults_size, 0);
        } else {
            done = follow != 0 ? removexattr(path, HERO_ACL_DEFAULT) : lremovexattr(path, HERO_ACL_DEFAULT);
            if (done != 0 && (errno == ENODATA || errno == ENOTSUP || errno == EOPNOTSUPP)) {
                done = 0;
            }
        }
        if (done != 0) {
            hero_layer_free(defaults);
            return hero_acl_failed();
        }
    }
    hero_layer_free(defaults);
    hero_acl_fine();
    return 0;
}

int64_t hero_acl_id_of(const char *name, int64_t group, void *buffer, uint64_t count) {
    hero_acl_answer a = {(unsigned char *)buffer, count, 0};
    char digits[21];
    hero_acl_answer d = {(unsigned char *)digits, sizeof digits, 0};
    uint64_t id;

    if (name == NULL || name[0] == 0 || (buffer == NULL && count > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    if (!hero_acl_number_of(name, group != 0, &id)) {
        return hero_acl_fail(HERO_ACL_UNKNOWN, 0);
    }
    hero_acl_decimal(&d, id);
    hero_acl_put(&a, digits, (size_t)d.used - 1);
    hero_acl_fine();
    return (int64_t)a.used;
}

#elif defined(_WIN32)

static int64_t hero_acl_kind_of(DWORD code) {
    switch (code) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return HERO_ACL_NOT_FOUND;
    case ERROR_DIRECTORY:
        return HERO_ACL_NOT_A_DIRECTORY;
    case ERROR_ACCESS_DENIED:
    case ERROR_PRIVILEGE_NOT_HELD:
    case ERROR_WRITE_PROTECT:
        return HERO_ACL_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
        return HERO_ACL_LIMIT;
    case ERROR_NOT_SUPPORTED:
    case ERROR_INVALID_FUNCTION:
        return HERO_ACL_UNSUPPORTED;
    case ERROR_INVALID_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_INVALID_SID:
    case ERROR_INVALID_ACL:
    case ERROR_INVALID_PARAMETER:
        return HERO_ACL_INVALID;
    case ERROR_NONE_MAPPED:
        return HERO_ACL_UNKNOWN;
    default:
        return HERO_ACL_OTHER;
    }
}

static int64_t hero_acl_failed_with(DWORD code) {
    return hero_acl_fail(hero_acl_kind_of(code), (int64_t)code);
}

/* `text`, UTF-8, as UTF-16 in a block the caller frees; NULL with the
 * failure set where it is not text. A surrogate standing alone may be
 * spelled as the three bytes its value would take, as a name the system
 * lists comes out of the layer; a pair spelled so is refused. */
static wchar_t *hero_acl_wide(const char *text) {
    const unsigned char *s = (const unsigned char *)text;
    wchar_t *out = (wchar_t *)hero_layer_alloc_zero(strlen(text) + 1, sizeof(wchar_t));
    size_t at = 0;
    size_t put = 0;
    bool after_high = false;

    if (out == NULL) {
        hero_acl_fail(HERO_ACL_LIMIT, 0);
        return NULL;
    }
    while (s[at] != 0) {
        uint32_t c = s[at];
        uint32_t least = 0;
        int length = 0;
        int k;
        bool good = true;

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
                good = false;
            }
        }
        for (k = 1; good && k <= length; k += 1) {
            unsigned char next = s[at + (size_t)k];

            good = (next & 0xC0u) == 0x80u;
            c = (c << 6) | (next & 0x3Fu);
        }
        if (!good || c < least || c > 0x10FFFFu || (after_high && c >= 0xDC00u && c <= 0xDFFFu)) {
            hero_layer_free(out);
            hero_acl_fail(HERO_ACL_NOT_TEXT, 0);
            return NULL;
        }
        at += 1 + (size_t)length;
        after_high = c >= 0xD800u && c <= 0xDBFFu;
        if (c >= 0x10000u) {
            out[put++] = (wchar_t)(0xD800u + ((c - 0x10000u) >> 10));
            out[put++] = (wchar_t)(0xDC00u + ((c - 0x10000u) & 0x3FFu));
        } else {
            out[put++] = (wchar_t)c;
        }
    }
    out[put] = 0;
    return out;
}

/* A wide text as bytes into `out`, UTF-8 and a lone surrogate as the three
 * bytes its value would take, zero-terminated; false where `room` is short. */
static bool hero_acl_narrow(const wchar_t *wide, char *out, size_t room) {
    size_t at = 0;
    size_t i;

    for (i = 0; wide[i] != 0; i += 1) {
        uint32_t c = (uint32_t)wide[i];

        if (c >= 0xD800u && c <= 0xDBFFu && (uint32_t)wide[i + 1] >= 0xDC00u && (uint32_t)wide[i + 1] <= 0xDFFFu) {
            c = 0x10000u + ((c - 0xD800u) << 10) + ((uint32_t)wide[i + 1] - 0xDC00u);
            i += 1;
        }
        if (room - at < 5) {
            return false;
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
    out[at] = 0;
    return true;
}

/* The flags Windows' entries carry that the header names; the rest of an
 * entry's flags are an audit's. */
#define HERO_ACL_WINDOWS_FLAGS                                                                                         \
    (HERO_ACL_FILE_INHERIT | HERO_ACL_FOLDER_INHERIT | HERO_ACL_LIMIT_INHERIT | HERO_ACL_ONLY_INHERIT |               \
     HERO_ACL_INHERITED)

/* A SID's text and the name the system gives it, `who` set to a user or a
 * group by what the system says it is: a SID it does not know is a user's,
 * nameless, as an account since removed is. */
static bool hero_acl_sid_out(hero_acl_answer *a, PSID sid, bool deny, uint64_t rights, uint64_t flags) {
    wchar_t *text = NULL;
    wchar_t name[256];
    wchar_t domain[256];
    wchar_t whole[520];
    DWORD name_length = 256;
    DWORD domain_length = 256;
    SID_NAME_USE use = SidTypeUnknown;
    char id[256];
    char named[1040];
    int64_t who = HERO_ACL_USER;

    if (!ConvertSidToStringSidW(sid, &text)) {
        return false;
    }
    if (!hero_acl_narrow(text, id, sizeof id)) {
        (void)LocalFree(text);
        return false;
    }
    (void)LocalFree(text);
    named[0] = 0;
    if (LookupAccountSidW(NULL, sid, name, &name_length, domain, &domain_length, &use)) {
        if (domain[0] != 0) {
            (void)_snwprintf(whole, 519, L"%ls\\%ls", domain, name);
        } else {
            (void)_snwprintf(whole, 519, L"%ls", name);
        }
        whole[519] = 0;
        if (!hero_acl_narrow(whole, named, sizeof named)) {
            named[0] = 0;
        }
        who = use == SidTypeUser || use == SidTypeComputer ? HERO_ACL_USER : HERO_ACL_GROUP;
    }
    hero_acl_entry_out(a, who, deny, rights, flags, id, named);
    return true;
}

/* The DACL of the file `wide` names, and its descriptor, which the caller
 * frees with `LocalFree`: through a handle where the link itself is asked. */
static DWORD hero_acl_dacl(const wchar_t *wide, bool follow, PACL *dacl, PSECURITY_DESCRIPTOR *descriptor) {
    HANDLE h;
    DWORD code;

    if (follow) {
        return GetNamedSecurityInfoW(wide, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, dacl, NULL,
                                     descriptor);
    }
    h = CreateFileW(wide, READ_CONTROL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }
    code = GetSecurityInfo(h, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, dacl, NULL, descriptor);
    (void)CloseHandle(h);
    return code;
}

int64_t hero_acl_read(const char *path, int64_t follow, void *buffer, uint64_t count) {
    hero_acl_answer a = {(unsigned char *)buffer, count, 0};
    wchar_t *wide;
    PACL dacl = NULL;
    PSECURITY_DESCRIPTOR descriptor = NULL;
    SECURITY_DESCRIPTOR_CONTROL control = 0;
    DWORD revision = 0;
    ACL_SIZE_INFORMATION sizes;
    DWORD code;
    DWORD i;

    if (path == NULL || path[0] == 0 || (buffer == NULL && count > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    wide = hero_acl_wide(path);
    if (wide == NULL) {
        return -1;
    }
    code = hero_acl_dacl(wide, follow != 0, &dacl, &descriptor);
    hero_layer_free(wide);
    if (code != ERROR_SUCCESS) {
        return hero_acl_failed_with(code);
    }
    if (dacl == NULL) {
        (void)LocalFree(descriptor);
        return hero_acl_cannot(HERO_ACL_CANNOT_READ, 0);
    }
    if (!GetSecurityDescriptorControl(descriptor, &control, &revision) ||
        !GetAclInformation(dacl, &sizes, sizeof sizes, AclSizeInformation)) {
        code = GetLastError();
        (void)LocalFree(descriptor);
        return hero_acl_failed_with(code);
    }
    hero_acl_text(&a, (control & SE_DACL_PROTECTED) != 0 ? "1" : "0");
    for (i = 0; i < sizes.AceCount; i += 1) {
        ACE_HEADER *header = NULL;
        bool deny;

        if (!GetAce(dacl, i, (LPVOID *)&header)) {
            code = GetLastError();
            (void)LocalFree(descriptor);
            return hero_acl_failed_with(code);
        }
        if (header->AceType != ACCESS_ALLOWED_ACE_TYPE && header->AceType != ACCESS_DENIED_ACE_TYPE) {
            (void)LocalFree(descriptor);
            return hero_acl_cannot(HERO_ACL_CANNOT_READ, (int64_t)i + 1);
        }
        deny = header->AceType == ACCESS_DENIED_ACE_TYPE;
        {
            ACCESS_ALLOWED_ACE *ace = (ACCESS_ALLOWED_ACE *)header;

            if (!hero_acl_sid_out(&a, (PSID)&ace->SidStart, deny, (uint64_t)ace->Mask,
                                  (uint64_t)(header->AceFlags & HERO_ACL_WINDOWS_FLAGS))) {
                code = GetLastError();
                (void)LocalFree(descriptor);
                return hero_acl_failed_with(code);
            }
        }
    }
    (void)LocalFree(descriptor);
    hero_acl_fine();
    return (int64_t)a.used;
}

int64_t hero_acl_write(const char *path, int64_t follow, const void *list, uint64_t size) {
    hero_acl_list parsed;
    PSID *sids;
    wchar_t *wide;
    PACL acl;
    DWORD room = sizeof(ACL);
    DWORD code = ERROR_SUCCESS;
    SECURITY_INFORMATION what;
    size_t i;

    if (path == NULL || path[0] == 0 || (list == NULL && size > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    if (!hero_acl_parse(list, size, &parsed)) {
        return -1;
    }
    for (i = 0; i < parsed.count; i += 1) {
        const hero_acl_entry *e = &parsed.entries[i];
        int64_t reason = e->who != HERO_ACL_USER && e->who != HERO_ACL_GROUP ? HERO_ACL_CANNOT_WHO
                         : e->rights > 0xFFFFFFFFu                            ? HERO_ACL_CANNOT_RIGHT
                         : (e->flags & ~(uint64_t)HERO_ACL_WINDOWS_FLAGS) != 0 ? HERO_ACL_CANNOT_FLAG
                                                                               : 0;

        if (reason != 0) {
            hero_layer_free(parsed.entries);
            return hero_acl_cannot(reason, (int64_t)i + 1);
        }
    }
    sids = (PSID *)hero_layer_alloc_zero(parsed.count + 1, sizeof(PSID));
    if (sids == NULL) {
        hero_layer_free(parsed.entries);
        return hero_acl_fail(HERO_ACL_LIMIT, 0);
    }
    for (i = 0; i < parsed.count && code == ERROR_SUCCESS; i += 1) {
        const hero_acl_entry *e = &parsed.entries[i];
        char text[256];
        wchar_t *id;

        /* An entry the parent hands down is the system's to write again,
         * unless the list is protected and holds it as its own. */
        if (!parsed.protected_ && (e->flags & HERO_ACL_INHERITED) != 0) {
            continue;
        }
        if (e->id_length == 0 || e->id_length >= sizeof text) {
            code = ERROR_INVALID_SID;
            break;
        }
        memcpy(text, e->id, e->id_length);
        text[e->id_length] = 0;
        id = hero_acl_wide(text);
        if (id == NULL) {
            code = ERROR_INVALID_SID;
            break;
        }
        if (!ConvertStringSidToSidW(id, &sids[i])) {
            code = GetLastError();
        } else {
            room += (DWORD)(offsetof(ACCESS_ALLOWED_ACE, SidStart) + GetLengthSid(sids[i]));
        }
        hero_layer_free(id);
    }
    room = (room + 3u) & ~3u;
    /* A list's size is sixteen bits (`ACL.AclSize`): a longer one is past
     * what Windows keeps, not a list of the wrong form. */
    if (code == ERROR_SUCCESS && room > 0xFFFCu) {
        for (i = 0; i < parsed.count; i += 1) {
            if (sids[i] != NULL) {
                (void)LocalFree(sids[i]);
            }
        }
        hero_layer_free(sids);
        hero_layer_free(parsed.entries);
        return hero_acl_fail(HERO_ACL_LIMIT, 0);
    }
    acl = code == ERROR_SUCCESS ? (PACL)hero_layer_alloc_zero(1, room) : NULL;
    if (code == ERROR_SUCCESS && acl == NULL) {
        code = ERROR_NOT_ENOUGH_MEMORY;
    }
    if (code == ERROR_SUCCESS && !InitializeAcl(acl, room, ACL_REVISION)) {
        code = GetLastError();
    }
    for (i = 0; i < parsed.count && code == ERROR_SUCCESS; i += 1) {
        const hero_acl_entry *e = &parsed.entries[i];
        BOOL added;

        if (sids[i] == NULL) {
            continue;
        }
        added = e->deny ? AddAccessDeniedAceEx(acl, ACL_REVISION, (DWORD)e->flags, (DWORD)e->rights, sids[i])
                        : AddAccessAllowedAceEx(acl, ACL_REVISION, (DWORD)e->flags, (DWORD)e->rights, sids[i]);
        if (!added) {
            code = GetLastError();
        }
    }
    for (i = 0; i < parsed.count; i += 1) {
        if (sids[i] != NULL) {
            (void)LocalFree(sids[i]);
        }
    }
    hero_layer_free(sids);
    what = DACL_SECURITY_INFORMATION |
           (parsed.protected_ ? PROTECTED_DACL_SECURITY_INFORMATION : UNPROTECTED_DACL_SECURITY_INFORMATION);
    hero_layer_free(parsed.entries);
    wide = code == ERROR_SUCCESS ? hero_acl_wide(path) : NULL;
    if (code == ERROR_SUCCESS && wide == NULL) {
        hero_layer_free(acl);
        return -1;
    }
    if (code == ERROR_SUCCESS && follow != 0) {
        code = SetNamedSecurityInfoW(wide, SE_FILE_OBJECT, what, NULL, NULL, acl, NULL);
    } else if (code == ERROR_SUCCESS) {
        HANDLE h = CreateFileW(wide, READ_CONTROL | WRITE_DAC, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                               NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, NULL);

        if (h == INVALID_HANDLE_VALUE) {
            code = GetLastError();
        } else {
            code = SetSecurityInfo(h, SE_FILE_OBJECT, what, NULL, NULL, acl, NULL);
            (void)CloseHandle(h);
        }
    }
    hero_layer_free(wide);
    hero_layer_free(acl);
    if (code != ERROR_SUCCESS) {
        return hero_acl_failed_with(code);
    }
    hero_acl_fine();
    return 0;
}

int64_t hero_acl_id_of(const char *name, int64_t group, void *buffer, uint64_t count) {
    hero_acl_answer a = {(unsigned char *)buffer, count, 0};
    wchar_t *wide;
    BYTE sid[SECURITY_MAX_SID_SIZE];
    DWORD sid_length = sizeof sid;
    wchar_t domain[256];
    DWORD domain_length = 256;
    SID_NAME_USE use = SidTypeUnknown;
    wchar_t *text = NULL;
    char id[256];
    bool user;

    if (name == NULL || name[0] == 0 || (buffer == NULL && count > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    wide = hero_acl_wide(name);
    if (wide == NULL) {
        return -1;
    }
    if (!LookupAccountNameW(NULL, wide, (PSID)sid, &sid_length, domain, &domain_length, &use)) {
        DWORD code = GetLastError();

        hero_layer_free(wide);
        return code == ERROR_NONE_MAPPED ? hero_acl_fail(HERO_ACL_UNKNOWN, (int64_t)code)
                                         : hero_acl_failed_with(code);
    }
    hero_layer_free(wide);
    user = use == SidTypeUser || use == SidTypeComputer;
    if (user == (group != 0)) {
        return hero_acl_fail(HERO_ACL_UNKNOWN, 0);
    }
    if (!ConvertSidToStringSidW((PSID)sid, &text)) {
        return hero_acl_failed_with(GetLastError());
    }
    if (!hero_acl_narrow(text, id, sizeof id)) {
        (void)LocalFree(text);
        return hero_acl_fail(HERO_ACL_OTHER, 0);
    }
    (void)LocalFree(text);
    hero_acl_put(&a, id, strlen(id));
    hero_acl_fine();
    return (int64_t)a.used;
}

#else

/* A POSIX system that is neither Darwin nor Linux: its lists, where it has
 * them (the BSDs' NFSv4 and POSIX.1e lists), are not read here. */

int64_t hero_acl_read(const char *path, int64_t follow, void *buffer, uint64_t count) {
    (void)follow;
    if (path == NULL || path[0] == 0 || (buffer == NULL && count > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    return hero_acl_fail(HERO_ACL_UNSUPPORTED, 0);
}

int64_t hero_acl_write(const char *path, int64_t follow, const void *list, uint64_t size) {
    (void)follow;
    if (path == NULL || path[0] == 0 || (list == NULL && size > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    return hero_acl_fail(HERO_ACL_UNSUPPORTED, 0);
}

int64_t hero_acl_id_of(const char *name, int64_t group, void *buffer, uint64_t count) {
    (void)group;
    if (name == NULL || name[0] == 0 || (buffer == NULL && count > 0)) {
        return hero_acl_fail(HERO_ACL_INVALID, 0);
    }
    return hero_acl_fail(HERO_ACL_UNSUPPORTED, 0);
}

#endif
