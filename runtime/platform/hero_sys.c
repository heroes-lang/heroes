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

/* A file of `/proc` read whole and given back before the call returns, through
 * `parts/alloc.c`, the single allocation point (design.md §4.20). */
#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
/* Before `windows.h`, which otherwise reads the older `winsock.h`, whose names
 * `iphlpapi.h`'s own headers define again; `ws2tcpip.h` reads `ws2ipdef.h`,
 * without which `iphlpapi.h` declares no `MIB_IF_ROW2` (round 5, the order
 * `hero_net.c` reads them in). */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <bcrypt.h>
#include <objbase.h>
#include <shlobj.h>
#include <tlhelp32.h>
#include <wchar.h>
/* A process's memory (`K32GetProcessMemoryInfo`, in `kernel32` since
 * Windows 7) and the interfaces' counters (`GetIfTable2`), round 5. */
#include <psapi.h>
#include <iphlpapi.h>
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
/* `GetIfTable2` and `FreeMibTable`, for `hero_sys_network_counters` alone
 * (round 5). */
#pragma comment(lib, "iphlpapi.lib")
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
#include <signal.h>
#include <spawn.h>
#include <sys/utsname.h>
#include <sys/wait.h>
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
/* What a monitor reads (round 5): on Darwin a process's task by its id, the
 * clock that counts its time, and the interfaces' list and their MIB. */
#if defined(__APPLE__)
#include <libproc.h>
#include <mach/mach_time.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_mib.h>
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
    case ESRCH:
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

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)

/* `value` times `numerator` over `denominator`, rounded down, into `*out`,
 * computed so that no step passes 64 bits where the answer does not: a count
 * of ticks or of a clock's units turned into micro- or nanoseconds. False
 * where the answer does not fit an `int64_t` or the denominator is 0. */
static bool hero_sys_scaled(uint64_t value, uint64_t numerator, uint64_t denominator, int64_t *out) {
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

/* One counter of `hero_sys_network_counters`: its decimal digits, no sign and
 * no zero before them, then one zero byte. */
static void hero_sys_counter(hero_sys_fill *f, uint64_t value) {
    char digits[21];
    size_t n = 0;

    do {
        digits[sizeof digits - 2 - n] = (char)('0' + (int)(value % 10u));
        value /= 10u;
        n += 1;
    } while (value > 0);
    digits[sizeof digits - 1] = '\0';
    hero_sys_put(f, digits + sizeof digits - 1 - n, n + 1);
}

/* The zero byte that ends a name, or a counter the platform does not keep. */
static void hero_sys_zero(hero_sys_fill *f) {
    hero_sys_put(f, "", 1);
}

#endif

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

int64_t hero_sys_data_folder(void *buffer, uint64_t count) {
    return hero_sys_known(buffer, count, &FOLDERID_RoamingAppData);
}

int64_t hero_sys_state_folder(void *buffer, uint64_t count) {
    return hero_sys_known(buffer, count, &FOLDERID_LocalAppData);
}

int64_t hero_sys_runtime_folder(void *buffer, uint64_t count) {
    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

/* ShellExecuteW answers a number above 32 where it did what it was asked, and
 * one of its own at or below it where not (its page). COM is begun for the
 * call where the thread had not begun it, as the page asks, since a handler
 * may be a COM object, and ended after only where this call began it. */
int64_t hero_sys_open(const char *target) {
    WCHAR *wide = NULL;
    int units = 0;
    HRESULT began = S_OK;
    HINSTANCE answer = NULL;
    INT_PTR code = 0;

    if (target == NULL || target[0] == '\0') {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, target, -1, NULL, 0);
    if (units <= 0) {
        return hero_sys_fail(HERO_SYS_INVALID, (int64_t)GetLastError());
    }
    wide = (WCHAR *)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)units * sizeof(WCHAR));
    if (wide == NULL) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, target, -1, wide, units) != units) {
        DWORD why = GetLastError();

        HeapFree(GetProcessHeap(), 0, wide);
        return hero_sys_fail(HERO_SYS_INVALID, (int64_t)why);
    }
    began = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    answer = ShellExecuteW(NULL, L"open", wide, NULL, NULL, SW_SHOWNORMAL);
    code = (INT_PTR)answer;
    if (began == S_OK || began == S_FALSE) {
        CoUninitialize();
    }
    HeapFree(GetProcessHeap(), 0, wide);
    if (code > 32) {
        hero_sys_fine();
        return 0;
    }
    switch (code) {
    case SE_ERR_FNF:
    case SE_ERR_PNF:
        return hero_sys_fail(HERO_SYS_NOT_FOUND, (int64_t)code);
    case SE_ERR_ACCESSDENIED:
        return hero_sys_fail(HERO_SYS_DENIED, (int64_t)code);
    case 0:
    case SE_ERR_OOM:
        return hero_sys_fail(HERO_SYS_LIMIT, (int64_t)code);
    case SE_ERR_NOASSOC:
    case SE_ERR_ASSOCINCOMPLETE:
        return hero_sys_fail(HERO_SYS_UNSUPPORTED, (int64_t)code);
    default:
        return hero_sys_fail(HERO_SYS_OTHER, (int64_t)code);
    }
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

/* A `FILETIME` span's 100-nanosecond units, its two halves joined. */
static uint64_t hero_sys_units(const FILETIME *span) {
    return ((uint64_t)span->dwHighDateTime << 32) | (uint64_t)span->dwLowDateTime;
}

/* `GetSystemTimes` sums every processor's time; its kernel time holds the
 * idle time (its page), which is taken out of it here. Windows tells no time
 * waiting for a read and none stolen by a hypervisor apart. */
int64_t hero_sys_cpu_times(int64_t *user_us, int64_t *system_us, int64_t *idle_us, int64_t *waiting_us, int64_t *stolen_us) {
    FILETIME idle;
    FILETIME kernel;
    FILETIME user;
    uint64_t in_idle = 0;
    uint64_t in_kernel = 0;
    int64_t idle_now = 0;
    int64_t system_now = 0;
    int64_t user_now = 0;

    if (user_us == NULL || system_us == NULL || idle_us == NULL || waiting_us == NULL || stolen_us == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (!GetSystemTimes(&idle, &kernel, &user)) {
        DWORD code = GetLastError();

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    in_idle = hero_sys_units(&idle);
    in_kernel = hero_sys_units(&kernel);
    if (in_kernel < in_idle) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    if (!hero_sys_scaled(in_idle, 1, 10, &idle_now) || !hero_sys_scaled(in_kernel - in_idle, 1, 10, &system_now) || !hero_sys_scaled(hero_sys_units(&user), 1, 10, &user_now)) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    *user_us = user_now;
    *system_us = system_now;
    *idle_us = idle_now;
    *waiting_us = 0;
    *stolen_us = 0;
    hero_sys_fine();
    return 0;
}

/* `OpenProcess` refuses an id no process has with `ERROR_INVALID_PARAMETER`
 * (its page), which is that id not found. The handle asks for the least
 * access both calls need, which a process of another user still grants where
 * it is not protected; it is closed before any answer. */
int64_t hero_sys_process_usage(int64_t pid, int64_t *resident, int64_t *virtual_bytes, int64_t *user_ns, int64_t *system_ns) {
    HANDLE process = NULL;
    FILETIME created;
    FILETIME ended;
    FILETIME kernel;
    FILETIME user;
    PROCESS_MEMORY_COUNTERS counters;
    int64_t in_user = 0;
    int64_t in_system = 0;
    BOOL timed = FALSE;
    BOOL counted = FALSE;
    DWORD code = 0;

    if (resident == NULL || virtual_bytes == NULL || user_ns == NULL || system_ns == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (pid < 1 || pid > (int64_t)MAXDWORD) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    if (process == NULL) {
        code = GetLastError();
        return hero_sys_fail(code == ERROR_INVALID_PARAMETER ? HERO_SYS_NOT_FOUND : hero_sys_kind_of(code), code);
    }
    memset(&counters, 0, sizeof counters);
    counters.cb = (DWORD)sizeof counters;
    timed = GetProcessTimes(process, &created, &ended, &kernel, &user);
    if (!timed) {
        code = GetLastError();
    } else {
        counted = K32GetProcessMemoryInfo(process, &counters, (DWORD)sizeof counters);
        if (!counted) {
            code = GetLastError();
        }
    }
    (void)CloseHandle(process);
    if (!timed || !counted) {
        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    if (!hero_sys_filetime(&user, &in_user) || !hero_sys_filetime(&kernel, &in_system)) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    if ((uint64_t)counters.WorkingSetSize > (uint64_t)INT64_MAX || (uint64_t)counters.PagefileUsage > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    *resident = (int64_t)counters.WorkingSetSize;
    *virtual_bytes = (int64_t)counters.PagefileUsage;
    *user_ns = in_user;
    *system_ns = in_system;
    hero_sys_fine();
    return 0;
}

/* `GetIfTable2` hands a table it allocated, given back by `FreeMibTable`
 * whatever is written. A filter interface (an NDIS filter over another,
 * which the row's flags name) counts again its interface's packets and is
 * left out. A row's counters are 64 bits; a sum that does not fit is a
 * reading gone wrong. */
int64_t hero_sys_network_counters(void *buffer, uint64_t count) {
    MIB_IF_TABLE2 *table = NULL;
    hero_sys_fill f = hero_sys_into(buffer, count);
    DWORD code = 0;
    bool over = false;

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    code = GetIfTable2(&table);
    if (code != NO_ERROR || table == NULL) {
        return hero_sys_fail(code == NO_ERROR ? HERO_SYS_OTHER : hero_sys_kind_of((int64_t)code), (int64_t)code);
    }
    for (ULONG i = 0; i < table->NumEntries && !over; i += 1) {
        const MIB_IF_ROW2 *row = &table->Table[i];
        uint64_t packets_in = 0;
        uint64_t packets_out = 0;

        if (row->InterfaceAndOperStatusFlags.FilterInterface) {
            continue;
        }
        if (__builtin_add_overflow((uint64_t)row->InUcastPkts, (uint64_t)row->InNUcastPkts, &packets_in) || __builtin_add_overflow((uint64_t)row->OutUcastPkts, (uint64_t)row->OutNUcastPkts, &packets_out)) {
            over = true;
            break;
        }
        hero_sys_wide(&f, row->Alias, wcsnlen(row->Alias, sizeof row->Alias / sizeof row->Alias[0]));
        hero_sys_zero(&f);
        hero_sys_counter(&f, (uint64_t)row->InOctets);
        hero_sys_counter(&f, (uint64_t)row->OutOctets);
        hero_sys_counter(&f, packets_in);
        hero_sys_counter(&f, packets_out);
        hero_sys_counter(&f, (uint64_t)row->InErrors);
        hero_sys_counter(&f, (uint64_t)row->OutErrors);
        hero_sys_counter(&f, (uint64_t)row->InDiscards);
        hero_sys_counter(&f, (uint64_t)row->OutDiscards);
    }
    FreeMibTable(table);
    if (over) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    return hero_sys_filled(&f);
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

int64_t hero_sys_data_folder(void *buffer, uint64_t count) {
#if defined(__APPLE__)
    return hero_sys_below_home(buffer, count, NULL, "/Library/Application Support");
#else
    return hero_sys_below_home(buffer, count, "XDG_DATA_HOME", "/.local/share");
#endif
}

int64_t hero_sys_state_folder(void *buffer, uint64_t count) {
#if defined(__APPLE__)
    return hero_sys_below_home(buffer, count, NULL, "/Library/Application Support");
#else
    return hero_sys_below_home(buffer, count, "XDG_STATE_HOME", "/.local/state");
#endif
}

/* The room Darwin's temporary folder is read into: a path under
 * `/var/folders`, some fifty bytes. */
#define HERO_SYS_TEMP_ROOM 1024

int64_t hero_sys_runtime_folder(void *buffer, uint64_t count) {
    hero_sys_fill f = hero_sys_into(buffer, count);

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
#if defined(__APPLE__)
    {
        char room[HERO_SYS_TEMP_ROOM];
        size_t needed = 0;
        size_t length = 0;

        errno = 0;
        needed = confstr(_CS_DARWIN_USER_TEMP_DIR, room, sizeof room);
        if (needed == 0) {
            int why = errno;

            return hero_sys_fail(why != 0 ? hero_sys_kind_of(why) : HERO_SYS_NOT_FOUND, why);
        }
        if (needed > sizeof room) {
            return hero_sys_fail(HERO_SYS_LIMIT, 0);
        }
        length = strlen(room);
        while (length > 1 && room[length - 1] == '/') {
            length -= 1;
        }
        hero_sys_put(&f, room, length);
    }
#else
    {
        const char *named = getenv("XDG_RUNTIME_DIR");

        if (named == NULL || named[0] != '/') {
            return hero_sys_fail(HERO_SYS_NOT_FOUND, 0);
        }
        hero_sys_text(&f, named);
    }
#endif
    return hero_sys_filled(&f);
}

/* The launcher is started with every signal at its default and none blocked,
 * whatever the program set (a runtime that ignores SIGPIPE, a thread that
 * blocks SIGINT), and waited for, a wait that a signal breaks asked again. */
int64_t hero_sys_open(const char *target) {
#if defined(__APPLE__)
    char launcher[] = "/usr/bin/open";
    char **environment = *_NSGetEnviron();
#else
    char launcher[] = "xdg-open";
    char **environment = environ;
#endif
    char *arguments[3];
    posix_spawnattr_t attributes;
    sigset_t none;
    sigset_t every;
    pid_t child = 0;
    int status = 0;
    int refused = 0;

    if (target == NULL || target[0] == '\0') {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    arguments[0] = launcher;
    arguments[1] = (char *)(uintptr_t)target;
    arguments[2] = NULL;
    refused = posix_spawnattr_init(&attributes);
    if (refused != 0) {
        return hero_sys_fail(hero_sys_kind_of(refused), refused);
    }
    sigemptyset(&none);
    sigfillset(&every);
    sigdelset(&every, SIGKILL);
    sigdelset(&every, SIGSTOP);
    refused = posix_spawnattr_setsigmask(&attributes, &none);
    if (refused == 0) {
        refused = posix_spawnattr_setsigdefault(&attributes, &every);
    }
    if (refused == 0) {
        refused = posix_spawnattr_setflags(&attributes, POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSIGDEF);
    }
    if (refused == 0) {
#if defined(__APPLE__)
        refused = posix_spawn(&child, launcher, NULL, &attributes, arguments, environment);
#else
        refused = posix_spawnp(&child, launcher, NULL, &attributes, arguments, environment);
#endif
    }
    (void)posix_spawnattr_destroy(&attributes);
    if (refused != 0) {
        return hero_sys_fail(refused == ENOENT ? HERO_SYS_NOT_FOUND : hero_sys_kind_of(refused), refused);
    }
    while (waitpid(child, &status, 0) < 0) {
        int why = errno;

        if (why != EINTR) {
            return hero_sys_fail(hero_sys_kind_of(why), why);
        }
    }
    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);

        if (code == 0) {
            hero_sys_fine();
            return 0;
        }
#if !defined(__APPLE__)
        if (code == 2) {
            return hero_sys_fail(HERO_SYS_NOT_FOUND, code);
        }
        if (code == 3) {
            return hero_sys_fail(HERO_SYS_UNSUPPORTED, code);
        }
#endif
        return hero_sys_fail(HERO_SYS_OTHER, code);
    }
    if (WIFSIGNALED(status)) {
        return hero_sys_fail(HERO_SYS_OTHER, 128 + WTERMSIG(status));
    }
    return hero_sys_fail(HERO_SYS_OTHER, status);
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

/* ---- What a monitor reads (round 5): the processors' time, a process by
 * its id, a volume's space and the interfaces' counters. ---- */

#if defined(__APPLE__)

int64_t hero_sys_cpu_times(int64_t *user_us, int64_t *system_us, int64_t *idle_us, int64_t *waiting_us, int64_t *stolen_us) {
    struct clockinfo clock;
    size_t clock_size = sizeof clock;
    natural_t processors = 0;
    processor_info_array_t info = NULL;
    mach_msg_type_number_t words = 0;
    mach_port_t host = MACH_PORT_NULL;
    kern_return_t asked = KERN_SUCCESS;
    uint64_t sums[CPU_STATE_MAX];
    int64_t user_now = 0;
    int64_t system_now = 0;
    int64_t idle_now = 0;
    bool whole = true;

    if (user_us == NULL || system_us == NULL || idle_us == NULL || waiting_us == NULL || stolen_us == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    memset(&clock, 0, sizeof clock);
    if (sysctlbyname("kern.clockrate", &clock, &clock_size, NULL, 0) != 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    if (clock_size != sizeof clock || clock.hz <= 0) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    /* As in `hero_sys_memory`, the reference `mach_host_self` adds is given
     * back whatever the question answered; the array the kernel hands is the
     * caller's to give back too. */
    host = mach_host_self();
    asked = host_processor_info(host, PROCESSOR_CPU_LOAD_INFO, &processors, &info, &words);
    (void)mach_port_deallocate(mach_task_self(), host);
    if (asked != KERN_SUCCESS) {
        return hero_sys_fail(HERO_SYS_OTHER, (int64_t)asked);
    }
    memset(sums, 0, sizeof sums);
    if ((uint64_t)words < (uint64_t)processors * (uint64_t)CPU_STATE_MAX) {
        whole = false;
    }
    for (natural_t p = 0; whole && p < processors; p += 1) {
        for (int state = 0; state < CPU_STATE_MAX; state += 1) {
            /* Each processor's count is 32 bits; the sums of a machine's
             * processors, fewer than 2^32 of them, are not. */
            sums[state] += (uint64_t)(uint32_t)info[(uint64_t)p * CPU_STATE_MAX + (uint64_t)state];
        }
    }
    (void)vm_deallocate(mach_task_self(), (vm_address_t)info, (vm_size_t)words * sizeof(integer_t));
    if (!whole) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    if (!hero_sys_scaled(sums[CPU_STATE_USER] + sums[CPU_STATE_NICE], 1000000, (uint64_t)clock.hz, &user_now) || !hero_sys_scaled(sums[CPU_STATE_SYSTEM], 1000000, (uint64_t)clock.hz, &system_now) || !hero_sys_scaled(sums[CPU_STATE_IDLE], 1000000, (uint64_t)clock.hz, &idle_now)) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    *user_us = user_now;
    *system_us = system_now;
    *idle_us = idle_now;
    *waiting_us = 0;
    *stolen_us = 0;
    hero_sys_fine();
    return 0;
}

/* `proc_pidinfo` answers the bytes it wrote, and fewer than the record's,
 * with `errno` set, where it refused: `ESRCH` for an id no process has (an
 * ended process not yet waited for among them), `EPERM` for a process of
 * another user. Its times are counted in the units of `mach_absolute_time`,
 * which `mach_timebase_info` turns into nanoseconds (125/3 on Apple's arm64
 * processors, 1/1 on Intel's: measured 2026-11-17 against `getrusage`). */
int64_t hero_sys_process_usage(int64_t pid, int64_t *resident, int64_t *virtual_bytes, int64_t *user_ns, int64_t *system_ns) {
    struct proc_taskinfo task;
    mach_timebase_info_data_t base;
    int got = 0;
    int64_t in_user = 0;
    int64_t in_system = 0;

    if (resident == NULL || virtual_bytes == NULL || user_ns == NULL || system_ns == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (pid < 1 || pid > (int64_t)INT32_MAX) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    memset(&task, 0, sizeof task);
    errno = 0;
    got = proc_pidinfo((int)pid, PROC_PIDTASKINFO, 0, &task, (int)sizeof task);
    if (got != (int)sizeof task) {
        int code = errno;

        return hero_sys_fail(code == 0 ? HERO_SYS_OTHER : hero_sys_kind_of(code), code);
    }
    memset(&base, 0, sizeof base);
    if (mach_timebase_info(&base) != KERN_SUCCESS || base.denom == 0) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    if (!hero_sys_scaled(task.pti_total_user, base.numer, base.denom, &in_user) || !hero_sys_scaled(task.pti_total_system, base.numer, base.denom, &in_system)) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    if (task.pti_resident_size > (uint64_t)INT64_MAX || task.pti_virtual_size > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    *resident = (int64_t)task.pti_resident_size;
    *virtual_bytes = (int64_t)task.pti_virtual_size;
    *user_ns = in_user;
    *system_ns = in_system;
    hero_sys_fine();
    return 0;
}

/* The interfaces are listed by `if_nameindex`, and each one's counters asked
 * of the interface MIB by its index (`net/if_mib.h`: `IFMIB_IFDATA` and
 * `IFDATA_GENERAL`), which answers its exact 64-bit counters. The list
 * `sysctl`'s `NET_RT_IFLIST2` gives, and `getifaddrs`, hand an ordinary
 * process the bytes cut to 32 bits and rounded down to KiB (measured
 * 2026-11-17: 950,809,600 bytes received on `en0` where `netstat -ib` and
 * this MIB said 31,015,581,622). An interface gone between the list and its
 * question (`ENOENT`) is left out. The packets dropped sent are the drops of
 * the interface's send queue, which Darwin counts in 32 bits. */
int64_t hero_sys_network_counters(void *buffer, uint64_t count) {
    hero_sys_fill f = hero_sys_into(buffer, count);
    struct if_nameindex *interfaces = NULL;

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    errno = 0;
    interfaces = if_nameindex();
    if (interfaces == NULL) {
        int code = errno;

        return hero_sys_fail(code == 0 ? HERO_SYS_OTHER : hero_sys_kind_of(code), code);
    }
    for (struct if_nameindex *one = interfaces; one->if_index != 0; one += 1) {
        int names[6] = {CTL_NET, PF_LINK, NETLINK_GENERIC, IFMIB_IFDATA, (int)one->if_index, IFDATA_GENERAL};
        struct ifmibdata data;
        size_t size = sizeof data;

        if (one->if_index > (unsigned int)INT32_MAX) {
            continue;
        }
        memset(&data, 0, sizeof data);
        if (sysctl(names, 6, &data, &size, NULL, 0) != 0) {
            int code = errno;

            if (code == ENOENT) {
                continue;
            }
            if_freenameindex(interfaces);
            return hero_sys_fail(hero_sys_kind_of(code), code);
        }
        if (size != sizeof data) {
            if_freenameindex(interfaces);
            return hero_sys_fail(HERO_SYS_OTHER, 0);
        }
        hero_sys_put(&f, data.ifmd_name, strnlen(data.ifmd_name, sizeof data.ifmd_name));
        hero_sys_zero(&f);
        hero_sys_counter(&f, data.ifmd_data.ifi_ibytes);
        hero_sys_counter(&f, data.ifmd_data.ifi_obytes);
        hero_sys_counter(&f, data.ifmd_data.ifi_ipackets);
        hero_sys_counter(&f, data.ifmd_data.ifi_opackets);
        hero_sys_counter(&f, data.ifmd_data.ifi_ierrors);
        hero_sys_counter(&f, data.ifmd_data.ifi_oerrors);
        hero_sys_counter(&f, data.ifmd_data.ifi_iqdrops);
        hero_sys_counter(&f, (uint64_t)data.ifmd_snd_drops);
    }
    if_freenameindex(interfaces);
    return hero_sys_filled(&f);
}

#elif defined(__linux__)

/* The most a file of `/proc` is read into: `/proc/net/dev` holds about 120
 * bytes an interface, so some hundred thousand interfaces. */
#define HERO_SYS_PROC_MOST ((size_t)16 << 20)

/* The file at `path` read whole into memory `*text` the caller gives back
 * with `hero_layer_free`, its length into `*length`; -1 with the kind set
 * where it cannot be opened or read, or holds more than `HERO_SYS_PROC_MOST`.
 * A file of `/proc` tells no size, so the room grows until a read finds the
 * end. */
static int64_t hero_sys_proc_file(const char *path, char **text, size_t *length) {
    int file = open(path, O_RDONLY | O_CLOEXEC);
    size_t room = 4096;
    size_t got = 0;
    char *bytes = NULL;

    if (file < 0) {
        int code = errno;

        return hero_sys_fail(hero_sys_kind_of(code), code);
    }
    bytes = hero_layer_alloc(room);
    if (bytes == NULL) {
        (void)close(file);
        return hero_sys_fail(HERO_SYS_LIMIT, ENOMEM);
    }
    for (;;) {
        ssize_t read_now = 0;

        if (got == room) {
            char *larger = NULL;

            if (room >= HERO_SYS_PROC_MOST) {
                hero_layer_free(bytes);
                (void)close(file);
                return hero_sys_fail(HERO_SYS_LIMIT, 0);
            }
            larger = hero_layer_grow(bytes, room * 2);
            if (larger == NULL) {
                hero_layer_free(bytes);
                (void)close(file);
                return hero_sys_fail(HERO_SYS_LIMIT, ENOMEM);
            }
            bytes = larger;
            room *= 2;
        }
        read_now = read(file, bytes + got, room - got);
        if (read_now < 0 && errno == EINTR) {
            continue;
        }
        if (read_now < 0) {
            int code = errno;

            hero_layer_free(bytes);
            (void)close(file);
            return hero_sys_fail(hero_sys_kind_of(code), code);
        }
        if (read_now == 0) {
            break;
        }
        got += (size_t)read_now;
    }
    (void)close(file);
    *text = bytes;
    *length = got;
    return 0;
}

/* The unsigned decimal number at `*at` in `text`, spaces before it skipped,
 * into `*value`, `*at` moved past it; false where no digit stands there
 * before the line's end or the number does not fit. */
static bool hero_sys_proc_number(const char *text, size_t length, size_t *at, uint64_t *value) {
    size_t i = *at;
    uint64_t n = 0;
    bool digits = false;

    while (i < length && (text[i] == ' ' || text[i] == '\t')) {
        i += 1;
    }
    while (i < length && text[i] >= '0' && text[i] <= '9') {
        if (__builtin_mul_overflow(n, (uint64_t)10, &n) || __builtin_add_overflow(n, (uint64_t)(text[i] - '0'), &n)) {
            return false;
        }
        digits = true;
        i += 1;
    }
    *at = i;
    *value = n;
    return digits;
}

/* The ticks `/proc` counts time in, from `sysconf`; 0 where it tells none. */
static uint64_t hero_sys_proc_hertz(void) {
    long hertz = sysconf(_SC_CLK_TCK);

    return hertz > 0 ? (uint64_t)hertz : 0;
}

/* `/proc/stat`'s first line is `cpu` and the machine's sums: user, nice,
 * system, idle, iowait, irq, softirq, steal, then guest and guest_nice, which
 * the user's and nice already hold (proc(5)); a kernel before 2.6 writes the
 * first four alone. A Linux with no `/proc` has nothing to give. */
int64_t hero_sys_cpu_times(int64_t *user_us, int64_t *system_us, int64_t *idle_us, int64_t *waiting_us, int64_t *stolen_us) {
    char *text = NULL;
    size_t length = 0;
    size_t at = 0;
    uint64_t columns[8];
    int read_columns = 0;
    uint64_t hertz = hero_sys_proc_hertz();
    uint64_t user = 0;
    uint64_t system = 0;
    int64_t out[5];

    if (user_us == NULL || system_us == NULL || idle_us == NULL || waiting_us == NULL || stolen_us == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (hertz == 0) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    if (hero_sys_proc_file("/proc/stat", &text, &length) != 0) {
        return hero_sys_last_error() == HERO_SYS_NOT_FOUND ? hero_sys_fail(HERO_SYS_UNSUPPORTED, hero_sys_last_os_error()) : -1;
    }
    memset(columns, 0, sizeof columns);
    if (length < 4 || memcmp(text, "cpu ", 4) != 0) {
        hero_layer_free(text);
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    at = 4;
    while (read_columns < 8 && hero_sys_proc_number(text, length, &at, &columns[read_columns])) {
        read_columns += 1;
    }
    hero_layer_free(text);
    if (read_columns < 4 || __builtin_add_overflow(columns[0], columns[1], &user) || __builtin_add_overflow(columns[2], columns[5], &system) || __builtin_add_overflow(system, columns[6], &system)) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    if (!hero_sys_scaled(user, 1000000, hertz, &out[0]) || !hero_sys_scaled(system, 1000000, hertz, &out[1]) || !hero_sys_scaled(columns[3], 1000000, hertz, &out[2]) || !hero_sys_scaled(columns[4], 1000000, hertz, &out[3]) || !hero_sys_scaled(columns[7], 1000000, hertz, &out[4])) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    *user_us = out[0];
    *system_us = out[1];
    *idle_us = out[2];
    *waiting_us = out[3];
    *stolen_us = out[4];
    hero_sys_fine();
    return 0;
}

/* `/proc/<pid>/stat` (proc(5)): the id, the command in parentheses, which
 * may hold spaces and parentheses itself, so the fields are counted from the
 * LAST `)`, then the state and the numbers; `utime` and `stime` are the 14th
 * and 15th fields, in ticks, `vsize` the 23rd, in bytes, and `rss` the 24th,
 * in pages. A process that has ended and is not yet waited for keeps its
 * line with a resident size of 0. */
int64_t hero_sys_process_usage(int64_t pid, int64_t *resident, int64_t *virtual_bytes, int64_t *user_ns, int64_t *system_ns) {
    char path[32];
    char digits[20];
    size_t n = 0;
    size_t p = 0;
    char *text = NULL;
    size_t length = 0;
    size_t at = 0;
    uint64_t fields[22];
    uint64_t hertz = hero_sys_proc_hertz();
    long page = sysconf(_SC_PAGESIZE);
    uint64_t resident_bytes = 0;
    int64_t out[2];
    uint64_t id = 0;

    if (resident == NULL || virtual_bytes == NULL || user_ns == NULL || system_ns == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (pid < 1 || pid > (int64_t)INT32_MAX) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (hertz == 0 || page <= 0) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    id = (uint64_t)pid;
    do {
        digits[n] = (char)('0' + (int)(id % 10u));
        id /= 10u;
        n += 1;
    } while (id > 0);
    memcpy(path, "/proc/", 6);
    p = 6;
    while (n > 0) {
        n -= 1;
        path[p] = digits[n];
        p += 1;
    }
    memcpy(path + p, "/stat", 6);
    if (hero_sys_proc_file(path, &text, &length) != 0) {
        return -1;
    }
    at = length;
    while (at > 0 && text[at - 1] != ')') {
        at -= 1;
    }
    /* After the `)`, a space, the state's one letter, then the numbers from
     * the 4th field on: `fields[k]` is field `k + 4`. */
    if (at == 0 || length - at < 3 || text[at] != ' ') {
        hero_layer_free(text);
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    at += 2;
    memset(fields, 0, sizeof fields);
    for (int k = 0; k < 21; k += 1) {
        if (!hero_sys_proc_number(text, length, &at, &fields[k])) {
            /* A field before `rss` may be negative (the priority and the
             * nice value): read past its sign, its value never used. */
            if (at < length && text[at] == '-' && k != 10 && k != 11 && k != 19 && k != 20) {
                at += 1;
                if (hero_sys_proc_number(text, length, &at, &fields[k])) {
                    continue;
                }
            }
            hero_layer_free(text);
            return hero_sys_fail(HERO_SYS_OTHER, 0);
        }
    }
    hero_layer_free(text);
    /* `fields[10]` is utime, `[11]` stime, `[19]` vsize, `[20]` rss. */
    if (__builtin_mul_overflow(fields[20], (uint64_t)page, &resident_bytes) || resident_bytes > (uint64_t)INT64_MAX || fields[19] > (uint64_t)INT64_MAX) {
        return hero_sys_fail(HERO_SYS_OTHER, 0);
    }
    if (!hero_sys_scaled(fields[10], 1000000000, hertz, &out[0]) || !hero_sys_scaled(fields[11], 1000000000, hertz, &out[1])) {
        return hero_sys_fail(HERO_SYS_LIMIT, 0);
    }
    *resident = (int64_t)resident_bytes;
    *virtual_bytes = (int64_t)fields[19];
    *user_ns = out[0];
    *system_ns = out[1];
    hero_sys_fine();
    return 0;
}

/* `/proc/net/dev` (proc(5)): two lines of headings, then a line for each
 * interface: its name, which holds no `:`, a `:`, eight counters received
 * (bytes, packets, errs, drop, fifo, frame, compressed, multicast) and eight
 * sent (bytes, packets, errs, drop, fifo, colls, carrier, compressed). */
int64_t hero_sys_network_counters(void *buffer, uint64_t count) {
    hero_sys_fill f = hero_sys_into(buffer, count);
    char *text = NULL;
    size_t length = 0;
    size_t at = 0;
    int line = 0;

    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    if (hero_sys_proc_file("/proc/net/dev", &text, &length) != 0) {
        return hero_sys_last_error() == HERO_SYS_NOT_FOUND ? hero_sys_fail(HERO_SYS_UNSUPPORTED, hero_sys_last_os_error()) : -1;
    }
    while (at < length) {
        size_t end = at;
        size_t name_from = at;
        size_t name_to = 0;
        uint64_t columns[16];

        while (end < length && text[end] != '\n') {
            end += 1;
        }
        line += 1;
        if (line > 2 && end > at) {
            while (name_from < end && text[name_from] == ' ') {
                name_from += 1;
            }
            name_to = name_from;
            while (name_to < end && text[name_to] != ':') {
                name_to += 1;
            }
            if (name_to == end || name_to == name_from) {
                hero_layer_free(text);
                return hero_sys_fail(HERO_SYS_OTHER, 0);
            }
            {
                size_t i = name_to + 1;

                for (int k = 0; k < 16; k += 1) {
                    if (!hero_sys_proc_number(text, end, &i, &columns[k])) {
                        hero_layer_free(text);
                        return hero_sys_fail(HERO_SYS_OTHER, 0);
                    }
                }
            }
            hero_sys_put(&f, text + name_from, name_to - name_from);
            hero_sys_zero(&f);
            hero_sys_counter(&f, columns[0]);
            hero_sys_counter(&f, columns[8]);
            hero_sys_counter(&f, columns[1]);
            hero_sys_counter(&f, columns[9]);
            hero_sys_counter(&f, columns[2]);
            hero_sys_counter(&f, columns[10]);
            hero_sys_counter(&f, columns[3]);
            hero_sys_counter(&f, columns[11]);
        }
        at = end + 1;
    }
    hero_layer_free(text);
    return hero_sys_filled(&f);
}

#else

/* A POSIX system neither Darwin nor Linux: none of these readings is written
 * for it, and none is invented. */
int64_t hero_sys_cpu_times(int64_t *user_us, int64_t *system_us, int64_t *idle_us, int64_t *waiting_us, int64_t *stolen_us) {
    if (user_us == NULL || system_us == NULL || idle_us == NULL || waiting_us == NULL || stolen_us == NULL) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_process_usage(int64_t pid, int64_t *resident, int64_t *virtual_bytes, int64_t *user_ns, int64_t *system_ns) {
    if (resident == NULL || virtual_bytes == NULL || user_ns == NULL || system_ns == NULL || pid < 1) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

int64_t hero_sys_network_counters(void *buffer, uint64_t count) {
    if (!hero_sys_room(buffer, count)) {
        return hero_sys_fail(HERO_SYS_INVALID, 0);
    }
    return hero_sys_fail(HERO_SYS_UNSUPPORTED, 0);
}

#endif

#endif
