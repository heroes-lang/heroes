/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_scm.c — a service of the Windows Service Control Manager
 * created, started, stopped, asked and deleted, behind `hero_scm.h`
 * (M-rebel-library, 2026-11-24; spec § 13).
 *
 * THE PROTOCOL, from `winsvc.h` as the toolchain's Windows headers declare it
 * and Microsoft's reference pages of the same calls: `OpenSCManagerW` opens
 * the manager with the rights a call needs and no more (connecting for every
 * call but the install, which creates); `CreateServiceW` writes the entry,
 * `ChangeServiceConfig2W` its description and its failure actions;
 * `StartServiceW` and `ControlService` ask and return, the state coming later;
 * `QueryServiceStatusEx` and `QueryServiceConfigW` read the state, the process
 * and the start type; `DeleteService` marks the entry deleted, which the
 * system removes once its last handle is closed.
 *
 * EACH CALL OPENS WHAT IT NEEDS AND CLOSES IT before it answers, so no handle
 * outlives a call and a program that holds none never keeps a deleted service
 * standing.
 *
 * AN INSTALL IS ONE STEP OR NOTHING: an entry created whose description or
 * failure actions the system then refuses is deleted again before the call
 * answers, so a failed install leaves no half-configured service behind.
 *
 * GetLastError IS READ AT ONCE after the call that failed, before any handle
 * is closed, since closing one may write it.
 *
 * EVERYTHING BUT WINDOWS answers `HERO_SCM_UNSUPPORTED`.
 */

#include "hero_scm.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsvc.h>
#if defined(_MSC_VER)
#pragma comment(lib, "advapi32.lib")
#endif
#endif

static _Thread_local int64_t hero_scm_kind;
static _Thread_local int64_t hero_scm_os;

static void hero_scm_fail(int64_t kind, int64_t os) {
    hero_scm_kind = kind;
    hero_scm_os = os;
}

int64_t hero_scm_last_error(void) {
    return hero_scm_kind;
}

int64_t hero_scm_last_os_error(void) {
    return hero_scm_os;
}

#if !defined(_WIN32)

/* -- a system with no Service Control Manager -------------------------------- */

int64_t hero_scm_supported(void) {
    return 0;
}

int64_t hero_scm_install(const char *name, const char *description, const char *command_line, int64_t at_boot, int64_t restart_delay_ms) {
    (void)name;
    (void)description;
    (void)command_line;
    (void)at_boot;
    (void)restart_delay_ms;
    hero_scm_fail(HERO_SCM_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_scm_start(const char *name) {
    (void)name;
    hero_scm_fail(HERO_SCM_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_scm_stop(const char *name) {
    (void)name;
    hero_scm_fail(HERO_SCM_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_scm_query(const char *name, int64_t *state, int64_t *pid, int64_t *at_boot, int64_t *exit_code) {
    (void)name;
    (void)state;
    (void)pid;
    (void)at_boot;
    (void)exit_code;
    hero_scm_fail(HERO_SCM_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_scm_remove(const char *name) {
    (void)name;
    hero_scm_fail(HERO_SCM_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_scm_last_message(void *buffer, uint64_t n) {
    (void)buffer;
    (void)n;
    return 0;
}

#else

_Static_assert(HERO_SCM_STATE_STOPPED == SERVICE_STOPPED, "the state numbers are the system's");
_Static_assert(HERO_SCM_STATE_START_PENDING == SERVICE_START_PENDING, "the state numbers are the system's");
_Static_assert(HERO_SCM_STATE_STOP_PENDING == SERVICE_STOP_PENDING, "the state numbers are the system's");
_Static_assert(HERO_SCM_STATE_RUNNING == SERVICE_RUNNING, "the state numbers are the system's");
_Static_assert(HERO_SCM_STATE_CONTINUE_PENDING == SERVICE_CONTINUE_PENDING, "the state numbers are the system's");
_Static_assert(HERO_SCM_STATE_PAUSE_PENDING == SERVICE_PAUSE_PENDING, "the state numbers are the system's");
_Static_assert(HERO_SCM_STATE_PAUSED == SERVICE_PAUSED, "the state numbers are the system's");

/* The failure actions an install with a restart writes: a restart at each of
 * the first three failures, the count of failures set back to none after a
 * day without one, as `sc failure` is commonly given. What the system does at
 * a fourth failure inside that day is its own rule, unrun here. */
#define HERO_SCM_RESTARTS 3
#define HERO_SCM_RESET_SECONDS 86400

/* The kind of the system's error number `e`. */
static int64_t hero_scm_kind_of(DWORD e) {
    switch (e) {
    case ERROR_ACCESS_DENIED:
        return HERO_SCM_DENIED;
    case ERROR_SERVICE_EXISTS:
    case ERROR_DUPLICATE_SERVICE_NAME:
        return HERO_SCM_EXISTS;
    case ERROR_SERVICE_DOES_NOT_EXIST:
        return HERO_SCM_NOT_FOUND;
    case ERROR_SERVICE_MARKED_FOR_DELETE:
        return HERO_SCM_MARKED;
    case ERROR_SERVICE_CANNOT_ACCEPT_CTRL:
        return HERO_SCM_BUSY;
    case ERROR_SERVICE_REQUEST_TIMEOUT:
        return HERO_SCM_TIMEOUT;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
        return HERO_SCM_LIMIT;
    default:
        return HERO_SCM_OTHER;
    }
}

/* Records the system's error `e` as the thread's failure; answers -1. */
static int64_t hero_scm_failed(DWORD e) {
    hero_scm_fail(hero_scm_kind_of(e), (int64_t)e);
    return -1;
}

/* UTF-8 text of at most `most` bytes as a zero-ended wide string the caller
 * frees with `hero_layer_free`; NULL, the failure recorded, where it is longer,
 * not UTF-8, or no memory is left. */
static wchar_t *hero_scm_wide(const char *text, size_t most) {
    size_t length = strlen(text);

    if (length > most) {
        hero_scm_fail(HERO_SCM_INVALID, 0);
        return NULL;
    }
    int need = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);

    if (need <= 0) {
        hero_scm_fail(HERO_SCM_INVALID, 0);
        return NULL;
    }
    wchar_t *wide = (wchar_t *)hero_layer_alloc((size_t)need * sizeof(wchar_t));

    if (wide == NULL) {
        hero_scm_fail(HERO_SCM_LIMIT, 0);
        return NULL;
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, wide, need) != need) {
        hero_layer_free(wide);
        hero_scm_fail(HERO_SCM_INVALID, 0);
        return NULL;
    }
    return wide;
}

/* The name as a wide string, refused where it is empty or longer than the
 * system's names are. */
static wchar_t *hero_scm_wide_name(const char *name) {
    if (name[0] == '\0') {
        hero_scm_fail(HERO_SCM_INVALID, 0);
        return NULL;
    }
    return hero_scm_wide(name, HERO_SCM_NAME_MOST);
}

/* The service `name` opened with `access` through a manager opened to
 * connect; NULL, the failure recorded, where either refuses. The caller
 * closes both, `*manager` too. */
static SC_HANDLE hero_scm_open(const char *name, DWORD access, SC_HANDLE *manager) {
    wchar_t *wide = hero_scm_wide_name(name);

    *manager = NULL;
    if (wide == NULL) {
        return NULL;
    }
    SC_HANDLE m = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);

    if (m == NULL) {
        (void)hero_scm_failed(GetLastError());
        hero_layer_free(wide);
        return NULL;
    }
    SC_HANDLE s = OpenServiceW(m, wide, access);

    if (s == NULL) {
        (void)hero_scm_failed(GetLastError());
        hero_layer_free(wide);
        (void)CloseServiceHandle(m);
        return NULL;
    }
    hero_layer_free(wide);
    *manager = m;
    return s;
}

static void hero_scm_close(SC_HANDLE service, SC_HANDLE manager) {
    (void)CloseServiceHandle(service);
    (void)CloseServiceHandle(manager);
}

int64_t hero_scm_supported(void) {
    return 1;
}

/* The description and the failure actions of the service `s` just created;
 * 0, or the system's error number. */
static DWORD hero_scm_configure(SC_HANDLE s, wchar_t *description, int64_t restart_delay_ms) {
    if (description[0] != L'\0') {
        SERVICE_DESCRIPTIONW said;

        said.lpDescription = description;
        if (!ChangeServiceConfig2W(s, SERVICE_CONFIG_DESCRIPTION, &said)) {
            return GetLastError();
        }
    }
    if (restart_delay_ms < 0) {
        return 0;
    }
    SC_ACTION actions[HERO_SCM_RESTARTS];

    for (int i = 0; i < HERO_SCM_RESTARTS; i += 1) {
        actions[i].Type = SC_ACTION_RESTART;
        actions[i].Delay = (DWORD)restart_delay_ms;
    }
    SERVICE_FAILURE_ACTIONSW failing;

    memset(&failing, 0, sizeof failing);
    failing.dwResetPeriod = HERO_SCM_RESET_SECONDS;
    failing.lpRebootMsg = NULL;
    failing.lpCommand = NULL;
    failing.cActions = HERO_SCM_RESTARTS;
    failing.lpsaActions = actions;
    if (!ChangeServiceConfig2W(s, SERVICE_CONFIG_FAILURE_ACTIONS, &failing)) {
        return GetLastError();
    }
    SERVICE_FAILURE_ACTIONS_FLAG counted;

    counted.fFailureActionsOnNonCrashFailures = TRUE;
    if (!ChangeServiceConfig2W(s, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, &counted)) {
        return GetLastError();
    }
    return 0;
}

int64_t hero_scm_install(const char *name, const char *description, const char *command_line, int64_t at_boot, int64_t restart_delay_ms) {
    if ((at_boot != 0 && at_boot != 1) || restart_delay_ms < -1 || restart_delay_ms > (int64_t)UINT32_MAX || command_line[0] == '\0') {
        hero_scm_fail(HERO_SCM_INVALID, 0);
        return -1;
    }
    wchar_t *wide_name = hero_scm_wide_name(name);

    if (wide_name == NULL) {
        return -1;
    }
    wchar_t *wide_description = hero_scm_wide(description, HERO_SCM_TEXT_MOST);

    if (wide_description == NULL) {
        hero_layer_free(wide_name);
        return -1;
    }
    wchar_t *wide_line = hero_scm_wide(command_line, HERO_SCM_TEXT_MOST);

    if (wide_line == NULL) {
        hero_layer_free(wide_name);
        hero_layer_free(wide_description);
        return -1;
    }
    int64_t answer = 0;
    SC_HANDLE m = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);

    if (m == NULL) {
        answer = hero_scm_failed(GetLastError());
    } else {
        /* SERVICE_START beside the configuration: failure actions that
         * restart are written only through a handle that may start. */
        SC_HANDLE s = CreateServiceW(
            m, wide_name, wide_name, SERVICE_CHANGE_CONFIG | SERVICE_START | SERVICE_QUERY_STATUS | DELETE,
            SERVICE_WIN32_OWN_PROCESS, at_boot == 1 ? SERVICE_AUTO_START : SERVICE_DEMAND_START,
            SERVICE_ERROR_NORMAL, wide_line, NULL, NULL, NULL, NULL, NULL
        );

        if (s == NULL) {
            answer = hero_scm_failed(GetLastError());
        } else {
            DWORD refused = hero_scm_configure(s, wide_description, restart_delay_ms);

            if (refused != 0) {
                answer = hero_scm_failed(refused);
                (void)DeleteService(s);
            }
            (void)CloseServiceHandle(s);
        }
        (void)CloseServiceHandle(m);
    }
    hero_layer_free(wide_name);
    hero_layer_free(wide_description);
    hero_layer_free(wide_line);
    if (answer == 0) {
        hero_scm_fail(HERO_SCM_OK, 0);
    }
    return answer;
}

int64_t hero_scm_start(const char *name) {
    SC_HANDLE m;
    SC_HANDLE s = hero_scm_open(name, SERVICE_START, &m);

    if (s == NULL) {
        return -1;
    }
    int64_t answer = 0;

    if (!StartServiceW(s, 0, NULL)) {
        DWORD e = GetLastError();

        answer = e == ERROR_SERVICE_ALREADY_RUNNING ? 1 : hero_scm_failed(e);
    }
    hero_scm_close(s, m);
    if (answer >= 0) {
        hero_scm_fail(HERO_SCM_OK, 0);
    }
    return answer;
}

int64_t hero_scm_stop(const char *name) {
    SC_HANDLE m;
    SC_HANDLE s = hero_scm_open(name, SERVICE_STOP, &m);

    if (s == NULL) {
        return -1;
    }
    int64_t answer = 0;
    SERVICE_STATUS status;

    if (!ControlService(s, SERVICE_CONTROL_STOP, &status)) {
        DWORD e = GetLastError();

        answer = e == ERROR_SERVICE_NOT_ACTIVE ? 1 : hero_scm_failed(e);
    }
    hero_scm_close(s, m);
    if (answer >= 0) {
        hero_scm_fail(HERO_SCM_OK, 0);
    }
    return answer;
}

/* 1 where the service `s` is started by the system at boot, 0 where not, -1
 * with the failure recorded. */
static int64_t hero_scm_starts_at_boot(SC_HANDLE s) {
    DWORD need = 0;

    if (QueryServiceConfigW(s, NULL, 0, &need)) {
        hero_scm_fail(HERO_SCM_OTHER, 0);
        return -1;
    }
    DWORD e = GetLastError();

    if (e != ERROR_INSUFFICIENT_BUFFER || need == 0) {
        return hero_scm_failed(e);
    }
    QUERY_SERVICE_CONFIGW *config = (QUERY_SERVICE_CONFIGW *)hero_layer_alloc(need);

    if (config == NULL) {
        hero_scm_fail(HERO_SCM_LIMIT, 0);
        return -1;
    }
    if (!QueryServiceConfigW(s, config, need, &need)) {
        e = GetLastError();
        hero_layer_free(config);
        return hero_scm_failed(e);
    }
    int64_t at_boot = config->dwStartType == SERVICE_AUTO_START ? 1 : 0;

    hero_layer_free(config);
    return at_boot;
}

int64_t hero_scm_query(const char *name, int64_t *state, int64_t *pid, int64_t *at_boot, int64_t *exit_code) {
    SC_HANDLE m;
    SC_HANDLE s = hero_scm_open(name, SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG, &m);

    if (s == NULL) {
        return -1;
    }
    SERVICE_STATUS_PROCESS status;
    DWORD got = 0;

    if (!QueryServiceStatusEx(s, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof status, &got)) {
        int64_t answer = hero_scm_failed(GetLastError());

        hero_scm_close(s, m);
        return answer;
    }
    int64_t boot = hero_scm_starts_at_boot(s);

    hero_scm_close(s, m);
    if (boot < 0) {
        return -1;
    }
    *state = (int64_t)status.dwCurrentState;
    *pid = (int64_t)status.dwProcessId;
    *at_boot = boot;
    *exit_code = status.dwWin32ExitCode == ERROR_SERVICE_SPECIFIC_ERROR ? (int64_t)status.dwServiceSpecificExitCode
                                                                         : (int64_t)status.dwWin32ExitCode;
    hero_scm_fail(HERO_SCM_OK, 0);
    return 0;
}

int64_t hero_scm_remove(const char *name) {
    SC_HANDLE m;
    SC_HANDLE s = hero_scm_open(name, DELETE, &m);

    if (s == NULL) {
        return -1;
    }
    int64_t answer = 0;

    if (!DeleteService(s)) {
        DWORD e = GetLastError();

        answer = e == ERROR_SERVICE_MARKED_FOR_DELETE ? 0 : hero_scm_failed(e);
    }
    hero_scm_close(s, m);
    if (answer == 0) {
        hero_scm_fail(HERO_SCM_OK, 0);
    }
    return answer;
}

int64_t hero_scm_last_message(void *buffer, uint64_t n) {
    int64_t os = hero_scm_os;
    wchar_t wide[512];

    if (os <= 0 || os > (int64_t)UINT32_MAX) {
        return 0;
    }
    DWORD got = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, (DWORD)os, 0, wide, (DWORD)(sizeof wide / sizeof wide[0]), NULL);

    while (got > 0 && (wide[got - 1] == L'\r' || wide[got - 1] == L'\n' || wide[got - 1] == L' ')) {
        got -= 1;
    }
    if (got == 0) {
        return 0;
    }
    int bytes = WideCharToMultiByte(CP_UTF8, 0, wide, (int)got, NULL, 0, NULL, NULL);

    if (bytes <= 0) {
        return 0;
    }
    if (buffer != NULL && n >= (uint64_t)bytes) {
        (void)WideCharToMultiByte(CP_UTF8, 0, wide, (int)got, (char *)buffer, (int)bytes, NULL, NULL);
    }
    return bytes;
}

#endif
