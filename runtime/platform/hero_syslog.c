/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_syslog.c — the system's own log behind `hero_syslog.h`
 * (M-rebel-library, 2026-11-24; spec § 13). Each call opens what it needs and
 * gives it back before it answers, so nothing outlives the call: a datagram
 * socket made for one send, an `os_log_t` made for one line, an event source
 * registered for one event.
 *
 * THE DATAGRAM never waits: a log whose queue is full is told, since a
 * program that blocks on its log is a program that has stopped serving. A
 * journal entry too long for one datagram goes, as the journal's native
 * protocol asks, as an empty datagram carrying one sealed memory file.
 */

#include "hero_syslog.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
/* The event log's and the registry's functions are advapi32's, named here as
 * every member names its own libraries (2026-11-25: until then this member
 * linked only through the runtime object's own directive, `parts/replace.c`). */
#if defined(_MSC_VER)
#pragma comment(lib, "advapi32.lib")
#endif
#else
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <os/log.h>
#endif
#if defined(__linux__)
/* `memfd_create` and the seals are declared by glibc under `_GNU_SOURCE`
 * alone, which a member that includes its own header first cannot define:
 * the call is asked by its number, its flags are the kernel's own header's,
 * and the seals the kernel's numbers as glibc's `bits/fcntl-linux.h` and the
 * kernel's `linux/fcntl.h` write them (read from the Linux arm64 image on
 * 2026-11-24). */
#include <linux/memfd.h>
#include <sys/syscall.h>
#if !defined(F_ADD_SEALS)
#define F_ADD_SEALS 1033
#define F_SEAL_SEAL 0x0001
#define F_SEAL_SHRINK 0x0002
#define F_SEAL_GROW 0x0004
#define F_SEAL_WRITE 0x0008
#endif
#endif
#endif

static _Thread_local int64_t hero_syslog_kind;
static _Thread_local int64_t hero_syslog_os;
static _Thread_local char hero_syslog_words[192];

static int64_t hero_syslog_fail(int64_t kind, int64_t os, const char *words) {
    hero_syslog_kind = kind;
    hero_syslog_os = os;
    hero_syslog_words[0] = '\0';
    if (words != NULL) {
        (void)snprintf(hero_syslog_words, sizeof hero_syslog_words, "%s", words);
    }
    return -1;
}

static int64_t hero_syslog_done(void) {
    hero_syslog_kind = HERO_SYSLOG_OK;
    hero_syslog_os = 0;
    hero_syslog_words[0] = '\0';
    return 0;
}

int64_t hero_syslog_last_error(void) {
    return hero_syslog_kind;
}

int64_t hero_syslog_last_os_error(void) {
    return hero_syslog_os;
}

int64_t hero_syslog_last_message(void *buffer, uint64_t n) {
    uint64_t length = (uint64_t)strlen(hero_syslog_words);

    if (buffer != NULL && n >= length) {
        memcpy(buffer, hero_syslog_words, (size_t)length);
    }
    return (int64_t)length;
}

int64_t hero_syslog_has(int64_t what) {
#if defined(_WIN32)
    return what == HERO_SYSLOG_HAS_EVENT_LOG ? 1 : 0;
#elif defined(__APPLE__)
    return what == HERO_SYSLOG_HAS_DATAGRAM || what == HERO_SYSLOG_HAS_UNIFIED ? 1 : 0;
#else
    return what == HERO_SYSLOG_HAS_DATAGRAM ? 1 : 0;
#endif
}

/* -- the datagram ------------------------------------------------------------ */

#if defined(_WIN32)

int64_t hero_syslog_send(const char *path, const void *data, uint64_t count, int64_t sealed) {
    (void)path;
    (void)data;
    (void)count;
    (void)sealed;
    return hero_syslog_fail(HERO_SYSLOG_UNSUPPORTED, 0, "Windows has no syslog socket and no journal");
}

#else

/* The kind a failed socket call's number is. */
static int64_t hero_syslog_kind_of(int code) {
    switch (code) {
    case ENOENT:
    case ECONNREFUSED:
    case ENOTSOCK:
    case EPROTOTYPE:
        return HERO_SYSLOG_NOT_FOUND;
    case EACCES:
    case EPERM:
        return HERO_SYSLOG_DENIED;
    case EAGAIN:
#if EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
    case ENOBUFS:
    case EMSGSIZE:
        return HERO_SYSLOG_LIMIT;
    default:
        return HERO_SYSLOG_OTHER;
    }
}

/* A local datagram socket, close-on-exec and never blocking; -1 with the
 * failure named. */
static int hero_syslog_socket(void) {
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (fd < 0) {
        (void)hero_syslog_fail(HERO_SYSLOG_OTHER, errno, NULL);
        return -1;
    }
    int flags = fcntl(fd, F_GETFD);
    int status = fcntl(fd, F_GETFL);

    if (flags < 0 || status < 0 || fcntl(fd, F_SETFD, flags | FD_CLOEXEC) != 0 || fcntl(fd, F_SETFL, status | O_NONBLOCK) != 0) {
        int code = errno;

        (void)close(fd);
        (void)hero_syslog_fail(HERO_SYSLOG_OTHER, code, NULL);
        return -1;
    }
#if defined(SO_NOSIGPIPE)
    int one = 1;

    (void)setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, (socklen_t)sizeof one);
#endif
    return fd;
}

#if defined(__linux__)
/* The journal's way for an entry too long for a datagram: the bytes in a
 * memory file, sealed against every change, sent as the one descriptor of an
 * empty datagram. 0, or -1 with the failure named. */
static int64_t hero_syslog_send_sealed(int fd, const struct sockaddr_un *address, socklen_t size, const void *data, uint64_t count) {
    int memory = (int)syscall(SYS_memfd_create, "hero-journal", (unsigned int)(MFD_CLOEXEC | MFD_ALLOW_SEALING));

    if (memory < 0) {
        return hero_syslog_fail(HERO_SYSLOG_OTHER, errno, "no memory file for a journal entry too long for a datagram");
    }
    const unsigned char *at = (const unsigned char *)data;
    uint64_t left = count;

    while (left > 0) {
        ssize_t wrote = write(memory, at, left);

        if (wrote < 0 && errno == EINTR) {
            continue;
        }
        if (wrote <= 0) {
            int code = errno;

            (void)close(memory);
            return hero_syslog_fail(HERO_SYSLOG_OTHER, code, NULL);
        }
        at += wrote;
        left -= (uint64_t)wrote;
    }
    if (fcntl(memory, F_ADD_SEALS, F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE) != 0) {
        int code = errno;

        (void)close(memory);
        return hero_syslog_fail(HERO_SYSLOG_OTHER, code, NULL);
    }
    union {
        struct cmsghdr align;
        char room[CMSG_SPACE(sizeof(int))];
    } control;
    struct msghdr message;

    memset(&control, 0, sizeof control);
    memset(&message, 0, sizeof message);
    message.msg_name = (void *)address;
    message.msg_namelen = size;
    message.msg_control = control.room;
    message.msg_controllen = sizeof control.room;
    struct cmsghdr *header = CMSG_FIRSTHDR(&message);

    header->cmsg_level = SOL_SOCKET;
    header->cmsg_type = SCM_RIGHTS;
    header->cmsg_len = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(header), &memory, sizeof(int));
    ssize_t sent = 0;

    do {
        sent = sendmsg(fd, &message, MSG_NOSIGNAL);
    } while (sent < 0 && errno == EINTR);
    int code = errno;

    (void)close(memory);
    if (sent < 0) {
        return hero_syslog_fail(hero_syslog_kind_of(code), code, NULL);
    }
    return hero_syslog_done();
}
#endif

int64_t hero_syslog_send(const char *path, const void *data, uint64_t count, int64_t sealed) {
    struct sockaddr_un address;

    if (path == NULL || data == NULL || count == 0 || (sealed != 0 && sealed != 1)) {
        return hero_syslog_fail(HERO_SYSLOG_INVALID, 0, "no path, no bytes, or a sealed flag that is neither 0 nor 1");
    }
    size_t length = strlen(path);

    memset(&address, 0, sizeof address);
    if (length == 0 || length >= sizeof address.sun_path) {
        return hero_syslog_fail(HERO_SYSLOG_INVALID, 0, "the socket's path is empty or longer than an address holds");
    }
    address.sun_family = AF_UNIX;
    memcpy(address.sun_path, path, length + 1);
    socklen_t size = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + length + 1);
#if defined(__APPLE__)
    address.sun_len = (unsigned char)size;
#endif
    int fd = hero_syslog_socket();

    if (fd < 0) {
        return -1;
    }
    ssize_t sent = 0;

    do {
#if defined(MSG_NOSIGNAL)
        sent = sendto(fd, data, (size_t)count, MSG_NOSIGNAL, (const struct sockaddr *)&address, size);
#else
        sent = sendto(fd, data, (size_t)count, 0, (const struct sockaddr *)&address, size);
#endif
    } while (sent < 0 && errno == EINTR);
    int code = errno;

    if (sent < 0 && code == EMSGSIZE && sealed == 1) {
#if defined(__linux__)
        int64_t said = hero_syslog_send_sealed(fd, &address, size, data, count);

        (void)close(fd);
        return said;
#endif
    }
    (void)close(fd);
    if (sent < 0) {
        return hero_syslog_fail(hero_syslog_kind_of(code), code, NULL);
    }
    if ((uint64_t)sent != count) {
        return hero_syslog_fail(HERO_SYSLOG_LIMIT, 0, "the datagram was cut short");
    }
    return hero_syslog_done();
}

#endif

/* -- the unified log --------------------------------------------------------- */

/* Whether `line` is 1 to `HERO_SYSLOG_LINE_MOST` bytes long. */
static bool hero_syslog_line_fits(const char *line) {
    return line != NULL && strlen(line) <= HERO_SYSLOG_LINE_MOST;
}

int64_t hero_syslog_unified(const char *subsystem, const char *category, int64_t level, const char *line) {
    if (subsystem == NULL || subsystem[0] == '\0' || category == NULL || level < 0 || level > HERO_SYSLOG_LEVEL_MOST || !hero_syslog_line_fits(line)) {
        return hero_syslog_fail(HERO_SYSLOG_INVALID, 0, "an empty subsystem, a level outside 0 to 7, or a line too long");
    }
#if defined(__APPLE__)
    os_log_t log = os_log_create(subsystem, category);

    if (log == NULL) {
        return hero_syslog_fail(HERO_SYSLOG_OTHER, 0, "the unified log made no log object");
    }
    os_log_type_t type = level <= 2 ? OS_LOG_TYPE_FAULT : level == 3 ? OS_LOG_TYPE_ERROR : level <= 5 ? OS_LOG_TYPE_DEFAULT : level == 6 ? OS_LOG_TYPE_INFO : OS_LOG_TYPE_DEBUG;

    os_log_with_type(log, type, "%{public}s", line);
    os_release(log);
    return hero_syslog_done();
#else
    return hero_syslog_fail(HERO_SYSLOG_UNSUPPORTED, 0, "the unified log is Apple's: this system has none");
#endif
}

/* -- the Event Log ------------------------------------------------------------ */

#if defined(_WIN32)

/* The registry key of `source` under the Application log, as UTF-16 into
 * `key`; false with the failure named. */
static bool hero_syslog_key(const char *source, wchar_t *key, int room) {
    static const wchar_t prefix[] = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\";
    int used = (int)(sizeof prefix / sizeof prefix[0]) - 1;

    if (source == NULL || source[0] == '\0' || strchr(source, '\\') != NULL) {
        (void)hero_syslog_fail(HERO_SYSLOG_INVALID, 0, "a source is a name with no backslash");
        return false;
    }
    memcpy(key, prefix, sizeof prefix);
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, source, -1, key + used, room - used) == 0) {
        (void)hero_syslog_fail(HERO_SYSLOG_INVALID, (int64_t)GetLastError(), "a source is UTF-8 of at most 256 characters");
        return false;
    }
    return true;
}

/* The kind a failed registry or event call's number is. */
static int64_t hero_syslog_win_kind(DWORD code) {
    return code == ERROR_ACCESS_DENIED ? HERO_SYSLOG_DENIED : code == ERROR_FILE_NOT_FOUND ? HERO_SYSLOG_NOT_FOUND : HERO_SYSLOG_OTHER;
}

int64_t hero_syslog_event(const char *source, int64_t level, int64_t id, const char *line) {
    wchar_t name[257];
    wchar_t *text = NULL;

    if (source == NULL || source[0] == '\0' || level < 0 || level > HERO_SYSLOG_LEVEL_MOST || id < 1 || id > 1000 || !hero_syslog_line_fits(line)) {
        return hero_syslog_fail(HERO_SYSLOG_INVALID, 0, "an empty source, a level outside 0 to 7, an id outside 1 to 1000, or a line too long");
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, source, -1, name, 257) == 0) {
        return hero_syslog_fail(HERO_SYSLOG_INVALID, (int64_t)GetLastError(), "a source is UTF-8 of at most 256 characters");
    }
    int units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, line, -1, NULL, 0);

    if (units <= 0) {
        return hero_syslog_fail(HERO_SYSLOG_INVALID, (int64_t)GetLastError(), "the line is no UTF-8");
    }
    text = (wchar_t *)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)units * sizeof(wchar_t));
    if (text == NULL) {
        return hero_syslog_fail(HERO_SYSLOG_LIMIT, 0, "no memory for the line");
    }
    (void)MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, line, -1, text, units);
    HANDLE log = RegisterEventSourceW(NULL, name);

    if (log == NULL) {
        DWORD code = GetLastError();

        (void)HeapFree(GetProcessHeap(), 0, text);
        return hero_syslog_fail(hero_syslog_win_kind(code), (int64_t)code, NULL);
    }
    WORD type = level <= 3 ? EVENTLOG_ERROR_TYPE : level == 4 ? EVENTLOG_WARNING_TYPE : EVENTLOG_INFORMATION_TYPE;
    LPCWSTR strings[1] = {text};
    BOOL reported = ReportEventW(log, type, 0, (DWORD)id, NULL, 1, 0, strings, NULL);
    DWORD code = reported ? 0 : GetLastError();

    (void)DeregisterEventSource(log);
    (void)HeapFree(GetProcessHeap(), 0, text);
    if (!reported) {
        return hero_syslog_fail(hero_syslog_win_kind(code), (int64_t)code, NULL);
    }
    return hero_syslog_done();
}

int64_t hero_syslog_event_install(const char *source) {
    static const wchar_t file[] = L"%SystemRoot%\\System32\\EventCreate.exe";
    wchar_t key[400];
    HKEY made = NULL;
    DWORD disposition = 0;
    DWORD types = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;

    if (!hero_syslog_key(source, key, 400)) {
        return -1;
    }
    LSTATUS said = RegCreateKeyExW(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &made, &disposition);

    if (said != ERROR_SUCCESS) {
        return hero_syslog_fail(hero_syslog_win_kind((DWORD)said), (int64_t)said, NULL);
    }
    if (disposition == REG_OPENED_EXISTING_KEY) {
        (void)RegCloseKey(made);
        return hero_syslog_fail(HERO_SYSLOG_EXISTS, 0, "a source of that name is registered already");
    }
    said = RegSetValueExW(made, L"EventMessageFile", 0, REG_EXPAND_SZ, (const BYTE *)file, (DWORD)sizeof file);
    if (said == ERROR_SUCCESS) {
        said = RegSetValueExW(made, L"TypesSupported", 0, REG_DWORD, (const BYTE *)&types, (DWORD)sizeof types);
    }
    (void)RegCloseKey(made);
    if (said != ERROR_SUCCESS) {
        (void)RegDeleteKeyW(HKEY_LOCAL_MACHINE, key);
        return hero_syslog_fail(hero_syslog_win_kind((DWORD)said), (int64_t)said, NULL);
    }
    return hero_syslog_done();
}

int64_t hero_syslog_event_remove(const char *source) {
    wchar_t key[400];

    if (!hero_syslog_key(source, key, 400)) {
        return -1;
    }
    LSTATUS said = RegDeleteKeyW(HKEY_LOCAL_MACHINE, key);

    if (said != ERROR_SUCCESS) {
        return hero_syslog_fail(hero_syslog_win_kind((DWORD)said), (int64_t)said, NULL);
    }
    return hero_syslog_done();
}

#else

int64_t hero_syslog_event(const char *source, int64_t level, int64_t id, const char *line) {
    (void)source;
    (void)level;
    (void)id;
    (void)line;
    return hero_syslog_fail(HERO_SYSLOG_UNSUPPORTED, 0, "the Event Log is Windows': this system has none");
}

int64_t hero_syslog_event_install(const char *source) {
    (void)source;
    return hero_syslog_fail(HERO_SYSLOG_UNSUPPORTED, 0, "the Event Log is Windows': this system has none");
}

int64_t hero_syslog_event_remove(const char *source) {
    (void)source;
    return hero_syslog_fail(HERO_SYSLOG_UNSUPPORTED, 0, "the Event Log is Windows': this system has none");
}

#endif
