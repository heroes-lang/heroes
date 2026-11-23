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
 * launchd's `launch_activate_socket`. Each is adopted into `hero_net.h`'s
 * table through `hero_net_adopt`, which ASKS what the descriptor is; what is
 * no socket of that table (a FIFO, a local socket) is left open and told as
 * an entry of its own kind rather than refused whole, since a unit may hand a
 * program several kinds and the program decides which it serves.
 *
 * ONCE: systemd's descriptors are adopted once in the life of a process
 * (`hero_activation_spent`), since the table owns a descriptor it adopts
 * and closes it with its handle: a second adoption after the program closed
 * one could take a descriptor number another layer has reused since.
 */

#include "hero_activation.h"

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

int64_t hero_activation_count(void) {
    return hero_activation_count_for(getenv("LISTEN_PID"), getenv("LISTEN_FDS"), (int64_t)getpid());
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

/* The sockets already taken into `entries`, closed. */
static void hero_activation_unwind(const hero_activation_entry *entries, int64_t filled) {
    for (int64_t at = 0; at < filled; at += 1) {
        if (entries[at].number >= 0) {
            hero_net_socket *held = hero_net_of(entries[at].number);

            if (held != NULL) {
                (void)hero_net_close(held);
            }
        }
    }
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
