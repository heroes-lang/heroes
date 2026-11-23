/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_printing.c — the system's printers, one member of the
 * archive the compiler puts on every program's link line (M-rebel-library,
 * 2026-11-22; `selfhost/cli/layer.hero` builds, keys and links it). Its
 * header, `hero_printing.h`, says what each function answers; this file says
 * how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * TWO ARMS. POSIX (macOS and Linux): CUPS's library, opened with `dlopen` at
 * the first call, as `hero_keyring.c` opens libsecret, so a build needs
 * neither its headers nor the library, a machine without it answers
 * `HERO_PRINTING_UNSUPPORTED`, and one code reaches the scheduler on both
 * systems: the destinations' calls for the printers and their papers, the
 * document calls for a job (`cupsCreateJob`, `cupsStartDocument`,
 * `cupsWriteRequestData`, `cupsFinishDocument`), and IPP's
 * Get-Job-Attributes for a job's state, its reasons, its words and its pages.
 * Where CUPS's own header stands (macOS), the layouts this file declares are
 * held to it at compile time. Windows: the spooler's calls (`EnumPrintersW`,
 * `DeviceCapabilitiesW`, `GetJobW`, `SetJobW`, `EnumJobsW`) for the printers,
 * their papers and their jobs, and for a document the system's own PDF
 * renderer (`Windows.Data.Pdf`, through `PdfCreateRenderer`) drawing each
 * page through Direct2D's print control into a print job the spooler opens
 * from a print ticket the printer's driver writes from the job's settings
 * (`PTConvertDevModeToPrintTicket`), its number told by the package's status
 * event. Those libraries but the spooler's, OLE's and the shell's light ones
 * are found with `GetProcAddress` at the first submission, so a program that
 * only lists printers loads none of them.
 *
 * THE LIBRARIES ARE LINKED BY THE MEMBER ITSELF, as `hero_keyring.c` links
 * its own: on Windows `winspool`, `gdi32`, `ole32` and `shlwapi` by `#pragma
 * comment(lib)`, and nothing on macOS or Linux. So a program that never calls
 * this member links none of them.
 *
 * MEMORY: a list is gathered in a block of `hero_layer_alloc`'s and copied
 * into the caller's buffer where it fits, so a short buffer gets nothing
 * written; every block is given back before the call returns.
 *
 * Written from the platforms' documentation: CUPS's programming manual and
 * header reference, RFC 8011 and PWG 5100.13 and 5101.1 for IPP's attributes
 * and the papers' names, and Microsoft's reference for the spooler, the print
 * ticket, Direct2D's printing and `Windows.Data.Pdf`; no other library's
 * source.
 */

#include "hero_printing.h"

#include "hero_layer_alloc.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ==========================================================================
 * WHAT BOTH ARMS SHARE: the failure of the last call, the checks of what a
 * caller hands in, and the list a call writes.
 * ========================================================================== */

#define HERO_PRINTING_WORDS_MOST 1024

/* The last failure on this thread: its kind, the system's number, and its
 * words. Per thread, as `errno` is, so two threads' calls never read each
 * other's. */
static _Thread_local int64_t hero_printing_kind;
static _Thread_local int64_t hero_printing_os;
static _Thread_local char hero_printing_words[HERO_PRINTING_WORDS_MOST];

static void hero_printing_clear(void) {
    hero_printing_kind = HERO_PRINTING_OK;
    hero_printing_os = 0;
    hero_printing_words[0] = '\0';
}

/* Records a failure and answers -1, the value every failing call answers. */
static int64_t hero_printing_fail(int64_t kind, int64_t os, const char *format, ...) {
    va_list words;

    hero_printing_kind = kind;
    hero_printing_os = os;
    va_start(words, format);
    (void)vsnprintf(hero_printing_words, sizeof hero_printing_words, format, words);
    va_end(words);
    return -1;
}

int64_t hero_printing_last_error(void) {
    return hero_printing_kind;
}

int64_t hero_printing_last_os_error(void) {
    return hero_printing_os;
}

int64_t hero_printing_last_message(void *buffer, uint64_t count) {
    size_t length = strlen(hero_printing_words);

    if (buffer != NULL && (uint64_t)length <= count) {
        memcpy(buffer, hero_printing_words, length);
    }
    return (int64_t)length;
}

/* Whether the `n` bytes at `s` are UTF-8: no overlong form, no surrogate,
 * nothing past U+10FFFF. */
static bool hero_printing_utf8(const unsigned char *s, size_t n) {
    size_t i = 0;

    while (i < n) {
        unsigned char c = s[i];
        size_t more;
        uint32_t code;

        if (c < 0x80) {
            i += 1;
            continue;
        }
        if (c >= 0xC2 && c <= 0xDF) {
            more = 1;
            code = c & 0x1Fu;
        } else if (c >= 0xE0 && c <= 0xEF) {
            more = 2;
            code = c & 0x0Fu;
        } else if (c >= 0xF0 && c <= 0xF4) {
            more = 3;
            code = c & 0x07u;
        } else {
            return false;
        }
        if (n - i <= more) {
            return false;
        }
        for (size_t k = 1; k <= more; k++) {
            if ((s[i + k] & 0xC0u) != 0x80u) {
                return false;
            }
            code = (code << 6) | (s[i + k] & 0x3Fu);
        }
        if ((more == 2 && (code < 0x800 || (code >= 0xD800 && code <= 0xDFFF))) || (more == 3 && (code < 0x10000 || code > 0x10FFFF))) {
            return false;
        }
        i += more + 1;
    }
    return true;
}

/* Whether `s` is a name this layer takes: not NULL, UTF-8, at most
 * `HERO_PRINTING_NAME_MOST` bytes, and empty only where `may_be_empty`;
 * where it is not, the failure is recorded with `what` in its words. */
static bool hero_printing_name(const char *what, const char *s, bool may_be_empty) {
    size_t length;

    if (s == NULL) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "no %s was given", what);
        return false;
    }
    length = strlen(s);
    if (length == 0 && !may_be_empty) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "the %s is empty", what);
        return false;
    }
    if (length > HERO_PRINTING_NAME_MOST) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "the %s is %zu bytes long, past the %d a name may hold", what, length, HERO_PRINTING_NAME_MOST);
        return false;
    }
    if (!hero_printing_utf8((const unsigned char *)s, length)) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "the %s is not UTF-8", what);
        return false;
    }
    return true;
}

/* Whether a buffer and its count agree: a count with no bytes behind it is
 * refused. */
static bool hero_printing_room(const void *buffer, uint64_t count) {
    if (buffer == NULL && count > 0) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "a count of %llu bytes was given with no buffer", (unsigned long long)count);
        return false;
    }
    return true;
}

/* Whether a job's settings are within their values (`hero_printing_submit`
 * in the header), every arm asking the same before the system is reached. */
static bool hero_printing_settings(const void *pdf, uint64_t size, int64_t copies, int64_t orientation, int64_t sides, int64_t color, int64_t collate, const int64_t *ranges, uint64_t range_count) {
    static const char signature[] = "%PDF-";
    uint64_t look = size < 1024 ? size : 1024;
    bool found = false;

    if (pdf == NULL && size > 0) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "a document of %llu bytes was given with no bytes", (unsigned long long)size);
        return false;
    }
    /* A PDF's header stands in its first 1024 bytes (ISO 32000-1, 7.5.2's
     * implementation note, which readers honour). */
    for (uint64_t i = 0; pdf != NULL && i + 5 <= look && !found; i++) {
        found = memcmp((const char *)pdf + i, signature, 5) == 0;
    }
    if (!found) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "the document is not a PDF: no %%PDF- header in its first %llu bytes", (unsigned long long)look);
        return false;
    }
    if (copies < 1 || copies > HERO_PRINTING_COPIES_MOST) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "%lld copies were asked, and a job takes 1 to %d", (long long)copies, HERO_PRINTING_COPIES_MOST);
        return false;
    }
    if (orientation < HERO_PRINTING_PORTRAIT || orientation > HERO_PRINTING_REVERSE_PORTRAIT) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "orientation %lld is none of the four", (long long)orientation);
        return false;
    }
    if (sides < HERO_PRINTING_ONE_SIDED || sides > HERO_PRINTING_TWO_SIDED_SHORT_EDGE) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "sides %lld is none of the three", (long long)sides);
        return false;
    }
    if (color < HERO_PRINTING_COLOR_DEFAULT || color > HERO_PRINTING_MONOCHROME) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "colour %lld is none of the three", (long long)color);
        return false;
    }
    if (collate != 0 && collate != 1) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "collate is %lld, where it is 0 or 1", (long long)collate);
        return false;
    }
    if ((ranges == NULL && range_count > 0) || range_count % 2 != 0 || range_count > 2 * (uint64_t)HERO_PRINTING_RANGES_MOST) {
        hero_printing_fail(HERO_PRINTING_INVALID, 0, "%llu numbers were given as page ranges, where they are pairs, at most %d", (unsigned long long)range_count, HERO_PRINTING_RANGES_MOST);
        return false;
    }
    for (uint64_t i = 0; i < range_count; i += 2) {
        int64_t first = ranges[i];
        int64_t last = ranges[i + 1];

        if (first < 1 || last < first || last > INT32_MAX || (i > 0 && first <= ranges[i - 1] + 1)) {
            hero_printing_fail(HERO_PRINTING_INVALID, 0, "page range %lld-%lld is out of order: a range runs from page 1 or later to a page not before its first, each after the one before it and not touching it", (long long)first, (long long)last);
            return false;
        }
    }
    return true;
}

/* A list gathered field by field (WHAT CROSSES in the header). Where the
 * system has no memory for it, `short_of_memory` holds and the call answers
 * `HERO_PRINTING_LIMIT`. */
typedef struct {
    char *bytes;
    size_t used;
    size_t room;
    bool short_of_memory;
} hero_printing_list;

static void hero_printing_add_bytes(hero_printing_list *list, const char *field, size_t length) {
    if (list->short_of_memory) {
        return;
    }
    if (list->used + length + 1 > list->room) {
        size_t room = list->room == 0 ? 1024 : list->room;
        char *grown;

        while (room < list->used + length + 1) {
            room *= 2;
        }
        grown = hero_layer_grow(list->bytes, room);
        if (grown == NULL) {
            list->short_of_memory = true;
            return;
        }
        list->bytes = grown;
        list->room = room;
    }
    memcpy(list->bytes + list->used, field, length);
    list->bytes[list->used + length] = '\0';
    list->used += length + 1;
}

/* A field of text; NULL is the empty field. */
static void hero_printing_add(hero_printing_list *list, const char *field) {
    hero_printing_add_bytes(list, field == NULL ? "" : field, field == NULL ? 0 : strlen(field));
}

static void hero_printing_add_number(hero_printing_list *list, int64_t n) {
    char digits[32];
    int length = snprintf(digits, sizeof digits, "%lld", (long long)n);

    hero_printing_add_bytes(list, digits, (size_t)length);
}

/* Hands the list to the caller (WHAT CROSSES) and gives its block back. */
static int64_t hero_printing_hand(hero_printing_list *list, void *buffer, uint64_t count) {
    int64_t answer;

    if (list->short_of_memory) {
        answer = hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for the list");
    } else {
        if (buffer != NULL && (uint64_t)list->used <= count && list->used > 0) {
            memcpy(buffer, list->bytes, list->used);
        }
        answer = (int64_t)list->used;
    }
    hero_layer_free(list->bytes);
    list->bytes = NULL;
    return answer;
}

/* The word of a job's state, by IPP's `job-state` number (RFC 8011,
 * 5.3.7), which the Windows arm writes too. */
static const char *hero_printing_job_word(int state) {
    switch (state) {
    case 3:
        return "pending";
    case 4:
        return "held";
    case 5:
        return "processing";
    case 6:
        return "stopped";
    case 7:
        return "cancelled";
    case 8:
        return "aborted";
    case 9:
        return "completed";
    default:
        return "pending";
    }
}

static bool hero_printing_ended(int state) {
    return state >= 7 && state <= 9;
}

/* The arms each define these three: a job's IPP state number with its
 * fields, the system asked once; a pause of `ms` milliseconds; and a
 * monotonic clock in milliseconds. `hero_printing_wait` is written once,
 * over them. */
static int64_t hero_printing_job_state(const char *printer, int64_t job, int *state, hero_printing_list *fields);
static void hero_printing_pause(int64_t ms);
static int64_t hero_printing_clock(void);

int64_t hero_printing_wait(const char *printer, int64_t job, int64_t within_ms) {
    int64_t deadline;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false)) {
        return -1;
    }
    if (within_ms < 0 || within_ms > 3600000) {
        return hero_printing_fail(HERO_PRINTING_INVALID, 0, "a wait of %lld ms was asked, where it is 0 to an hour", (long long)within_ms);
    }
    deadline = hero_printing_clock() + within_ms;
    for (;;) {
        int state = 0;
        int64_t now;

        if (hero_printing_job_state(printer, job, &state, NULL) < 0) {
            return -1;
        }
        if (hero_printing_ended(state)) {
            return 1;
        }
        now = hero_printing_clock();
        if (now >= deadline) {
            return 0;
        }
        hero_printing_pause(deadline - now < 250 ? deadline - now : 250);
    }
}

int64_t hero_printing_job(const char *printer, int64_t job, void *buffer, uint64_t count) {
    hero_printing_list list = {0};
    int state = 0;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count)) {
        return -1;
    }
    if (hero_printing_job_state(printer, job, &state, &list) < 0) {
        hero_layer_free(list.bytes);
        return -1;
    }
    return hero_printing_hand(&list, buffer, count);
}

#if defined(_WIN32)

/* ==========================================================================
 * WINDOWS: the spooler, and the system's PDF renderer through Direct2D.
 * ========================================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winspool.h>
#include <objbase.h>
#include <ocidl.h>
#include <shlwapi.h>
#include <documenttarget.h>
#if defined(_MSC_VER)
/* What only a program that takes this member links. MSVC's word; a MinGW
 * target links them by `-l`. */
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shlwapi.lib")
#endif

/* A Win32 error's kind. */
static int64_t hero_printing_win_kind(DWORD code) {
    switch (code) {
    case ERROR_INVALID_PRINTER_NAME:
    case ERROR_FILE_NOT_FOUND:
        return HERO_PRINTING_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return HERO_PRINTING_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
        return HERO_PRINTING_LIMIT;
    case RPC_S_SERVER_UNAVAILABLE:
    case ERROR_SERVICE_NOT_ACTIVE:
        return HERO_PRINTING_UNSUPPORTED;
    default:
        return HERO_PRINTING_OTHER;
    }
}

/* An HRESULT's kind. */
static int64_t hero_printing_hr_kind(HRESULT hr) {
    if (hr == E_OUTOFMEMORY) {
        return HERO_PRINTING_LIMIT;
    }
    if (hr == E_ACCESSDENIED) {
        return HERO_PRINTING_DENIED;
    }
    if (hr == HRESULT_FROM_WIN32(ERROR_INVALID_PRINTER_NAME)) {
        return HERO_PRINTING_NOT_FOUND;
    }
    return HERO_PRINTING_OTHER;
}

/* Records the system's failure `code` (a Win32 error or an HRESULT) with
 * what was asked, the system's own words after it. */
static int64_t hero_printing_win_fail(int64_t kind, DWORD code, const char *what) {
    WCHAR wide[512];
    char words[512];
    DWORD n = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, 0, wide, 512, NULL);
    int m;

    while (n > 0 && (wide[n - 1] == L'\r' || wide[n - 1] == L'\n' || wide[n - 1] == L' ' || wide[n - 1] == L'.')) {
        n--;
    }
    m = n == 0 ? 0 : WideCharToMultiByte(CP_UTF8, 0, wide, (int)n, words, (int)sizeof words - 1, NULL, NULL);
    words[m < 0 ? 0 : m] = '\0';
    return hero_printing_fail(kind, (int64_t)code, "%s: %s (0x%08lx)", what, m > 0 ? words : "the system gave no words", (unsigned long)code);
}

/* `s`, UTF-8 the caller checked, as UTF-16 into `wide` of `room` units. */
static bool hero_printing_wide(const char *s, WCHAR *wide, int room) {
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, -1, wide, room);

    if (n <= 0) {
        hero_printing_fail(HERO_PRINTING_INVALID, (int64_t)GetLastError(), "a name could not be written as UTF-16");
        return false;
    }
    return true;
}

/* `wide` as a UTF-8 field of the list; NULL is the empty field. */
static void hero_printing_add_wide(hero_printing_list *list, const WCHAR *wide) {
    char narrow[1024];
    int n;

    if (wide == NULL || wide[0] == L'\0') {
        hero_printing_add(list, "");
        return;
    }
    n = WideCharToMultiByte(CP_UTF8, 0, wide, -1, narrow, (int)sizeof narrow, NULL, NULL);
    hero_printing_add(list, n > 0 ? narrow : "");
}

static void hero_printing_pause(int64_t ms) {
    Sleep((DWORD)ms);
}

static int64_t hero_printing_clock(void) {
    return (int64_t)GetTickCount64();
}

/* Opens `printer` for reading, its name checked by the caller. */
static HANDLE hero_printing_open(const char *printer) {
    WCHAR name[HERO_PRINTING_NAME_MOST + 1];
    HANDLE handle = NULL;

    if (!hero_printing_wide(printer, name, HERO_PRINTING_NAME_MOST + 1)) {
        return NULL;
    }
    if (!OpenPrinterW(name, &handle, NULL)) {
        DWORD code = GetLastError();

        hero_printing_win_fail(hero_printing_win_kind(code), code, "the printer could not be opened");
        return NULL;
    }
    return handle;
}

/* A block the spooler fills, asked for again while it grows between the two
 * calls; NULL with the failure recorded. `ask` answers whether the spooler
 * filled `size` bytes, setting `*needed`. */
typedef BOOL (*hero_printing_asker)(void *context, BYTE *block, DWORD size, DWORD *needed);

static BYTE *hero_printing_ask(hero_printing_asker ask, void *context, const char *what) {
    DWORD needed = 0;
    BYTE *block = NULL;

    if (ask(context, NULL, 0, &needed)) {
        needed = needed == 0 ? 1 : needed;
    }
    for (int round = 0; round < 8; round++) {
        DWORD code = GetLastError();

        if (needed == 0) {
            hero_printing_win_fail(hero_printing_win_kind(code), code, what);
            return NULL;
        }
        block = hero_layer_alloc(needed);
        if (block == NULL) {
            hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for %s", what);
            return NULL;
        }
        if (ask(context, block, needed, &needed)) {
            return block;
        }
        code = GetLastError();
        hero_layer_free(block);
        block = NULL;
        if (code != ERROR_INSUFFICIENT_BUFFER) {
            hero_printing_win_fail(hero_printing_win_kind(code), code, what);
            return NULL;
        }
    }
    hero_printing_fail(HERO_PRINTING_OTHER, 0, "%s kept growing", what);
    return NULL;
}

static BOOL hero_printing_ask_printers(void *context, BYTE *block, DWORD size, DWORD *needed) {
    DWORD *returned = context;

    return EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, block, size, needed, returned);
}

/* The state word and the reasons of a printer, by its status bits and
 * attributes, in IPP's words (PWG 5100.13's `printer-state-reasons`). */
static void hero_printing_printer_state(DWORD status, DWORD attributes, DWORD jobs, hero_printing_list *list) {
    static const struct {
        DWORD bit;
        const char *reason;
        bool stops;
    } reasons[] = {
        {PRINTER_STATUS_PAUSED, "paused", true},
        {PRINTER_STATUS_ERROR, "other", true},
        {PRINTER_STATUS_PAPER_JAM, "media-jam", true},
        {PRINTER_STATUS_PAPER_OUT, "media-empty", true},
        {PRINTER_STATUS_MANUAL_FEED, "media-needed", false},
        {PRINTER_STATUS_PAPER_PROBLEM, "media-needed", true},
        {PRINTER_STATUS_OFFLINE, "offline-report", true},
        {PRINTER_STATUS_NOT_AVAILABLE, "offline-report", true},
        {PRINTER_STATUS_OUTPUT_BIN_FULL, "output-area-full", true},
        {PRINTER_STATUS_TONER_LOW, "toner-low", false},
        {PRINTER_STATUS_NO_TONER, "toner-empty", true},
        {PRINTER_STATUS_USER_INTERVENTION, "other", true},
        {PRINTER_STATUS_OUT_OF_MEMORY, "other", true},
        {PRINTER_STATUS_DOOR_OPEN, "door-open", true},
        {PRINTER_STATUS_PENDING_DELETION, "deactivated", true},
    };
    char joined[512] = "";
    size_t used = 0;
    bool stopped = false;
    const char *word;

    for (size_t i = 0; i < sizeof reasons / sizeof reasons[0]; i++) {
        if ((status & reasons[i].bit) != 0 && strstr(joined, reasons[i].reason) == NULL) {
            int n = snprintf(joined + used, sizeof joined - used, "%s%s", used == 0 ? "" : ",", reasons[i].reason);

            used += n > 0 ? (size_t)n : 0;
            stopped = stopped || reasons[i].stops;
        }
    }
    if ((attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE) != 0 && strstr(joined, "offline-report") == NULL) {
        int n = snprintf(joined + used, sizeof joined - used, "%soffline-report", used == 0 ? "" : ",");

        used += n > 0 ? (size_t)n : 0;
        stopped = true;
    }
    if (stopped) {
        word = "stopped";
    } else if ((status & (PRINTER_STATUS_PRINTING | PRINTER_STATUS_PROCESSING | PRINTER_STATUS_BUSY | PRINTER_STATUS_IO_ACTIVE | PRINTER_STATUS_WARMING_UP | PRINTER_STATUS_INITIALIZING)) != 0 || jobs > 0) {
        word = "processing";
    } else {
        word = "idle";
    }
    hero_printing_add(list, word);
    hero_printing_add(list, joined);
}

int64_t hero_printing_printers(void *buffer, uint64_t count) {
    hero_printing_list list = {0};
    DWORD returned = 0;
    WCHAR fallback[HERO_PRINTING_NAME_MOST + 1];
    DWORD fallback_length = HERO_PRINTING_NAME_MOST + 1;
    bool has_default;
    BYTE *block;

    hero_printing_clear();
    if (!hero_printing_room(buffer, count)) {
        return -1;
    }
    has_default = GetDefaultPrinterW(fallback, &fallback_length) != 0;
    block = hero_printing_ask(hero_printing_ask_printers, &returned, "the printers could not be listed");
    if (block == NULL) {
        return -1;
    }
    for (DWORD i = 0; i < returned; i++) {
        const PRINTER_INFO_2W *p = &((const PRINTER_INFO_2W *)block)[i];

        hero_printing_add_wide(&list, p->pPrinterName);
        hero_printing_add_wide(&list, p->pPrinterName);
        hero_printing_add_wide(&list, p->pLocation);
        hero_printing_add_wide(&list, p->pDriverName);
        hero_printing_printer_state(p->Status, p->Attributes, p->cJobs, &list);
        hero_printing_add(&list, "");
        hero_printing_add(&list, (p->Status & PRINTER_STATUS_PENDING_DELETION) != 0 ? "0" : "1");
        hero_printing_add(&list, has_default && p->pPrinterName != NULL && wcscmp(p->pPrinterName, fallback) == 0 ? "1" : "0");
    }
    hero_layer_free(block);
    return hero_printing_hand(&list, buffer, count);
}

/* The papers' PWG names by their size in tenths of a millimetre, Windows'
 * unit (PWG 5101.1's table, the sizes every driver lists). */
static const struct {
    int width;
    int length;
    const char *name;
} hero_printing_pwg[] = {
    {2159, 2794, "na_letter_8.5x11in"},
    {2159, 3556, "na_legal_8.5x14in"},
    {1842, 2667, "na_executive_7.25x10.5in"},
    {2794, 4318, "na_ledger_11x17in"},
    {1397, 2159, "na_invoice_5.5x8.5in"},
    {2159, 3302, "na_foolscap_8.5x13in"},
    {1048, 2413, "na_number-10_4.125x9.5in"},
    {984, 1905, "na_monarch_3.875x7.5in"},
    {4200, 5940, "iso_a2_420x594mm"},
    {2970, 4200, "iso_a3_297x420mm"},
    {2100, 2970, "iso_a4_210x297mm"},
    {1480, 2100, "iso_a5_148x210mm"},
    {1050, 1480, "iso_a6_105x148mm"},
    {2500, 3530, "iso_b4_250x353mm"},
    {1760, 2500, "iso_b5_176x250mm"},
    {2570, 3640, "jis_b4_257x364mm"},
    {1820, 2570, "jis_b5_182x257mm"},
    {1100, 2200, "iso_dl_110x220mm"},
    {1620, 2290, "iso_c5_162x229mm"},
    {2290, 3240, "iso_c4_229x324mm"},
    {1140, 1620, "iso_c6_114x162mm"},
    {1000, 1480, "jpn_hagaki_100x148mm"},
};

/* The papers a printer's driver lists: their numbers, names and sizes. */
typedef struct {
    int count;
    WORD *ids;
    WCHAR (*names)[64];
    POINT *sizes;
    char (*pwg)[HERO_PRINTING_NAME_MOST + 1];
} hero_printing_papers;

static void hero_printing_papers_free(hero_printing_papers *papers) {
    hero_layer_free(papers->ids);
    hero_layer_free(papers->names);
    hero_layer_free(papers->sizes);
    hero_layer_free(papers->pwg);
    memset(papers, 0, sizeof *papers);
}

/* Reads the papers of `name` on `port`, each given its PWG name: the
 * table's where its size is one of the table's and no paper before it took
 * the name, else `om_dmpaper-<number>_<w>x<h>mm`, so every paper's name is
 * one and names that paper alone. */
static bool hero_printing_papers_of(const WCHAR *name, const WCHAR *port, hero_printing_papers *papers) {
    int n = DeviceCapabilitiesW(name, port, DC_PAPERS, NULL, NULL);

    memset(papers, 0, sizeof *papers);
    if (n < 0) {
        DWORD code = GetLastError();

        hero_printing_win_fail(hero_printing_win_kind(code), code, "the printer's papers could not be read");
        return false;
    }
    if (n == 0) {
        return true;
    }
    papers->ids = hero_layer_alloc((size_t)n * sizeof *papers->ids);
    papers->names = hero_layer_alloc((size_t)n * sizeof *papers->names);
    papers->sizes = hero_layer_alloc((size_t)n * sizeof *papers->sizes);
    papers->pwg = hero_layer_alloc((size_t)n * sizeof *papers->pwg);
    if (papers->ids == NULL || papers->names == NULL || papers->sizes == NULL || papers->pwg == NULL) {
        hero_printing_papers_free(papers);
        hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for the printer's papers");
        return false;
    }
    if (DeviceCapabilitiesW(name, port, DC_PAPERS, (LPWSTR)papers->ids, NULL) != n || DeviceCapabilitiesW(name, port, DC_PAPERNAMES, (LPWSTR)papers->names, NULL) != n ||
        DeviceCapabilitiesW(name, port, DC_PAPERSIZE, (LPWSTR)papers->sizes, NULL) != n) {
        hero_printing_papers_free(papers);
        hero_printing_fail(HERO_PRINTING_OTHER, 0, "the printer's driver listed its papers in counts that disagree");
        return false;
    }
    papers->count = n;
    for (int i = 0; i < n; i++) {
        const char *known = NULL;

        papers->names[i][63] = L'\0';
        for (size_t k = 0; k < sizeof hero_printing_pwg / sizeof hero_printing_pwg[0] && known == NULL; k++) {
            int dw = papers->sizes[i].x - hero_printing_pwg[k].width;
            int dl = papers->sizes[i].y - hero_printing_pwg[k].length;

            if (dw >= -1 && dw <= 1 && dl >= -1 && dl <= 1) {
                known = hero_printing_pwg[k].name;
            }
        }
        for (int j = 0; j < i && known != NULL; j++) {
            if (strcmp(papers->pwg[j], known) == 0) {
                known = NULL;
            }
        }
        if (known != NULL) {
            snprintf(papers->pwg[i], sizeof papers->pwg[i], "%s", known);
        } else {
            snprintf(papers->pwg[i], sizeof papers->pwg[i], "om_dmpaper-%u_%d.%dx%d.%dmm", (unsigned)papers->ids[i], (int)(papers->sizes[i].x / 10), (int)(papers->sizes[i].x % 10), (int)(papers->sizes[i].y / 10), (int)(papers->sizes[i].y % 10));
        }
    }
    return true;
}

/* The printer's default settings, in a block of the caller's to give back. */
static DEVMODEW *hero_printing_devmode(HANDLE handle, WCHAR *name) {
    LONG size = DocumentPropertiesW(NULL, handle, name, NULL, NULL, 0);
    DEVMODEW *mode;

    if (size <= 0) {
        DWORD code = GetLastError();

        hero_printing_win_fail(hero_printing_win_kind(code), code, "the printer's settings could not be read");
        return NULL;
    }
    mode = hero_layer_alloc((size_t)size);
    if (mode == NULL) {
        hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for the printer's settings");
        return NULL;
    }
    if (DocumentPropertiesW(NULL, handle, name, mode, NULL, DM_OUT_BUFFER) != IDOK) {
        DWORD code = GetLastError();

        hero_layer_free(mode);
        hero_printing_win_fail(hero_printing_win_kind(code), code, "the printer's settings could not be read");
        return NULL;
    }
    return mode;
}

/* The port a printer prints to, which `DeviceCapabilitiesW` asks. */
static BOOL hero_printing_ask_printer(void *context, BYTE *block, DWORD size, DWORD *needed) {
    return GetPrinterW((HANDLE)context, 2, block, size, needed);
}

int64_t hero_printing_media(const char *printer, void *buffer, uint64_t count) {
    hero_printing_list list = {0};
    hero_printing_papers papers;
    HANDLE handle;
    PRINTER_INFO_2W *info;
    DEVMODEW *mode;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count)) {
        return -1;
    }
    handle = hero_printing_open(printer);
    if (handle == NULL) {
        return -1;
    }
    info = (PRINTER_INFO_2W *)hero_printing_ask(hero_printing_ask_printer, handle, "the printer could not be read");
    if (info == NULL) {
        ClosePrinter(handle);
        return -1;
    }
    mode = hero_printing_devmode(handle, info->pPrinterName);
    if (mode == NULL || !hero_printing_papers_of(info->pPrinterName, info->pPortName, &papers)) {
        hero_layer_free(mode);
        hero_layer_free(info);
        ClosePrinter(handle);
        return -1;
    }
    for (int i = 0; i < papers.count; i++) {
        int margins[4] = {0, 0, 0, 0};
        DEVMODEW *one = hero_layer_alloc((size_t)mode->dmSize + mode->dmDriverExtra);
        HDC context = NULL;

        /* The unprinted margins, read from an information context of the
         * printer set to this paper upright. */
        if (one != NULL) {
            memcpy(one, mode, (size_t)mode->dmSize + mode->dmDriverExtra);
            one->dmPaperSize = (short)papers.ids[i];
            one->dmOrientation = DMORIENT_PORTRAIT;
            one->dmFields |= DM_PAPERSIZE | DM_ORIENTATION;
            context = CreateICW(L"WINSPOOL", info->pPrinterName, NULL, one);
        }
        if (context != NULL) {
            int dpi_x = GetDeviceCaps(context, LOGPIXELSX);
            int dpi_y = GetDeviceCaps(context, LOGPIXELSY);
            int left = GetDeviceCaps(context, PHYSICALOFFSETX);
            int top = GetDeviceCaps(context, PHYSICALOFFSETY);
            int right = GetDeviceCaps(context, PHYSICALWIDTH) - GetDeviceCaps(context, HORZRES) - left;
            int bottom = GetDeviceCaps(context, PHYSICALHEIGHT) - GetDeviceCaps(context, VERTRES) - top;

            if (dpi_x > 0 && dpi_y > 0) {
                margins[0] = (int)((int64_t)left * 2540 / dpi_x);
                margins[1] = (int)((int64_t)(right < 0 ? 0 : right) * 2540 / dpi_x);
                margins[2] = (int)((int64_t)top * 2540 / dpi_y);
                margins[3] = (int)((int64_t)(bottom < 0 ? 0 : bottom) * 2540 / dpi_y);
            }
            DeleteDC(context);
        }
        hero_layer_free(one);
        hero_printing_add(&list, papers.pwg[i]);
        hero_printing_add_wide(&list, papers.names[i]);
        hero_printing_add_number(&list, (int64_t)papers.sizes[i].x * 10);
        hero_printing_add_number(&list, (int64_t)papers.sizes[i].y * 10);
        hero_printing_add_number(&list, margins[0]);
        hero_printing_add_number(&list, margins[1]);
        hero_printing_add_number(&list, margins[2]);
        hero_printing_add_number(&list, margins[3]);
        hero_printing_add(&list, (mode->dmFields & DM_PAPERSIZE) != 0 && mode->dmPaperSize == (short)papers.ids[i] ? "1" : "0");
    }
    hero_printing_papers_free(&papers);
    hero_layer_free(mode);
    hero_layer_free(info);
    ClosePrinter(handle);
    return hero_printing_hand(&list, buffer, count);
}

int64_t hero_printing_capabilities(const char *printer, void *buffer, uint64_t count) {
    hero_printing_list list = {0};
    HANDLE handle;
    PRINTER_INFO_2W *info;
    int copies;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count)) {
        return -1;
    }
    handle = hero_printing_open(printer);
    if (handle == NULL) {
        return -1;
    }
    info = (PRINTER_INFO_2W *)hero_printing_ask(hero_printing_ask_printer, handle, "the printer could not be read");
    ClosePrinter(handle);
    if (info == NULL) {
        return -1;
    }
    copies = DeviceCapabilitiesW(info->pPrinterName, info->pPortName, DC_COPIES, NULL, NULL);
    hero_printing_add(&list, DeviceCapabilitiesW(info->pPrinterName, info->pPortName, DC_DUPLEX, NULL, NULL) == 1 ? "1" : "0");
    hero_printing_add(&list, DeviceCapabilitiesW(info->pPrinterName, info->pPortName, DC_COLORDEVICE, NULL, NULL) == 1 ? "1" : "0");
    hero_printing_add_number(&list, copies < 1 ? 1 : copies > HERO_PRINTING_COPIES_MOST ? HERO_PRINTING_COPIES_MOST : copies);
    hero_layer_free(info);
    return hero_printing_hand(&list, buffer, count);
}

/* THE JOBS THIS PROCESS SUBMITTED, so a job the spooler has forgotten is
 * still told by how it ended: the spooler drops a job from its queue once it
 * has printed, been deleted or failed, and answers no state for it then.
 * A ring of the last `HERO_PRINTING_KEPT` jobs, each its printer's name's
 * hash, its number, the last state seen and whether this process cancelled
 * it, under a slim lock: a job submitted on one thread is asked about on
 * another. */
#define HERO_PRINTING_KEPT 256

typedef struct {
    uint64_t printer;
    int64_t job;
    int last;
    bool cancelled;
} hero_printing_kept_job;

static hero_printing_kept_job hero_printing_kept[HERO_PRINTING_KEPT];
static size_t hero_printing_kept_next;
static SRWLOCK hero_printing_kept_lock = SRWLOCK_INIT;

/* FNV-1a over the name's bytes: two printers' jobs of one number kept
 * apart. */
static uint64_t hero_printing_hash(const char *s) {
    uint64_t h = 14695981039346656037ull;

    for (; *s != '\0'; s++) {
        h = (h ^ (unsigned char)*s) * 1099511628211ull;
    }
    return h;
}

/* Remembers job `job` of `printer`, or updates what was kept: `last` its
 * state where it is more than 0, and its cancellation. Answers the kept
 * entry's copy through `out` where it is not NULL. */
static bool hero_printing_keep(const char *printer, int64_t job, int last, bool cancelled, bool add, hero_printing_kept_job *out) {
    uint64_t h = hero_printing_hash(printer);
    bool found = false;

    AcquireSRWLockExclusive(&hero_printing_kept_lock);
    for (size_t i = 0; i < HERO_PRINTING_KEPT && !found; i++) {
        hero_printing_kept_job *k = &hero_printing_kept[i];

        if (k->job == job && k->printer == h) {
            found = true;
            k->last = last > 0 ? last : k->last;
            k->cancelled = k->cancelled || cancelled;
            if (out != NULL) {
                *out = *k;
            }
        }
    }
    if (!found && add) {
        hero_printing_kept_job *k = &hero_printing_kept[hero_printing_kept_next];

        k->printer = h;
        k->job = job;
        k->last = last;
        k->cancelled = cancelled;
        hero_printing_kept_next = (hero_printing_kept_next + 1) % HERO_PRINTING_KEPT;
        found = true;
        if (out != NULL) {
            *out = *k;
        }
    }
    ReleaseSRWLockExclusive(&hero_printing_kept_lock);
    return found;
}

/* IPP's state number of a job the spooler still holds, by its status bits. */
static int hero_printing_win_job_state(DWORD status) {
    if ((status & (JOB_STATUS_DELETING | JOB_STATUS_DELETED)) != 0) {
        return 7;
    }
    if ((status & (JOB_STATUS_PRINTED | JOB_STATUS_COMPLETE)) != 0) {
        return 9;
    }
    if ((status & (JOB_STATUS_ERROR | JOB_STATUS_OFFLINE | JOB_STATUS_PAPEROUT | JOB_STATUS_USER_INTERVENTION | JOB_STATUS_BLOCKED_DEVQ)) != 0) {
        return 6;
    }
    if ((status & JOB_STATUS_PAUSED) != 0) {
        return 4;
    }
    if ((status & (JOB_STATUS_PRINTING | JOB_STATUS_SPOOLING)) != 0) {
        return 5;
    }
    return 3;
}

typedef struct {
    HANDLE handle;
    DWORD job;
} hero_printing_job_ask;

static BOOL hero_printing_ask_job(void *context, BYTE *block, DWORD size, DWORD *needed) {
    hero_printing_job_ask *ask = context;

    return GetJobW(ask->handle, ask->job, 1, block, size, needed);
}

static int64_t hero_printing_job_state(const char *printer, int64_t job, int *state, hero_printing_list *fields) {
    hero_printing_job_ask ask;
    hero_printing_kept_job kept;
    JOB_INFO_1W *info;
    DWORD needed = 0;

    if (job < 1 || job > (int64_t)UINT32_MAX) {
        return hero_printing_fail(HERO_PRINTING_NOT_FOUND, 0, "no job %lld stands on printer \"%s\"", (long long)job, printer);
    }
    ask.handle = hero_printing_open(printer);
    ask.job = (DWORD)job;
    if (ask.handle == NULL) {
        return -1;
    }
    if (!GetJobW(ask.handle, ask.job, 1, NULL, 0, &needed) && GetLastError() == ERROR_INVALID_PARAMETER) {
        ClosePrinter(ask.handle);
        /* Gone from the queue: told by what this process saw of it. */
        if (!hero_printing_keep(printer, job, 0, false, false, &kept)) {
            return hero_printing_fail(HERO_PRINTING_NOT_FOUND, ERROR_INVALID_PARAMETER, "no job %lld stands on printer \"%s\"", (long long)job, printer);
        }
        *state = kept.cancelled ? 7 : kept.last == 6 ? 8 : 9;
        if (fields != NULL) {
            hero_printing_add(fields, hero_printing_job_word(*state));
            hero_printing_add(fields, kept.cancelled ? "job-canceled-by-user" : kept.last == 6 ? "aborted-by-system" : "job-completed-successfully");
            hero_printing_add(fields, "");
            hero_printing_add_number(fields, 0);
        }
        return 0;
    }
    info = (JOB_INFO_1W *)hero_printing_ask(hero_printing_ask_job, &ask, "the job could not be read");
    ClosePrinter(ask.handle);
    if (info == NULL) {
        return -1;
    }
    *state = hero_printing_win_job_state(info->Status);
    (void)hero_printing_keep(printer, job, *state, false, false, NULL);
    if (fields != NULL) {
        hero_printing_add(fields, hero_printing_job_word(*state));
        hero_printing_add(fields, *state == 6 ? "job-stopped" : *state == 5 ? "job-printing" : "");
        hero_printing_add_wide(fields, info->pStatus);
        hero_printing_add_number(fields, (int64_t)info->PagesPrinted);
    }
    hero_layer_free(info);
    return 0;
}

typedef struct {
    HANDLE handle;
    DWORD *returned;
} hero_printing_jobs_ask;

static BOOL hero_printing_ask_jobs(void *context, BYTE *block, DWORD size, DWORD *needed) {
    hero_printing_jobs_ask *ask = context;

    return EnumJobsW(ask->handle, 0, 0xFFFFFFFFu, 1, block, size, needed, ask->returned);
}

int64_t hero_printing_jobs(const char *printer, void *buffer, uint64_t count) {
    hero_printing_list list = {0};
    hero_printing_jobs_ask ask;
    DWORD returned = 0;
    BYTE *block;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count)) {
        return -1;
    }
    ask.handle = hero_printing_open(printer);
    ask.returned = &returned;
    if (ask.handle == NULL) {
        return -1;
    }
    block = hero_printing_ask(hero_printing_ask_jobs, &ask, "the printer's jobs could not be listed");
    ClosePrinter(ask.handle);
    if (block == NULL) {
        return -1;
    }
    for (DWORD i = 0; i < returned; i++) {
        const JOB_INFO_1W *j = &((const JOB_INFO_1W *)block)[i];

        if (!hero_printing_ended(hero_printing_win_job_state(j->Status))) {
            hero_printing_add_number(&list, (int64_t)j->JobId);
        }
    }
    hero_layer_free(block);
    return hero_printing_hand(&list, buffer, count);
}

int64_t hero_printing_cancel(const char *printer, int64_t job) {
    HANDLE handle;
    int state = 0;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false)) {
        return -1;
    }
    if (hero_printing_job_state(printer, job, &state, NULL) < 0) {
        return -1;
    }
    if (hero_printing_ended(state)) {
        return 0;
    }
    handle = hero_printing_open(printer);
    if (handle == NULL) {
        return -1;
    }
    if (!SetJobW(handle, (DWORD)job, 0, NULL, JOB_CONTROL_DELETE)) {
        DWORD code = GetLastError();

        ClosePrinter(handle);
        return hero_printing_win_fail(code == ERROR_INVALID_PARAMETER ? HERO_PRINTING_NOT_FOUND : hero_printing_win_kind(code), code, "the job could not be cancelled");
    }
    ClosePrinter(handle);
    (void)hero_printing_keep(printer, job, 7, true, true, NULL);
    return 0;
}

/* ---- The document: Windows.Data.Pdf through Direct2D's print control ----
 *
 * COM and WinRT interfaces are reached by their vtables' slots, each read
 * from the Windows SDK's own C declarations (the D2D ones from MinGW's
 * `d2d1_1.h`, whose C vtables the SDK's header does not write), so this file
 * includes no C++ header. A slot is the function's index in the vtable,
 * `IUnknown`'s three first, `IInspectable`'s three after them. */

typedef void (STDMETHODCALLTYPE *hero_slot)(void);
#define HERO_SLOT(object, n) ((*(hero_slot **)(object))[n])

static void hero_printing_release(void *object) {
    if (object != NULL) {
        ((ULONG(STDMETHODCALLTYPE *)(void *))HERO_SLOT(object, 2))(object);
    }
}

static HRESULT hero_printing_query(void *object, const GUID *iid, void **out) {
    return ((HRESULT(STDMETHODCALLTYPE *)(void *, const GUID *, void **))HERO_SLOT(object, 0))(object, iid, out);
}

static const GUID hero_iid_unknown = {0x00000000, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const GUID hero_iid_dispatch = {0x00020400, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const GUID hero_iid_async_info = {0x00000036, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const GUID hero_iid_random_access_stream = {0x905a0fe1, 0xbc53, 0x11df, {0x8c, 0x49, 0x00, 0x1e, 0x4f, 0xc6, 0x86, 0xda}};
static const GUID hero_iid_pdf_document_statics = {0x433a0b5f, 0xc007, 0x4788, {0x90, 0xf2, 0x08, 0x14, 0x3d, 0x92, 0x25, 0x99}};
static const GUID hero_iid_dxgi_device = {0x54ec77fa, 0x1377, 0x44e6, {0x8c, 0x32, 0x88, 0xfd, 0x5f, 0x44, 0xc8, 0x4c}};
static const GUID hero_iid_d2d_factory1 = {0xbb12d362, 0xdaee, 0x4b9a, {0xaa, 0x1d, 0x14, 0xba, 0x40, 0x1c, 0xfa, 0x1f}};
static const GUID hero_clsid_wic_factory = {0xcacaf262, 0x9370, 0x4615, {0xa1, 0x3b, 0x9f, 0x55, 0x39, 0xda, 0x4c, 0x0a}};
static const GUID hero_iid_wic_factory = {0xec5ec8a9, 0xc395, 0x4314, {0x9c, 0x77, 0x54, 0xd7, 0xa9, 0x35, 0xff, 0x70}};
static const GUID hero_clsid_package_factory = {0x348ef17d, 0x6c81, 0x4982, {0x92, 0xb4, 0xee, 0x18, 0x8a, 0x43, 0x86, 0x7a}};
static const GUID hero_iid_package_factory = {0xd2959bf7, 0xb31b, 0x4a3d, {0x96, 0x00, 0x71, 0x2e, 0xb1, 0x33, 0x5b, 0xa4}};
static const GUID hero_iid_package_status = {0xed90c8ad, 0x5c34, 0x4d05, {0xa1, 0xec, 0x0e, 0x8a, 0x9b, 0x3a, 0xd7, 0xaf}};
static const GUID hero_iid_connection_points = {0xb196b284, 0xbab4, 0x101a, {0xb6, 0x9c, 0x00, 0xaa, 0x00, 0x34, 0x1d, 0x07}};

/* Direct2D's and the renderer's structures, as their headers lay them out. */
typedef struct {
    FLOAT width;
    FLOAT height;
} hero_d2d_size;

typedef struct {
    FLOAT m11, m12, m21, m22, dx, dy;
} hero_d2d_matrix;

typedef struct {
    FLOAT left, top, right, bottom;
} hero_d2d_rect;

typedef struct {
    FLOAT r, g, b, a;
} hero_d2d_color;

typedef struct {
    UINT32 font_subset;
    FLOAT raster_dpi;
    UINT32 color_space;
} hero_d2d_print_properties;

typedef struct {
    hero_d2d_rect source;
    UINT32 width;
    UINT32 height;
    hero_d2d_color background;
    BOOLEAN ignore_high_contrast;
} hero_pdf_render_params;

/* The functions found with `GetProcAddress`, once (`hero_printing_render_once`):
 * the WinRT runtime's, the shell's stream over a stream, Direct3D's,
 * Direct2D's, the PDF renderer's and the print ticket's. */
typedef struct {
    HRESULT(WINAPI *ro_initialize)(int kind);
    void(WINAPI *ro_uninitialize)(void);
    HRESULT(WINAPI *ro_get_factory)(void *class_id, const GUID *iid, void **factory);
    HRESULT(WINAPI *string_create)(const WCHAR *text, UINT32 length, void **out);
    HRESULT(WINAPI *string_delete)(void *string);
    HRESULT(WINAPI *stream_over_stream)(IStream *stream, int options, const GUID *iid, void **out);
    HRESULT(WINAPI *d3d_create)(void *adapter, int driver, HMODULE software, UINT flags, const void *levels, UINT level_count, UINT sdk, void **device, void *level, void **context);
    HRESULT(WINAPI *d2d_create)(int kind, const GUID *iid, const void *options, void **factory);
    HRESULT(WINAPI *pdf_renderer)(void *dxgi, void **renderer);
    HRESULT(WINAPI *pt_open)(const WCHAR *printer, DWORD version, HANDLE *provider);
    HRESULT(WINAPI *pt_close)(HANDLE provider);
    HRESULT(WINAPI *pt_from_devmode)(HANDLE provider, ULONG size, DEVMODEW *mode, int scope, IStream *ticket);
} hero_printing_render_functions;

/* Found once inside `InitOnceExecuteOnce` and only read after it, as
 * `hero_keyring.c`'s functions are: the functions are the process's. */
static hero_printing_render_functions hero_printing_render;
static bool hero_printing_render_found;
static INIT_ONCE hero_printing_render_once = INIT_ONCE_STATIC_INIT;

static FARPROC hero_printing_find(const WCHAR *library, const char *name) {
    HMODULE module = LoadLibraryExW(library, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);

    return module == NULL ? NULL : GetProcAddress(module, name);
}

#define HERO_PRINTING_FIND(field, library, name)                                                       \
    do {                                                                                              \
        FARPROC found = hero_printing_find(library, name);                                            \
        ok = ok && found != NULL;                                                                     \
        if (found != NULL) {                                                                          \
            memcpy(&hero_printing_render.field, &found, sizeof hero_printing_render.field);          \
        }                                                                                             \
    } while (0)

static BOOL CALLBACK hero_printing_render_load(PINIT_ONCE once, void *parameter, void **context) {
    bool ok = true;

    (void)once;
    (void)parameter;
    (void)context;
    HERO_PRINTING_FIND(ro_initialize, L"combase.dll", "RoInitialize");
    HERO_PRINTING_FIND(ro_uninitialize, L"combase.dll", "RoUninitialize");
    HERO_PRINTING_FIND(ro_get_factory, L"combase.dll", "RoGetActivationFactory");
    HERO_PRINTING_FIND(string_create, L"combase.dll", "WindowsCreateString");
    HERO_PRINTING_FIND(string_delete, L"combase.dll", "WindowsDeleteString");
    HERO_PRINTING_FIND(stream_over_stream, L"shcore.dll", "CreateRandomAccessStreamOverStream");
    HERO_PRINTING_FIND(d3d_create, L"d3d11.dll", "D3D11CreateDevice");
    HERO_PRINTING_FIND(d2d_create, L"d2d1.dll", "D2D1CreateFactory");
    HERO_PRINTING_FIND(pdf_renderer, L"Windows.Data.Pdf.dll", "PdfCreateRenderer");
    HERO_PRINTING_FIND(pt_open, L"prntvpt.dll", "PTOpenProvider");
    HERO_PRINTING_FIND(pt_close, L"prntvpt.dll", "PTCloseProvider");
    HERO_PRINTING_FIND(pt_from_devmode, L"prntvpt.dll", "PTConvertDevModeToPrintTicket");
    hero_printing_render_found = ok;
    return TRUE;
}

/* THE STATUS SINK: the package target tells the job's number and how the
 * package went through `IPrintDocumentPackageStatusEvent`, an `IDispatch`
 * this file implements with the one method the target calls. Its fields are
 * written by the target's thread and read by the submitting one, so they are
 * interlocked. */
typedef struct hero_printing_sink_table hero_printing_sink_table;

typedef struct {
    const hero_printing_sink_table *vtable;
    volatile LONG references;
    volatile LONG job;
    volatile LONG completion;
    volatile LONG status;
} hero_printing_sink;

static HRESULT STDMETHODCALLTYPE hero_sink_query(void *self, const GUID *iid, void **out) {
    if (IsEqualGUID(iid, &hero_iid_unknown) || IsEqualGUID(iid, &hero_iid_dispatch) || IsEqualGUID(iid, &hero_iid_package_status)) {
        *out = self;
        InterlockedIncrement(&((hero_printing_sink *)self)->references);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE hero_sink_add(void *self) {
    return (ULONG)InterlockedIncrement(&((hero_printing_sink *)self)->references);
}

static ULONG STDMETHODCALLTYPE hero_sink_release(void *self) {
    return (ULONG)InterlockedDecrement(&((hero_printing_sink *)self)->references);
}

static HRESULT STDMETHODCALLTYPE hero_sink_type_count(void *self, UINT *count) {
    (void)self;
    *count = 0;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_sink_type_info(void *self, UINT index, LCID locale, void **info) {
    (void)self;
    (void)index;
    (void)locale;
    *info = NULL;
    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE hero_sink_ids(void *self, const GUID *iid, LPOLESTR *names, UINT count, LCID locale, DISPID *ids) {
    (void)self;
    (void)iid;
    (void)names;
    (void)count;
    (void)locale;
    (void)ids;
    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE hero_sink_invoke(void *self, DISPID id, const GUID *iid, LCID locale, WORD flags, void *params, void *result, void *exception, UINT *argument) {
    (void)self;
    (void)id;
    (void)iid;
    (void)locale;
    (void)flags;
    (void)params;
    (void)result;
    (void)exception;
    (void)argument;
    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE hero_sink_updated(void *self, PrintDocumentPackageStatus *status) {
    hero_printing_sink *sink = self;

    if (status != NULL) {
        if (status->JobId != 0) {
            InterlockedExchange(&sink->job, (LONG)status->JobId);
        }
        InterlockedExchange(&sink->status, (LONG)status->PackageStatus);
        InterlockedExchange(&sink->completion, (LONG)status->Completion);
    }
    return S_OK;
}

struct hero_printing_sink_table {
    HRESULT(STDMETHODCALLTYPE *query)(void *self, const GUID *iid, void **out);
    ULONG(STDMETHODCALLTYPE *add)(void *self);
    ULONG(STDMETHODCALLTYPE *release)(void *self);
    HRESULT(STDMETHODCALLTYPE *type_count)(void *self, UINT *count);
    HRESULT(STDMETHODCALLTYPE *type_info)(void *self, UINT index, LCID locale, void **info);
    HRESULT(STDMETHODCALLTYPE *ids)(void *self, const GUID *iid, LPOLESTR *names, UINT count, LCID locale, DISPID *ids);
    HRESULT(STDMETHODCALLTYPE *invoke)(void *self, DISPID id, const GUID *iid, LCID locale, WORD flags, void *params, void *result, void *exception, UINT *argument);
    HRESULT(STDMETHODCALLTYPE *updated)(void *self, PrintDocumentPackageStatus *status);
};

static const hero_printing_sink_table hero_sink_vtable = {
    hero_sink_query, hero_sink_add, hero_sink_release, hero_sink_type_count, hero_sink_type_info, hero_sink_ids, hero_sink_invoke, hero_sink_updated,
};

/* What one submission hands its worker thread, and what the worker answers. */
typedef struct {
    WCHAR printer[HERO_PRINTING_NAME_MOST + 1];
    WCHAR title[HERO_PRINTING_NAME_MOST + 1];
    const BYTE *pdf;
    UINT size;
    IStream *ticket;
    hero_d2d_size paper;
    int64_t orientation;
    const int64_t *ranges;
    uint64_t range_count;
    int64_t job;
    int64_t kind;
    HRESULT failure;
    char what[160];
} hero_printing_render_work;

static bool hero_printing_render_failed(hero_printing_render_work *work, HRESULT hr, int64_t kind, const char *what) {
    work->failure = hr;
    work->kind = kind;
    snprintf(work->what, sizeof work->what, "%s", what);
    return false;
}

/* Waits for a WinRT operation to end, then answers its result through
 * `IAsyncOperation<T>::GetResults` (slot 8). */
static HRESULT hero_printing_await(void *operation, void **result) {
    void *info = NULL;
    HRESULT hr = hero_printing_query(operation, &hero_iid_async_info, &info);
    int status = 0;
    DWORD waited = 0;

    if (FAILED(hr)) {
        return hr;
    }
    for (;;) {
        /* IAsyncInfo::get_Status, slot 7: 0 started, 1 completed, 2
         * cancelled, 3 failed. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, int *))HERO_SLOT(info, 7))(info, &status);
        if (FAILED(hr) || status != 0 || waited >= 120000) {
            break;
        }
        Sleep(2);
        waited += 2;
    }
    if (SUCCEEDED(hr) && status == 3) {
        HRESULT code = E_FAIL;

        /* IAsyncInfo::get_ErrorCode, slot 8. */
        (void)((HRESULT(STDMETHODCALLTYPE *)(void *, HRESULT *))HERO_SLOT(info, 8))(info, &code);
        hr = FAILED(code) ? code : E_FAIL;
    } else if (SUCCEEDED(hr) && status != 1) {
        hr = status == 0 ? HRESULT_FROM_WIN32(ERROR_TIMEOUT) : E_ABORT;
    }
    hero_printing_release(info);
    if (FAILED(hr)) {
        return hr;
    }
    return ((HRESULT(STDMETHODCALLTYPE *)(void *, void **))HERO_SLOT(operation, 8))(operation, result);
}

/* Whether page `number`, from 1, is one the ranges name; every page where
 * they name none. */
static bool hero_printing_page_named(const hero_printing_render_work *work, int64_t number) {
    if (work->range_count == 0) {
        return true;
    }
    for (uint64_t i = 0; i < work->range_count; i += 2) {
        if (number >= work->ranges[i] && number <= work->ranges[i + 1]) {
            return true;
        }
    }
    return false;
}

/* Loads the document, opens the job and draws its pages, on the worker's own
 * multithreaded apartment. Every object is given back on every path. */
static bool hero_printing_render_job(hero_printing_render_work *work) {
    const hero_printing_render_functions *f = &hero_printing_render;
    IStream *memory = NULL;
    void *stream = NULL, *class_name = NULL, *statics = NULL, *operation = NULL, *document = NULL;
    void *d3d = NULL, *dxgi = NULL, *factory = NULL, *device = NULL, *context = NULL, *wic = NULL;
    void *package_factory = NULL, *target = NULL, *points = NULL, *point = NULL, *control = NULL, *renderer = NULL;
    hero_printing_sink sink = {&hero_sink_vtable, 1, 0, 0, 0};
    DWORD cookie = 0;
    UINT32 pages = 0, drawn = 0;
    bool done = false, cancel = false;
    HRESULT hr;

    memory = SHCreateMemStream(work->pdf, work->size);
    if (memory == NULL) {
        return hero_printing_render_failed(work, E_OUTOFMEMORY, HERO_PRINTING_LIMIT, "the document could not be held");
    }
    hr = f->stream_over_stream(memory, 0, &hero_iid_random_access_stream, &stream);
    if (SUCCEEDED(hr)) {
        static const WCHAR pdf_class[] = L"Windows.Data.Pdf.PdfDocument";

        hr = f->string_create(pdf_class, (UINT32)(sizeof pdf_class / sizeof pdf_class[0] - 1), &class_name);
    }
    if (SUCCEEDED(hr)) {
        hr = f->ro_get_factory(class_name, &hero_iid_pdf_document_statics, &statics);
    }
    if (SUCCEEDED(hr)) {
        /* IPdfDocumentStatics::LoadFromStreamAsync, slot 8. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, void **))HERO_SLOT(statics, 8))(statics, stream, &operation);
    }
    if (SUCCEEDED(hr)) {
        hr = hero_printing_await(operation, &document);
        if (FAILED(hr)) {
            hero_printing_render_failed(work, hr, HERO_PRINTING_INVALID, "the system's PDF renderer refused the document");
            goto out;
        }
    }
    if (SUCCEEDED(hr)) {
        /* IPdfDocument::get_PageCount, slot 7. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, UINT32 *))HERO_SLOT(document, 7))(document, &pages);
    }
    if (FAILED(hr)) {
        hero_printing_render_failed(work, hr, hero_printing_hr_kind(hr), "the system's PDF renderer could not open the document");
        goto out;
    }
    for (UINT32 i = 0; i < pages; i++) {
        drawn += hero_printing_page_named(work, (int64_t)i + 1) ? 1 : 0;
    }
    if (drawn == 0) {
        hero_printing_render_failed(work, S_OK, HERO_PRINTING_INVALID, "the page ranges name no page of the document");
        goto out;
    }
    /* Direct3D's device, WARP where the machine has no adapter, as a server
     * often has none; Direct2D's device over it. */
    hr = f->d3d_create(NULL, 1, NULL, 0x20, NULL, 0, 7, &d3d, NULL, NULL);
    if (FAILED(hr)) {
        hr = f->d3d_create(NULL, 5, NULL, 0x20, NULL, 0, 7, &d3d, NULL, NULL);
    }
    if (SUCCEEDED(hr)) {
        hr = hero_printing_query(d3d, &hero_iid_dxgi_device, &dxgi);
    }
    if (SUCCEEDED(hr)) {
        hr = f->d2d_create(1, &hero_iid_d2d_factory1, NULL, &factory);
    }
    if (SUCCEEDED(hr)) {
        /* ID2D1Factory1::CreateDevice, slot 17. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, void **))HERO_SLOT(factory, 17))(factory, dxgi, &device);
    }
    if (SUCCEEDED(hr)) {
        /* ID2D1Device::CreateDeviceContext, slot 4. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, int, void **))HERO_SLOT(device, 4))(device, 0, &context);
    }
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(&hero_clsid_wic_factory, NULL, CLSCTX_INPROC_SERVER, &hero_iid_wic_factory, &wic);
    }
    if (SUCCEEDED(hr)) {
        hr = f->pdf_renderer(dxgi, &renderer);
    }
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(&hero_clsid_package_factory, NULL, CLSCTX_INPROC_SERVER, &hero_iid_package_factory, &package_factory);
    }
    if (FAILED(hr)) {
        hero_printing_render_failed(work, hr, hero_printing_hr_kind(hr), "the system's drawing for printing could not be made");
        goto out;
    }
    /* IPrintDocumentPackageTargetFactory::CreateDocumentPackageTargetForPrintJob,
     * slot 3: from here a job stands, cancelled on every failing path. */
    hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, const WCHAR *, const WCHAR *, IStream *, IStream *, void **))HERO_SLOT(package_factory, 3))(package_factory, work->printer, work->title, NULL, work->ticket, &target);
    if (FAILED(hr)) {
        hero_printing_render_failed(work, hr, hero_printing_hr_kind(hr), "the spooler did not open the job");
        goto out;
    }
    cancel = true;
    if (SUCCEEDED(hero_printing_query(target, &hero_iid_connection_points, &points))) {
        /* IConnectionPointContainer::FindConnectionPoint, slot 4;
         * IConnectionPoint::Advise, slot 5. */
        if (SUCCEEDED(((HRESULT(STDMETHODCALLTYPE *)(void *, const GUID *, void **))HERO_SLOT(points, 4))(points, &hero_iid_package_status, &point))) {
            (void)((HRESULT(STDMETHODCALLTYPE *)(void *, void *, DWORD *))HERO_SLOT(point, 5))(point, &sink, &cookie);
        }
    }
    {
        hero_d2d_print_properties properties = {0, 600.0f, 0};

        /* ID2D1Device::CreatePrintControl, slot 5. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, void *, const hero_d2d_print_properties *, void **))HERO_SLOT(device, 5))(device, wic, target, &properties, &control);
    }
    for (UINT32 i = 0; SUCCEEDED(hr) && i < pages; i++) {
        void *page = NULL, *commands = NULL;
        hero_d2d_size size = {0, 0};
        hero_pdf_render_params params = {{0, 0, 0, 0}, 0, 0, {1.0f, 1.0f, 1.0f, 1.0f}, TRUE};
        hero_d2d_matrix place;
        FLOAT scale = 1.0f;
        bool reverse = work->orientation == HERO_PRINTING_REVERSE_LANDSCAPE || work->orientation == HERO_PRINTING_REVERSE_PORTRAIT;

        if (!hero_printing_page_named(work, (int64_t)i + 1)) {
            continue;
        }
        /* IPdfDocument::GetPage, slot 6; IPdfPage::get_Size, slot 10, in
         * device-independent pixels, 96 to the inch. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, UINT32, void **))HERO_SLOT(document, 6))(document, i, &page);
        if (SUCCEEDED(hr)) {
            hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, hero_d2d_size *))HERO_SLOT(page, 10))(page, &size);
        }
        if (SUCCEEDED(hr)) {
            /* ID2D1DeviceContext::CreateCommandList, slot 67; SetTarget, 74;
             * BeginDraw, 48. */
            hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void **))HERO_SLOT(context, 67))(context, &commands);
        }
        if (SUCCEEDED(hr)) {
            ((void(STDMETHODCALLTYPE *)(void *, void *))HERO_SLOT(context, 74))(context, commands);
            ((void(STDMETHODCALLTYPE *)(void *))HERO_SLOT(context, 48))(context);
            /* The page fitted to the paper where it is larger, as CUPS's
             * `print-scaling` of `auto` fits it, centred, and turned half a
             * turn for a reverse orientation. SetTransform, slot 30. */
            if (size.width > 0 && size.height > 0 && (size.width > work->paper.width || size.height > work->paper.height)) {
                FLOAT across = work->paper.width / size.width;
                FLOAT down = work->paper.height / size.height;

                scale = across < down ? across : down;
            }
            place.m11 = reverse ? -scale : scale;
            place.m12 = 0;
            place.m21 = 0;
            place.m22 = reverse ? -scale : scale;
            place.dx = reverse ? (work->paper.width + size.width * scale) / 2 : (work->paper.width - size.width * scale) / 2;
            place.dy = reverse ? (work->paper.height + size.height * scale) / 2 : (work->paper.height - size.height * scale) / 2;
            ((void(STDMETHODCALLTYPE *)(void *, const hero_d2d_matrix *))HERO_SLOT(context, 30))(context, &place);
            /* IPdfRendererNative::RenderPageToDeviceContext, slot 4. */
            hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, void *, hero_pdf_render_params *))HERO_SLOT(renderer, 4))(renderer, page, context, &params);
            /* EndDraw, slot 49, always after a BeginDraw. */
            {
                HRESULT ended = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, void *))HERO_SLOT(context, 49))(context, NULL, NULL);

                hr = FAILED(hr) ? hr : ended;
            }
            ((void(STDMETHODCALLTYPE *)(void *, void *))HERO_SLOT(context, 74))(context, NULL);
        }
        if (SUCCEEDED(hr)) {
            /* ID2D1CommandList::Close, slot 5; ID2D1PrintControl::AddPage,
             * slot 3. */
            hr = ((HRESULT(STDMETHODCALLTYPE *)(void *))HERO_SLOT(commands, 5))(commands);
        }
        if (SUCCEEDED(hr)) {
            hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, hero_d2d_size, IStream *, void *, void *))HERO_SLOT(control, 3))(control, commands, work->paper, NULL, NULL, NULL);
        }
        hero_printing_release(commands);
        hero_printing_release(page);
    }
    if (SUCCEEDED(hr)) {
        /* ID2D1PrintControl::Close, slot 4: the package is whole. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *))HERO_SLOT(control, 4))(control);
    }
    if (FAILED(hr)) {
        hero_printing_render_failed(work, hr, hero_printing_hr_kind(hr), "the document's pages could not be drawn for the printer");
        goto out;
    }
    /* The job's number arrives by the status event; the package is the
     * spooler's once closed, so it is waited for at most ten seconds. */
    for (DWORD waited = 0; InterlockedCompareExchange(&sink.job, 0, 0) == 0 && waited < 10000; waited += 10) {
        if (InterlockedCompareExchange(&sink.completion, 0, 0) >= 2) {
            break;
        }
        Sleep(10);
    }
    work->job = InterlockedCompareExchange(&sink.job, 0, 0);
    if (work->job == 0) {
        LONG status = InterlockedCompareExchange(&sink.status, 0, 0);

        hero_printing_render_failed(work, FAILED(status) ? status : E_FAIL, HERO_PRINTING_OTHER, "the spooler told no number for the job");
        goto out;
    }
    cancel = false;
    done = true;
out:
    if (cancel && target != NULL) {
        /* IPrintDocumentPackageTarget::Cancel, slot 5. */
        (void)((HRESULT(STDMETHODCALLTYPE *)(void *))HERO_SLOT(target, 5))(target);
    }
    if (point != NULL && cookie != 0) {
        /* IConnectionPoint::Unadvise, slot 6. */
        (void)((HRESULT(STDMETHODCALLTYPE *)(void *, DWORD))HERO_SLOT(point, 6))(point, cookie);
    }
    hero_printing_release(control);
    hero_printing_release(point);
    hero_printing_release(points);
    hero_printing_release(target);
    hero_printing_release(package_factory);
    hero_printing_release(renderer);
    hero_printing_release(wic);
    hero_printing_release(context);
    hero_printing_release(device);
    hero_printing_release(factory);
    hero_printing_release(dxgi);
    hero_printing_release(d3d);
    hero_printing_release(document);
    hero_printing_release(operation);
    hero_printing_release(statics);
    if (class_name != NULL) {
        (void)f->string_delete(class_name);
    }
    hero_printing_release(stream);
    if (memory != NULL) {
        memory->lpVtbl->Release(memory);
    }
    if (!done && work->kind == HERO_PRINTING_OK) {
        hero_printing_render_failed(work, FAILED(hr) ? hr : E_FAIL, hero_printing_hr_kind(hr), "the document could not be printed");
    }
    return done;
}

/* The worker: a thread of its own, so the job's COM objects live in a
 * multithreaded apartment whatever the calling thread's apartment is (a
 * window's thread is single-threaded, and a WinRT operation awaited there
 * by polling would wait on itself). */
static DWORD WINAPI hero_printing_render_thread(void *parameter) {
    hero_printing_render_work *work = parameter;
    HRESULT hr = hero_printing_render.ro_initialize(1);

    if (FAILED(hr)) {
        hero_printing_render_failed(work, hr, HERO_PRINTING_OTHER, "the printing thread's apartment could not be entered");
        return 0;
    }
    (void)hero_printing_render_job(work);
    hero_printing_render.ro_uninitialize();
    return 0;
}

/* The DEVMODE `mode` changed to the job's settings, checked against what
 * the driver says the printer can do; false with the failure recorded. */
static bool hero_printing_settings_into(DEVMODEW *mode, const WCHAR *name, const WCHAR *port, const hero_printing_papers *papers, int64_t copies, const char *media, int64_t orientation, int64_t sides, int64_t color, int64_t collate) {
    if (copies > 1) {
        int most = DeviceCapabilitiesW(name, port, DC_COPIES, NULL, NULL);

        if (most >= 0 && copies > most) {
            hero_printing_fail(HERO_PRINTING_UNSUPPORTED, 0, "%lld copies were asked, and the printer makes at most %d", (long long)copies, most);
            return false;
        }
    }
    mode->dmCopies = (short)copies;
    mode->dmCollate = collate == 1 ? DMCOLLATE_TRUE : DMCOLLATE_FALSE;
    mode->dmFields |= DM_COPIES | DM_COLLATE;
    if (media[0] != '\0') {
        int found = -1;

        for (int i = 0; i < papers->count && found < 0; i++) {
            found = strcmp(papers->pwg[i], media) == 0 ? i : -1;
        }
        if (found < 0) {
            hero_printing_fail(HERO_PRINTING_NOT_FOUND, 0, "the printer takes no paper named \"%s\"", media);
            return false;
        }
        mode->dmPaperSize = (short)papers->ids[found];
        mode->dmFields |= DM_PAPERSIZE;
        mode->dmFields &= ~(DWORD)(DM_PAPERLENGTH | DM_PAPERWIDTH);
    }
    mode->dmOrientation = orientation == HERO_PRINTING_LANDSCAPE || orientation == HERO_PRINTING_REVERSE_LANDSCAPE ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
    mode->dmFields |= DM_ORIENTATION;
    if (sides != HERO_PRINTING_ONE_SIDED && DeviceCapabilitiesW(name, port, DC_DUPLEX, NULL, NULL) != 1) {
        hero_printing_fail(HERO_PRINTING_UNSUPPORTED, 0, "the printer prints on one side only");
        return false;
    }
    mode->dmDuplex = sides == HERO_PRINTING_ONE_SIDED ? DMDUP_SIMPLEX : sides == HERO_PRINTING_TWO_SIDED_LONG_EDGE ? DMDUP_VERTICAL : DMDUP_HORIZONTAL;
    mode->dmFields |= DM_DUPLEX;
    if (color != HERO_PRINTING_COLOR_DEFAULT) {
        if (color == HERO_PRINTING_COLOR && DeviceCapabilitiesW(name, port, DC_COLORDEVICE, NULL, NULL) != 1) {
            hero_printing_fail(HERO_PRINTING_UNSUPPORTED, 0, "the printer prints no colour");
            return false;
        }
        mode->dmColor = color == HERO_PRINTING_COLOR ? DMCOLOR_COLOR : DMCOLOR_MONOCHROME;
        mode->dmFields |= DM_COLOR;
    }
    return true;
}

int64_t hero_printing_submit(const char *printer, const char *title, const void *pdf, uint64_t size, int64_t copies, const char *media, int64_t orientation, int64_t sides, int64_t color, int64_t collate, const int64_t *ranges, uint64_t range_count) {
    hero_printing_render_work *work;
    hero_printing_papers papers;
    PRINTER_INFO_2W *info = NULL;
    DEVMODEW *mode = NULL;
    HANDLE handle, provider = NULL, thread;
    HDC context;
    int64_t answer = -1;
    HRESULT hr;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_name("job's title", title, true) || !hero_printing_name("paper's name", media, true) ||
        !hero_printing_settings(pdf, size, copies, orientation, sides, color, collate, ranges, range_count)) {
        return -1;
    }
    if (size > (uint64_t)UINT32_MAX) {
        return hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the document is %llu bytes, past the 4 GiB Windows' stream holds", (unsigned long long)size);
    }
    (void)InitOnceExecuteOnce(&hero_printing_render_once, hero_printing_render_load, NULL, NULL);
    if (!hero_printing_render_found) {
        return hero_printing_fail(HERO_PRINTING_UNSUPPORTED, 0, "this Windows has no PDF renderer or Direct2D printing (Windows.Data.Pdf.dll, d2d1.dll, prntvpt.dll), which Windows 10 and later ship");
    }
    handle = hero_printing_open(printer);
    if (handle == NULL) {
        return -1;
    }
    work = hero_layer_alloc(sizeof *work);
    if (work == NULL) {
        ClosePrinter(handle);
        return hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for the job");
    }
    memset(work, 0, sizeof *work);
    memset(&papers, 0, sizeof papers);
    info = (PRINTER_INFO_2W *)hero_printing_ask(hero_printing_ask_printer, handle, "the printer could not be read");
    if (info == NULL || (mode = hero_printing_devmode(handle, info->pPrinterName)) == NULL || !hero_printing_papers_of(info->pPrinterName, info->pPortName, &papers) ||
        !hero_printing_settings_into(mode, info->pPrinterName, info->pPortName, &papers, copies, media, orientation, sides, color, collate)) {
        goto out;
    }
    if (DocumentPropertiesW(NULL, handle, info->pPrinterName, mode, mode, DM_IN_BUFFER | DM_OUT_BUFFER) != IDOK) {
        DWORD code = GetLastError();

        hero_printing_win_fail(hero_printing_win_kind(code), code, "the printer's driver refused the job's settings");
        goto out;
    }
    /* The paper's size as the driver sets it for the job, turned for its
     * orientation, in Direct2D's units, 96 to the inch. */
    context = CreateICW(L"WINSPOOL", info->pPrinterName, NULL, mode);
    if (context == NULL) {
        DWORD code = GetLastError();

        hero_printing_win_fail(hero_printing_win_kind(code), code, "the printer's paper could not be measured");
        goto out;
    }
    work->paper.width = (FLOAT)GetDeviceCaps(context, PHYSICALWIDTH) * 96.0f / (FLOAT)GetDeviceCaps(context, LOGPIXELSX);
    work->paper.height = (FLOAT)GetDeviceCaps(context, PHYSICALHEIGHT) * 96.0f / (FLOAT)GetDeviceCaps(context, LOGPIXELSY);
    DeleteDC(context);
    /* The print ticket the job opens with, written by the driver from the
     * settings. */
    hr = CreateStreamOnHGlobal(NULL, TRUE, &work->ticket);
    if (SUCCEEDED(hr)) {
        hr = hero_printing_render.pt_open(info->pPrinterName, 1, &provider);
    }
    if (SUCCEEDED(hr)) {
        hr = hero_printing_render.pt_from_devmode(provider, (ULONG)(mode->dmSize + mode->dmDriverExtra), mode, 2, work->ticket);
        (void)hero_printing_render.pt_close(provider);
    }
    if (SUCCEEDED(hr)) {
        LARGE_INTEGER start = {0};

        hr = work->ticket->lpVtbl->Seek(work->ticket, start, STREAM_SEEK_SET, NULL);
    }
    if (FAILED(hr)) {
        hero_printing_win_fail(hero_printing_hr_kind(hr), (DWORD)hr, "the job's print ticket could not be written");
        goto out;
    }
    for (size_t i = 0; i < HERO_PRINTING_NAME_MOST && info->pPrinterName[i] != L'\0'; i++) {
        work->printer[i] = info->pPrinterName[i];
    }
    if (!hero_printing_wide(title[0] == '\0' ? "Untitled" : title, work->title, HERO_PRINTING_NAME_MOST + 1)) {
        goto out;
    }
    work->pdf = pdf;
    work->size = (UINT)size;
    work->orientation = orientation;
    work->ranges = ranges;
    work->range_count = range_count;
    thread = CreateThread(NULL, 0, hero_printing_render_thread, work, 0, NULL);
    if (thread == NULL) {
        DWORD code = GetLastError();

        hero_printing_win_fail(HERO_PRINTING_LIMIT, code, "no thread could be made to print the document");
        goto out;
    }
    (void)WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    if (work->job > 0) {
        answer = work->job;
        (void)hero_printing_keep(printer, answer, 3, false, true, NULL);
    } else {
        hero_printing_win_fail(work->kind == HERO_PRINTING_OK ? HERO_PRINTING_OTHER : work->kind, (DWORD)work->failure, work->what);
    }
out:
    if (work->ticket != NULL) {
        work->ticket->lpVtbl->Release(work->ticket);
    }
    hero_layer_free(work);
    hero_printing_papers_free(&papers);
    hero_layer_free(mode);
    hero_layer_free(info);
    ClosePrinter(handle);
    return answer;
}

#else

/* ==========================================================================
 * POSIX: CUPS, opened at the first call.
 * ========================================================================== */

#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

/* CUPS's structures, as its header lays them out since CUPS 1.6, the
 * oldest a supported system ships. */
typedef struct {
    char *name;
    char *value;
} hero_cups_option;

typedef struct {
    char *name;
    char *instance;
    int is_default;
    int num_options;
    hero_cups_option *options;
} hero_cups_dest;

typedef struct {
    char media[128];
    int width;
    int length;
    int bottom;
    int left;
    int right;
    int top;
} hero_cups_size;

typedef struct {
    const char *pwg;
    const char *legacy;
    const char *ppd;
    int width;
    int length;
} hero_pwg_media;

typedef struct {
    int id;
    char *dest;
    char *title;
    char *user;
    char *format;
    int state;
    int size;
    int priority;
    time_t completed_time;
    time_t creation_time;
    time_t processing_time;
} hero_cups_job;

#if defined(__has_include)
#if __has_include(<cups/cups.h>)
/* Where CUPS's header stands, the layouts above are held to it. */
#include <cups/cups.h>
_Static_assert(sizeof(hero_cups_option) == sizeof(cups_option_t), "cups_option_t's layout");
_Static_assert(sizeof(hero_cups_dest) == sizeof(cups_dest_t) && offsetof(hero_cups_dest, options) == offsetof(cups_dest_t, options), "cups_dest_t's layout");
_Static_assert(sizeof(hero_cups_size) == sizeof(cups_size_t) && offsetof(hero_cups_size, top) == offsetof(cups_size_t, top), "cups_size_t's layout");
_Static_assert(sizeof(hero_pwg_media) == sizeof(pwg_media_t) && offsetof(hero_pwg_media, length) == offsetof(pwg_media_t, length), "pwg_media_t's layout");
_Static_assert(sizeof(hero_cups_job) == sizeof(cups_job_t) && offsetof(hero_cups_job, processing_time) == offsetof(cups_job_t, processing_time), "cups_job_t's layout");
_Static_assert(IPP_OP_GET_JOB_ATTRIBUTES == 0x0009 && IPP_TAG_OPERATION == 0x01 && IPP_TAG_URI == 0x45 && IPP_TAG_NAME == 0x42, "IPP's numbers");
_Static_assert(IPP_TAG_ENUM == 0x23 && IPP_TAG_KEYWORD == 0x44 && IPP_TAG_TEXT == 0x41 && IPP_TAG_INTEGER == 0x21 && HTTP_STATUS_CONTINUE == 100, "IPP's numbers");
#endif
#endif

/* The functions of libcups this arm calls. An `http_t *`, an `ipp_t *`, an
 * attribute and a destination's information are pointers this file never
 * reads through, so they are `void *`; IPP's enumerations are `int`. */
typedef struct {
    int (*get_dests2)(void *http, hero_cups_dest **dests);
    void (*free_dests)(int count, hero_cups_dest *dests);
    hero_cups_dest *(*get_dest)(const char *name, const char *instance, int count, hero_cups_dest *dests);
    const char *(*get_option)(const char *name, int count, hero_cups_option *options);
    void *(*copy_dest_info)(void *http, hero_cups_dest *dest);
    void (*free_dest_info)(void *info);
    int (*media_count)(void *http, hero_cups_dest *dest, void *info, unsigned flags);
    int (*media_by_index)(void *http, hero_cups_dest *dest, void *info, int n, unsigned flags, hero_cups_size *size);
    int (*media_default)(void *http, hero_cups_dest *dest, void *info, unsigned flags, hero_cups_size *size);
    const char *(*localize_media)(void *http, hero_cups_dest *dest, void *info, unsigned flags, hero_cups_size *size);
    hero_pwg_media *(*pwg_media)(const char *pwg);
    int (*check_supported)(void *http, hero_cups_dest *dest, void *info, const char *option, const char *value);
    void *(*find_supported)(void *http, hero_cups_dest *dest, void *info, const char *option);
    int (*ipp_get_range)(void *attribute, int element, int *upper);
    int (*add_option)(const char *name, const char *value, int count, hero_cups_option **options);
    void (*free_options)(int count, hero_cups_option *options);
    int (*create_job)(void *http, const char *name, const char *title, int count, hero_cups_option *options);
    int (*start_document)(void *http, const char *name, int job, const char *document, const char *format, int last);
    int (*write_request_data)(void *http, const char *buffer, size_t length);
    int (*finish_document)(void *http, const char *name);
    int (*cancel_job2)(void *http, const char *name, int job, int purge);
    int (*last_error)(void);
    const char *(*last_error_string)(void);
    void *(*ipp_new_request)(int operation);
    void *(*ipp_add_string)(void *ipp, int group, int tag, const char *name, const char *language, const char *value);
    void *(*do_request)(void *http, void *request, const char *resource);
    void *(*ipp_find_attribute)(void *ipp, const char *name, int tag);
    int (*ipp_get_count)(void *attribute);
    int (*ipp_get_integer)(void *attribute, int element);
    const char *(*ipp_get_string)(void *attribute, int element, const char **language);
    void (*ipp_delete)(void *ipp);
    const char *(*user)(void);
    int (*get_jobs2)(void *http, hero_cups_job **jobs, const char *name, int mine, int which);
    void (*free_jobs)(int count, hero_cups_job *jobs);
} hero_printing_cups;

/* Found once inside `pthread_once` and only read after it, as
 * `hero_keyring.c`'s libsecret functions are: the functions are the
 * process's, the same on every thread, and libcups keeps its connection to
 * the scheduler and its last error per thread. */
static hero_printing_cups hero_printing_functions;
static bool hero_printing_found;
static pthread_once_t hero_printing_once = PTHREAD_ONCE_INIT;

static bool hero_printing_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_PRINTING_FIND(field, name) ok = ok && hero_printing_find(library, name, &hero_printing_functions.field, sizeof hero_printing_functions.field)

static void hero_printing_load(void) {
    bool ok = true;
#if defined(__APPLE__)
    void *library = dlopen("/usr/lib/libcups.2.dylib", RTLD_NOW | RTLD_LOCAL);
#else
    void *library = dlopen("libcups.so.2", RTLD_NOW | RTLD_LOCAL);
#endif

    if (library == NULL) {
        return;
    }
    HERO_PRINTING_FIND(get_dests2, "cupsGetDests2");
    HERO_PRINTING_FIND(free_dests, "cupsFreeDests");
    HERO_PRINTING_FIND(get_dest, "cupsGetDest");
    HERO_PRINTING_FIND(get_option, "cupsGetOption");
    HERO_PRINTING_FIND(copy_dest_info, "cupsCopyDestInfo");
    HERO_PRINTING_FIND(free_dest_info, "cupsFreeDestInfo");
    HERO_PRINTING_FIND(media_count, "cupsGetDestMediaCount");
    HERO_PRINTING_FIND(media_by_index, "cupsGetDestMediaByIndex");
    HERO_PRINTING_FIND(media_default, "cupsGetDestMediaDefault");
    HERO_PRINTING_FIND(localize_media, "cupsLocalizeDestMedia");
    HERO_PRINTING_FIND(pwg_media, "pwgMediaForPWG");
    HERO_PRINTING_FIND(check_supported, "cupsCheckDestSupported");
    HERO_PRINTING_FIND(find_supported, "cupsFindDestSupported");
    HERO_PRINTING_FIND(ipp_get_range, "ippGetRange");
    HERO_PRINTING_FIND(add_option, "cupsAddOption");
    HERO_PRINTING_FIND(free_options, "cupsFreeOptions");
    HERO_PRINTING_FIND(create_job, "cupsCreateJob");
    HERO_PRINTING_FIND(start_document, "cupsStartDocument");
    HERO_PRINTING_FIND(write_request_data, "cupsWriteRequestData");
    HERO_PRINTING_FIND(finish_document, "cupsFinishDocument");
    HERO_PRINTING_FIND(cancel_job2, "cupsCancelJob2");
    HERO_PRINTING_FIND(last_error, "cupsLastError");
    HERO_PRINTING_FIND(last_error_string, "cupsLastErrorString");
    HERO_PRINTING_FIND(ipp_new_request, "ippNewRequest");
    HERO_PRINTING_FIND(ipp_add_string, "ippAddString");
    HERO_PRINTING_FIND(do_request, "cupsDoRequest");
    HERO_PRINTING_FIND(ipp_find_attribute, "ippFindAttribute");
    HERO_PRINTING_FIND(ipp_get_count, "ippGetCount");
    HERO_PRINTING_FIND(ipp_get_integer, "ippGetInteger");
    HERO_PRINTING_FIND(ipp_get_string, "ippGetString");
    HERO_PRINTING_FIND(ipp_delete, "ippDelete");
    HERO_PRINTING_FIND(user, "cupsUser");
    HERO_PRINTING_FIND(get_jobs2, "cupsGetJobs2");
    HERO_PRINTING_FIND(free_jobs, "cupsFreeJobs");
    hero_printing_found = ok;
}

/* Whether libcups is here, recording `UNSUPPORTED` where it is not. */
static bool hero_printing_ready(void) {
    (void)pthread_once(&hero_printing_once, hero_printing_load);
    if (!hero_printing_found) {
        hero_printing_fail(HERO_PRINTING_UNSUPPORTED, 0, "CUPS's library is not installed (libcups.so.2 on Linux), so this machine prints nothing");
        return false;
    }
    return true;
}

/* IPP's status codes (RFC 8011, 4.1.6; CUPS's additions from 0x1000). */
#define HERO_IPP_OK_MOST 0x00FF
#define HERO_IPP_FORBIDDEN 0x0401
#define HERO_IPP_NOT_AUTHENTICATED 0x0402
#define HERO_IPP_NOT_AUTHORIZED 0x0403
#define HERO_IPP_NOT_FOUND 0x0406
#define HERO_IPP_GONE 0x0407
#define HERO_IPP_FORMAT_UNSUPPORTED 0x040A
#define HERO_IPP_VALUES_UNSUPPORTED 0x040B
#define HERO_IPP_OPERATION_UNSUPPORTED 0x0501
#define HERO_IPP_SERVICE_UNAVAILABLE 0x0502
#define HERO_IPP_TOO_MANY_JOBS 0x050B
#define HERO_IPP_TOO_MANY_DOCUMENTS 0x050C
#define HERO_IPP_AUTHENTICATION_CANCELED 0x1000

static int64_t hero_printing_cups_kind(int status) {
    switch (status) {
    case HERO_IPP_NOT_FOUND:
    case HERO_IPP_GONE:
        return HERO_PRINTING_NOT_FOUND;
    case HERO_IPP_FORBIDDEN:
    case HERO_IPP_NOT_AUTHENTICATED:
    case HERO_IPP_NOT_AUTHORIZED:
    case HERO_IPP_AUTHENTICATION_CANCELED:
        return HERO_PRINTING_DENIED;
    case HERO_IPP_FORMAT_UNSUPPORTED:
    case HERO_IPP_VALUES_UNSUPPORTED:
    case HERO_IPP_OPERATION_UNSUPPORTED:
    case HERO_IPP_SERVICE_UNAVAILABLE:
        return HERO_PRINTING_UNSUPPORTED;
    case HERO_IPP_TOO_MANY_JOBS:
    case HERO_IPP_TOO_MANY_DOCUMENTS:
        return HERO_PRINTING_LIMIT;
    default:
        return HERO_PRINTING_OTHER;
    }
}

/* Records CUPS's last failure on this thread with what was asked. */
static int64_t hero_printing_cups_fail(const char *what) {
    const hero_printing_cups *f = &hero_printing_functions;
    int status = f->last_error();
    const char *words = f->last_error_string();

    if (status == HERO_IPP_SERVICE_UNAVAILABLE) {
        return hero_printing_fail(HERO_PRINTING_UNSUPPORTED, status, "%s: no print scheduler answers (%s)", what, words != NULL ? words : "service unavailable");
    }
    return hero_printing_fail(hero_printing_cups_kind(status), status, "%s: %s (IPP status 0x%04x)", what, words != NULL && words[0] != '\0' ? words : "CUPS gave no words", status);
}

static void hero_printing_pause(int64_t ms) {
    struct timespec span = {(time_t)(ms / 1000), (long)(ms % 1000) * 1000000L};

    while (nanosleep(&span, &span) != 0) {
    }
}

static int64_t hero_printing_clock(void) {
    struct timespec now;

    (void)clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
}

/* The destinations, read once per call; the queue named `printer` in them,
 * where `printer` is not NULL. False with the failure recorded. */
static bool hero_printing_dests(const char *printer, int *count, hero_cups_dest **dests, hero_cups_dest **found) {
    const hero_printing_cups *f = &hero_printing_functions;

    *dests = NULL;
    *count = f->get_dests2(NULL, dests);
    if (*count == 0) {
        int status = f->last_error();

        /* No queue at all is an empty list, which CUPS tells as not found. */
        if (status > HERO_IPP_OK_MOST && status != HERO_IPP_NOT_FOUND) {
            hero_printing_cups_fail("the printers could not be listed");
            return false;
        }
    }
    if (printer != NULL) {
        *found = f->get_dest(printer, NULL, *count, *dests);
        if (*found == NULL) {
            f->free_dests(*count, *dests);
            hero_printing_fail(HERO_PRINTING_NOT_FOUND, HERO_IPP_NOT_FOUND, "no printer named \"%s\"", printer);
            return false;
        }
    }
    return true;
}

int64_t hero_printing_printers(void *buffer, uint64_t count) {
    const hero_printing_cups *f;
    hero_printing_list list = {0};
    hero_cups_dest *dests = NULL;
    int n = 0;

    hero_printing_clear();
    if (!hero_printing_room(buffer, count) || !hero_printing_ready() || !hero_printing_dests(NULL, &n, &dests, NULL)) {
        return -1;
    }
    f = &hero_printing_functions;
    for (int i = 0; i < n; i++) {
        const hero_cups_dest *d = &dests[i];
        const char *info = f->get_option("printer-info", d->num_options, d->options);
        const char *state = f->get_option("printer-state", d->num_options, d->options);
        const char *reasons = f->get_option("printer-state-reasons", d->num_options, d->options);
        const char *accepting = f->get_option("printer-is-accepting-jobs", d->num_options, d->options);
        bool is_default = d->is_default != 0;

        /* An instance is a queue's saved options under another name, not a
         * printer: it is left out, and a default instance makes its queue
         * the default. */
        if (d->instance != NULL) {
            continue;
        }
        for (int k = 0; k < n && !is_default; k++) {
            is_default = dests[k].is_default != 0 && dests[k].instance != NULL && strcmp(dests[k].name, d->name) == 0;
        }
        hero_printing_add(&list, d->name);
        hero_printing_add(&list, info != NULL && info[0] != '\0' ? info : d->name);
        hero_printing_add(&list, f->get_option("printer-location", d->num_options, d->options));
        hero_printing_add(&list, f->get_option("printer-make-and-model", d->num_options, d->options));
        hero_printing_add(&list, state != NULL && strcmp(state, "5") == 0 ? "stopped" : state != NULL && strcmp(state, "4") == 0 ? "processing" : "idle");
        hero_printing_add(&list, reasons != NULL && strcmp(reasons, "none") != 0 ? reasons : "");
        hero_printing_add(&list, f->get_option("printer-state-message", d->num_options, d->options));
        hero_printing_add(&list, accepting != NULL && strcmp(accepting, "false") == 0 ? "0" : "1");
        hero_printing_add(&list, is_default ? "1" : "0");
    }
    f->free_dests(n, dests);
    return hero_printing_hand(&list, buffer, count);
}

/* The words a person reads for a paper: CUPS's localisation where the
 * printer gives one, else the paper's short name in PWG's table (`A4`,
 * `Letter`), else its PWG name. */
static const char *hero_printing_label(hero_cups_dest *dest, void *info, hero_cups_size *size) {
    const hero_printing_cups *f = &hero_printing_functions;
    const char *localized = f->localize_media(NULL, dest, info, 0, size);
    const hero_pwg_media *known;

    if (localized != NULL && localized[0] != '\0') {
        return localized;
    }
    known = f->pwg_media(size->media);
    return known != NULL && known->ppd != NULL ? known->ppd : size->media;
}

/* CUPS keys a paper it lists with zero margins `<PWG name>_borderless`, its
 * own suffix and no PWG name, and lists a size twice where the printer
 * prints it with margins and without (measured 2026-11-23: a PPD queue of
 * Debian's cups-pdf named every paper so, A4 among them; an AirPrint queue
 * on macOS named `oe_photo-l_3.5x5in` and `oe_photo-l_3.5x5in_borderless`).
 * A paper's name here is the PWG name, which a job's `media` takes and CUPS
 * maps to the queue's own size. */
static const char hero_printing_borderless[] = "_borderless";

/* `key`, a paper's name of at most 127 bytes, made its PWG name where it
 * stands, its `_borderless` suffix cut; true where it had one. */
static bool hero_printing_pwg_name(char *key) {
    size_t length = strnlen(key, 127);
    size_t suffix = sizeof hero_printing_borderless - 1;
    bool borderless = length > suffix && memcmp(key + length - suffix, hero_printing_borderless, suffix) == 0;

    key[borderless ? length - suffix : length] = '\0';
    return borderless;
}

/* The papers of `dest`, each once by its PWG name: where a size is listed
 * with margins and without, the one with margins, which a job gets where it
 * asks no borderless printing. Answers how many `kept` holds, or -1 with the
 * failure recorded; `kept` is the caller's to give back. */
static int hero_printing_papers_of(hero_cups_dest *dest, void *info, hero_cups_size **kept) {
    const hero_printing_cups *f = &hero_printing_functions;
    int listed = f->media_count(NULL, dest, info, 0);
    hero_cups_size *all;
    bool *edgeless;
    int count = 0;

    *kept = NULL;
    if (listed <= 0) {
        return 0;
    }
    all = hero_layer_alloc((size_t)listed * sizeof *all);
    edgeless = hero_layer_alloc((size_t)listed * sizeof *edgeless);
    if (all == NULL || edgeless == NULL) {
        hero_layer_free(all);
        hero_layer_free(edgeless);
        hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for the printer's papers");
        return -1;
    }
    for (int i = 0; i < listed; i++) {
        if (f->media_by_index(NULL, dest, info, i, 0, &all[count]) != 0) {
            all[count].media[sizeof all[count].media - 1] = '\0';
            count++;
        }
    }
    for (int i = 0; i < count; i++) {
        edgeless[i] = hero_printing_pwg_name(all[i].media);
    }
    /* Each name once: the first entry with margins, else the first
     * without. */
    {
        int out = 0;

        for (int i = 0; i < count; i++) {
            bool keep = true;

            for (int j = 0; j < count && keep; j++) {
                if (j != i && strcmp(all[j].media, all[i].media) == 0) {
                    keep = edgeless[j] && !edgeless[i] ? true : edgeless[i] && !edgeless[j] ? false : j > i;
                }
            }
            if (keep) {
                all[out++] = all[i];
            }
        }
        count = out;
    }
    hero_layer_free(edgeless);
    *kept = all;
    return count;
}

int64_t hero_printing_media(const char *printer, void *buffer, uint64_t count) {
    const hero_printing_cups *f;
    hero_printing_list list = {0};
    hero_cups_dest *dests = NULL, *dest = NULL;
    hero_cups_size fallback;
    hero_cups_size *papers = NULL;
    void *info;
    int n = 0, kept;
    bool has_default;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count) || !hero_printing_ready() || !hero_printing_dests(printer, &n, &dests, &dest)) {
        return -1;
    }
    f = &hero_printing_functions;
    info = f->copy_dest_info(NULL, dest);
    if (info == NULL) {
        int64_t failed = hero_printing_cups_fail("the printer's papers could not be read");

        f->free_dests(n, dests);
        return failed;
    }
    has_default = f->media_default(NULL, dest, info, 0, &fallback) != 0;
    if (has_default) {
        fallback.media[sizeof fallback.media - 1] = '\0';
        (void)hero_printing_pwg_name(fallback.media);
    }
    kept = hero_printing_papers_of(dest, info, &papers);
    for (int i = 0; i < kept; i++) {
        hero_cups_size *size = &papers[i];

        hero_printing_add(&list, size->media);
        hero_printing_add(&list, hero_printing_label(dest, info, size));
        hero_printing_add_number(&list, size->width);
        hero_printing_add_number(&list, size->length);
        hero_printing_add_number(&list, size->left);
        hero_printing_add_number(&list, size->right);
        hero_printing_add_number(&list, size->top);
        hero_printing_add_number(&list, size->bottom);
        hero_printing_add(&list, has_default && strcmp(size->media, fallback.media) == 0 ? "1" : "0");
    }
    hero_layer_free(papers);
    f->free_dest_info(info);
    f->free_dests(n, dests);
    if (kept < 0) {
        hero_layer_free(list.bytes);
        return -1;
    }
    return hero_printing_hand(&list, buffer, count);
}

int64_t hero_printing_capabilities(const char *printer, void *buffer, uint64_t count) {
    const hero_printing_cups *f;
    hero_printing_list list = {0};
    hero_cups_dest *dests = NULL, *dest = NULL;
    void *info, *copies;
    int n = 0, upper = 1;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count) || !hero_printing_ready() || !hero_printing_dests(printer, &n, &dests, &dest)) {
        return -1;
    }
    f = &hero_printing_functions;
    info = f->copy_dest_info(NULL, dest);
    if (info == NULL) {
        int64_t failed = hero_printing_cups_fail("the printer could not be asked what it supports");

        f->free_dests(n, dests);
        return failed;
    }
    /* `copies-supported` is a range (RFC 8011, 5.4.20); a printer that
     * names none makes one copy at a time. */
    copies = f->find_supported(NULL, dest, info, "copies");
    if (copies != NULL) {
        (void)f->ipp_get_range(copies, 0, &upper);
    }
    hero_printing_add(&list, f->check_supported(NULL, dest, info, "sides", "two-sided-long-edge") != 0 ? "1" : "0");
    hero_printing_add(&list, f->check_supported(NULL, dest, info, "print-color-mode", "color") != 0 ? "1" : "0");
    hero_printing_add_number(&list, upper < 1 ? 1 : upper > HERO_PRINTING_COPIES_MOST ? HERO_PRINTING_COPIES_MOST : upper);
    f->free_dest_info(info);
    f->free_dests(n, dests);
    return hero_printing_hand(&list, buffer, count);
}

/* Whether the printer takes `value` for `option`, recording `UNSUPPORTED`
 * where it does not. */
static bool hero_printing_takes(hero_cups_dest *dest, void *info, const char *option, const char *value, const char *words) {
    if (hero_printing_functions.check_supported(NULL, dest, info, option, value) == 0) {
        hero_printing_fail(HERO_PRINTING_UNSUPPORTED, 0, "the printer %s (%s %s is not among what it supports)", words, option, value);
        return false;
    }
    return true;
}

int64_t hero_printing_submit(const char *printer, const char *title, const void *pdf, uint64_t size, int64_t copies, const char *media, int64_t orientation, int64_t sides, int64_t color, int64_t collate, const int64_t *ranges, uint64_t range_count) {
    static const char *const side_words[] = {"", "one-sided", "two-sided-long-edge", "two-sided-short-edge"};
    const hero_printing_cups *f;
    hero_cups_dest *dests = NULL, *dest = NULL;
    hero_cups_option *options = NULL;
    void *info = NULL;
    char number[32];
    char pages[HERO_PRINTING_RANGES_MOST * 24];
    size_t pages_used = 0;
    int n = 0, count = 0, job = 0;
    int64_t answer = -1;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_name("job's title", title, true) || !hero_printing_name("paper's name", media, true) ||
        !hero_printing_settings(pdf, size, copies, orientation, sides, color, collate, ranges, range_count) || !hero_printing_ready() || !hero_printing_dests(printer, &n, &dests, &dest)) {
        return -1;
    }
    f = &hero_printing_functions;
    info = f->copy_dest_info(NULL, dest);
    if (info == NULL) {
        hero_printing_cups_fail("the printer could not be asked what it supports");
        goto out;
    }
    if (media[0] != '\0') {
        hero_cups_size *papers = NULL;
        int kept = hero_printing_papers_of(dest, info, &papers);
        bool found = false;

        for (int i = 0; i < kept && !found; i++) {
            found = strcmp(papers[i].media, media) == 0;
        }
        hero_layer_free(papers);
        if (kept < 0) {
            goto out;
        }
        if (!found) {
            hero_printing_fail(HERO_PRINTING_NOT_FOUND, 0, "printer \"%s\" takes no paper named \"%s\"", printer, media);
            goto out;
        }
    }
    snprintf(number, sizeof number, "%lld", (long long)copies);
    if ((copies > 1 && !hero_printing_takes(dest, info, "copies", number, "makes fewer copies")) ||
        (sides != HERO_PRINTING_ONE_SIDED && !hero_printing_takes(dest, info, "sides", side_words[sides], "prints on one side only")) ||
        (color != HERO_PRINTING_COLOR_DEFAULT && !hero_printing_takes(dest, info, "print-color-mode", color == HERO_PRINTING_COLOR ? "color" : "monochrome", color == HERO_PRINTING_COLOR ? "prints no colour" : "prints colour alone"))) {
        goto out;
    }
    count = f->add_option("copies", number, count, &options);
    if (media[0] != '\0') {
        count = f->add_option("media", media, count, &options);
    }
    snprintf(number, sizeof number, "%lld", (long long)orientation);
    count = f->add_option("orientation-requested", number, count, &options);
    count = f->add_option("sides", side_words[sides], count, &options);
    if (color != HERO_PRINTING_COLOR_DEFAULT) {
        count = f->add_option("print-color-mode", color == HERO_PRINTING_COLOR ? "color" : "monochrome", count, &options);
    }
    count = f->add_option("multiple-document-handling", collate == 1 ? "separate-documents-collated-copies" : "separate-documents-uncollated-copies", count, &options);
    for (uint64_t i = 0; i < range_count; i += 2) {
        int written = snprintf(pages + pages_used, sizeof pages - pages_used, "%s%lld-%lld", i == 0 ? "" : ",", (long long)ranges[i], (long long)ranges[i + 1]);

        pages_used += written > 0 ? (size_t)written : 0;
    }
    if (range_count > 0) {
        count = f->add_option("page-ranges", pages, count, &options);
    }
    job = f->create_job(NULL, printer, title[0] == '\0' ? "Untitled" : title, count, options);
    if (job <= 0) {
        hero_printing_cups_fail("the job could not be made");
        goto out;
    }
    if (f->start_document(NULL, printer, job, title[0] == '\0' ? "Untitled" : title, "application/pdf", 1) != 100) {
        hero_printing_cups_fail("the document could not be begun");
        goto cancel;
    }
    for (uint64_t sent = 0; sent < size;) {
        size_t piece = size - sent < 65536 ? (size_t)(size - sent) : 65536;

        if (f->write_request_data(NULL, (const char *)pdf + sent, piece) != 100) {
            hero_printing_cups_fail("the document could not be sent");
            (void)f->finish_document(NULL, printer);
            goto cancel;
        }
        sent += piece;
    }
    if (f->finish_document(NULL, printer) > HERO_IPP_OK_MOST) {
        hero_printing_cups_fail("the document was refused");
        goto cancel;
    }
    answer = job;
    goto out;
cancel:
    /* No job stands after a failure: one the scheduler had begun is
     * cancelled and purged, the failure's words kept. */
    {
        int64_t kind = hero_printing_kind;
        int64_t os = hero_printing_os;
        char words[HERO_PRINTING_WORDS_MOST];

        memcpy(words, hero_printing_words, sizeof words);
        (void)f->cancel_job2(NULL, printer, job, 1);
        hero_printing_kind = kind;
        hero_printing_os = os;
        memcpy(hero_printing_words, words, sizeof words);
    }
out:
    if (options != NULL) {
        f->free_options(count, options);
    }
    if (info != NULL) {
        f->free_dest_info(info);
    }
    f->free_dests(n, dests);
    return answer;
}

/* Joins the keywords of `attribute` with commas, `none` left out. */
static void hero_printing_add_keywords(hero_printing_list *list, void *attribute) {
    const hero_printing_cups *f = &hero_printing_functions;
    char joined[1024] = "";
    size_t used = 0;
    int count = attribute == NULL ? 0 : f->ipp_get_count(attribute);

    for (int i = 0; i < count; i++) {
        const char *word = f->ipp_get_string(attribute, i, NULL);

        if (word != NULL && strcmp(word, "none") != 0 && used + strlen(word) + 2 < sizeof joined) {
            int written = snprintf(joined + used, sizeof joined - used, "%s%s", used == 0 ? "" : ",", word);

            used += written > 0 ? (size_t)written : 0;
        }
    }
    hero_printing_add(list, joined);
}

/* Whether the last part of `uri`, its `%XX` escapes read, is `name`. */
static bool hero_printing_uri_names(const char *uri, const char *name) {
    const char *last = strrchr(uri, '/');
    size_t k = 0;

    if (last == NULL) {
        return false;
    }
    for (const char *c = last + 1; *c != '\0'; c++, k++) {
        int byte = (unsigned char)*c;

        if (*c == '%' && c[1] != '\0' && c[2] != '\0') {
            char hex[3] = {c[1], c[2], '\0'};
            char *end = NULL;
            long value = strtol(hex, &end, 16);

            if (end != hex + 2) {
                return false;
            }
            byte = (int)value;
            c += 2;
        }
        if (name[k] == '\0' || (unsigned char)name[k] != byte) {
            return false;
        }
    }
    return name[k] == '\0';
}

static int64_t hero_printing_job_state(const char *printer, int64_t job, int *state, hero_printing_list *fields) {
    const hero_printing_cups *f;
    char uri[64];
    void *request, *answer, *attribute;
    const char *where;
    int status;

    if (!hero_printing_ready()) {
        return -1;
    }
    if (job < 1 || job > INT32_MAX) {
        return hero_printing_fail(HERO_PRINTING_NOT_FOUND, HERO_IPP_NOT_FOUND, "no job %lld stands on printer \"%s\"", (long long)job, printer);
    }
    f = &hero_printing_functions;
    snprintf(uri, sizeof uri, "ipp://localhost/jobs/%lld", (long long)job);
    request = f->ipp_new_request(0x0009);
    if (request == NULL) {
        return hero_printing_fail(HERO_PRINTING_LIMIT, 0, "the system has no memory for the request");
    }
    (void)f->ipp_add_string(request, 0x01, 0x45, "job-uri", NULL, uri);
    (void)f->ipp_add_string(request, 0x01, 0x42, "requesting-user-name", NULL, f->user());
    answer = f->do_request(NULL, request, "/");
    status = f->last_error();
    if (answer == NULL || status > HERO_IPP_OK_MOST) {
        if (answer != NULL) {
            f->ipp_delete(answer);
        }
        if (status == HERO_IPP_NOT_FOUND || status == HERO_IPP_GONE) {
            return hero_printing_fail(HERO_PRINTING_NOT_FOUND, status, "no job %lld stands on printer \"%s\"", (long long)job, printer);
        }
        return hero_printing_cups_fail("the job could not be read");
    }
    /* The job is the printer's where its printer's URI's last part, its
     * escapes read, is the printer's own name. */
    attribute = f->ipp_find_attribute(answer, "job-printer-uri", 0x45);
    where = attribute == NULL ? NULL : f->ipp_get_string(attribute, 0, NULL);
    if (where == NULL || !hero_printing_uri_names(where, printer)) {
        f->ipp_delete(answer);
        return hero_printing_fail(HERO_PRINTING_NOT_FOUND, HERO_IPP_NOT_FOUND, "no job %lld stands on printer \"%s\"", (long long)job, printer);
    }
    attribute = f->ipp_find_attribute(answer, "job-state", 0x23);
    *state = attribute == NULL ? 3 : f->ipp_get_integer(attribute, 0);
    if (fields != NULL) {
        hero_printing_add(fields, hero_printing_job_word(*state));
        hero_printing_add_keywords(fields, f->ipp_find_attribute(answer, "job-state-reasons", 0x44));
        attribute = f->ipp_find_attribute(answer, "job-state-message", 0x41);
        hero_printing_add(fields, attribute == NULL ? "" : f->ipp_get_string(attribute, 0, NULL));
        attribute = f->ipp_find_attribute(answer, "job-impressions-completed", 0x21);
        hero_printing_add_number(fields, attribute == NULL ? 0 : f->ipp_get_integer(attribute, 0));
    }
    f->ipp_delete(answer);
    return 0;
}

int64_t hero_printing_jobs(const char *printer, void *buffer, uint64_t count) {
    const hero_printing_cups *f;
    hero_printing_list list = {0};
    hero_cups_dest *dests = NULL, *dest = NULL;
    hero_cups_job *jobs = NULL;
    int n = 0, found;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || !hero_printing_room(buffer, count) || !hero_printing_ready() || !hero_printing_dests(printer, &n, &dests, &dest)) {
        return -1;
    }
    f = &hero_printing_functions;
    f->free_dests(n, dests);
    found = f->get_jobs2(NULL, &jobs, printer, 0, 0);
    if (found < 0 || (found == 0 && f->last_error() > HERO_IPP_OK_MOST && f->last_error() != HERO_IPP_NOT_FOUND)) {
        return hero_printing_cups_fail("the printer's jobs could not be listed");
    }
    /* Oldest first: CUPS numbers its jobs in the order they came. The
     * entries are sorted where they stand, `cupsFreeJobs` freeing each by
     * its own pointers whatever its place. */
    for (int i = 1; i < found; i++) {
        hero_cups_job moving = jobs[i];
        int k = i;

        for (; k > 0 && jobs[k - 1].id > moving.id; k--) {
            jobs[k] = jobs[k - 1];
        }
        jobs[k] = moving;
    }
    for (int i = 0; i < found; i++) {
        if (!hero_printing_ended(jobs[i].state)) {
            hero_printing_add_number(&list, jobs[i].id);
        }
    }
    f->free_jobs(found, jobs);
    return hero_printing_hand(&list, buffer, count);
}

int64_t hero_printing_cancel(const char *printer, int64_t job) {
    int state = 0;
    int status;

    hero_printing_clear();
    if (!hero_printing_name("printer's name", printer, false) || hero_printing_job_state(printer, job, &state, NULL) < 0) {
        return -1;
    }
    if (hero_printing_ended(state)) {
        return 0;
    }
    status = hero_printing_functions.cancel_job2(NULL, printer, (int)job, 0);
    if (status > HERO_IPP_OK_MOST) {
        char words[HERO_PRINTING_WORDS_MOST];
        const char *said = hero_printing_functions.last_error_string();

        snprintf(words, sizeof words, "%s", said != NULL && said[0] != '\0' ? said : "CUPS gave no words");
        /* A job that ended between the two calls is told as it ended. */
        if (hero_printing_job_state(printer, job, &state, NULL) == 0 && hero_printing_ended(state)) {
            hero_printing_clear();
            return 0;
        }
        return hero_printing_fail(hero_printing_cups_kind(status), status, "the job could not be cancelled: %s (IPP status 0x%04x)", words, status);
    }
    return 0;
}

#endif
