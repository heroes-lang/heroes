/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_activation.c — the platform layer's two protocols a POSIX
 * service manager speaks to the program it starts, behind `hero_activation.h`
 * (M-rebel-library, 2026-11-23; spec § 13).
 *
 * THE NOTICE is systemd's `sd_notify` protocol: one datagram of
 * newline-separated `NAME=value` lines sent to the local datagram socket
 * `$NOTIFY_SOCKET` names, a path or, on Linux, `@` and an abstract name. The
 * layer sends one datagram from a socket made for the call and closed with
 * it, never waiting: a notice that cannot be queued is told, because a
 * program that blocks on its supervisor's queue is a program whose watchdog
 * has not been fed.
 *
 * THE SOCKETS HANDED OVER are systemd's `$LISTEN_PID` and `$LISTEN_FDS`
 * (descriptors 3 to 3 + `$LISTEN_FDS` - 1, the pid saying they were meant for
 * this process and not for a child that inherited the variables) and
 * launchd's `launch_activate_socket`. Each is ASKED what it is and adopted
 * into the table of the member that holds its kind: `hero_net.h`'s for a
 * TCP or UDP socket (`hero_net_adopt`), `hero_ipc.h`'s for a local stream
 * socket (`hero_ipc_adopt_listener`, `hero_ipc_adopt_stream`) and
 * `hero_stream.h`'s for a FIFO, a device or a file (`hero_stream_adopt`); a
 * local listener or a stream is kept here under a number the program takes
 * it back by, once. What no member holds (a local datagram socket, a
 * netlink one, a message queue) is left open and told as an entry of its own
 * kind rather than refused whole, since a unit may hand a program several
 * kinds and the program decides which it serves.
 *
 * ONCE: systemd's descriptors are adopted once in the life of a process
 * (`hero_activation_spent`), since the table owns a descriptor it adopts
 * and closes it with its handle: a second adoption after the program closed
 * one could take a descriptor number another layer has reused since.
 */

#include "hero_activation.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"
#include "hero_net.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(_WIN32)
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <launch.h>
#endif
#if defined(__linux__)
#include <sys/syscall.h>
#include <sys/vfs.h>
#endif
#endif

static _Thread_local int64_t hero_activation_kind;
static _Thread_local int64_t hero_activation_os;
static _Thread_local char hero_activation_words[192];

static int64_t hero_activation_fail(int64_t kind, int64_t os, const char *words) {
    hero_activation_kind = kind;
    hero_activation_os = os;
    hero_activation_words[0] = '\0';
    if (words != NULL) {
        (void)snprintf(hero_activation_words, sizeof hero_activation_words, "%s", words);
    }
    return -1;
}

int64_t hero_activation_last_error(void) {
    return hero_activation_kind;
}

int64_t hero_activation_last_os_error(void) {
    return hero_activation_os;
}

int64_t hero_activation_last_message(void *buffer, uint64_t n) {
    uint64_t length = (uint64_t)strlen(hero_activation_words);

    if (buffer != NULL && n >= length) {
        memcpy(buffer, hero_activation_words, (size_t)length);
    }
    return (int64_t)length;
}

/* THE VALUES KEPT FOR THE PROGRAM: a local listener or a stream an adoption
 * made is kept here under a number, which the entry carries across to the
 * program, and handed over once by the takers, so a number can never give
 * two owners one value, and a number this layer never wrote names nothing.
 * Each place is spent once: a process is handed systemd's descriptors once
 * and each launchd name's once, so twice `HERO_ACTIVATION_MOST` places hold
 * every adoption a process can make but a launchd job naming thousands of
 * local sockets, which is told `HERO_ACTIVATION_LIMIT`. */
#define HERO_ACTIVATION_KEPT_MOST (2 * HERO_ACTIVATION_MOST)
static _Atomic uint64_t hero_activation_kept[HERO_ACTIVATION_KEPT_MOST];

/* The handle kept under `number` where it has the tag `tag`, taken out;
 * 0 with the failure set where there is none or it has another tag. */
static uint64_t hero_activation_take_kept(int64_t number, uint64_t tag) {
    if (number < 1 || number > HERO_ACTIVATION_KEPT_MOST) {
        (void)hero_activation_fail(HERO_ACTIVATION_NOT_FOUND, 0, "no value is kept under that number");
        return 0;
    }
    _Atomic uint64_t *place = &hero_activation_kept[number - 1];
    uint64_t value = atomic_load(place);

    for (;;) {
        if (value == 0) {
            (void)hero_activation_fail(HERO_ACTIVATION_NOT_FOUND, 0, "the value kept under that number was taken already");
            return 0;
        }
        if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(tag)) {
            (void)hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "the value kept under that number is of another kind");
            return 0;
        }
        if (atomic_compare_exchange_weak(place, &value, 0)) {
            return value;
        }
    }
}

hero_ipc_listener *hero_activation_take_listener(int64_t number) {
    return (hero_ipc_listener *)(uintptr_t)hero_activation_take_kept(number, HERO_HANDLE_TAG_IPC_LISTENER);
}

hero_stream *hero_activation_take_stream(int64_t number) {
    return (hero_stream *)(uintptr_t)hero_activation_take_kept(number, HERO_HANDLE_TAG_STREAM);
}

/* The digits of `text` as a number of 64 bits, 1 to 20 of them and nothing
 * else, as `sd_listen_fds(3)`'s reader takes `$LISTEN_PIDFDID`. */
static bool hero_activation_digits64(const char *text, uint64_t *number) {
    size_t length = strlen(text);
    uint64_t value = 0;

    if (length == 0 || length > 20) {
        return false;
    }
    for (size_t at = 0; at < length; at += 1) {
        if (text[at] < '0' || text[at] > '9') {
            return false;
        }
        uint64_t digit = (uint64_t)(text[at] - '0');

        if (value > (UINT64_MAX - digit) / 10) {
            return false;
        }
        value = value * 10 + digit;
    }
    *number = value;
    return true;
}

int64_t hero_activation_pidfd_check(const char *listed_text, int64_t own_known, uint64_t own) {
    uint64_t listed = 0;

    if (listed_text == NULL) {
        return 1;
    }
    if (!hero_activation_digits64(listed_text, &listed)) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "LISTEN_PIDFDID is no number");
    }
    if (own_known != 1) {
        return 1;
    }
    return listed == own ? 1 : 0;
}

#if defined(_WIN32)

int64_t hero_activation_has(int64_t what) {
    (void)what;
    return 0;
}

static int64_t hero_activation_none(void) {
    return hero_activation_fail(HERO_ACTIVATION_UNSUPPORTED, 0, "this system has no systemd and no launchd");
}

int64_t hero_activation_notify(const char *name, const void *data, uint64_t count) {
    (void)name;
    (void)data;
    (void)count;
    return hero_activation_none();
}

int64_t hero_activation_receive(const char *path, int64_t within_ms, void *buffer, uint64_t n) {
    (void)path;
    (void)within_ms;
    (void)buffer;
    (void)n;
    return hero_activation_none();
}

int64_t hero_activation_count(void) {
    return hero_activation_none();
}

int64_t hero_activation_count_for(const char *pid_text, const char *count_text, int64_t pid) {
    (void)pid_text;
    (void)count_text;
    (void)pid;
    return hero_activation_none();
}

int64_t hero_activation_monotonic_usec(void) {
    return hero_activation_none();
}

int64_t hero_activation_inherit(hero_activation_entry *entries, uint64_t n) {
    (void)entries;
    (void)n;
    return hero_activation_none();
}

int64_t hero_activation_launchd(const char *name, hero_activation_entry *entries, uint64_t n) {
    (void)name;
    (void)entries;
    (void)n;
    return hero_activation_none();
}

int64_t hero_activation_describe(int64_t descriptor, void *buffer, uint64_t n) {
    (void)descriptor;
    (void)buffer;
    (void)n;
    return 0;
}

int64_t hero_activation_own_pidfd_id(void *buffer, uint64_t n) {
    (void)buffer;
    (void)n;
    return 0;
}

#else /* POSIX */

int64_t hero_activation_has(int64_t what) {
    if (what == HERO_ACTIVATION_HAS_NOTIFY || what == HERO_ACTIVATION_HAS_INHERIT) {
        return 1;
    }
#if defined(__APPLE__)
    if (what == HERO_ACTIVATION_HAS_LAUNCHD) {
        return 1;
    }
#endif
    return 0;
}

/* The kind the system's number for a failed socket call is. */
static int64_t hero_activation_kind_of(int code) {
    if (code == ENOENT || code == ECONNREFUSED || code == ENOTDIR || code == ENOTSOCK) {
        return HERO_ACTIVATION_NOT_FOUND;
    }
    if (code == EACCES || code == EPERM) {
        return HERO_ACTIVATION_DENIED;
    }
    if (code == EAGAIN || code == ENOBUFS || code == ENOMEM || code == EMFILE || code == ENFILE || code == EMSGSIZE) {
        return HERO_ACTIVATION_LIMIT;
    }
    if (code == EADDRINUSE || code == EEXIST) {
        return HERO_ACTIVATION_EXISTS;
    }
    return HERO_ACTIVATION_OTHER;
}

/* A datagram socket of the local family, close-on-exec, and on Darwin never
 * raising SIGPIPE; -1, the failure set, where it cannot be made. */
static int hero_activation_socket(void) {
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (fd < 0) {
        (void)hero_activation_fail(hero_activation_kind_of(errno), errno, NULL);
        return -1;
    }
    int flags = fcntl(fd, F_GETFD);
    bool made = flags >= 0 && fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == 0;
#if defined(SO_NOSIGPIPE)
    int one = 1;

    made = made && setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, (socklen_t)sizeof one) == 0;
#endif
    if (!made) {
        (void)hero_activation_fail(HERO_ACTIVATION_OTHER, errno, NULL);
        (void)close(fd);
        return -1;
    }
    return fd;
}

/* The address `name` names: a path, or `@` and a name for the abstract
 * space Linux alone has. False, the failure set, for a name no socket has. */
static bool hero_activation_address(const char *name, struct sockaddr_un *addr, socklen_t *size, bool *abstract) {
    memset(addr, 0, sizeof *addr);
    addr->sun_family = AF_UNIX;
    *abstract = false;
    size_t length = name == NULL ? 0 : strlen(name);

    if (length == 0) {
        (void)hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "the socket's name is empty");
        return false;
    }
    if (strncmp(name, "vsock:", 6) == 0) {
        (void)hero_activation_fail(HERO_ACTIVATION_UNSUPPORTED, 0, "a socket of the virtual machine family is no local socket");
        return false;
    }
    if (name[0] == '@') {
#if defined(__linux__)
        if (length > sizeof addr->sun_path) {
            (void)hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "the abstract name is longer than a socket's address holds");
            return false;
        }
        memcpy(addr->sun_path + 1, name + 1, length - 1);
        *size = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + length);
        *abstract = true;
        return true;
#else
        (void)hero_activation_fail(HERO_ACTIVATION_UNSUPPORTED, 0, "abstract socket names exist on Linux alone");
        return false;
#endif
    }
    if (length >= sizeof addr->sun_path) {
        (void)hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "the path is longer than a socket's address holds");
        return false;
    }
    memcpy(addr->sun_path, name, length);
    *size = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + length + 1);
    return true;
}

int64_t hero_activation_notify(const char *name, const void *data, uint64_t count) {
    struct sockaddr_un addr;
    socklen_t size = 0;
    bool abstract = false;

    if (data == NULL || count == 0 || count > HERO_ACTIVATION_NOTICE_MOST) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "a notice holds 1 to 4,096 bytes");
    }
    if (!hero_activation_address(name, &addr, &size, &abstract)) {
        return -1;
    }
    int fd = hero_activation_socket();

    if (fd < 0) {
        return -1;
    }
    int flags = fcntl(fd, F_GETFL);

    if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) != 0) {
        (void)hero_activation_fail(HERO_ACTIVATION_OTHER, errno, NULL);
        (void)close(fd);
        return -1;
    }
#if defined(MSG_NOSIGNAL)
    int options = MSG_NOSIGNAL;
#else
    int options = 0;
#endif
    ssize_t sent;

    do {
        sent = sendto(fd, data, (size_t)count, options, (const struct sockaddr *)&addr, size);
    } while (sent < 0 && errno == EINTR);
    int code = errno;

    (void)close(fd);
    if (sent < 0) {
        return hero_activation_fail(hero_activation_kind_of(code), code, NULL);
    }
    return sent == (ssize_t)count ? 0 : hero_activation_fail(HERO_ACTIVATION_OTHER, 0, "the system sent part of a datagram");
}

int64_t hero_activation_receive(const char *path, int64_t within_ms, void *buffer, uint64_t n) {
    struct sockaddr_un addr;
    socklen_t size = 0;
    bool abstract = false;

    if (within_ms < 0 || buffer == NULL || n == 0) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "a wait below 0, or a buffer of no bytes");
    }
    if (!hero_activation_address(path, &addr, &size, &abstract)) {
        return -1;
    }
    int fd = hero_activation_socket();

    if (fd < 0) {
        return -1;
    }
    if (bind(fd, (const struct sockaddr *)&addr, size) != 0) {
        int code = errno;

        (void)close(fd);
        return hero_activation_fail(hero_activation_kind_of(code), code, NULL);
    }
    int64_t answer = -1;
    struct pollfd watched = {.fd = fd, .events = POLLIN, .revents = 0};
    int ready;

    do {
        ready = poll(&watched, 1, within_ms > (int64_t)INT_MAX ? INT_MAX : (int)within_ms);
    } while (ready < 0 && errno == EINTR);
    if (ready < 0) {
        answer = hero_activation_fail(HERO_ACTIVATION_OTHER, errno, NULL);
    } else if (ready == 0) {
        answer = hero_activation_fail(HERO_ACTIVATION_TIMEOUT, 0, NULL);
    } else {
        struct iovec part = {.iov_base = buffer, .iov_len = (size_t)n};
        struct msghdr message;

        memset(&message, 0, sizeof message);
        message.msg_iov = &part;
        message.msg_iovlen = 1;
        ssize_t got = recvmsg(fd, &message, 0);

        if (got < 0) {
            answer = hero_activation_fail(hero_activation_kind_of(errno), errno, NULL);
        } else if ((message.msg_flags & MSG_TRUNC) != 0) {
            answer = hero_activation_fail(HERO_ACTIVATION_LIMIT, 0, "the datagram is longer than the buffer");
        } else {
            answer = (int64_t)got;
        }
    }
    if (!abstract) {
        (void)unlink(path);
    }
    (void)close(fd);
    return answer;
}

/* The digits of `text` as a number, 1 to 17 of them and nothing else. */
static bool hero_activation_digits(const char *text, int64_t *number) {
    size_t length = strlen(text);
    int64_t value = 0;

    if (length == 0 || length > 17) {
        return false;
    }
    for (size_t at = 0; at < length; at += 1) {
        if (text[at] < '0' || text[at] > '9') {
            return false;
        }
        value = value * 10 + (text[at] - '0');
    }
    *number = value;
    return true;
}

int64_t hero_activation_count_for(const char *pid_text, const char *count_text, int64_t pid) {
    int64_t listed = 0;
    int64_t count = 0;

    if (pid_text == NULL || count_text == NULL) {
        return 0;
    }
    if (!hero_activation_digits(pid_text, &listed)) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "LISTEN_PID is no number");
    }
    if (listed != pid) {
        return 0;
    }
    if (!hero_activation_digits(count_text, &count)) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "LISTEN_FDS is no number");
    }
    if (count > HERO_ACTIVATION_MOST) {
        return hero_activation_fail(HERO_ACTIVATION_LIMIT, 0, "LISTEN_FDS is above 4,096");
    }
    return count;
}

/* This process's pidfd inode number into `*id`; false where the system has
 * no stable one. The pidfd file system's magic tells a number that names one
 * process from the one inode every pidfd shared before it, and the number
 * is `fstat`'s, which `sd_pidfd_get_inode_id(3)` names on 64 bits. */
static bool hero_activation_own_pidfd(uint64_t *id) {
#if defined(__linux__) && defined(SYS_pidfd_open)
    struct statfs system;
    struct stat info;
    int fd = (int)syscall(SYS_pidfd_open, (pid_t)getpid(), 0);

    if (fd < 0) {
        return false;
    }
    bool stable = sizeof info.st_ino == 8 && fstatfs(fd, &system) == 0 && (uint64_t)system.f_type == UINT64_C(0x50494446) && fstat(fd, &info) == 0;

    if (stable) {
        *id = (uint64_t)info.st_ino;
    }
    (void)close(fd);
    return stable;
#else
    (void)id;
    return false;
#endif
}

int64_t hero_activation_own_pidfd_id(void *buffer, uint64_t n) {
    uint64_t id = 0;
    char digits[24];

    if (!hero_activation_own_pidfd(&id)) {
        return 0;
    }
    int length = snprintf(digits, sizeof digits, "%llu", (unsigned long long)id);

    if (length <= 0) {
        return 0;
    }
    if (buffer != NULL && n >= (uint64_t)length) {
        memcpy(buffer, digits, (size_t)length);
    }
    return (int64_t)length;
}

int64_t hero_activation_count(void) {
    int64_t count = hero_activation_count_for(getenv("LISTEN_PID"), getenv("LISTEN_FDS"), (int64_t)getpid());
    uint64_t own = 0;

    if (count <= 0) {
        return count;
    }
    bool known = hero_activation_own_pidfd(&own);
    int64_t mine = hero_activation_pidfd_check(getenv("LISTEN_PIDFDID"), known ? 1 : 0, own);

    if (mine < 0) {
        return -1;
    }
    return mine == 1 ? count : 0;
}

int64_t hero_activation_monotonic_usec(void) {
    struct timespec now;

    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        return hero_activation_fail(HERO_ACTIVATION_OTHER, errno, NULL);
    }
    return (int64_t)now.tv_sec * 1000000 + (int64_t)(now.tv_nsec / 1000);
}

/* Whether the systemd descriptors were adopted: the process's one flag. */
static _Atomic int hero_activation_spent;

/* Whether an entry of `kind` is kept by this layer rather than numbered by
 * `hero_net.h`. */
static bool hero_activation_is_kept(int64_t kind) {
    return kind == HERO_ACTIVATION_ENTRY_LOCAL_LISTENER || kind == HERO_ACTIVATION_ENTRY_LOCAL_CONNECTION || kind == HERO_ACTIVATION_ENTRY_FIFO ||
           kind == HERO_ACTIVATION_ENTRY_SPECIAL;
}

/* The values already taken into `entries`, closed. */
static void hero_activation_unwind(const hero_activation_entry *entries, int64_t filled) {
    for (int64_t at = 0; at < filled; at += 1) {
        int64_t kind = entries[at].kind;

        if (kind == HERO_ACTIVATION_ENTRY_LOCAL_LISTENER) {
            hero_ipc_listener *held = hero_activation_take_listener(entries[at].number);

            if (held != NULL) {
                (void)hero_ipc_listener_close(held);
            }
        } else if (hero_activation_is_kept(kind)) {
            hero_stream *held = hero_activation_take_stream(entries[at].number);

            if (held != NULL) {
                (void)hero_stream_close(held);
            }
        } else if (kind != HERO_ACTIVATION_ENTRY_OTHER && entries[at].number >= 0) {
            hero_net_socket *held = hero_net_of(entries[at].number);

            if (held != NULL) {
                (void)hero_net_close(held);
            }
        }
    }
}

/* Whether `fd` is a POSIX message queue, which Linux's file system of them
 * shows as a regular file whose bytes are its status, never its messages. */
static bool hero_activation_is_queue(int fd) {
#if defined(__linux__)
    struct statfs system;

    return fstatfs(fd, &system) == 0 && (uint64_t)system.f_type == UINT64_C(0x19800202);
#else
    (void)fd;
    return false;
#endif
}

/* The failure a member's kind `said` is, for the descriptor handed over:
 * -1, or -2 where the member closed it (its table was full). */
static int hero_activation_refused(int64_t said, int64_t os) {
    if (said == HERO_ACTIVATION_LIMIT) {
        (void)hero_activation_fail(HERO_ACTIVATION_LIMIT, os, "the table of local sockets or streams is full");
        return -2;
    }
    if (said == HERO_ACTIVATION_NOT_FOUND) {
        (void)hero_activation_fail(HERO_ACTIVATION_INVALID, os, "a descriptor handed over is not open");
        return -1;
    }
    if (said == HERO_ACTIVATION_EXISTS) {
        (void)hero_activation_fail(HERO_ACTIVATION_EXISTS, 0, "a descriptor handed over is held already");
        return -1;
    }
    (void)hero_activation_fail(HERO_ACTIVATION_OTHER, os, NULL);
    return -1;
}

/* The next place to keep a value in. */
static _Atomic int64_t hero_activation_kept_next;

/* The handle `value` kept, its number (1 or more); -1 where every place is
 * spent, nothing kept. */
static int64_t hero_activation_keep(uint64_t value) {
    int64_t at = atomic_fetch_add(&hero_activation_kept_next, 1);

    if (at < 0 || at >= HERO_ACTIVATION_KEPT_MOST) {
        return -1;
    }
    atomic_store(&hero_activation_kept[at], value);
    return at + 1;
}

/* Takes the descriptor `fd` that `hero_net.h` has no socket for into
 * `entry` as a local listener, a local connection, a FIFO or a special
 * file: 0 where it is one of them, 1 where it is none (an `OTHER` entry is
 * the caller's to write), -1 or -2 as `hero_activation_take` answers. */
static int hero_activation_take_local(int fd, hero_activation_entry *entry) {
    struct stat info;
    uint64_t made = 0;
    int64_t kind = 0;

    if (fstat(fd, &info) != 0) {
        return 1;
    }
    if (S_ISSOCK(info.st_mode)) {
        hero_ipc_listener *listener = hero_ipc_adopt_listener((int64_t)fd);

        if (listener != NULL) {
            made = (uint64_t)(uintptr_t)listener;
            kind = HERO_ACTIVATION_ENTRY_LOCAL_LISTENER;
        } else if (hero_ipc_last_error() == HERO_IPC_INVALID) {
            hero_stream *connection = hero_ipc_adopt_stream((int64_t)fd);

            if (connection == NULL) {
                int64_t said = hero_ipc_last_error();

                if (said == HERO_IPC_INVALID || said == HERO_IPC_UNSUPPORTED) {
                    return 1;
                }
                return hero_activation_refused(said == HERO_IPC_LIMIT ? HERO_ACTIVATION_LIMIT : said == HERO_IPC_NOT_OPEN ? HERO_ACTIVATION_NOT_FOUND : HERO_ACTIVATION_OTHER,
                                               hero_ipc_last_os_error());
            }
            made = (uint64_t)(uintptr_t)connection;
            kind = HERO_ACTIVATION_ENTRY_LOCAL_CONNECTION;
        } else {
            int64_t said = hero_ipc_last_error();

            if (said == HERO_IPC_UNSUPPORTED) {
                return 1;
            }
            return hero_activation_refused(said == HERO_IPC_LIMIT       ? HERO_ACTIVATION_LIMIT
                                           : said == HERO_IPC_NOT_OPEN  ? HERO_ACTIVATION_NOT_FOUND
                                           : said == HERO_IPC_EXISTS    ? HERO_ACTIVATION_EXISTS
                                                                        : HERO_ACTIVATION_OTHER,
                                           hero_ipc_last_os_error());
        }
    } else if (S_ISFIFO(info.st_mode) || S_ISCHR(info.st_mode) || (S_ISREG(info.st_mode) && !hero_activation_is_queue(fd))) {
        hero_stream *stream = hero_stream_adopt((int64_t)fd);

        if (stream == NULL) {
            int64_t said = hero_stream_last_error();

            if (said == HERO_STREAM_INVALID) {
                return 1;
            }
            return hero_activation_refused(said == HERO_STREAM_LIMIT       ? HERO_ACTIVATION_LIMIT
                                           : said == HERO_STREAM_NOT_OPEN  ? HERO_ACTIVATION_NOT_FOUND
                                           : said == HERO_STREAM_EXISTS    ? HERO_ACTIVATION_EXISTS
                                                                           : HERO_ACTIVATION_OTHER,
                                           hero_stream_last_os_error());
        }
        made = (uint64_t)(uintptr_t)stream;
        kind = S_ISFIFO(info.st_mode) ? HERO_ACTIVATION_ENTRY_FIFO : HERO_ACTIVATION_ENTRY_SPECIAL;
    } else {
        return 1;
    }
    int64_t number = hero_activation_keep(made);

    if (number < 0) {
        if (kind == HERO_ACTIVATION_ENTRY_LOCAL_LISTENER) {
            (void)hero_ipc_listener_close((hero_ipc_listener *)(uintptr_t)made);
        } else {
            (void)hero_stream_close((hero_stream *)(uintptr_t)made);
        }
        (void)hero_activation_fail(HERO_ACTIVATION_LIMIT, 0, "more local sockets and streams were handed over than the layer keeps");
        return -2;
    }
    entry->kind = kind;
    entry->number = number;
    entry->descriptor = fd;
    return 0;
}

/* Takes descriptor `fd` into `entry`: 0 where it is one, 1 where it is
 * anything the table has no kind for, left open; -1 where the whole call
 * fails, the failure set, `fd` still open, and -2 where it fails and `fd`
 * has been closed by the table. */
static int hero_activation_take(int fd, hero_activation_entry *entry) {
    int64_t kind = 0;
    hero_net_socket *socket_taken = hero_net_adopt((int64_t)fd, &kind);

    if (socket_taken != NULL) {
        int64_t number = hero_net_number(socket_taken);

        if (number < 0) {
            (void)hero_net_close(socket_taken);
            (void)hero_activation_fail(HERO_ACTIVATION_OTHER, 0, "the socket could not be numbered");
            return -2;
        }
        entry->kind = kind == HERO_NET_ADOPTED_LISTENER ? HERO_ACTIVATION_ENTRY_LISTENER
                      : kind == HERO_NET_ADOPTED_CONNECTION ? HERO_ACTIVATION_ENTRY_CONNECTION
                                                            : HERO_ACTIVATION_ENTRY_DATAGRAM;
        entry->number = number;
        entry->descriptor = fd;
        return 0;
    }
    int64_t said = hero_net_last_error();

    if (said == HERO_NET_UNSUPPORTED || said == HERO_NET_INVALID) {
        int local = hero_activation_take_local(fd, entry);

        if (local != 1) {
            return local;
        }
        int flags = fcntl(fd, F_GETFD);

        if (flags >= 0) {
            (void)fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
        }
        entry->kind = HERO_ACTIVATION_ENTRY_OTHER;
        entry->number = -1;
        entry->descriptor = fd;
        return 1;
    }
    if (said == HERO_NET_LIMIT) {
        (void)hero_activation_fail(HERO_ACTIVATION_LIMIT, hero_net_last_os_error(), "the socket table is full");
        return -2;
    }
    if (said == HERO_NET_NOT_OPEN) {
        (void)hero_activation_fail(HERO_ACTIVATION_INVALID, hero_net_last_os_error(), "a descriptor handed over is not open");
        return -1;
    }
    if (said == HERO_NET_IN_USE) {
        (void)hero_activation_fail(HERO_ACTIVATION_EXISTS, 0, "a descriptor handed over is held already");
        return -1;
    }
    (void)hero_activation_fail(HERO_ACTIVATION_OTHER, hero_net_last_os_error(), NULL);
    return -1;
}

int64_t hero_activation_inherit(hero_activation_entry *entries, uint64_t n) {
    int64_t count = hero_activation_count();

    if (count <= 0) {
        return count;
    }
    if (entries == NULL || n < (uint64_t)count) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "the room is below the descriptors handed over");
    }
    if (atomic_exchange(&hero_activation_spent, 1) != 0) {
        return hero_activation_fail(HERO_ACTIVATION_EXISTS, 0, "the descriptors were taken once already");
    }
    for (int64_t at = 0; at < count; at += 1) {
        if (hero_activation_take((int)(3 + at), &entries[at]) < 0) {
            hero_activation_unwind(entries, at);
            return -1;
        }
    }
    return count;
}

int64_t hero_activation_launchd(const char *name, hero_activation_entry *entries, uint64_t n) {
#if defined(__APPLE__)
    if (name == NULL || name[0] == '\0' || entries == NULL || n < HERO_ACTIVATION_LAUNCHD_MOST) {
        return hero_activation_fail(HERO_ACTIVATION_INVALID, 0, "a name that is empty, or room for fewer than 64 sockets");
    }
    int *fds = NULL;
    size_t count = 0;
    int said = launch_activate_socket(name, &fds, &count);

    if (said != 0) {
        int64_t kind = said == ESRCH || said == ENOENT ? HERO_ACTIVATION_NOT_FOUND
                       : said == EALREADY             ? HERO_ACTIVATION_EXISTS
                       : said == EINVAL               ? HERO_ACTIVATION_INVALID
                                                      : HERO_ACTIVATION_OTHER;

        return hero_activation_fail(kind, said, NULL);
    }
    if (count > HERO_ACTIVATION_LAUNCHD_MOST) {
        for (size_t at = 0; at < count; at += 1) {
            (void)close(fds[at]);
        }
        hero_layer_free(fds);
        return hero_activation_fail(HERO_ACTIVATION_LIMIT, 0, "launchd handed over more than 64 sockets");
    }
    for (size_t at = 0; at < count; at += 1) {
        int taken = hero_activation_take(fds[at], &entries[at]);

        if (taken < 0) {
            hero_activation_unwind(entries, (int64_t)at);
            for (size_t rest = taken == -2 ? at + 1 : at; rest < count; rest += 1) {
                (void)close(fds[rest]);
            }
            hero_layer_free(fds);
            return -1;
        }
    }
    hero_layer_free(fds);
    return (int64_t)count;
#else
    (void)name;
    (void)entries;
    (void)n;
    return hero_activation_fail(HERO_ACTIVATION_UNSUPPORTED, 0, "launchd is Apple's: this system has none");
#endif
}

int64_t hero_activation_describe(int64_t descriptor, void *buffer, uint64_t n) {
    struct stat info;
    char words[96];

    if (descriptor < 0 || descriptor > (int64_t)INT_MAX || fstat((int)descriptor, &info) != 0) {
        return 0;
    }
    if (S_ISFIFO(info.st_mode)) {
        (void)snprintf(words, sizeof words, "a FIFO");
    } else if (S_ISREG(info.st_mode)) {
        (void)snprintf(words, sizeof words, "a regular file");
    } else if (S_ISDIR(info.st_mode)) {
        (void)snprintf(words, sizeof words, "a directory");
    } else if (S_ISCHR(info.st_mode)) {
        (void)snprintf(words, sizeof words, "a character device");
    } else if (S_ISBLK(info.st_mode)) {
        (void)snprintf(words, sizeof words, "a block device");
    } else if (S_ISSOCK(info.st_mode)) {
        struct sockaddr_storage where;
        socklen_t where_size = (socklen_t)sizeof where;
        int type = 0;
        socklen_t type_size = (socklen_t)sizeof type;

        memset(&where, 0, sizeof where);
        if (getsockname((int)descriptor, (struct sockaddr *)&where, &where_size) != 0 || getsockopt((int)descriptor, SOL_SOCKET, SO_TYPE, &type, &type_size) != 0) {
            (void)snprintf(words, sizeof words, "a socket");
        } else {
            const char *kind = type == SOCK_STREAM ? "stream" : type == SOCK_DGRAM ? "datagram" : "of another type";
            const char *family = where.ss_family == AF_UNIX ? "a local" : where.ss_family == AF_INET || where.ss_family == AF_INET6 ? "an Internet" : "a";

            (void)snprintf(words, sizeof words, "%s %s socket", family, kind);
        }
    } else {
        (void)snprintf(words, sizeof words, "a file of a kind this layer does not name");
    }
    size_t length = strlen(words);

    if (buffer != NULL && n >= (uint64_t)length) {
        memcpy(buffer, words, length);
    }
    return (int64_t)length;
}

#endif
