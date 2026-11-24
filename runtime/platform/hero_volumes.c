/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_volumes.c — the platform layer's list of the volumes
 * mounted on the machine, one member of the archive the compiler puts on
 * every program's link line (M-rebel-library, gap G08). Its header,
 * `hero_volumes.h`, says what the answer holds; this file says how each
 * platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * NOTHING SURVIVES BETWEEN CALLS BUT THE LAST ERROR, which is per thread: no
 * table and no lock. What one call needs (the system's table of mounts, the
 * labels udev links) it takes through `parts/alloc.c`, the single allocation
 * point (the net's `runtime` suite, rule 1), and gives back before it
 * returns.
 *
 * ONE ENTRY IS BUILT WHOLE BEFORE IT IS WRITTEN: a mount the platform lists
 * and then will not tell the size of is left out and never half-written, so
 * the fill form's answer is a whole number of entries.
 */

#include "hero_volumes.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <winnetwk.h>
#include <wchar.h>
/* The library the member needs, named by the member, as `hero_sys.c` names
 * its own: the request travels in the object and the linker reads it only
 * where it takes this member. `mpr` names the share a network drive is
 * connected to; MSVC's word, a MinGW target links by `-l`. */
#if defined(_MSC_VER)
#pragma comment(lib, "mpr.lib")
#endif
#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/attr.h>
#include <sys/mount.h>
#include <sys/param.h>
#endif
#if defined(__linux__)
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#endif
#endif

static _Thread_local int64_t hero_volumes_kind;
static _Thread_local int64_t hero_volumes_os;

static int64_t hero_volumes_fail(int64_t kind, int64_t os) {
    hero_volumes_kind = kind;
    hero_volumes_os = os;
    return -1;
}

int64_t hero_volumes_last_error(void) {
    return hero_volumes_kind;
}

int64_t hero_volumes_last_os_error(void) {
    return hero_volumes_os;
}

/* The helpers below serve the three arms that list; a POSIX system with no
 * table of mounts compiles none of them. */
#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)

static void hero_volumes_fine(void) {
    hero_volumes_kind = HERO_VOLUMES_OK;
    hero_volumes_os = 0;
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_volumes_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_ACCESS_DENIED:
        return HERO_VOLUMES_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
        return HERO_VOLUMES_LIMIT;
    case ERROR_NOT_SUPPORTED:
    case ERROR_CALL_NOT_IMPLEMENTED:
        return HERO_VOLUMES_UNSUPPORTED;
#else
    case EACCES:
    case EPERM:
        return HERO_VOLUMES_DENIED;
    case ENOMEM:
    case EMFILE:
    case ENFILE:
    case EAGAIN:
        return HERO_VOLUMES_LIMIT;
    case ENOSYS:
    case ENOTSUP:
        return HERO_VOLUMES_UNSUPPORTED;
#endif
    default:
        return HERO_VOLUMES_OTHER;
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
} hero_volumes_fill;

/* `n` more bytes of the answer: written where all of them fit behind the ones
 * before, counted either way. Once one piece did not fit, `used` is past
 * `count` and no later piece is written, so nothing lands past the buffer. */
static void hero_volumes_put(hero_volumes_fill *f, const void *bytes, size_t n) {
    if (n > 0 && f->used <= f->count && (uint64_t)n <= f->count - f->used) {
        memcpy(f->room + f->used, bytes, n);
    }
    if (__builtin_add_overflow(f->used, (uint64_t)n, &f->used)) {
        f->over = true;
    }
}

/* One field: `n` bytes and the zero byte that ends them. */
static void hero_volumes_field(hero_volumes_fill *f, const char *bytes, size_t n) {
    hero_volumes_put(f, bytes, n);
    hero_volumes_put(f, "", 1);
}

static void hero_volumes_decimal(hero_volumes_fill *f, uint64_t value) {
    char digits[21];
    size_t n = 0;

    do {
        digits[sizeof digits - 2 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    digits[sizeof digits - 1] = '\0';
    hero_volumes_put(f, digits + sizeof digits - 1 - n, n + 1);
}

static void hero_volumes_flag(hero_volumes_fill *f, bool on) {
    hero_volumes_field(f, on ? "1" : "0", 1);
}

#if defined(__APPLE__) || defined(__linux__)

/* `blocks` of `size` bytes, false where the product is past what an
 * `int64_t` holds: the caller reads every figure as one. */
static bool hero_volumes_bytes(uint64_t blocks, uint64_t size, uint64_t *out) {
    uint64_t product = 0;

    if (__builtin_mul_overflow(blocks, size, &product) || product > (uint64_t)INT64_MAX) {
        return false;
    }
    *out = product;
    return true;
}

#endif

/* The three figures and four marks of one volume, beside the texts its arm
 * writes. */
typedef struct {
    uint64_t total;
    uint64_t free;
    uint64_t available;
    bool read_only;
    bool network;
    bool removable;
    bool hidden;
} hero_volumes_facts;

/* Fields 4 to 10 of an entry, the texts before them written by the arm. */
static void hero_volumes_emit_facts(hero_volumes_fill *f, const hero_volumes_facts *facts) {
    hero_volumes_decimal(f, facts->total);
    hero_volumes_decimal(f, facts->free);
    hero_volumes_decimal(f, facts->available);
    hero_volumes_flag(f, facts->read_only);
    hero_volumes_flag(f, facts->network);
    hero_volumes_flag(f, facts->removable);
    hero_volumes_flag(f, facts->hidden);
}

/* What a fill form answers: the bytes of the whole answer. */
static int64_t hero_volumes_filled(const hero_volumes_fill *f) {
    if (f->over || f->used > (uint64_t)INT64_MAX) {
        return hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
    }
    hero_volumes_fine();
    return (int64_t)f->used;
}

#endif

#if defined(__APPLE__)

/* The room a volume's name is read into: at most 255 UTF-16 units, three
 * bytes each, and the zero after them (`getattrlist(2)`, `ATTR_VOL_NAME`). */
#define HERO_VOLUMES_NAME 1024

/* The name a person gave the volume mounted at `s`, as Finder shows it, into
 * `out`; empty where it has none, where the system will not tell it, and for
 * a volume on another machine, whose name is not asked since the asking
 * waits on its server, which `MNT_NOWAIT` spared the list itself. */
static void hero_volumes_darwin_label(const struct statfs *s, char *out) {
    struct attrlist asked;
    /* The answer: its length, the name's reference, then the name; words
     * keep it aligned as `getattrlist` asks. */
    uint32_t answer[(HERO_VOLUMES_NAME + 16) / 4];
    attrreference_t reference;
    const char *bytes = (const char *)answer;
    size_t whole = sizeof answer;
    size_t at = 0;
    size_t n = 0;

    out[0] = '\0';
    if ((s->f_flags & MNT_LOCAL) == 0) {
        return;
    }
    memset(&asked, 0, sizeof asked);
    asked.bitmapcount = ATTR_BIT_MAP_COUNT;
    asked.volattr = ATTR_VOL_INFO | ATTR_VOL_NAME;
    if (getattrlist(s->f_mntonname, &asked, answer, sizeof answer, FSOPT_NOFOLLOW) != 0) {
        return;
    }
    memcpy(&reference, bytes + sizeof(uint32_t), sizeof reference);
    /* The offset counts from the reference itself, and the length holds the
     * zero that ends the name. */
    if (reference.attr_dataoffset < 0 || reference.attr_length == 0) {
        return;
    }
    at = sizeof(uint32_t) + (size_t)reference.attr_dataoffset;
    if (at >= whole || (size_t)reference.attr_length > whole - at) {
        return;
    }
    n = strnlen(bytes + at, (size_t)reference.attr_length);
    if (n >= HERO_VOLUMES_NAME) {
        return;
    }
    memcpy(out, bytes + at, n);
    out[n] = '\0';
}

/* Every mount `getfsstat` lists, its figures the system's last where a
 * network volume would make the asking wait (`MNT_NOWAIT`). */
static int64_t hero_volumes_darwin(hero_volumes_fill *f) {
    struct statfs *table = NULL;
    int got = 0;
    int tries = 0;

    for (;;) {
        int listed = getfsstat(NULL, 0, MNT_NOWAIT);
        int room = 0;
        size_t bytes = 0;

        if (listed < 0) {
            int why = errno;

            return hero_volumes_fail(hero_volumes_kind_of(why), why);
        }
        /* Room for the mounts made before the table is read. */
        room = listed + 16;
        if (__builtin_mul_overflow((size_t)room, sizeof *table, &bytes) || bytes > (size_t)INT32_MAX) {
            return hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
        }
        table = hero_layer_alloc_zero((size_t)room, sizeof *table);
        if (table == NULL) {
            return hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
        }
        got = getfsstat(table, (int)bytes, MNT_NOWAIT);
        if (got < 0) {
            int why = errno;

            hero_layer_free(table);
            return hero_volumes_fail(hero_volumes_kind_of(why), why);
        }
        if (got < room) {
            break;
        }
        /* The table filled the room: more were mounted meanwhile. */
        hero_layer_free(table);
        table = NULL;
        tries += 1;
        if (tries == 8) {
            return hero_volumes_fail(HERO_VOLUMES_OTHER, 0);
        }
    }
    for (int i = 0; i < got; i += 1) {
        const struct statfs *s = &table[i];
        hero_volumes_facts facts;
        char label[HERO_VOLUMES_NAME];

        memset(&facts, 0, sizeof facts);
        if (!hero_volumes_bytes(s->f_blocks, s->f_bsize, &facts.total) ||
            !hero_volumes_bytes(s->f_bfree, s->f_bsize, &facts.free) ||
            !hero_volumes_bytes(s->f_bavail, s->f_bsize, &facts.available)) {
            hero_layer_free(table);
            return hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
        }
        facts.read_only = (s->f_flags & MNT_RDONLY) != 0;
        facts.network = (s->f_flags & MNT_LOCAL) == 0;
        facts.removable = (s->f_flags & MNT_REMOVABLE) != 0;
        facts.hidden = (s->f_flags & MNT_DONTBROWSE) != 0;
        hero_volumes_darwin_label(s, label);
        hero_volumes_field(f, s->f_mntonname, strnlen(s->f_mntonname, sizeof s->f_mntonname));
        hero_volumes_field(f, s->f_mntfromname, strnlen(s->f_mntfromname, sizeof s->f_mntfromname));
        hero_volumes_field(f, s->f_fstypename, strnlen(s->f_fstypename, sizeof s->f_fstypename));
        hero_volumes_field(f, label, strlen(label));
        hero_volumes_emit_facts(f, &facts);
    }
    hero_layer_free(table);
    return hero_volumes_filled(f);
}

#elif defined(__linux__)

/* The file systems systemd v262.2 counts as stored on another machine
 * (`fstype_is_network`, `src/basic/mountpoint-util.c`, with the set
 * `@network` of `src/basic/filesystem-sets.py`; read 2026-11-24), each
 * compared with `fuse.` taken off the front, as systemd compares it. */
static const char *const hero_volumes_network_kinds[] = {
    "afs", "ceph", "cifs", "gfs", "gfs2", "ncp", "ncpfs", "nfs", "nfs4", "ocfs2", "orangefs", "pvfs2", "smb3",
    "smbfs", "davfs", "glusterfs", "lustre", "sshfs",
};

static bool hero_volumes_is_network(const char *kind) {
    if (strncmp(kind, "fuse.", 5) == 0) {
        kind += 5;
    }
    for (size_t i = 0; i < sizeof hero_volumes_network_kinds / sizeof hero_volumes_network_kinds[0]; i += 1) {
        if (strcmp(kind, hero_volumes_network_kinds[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* A whole file of the kernel's, read into a block the caller frees: its
 * bytes and a zero after them, or NULL with the error set. */
static char *hero_volumes_read_all(const char *path, size_t *length) {
    int fd = open(path, O_RDONLY | O_CLOEXEC);
    size_t room = 16384;
    size_t used = 0;
    char *text = NULL;

    if (fd < 0) {
        int why = errno;

        (void)hero_volumes_fail(why == ENOENT ? HERO_VOLUMES_UNSUPPORTED : hero_volumes_kind_of(why), why);
        return NULL;
    }
    text = hero_layer_alloc(room);
    if (text == NULL) {
        (void)close(fd);
        (void)hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
        return NULL;
    }
    for (;;) {
        ssize_t got = 0;

        if (used + 1 >= room) {
            char *grown = NULL;

            if (room > ((size_t)1 << 30)) {
                hero_layer_free(text);
                (void)close(fd);
                (void)hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
                return NULL;
            }
            grown = hero_layer_grow(text, room * 2);
            if (grown == NULL) {
                hero_layer_free(text);
                (void)close(fd);
                (void)hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
                return NULL;
            }
            text = grown;
            room *= 2;
        }
        got = read(fd, text + used, room - 1 - used);
        if (got < 0) {
            int why = errno;

            if (why == EINTR) {
                continue;
            }
            hero_layer_free(text);
            (void)close(fd);
            (void)hero_volumes_fail(hero_volumes_kind_of(why), why);
            return NULL;
        }
        if (got == 0) {
            break;
        }
        used += (size_t)got;
    }
    (void)close(fd);
    text[used] = '\0';
    *length = used;
    return text;
}

/* `text` with the kernel's escapes of a mount's fields taken out in place: a
 * backslash and three octal digits are that byte (`\040` a space). */
static void hero_volumes_unescape(char *text) {
    char *to = text;

    for (const char *from = text; *from != '\0'; from += 1) {
        if (from[0] == '\\' && from[1] >= '0' && from[1] <= '3' && from[2] >= '0' && from[2] <= '7' &&
            from[3] >= '0' && from[3] <= '7') {
            *to = (char)(((from[1] - '0') << 6) | ((from[2] - '0') << 3) | (from[3] - '0'));
            to += 1;
            from += 3;
            continue;
        }
        *to = *from;
        to += 1;
    }
    *to = '\0';
}

static int hero_volumes_hex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

/* The room a label is held in: udev's names are at most a file name, 255
 * bytes, and the zero after them. */
#define HERO_VOLUMES_LABEL 256

/* One label udev links in `/dev/disk/by-label`: the device it names and the
 * label, its `\xHH` escapes taken out. */
typedef struct {
    dev_t device;
    char label[HERO_VOLUMES_LABEL];
} hero_volumes_named;

/* Every label udev links, into a block the caller frees; none where udev
 * keeps no such folder (a container, a system without udev), never a
 * failure, since a volume with no label is an answer. NULL only where the
 * memory ran out, with the error set. */
static hero_volumes_named *hero_volumes_labels(size_t *count) {
    DIR *folder = opendir("/dev/disk/by-label");
    hero_volumes_named *named = NULL;
    size_t room = 0;
    size_t used = 0;
    struct dirent *entry = NULL;

    *count = 0;
    named = hero_layer_alloc(sizeof *named);
    if (named == NULL) {
        if (folder != NULL) {
            (void)closedir(folder);
        }
        (void)hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
        return NULL;
    }
    room = 1;
    if (folder == NULL) {
        return named;
    }
    while ((entry = readdir(folder)) != NULL) {
        struct stat facts;
        hero_volumes_named one;
        size_t at = 0;

        if (entry->d_name[0] == '.') {
            continue;
        }
        if (fstatat(dirfd(folder), entry->d_name, &facts, 0) != 0 || !S_ISBLK(facts.st_mode)) {
            continue;
        }
        one.device = facts.st_rdev;
        for (const char *c = entry->d_name; *c != '\0' && at + 1 < sizeof one.label; c += 1) {
            int high = -1;
            int low = -1;

            if (c[0] == '\\' && c[1] == 'x' && (high = hero_volumes_hex(c[2])) >= 0 &&
                (low = hero_volumes_hex(c[3])) >= 0) {
                one.label[at] = (char)((high << 4) | low);
                c += 3;
            } else {
                one.label[at] = *c;
            }
            at += 1;
        }
        one.label[at] = '\0';
        if (used == room) {
            hero_volumes_named *grown = hero_layer_grow(named, room * 2 * sizeof *named);

            if (grown == NULL) {
                hero_layer_free(named);
                (void)closedir(folder);
                (void)hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
                return NULL;
            }
            named = grown;
            room *= 2;
        }
        named[used] = one;
        used += 1;
    }
    (void)closedir(folder);
    *count = used;
    return named;
}

/* Whether the kernel says the disk under the device `major`:`minor` can be
 * taken out: its own `removable`, or its disk's where it is a partition. A
 * device with no disk under it (major 0: a file system that stores nothing
 * or one on another machine) is not. */
static bool hero_volumes_removable(unsigned major, unsigned minor) {
    char path[96];
    int fd = -1;
    char mark = '0';

    if (major == 0) {
        return false;
    }
    (void)snprintf(path, sizeof path, "/sys/dev/block/%u:%u/partition", major, minor);
    if (access(path, F_OK) == 0) {
        (void)snprintf(path, sizeof path, "/sys/dev/block/%u:%u/../removable", major, minor);
    } else {
        (void)snprintf(path, sizeof path, "/sys/dev/block/%u:%u/removable", major, minor);
    }
    fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return false;
    }
    if (read(fd, &mark, 1) != 1) {
        mark = '0';
    }
    (void)close(fd);
    return mark == '1';
}

/* The fields of one line of `/proc/self/mountinfo` (proc(5)): the line is
 * cut at its spaces, the fields the answer needs named. */
typedef struct {
    char *numbers;
    char *mount;
    char *kind;
    char *source;
} hero_volumes_line;

/* `line` cut into its fields; false where it is not the kernel's form, ten
 * fields at least with a lone `-` among them after the sixth. */
static bool hero_volumes_cut(char *line, hero_volumes_line *out) {
    char *fields[64];
    size_t n = 0;
    size_t dash = 0;
    char *at = line;

    while (*at != '\0' && n < sizeof fields / sizeof fields[0]) {
        fields[n] = at;
        n += 1;
        while (*at != '\0' && *at != ' ') {
            at += 1;
        }
        if (*at == ' ') {
            *at = '\0';
            at += 1;
        }
    }
    for (size_t i = 6; i < n; i += 1) {
        if (strcmp(fields[i], "-") == 0) {
            dash = i;
            break;
        }
    }
    if (dash == 0 || dash + 2 >= n) {
        return false;
    }
    out->numbers = fields[2];
    out->mount = fields[4];
    out->kind = fields[dash + 1];
    out->source = fields[dash + 2];
    return true;
}

/* Every mount of this process's namespace, in the kernel's order, each asked
 * `statvfs`: one it will not answer (a folder this user may not enter) is
 * left out. */
static int64_t hero_volumes_linux(hero_volumes_fill *f) {
    size_t length = 0;
    char *text = hero_volumes_read_all("/proc/self/mountinfo", &length);
    hero_volumes_named *named = NULL;
    size_t labels = 0;
    char *line = NULL;

    if (text == NULL) {
        return -1;
    }
    named = hero_volumes_labels(&labels);
    if (named == NULL) {
        hero_layer_free(text);
        return -1;
    }
    line = text;
    while (line < text + length) {
        char *end = memchr(line, '\n', (size_t)(text + length - line));
        char *next = end == NULL ? text + length : end + 1;
        hero_volumes_line cut;
        unsigned major = 0;
        unsigned minor = 0;
        struct statvfs room;
        hero_volumes_facts facts;
        unsigned long unit = 0;
        const char *label = "";

        if (end != NULL) {
            *end = '\0';
        }
        if (!hero_volumes_cut(line, &cut) || sscanf(cut.numbers, "%u:%u", &major, &minor) != 2) {
            line = next;
            continue;
        }
        hero_volumes_unescape(cut.mount);
        hero_volumes_unescape(cut.source);
        hero_volumes_unescape(cut.kind);
        if (statvfs(cut.mount, &room) != 0) {
            line = next;
            continue;
        }
        memset(&facts, 0, sizeof facts);
        unit = room.f_frsize != 0 ? room.f_frsize : room.f_bsize;
        if (!hero_volumes_bytes((uint64_t)room.f_blocks, (uint64_t)unit, &facts.total) ||
            !hero_volumes_bytes((uint64_t)room.f_bfree, (uint64_t)unit, &facts.free) ||
            !hero_volumes_bytes((uint64_t)room.f_bavail, (uint64_t)unit, &facts.available)) {
            hero_layer_free(named);
            hero_layer_free(text);
            return hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
        }
        facts.read_only = (room.f_flag & ST_RDONLY) != 0;
        facts.network = hero_volumes_is_network(cut.kind);
        facts.removable = hero_volumes_removable(major, minor);
        facts.hidden = false;
        if (major != 0) {
            for (size_t i = 0; i < labels; i += 1) {
                if (named[i].device == makedev(major, minor)) {
                    label = named[i].label;
                    break;
                }
            }
        }
        hero_volumes_field(f, cut.mount, strlen(cut.mount));
        hero_volumes_field(f, cut.source, strlen(cut.source));
        hero_volumes_field(f, cut.kind, strlen(cut.kind));
        hero_volumes_field(f, label, strlen(label));
        hero_volumes_emit_facts(f, &facts);
        line = next;
    }
    hero_layer_free(named);
    hero_layer_free(text);
    return hero_volumes_filled(f);
}

#elif defined(_WIN32)

/* `units` UTF-16 units, or up to the first zero where `units` is -1, written
 * into the answer as UTF-8 and ended by a zero byte: a pair as its one
 * character, and a unit that is half of no pair as the three bytes its own
 * number takes, as `hero_disk.h` gives such a name, so a path the answer
 * holds is one the layer takes back. */
static void hero_volumes_wide(hero_volumes_fill *f, const WCHAR *wide, int64_t units) {
    size_t length = units < 0 ? wcslen(wide) : (size_t)units;

    for (size_t i = 0; i < length; i += 1) {
        uint32_t c = (uint32_t)wide[i];
        unsigned char b[4];
        size_t n = 0;

        if (c >= 0xD800u && c <= 0xDBFFu && i + 1 < length) {
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
        hero_volumes_put(f, b, n);
    }
    hero_volumes_put(f, "", 1);
}

/* The room the system's texts of one volume are read into: a label and a
 * file system's name are at most `MAX_PATH` units
 * (`GetVolumeInformationW`), a volume's name 49 and its zero, and a share's
 * name as long as a path may be. */
#define HERO_VOLUMES_SHARE 32768

/* The volume mounted at `root`, a drive's root or a folder ending in `\`,
 * written as one entry; nothing where the system will not tell its size (a
 * drive with no medium, a share that does not answer). `named` is the
 * volume's own name where the list of folders gave it, else NULL. */
static void hero_volumes_root(hero_volumes_fill *f, const WCHAR *root, const WCHAR *named, WCHAR *share) {
    UINT type = GetDriveTypeW(root);
    WCHAR label[MAX_PATH + 1];
    WCHAR kind[MAX_PATH + 1];
    WCHAR volume[50];
    DWORD flags = 0;
    ULARGE_INTEGER available;
    ULARGE_INTEGER total;
    ULARGE_INTEGER free;
    const WCHAR *device = L"";
    hero_volumes_facts facts;

    if (type == DRIVE_NO_ROOT_DIR) {
        return;
    }
    if (!GetVolumeInformationW(root, label, MAX_PATH + 1, NULL, NULL, &flags, kind, MAX_PATH + 1)) {
        return;
    }
    if (!GetDiskFreeSpaceExW(root, &available, &total, &free)) {
        return;
    }
    memset(&facts, 0, sizeof facts);
    if (total.QuadPart > (ULONGLONG)INT64_MAX || free.QuadPart > (ULONGLONG)INT64_MAX ||
        available.QuadPart > (ULONGLONG)INT64_MAX) {
        f->over = true;
        return;
    }
    facts.total = total.QuadPart;
    facts.free = free.QuadPart;
    facts.available = available.QuadPart;
    facts.read_only = (flags & FILE_READ_ONLY_VOLUME) != 0;
    facts.network = type == DRIVE_REMOTE;
    facts.removable = type == DRIVE_REMOVABLE || type == DRIVE_CDROM;
    facts.hidden = false;
    if (named != NULL) {
        device = named;
    } else if (type == DRIVE_REMOTE) {
        WCHAR letter[3] = {root[0], L':', L'\0'};
        DWORD room = HERO_VOLUMES_SHARE;

        if (WNetGetConnectionW(letter, share, &room) == NO_ERROR) {
            device = share;
        }
    } else if (GetVolumeNameForVolumeMountPointW(root, volume, 50)) {
        device = volume;
    } else {
        /* A letter `subst` gave a folder names that folder. */
        WCHAR letter[3] = {root[0], L':', L'\0'};

        if (QueryDosDeviceW(letter, share, HERO_VOLUMES_SHARE) != 0) {
            device = share;
        }
    }
    hero_volumes_wide(f, root, -1);
    hero_volumes_wide(f, device, -1);
    hero_volumes_wide(f, kind, -1);
    hero_volumes_wide(f, label, -1);
    hero_volumes_emit_facts(f, &facts);
}

/* Every drive letter, then every folder a local volume is mounted on that is
 * not a drive's root, the system's dialogs for a drive with no medium kept
 * from the screen while it asks. */
static int64_t hero_volumes_windows(hero_volumes_fill *f) {
    WCHAR letters[26 * 4 + 2];
    DWORD written = GetLogicalDriveStringsW((DWORD)(sizeof letters / sizeof letters[0]), letters);
    WCHAR *share = NULL;
    WCHAR volume[MAX_PATH + 1];
    HANDLE volumes = INVALID_HANDLE_VALUE;
    DWORD before = 0;
    int64_t answer = 0;
    bool moded = false;

    if (written == 0 || written >= sizeof letters / sizeof letters[0]) {
        DWORD why = GetLastError();

        return hero_volumes_fail(hero_volumes_kind_of((int64_t)why), (int64_t)why);
    }
    share = hero_layer_alloc(HERO_VOLUMES_SHARE * sizeof(WCHAR));
    if (share == NULL) {
        return hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
    }
    moded = SetThreadErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX, &before) != 0;
    for (WCHAR *root = letters; *root != L'\0'; root += wcslen(root) + 1) {
        hero_volumes_root(f, root, NULL, share);
    }
    volumes = FindFirstVolumeW(volume, MAX_PATH + 1);
    if (volumes == INVALID_HANDLE_VALUE) {
        DWORD why = GetLastError();

        if (why != ERROR_NO_MORE_FILES) {
            answer = hero_volumes_fail(hero_volumes_kind_of((int64_t)why), (int64_t)why);
        }
    } else {
        for (;;) {
            DWORD need = 0;
            WCHAR *paths = NULL;

            if (!GetVolumePathNamesForVolumeNameW(volume, NULL, 0, &need) && GetLastError() == ERROR_MORE_DATA &&
                need > 0) {
                paths = hero_layer_alloc((size_t)need * sizeof(WCHAR));
                if (paths == NULL) {
                    answer = hero_volumes_fail(HERO_VOLUMES_LIMIT, 0);
                    break;
                }
                if (GetVolumePathNamesForVolumeNameW(volume, paths, need, &need)) {
                    for (WCHAR *path = paths; *path != L'\0'; path += wcslen(path) + 1) {
                        bool letter = wcslen(path) == 3 && path[1] == L':' && path[2] == L'\\';

                        if (!letter) {
                            hero_volumes_root(f, path, volume, share);
                        }
                    }
                }
                hero_layer_free(paths);
            }
            if (!FindNextVolumeW(volumes, volume, MAX_PATH + 1)) {
                DWORD why = GetLastError();

                if (why != ERROR_NO_MORE_FILES) {
                    answer = hero_volumes_fail(hero_volumes_kind_of((int64_t)why), (int64_t)why);
                }
                break;
            }
        }
        (void)FindVolumeClose(volumes);
    }
    if (moded) {
        (void)SetThreadErrorMode(before, NULL);
    }
    hero_layer_free(share);
    if (answer < 0) {
        return answer;
    }
    return hero_volumes_filled(f);
}

#endif

int64_t hero_volumes_list(void *buffer, uint64_t count) {
    if (buffer == NULL && count > 0) {
        return hero_volumes_fail(HERO_VOLUMES_INVALID, 0);
    }
#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
    hero_volumes_fill f;

    f.room = buffer;
    f.count = count;
    f.used = 0;
    f.over = false;
#if defined(__APPLE__)
    return hero_volumes_darwin(&f);
#elif defined(__linux__)
    return hero_volumes_linux(&f);
#else
    return hero_volumes_windows(&f);
#endif
#else
    return hero_volumes_fail(HERO_VOLUMES_UNSUPPORTED, 0);
#endif
}
