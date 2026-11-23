/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_print_gtk.c — the GTK 4 arm of the print and page-setup
 * dialogs (`hero_ui_print_gtk.h` says what each function answers): GTK
 * 4.14's `GtkPrintDialog`, its `GtkPrintSettings` and `GtkPageSetup` first
 * set from the settings' text and read back once the person has answered. A
 * paper's PWG name is the one `hero_printing_media` lists for the printer,
 * matched to GTK's paper by its size, so the dialogs and `printing` name
 * every paper alike; a paper no list holds (a file printer's) is written
 * `<GTK's name>_<w>x<h>mm`.
 *
 * GTK'S FUNCTIONS are found with `dlsym` once, inside `pthread_once`, in the
 * GTK 4 `hero_ui_gtk.c` opened, as that member finds its own; where GTK is
 * older than 4.14 every show answers `HERO_UI_UNSUPPORTED` by name.
 *
 * THE DIALOGS SHOWN are kept per thread, the UI thread's. An injected answer
 * or an end does what the person does: it answers GTK's own print dialog,
 * the toplevel `GtkDialog` transient for the program's window, OK or
 * Cancel, and the setup's callback then queues the injected settings, a
 * cancel, or nothing for a dialog ended. NEVER BY THE SETUP'S CANCELLABLE:
 * `g_cancellable_cancel` on a shown `GtkPrintDialog`'s setup read address
 * 0x10 inside GTK 4.18.6 and the program died (measured 2026-11-23 under
 * ASan, Linux arm64), so no cancellable is given. Where the dialog stands
 * outside the process, a desktop portal's, no window answers, and an
 * injected answer is refused `HERO_UI_UNSUPPORTED` by name.
 *
 * OFF GTK (macOS, Windows) every function answers `HERO_UI_UNSUPPORTED`.
 *
 * Written from GTK's and GLib's reference documentation; no other library's
 * source.
 */

#include "hero_ui_print_gtk.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define HERO_UI_PRINT_OK 0
#define HERO_UI_PRINT_INVALID 1
#define HERO_UI_PRINT_UNSUPPORTED 4
#define HERO_UI_PRINT_NOT_FOUND 6
#define HERO_UI_PRINT_OTHER 8
#define HERO_UI_PRINT_EVENT_DIALOG 17

/* The words of the last refusal on this thread. */
static _Thread_local char hero_ui_print_gtk_said[512];

int64_t hero_ui_print_gtk_words(char *words, int64_t count) {
    size_t length = strlen(hero_ui_print_gtk_said);

    if (words == NULL || count <= 0) {
        return 0;
    }
    if ((int64_t)length >= count) {
        length = (size_t)count - 1;
    }
    memcpy(words, hero_ui_print_gtk_said, length);
    words[length] = '\0';
    return (int64_t)length;
}

static int64_t hero_ui_print_gtk_refuse(int64_t kind, const char *words) {
    snprintf(hero_ui_print_gtk_said, sizeof hero_ui_print_gtk_said, "%s", words);
    return kind;
}

#if !defined(_WIN32) && !defined(__APPLE__)

#include <dlfcn.h>
#include <pthread.h>

#include "hero_layer_alloc.h"
#include "hero_ui_print_text.h"

/* GLib's and GTK's types this member only hands back, and the two it reads. */
typedef void hero_gtk_object;
typedef struct {
    uint32_t domain;
    int code;
    char *message;
} hero_gtk_error;
typedef struct {
    int start;
    int end;
} hero_gtk_page_range;
typedef void (*hero_gtk_ready)(hero_gtk_object *source, hero_gtk_object *result, void *data);

/* GTK's unit of a millimetre and its constants this member writes. */
#define HERO_GTK_UNIT_MM 3
#define HERO_GTK_PRINT_PAGES_ALL 0
#define HERO_GTK_PRINT_PAGES_RANGES 2
#define HERO_GTK_DUPLEX_SIMPLEX 0
#define HERO_GTK_DUPLEX_HORIZONTAL 1
#define HERO_GTK_DUPLEX_VERTICAL 2

typedef struct {
    hero_gtk_object *(*dialog_new)(void);
    void (*dialog_set_print_settings)(hero_gtk_object *dialog, hero_gtk_object *settings);
    void (*dialog_set_page_setup)(hero_gtk_object *dialog, hero_gtk_object *setup);
    void (*dialog_setup)(hero_gtk_object *dialog, hero_gtk_object *parent, hero_gtk_object *cancellable, hero_gtk_ready ready, void *data);
    hero_gtk_object *(*dialog_setup_finish)(hero_gtk_object *dialog, hero_gtk_object *result, hero_gtk_error **error);
    hero_gtk_object *(*setup_get_print_settings)(hero_gtk_object *setup);
    hero_gtk_object *(*setup_get_page_setup)(hero_gtk_object *setup);
    void (*setup_unref)(hero_gtk_object *setup);
    hero_gtk_object *(*settings_new)(void);
    void (*settings_set_printer)(hero_gtk_object *settings, const char *printer);
    const char *(*settings_get_printer)(hero_gtk_object *settings);
    void (*settings_set_n_copies)(hero_gtk_object *settings, int copies);
    int (*settings_get_n_copies)(hero_gtk_object *settings);
    void (*settings_set_collate)(hero_gtk_object *settings, int collate);
    int (*settings_get_collate)(hero_gtk_object *settings);
    void (*settings_set_duplex)(hero_gtk_object *settings, int duplex);
    int (*settings_get_duplex)(hero_gtk_object *settings);
    void (*settings_set_use_color)(hero_gtk_object *settings, int color);
    int (*settings_get_use_color)(hero_gtk_object *settings);
    void (*settings_set_print_pages)(hero_gtk_object *settings, int pages);
    int (*settings_get_print_pages)(hero_gtk_object *settings);
    void (*settings_set_page_ranges)(hero_gtk_object *settings, hero_gtk_page_range *ranges, int count);
    hero_gtk_page_range *(*settings_get_page_ranges)(hero_gtk_object *settings, int *count);
    hero_gtk_object *(*page_setup_new)(void);
    void (*page_setup_set_paper_size)(hero_gtk_object *setup, hero_gtk_object *size);
    hero_gtk_object *(*page_setup_get_paper_size)(hero_gtk_object *setup);
    void (*page_setup_set_orientation)(hero_gtk_object *setup, int orientation);
    int (*page_setup_get_orientation)(hero_gtk_object *setup);
    hero_gtk_object *(*paper_size_new)(const char *name);
    void (*paper_size_free)(hero_gtk_object *size);
    const char *(*paper_size_get_name)(hero_gtk_object *size);
    double (*paper_size_get_width)(hero_gtk_object *size, int unit);
    double (*paper_size_get_height)(hero_gtk_object *size, int unit);
    hero_gtk_object *(*toplevels)(void);
    unsigned (*model_n_items)(hero_gtk_object *model);
    hero_gtk_object *(*model_get_item)(hero_gtk_object *model, unsigned position);
    hero_gtk_object *(*window_get_transient_for)(hero_gtk_object *window);
    uintptr_t (*dialog_get_type)(void);
    int (*type_check_instance_is_a)(hero_gtk_object *instance, uintptr_t type);
    void (*dialog_response)(hero_gtk_object *dialog, int response);
    void (*object_unref)(hero_gtk_object *object);
    void (*error_free)(hero_gtk_error *error);
    void (*g_free)(void *block);
    unsigned long (*signal_connect_data)(hero_gtk_object *instance, const char *signal, void (*handler)(void), void *data, void (*destroy)(void *, void *), int flags);
    unsigned (*signal_handlers_disconnect_matched)(hero_gtk_object *instance, int mask, unsigned signal, uint32_t detail, void *closure, void *func, void *data);
} hero_ui_print_gtk_functions;

/* WebKitGTK 6.0's print operation, found apart: a program with no web view
 * has no WebKit to find. */
typedef struct {
    hero_gtk_object *(*operation_new)(hero_gtk_object *web_view);
    void (*set_print_settings)(hero_gtk_object *operation, hero_gtk_object *settings);
    void (*set_page_setup)(hero_gtk_object *operation, hero_gtk_object *setup);
    hero_gtk_object *(*get_print_settings)(hero_gtk_object *operation);
    hero_gtk_object *(*get_page_setup)(hero_gtk_object *operation);
    int (*run_dialog)(hero_gtk_object *operation, hero_gtk_object *parent);
    void (*print)(hero_gtk_object *operation);
} hero_ui_print_gtk_webkit_functions;

/* Found once inside `pthread_once` and only read after it, as
 * `hero_ui_gtk.c`'s are: the functions are the process's. */
static hero_ui_print_gtk_functions hero_ui_print_gtk_found_functions;
static bool hero_ui_print_gtk_found;
static pthread_once_t hero_ui_print_gtk_once = PTHREAD_ONCE_INIT;

static bool hero_ui_print_gtk_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_UI_PRINT_GTK_FIND(field, name) ok = ok && hero_ui_print_gtk_find(gtk, name, &hero_ui_print_gtk_found_functions.field, sizeof hero_ui_print_gtk_found_functions.field)

static void hero_ui_print_gtk_load(void) {
    bool ok = true;
    void *gtk = dlopen("libgtk-4.so.1", RTLD_NOW | RTLD_LOCAL);

    if (gtk == NULL) {
        return;
    }
    HERO_UI_PRINT_GTK_FIND(dialog_new, "gtk_print_dialog_new");
    HERO_UI_PRINT_GTK_FIND(dialog_set_print_settings, "gtk_print_dialog_set_print_settings");
    HERO_UI_PRINT_GTK_FIND(dialog_set_page_setup, "gtk_print_dialog_set_page_setup");
    HERO_UI_PRINT_GTK_FIND(dialog_setup, "gtk_print_dialog_setup");
    HERO_UI_PRINT_GTK_FIND(dialog_setup_finish, "gtk_print_dialog_setup_finish");
    HERO_UI_PRINT_GTK_FIND(setup_get_print_settings, "gtk_print_setup_get_print_settings");
    HERO_UI_PRINT_GTK_FIND(setup_get_page_setup, "gtk_print_setup_get_page_setup");
    HERO_UI_PRINT_GTK_FIND(setup_unref, "gtk_print_setup_unref");
    HERO_UI_PRINT_GTK_FIND(settings_new, "gtk_print_settings_new");
    HERO_UI_PRINT_GTK_FIND(settings_set_printer, "gtk_print_settings_set_printer");
    HERO_UI_PRINT_GTK_FIND(settings_get_printer, "gtk_print_settings_get_printer");
    HERO_UI_PRINT_GTK_FIND(settings_set_n_copies, "gtk_print_settings_set_n_copies");
    HERO_UI_PRINT_GTK_FIND(settings_get_n_copies, "gtk_print_settings_get_n_copies");
    HERO_UI_PRINT_GTK_FIND(settings_set_collate, "gtk_print_settings_set_collate");
    HERO_UI_PRINT_GTK_FIND(settings_get_collate, "gtk_print_settings_get_collate");
    HERO_UI_PRINT_GTK_FIND(settings_set_duplex, "gtk_print_settings_set_duplex");
    HERO_UI_PRINT_GTK_FIND(settings_get_duplex, "gtk_print_settings_get_duplex");
    HERO_UI_PRINT_GTK_FIND(settings_set_use_color, "gtk_print_settings_set_use_color");
    HERO_UI_PRINT_GTK_FIND(settings_get_use_color, "gtk_print_settings_get_use_color");
    HERO_UI_PRINT_GTK_FIND(settings_set_print_pages, "gtk_print_settings_set_print_pages");
    HERO_UI_PRINT_GTK_FIND(settings_get_print_pages, "gtk_print_settings_get_print_pages");
    HERO_UI_PRINT_GTK_FIND(settings_set_page_ranges, "gtk_print_settings_set_page_ranges");
    HERO_UI_PRINT_GTK_FIND(settings_get_page_ranges, "gtk_print_settings_get_page_ranges");
    HERO_UI_PRINT_GTK_FIND(page_setup_new, "gtk_page_setup_new");
    HERO_UI_PRINT_GTK_FIND(page_setup_set_paper_size, "gtk_page_setup_set_paper_size");
    HERO_UI_PRINT_GTK_FIND(page_setup_get_paper_size, "gtk_page_setup_get_paper_size");
    HERO_UI_PRINT_GTK_FIND(page_setup_set_orientation, "gtk_page_setup_set_orientation");
    HERO_UI_PRINT_GTK_FIND(page_setup_get_orientation, "gtk_page_setup_get_orientation");
    HERO_UI_PRINT_GTK_FIND(paper_size_new, "gtk_paper_size_new");
    HERO_UI_PRINT_GTK_FIND(paper_size_free, "gtk_paper_size_free");
    HERO_UI_PRINT_GTK_FIND(paper_size_get_name, "gtk_paper_size_get_name");
    HERO_UI_PRINT_GTK_FIND(paper_size_get_width, "gtk_paper_size_get_width");
    HERO_UI_PRINT_GTK_FIND(paper_size_get_height, "gtk_paper_size_get_height");
    HERO_UI_PRINT_GTK_FIND(toplevels, "gtk_window_get_toplevels");
    HERO_UI_PRINT_GTK_FIND(model_n_items, "g_list_model_get_n_items");
    HERO_UI_PRINT_GTK_FIND(model_get_item, "g_list_model_get_item");
    HERO_UI_PRINT_GTK_FIND(window_get_transient_for, "gtk_window_get_transient_for");
    HERO_UI_PRINT_GTK_FIND(dialog_get_type, "gtk_dialog_get_type");
    HERO_UI_PRINT_GTK_FIND(type_check_instance_is_a, "g_type_check_instance_is_a");
    HERO_UI_PRINT_GTK_FIND(dialog_response, "gtk_dialog_response");
    HERO_UI_PRINT_GTK_FIND(object_unref, "g_object_unref");
    HERO_UI_PRINT_GTK_FIND(error_free, "g_error_free");
    HERO_UI_PRINT_GTK_FIND(g_free, "g_free");
    HERO_UI_PRINT_GTK_FIND(signal_connect_data, "g_signal_connect_data");
    HERO_UI_PRINT_GTK_FIND(signal_handlers_disconnect_matched, "g_signal_handlers_disconnect_matched");
    hero_ui_print_gtk_found = ok;
}

/* Found once inside `pthread_once` and only read after it, as GTK's above. */
static hero_ui_print_gtk_webkit_functions hero_ui_print_gtk_webkit;
static bool hero_ui_print_gtk_webkit_found;
static pthread_once_t hero_ui_print_gtk_webkit_once = PTHREAD_ONCE_INIT;

#define HERO_UI_PRINT_WEBKIT_FIND(field, name) ok = ok && hero_ui_print_gtk_find(webkit, name, &hero_ui_print_gtk_webkit.field, sizeof hero_ui_print_gtk_webkit.field)

static void hero_ui_print_gtk_webkit_load(void) {
    bool ok = true;
    void *webkit = dlopen("libwebkitgtk-6.0.so.4", RTLD_NOW | RTLD_LOCAL);

    if (webkit == NULL) {
        return;
    }
    HERO_UI_PRINT_WEBKIT_FIND(operation_new, "webkit_print_operation_new");
    HERO_UI_PRINT_WEBKIT_FIND(set_print_settings, "webkit_print_operation_set_print_settings");
    HERO_UI_PRINT_WEBKIT_FIND(set_page_setup, "webkit_print_operation_set_page_setup");
    HERO_UI_PRINT_WEBKIT_FIND(get_print_settings, "webkit_print_operation_get_print_settings");
    HERO_UI_PRINT_WEBKIT_FIND(get_page_setup, "webkit_print_operation_get_page_setup");
    HERO_UI_PRINT_WEBKIT_FIND(run_dialog, "webkit_print_operation_run_dialog");
    HERO_UI_PRINT_WEBKIT_FIND(print, "webkit_print_operation_print");
    hero_ui_print_gtk_webkit_found = ok;
}

/* A dialog shown, until its setup's callback runs. */
typedef struct hero_ui_print_gtk_shown {
    struct hero_ui_print_gtk_shown *next;
    int64_t dialog;
    void *window;
    hero_gtk_object *box;
    /* A web view's print: its WebKit operation, where `box` is NULL, and
     * whether it asked the person through GTK's print dialog. */
    hero_gtk_object *operation;
    bool asked;
    void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t);
    hero_ui_print_settings given;
    bool injected;
    bool cancelled;
    bool ended;
} hero_ui_print_gtk_shown;

/* The dialogs shown on this thread, the UI thread's own. */
static _Thread_local hero_ui_print_gtk_shown *hero_ui_print_gtk_list;

static hero_ui_print_gtk_shown *hero_ui_print_gtk_named(int64_t dialog) {
    for (hero_ui_print_gtk_shown *s = hero_ui_print_gtk_list; s != NULL; s = s->next) {
        if (s->dialog == dialog) {
            return s;
        }
    }
    return NULL;
}

static void hero_ui_print_gtk_unlink(hero_ui_print_gtk_shown *gone) {
    for (hero_ui_print_gtk_shown **at = &hero_ui_print_gtk_list; *at != NULL; at = &(*at)->next) {
        if (*at == gone) {
            *at = gone->next;
            return;
        }
    }
}

/* The PWG name of a paper `width` by `height` millimetres: the one the
 * printing member lists for `printer` within a tenth of a millimetre, else
 * `<gtk name>_<w>x<h>mm`. */
static void hero_ui_print_gtk_pwg(const char *printer, const char *gtk_name, double width, double height, char *out, size_t room) {
    char *list = printer[0] != '\0' ? hero_layer_alloc(65536) : NULL;
    int64_t got = list != NULL ? hero_printing_media(printer, list, 65536) : -1;

    snprintf(out, room, "%s_%gx%gmm", gtk_name, width, height);
    for (int64_t at = 0, field = 0; got > 0 && got <= 65536 && at < got; field++) {
        const char *text = list + at;

        if (field % HERO_PRINTING_MEDIA_FIELDS == 0) {
            const char *label = text + strlen(text) + 1;
            const char *across = label + strlen(label) + 1;
            const char *down = across + strlen(across) + 1;
            double w = (double)strtoll(across, NULL, 10) / 100.0;
            double h = (double)strtoll(down, NULL, 10) / 100.0;

            if (w - width < 0.15 && width - w < 0.15 && h - height < 0.15 && height - h < 0.15) {
                snprintf(out, room, "%s", text);
                break;
            }
        }
        at += (int64_t)strlen(text) + 1;
    }
    hero_layer_free(list);
}

/* What GTK's `settings` and `page` hold, read into `chosen`, which holds the
 * given settings first: the colour where the program left it to the
 * printer stays so. */
static void hero_ui_print_gtk_read_back(hero_gtk_object *settings, hero_gtk_object *page, hero_ui_print_settings *chosen) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    hero_ui_print_settings given = *chosen;


    if (settings != NULL) {
        const char *printer = f->settings_get_printer(settings);
        int duplex = f->settings_get_duplex(settings);

        snprintf(chosen->printer, sizeof chosen->printer, "%s", printer != NULL ? printer : "");
        chosen->copies = f->settings_get_n_copies(settings);
        chosen->copies = chosen->copies < 1 ? 1 : chosen->copies > HERO_PRINTING_COPIES_MOST ? HERO_PRINTING_COPIES_MOST : chosen->copies;
        chosen->collate = f->settings_get_collate(settings) ? 1 : 0;
        chosen->sides = duplex == HERO_GTK_DUPLEX_HORIZONTAL ? HERO_PRINTING_TWO_SIDED_LONG_EDGE : duplex == HERO_GTK_DUPLEX_VERTICAL ? HERO_PRINTING_TWO_SIDED_SHORT_EDGE : HERO_PRINTING_ONE_SIDED;
        if (given.color != HERO_PRINTING_COLOR_DEFAULT) {
            chosen->color = f->settings_get_use_color(settings) ? HERO_PRINTING_COLOR : HERO_PRINTING_MONOCHROME;
        }
        chosen->range_count = 0;
        if (f->settings_get_print_pages(settings) == HERO_GTK_PRINT_PAGES_RANGES) {
            int count = 0;
            hero_gtk_page_range *ranges = f->settings_get_page_ranges(settings, &count);

            for (int i = 0; ranges != NULL && i < count && i < HERO_PRINTING_RANGES_MOST; i++) {
                chosen->pages[chosen->range_count++] = (int64_t)ranges[i].start + 1;
                chosen->pages[chosen->range_count++] = (int64_t)ranges[i].end + 1;
            }
            f->g_free(ranges);
        }
    }
    if (page != NULL) {
        int orientation = f->page_setup_get_orientation(page);
        hero_gtk_object *size = f->page_setup_get_paper_size(page);

        chosen->orientation = orientation == 1 ? HERO_PRINTING_LANDSCAPE : orientation == 2 ? HERO_PRINTING_REVERSE_PORTRAIT : orientation == 3 ? HERO_PRINTING_REVERSE_LANDSCAPE : HERO_PRINTING_PORTRAIT;
        if (size != NULL) {
            const char *name = f->paper_size_get_name(size);

            hero_ui_print_gtk_pwg(chosen->printer, name != NULL ? name : "custom", f->paper_size_get_width(size, HERO_GTK_UNIT_MM), f->paper_size_get_height(size, HERO_GTK_UNIT_MM), chosen->paper, sizeof chosen->paper);
        }
    }
}

/* Queues the answer of `shown`, once, and forgets it. */
static void hero_ui_print_gtk_answered(hero_gtk_object *source, hero_gtk_object *result, void *data) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    hero_ui_print_gtk_shown *shown = data;
    hero_gtk_error *error = NULL;
    hero_gtk_object *setup = f->dialog_setup_finish(source, result, &error);
    char text[HERO_PRINTING_RANGES_MOST * 24 + 1024];
    int64_t length;

    if (error != NULL) {
        f->error_free(error);
    }
    if (!shown->ended && shown->injected) {
        length = hero_ui_print_write(&shown->given, text, sizeof text);
        shown->push(HERO_UI_PRINT_EVENT_DIALOG, shown->dialog, 1, 0, (const uint8_t *)text, length < 0 ? 0 : length);
    } else if (!shown->ended && (setup == NULL || shown->cancelled)) {
        shown->push(HERO_UI_PRINT_EVENT_DIALOG, shown->dialog, 0, 0, NULL, 0);
    } else if (!shown->ended) {
        hero_ui_print_settings chosen = shown->given;
        hero_ui_print_gtk_read_back(f->setup_get_print_settings(setup), f->setup_get_page_setup(setup), &chosen);
        length = hero_ui_print_write(&chosen, text, sizeof text);
        shown->push(HERO_UI_PRINT_EVENT_DIALOG, shown->dialog, 1, 0, (const uint8_t *)text, length < 0 ? 0 : length);
    }
    if (setup != NULL) {
        f->setup_unref(setup);
    }
    hero_ui_print_gtk_unlink(shown);
    f->object_unref(shown->box);
    hero_layer_free(shown);
}

/* GTK's print settings and page setup of `given`, two new objects the
 * caller unreferences. */
static void hero_ui_print_gtk_objects(const hero_ui_print_settings *given, hero_gtk_object **settings_out, hero_gtk_object **page_out) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    hero_gtk_object *print_settings, *page_setup;

    print_settings = f->settings_new();
    page_setup = f->page_setup_new();
    if (given->printer[0] != '\0') {
        f->settings_set_printer(print_settings, given->printer);
    }
    f->settings_set_n_copies(print_settings, (int)given->copies);
    f->settings_set_collate(print_settings, given->collate == 1);
    f->settings_set_duplex(print_settings, given->sides == HERO_PRINTING_TWO_SIDED_LONG_EDGE ? HERO_GTK_DUPLEX_HORIZONTAL : given->sides == HERO_PRINTING_TWO_SIDED_SHORT_EDGE ? HERO_GTK_DUPLEX_VERTICAL : HERO_GTK_DUPLEX_SIMPLEX);
    if (given->color != HERO_PRINTING_COLOR_DEFAULT) {
        f->settings_set_use_color(print_settings, given->color == HERO_PRINTING_COLOR);
    }
    if (given->range_count > 0) {
        hero_gtk_page_range ranges[HERO_PRINTING_RANGES_MOST];

        for (int64_t i = 0; i + 1 < given->range_count; i += 2) {
            ranges[i / 2].start = (int)given->pages[i] - 1;
            ranges[i / 2].end = (int)given->pages[i + 1] - 1;
        }
        f->settings_set_print_pages(print_settings, HERO_GTK_PRINT_PAGES_RANGES);
        f->settings_set_page_ranges(print_settings, ranges, (int)(given->range_count / 2));
    } else {
        f->settings_set_print_pages(print_settings, HERO_GTK_PRINT_PAGES_ALL);
    }
    f->page_setup_set_orientation(page_setup, given->orientation == HERO_PRINTING_LANDSCAPE ? 1 : given->orientation == HERO_PRINTING_REVERSE_PORTRAIT ? 2 : given->orientation == HERO_PRINTING_REVERSE_LANDSCAPE ? 3 : 0);
    if (given->paper[0] != '\0') {
        hero_gtk_object *size = f->paper_size_new(given->paper);

        if (size != NULL) {
            f->page_setup_set_paper_size(page_setup, size);
            f->paper_size_free(size);
        }
    }
    *settings_out = print_settings;
    *page_out = page_setup;
}

int64_t hero_ui_print_gtk_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    hero_ui_print_gtk_shown *shown;
    hero_gtk_object *print_settings, *page_setup;

    if (window == NULL || push == NULL || (page != 0 && page != 1)) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "no window, no queue, or a dialog of neither kind");
    }
    (void)pthread_once(&hero_ui_print_gtk_once, hero_ui_print_gtk_load);
    if (!hero_ui_print_gtk_found) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_UNSUPPORTED, "GTK 4.14's GtkPrintDialog is not in this GTK, which is older");
    }
    if (hero_ui_print_gtk_named(dialog) != NULL) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "that dialog is shown already");
    }
    shown = hero_layer_alloc(sizeof *shown);
    if (shown == NULL) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_OTHER, "the system has no memory for the dialog");
    }
    memset(shown, 0, sizeof *shown);
    if (!hero_ui_print_read(settings, count, &shown->given)) {
        hero_layer_free(shown);
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "the dialog's settings are not the text hero_ui_print_win32.h names");
    }
    shown->dialog = dialog;
    shown->window = window;
    shown->push = push;
    shown->box = f->dialog_new();
    hero_ui_print_gtk_objects(&shown->given, &print_settings, &page_setup);
    f->dialog_set_print_settings(shown->box, print_settings);
    f->dialog_set_page_setup(shown->box, page_setup);
    f->object_unref(print_settings);
    f->object_unref(page_setup);
    shown->next = hero_ui_print_gtk_list;
    hero_ui_print_gtk_list = shown;
    f->dialog_setup(shown->box, window, NULL, hero_ui_print_gtk_answered, shown);
    return HERO_UI_PRINT_OK;
}

/* GTK's own dialog over `window`: the toplevel `GtkDialog` transient for it,
 * answered `response` (GTK's OK -5, Cancel -6); false where none stands. */
static bool hero_ui_print_gtk_respond(void *window, int response) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    hero_gtk_object *tops = f->toplevels();
    unsigned count = tops != NULL ? f->model_n_items(tops) : 0;
    hero_gtk_object *found = NULL;

    for (unsigned i = 0; i < count && found == NULL; i++) {
        hero_gtk_object *top = f->model_get_item(tops, i);

        if (top != NULL && f->window_get_transient_for(top) == window && f->type_check_instance_is_a(top, f->dialog_get_type())) {
            found = top;
        } else if (top != NULL) {
            f->object_unref(top);
        }
    }
    if (found == NULL) {
        return false;
    }
    f->dialog_response(found, response);
    f->object_unref(found);
    return true;
}

/* The end of a web view's print, by WebKit's `finished` (`printed`) or
 * `failed`, or the person's Cancel: queued once, unless the print was ended
 * first, and forgotten. */
static void hero_ui_print_gtk_web_done(hero_ui_print_gtk_shown *shown, bool printed) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    char text[HERO_PRINTING_RANGES_MOST * 24 + 1024];

    if (!shown->ended && printed) {
        hero_ui_print_settings chosen = shown->given;
        int64_t length;

        hero_ui_print_gtk_read_back(hero_ui_print_gtk_webkit.get_print_settings(shown->operation), hero_ui_print_gtk_webkit.get_page_setup(shown->operation), &chosen);
        length = hero_ui_print_write(&chosen, text, sizeof text);
        shown->push(HERO_UI_PRINT_EVENT_DIALOG, shown->dialog, 1, 0, (const uint8_t *)text, length < 0 ? 0 : length);
    } else if (!shown->ended) {
        shown->push(HERO_UI_PRINT_EVENT_DIALOG, shown->dialog, 0, 0, NULL, 0);
    }
    /* G_SIGNAL_MATCH_DATA: every handler this print connected. */
    (void)f->signal_handlers_disconnect_matched(shown->operation, 16, 0, 0, NULL, NULL, shown);
    hero_ui_print_gtk_unlink(shown);
    f->object_unref(shown->operation);
    hero_layer_free(shown);
}

static void hero_ui_print_gtk_web_finished(hero_gtk_object *operation, void *data) {
    (void)operation;
    hero_ui_print_gtk_web_done(data, true);
}

static void hero_ui_print_gtk_web_failed(hero_gtk_object *operation, hero_gtk_error *error, void *data) {
    (void)operation;
    (void)error;
    hero_ui_print_gtk_web_done(data, false);
}

/* Whether `shown` is still on this thread's list: a print may have ended
 * inside a call that ran GTK's loop. */
static bool hero_ui_print_gtk_listed(const hero_ui_print_gtk_shown *shown) {
    for (const hero_ui_print_gtk_shown *s = hero_ui_print_gtk_list; s != NULL; s = s->next) {
        if (s == shown) {
            return true;
        }
    }
    return false;
}

int64_t hero_ui_print_gtk_web(void *view, void *window, int64_t dialog, int64_t ask, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    const hero_ui_print_gtk_functions *f = &hero_ui_print_gtk_found_functions;
    hero_ui_print_gtk_shown *shown;
    hero_gtk_object *print_settings, *page_setup;

    if (view == NULL || window == NULL || push == NULL || (ask != 0 && ask != 1)) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "no web view, no window, no queue, or an ask of neither kind");
    }
    (void)pthread_once(&hero_ui_print_gtk_once, hero_ui_print_gtk_load);
    (void)pthread_once(&hero_ui_print_gtk_webkit_once, hero_ui_print_gtk_webkit_load);
    if (!hero_ui_print_gtk_found || !hero_ui_print_gtk_webkit_found) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_UNSUPPORTED, "WebKitGTK 6.0's print operation or GTK 4.14 is not on this machine");
    }
    if (hero_ui_print_gtk_named(dialog) != NULL) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "that dialog is shown already");
    }
    shown = hero_layer_alloc(sizeof *shown);
    if (shown == NULL) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_OTHER, "the system has no memory for the print");
    }
    memset(shown, 0, sizeof *shown);
    if (!hero_ui_print_read(settings, count, &shown->given)) {
        hero_layer_free(shown);
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "the print's settings are not the text hero_ui_print_win32.h names");
    }
    shown->dialog = dialog;
    shown->window = window;
    shown->push = push;
    shown->asked = ask == 1;
    shown->operation = hero_ui_print_gtk_webkit.operation_new(view);
    hero_ui_print_gtk_objects(&shown->given, &print_settings, &page_setup);
    hero_ui_print_gtk_webkit.set_print_settings(shown->operation, print_settings);
    hero_ui_print_gtk_webkit.set_page_setup(shown->operation, page_setup);
    f->object_unref(print_settings);
    f->object_unref(page_setup);
    (void)f->signal_connect_data(shown->operation, "finished", (void (*)(void))hero_ui_print_gtk_web_finished, shown, NULL, 0);
    (void)f->signal_connect_data(shown->operation, "failed", (void (*)(void))hero_ui_print_gtk_web_failed, shown, NULL, 0);
    shown->next = hero_ui_print_gtk_list;
    hero_ui_print_gtk_list = shown;
    if (ask == 1) {
        /* GTK's print dialog, run until the person answers (GTK's loop runs
         * meanwhile): 0 prints, which ends by `finished` or `failed`, and
         * 1 is the person's Cancel. */
        int response = hero_ui_print_gtk_webkit.run_dialog(shown->operation, window);

        if (response != 0 && hero_ui_print_gtk_listed(shown)) {
            hero_ui_print_gtk_web_done(shown, false);
        }
    } else {
        hero_ui_print_gtk_webkit.print(shown->operation);
    }
    return HERO_UI_PRINT_OK;
}

int64_t hero_ui_print_gtk_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count) {
    hero_ui_print_gtk_shown *shown = hero_ui_print_gtk_named(dialog);
    hero_ui_print_settings chosen = {0};

    if (shown == NULL || shown->ended || shown->injected || shown->cancelled) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_NOT_FOUND, "no such print dialog stands");
    }
    if (value != 0 && value != 1) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "an answer is 1 or 0");
    }
    if (shown->operation != NULL && !shown->asked) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_NOT_FOUND, "that web view's page prints with no dialog to answer");
    }
    if (value == 1 && !hero_ui_print_read(settings, count, &chosen)) {
        return hero_ui_print_gtk_refuse(HERO_UI_PRINT_INVALID, "the answer's settings are not the text hero_ui_print_win32.h names");
    }
    /* The answer is recorded before the dialog is answered: GTK runs the
     * setup's callback inside the response, which queues it and forgets
     * `shown` (measured under ASan: a write after it, a use after free). */
    {
        hero_ui_print_settings before = shown->given;
        void *over = shown->window;

        if (value == 1) {
            shown->given = chosen;
            shown->injected = true;
        } else {
            shown->cancelled = true;
        }
        if (!hero_ui_print_gtk_respond(over, value == 1 ? -5 : -6)) {
            shown->given = before;
            shown->injected = false;
            shown->cancelled = false;
            return hero_ui_print_gtk_refuse(HERO_UI_PRINT_UNSUPPORTED, "the print dialog stands outside the program (a desktop portal's), and no answer reaches it but the person's");
        }
    }
    return HERO_UI_PRINT_OK;
}

void hero_ui_print_gtk_end(void *window) {
    /* A response may run the callback, which unlinks and frees its entry,
     * so the walk begins again after each; an entry marked ended is passed
     * over, so it ends. */
    for (bool again = true; again;) {
        again = false;
        for (hero_ui_print_gtk_shown *s = hero_ui_print_gtk_list; s != NULL; s = s->next) {
            if (!s->ended && (window == NULL || s->window == window)) {
                void *over = s->window;

                s->ended = true;
                (void)hero_ui_print_gtk_respond(over, -6);
                again = true;
                break;
            }
        }
    }
}

#else

int64_t hero_ui_print_gtk_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    (void)window;
    (void)dialog;
    (void)page;
    (void)settings;
    (void)count;
    (void)push;
    return hero_ui_print_gtk_refuse(HERO_UI_PRINT_UNSUPPORTED, "the GTK 4 print dialogs stand where GTK draws the windows, on neither macOS nor Windows");
}

int64_t hero_ui_print_gtk_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count) {
    (void)dialog;
    (void)value;
    (void)settings;
    (void)count;
    return hero_ui_print_gtk_refuse(HERO_UI_PRINT_UNSUPPORTED, "the GTK 4 print dialogs stand where GTK draws the windows, on neither macOS nor Windows");
}

void hero_ui_print_gtk_end(void *window) {
    (void)window;
}

int64_t hero_ui_print_gtk_web(void *view, void *window, int64_t dialog, int64_t ask, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t)) {
    (void)view;
    (void)window;
    (void)dialog;
    (void)ask;
    (void)settings;
    (void)count;
    (void)push;
    return hero_ui_print_gtk_refuse(HERO_UI_PRINT_UNSUPPORTED, "WebKitGTK's printing stands where GTK draws the windows, on neither macOS nor Windows");
}

#endif
