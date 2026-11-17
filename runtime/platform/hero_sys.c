/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_sys.c — the platform layer's questions about the machine and
 * the process, one member of the archive the compiler puts on every program's
 * link line (M-rebel-library; `selfhost/cli/layer.hero` builds, keys and
 * links it). Its header, `hero_sys.h`, says what each function answers; this
 * file says how each platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled. It is also why no feature macro stands above the include:
 * on Linux the member is compiled as the runtime is, `-std=gnu11`, whose
 * default names are enough for everything below but `sched_getaffinity`,
 * which is asked of the kernel by its number instead.
 *
 * NOTHING SURVIVES BETWEEN CALLS BUT THE LAST ERROR, which is per thread: no
 * table, no lock and no allocation (the runtime's allocator is reached only
 * through `parts/alloc.c`, the net's `runtime` suite, rule 1). Every answer
 * is written straight into the caller's buffer, through `hero_sys_put`,
 * which counts every byte of the answer and writes only those that fit.
 *
 * THE GROUPS ARE WIDENED WHERE THEY LAND. POSIX hands `gid_t`s, four bytes
 * each, and the header promises `int64_t`s; with no allocation and a list
 * that may hold 65,536 ids on Linux, the system writes them into the
 * caller's own buffer, which is at least twice the room they take, and they
 * are widened in place from the last to the first, each read and written
 * through `memcpy`, so no id is read as a type it was not written as and
 * none is overwritten before it is read.
 *
 * WINDOWS' TEXT IS ENCODED HERE, NOT BY `WideCharToMultiByte`, which replaces
 * a unit that is half of no pair or refuses the whole text: `hero_sys_wide`
 * writes such a unit as UTF-8's rule gives its number, so nothing is
 * replaced, one bad name hides no other, and the caller's own validation
 * refuses exactly that text (`hero_sys.h`, THE FILL FORM).
 */

#include "hero_sys.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#include <objbase.h>
#include <shlobj.h>
#include <tlhelp32.h>
#include <wchar.h>
/* The libraries the member needs, named by the member, as `hero_net.c` names
 * `ws2_32`: the request travels in the object, and the linker reads it only
 * where it takes this member, so a program that asks nothing of this header
 * links none of them. `bcrypt` holds the random generator, `shell32` the
 * known folders, `ole32` the call that gives a known folder's path back, and
 * `uuid` the folders' ids. MSVC's word; a MinGW target links them by `-l`. */
#if defined(_MSC_VER)
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")
/* `GetUserNameW`, as `parts/replace.c` names it for `GetFileSecurityW`. */
#pragma comment(lib, "advapi32.lib")
#endif
#else
#include <errno.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/random.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <unistd.h>
/* The environment's list: a global of the C library, which a program
 * declares itself (POSIX); on Darwin a library may not name it, and
 * `_NSGetEnviron()` answers where it is. */
#if defined(__APPLE__)
#include <crt_externs.h>
#else
extern char **environ;
#endif
#if defined(__linux__)
#include <sys/syscall.h>
#endif
/* The machine's memory: Darwin's counters of pages and its `sysctl`, and
 * Linux's `/proc/meminfo` with `sysinfo` behind it. */
#if defined(__APPLE__)
#include <mach/mach.h>
#include <sys/sysctl.h>
#endif
#if defined(__linux__)
#include <fcntl.h>
#include <sys/sysinfo.h>
#endif
#endif

/* The room a host name is read into: POSIX promises at most 255 bytes, and a
 * DNS name has at most 255. */
#define HERO_SYS_NAME_ROOM 1024

static _Thread_local int64_t hero_sys_kind;
static _Thread_local int64_t hero_sys_os;

static void hero_sys_fine(void) {
    hero_sys_kind = HERO_SYS_OK;
    hero_sys_os = 0;
}

static int64_t hero_sys_fail(int64_t kind, int64_t os) {
    hero_sys_kind = kind;
    hero_sys_os = os;
    return -1;
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_sys_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_ACCESS_DENIED:
        return HERO_SYS_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
        return HERO_SYS_LIMIT;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return HERO_SYS_NOT_FOUND;
    case ERROR_NOT_SUPPORTED:
    case ERROR_CALL_NOT_IMPLEMENTED:
        return HERO_SYS_UNSUPPORTED;
#else
    case EACCES:
    case EPERM:
        return HERO_SYS_DENIED;
    case ENOMEM:
    case EMFILE:
    case ENFILE:
    case EAGAIN:
        return HERO_SYS_LIMIT;
    case ENOENT:
        return HERO_SYS_NOT_FOUND;
    case ENOSYS:
    case ENOTSUP:
    case ENOTTY:
        return HERO_SYS_UNSUPPORTED;
#endif
    default:
        return HERO_SYS_OTHER;
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
} hero_sys_fill;

static hero_sys_fill hero_sys_into(void *buffer, uint64_t count) {
    hero_sys_fill f;

    f.room = buffer;
    f.count = count;
    f.used = 0;
    f.over = false;
    return f;
}

/* Whether a buffer and its count can be filled: a count has a buffer behind
 * it. */
static bool hero_sys_room(const void *buffer, uint64_t count) {
    return buffer != NULL || count == 0;
}

/* `n` more bytes of the answer: written where all of them fit behind the ones
 * before, counted either way. Once one piece did not fit, `used` is past
 * `count` and no later piece is written, so nothing lands past the buffer. */
static void hero_sys_put(hero_sys_fill *f, const void *bytes, size_t n) {
    if (n > 0 && f->used <= f->count && (uint64_t)n <= f->count - f->used) {
        memcpy(f->room + f->used, bytes, n);
    }
    if (__builtin_add_overflow(f->used, (uint64_t)n, &f->used)) {
        f->over = true;
    }
}

/* What a fill form answers: the bytes of the whole answer. */
static int64_t hero_sys_filled(const hero_sys_fill *f) {
    if (f->over || f->used > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    hero_sys_fine();
    return (int64_t)f->used;
}

int64_t hero_sys_last_error(void) {
    return hero_sys_kind;
}

int64_t hero_sys_last_os_error(void) {
    return hero_sys_os;
}

/* One question for both arms: the target clang compiles the member for. Its
 * GNU name and its MSVC name, since a Windows target may define either. */
int64_t hero_sys_architecture(void) {
    hero_sys_fine();
#if defined(__x86_64__) || defined(_M_X64)
    return HERO_SYS_ARCH_X86_64;
#elif defined(__aarch64__) || defined(_M_ARM64)
    return HERO_SYS_ARCH_ARM64;
#else
    return HERO_SYS_ARCH_OTHER;
#endif
}

/* `seconds` and the `parts` of a second below it, each `scale` nanoseconds,
 * as nanoseconds into `*out`; false where the sum does not fit or either is
 * below 0. */
static bool hero_sys_nanoseconds(int64_t seconds, int64_t parts, int64_t scale, int64_t *out) {
    int64_t whole = 0;
    int64_t below = 0;

    if (seconds < 0 || parts < 0) {
        return false;
    }
    if (__builtin_mul_overflow(seconds, (int64_t)1000000000, &whole) || __builtin_mul_overflow(parts, scale, &below) || __builtin_add_overflow(whole, below, out)) {
        return false;
    }
    return true;
}

#if defined(_WIN32)

/* The most one call of the generator is asked for, which its `ULONG` count
 * can say. */
#define HERO_SYS_RANDOM_CHUNK ((uint64_t)1 << 30)

/* `units` UTF-16 units as UTF-8: a pair as its one character, and a unit that
 * is half of no pair as the three bytes its own number takes. */
static void hero_sys_wide(hero_sys_fill *f, const WCHAR *wide, size_t units) {
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
        hero_sys_put(f, b, n);
    }
}

int64_t hero_sys_host_name(void *buffer, uint64_t count) {
    WCHAR name[HERO_SYS_NAME_ROOM];
    DWORD units = HERO_SYS_NAME_ROOM;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (!GetComputerNameExW(ComputerNameDnsHostname, name, &units)) {
        DWORD code = GetLastError();

        return hero_sys_fail(code == ERROR_MORE_DATA ? HERO_SYS_LIMIT : hero_sys_kind_of(code), code);
    }
    hero_sys_wide(&f, name, units);
    return hero_sys_filled(&f);
}

/* Windows keeps no call for it: the list of running processes holds, for
 * each, the id of the one that made it. */
int64_t hero_sys_parent_pid(void) {
    DWORD self = GetCurrentProcessId();
    HANDLE all = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W entry;

    if (all == INVALID_HANDLE_VALUE) {
        DWORD code = GetLastError();

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    memset(&entry, 0, sizeof entry);
    entry.dwSize = sizeof entry;
    if (Process32FirstW(all, &entry)) {
        do {
            if (entry.th32ProcessID == self) {
                DWORD parent = entry.th32ParentProcessID;

                (void)CloseHandle(all);
                hero_sys_fine();
                return (int64_t)parent;
            }
        } while (Process32NextW(all, &entry));
    }
    (void)CloseHandle(all);
    return hero_sys_fail(HERO_SYS_NOT_FOUND, 0);
}

int64_t hero_sys_user_id(void) {
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_effective_user_id(void) {
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_group_id(void) {
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_effective_group_id(void) {
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_groups(int64_t *ids, uint64_t count) {
    (void)ids;
    (void)count;
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_page_size(void) {
    SYSTEM_INFO info;

    memset(&info, 0, sizeof info);
    GetSystemInfo(&info);
    if (info.dwPageSize == 0) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    hero_sys_fine();
    return (int64_t)info.dwPageSize;
}

int64_t hero_sys_processor_count(void) {
    DWORD_PTR allowed = 0;
    DWORD_PTR machine = 0;
    DWORD active = 0;

    /* The mask is 0 where the process spans more than one group of
     * processors, which then are all its own. */
    if (GetProcessAffinityMask(GetCurrentProcess(), &allowed, &machine) && allowed != 0) {
        hero_sys_fine();
        return (int64_t)__builtin_popcountll((unsigned long long)allowed);
    }
    active = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
    if (active == 0) {
        DWORD code = GetLastError();

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    hero_sys_fine();
    return (int64_t)active;
}

/* A known folder's path. The system allocates it and `CoTaskMemFree` gives it
 * back, whether the call answered one or not (SHGetKnownFolderPath's page). */
static int64_t hero_sys_known(void *buffer, uint64_t count, const KNOWNFOLDERID *which) {
    PWSTR wide = NULL;
    HRESULT answer = S_OK;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    answer = SHGetKnownFolderPath(which, 0, NULL, &wide);
    if (FAILED(answer) || wide == NULL) {
        int64_t kind = HERO_SYS_OTHER;

        if (FAILED(answer) && HRESULT_FACILITY(answer) == FACILITY_WIN32) {
            kind = hero_sys_kind_of((int64_t)HRESULT_CODE(answer));
        }
        CoTaskMemFree(wide);
        return hero_sys_fail(kind, (int64_t)answer);
    }
    hero_sys_wide(&f, wide, wcslen(wide));
    CoTaskMemFree(wide);
    return hero_sys_filled(&f);
}

int64_t hero_sys_home_folder(void *buffer, uint64_t count) {
    return hero_sys_known(buffer, count, &FOLDERID_Profile);
}

int64_t hero_sys_cache_folder(void *buffer, uint64_t count) {
    return hero_sys_known(buffer, count, &FOLDERID_LocalAppData);
}

int64_t hero_sys_settings_folder(void *buffer, uint64_t count) {
    return hero_sys_known(buffer, count, &FOLDERID_RoamingAppData);
}

/* The block is a copy the system makes for this call, each variable ended by
 * a zero unit and the block by one more. */
int64_t hero_sys_environment(void *buffer, uint64_t count) {
    LPWCH block = NULL;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    block = GetEnvironmentStringsW();
    if (block == NULL) {
        DWORD code = GetLastError();

        return hero_sys_fail(code == 0 ? HERO_SYS_LIMIT : hero_sys_kind_of(code), code);
    }
    for (const WCHAR *e = block; *e != L'\0'; e += wcslen(e) + 1) {
        if (e[0] == L'=') {
            continue;
        }
        hero_sys_wide(&f, e, wcslen(e));
        hero_sys_put(&f, "", 1);
    }
    (void)FreeEnvironmentStringsW(block);
    return hero_sys_filled(&f);
}

int64_t hero_sys_random(void *buffer, uint64_t count) {
    unsigned char *at = buffer;
    uint64_t left = count;

    if (!hero_sys_room(buffer, count) || count > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    while (left > 0) {
        ULONG chunk = left < HERO_SYS_RANDOM_CHUNK ? (ULONG)left : (ULONG)HERO_SYS_RANDOM_CHUNK;
        NTSTATUS status = BCryptGenRandom(NULL, at, chunk, BCRYPT_USE_SYSTEM_PREFERRED_RNG);

        if (status < 0) {
            return hero_sys_fail(HERO_SYS_OTHER, (int64_t)status);
        }
        at += chunk;
        left -= chunk;
    }
    hero_sys_fine();
    return (int64_t)count;
}

int64_t hero_sys_terminal_size(int64_t *columns, int64_t *rows) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE out = NULL;
    int64_t wide = 0;
    int64_t tall = 0;

    if (columns == NULL || rows == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == NULL || out == INVALID_HANDLE_VALUE) {
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
    }
    memset(&info, 0, sizeof info);
    if (!GetConsoleScreenBufferInfo(out, &info)) {
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, (int64_t)GetLastError());
    }
    wide = (int64_t)info.srWindow.Right - (int64_t)info.srWindow.Left + 1;
    tall = (int64_t)info.srWindow.Bottom - (int64_t)info.srWindow.Top + 1;
    if (wide <= 0 || tall <= 0) {
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
    }
    *columns = wide;
    *rows = tall;
    hero_sys_fine();
    return 0;
}

int64_t hero_sys_system(void) {
    hero_sys_fine();
    return HERO_SYS_SYSTEM_WINDOWS;
}

/* `value` in decimal: its digits, no sign and no zero before them. */
static void hero_sys_decimal(hero_sys_fill *f, uint64_t value) {
    char digits[20];
    size_t n = 0;

    do {
        digits[sizeof digits - 1 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    hero_sys_put(f, digits + sizeof digits - n, n);
}

/* `RtlGetVersion`'s shape, from its page: it fills the record whose size the
 * caller set and answers an `NTSTATUS`, below 0 where it failed. */
typedef LONG(WINAPI *hero_sys_rtl_version)(OSVERSIONINFOW *);

int64_t hero_sys_system_version(void *buffer, uint64_t count) {
    HMODULE ntdll = NULL;
    FARPROC found = NULL;
    hero_sys_rtl_version version = NULL;
    OSVERSIONINFOW info;
    LONG status = 0;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    /* `ntdll` is in every process before its first instruction, so it is
     * found rather than loaded, and nothing is given back. */
    ntdll = GetModuleHandleW(L"ntdll.dll");
    if (ntdll == NULL) {
        DWORD code = GetLastError();

        return hero_sys_fail(HERO_SYS_UNSUPPORTED, code);
    }
    found = GetProcAddress(ntdll, "RtlGetVersion");
    if (found == NULL) {
        DWORD code = GetLastError();

        return hero_sys_fail(HERO_SYS_UNSUPPORTED, code);
    }
    /* Through the one function type C lets any other become and come back
     * from, so no cast joins two unrelated shapes. */
    version = (hero_sys_rtl_version)(void (*)(void))found;
    memset(&info, 0, sizeof info);
    info.dwOSVersionInfoSize = sizeof info;
    status = version(&info);
    if (status < 0) {
        return hero_sys_fail(HERO_SYS_OTHER, (int64_t)status);
    }
    hero_sys_decimal(&f, info.dwMajorVersion);
    hero_sys_put(&f, ".", 1);
    hero_sys_decimal(&f, info.dwMinorVersion);
    hero_sys_put(&f, ".", 1);
    hero_sys_decimal(&f, info.dwBuildNumber);
    return hero_sys_filled(&f);
}

int64_t hero_sys_user_name(void *buffer, uint64_t count) {
    WCHAR name[HERO_SYS_NAME_ROOM];
    DWORD units = HERO_SYS_NAME_ROOM;
    size_t length = 0;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (!GetUserNameW(name, &units)) {
        DWORD code = GetLastError();

        return hero_sys_fail(code == ERROR_INSUFFICIENT_BUFFER ? HERO_SYS_LIMIT : hero_sys_kind_of(code), code);
    }
    /* `units` counts the zero unit that ends the name; the name is read up
     * to that unit and never past the room. */
    while (length < (size_t)units && length < HERO_SYS_NAME_ROOM && name[length] != L'\0') {
        length += 1;
    }
    if (length == 0) {
        return hero_sys_fail(HERO_SYS_NOT_FOUND, 0);
    }
    hero_sys_wide(&f, name, length);
    return hero_sys_filled(&f);
}

/* A `FILETIME` span, 100-nanosecond units in two halves, as nanoseconds;
 * false where it does not fit. */
static bool hero_sys_filetime(const FILETIME *span, int64_t *out) {
    uint64_t units = ((uint64_t)span->dwHighDateTime << 32) | (uint64_t)span->dwLowDateTime;

    if (units > (uint64_t)INT64_MAX) {
        return false;
    }
    return hero_sys_nanoseconds((int64_t)(units / 10000000u), (int64_t)(units % 10000000u), 100, out);
}

int64_t hero_sys_process_times(int64_t *user_ns, int64_t *system_ns) {
    FILETIME created;
    FILETIME ended;
    FILETIME kernel;
    FILETIME user;
    int64_t in_user = 0;
    int64_t in_system = 0;

    if (user_ns == NULL || system_ns == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (!GetProcessTimes(GetCurrentProcess(), &created, &ended, &kernel, &user)) {
        DWORD code = GetLastError();

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    if (!hero_sys_filetime(&user, &in_user) || !hero_sys_filetime(&kernel, &in_system)) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    *user_ns = in_user;
    *system_ns = in_system;
    hero_sys_fine();
    return 0;
}

/* Windows keeps no average of the processes waiting to run. */
int64_t hero_sys_load_average(int64_t *one, int64_t *five, int64_t *fifteen) {
    if (one == NULL || five == NULL || fifteen == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

/* `ullAvailPhys` is the memory of the standby, free and zeroed lists, which
 * Windows hands a program without writing anything out (the reference of
 * `MEMORYSTATUSEX`). */
int64_t hero_sys_memory(int64_t *total, int64_t *available) {
    MEMORYSTATUSEX status;
    DWORDLONG free_now = 0;

    if (total == NULL || available == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    memset(&status, 0, sizeof status);
    status.dwLength = (DWORD)sizeof status;
    if (!GlobalMemoryStatusEx(&status)) {
        DWORD code = GetLastError();

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    if (status.ullTotalPhys == 0 || status.ullTotalPhys > (DWORDLONG)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    free_now = status.ullAvailPhys < status.ullTotalPhys ? status.ullAvailPhys : status.ullTotalPhys;
    *total = (int64_t)status.ullTotalPhys;
    *available = (int64_t)free_now;
    hero_sys_fine();
    return 0;
}

#else

/* The most `getentropy` hands in one call (its page: `EIO` past 256). */
#define HERO_SYS_ENTROPY_CHUNK 256u
/* The room the system's record of a user is read into. */
#define HERO_SYS_RECORD_ROOM 4096
/* How often the groups are asked again where their list grew between the
 * count and the reading. */
#define HERO_SYS_GROUP_TRIES 4
/* The words of the set of processors Linux is asked for, 8,192 processors on
 * a machine of 64 bits. */
#define HERO_SYS_CPU_WORDS 128

_Static_assert(sizeof(gid_t) <= sizeof(int64_t) && _Alignof(gid_t) <= _Alignof(int64_t), "the groups are widened in the caller's own buffer");

static void hero_sys_text(hero_sys_fill *f, const char *text) {
    hero_sys_put(f, text, strlen(text));
}

int64_t hero_sys_host_name(void *buffer, uint64_t count) {
    char name[HERO_SYS_NAME_ROOM + 1];
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    /* A name cut short need not end with a zero byte (POSIX): the room is
     * zeroed, and its last byte is never the system's. */
    memset(name, 0, sizeof name);
    if (gethostname(name, HERO_SYS_NAME_ROOM) != 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    hero_sys_text(&f, name);
    return hero_sys_filled(&f);
}

int64_t hero_sys_parent_pid(void) {
    hero_sys_fine();
    return (int64_t)getppid();
}

int64_t hero_sys_user_id(void) {
    hero_sys_fine();
    return (int64_t)getuid();
}

int64_t hero_sys_effective_user_id(void) {
    hero_sys_fine();
    return (int64_t)geteuid();
}

int64_t hero_sys_group_id(void) {
    hero_sys_fine();
    return (int64_t)getgid();
}

int64_t hero_sys_effective_group_id(void) {
    hero_sys_fine();
    return (int64_t)getegid();
}

int64_t hero_sys_groups(int64_t *ids, uint64_t count) {
    if (ids == NULL && count > 0) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    for (int attempt = 0; attempt < HERO_SYS_GROUP_TRIES; attempt += 1) {
        int have = getgroups(0, NULL);
        int got = 0;

        if (have < 0) {
            int code = errno;

            return hero_sys_fail(hero_sys_kind_of(code), code);
        }
        if (have == 0 || (uint64_t)have > count) {
            hero_sys_fine();
            return (int64_t)have;
        }
        got = getgroups(have, (gid_t *)(void *)ids);
        if (got < 0) {
            int code = errno;

            /* The list grew between the two calls: asked again. */
            if (code == EINVAL) {
                continue;
            }
            return hero_sys_fail(hero_sys_kind_of(code), code);
        }
        for (int i = got - 1; i >= 0; i -= 1) {
            gid_t id;
            int64_t widened = 0;

            memcpy(&id, (const unsigned char *)ids + (size_t)i * sizeof id, sizeof id);
            widened = (int64_t)id;
            memcpy((unsigned char *)ids + (size_t)i * sizeof widened, &widened, sizeof widened);
        }
        hero_sys_fine();
        return (int64_t)got;
    }
    return hero_sys_fail(HERO_SYS_OTHER, EINVAL);
}

int64_t hero_sys_page_size(void) {
    long page = 0;

    errno = 0;
    page = sysconf(_SC_PAGESIZE);
    if (page <= 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    hero_sys_fine();
    return (int64_t)page;
}

int64_t hero_sys_processor_count(void) {
    long online = 0;

#if defined(__linux__)
    /* The kernel's own call, by its number: the C library declares its
     * wrapper only under `_GNU_SOURCE`, which this member cannot define above
     * its own header. It answers how many bytes of the set it wrote
     * (sched_setaffinity(2), "C library/kernel differences"), and refuses a
     * machine of more processors than the set holds, which is then asked how
     * many are online. */
    unsigned long allowed_set[HERO_SYS_CPU_WORDS];
    long wrote = 0;

    memset(allowed_set, 0, sizeof allowed_set);
    wrote = syscall(SYS_sched_getaffinity, 0, sizeof allowed_set, allowed_set);
    if (wrote > 0 && (size_t)wrote <= sizeof allowed_set) {
        int64_t allowed = 0;

        for (size_t i = 0; i < (size_t)wrote / sizeof allowed_set[0]; i += 1) {
            allowed += (int64_t)__builtin_popcountl(allowed_set[i]);
        }
        if (allowed > 0) {
            hero_sys_fine();
            return allowed;
        }
    }
#endif
    errno = 0;
    online = sysconf(_SC_NPROCESSORS_ONLN);
    if (online <= 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    hero_sys_fine();
    return (int64_t)online;
}

/* The user's home folder: `$HOME`, or where it is unset or empty the folder
 * the system's record of the user names, read into `room`. NULL with the
 * kind set where neither names one. */
static const char *hero_sys_home(char *room, size_t size) {
    const char *home = getenv("HOME");
    struct passwd record;
    struct passwd *found = NULL;
    int refused = 0;

    if (home != NULL && home[0] != '\0') {
        return home;
    }
    refused = getpwuid_r(getuid(), &record, room, size, &found);
    if (refused != 0) {
        (void)hero_sys_fail(refused == ERANGE ? HERO_SYS_LIMIT : hero_sys_kind_of(refused), refused);
        return NULL;
    }
    if (found == NULL || found->pw_dir == NULL || found->pw_dir[0] == '\0') {
        (void)hero_sys_fail(HERO_SYS_NOT_FOUND, 0);
        return NULL;
    }
    return found->pw_dir;
}

int64_t hero_sys_home_folder(void *buffer, uint64_t count) {
    char room[HERO_SYS_RECORD_ROOM];
    const char *home = NULL;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    home = hero_sys_home(room, sizeof room);
    if (home == NULL) {
        return -1;
    }
    hero_sys_text(&f, home);
    return hero_sys_filled(&f);
}

/* The folder `variable` names where it is set to an absolute path (the XDG
 * rule: one that is not is ignored), else `below` under the home folder,
 * whose own last slashes are left out; a NULL `variable` asks none. */
static int64_t hero_sys_below_home(void *buffer, uint64_t count, const char *variable, const char *below) {
    char room[HERO_SYS_RECORD_ROOM];
    const char *named = variable != NULL ? getenv(variable) : NULL;
    const char *home = NULL;
    size_t length = 0;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (named != NULL && named[0] == '/') {
        hero_sys_text(&f, named);
        return hero_sys_filled(&f);
    }
    home = hero_sys_home(room, sizeof room);
    if (home == NULL) {
        return -1;
    }
    length = strlen(home);
    while (length > 0 && home[length - 1] == '/') {
        length -= 1;
    }
    hero_sys_put(&f, home, length);
    hero_sys_text(&f, below);
    return hero_sys_filled(&f);
}

int64_t hero_sys_cache_folder(void *buffer, uint64_t count) {
#if defined(__APPLE__)
    return hero_sys_below_home(buffer, count, NULL, "/Library/Caches");
#else
    return hero_sys_below_home(buffer, count, "XDG_CACHE_HOME", "/.cache");
#endif
}

int64_t hero_sys_settings_folder(void *buffer, uint64_t count) {
#if defined(__APPLE__)
    return hero_sys_below_home(buffer, count, NULL, "/Library/Application Support");
#else
    return hero_sys_below_home(buffer, count, "XDG_CONFIG_HOME", "/.config");
#endif
}

int64_t hero_sys_environment(void *buffer, uint64_t count) {
#if defined(__APPLE__)
    char ***held = _NSGetEnviron();
    char **list = held != NULL ? *held : NULL;
#else
    char **list = environ;
#endif
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (list != NULL) {
        for (char **entry = list; *entry != NULL; entry += 1) {
            size_t length = strlen(*entry);

            if (length > 0) {
                hero_sys_put(&f, *entry, length + 1);
            }
        }
    }
    return hero_sys_filled(&f);
}

int64_t hero_sys_random(void *buffer, uint64_t count) {
    unsigned char *at = buffer;
    uint64_t left = count;

    if (!hero_sys_room(buffer, count) || count > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    while (left > 0) {
        size_t chunk = left < HERO_SYS_ENTROPY_CHUNK ? (size_t)left : HERO_SYS_ENTROPY_CHUNK;

        if (getentropy(at, chunk) != 0) {
            int code = errno;

            if (code == EINTR) {
                continue;
            }
            return hero_sys_fail(hero_sys_kind_of(code), code);
        }
        at += chunk;
        left -= chunk;
    }
    hero_sys_fine();
    return (int64_t)count;
}

int64_t hero_sys_terminal_size(int64_t *columns, int64_t *rows) {
    struct winsize size;

    if (columns == NULL || rows == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    memset(&size, 0, sizeof size);
    /* Whatever the system refuses it with, an output that does not answer
     * this is no terminal that tells a size. */
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) != 0) {
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, (int64_t)errno);
    }
    if (size.ws_col == 0 || size.ws_row == 0) {
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
    }
    *columns = (int64_t)size.ws_col;
    *rows = (int64_t)size.ws_row;
    hero_sys_fine();
    return 0;
}

int64_t hero_sys_system(void) {
    hero_sys_fine();
#if defined(__APPLE__)
    return HERO_SYS_SYSTEM_DARWIN;
#elif defined(__linux__)
    return HERO_SYS_SYSTEM_LINUX;
#else
    return HERO_SYS_SYSTEM_OTHER;
#endif
}

int64_t hero_sys_system_version(void *buffer, uint64_t count) {
    struct utsname names;
    const char *end = NULL;
    size_t length = 0;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    memset(&names, 0, sizeof names);
    if (uname(&names) < 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    /* POSIX ends the field with a zero byte; it is read no further than the
     * field either way. */
    end = memchr(names.release, '\0', sizeof names.release);
    length = end != NULL ? (size_t)(end - names.release) : sizeof names.release;
    if (length == 0) {
        return hero_sys_fail(HERO_SYS_NOT_FOUND, 0);
    }
    hero_sys_put(&f, names.release, length);
    return hero_sys_filled(&f);
}

int64_t hero_sys_user_name(void *buffer, uint64_t count) {
    char room[HERO_SYS_RECORD_ROOM];
    struct passwd record;
    struct passwd *found = NULL;
    const char *name = NULL;
    int refused = 0;
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    refused = getpwuid_r(geteuid(), &record, room, sizeof room, &found);
    if (refused == 0 && found != NULL && found->pw_name != NULL && found->pw_name[0] != '\0') {
        name = found->pw_name;
    } else {
        name = getenv("USER");
        if (name == NULL || name[0] == '\0') {
            if (refused != 0) {
                return hero_sys_fail(refused == ERANGE ? HERO_SYS_LIMIT : hero_sys_kind_of(refused), refused);
            }
            return hero_sys_fail(HERO_SYS_NOT_FOUND, 0);
        }
    }
    hero_sys_text(&f, name);
    return hero_sys_filled(&f);
}

int64_t hero_sys_process_times(int64_t *user_ns, int64_t *system_ns) {
    struct rusage usage;
    int64_t in_user = 0;
    int64_t in_system = 0;

    if (user_ns == NULL || system_ns == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    memset(&usage, 0, sizeof usage);
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    if (!hero_sys_nanoseconds((int64_t)usage.ru_utime.tv_sec, (int64_t)usage.ru_utime.tv_usec, 1000, &in_user) || !hero_sys_nanoseconds((int64_t)usage.ru_stime.tv_sec, (int64_t)usage.ru_stime.tv_usec, 1000, &in_system)) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    *user_ns = in_user;
    *system_ns = in_system;
    hero_sys_fine();
    return 0;
}

/* The largest average that, multiplied by 1000, still fits: no machine has
 * so many processes waiting, so one above it is a reading gone wrong. */
#define HERO_SYS_LOAD_MOST 9.0e15

int64_t hero_sys_load_average(int64_t *one, int64_t *five, int64_t *fifteen) {
    double loads[3] = {0.0, 0.0, 0.0};
    int64_t scaled[3] = {0, 0, 0};
    int got = 0;

    if (one == NULL || five == NULL || fifteen == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    errno = 0;
    got = getloadavg(loads, 3);
    /* A system that cannot tell them, a Linux with no `/proc`, refuses with
     * no number or with `ENOENT`: it has no such thing to give. */
    if (got < 0) {
        int code = errno;

        return hero_sys_fail(code == 0 || code == ENOENT ? HERO_SYS_UNSUPPORTED : hero_sys_kind_of(code), code);
    }
    if (got < 3) {
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
    }
    for (int i = 0; i < 3; i += 1) {
        /* A NaN fails the first comparison. */
        if (!(loads[i] >= 0.0 && loads[i] < HERO_SYS_LOAD_MOST)) {
            return hero_sys_fail(HERO_SYS_OTHER, 0);
        }
        scaled[i] = (int64_t)(loads[i] * 1000.0 + 0.5);
    }
    *one = scaled[0];
    *five = scaled[1];
    *fifteen = scaled[2];
    hero_sys_fine();
    return 0;
}

#if defined(__APPLE__) || defined(__linux__)

/* The two answers of `hero_sys_memory` written, the available never above
 * the total: two readings of counters that move apart are not one. */
static int64_t hero_sys_memory_answer(uint64_t whole, uint64_t free_now, int64_t *total, int64_t *available) {
    if (whole == 0 || whole > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    *total = (int64_t)whole;
    *available = (int64_t)(free_now < whole ? free_now : whole);
    hero_sys_fine();
    return 0;
}

#endif

#if defined(__APPLE__)

int64_t hero_sys_memory(int64_t *total, int64_t *available) {
    uint64_t whole = 0;
    size_t whole_size = sizeof whole;
    vm_statistics64_data_t pages;
    mach_msg_type_number_t words = HOST_VM_INFO64_COUNT;
    vm_size_t page = 0;
    mach_port_t host = MACH_PORT_NULL;
    kern_return_t counted = KERN_SUCCESS;
    kern_return_t paged = KERN_SUCCESS;
    uint64_t free_pages = 0;
    uint64_t free_now = 0;

    if (total == NULL || available == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (sysctlbyname("hw.memsize", &whole, &whole_size, NULL, 0) != 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    if (whole_size != sizeof whole) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    memset(&pages, 0, sizeof pages);
    /* `mach_host_self` adds a reference to the host's port each call, which
     * is given back below whatever the two questions answered. */
    host = mach_host_self();
    counted = host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&pages, &words);
    paged = host_page_size(host, &page);
    (void)mach_port_deallocate(mach_task_self(), host);
    if (counted != KERN_SUCCESS || paged != KERN_SUCCESS || page == 0) {
        return hero_sys_fail(HERO_SYS_OTHER, counted != KERN_SUCCESS ? (int64_t)counted : (int64_t)paged);
    }
    /* The free count holds the speculative pages already (`vm_stat` prints
     * them apart by taking them out of it), so they are not added again. */
    free_pages = (uint64_t)pages.free_count + (uint64_t)pages.inactive_count;
    if (__builtin_mul_overflow(free_pages, (uint64_t)page, &free_now)) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    return hero_sys_memory_answer(whole, free_now, total, available);
}

#elif defined(__linux__)

/* The room `/proc/meminfo` is read into: its two lines asked are its first
 * and third, and the whole file is about 1.5 KB on Linux 6. */
#define HERO_SYS_MEMINFO_ROOM 8192

/* The bytes the line of `text` named `name` gives, its number of KiB times
 * 1024, into `*bytes`; false where no line opens with `name`, its number has
 * no digit or does not fit, or it is not counted in `kB`. */
static bool hero_sys_meminfo_line(const char *text, size_t length, const char *name, uint64_t *bytes) {
    size_t named = strlen(name);
    size_t at = 0;

    while (at < length) {
        size_t end = at;

        while (end < length && text[end] != '\n') {
            end += 1;
        }
        if (end - at > named && memcmp(text + at, name, named) == 0) {
            size_t i = at + named;
            uint64_t kib = 0;
            bool digits = false;

            while (i < end && (text[i] == ' ' || text[i] == '\t')) {
                i += 1;
            }
            while (i < end && text[i] >= '0' && text[i] <= '9') {
                if (__builtin_mul_overflow(kib, (uint64_t)10, &kib) || __builtin_add_overflow(kib, (uint64_t)(text[i] - '0'), &kib)) {
                    return false;
                }
                digits = true;
                i += 1;
            }
            while (i < end && text[i] == ' ') {
                i += 1;
            }
            if (!digits || end - i < 2 || text[i] != 'k' || text[i + 1] != 'B') {
                return false;
            }
            return !__builtin_mul_overflow(kib, (uint64_t)1024, bytes);
        }
        at = end + 1;
    }
    return false;
}

/* `/proc/meminfo` read whole into `room`, its length into `*length`; false
 * where it cannot be opened or read, a Linux with no `/proc`. */
static bool hero_sys_meminfo(char *room, size_t size, size_t *length) {
    int file = open("/proc/meminfo", O_RDONLY | O_CLOEXEC);
    size_t got = 0;

    if (file < 0) {
        return false;
    }
    while (got < size) {
        ssize_t read_now = read(file, room + got, size - got);

        if (read_now < 0 && errno == EINTR) {
            continue;
        }
        if (read_now < 0) {
            (void)close(file);
            return false;
        }
        if (read_now == 0) {
            break;
        }
        got += (size_t)read_now;
    }
    (void)close(file);
    *length = got;
    return true;
}

int64_t hero_sys_memory(int64_t *total, int64_t *available) {
    char room[HERO_SYS_MEMINFO_ROOM];
    size_t length = 0;
    uint64_t whole = 0;
    uint64_t free_now = 0;
    struct sysinfo info;
    uint64_t unit = 0;
    uint64_t loose = 0;

    if (total == NULL || available == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (hero_sys_meminfo(room, sizeof room, &length) && hero_sys_meminfo_line(room, length, "MemTotal:", &whole) && hero_sys_meminfo_line(room, length, "MemAvailable:", &free_now)) {
        return hero_sys_memory_answer(whole, free_now, total, available);
    }
    memset(&info, 0, sizeof info);
    if (sysinfo(&info) != 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    /* `mem_unit` is 0 on kernels before 2.3.23, whose sizes are bytes. */
    unit = info.mem_unit == 0 ? 1 : (uint64_t)info.mem_unit;
    if (__builtin_mul_overflow((uint64_t)info.totalram, unit, &whole) || __builtin_add_overflow((uint64_t)info.freeram, (uint64_t)info.bufferram, &loose) || __builtin_mul_overflow(loose, unit, &free_now)) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    return hero_sys_memory_answer(whole, free_now, total, available);
}

#else

/* A POSIX system neither Darwin nor Linux: no reading of its memory is
 * written for it, and none is invented. */
int64_t hero_sys_memory(int64_t *total, int64_t *available) {
    if (total == NULL || available == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

#endif

#endif
