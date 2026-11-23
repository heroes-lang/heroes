/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_print_win32.c — the Win32 arm of the print and
 * page-setup dialogs (`hero_ui_print_win32.h` says what each function
 * answers): the system's print dialog, `PrintDlgExW`, and its page setup,
 * `PageSetupDlgW`, over the window's `HWND`, their fields first set from the
 * settings' text through a DEVMODE and a DEVNAMES of the printer, and read
 * back from them once the person has answered. A paper's PWG name is the one
 * `hero_printing_media` lists, matched to the driver's paper by its size, so
 * the dialogs and `printing` name every paper alike.
 *
 * OFF WINDOWS every function answers `HERO_UI_UNSUPPORTED`: `hero_ui.c`
 * calls this arm on Windows alone, and the member is compiled everywhere as
 * every member is.
 *
 * THE LIBRARIES ARE NAMED BY THE MEMBER, `comdlg32` and `winspool`, by
 * `#pragma comment(lib)` on MSVC's target, as `hero_ui_win32.c` names its
 * own.
 *
 * Written from Microsoft's documentation of the common dialogs and the
 * spooler; no other library's source.
 */

#include "hero_ui_print_win32.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"

/* The kinds of `hero_ui.h` this arm answers. */
#define HERO_UI_PRINT_OK 0
#define HERO_UI_PRINT_INVALID 1
#define HERO_UI_PRINT_UNSUPPORTED 4
#define HERO_UI_PRINT_NOT_FOUND 6
#define HERO_UI_PRINT_OTHER 8
#define HERO_UI_PRINT_EVENT_DIALOG 17
/* The room a printer's list of papers is read into: a hundred papers of a
 * driver take about 6 KB. */
#define HERO_UI_PRINT_LIST_ROOM 65536

/* The words of the last refusal on this thread. */
static _Thread_local char hero_ui_print_win32_said[512];

int64_t hero_ui_print_win32_words(char *words, int64_t count) {
    size_t length = strlen(hero_ui_print_win32_said);

    if (words == NULL || count <= 0) {
        return 0;
    }
    if ((int64_t)length >= count) {
        length = (size_t)count - 1;
    }
    memcpy(words, hero_ui_print_win32_said, length);
    words[length] = '\0';
    return (int64_t)length;
}

static int64_t hero_ui_print_win32_refuse(int64_t kind, const char *words) {
    snprintf(hero_ui_print_win32_said, sizeof hero_ui_print_win32_said, "%s", words);
    return kind;
}

int64_t hero_ui_print_win32_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count) {
    (void)dialog;
    (void)value;
    (void)settings;
    (void)count;
    return hero_ui_print_win32_refuse(HERO_UI_PRINT_NOT_FOUND, "no print dialog of the Win32 arm stands: each waits for its person and is gone when its call returns");
}

static void hero_ui_print_win32_end_web(void *window);

void hero_ui_print_win32_end(void *window) {
    hero_ui_print_win32_end_web(window);
}

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
/* Before `commdlg.h`, which declares `PrintDlgExW` only where COM's
 * interface words are. */
#include <objbase.h>
#include <commdlg.h>
#include <winspool.h>

#include "hero_ui_print_text.h"
#if defined(_MSC_VER)
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "winspool.lib")
#endif

/* The printer's DEVMODE and DEVNAMES, movable blocks the dialogs take and
 * hand back, set from `settings`; false with the refusal recorded. */
static bool hero_ui_print_win32_blocks(const hero_ui_print_settings *settings, HGLOBAL *mode_block, HGLOBAL *names_block) {
    WCHAR name[HERO_PRINTING_NAME_MOST + 1];
    DWORD length = HERO_PRINTING_NAME_MOST + 1;
    HANDLE printer = NULL;
    BYTE *info_bytes = NULL;
    PRINTER_INFO_2W *info;
    DWORD needed = 0;
    LONG size;
    DEVMODEW *mode;
    DEVNAMES *names;
    size_t driver, device, port;

    *mode_block = NULL;
    *names_block = NULL;
    if (settings->printer[0] != '\0') {
        if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, settings->printer, -1, name, HERO_PRINTING_NAME_MOST + 1) <= 0) {
            hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "the printer's name is not UTF-8");
            return false;
        }
    } else if (!GetDefaultPrinterW(name, &length)) {
        /* No default printer: the dialog chooses one. */
        return true;
    }
    if (!OpenPrinterW(name, &printer, NULL)) {
        hero_ui_print_win32_refuse(HERO_UI_PRINT_NOT_FOUND, "no printer of that name: the spooler could not open it");
        return false;
    }
    (void)GetPrinterW(printer, 2, NULL, 0, &needed);
    info_bytes = needed > 0 ? hero_layer_alloc(needed) : NULL;
    if (info_bytes == NULL || !GetPrinterW(printer, 2, info_bytes, needed, &needed)) {
        hero_layer_free(info_bytes);
        ClosePrinter(printer);
        hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, "the printer could not be read");
        return false;
    }
    info = (PRINTER_INFO_2W *)info_bytes;
    size = DocumentPropertiesW(NULL, printer, info->pPrinterName, NULL, NULL, 0);
    *mode_block = size > 0 ? GlobalAlloc(GHND, (SIZE_T)size) : NULL;
    mode = *mode_block != NULL ? GlobalLock(*mode_block) : NULL;
    if (mode == NULL || DocumentPropertiesW(NULL, printer, info->pPrinterName, mode, NULL, DM_OUT_BUFFER) != IDOK) {
        if (mode != NULL) {
            GlobalUnlock(*mode_block);
        }
        if (*mode_block != NULL) {
            GlobalFree(*mode_block);
            *mode_block = NULL;
        }
        hero_layer_free(info_bytes);
        ClosePrinter(printer);
        hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, "the printer's settings could not be read");
        return false;
    }
    mode->dmCopies = (short)settings->copies;
    mode->dmCollate = settings->collate == 1 ? DMCOLLATE_TRUE : DMCOLLATE_FALSE;
    mode->dmOrientation = settings->orientation == HERO_PRINTING_LANDSCAPE || settings->orientation == HERO_PRINTING_REVERSE_LANDSCAPE ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
    mode->dmDuplex = settings->sides == HERO_PRINTING_TWO_SIDED_LONG_EDGE ? DMDUP_VERTICAL : settings->sides == HERO_PRINTING_TWO_SIDED_SHORT_EDGE ? DMDUP_HORIZONTAL : DMDUP_SIMPLEX;
    mode->dmFields |= DM_COPIES | DM_COLLATE | DM_ORIENTATION | DM_DUPLEX;
    if (settings->color != HERO_PRINTING_COLOR_DEFAULT) {
        mode->dmColor = settings->color == HERO_PRINTING_COLOR ? DMCOLOR_COLOR : DMCOLOR_MONOCHROME;
        mode->dmFields |= DM_COLOR;
    }
    if (settings->paper[0] != '\0') {
        /* The paper of the printing member's list by its name, then the
         * driver's paper of its size. */
        char *list = hero_layer_alloc(HERO_UI_PRINT_LIST_ROOM);
        char narrow[HERO_PRINTING_NAME_MOST + 1];
        int64_t got = -1, width = -1, height = -1;
        int papers = DeviceCapabilitiesW(info->pPrinterName, info->pPortName, DC_PAPERS, NULL, NULL);

        if (list != NULL && WideCharToMultiByte(CP_UTF8, 0, info->pPrinterName, -1, narrow, (int)sizeof narrow, NULL, NULL) > 0) {
            got = hero_printing_media(narrow, list, HERO_UI_PRINT_LIST_ROOM);
        }
        for (int64_t at = 0, field = 0; got > 0 && got <= HERO_UI_PRINT_LIST_ROOM && at < got; field++) {
            const char *text = list + at;

            if (field % HERO_PRINTING_MEDIA_FIELDS == 0 && strcmp(text, settings->paper) == 0) {
                const char *label = text + strlen(text) + 1;
                const char *across = label + strlen(label) + 1;
                const char *down = across + strlen(across) + 1;

                width = strtoll(across, NULL, 10);
                height = strtoll(down, NULL, 10);
            }
            at += (int64_t)strlen(text) + 1;
        }
        hero_layer_free(list);
        if (width > 0 && papers > 0) {
            WORD *ids = hero_layer_alloc((size_t)papers * sizeof(WORD));
            POINT *sizes = hero_layer_alloc((size_t)papers * sizeof(POINT));

            if (ids != NULL && sizes != NULL && DeviceCapabilitiesW(info->pPrinterName, info->pPortName, DC_PAPERS, (LPWSTR)ids, NULL) == papers &&
                DeviceCapabilitiesW(info->pPrinterName, info->pPortName, DC_PAPERSIZE, (LPWSTR)sizes, NULL) == papers) {
                for (int i = 0; i < papers; i++) {
                    if ((int64_t)sizes[i].x * 10 == width && (int64_t)sizes[i].y * 10 == height) {
                        mode->dmPaperSize = (short)ids[i];
                        mode->dmFields |= DM_PAPERSIZE;
                        mode->dmFields &= ~(DWORD)(DM_PAPERLENGTH | DM_PAPERWIDTH);
                        break;
                    }
                }
            }
            hero_layer_free(ids);
            hero_layer_free(sizes);
        }
    }
    (void)DocumentPropertiesW(NULL, printer, info->pPrinterName, mode, mode, DM_IN_BUFFER | DM_OUT_BUFFER);
    GlobalUnlock(*mode_block);
    driver = wcslen(info->pDriverName) + 1;
    device = wcslen(info->pPrinterName) + 1;
    port = wcslen(info->pPortName) + 1;
    *names_block = GlobalAlloc(GHND, sizeof(DEVNAMES) + (driver + device + port) * sizeof(WCHAR));
    names = *names_block != NULL ? GlobalLock(*names_block) : NULL;
    if (names != NULL) {
        WCHAR *text = (WCHAR *)(names + 1);

        names->wDriverOffset = (WORD)(sizeof(DEVNAMES) / sizeof(WCHAR));
        names->wDeviceOffset = (WORD)(names->wDriverOffset + driver);
        names->wOutputOffset = (WORD)(names->wDeviceOffset + device);
        memcpy(text, info->pDriverName, driver * sizeof(WCHAR));
        memcpy(text + driver, info->pPrinterName, device * sizeof(WCHAR));
        memcpy(text + driver + device, info->pPortName, port * sizeof(WCHAR));
        GlobalUnlock(*names_block);
    }
    hero_layer_free(info_bytes);
    ClosePrinter(printer);
    return true;
}

/* The settings the blocks hold once the person has answered, the ranges
 * the print dialog's own where it took any. */
static void hero_ui_print_win32_read_back(HGLOBAL mode_block, HGLOBAL names_block, hero_ui_print_settings *settings) {
    DEVMODEW *mode = mode_block != NULL ? GlobalLock(mode_block) : NULL;
    DEVNAMES *names = names_block != NULL ? GlobalLock(names_block) : NULL;

    if (names != NULL) {
        const WCHAR *device = (const WCHAR *)names + names->wDeviceOffset;

        if (WideCharToMultiByte(CP_UTF8, 0, device, -1, settings->printer, (int)sizeof settings->printer, NULL, NULL) <= 0) {
            settings->printer[0] = '\0';
        }
        GlobalUnlock(names_block);
    }
    if (mode != NULL) {
        if ((mode->dmFields & DM_COPIES) != 0 && mode->dmCopies >= 1) {
            settings->copies = mode->dmCopies > HERO_PRINTING_COPIES_MOST ? HERO_PRINTING_COPIES_MOST : mode->dmCopies;
        }
        if ((mode->dmFields & DM_COLLATE) != 0) {
            settings->collate = mode->dmCollate == DMCOLLATE_TRUE ? 1 : 0;
        }
        if ((mode->dmFields & DM_ORIENTATION) != 0) {
            bool reverse = settings->orientation == HERO_PRINTING_REVERSE_LANDSCAPE || settings->orientation == HERO_PRINTING_REVERSE_PORTRAIT;

            settings->orientation = mode->dmOrientation == DMORIENT_LANDSCAPE ? (reverse ? HERO_PRINTING_REVERSE_LANDSCAPE : HERO_PRINTING_LANDSCAPE) : (reverse ? HERO_PRINTING_REVERSE_PORTRAIT : HERO_PRINTING_PORTRAIT);
        }
        if ((mode->dmFields & DM_DUPLEX) != 0) {
            settings->sides = mode->dmDuplex == DMDUP_VERTICAL ? HERO_PRINTING_TWO_SIDED_LONG_EDGE : mode->dmDuplex == DMDUP_HORIZONTAL ? HERO_PRINTING_TWO_SIDED_SHORT_EDGE : HERO_PRINTING_ONE_SIDED;
        }
        if ((mode->dmFields & DM_COLOR) != 0 && settings->color != HERO_PRINTING_COLOR_DEFAULT) {
            settings->color = mode->dmColor == DMCOLOR_COLOR ? HERO_PRINTING_COLOR : HERO_PRINTING_MONOCHROME;
        }
        if ((mode->dmFields & DM_PAPERSIZE) != 0 && settings->printer[0] != '\0') {
            /* The PWG name the printing member lists for the paper's size. */
            WCHAR name[HERO_PRINTING_NAME_MOST + 1];
            int papers;

            if (MultiByteToWideChar(CP_UTF8, 0, settings->printer, -1, name, HERO_PRINTING_NAME_MOST + 1) > 0 && (papers = DeviceCapabilitiesW(name, NULL, DC_PAPERS, NULL, NULL)) > 0) {
                WORD *ids = hero_layer_alloc((size_t)papers * sizeof(WORD));
                POINT *sizes = hero_layer_alloc((size_t)papers * sizeof(POINT));
                char *list = hero_layer_alloc(HERO_UI_PRINT_LIST_ROOM);
                int64_t width = -1, height = -1;

                if (ids != NULL && sizes != NULL && DeviceCapabilitiesW(name, NULL, DC_PAPERS, (LPWSTR)ids, NULL) == papers && DeviceCapabilitiesW(name, NULL, DC_PAPERSIZE, (LPWSTR)sizes, NULL) == papers) {
                    for (int i = 0; i < papers; i++) {
                        if (ids[i] == (WORD)mode->dmPaperSize) {
                            width = (int64_t)sizes[i].x * 10;
                            height = (int64_t)sizes[i].y * 10;
                        }
                    }
                }
                hero_layer_free(ids);
                hero_layer_free(sizes);
                {
                    int64_t got = width > 0 && list != NULL ? hero_printing_media(settings->printer, list, HERO_UI_PRINT_LIST_ROOM) : -1;

                    for (int64_t at = 0, field = 0; got > 0 && got <= HERO_UI_PRINT_LIST_ROOM && at < got; field++) {
                        const char *text = list + at;

                        if (field % HERO_PRINTING_MEDIA_FIELDS == 0) {
                            const char *label = text + strlen(text) + 1;
                            const char *w = label + strlen(label) + 1;
                            const char *h = w + strlen(w) + 1;

                            if (strtoll(w, NULL, 10) == width && strtoll(h, NULL, 10) == height) {
                                snprintf(settings->paper, sizeof settings->paper, "%s", text);
                            }
                        }
                        at += (int64_t)strlen(text) + 1;
                    }
                }
                hero_layer_free(list);
            }
        }
        GlobalUnlock(mode_block);
    }
}

/* Shows the print dialog (`page` 0) or the page setup (`page` 1) over
 * `window`, its fields first set from `*settings`, and waits for the person:
 * 1 with `*settings` what they chose, 0 where they cancelled, or a negated
 * kind of `hero_ui.h` with the refusal recorded. */
static int64_t hero_ui_print_win32_choose(HWND window, int64_t page, hero_ui_print_settings *settings) {
    hero_ui_print_settings given = *settings;
    HGLOBAL mode_block = NULL, names_block = NULL;
    bool chosen = false;

    if (!hero_ui_print_win32_blocks(&given, &mode_block, &names_block)) {
        return given.printer[0] != '\0' ? -HERO_UI_PRINT_NOT_FOUND : -HERO_UI_PRINT_OTHER;
    }
    if (page == 0) {
        PRINTPAGERANGE ranges[HERO_PRINTING_RANGES_MOST];
        PRINTDLGEXW dialog_box;
        HRESULT hr;

        memset(&dialog_box, 0, sizeof dialog_box);
        dialog_box.lStructSize = sizeof dialog_box;
        dialog_box.hwndOwner = window;
        dialog_box.hDevMode = mode_block;
        dialog_box.hDevNames = names_block;
        dialog_box.Flags = PD_NOSELECTION | PD_NOCURRENTPAGE | PD_USEDEVMODECOPIESANDCOLLATE;
        dialog_box.nMinPage = 1;
        dialog_box.nMaxPage = 0xFFFF;
        dialog_box.nMaxPageRanges = HERO_PRINTING_RANGES_MOST;
        dialog_box.lpPageRanges = ranges;
        dialog_box.nCopies = (DWORD)given.copies;
        dialog_box.nStartPage = START_PAGE_GENERAL;
        for (int64_t i = 0; i + 1 < given.range_count; i += 2) {
            ranges[i / 2].nFromPage = (DWORD)given.pages[i];
            ranges[i / 2].nToPage = (DWORD)given.pages[i + 1];
            dialog_box.nPageRanges++;
        }
        if (dialog_box.nPageRanges > 0) {
            dialog_box.Flags |= PD_PAGENUMS;
        }
        hr = PrintDlgExW(&dialog_box);
        mode_block = dialog_box.hDevMode;
        names_block = dialog_box.hDevNames;
        if (FAILED(hr)) {
            char words[160];

            snprintf(words, sizeof words, "the print dialog could not be shown (0x%08lx)", (unsigned long)hr);
            if (mode_block != NULL) {
                GlobalFree(mode_block);
            }
            if (names_block != NULL) {
                GlobalFree(names_block);
            }
            return -hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, words);
        }
        chosen = dialog_box.dwResultAction == PD_RESULT_PRINT;
        if (chosen) {
            given.range_count = 0;
            if ((dialog_box.Flags & PD_PAGENUMS) != 0) {
                for (DWORD i = 0; i < dialog_box.nPageRanges && i < HERO_PRINTING_RANGES_MOST; i++) {
                    given.pages[given.range_count++] = ranges[i].nFromPage;
                    given.pages[given.range_count++] = ranges[i].nToPage;
                }
            }
        }
    } else {
        PAGESETUPDLGW setup;

        memset(&setup, 0, sizeof setup);
        setup.lStructSize = sizeof setup;
        setup.hwndOwner = window;
        setup.hDevMode = mode_block;
        setup.hDevNames = names_block;
        setup.Flags = PSD_INHUNDREDTHSOFMILLIMETERS;
        chosen = PageSetupDlgW(&setup) != 0;
        mode_block = setup.hDevMode;
        names_block = setup.hDevNames;
        if (!chosen && CommDlgExtendedError() != 0) {
            char words[160];

            snprintf(words, sizeof words, "the page-setup dialog could not be shown (error 0x%lx)", (unsigned long)CommDlgExtendedError());
            if (mode_block != NULL) {
                GlobalFree(mode_block);
            }
            if (names_block != NULL) {
                GlobalFree(names_block);
            }
            return -hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, words);
        }
    }
    if (chosen) {
        hero_ui_print_win32_read_back(mode_block, names_block, &given);
        *settings = given;
    }
    if (mode_block != NULL) {
        GlobalFree(mode_block);
    }
    if (names_block != NULL) {
        GlobalFree(names_block);
    }
    return chosen ? 1 : 0;
}

int64_t hero_ui_print_win32_check(const uint8_t *settings, int64_t count) {
    hero_ui_print_settings given;
    WCHAR name[HERO_PRINTING_NAME_MOST + 1];
    HANDLE printer = NULL;

    if (!hero_ui_print_read(settings, count, &given)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "the dialog's settings are not the text hero_ui_print_win32.h names");
    }
    if (given.printer[0] == '\0') {
        return HERO_UI_PRINT_OK;
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, given.printer, -1, name, HERO_PRINTING_NAME_MOST + 1) <= 0) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "the printer's name is not UTF-8");
    }
    if (!OpenPrinterW(name, &printer, NULL)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_NOT_FOUND, "no printer of that name: the spooler could not open it");
    }
    (void)ClosePrinter(printer);
    return HERO_UI_PRINT_OK;
}

int64_t hero_ui_print_win32_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    hero_ui_print_settings given;
    char text[HERO_PRINTING_RANGES_MOST * 24 + 1024];
    int64_t chosen, length;

    if (window == NULL || push == NULL || (page != 0 && page != 1)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "no window, no queue, or a dialog of neither kind");
    }
    if (!hero_ui_print_read(settings, count, &given)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "the dialog's settings are not the text hero_ui_print_win32.h names");
    }
    chosen = hero_ui_print_win32_choose((HWND)window, page, &given);
    if (chosen < 0) {
        return -chosen;
    }
    if (chosen == 1) {
        length = hero_ui_print_write(&given, text, sizeof text);
        push(HERO_UI_PRINT_EVENT_DIALOG, dialog, 1, 0, (const uint8_t *)text, length < 0 ? 0 : length);
    } else {
        push(HERO_UI_PRINT_EVENT_DIALOG, dialog, 0, 0, NULL, 0);
    }
    return HERO_UI_PRINT_OK;
}

/* ---- A web view's page printed: WebView2's own `Print` ----
 *
 * WebView2's interfaces are reached by their vtables' slots, each counted in
 * the order Microsoft's published `WebView2.idl` gives (SDK
 * `Microsoft.Web.WebView2` 1.0.4370.26, read 2026-11-23, the same the web
 * view's arm, `hero_ui_web_win32.c`, was read from; nothing of it copied but
 * the orders and the identifiers): `ICoreWebView2_16::Print` is slot 113,
 * `ICoreWebView2_2::get_Environment` 67, `ICoreWebView2Environment6::
 * CreatePrintSettings` 14, and the print settings' setters as below. The
 * counting is held to the web view's arm's own table of `ICoreWebView2_4`,
 * 74 methods either way. UNRUN on Windows: the box was offline the day this
 * arm was written, so it is compile-checked alone. */

typedef void(STDMETHODCALLTYPE *hero_wv2p_slot)(void);
#define HERO_WV2P_SLOT(object, n) ((*(hero_wv2p_slot **)(object))[n])

static const GUID hero_iid_wv2_16 = {0x0eb34dc9, 0x9f91, 0x41e1, {0x86, 0x39, 0x95, 0xcd, 0x59, 0x43, 0x90, 0x6b}};
static const GUID hero_iid_wv2_environment6 = {0xe59ee362, 0xacbd, 0x4857, {0x9a, 0x8e, 0xd3, 0x64, 0x4d, 0x94, 0x59, 0xa9}};
static const GUID hero_iid_wv2_print_settings2 = {0xca7f0e1f, 0x3484, 0x41d1, {0x8c, 0x1a, 0x65, 0xcd, 0x44, 0xa6, 0x3f, 0x8d}};
static const GUID hero_iid_wv2_print_completed = {0x8fd80075, 0xed08, 0x42db, {0x85, 0x70, 0xf5, 0xd1, 0x49, 0x77, 0x46, 0x1e}};
static const GUID hero_iid_wv2_unknown = {0x00000000, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

static void hero_wv2p_release(void *object) {
    if (object != NULL) {
        ((ULONG(STDMETHODCALLTYPE *)(void *))HERO_WV2P_SLOT(object, 2))(object);
    }
}

static HRESULT hero_wv2p_query(void *object, const GUID *iid, void **out) {
    return ((HRESULT(STDMETHODCALLTYPE *)(void *, const GUID *, void **))HERO_WV2P_SLOT(object, 0))(object, iid, out);
}

/* The completion handler `Print` calls once the page is handed to the
 * system or could not be: a COM object of this file's, counted, which
 * queues the answer unless its print was ended first. Kept on the UI
 * thread's list until it is invoked, so `end` can mark it. */
typedef struct hero_wv2p_done hero_wv2p_done;

typedef struct {
    HRESULT(STDMETHODCALLTYPE *query)(hero_wv2p_done *self, const GUID *iid, void **out);
    ULONG(STDMETHODCALLTYPE *add)(hero_wv2p_done *self);
    ULONG(STDMETHODCALLTYPE *release)(hero_wv2p_done *self);
    HRESULT(STDMETHODCALLTYPE *invoke)(hero_wv2p_done *self, HRESULT error, int status);
} hero_wv2p_done_vtbl;

struct hero_wv2p_done {
    const hero_wv2p_done_vtbl *lpVtbl;
    LONG references;
    hero_wv2p_done *next;
    void *window;
    int64_t dialog;
    bool ended;
    hero_ui_print_settings given;
    void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t);
};

/* The web views' prints running on this thread, the UI thread's. */
static _Thread_local hero_wv2p_done *hero_wv2p_running;

static HRESULT STDMETHODCALLTYPE hero_wv2p_done_query(hero_wv2p_done *self, const GUID *iid, void **out) {
    if (IsEqualGUID(iid, &hero_iid_wv2_unknown) || IsEqualGUID(iid, &hero_iid_wv2_print_completed)) {
        *out = self;
        InterlockedIncrement(&self->references);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE hero_wv2p_done_add(hero_wv2p_done *self) {
    return (ULONG)InterlockedIncrement(&self->references);
}

static ULONG STDMETHODCALLTYPE hero_wv2p_done_release(hero_wv2p_done *self) {
    LONG left = InterlockedDecrement(&self->references);

    if (left == 0) {
        for (hero_wv2p_done **at = &hero_wv2p_running; *at != NULL; at = &(*at)->next) {
            if (*at == self) {
                *at = self->next;
                break;
            }
        }
        hero_layer_free(self);
    }
    return (ULONG)left;
}

/* COREWEBVIEW2_PRINT_STATUS_SUCCEEDED is 0. */
static HRESULT STDMETHODCALLTYPE hero_wv2p_done_invoke(hero_wv2p_done *self, HRESULT error, int status) {
    char text[HERO_PRINTING_RANGES_MOST * 24 + 1024];

    if (!self->ended) {
        self->ended = true;
        if (SUCCEEDED(error) && status == 0) {
            int64_t length = hero_ui_print_write(&self->given, text, sizeof text);

            self->push(HERO_UI_PRINT_EVENT_DIALOG, self->dialog, 1, 0, (const uint8_t *)text, length < 0 ? 0 : length);
        } else {
            self->push(HERO_UI_PRINT_EVENT_DIALOG, self->dialog, 0, 0, NULL, 0);
        }
    }
    return S_OK;
}

static const hero_wv2p_done_vtbl hero_wv2p_done_table = {hero_wv2p_done_query, hero_wv2p_done_add, hero_wv2p_done_release, hero_wv2p_done_invoke};

/* `text` as UTF-16 into `wide` of `room` units; false where it is no
 * UTF-8. */
static bool hero_wv2p_wide(const char *text, WCHAR *wide, int room) {
    return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, wide, room) > 0;
}

int64_t hero_ui_print_win32_web(void *view, void *window, int64_t dialog, int64_t ask, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    hero_ui_print_settings given;
    void *core = NULL, *environment = NULL, *environment6 = NULL, *base = NULL, *print = NULL;
    hero_wv2p_done *done;
    WCHAR wide[HERO_PRINTING_RANGES_MOST * 24 + 1];
    char ranges[HERO_PRINTING_RANGES_MOST * 24 + 1] = "";
    size_t used = 0;
    HRESULT hr;

    if (view == NULL || window == NULL || push == NULL || (ask != 0 && ask != 1)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "no web view, no window, no queue, or an ask of neither kind");
    }
    if (!hero_ui_print_read(settings, count, &given)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "the print's settings are not the text hero_ui_print_win32.h names");
    }
    if (ask == 1) {
        int64_t chosen = hero_ui_print_win32_choose((HWND)window, 0, &given);

        if (chosen < 0) {
            return -chosen;
        }
        if (chosen == 0) {
            push(HERO_UI_PRINT_EVENT_DIALOG, dialog, 0, 0, NULL, 0);
            return HERO_UI_PRINT_OK;
        }
    }
    if (given.printer[0] == '\0') {
        DWORD length = HERO_PRINTING_NAME_MOST + 1;
        WCHAR name[HERO_PRINTING_NAME_MOST + 1];

        if (GetDefaultPrinterW(name, &length)) {
            (void)WideCharToMultiByte(CP_UTF8, 0, name, -1, given.printer, (int)sizeof given.printer, NULL, NULL);
        }
    }
    hr = hero_wv2p_query(view, &hero_iid_wv2_16, &core);
    if (FAILED(hr)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_UNSUPPORTED, "this WebView2 runtime has no Print (ICoreWebView2_16, runtime 1.0.1518 or later)");
    }
    /* ICoreWebView2_2::get_Environment, slot 67. */
    hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void **))HERO_WV2P_SLOT(core, 67))(core, &environment);
    if (SUCCEEDED(hr)) {
        hr = hero_wv2p_query(environment, &hero_iid_wv2_environment6, &environment6);
    }
    if (SUCCEEDED(hr)) {
        /* ICoreWebView2Environment6::CreatePrintSettings, slot 14. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void **))HERO_WV2P_SLOT(environment6, 14))(environment6, &base);
    }
    if (SUCCEEDED(hr)) {
        hr = hero_wv2p_query(base, &hero_iid_wv2_print_settings2, &print);
    }
    if (FAILED(hr)) {
        hero_wv2p_release(base);
        hero_wv2p_release(environment6);
        hero_wv2p_release(environment);
        hero_wv2p_release(core);
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_UNSUPPORTED, "this WebView2 runtime makes no print settings of the second kind");
    }
    for (int64_t i = 0; i + 1 < given.range_count; i += 2) {
        int n = snprintf(ranges + used, sizeof ranges - used, "%s%lld-%lld", i == 0 ? "" : ",", (long long)given.pages[i], (long long)given.pages[i + 1]);

        used += n > 0 ? (size_t)n : 0;
    }
    /* The settings' setters: put_Orientation 4 (portrait 0, landscape 1),
     * put_PageRanges 30, put_Copies 34, put_Collation 36 (collated 1,
     * uncollated 2), put_ColorMode 38 (default 0, colour 1, grey 2),
     * put_Duplex 40 (one side 1, long edge 2, short edge 3),
     * put_PrinterName 44. The reverse orientations print as their quarter
     * turn: WebView2's settings have two. */
    (void)((HRESULT(STDMETHODCALLTYPE *)(void *, int))HERO_WV2P_SLOT(print, 4))(print, given.orientation == HERO_PRINTING_LANDSCAPE || given.orientation == HERO_PRINTING_REVERSE_LANDSCAPE ? 1 : 0);
    if (hero_wv2p_wide(ranges, wide, (int)(sizeof wide / sizeof wide[0]))) {
        (void)((HRESULT(STDMETHODCALLTYPE *)(void *, LPCWSTR))HERO_WV2P_SLOT(print, 30))(print, wide);
    }
    (void)((HRESULT(STDMETHODCALLTYPE *)(void *, INT32))HERO_WV2P_SLOT(print, 34))(print, (INT32)given.copies);
    (void)((HRESULT(STDMETHODCALLTYPE *)(void *, int))HERO_WV2P_SLOT(print, 36))(print, given.collate == 1 ? 1 : 2);
    (void)((HRESULT(STDMETHODCALLTYPE *)(void *, int))HERO_WV2P_SLOT(print, 38))(print, given.color == HERO_PRINTING_COLOR ? 1 : given.color == HERO_PRINTING_MONOCHROME ? 2 : 0);
    (void)((HRESULT(STDMETHODCALLTYPE *)(void *, int))HERO_WV2P_SLOT(print, 40))(print, (int)given.sides);
    if (given.printer[0] != '\0' && hero_wv2p_wide(given.printer, wide, (int)(sizeof wide / sizeof wide[0]))) {
        (void)((HRESULT(STDMETHODCALLTYPE *)(void *, LPCWSTR))HERO_WV2P_SLOT(print, 44))(print, wide);
    }
    done = hero_layer_alloc(sizeof *done);
    if (done == NULL) {
        hr = E_OUTOFMEMORY;
    } else {
        memset(done, 0, sizeof *done);
        done->lpVtbl = &hero_wv2p_done_table;
        done->references = 1;
        done->window = window;
        done->dialog = dialog;
        done->given = given;
        done->push = push;
        done->next = hero_wv2p_running;
        hero_wv2p_running = done;
        /* ICoreWebView2_16::Print, slot 113. */
        hr = ((HRESULT(STDMETHODCALLTYPE *)(void *, void *, void *))HERO_WV2P_SLOT(core, 113))(core, base, done);
        if (FAILED(hr)) {
            done->ended = true;
        }
        hero_wv2p_done_release(done);
    }
    hero_wv2p_release(print);
    hero_wv2p_release(base);
    hero_wv2p_release(environment6);
    hero_wv2p_release(environment);
    hero_wv2p_release(core);
    if (FAILED(hr)) {
        char words[160];

        snprintf(words, sizeof words, "WebView2 did not print the page (0x%08lx)", (unsigned long)hr);
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, words);
    }
    return HERO_UI_PRINT_OK;
}

/* Every web view's print over `window`, or every one, marked ended: its
 * completion, still to come, queues nothing. */
static void hero_ui_print_win32_end_web(void *window) {
    for (hero_wv2p_done *d = hero_wv2p_running; d != NULL; d = d->next) {
        if (window == NULL || d->window == window) {
            d->ended = true;
        }
    }
}

#else

int64_t hero_ui_print_win32_check(const uint8_t *settings, int64_t count) {
    (void)settings;
    (void)count;
    return hero_ui_print_win32_refuse(HERO_UI_PRINT_UNSUPPORTED, "the Win32 print dialogs stand on Windows alone");
}

int64_t hero_ui_print_win32_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    (void)window;
    (void)dialog;
    (void)page;
    (void)settings;
    (void)count;
    (void)push;
    return hero_ui_print_win32_refuse(HERO_UI_PRINT_UNSUPPORTED, "the Win32 print dialogs stand on Windows alone");
}

int64_t hero_ui_print_win32_web(void *view, void *window, int64_t dialog, int64_t ask, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    (void)view;
    (void)window;
    (void)dialog;
    (void)ask;
    (void)settings;
    (void)count;
    (void)push;
    return hero_ui_print_win32_refuse(HERO_UI_PRINT_UNSUPPORTED, "WebView2's printing stands on Windows alone");
}

static void hero_ui_print_win32_end_web(void *window) {
    (void)window;
}

#endif
