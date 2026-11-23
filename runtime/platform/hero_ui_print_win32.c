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

void hero_ui_print_win32_end(void *window) {
    (void)window;
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

int64_t hero_ui_print_win32_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    hero_ui_print_settings given;
    HGLOBAL mode_block = NULL, names_block = NULL;
    char text[HERO_PRINTING_RANGES_MOST * 24 + 1024];
    int64_t length;
    bool chosen = false;

    if (window == NULL || push == NULL || (page != 0 && page != 1)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "no window, no queue, or a dialog of neither kind");
    }
    if (!hero_ui_print_read(settings, count, &given)) {
        return hero_ui_print_win32_refuse(HERO_UI_PRINT_INVALID, "the dialog's settings are not the text hero_ui_print_win32.h names");
    }
    if (!hero_ui_print_win32_blocks(&given, &mode_block, &names_block)) {
        return given.printer[0] != '\0' ? HERO_UI_PRINT_NOT_FOUND : HERO_UI_PRINT_OTHER;
    }
    if (page == 0) {
        PRINTPAGERANGE ranges[HERO_PRINTING_RANGES_MOST];
        PRINTDLGEXW dialog_box;
        HRESULT hr;

        memset(&dialog_box, 0, sizeof dialog_box);
        dialog_box.lStructSize = sizeof dialog_box;
        dialog_box.hwndOwner = (HWND)window;
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
            return hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, words);
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
        setup.hwndOwner = (HWND)window;
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
            return hero_ui_print_win32_refuse(HERO_UI_PRINT_OTHER, words);
        }
    }
    if (chosen) {
        hero_ui_print_win32_read_back(mode_block, names_block, &given);
        length = hero_ui_print_write(&given, text, sizeof text);
        push(HERO_UI_PRINT_EVENT_DIALOG, dialog, 1, 0, (const uint8_t *)text, length < 0 ? 0 : length);
    } else {
        push(HERO_UI_PRINT_EVENT_DIALOG, dialog, 0, 0, NULL, 0);
    }
    if (mode_block != NULL) {
        GlobalFree(mode_block);
    }
    if (names_block != NULL) {
        GlobalFree(names_block);
    }
    return HERO_UI_PRINT_OK;
}

#else

int64_t hero_ui_print_win32_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    (void)window;
    (void)dialog;
    (void)page;
    (void)settings;
    (void)count;
    (void)push;
    return hero_ui_print_win32_refuse(HERO_UI_PRINT_UNSUPPORTED, "the Win32 print dialogs stand on Windows alone");
}

#endif
