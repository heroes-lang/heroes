/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_gtk.c: the native user interface's arm on every system
 * that is neither Apple's nor Windows, GTK 4's own widgets, one member of the
 * archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero`), compiled to nothing on macOS and Windows.
 * `hero_ui.c` calls its functions, THE ARM of `platform/hero_ui_arm.h`, on
 * the UI thread with every argument checked; it calls back what `hero_ui.c`
 * keeps for both C arms, through the same header.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled. */

#include "hero_ui_gtk.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"

#if !defined(_WIN32) && !defined(__APPLE__)
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "hero_ui_web_gtk.h"

/* ==========================================================================
 * THE GTK ARM: GTK 4's own widgets, `GtkButton`, `GtkEntry`, `GtkTextView`,
 * `GtkCheckButton`, `GtkListBox`, `GtkDropDown`, `GtkProgressBar`,
 * `GtkScale`, `GtkPicture`, `GtkLabel`, and GMenu's models in a
 * `GtkPopoverMenuBar`, on every system that is neither Apple's nor Windows.
 *
 * WHY `dlopen`, as `hero_keyring.c` says of libsecret: the layer's archive is
 * compiled for every program a machine builds, so a member that included
 * `<gtk/gtk.h>` would stop every build on a machine without GTK's headers,
 * and one linked `-lgtk-4` every program on one without the library. So the
 * member names no header of GTK, GLib or GIO: it declares the functions it
 * calls with their own signatures (GTK 4's ABI is kept under the one
 * `libgtk-4.so.1`, which needs GLib, GObject, GIO and GDK and so loads them)
 * and the two structures it reads, `GError` and `GtkRequisition`, as their
 * headers lay them out; the first call opens the library once, `RTLD_LOCAL`,
 * and finds every function by `dlsym` on its handle, which searches the
 * library and the libraries it needs. A function GTK 4.0 has missing is the
 * library missing: then `native_here` answers 0 and `open` refuses
 * `HERO_UI_UNSUPPORTED`, naming the library. GTK 4.10's dialogs,
 * `GtkAlertDialog` and `GtkFileDialog`, are used where the library has them
 * and GTK 4.0's `GtkMessageDialog` and `GtkFileChooserNative` where it does
 * not, so the least GTK this arm opens on is 4.0. The library is never
 * closed. Every number below was read from GTK 4.18.6's headers by a program
 * compiled against them, 2026-11-22.
 *
 * WHERE A DISPLAY IS NAMED: `native_here` answers 1 only where `DISPLAY` or
 * `WAYLAND_DISPLAY` names one and the library is there; `open` then asks
 * `gtk_init_check`, which opens it, and refuses `UNSUPPORTED` where it
 * cannot. GTK is told not to set the process's locale first
 * (`gtk_disable_setlocale`), since the runtime prints numbers in C's.
 *
 * EVERY CALL IS ON THE UI THREAD, the process's main thread (THE THREAD),
 * the one GTK's own documentation asks for; `native_here` alone may come from
 * another, and the library is found under a `pthread_once`.
 *
 * A WINDOW is a `GtkWindow` holding a box: the menu bar, shown while a menu
 * is, and the content, an overlay whose child is a drawing area that draws
 * nothing and whose overlay is a `GtkFixed` the widgets stand in at their
 * points. The overlay's own size is its drawing area's, so the widgets'
 * places never keep the person from making the window smaller, and the
 * drawing area's `resize` is the content's new size. GTK 4 sizes a shown
 * window by its default size, which counts the menu bar and a title bar GTK
 * draws itself; each is measured as the window's size less its content's,
 * and a content that took another size than asked while the window took the
 * one asked is given it again.
 *
 * WHAT THE PERSON DOES arrives as each widget's own signal (`clicked`,
 * `toggled`, `changed`, `selected-rows-changed`, `notify::selected`,
 * `value-changed`), a menu item's action's `activate`, a window's
 * `close-request` and its content's `resize`; what the program sets is
 * muted. A signal carries the widget's number, and the widget it came from is
 * asked of the table, so a stale one finds none.
 *
 * WHAT `inject` DOES is what the person's act does: `clicked` emitted on a
 * button, a checkbox's or a radio item's `active` set, an edit's text
 * replaced by the editable's own delete and insert, a list's row selected, a
 * drop-down's item chosen, a scale's value set, a menu item's action
 * activated, `gtk_window_close` (the window manager's close button's own
 * path), and a new default size.
 *
 * A COMBO BOX is a `GtkDropDown`, which always holds a choice while it holds
 * items (its selection chooses its first item itself, measured under Xvfb on
 * GTK 4.18.6). `hero_ui.h` says a combo box may hold none (after `set_items`,
 * and `set_value` of -1), so while it does, its list starts with an empty row,
 * chosen, which is taken away when an item is chosen.
 *
 * MENUS: a window's menus are a GMenu of submenus, each a GMenu of sections
 * between its separators, rebuilt when one of its items changes, with a
 * hidden item left out; each item is an action of the window's group,
 * `hero.item<number>`, disabled where the item, its menu or either's being
 * shown forbids it, and its shortcut a `GtkShortcut` of the window's
 * controller naming that action, shown in the menu by `gtk_accelerator_name`.
 * A label's `_` is written `__`, since a menu reads one `_` as a key's mark.
 *
 * THE LOOP: `pump` runs GLib's default main context until one of its
 * dispatches queues an event or its time passes, waking at its deadline by a
 * timeout source of its own.
 *
 * Written from GTK 4's, GLib's and GIO's documentation and headers; no other
 * library's source.
 * ========================================================================== */

/* GLib's `GError`: a domain, a code and the words. */
typedef struct hero_ui_gerror {
    uint32_t domain;
    int code;
    char *message;
} hero_ui_gerror;

/* GTK's `GtkRequisition`. */
typedef struct hero_ui_gtk_size {
    int width;
    int height;
} hero_ui_gtk_size;

/* GTK's `GtkTextIter`, 80 bytes on every 64-bit target GTK 4.18.6 builds for;
 * held in more room, aligned as its pointers are. */
typedef union hero_ui_gtk_iter {
    void *align;
    unsigned char bytes[256];
} hero_ui_gtk_iter;

/* GTK's numbers this arm hands it, each read from GTK 4.18.6's headers. */
#define HERO_GTK_ORIENTATION_HORIZONTAL 0
#define HERO_GTK_ORIENTATION_VERTICAL 1
#define HERO_GTK_SELECTION_SINGLE 1
#define HERO_GTK_SELECTION_MULTIPLE 3
#define HERO_GTK_POLICY_AUTOMATIC 1
#define HERO_GTK_POLICY_NEVER 2
#define HERO_GTK_WRAP_WORD_CHAR 3
#define HERO_GDK_SHIFT_MASK 1u
#define HERO_GDK_CONTROL_MASK 4u
#define HERO_GDK_ALT_MASK 8u
#define HERO_GTK_MESSAGE_INFO 0
#define HERO_GTK_MESSAGE_WARNING 1
#define HERO_GTK_MESSAGE_ERROR 3
#define HERO_GTK_BUTTONS_NONE 0
#define HERO_GTK_RESPONSE_ACCEPT (-3)
#define HERO_GTK_RESPONSE_OK (-5)
#define HERO_GTK_RESPONSE_CANCEL (-6)
#define HERO_GTK_RESPONSE_YES (-8)
#define HERO_GTK_RESPONSE_NO (-9)
#define HERO_GTK_FILE_CHOOSER_ACTION_OPEN 0
#define HERO_GTK_FILE_CHOOSER_ACTION_SAVE 1
#define HERO_GTK_DIALOG_MODAL 1
#define HERO_GTK_DIALOG_DESTROY_WITH_PARENT 2
#define HERO_GTK_DIALOG_ERROR_CANCELLED 1
#define HERO_GTK_DIALOG_ERROR_DISMISSED 2
#define HERO_GDK_MEMORY_R8G8B8A8 5
#define HERO_GTK_INVALID_LIST_POSITION 0xFFFFFFFFu
/* GDK's key values of the keys `hero_ui.h` names. */
#define HERO_GDK_KEY_F1 0xFFBEu

/* The most a slider or a progress bar is told apart by, where its range is
 * wider than a double holds every integer of: 2^53. */
#define HERO_UI_GTK_EXACT (INT64_C(1) << 53)

/* The functions, found once by `hero_ui_gtk_load` under `hero_ui_gtk_once`,
 * and read only after it. Each pointer is written with the C types GTK's
 * header gives its function: a pointer of GTK's or GLib's as `void *`,
 * `gboolean` and `gint` as `int`, `guint` as `unsigned`, `gsize` and `GType`
 * as `size_t`, `gdouble` as `double`, `gfloat` as `float`. */
typedef void (*hero_ui_gtk_callback)(void);
typedef int (*hero_ui_gtk_source)(void *data);
typedef void (*hero_ui_gtk_ready)(void *source, void *result, void *data);

typedef struct hero_ui_gtk_functions {
    int (*init_check)(void);
    void (*disable_setlocale)(void);
    void *(*window_new)(void);
    void (*window_set_title)(void *window, const char *title);
    void (*window_set_child)(void *window, void *child);
    void (*window_set_default_size)(void *window, int width, int height);
    void (*window_set_resizable)(void *window, int resizable);
    void (*window_present)(void *window);
    void (*window_close)(void *window);
    void (*window_destroy)(void *window);
    void (*widget_set_visible)(void *widget, int visible);
    int (*widget_get_visible)(void *widget);
    void (*widget_set_sensitive)(void *widget, int sensitive);
    int (*widget_is_sensitive)(void *widget);
    void (*widget_set_size_request)(void *widget, int width, int height);
    void (*widget_get_size_request)(void *widget, int *width, int *height);
    void (*widget_get_preferred_size)(void *widget, hero_ui_gtk_size *least, hero_ui_gtk_size *natural);
    void (*widget_set_hexpand)(void *widget, int expand);
    void (*widget_set_vexpand)(void *widget, int expand);
    int (*widget_get_width)(void *widget);
    int (*widget_get_height)(void *widget);
    void (*widget_realize)(void *widget);
    int (*widget_get_realized)(void *widget);
    void (*widget_add_controller)(void *widget, void *controller);
    void (*widget_insert_action_group)(void *widget, const char *prefix, void *group);
    void *(*box_new)(int orientation, int spacing);
    void (*box_append)(void *box, void *child);
    void *(*overlay_new)(void);
    void (*overlay_set_child)(void *overlay, void *child);
    void (*overlay_add_overlay)(void *overlay, void *child);
    void (*overlay_set_clip_overlay)(void *overlay, void *child, int clip);
    void *(*drawing_area_new)(void);
    void *(*fixed_new)(void);
    void (*fixed_put)(void *fixed, void *child, double x, double y);
    void (*fixed_move)(void *fixed, void *child, double x, double y);
    void (*fixed_remove)(void *fixed, void *child);
    void *(*label_new)(const char *text);
    void (*label_set_text)(void *label, const char *text);
    void (*label_set_xalign)(void *label, float align);
    void (*label_set_yalign)(void *label, float align);
    void *(*button_new_with_label)(const char *text);
    void (*button_set_label)(void *button, const char *text);
    void *(*entry_new)(void);
    void (*editable_set_text)(void *editable, const char *text);
    const char *(*editable_get_text)(void *editable);
    void (*editable_set_position)(void *editable, int position);
    void (*editable_delete_text)(void *editable, int start, int end);
    void (*editable_insert_text)(void *editable, const char *text, int length, int *position);
    int (*editable_get_editable)(void *editable);
    void *(*scrolled_window_new)(void);
    void (*scrolled_window_set_child)(void *scrolled, void *child);
    void (*scrolled_window_set_policy)(void *scrolled, int across, int down);
    void (*scrolled_window_set_has_frame)(void *scrolled, int frame);
    void *(*text_view_new)(void);
    void *(*text_view_get_buffer)(void *view);
    void (*text_view_set_wrap_mode)(void *view, int mode);
    int (*text_view_get_editable)(void *view);
    void (*text_buffer_set_text)(void *buffer, const char *text, int length);
    void (*text_buffer_get_start_iter)(void *buffer, hero_ui_gtk_iter *iter);
    void (*text_buffer_get_end_iter)(void *buffer, hero_ui_gtk_iter *iter);
    char *(*text_buffer_get_text)(void *buffer, const hero_ui_gtk_iter *start, const hero_ui_gtk_iter *end, int hidden);
    void (*text_buffer_place_cursor)(void *buffer, const hero_ui_gtk_iter *where);
    void (*text_buffer_begin_user_action)(void *buffer);
    void (*text_buffer_end_user_action)(void *buffer);
    int (*text_buffer_delete_interactive)(void *buffer, hero_ui_gtk_iter *start, hero_ui_gtk_iter *end, int editable);
    int (*text_buffer_insert_interactive_at_cursor)(void *buffer, const char *text, int length, int editable);
    void *(*check_button_new_with_label)(const char *text);
    void (*check_button_set_label)(void *button, const char *text);
    void (*check_button_set_active)(void *button, int active);
    int (*check_button_get_active)(void *button);
    void (*check_button_set_group)(void *button, void *group);
    void *(*list_box_new)(void);
    void (*list_box_set_selection_mode)(void *box, int mode);
    void (*list_box_append)(void *box, void *child);
    void (*list_box_remove)(void *box, void *child);
    void *(*list_box_get_row_at_index)(void *box, int index);
    void (*list_box_select_row)(void *box, void *row);
    void (*list_box_unselect_all)(void *box);
    int (*list_box_row_is_selected)(void *row);
    void *(*string_list_new)(const char *const *strings);
    void (*string_list_append)(void *list, const char *text);
    void (*string_list_remove)(void *list, unsigned position);
    void (*string_list_splice)(void *list, unsigned position, unsigned removals, const char *const *additions);
    void *(*drop_down_new)(void *model, void *expression);
    void (*drop_down_set_model)(void *drop, void *model);
    void (*drop_down_set_selected)(void *drop, unsigned position);
    unsigned (*drop_down_get_selected)(void *drop);
    void *(*progress_bar_new)(void);
    void (*progress_bar_set_fraction)(void *bar, double fraction);
    void *(*scale_new_with_range)(int orientation, double least, double most, double step);
    void (*scale_set_draw_value)(void *scale, int draw);
    void (*range_set_range)(void *range, double least, double most);
    void (*range_set_value)(void *range, double value);
    double (*range_get_value)(void *range);
    void (*range_set_increments)(void *range, double step, double page);
    void *(*picture_new)(void);
    void (*picture_set_paintable)(void *picture, void *paintable);
    void *(*popover_menu_bar_new_from_model)(void *model);
    void *(*shortcut_controller_new)(void);
    void (*shortcut_controller_add_shortcut)(void *controller, void *shortcut);
    void (*shortcut_controller_remove_shortcut)(void *controller, void *shortcut);
    void *(*shortcut_new)(void *trigger, void *action);
    void *(*keyval_trigger_new)(unsigned keyval, unsigned modifiers);
    void *(*named_action_new)(const char *name);
    char *(*accelerator_name)(unsigned keyval, unsigned modifiers);
    void *(*message_dialog_new)(void *parent, int flags, int type, int buttons, const char *format, ...);
    void (*message_dialog_format_secondary_text)(void *dialog, const char *format, ...);
    void *(*dialog_add_button)(void *dialog, const char *text, int response);
    void (*dialog_set_default_response)(void *dialog, int response);
    void *(*file_chooser_native_new)(const char *title, void *parent, int action, const char *accept, const char *cancel);
    void (*native_dialog_show)(void *dialog);
    void (*native_dialog_set_modal)(void *dialog, int modal);
    void (*file_chooser_set_select_multiple)(void *chooser, int multiple);
    void (*file_chooser_set_current_name)(void *chooser, const char *name);
    void (*file_chooser_add_filter)(void *chooser, void *filter);
    void *(*file_chooser_get_files)(void *chooser);
    void *(*file_filter_new)(void);
    void (*file_filter_set_name)(void *filter, const char *name);
    void (*file_filter_add_pattern)(void *filter, const char *pattern);
    size_t (*file_filter_get_type)(void);
    uint32_t (*dialog_error_quark)(void);
    void *(*memory_texture_new)(int width, int height, int format, void *bytes, size_t stride);
    int (*main_context_iteration)(void *context, int may_block);
    unsigned (*timeout_add)(unsigned interval, hero_ui_gtk_source function, void *data);
    unsigned (*idle_add)(hero_ui_gtk_source function, void *data);
    int (*source_remove)(unsigned tag);
    /* GLib's `g_free`, under the library's own name: a release of GLib's
     * memory, never C's `free` (the net's `runtime` suite reads a call to
     * `free` anywhere outside `parts/alloc.c` as a second allocation point). */
    void (*g_free)(void *memory);
    void *(*bytes_new)(const void *data, size_t size);
    void (*bytes_unref)(void *bytes);
    const char *(*dgettext)(const char *domain, const char *words);
    void (*error_free)(hero_ui_gerror *error);
    void *(*variant_new_string)(const char *text);
    unsigned long (*signal_connect_data)(void *instance, const char *signal, hero_ui_gtk_callback handler, void *data, void *destroy, int flags);
    void (*signal_emit_by_name)(void *instance, const char *signal, ...);
    void (*object_unref)(void *object);
    void *(*menu_new)(void);
    void (*menu_remove_all)(void *menu);
    void (*menu_append_section)(void *menu, const char *label, void *section);
    void (*menu_append_submenu)(void *menu, const char *label, void *submenu);
    void (*menu_append_item)(void *menu, void *item);
    void *(*menu_item_new)(const char *label, const char *action);
    void (*menu_item_set_attribute_value)(void *item, const char *attribute, void *value);
    void *(*simple_action_new)(const char *name, void *parameter_type);
    void (*simple_action_set_enabled)(void *action, int enabled);
    void *(*simple_action_group_new)(void);
    void (*action_map_add_action)(void *map, void *action);
    void (*action_map_remove_action)(void *map, const char *name);
    void (*action_activate)(void *action, void *parameter);
    unsigned (*list_model_get_n_items)(void *list);
    void *(*list_model_get_item)(void *list, unsigned position);
    char *(*file_get_path)(void *file);
    void *(*list_store_new)(size_t type);
    void (*list_store_append)(void *store, void *item);
    /* GTK 4.10's dialogs, NULL where the library is older. */
    void *(*alert_dialog_new)(const char *format, ...);
    void (*alert_dialog_set_detail)(void *dialog, const char *detail);
    void (*alert_dialog_set_buttons)(void *dialog, const char *const *labels);
    void (*alert_dialog_set_cancel_button)(void *dialog, int button);
    void (*alert_dialog_set_default_button)(void *dialog, int button);
    void (*alert_dialog_set_modal)(void *dialog, int modal);
    void (*alert_dialog_choose)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    int (*alert_dialog_choose_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void *(*file_dialog_new)(void);
    void (*file_dialog_set_title)(void *dialog, const char *title);
    void (*file_dialog_set_modal)(void *dialog, int modal);
    void (*file_dialog_set_initial_name)(void *dialog, const char *name);
    void (*file_dialog_set_filters)(void *dialog, void *filters);
    void (*file_dialog_open)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_open_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void (*file_dialog_open_multiple)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_open_multiple_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void (*file_dialog_save)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_save_finish)(void *dialog, void *result, hero_ui_gerror **error);
    /* GTK 4.4's suffix filter, which matches its letters in either case. */
    void (*file_filter_add_suffix)(void *filter, const char *suffix);
} hero_ui_gtk_functions;

static hero_ui_gtk_functions hero_ui_gtk;
static bool hero_ui_gtk_found;
static pthread_once_t hero_ui_gtk_once = PTHREAD_ONCE_INIT;

#define HERO_UI_GTK_LIBRARY "libgtk-4.so.1"

/* One function by name into `*slot`; false where the library has none. C11
 * has no conversion between an object pointer and a function pointer, so the
 * address is copied by its bytes, which POSIX makes the same for both
 * (`dlsym`'s own page). */
static bool hero_ui_gtk_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_UI_GTK_NEED(field, name) ok = hero_ui_gtk_find(library, name, &hero_ui_gtk.field, sizeof hero_ui_gtk.field) && ok
#define HERO_UI_GTK_MAY(field, name) (void)hero_ui_gtk_find(library, name, &hero_ui_gtk.field, sizeof hero_ui_gtk.field)

static void hero_ui_gtk_load(void) {
    bool ok = true;
    void *library = dlopen(HERO_UI_GTK_LIBRARY, RTLD_NOW | RTLD_LOCAL);

    if (library == NULL) {
        return;
    }
    HERO_UI_GTK_NEED(init_check, "gtk_init_check");
    HERO_UI_GTK_NEED(disable_setlocale, "gtk_disable_setlocale");
    HERO_UI_GTK_NEED(window_new, "gtk_window_new");
    HERO_UI_GTK_NEED(window_set_title, "gtk_window_set_title");
    HERO_UI_GTK_NEED(window_set_child, "gtk_window_set_child");
    HERO_UI_GTK_NEED(window_set_default_size, "gtk_window_set_default_size");
    HERO_UI_GTK_NEED(window_set_resizable, "gtk_window_set_resizable");
    HERO_UI_GTK_NEED(window_present, "gtk_window_present");
    HERO_UI_GTK_NEED(window_close, "gtk_window_close");
    HERO_UI_GTK_NEED(window_destroy, "gtk_window_destroy");
    HERO_UI_GTK_NEED(widget_set_visible, "gtk_widget_set_visible");
    HERO_UI_GTK_NEED(widget_get_visible, "gtk_widget_get_visible");
    HERO_UI_GTK_NEED(widget_set_sensitive, "gtk_widget_set_sensitive");
    HERO_UI_GTK_NEED(widget_is_sensitive, "gtk_widget_is_sensitive");
    HERO_UI_GTK_NEED(widget_set_size_request, "gtk_widget_set_size_request");
    HERO_UI_GTK_NEED(widget_get_size_request, "gtk_widget_get_size_request");
    HERO_UI_GTK_NEED(widget_get_preferred_size, "gtk_widget_get_preferred_size");
    HERO_UI_GTK_NEED(widget_set_hexpand, "gtk_widget_set_hexpand");
    HERO_UI_GTK_NEED(widget_set_vexpand, "gtk_widget_set_vexpand");
    HERO_UI_GTK_NEED(widget_get_width, "gtk_widget_get_width");
    HERO_UI_GTK_NEED(widget_get_height, "gtk_widget_get_height");
    HERO_UI_GTK_NEED(widget_realize, "gtk_widget_realize");
    HERO_UI_GTK_NEED(widget_get_realized, "gtk_widget_get_realized");
    HERO_UI_GTK_NEED(widget_add_controller, "gtk_widget_add_controller");
    HERO_UI_GTK_NEED(widget_insert_action_group, "gtk_widget_insert_action_group");
    HERO_UI_GTK_NEED(box_new, "gtk_box_new");
    HERO_UI_GTK_NEED(box_append, "gtk_box_append");
    HERO_UI_GTK_NEED(overlay_new, "gtk_overlay_new");
    HERO_UI_GTK_NEED(overlay_set_child, "gtk_overlay_set_child");
    HERO_UI_GTK_NEED(overlay_add_overlay, "gtk_overlay_add_overlay");
    HERO_UI_GTK_NEED(overlay_set_clip_overlay, "gtk_overlay_set_clip_overlay");
    HERO_UI_GTK_NEED(drawing_area_new, "gtk_drawing_area_new");
    HERO_UI_GTK_NEED(fixed_new, "gtk_fixed_new");
    HERO_UI_GTK_NEED(fixed_put, "gtk_fixed_put");
    HERO_UI_GTK_NEED(fixed_move, "gtk_fixed_move");
    HERO_UI_GTK_NEED(fixed_remove, "gtk_fixed_remove");
    HERO_UI_GTK_NEED(label_new, "gtk_label_new");
    HERO_UI_GTK_NEED(label_set_text, "gtk_label_set_text");
    HERO_UI_GTK_NEED(label_set_xalign, "gtk_label_set_xalign");
    HERO_UI_GTK_NEED(label_set_yalign, "gtk_label_set_yalign");
    HERO_UI_GTK_NEED(button_new_with_label, "gtk_button_new_with_label");
    HERO_UI_GTK_NEED(button_set_label, "gtk_button_set_label");
    HERO_UI_GTK_NEED(entry_new, "gtk_entry_new");
    HERO_UI_GTK_NEED(editable_set_text, "gtk_editable_set_text");
    HERO_UI_GTK_NEED(editable_get_text, "gtk_editable_get_text");
    HERO_UI_GTK_NEED(editable_set_position, "gtk_editable_set_position");
    HERO_UI_GTK_NEED(editable_delete_text, "gtk_editable_delete_text");
    HERO_UI_GTK_NEED(editable_insert_text, "gtk_editable_insert_text");
    HERO_UI_GTK_NEED(editable_get_editable, "gtk_editable_get_editable");
    HERO_UI_GTK_NEED(scrolled_window_new, "gtk_scrolled_window_new");
    HERO_UI_GTK_NEED(scrolled_window_set_child, "gtk_scrolled_window_set_child");
    HERO_UI_GTK_NEED(scrolled_window_set_policy, "gtk_scrolled_window_set_policy");
    HERO_UI_GTK_NEED(scrolled_window_set_has_frame, "gtk_scrolled_window_set_has_frame");
    HERO_UI_GTK_NEED(text_view_new, "gtk_text_view_new");
    HERO_UI_GTK_NEED(text_view_get_buffer, "gtk_text_view_get_buffer");
    HERO_UI_GTK_NEED(text_view_set_wrap_mode, "gtk_text_view_set_wrap_mode");
    HERO_UI_GTK_NEED(text_view_get_editable, "gtk_text_view_get_editable");
    HERO_UI_GTK_NEED(text_buffer_set_text, "gtk_text_buffer_set_text");
    HERO_UI_GTK_NEED(text_buffer_get_start_iter, "gtk_text_buffer_get_start_iter");
    HERO_UI_GTK_NEED(text_buffer_get_end_iter, "gtk_text_buffer_get_end_iter");
    HERO_UI_GTK_NEED(text_buffer_get_text, "gtk_text_buffer_get_text");
    HERO_UI_GTK_NEED(text_buffer_place_cursor, "gtk_text_buffer_place_cursor");
    HERO_UI_GTK_NEED(text_buffer_begin_user_action, "gtk_text_buffer_begin_user_action");
    HERO_UI_GTK_NEED(text_buffer_end_user_action, "gtk_text_buffer_end_user_action");
    HERO_UI_GTK_NEED(text_buffer_delete_interactive, "gtk_text_buffer_delete_interactive");
    HERO_UI_GTK_NEED(text_buffer_insert_interactive_at_cursor, "gtk_text_buffer_insert_interactive_at_cursor");
    HERO_UI_GTK_NEED(check_button_new_with_label, "gtk_check_button_new_with_label");
    HERO_UI_GTK_NEED(check_button_set_label, "gtk_check_button_set_label");
    HERO_UI_GTK_NEED(check_button_set_active, "gtk_check_button_set_active");
    HERO_UI_GTK_NEED(check_button_get_active, "gtk_check_button_get_active");
    HERO_UI_GTK_NEED(check_button_set_group, "gtk_check_button_set_group");
    HERO_UI_GTK_NEED(list_box_new, "gtk_list_box_new");
    HERO_UI_GTK_NEED(list_box_set_selection_mode, "gtk_list_box_set_selection_mode");
    HERO_UI_GTK_NEED(list_box_append, "gtk_list_box_append");
    HERO_UI_GTK_NEED(list_box_remove, "gtk_list_box_remove");
    HERO_UI_GTK_NEED(list_box_get_row_at_index, "gtk_list_box_get_row_at_index");
    HERO_UI_GTK_NEED(list_box_select_row, "gtk_list_box_select_row");
    HERO_UI_GTK_NEED(list_box_unselect_all, "gtk_list_box_unselect_all");
    HERO_UI_GTK_NEED(list_box_row_is_selected, "gtk_list_box_row_is_selected");
    HERO_UI_GTK_NEED(string_list_new, "gtk_string_list_new");
    HERO_UI_GTK_NEED(string_list_append, "gtk_string_list_append");
    HERO_UI_GTK_NEED(string_list_remove, "gtk_string_list_remove");
    HERO_UI_GTK_NEED(string_list_splice, "gtk_string_list_splice");
    HERO_UI_GTK_NEED(drop_down_new, "gtk_drop_down_new");
    HERO_UI_GTK_NEED(drop_down_set_model, "gtk_drop_down_set_model");
    HERO_UI_GTK_NEED(drop_down_set_selected, "gtk_drop_down_set_selected");
    HERO_UI_GTK_NEED(drop_down_get_selected, "gtk_drop_down_get_selected");
    HERO_UI_GTK_NEED(progress_bar_new, "gtk_progress_bar_new");
    HERO_UI_GTK_NEED(progress_bar_set_fraction, "gtk_progress_bar_set_fraction");
    HERO_UI_GTK_NEED(scale_new_with_range, "gtk_scale_new_with_range");
    HERO_UI_GTK_NEED(scale_set_draw_value, "gtk_scale_set_draw_value");
    HERO_UI_GTK_NEED(range_set_range, "gtk_range_set_range");
    HERO_UI_GTK_NEED(range_set_value, "gtk_range_set_value");
    HERO_UI_GTK_NEED(range_get_value, "gtk_range_get_value");
    HERO_UI_GTK_NEED(range_set_increments, "gtk_range_set_increments");
    HERO_UI_GTK_NEED(picture_new, "gtk_picture_new");
    HERO_UI_GTK_NEED(picture_set_paintable, "gtk_picture_set_paintable");
    HERO_UI_GTK_NEED(popover_menu_bar_new_from_model, "gtk_popover_menu_bar_new_from_model");
    HERO_UI_GTK_NEED(shortcut_controller_new, "gtk_shortcut_controller_new");
    HERO_UI_GTK_NEED(shortcut_controller_add_shortcut, "gtk_shortcut_controller_add_shortcut");
    HERO_UI_GTK_NEED(shortcut_controller_remove_shortcut, "gtk_shortcut_controller_remove_shortcut");
    HERO_UI_GTK_NEED(shortcut_new, "gtk_shortcut_new");
    HERO_UI_GTK_NEED(keyval_trigger_new, "gtk_keyval_trigger_new");
    HERO_UI_GTK_NEED(named_action_new, "gtk_named_action_new");
    HERO_UI_GTK_NEED(accelerator_name, "gtk_accelerator_name");
    HERO_UI_GTK_NEED(message_dialog_new, "gtk_message_dialog_new");
    HERO_UI_GTK_NEED(message_dialog_format_secondary_text, "gtk_message_dialog_format_secondary_text");
    HERO_UI_GTK_NEED(dialog_add_button, "gtk_dialog_add_button");
    HERO_UI_GTK_NEED(dialog_set_default_response, "gtk_dialog_set_default_response");
    HERO_UI_GTK_NEED(file_chooser_native_new, "gtk_file_chooser_native_new");
    HERO_UI_GTK_NEED(native_dialog_show, "gtk_native_dialog_show");
    HERO_UI_GTK_NEED(native_dialog_set_modal, "gtk_native_dialog_set_modal");
    HERO_UI_GTK_NEED(file_chooser_set_select_multiple, "gtk_file_chooser_set_select_multiple");
    HERO_UI_GTK_NEED(file_chooser_set_current_name, "gtk_file_chooser_set_current_name");
    HERO_UI_GTK_NEED(file_chooser_add_filter, "gtk_file_chooser_add_filter");
    HERO_UI_GTK_NEED(file_chooser_get_files, "gtk_file_chooser_get_files");
    HERO_UI_GTK_NEED(file_filter_new, "gtk_file_filter_new");
    HERO_UI_GTK_NEED(file_filter_set_name, "gtk_file_filter_set_name");
    HERO_UI_GTK_NEED(file_filter_add_pattern, "gtk_file_filter_add_pattern");
    HERO_UI_GTK_NEED(file_filter_get_type, "gtk_file_filter_get_type");
    HERO_UI_GTK_NEED(dialog_error_quark, "gtk_dialog_error_quark");
    HERO_UI_GTK_NEED(memory_texture_new, "gdk_memory_texture_new");
    HERO_UI_GTK_NEED(main_context_iteration, "g_main_context_iteration");
    HERO_UI_GTK_NEED(timeout_add, "g_timeout_add");
    HERO_UI_GTK_NEED(idle_add, "g_idle_add");
    HERO_UI_GTK_NEED(source_remove, "g_source_remove");
    HERO_UI_GTK_NEED(g_free, "g_free");
    HERO_UI_GTK_NEED(bytes_new, "g_bytes_new");
    HERO_UI_GTK_NEED(bytes_unref, "g_bytes_unref");
    HERO_UI_GTK_NEED(dgettext, "g_dgettext");
    HERO_UI_GTK_NEED(error_free, "g_error_free");
    HERO_UI_GTK_NEED(variant_new_string, "g_variant_new_string");
    HERO_UI_GTK_NEED(signal_connect_data, "g_signal_connect_data");
    HERO_UI_GTK_NEED(signal_emit_by_name, "g_signal_emit_by_name");
    HERO_UI_GTK_NEED(object_unref, "g_object_unref");
    HERO_UI_GTK_NEED(menu_new, "g_menu_new");
    HERO_UI_GTK_NEED(menu_remove_all, "g_menu_remove_all");
    HERO_UI_GTK_NEED(menu_append_section, "g_menu_append_section");
    HERO_UI_GTK_NEED(menu_append_submenu, "g_menu_append_submenu");
    HERO_UI_GTK_NEED(menu_append_item, "g_menu_append_item");
    HERO_UI_GTK_NEED(menu_item_new, "g_menu_item_new");
    HERO_UI_GTK_NEED(menu_item_set_attribute_value, "g_menu_item_set_attribute_value");
    HERO_UI_GTK_NEED(simple_action_new, "g_simple_action_new");
    HERO_UI_GTK_NEED(simple_action_set_enabled, "g_simple_action_set_enabled");
    HERO_UI_GTK_NEED(simple_action_group_new, "g_simple_action_group_new");
    HERO_UI_GTK_NEED(action_map_add_action, "g_action_map_add_action");
    HERO_UI_GTK_NEED(action_map_remove_action, "g_action_map_remove_action");
    HERO_UI_GTK_NEED(action_activate, "g_action_activate");
    HERO_UI_GTK_NEED(list_model_get_n_items, "g_list_model_get_n_items");
    HERO_UI_GTK_NEED(list_model_get_item, "g_list_model_get_item");
    HERO_UI_GTK_NEED(file_get_path, "g_file_get_path");
    HERO_UI_GTK_NEED(list_store_new, "g_list_store_new");
    HERO_UI_GTK_NEED(list_store_append, "g_list_store_append");
    HERO_UI_GTK_MAY(alert_dialog_new, "gtk_alert_dialog_new");
    HERO_UI_GTK_MAY(alert_dialog_set_detail, "gtk_alert_dialog_set_detail");
    HERO_UI_GTK_MAY(alert_dialog_set_buttons, "gtk_alert_dialog_set_buttons");
    HERO_UI_GTK_MAY(alert_dialog_set_cancel_button, "gtk_alert_dialog_set_cancel_button");
    HERO_UI_GTK_MAY(alert_dialog_set_default_button, "gtk_alert_dialog_set_default_button");
    HERO_UI_GTK_MAY(alert_dialog_set_modal, "gtk_alert_dialog_set_modal");
    HERO_UI_GTK_MAY(alert_dialog_choose, "gtk_alert_dialog_choose");
    HERO_UI_GTK_MAY(alert_dialog_choose_finish, "gtk_alert_dialog_choose_finish");
    HERO_UI_GTK_MAY(file_dialog_new, "gtk_file_dialog_new");
    HERO_UI_GTK_MAY(file_dialog_set_title, "gtk_file_dialog_set_title");
    HERO_UI_GTK_MAY(file_dialog_set_modal, "gtk_file_dialog_set_modal");
    HERO_UI_GTK_MAY(file_dialog_set_initial_name, "gtk_file_dialog_set_initial_name");
    HERO_UI_GTK_MAY(file_dialog_set_filters, "gtk_file_dialog_set_filters");
    HERO_UI_GTK_MAY(file_dialog_open, "gtk_file_dialog_open");
    HERO_UI_GTK_MAY(file_dialog_open_finish, "gtk_file_dialog_open_finish");
    HERO_UI_GTK_MAY(file_dialog_open_multiple, "gtk_file_dialog_open_multiple");
    HERO_UI_GTK_MAY(file_dialog_open_multiple_finish, "gtk_file_dialog_open_multiple_finish");
    HERO_UI_GTK_MAY(file_dialog_save, "gtk_file_dialog_save");
    HERO_UI_GTK_MAY(file_dialog_save_finish, "gtk_file_dialog_save_finish");
    HERO_UI_GTK_MAY(file_filter_add_suffix, "gtk_file_filter_add_suffix");
    hero_ui_gtk_found = ok;
}

/* Whether the library and every function of GTK 4.0 this arm calls are
 * there, found once. */
static bool hero_ui_gtk_loaded(void) {
    (void)pthread_once(&hero_ui_gtk_once, hero_ui_gtk_load);
    return hero_ui_gtk_found;
}

struct hero_ui_native {
    int64_t id;
    int64_t kind;
    int64_t parent;
    /* The window it stands in; a window's own number, a menu item's its
     * menu's window. */
    int64_t window;
    /* What stands in the window's content: the control, the scrolled window
     * around a list or a text area, a radio group's own `GtkFixed`; a
     * window's `GtkWindow`. */
    void *widget;
    /* The list box, the text view, the drop-down and its strings, a text
     * area's buffer. */
    void *inner;
    void *buffer;
    void *strings;
    /* Shown as the program asks; a placed widget shows once placed too. */
    bool visible;
    bool enabled;
    bool placed;
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
    /* A radio group's items and where each was placed, four numbers an item,
     * a width below 0 where it was not. */
    void **radios;
    int64_t *radio_places;
    int64_t radio_count;
    /* A list's or a combo box's items, and whether a combo box's list starts
     * with the empty row of no choice (A COMBO BOX). */
    int64_t items;
    bool placeholder;
    /* A slider's or a progress bar's range and value, exact. */
    int64_t least;
    int64_t most;
    int64_t value;
    /* A picture's size in pixels. */
    int64_t image_width;
    int64_t image_height;
    /* A window's content, its drawing area, its menu bar and its model, its
     * actions and its shortcuts. */
    void *fixed;
    void *area;
    void *bar;
    void *bar_model;
    void *actions;
    void *shortcuts;
    /* A window's content size and least size in points, the default size it
     * was given last, and whether a new size of the content is the person's
     * act `inject` gave. */
    int64_t content_width;
    int64_t content_height;
    int64_t least_width;
    int64_t least_height;
    int asked_width;
    int asked_height;
    bool resize_injected;
    /* A menu's model; a window's menus, a menu's items and separators, in
     * the order made. */
    void *menu_model;
    hero_ui_ids order;
    /* A menu's or an item's text as given. */
    char *label;
    /* An item's action and shortcut, its key value and the keys held. */
    void *action;
    void *shortcut;
    unsigned keyval;
    unsigned held;
};

/* Above 0 while `inject` acts, so a slider moved to where it stands is told. */
static _Thread_local int64_t hero_ui_gtk_injecting;
/* The paths the last file dialog answered, each followed by one zero byte. */
static _Thread_local uint8_t *hero_ui_gtk_paths;
static _Thread_local int64_t hero_ui_gtk_paths_count;

/* -- Words, numbers and text ------------------------------------------------ */

/* `count` bytes of checked UTF-8 at `text` with a zero after them, in a
 * block of `hero_layer_alloc`, or NULL; each `_` doubled where `marks`. */
static char *hero_ui_gtk_text(const uint8_t *text, int64_t count, bool marks) {
    int64_t more = 0;
    char *made;
    int64_t at = 0;

    for (int64_t i = 0; marks && i < count; i += 1) {
        more += text[i] == '_' ? 1 : 0;
    }
    made = hero_layer_alloc((size_t)(count + more) + 1);
    if (made == NULL) {
        return NULL;
    }
    for (int64_t i = 0; i < count; i += 1) {
        if (marks && text[i] == '_') {
            made[at++] = '_';
        }
        made[at++] = (char)text[i];
    }
    made[at] = '\0';
    return made;
}

/* The C text `text` with each `_` doubled, as a menu shows a label. */
static char *hero_ui_gtk_marked(const char *text) {
    const char *given = text != NULL ? text : "";

    return hero_ui_gtk_text((const uint8_t *)given, (int64_t)strlen(given), true);
}

/* GTK's own words for a button, in the person's language, `_` taken out. */
static void hero_ui_gtk_words_of(const char *msgid, char *into, size_t room) {
    const char *said = hero_ui_gtk.dgettext("gtk40", msgid);
    size_t at = 0;

    for (size_t i = 0; said != NULL && said[i] != '\0' && at + 1 < room; i += 1) {
        if (said[i] != '_') {
            into[at++] = said[i];
        }
    }
    into[at] = '\0';
}

/* The WebKitGTK member's answer `answer`, its words kept where it refused
 * `HERO_UI_OTHER` or `HERO_UI_UNSUPPORTED`, so the failure names what is
 * missing. */
static int64_t hero_ui_gtk_web(int64_t answer) {
    char words[HERO_UI_WORDS_MOST];

    if (answer != HERO_UI_OTHER && answer != HERO_UI_UNSUPPORTED) {
        return answer;
    }
    if (hero_ui_web_gtk_words(words, (int64_t)sizeof words) <= 0) {
        (void)snprintf(words, sizeof words, "WebKitGTK refused, giving no words");
    }
    return hero_ui_refused(answer, words);
}

/* Connects `handler` to `signal` of `instance`, carrying widget number `id`. */
static void hero_ui_gtk_connect(void *instance, const char *signal, hero_ui_gtk_callback handler, int64_t id) {
    (void)hero_ui_gtk.signal_connect_data(instance, signal, handler, (void *)(intptr_t)id, NULL, 0);
}

_Static_assert(sizeof(void *) == sizeof(int64_t), "a signal's data carries a widget's 64-bit number");

/* The widget whose signal carried `data`, where `instance` is still its own
 * (or one of a radio group's items), else NULL. */
static hero_ui_native *hero_ui_gtk_of(void *data, void *instance) {
    hero_ui_native *widget = hero_ui_find((int64_t)(intptr_t)data);

    if (widget == NULL) {
        return NULL;
    }
    if (widget->widget == instance || widget->inner == instance || widget->buffer == instance || widget->action == instance || widget->area == instance) {
        return widget;
    }
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        if (widget->radios[i] == instance) {
            return widget;
        }
    }
    return NULL;
}

/* The window of number `id`, or NULL. */
static hero_ui_native *hero_ui_gtk_window(int64_t id) {
    hero_ui_native *window = hero_ui_find(id);

    return window != NULL && window->kind == HERO_UI_WINDOW ? window : NULL;
}

/* `value` as GTK's `int` of a size or a place. */
static int hero_ui_gtk_int(int64_t value) {
    return (int)hero_ui_between(value, -(INT64_C(1) << 24), INT64_C(1) << 24);
}

/* A slider's or a progress bar's value as a double, and back, exact while
 * the range is within 2^53 of 0, its share of the range past that. */
static double hero_ui_gtk_double(const hero_ui_native *widget, int64_t value) {
    if (widget->least >= -HERO_UI_GTK_EXACT && widget->most <= HERO_UI_GTK_EXACT) {
        return (double)value;
    }
    return hero_ui_fraction(value, widget->least, widget->most) * (double)HERO_UI_GTK_EXACT;
}

static int64_t hero_ui_gtk_integer(const hero_ui_native *widget, double value) {
    if (widget->least >= -HERO_UI_GTK_EXACT && widget->most <= HERO_UI_GTK_EXACT) {
        /* Inside 2^53 of 0 a double holds the value, and its rounding fits. */
        double held = value < (double)widget->least ? (double)widget->least : (value > (double)widget->most ? (double)widget->most : value);

        return hero_ui_between(held >= 0 ? (int64_t)(held + 0.5) : -(int64_t)(-held + 0.5), widget->least, widget->most);
    }
    return hero_ui_at_fraction(value / (double)HERO_UI_GTK_EXACT, widget->least, widget->most);
}

/* -- Windows: their size, their menus ---------------------------------------- */

/* Gives window `window` the default size whose content is the content size
 * it keeps: the window's size less its content's, the menu bar and a title
 * bar GTK draws, measured where the window was laid out, else the menu bar's
 * own height. */
static void hero_ui_gtk_size_content(hero_ui_native *window) {
    int outer_width = hero_ui_gtk.widget_get_width(window->widget);
    int outer_height = hero_ui_gtk.widget_get_height(window->widget);
    int inner_width = hero_ui_gtk.widget_get_width(window->area);
    int inner_height = hero_ui_gtk.widget_get_height(window->area);
    int extra_width = 0;
    int extra_height = 0;

    if (outer_width > 0 && inner_width > 0 && outer_height > 0 && inner_height > 0) {
        extra_width = outer_width - inner_width;
        extra_height = outer_height - inner_height;
    } else if (window->bar != NULL && window->order.count > 0) {
        hero_ui_gtk_size least;
        hero_ui_gtk_size natural;

        hero_ui_gtk.widget_get_preferred_size(window->bar, &least, &natural);
        extra_height = natural.height;
    }
    window->asked_width = hero_ui_gtk_int(window->content_width) + extra_width;
    window->asked_height = hero_ui_gtk_int(window->content_height) + extra_height;
    hero_ui_gtk.window_set_default_size(window->widget, window->asked_width, window->asked_height);
}

/* Gives window `data` its content size again, from an idle source: what the
 * content's `resize` asks where the window's size was right and its
 * content's not, since a size given while GTK lays a window out is lost. */
static int hero_ui_gtk_resize_again(void *data) {
    hero_ui_native *window = hero_ui_gtk_window((int64_t)(intptr_t)data);

    if (window != NULL) {
        hero_ui_gtk_size_content(window);
    }
    return 0;
}

/* Window content `area` took the size `width` by `height`. */
static void hero_ui_gtk_resized(void *area, int width, int height, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, area);

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return;
    }
    if (width == hero_ui_gtk_int(window->content_width) && height == hero_ui_gtk_int(window->content_height)) {
        window->resize_injected = false;
        return;
    }
    if (!window->resize_injected && hero_ui_gtk.widget_get_width(window->widget) == window->asked_width && hero_ui_gtk.widget_get_height(window->widget) == window->asked_height) {
        /* The window took the size asked and its content another: the room
         * the menu bar and the title bar take was not yet known. */
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_resize_again, data);
        return;
    }
    window->resize_injected = false;
    window->content_width = width;
    window->content_height = height;
    hero_ui_tell(HERO_UI_EVENT_RESIZE, window->id, width, height);
}

/* The person asked window `instance` to close: told, and nothing closed. */
static int hero_ui_gtk_close_asked(void *instance, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, instance);

    if (window != NULL && window->kind == HERO_UI_WINDOW) {
        hero_ui_tell(HERO_UI_EVENT_CLOSE, window->id, 0, 0);
    }
    return 1;
}

/* Whether menu item `item`, its menu and the showing of both let the person
 * choose it. */
static bool hero_ui_gtk_usable(const hero_ui_native *item) {
    const hero_ui_native *menu = hero_ui_find(item->parent);

    return item->kind == HERO_UI_MENU_ITEM && item->enabled && item->visible && menu != NULL && menu->enabled && menu->visible;
}

/* The action's name of item `id`, `item<id>`, into `into`. */
static void hero_ui_gtk_action_name(int64_t id, bool prefixed, char *into, size_t room) {
    (void)snprintf(into, room, "%sitem%lld", prefixed ? "hero." : "", (long long)id);
}

/* Builds menu `menu`'s model again: its items shown, in their order, in
 * sections between its separators; and sets each item's action enabled as
 * `hero_ui_gtk_usable` says. */
static int64_t hero_ui_gtk_fill_menu(hero_ui_native *menu) {
    void *section = NULL;
    int64_t in_section = 0;
    int64_t answer = HERO_UI_OK;

    hero_ui_gtk.menu_remove_all(menu->menu_model);
    for (int64_t i = 0; i < menu->order.count; i += 1) {
        hero_ui_native *item = hero_ui_find(menu->order.at[i]);

        if (item == NULL) {
            continue;
        }
        if (item->action != NULL) {
            hero_ui_gtk.simple_action_set_enabled(item->action, hero_ui_gtk_usable(item) ? 1 : 0);
        }
        if (!item->visible) {
            continue;
        }
        if (item->kind == HERO_UI_MENU_SEPARATOR) {
            if (section != NULL && in_section > 0) {
                hero_ui_gtk.menu_append_section(menu->menu_model, NULL, section);
                hero_ui_gtk.object_unref(section);
                section = NULL;
                in_section = 0;
            }
            continue;
        }
        if (section == NULL) {
            section = hero_ui_gtk.menu_new();
        }
        {
            char action[64];
            char *label = hero_ui_gtk_marked(item->label);
            void *entry;

            if (label == NULL) {
                answer = HERO_UI_LIMIT;
                continue;
            }
            hero_ui_gtk_action_name(item->id, true, action, sizeof action);
            entry = hero_ui_gtk.menu_item_new(label, action);
            hero_layer_free(label);
            if (item->keyval != 0) {
                char *accel = hero_ui_gtk.accelerator_name(item->keyval, item->held);

                if (accel != NULL) {
                    hero_ui_gtk.menu_item_set_attribute_value(entry, "accel", hero_ui_gtk.variant_new_string(accel));
                    hero_ui_gtk.g_free(accel);
                }
            }
            hero_ui_gtk.menu_append_item(section, entry);
            hero_ui_gtk.object_unref(entry);
            in_section += 1;
        }
    }
    if (section != NULL) {
        if (in_section > 0) {
            hero_ui_gtk.menu_append_section(menu->menu_model, NULL, section);
        }
        hero_ui_gtk.object_unref(section);
    }
    return answer;
}

/* Builds window `window`'s menu bar's model again: its menus shown, in their
 * order; the bar is shown while it holds one, and the content keeps its
 * size. */
static int64_t hero_ui_gtk_fill_bar(hero_ui_native *window) {
    int64_t shown = 0;
    int64_t answer = HERO_UI_OK;

    hero_ui_gtk.menu_remove_all(window->bar_model);
    for (int64_t i = 0; i < window->order.count; i += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[i]);
        char *label;

        if (menu == NULL) {
            continue;
        }
        (void)hero_ui_gtk_fill_menu(menu);
        if (!menu->visible) {
            continue;
        }
        label = hero_ui_gtk_marked(menu->label);
        if (label == NULL) {
            answer = HERO_UI_LIMIT;
            continue;
        }
        hero_ui_gtk.menu_append_submenu(window->bar_model, label, menu->menu_model);
        hero_layer_free(label);
        shown += 1;
    }
    hero_ui_gtk.widget_set_visible(window->bar, shown > 0 ? 1 : 0);
    hero_ui_gtk_size_content(window);
    return answer;
}

/* An item's action was activated: by its menu, its shortcut, or `inject`. */
static void hero_ui_gtk_chosen(void *action, void *parameter, void *data) {
    hero_ui_native *item = hero_ui_gtk_of(data, action);

    (void)parameter;
    if (item != NULL && hero_ui_gtk_usable(item)) {
        hero_ui_tell(HERO_UI_EVENT_MENU, item->id, 0, 0);
    }
}

/* -- What the person does ---------------------------------------------------- */

static void hero_ui_gtk_clicked(void *button, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, button);

    if (widget != NULL && widget->kind == HERO_UI_BUTTON) {
        hero_ui_tell(HERO_UI_EVENT_CLICK, widget->id, 0, 0);
    }
}

static void hero_ui_gtk_toggled(void *button, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, button);

    if (widget == NULL) {
        return;
    }
    if (widget->kind == HERO_UI_CHECKBOX) {
        hero_ui_tell(HERO_UI_EVENT_TOGGLE, widget->id, hero_ui_gtk.check_button_get_active(button) ? 1 : 0, 0);
        return;
    }
    for (int64_t i = 0; widget->kind == HERO_UI_RADIO_GROUP && i < widget->radio_count; i += 1) {
        if (widget->radios[i] == button && hero_ui_gtk.check_button_get_active(button)) {
            hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, i, 0);
        }
    }
}

static void hero_ui_gtk_edited(void *instance, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, instance);

    if (widget != NULL && (widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA)) {
        hero_ui_tell(HERO_UI_EVENT_TEXT, widget->id, 0, 0);
    }
}

static void hero_ui_gtk_rows_chosen(void *box, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, box);

    if (widget != NULL && (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE)) {
        hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, 0, 0);
    }
}

/* Takes the empty row of no choice out of combo box `data`'s list, from an
 * idle source, once an item was chosen: never inside the drop-down's own
 * notification. */
static int hero_ui_gtk_drop_placeholder(void *data) {
    hero_ui_native *combo = hero_ui_find((int64_t)(intptr_t)data);

    if (combo != NULL && combo->kind == HERO_UI_COMBO && combo->placeholder && hero_ui_gtk.drop_down_get_selected(combo->inner) != 0) {
        hero_ui_mute(1);
        combo->placeholder = false;
        hero_ui_gtk.string_list_remove(combo->strings, 0);
        hero_ui_mute(-1);
    }
    return 0;
}

/* The item combo box `combo` has chosen, -1 for none. */
static int64_t hero_ui_gtk_combo_choice(const hero_ui_native *combo) {
    unsigned chosen = hero_ui_gtk.drop_down_get_selected(combo->inner);
    int64_t item;

    if (chosen == HERO_GTK_INVALID_LIST_POSITION) {
        return -1;
    }
    item = (int64_t)chosen - (combo->placeholder ? 1 : 0);
    return item >= 0 && item < combo->items ? item : -1;
}

static void hero_ui_gtk_combo_chosen(void *drop, void *spec, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, drop);

    (void)spec;
    if (widget == NULL || widget->kind != HERO_UI_COMBO || hero_ui_is_muted()) {
        return;
    }
    if (widget->placeholder && hero_ui_gtk.drop_down_get_selected(drop) != 0) {
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_drop_placeholder, data);
    }
    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, hero_ui_gtk_combo_choice(widget), 0);
}

static void hero_ui_gtk_slid(void *range, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, range);
    int64_t value;

    if (widget == NULL || widget->kind != HERO_UI_SLIDER) {
        return;
    }
    value = hero_ui_gtk_integer(widget, hero_ui_gtk.range_get_value(range));
    if (value != widget->value || hero_ui_gtk_injecting > 0) {
        widget->value = value;
        hero_ui_tell(HERO_UI_EVENT_VALUE, widget->id, value, 0);
    }
}

/* -- The arm ------------------------------------------------------------------ */

/* Whether `name` names something in the environment. */
static bool hero_ui_gtk_named(const char *name) {
    const char *value = getenv(name);

    return value != NULL && value[0] != '\0';
}

int64_t hero_ui_arm_here(void) {
    if (!hero_ui_gtk_named("DISPLAY") && !hero_ui_gtk_named("WAYLAND_DISPLAY")) {
        return 0;
    }
    return hero_ui_gtk_loaded() ? 1 : 0;
}

void hero_ui_arm_absent(char *words, size_t count) {
    if (!hero_ui_gtk_named("DISPLAY") && !hero_ui_gtk_named("WAYLAND_DISPLAY")) {
        (void)hero_ui_copy_text(words, count, "no display is named, neither DISPLAY nor WAYLAND_DISPLAY");
        return;
    }
    (void)hero_ui_copy_text(words, count, "GTK 4 is not here: " HERO_UI_GTK_LIBRARY " is not installed, or lacks a function of GTK 4.0");
}

int64_t hero_ui_arm_open(void) {
    if (!hero_ui_gtk_loaded()) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "GTK 4 is not here: " HERO_UI_GTK_LIBRARY " is not installed, or lacks a function of GTK 4.0");
    }
    hero_ui_gtk.disable_setlocale();
    if (!hero_ui_gtk.init_check()) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "GTK 4 could not open the display DISPLAY or WAYLAND_DISPLAY names");
    }
    hero_ui_unmute();
    hero_ui_gtk_injecting = 0;
    hero_ui_make_host();
    return HERO_UI_OK;
}

/* The container `widget` stands in: its window's content, or its radio
 * group's own fixed for an item. */
static void *hero_ui_gtk_container(const hero_ui_native *widget) {
    hero_ui_native *window = hero_ui_gtk_window(widget->window);

    return window != NULL ? window->fixed : NULL;
}

/* Destroys `widget`'s GTK objects and gives back its record; its children
 * are gone already. */
static void hero_ui_gtk_drop(hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);

    switch (widget->kind) {
    case HERO_UI_WINDOW:
        if (widget->widget != NULL) {
            hero_ui_gtk.window_destroy(widget->widget);
        }
        if (widget->bar_model != NULL) {
            hero_ui_gtk.object_unref(widget->bar_model);
        }
        if (widget->actions != NULL) {
            hero_ui_gtk.object_unref(widget->actions);
        }
        break;
    case HERO_UI_MENU:
        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_gtk_fill_bar(owner);
        }
        if (widget->menu_model != NULL) {
            hero_ui_gtk.object_unref(widget->menu_model);
        }
        break;
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR: {
        hero_ui_native *window = hero_ui_gtk_window(widget->window);

        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_gtk_fill_menu(owner);
        }
        if (window != NULL && widget->shortcut != NULL) {
            hero_ui_gtk.shortcut_controller_remove_shortcut(window->shortcuts, widget->shortcut);
        }
        if (window != NULL && widget->action != NULL) {
            char name[64];

            hero_ui_gtk_action_name(widget->id, false, name, sizeof name);
            hero_ui_gtk.action_map_remove_action(window->actions, name);
        }
        if (widget->action != NULL) {
            hero_ui_gtk.object_unref(widget->action);
        }
        break;
    }
    default: {
        void *container = hero_ui_gtk_container(widget);

        if (widget->kind == HERO_UI_WEBVIEW) {
            /* The member lets go of its page and handlers and calls the
             * host's `gone`, and then its widget goes as any other. */
            (void)hero_ui_web_gtk_destroy(widget->id);
        }
        if (container != NULL && widget->widget != NULL) {
            hero_ui_gtk.fixed_remove(container, widget->widget);
        }
        break;
    }
    }
    hero_layer_free(widget->radios);
    hero_layer_free(widget->radio_places);
    hero_layer_free(widget->label);
    hero_ui_ids_free(&widget->order);
    hero_ui_forget(widget->id);
    hero_layer_free(widget);
}

/* Destroys widget `id` and every widget whose parent it is, children first. */
static void hero_ui_gtk_destroy_tree(int64_t id) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return;
    }
    for (;;) {
        int64_t child = 0;

        for (size_t at = 0; at < hero_ui_map_room() && child == 0; at += 1) {
            hero_ui_native *each = hero_ui_at(at);

            if (each != NULL && each->parent == id && each->id != id) {
                child = each->id;
            }
        }
        if (child == 0) {
            break;
        }
        hero_ui_gtk_destroy_tree(child);
    }
    hero_ui_gtk_drop(widget);
}

void hero_ui_arm_close(void) {
    for (;;) {
        int64_t first = 0;

        for (size_t at = 0; at < hero_ui_map_room() && first == 0; at += 1) {
            hero_ui_native *widget = hero_ui_at(at);

            if (widget != NULL) {
                first = widget->kind == HERO_UI_WINDOW || hero_ui_find(widget->parent) == NULL ? widget->id : widget->parent;
            }
        }
        if (first == 0) {
            break;
        }
        hero_ui_gtk_destroy_tree(first);
    }
    hero_ui_free_map();
    hero_layer_free(hero_ui_gtk_paths);
    hero_ui_gtk_paths = NULL;
    hero_ui_gtk_paths_count = 0;
    /* GTK finishes a window's destruction in its own loop's turns: they are
     * given those turns before the app is gone, a bounded number, so a source
     * that is always ready cannot hold `close`. */
    for (int turns = 0; turns < 1000 && hero_ui_gtk.main_context_iteration(NULL, 0); turns += 1) {
    }
}

/* Makes window `made`: its box, its menu bar, its content, its actions and
 * its shortcuts. */
static bool hero_ui_gtk_make_window(hero_ui_native *made) {
    void *box;
    void *overlay;

    made->widget = hero_ui_gtk.window_new();
    made->bar_model = hero_ui_gtk.menu_new();
    made->actions = hero_ui_gtk.simple_action_group_new();
    made->shortcuts = hero_ui_gtk.shortcut_controller_new();
    if (made->widget == NULL || made->bar_model == NULL || made->actions == NULL || made->shortcuts == NULL) {
        return false;
    }
    box = hero_ui_gtk.box_new(HERO_GTK_ORIENTATION_VERTICAL, 0);
    made->bar = hero_ui_gtk.popover_menu_bar_new_from_model(made->bar_model);
    overlay = hero_ui_gtk.overlay_new();
    made->area = hero_ui_gtk.drawing_area_new();
    made->fixed = hero_ui_gtk.fixed_new();
    hero_ui_gtk.widget_set_hexpand(made->area, 1);
    hero_ui_gtk.widget_set_vexpand(made->area, 1);
    hero_ui_gtk.widget_set_vexpand(overlay, 1);
    hero_ui_gtk.overlay_set_child(overlay, made->area);
    hero_ui_gtk.overlay_add_overlay(overlay, made->fixed);
    hero_ui_gtk.overlay_set_clip_overlay(overlay, made->fixed, 1);
    hero_ui_gtk.widget_set_visible(made->bar, 0);
    hero_ui_gtk.box_append(box, made->bar);
    hero_ui_gtk.box_append(box, overlay);
    hero_ui_gtk.window_set_child(made->widget, box);
    hero_ui_gtk.widget_insert_action_group(made->widget, "hero", made->actions);
    hero_ui_gtk.widget_add_controller(made->widget, made->shortcuts);
    hero_ui_gtk_connect(made->widget, "close-request", (hero_ui_gtk_callback)hero_ui_gtk_close_asked, made->id);
    hero_ui_gtk_connect(made->area, "resize", (hero_ui_gtk_callback)hero_ui_gtk_resized, made->id);
    made->content_width = 100;
    made->content_height = 100;
    return true;
}

/* Puts `made`'s widget in its window's content, at no place and hidden. */
static void hero_ui_gtk_stand(hero_ui_native *made, hero_ui_native *window) {
    hero_ui_gtk.widget_set_visible(made->widget, 0);
    hero_ui_gtk.fixed_put(window->fixed, made->widget, 0, 0);
}

/* Makes web view `id` in window `window`: THE WEB VIEW, kind 17, is
 * `hero_ui_web_gtk.c`'s, which answers WebKitGTK's widget; this arm stands
 * it in the window's content as any widget, hidden until placed. */
static int64_t hero_ui_gtk_make_web(int64_t id, hero_ui_native *window) {
    hero_ui_native *made = hero_layer_alloc_zero(1, sizeof *made);
    int64_t answer;

    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    made->id = id;
    made->kind = HERO_UI_WEBVIEW;
    made->parent = window->id;
    made->window = window->id;
    made->visible = true;
    made->enabled = true;
    if (!hero_ui_keep(id, made)) {
        hero_layer_free(made);
        return HERO_UI_LIMIT;
    }
    answer = hero_ui_gtk_web(hero_ui_web_gtk_create(id, hero_ui_web_host_of(), &made->widget));
    if (answer == HERO_UI_OK && made->widget == NULL) {
        (void)hero_ui_web_gtk_destroy(id);
        answer = hero_ui_refused(HERO_UI_OTHER, "WebKitGTK made no widget");
    }
    if (answer != HERO_UI_OK) {
        hero_ui_forget(id);
        hero_layer_free(made);
        return answer;
    }
    hero_ui_gtk_stand(made, window);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    hero_ui_native *owner = NULL;
    hero_ui_native *window = NULL;
    hero_ui_native *made;
    bool ok = true;

    if (hero_ui_find(id) != NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (kind != HERO_UI_WINDOW) {
        owner = hero_ui_find(parent);
        if (owner == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        if ((kind == HERO_UI_MENU_ITEM || kind == HERO_UI_MENU_SEPARATOR) != (owner->kind == HERO_UI_MENU) || (owner->kind != HERO_UI_MENU && owner->kind != HERO_UI_WINDOW)) {
            return HERO_UI_INVALID;
        }
        window = hero_ui_gtk_window(owner->window);
        if (window == NULL) {
            return HERO_UI_NOT_FOUND;
        }
    }
    if (kind == HERO_UI_WEBVIEW) {
        return hero_ui_gtk_make_web(id, window);
    }
    made = hero_layer_alloc_zero(1, sizeof *made);
    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    made->id = id;
    made->kind = kind;
    made->parent = parent;
    made->window = window != NULL ? window->id : id;
    made->visible = true;
    made->enabled = true;
    made->least = 0;
    made->most = 100;
    switch (kind) {
    case HERO_UI_WINDOW:
        ok = hero_ui_gtk_make_window(made);
        break;
    case HERO_UI_LABEL:
        made->widget = hero_ui_gtk.label_new("");
        hero_ui_gtk.label_set_xalign(made->widget, 0.0f);
        hero_ui_gtk.label_set_yalign(made->widget, 0.0f);
        break;
    case HERO_UI_BUTTON:
        made->widget = hero_ui_gtk.button_new_with_label("");
        hero_ui_gtk_connect(made->widget, "clicked", (hero_ui_gtk_callback)hero_ui_gtk_clicked, id);
        break;
    case HERO_UI_TEXT_FIELD:
        made->widget = hero_ui_gtk.entry_new();
        hero_ui_gtk_connect(made->widget, "changed", (hero_ui_gtk_callback)hero_ui_gtk_edited, id);
        break;
    case HERO_UI_TEXT_AREA:
        made->widget = hero_ui_gtk.scrolled_window_new();
        made->inner = hero_ui_gtk.text_view_new();
        made->buffer = hero_ui_gtk.text_view_get_buffer(made->inner);
        hero_ui_gtk.text_view_set_wrap_mode(made->inner, HERO_GTK_WRAP_WORD_CHAR);
        hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_NEVER, HERO_GTK_POLICY_AUTOMATIC);
        hero_ui_gtk.scrolled_window_set_has_frame(made->widget, 1);
        hero_ui_gtk.scrolled_window_set_child(made->widget, made->inner);
        hero_ui_gtk_connect(made->buffer, "changed", (hero_ui_gtk_callback)hero_ui_gtk_edited, id);
        break;
    case HERO_UI_CHECKBOX:
        made->widget = hero_ui_gtk.check_button_new_with_label("");
        hero_ui_gtk_connect(made->widget, "toggled", (hero_ui_gtk_callback)hero_ui_gtk_toggled, id);
        break;
    case HERO_UI_RADIO_GROUP:
        made->widget = hero_ui_gtk.fixed_new();
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        made->widget = hero_ui_gtk.scrolled_window_new();
        made->inner = hero_ui_gtk.list_box_new();
        hero_ui_gtk.list_box_set_selection_mode(made->inner, kind == HERO_UI_LIST_MULTIPLE ? HERO_GTK_SELECTION_MULTIPLE : HERO_GTK_SELECTION_SINGLE);
        hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_NEVER, HERO_GTK_POLICY_AUTOMATIC);
        hero_ui_gtk.scrolled_window_set_has_frame(made->widget, 1);
        hero_ui_gtk.scrolled_window_set_child(made->widget, made->inner);
        hero_ui_gtk_connect(made->inner, "selected-rows-changed", (hero_ui_gtk_callback)hero_ui_gtk_rows_chosen, id);
        break;
    case HERO_UI_COMBO:
        made->strings = hero_ui_gtk.string_list_new(NULL);
        made->widget = made->strings != NULL ? hero_ui_gtk.drop_down_new(made->strings, NULL) : NULL;
        made->inner = made->widget;
        if (made->widget != NULL) {
            hero_ui_gtk_connect(made->widget, "notify::selected", (hero_ui_gtk_callback)hero_ui_gtk_combo_chosen, id);
        }
        break;
    case HERO_UI_PROGRESS:
        made->widget = hero_ui_gtk.progress_bar_new();
        break;
    case HERO_UI_SLIDER:
        made->widget = hero_ui_gtk.scale_new_with_range(HERO_GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
        if (made->widget != NULL) {
            hero_ui_gtk.scale_set_draw_value(made->widget, 0);
            hero_ui_gtk.range_set_increments(made->widget, 1, 10);
            hero_ui_gtk_connect(made->widget, "value-changed", (hero_ui_gtk_callback)hero_ui_gtk_slid, id);
        }
        break;
    case HERO_UI_IMAGE:
        made->widget = hero_ui_gtk.picture_new();
        break;
    case HERO_UI_MENU:
        made->menu_model = hero_ui_gtk.menu_new();
        ok = made->menu_model != NULL;
        break;
    case HERO_UI_MENU_ITEM: {
        char name[64];

        hero_ui_gtk_action_name(id, false, name, sizeof name);
        made->action = hero_ui_gtk.simple_action_new(name, NULL);
        ok = made->action != NULL;
        if (ok) {
            hero_ui_gtk_connect(made->action, "activate", (hero_ui_gtk_callback)hero_ui_gtk_chosen, id);
        }
        break;
    }
    case HERO_UI_MENU_SEPARATOR:
        break;
    default:
        hero_layer_free(made);
        return HERO_UI_INVALID;
    }
    if (!ok || (made->widget == NULL && kind != HERO_UI_WINDOW && kind != HERO_UI_MENU && kind != HERO_UI_MENU_ITEM && kind != HERO_UI_MENU_SEPARATOR) || !hero_ui_keep(id, made)) {
        if (kind == HERO_UI_WINDOW && made->widget != NULL) {
            hero_ui_gtk.window_destroy(made->widget);
        }
        if (made->bar_model != NULL) {
            hero_ui_gtk.object_unref(made->bar_model);
        }
        if (made->actions != NULL) {
            hero_ui_gtk.object_unref(made->actions);
        }
        if (made->menu_model != NULL) {
            hero_ui_gtk.object_unref(made->menu_model);
        }
        if (made->action != NULL) {
            hero_ui_gtk.object_unref(made->action);
        }
        hero_layer_free(made);
        return ok ? HERO_UI_LIMIT : hero_ui_refused(HERO_UI_OTHER, "GTK made no widget of that kind");
    }
    switch (kind) {
    case HERO_UI_WINDOW:
        hero_ui_gtk_size_content(made);
        return HERO_UI_OK;
    case HERO_UI_MENU:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_gtk_drop(made);
            return HERO_UI_LIMIT;
        }
        return hero_ui_gtk_fill_bar(owner);
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_gtk_drop(made);
            return HERO_UI_LIMIT;
        }
        if (made->action != NULL) {
            hero_ui_gtk.action_map_add_action(window->actions, made->action);
        }
        return hero_ui_gtk_fill_menu(owner);
    default:
        hero_ui_gtk_stand(made, window);
        return HERO_UI_OK;
    }
}

int64_t hero_ui_arm_destroy(int64_t id) {
    if (hero_ui_find(id) == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_gtk_destroy_tree(id);
    return HERO_UI_OK;
}

/* Sets text area `widget`'s whole text, the caret put at its end. */
static void hero_ui_gtk_area_text(hero_ui_native *widget, const char *text) {
    hero_ui_gtk_iter end;

    hero_ui_gtk.text_buffer_set_text(widget->buffer, text, -1);
    hero_ui_gtk.text_buffer_get_end_iter(widget->buffer, &end);
    hero_ui_gtk.text_buffer_place_cursor(widget->buffer, &end);
}

int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    char *made;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_WINDOW:
    case HERO_UI_LABEL:
    case HERO_UI_BUTTON:
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
    case HERO_UI_CHECKBOX:
    case HERO_UI_MENU:
    case HERO_UI_MENU_ITEM:
        break;
    default:
        return HERO_UI_INVALID;
    }
    made = hero_ui_gtk_text(text, count, false);
    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_mute(1);
    switch (widget->kind) {
    case HERO_UI_WINDOW:
        hero_ui_gtk.window_set_title(widget->widget, made);
        break;
    case HERO_UI_LABEL:
        hero_ui_gtk.label_set_text(widget->widget, made);
        break;
    case HERO_UI_BUTTON:
        hero_ui_gtk.button_set_label(widget->widget, made);
        break;
    case HERO_UI_CHECKBOX:
        hero_ui_gtk.check_button_set_label(widget->widget, made);
        break;
    case HERO_UI_TEXT_FIELD:
        hero_ui_gtk.editable_set_text(widget->widget, made);
        hero_ui_gtk.editable_set_position(widget->widget, -1);
        break;
    case HERO_UI_TEXT_AREA:
        hero_ui_gtk_area_text(widget, made);
        break;
    default:
        hero_layer_free(widget->label);
        widget->label = made;
        made = NULL;
        break;
    }
    hero_ui_mute(-1);
    hero_layer_free(made);
    if (widget->kind == HERO_UI_MENU) {
        return hero_ui_gtk_fill_bar(hero_ui_find(widget->parent));
    }
    if (widget->kind == HERO_UI_MENU_ITEM) {
        return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
    }
    return HERO_UI_OK;
}

/* Places radio group `group`'s items inside its own fixed, at their places
 * less the group's. */
static void hero_ui_gtk_put_radios(hero_ui_native *group) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        const int64_t *place = &group->radio_places[i * 4];

        if (place[2] < 0) {
            hero_ui_gtk.widget_set_visible(group->radios[i], 0);
            continue;
        }
        hero_ui_gtk.fixed_move(group->widget, group->radios[i], (double)(place[0] - group->x), (double)(place[1] - group->y));
        hero_ui_gtk.widget_set_size_request(group->radios[i], hero_ui_gtk_int(place[2]), hero_ui_gtk_int(place[3]));
        hero_ui_gtk.widget_set_visible(group->radios[i], 1);
    }
}

int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t texts = hero_ui_texts_in(items, count);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_LIST && widget->kind != HERO_UI_LIST_MULTIPLE && widget->kind != HERO_UI_COMBO && widget->kind != HERO_UI_RADIO_GROUP) {
        return HERO_UI_INVALID;
    }
    hero_ui_mute(1);
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        void **radios = texts > 0 ? hero_layer_alloc_zero((size_t)texts, sizeof *radios) : NULL;
        int64_t *places = texts > 0 ? hero_layer_alloc_zero((size_t)texts * 4, sizeof *places) : NULL;

        if (texts > 0 && (radios == NULL || places == NULL)) {
            hero_layer_free(radios);
            hero_layer_free(places);
            hero_ui_mute(-1);
            return HERO_UI_LIMIT;
        }
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            hero_ui_gtk.fixed_remove(widget->widget, widget->radios[i]);
        }
        hero_layer_free(widget->radios);
        hero_layer_free(widget->radio_places);
        widget->radios = radios;
        widget->radio_places = places;
        widget->radio_count = 0;
        for (int64_t i = 0; i < texts; i += 1) {
            int64_t length = 0;
            const uint8_t *text = hero_ui_text_at(items, count, i, &length);
            char *label = hero_ui_gtk_text(text, length, false);
            void *radio = label != NULL ? hero_ui_gtk.check_button_new_with_label(label) : NULL;

            hero_layer_free(label);
            if (radio == NULL) {
                hero_ui_mute(-1);
                return HERO_UI_LIMIT;
            }
            if (i > 0) {
                hero_ui_gtk.check_button_set_group(radio, radios[0]);
            }
            hero_ui_gtk.widget_set_sensitive(radio, widget->enabled ? 1 : 0);
            hero_ui_gtk.widget_set_visible(radio, 0);
            hero_ui_gtk.fixed_put(widget->widget, radio, 0, 0);
            hero_ui_gtk_connect(radio, "toggled", (hero_ui_gtk_callback)hero_ui_gtk_toggled, widget->id);
            radios[i] = radio;
            places[i * 4 + 2] = -1;
            widget->radio_count = i + 1;
        }
    } else if (widget->kind == HERO_UI_COMBO) {
        void *strings = hero_ui_gtk.string_list_new(NULL);

        if (strings == NULL) {
            hero_ui_mute(-1);
            return HERO_UI_LIMIT;
        }
        if (texts > 0) {
            hero_ui_gtk.string_list_append(strings, "");
        }
        for (int64_t i = 0; i < texts; i += 1) {
            int64_t length = 0;
            const uint8_t *text = hero_ui_text_at(items, count, i, &length);
            char *made = hero_ui_gtk_text(text, length, false);

            if (made == NULL) {
                hero_ui_gtk.object_unref(strings);
                hero_ui_mute(-1);
                return HERO_UI_LIMIT;
            }
            hero_ui_gtk.string_list_append(strings, made);
            hero_layer_free(made);
        }
        hero_ui_gtk.drop_down_set_model(widget->inner, strings);
        hero_ui_gtk.object_unref(strings);
        widget->strings = strings;
        widget->items = texts;
        widget->placeholder = texts > 0;
        hero_ui_gtk.drop_down_set_selected(widget->inner, texts > 0 ? 0 : HERO_GTK_INVALID_LIST_POSITION);
    } else {
        void *row;

        while ((row = hero_ui_gtk.list_box_get_row_at_index(widget->inner, 0)) != NULL) {
            hero_ui_gtk.list_box_remove(widget->inner, row);
        }
        widget->items = 0;
        for (int64_t i = 0; i < texts; i += 1) {
            int64_t length = 0;
            const uint8_t *text = hero_ui_text_at(items, count, i, &length);
            char *made = hero_ui_gtk_text(text, length, false);
            void *label = made != NULL ? hero_ui_gtk.label_new(made) : NULL;

            hero_layer_free(made);
            if (label == NULL) {
                hero_ui_mute(-1);
                return HERO_UI_LIMIT;
            }
            hero_ui_gtk.label_set_xalign(label, 0.0f);
            hero_ui_gtk.list_box_append(widget->inner, label);
            widget->items += 1;
        }
        hero_ui_gtk.list_box_unselect_all(widget->inner);
    }
    hero_ui_mute(-1);
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        hero_ui_gtk_put_radios(widget);
    }
    return HERO_UI_OK;
}

/* Chooses item `item` of combo box `combo`, -1 for none, the empty row of no
 * choice put in or taken out as the choice needs. */
static void hero_ui_gtk_choose(hero_ui_native *combo, int64_t item) {
    if (item < 0 && combo->items > 0 && !combo->placeholder) {
        const char *const empty[] = {"", NULL};

        hero_ui_gtk.string_list_splice(combo->strings, 0, 0, empty);
        combo->placeholder = true;
    }
    if (item >= 0 && combo->placeholder) {
        combo->placeholder = false;
        hero_ui_gtk.string_list_remove(combo->strings, 0);
    }
    hero_ui_gtk.drop_down_set_selected(combo->inner, item < 0 ? (combo->placeholder ? 0 : HERO_GTK_INVALID_LIST_POSITION) : (unsigned)item);
}

/* Shows a slider's or a progress bar's range and value. */
static void hero_ui_gtk_show_range(hero_ui_native *widget) {
    if (widget->kind == HERO_UI_SLIDER) {
        hero_ui_gtk.range_set_range(widget->widget, hero_ui_gtk_double(widget, widget->least), hero_ui_gtk_double(widget, widget->most));
        hero_ui_gtk.range_set_value(widget->widget, hero_ui_gtk_double(widget, widget->value));
    } else {
        hero_ui_gtk.progress_bar_set_fraction(widget->widget, hero_ui_fraction(widget->value, widget->least, widget->most));
    }
}

int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t answer = HERO_UI_OK;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_mute(1);
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        if (value != 0 && value != 1) {
            answer = HERO_UI_INVALID;
            break;
        }
        hero_ui_gtk.check_button_set_active(widget->widget, value == 1 ? 1 : 0);
        break;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        if (value < widget->least || value > widget->most) {
            answer = HERO_UI_INVALID;
            break;
        }
        widget->value = value;
        hero_ui_gtk_show_range(widget);
        break;
    case HERO_UI_COMBO:
        if (value < -1 || value >= widget->items) {
            answer = HERO_UI_INVALID;
            break;
        }
        hero_ui_gtk_choose(widget, value);
        break;
    case HERO_UI_RADIO_GROUP:
        if (value < -1 || value >= widget->radio_count) {
            answer = HERO_UI_INVALID;
            break;
        }
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            hero_ui_gtk.check_button_set_active(widget->radios[i], i == value ? 1 : 0);
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    hero_ui_mute(-1);
    return answer;
}

int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_SLIDER && widget->kind != HERO_UI_PROGRESS) {
        return HERO_UI_INVALID;
    }
    widget->least = least;
    widget->most = most;
    widget->value = hero_ui_between(widget->value, least, most);
    hero_ui_mute(1);
    hero_ui_gtk_show_range(widget);
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if ((widget->kind != HERO_UI_LIST && widget->kind != HERO_UI_LIST_MULTIPLE) || (count > 1 && widget->kind != HERO_UI_LIST_MULTIPLE)) {
        return HERO_UI_INVALID;
    }
    for (int64_t i = 0; i < count; i += 1) {
        if (indices[i] >= widget->items) {
            return HERO_UI_INVALID;
        }
    }
    hero_ui_mute(1);
    hero_ui_gtk.list_box_unselect_all(widget->inner);
    for (int64_t i = 0; i < count; i += 1) {
        hero_ui_gtk.list_box_select_row(widget->inner, hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)indices[i]));
    }
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

/* Shows `widget` where it is placed and asked to be. */
static void hero_ui_gtk_show_placed(hero_ui_native *widget) {
    hero_ui_gtk.widget_set_visible(widget->widget, widget->placed && widget->visible ? 1 : 0);
}

int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (which == HERO_UI_FLAG_RESIZABLE) {
        if (widget->kind != HERO_UI_WINDOW) {
            return HERO_UI_INVALID;
        }
        hero_ui_gtk.window_set_resizable(widget->widget, on == 1 ? 1 : 0);
        return HERO_UI_OK;
    }
    if (which == HERO_UI_FLAG_VISIBLE) {
        widget->visible = on == 1;
        switch (widget->kind) {
        case HERO_UI_WINDOW:
            if (on == 1) {
                hero_ui_gtk.window_present(widget->widget);
            } else {
                hero_ui_gtk.widget_set_visible(widget->widget, 0);
            }
            return HERO_UI_OK;
        case HERO_UI_MENU:
            return hero_ui_gtk_fill_bar(hero_ui_find(widget->parent));
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
        default:
            hero_ui_gtk_show_placed(widget);
            return HERO_UI_OK;
        }
    }
    if (which == HERO_UI_FLAG_ENABLED) {
        /* A window and a web view take no greying, as AppKit's take none. */
        if (widget->kind == HERO_UI_WINDOW || widget->kind == HERO_UI_WEBVIEW) {
            return HERO_UI_INVALID;
        }
        widget->enabled = on == 1;
        switch (widget->kind) {
        case HERO_UI_MENU:
            return hero_ui_gtk_fill_menu(widget);
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
        default:
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                hero_ui_gtk.widget_set_sensitive(widget->radios[i], on == 1 ? 1 : 0);
            }
            hero_ui_gtk.widget_set_sensitive(widget->widget, on == 1 ? 1 : 0);
            return HERO_UI_OK;
        }
    }
    return HERO_UI_INVALID;
}

/* GDK's key value of the key `hero_ui.h` names; false for a name it does not
 * list. */
static bool hero_ui_gtk_key_of(const uint8_t *key, int64_t count, unsigned *keyval) {
    static const struct {
        const char *name;
        unsigned keyval;
    } named[] = {
        {"Enter", 0xFF0Du}, {"Escape", 0xFF1Bu}, {"Tab", 0xFF09u},  {"Space", 0x20u},   {"Delete", 0xFFFFu}, {"Backspace", 0xFF08u}, {"Up", 0xFF52u},
        {"Down", 0xFF54u},  {"Left", 0xFF51u},   {"Right", 0xFF53u}, {"Home", 0xFF50u}, {"End", 0xFF57u},    {"PageUp", 0xFF55u},    {"PageDown", 0xFF56u},
    };

    if (count == 1 && ((key[0] >= 'a' && key[0] <= 'z') || (key[0] >= '0' && key[0] <= '9'))) {
        *keyval = key[0];
        return true;
    }
    if (count >= 2 && count <= 3 && key[0] == 'F') {
        unsigned n = 0;

        for (int64_t i = 1; i < count; i += 1) {
            if (key[i] < '0' || key[i] > '9') {
                return false;
            }
            n = n * 10u + (unsigned)(key[i] - '0');
        }
        if (n >= 1 && n <= 12 && !(count == 3 && key[1] == '0')) {
            *keyval = HERO_GDK_KEY_F1 + n - 1;
            return true;
        }
        return false;
    }
    for (size_t i = 0; i < sizeof named / sizeof named[0]; i += 1) {
        if (strlen(named[i].name) == (size_t)count && memcmp(named[i].name, key, (size_t)count) == 0) {
            *keyval = named[i].keyval;
            return true;
        }
    }
    return false;
}

int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    hero_ui_native *item = hero_ui_find(id);
    hero_ui_native *window;
    unsigned keyval = 0;

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM || (count > 0 && !hero_ui_gtk_key_of(key, count, &keyval))) {
        return HERO_UI_INVALID;
    }
    window = hero_ui_gtk_window(item->window);
    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->shortcut != NULL) {
        hero_ui_gtk.shortcut_controller_remove_shortcut(window->shortcuts, item->shortcut);
        item->shortcut = NULL;
    }
    item->keyval = keyval;
    item->held = ((modifiers & HERO_UI_PRIMARY) != 0 ? HERO_GDK_CONTROL_MASK : 0u) | ((modifiers & HERO_UI_SHIFT) != 0 ? HERO_GDK_SHIFT_MASK : 0u) | ((modifiers & HERO_UI_ALT) != 0 ? HERO_GDK_ALT_MASK : 0u);
    if (keyval != 0) {
        char action[64];
        void *trigger = hero_ui_gtk.keyval_trigger_new(keyval, item->held);
        void *named;

        hero_ui_gtk_action_name(item->id, true, action, sizeof action);
        named = hero_ui_gtk.named_action_new(action);
        item->shortcut = trigger != NULL && named != NULL ? hero_ui_gtk.shortcut_new(trigger, named) : NULL;
        if (item->shortcut == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.shortcut_controller_add_shortcut(window->shortcuts, item->shortcut);
    }
    return hero_ui_gtk_fill_menu(hero_ui_find(item->parent));
}

int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    hero_ui_native *picture = hero_ui_find(id);
    void *bytes;
    void *texture;

    if (picture == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (picture->kind != HERO_UI_IMAGE) {
        return HERO_UI_INVALID;
    }
    if (width == 0 || height == 0) {
        hero_ui_gtk.picture_set_paintable(picture->widget, NULL);
        picture->image_width = 0;
        picture->image_height = 0;
        return HERO_UI_OK;
    }
    if (width > INT32_MAX / 4 || height > INT32_MAX / 4) {
        return HERO_UI_LIMIT;
    }
    bytes = hero_ui_gtk.bytes_new(rgba, (size_t)(width * height * 4));
    texture = bytes != NULL ? hero_ui_gtk.memory_texture_new((int)width, (int)height, HERO_GDK_MEMORY_R8G8B8A8, bytes, (size_t)width * 4) : NULL;
    if (bytes != NULL) {
        hero_ui_gtk.bytes_unref(bytes);
    }
    if (texture == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_gtk.picture_set_paintable(picture->widget, texture);
    hero_ui_gtk.object_unref(texture);
    picture->image_width = width;
    picture->image_height = height;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    hero_ui_native *window = hero_ui_find(id);

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (window->kind != HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    if (which == HERO_UI_SIZE_LEAST) {
        window->least_width = width;
        window->least_height = height;
        hero_ui_gtk.widget_set_size_request(window->area, hero_ui_gtk_int(width), hero_ui_gtk_int(height));
        return HERO_UI_OK;
    }
    if (width == window->content_width && height == window->content_height && hero_ui_gtk.widget_get_width(window->area) == hero_ui_gtk_int(width) && hero_ui_gtk.widget_get_height(window->area) == hero_ui_gtk_int(height)) {
        return HERO_UI_OK;
    }
    window->content_width = width;
    window->content_height = height;
    hero_ui_gtk_size_content(window);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    hero_ui_native *widget = hero_ui_find(id);
    void *container;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    container = hero_ui_gtk_container(widget);
    if (widget->widget == NULL || widget->kind == HERO_UI_WINDOW || container == NULL) {
        return HERO_UI_INVALID;
    }
    if (index >= 0) {
        int64_t *place;

        if (widget->kind != HERO_UI_RADIO_GROUP || index >= widget->radio_count) {
            return HERO_UI_INVALID;
        }
        place = &widget->radio_places[index * 4];
        place[0] = x;
        place[1] = y;
        place[2] = width;
        place[3] = height;
        hero_ui_gtk_put_radios(widget);
        return HERO_UI_OK;
    }
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
    widget->placed = true;
    if (widget->kind == HERO_UI_IMAGE && widget->image_width > 0 && widget->image_width <= width && widget->image_height <= height) {
        /* A picture stands at its own size in the middle of its place, as
         * AppKit draws one. */
        x += (width - widget->image_width) / 2;
        y += (height - widget->image_height) / 2;
        width = widget->image_width;
        height = widget->image_height;
    }
    hero_ui_gtk.fixed_move(container, widget->widget, (double)hero_ui_gtk_int(x), (double)hero_ui_gtk_int(y));
    hero_ui_gtk.widget_set_size_request(widget->widget, hero_ui_gtk_int(width), hero_ui_gtk_int(height));
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        hero_ui_gtk_put_radios(widget);
    }
    hero_ui_gtk_show_placed(widget);
    return HERO_UI_OK;
}

/* The size GTK draws `widget` at as it stands with its text and items,
 * apart from the size it was placed at, and shown for the moment it is
 * measured: GTK 4 measures a widget it does not show as 0 by 0, and this arm
 * hides one until it is placed (measured under Xvfb, GTK 4.18.6). */
static hero_ui_gtk_size hero_ui_gtk_natural(void *widget) {
    int width = -1;
    int height = -1;
    int shown = hero_ui_gtk.widget_get_visible(widget);
    hero_ui_gtk_size least;
    hero_ui_gtk_size natural;

    hero_ui_gtk.widget_get_size_request(widget, &width, &height);
    hero_ui_gtk.widget_set_size_request(widget, -1, -1);
    if (!shown) {
        hero_ui_gtk.widget_set_visible(widget, 1);
    }
    hero_ui_gtk.widget_get_preferred_size(widget, &least, &natural);
    if (!shown) {
        hero_ui_gtk.widget_set_visible(widget, 0);
    }
    hero_ui_gtk.widget_set_size_request(widget, width, height);
    if (natural.width < least.width) {
        natural.width = least.width;
    }
    if (natural.height < least.height) {
        natural.height = least.height;
    }
    return natural;
}

int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_gtk_size size = {0, 0};

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    *width = 0;
    *height = 0;
    switch (widget->kind) {
    case HERO_UI_LABEL:
    case HERO_UI_BUTTON:
    case HERO_UI_CHECKBOX:
    case HERO_UI_COMBO:
        size = hero_ui_gtk_natural(widget->widget);
        break;
    case HERO_UI_RADIO_GROUP:
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            hero_ui_gtk_size item = hero_ui_gtk_natural(widget->radios[i]);

            size.width = item.width > size.width ? item.width : size.width;
            size.height += item.height;
        }
        break;
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_PROGRESS:
    case HERO_UI_SLIDER:
        /* The headless side's width, and GTK's height. */
        size = hero_ui_gtk_natural(widget->widget);
        size.width = 160;
        break;
    case HERO_UI_TEXT_AREA:
        size.width = 240;
        size.height = 96;
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        size.width = 160;
        size.height = 96;
        break;
    case HERO_UI_IMAGE:
        *width = widget->image_width;
        *height = widget->image_height;
        return HERO_UI_OK;
    case HERO_UI_WEBVIEW:
        /* The headless side's size of a web view, where nothing else decides
         * it (`ui/internal/sizes.hero`). */
        *width = 320;
        *height = 240;
        return HERO_UI_OK;
    default:
        return HERO_UI_OK;
    }
    *width = size.width;
    *height = size.height;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_TEXT_FIELD) {
        const char *text = hero_ui_gtk.editable_get_text(widget->widget);
        size_t bytes = text != NULL ? strlen(text) : 0;

        *length = (int64_t)bytes;
        if (bytes > 0 && *length <= count) {
            memcpy(buffer, text, bytes);
        }
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_TEXT_AREA) {
        hero_ui_gtk_iter start;
        hero_ui_gtk_iter end;
        char *text;
        size_t bytes;

        hero_ui_gtk.text_buffer_get_start_iter(widget->buffer, &start);
        hero_ui_gtk.text_buffer_get_end_iter(widget->buffer, &end);
        text = hero_ui_gtk.text_buffer_get_text(widget->buffer, &start, &end, 1);
        if (text == NULL) {
            return HERO_UI_LIMIT;
        }
        bytes = strlen(text);
        *length = (int64_t)bytes;
        if (bytes > 0 && *length <= count) {
            memcpy(buffer, text, bytes);
        }
        hero_ui_gtk.g_free(text);
        return HERO_UI_OK;
    }
    return HERO_UI_INVALID;
}

/* The item radio group `group` has checked, -1 for none. */
static int64_t hero_ui_gtk_checked_radio(const hero_ui_native *group) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        if (hero_ui_gtk.check_button_get_active(group->radios[i])) {
            return i;
        }
    }
    return -1;
}

int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        *value = hero_ui_gtk.check_button_get_active(widget->widget) ? 1 : 0;
        return HERO_UI_OK;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        *value = widget->value;
        return HERO_UI_OK;
    case HERO_UI_COMBO:
        *value = hero_ui_gtk_combo_choice(widget);
        return HERO_UI_OK;
    case HERO_UI_RADIO_GROUP:
        *value = hero_ui_gtk_checked_radio(widget);
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

int64_t hero_ui_arm_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t one;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
        int64_t found = 0;

        for (int64_t i = 0; i < widget->items; i += 1) {
            void *row = hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)i);

            if (row != NULL && hero_ui_gtk.list_box_row_is_selected(row)) {
                if (found < count) {
                    indices[found] = i;
                }
                found += 1;
            }
        }
        *chosen = found;
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_COMBO) {
        one = hero_ui_gtk_combo_choice(widget);
    } else if (widget->kind == HERO_UI_RADIO_GROUP) {
        one = hero_ui_gtk_checked_radio(widget);
    } else {
        return HERO_UI_INVALID;
    }
    *chosen = one >= 0 ? 1 : 0;
    if (one >= 0 && count >= 1) {
        indices[0] = one;
    }
    return HERO_UI_OK;
}

/* Wakes the loop `pump` blocks in: the deadline's timeout source. */
static int hero_ui_gtk_woke(void *data) {
    *(bool *)data = true;
    return 0;
}

/* The monotonic clock, in milliseconds. */
static int64_t hero_ui_gtk_now(void) {
    struct timespec now;

    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        return 0;
    }
    return (int64_t)now.tv_sec * 1000 + (int64_t)now.tv_nsec / 1000000;
}

int64_t hero_ui_arm_pump(int64_t ms) {
    uint64_t before = hero_ui_pushed_count();
    int64_t deadline = hero_ui_gtk_now() + hero_ui_between(ms, 0, INT32_MAX);

    for (;;) {
        int64_t now;
        bool woke = false;
        unsigned timer;

        while (hero_ui_pushed_count() == before && hero_ui_gtk.main_context_iteration(NULL, 0)) {
        }
        now = hero_ui_gtk_now();
        if (hero_ui_pushed_count() != before || now >= deadline) {
            return HERO_UI_OK;
        }
        timer = hero_ui_gtk.timeout_add((unsigned)(deadline - now), hero_ui_gtk_woke, &woke);
        (void)hero_ui_gtk.main_context_iteration(NULL, 1);
        if (!woke) {
            (void)hero_ui_gtk.source_remove(timer);
        }
    }
}

int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t answer = HERO_UI_OK;
    bool usable;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    usable = widget->widget != NULL && hero_ui_gtk.widget_is_sensitive(widget->widget);
    hero_ui_gtk_injecting += 1;
    switch (kind) {
    case HERO_UI_EVENT_CLICK:
    case HERO_UI_EVENT_TOGGLE:
        if (widget->kind == HERO_UI_BUTTON) {
            if (usable) {
                hero_ui_gtk.signal_emit_by_name(widget->widget, "clicked");
            }
        } else if (widget->kind == HERO_UI_CHECKBOX) {
            if (usable) {
                hero_ui_gtk.check_button_set_active(widget->widget, hero_ui_gtk.check_button_get_active(widget->widget) ? 0 : 1);
            }
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_TEXT:
        if (widget->kind == HERO_UI_TEXT_FIELD) {
            char *typed = hero_ui_gtk_text(text, count, false);
            int position = 0;

            if (typed == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (usable && hero_ui_gtk.editable_get_editable(widget->widget)) {
                hero_ui_gtk.editable_delete_text(widget->widget, 0, -1);
                hero_ui_gtk.editable_insert_text(widget->widget, typed, (int)count, &position);
            }
            hero_layer_free(typed);
        } else if (widget->kind == HERO_UI_TEXT_AREA) {
            char *typed = hero_ui_gtk_text(text, count, false);

            if (typed == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (usable && hero_ui_gtk.text_view_get_editable(widget->inner)) {
                hero_ui_gtk_iter start;
                hero_ui_gtk_iter end;

                hero_ui_gtk.text_buffer_begin_user_action(widget->buffer);
                hero_ui_gtk.text_buffer_get_start_iter(widget->buffer, &start);
                hero_ui_gtk.text_buffer_get_end_iter(widget->buffer, &end);
                (void)hero_ui_gtk.text_buffer_delete_interactive(widget->buffer, &start, &end, 1);
                (void)hero_ui_gtk.text_buffer_insert_interactive_at_cursor(widget->buffer, typed, (int)count, 1);
                hero_ui_gtk.text_buffer_end_user_action(widget->buffer);
            }
            hero_layer_free(typed);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_SELECT:
        if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
            if (value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (usable) {
                if (value < 0 || second != 1 || widget->kind == HERO_UI_LIST) {
                    hero_ui_gtk.list_box_unselect_all(widget->inner);
                }
                if (value >= 0) {
                    hero_ui_gtk.list_box_select_row(widget->inner, hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)value));
                }
            }
        } else if (widget->kind == HERO_UI_COMBO) {
            if (value < 0 || value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (usable) {
                /* The empty row of no choice is taken out first, muted, as
                 * the person's choice would take it out after; a choice of
                 * what stands is told, as AppKit's is. */
                if (widget->placeholder) {
                    hero_ui_mute(1);
                    widget->placeholder = false;
                    hero_ui_gtk.string_list_remove(widget->strings, 0);
                    hero_ui_mute(-1);
                }
                if (hero_ui_gtk_combo_choice(widget) == value) {
                    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, value, 0);
                } else {
                    hero_ui_gtk.drop_down_set_selected(widget->inner, (unsigned)value);
                }
            }
        } else if (widget->kind == HERO_UI_RADIO_GROUP) {
            if (value < 0 || value >= widget->radio_count) {
                answer = HERO_UI_INVALID;
            } else if (usable && hero_ui_gtk.widget_is_sensitive(widget->radios[value])) {
                if (hero_ui_gtk.check_button_get_active(widget->radios[value])) {
                    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, value, 0);
                } else {
                    hero_ui_gtk.check_button_set_active(widget->radios[value], 1);
                }
            }
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_VALUE:
        if (widget->kind != HERO_UI_SLIDER) {
            answer = HERO_UI_INVALID;
        } else if (usable) {
            int64_t held = hero_ui_between(value, widget->least, widget->most);

            if (held == widget->value) {
                hero_ui_tell(HERO_UI_EVENT_VALUE, widget->id, held, 0);
            } else {
                hero_ui_gtk.range_set_value(widget->widget, hero_ui_gtk_double(widget, held));
            }
        }
        break;
    case HERO_UI_EVENT_MENU:
        if (widget->kind != HERO_UI_MENU_ITEM) {
            answer = HERO_UI_INVALID;
        } else if (hero_ui_gtk_usable(widget)) {
            hero_ui_gtk.action_activate(widget->action, NULL);
        }
        break;
    case HERO_UI_EVENT_MESSAGE:
        /* The page sends `text` by its own bridge, so the message comes as
         * the page's would. */
        answer = widget->kind == HERO_UI_WEBVIEW ? hero_ui_gtk_web(hero_ui_web_gtk_send(id, text, count)) : HERO_UI_INVALID;
        break;
    case HERO_UI_EVENT_CLOSE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else {
            if (!hero_ui_gtk.widget_get_realized(widget->widget)) {
                hero_ui_gtk.widget_realize(widget->widget);
            }
            hero_ui_gtk.window_close(widget->widget);
        }
        break;
    case HERO_UI_EVENT_RESIZE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else {
            int64_t content_width = widget->content_width;
            int64_t content_height = widget->content_height;

            widget->content_width = hero_ui_between(value, widget->least_width, INT64_C(1) << 24);
            widget->content_height = hero_ui_between(second, widget->least_height, INT64_C(1) << 24);
            hero_ui_gtk_size_content(widget);
            /* The content keeps its size until GTK gives it the new one,
             * which its `resize` then tells as the person's. */
            widget->resize_injected = true;
            widget->content_width = content_width;
            widget->content_height = content_height;
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    hero_ui_gtk_injecting -= 1;
    return answer;
}

/* The `GtkWindow` of number `window` for a dialog to stand over, NULL for 0;
 * false where that number is no window. */
static bool hero_ui_gtk_owner(int64_t window, void **owner, int64_t *answer) {
    hero_ui_native *found;

    *owner = NULL;
    if (window == 0) {
        return true;
    }
    found = hero_ui_find(window);
    if (found == NULL || found->kind != HERO_UI_WINDOW) {
        *answer = found == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
        return false;
    }
    *owner = found->widget;
    return true;
}

/* What a dialog run in a nested loop answers. */
typedef struct hero_ui_gtk_answer {
    bool done;
    int response;
    void *value;
    hero_ui_gerror *error;
    int multiple;
} hero_ui_gtk_answer;

/* Runs GLib's loop until `answer` is done: a dialog is the person's for as
 * long as they take, as every system's modal dialog is. */
static void hero_ui_gtk_await(hero_ui_gtk_answer *answer) {
    while (!answer->done) {
        (void)hero_ui_gtk.main_context_iteration(NULL, 1);
    }
}

static void hero_ui_gtk_alert_answered(void *dialog, void *result, void *data) {
    hero_ui_gtk_answer *answer = data;

    answer->response = hero_ui_gtk.alert_dialog_choose_finish(dialog, result, &answer->error);
    answer->done = true;
}

static void hero_ui_gtk_dialog_answered(void *dialog, int response, void *data) {
    hero_ui_gtk_answer *answer = data;

    (void)dialog;
    answer->response = response;
    answer->done = true;
}

int64_t hero_ui_arm_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, int64_t *answer) {
    void *owner;
    int64_t refused = HERO_UI_OK;
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *said = hero_ui_gtk_text(text, text_count, false);
    hero_ui_gtk_answer got = {false, -1, NULL, NULL, 0};
    /* The buttons in the order shown, the answer each gives, GTK's own word
     * for each, and which one a dismissed dialog answers. */
    int64_t answers[3];
    const char *msgids[3];
    int shown = 0;
    int cancel = -1;

    if (heading == NULL || said == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return HERO_UI_LIMIT;
    }
    if (!hero_ui_gtk_owner(window, &owner, &refused)) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return refused;
    }
    if (buttons == HERO_UI_BUTTONS_YES_NO || buttons == HERO_UI_BUTTONS_YES_NO_CANCEL) {
        if (buttons == HERO_UI_BUTTONS_YES_NO_CANCEL) {
            cancel = shown;
            answers[shown] = HERO_UI_ANSWER_CANCEL;
            msgids[shown++] = "_Cancel";
        }
        answers[shown] = HERO_UI_ANSWER_NO;
        msgids[shown++] = "_No";
        answers[shown] = HERO_UI_ANSWER_YES;
        msgids[shown++] = "_Yes";
    } else {
        if (buttons == HERO_UI_BUTTONS_OK_CANCEL) {
            cancel = shown;
            answers[shown] = HERO_UI_ANSWER_CANCEL;
            msgids[shown++] = "_Cancel";
        }
        answers[shown] = HERO_UI_ANSWER_OK;
        msgids[shown++] = "_OK";
    }
    if (hero_ui_gtk.alert_dialog_new != NULL && hero_ui_gtk.alert_dialog_choose != NULL && hero_ui_gtk.alert_dialog_choose_finish != NULL && hero_ui_gtk.alert_dialog_set_buttons != NULL && hero_ui_gtk.alert_dialog_set_detail != NULL && hero_ui_gtk.alert_dialog_set_cancel_button != NULL && hero_ui_gtk.alert_dialog_set_default_button != NULL && hero_ui_gtk.alert_dialog_set_modal != NULL) {
        char words[3][64];
        const char *labels[4] = {NULL, NULL, NULL, NULL};
        void *dialog = hero_ui_gtk.alert_dialog_new("%s", heading);

        if (dialog == NULL) {
            hero_layer_free(heading);
            hero_layer_free(said);
            return HERO_UI_LIMIT;
        }
        for (int i = 0; i < shown; i += 1) {
            hero_ui_gtk_words_of(msgids[i], words[i], sizeof words[i]);
            labels[i] = words[i];
        }
        hero_ui_gtk.alert_dialog_set_detail(dialog, said);
        hero_ui_gtk.alert_dialog_set_buttons(dialog, labels);
        hero_ui_gtk.alert_dialog_set_cancel_button(dialog, cancel);
        hero_ui_gtk.alert_dialog_set_default_button(dialog, shown - 1);
        hero_ui_gtk.alert_dialog_set_modal(dialog, 1);
        hero_ui_gtk.alert_dialog_choose(dialog, owner, NULL, hero_ui_gtk_alert_answered, &got);
        hero_ui_gtk_await(&got);
        if (got.error != NULL) {
            hero_ui_gtk.error_free(got.error);
        }
        hero_ui_gtk.object_unref(dialog);
    } else {
        int type = tone == HERO_UI_TONE_ERROR ? HERO_GTK_MESSAGE_ERROR : (tone == HERO_UI_TONE_WARNING ? HERO_GTK_MESSAGE_WARNING : HERO_GTK_MESSAGE_INFO);
        void *dialog = hero_ui_gtk.message_dialog_new(owner, HERO_GTK_DIALOG_MODAL | HERO_GTK_DIALOG_DESTROY_WITH_PARENT, type, HERO_GTK_BUTTONS_NONE, "%s", heading);

        if (dialog == NULL) {
            hero_layer_free(heading);
            hero_layer_free(said);
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.message_dialog_format_secondary_text(dialog, "%s", said);
        for (int i = 0; i < shown; i += 1) {
            (void)hero_ui_gtk.dialog_add_button(dialog, hero_ui_gtk.dgettext("gtk40", msgids[i]), i);
        }
        hero_ui_gtk.dialog_set_default_response(dialog, shown - 1);
        /* The signal's data is the answer, not a widget's number. */
        (void)hero_ui_gtk.signal_connect_data(dialog, "response", (hero_ui_gtk_callback)hero_ui_gtk_dialog_answered, &got, NULL, 0);
        hero_ui_gtk.window_present(dialog);
        hero_ui_gtk_await(&got);
        hero_ui_gtk.window_destroy(dialog);
    }
    hero_layer_free(heading);
    hero_layer_free(said);
    /* A dialog dismissed with no button pressed answers Cancel, as an
     * answer no button gives is taken everywhere else. */
    *answer = got.response >= 0 && got.response < shown ? answers[got.response] : HERO_UI_ANSWER_CANCEL;
    return HERO_UI_OK;
}

static void hero_ui_gtk_file_answered(void *dialog, void *result, void *data) {
    hero_ui_gtk_answer *answer = data;

    if (answer->multiple == 1) {
        answer->value = hero_ui_gtk.file_dialog_open_multiple_finish(dialog, result, &answer->error);
    } else if (answer->multiple == 2) {
        answer->value = hero_ui_gtk.file_dialog_save_finish(dialog, result, &answer->error);
    } else {
        answer->value = hero_ui_gtk.file_dialog_open_finish(dialog, result, &answer->error);
    }
    answer->done = true;
}

/* Keeps the path of GIO file `file`, followed by one zero byte, at the end
 * of `paths`; a file with no local path, or one that is no UTF-8, is passed
 * over, since `hero_ui.h` answers UTF-8 paths. */
static bool hero_ui_gtk_keep_path(void *file, uint8_t **paths, int64_t *count, int64_t *room) {
    char *path = hero_ui_gtk.file_get_path(file);
    size_t bytes = path != NULL ? strlen(path) : 0;

    if (bytes == 0 || !hero_ui_is_utf8((const uint8_t *)path, bytes)) {
        hero_ui_gtk.g_free(path);
        return true;
    }
    if (*count + (int64_t)bytes + 1 > *room) {
        int64_t grown_room = (*room + (int64_t)bytes + 1) * 2;
        uint8_t *grown = hero_layer_grow(*paths, (size_t)grown_room);

        if (grown == NULL) {
            hero_ui_gtk.g_free(path);
            return false;
        }
        *paths = grown;
        *room = grown_room;
    }
    memcpy(*paths + *count, path, bytes);
    *count += (int64_t)bytes;
    (*paths)[*count] = 0;
    *count += 1;
    hero_ui_gtk.g_free(path);
    return true;
}

/* GTK's file filters of `filters`, each filter's name and then its
 * extensions joined by `;`; NULL where it names none. */
static void *hero_ui_gtk_filters(const uint8_t *filters, int64_t count, int64_t *made) {
    int64_t texts = hero_ui_texts_in(filters, count);
    void *store = texts >= 2 ? hero_ui_gtk.list_store_new(hero_ui_gtk.file_filter_get_type()) : NULL;

    *made = 0;
    for (int64_t f = 0; store != NULL && f + 1 < texts; f += 2) {
        int64_t name_length = 0;
        int64_t list_length = 0;
        const uint8_t *name = hero_ui_text_at(filters, count, f, &name_length);
        const uint8_t *list = hero_ui_text_at(filters, count, f + 1, &list_length);
        char *spelled_name = hero_ui_gtk_text(name, name_length, false);
        char *spelled_list = hero_ui_gtk_text(list, list_length, false);
        void *filter = hero_ui_gtk.file_filter_new();

        if (spelled_name == NULL || spelled_list == NULL || filter == NULL) {
            hero_layer_free(spelled_name);
            hero_layer_free(spelled_list);
            continue;
        }
        hero_ui_gtk.file_filter_set_name(filter, spelled_name);
        if (strspn(spelled_list, ";.") == strlen(spelled_list)) {
            /* A filter naming no extension takes every file, as Win32's
             * `*.*` does. */
            hero_ui_gtk.file_filter_add_pattern(filter, "*");
        }
        for (char *each = spelled_list, *end; *each != '\0' || each == spelled_list; each = end + 1) {
            char pattern[256];

            end = strchr(each, ';');
            if (end == NULL) {
                end = each + strlen(each);
            }
            if (*each == '.') {
                each += 1;
            }
            if (end > each && (size_t)(end - each) < sizeof pattern - 3) {
                char held = *end;

                *end = '\0';
                if (hero_ui_gtk.file_filter_add_suffix != NULL) {
                    hero_ui_gtk.file_filter_add_suffix(filter, each);
                } else {
                    (void)snprintf(pattern, sizeof pattern, "*.%s", each);
                    hero_ui_gtk.file_filter_add_pattern(filter, pattern);
                }
                *end = held;
            }
            if (*end == '\0') {
                break;
            }
        }
        hero_ui_gtk.list_store_append(store, filter);
        hero_ui_gtk.object_unref(filter);
        hero_layer_free(spelled_name);
        hero_layer_free(spelled_list);
        *made += 1;
    }
    return store;
}

int64_t hero_ui_arm_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes) {
    void *owner;
    int64_t refused = HERO_UI_OK;
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *proposed = hero_ui_gtk_text(name, name_count, false);
    int64_t filter_count = 0;
    void *store;
    hero_ui_gtk_answer got = {false, -1, NULL, NULL, 0};
    uint8_t *paths = NULL;
    int64_t paths_count = 0;
    int64_t paths_room = 0;
    bool kept = true;
    int64_t result = HERO_UI_OK;

    *bytes = 0;
    if (heading == NULL || proposed == NULL) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        return HERO_UI_LIMIT;
    }
    if (!hero_ui_gtk_owner(window, &owner, &refused)) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        return refused;
    }
    store = hero_ui_gtk_filters(filters, filters_count, &filter_count);
    if (hero_ui_gtk.file_dialog_new != NULL && hero_ui_gtk.file_dialog_set_title != NULL && hero_ui_gtk.file_dialog_set_modal != NULL && hero_ui_gtk.file_dialog_set_initial_name != NULL && hero_ui_gtk.file_dialog_set_filters != NULL && hero_ui_gtk.file_dialog_open != NULL && hero_ui_gtk.file_dialog_open_finish != NULL && hero_ui_gtk.file_dialog_open_multiple != NULL && hero_ui_gtk.file_dialog_open_multiple_finish != NULL && hero_ui_gtk.file_dialog_save != NULL && hero_ui_gtk.file_dialog_save_finish != NULL) {
        void *dialog = hero_ui_gtk.file_dialog_new();

        if (dialog == NULL) {
            result = HERO_UI_LIMIT;
        } else {
            if (heading[0] != '\0') {
                hero_ui_gtk.file_dialog_set_title(dialog, heading);
            }
            hero_ui_gtk.file_dialog_set_modal(dialog, 1);
            if (store != NULL && filter_count > 0) {
                hero_ui_gtk.file_dialog_set_filters(dialog, store);
            }
            if (save == 1) {
                got.multiple = 2;
                if (proposed[0] != '\0') {
                    hero_ui_gtk.file_dialog_set_initial_name(dialog, proposed);
                }
                hero_ui_gtk.file_dialog_save(dialog, owner, NULL, hero_ui_gtk_file_answered, &got);
            } else if (multiple == 1) {
                got.multiple = 1;
                hero_ui_gtk.file_dialog_open_multiple(dialog, owner, NULL, hero_ui_gtk_file_answered, &got);
            } else {
                hero_ui_gtk.file_dialog_open(dialog, owner, NULL, hero_ui_gtk_file_answered, &got);
            }
            hero_ui_gtk_await(&got);
            hero_ui_gtk.object_unref(dialog);
        }
    } else {
        char accept[64];
        char cancel[64];
        void *dialog;

        hero_ui_gtk_words_of(save == 1 ? "_Save" : "_Open", accept, sizeof accept);
        hero_ui_gtk_words_of("_Cancel", cancel, sizeof cancel);
        dialog = hero_ui_gtk.file_chooser_native_new(heading[0] != '\0' ? heading : NULL, owner, save == 1 ? HERO_GTK_FILE_CHOOSER_ACTION_SAVE : HERO_GTK_FILE_CHOOSER_ACTION_OPEN, accept, cancel);
        if (dialog == NULL) {
            result = HERO_UI_LIMIT;
        } else {
            unsigned filters_made = store != NULL ? hero_ui_gtk.list_model_get_n_items(store) : 0;

            hero_ui_gtk.native_dialog_set_modal(dialog, 1);
            hero_ui_gtk.file_chooser_set_select_multiple(dialog, save == 0 && multiple == 1 ? 1 : 0);
            if (save == 1 && proposed[0] != '\0') {
                hero_ui_gtk.file_chooser_set_current_name(dialog, proposed);
            }
            for (unsigned i = 0; i < filters_made; i += 1) {
                void *filter = hero_ui_gtk.list_model_get_item(store, i);

                hero_ui_gtk.file_chooser_add_filter(dialog, filter);
                hero_ui_gtk.object_unref(filter);
            }
            (void)hero_ui_gtk.signal_connect_data(dialog, "response", (hero_ui_gtk_callback)hero_ui_gtk_dialog_answered, &got, NULL, 0);
            hero_ui_gtk.native_dialog_show(dialog);
            hero_ui_gtk_await(&got);
            if (got.response == HERO_GTK_RESPONSE_ACCEPT) {
                got.value = hero_ui_gtk.file_chooser_get_files(dialog);
                got.multiple = 1;
            }
            hero_ui_gtk.object_unref(dialog);
        }
    }
    if (store != NULL) {
        hero_ui_gtk.object_unref(store);
    }
    hero_layer_free(heading);
    hero_layer_free(proposed);
    if (got.error != NULL) {
        if (got.error->domain != hero_ui_gtk.dialog_error_quark() || (got.error->code != HERO_GTK_DIALOG_ERROR_CANCELLED && got.error->code != HERO_GTK_DIALOG_ERROR_DISMISSED)) {
            char words[HERO_UI_WORDS_MOST];

            (void)snprintf(words, sizeof words, "GTK's file dialog failed: %s", got.error->message != NULL ? got.error->message : "no words");
            result = hero_ui_refused(HERO_UI_OTHER, words);
        }
        hero_ui_gtk.error_free(got.error);
    }
    if (got.value != NULL) {
        if (got.multiple == 1) {
            unsigned files = hero_ui_gtk.list_model_get_n_items(got.value);

            for (unsigned i = 0; i < files && kept; i += 1) {
                void *file = hero_ui_gtk.list_model_get_item(got.value, i);

                kept = hero_ui_gtk_keep_path(file, &paths, &paths_count, &paths_room);
                hero_ui_gtk.object_unref(file);
            }
        } else {
            kept = hero_ui_gtk_keep_path(got.value, &paths, &paths_count, &paths_room);
        }
        hero_ui_gtk.object_unref(got.value);
    }
    if (!kept) {
        hero_layer_free(paths);
        return HERO_UI_LIMIT;
    }
    if (result != HERO_UI_OK) {
        hero_layer_free(paths);
        return result;
    }
    hero_layer_free(hero_ui_gtk_paths);
    hero_ui_gtk_paths = paths;
    hero_ui_gtk_paths_count = paths_count;
    *bytes = paths_count;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_dialog_paths(uint8_t *buffer, int64_t count) {
    if (hero_ui_gtk_paths_count > 0 && hero_ui_gtk_paths_count <= count) {
        memcpy(buffer, hero_ui_gtk_paths, (size_t)hero_ui_gtk_paths_count);
    }
    return hero_ui_gtk_paths_count;
}

int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return widget->kind == HERO_UI_WEBVIEW ? hero_ui_gtk_web(hero_ui_web_gtk_load(id, url, count)) : HERO_UI_INVALID;
}

int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return widget->kind == HERO_UI_WEBVIEW ? hero_ui_gtk_web(hero_ui_web_gtk_post(id, text, count)) : HERO_UI_INVALID;
}

/* -- WHAT THIS ARM DOES NOT DRAW -------------------------------------------
 *
 * The calls `hero_ui.c` adds after THE WEB VIEW'S ENTRY POINTS that only the
 * macOS arm draws: each answers `HERO_UI_UNSUPPORTED` here, which the entry
 * point tells in `hero_ui_elsewhere`'s words, and `hero_ui_arm_makes` and
 * `hero_ui_arm_acts` answer false for the kinds and acts this arm does not
 * draw. Drawing one is writing its function here, and answering true for
 * its kind or act. */

/* -- THE CALLS ON A WINDOW AS THE SYSTEM SHOWS IT ---------------------- */

/* This part's kinds from `HERO_UI_TABS` on, a menu inside a menu, and the
 * acts `ACTIVATE`, `SUBMIT`, `SORT` and `UNFOLD` are drawn on macOS alone:
 * every other arm answers false here, so the entry point tells
 * `HERO_UI_UNSUPPORTED` in `hero_ui_elsewhere`'s words before that arm's
 * `create` or `inject`, which knows none of them, could call the kind or
 * the act `HERO_UI_INVALID`. An arm that draws one answers true for it. */
bool hero_ui_arm_makes(int64_t kind, int64_t parent) {
    const hero_ui_native *owner;

    if (kind >= HERO_UI_TABS) {
        return false;
    }
    if (kind != HERO_UI_MENU) {
        return true;
    }
    owner = hero_ui_find(parent);
    return owner == NULL || owner->kind != HERO_UI_MENU;
}

bool hero_ui_arm_acts(int64_t act) {
    return act != HERO_UI_EVENT_ACTIVATE && act != HERO_UI_EVENT_SUBMIT && act != HERO_UI_EVENT_SORT && act != HERO_UI_EVENT_UNFOLD;
}

int64_t hero_ui_arm_windows_words(char *words, int64_t count) {
    (void)words;
    (void)count;
    return 0;
}

int64_t hero_ui_arm_press_key(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    (void)id;
    (void)key;
    (void)count;
    (void)modifiers;
    return HERO_UI_UNSUPPORTED;
}

/* -- WINDOWS ON THE SCREEN ------------------------------------------------- */

int64_t hero_ui_arm_watch(int64_t id) {
    (void)id;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_screens(int64_t *values, int64_t count, int64_t *screens) {
    (void)values;
    (void)count;
    (void)screens;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_frame_of(int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height) {
    (void)id;
    (void)screen;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_set_position(int64_t id, int64_t screen, int64_t x, int64_t y, int64_t person) {
    (void)id;
    (void)screen;
    (void)x;
    (void)y;
    (void)person;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_center(int64_t id, int64_t over) {
    (void)id;
    (void)over;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_set_state(int64_t id, int64_t state, int64_t person) {
    (void)id;
    (void)state;
    (void)person;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_state(int64_t id, int64_t *state) {
    (void)id;
    (void)state;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_window_set(int64_t id, int64_t which, int64_t value) {
    (void)id;
    (void)which;
    (void)value;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_set_icon(const uint8_t *rgba, int64_t width, int64_t height) {
    (void)rgba;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

bool hero_ui_arm_modal_here(void) {
    return false;
}

int64_t hero_ui_arm_modal_pump(int64_t ms) {
    (void)ms;
    return HERO_UI_UNSUPPORTED;
}

/* -- A MENU ITEM'S ROLE ---------------------------------------------------- */

/* Win32 and GTK 4 hold no application menu, so their arm, once written,
 * checks the item and answers it done, the item staying where the program
 * put it (`hero_ui.h`, A MENU ITEM'S ROLE); until then no native app opens
 * here and this is not reached. */
int64_t hero_ui_arm_set_role(int64_t id, int64_t role) {
    (void)id;
    (void)role;
    return HERO_UI_UNSUPPORTED;
}

/* -- RETURN AND ESCAPE ----------------------------------------------------- */

int64_t hero_ui_arm_set_button_key(int64_t id, int64_t key) {
    (void)id;
    (void)key;
    return HERO_UI_UNSUPPORTED;
}

/* -- THE WIDGET WITH THE KEYS ---------------------------------------------- */

int64_t hero_ui_arm_set_focus(int64_t id) {
    (void)id;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_focus(int64_t id, int64_t *focus) {
    (void)id;
    (void)focus;
    return HERO_UI_UNSUPPORTED;
}

/* -- A FOLDER CHOSEN, AND A MESSAGE BOX OF THE PROGRAM'S WORDS -------------- */

int64_t hero_ui_arm_folder_dialog(int64_t multiple, const uint8_t *title, int64_t title_count, int64_t *bytes) {
    (void)multiple;
    (void)title;
    (void)title_count;
    (void)bytes;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_ask(int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel, int64_t *pressed) {
    (void)tone;
    (void)title;
    (void)title_count;
    (void)text;
    (void)text_count;
    (void)buttons;
    (void)buttons_count;
    (void)cancel;
    (void)pressed;
    return HERO_UI_UNSUPPORTED;
}

/* -- CHECK AND RADIO MENU ITEMS -------------------------------------------- */

int64_t hero_ui_arm_set_check(int64_t id, int64_t mode) {
    (void)id;
    (void)mode;
    return HERO_UI_UNSUPPORTED;
}

/* -- A MASKED FIELD, AND A PROGRESS BAR WITH NO VALUE ----------------------- */

int64_t hero_ui_arm_set_masked(int64_t id, int64_t on) {
    (void)id;
    (void)on;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_set_moving(int64_t id, int64_t on) {
    (void)id;
    (void)on;
    return HERO_UI_UNSUPPORTED;
}

/* -- THE CLIPBOARD --------------------------------------------------------- */

int64_t hero_ui_arm_clipboard_set(int64_t kind, const uint8_t *bytes, int64_t count, int64_t width, int64_t height) {
    (void)kind;
    (void)bytes;
    (void)count;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_clipboard_get(int64_t kind, int64_t *bytes, int64_t *width, int64_t *height) {
    (void)kind;
    (void)bytes;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_clipboard_bytes(uint8_t *buffer, int64_t count) {
    (void)buffer;
    (void)count;
    return -1;
}

/* -- BOXES THE SYSTEM DRAWS ------------------------------------------------ */

int64_t hero_ui_arm_insets(int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom) {
    (void)id;
    (void)left;
    (void)top;
    (void)right;
    (void)bottom;
    return HERO_UI_UNSUPPORTED;
}

/* -- A TABLE --------------------------------------------------------------- */

int64_t hero_ui_arm_set_columns(int64_t id, const uint8_t *titles, int64_t count) {
    (void)id;
    (void)titles;
    (void)count;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_set_sort(int64_t id, int64_t column, int64_t ascending) {
    (void)id;
    (void)column;
    (void)ascending;
    return HERO_UI_UNSUPPORTED;
}

/* -- A TREE ---------------------------------------------------------------- */

int64_t hero_ui_arm_tree_add(int64_t id, int64_t item, int64_t under, const uint8_t *text, int64_t count, int64_t foldable) {
    (void)id;
    (void)item;
    (void)under;
    (void)text;
    (void)count;
    (void)foldable;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_tree_remove(int64_t id, int64_t item) {
    (void)id;
    (void)item;
    return HERO_UI_UNSUPPORTED;
}

int64_t hero_ui_arm_tree_unfold(int64_t id, int64_t item, int64_t on) {
    (void)id;
    (void)item;
    (void)on;
    return HERO_UI_UNSUPPORTED;
}

#endif
