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
 * `GtkPopoverMenuBar`, on every system that is neither Apple's nor Windows;
 * and, from WHAT `hero_ui.c` ADDED AFTER THE WEB VIEW on, `GtkNotebook`,
 * `GtkScrolledWindow`, `GtkPaned`, `GtkColumnView`, a `GtkListView` over a
 * `GtkTreeListModel`, `GtkPasswordEntry`, `GtkPopoverMenu`, `GdkClipboard`,
 * GTK 4.10's folder and alert dialogs, and a window among the screens.
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
 * library and the libraries it needs, Xlib among them where GTK has its X11
 * backend; XTest's `libXtst.so.6` alone is opened beside it, where it is
 * installed. A function GTK 4.0 has missing is the
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
#define HERO_GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER 2
#define HERO_GTK_RESPONSE_DELETE_EVENT (-4)
#define HERO_GTK_OVERFLOW_HIDDEN 1
#define HERO_GTK_SORT_ASCENDING 0
#define HERO_GTK_SORT_DESCENDING 1
#define HERO_GTK_ALIGN_START 1
#define HERO_GTK_PHASE_BUBBLE 2
#define HERO_GTK_EVENT_SEQUENCE_CLAIMED 1
#define HERO_GTK_SHORTCUT_SCOPE_GLOBAL 2
#define HERO_GDK_BUTTON_SECONDARY 3u
#define HERO_GDK_TOPLEVEL_STATE_MINIMIZED 1u
#define HERO_GDK_TOPLEVEL_STATE_MAXIMIZED 2u
#define HERO_GDK_TOPLEVEL_STATE_FULLSCREEN 8u
#define HERO_GDK_TOPLEVEL_STATE_ABOVE 16u
#define HERO_G_TYPE_STRING 64u
#define HERO_G_SIGNAL_MATCH_FUNC_DATA 24
#define HERO_G_IO_ERROR_NOT_FOUND 1
#define HERO_G_BUS_TYPE_SESSION 2
#define HERO_GTK_ACCESSIBLE_PROPERTY_LABEL 4
#define HERO_GTK_ACCESSIBLE_RELATION_LABELLED_BY 10
/* GTK 4.20's `GtkInterfaceColorScheme`, from its documentation (unread on a
 * GTK that has it: GTK 4.18.6 has no such setting). */
#define HERO_GTK_INTERFACE_COLOR_SCHEME_DARK 2
#define HERO_GTK_INTERFACE_COLOR_SCHEME_LIGHT 3
#define HERO_G_IO_ERROR_NOT_SUPPORTED 15
/* A radio item's target, and its action's state when it is checked. */
#define HERO_UI_GTK_RADIO_ON "on"
#define HERO_UI_GTK_RADIO_OFF "off"
/* GDK's key values of the keys `hero_ui.h` names, which are X11's keysyms. */
#define HERO_GDK_KEY_F1 0xFFBEu
#define HERO_GDK_KEY_F10 0xFFC7u
#define HERO_GDK_KEY_ESCAPE 0xFF1Bu
#define HERO_GDK_KEY_MENU 0xFF67u
#define HERO_GDK_KEY_SHIFT_L 0xFFE1u
#define HERO_GDK_KEY_CONTROL_L 0xFFE3u
#define HERO_GDK_KEY_ALT_L 0xFFE9u
/* Xlib's numbers, read from libX11 1.8.12's headers (Debian 13, arm64). */
#define HERO_X_CLIENT_MESSAGE 33
#define HERO_X_CONFIGURE_NOTIFY 22
#define HERO_X_PROPERTY_NOTIFY 28
#define HERO_X_SUBSTRUCTURE_MASKS ((1L << 20) | (1L << 19))
#define HERO_XA_CARDINAL 6ul
#define HERO_XA_ATOM 4ul
#define HERO_XA_WINDOW 33ul

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
    /* Boxes the system draws: tabs, a scrolled box, two panes (GTK 4.0). */
    void *(*notebook_new)(void);
    int (*notebook_append_page)(void *notebook, void *child, void *label);
    void (*notebook_remove_page)(void *notebook, int page);
    int (*notebook_page_num)(void *notebook, void *child);
    void (*notebook_set_current_page)(void *notebook, int page);
    int (*notebook_get_current_page)(void *notebook);
    void (*notebook_set_tab_label_text)(void *notebook, void *child, const char *text);
    void (*notebook_set_scrollable)(void *notebook, int scrollable);
    int (*notebook_get_n_pages)(void *notebook);
    void (*widget_remove_controller)(void *widget, void *controller);
    void *(*settings_get_default)(void);
    void (*object_get)(void *object, const char *first, ...);
    int (*surface_get_mapped)(void *surface);
    void (*column_view_column_set_expand)(void *column, int expand);
    void *(*paned_new)(int orientation);
    void (*paned_set_start_child)(void *paned, void *child);
    void (*paned_set_end_child)(void *paned, void *child);
    void (*paned_set_position)(void *paned, int position);
    int (*paned_get_position)(void *paned);
    void (*paned_set_shrink_start_child)(void *paned, int shrink);
    void (*paned_set_shrink_end_child)(void *paned, int shrink);
    void (*paned_set_resize_start_child)(void *paned, int resize);
    void (*paned_set_resize_end_child)(void *paned, int resize);
    void (*widget_set_overflow)(void *widget, int overflow);
    int (*widget_compute_point)(void *widget, void *target, const float *point, float *out);
    int (*scrolled_window_get_overlay_scrolling)(void *scrolled);
    void *(*scrolled_window_get_vscrollbar)(void *scrolled);
    /* Rows opened, Return, the keys (GTK 4.0). */
    void (*list_box_set_activate_on_single_click)(void *box, int single);
    int (*list_box_row_get_index)(void *row);
    int (*widget_activate)(void *widget);
    void *(*editable_get_delegate)(void *editable);
    int (*widget_grab_focus)(void *widget);
    void *(*root_get_focus)(void *root);
    void *(*widget_get_parent)(void *widget);
    void (*window_set_default_widget)(void *window, void *widget);
    void (*shortcut_controller_set_scope)(void *controller, int scope);
    void *(*activate_action_get)(void);
    void *(*callback_action_new)(hero_ui_gtk_callback callback, void *data, void *destroy);
    void (*shortcut_set_arguments)(void *shortcut, void *arguments);
    void (*text_buffer_begin_irreversible_action)(void *buffer);
    void (*text_buffer_end_irreversible_action)(void *buffer);
    /* A masked field, a progress bar with no value (GTK 4.0). */
    void *(*password_entry_new)(void);
    void (*progress_bar_pulse)(void *bar);
    void (*progress_bar_set_pulse_step)(void *bar, double step);
    /* Menus inside menus, check and radio items, context menus (GTK 4.0,
     * GLib 2.28). */
    void *(*simple_action_new_stateful)(const char *name, void *parameter_type, void *state);
    void (*simple_action_set_state)(void *action, void *state);
    void *(*variant_new_boolean)(int value);
    void *(*variant_type_new)(const char *type);
    void (*variant_type_free)(void *type);
    void (*menu_item_set_action_and_target_value)(void *item, const char *action, void *target);
    void *(*popover_menu_new_from_model)(void *model);
    void (*widget_set_parent)(void *widget, void *parent);
    void (*widget_unparent)(void *widget);
    void (*popover_set_pointing_to)(void *popover, const int *rectangle);
    void (*popover_set_has_arrow)(void *popover, int arrow);
    void (*popover_popup)(void *popover);
    void (*widget_set_halign)(void *widget, int align);
    void *(*gesture_click_new)(void);
    void (*gesture_single_set_button)(void *gesture, unsigned button);
    int (*gesture_set_state)(void *gesture, int state);
    void (*entry_set_extra_menu)(void *entry, void *model);
    void (*password_entry_set_extra_menu)(void *entry, void *model);
    void (*text_view_set_extra_menu)(void *view, void *model);
    /* The clipboard (GTK 4.0, GLib 2.16). */
    void *(*widget_get_display)(void *widget);
    void *(*display_get_default)(void);
    void *(*display_get_clipboard)(void *display);
    void (*clipboard_set_text)(void *clipboard, const char *text);
    void (*clipboard_set_texture)(void *clipboard, void *texture);
    int (*clipboard_set_content)(void *clipboard, void *provider);
    void *(*content_provider_new_for_bytes)(const char *mime_type, void *bytes);
    void *(*content_provider_new_union)(void **providers, size_t count);
    void (*clipboard_read_text_async)(void *clipboard, void *cancellable, hero_ui_gtk_ready callback, void *data);
    char *(*clipboard_read_text_finish)(void *clipboard, void *result, hero_ui_gerror **error);
    void (*clipboard_read_value_async)(void *clipboard, size_t type, int priority, void *cancellable, hero_ui_gtk_ready callback, void *data);
    const void *(*clipboard_read_value_finish)(void *clipboard, void *result, hero_ui_gerror **error);
    void (*clipboard_read_texture_async)(void *clipboard, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*clipboard_read_texture_finish)(void *clipboard, void *result, hero_ui_gerror **error);
    size_t (*file_list_get_type)(void);
    void *(*file_list_get_files)(void *list);
    void (*slist_free)(void *list);
    void *(*value_get_boxed)(const void *value);
    char *(*filename_to_uri)(const char *path, const char *hostname, hero_ui_gerror **error);
    int (*texture_get_width)(void *texture);
    int (*texture_get_height)(void *texture);
    void (*texture_download)(void *texture, uint8_t *data, size_t stride);
    uint32_t (*io_error_quark)(void);
    void *(*cancellable_new)(void);
    void (*cancellable_cancel)(void *cancellable);
    /* A table and a tree (GTK 4.0). */
    void *(*column_view_new)(void *model);
    void (*column_view_append_column)(void *view, void *column);
    void (*column_view_remove_column)(void *view, void *column);
    void *(*column_view_get_sorter)(void *view);
    void (*column_view_sort_by_column)(void *view, void *column, int direction);
    void (*column_view_set_reorderable)(void *view, int reorderable);
    void *(*column_view_column_new)(const char *title, void *factory);
    void (*column_view_column_set_sorter)(void *column, void *sorter);
    void (*column_view_column_set_resizable)(void *column, int resizable);
    void *(*custom_sorter_new)(void *function, void *data, void *destroy);
    void *(*signal_list_item_factory_new)(void);
    void (*list_item_set_child)(void *item, void *child);
    void *(*list_item_get_child)(void *item);
    void *(*list_item_get_item)(void *item);
    void *(*multi_selection_new)(void *model);
    void *(*single_selection_new)(void *model);
    void (*single_selection_set_autoselect)(void *selection, int autoselect);
    void (*single_selection_set_can_unselect)(void *selection, int can);
    unsigned (*single_selection_get_selected)(void *selection);
    void (*single_selection_set_selected)(void *selection, unsigned position);
    int (*selection_model_select_item)(void *model, unsigned position, int unselect_rest);
    int (*selection_model_unselect_all)(void *model);
    int (*selection_model_is_selected)(void *model, unsigned position);
    const char *(*string_object_get_string)(void *object);
    const char *(*string_list_get_string)(void *list, unsigned position);
    void *(*tree_list_model_new)(void *root, int passthrough, int autoexpand, void *(*create)(void *item, void *data), void *data, void *destroy);
    void *(*tree_list_model_get_row)(void *model, unsigned position);
    void *(*tree_list_model_get_child_row)(void *model, unsigned position);
    void *(*tree_list_row_get_child_row)(void *row, unsigned position);
    void *(*tree_list_row_get_item)(void *row);
    int (*tree_list_row_get_expanded)(void *row);
    void (*tree_list_row_set_expanded)(void *row, int expanded);
    unsigned (*tree_list_row_get_position)(void *row);
    void *(*tree_expander_new)(void);
    void (*tree_expander_set_list_row)(void *expander, void *row);
    void (*tree_expander_set_child)(void *expander, void *child);
    void *(*list_view_new)(void *model, void *factory);
    void *(*object_ref)(void *object);
    void (*object_set_data)(void *object, const char *key, void *data);
    void *(*object_get_data)(void *object, const char *key);
    unsigned (*signal_handlers_disconnect_matched)(void *instance, int mask, unsigned signal, uint32_t detail, void *closure, hero_ui_gtk_callback function, void *data);
    /* Windows on the screen (GTK 4.0). */
    void *(*native_get_surface)(void *native);
    void *(*display_get_monitors)(void *display);
    void (*monitor_get_geometry)(void *monitor, int *rectangle);
    int (*monitor_get_scale_factor)(void *monitor);
    int (*surface_get_scale_factor)(void *surface);
    int (*surface_get_width)(void *surface);
    int (*surface_get_height)(void *surface);
    void (*window_maximize)(void *window);
    void (*window_unmaximize)(void *window);
    void (*window_minimize)(void *window);
    void (*window_unminimize)(void *window);
    void (*window_fullscreen)(void *window);
    void (*window_unfullscreen)(void *window);
    unsigned (*toplevel_get_state)(void *toplevel);
    void (*toplevel_set_icon_list)(void *toplevel, void *list);
    void (*window_set_decorated)(void *window, int decorated);
    void (*widget_set_opacity)(void *widget, double opacity);
    void (*window_set_modal)(void *window, int modal);
    void (*window_set_transient_for)(void *window, void *parent);
    int (*widget_get_mapped)(void *widget);
    int (*window_is_active)(void *window);
    int (*text_grab_focus_without_selecting)(void *text);
    void *(*window_get_toplevels)(void);
    void *(*widget_get_first_child)(void *widget);
    void *(*widget_get_next_sibling)(void *widget);
    size_t (*button_get_type)(void);
    const char *(*button_get_label)(void *button);
    void (*native_dialog_hide)(void *dialog);
    void *(*bus_get_sync)(int type, void *cancellable, hero_ui_gerror **error);
    void *(*dbus_connection_call_sync)(void *connection, const char *name, const char *path, const char *interface, const char *method, void *parameters, const void *reply_type, int flags, int timeout, void *cancellable, hero_ui_gerror **error);
    void *(*variant_new)(const char *format, ...);
    void *(*variant_new_strv)(const char *const *strings, long length);
    void *(*variant_new_array)(const char *type, void *const *children, size_t count);
    void (*variant_unref)(void *variant);
    const char *(*get_prgname)(void);
    void *(*object_class_find_property)(void *klass, const char *name);
    void (*object_set)(void *object, const char *first, ...);
    void (*accessible_update_property)(void *accessible, int first, ...);
    void (*accessible_reset_property)(void *accessible, int property);
    void (*accessible_update_relation)(void *accessible, int first, ...);
    void (*accessible_reset_relation)(void *accessible, int relation);
    unsigned (*get_minor_version)(void);
    void *(*list_append)(void *list, void *data);
    void (*list_free)(void *list);
    int (*type_check_instance_is_a)(void *instance, size_t type);
    /* GTK 4.10's, GTK 4.14's and the X11 backend's, NULL where the library
     * has none of them. */
    void (*file_dialog_select_folder)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_select_folder_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void (*file_dialog_select_multiple_folders)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_select_multiple_folders_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void *(*texture_downloader_new)(void *texture);
    void (*texture_downloader_set_format)(void *downloader, int format);
    void (*texture_downloader_download_into)(void *downloader, uint8_t *data, size_t stride);
    void (*texture_downloader_free)(void *downloader);
    void *(*column_view_sorter_get_primary_sort_column)(void *sorter);
    int (*column_view_sorter_get_primary_sort_order)(void *sorter);
    double (*monitor_get_scale)(void *monitor);
    size_t (*x11_display_get_type)(void);
    void *(*x11_display_get_xdisplay)(void *display);
    unsigned long (*x11_display_get_xrootwindow)(void *display);
    unsigned long (*x11_surface_get_xid)(void *surface);
    unsigned long (*x11_get_xatom_by_name_for_display)(void *display, const char *name);
    void (*x11_monitor_get_workarea)(void *monitor, int *rectangle);
    /* Xlib's, which GTK's X11 backend loads, and the XTest extension's. */
    int (*x_move_window)(void *display, unsigned long window, int x, int y);
    int (*x_translate_coordinates)(void *display, unsigned long from, unsigned long to, int x, int y, int *to_x, int *to_y, unsigned long *child);
    int (*x_get_window_property)(void *display, unsigned long window, unsigned long property, long offset, long length, int remove, unsigned long type, unsigned long *actual_type, int *actual_format, unsigned long *items, unsigned long *after, unsigned char **data);
    int (*x_free)(void *data);
    int (*x_sync)(void *display, int discard);
    int (*x_send_event)(void *display, unsigned long window, int propagate, long mask, void *event);
    int (*x_get_window_attributes)(void *display, unsigned long window, void *attributes);
    int (*x_get_input_focus)(void *display, unsigned long *focus, int *revert);
    int (*x_set_input_focus)(void *display, unsigned long focus, int revert, unsigned long time);
    int (*x_query_tree)(void *display, unsigned long window, unsigned long *root, unsigned long *parent, unsigned long **children, unsigned *count);
    void *(*x11_display_get_primary_monitor)(void *display);
    size_t (*wayland_display_get_type)(void);
    unsigned char (*x_keysym_to_keycode)(void *display, unsigned long keysym);
    int (*x_test_query_extension)(void *display, int *event, int *error, int *major, int *minor);
    int (*x_test_fake_key_event)(void *display, unsigned keycode, int press, unsigned long delay);
} hero_ui_gtk_functions;

static hero_ui_gtk_functions hero_ui_gtk;
static bool hero_ui_gtk_found;
static pthread_once_t hero_ui_gtk_once = PTHREAD_ONCE_INIT;

#define HERO_UI_GTK_LIBRARY "libgtk-4.so.1"
#define HERO_UI_GTK_XTEST_LIBRARY "libXtst.so.6"

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
    HERO_UI_GTK_NEED(notebook_new, "gtk_notebook_new");
    HERO_UI_GTK_NEED(notebook_append_page, "gtk_notebook_append_page");
    HERO_UI_GTK_NEED(notebook_remove_page, "gtk_notebook_remove_page");
    HERO_UI_GTK_NEED(notebook_page_num, "gtk_notebook_page_num");
    HERO_UI_GTK_NEED(notebook_set_current_page, "gtk_notebook_set_current_page");
    HERO_UI_GTK_NEED(notebook_get_current_page, "gtk_notebook_get_current_page");
    HERO_UI_GTK_NEED(notebook_set_tab_label_text, "gtk_notebook_set_tab_label_text");
    HERO_UI_GTK_NEED(notebook_set_scrollable, "gtk_notebook_set_scrollable");
    HERO_UI_GTK_NEED(notebook_get_n_pages, "gtk_notebook_get_n_pages");
    HERO_UI_GTK_NEED(widget_remove_controller, "gtk_widget_remove_controller");
    HERO_UI_GTK_NEED(settings_get_default, "gtk_settings_get_default");
    HERO_UI_GTK_NEED(object_get, "g_object_get");
    HERO_UI_GTK_NEED(surface_get_mapped, "gdk_surface_get_mapped");
    HERO_UI_GTK_NEED(column_view_column_set_expand, "gtk_column_view_column_set_expand");
    HERO_UI_GTK_NEED(paned_new, "gtk_paned_new");
    HERO_UI_GTK_NEED(paned_set_start_child, "gtk_paned_set_start_child");
    HERO_UI_GTK_NEED(paned_set_end_child, "gtk_paned_set_end_child");
    HERO_UI_GTK_NEED(paned_set_position, "gtk_paned_set_position");
    HERO_UI_GTK_NEED(paned_get_position, "gtk_paned_get_position");
    HERO_UI_GTK_NEED(paned_set_shrink_start_child, "gtk_paned_set_shrink_start_child");
    HERO_UI_GTK_NEED(paned_set_shrink_end_child, "gtk_paned_set_shrink_end_child");
    HERO_UI_GTK_NEED(paned_set_resize_start_child, "gtk_paned_set_resize_start_child");
    HERO_UI_GTK_NEED(paned_set_resize_end_child, "gtk_paned_set_resize_end_child");
    HERO_UI_GTK_NEED(widget_set_overflow, "gtk_widget_set_overflow");
    HERO_UI_GTK_NEED(widget_compute_point, "gtk_widget_compute_point");
    HERO_UI_GTK_NEED(scrolled_window_get_overlay_scrolling, "gtk_scrolled_window_get_overlay_scrolling");
    HERO_UI_GTK_NEED(scrolled_window_get_vscrollbar, "gtk_scrolled_window_get_vscrollbar");
    HERO_UI_GTK_NEED(list_box_set_activate_on_single_click, "gtk_list_box_set_activate_on_single_click");
    HERO_UI_GTK_NEED(list_box_row_get_index, "gtk_list_box_row_get_index");
    HERO_UI_GTK_NEED(widget_activate, "gtk_widget_activate");
    HERO_UI_GTK_NEED(editable_get_delegate, "gtk_editable_get_delegate");
    HERO_UI_GTK_NEED(widget_grab_focus, "gtk_widget_grab_focus");
    HERO_UI_GTK_NEED(root_get_focus, "gtk_root_get_focus");
    HERO_UI_GTK_NEED(widget_get_parent, "gtk_widget_get_parent");
    HERO_UI_GTK_NEED(window_set_default_widget, "gtk_window_set_default_widget");
    HERO_UI_GTK_NEED(shortcut_controller_set_scope, "gtk_shortcut_controller_set_scope");
    HERO_UI_GTK_NEED(activate_action_get, "gtk_activate_action_get");
    HERO_UI_GTK_NEED(callback_action_new, "gtk_callback_action_new");
    HERO_UI_GTK_NEED(shortcut_set_arguments, "gtk_shortcut_set_arguments");
    HERO_UI_GTK_NEED(text_buffer_begin_irreversible_action, "gtk_text_buffer_begin_irreversible_action");
    HERO_UI_GTK_NEED(text_buffer_end_irreversible_action, "gtk_text_buffer_end_irreversible_action");
    HERO_UI_GTK_NEED(password_entry_new, "gtk_password_entry_new");
    HERO_UI_GTK_NEED(progress_bar_pulse, "gtk_progress_bar_pulse");
    HERO_UI_GTK_NEED(progress_bar_set_pulse_step, "gtk_progress_bar_set_pulse_step");
    HERO_UI_GTK_NEED(simple_action_new_stateful, "g_simple_action_new_stateful");
    HERO_UI_GTK_NEED(simple_action_set_state, "g_simple_action_set_state");
    HERO_UI_GTK_NEED(variant_new_boolean, "g_variant_new_boolean");
    HERO_UI_GTK_NEED(variant_type_new, "g_variant_type_new");
    HERO_UI_GTK_NEED(variant_type_free, "g_variant_type_free");
    HERO_UI_GTK_NEED(menu_item_set_action_and_target_value, "g_menu_item_set_action_and_target_value");
    HERO_UI_GTK_NEED(popover_menu_new_from_model, "gtk_popover_menu_new_from_model");
    HERO_UI_GTK_NEED(widget_set_parent, "gtk_widget_set_parent");
    HERO_UI_GTK_NEED(widget_unparent, "gtk_widget_unparent");
    HERO_UI_GTK_NEED(popover_set_pointing_to, "gtk_popover_set_pointing_to");
    HERO_UI_GTK_NEED(popover_set_has_arrow, "gtk_popover_set_has_arrow");
    HERO_UI_GTK_NEED(popover_popup, "gtk_popover_popup");
    HERO_UI_GTK_NEED(widget_set_halign, "gtk_widget_set_halign");
    HERO_UI_GTK_NEED(gesture_click_new, "gtk_gesture_click_new");
    HERO_UI_GTK_NEED(gesture_single_set_button, "gtk_gesture_single_set_button");
    HERO_UI_GTK_NEED(gesture_set_state, "gtk_gesture_set_state");
    HERO_UI_GTK_NEED(entry_set_extra_menu, "gtk_entry_set_extra_menu");
    HERO_UI_GTK_NEED(password_entry_set_extra_menu, "gtk_password_entry_set_extra_menu");
    HERO_UI_GTK_NEED(text_view_set_extra_menu, "gtk_text_view_set_extra_menu");
    HERO_UI_GTK_NEED(widget_get_display, "gtk_widget_get_display");
    HERO_UI_GTK_NEED(display_get_default, "gdk_display_get_default");
    HERO_UI_GTK_NEED(display_get_clipboard, "gdk_display_get_clipboard");
    HERO_UI_GTK_NEED(clipboard_set_text, "gdk_clipboard_set_text");
    HERO_UI_GTK_NEED(clipboard_set_texture, "gdk_clipboard_set_texture");
    HERO_UI_GTK_NEED(clipboard_set_content, "gdk_clipboard_set_content");
    HERO_UI_GTK_NEED(content_provider_new_for_bytes, "gdk_content_provider_new_for_bytes");
    HERO_UI_GTK_NEED(content_provider_new_union, "gdk_content_provider_new_union");
    HERO_UI_GTK_NEED(clipboard_read_text_async, "gdk_clipboard_read_text_async");
    HERO_UI_GTK_NEED(clipboard_read_text_finish, "gdk_clipboard_read_text_finish");
    HERO_UI_GTK_NEED(clipboard_read_value_async, "gdk_clipboard_read_value_async");
    HERO_UI_GTK_NEED(clipboard_read_value_finish, "gdk_clipboard_read_value_finish");
    HERO_UI_GTK_NEED(clipboard_read_texture_async, "gdk_clipboard_read_texture_async");
    HERO_UI_GTK_NEED(clipboard_read_texture_finish, "gdk_clipboard_read_texture_finish");
    HERO_UI_GTK_NEED(file_list_get_type, "gdk_file_list_get_type");
    HERO_UI_GTK_NEED(slist_free, "g_slist_free");
    HERO_UI_GTK_NEED(value_get_boxed, "g_value_get_boxed");
    HERO_UI_GTK_NEED(filename_to_uri, "g_filename_to_uri");
    HERO_UI_GTK_NEED(texture_get_width, "gdk_texture_get_width");
    HERO_UI_GTK_NEED(texture_get_height, "gdk_texture_get_height");
    HERO_UI_GTK_NEED(texture_download, "gdk_texture_download");
    HERO_UI_GTK_NEED(io_error_quark, "g_io_error_quark");
    HERO_UI_GTK_NEED(cancellable_new, "g_cancellable_new");
    HERO_UI_GTK_NEED(cancellable_cancel, "g_cancellable_cancel");
    HERO_UI_GTK_NEED(column_view_new, "gtk_column_view_new");
    HERO_UI_GTK_NEED(column_view_append_column, "gtk_column_view_append_column");
    HERO_UI_GTK_NEED(column_view_remove_column, "gtk_column_view_remove_column");
    HERO_UI_GTK_NEED(column_view_get_sorter, "gtk_column_view_get_sorter");
    HERO_UI_GTK_NEED(column_view_sort_by_column, "gtk_column_view_sort_by_column");
    HERO_UI_GTK_NEED(column_view_set_reorderable, "gtk_column_view_set_reorderable");
    HERO_UI_GTK_NEED(column_view_column_new, "gtk_column_view_column_new");
    HERO_UI_GTK_NEED(column_view_column_set_sorter, "gtk_column_view_column_set_sorter");
    HERO_UI_GTK_NEED(column_view_column_set_resizable, "gtk_column_view_column_set_resizable");
    HERO_UI_GTK_NEED(custom_sorter_new, "gtk_custom_sorter_new");
    HERO_UI_GTK_NEED(signal_list_item_factory_new, "gtk_signal_list_item_factory_new");
    HERO_UI_GTK_NEED(list_item_set_child, "gtk_list_item_set_child");
    HERO_UI_GTK_NEED(list_item_get_child, "gtk_list_item_get_child");
    HERO_UI_GTK_NEED(list_item_get_item, "gtk_list_item_get_item");
    HERO_UI_GTK_NEED(multi_selection_new, "gtk_multi_selection_new");
    HERO_UI_GTK_NEED(single_selection_new, "gtk_single_selection_new");
    HERO_UI_GTK_NEED(single_selection_set_autoselect, "gtk_single_selection_set_autoselect");
    HERO_UI_GTK_NEED(single_selection_set_can_unselect, "gtk_single_selection_set_can_unselect");
    HERO_UI_GTK_NEED(single_selection_get_selected, "gtk_single_selection_get_selected");
    HERO_UI_GTK_NEED(single_selection_set_selected, "gtk_single_selection_set_selected");
    HERO_UI_GTK_NEED(selection_model_select_item, "gtk_selection_model_select_item");
    HERO_UI_GTK_NEED(selection_model_unselect_all, "gtk_selection_model_unselect_all");
    HERO_UI_GTK_NEED(selection_model_is_selected, "gtk_selection_model_is_selected");
    HERO_UI_GTK_NEED(string_object_get_string, "gtk_string_object_get_string");
    HERO_UI_GTK_NEED(string_list_get_string, "gtk_string_list_get_string");
    HERO_UI_GTK_NEED(tree_list_model_new, "gtk_tree_list_model_new");
    HERO_UI_GTK_NEED(tree_list_model_get_row, "gtk_tree_list_model_get_row");
    HERO_UI_GTK_NEED(tree_list_model_get_child_row, "gtk_tree_list_model_get_child_row");
    HERO_UI_GTK_NEED(tree_list_row_get_child_row, "gtk_tree_list_row_get_child_row");
    HERO_UI_GTK_NEED(tree_list_row_get_item, "gtk_tree_list_row_get_item");
    HERO_UI_GTK_NEED(tree_list_row_get_expanded, "gtk_tree_list_row_get_expanded");
    HERO_UI_GTK_NEED(tree_list_row_set_expanded, "gtk_tree_list_row_set_expanded");
    HERO_UI_GTK_NEED(tree_list_row_get_position, "gtk_tree_list_row_get_position");
    HERO_UI_GTK_NEED(tree_expander_new, "gtk_tree_expander_new");
    HERO_UI_GTK_NEED(tree_expander_set_list_row, "gtk_tree_expander_set_list_row");
    HERO_UI_GTK_NEED(tree_expander_set_child, "gtk_tree_expander_set_child");
    HERO_UI_GTK_NEED(list_view_new, "gtk_list_view_new");
    HERO_UI_GTK_NEED(object_ref, "g_object_ref");
    HERO_UI_GTK_NEED(object_set_data, "g_object_set_data");
    HERO_UI_GTK_NEED(object_get_data, "g_object_get_data");
    HERO_UI_GTK_NEED(signal_handlers_disconnect_matched, "g_signal_handlers_disconnect_matched");
    HERO_UI_GTK_NEED(native_get_surface, "gtk_native_get_surface");
    HERO_UI_GTK_NEED(display_get_monitors, "gdk_display_get_monitors");
    HERO_UI_GTK_NEED(monitor_get_geometry, "gdk_monitor_get_geometry");
    HERO_UI_GTK_NEED(monitor_get_scale_factor, "gdk_monitor_get_scale_factor");
    HERO_UI_GTK_NEED(surface_get_scale_factor, "gdk_surface_get_scale_factor");
    HERO_UI_GTK_NEED(surface_get_width, "gdk_surface_get_width");
    HERO_UI_GTK_NEED(surface_get_height, "gdk_surface_get_height");
    HERO_UI_GTK_NEED(window_maximize, "gtk_window_maximize");
    HERO_UI_GTK_NEED(window_unmaximize, "gtk_window_unmaximize");
    HERO_UI_GTK_NEED(window_minimize, "gtk_window_minimize");
    HERO_UI_GTK_NEED(window_unminimize, "gtk_window_unminimize");
    HERO_UI_GTK_NEED(window_fullscreen, "gtk_window_fullscreen");
    HERO_UI_GTK_NEED(window_unfullscreen, "gtk_window_unfullscreen");
    HERO_UI_GTK_NEED(toplevel_get_state, "gdk_toplevel_get_state");
    HERO_UI_GTK_NEED(toplevel_set_icon_list, "gdk_toplevel_set_icon_list");
    HERO_UI_GTK_NEED(window_set_decorated, "gtk_window_set_decorated");
    HERO_UI_GTK_NEED(widget_set_opacity, "gtk_widget_set_opacity");
    HERO_UI_GTK_NEED(window_set_modal, "gtk_window_set_modal");
    HERO_UI_GTK_NEED(window_set_transient_for, "gtk_window_set_transient_for");
    HERO_UI_GTK_NEED(widget_get_mapped, "gtk_widget_get_mapped");
    HERO_UI_GTK_NEED(window_is_active, "gtk_window_is_active");
    HERO_UI_GTK_NEED(text_grab_focus_without_selecting, "gtk_text_grab_focus_without_selecting");
    HERO_UI_GTK_NEED(window_get_toplevels, "gtk_window_get_toplevels");
    HERO_UI_GTK_NEED(widget_get_first_child, "gtk_widget_get_first_child");
    HERO_UI_GTK_NEED(widget_get_next_sibling, "gtk_widget_get_next_sibling");
    HERO_UI_GTK_NEED(button_get_type, "gtk_button_get_type");
    HERO_UI_GTK_NEED(button_get_label, "gtk_button_get_label");
    HERO_UI_GTK_NEED(native_dialog_hide, "gtk_native_dialog_hide");
    HERO_UI_GTK_NEED(bus_get_sync, "g_bus_get_sync");
    HERO_UI_GTK_NEED(dbus_connection_call_sync, "g_dbus_connection_call_sync");
    HERO_UI_GTK_NEED(variant_new, "g_variant_new");
    HERO_UI_GTK_NEED(variant_new_strv, "g_variant_new_strv");
    HERO_UI_GTK_NEED(variant_new_array, "g_variant_new_array");
    HERO_UI_GTK_NEED(variant_unref, "g_variant_unref");
    HERO_UI_GTK_NEED(get_prgname, "g_get_prgname");
    HERO_UI_GTK_NEED(object_class_find_property, "g_object_class_find_property");
    HERO_UI_GTK_NEED(object_set, "g_object_set");
    HERO_UI_GTK_NEED(accessible_update_property, "gtk_accessible_update_property");
    HERO_UI_GTK_NEED(accessible_reset_property, "gtk_accessible_reset_property");
    HERO_UI_GTK_NEED(accessible_update_relation, "gtk_accessible_update_relation");
    HERO_UI_GTK_NEED(accessible_reset_relation, "gtk_accessible_reset_relation");
    HERO_UI_GTK_NEED(get_minor_version, "gtk_get_minor_version");
    HERO_UI_GTK_NEED(list_append, "g_list_append");
    HERO_UI_GTK_NEED(list_free, "g_list_free");
    HERO_UI_GTK_NEED(type_check_instance_is_a, "g_type_check_instance_is_a");
    HERO_UI_GTK_MAY(file_list_get_files, "gdk_file_list_get_files");
    HERO_UI_GTK_MAY(file_dialog_select_folder, "gtk_file_dialog_select_folder");
    HERO_UI_GTK_MAY(file_dialog_select_folder_finish, "gtk_file_dialog_select_folder_finish");
    HERO_UI_GTK_MAY(file_dialog_select_multiple_folders, "gtk_file_dialog_select_multiple_folders");
    HERO_UI_GTK_MAY(file_dialog_select_multiple_folders_finish, "gtk_file_dialog_select_multiple_folders_finish");
    HERO_UI_GTK_MAY(texture_downloader_new, "gdk_texture_downloader_new");
    HERO_UI_GTK_MAY(texture_downloader_set_format, "gdk_texture_downloader_set_format");
    HERO_UI_GTK_MAY(texture_downloader_download_into, "gdk_texture_downloader_download_into");
    HERO_UI_GTK_MAY(texture_downloader_free, "gdk_texture_downloader_free");
    HERO_UI_GTK_MAY(column_view_sorter_get_primary_sort_column, "gtk_column_view_sorter_get_primary_sort_column");
    HERO_UI_GTK_MAY(column_view_sorter_get_primary_sort_order, "gtk_column_view_sorter_get_primary_sort_order");
    HERO_UI_GTK_MAY(monitor_get_scale, "gdk_monitor_get_scale");
    HERO_UI_GTK_MAY(x11_display_get_type, "gdk_x11_display_get_type");
    HERO_UI_GTK_MAY(x11_display_get_xdisplay, "gdk_x11_display_get_xdisplay");
    HERO_UI_GTK_MAY(x11_display_get_xrootwindow, "gdk_x11_display_get_xrootwindow");
    HERO_UI_GTK_MAY(x11_surface_get_xid, "gdk_x11_surface_get_xid");
    HERO_UI_GTK_MAY(x11_get_xatom_by_name_for_display, "gdk_x11_get_xatom_by_name_for_display");
    HERO_UI_GTK_MAY(x11_monitor_get_workarea, "gdk_x11_monitor_get_workarea");
    HERO_UI_GTK_MAY(x_move_window, "XMoveWindow");
    HERO_UI_GTK_MAY(x_translate_coordinates, "XTranslateCoordinates");
    HERO_UI_GTK_MAY(x_get_window_property, "XGetWindowProperty");
    HERO_UI_GTK_MAY(x_free, "XFree");
    HERO_UI_GTK_MAY(x_sync, "XSync");
    HERO_UI_GTK_MAY(x_send_event, "XSendEvent");
    HERO_UI_GTK_MAY(x_keysym_to_keycode, "XKeysymToKeycode");
    HERO_UI_GTK_MAY(x_get_window_attributes, "XGetWindowAttributes");
    HERO_UI_GTK_MAY(x_get_input_focus, "XGetInputFocus");
    HERO_UI_GTK_MAY(x_set_input_focus, "XSetInputFocus");
    HERO_UI_GTK_MAY(x_query_tree, "XQueryTree");
    HERO_UI_GTK_MAY(x11_display_get_primary_monitor, "gdk_x11_display_get_primary_monitor");
    HERO_UI_GTK_MAY(wayland_display_get_type, "gdk_wayland_display_get_type");
    {
        /* X11's XTest extension, the one way X11 lets a client press a key
         * as the keyboard does; GTK needs none, so it may be missing. */
        void *xtest = dlopen(HERO_UI_GTK_XTEST_LIBRARY, RTLD_NOW | RTLD_LOCAL);

        if (xtest != NULL) {
            (void)hero_ui_gtk_find(xtest, "XTestQueryExtension", &hero_ui_gtk.x_test_query_extension, sizeof hero_ui_gtk.x_test_query_extension);
            (void)hero_ui_gtk_find(xtest, "XTestFakeKeyEvent", &hero_ui_gtk.x_test_fake_key_event, sizeof hero_ui_gtk.x_test_fake_key_event);
        }
    }
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
    /* A menu item's check mode (`HERO_UI_CHECK_*`), whether it is checked,
     * and its role. */
    int64_t check;
    bool checked;
    int64_t role;
    /* The widget's context menu, 0 for none; a context menu's popover, its
     * click and its keys, on the widget `inner` names. */
    int64_t context;
    void *popover;
    void *gesture;
    void *keys;
    /* A button's key (`HERO_UI_BUTTON_KEY_*`) and the controller holding its
     * Escape; a window's buttons for Return and Escape, 0 for none. */
    int64_t button_key;
    void *key_controller;
    int64_t enter_button;
    int64_t escape_button;
    /* A text field drawn masked; a progress bar's pulse, 0 while it stands. */
    bool masked;
    unsigned pulse;
    /* A split's divider: its size along the split's axis when its place was
     * last told, and whether a pane is its first or its second. */
    int split_extent;
    int pane_slot;
    /* A table's or a tree's selection model, a table's columns and cells, a
     * tree's items (A TABLE, A TREE). */
    void *selection;
    void **columns;
    int64_t column_count;
    char **cells;
    int64_t rows;
    struct hero_ui_gtk_tree *tree;
    /* A window among the screens (WINDOWS ON THE SCREEN): the place the
     * program asked while it was not shown, whether it is watched, what was
     * told of it last, and whether the program's own call is moving it. */
    bool want_placed;
    int64_t want_screen;
    int64_t want_x;
    int64_t want_y;
    bool watched;
    bool quiet;
    int64_t told_state;
    int64_t told_screen;
    int64_t told_x;
    int64_t told_y;
    bool on_top;
    int64_t asked_state;
    bool placed_known;
    bool focus_given;
    bool want_framed;
    /* What a move asks beyond the frame's place to land it there: learned
     * from the window manager's last move, the gravity it reads. */
    bool move_known;
    int64_t move_dx;
    int64_t move_dy;
    /* Whether the content size is the program's, not yet given by GTK, so
     * that a content of another size in a window of the size asked is GTK
     * still laying the window out rather than the person's resize. */
    bool content_asked;
    /* A table's sorter; a split's panes. */
    void *sorter;
    int64_t panes;
    /* The accessible name the program gave, NULL for none. */
    char *accessible;
};

/* What the calls below `hero_ui_arm_create` define and it calls. */
static bool hero_ui_gtk_make_box(hero_ui_native *made, hero_ui_native *owner);
static bool hero_ui_gtk_make_table(hero_ui_native *made);
static bool hero_ui_gtk_make_tree(hero_ui_native *made);
static void hero_ui_gtk_free_view(hero_ui_native *widget);
static int64_t hero_ui_gtk_hang(hero_ui_native *made, hero_ui_native *owner);
static int64_t hero_ui_gtk_attach_context(hero_ui_native *menu, hero_ui_native *owner);
static void hero_ui_gtk_detach_context(hero_ui_native *menu);
static void hero_ui_gtk_returned(void *entry, void *data);
static void hero_ui_gtk_row_opened(void *box, void *row, void *data);
static void hero_ui_gtk_realized(void *window, void *data);
static void hero_ui_gtk_mapped(void *window, void *data);
static void hero_ui_gtk_unhang(hero_ui_native *widget, hero_ui_native *owner);
static void hero_ui_gtk_forget_keys(hero_ui_native *button);
static bool hero_ui_gtk_takes_keys(int64_t kind);
static int64_t hero_ui_gtk_tree_select(hero_ui_native *tree, int64_t item);
static int64_t hero_ui_gtk_tree_chosen(const hero_ui_native *tree);
static int64_t hero_ui_gtk_context_refill(hero_ui_native *owner);
static int hero_ui_gtk_divider(hero_ui_native *split);
static int64_t hero_ui_gtk_act(int64_t kind, hero_ui_native *widget, int64_t value, int64_t second, bool usable);
static int64_t hero_ui_gtk_table_rows(hero_ui_native *table, const uint8_t *items, int64_t count);
static void hero_ui_gtk_close_more(void);
static int64_t hero_ui_gtk_said_so(int64_t kind, const char *words);
static void hero_ui_gtk_send_docked(hero_ui_native *window);
static void hero_ui_gtk_watch_appearance(void);
static void hero_ui_gtk_unwatch_appearance(void);
static bool hero_ui_gtk_keep_path(void *file, uint8_t **paths, int64_t *count, int64_t *room);
static void *hero_ui_gtk_filters(const uint8_t *filters, int64_t count, int64_t *made);

/* Above 0 while `inject` acts, so a slider moved to where it stands is told. */
static _Thread_local int64_t hero_ui_gtk_injecting;

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
    if (widget->widget == instance || widget->inner == instance || widget->buffer == instance || widget->action == instance || widget->area == instance || widget->selection == instance || widget->sorter == instance ||
        widget->gesture == instance) {
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
    window->content_asked = true;
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
        window->content_asked = false;
        return;
    }
    if (window->content_asked && !window->resize_injected && hero_ui_gtk.widget_get_width(window->widget) == window->asked_width && hero_ui_gtk.widget_get_height(window->widget) == window->asked_height) {
        /* The window took the size asked and its content another: the room
         * the menu bar and the title bar take was not yet known. */
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_resize_again, data);
        return;
    }
    window->resize_injected = false;
    window->content_asked = false;
    window->content_width = width;
    window->content_height = height;
    hero_ui_tell(HERO_UI_EVENT_RESIZE, window->id, width, height);
}

/* The modal window (`hero_ui_set_modal`), 0 while none is. */
static _Thread_local int64_t hero_ui_gtk_modal;

/* The person asked window `instance` to close: told, and nothing closed;
 * while another window is modal, the others take no act of the person's. */
static int hero_ui_gtk_close_asked(void *instance, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, instance);

    if (window != NULL && window->kind == HERO_UI_WINDOW && (hero_ui_gtk_modal == 0 || hero_ui_gtk_modal == window->id)) {
        hero_ui_tell(HERO_UI_EVENT_CLOSE, window->id, 0, 0);
    }
    return 1;
}

/* Whether menu item `item`, each menu it stands in and the showing of each
 * let the person choose it, and, in a context menu, whether the widget the
 * menu belongs to takes the person's acts. */
static bool hero_ui_gtk_usable(const hero_ui_native *item) {
    const hero_ui_native *at = item;

    if (item->kind != HERO_UI_MENU_ITEM) {
        return false;
    }
    while (at != NULL && (at->kind == HERO_UI_MENU_ITEM || at->kind == HERO_UI_MENU || at->kind == HERO_UI_CONTEXT_MENU)) {
        if (!at->enabled || !at->visible) {
            return false;
        }
        if (at->kind == HERO_UI_CONTEXT_MENU) {
            const hero_ui_native *owner = hero_ui_find(at->parent);

            return owner != NULL && owner->enabled;
        }
        at = hero_ui_find(at->parent);
    }
    return at != NULL && at->kind == HERO_UI_WINDOW;
}

/* Whether menu item `item` stands in a context menu, at any depth: its
 * shortcut is shown there and takes no key. */
static bool hero_ui_gtk_in_context(const hero_ui_native *item) {
    for (const hero_ui_native *at = hero_ui_find(item->parent); at != NULL; at = hero_ui_find(at->parent)) {
        if (at->kind == HERO_UI_CONTEXT_MENU) {
            return true;
        }
        if (at->kind != HERO_UI_MENU) {
            return false;
        }
    }
    return false;
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
        if (item->kind == HERO_UI_MENU && hero_ui_gtk_fill_menu(item) != HERO_UI_OK) {
            answer = HERO_UI_LIMIT;
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
            if (item->kind == HERO_UI_MENU) {
                /* A menu inside this one: its own model, filled above. */
                hero_ui_gtk.menu_append_submenu(section, label, item->menu_model);
                hero_layer_free(label);
                in_section += 1;
                continue;
            }
            if (item->check == HERO_UI_CHECK_RADIO) {
                /* A radio item is drawn so where its action's state is a
                 * string and the item names one, its target. */
                entry = hero_ui_gtk.menu_item_new(label, NULL);
                hero_ui_gtk.menu_item_set_action_and_target_value(entry, action, hero_ui_gtk.variant_new_string(HERO_UI_GTK_RADIO_ON));
            } else {
                entry = hero_ui_gtk.menu_item_new(label, action);
            }
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

/* Shows menu item `item`'s check as its action's state. */
static void hero_ui_gtk_show_check(hero_ui_native *item) {
    if (item->action == NULL || item->check == HERO_UI_CHECK_NONE) {
        return;
    }
    if (item->check == HERO_UI_CHECK_BOX) {
        hero_ui_gtk.simple_action_set_state(item->action, hero_ui_gtk.variant_new_boolean(item->checked ? 1 : 0));
    } else {
        hero_ui_gtk.simple_action_set_state(item->action, hero_ui_gtk.variant_new_string(item->checked ? HERO_UI_GTK_RADIO_ON : HERO_UI_GTK_RADIO_OFF));
    }
}

/* Checks radio item `item` and unchecks the other radio items of its group:
 * the radio items of its menu no other item or separator stands between. */
static void hero_ui_gtk_check_radio(hero_ui_native *item) {
    hero_ui_native *menu = hero_ui_find(item->parent);
    int64_t at = -1;

    item->checked = true;
    hero_ui_gtk_show_check(item);
    for (int64_t i = 0; menu != NULL && i < menu->order.count; i += 1) {
        at = menu->order.at[i] == item->id ? i : at;
    }
    for (int64_t step = -1; at >= 0 && step <= 1; step += 2) {
        for (int64_t i = at + step; i >= 0 && i < menu->order.count; i += step) {
            hero_ui_native *other = hero_ui_find(menu->order.at[i]);

            if (other == NULL || other->kind != HERO_UI_MENU_ITEM || other->check != HERO_UI_CHECK_RADIO) {
                break;
            }
            other->checked = false;
            hero_ui_gtk_show_check(other);
        }
    }
}

/* An item's action was activated: by its menu, its shortcut, or `inject`.
 * A check item turns over and a radio item is checked alone in its group
 * before the choice is told, its `value` the state after. */
static void hero_ui_gtk_chosen(void *action, void *parameter, void *data) {
    hero_ui_native *item = hero_ui_gtk_of(data, action);

    (void)parameter;
    if (item == NULL || !hero_ui_gtk_usable(item)) {
        return;
    }
    if (item->check == HERO_UI_CHECK_BOX) {
        item->checked = !item->checked;
        hero_ui_gtk_show_check(item);
    } else if (item->check == HERO_UI_CHECK_RADIO) {
        hero_ui_gtk_check_radio(item);
    }
    hero_ui_tell(HERO_UI_EVENT_MENU, item->id, item->check != HERO_UI_CHECK_NONE && item->checked ? 1 : 0, 0);
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
    hero_ui_gtk_watch_appearance();
    return HERO_UI_OK;
}

/* Whether a frame, a window or one of the boxes the system draws whose
 * content holds widgets: a page, a scrolled box, a pane. */
static bool hero_ui_gtk_is_frame(int64_t kind) {
    return kind == HERO_UI_WINDOW || kind == HERO_UI_PAGE || kind == HERO_UI_SCROLL || kind == HERO_UI_PANE;
}

/* The `GtkFixed` `widget` stands in: the content of the window, page,
 * scrolled box or pane that is its parent; NULL for what stands in none (a
 * page in its tabs, a pane in its split, a menu and its items). */
static void *hero_ui_gtk_container(const hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);

    return owner != NULL && hero_ui_gtk_is_frame(owner->kind) ? owner->fixed : NULL;
}

/* Whether `owner` may hold a widget of `kind`: a menu's items in a menu or a
 * context menu, a menu in a window's bar, a menu or a context menu, a page
 * in tabs, a pane in a split holding fewer than two, one context menu on a
 * widget that is none of the menus' and holds none, and every other kind in
 * a frame. */
static bool hero_ui_gtk_takes(const hero_ui_native *owner, int64_t kind) {
    switch (kind) {
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        return owner->kind == HERO_UI_MENU || owner->kind == HERO_UI_CONTEXT_MENU;
    case HERO_UI_MENU:
        return owner->kind == HERO_UI_WINDOW || owner->kind == HERO_UI_MENU || owner->kind == HERO_UI_CONTEXT_MENU;
    case HERO_UI_PAGE:
        return owner->kind == HERO_UI_TABS;
    case HERO_UI_PANE:
        return (owner->kind == HERO_UI_SPLIT_ROW || owner->kind == HERO_UI_SPLIT_COLUMN) && owner->panes < 2;
    case HERO_UI_CONTEXT_MENU:
        return owner->context == 0 && owner->kind != HERO_UI_MENU && owner->kind != HERO_UI_MENU_ITEM && owner->kind != HERO_UI_MENU_SEPARATOR && owner->kind != HERO_UI_CONTEXT_MENU && owner->kind != HERO_UI_PAGE &&
               owner->kind != HERO_UI_PANE;
    default:
        return hero_ui_gtk_is_frame(owner->kind);
    }
}

/* Destroys `widget`'s GTK objects and gives back its record; its children
 * are gone already. */
static void hero_ui_gtk_drop(hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);

    switch (widget->kind) {
    case HERO_UI_WINDOW:
        if (hero_ui_gtk_modal == widget->id) {
            hero_ui_gtk_modal = 0;
        }
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
    case HERO_UI_CONTEXT_MENU:
        if (widget->kind == HERO_UI_CONTEXT_MENU) {
            hero_ui_gtk_detach_context(widget);
        } else if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)(owner->kind == HERO_UI_WINDOW ? hero_ui_gtk_fill_bar(owner) : hero_ui_gtk_fill_menu(owner));
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
    case HERO_UI_PAGE:
    case HERO_UI_PANE:
        hero_ui_gtk_unhang(widget, owner);
        break;
    default: {
        void *container = hero_ui_gtk_container(widget);

        if (widget->kind == HERO_UI_WEBVIEW) {
            /* The member lets go of its page and handlers and calls the
             * host's `gone`, and then its widget goes as any other. */
            (void)hero_ui_web_gtk_destroy(widget->id);
        }
        if (widget->kind == HERO_UI_BUTTON) {
            hero_ui_gtk_forget_keys(widget);
        }
        if (widget->pulse != 0) {
            (void)hero_ui_gtk.source_remove(widget->pulse);
            widget->pulse = 0;
        }
        if (container != NULL && widget->widget != NULL) {
            hero_ui_gtk.fixed_remove(container, widget->widget);
        }
        hero_ui_gtk_free_view(widget);
        break;
    }
    }
    hero_layer_free(widget->radios);
    hero_layer_free(widget->radio_places);
    hero_layer_free(widget->label);
    hero_layer_free(widget->accessible);
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
    hero_ui_gtk_close_more();
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

/* Puts `made`'s widget in the content of the frame it was made in, at no
 * place and hidden. */
static void hero_ui_gtk_stand(hero_ui_native *made) {
    hero_ui_gtk.widget_set_visible(made->widget, 0);
    hero_ui_gtk.fixed_put(hero_ui_gtk_container(made), made->widget, 0, 0);
}

/* Makes web view `id` in window `window`: THE WEB VIEW, kind 17, is
 * `hero_ui_web_gtk.c`'s, which answers WebKitGTK's widget; this arm stands
 * it in the window's content as any widget, hidden until placed. */
static int64_t hero_ui_gtk_make_web(int64_t id, hero_ui_native *owner, hero_ui_native *window) {
    hero_ui_native *made = hero_layer_alloc_zero(1, sizeof *made);
    int64_t answer;

    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    made->id = id;
    made->kind = HERO_UI_WEBVIEW;
    made->parent = owner->id;
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
    hero_ui_gtk_stand(made);
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
    if (kind == HERO_UI_TRAY) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "GTK 4 has no tray icon (GTK 3's GtkStatusIcon is gone), and this arm draws no StatusNotifierItem over D-Bus in its place");
    }
    if (kind == HERO_UI_TABLE && (hero_ui_gtk.column_view_sorter_get_primary_sort_column == NULL || hero_ui_gtk.column_view_sorter_get_primary_sort_order == NULL)) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a table's sort is read from GTK 4.10's gtk_column_view_sorter_get_primary_sort_column, which this GTK lacks");
    }
    if (kind != HERO_UI_WINDOW) {
        owner = hero_ui_find(parent);
        if (owner == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        if (!hero_ui_gtk_takes(owner, kind)) {
            return HERO_UI_INVALID;
        }
        window = hero_ui_gtk_window(owner->window);
        if (window == NULL) {
            return HERO_UI_NOT_FOUND;
        }
    }
    if (kind == HERO_UI_WEBVIEW) {
        return hero_ui_gtk_make_web(id, owner, window);
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
        hero_ui_gtk_connect(made->widget, "activate", (hero_ui_gtk_callback)hero_ui_gtk_returned, id);
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
        hero_ui_gtk.list_box_set_activate_on_single_click(made->inner, 0);
        hero_ui_gtk_connect(made->inner, "selected-rows-changed", (hero_ui_gtk_callback)hero_ui_gtk_rows_chosen, id);
        hero_ui_gtk_connect(made->inner, "row-activated", (hero_ui_gtk_callback)hero_ui_gtk_row_opened, id);
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
    case HERO_UI_TABS:
    case HERO_UI_PAGE:
    case HERO_UI_SCROLL:
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
    case HERO_UI_PANE:
        ok = hero_ui_gtk_make_box(made, owner);
        break;
    case HERO_UI_TABLE:
        ok = hero_ui_gtk_make_table(made);
        break;
    case HERO_UI_TREE:
        ok = hero_ui_gtk_make_tree(made);
        break;
    case HERO_UI_CONTEXT_MENU:
        made->menu_model = hero_ui_gtk.menu_new();
        ok = made->menu_model != NULL;
        break;
    default:
        hero_layer_free(made);
        return HERO_UI_INVALID;
    }
    if (!ok || (made->widget == NULL && kind != HERO_UI_WINDOW && kind != HERO_UI_MENU && kind != HERO_UI_MENU_ITEM && kind != HERO_UI_MENU_SEPARATOR && kind != HERO_UI_CONTEXT_MENU) || !hero_ui_keep(id, made)) {
        if (made->tree != NULL || made->selection != NULL) {
            hero_ui_gtk_free_view(made);
        }
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
        hero_ui_gtk_connect(made->widget, "realize", (hero_ui_gtk_callback)hero_ui_gtk_realized, id);
        hero_ui_gtk_connect(made->widget, "map", (hero_ui_gtk_callback)hero_ui_gtk_mapped, id);
        return HERO_UI_OK;
    case HERO_UI_MENU:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_gtk_drop(made);
            return HERO_UI_LIMIT;
        }
        return owner->kind == HERO_UI_WINDOW ? hero_ui_gtk_fill_bar(owner) : hero_ui_gtk_fill_menu(owner);
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
    case HERO_UI_PAGE:
    case HERO_UI_PANE:
        return hero_ui_gtk_hang(made, owner);
    case HERO_UI_CONTEXT_MENU:
        return hero_ui_gtk_attach_context(made, owner);
    default:
        hero_ui_gtk_stand(made);
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

    /* The program's text is no step the person's Undo takes back. */
    hero_ui_gtk.text_buffer_begin_irreversible_action(widget->buffer);
    hero_ui_gtk.text_buffer_set_text(widget->buffer, text, -1);
    hero_ui_gtk.text_buffer_end_irreversible_action(widget->buffer);
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
    case HERO_UI_PAGE:
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
    case HERO_UI_PAGE: {
        hero_ui_native *tabs = hero_ui_find(widget->parent);

        if (tabs != NULL) {
            hero_ui_gtk.notebook_set_tab_label_text(tabs->widget, widget->widget, made);
        }
        break;
    }
    default:
        hero_layer_free(widget->label);
        widget->label = made;
        made = NULL;
        break;
    }
    hero_ui_mute(-1);
    hero_layer_free(made);
    if (widget->kind == HERO_UI_MENU) {
        hero_ui_native *owner = hero_ui_find(widget->parent);

        return owner->kind == HERO_UI_WINDOW ? hero_ui_gtk_fill_bar(owner) : hero_ui_gtk_fill_menu(owner);
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
    if (widget->kind == HERO_UI_TABLE) {
        return hero_ui_gtk_table_rows(widget, items, count);
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
    } else if (widget->pulse == 0) {
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
    case HERO_UI_MENU_ITEM:
        if (widget->check == HERO_UI_CHECK_NONE || (value != 0 && value != 1)) {
            answer = HERO_UI_INVALID;
            break;
        }
        if (widget->check == HERO_UI_CHECK_RADIO && value == 1) {
            hero_ui_gtk_check_radio(widget);
        } else {
            widget->checked = value == 1;
            hero_ui_gtk_show_check(widget);
        }
        break;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        if (value < 0) {
            answer = HERO_UI_INVALID;
            break;
        }
        widget->value = value;
        hero_ui_gtk.paned_set_position(widget->widget, hero_ui_gtk_int(value));
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
    if (widget->kind == HERO_UI_SPLIT_ROW || widget->kind == HERO_UI_SPLIT_COLUMN) {
        /* Where the person may drag the divider: kept by `hero_ui_gtk_divided`. */
        widget->least = least;
        widget->most = most;
        return HERO_UI_OK;
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
    if (widget->kind == HERO_UI_TABS) {
        if (count != 1 || indices[0] >= hero_ui_gtk.notebook_get_n_pages(widget->widget)) {
            return HERO_UI_INVALID;
        }
        hero_ui_mute(1);
        hero_ui_gtk.notebook_set_current_page(widget->widget, (int)indices[0]);
        hero_ui_mute(-1);
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_TABLE) {
        for (int64_t i = 0; i < count; i += 1) {
            if (indices[i] >= widget->rows) {
                return HERO_UI_INVALID;
            }
        }
        hero_ui_mute(1);
        (void)hero_ui_gtk.selection_model_unselect_all(widget->selection);
        for (int64_t i = 0; i < count; i += 1) {
            (void)hero_ui_gtk.selection_model_select_item(widget->selection, (unsigned)indices[i], 0);
        }
        hero_ui_mute(-1);
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_TREE) {
        if (count > 1) {
            return HERO_UI_INVALID;
        }
        return hero_ui_gtk_tree_select(widget, count == 1 ? indices[0] : -1);
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
        case HERO_UI_MENU: {
            hero_ui_native *owner = hero_ui_find(widget->parent);

            return owner->kind == HERO_UI_WINDOW ? hero_ui_gtk_fill_bar(owner) : hero_ui_gtk_fill_menu(owner);
        }
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
        case HERO_UI_CONTEXT_MENU:
            return hero_ui_gtk_fill_menu(widget->kind == HERO_UI_CONTEXT_MENU ? widget : hero_ui_find(widget->parent));
        case HERO_UI_PAGE:
        case HERO_UI_PANE:
            /* The system sizes them, so they stand shown as the program asks. */
            hero_ui_gtk.widget_set_visible(widget->widget, on == 1 ? 1 : 0);
            return HERO_UI_OK;
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
        case HERO_UI_CONTEXT_MENU:
            return hero_ui_gtk_fill_menu(widget);
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
        default:
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                hero_ui_gtk.widget_set_sensitive(widget->radios[i], on == 1 ? 1 : 0);
            }
            hero_ui_gtk.widget_set_sensitive(widget->widget, on == 1 ? 1 : 0);
            /* A greyed-out widget's context menu chooses nothing. */
            return hero_ui_gtk_context_refill(widget);
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
    /* A context menu's item shows its shortcut and takes no key, as the
     * systems' context menus do. */
    if (keyval != 0 && !hero_ui_gtk_in_context(item)) {
        char action[64];
        void *trigger = hero_ui_gtk.keyval_trigger_new(keyval, item->held);
        void *named;

        hero_ui_gtk_action_name(item->id, true, action, sizeof action);
        named = hero_ui_gtk.named_action_new(action);
        item->shortcut = trigger != NULL && named != NULL ? hero_ui_gtk.shortcut_new(trigger, named) : NULL;
        if (item->shortcut == NULL) {
            return HERO_UI_LIMIT;
        }
        if (item->check == HERO_UI_CHECK_RADIO) {
            hero_ui_gtk.shortcut_set_arguments(item->shortcut, hero_ui_gtk.variant_new_string(HERO_UI_GTK_RADIO_ON));
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
    if (window->kind == HERO_UI_SCROLL && which == HERO_UI_SIZE_CONTENT) {
        /* A scrolled box's content: its fixed's size, which the scrolled
         * window scrolls over. */
        hero_ui_gtk.widget_set_size_request(window->fixed, hero_ui_gtk_int(width), hero_ui_gtk_int(height));
        return HERO_UI_OK;
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
    if (widget->widget == NULL || widget->kind == HERO_UI_WINDOW || widget->kind == HERO_UI_PAGE || widget->kind == HERO_UI_PANE || container == NULL) {
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
    case HERO_UI_TREE:
        size.width = 160;
        size.height = 96;
        break;
    case HERO_UI_TABLE:
        /* The headless side's: a hundred points a column, at least a text
         * field's width, and a row of headers above a text area's height. */
        size.width = 100 * widget->column_count > 160 ? 100 * (int)widget->column_count : 160;
        size.height = 96 + 24;
        break;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN: {
        int thick = hero_ui_gtk_divider(widget);

        *width = thick;
        *height = thick;
        return HERO_UI_OK;
    }
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
    case HERO_UI_MENU_ITEM:
        if (widget->check == HERO_UI_CHECK_NONE) {
            return HERO_UI_INVALID;
        }
        *value = widget->checked ? 1 : 0;
        return HERO_UI_OK;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        *value = hero_ui_gtk.paned_get_position(widget->widget);
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
    if (widget->kind == HERO_UI_TABLE) {
        int64_t found = 0;

        for (int64_t i = 0; i < widget->rows; i += 1) {
            if (hero_ui_gtk.selection_model_is_selected(widget->selection, (unsigned)i)) {
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
    } else if (widget->kind == HERO_UI_TABS) {
        one = hero_ui_gtk.notebook_get_current_page(widget->widget);
    } else if (widget->kind == HERO_UI_TREE) {
        int64_t item = hero_ui_gtk_tree_chosen(widget);

        one = item > 0 ? item : -1;
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
            answer = hero_ui_gtk_act(kind, widget, value, second, usable);
        }
        break;
    case HERO_UI_EVENT_VALUE:
        if (widget->kind == HERO_UI_SPLIT_ROW || widget->kind == HERO_UI_SPLIT_COLUMN) {
            answer = hero_ui_gtk_act(kind, widget, value, second, usable);
        } else if (widget->kind != HERO_UI_SLIDER) {
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
            /* A radio item's action takes its target, as its menu hands it. */
            hero_ui_gtk.action_activate(widget->action, widget->check == HERO_UI_CHECK_RADIO ? hero_ui_gtk.variant_new_string(HERO_UI_GTK_RADIO_ON) : NULL);
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
        answer = hero_ui_gtk_act(kind, widget, value, second, usable);
        break;
    }
    hero_ui_gtk_injecting -= 1;
    return answer;
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

/* ==========================================================================
 * WHAT `hero_ui.c` ADDED AFTER THE WEB VIEW: boxes the system draws, a row
 * opened, Return and Escape, the widget with the keys, menus inside menus,
 * check and radio items, context menus, a masked field, a moving bar, the
 * clipboard, a table, a tree, windows among the screens, a key pressed,
 * dialogs answered through the queue, the task bar, a notification, the
 * appearance and accessible names.
 *
 * WHAT GTK 4 OR ITS DISPLAY CANNOT DO is answered `HERO_UI_UNSUPPORTED` with
 * words naming what is missing, kept by `hero_ui_gtk_said_so` and handed to
 * the entry point by `hero_ui_arm_windows_words`, which empties them as it
 * reads them, so no call reads another's. On Wayland no program places, reads
 * or raises its own window or reads whether it is minimized (`xdg_toplevel`
 * has no such request, and of `set_minimized` it says "the client cannot
 * query or undo this"), and GTK 4 makes no key event. On X11 a position is
 * the X server's own, read and set through the window's XID; a window's
 * state and its stacking are a window manager's, so with none running (no
 * `_NET_SUPPORTING_WM_CHECK` on the root window) they are refused; a key is
 * pressed through the XTest extension, `libXtst.so.6`, opened beside GTK
 * where it is installed, GTK itself needing none. No system of GTK 4 puts a
 * translucent material behind a window.
 * ========================================================================== */

/* The words of this part's last refusal, which `hero_ui_arm_windows_words`
 * hands on once. */
static _Thread_local char hero_ui_gtk_said[HERO_UI_WORDS_MOST];
/* What the last read of the clipboard found, and whether it found any. */
static _Thread_local uint8_t *hero_ui_gtk_clip;
static _Thread_local int64_t hero_ui_gtk_clip_count;
static _Thread_local bool hero_ui_gtk_clip_kept;
/* The app's icon, a `GdkTexture`, NULL for the system's own. */
static _Thread_local void *hero_ui_gtk_icon;
/* Whether the X11 display's events are watched, and whether a check of the
 * windows' places is queued. */
static _Thread_local bool hero_ui_gtk_xevents;
static _Thread_local bool hero_ui_gtk_places_due;
/* Whether the app's windows are kept out of the task bar (`set_docked`). */
static _Thread_local bool hero_ui_gtk_undocked;

/* `kind`, its words `words` kept for the entry point. */
static int64_t hero_ui_gtk_said_so(int64_t kind, const char *words) {
    (void)hero_ui_copy_text(hero_ui_gtk_said, sizeof hero_ui_gtk_said, words);
    return kind;
}

int64_t hero_ui_arm_windows_words(char *words, int64_t count) {
    int64_t given;

    if (words == NULL || count <= 0 || hero_ui_gtk_said[0] == '\0') {
        return 0;
    }
    given = (int64_t)hero_ui_copy_text(words, (size_t)count, hero_ui_gtk_said);
    hero_ui_gtk_said[0] = '\0';
    return given;
}

/* Runs GLib's loop for up to `ms` milliseconds, or until `done` reads true
 * (NULL for the whole time); true where `done` came true. */
static bool hero_ui_gtk_spin(int64_t ms, bool (*done)(void *data), void *data) {
    int64_t deadline = hero_ui_gtk_now() + ms;

    for (;;) {
        int64_t now;
        bool woke = false;
        unsigned timer;

        while (hero_ui_gtk.main_context_iteration(NULL, 0)) {
            if (done != NULL && done(data)) {
                return true;
            }
        }
        if (done != NULL && done(data)) {
            return true;
        }
        now = hero_ui_gtk_now();
        if (now >= deadline) {
            return false;
        }
        /* Waking at least every 20 ms, since what `done` reads may change
         * with no event of GLib's (an X server's answer read by Xlib). */
        timer = hero_ui_gtk.timeout_add((unsigned)(deadline - now < 20 ? deadline - now : 20), hero_ui_gtk_woke, &woke);
        (void)hero_ui_gtk.main_context_iteration(NULL, 1);
        if (!woke) {
            (void)hero_ui_gtk.source_remove(timer);
        }
    }
}

/* -- BOXES THE SYSTEM DRAWS: PAGES, A SCROLLED BOX, TWO PANES ---------------
 *
 * Tabs are a `GtkNotebook`, a page a `GtkFixed` it holds, its tab a label; a
 * scrolled box a `GtkScrolledWindow` over a `GtkFixed` the program sizes; a
 * split a `GtkPaned`, a pane a `GtkFixed` it holds first or second. A page's,
 * a scrolled box's and a pane's widgets stand in its fixed as a window's
 * stand in its content (`hero_ui_gtk_container`). The person choosing a tab
 * is the notebook's `page` changing; the person dragging the divider is the
 * paned's `position` changing while its own size along its axis does not,
 * which a resize of its place changes; the divider is kept within the range
 * the program set by putting it back at the nearest end. */

/* The pane of split `split` hung first, `slot` 0, or second, else NULL;
 * `apart` is never answered. */
static hero_ui_native *hero_ui_gtk_pane(const hero_ui_native *split, int slot, const hero_ui_native *apart) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *each = hero_ui_at(at);

        if (each != NULL && each != apart && each->kind == HERO_UI_PANE && each->parent == split->id && each->pane_slot == slot) {
            return each;
        }
    }
    return NULL;
}

/* The person chose another tab of notebook `notebook`. */
static void hero_ui_gtk_page_shown(void *notebook, void *spec, void *data) {
    hero_ui_native *tabs = hero_ui_gtk_of(data, notebook);

    (void)spec;
    if (tabs != NULL && tabs->kind == HERO_UI_TABS) {
        hero_ui_tell(HERO_UI_EVENT_SELECT, tabs->id, hero_ui_gtk.notebook_get_current_page(notebook), 0);
    }
}

/* The divider of paned `paned` moved: by the person's drag, kept within the
 * program's range and told; by a resize of the split's own place, not told;
 * by the program, muted. */
static void hero_ui_gtk_divided(void *paned, void *spec, void *data) {
    hero_ui_native *split = hero_ui_gtk_of(data, paned);
    int extent;
    int64_t at;
    int64_t held;

    (void)spec;
    if (split == NULL || (split->kind != HERO_UI_SPLIT_ROW && split->kind != HERO_UI_SPLIT_COLUMN) || hero_ui_is_muted()) {
        return;
    }
    extent = split->kind == HERO_UI_SPLIT_ROW ? hero_ui_gtk.widget_get_width(paned) : hero_ui_gtk.widget_get_height(paned);
    at = hero_ui_gtk.paned_get_position(paned);
    if (extent != split->split_extent && hero_ui_gtk_injecting == 0) {
        split->split_extent = extent;
        split->value = at;
        return;
    }
    held = hero_ui_between(at, split->least, split->most);
    if (held != at) {
        hero_ui_mute(1);
        hero_ui_gtk.paned_set_position(paned, hero_ui_gtk_int(held));
        hero_ui_mute(-1);
    }
    if (held != split->value || hero_ui_gtk_injecting > 0) {
        split->value = held;
        hero_ui_tell(HERO_UI_EVENT_VALUE, split->id, held, 0);
    }
}

static bool hero_ui_gtk_make_box(hero_ui_native *made, hero_ui_native *owner) {
    (void)owner;
    switch (made->kind) {
    case HERO_UI_TABS:
        made->widget = hero_ui_gtk.notebook_new();
        if (made->widget == NULL) {
            return false;
        }
        hero_ui_gtk.notebook_set_scrollable(made->widget, 1);
        hero_ui_gtk_connect(made->widget, "notify::page", (hero_ui_gtk_callback)hero_ui_gtk_page_shown, made->id);
        return true;
    case HERO_UI_PAGE:
    case HERO_UI_PANE:
        made->widget = hero_ui_gtk.fixed_new();
        made->fixed = made->widget;
        if (made->widget != NULL) {
            /* What stands past a page's or a pane's edge is cut there, as a
             * window's content is. */
            hero_ui_gtk.widget_set_overflow(made->widget, HERO_GTK_OVERFLOW_HIDDEN);
        }
        return made->widget != NULL;
    case HERO_UI_SCROLL:
        made->widget = hero_ui_gtk.scrolled_window_new();
        made->fixed = hero_ui_gtk.fixed_new();
        if (made->widget == NULL || made->fixed == NULL) {
            return false;
        }
        hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_NEVER, HERO_GTK_POLICY_AUTOMATIC);
        hero_ui_gtk.scrolled_window_set_child(made->widget, made->fixed);
        return true;
    default:
        made->widget = hero_ui_gtk.paned_new(made->kind == HERO_UI_SPLIT_ROW ? HERO_GTK_ORIENTATION_HORIZONTAL : HERO_GTK_ORIENTATION_VERTICAL);
        if (made->widget == NULL) {
            return false;
        }
        /* The program's range bounds the divider, never GTK's least sizes;
         * the first pane keeps its size as the split's place grows. */
        hero_ui_gtk.paned_set_shrink_start_child(made->widget, 1);
        hero_ui_gtk.paned_set_shrink_end_child(made->widget, 1);
        hero_ui_gtk.paned_set_resize_start_child(made->widget, 0);
        hero_ui_gtk.paned_set_resize_end_child(made->widget, 1);
        made->least = 0;
        made->most = INT32_MAX;
        made->value = 0;
        hero_ui_gtk_connect(made->widget, "notify::position", (hero_ui_gtk_callback)hero_ui_gtk_divided, made->id);
        return true;
    }
}

static int64_t hero_ui_gtk_hang(hero_ui_native *made, hero_ui_native *owner) {
    if (made->kind == HERO_UI_PAGE) {
        int at;

        hero_ui_mute(1);
        at = hero_ui_gtk.notebook_append_page(owner->widget, made->widget, NULL);
        hero_ui_mute(-1);
        if (at < 0) {
            hero_ui_gtk_drop(made);
            return hero_ui_refused(HERO_UI_OTHER, "GTK's notebook took no page");
        }
        return HERO_UI_OK;
    }
    made->pane_slot = hero_ui_gtk_pane(owner, 0, made) != NULL ? 1 : 0;
    hero_ui_mute(1);
    if (made->pane_slot == 0) {
        hero_ui_gtk.paned_set_start_child(owner->widget, made->widget);
    } else {
        hero_ui_gtk.paned_set_end_child(owner->widget, made->widget);
    }
    hero_ui_mute(-1);
    owner->panes += 1;
    return HERO_UI_OK;
}

static void hero_ui_gtk_unhang(hero_ui_native *widget, hero_ui_native *owner) {
    if (owner == NULL) {
        return;
    }
    hero_ui_mute(1);
    if (widget->kind == HERO_UI_PAGE) {
        int at = hero_ui_gtk.notebook_page_num(owner->widget, widget->widget);

        if (at >= 0) {
            hero_ui_gtk.notebook_remove_page(owner->widget, at);
        }
    } else {
        if (widget->pane_slot == 0) {
            hero_ui_gtk.paned_set_start_child(owner->widget, NULL);
        } else {
            hero_ui_gtk.paned_set_end_child(owner->widget, NULL);
        }
        owner->panes -= 1;
    }
    hero_ui_mute(-1);
}

/* The thickness of split `split`'s divider: the paned's natural size along
 * its axis less its two panes', a pane it does not hold yet stood in for by
 * an empty fixed while it is measured. */
static int hero_ui_gtk_divider(hero_ui_native *split) {
    bool row = split->kind == HERO_UI_SPLIT_ROW;
    hero_ui_native *first = hero_ui_gtk_pane(split, 0, NULL);
    hero_ui_native *second = hero_ui_gtk_pane(split, 1, NULL);
    void *start = first != NULL ? first->widget : hero_ui_gtk.fixed_new();
    void *end = second != NULL ? second->widget : hero_ui_gtk.fixed_new();
    hero_ui_gtk_size whole;
    hero_ui_gtk_size one;
    hero_ui_gtk_size two;
    int thick;

    hero_ui_mute(1);
    if (first == NULL) {
        hero_ui_gtk.paned_set_start_child(split->widget, start);
    }
    if (second == NULL) {
        hero_ui_gtk.paned_set_end_child(split->widget, end);
    }
    whole = hero_ui_gtk_natural(split->widget);
    one = hero_ui_gtk_natural(start);
    two = hero_ui_gtk_natural(end);
    if (first == NULL) {
        hero_ui_gtk.paned_set_start_child(split->widget, NULL);
    }
    if (second == NULL) {
        hero_ui_gtk.paned_set_end_child(split->widget, NULL);
    }
    hero_ui_mute(-1);
    thick = row ? whole.width - one.width - two.width : whole.height - one.height - two.height;
    return thick > 0 ? thick : 1;
}

int64_t hero_ui_arm_insets(int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    *left = 0;
    *top = 0;
    *right = 0;
    *bottom = 0;
    if (widget->kind == HERO_UI_SCROLL) {
        int overlay = 1;
        void *settings = hero_ui_gtk.settings_get_default();

        if (settings != NULL) {
            hero_ui_gtk.object_get(settings, "gtk-overlay-scrolling", &overlay, NULL);
        }
        if (!overlay || !hero_ui_gtk.scrolled_window_get_overlay_scrolling(widget->widget)) {
            /* The scroll bar stands beside the content rather than over it. */
            *right = hero_ui_gtk_natural(hero_ui_gtk.scrolled_window_get_vscrollbar(widget->widget)).width;
        }
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_TABS) {
        /* Measured around a page of 400 by 300 points put in for the
         * moment: the room left beside and below it is the border, the room
         * above it the tabs and the border. */
        void *probe = hero_ui_gtk.fixed_new();
        hero_ui_gtk_size whole;
        int at;
        int64_t wide;
        int64_t high;

        if (probe == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.widget_set_size_request(probe, 400, 300);
        hero_ui_mute(1);
        at = hero_ui_gtk.notebook_append_page(widget->widget, probe, NULL);
        whole = hero_ui_gtk_natural(widget->widget);
        if (at >= 0) {
            hero_ui_gtk.notebook_remove_page(widget->widget, at);
        }
        hero_ui_mute(-1);
        wide = whole.width > 400 ? whole.width - 400 : 0;
        high = whole.height > 300 ? whole.height - 300 : 0;
        *left = wide / 2;
        *right = wide - wide / 2;
        *bottom = *left < high ? *left : 0;
        *top = high - *bottom;
        return HERO_UI_OK;
    }
    return HERO_UI_INVALID;
}

/* -- A LIST'S ROW OPENED, RETURN AND ESCAPE, THE WIDGET WITH THE KEYS ---------
 *
 * A list opens a row on a double click and on Return, its `row-activated`
 * (single-click activation is off). A text field's `activate` is Return in
 * it: its window's button for Return is clicked where the person reaches it,
 * else the field is submitted; the button is also the window's default
 * widget, so Return anywhere else in the window clicks it, as GTK draws and
 * does for a dialog's. Escape is a shortcut of the window's, held by a
 * controller on its button in the window's global scope, activating it. */

static void hero_ui_gtk_row_opened(void *box, void *row, void *data) {
    hero_ui_native *list = hero_ui_gtk_of(data, box);

    if (list != NULL && (list->kind == HERO_UI_LIST || list->kind == HERO_UI_LIST_MULTIPLE)) {
        hero_ui_tell(HERO_UI_EVENT_ACTIVATE, list->id, hero_ui_gtk.list_box_row_get_index(row), 0);
    }
}

/* Button `id` where the person reaches it: shown and taking their acts. */
static hero_ui_native *hero_ui_gtk_reachable_button(int64_t id) {
    hero_ui_native *button = id != 0 ? hero_ui_find(id) : NULL;

    if (button == NULL || button->kind != HERO_UI_BUTTON || !hero_ui_gtk.widget_get_visible(button->widget) || !hero_ui_gtk.widget_is_sensitive(button->widget)) {
        return NULL;
    }
    return button;
}

static void hero_ui_gtk_returned(void *entry, void *data) {
    hero_ui_native *field = hero_ui_gtk_of(data, entry);
    hero_ui_native *window;
    hero_ui_native *button;

    if (field == NULL || field->kind != HERO_UI_TEXT_FIELD) {
        return;
    }
    window = hero_ui_gtk_window(field->window);
    button = window != NULL ? hero_ui_gtk_reachable_button(window->enter_button) : NULL;
    if (button != NULL) {
        (void)hero_ui_gtk.widget_activate(button->widget);
        return;
    }
    hero_ui_tell(HERO_UI_EVENT_SUBMIT, field->id, 0, 0);
}

static void hero_ui_gtk_forget_keys(hero_ui_native *button) {
    hero_ui_native *window = hero_ui_gtk_window(button->window);

    if (window != NULL && window->enter_button == button->id) {
        window->enter_button = 0;
        hero_ui_gtk.window_set_default_widget(window->widget, NULL);
    }
    if (window != NULL && window->escape_button == button->id) {
        window->escape_button = 0;
    }
    if (button->key_controller != NULL) {
        hero_ui_gtk.widget_remove_controller(button->widget, button->key_controller);
        button->key_controller = NULL;
    }
    button->button_key = HERO_UI_BUTTON_KEY_NONE;
}

int64_t hero_ui_arm_set_button_key(int64_t id, int64_t key) {
    hero_ui_native *button = hero_ui_find(id);
    hero_ui_native *window;

    if (button == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (button->kind != HERO_UI_BUTTON) {
        return HERO_UI_INVALID;
    }
    window = hero_ui_gtk_window(button->window);
    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_gtk_forget_keys(button);
    if (key == HERO_UI_BUTTON_KEY_ENTER) {
        hero_ui_native *before = hero_ui_find(window->enter_button);

        if (before != NULL) {
            hero_ui_gtk_forget_keys(before);
        }
        window->enter_button = id;
        hero_ui_gtk.window_set_default_widget(window->widget, button->widget);
    } else if (key == HERO_UI_BUTTON_KEY_ESCAPE) {
        hero_ui_native *before = hero_ui_find(window->escape_button);
        void *controller = hero_ui_gtk.shortcut_controller_new();
        void *trigger = hero_ui_gtk.keyval_trigger_new(HERO_GDK_KEY_ESCAPE, 0);
        void *shortcut;

        if (before != NULL) {
            hero_ui_gtk_forget_keys(before);
        }
        if (controller == NULL || trigger == NULL) {
            return HERO_UI_LIMIT;
        }
        shortcut = hero_ui_gtk.shortcut_new(trigger, hero_ui_gtk.object_ref(hero_ui_gtk.activate_action_get()));
        if (shortcut == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.shortcut_controller_set_scope(controller, HERO_GTK_SHORTCUT_SCOPE_GLOBAL);
        hero_ui_gtk.shortcut_controller_add_shortcut(controller, shortcut);
        hero_ui_gtk.widget_add_controller(button->widget, controller);
        button->key_controller = controller;
        window->escape_button = id;
    }
    button->button_key = key;
    return HERO_UI_OK;
}

static bool hero_ui_gtk_takes_keys(int64_t kind) {
    return kind == HERO_UI_TEXT_FIELD || kind == HERO_UI_TEXT_AREA || kind == HERO_UI_LIST || kind == HERO_UI_LIST_MULTIPLE;
}

/* The GTK widget given the keys for `widget`: the field, the text view, or
 * a list's first chosen row, else its first row, else the list box. */
static void *hero_ui_gtk_key_target(const hero_ui_native *widget) {
    if (widget->kind == HERO_UI_TEXT_FIELD) {
        return widget->widget;
    }
    if (widget->kind == HERO_UI_TEXT_AREA) {
        return widget->inner;
    }
    for (int64_t i = 0; i < widget->items; i += 1) {
        void *row = hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)i);

        if (row != NULL && hero_ui_gtk.list_box_row_is_selected(row)) {
            return row;
        }
    }
    {
        void *first = hero_ui_gtk.list_box_get_row_at_index(widget->inner, 0);

        return first != NULL ? first : widget->inner;
    }
}

int64_t hero_ui_arm_set_focus(int64_t id) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!hero_ui_gtk_takes_keys(widget->kind) || !widget->visible || !hero_ui_gtk.widget_get_visible(widget->widget) || !hero_ui_gtk.widget_is_sensitive(widget->widget)) {
        return HERO_UI_INVALID;
    }
    return hero_ui_gtk.widget_grab_focus(hero_ui_gtk_key_target(widget)) ? HERO_UI_OK : HERO_UI_INVALID;
}

/* The widget of window `window` that takes the keys and holds GTK widget
 * `gtk`, or NULL. */
static hero_ui_native *hero_ui_gtk_keyed(const hero_ui_native *window, const void *gtk) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *each = hero_ui_at(at);

        if (each != NULL && each->window == window->id && hero_ui_gtk_takes_keys(each->kind) && (each->widget == gtk || each->inner == gtk)) {
            return each;
        }
    }
    return NULL;
}

/* The widget of window `window` with the keys, or NULL where none of its
 * widgets that take them has them. */
static hero_ui_native *hero_ui_gtk_focused(const hero_ui_native *window) {
    for (void *at = hero_ui_gtk.root_get_focus(window->widget); at != NULL && at != window->widget; at = hero_ui_gtk.widget_get_parent(at)) {
        hero_ui_native *found = hero_ui_gtk_keyed(window, at);

        if (found != NULL) {
            return found;
        }
    }
    return NULL;
}

int64_t hero_ui_arm_focus(int64_t id, int64_t *focus) {
    hero_ui_native *window = hero_ui_find(id);
    hero_ui_native *found;

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (window->kind != HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    found = hero_ui_gtk_focused(window);
    *focus = found != NULL ? found->id : id;
    return HERO_UI_OK;
}

/* Gives the first widget of window `window` that takes the keys, in reading
 * order, the keys, where none of them has them: what a window shown the
 * first time does on every system (GTK's own first focus may be a button). */
static void hero_ui_gtk_first_focus(hero_ui_native *window) {
    hero_ui_native *best = NULL;
    double best_x = 0;
    double best_y = 0;

    if (hero_ui_gtk_focused(window) != NULL) {
        return;
    }
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *each = hero_ui_at(at);
        float origin[2] = {0.0f, 0.0f};
        float found[2] = {0.0f, 0.0f};

        if (each == NULL || each->window != window->id || !hero_ui_gtk_takes_keys(each->kind) || !hero_ui_gtk.widget_get_visible(each->widget) || !hero_ui_gtk.widget_is_sensitive(each->widget)) {
            continue;
        }
        if (!hero_ui_gtk.widget_compute_point(each->widget, window->widget, origin, found)) {
            found[0] = (float)each->x;
            found[1] = (float)each->y;
        }
        if (best == NULL || found[1] < best_y || (found[1] == best_y && found[0] < best_x)) {
            best = each;
            best_x = found[0];
            best_y = found[1];
        }
    }
    if (best != NULL) {
        (void)hero_ui_gtk.widget_grab_focus(hero_ui_gtk_key_target(best));
    }
}

/* -- MENUS INSIDE MENUS, CHECK AND RADIO ITEMS, ROLES, CONTEXT MENUS ---------
 *
 * A menu inside a menu is a submenu of the GMenu it stands in. A check item's
 * action has a boolean state, which GTK draws as a check; a radio item's
 * action a string state and the item a target, `on`, which GTK draws as a
 * radio checked where the two match; the arm turns them itself before it
 * tells the choice (`hero_ui_gtk_chosen`). GTK 4 holds no application menu,
 * so a role leaves an item where the program put it. A context menu is a
 * `GtkPopoverMenu` over the widget's GMenu, opened by a secondary click or
 * by Shift+F10 and the Menu key on its widget; a text field's and a text
 * area's are their own context menu's extra items, beside Cut, Copy and
 * Paste, as GTK gives a text widget's. */

int64_t hero_ui_arm_set_role(int64_t id, int64_t role) {
    hero_ui_native *item = hero_ui_find(id);

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM) {
        return HERO_UI_INVALID;
    }
    item->role = role;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_check(int64_t id, int64_t mode) {
    hero_ui_native *item = hero_ui_find(id);
    hero_ui_native *window;
    char name[64];
    void *action;

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM) {
        return HERO_UI_INVALID;
    }
    window = hero_ui_gtk_window(item->window);
    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_gtk_action_name(id, false, name, sizeof name);
    if (mode == HERO_UI_CHECK_BOX) {
        action = hero_ui_gtk.simple_action_new_stateful(name, NULL, hero_ui_gtk.variant_new_boolean(0));
    } else if (mode == HERO_UI_CHECK_RADIO) {
        void *type = hero_ui_gtk.variant_type_new("s");

        action = type != NULL ? hero_ui_gtk.simple_action_new_stateful(name, type, hero_ui_gtk.variant_new_string(HERO_UI_GTK_RADIO_OFF)) : NULL;
        if (type != NULL) {
            hero_ui_gtk.variant_type_free(type);
        }
    } else {
        action = hero_ui_gtk.simple_action_new(name, NULL);
    }
    if (action == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_gtk.action_map_remove_action(window->actions, name);
    hero_ui_gtk.object_unref(item->action);
    item->action = action;
    item->check = mode;
    item->checked = false;
    hero_ui_gtk_connect(action, "activate", (hero_ui_gtk_callback)hero_ui_gtk_chosen, id);
    hero_ui_gtk.action_map_add_action(window->actions, action);
    if (item->shortcut != NULL) {
        hero_ui_gtk.shortcut_set_arguments(item->shortcut, mode == HERO_UI_CHECK_RADIO ? hero_ui_gtk.variant_new_string(HERO_UI_GTK_RADIO_ON) : NULL);
    }
    return hero_ui_gtk_fill_menu(hero_ui_find(item->parent));
}

/* The person's secondary click on context menu `data`'s widget. */
static void hero_ui_gtk_context_pressed(void *gesture, int presses, double x, double y, void *data) {
    hero_ui_native *menu = hero_ui_gtk_of(data, gesture);
    hero_ui_native *owner = menu != NULL ? hero_ui_find(menu->parent) : NULL;
    int point[4];

    (void)presses;
    if (menu == NULL || owner == NULL || !owner->enabled || !menu->visible) {
        return;
    }
    (void)hero_ui_gtk.gesture_set_state(gesture, HERO_GTK_EVENT_SEQUENCE_CLAIMED);
    point[0] = (int)x;
    point[1] = (int)y;
    point[2] = 1;
    point[3] = 1;
    hero_ui_gtk.popover_set_pointing_to(menu->popover, point);
    hero_ui_gtk.popover_popup(menu->popover);
}

/* Shift+F10 or the Menu key on context menu `data`'s widget: the menu shown
 * at the widget's corner. */
static int hero_ui_gtk_context_keyed(void *widget, void *arguments, void *data) {
    hero_ui_native *menu = hero_ui_find((int64_t)(intptr_t)data);
    hero_ui_native *owner = menu != NULL && menu->kind == HERO_UI_CONTEXT_MENU ? hero_ui_find(menu->parent) : NULL;
    int point[4] = {0, 0, 1, 1};

    (void)widget;
    (void)arguments;
    if (owner == NULL || !owner->enabled || !menu->visible) {
        return 0;
    }
    hero_ui_gtk.popover_set_pointing_to(menu->popover, point);
    hero_ui_gtk.popover_popup(menu->popover);
    return 1;
}

/* Sets text field `field`'s, or text area `area`'s, extra menu `model`. */
static void hero_ui_gtk_extra_menu(hero_ui_native *owner, void *model) {
    if (owner->kind == HERO_UI_TEXT_AREA) {
        hero_ui_gtk.text_view_set_extra_menu(owner->inner, model);
    } else if (owner->masked) {
        hero_ui_gtk.password_entry_set_extra_menu(owner->widget, model);
    } else {
        hero_ui_gtk.entry_set_extra_menu(owner->widget, model);
    }
}

static int64_t hero_ui_gtk_attach_context(hero_ui_native *menu, hero_ui_native *owner) {
    static const struct {
        unsigned keyval;
        unsigned held;
    } opening[] = {{HERO_GDK_KEY_F10, HERO_GDK_SHIFT_MASK}, {HERO_GDK_KEY_MENU, 0u}};
    void *target;

    owner->context = menu->id;
    if (owner->kind == HERO_UI_TEXT_FIELD || owner->kind == HERO_UI_TEXT_AREA) {
        hero_ui_gtk_extra_menu(owner, menu->menu_model);
        return hero_ui_gtk_fill_menu(menu);
    }
    target = owner->kind == HERO_UI_WINDOW ? hero_ui_gtk.widget_get_parent(owner->area) : owner->widget;
    menu->inner = target;
    menu->popover = hero_ui_gtk.popover_menu_new_from_model(menu->menu_model);
    menu->gesture = hero_ui_gtk.gesture_click_new();
    menu->keys = hero_ui_gtk.shortcut_controller_new();
    if (target == NULL || menu->popover == NULL || menu->gesture == NULL || menu->keys == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_gtk.popover_set_has_arrow(menu->popover, 0);
    hero_ui_gtk.widget_set_halign(menu->popover, HERO_GTK_ALIGN_START);
    hero_ui_gtk.widget_set_parent(menu->popover, target);
    hero_ui_gtk.gesture_single_set_button(menu->gesture, HERO_GDK_BUTTON_SECONDARY);
    hero_ui_gtk_connect(menu->gesture, "pressed", (hero_ui_gtk_callback)hero_ui_gtk_context_pressed, menu->id);
    hero_ui_gtk.widget_add_controller(target, menu->gesture);
    for (size_t i = 0; i < sizeof opening / sizeof opening[0]; i += 1) {
        void *trigger = hero_ui_gtk.keyval_trigger_new(opening[i].keyval, opening[i].held);
        void *action = hero_ui_gtk.callback_action_new((hero_ui_gtk_callback)hero_ui_gtk_context_keyed, (void *)(intptr_t)menu->id, NULL);
        void *shortcut = trigger != NULL && action != NULL ? hero_ui_gtk.shortcut_new(trigger, action) : NULL;

        if (shortcut != NULL) {
            hero_ui_gtk.shortcut_controller_add_shortcut(menu->keys, shortcut);
        }
    }
    hero_ui_gtk.widget_add_controller(target, menu->keys);
    return hero_ui_gtk_fill_menu(menu);
}

static void hero_ui_gtk_detach_context(hero_ui_native *menu) {
    hero_ui_native *owner = hero_ui_find(menu->parent);

    if (owner != NULL && owner->context == menu->id) {
        owner->context = 0;
        if (owner->kind == HERO_UI_TEXT_FIELD || owner->kind == HERO_UI_TEXT_AREA) {
            hero_ui_gtk_extra_menu(owner, NULL);
        }
    }
    if (menu->popover != NULL) {
        hero_ui_gtk.widget_unparent(menu->popover);
        menu->popover = NULL;
    }
    if (menu->inner != NULL && menu->gesture != NULL) {
        hero_ui_gtk.widget_remove_controller(menu->inner, menu->gesture);
    }
    if (menu->inner != NULL && menu->keys != NULL) {
        hero_ui_gtk.widget_remove_controller(menu->inner, menu->keys);
    }
    menu->gesture = NULL;
    menu->keys = NULL;
}

static int64_t hero_ui_gtk_context_refill(hero_ui_native *owner) {
    hero_ui_native *menu = owner->context != 0 ? hero_ui_find(owner->context) : NULL;

    return menu != NULL ? hero_ui_gtk_fill_menu(menu) : HERO_UI_OK;
}

/* -- A MASKED FIELD, AND A PROGRESS BAR WITH NO VALUE ------------------------
 *
 * A masked field is a `GtkPasswordEntry` put in the text field's place, its
 * text, its place, its greying, its keys and its context menu carried over,
 * as macOS puts an `NSSecureTextField` there; GTK refuses to copy its text
 * and shows Caps Lock held. A moving bar is pulsed every 100 ms by a timeout
 * of its own, removed when it stops or goes. */

int64_t hero_ui_arm_set_masked(int64_t id, int64_t on) {
    hero_ui_native *field = hero_ui_find(id);
    hero_ui_native *window;
    void *container;
    void *old;
    void *fresh;
    const char *held;
    char *text;
    bool focused;

    if (field == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (field->kind != HERO_UI_TEXT_FIELD) {
        return HERO_UI_INVALID;
    }
    if (field->masked == (on == 1)) {
        return HERO_UI_OK;
    }
    window = hero_ui_gtk_window(field->window);
    container = hero_ui_gtk_container(field);
    old = field->widget;
    held = hero_ui_gtk.editable_get_text(old);
    text = hero_ui_gtk_text((const uint8_t *)(held != NULL ? held : ""), held != NULL ? (int64_t)strlen(held) : 0, false);
    fresh = on == 1 ? hero_ui_gtk.password_entry_new() : hero_ui_gtk.entry_new();
    if (text == NULL || fresh == NULL || container == NULL || window == NULL) {
        hero_layer_free(text);
        return HERO_UI_LIMIT;
    }
    focused = hero_ui_gtk_focused(window) == field;
    hero_ui_mute(1);
    hero_ui_gtk.editable_set_text(fresh, text);
    hero_ui_mute(-1);
    hero_layer_free(text);
    hero_ui_gtk.widget_set_sensitive(fresh, field->enabled ? 1 : 0);
    hero_ui_gtk.widget_set_visible(fresh, 0);
    hero_ui_gtk.fixed_put(container, fresh, (double)hero_ui_gtk_int(field->x), (double)hero_ui_gtk_int(field->y));
    if (field->placed) {
        hero_ui_gtk.widget_set_size_request(fresh, hero_ui_gtk_int(field->width), hero_ui_gtk_int(field->height));
    }
    hero_ui_gtk_connect(fresh, "changed", (hero_ui_gtk_callback)hero_ui_gtk_edited, id);
    hero_ui_gtk_connect(fresh, "activate", (hero_ui_gtk_callback)hero_ui_gtk_returned, id);
    field->widget = fresh;
    field->masked = on == 1;
    if (field->context != 0) {
        hero_ui_native *menu = hero_ui_find(field->context);

        if (menu != NULL) {
            hero_ui_gtk_extra_menu(field, menu->menu_model);
        }
    }
    hero_ui_gtk_show_placed(field);
    hero_ui_gtk.fixed_remove(container, old);
    if (focused) {
        (void)hero_ui_gtk.widget_grab_focus(fresh);
    }
    return HERO_UI_OK;
}

/* Pulses moving bar `data` once more. */
static int hero_ui_gtk_pulsed(void *data) {
    hero_ui_native *bar = hero_ui_find((int64_t)(intptr_t)data);

    if (bar == NULL || bar->kind != HERO_UI_PROGRESS || bar->pulse == 0) {
        return 0;
    }
    hero_ui_gtk.progress_bar_pulse(bar->widget);
    return 1;
}

int64_t hero_ui_arm_set_moving(int64_t id, int64_t on) {
    hero_ui_native *bar = hero_ui_find(id);

    if (bar == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (bar->kind != HERO_UI_PROGRESS) {
        return HERO_UI_INVALID;
    }
    if (on == 1 && bar->pulse == 0) {
        hero_ui_gtk.progress_bar_set_pulse_step(bar->widget, 0.1);
        hero_ui_gtk.progress_bar_pulse(bar->widget);
        bar->pulse = hero_ui_gtk.timeout_add(100, hero_ui_gtk_pulsed, (void *)(intptr_t)id);
    } else if (on == 0 && bar->pulse != 0) {
        (void)hero_ui_gtk.source_remove(bar->pulse);
        bar->pulse = 0;
        hero_ui_gtk_show_range(bar);
    }
    return HERO_UI_OK;
}

/* -- THE CLIPBOARD -----------------------------------------------------------
 *
 * The display's `GdkClipboard`. What a write puts is offered as its own kind
 * alone, so a read of another kind finds nothing, as `hero_ui.h` says: text
 * as `text/plain;charset=utf-8`, its bytes exact; paths as `text/uri-list`
 * and as GNOME's `x-special/gnome-copied-files`, which file managers paste,
 * and never as text, which a `GdkFileList` value would also have offered; an
 * image as a `GdkMemoryTexture` of straight RGBA. A read is asynchronous: the
 * arm runs GLib's loop until its callback, which keeps what it found, ran, or
 * cancels it after five seconds where the program holding the clipboard does
 * not answer. A read that finds no format of the kind asked is NOT_FOUND. */

/* What one read of the clipboard found. */
typedef struct hero_ui_gtk_read {
    bool done;
    int64_t kind;
    int64_t answer;
    uint8_t *bytes;
    int64_t count;
    int64_t room;
    int64_t width;
    int64_t height;
    char words[HERO_UI_WORDS_MOST];
} hero_ui_gtk_read;

/* GLib's `GSList`. */
typedef struct hero_ui_gtk_slist {
    void *data;
    struct hero_ui_gtk_slist *next;
} hero_ui_gtk_slist;

/* The display's clipboard, or NULL. */
static void *hero_ui_gtk_clipboard(void) {
    void *display = hero_ui_gtk.display_get_default();

    return display != NULL ? hero_ui_gtk.display_get_clipboard(display) : NULL;
}

/* Appends `count` bytes at `bytes` to `read`'s, growing them; false where
 * no memory is left. */
static bool hero_ui_gtk_read_add(hero_ui_gtk_read *read, const void *bytes, int64_t count) {
    if (read->count + count > read->room) {
        int64_t room = (read->room + count) * 2 + 64;
        uint8_t *grown = hero_layer_grow(read->bytes, (size_t)room);

        if (grown == NULL) {
            return false;
        }
        read->bytes = grown;
        read->room = room;
    }
    if (count > 0) {
        memcpy(read->bytes + read->count, bytes, (size_t)count);
    }
    read->count += count;
    return true;
}

/* Keeps the error `error` of a read as its answer: an empty clipboard, and
 * one holding no format of the kind asked, NOT_FOUND (GIO's `NOT_FOUND` and
 * `NOT_SUPPORTED`, measured under Xvfb on GTK 4.18.6); any other, GTK's
 * words. */
static void hero_ui_gtk_read_failed(hero_ui_gtk_read *read, hero_ui_gerror *error) {
    if (error->domain == hero_ui_gtk.io_error_quark() && (error->code == HERO_G_IO_ERROR_NOT_FOUND || error->code == HERO_G_IO_ERROR_NOT_SUPPORTED)) {
        read->answer = HERO_UI_NOT_FOUND;
    } else {
        (void)snprintf(read->words, sizeof read->words, "GTK's clipboard failed: %s", error->message != NULL ? error->message : "no words");
        read->answer = HERO_UI_OTHER;
    }
    hero_ui_gtk.error_free(error);
}

/* Writes texture `texture`'s pixels into `read` as 8-bit RGBA, alpha
 * straight, rows from the top: GTK 4.10's downloader where the library has
 * it, else Cairo's premultiplied 32-bit pixels taken back to straight. */
static void hero_ui_gtk_read_texture(hero_ui_gtk_read *read, void *texture) {
    int64_t width = hero_ui_gtk.texture_get_width(texture);
    int64_t height = hero_ui_gtk.texture_get_height(texture);
    int64_t count = width * height * 4;

    if (width <= 0 || height <= 0 || count > HERO_UI_CLIP_MOST) {
        (void)snprintf(read->words, sizeof read->words, "an image of %lld by %lld pixels is past the %d bytes a read takes", (long long)width, (long long)height, HERO_UI_CLIP_MOST);
        read->answer = HERO_UI_LIMIT;
        return;
    }
    read->bytes = hero_layer_alloc((size_t)count);
    if (read->bytes == NULL) {
        read->answer = HERO_UI_LIMIT;
        return;
    }
    if (hero_ui_gtk.texture_downloader_new != NULL && hero_ui_gtk.texture_downloader_set_format != NULL && hero_ui_gtk.texture_downloader_download_into != NULL && hero_ui_gtk.texture_downloader_free != NULL) {
        void *downloader = hero_ui_gtk.texture_downloader_new(texture);

        hero_ui_gtk.texture_downloader_set_format(downloader, HERO_GDK_MEMORY_R8G8B8A8);
        hero_ui_gtk.texture_downloader_download_into(downloader, read->bytes, (size_t)width * 4);
        hero_ui_gtk.texture_downloader_free(downloader);
    } else {
        hero_ui_gtk.texture_download(texture, read->bytes, (size_t)width * 4);
        for (int64_t at = 0; at < count; at += 4) {
            uint32_t pixel;
            uint32_t alpha;
            uint32_t channel[3];

            memcpy(&pixel, read->bytes + at, sizeof pixel);
            alpha = pixel >> 24;
            channel[0] = (pixel >> 16) & 0xFFu;
            channel[1] = (pixel >> 8) & 0xFFu;
            channel[2] = pixel & 0xFFu;
            for (int c = 0; c < 3; c += 1) {
                uint32_t straight = alpha == 0 ? 0 : (channel[c] * 255u + alpha / 2u) / alpha;

                read->bytes[at + c] = (uint8_t)(straight > 255u ? 255u : straight);
            }
            read->bytes[at + 3] = (uint8_t)alpha;
        }
    }
    read->count = count;
    read->room = count;
    read->width = width;
    read->height = height;
}

/* A read's callback: what was found kept in the read, before GLib frees the
 * result that holds it. */
static void hero_ui_gtk_clip_answered(void *clipboard, void *result, void *data) {
    hero_ui_gtk_read *read = data;
    hero_ui_gerror *error = NULL;

    read->answer = HERO_UI_OK;
    if (read->kind == HERO_UI_CLIP_TEXT) {
        char *text = hero_ui_gtk.clipboard_read_text_finish(clipboard, result, &error);

        if (text != NULL) {
            if (!hero_ui_gtk_read_add(read, text, (int64_t)strlen(text))) {
                read->answer = HERO_UI_LIMIT;
            }
            hero_ui_gtk.g_free(text);
        }
    } else if (read->kind == HERO_UI_CLIP_FILES) {
        const void *value = hero_ui_gtk.clipboard_read_value_finish(clipboard, result, &error);

        if (value != NULL) {
            void *list = hero_ui_gtk.value_get_boxed(value);
            hero_ui_gtk_slist *files = list != NULL ? hero_ui_gtk.file_list_get_files(list) : NULL;

            for (hero_ui_gtk_slist *each = files; each != NULL && read->answer == HERO_UI_OK; each = each->next) {
                char *path = hero_ui_gtk.file_get_path(each->data);
                size_t bytes = path != NULL ? strlen(path) : 0;

                /* A file with no local path, or none of UTF-8, is passed over,
                 * as the file dialogs pass it over. */
                if (bytes > 0 && hero_ui_is_utf8((const uint8_t *)path, bytes) && (!hero_ui_gtk_read_add(read, path, (int64_t)bytes) || !hero_ui_gtk_read_add(read, "", 1))) {
                    read->answer = HERO_UI_LIMIT;
                }
                hero_ui_gtk.g_free(path);
            }
            hero_ui_gtk.slist_free(files);
            if (read->answer == HERO_UI_OK && read->count == 0) {
                read->answer = HERO_UI_NOT_FOUND;
            }
        }
    } else {
        void *texture = hero_ui_gtk.clipboard_read_texture_finish(clipboard, result, &error);

        if (texture != NULL) {
            hero_ui_gtk_read_texture(read, texture);
            hero_ui_gtk.object_unref(texture);
        }
    }
    if (error != NULL) {
        hero_ui_gtk_read_failed(read, error);
    }
    if (read->answer == HERO_UI_OK && read->count > HERO_UI_CLIP_MOST) {
        read->answer = HERO_UI_LIMIT;
    }
    read->done = true;
}

/* Whether read `data` is done. */
static bool hero_ui_gtk_read_done(void *data) {
    return ((hero_ui_gtk_read *)data)->done;
}

int64_t hero_ui_arm_clipboard_set(int64_t kind, const uint8_t *bytes, int64_t count, int64_t width, int64_t height) {
    void *clipboard = hero_ui_gtk_clipboard();
    bool ok = true;

    if (clipboard == NULL) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "GTK holds no display, so no clipboard");
    }
    if (kind == HERO_UI_CLIP_NOTHING) {
        ok = hero_ui_gtk.clipboard_set_content(clipboard, NULL) != 0;
    } else if (kind == HERO_UI_CLIP_TEXT) {
        void *held = hero_ui_gtk.bytes_new(bytes, (size_t)count);
        void *provider = held != NULL ? hero_ui_gtk.content_provider_new_for_bytes("text/plain;charset=utf-8", held) : NULL;

        if (held != NULL) {
            hero_ui_gtk.bytes_unref(held);
        }
        if (provider == NULL) {
            return HERO_UI_LIMIT;
        }
        ok = hero_ui_gtk.clipboard_set_content(clipboard, provider) != 0;
        hero_ui_gtk.object_unref(provider);
    } else if (kind == HERO_UI_CLIP_FILES) {
        hero_ui_gtk_read uris = {0};
        hero_ui_gtk_read copied = {0};
        void *providers[2] = {NULL, NULL};
        void *both = NULL;

        ok = hero_ui_gtk_read_add(&copied, "copy", 4);
        for (int64_t at = 0; ok && at < count;) {
            const uint8_t *end = memchr(bytes + at, 0, (size_t)(count - at));
            hero_ui_gerror *error = NULL;
            char *uri = hero_ui_gtk.filename_to_uri((const char *)(bytes + at), NULL, &error);

            if (uri == NULL) {
                if (error != NULL) {
                    hero_ui_gtk.error_free(error);
                }
                hero_layer_free(uris.bytes);
                hero_layer_free(copied.bytes);
                return HERO_UI_INVALID;
            }
            ok = hero_ui_gtk_read_add(&uris, uri, (int64_t)strlen(uri)) && hero_ui_gtk_read_add(&uris, "\r\n", 2) && hero_ui_gtk_read_add(&copied, "\n", 1) && hero_ui_gtk_read_add(&copied, uri, (int64_t)strlen(uri));
            hero_ui_gtk.g_free(uri);
            at = (int64_t)(end - bytes) + 1;
        }
        if (ok) {
            void *listed = hero_ui_gtk.bytes_new(uris.bytes, (size_t)uris.count);
            void *gnome = hero_ui_gtk.bytes_new(copied.bytes, (size_t)copied.count);

            providers[0] = listed != NULL ? hero_ui_gtk.content_provider_new_for_bytes("text/uri-list", listed) : NULL;
            providers[1] = gnome != NULL ? hero_ui_gtk.content_provider_new_for_bytes("x-special/gnome-copied-files", gnome) : NULL;
            if (listed != NULL) {
                hero_ui_gtk.bytes_unref(listed);
            }
            if (gnome != NULL) {
                hero_ui_gtk.bytes_unref(gnome);
            }
            both = providers[0] != NULL && providers[1] != NULL ? hero_ui_gtk.content_provider_new_union(providers, 2) : NULL;
        }
        hero_layer_free(uris.bytes);
        hero_layer_free(copied.bytes);
        if (both == NULL) {
            return HERO_UI_LIMIT;
        }
        ok = hero_ui_gtk.clipboard_set_content(clipboard, both) != 0;
        hero_ui_gtk.object_unref(both);
    } else {
        void *held = hero_ui_gtk.bytes_new(bytes, (size_t)count);
        void *texture = held != NULL && width <= INT32_MAX / 4 && height <= INT32_MAX ? hero_ui_gtk.memory_texture_new((int)width, (int)height, HERO_GDK_MEMORY_R8G8B8A8, held, (size_t)width * 4) : NULL;

        if (held != NULL) {
            hero_ui_gtk.bytes_unref(held);
        }
        if (texture == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.clipboard_set_texture(clipboard, texture);
        hero_ui_gtk.object_unref(texture);
    }
    return ok ? HERO_UI_OK : hero_ui_gtk_said_so(HERO_UI_OTHER, "GTK's clipboard refused what was put on it");
}

int64_t hero_ui_arm_clipboard_get(int64_t kind, int64_t *bytes, int64_t *width, int64_t *height) {
    void *clipboard = hero_ui_gtk_clipboard();
    void *cancel;
    hero_ui_gtk_read read = {0};

    *bytes = 0;
    if (clipboard == NULL) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "GTK holds no display, so no clipboard");
    }
    if (kind == HERO_UI_CLIP_FILES && hero_ui_gtk.file_list_get_files == NULL) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "GTK 4.6 reads a clipboard's paths, and this GTK lacks gdk_file_list_get_files");
    }
    cancel = hero_ui_gtk.cancellable_new();
    read.kind = kind;
    if (kind == HERO_UI_CLIP_TEXT) {
        hero_ui_gtk.clipboard_read_text_async(clipboard, cancel, hero_ui_gtk_clip_answered, &read);
    } else if (kind == HERO_UI_CLIP_FILES) {
        hero_ui_gtk.clipboard_read_value_async(clipboard, hero_ui_gtk.file_list_get_type(), 0, cancel, hero_ui_gtk_clip_answered, &read);
    } else {
        hero_ui_gtk.clipboard_read_texture_async(clipboard, cancel, hero_ui_gtk_clip_answered, &read);
    }
    if (!hero_ui_gtk_spin(5000, hero_ui_gtk_read_done, &read)) {
        /* The program holding the clipboard did not answer: the read is
         * cancelled, and its callback runs once more before `read` goes. */
        hero_ui_gtk.cancellable_cancel(cancel);
        while (!read.done) {
            (void)hero_ui_gtk.main_context_iteration(NULL, 1);
        }
        hero_layer_free(read.bytes);
        hero_ui_gtk.object_unref(cancel);
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "the program holding the clipboard did not answer within five seconds");
    }
    hero_ui_gtk.object_unref(cancel);
    if (read.answer != HERO_UI_OK) {
        hero_layer_free(read.bytes);
        if (read.words[0] != '\0') {
            (void)hero_ui_gtk_said_so(read.answer, read.words);
        }
        return read.answer;
    }
    hero_layer_free(hero_ui_gtk_clip);
    hero_ui_gtk_clip = read.bytes;
    hero_ui_gtk_clip_count = read.count;
    hero_ui_gtk_clip_kept = true;
    *bytes = read.count;
    *width = read.width;
    *height = read.height;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_clipboard_bytes(uint8_t *buffer, int64_t count) {
    if (!hero_ui_gtk_clip_kept) {
        return -1;
    }
    if (hero_ui_gtk_clip_count > 0 && hero_ui_gtk_clip_count <= count) {
        memcpy(buffer, hero_ui_gtk_clip, (size_t)hero_ui_gtk_clip_count);
    }
    return hero_ui_gtk_clip_count;
}

/* -- A TABLE ------------------------------------------------------------------
 *
 * A `GtkColumnView` in a scrolled window, over a `GtkMultiSelection` of a
 * `GtkStringList` whose strings are the rows' numbers, each column a
 * `GtkSignalListItemFactory` whose labels read the row's cell from the arm's
 * own cells. Every column has a sorter that finds every two rows equal, so
 * its header is clicked as a sortable one's and the view's sorter tells the
 * click, which the arm tells as `SORT` (GTK 4.10's primary sort column and
 * order), while the rows are never reordered: the program sorts them and
 * gives them again. A row opened is the view's `activate`. */

static void hero_ui_gtk_rows_picked(void *model, unsigned position, unsigned count, void *data) {
    hero_ui_native *table = hero_ui_gtk_of(data, model);

    (void)position;
    (void)count;
    if (table != NULL && table->kind == HERO_UI_TABLE) {
        hero_ui_tell(HERO_UI_EVENT_SELECT, table->id, 0, 0);
    }
}

static void hero_ui_gtk_table_opened(void *view, unsigned position, void *data) {
    hero_ui_native *table = hero_ui_gtk_of(data, view);

    if (table != NULL && table->kind == HERO_UI_TABLE) {
        hero_ui_tell(HERO_UI_EVENT_ACTIVATE, table->id, (int64_t)position, 0);
    }
}

/* The column and the order table `table` is sorted by, -1 for none. */
static int64_t hero_ui_gtk_sorted_by(const hero_ui_native *table, bool *ascending) {
    void *column = hero_ui_gtk.column_view_sorter_get_primary_sort_column(table->sorter);

    *ascending = hero_ui_gtk.column_view_sorter_get_primary_sort_order(table->sorter) == HERO_GTK_SORT_ASCENDING;
    for (int64_t i = 0; column != NULL && i < table->column_count; i += 1) {
        if (table->columns[i] == column) {
            return i;
        }
    }
    return -1;
}

static void hero_ui_gtk_sort_asked(void *sorter, int change, void *data) {
    hero_ui_native *table = hero_ui_gtk_of(data, sorter);
    bool ascending = true;
    int64_t column;

    (void)change;
    if (table == NULL || table->kind != HERO_UI_TABLE || hero_ui_is_muted()) {
        return;
    }
    column = hero_ui_gtk_sorted_by(table, &ascending);
    if (column >= 0) {
        hero_ui_tell(HERO_UI_EVENT_SORT, table->id, column, ascending ? 1 : 0);
    }
}

static void hero_ui_gtk_cell_made(void *factory, void *item, void *data) {
    void *label = hero_ui_gtk.label_new("");

    (void)factory;
    (void)data;
    if (label != NULL) {
        hero_ui_gtk.label_set_xalign(label, 0.0f);
        hero_ui_gtk.list_item_set_child(item, label);
    }
}

/* The number text `number` writes in decimal, -1 where it writes none. */
static int64_t hero_ui_gtk_number_of(const char *number) {
    char *end = NULL;
    long long value;

    if (number == NULL) {
        return -1;
    }
    value = strtoll(number, &end, 10);
    return end != number && *end == '\0' && value >= 0 ? (int64_t)value : -1;
}

/* The row a list item's string object names, -1 where none. */
static int64_t hero_ui_gtk_row_named(void *object) {
    return hero_ui_gtk_number_of(object != NULL ? hero_ui_gtk.string_object_get_string(object) : NULL);
}

static void hero_ui_gtk_cell_shown(void *factory, void *item, void *data) {
    hero_ui_native *table = hero_ui_find((int64_t)(intptr_t)data);
    void *label = hero_ui_gtk.list_item_get_child(item);
    int64_t column = (int64_t)(intptr_t)hero_ui_gtk.object_get_data(factory, "hero-column");
    int64_t row = hero_ui_gtk_row_named(hero_ui_gtk.list_item_get_item(item));

    if (label == NULL) {
        return;
    }
    if (table != NULL && table->kind == HERO_UI_TABLE && row >= 0 && row < table->rows && column >= 0 && column < table->column_count) {
        hero_ui_gtk.label_set_text(label, table->cells[row * table->column_count + column]);
    } else {
        hero_ui_gtk.label_set_text(label, "");
    }
}

static bool hero_ui_gtk_make_table(hero_ui_native *made) {
    made->strings = hero_ui_gtk.string_list_new(NULL);
    made->selection = made->strings != NULL ? hero_ui_gtk.multi_selection_new(made->strings) : NULL;
    made->inner = made->selection != NULL ? hero_ui_gtk.column_view_new(made->selection) : NULL;
    made->widget = hero_ui_gtk.scrolled_window_new();
    if (made->inner == NULL || made->widget == NULL) {
        return false;
    }
    hero_ui_gtk.column_view_set_reorderable(made->inner, 0);
    hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_AUTOMATIC, HERO_GTK_POLICY_AUTOMATIC);
    hero_ui_gtk.scrolled_window_set_has_frame(made->widget, 1);
    hero_ui_gtk.scrolled_window_set_child(made->widget, made->inner);
    made->sorter = hero_ui_gtk.column_view_get_sorter(made->inner);
    hero_ui_gtk_connect(made->inner, "activate", (hero_ui_gtk_callback)hero_ui_gtk_table_opened, made->id);
    hero_ui_gtk_connect(made->selection, "selection-changed", (hero_ui_gtk_callback)hero_ui_gtk_rows_picked, made->id);
    if (made->sorter != NULL) {
        hero_ui_gtk_connect(made->sorter, "changed", (hero_ui_gtk_callback)hero_ui_gtk_sort_asked, made->id);
    }
    return made->sorter != NULL;
}

/* Gives back table `table`'s cells. */
static void hero_ui_gtk_free_cells(hero_ui_native *table) {
    for (int64_t i = 0; table->cells != NULL && i < table->rows * table->column_count; i += 1) {
        hero_layer_free(table->cells[i]);
    }
    hero_layer_free(table->cells);
    table->cells = NULL;
}

/* Gives the rows `rows` to table `table`'s list, as the numbers 0 to
 * `rows` - 1, in place of what it held, muted: a choice of a row it held is
 * let go. */
static bool hero_ui_gtk_number_rows(hero_ui_native *table, int64_t rows) {
    char (*numbers)[24] = rows > 0 ? hero_layer_alloc((size_t)rows * sizeof *numbers) : NULL;
    const char **additions = hero_layer_alloc_zero((size_t)rows + 1, sizeof *additions);

    if ((rows > 0 && numbers == NULL) || additions == NULL) {
        hero_layer_free(numbers);
        hero_layer_free(additions);
        return false;
    }
    for (int64_t i = 0; i < rows; i += 1) {
        (void)snprintf(numbers[i], sizeof numbers[i], "%lld", (long long)i);
        additions[i] = numbers[i];
    }
    hero_ui_mute(1);
    hero_ui_gtk.string_list_splice(table->strings, 0, hero_ui_gtk.list_model_get_n_items(table->strings), (const char *const *)additions);
    hero_ui_mute(-1);
    hero_layer_free(numbers);
    hero_layer_free(additions);
    return true;
}

int64_t hero_ui_arm_set_columns(int64_t id, const uint8_t *titles, int64_t count) {
    hero_ui_native *table = hero_ui_find(id);
    int64_t made = hero_ui_texts_in(titles, count);
    void **columns;

    if (table == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (table->kind != HERO_UI_TABLE || made < 1) {
        return HERO_UI_INVALID;
    }
    columns = hero_layer_alloc_zero((size_t)made, sizeof *columns);
    if (columns == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_mute(1);
    for (int64_t i = 0; i < table->column_count; i += 1) {
        hero_ui_gtk.column_view_remove_column(table->inner, table->columns[i]);
        hero_ui_gtk.object_unref(table->columns[i]);
    }
    hero_layer_free(table->columns);
    hero_ui_gtk_free_cells(table);
    table->columns = columns;
    table->column_count = 0;
    table->rows = 0;
    (void)hero_ui_gtk_number_rows(table, 0);
    for (int64_t i = 0; i < made; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(titles, count, i, &length);
        char *title = hero_ui_gtk_text(text, length, false);
        void *factory = hero_ui_gtk.signal_list_item_factory_new();
        void *sorter = hero_ui_gtk.custom_sorter_new(NULL, NULL, NULL);
        void *column;

        if (title == NULL || factory == NULL || sorter == NULL) {
            hero_layer_free(title);
            hero_ui_mute(-1);
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.object_set_data(factory, "hero-column", (void *)(intptr_t)i);
        hero_ui_gtk_connect(factory, "setup", (hero_ui_gtk_callback)hero_ui_gtk_cell_made, id);
        hero_ui_gtk_connect(factory, "bind", (hero_ui_gtk_callback)hero_ui_gtk_cell_shown, id);
        column = hero_ui_gtk.column_view_column_new(title, factory);
        hero_layer_free(title);
        if (column == NULL) {
            hero_ui_gtk.object_unref(sorter);
            hero_ui_mute(-1);
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.column_view_column_set_resizable(column, 1);
        hero_ui_gtk.column_view_column_set_sorter(column, sorter);
        hero_ui_gtk.object_unref(sorter);
        hero_ui_gtk.column_view_column_set_expand(column, i == made - 1 ? 1 : 0);
        hero_ui_gtk.column_view_append_column(table->inner, column);
        table->columns[i] = column;
        table->column_count = i + 1;
    }
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

/* Gives table `table` the cells `items`, row after row. */
static int64_t hero_ui_gtk_table_rows(hero_ui_native *table, const uint8_t *items, int64_t count) {
    int64_t texts = hero_ui_texts_in(items, count);
    char **cells;

    if (table->column_count < 1 || texts % table->column_count != 0) {
        return HERO_UI_INVALID;
    }
    cells = texts > 0 ? hero_layer_alloc_zero((size_t)texts, sizeof *cells) : NULL;
    if (texts > 0 && cells == NULL) {
        return HERO_UI_LIMIT;
    }
    for (int64_t i = 0; i < texts; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(items, count, i, &length);

        cells[i] = hero_ui_gtk_text(text, length, false);
        if (cells[i] == NULL) {
            for (int64_t j = 0; j < i; j += 1) {
                hero_layer_free(cells[j]);
            }
            hero_layer_free(cells);
            return HERO_UI_LIMIT;
        }
    }
    hero_ui_gtk_free_cells(table);
    table->cells = cells;
    table->rows = texts / table->column_count;
    return hero_ui_gtk_number_rows(table, table->rows) ? HERO_UI_OK : HERO_UI_LIMIT;
}

int64_t hero_ui_arm_set_sort(int64_t id, int64_t column, int64_t ascending) {
    hero_ui_native *table = hero_ui_find(id);

    if (table == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (table->kind != HERO_UI_TABLE || column >= table->column_count) {
        return HERO_UI_INVALID;
    }
    hero_ui_mute(1);
    hero_ui_gtk.column_view_sort_by_column(table->inner, column < 0 ? NULL : table->columns[column], ascending == 1 ? HERO_GTK_SORT_ASCENDING : HERO_GTK_SORT_DESCENDING);
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

/* -- A TREE -------------------------------------------------------------------
 *
 * A `GtkListView` of `GtkTreeExpander` rows over a `GtkSingleSelection` of a
 * `GtkTreeListModel`, whose root and each item's children are a
 * `GtkStringList` of the items' numbers. The arm keeps each item, sorted by
 * its number: its text, the item it stands under, whether it is foldable,
 * and its children's list, made when it first holds or may hold an item;
 * the model asks for that list when a row unfolds, and a row is foldable
 * where its item is or holds items. GTK decides once whether a row unfolds,
 * so an item given its first child is put in its list again, its row made
 * anew. The person unfolding or folding a row is its `expanded` changing,
 * watched while the row is shown; folding a row that holds the item chosen
 * lets the choice go before GTK tells the fold, so the arm tells the fold
 * first, then the choice, as macOS tells them. */

typedef struct hero_ui_gtk_item {
    int64_t item;
    int64_t under;
    char *text;
    bool foldable;
    void *children;
} hero_ui_gtk_item;

typedef struct hero_ui_gtk_tree {
    hero_ui_gtk_item *items;
    int64_t count;
    int64_t room;
    void *root;
    void *model;
    /* The item chosen as the arm last told it, 0 for none; the item whose
     * fold was told before the choice it let go; whether an act's unfolding
     * was told. */
    int64_t chosen;
    int64_t folding;
    bool told;
} hero_ui_gtk_tree;

/* Where item `item` stands in the tree's sorted items, or where it would. */
static int64_t hero_ui_gtk_item_at(const hero_ui_gtk_tree *tree, int64_t item, bool *found) {
    int64_t low = 0;
    int64_t high = tree->count;

    while (low < high) {
        int64_t middle = low + (high - low) / 2;

        if (tree->items[middle].item < item) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }
    *found = low < tree->count && tree->items[low].item == item;
    return low;
}

/* Item `item` of tree `tree`, or NULL. */
static hero_ui_gtk_item *hero_ui_gtk_item_of(const hero_ui_gtk_tree *tree, int64_t item) {
    bool found = false;
    int64_t at = hero_ui_gtk_item_at(tree, item, &found);

    return found ? &tree->items[at] : NULL;
}

/* The number a tree's string object names, 0 where none. */
static int64_t hero_ui_gtk_item_named(void *object) {
    int64_t item = hero_ui_gtk_row_named(object);

    return item > 0 ? item : 0;
}

/* The model's create function: the list of an item's children, where it is
 * foldable or holds items, else NULL, so its row does not unfold. */
static void *hero_ui_gtk_children_of(void *object, void *data) {
    hero_ui_native *widget = hero_ui_find((int64_t)(intptr_t)data);
    hero_ui_gtk_item *item;

    if (widget == NULL || widget->kind != HERO_UI_TREE || widget->tree == NULL) {
        return NULL;
    }
    item = hero_ui_gtk_item_of(widget->tree, hero_ui_gtk_item_named(object));
    if (item == NULL) {
        return NULL;
    }
    if (item->children == NULL && item->foldable) {
        item->children = hero_ui_gtk.string_list_new(NULL);
    }
    if (item->children == NULL || (!item->foldable && hero_ui_gtk.list_model_get_n_items(item->children) == 0)) {
        return NULL;
    }
    return hero_ui_gtk.object_ref(item->children);
}

/* The list item `under` holds its items in: the root for 0. */
static void *hero_ui_gtk_list_under(const hero_ui_gtk_tree *tree, int64_t under) {
    hero_ui_gtk_item *item;

    if (under == 0) {
        return tree->root;
    }
    item = hero_ui_gtk_item_of(tree, under);
    return item != NULL ? item->children : NULL;
}

/* Where item `item` stands in list `list`, -1 where it does not. */
static int64_t hero_ui_gtk_position_in(void *list, int64_t item) {
    unsigned count = list != NULL ? hero_ui_gtk.list_model_get_n_items(list) : 0;
    char number[24];

    (void)snprintf(number, sizeof number, "%lld", (long long)item);
    for (unsigned i = 0; i < count; i += 1) {
        const char *each = hero_ui_gtk.string_list_get_string(list, i);

        if (each != NULL && strcmp(each, number) == 0) {
            return i;
        }
    }
    return -1;
}

/* The row of item `item` where the person sees it, every item over it
 * unfolded; NULL where one is folded. A reference the caller gives back. */
static void *hero_ui_gtk_tree_row(const hero_ui_gtk_tree *tree, int64_t item) {
    int64_t path[64];
    int64_t depth = 0;
    void *row = NULL;

    for (int64_t at = item; at != 0 && depth < (int64_t)(sizeof path / sizeof path[0]); depth += 1) {
        hero_ui_gtk_item *each = hero_ui_gtk_item_of(tree, at);

        if (each == NULL) {
            return NULL;
        }
        path[depth] = at;
        at = each->under;
    }
    for (int64_t level = depth - 1; level >= 0; level -= 1) {
        int64_t under = level == depth - 1 ? 0 : path[level + 1];
        int64_t position = hero_ui_gtk_position_in(hero_ui_gtk_list_under(tree, under), path[level]);
        void *next;

        if (position < 0) {
            next = NULL;
        } else if (row == NULL) {
            next = hero_ui_gtk.tree_list_model_get_child_row(tree->model, (unsigned)position);
        } else {
            next = hero_ui_gtk.tree_list_row_get_child_row(row, (unsigned)position);
        }
        if (row != NULL) {
            hero_ui_gtk.object_unref(row);
        }
        row = next;
        if (row == NULL) {
            return NULL;
        }
    }
    return row;
}

/* The item of tree `widget` chosen in GTK's selection, 0 for none. */
static int64_t hero_ui_gtk_tree_chosen(const hero_ui_native *widget) {
    unsigned position = hero_ui_gtk.single_selection_get_selected(widget->selection);
    void *row;
    void *object;
    int64_t item;

    if (position == HERO_GTK_INVALID_LIST_POSITION || widget->tree == NULL) {
        return 0;
    }
    row = hero_ui_gtk.tree_list_model_get_row(widget->tree->model, position);
    object = row != NULL ? hero_ui_gtk.tree_list_row_get_item(row) : NULL;
    item = hero_ui_gtk_item_named(object);
    if (object != NULL) {
        hero_ui_gtk.object_unref(object);
    }
    if (row != NULL) {
        hero_ui_gtk.object_unref(row);
    }
    return item;
}

/* The item folded that holds item `item` and stands where the person sees
 * it, every item over it unfolded; 0 for none. */
static int64_t hero_ui_gtk_fold_over(const hero_ui_gtk_tree *tree, int64_t item) {
    int64_t path[64];
    int64_t depth = 0;
    hero_ui_gtk_item *each = hero_ui_gtk_item_of(tree, item);

    for (int64_t at = each != NULL ? each->under : 0; at != 0 && depth < (int64_t)(sizeof path / sizeof path[0]); depth += 1) {
        hero_ui_gtk_item *over = hero_ui_gtk_item_of(tree, at);

        if (over == NULL) {
            return 0;
        }
        path[depth] = at;
        at = over->under;
    }
    for (int64_t level = depth - 1; level >= 0; level -= 1) {
        void *row = hero_ui_gtk_tree_row(tree, path[level]);
        bool unfolded = row != NULL && hero_ui_gtk.tree_list_row_get_expanded(row);

        if (row != NULL) {
            hero_ui_gtk.object_unref(row);
        }
        if (!unfolded) {
            return path[level];
        }
    }
    return 0;
}

/* The tree's choice changed: told where the item chosen is another, the
 * fold that let it go told first. */
static void hero_ui_gtk_tree_picked(void *selection, void *spec, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, selection);
    int64_t now;

    (void)spec;
    if (widget == NULL || widget->kind != HERO_UI_TREE || widget->tree == NULL) {
        return;
    }
    now = hero_ui_gtk_tree_chosen(widget);
    if (now == widget->tree->chosen) {
        return;
    }
    if (now == 0 && !hero_ui_is_muted()) {
        int64_t folded = hero_ui_gtk_fold_over(widget->tree, widget->tree->chosen);

        if (folded != 0) {
            hero_ui_tell(HERO_UI_EVENT_UNFOLD, widget->id, folded, 0);
            widget->tree->folding = folded;
            widget->tree->told = true;
        }
    }
    widget->tree->chosen = now;
    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, now, 0);
}

/* Row `row` of tree `data` unfolded or folded. */
static void hero_ui_gtk_row_folded(void *row, void *spec, void *data) {
    hero_ui_native *widget = hero_ui_find((int64_t)(intptr_t)data);
    void *object;
    int64_t item;

    (void)spec;
    if (widget == NULL || widget->kind != HERO_UI_TREE || widget->tree == NULL || hero_ui_is_muted()) {
        return;
    }
    object = hero_ui_gtk.tree_list_row_get_item(row);
    item = hero_ui_gtk_item_named(object);
    if (object != NULL) {
        hero_ui_gtk.object_unref(object);
    }
    if (item == 0) {
        return;
    }
    if (widget->tree->folding == item) {
        widget->tree->folding = 0;
        return;
    }
    widget->tree->told = true;
    hero_ui_tell(HERO_UI_EVENT_UNFOLD, widget->id, item, hero_ui_gtk.tree_list_row_get_expanded(row) ? 1 : 0);
}

static void hero_ui_gtk_tree_row_made(void *factory, void *item, void *data) {
    void *expander = hero_ui_gtk.tree_expander_new();
    void *label = hero_ui_gtk.label_new("");

    (void)factory;
    (void)data;
    if (expander == NULL || label == NULL) {
        return;
    }
    hero_ui_gtk.label_set_xalign(label, 0.0f);
    hero_ui_gtk.tree_expander_set_child(expander, label);
    hero_ui_gtk.object_set_data(expander, "hero-label", label);
    hero_ui_gtk.list_item_set_child(item, expander);
}

static void hero_ui_gtk_tree_row_shown(void *factory, void *item, void *data) {
    hero_ui_native *widget = hero_ui_find((int64_t)(intptr_t)data);
    void *row = hero_ui_gtk.list_item_get_item(item);
    void *expander = hero_ui_gtk.list_item_get_child(item);
    void *label = expander != NULL ? hero_ui_gtk.object_get_data(expander, "hero-label") : NULL;
    void *object;
    hero_ui_gtk_item *shown = NULL;

    (void)factory;
    if (row == NULL || expander == NULL || label == NULL || widget == NULL || widget->tree == NULL) {
        return;
    }
    hero_ui_gtk.tree_expander_set_list_row(expander, row);
    object = hero_ui_gtk.tree_list_row_get_item(row);
    shown = hero_ui_gtk_item_of(widget->tree, hero_ui_gtk_item_named(object));
    if (object != NULL) {
        hero_ui_gtk.object_unref(object);
    }
    hero_ui_gtk.label_set_text(label, shown != NULL && shown->text != NULL ? shown->text : "");
    (void)hero_ui_gtk.signal_connect_data(row, "notify::expanded", (hero_ui_gtk_callback)hero_ui_gtk_row_folded, data, NULL, 0);
}

static void hero_ui_gtk_tree_row_hidden(void *factory, void *item, void *data) {
    void *row = hero_ui_gtk.list_item_get_item(item);
    void *expander = hero_ui_gtk.list_item_get_child(item);

    (void)factory;
    if (row != NULL) {
        (void)hero_ui_gtk.signal_handlers_disconnect_matched(row, HERO_G_SIGNAL_MATCH_FUNC_DATA, 0, 0, NULL, (hero_ui_gtk_callback)hero_ui_gtk_row_folded, data);
    }
    if (expander != NULL) {
        hero_ui_gtk.tree_expander_set_list_row(expander, NULL);
    }
}

static void hero_ui_gtk_tree_opened(void *view, unsigned position, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, view);
    void *row;
    void *object;
    int64_t item;

    if (widget == NULL || widget->kind != HERO_UI_TREE || widget->tree == NULL) {
        return;
    }
    row = hero_ui_gtk.tree_list_model_get_row(widget->tree->model, position);
    object = row != NULL ? hero_ui_gtk.tree_list_row_get_item(row) : NULL;
    item = hero_ui_gtk_item_named(object);
    if (object != NULL) {
        hero_ui_gtk.object_unref(object);
    }
    if (row != NULL) {
        hero_ui_gtk.object_unref(row);
    }
    if (item != 0) {
        hero_ui_tell(HERO_UI_EVENT_ACTIVATE, widget->id, item, 0);
    }
}

static bool hero_ui_gtk_make_tree(hero_ui_native *made) {
    void *factory = hero_ui_gtk.signal_list_item_factory_new();

    made->tree = hero_layer_alloc_zero(1, sizeof *made->tree);
    if (made->tree == NULL || factory == NULL) {
        return false;
    }
    made->tree->root = hero_ui_gtk.string_list_new(NULL);
    made->tree->model = made->tree->root != NULL ? hero_ui_gtk.tree_list_model_new(made->tree->root, 0, 0, hero_ui_gtk_children_of, (void *)(intptr_t)made->id, NULL) : NULL;
    made->selection = made->tree->model != NULL ? hero_ui_gtk.single_selection_new(made->tree->model) : NULL;
    if (made->selection == NULL) {
        return false;
    }
    hero_ui_gtk.single_selection_set_autoselect(made->selection, 0);
    hero_ui_gtk.single_selection_set_can_unselect(made->selection, 1);
    hero_ui_gtk.single_selection_set_selected(made->selection, HERO_GTK_INVALID_LIST_POSITION);
    hero_ui_gtk_connect(factory, "setup", (hero_ui_gtk_callback)hero_ui_gtk_tree_row_made, made->id);
    hero_ui_gtk_connect(factory, "bind", (hero_ui_gtk_callback)hero_ui_gtk_tree_row_shown, made->id);
    hero_ui_gtk_connect(factory, "unbind", (hero_ui_gtk_callback)hero_ui_gtk_tree_row_hidden, made->id);
    made->inner = hero_ui_gtk.list_view_new(made->selection, factory);
    made->widget = hero_ui_gtk.scrolled_window_new();
    if (made->inner == NULL || made->widget == NULL) {
        return false;
    }
    hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_AUTOMATIC, HERO_GTK_POLICY_AUTOMATIC);
    hero_ui_gtk.scrolled_window_set_has_frame(made->widget, 1);
    hero_ui_gtk.scrolled_window_set_child(made->widget, made->inner);
    hero_ui_gtk_connect(made->selection, "notify::selected", (hero_ui_gtk_callback)hero_ui_gtk_tree_picked, made->id);
    hero_ui_gtk_connect(made->inner, "activate", (hero_ui_gtk_callback)hero_ui_gtk_tree_opened, made->id);
    return true;
}

static void hero_ui_gtk_free_view(hero_ui_native *widget) {
    hero_ui_gtk_free_cells(widget);
    for (int64_t i = 0; i < widget->column_count; i += 1) {
        hero_ui_gtk.object_unref(widget->columns[i]);
    }
    hero_layer_free(widget->columns);
    widget->columns = NULL;
    widget->column_count = 0;
    if (widget->tree != NULL) {
        for (int64_t i = 0; i < widget->tree->count; i += 1) {
            hero_layer_free(widget->tree->items[i].text);
            if (widget->tree->items[i].children != NULL) {
                hero_ui_gtk.object_unref(widget->tree->items[i].children);
            }
        }
        hero_layer_free(widget->tree->items);
        hero_layer_free(widget->tree);
        widget->tree = NULL;
    }
}

/* Puts item `item`'s number in its list again at the same place, so GTK
 * asks again whether its row unfolds; its choice kept, muted. */
static void hero_ui_gtk_remake_row(hero_ui_native *widget, int64_t item) {
    hero_ui_gtk_item *each = hero_ui_gtk_item_of(widget->tree, item);
    void *list = each != NULL ? hero_ui_gtk_list_under(widget->tree, each->under) : NULL;
    int64_t position = hero_ui_gtk_position_in(list, item);
    char number[24];
    const char *again[2] = {number, NULL};
    int64_t chosen = widget->tree->chosen;

    if (position < 0) {
        return;
    }
    (void)snprintf(number, sizeof number, "%lld", (long long)item);
    hero_ui_mute(1);
    hero_ui_gtk.string_list_splice(list, (unsigned)position, 1, again);
    hero_ui_mute(-1);
    if (chosen != 0 && hero_ui_gtk_tree_chosen(widget) != chosen) {
        (void)hero_ui_gtk_tree_select(widget, chosen);
    }
}

int64_t hero_ui_arm_tree_add(int64_t id, int64_t item, int64_t under, const uint8_t *text, int64_t count, int64_t foldable) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_gtk_tree *tree;
    hero_ui_gtk_item *parent = NULL;
    bool found = false;
    bool listed = true;
    int64_t at;
    char *shown;
    char number[24];

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_TREE || widget->tree == NULL) {
        return HERO_UI_INVALID;
    }
    tree = widget->tree;
    at = hero_ui_gtk_item_at(tree, item, &found);
    parent = under != 0 ? hero_ui_gtk_item_of(tree, under) : NULL;
    if (found || (under != 0 && parent == NULL)) {
        return HERO_UI_NOT_FOUND;
    }
    if (parent != NULL && parent->children == NULL) {
        listed = false;
        parent->children = hero_ui_gtk.string_list_new(NULL);
        if (parent->children == NULL) {
            return HERO_UI_LIMIT;
        }
    }
    shown = hero_ui_gtk_text(text, count, false);
    if (shown == NULL) {
        return HERO_UI_LIMIT;
    }
    if (tree->count == tree->room) {
        int64_t room = tree->room * 2 + 16;
        hero_ui_gtk_item *grown = hero_layer_grow(tree->items, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            hero_layer_free(shown);
            return HERO_UI_LIMIT;
        }
        tree->items = grown;
        tree->room = room;
    }
    memmove(&tree->items[at + 1], &tree->items[at], (size_t)(tree->count - at) * sizeof tree->items[0]);
    tree->items[at] = (hero_ui_gtk_item){item, under, shown, foldable == 1, NULL};
    tree->count += 1;
    /* The items moved: the parent is found again. */
    parent = under != 0 ? hero_ui_gtk_item_of(tree, under) : NULL;
    (void)snprintf(number, sizeof number, "%lld", (long long)item);
    hero_ui_mute(1);
    hero_ui_gtk.string_list_append(under == 0 ? tree->root : parent->children, number);
    hero_ui_mute(-1);
    if (parent != NULL && !parent->foldable && (!listed || hero_ui_gtk.list_model_get_n_items(parent->children) == 1)) {
        /* Its first item: GTK is to ask again whether its row unfolds. */
        hero_ui_gtk_remake_row(widget, under);
    }
    return HERO_UI_OK;
}

/* Takes item `item` and every item under it out of tree `tree`'s items. */
static void hero_ui_gtk_forget_item(hero_ui_gtk_tree *tree, int64_t item) {
    bool found = false;
    int64_t at = hero_ui_gtk_item_at(tree, item, &found);
    void *children;

    if (!found) {
        return;
    }
    children = tree->items[at].children;
    while (children != NULL && hero_ui_gtk.list_model_get_n_items(children) > 0) {
        int64_t child = hero_ui_gtk_number_of(hero_ui_gtk.string_list_get_string(children, 0));

        hero_ui_gtk.string_list_remove(children, 0);
        hero_ui_gtk_forget_item(tree, child);
    }
    at = hero_ui_gtk_item_at(tree, item, &found);
    hero_layer_free(tree->items[at].text);
    if (tree->items[at].children != NULL) {
        hero_ui_gtk.object_unref(tree->items[at].children);
    }
    memmove(&tree->items[at], &tree->items[at + 1], (size_t)(tree->count - at - 1) * sizeof tree->items[0]);
    tree->count -= 1;
}

int64_t hero_ui_arm_tree_remove(int64_t id, int64_t item) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_gtk_item *each;
    void *list;
    int64_t under;
    int64_t position;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_TREE || widget->tree == NULL) {
        return HERO_UI_INVALID;
    }
    each = hero_ui_gtk_item_of(widget->tree, item);
    if (each == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    under = each->under;
    list = hero_ui_gtk_list_under(widget->tree, under);
    position = hero_ui_gtk_position_in(list, item);
    hero_ui_mute(1);
    if (position >= 0) {
        hero_ui_gtk.string_list_remove(list, (unsigned)position);
    }
    hero_ui_gtk_forget_item(widget->tree, item);
    widget->tree->chosen = hero_ui_gtk_tree_chosen(widget);
    hero_ui_mute(-1);
    {
        hero_ui_gtk_item *parent = under != 0 ? hero_ui_gtk_item_of(widget->tree, under) : NULL;

        if (parent != NULL && !parent->foldable && parent->children != NULL && hero_ui_gtk.list_model_get_n_items(parent->children) == 0) {
            /* Its last item gone: its row no longer unfolds. */
            hero_ui_gtk_remake_row(widget, under);
        }
    }
    return HERO_UI_OK;
}

/* Unfolds every item over item `item` of tree `widget`, muted. */
static void hero_ui_gtk_unfold_over(hero_ui_native *widget, int64_t item) {
    hero_ui_gtk_item *each = hero_ui_gtk_item_of(widget->tree, item);
    int64_t path[64];
    int64_t depth = 0;

    for (int64_t at = each != NULL ? each->under : 0; at != 0 && depth < (int64_t)(sizeof path / sizeof path[0]); depth += 1) {
        hero_ui_gtk_item *over = hero_ui_gtk_item_of(widget->tree, at);

        if (over == NULL) {
            return;
        }
        path[depth] = at;
        at = over->under;
    }
    hero_ui_mute(1);
    for (int64_t level = depth - 1; level >= 0; level -= 1) {
        void *row = hero_ui_gtk_tree_row(widget->tree, path[level]);

        if (row != NULL) {
            hero_ui_gtk.tree_list_row_set_expanded(row, 1);
            hero_ui_gtk.object_unref(row);
        }
    }
    hero_ui_mute(-1);
}

static int64_t hero_ui_gtk_tree_select(hero_ui_native *widget, int64_t item) {
    void *row;

    if (widget->tree == NULL) {
        return HERO_UI_INVALID;
    }
    if (item < 0) {
        hero_ui_mute(1);
        hero_ui_gtk.single_selection_set_selected(widget->selection, HERO_GTK_INVALID_LIST_POSITION);
        widget->tree->chosen = 0;
        hero_ui_mute(-1);
        return HERO_UI_OK;
    }
    if (hero_ui_gtk_item_of(widget->tree, item) == NULL) {
        return HERO_UI_INVALID;
    }
    hero_ui_gtk_unfold_over(widget, item);
    row = hero_ui_gtk_tree_row(widget->tree, item);
    if (row == NULL) {
        return HERO_UI_INVALID;
    }
    hero_ui_mute(1);
    hero_ui_gtk.single_selection_set_selected(widget->selection, hero_ui_gtk.tree_list_row_get_position(row));
    widget->tree->chosen = item;
    hero_ui_mute(-1);
    hero_ui_gtk.object_unref(row);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_tree_unfold(int64_t id, int64_t item, int64_t on) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_gtk_item *each;
    void *row;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_TREE || widget->tree == NULL) {
        return HERO_UI_INVALID;
    }
    each = hero_ui_gtk_item_of(widget->tree, item);
    if (each == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!each->foldable && (each->children == NULL || hero_ui_gtk.list_model_get_n_items(each->children) == 0)) {
        return HERO_UI_INVALID;
    }
    if (on == 1) {
        hero_ui_gtk_unfold_over(widget, item);
    }
    row = hero_ui_gtk_tree_row(widget->tree, item);
    if (row == NULL) {
        return HERO_UI_OK;
    }
    hero_ui_mute(1);
    hero_ui_gtk.tree_list_row_set_expanded(row, on == 1 ? 1 : 0);
    widget->tree->chosen = hero_ui_gtk_tree_chosen(widget);
    hero_ui_mute(-1);
    hero_ui_gtk.object_unref(row);
    return HERO_UI_OK;
}

/* -- WHAT THE PERSON DOES TO THE NEW KINDS -------------------------------- */

static int64_t hero_ui_gtk_act(int64_t kind, hero_ui_native *widget, int64_t value, int64_t second, bool usable) {
    if (kind == HERO_UI_EVENT_SELECT && widget->kind == HERO_UI_TABS) {
        if (value < 0 || value >= hero_ui_gtk.notebook_get_n_pages(widget->widget)) {
            return HERO_UI_INVALID;
        }
        if (usable) {
            if (hero_ui_gtk.notebook_get_current_page(widget->widget) == value) {
                hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, value, 0);
            } else {
                hero_ui_gtk.notebook_set_current_page(widget->widget, (int)value);
            }
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_SELECT && widget->kind == HERO_UI_TABLE) {
        if (value >= widget->rows) {
            return HERO_UI_INVALID;
        }
        if (usable) {
            if (value < 0) {
                (void)hero_ui_gtk.selection_model_unselect_all(widget->selection);
            } else {
                (void)hero_ui_gtk.selection_model_select_item(widget->selection, (unsigned)value, second == 1 ? 0 : 1);
            }
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_SELECT && widget->kind == HERO_UI_TREE) {
        void *row;

        if (value < 0) {
            if (usable) {
                hero_ui_gtk.single_selection_set_selected(widget->selection, HERO_GTK_INVALID_LIST_POSITION);
            }
            return HERO_UI_OK;
        }
        row = hero_ui_gtk_tree_row(widget->tree, value);
        if (row == NULL) {
            return HERO_UI_INVALID;
        }
        if (usable) {
            if (widget->tree->chosen == value) {
                hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, value, 0);
            } else {
                hero_ui_gtk.single_selection_set_selected(widget->selection, hero_ui_gtk.tree_list_row_get_position(row));
            }
        }
        hero_ui_gtk.object_unref(row);
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_VALUE) {
        int64_t held = hero_ui_between(value, widget->least, widget->most);

        if (usable) {
            widget->split_extent = widget->kind == HERO_UI_SPLIT_ROW ? hero_ui_gtk.widget_get_width(widget->widget) : hero_ui_gtk.widget_get_height(widget->widget);
            if (hero_ui_gtk.paned_get_position(widget->widget) == held) {
                widget->value = held;
                hero_ui_tell(HERO_UI_EVENT_VALUE, widget->id, held, 0);
            } else {
                hero_ui_gtk.paned_set_position(widget->widget, hero_ui_gtk_int(held));
            }
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_ACTIVATE && (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE)) {
        void *row;

        if (value < 0 || value >= widget->items) {
            return HERO_UI_INVALID;
        }
        row = hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)value);
        if (usable && row != NULL) {
            /* A row not chosen is chosen first, as the first click of a
             * double click chooses it. */
            if (!hero_ui_gtk.list_box_row_is_selected(row)) {
                hero_ui_gtk.list_box_unselect_all(widget->inner);
                hero_ui_gtk.list_box_select_row(widget->inner, row);
            }
            (void)hero_ui_gtk.widget_activate(row);
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_ACTIVATE && widget->kind == HERO_UI_TABLE) {
        if (value < 0 || value >= widget->rows) {
            return HERO_UI_INVALID;
        }
        if (usable) {
            if (!hero_ui_gtk.selection_model_is_selected(widget->selection, (unsigned)value)) {
                (void)hero_ui_gtk.selection_model_select_item(widget->selection, (unsigned)value, 1);
            }
            hero_ui_gtk.signal_emit_by_name(widget->inner, "activate", (unsigned)value);
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_ACTIVATE && widget->kind == HERO_UI_TREE) {
        void *row = hero_ui_gtk_tree_row(widget->tree, value);
        unsigned position;

        if (row == NULL) {
            return HERO_UI_INVALID;
        }
        position = hero_ui_gtk.tree_list_row_get_position(row);
        hero_ui_gtk.object_unref(row);
        if (usable) {
            if (widget->tree->chosen != value) {
                hero_ui_gtk.single_selection_set_selected(widget->selection, position);
            }
            hero_ui_gtk.signal_emit_by_name(widget->inner, "activate", position);
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_SUBMIT) {
        if (widget->kind != HERO_UI_TEXT_FIELD) {
            return HERO_UI_INVALID;
        }
        if (usable) {
            /* Return in the field: the signal GTK's Return key emits on the
             * field's text widget, which the field hands on as its own. */
            void *text = hero_ui_gtk.editable_get_delegate(widget->widget);

            hero_ui_gtk.signal_emit_by_name(text != NULL ? text : widget->widget, "activate");
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_SORT && widget->kind == HERO_UI_TABLE) {
        bool ascending = true;
        int64_t column;

        if (value < 0 || value >= widget->column_count || (second != 0 && second != 1)) {
            return HERO_UI_INVALID;
        }
        if (usable) {
            column = hero_ui_gtk_sorted_by(widget, &ascending);
            if (column == value && ascending == (second == 1)) {
                hero_ui_tell(HERO_UI_EVENT_SORT, widget->id, value, second);
            } else {
                hero_ui_gtk.column_view_sort_by_column(widget->inner, widget->columns[value], second == 1 ? HERO_GTK_SORT_ASCENDING : HERO_GTK_SORT_DESCENDING);
            }
        }
        return HERO_UI_OK;
    }
    if (kind == HERO_UI_EVENT_UNFOLD && widget->kind == HERO_UI_TREE) {
        hero_ui_gtk_item *each = hero_ui_gtk_item_of(widget->tree, value);
        void *row;

        if (each == NULL || (second != 0 && second != 1) || (!each->foldable && (each->children == NULL || hero_ui_gtk.list_model_get_n_items(each->children) == 0))) {
            return HERO_UI_INVALID;
        }
        row = hero_ui_gtk_tree_row(widget->tree, value);
        if (row == NULL) {
            return HERO_UI_INVALID;
        }
        if (usable && hero_ui_gtk.tree_list_row_get_expanded(row) != (second == 1)) {
            widget->tree->told = false;
            widget->tree->folding = 0;
            hero_ui_gtk.tree_list_row_set_expanded(row, second == 1 ? 1 : 0);
            if (!widget->tree->told) {
                /* Its row was not shown, so no watch of it told the fold. */
                hero_ui_tell(HERO_UI_EVENT_UNFOLD, widget->id, value, second);
            }
            widget->tree->folding = 0;
        }
        hero_ui_gtk.object_unref(row);
        return HERO_UI_OK;
    }
    return HERO_UI_INVALID;
}

/* -- WINDOWS ON THE SCREEN ----------------------------------------------------
 *
 * THE SCREENS are GDK's monitors, in the desktop's points (GDK's
 * application pixels), the X server's primary monitor first where the
 * display is X11, its top-left corner the desktop's origin; a work area is
 * the X11 backend's (`_NET_WORKAREA` or GNOME's per-monitor areas, set by a
 * window manager), else the whole area, Wayland telling a program none.
 *
 * ON X11 a window's frame is read from the X server: its X window's place
 * on the root window, less the frame a window manager draws around it
 * (`_NET_FRAME_EXTENTS`) and the shadow GTK draws inside it
 * (`_GTK_FRAME_EXTENTS`), in pixels over the surface's scale. A window is
 * moved by `XMoveWindow` on its X window, which a window manager reads by
 * the window's gravity: the arm asks once, reads where the frame went, and
 * asks again corrected for what the manager added, until it stands where
 * asked. A place asked while the window is not shown is kept and given
 * once it is mapped. A window's moves are told from the X server's
 * `ConfigureNotify`, read through GDK's `xevent` signal and checked from an
 * idle source, each window's screen before its place.
 *
 * A STATE is GTK's (`gtk_window_maximize` and the rest), read back from the
 * surface's `GdkToplevelState`, whose changes are told as the person's
 * unless the program's own call is bringing the window there. Keeping a
 * window above the others is `_NET_WM_STATE_ABOVE`, asked of the window
 * manager by the client message EWMH names. The app's icon is
 * `gdk_toplevel_set_icon_list` on every window's surface, X11's
 * `_NET_WM_ICON`, given again to each window as it is realized. A modal
 * window is GTK's, `gtk_window_set_modal`, transient for another window of
 * the app's, whose grab keeps the other windows from the person's acts. */

/* The most screens read. */
#define HERO_UI_GTK_SCREENS_MOST 64

/* Xlib's `XClientMessageEvent` inside its `XEvent`, as libX11 1.8.12 lays
 * them out on every 64-bit target: 192 bytes, the window at 32, the message's
 * type at 40, its format at 48, its data at 56. */
typedef union hero_ui_x_event {
    struct {
        int type;
        unsigned long serial;
        int send_event;
        void *display;
        unsigned long window;
        unsigned long message_type;
        int format;
        long data[5];
    } client;
    long pad[24];
} hero_ui_x_event;

_Static_assert(sizeof(hero_ui_x_event) == 192, "XEvent is 192 bytes on a 64-bit target");

/* Xlib's `XWindowAttributes`, 136 bytes, read for its map state at 92. */
typedef union hero_ui_x_attributes {
    unsigned char bytes[136];
    long align;
} hero_ui_x_attributes;

#define HERO_X_MAP_STATE_AT 92
#define HERO_X_IS_VIEWABLE 2
#define HERO_X_REVERT_TO_PARENT 2

typedef struct hero_ui_gtk_rect {
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
} hero_ui_gtk_rect;

typedef struct hero_ui_gtk_screen {
    hero_ui_gtk_rect area;
    hero_ui_gtk_rect work;
    int64_t scale;
    void *monitor;
} hero_ui_gtk_screen;

static const char hero_ui_gtk_wayland_place[] = "Wayland lets no program place or read its own window's position: xdg_toplevel has no such request";
static const char hero_ui_gtk_wayland_top[] = "Wayland lets no program keep its window above every other: xdg_toplevel has no such request";
static const char hero_ui_gtk_wayland_minimized[] = "Wayland lets no program read whether its window is minimized: xdg_toplevel's set_minimized says the client cannot query or undo it";
static const char hero_ui_gtk_wayland_key[] = "Wayland lets no program press a key: GTK 4 makes no key event, and the compositor takes keys from the person's devices alone";
static const char hero_ui_gtk_no_manager[] =
    "no window manager runs on this X display (its root window names none in _NET_SUPPORTING_WM_CHECK), and X11 leaves a window's state and its stacking to one";

/* Xlib's display GTK opened, where the display is X11 and the backend's
 * functions and Xlib's are all found; else NULL. */
static void *hero_ui_gtk_xdisplay(void) {
    void *display = hero_ui_gtk.display_get_default();

    if (display == NULL || hero_ui_gtk.x11_display_get_type == NULL || hero_ui_gtk.x11_display_get_xdisplay == NULL || hero_ui_gtk.x11_display_get_xrootwindow == NULL || hero_ui_gtk.x11_surface_get_xid == NULL ||
        hero_ui_gtk.x11_get_xatom_by_name_for_display == NULL || hero_ui_gtk.x_move_window == NULL || hero_ui_gtk.x_translate_coordinates == NULL || hero_ui_gtk.x_get_window_property == NULL ||
        hero_ui_gtk.x_free == NULL || hero_ui_gtk.x_sync == NULL || hero_ui_gtk.x_send_event == NULL || hero_ui_gtk.x_get_window_attributes == NULL) {
        return NULL;
    }
    if (!hero_ui_gtk.type_check_instance_is_a(display, hero_ui_gtk.x11_display_get_type())) {
        return NULL;
    }
    return hero_ui_gtk.x11_display_get_xdisplay(display);
}

/* `words` where the display is Wayland's; else what GTK's backend there,
 * neither X11 nor Wayland, offers no program. */
static int64_t hero_ui_gtk_not_x11(const char *words) {
    void *display = hero_ui_gtk.display_get_default();

    if (display != NULL && hero_ui_gtk.wayland_display_get_type != NULL && hero_ui_gtk.type_check_instance_is_a(display, hero_ui_gtk.wayland_display_get_type())) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, words);
    }
    return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "GTK's backend on this display is neither X11 nor Wayland, and offers no program this");
}

/* The X atom of `name`. */
static unsigned long hero_ui_gtk_atom(const char *name) {
    return hero_ui_gtk.x11_get_xatom_by_name_for_display(hero_ui_gtk.display_get_default(), name);
}

/* The first `count` 32-bit numbers of property `name` of type `type` on X
 * window `window`, into `into`; false where it holds fewer. */
static bool hero_ui_gtk_x_numbers(void *xd, unsigned long window, const char *name, unsigned long type, long *into, long count) {
    unsigned long actual = 0;
    unsigned long items = 0;
    unsigned long after = 0;
    int format = 0;
    unsigned char *data = NULL;
    bool ok;

    if (hero_ui_gtk.x_get_window_property(xd, window, hero_ui_gtk_atom(name), 0, count, 0, type, &actual, &format, &items, &after, &data) != 0 || data == NULL) {
        return false;
    }
    /* Xlib hands a property of format 32 as C `long`s. */
    ok = actual == type && format == 32 && items >= (unsigned long)count;
    if (ok) {
        memcpy(into, data, (size_t)count * sizeof *into);
    }
    (void)hero_ui_gtk.x_free(data);
    return ok;
}

/* Whether a window manager runs on X display `xd`, by EWMH's check: the
 * root window names a window that names itself. */
static bool hero_ui_gtk_manager_runs(void *xd) {
    unsigned long root = hero_ui_gtk.x11_display_get_xrootwindow(hero_ui_gtk.display_get_default());
    long checked = 0;
    long again = 0;

    return hero_ui_gtk_x_numbers(xd, root, "_NET_SUPPORTING_WM_CHECK", HERO_XA_WINDOW, &checked, 1) && hero_ui_gtk_x_numbers(xd, (unsigned long)checked, "_NET_SUPPORTING_WM_CHECK", HERO_XA_WINDOW, &again, 1) && again == checked;
}

/* Whether the window manager on X display `xd` names `hint` among the hints
 * it supports (`_NET_SUPPORTED`). */
static bool hero_ui_gtk_manager_offers(void *xd, const char *hint) {
    unsigned long root = hero_ui_gtk.x11_display_get_xrootwindow(hero_ui_gtk.display_get_default());
    unsigned long wanted = hero_ui_gtk_atom(hint);
    unsigned long actual = 0;
    unsigned long items = 0;
    unsigned long after = 0;
    int format = 0;
    unsigned char *data = NULL;
    bool found = false;

    if (hero_ui_gtk.x_get_window_property(xd, root, hero_ui_gtk_atom("_NET_SUPPORTED"), 0, 4096, 0, HERO_XA_ATOM, &actual, &format, &items, &after, &data) != 0 || data == NULL) {
        return false;
    }
    for (unsigned long i = 0; actual == HERO_XA_ATOM && format == 32 && i < items && !found; i += 1) {
        long each;

        memcpy(&each, data + i * sizeof each, sizeof each);
        found = (unsigned long)each == wanted;
    }
    (void)hero_ui_gtk.x_free(data);
    return found;
}

/* The screens, at most `most`, the main one first; their count. */
static int64_t hero_ui_gtk_screens_now(hero_ui_gtk_screen *into, int64_t most) {
    void *display = hero_ui_gtk.display_get_default();
    void *monitors = display != NULL ? hero_ui_gtk.display_get_monitors(display) : NULL;
    unsigned count = monitors != NULL ? hero_ui_gtk.list_model_get_n_items(monitors) : 0;
    void *primary = NULL;
    int64_t made = 0;
    int64_t main = 0;
    bool x11 = hero_ui_gtk_xdisplay() != NULL;

    if (x11 && hero_ui_gtk.x11_display_get_primary_monitor != NULL) {
        primary = hero_ui_gtk.x11_display_get_primary_monitor(display);
    }
    for (unsigned i = 0; i < count && made < most; i += 1) {
        void *monitor = hero_ui_gtk.list_model_get_item(monitors, i);
        int area[4] = {0, 0, 0, 0};
        int work[4];

        if (monitor == NULL) {
            continue;
        }
        hero_ui_gtk.monitor_get_geometry(monitor, area);
        memcpy(work, area, sizeof work);
        if (x11 && hero_ui_gtk.x11_monitor_get_workarea != NULL) {
            hero_ui_gtk.x11_monitor_get_workarea(monitor, work);
        }
        into[made].area = (hero_ui_gtk_rect){area[0], area[1], area[2], area[3]};
        into[made].work = (hero_ui_gtk_rect){work[0], work[1], work[2], work[3]};
        if (hero_ui_gtk.monitor_get_scale != NULL) {
            into[made].scale = (int64_t)(hero_ui_gtk.monitor_get_scale(monitor) * 100.0 + 0.5);
        } else {
            into[made].scale = (int64_t)hero_ui_gtk.monitor_get_scale_factor(monitor) * 100;
        }
        into[made].monitor = monitor;
        if (monitor == primary) {
            main = made;
        }
        hero_ui_gtk.object_unref(monitor);
        made += 1;
    }
    for (int64_t i = 0; primary == NULL && i < made; i += 1) {
        /* With no primary monitor named, the one at the desktop's origin. */
        if (into[i].area.x == 0 && into[i].area.y == 0) {
            main = i;
            break;
        }
    }
    if (main > 0) {
        hero_ui_gtk_screen first = into[main];

        memmove(&into[1], &into[0], (size_t)main * sizeof into[0]);
        into[0] = first;
    }
    for (int64_t i = made - 1; i >= 0; i -= 1) {
        /* The desktop's origin is the main screen's top-left corner. */
        into[i].area.x -= into[0].area.x;
        into[i].area.y -= into[0].area.y;
        into[i].work.x -= into[0].area.x;
        into[i].work.y -= into[0].area.y;
        if (i == 0) {
            into[0].area.x = 0;
            into[0].area.y = 0;
        }
    }
    return made;
}

/* The screen rectangle `rect` stands on most, by the area they share; 0
 * where it shares none with any. */
static int64_t hero_ui_gtk_screen_under(const hero_ui_gtk_screen *screens, int64_t count, hero_ui_gtk_rect rect) {
    int64_t best = 0;
    int64_t most = -1;

    for (int64_t i = 0; i < count; i += 1) {
        const hero_ui_gtk_rect *area = &screens[i].area;
        int64_t wide = (rect.x + rect.width < area->x + area->width ? rect.x + rect.width : area->x + area->width) - (rect.x > area->x ? rect.x : area->x);
        int64_t high = (rect.y + rect.height < area->y + area->height ? rect.y + rect.height : area->y + area->height) - (rect.y > area->y ? rect.y : area->y);
        int64_t shared = wide > 0 && high > 0 ? wide * high : 0;

        if (shared > most) {
            best = i;
            most = shared;
        }
    }
    return best;
}

/* Window `window`'s surface, or NULL where it was never realized. */
static void *hero_ui_gtk_surface(const hero_ui_native *window) {
    return window->widget != NULL ? hero_ui_gtk.native_get_surface(window->widget) : NULL;
}

/* Whether window `window`'s X window is viewable, mapped with every window
 * over it. */
static bool hero_ui_gtk_viewable(void *data) {
    hero_ui_native *window = data;
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    hero_ui_x_attributes attributes;
    int state;

    if (xd == NULL || surface == NULL) {
        return false;
    }
    memset(&attributes, 0, sizeof attributes);
    if (hero_ui_gtk.x_get_window_attributes(xd, hero_ui_gtk.x11_surface_get_xid(surface), &attributes) == 0) {
        return false;
    }
    memcpy(&state, attributes.bytes + HERO_X_MAP_STATE_AT, sizeof state);
    return state == HERO_X_IS_VIEWABLE;
}

/* The state window `window` stands in, as GDK last read it. */
static int64_t hero_ui_gtk_state_of(const hero_ui_native *window) {
    void *surface = hero_ui_gtk_surface(window);
    unsigned flags;

    if (surface == NULL) {
        return window->asked_state != 0 ? window->asked_state : HERO_UI_STATE_NORMAL;
    }
    flags = hero_ui_gtk.toplevel_get_state(surface);
    if ((flags & HERO_GDK_TOPLEVEL_STATE_MINIMIZED) != 0) {
        return HERO_UI_STATE_MINIMIZED;
    }
    if ((flags & HERO_GDK_TOPLEVEL_STATE_FULLSCREEN) != 0) {
        return HERO_UI_STATE_FULL_SCREEN;
    }
    if ((flags & HERO_GDK_TOPLEVEL_STATE_MAXIMIZED) != 0) {
        return HERO_UI_STATE_MAXIMIZED;
    }
    return HERO_UI_STATE_NORMAL;
}

/* Window `window`'s frame in the desktop's pixels and the scale of its
 * surface; false where it has no X window. */
static bool hero_ui_gtk_x_frame(const hero_ui_native *window, hero_ui_gtk_rect *frame, int *scale) {
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    unsigned long xid;
    unsigned long child = 0;
    int x = 0;
    int y = 0;
    long managed[4] = {0, 0, 0, 0};
    long shadow[4] = {0, 0, 0, 0};

    if (xd == NULL || surface == NULL) {
        return false;
    }
    xid = hero_ui_gtk.x11_surface_get_xid(surface);
    if (!hero_ui_gtk.x_translate_coordinates(xd, xid, hero_ui_gtk.x11_display_get_xrootwindow(hero_ui_gtk.display_get_default()), 0, 0, &x, &y, &child)) {
        return false;
    }
    *scale = hero_ui_gtk.surface_get_scale_factor(surface);
    if (*scale < 1) {
        *scale = 1;
    }
    (void)hero_ui_gtk_x_numbers(xd, xid, "_NET_FRAME_EXTENTS", HERO_XA_CARDINAL, managed, 4);
    (void)hero_ui_gtk_x_numbers(xd, xid, "_GTK_FRAME_EXTENTS", HERO_XA_CARDINAL, shadow, 4);
    frame->x = (int64_t)x + shadow[0] - managed[0];
    frame->y = (int64_t)y + shadow[2] - managed[2];
    frame->width = (int64_t)hero_ui_gtk.surface_get_width(surface) * *scale - shadow[0] - shadow[1] + managed[0] + managed[1];
    frame->height = (int64_t)hero_ui_gtk.surface_get_height(surface) * *scale - shadow[2] - shadow[3] + managed[2] + managed[3];
    return true;
}

/* Window `window`'s frame in the desktop's points, the screen it stands on
 * most and its place there; an estimate from its content where it has no X
 * window yet, at the place the program asked. */
static void hero_ui_gtk_frame_points(const hero_ui_native *window, const hero_ui_gtk_screen *screens, int64_t count, hero_ui_gtk_rect *frame, int64_t *screen) {
    int scale = 1;

    if (hero_ui_gtk_x_frame(window, frame, &scale)) {
        frame->x /= scale;
        frame->y /= scale;
        frame->width /= scale;
        frame->height /= scale;
    } else {
        frame->x = 0;
        frame->y = 0;
        frame->width = window->content_width;
        frame->height = window->content_height;
    }
    if (window->want_placed && window->want_screen < count) {
        frame->x = screens[window->want_screen].area.x + window->want_x;
        frame->y = screens[window->want_screen].area.y + window->want_y;
    }
    *screen = hero_ui_gtk_screen_under(screens, count, *frame);
}

/* What `hero_ui_gtk_moved` waits for. */
typedef struct hero_ui_gtk_move {
    hero_ui_native *window;
    hero_ui_gtk_rect before;
    int64_t x;
    int64_t y;
} hero_ui_gtk_move;

/* Whether the frame of the move `data` waits for left where it stood. */
static bool hero_ui_gtk_moved(void *data) {
    hero_ui_gtk_move *move = data;
    hero_ui_gtk_rect now;
    int scale = 1;

    return hero_ui_gtk_x_frame(move->window, &now, &scale) && (now.x != move->before.x || now.y != move->before.y || (now.x == move->x && now.y == move->y));
}

/* Moves window `window`'s frame to `x`, `y` in the desktop's points, as
 * this part's words say; the person's move told where `person`, else the
 * window's moves kept quiet until it stands there. */
static int64_t hero_ui_gtk_move_to(hero_ui_native *window, int64_t x, int64_t y, bool person) {
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    hero_ui_gtk_move move = {window, {0, 0, 0, 0}, 0, 0};
    int64_t ask_x;
    int64_t ask_y;
    int scale = 1;
    bool there = false;

    if (xd == NULL || surface == NULL || !hero_ui_gtk_x_frame(window, &move.before, &scale)) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "the window has no X window to move");
    }
    move.x = x * scale;
    move.y = y * scale;
    window->quiet = !person;
    if (!window->move_known) {
        int client_x = 0;
        int client_y = 0;
        unsigned long child = 0;

        /* A window manager reads a move by the window's gravity, which
         * ICCCM puts at the frame's corner; with none, the X window itself
         * stands where it is moved. */
        (void)hero_ui_gtk.x_translate_coordinates(xd, hero_ui_gtk.x11_surface_get_xid(surface), hero_ui_gtk.x11_display_get_xrootwindow(hero_ui_gtk.display_get_default()), 0, 0, &client_x, &client_y, &child);
        window->move_dx = hero_ui_gtk_manager_runs(xd) ? 0 : client_x - move.before.x;
        window->move_dy = hero_ui_gtk_manager_runs(xd) ? 0 : client_y - move.before.y;
    }
    ask_x = move.x + window->move_dx;
    ask_y = move.y + window->move_dy;
    for (int attempt = 0; attempt < 3 && !there; attempt += 1) {
        hero_ui_gtk_rect now;

        if (move.before.x == move.x && move.before.y == move.y) {
            there = true;
            break;
        }
        (void)hero_ui_gtk.x_move_window(xd, hero_ui_gtk.x11_surface_get_xid(surface), (int)hero_ui_between(ask_x, INT32_MIN / 2, INT32_MAX / 2), (int)hero_ui_between(ask_y, INT32_MIN / 2, INT32_MAX / 2));
        (void)hero_ui_gtk.x_sync(xd, 0);
        (void)hero_ui_gtk_spin(1000, hero_ui_gtk_moved, &move);
        if (!hero_ui_gtk_x_frame(window, &now, &scale)) {
            break;
        }
        there = now.x == move.x && now.y == move.y;
        /* The window manager put the frame off by what it adds: asked
         * again, corrected by that much, and the correction kept for the
         * window's next move. */
        ask_x -= now.x - move.x;
        ask_y -= now.y - move.y;
        window->move_dx = ask_x - move.x;
        window->move_dy = ask_y - move.y;
        window->move_known = true;
        move.before = now;
    }
    if (!person) {
        hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
        int64_t count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);
        hero_ui_gtk_rect frame;
        int64_t screen = 0;

        /* What the program set is no move of the person's. */
        (void)hero_ui_gtk_spin(0, NULL, NULL);
        hero_ui_gtk_frame_points(window, screens, count, &frame, &screen);
        window->told_screen = screen;
        window->told_x = frame.x - (count > 0 ? screens[screen].area.x : 0);
        window->told_y = frame.y - (count > 0 ? screens[screen].area.y : 0);
        window->placed_known = true;
        window->quiet = false;
    }
    return there ? HERO_UI_OK : hero_ui_gtk_said_so(HERO_UI_OTHER, "the window manager did not put the window where it was asked within three seconds");
}

/* Asks the window manager to keep window `window` above every other, or
 * not, by EWMH's `_NET_WM_STATE` client message. */
static void hero_ui_gtk_send_above(hero_ui_native *window) {
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    hero_ui_x_event event;

    if (xd == NULL || surface == NULL) {
        return;
    }
    memset(&event, 0, sizeof event);
    event.client.type = HERO_X_CLIENT_MESSAGE;
    event.client.send_event = 1;
    event.client.window = hero_ui_gtk.x11_surface_get_xid(surface);
    event.client.message_type = hero_ui_gtk_atom("_NET_WM_STATE");
    event.client.format = 32;
    event.client.data[0] = window->on_top ? 1 : 0;
    event.client.data[1] = (long)hero_ui_gtk_atom("_NET_WM_STATE_ABOVE");
    event.client.data[2] = 0;
    /* 1: the request is an application's. */
    event.client.data[3] = 1;
    (void)hero_ui_gtk.x_send_event(xd, hero_ui_gtk.x11_display_get_xrootwindow(hero_ui_gtk.display_get_default()), 0, HERO_X_SUBSTRUCTURE_MASKS, &event);
    (void)hero_ui_gtk.x_sync(xd, 0);
}

/* Waits up to two seconds for window `window`, shown, to be viewable, and
 * gives it the place the program asked while it was not. */
static void hero_ui_gtk_settle(hero_ui_native *window) {
    if (!window->visible || hero_ui_gtk_xdisplay() == NULL || hero_ui_gtk_state_of(window) == HERO_UI_STATE_MINIMIZED) {
        return;
    }
    (void)hero_ui_gtk_spin(2000, hero_ui_gtk_viewable, window);
    if (window->want_placed && hero_ui_gtk_viewable(window)) {
        hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
        int64_t count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);

        window->want_placed = false;
        if (window->want_screen < count) {
            (void)hero_ui_gtk_move_to(window, screens[window->want_screen].area.x + window->want_x, screens[window->want_screen].area.y + window->want_y, false);
        }
    }
}

/* Gives window `data`, mapped a moment ago, the place and the stacking the
 * program asked while it was not shown. */
static int hero_ui_gtk_after_map(void *data) {
    hero_ui_native *window = hero_ui_gtk_window((int64_t)(intptr_t)data);

    if (window != NULL) {
        hero_ui_gtk_settle(window);
        if (window->on_top) {
            hero_ui_gtk_send_above(window);
        }
        if (hero_ui_gtk_undocked) {
            hero_ui_gtk_send_docked(window);
        }
    }
    return 0;
}

/* Gives window `window`'s surface the app's icon, or none. */
static void hero_ui_gtk_give_icon(const hero_ui_native *window) {
    void *surface = hero_ui_gtk_surface(window);
    void *icons;

    if (surface == NULL) {
        return;
    }
    icons = hero_ui_gtk_icon != NULL ? hero_ui_gtk.list_append(NULL, hero_ui_gtk_icon) : NULL;
    hero_ui_gtk.toplevel_set_icon_list(surface, icons);
    if (icons != NULL) {
        hero_ui_gtk.list_free(icons);
    }
}

static void hero_ui_gtk_realized(void *widget, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, widget);

    if (window != NULL && window->kind == HERO_UI_WINDOW && hero_ui_gtk_icon != NULL) {
        hero_ui_gtk_give_icon(window);
    }
}

static void hero_ui_gtk_mapped(void *widget, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, widget);

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return;
    }
    if (!window->focus_given) {
        window->focus_given = true;
        hero_ui_gtk_first_focus(window);
    }
    if (window->want_placed || window->on_top || hero_ui_gtk_undocked) {
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_after_map, data);
    }
}

/* Checks each watched window's screen and place against what was told of
 * them, from an idle source after the X server moved a window, and tells
 * what the person changed: its screen before its place. */
static int hero_ui_gtk_check_places(void *data) {
    hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
    int64_t count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);

    (void)data;
    hero_ui_gtk_places_due = false;
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *window = hero_ui_at(at);
        hero_ui_gtk_rect frame;
        int64_t screen = 0;
        int64_t x;
        int64_t y;

        if (window == NULL || window->kind != HERO_UI_WINDOW || !window->watched || window->quiet || !hero_ui_gtk_viewable(window) || count == 0) {
            continue;
        }
        hero_ui_gtk_frame_points(window, screens, count, &frame, &screen);
        x = frame.x - screens[screen].area.x;
        y = frame.y - screens[screen].area.y;
        if (window->placed_known && screen != window->told_screen) {
            hero_ui_tell(HERO_UI_EVENT_SCREEN, window->id, screen, 0);
        }
        if (window->placed_known && (screen != window->told_screen || x != window->told_x || y != window->told_y)) {
            hero_ui_tell(HERO_UI_EVENT_MOVE, window->id, x, y);
        }
        window->told_screen = screen;
        window->told_x = x;
        window->told_y = y;
        window->placed_known = true;
    }
    return 0;
}

/* An X event GDK read: a window configured queues a check of the places. */
static int hero_ui_gtk_x_event(void *display, void *event, void *data) {
    int type;

    (void)display;
    (void)data;
    memcpy(&type, event, sizeof type);
    if (type == HERO_X_CONFIGURE_NOTIFY && !hero_ui_gtk_places_due) {
        hero_ui_gtk_places_due = true;
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_check_places, NULL);
    }
    return 0;
}

/* Window `data`'s surface changed its state: told where the person changed
 * it, never while the program's own call brings it there. */
static void hero_ui_gtk_restated(void *surface, void *spec, void *data) {
    hero_ui_native *window = hero_ui_gtk_window((int64_t)(intptr_t)data);
    int64_t state;

    (void)spec;
    if (window == NULL || !window->watched || hero_ui_gtk_surface(window) != surface) {
        return;
    }
    state = hero_ui_gtk_state_of(window);
    if (state == window->told_state) {
        return;
    }
    window->told_state = state;
    if (!window->quiet) {
        hero_ui_tell(HERO_UI_EVENT_STATE, window->id, state, 0);
    }
}

/* Window `data`'s surface entered monitor `monitor`, away from X11: its new
 * screen told. */
static void hero_ui_gtk_entered(void *surface, void *monitor, void *data) {
    hero_ui_native *window = hero_ui_gtk_window((int64_t)(intptr_t)data);
    hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
    int64_t count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);

    if (window == NULL || !window->watched || hero_ui_gtk_surface(window) != surface) {
        return;
    }
    for (int64_t i = 0; i < count; i += 1) {
        if (screens[i].monitor == monitor && i != window->told_screen) {
            window->told_screen = i;
            hero_ui_tell(HERO_UI_EVENT_SCREEN, window->id, i, 0);
        }
    }
}

int64_t hero_ui_arm_watch(int64_t id) {
    hero_ui_native *window = hero_ui_gtk_window(id);
    void *surface;

    if (window == NULL || window->watched) {
        return HERO_UI_OK;
    }
    surface = hero_ui_gtk_surface(window);
    if (surface == NULL) {
        return HERO_UI_OK;
    }
    window->watched = true;
    window->told_state = hero_ui_gtk_state_of(window);
    hero_ui_gtk_connect(surface, "notify::state", (hero_ui_gtk_callback)hero_ui_gtk_restated, id);
    if (hero_ui_gtk_xdisplay() != NULL) {
        if (!hero_ui_gtk_xevents) {
            (void)hero_ui_gtk.signal_connect_data(hero_ui_gtk.display_get_default(), "xevent", (hero_ui_gtk_callback)hero_ui_gtk_x_event, NULL, NULL, 0);
            hero_ui_gtk_xevents = true;
        }
    } else {
        hero_ui_gtk_connect(surface, "enter-monitor", (hero_ui_gtk_callback)hero_ui_gtk_entered, id);
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_screens(int64_t *values, int64_t count, int64_t *screens) {
    hero_ui_gtk_screen found[HERO_UI_GTK_SCREENS_MOST];
    int64_t made = hero_ui_gtk_screens_now(found, HERO_UI_GTK_SCREENS_MOST);

    *screens = made;
    if (made == 0) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "GDK lists no monitor on this display");
    }
    if (made * HERO_UI_SCREEN_FIELDS > count) {
        return HERO_UI_OK;
    }
    for (int64_t i = 0; i < made; i += 1) {
        int64_t *at = &values[i * HERO_UI_SCREEN_FIELDS];

        at[HERO_UI_SCREEN_LEFT] = found[i].area.x;
        at[HERO_UI_SCREEN_TOP] = found[i].area.y;
        at[HERO_UI_SCREEN_WIDTH] = found[i].area.width;
        at[HERO_UI_SCREEN_HEIGHT] = found[i].area.height;
        at[HERO_UI_SCREEN_WORK_LEFT] = found[i].work.x;
        at[HERO_UI_SCREEN_WORK_TOP] = found[i].work.y;
        at[HERO_UI_SCREEN_WORK_WIDTH] = found[i].work.width;
        at[HERO_UI_SCREEN_WORK_HEIGHT] = found[i].work.height;
        at[HERO_UI_SCREEN_SCALE] = found[i].scale;
    }
    return HERO_UI_OK;
}

/* Window `id` of the native side, or NULL with `*kind` the answer. */
static hero_ui_native *hero_ui_gtk_own_window(int64_t id, int64_t *kind) {
    hero_ui_native *found = hero_ui_find(id);

    if (found == NULL) {
        *kind = HERO_UI_NOT_FOUND;
        return NULL;
    }
    if (found->kind != HERO_UI_WINDOW) {
        *kind = HERO_UI_INVALID;
        return NULL;
    }
    return found;
}

int64_t hero_ui_arm_frame_of(int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height) {
    int64_t kind = HERO_UI_OK;
    hero_ui_native *window = hero_ui_gtk_own_window(id, &kind);
    hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
    int64_t count;
    hero_ui_gtk_rect frame;

    if (window == NULL) {
        return kind;
    }
    if (hero_ui_gtk_xdisplay() == NULL) {
        return hero_ui_gtk_not_x11(hero_ui_gtk_wayland_place);
    }
    hero_ui_gtk_settle(window);
    count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);
    if (count == 0) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "GDK lists no monitor on this display");
    }
    hero_ui_gtk_frame_points(window, screens, count, &frame, screen);
    *x = frame.x - screens[*screen].area.x;
    *y = frame.y - screens[*screen].area.y;
    *width = frame.width;
    *height = frame.height;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_position(int64_t id, int64_t screen, int64_t x, int64_t y, int64_t person) {
    int64_t kind = HERO_UI_OK;
    hero_ui_native *window = hero_ui_gtk_own_window(id, &kind);
    hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
    int64_t count;

    if (window == NULL) {
        return kind;
    }
    if (hero_ui_gtk_xdisplay() == NULL) {
        return hero_ui_gtk_not_x11(hero_ui_gtk_wayland_place);
    }
    count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);
    if (screen >= count) {
        return HERO_UI_INVALID;
    }
    hero_ui_gtk_settle(window);
    if (!window->visible || !hero_ui_gtk_viewable(window)) {
        /* Given once the window is mapped (`hero_ui_gtk_after_map`). */
        window->want_placed = true;
        window->want_screen = screen;
        window->want_x = x;
        window->want_y = y;
        return HERO_UI_OK;
    }
    window->want_placed = false;
    return hero_ui_gtk_move_to(window, screens[screen].area.x + x, screens[screen].area.y + y, person == 1);
}

int64_t hero_ui_arm_center(int64_t id, int64_t over) {
    int64_t kind = HERO_UI_OK;
    hero_ui_native *window = hero_ui_gtk_own_window(id, &kind);
    hero_ui_native *anchor = NULL;
    hero_ui_gtk_screen screens[HERO_UI_GTK_SCREENS_MOST];
    int64_t count;
    hero_ui_gtk_rect frame;
    hero_ui_gtk_rect around;
    hero_ui_gtk_rect work;
    int64_t screen = 0;
    int64_t x;
    int64_t y;

    if (window == NULL) {
        return kind;
    }
    if (over != 0) {
        anchor = hero_ui_gtk_own_window(over, &kind);
        if (anchor == NULL) {
            return kind;
        }
    }
    if (hero_ui_gtk_xdisplay() == NULL) {
        return hero_ui_gtk_not_x11(hero_ui_gtk_wayland_place);
    }
    hero_ui_gtk_settle(window);
    count = hero_ui_gtk_screens_now(screens, HERO_UI_GTK_SCREENS_MOST);
    if (count == 0) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "GDK lists no monitor on this display");
    }
    hero_ui_gtk_frame_points(window, screens, count, &frame, &screen);
    if (anchor != NULL) {
        hero_ui_gtk_settle(anchor);
        hero_ui_gtk_frame_points(anchor, screens, count, &around, &screen);
    } else {
        around = screens[screen].work;
    }
    work = screens[screen].work;
    x = around.x + (around.width - frame.width) / 2;
    y = around.y + (around.height - frame.height) / 2;
    /* Within the work area as far as the window's size allows. */
    x = x + frame.width > work.x + work.width ? work.x + work.width - frame.width : x;
    y = y + frame.height > work.y + work.height ? work.y + work.height - frame.height : y;
    x = x < work.x ? work.x : x;
    y = y < work.y ? work.y : y;
    if (!window->visible || !hero_ui_gtk_viewable(window)) {
        window->want_placed = true;
        window->want_screen = screen;
        window->want_x = x - screens[screen].area.x;
        window->want_y = y - screens[screen].area.y;
        return HERO_UI_OK;
    }
    return hero_ui_gtk_move_to(window, x, y, false);
}

/* Waits, up to a second, for window `window`'s frame to read the same
 * twice 50 ms apart. */
static void hero_ui_gtk_still(const hero_ui_native *window) {
    hero_ui_gtk_rect before = {0, 0, 0, 0};
    int scale = 1;

    (void)hero_ui_gtk_x_frame(window, &before, &scale);
    for (int turn = 0; turn < 20; turn += 1) {
        hero_ui_gtk_rect now = {0, 0, 0, 0};

        (void)hero_ui_gtk_spin(50, NULL, NULL);
        (void)hero_ui_gtk_x_frame(window, &now, &scale);
        if (now.x == before.x && now.y == before.y && now.width == before.width && now.height == before.height) {
            return;
        }
        before = now;
    }
}

/* What `hero_ui_gtk_reached` waits for. */
typedef struct hero_ui_gtk_goal {
    hero_ui_native *window;
    int64_t state;
} hero_ui_gtk_goal;

static bool hero_ui_gtk_reached(void *data) {
    hero_ui_gtk_goal *goal = data;

    return hero_ui_gtk_state_of(goal->window) == goal->state;
}

int64_t hero_ui_arm_set_state(int64_t id, int64_t state, int64_t person) {
    int64_t kind = HERO_UI_OK;
    hero_ui_native *window = hero_ui_gtk_own_window(id, &kind);
    void *xd;
    int64_t now;
    hero_ui_gtk_goal goal;
    bool reached;

    if (window == NULL) {
        return kind;
    }
    xd = hero_ui_gtk_xdisplay();
    if (xd != NULL && !hero_ui_gtk_manager_runs(xd)) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, hero_ui_gtk_no_manager);
    }
    hero_ui_gtk_settle(window);
    now = hero_ui_gtk_state_of(window);
    window->asked_state = state;
    if (now == state) {
        return HERO_UI_OK;
    }
    window->quiet = person != 1;
    if (now == HERO_UI_STATE_MINIMIZED) {
        hero_ui_gtk.window_unminimize(window->widget);
    }
    if (now == HERO_UI_STATE_FULL_SCREEN && state != HERO_UI_STATE_MINIMIZED) {
        hero_ui_gtk.window_unfullscreen(window->widget);
    }
    if (now == HERO_UI_STATE_MAXIMIZED && state != HERO_UI_STATE_MINIMIZED) {
        hero_ui_gtk.window_unmaximize(window->widget);
    }
    if (state == HERO_UI_STATE_MINIMIZED) {
        hero_ui_gtk.window_minimize(window->widget);
    } else if (state == HERO_UI_STATE_MAXIMIZED) {
        hero_ui_gtk.window_maximize(window->widget);
    } else if (state == HERO_UI_STATE_FULL_SCREEN) {
        hero_ui_gtk.window_fullscreen(window->widget);
    }
    if (xd == NULL && state == HERO_UI_STATE_MINIMIZED) {
        /* Wayland tells no program its window was minimized. */
        window->quiet = false;
        return HERO_UI_OK;
    }
    goal.window = window;
    goal.state = state;
    reached = hero_ui_gtk_spin(5000, hero_ui_gtk_reached, &goal);
    if (reached && state != HERO_UI_STATE_MINIMIZED) {
        /* The window manager names the state before it gives the window
         * its new size: waited for until its frame stands still. */
        hero_ui_gtk_still(window);
    }
    if (window->quiet) {
        window->told_state = hero_ui_gtk_state_of(window);
        window->quiet = false;
    }
    return reached ? HERO_UI_OK : hero_ui_gtk_said_so(HERO_UI_OTHER, "the window manager did not bring the window to that state within five seconds");
}

int64_t hero_ui_arm_state(int64_t id, int64_t *state) {
    int64_t kind = HERO_UI_OK;
    hero_ui_native *window = hero_ui_gtk_own_window(id, &kind);

    if (window == NULL) {
        return kind;
    }
    *state = hero_ui_gtk_state_of(window);
    if (hero_ui_gtk_xdisplay() == NULL && *state == HERO_UI_STATE_NORMAL && window->asked_state == HERO_UI_STATE_MINIMIZED) {
        return hero_ui_gtk_not_x11(hero_ui_gtk_wayland_minimized);
    }
    return HERO_UI_OK;
}

/* Whether window `data`'s frame extents read as framed, `true`, or bare. */
static bool hero_ui_gtk_framed_now(void *data) {
    hero_ui_native *window = data;
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    long managed[4] = {0, 0, 0, 0};
    bool framed;

    if (xd == NULL || surface == NULL) {
        return true;
    }
    (void)hero_ui_gtk_x_numbers(xd, hero_ui_gtk.x11_surface_get_xid(surface), "_NET_FRAME_EXTENTS", HERO_XA_CARDINAL, managed, 4);
    framed = managed[0] + managed[1] + managed[2] + managed[3] > 0;
    return framed == window->want_framed;
}

/* Whether window `data`'s surface reads as kept above, as asked. */
static bool hero_ui_gtk_above_now(void *data) {
    hero_ui_native *window = data;
    void *surface = hero_ui_gtk_surface(window);

    return surface != NULL && ((hero_ui_gtk.toplevel_get_state(surface) & HERO_GDK_TOPLEVEL_STATE_ABOVE) != 0) == window->on_top;
}

int64_t hero_ui_arm_window_set(int64_t id, int64_t which, int64_t value) {
    int64_t kind = HERO_UI_OK;
    hero_ui_native *window = hero_ui_gtk_own_window(id, &kind);
    void *xd = hero_ui_gtk_xdisplay();

    if (window == NULL) {
        return kind;
    }
    switch (which) {
    case HERO_UI_WINDOW_SET_ON_TOP:
        if (xd == NULL) {
            return hero_ui_gtk_not_x11(hero_ui_gtk_wayland_top);
        }
        if (!hero_ui_gtk_manager_runs(xd)) {
            return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, hero_ui_gtk_no_manager);
        }
        if (!hero_ui_gtk_manager_offers(xd, "_NET_WM_STATE_ABOVE")) {
            return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "the window manager running here does not offer _NET_WM_STATE_ABOVE, which keeps a window above every other");
        }
        window->on_top = value == 1;
        hero_ui_gtk_settle(window);
        if (hero_ui_gtk_viewable(window)) {
            hero_ui_gtk_send_above(window);
            (void)hero_ui_gtk_spin(1000, hero_ui_gtk_above_now, window);
        }
        return HERO_UI_OK;
    case HERO_UI_WINDOW_SET_FRAMED:
        if (hero_ui_gtk_state_of(window) == HERO_UI_STATE_FULL_SCREEN) {
            return hero_ui_gtk_said_so(HERO_UI_OTHER, "a window in full screen has no frame to give or take");
        }
        hero_ui_gtk.window_set_decorated(window->widget, value == 1 ? 1 : 0);
        if (xd != NULL && hero_ui_gtk_manager_runs(xd) && hero_ui_gtk_viewable(window)) {
            /* The window manager draws the frame: waited for, so a frame
             * read next reads it as it now stands. */
            window->want_framed = value == 1;
            (void)hero_ui_gtk_spin(1000, hero_ui_gtk_framed_now, window);
        }
        return HERO_UI_OK;
    case HERO_UI_WINDOW_SET_OPACITY:
        hero_ui_gtk.widget_set_opacity(window->widget, (double)value / 255.0);
        return HERO_UI_OK;
    case HERO_UI_WINDOW_SET_BACKDROP:
        if (value == HERO_UI_BACKDROP_NONE) {
            return HERO_UI_OK;
        }
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "GTK 4 offers no translucent material behind a window: a see-through background drawn by CSS is not the system's blur");
    default:
        if (value == 1) {
            hero_ui_native *other = NULL;
            char words[HERO_UI_WORDS_MOST];

            if (hero_ui_gtk_modal != 0 && hero_ui_gtk_modal != id) {
                (void)snprintf(words, sizeof words, "window %lld is modal already, and one window is modal at a time", (long long)hero_ui_gtk_modal);
                return hero_ui_gtk_said_so(HERO_UI_OTHER, words);
            }
            for (size_t at = 0; at < hero_ui_map_room() && other == NULL; at += 1) {
                hero_ui_native *each = hero_ui_at(at);

                other = each != NULL && each->kind == HERO_UI_WINDOW && each->id != id && each->visible ? each : NULL;
            }
            hero_ui_gtk.window_set_transient_for(window->widget, other != NULL ? other->widget : NULL);
            hero_ui_gtk.window_set_modal(window->widget, 1);
            if (window->visible) {
                hero_ui_gtk.window_present(window->widget);
            }
            hero_ui_gtk_modal = id;
        } else if (hero_ui_gtk_modal == id) {
            hero_ui_gtk.window_set_modal(window->widget, 0);
            hero_ui_gtk.window_set_transient_for(window->widget, NULL);
            hero_ui_gtk_modal = 0;
        }
        return HERO_UI_OK;
    }
}

int64_t hero_ui_arm_set_icon(const uint8_t *rgba, int64_t width, int64_t height) {
    void *texture = NULL;

    if (width > 0 && height > 0) {
        void *bytes = hero_ui_gtk.bytes_new(rgba, (size_t)(width * height * 4));

        texture = bytes != NULL && width <= INT32_MAX / 4 && height <= INT32_MAX ? hero_ui_gtk.memory_texture_new((int)width, (int)height, HERO_GDK_MEMORY_R8G8B8A8, bytes, (size_t)width * 4) : NULL;
        if (bytes != NULL) {
            hero_ui_gtk.bytes_unref(bytes);
        }
        if (texture == NULL) {
            return HERO_UI_LIMIT;
        }
    }
    if (hero_ui_gtk_icon != NULL) {
        hero_ui_gtk.object_unref(hero_ui_gtk_icon);
    }
    hero_ui_gtk_icon = texture;
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *each = hero_ui_at(at);

        if (each != NULL && each->kind == HERO_UI_WINDOW) {
            hero_ui_gtk_give_icon(each);
        }
    }
    return HERO_UI_OK;
}

/* GTK's own loop serves while a window is modal: its modality is the grab
 * GTK holds, not a loop of its own. */
bool hero_ui_arm_modal_here(void) {
    return false;
}

int64_t hero_ui_arm_modal_pump(int64_t ms) {
    return hero_ui_arm_pump(ms);
}

/* -- A KEY PRESSED --------------------------------------------------------- */

/* Whether the X server gives window `data` the keys: its focus is the
 * window's X window or one inside it (GDK's focus window, where a window
 * manager gives the focus). GTK's `is-active` is no such answer: it reads
 * true before the X server mapped the window (measured under Xvfb, GTK
 * 4.18.6), and a key pressed then is lost. */
static bool hero_ui_gtk_keyed_window(void *data) {
    const hero_ui_native *window = data;
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    unsigned long xid;
    unsigned long at = 0;
    int revert = 0;

    if (xd == NULL || surface == NULL || hero_ui_gtk.x_get_input_focus == NULL || hero_ui_gtk.x_query_tree == NULL || !hero_ui_gtk.window_is_active(window->widget)) {
        return false;
    }
    xid = hero_ui_gtk.x11_surface_get_xid(surface);
    (void)hero_ui_gtk.x_get_input_focus(xd, &at, &revert);
    for (int depth = 0; at > 1 && depth < 16; depth += 1) {
        unsigned long root = 0;
        unsigned long parent = 0;
        unsigned long *children = NULL;
        unsigned count = 0;

        if (at == xid) {
            return true;
        }
        if (!hero_ui_gtk.x_query_tree(xd, at, &root, &parent, &children, &count)) {
            return false;
        }
        if (children != NULL) {
            (void)hero_ui_gtk.x_free(children);
        }
        at = parent == root ? 0 : parent;
    }
    return false;
}

int64_t hero_ui_arm_press_key(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;
    unsigned keyval = 0;
    unsigned held[3];
    int holding = 0;
    unsigned code;
    void *xd;
    int event = 0;
    int error = 0;
    int major = 0;
    int minor = 0;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_gtk_window(widget->window);
    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if ((widget != window && !hero_ui_gtk_takes_keys(widget->kind)) || !hero_ui_gtk_key_of(key, count, &keyval)) {
        return HERO_UI_INVALID;
    }
    xd = hero_ui_gtk_xdisplay();
    if (xd == NULL) {
        return hero_ui_gtk_not_x11(hero_ui_gtk_wayland_key);
    }
    if (hero_ui_gtk.x_test_query_extension == NULL || hero_ui_gtk.x_test_fake_key_event == NULL || hero_ui_gtk.x_keysym_to_keycode == NULL) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, HERO_UI_GTK_XTEST_LIBRARY " is not installed: X11 lets a program press a key as the keyboard does only through its XTest extension");
    }
    if (!hero_ui_gtk.x_test_query_extension(xd, &event, &error, &major, &minor)) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "this X server offers no XTEST extension, through which alone X11 lets a program press a key");
    }
    if (!window->visible) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "the window is hidden, and a key reaches a window shown alone");
    }
    hero_ui_gtk_settle(window);
    hero_ui_gtk.window_present(window->widget);
    if (!hero_ui_gtk_manager_runs(xd) && hero_ui_gtk.x_set_input_focus != NULL && hero_ui_gtk_viewable(window)) {
        /* With no window manager the X server's focus follows the pointer
         * until a client sets it: given to the window, as GDK's own
         * present gives it where the window was viewable. */
        (void)hero_ui_gtk.x_set_input_focus(xd, hero_ui_gtk.x11_surface_get_xid(hero_ui_gtk_surface(window)), HERO_X_REVERT_TO_PARENT, 0);
        (void)hero_ui_gtk.x_sync(xd, 0);
    }
    if (!hero_ui_gtk_spin(2000, hero_ui_gtk_keyed_window, window)) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "the window could not be given the keys within two seconds: the window manager keeps another in front");
    }
    if (widget != window && hero_ui_gtk_focused(window) != widget) {
        /* Given the keys as the person's click gives them: a field's text
         * is not chosen whole, as GTK's own focus by Tab chooses it. */
        void *text = widget->kind == HERO_UI_TEXT_FIELD ? hero_ui_gtk.editable_get_delegate(widget->widget) : NULL;

        if (!widget->visible || !hero_ui_gtk.widget_get_visible(widget->widget) || !hero_ui_gtk.widget_is_sensitive(widget->widget)) {
            return HERO_UI_INVALID;
        }
        if (text != NULL ? !hero_ui_gtk.text_grab_focus_without_selecting(text) : hero_ui_arm_set_focus(id) != HERO_UI_OK) {
            return HERO_UI_INVALID;
        }
    }
    code = hero_ui_gtk.x_keysym_to_keycode(xd, keyval);
    if (code == 0) {
        return hero_ui_gtk_said_so(HERO_UI_OTHER, "this keyboard's layout has no key for it");
    }
    if ((modifiers & HERO_UI_PRIMARY) != 0) {
        held[holding++] = hero_ui_gtk.x_keysym_to_keycode(xd, HERO_GDK_KEY_CONTROL_L);
    }
    if ((modifiers & HERO_UI_SHIFT) != 0) {
        held[holding++] = hero_ui_gtk.x_keysym_to_keycode(xd, HERO_GDK_KEY_SHIFT_L);
    }
    if ((modifiers & HERO_UI_ALT) != 0) {
        held[holding++] = hero_ui_gtk.x_keysym_to_keycode(xd, HERO_GDK_KEY_ALT_L);
    }
    for (int i = 0; i < holding; i += 1) {
        (void)hero_ui_gtk.x_test_fake_key_event(xd, held[i], 1, 0);
    }
    (void)hero_ui_gtk.x_test_fake_key_event(xd, code, 1, 0);
    (void)hero_ui_gtk.x_test_fake_key_event(xd, code, 0, 0);
    for (int i = holding - 1; i >= 0; i -= 1) {
        (void)hero_ui_gtk.x_test_fake_key_event(xd, held[i], 0, 0);
    }
    /* The server has made the key's events once it answers; GTK then reads
     * and handles them in its loop's turns. */
    (void)hero_ui_gtk.x_sync(xd, 0);
    (void)hero_ui_gtk_spin(30, NULL, NULL);
    return HERO_UI_OK;
}

/* -- DIALOGS ANSWERED THROUGH THE QUEUE ----------------------------------------
 *
 * Every dialog of GTK 4 answers later by itself: GTK 4.10's `GtkAlertDialog`
 * and `GtkFileDialog` by a callback, GTK 4.0's `GtkMessageDialog` and
 * `GtkFileChooserNative` by their `response`. So each `show_*` shows its
 * dialog over its window and returns at once, and the callback queues the
 * person's answer (`hero_ui_tell_text`), the paths in its text. The arm keeps
 * each dialog shown, with a `GCancellable` of its own, until its callback
 * ran; one ended (its window going, or the app) is cancelled and marked so
 * its callback queues nothing. An answer given as the person's presses the
 * button of that label in the dialog's own window, which the arm found as
 * the toplevel the dialog added; a file or folder dialog is only cancelled
 * so, no program choosing in it. */

#define HERO_UI_GTK_DIALOG_MESSAGE 1
#define HERO_UI_GTK_DIALOG_ASK 2
#define HERO_UI_GTK_DIALOG_FILES 3
#define HERO_UI_GTK_DIALOG_FOLDERS 4

/* How a file or folder dialog chooses, which its callback finishes. */
#define HERO_UI_GTK_CHOOSE_OPEN 0
#define HERO_UI_GTK_CHOOSE_OPEN_MANY 1
#define HERO_UI_GTK_CHOOSE_SAVE 2
#define HERO_UI_GTK_CHOOSE_FOLDER 3
#define HERO_UI_GTK_CHOOSE_FOLDERS 4

/* A dialog shown and not yet answered. */
typedef struct hero_ui_gtk_dialog {
    int64_t dialog;
    int64_t window;
    int kind;
    int choose;
    /* A message box's and an ask's buttons in the order shown, and what each
     * answers; an ask's cancel button. */
    char **labels;
    int64_t answers[3];
    int64_t shown;
    int64_t cancel;
    void *cancellable;
    /* GTK 4.0's dialog object the arm holds (a `GtkMessageDialog`'s window,
     * a `GtkFileChooserNative`), and an alert's window, found as shown. */
    void *object;
    void *window_shown;
    bool ended;
    struct hero_ui_gtk_dialog *next;
} hero_ui_gtk_dialog;

/* The dialogs shown, in the order shown. */
static _Thread_local hero_ui_gtk_dialog *hero_ui_gtk_dialogs;

/* A dialog `dialog` over window `window`, kept at the end of the dialogs
 * shown; NULL where no memory is left. */
static hero_ui_gtk_dialog *hero_ui_gtk_dialog_new(int64_t dialog, int64_t window, int kind) {
    hero_ui_gtk_dialog *made = hero_layer_alloc_zero(1, sizeof *made);
    hero_ui_gtk_dialog **at = &hero_ui_gtk_dialogs;

    if (made == NULL) {
        return NULL;
    }
    made->dialog = dialog;
    made->window = window;
    made->kind = kind;
    made->cancel = -1;
    made->cancellable = hero_ui_gtk.cancellable_new();
    while (*at != NULL) {
        at = &(*at)->next;
    }
    *at = made;
    return made;
}

/* Takes dialog `gone` out of the dialogs shown and gives back what it held. */
static void hero_ui_gtk_dialog_free(hero_ui_gtk_dialog *gone) {
    for (hero_ui_gtk_dialog **at = &hero_ui_gtk_dialogs; *at != NULL; at = &(*at)->next) {
        if (*at == gone) {
            *at = gone->next;
            break;
        }
    }
    for (int64_t i = 0; gone->labels != NULL && i < gone->shown; i += 1) {
        hero_layer_free(gone->labels[i]);
    }
    hero_layer_free(gone->labels);
    if (gone->cancellable != NULL) {
        hero_ui_gtk.object_unref(gone->cancellable);
    }
    if (gone->object != NULL && gone->kind >= HERO_UI_GTK_DIALOG_FILES) {
        hero_ui_gtk.object_unref(gone->object);
    }
    hero_layer_free(gone);
}

/* Dialog `done` answered `value`, its paths `paths`: queued unless it was
 * ended, and given back. */
static void hero_ui_gtk_dialog_told(hero_ui_gtk_dialog *done, int64_t value, const uint8_t *paths, int64_t count) {
    if (!done->ended) {
        hero_ui_tell_text(HERO_UI_EVENT_DIALOG, done->dialog, value, 0, paths, count);
    }
    hero_ui_gtk_dialog_free(done);
}

/* What a message box's or an ask's button `index` answers. */
static int64_t hero_ui_gtk_pressed(const hero_ui_gtk_dialog *shown, int64_t index) {
    if (shown->kind == HERO_UI_GTK_DIALOG_MESSAGE) {
        return index >= 0 && index < shown->shown ? shown->answers[index] : HERO_UI_ANSWER_CANCEL;
    }
    return index >= 0 && index < shown->shown ? index : shown->cancel;
}

static void hero_ui_gtk_alert_done(void *dialog, void *result, void *data) {
    hero_ui_gerror *error = NULL;
    int index = hero_ui_gtk.alert_dialog_choose_finish(dialog, result, &error);

    if (error != NULL) {
        hero_ui_gtk.error_free(error);
        index = -1;
    }
    hero_ui_gtk_dialog_told(data, hero_ui_gtk_pressed(data, index), NULL, 0);
}

static void hero_ui_gtk_message_responded(void *dialog, int response, void *data) {
    hero_ui_gtk_dialog *shown = data;

    shown->object = NULL;
    hero_ui_gtk.window_destroy(dialog);
    hero_ui_gtk_dialog_told(shown, hero_ui_gtk_pressed(shown, response >= 0 ? response : -1), NULL, 0);
}

/* Queues the files GIO list or file `value` names, `many` for a list. */
static void hero_ui_gtk_dialog_chose(hero_ui_gtk_dialog *shown, void *value, bool many) {
    uint8_t *paths = NULL;
    int64_t count = 0;
    int64_t room = 0;
    int64_t made = 0;
    bool kept = true;

    if (value != NULL && many) {
        unsigned files = hero_ui_gtk.list_model_get_n_items(value);

        for (unsigned i = 0; i < files && kept; i += 1) {
            void *file = hero_ui_gtk.list_model_get_item(value, i);
            int64_t before = count;

            kept = hero_ui_gtk_keep_path(file, &paths, &count, &room);
            made += count > before ? 1 : 0;
            hero_ui_gtk.object_unref(file);
        }
    } else if (value != NULL) {
        int64_t before = count;

        kept = hero_ui_gtk_keep_path(value, &paths, &count, &room);
        made += count > before ? 1 : 0;
    }
    if (!kept) {
        /* No memory left for the paths: the choice is told as none rather
         * than as fewer than the person chose. */
        made = 0;
        count = 0;
    }
    hero_ui_gtk_dialog_told(shown, made, made > 0 ? paths : NULL, made > 0 ? count : 0);
    hero_layer_free(paths);
}

static void hero_ui_gtk_files_done(void *dialog, void *result, void *data) {
    hero_ui_gtk_dialog *shown = data;
    hero_ui_gerror *error = NULL;
    void *value;
    bool many = shown->choose == HERO_UI_GTK_CHOOSE_OPEN_MANY || shown->choose == HERO_UI_GTK_CHOOSE_FOLDERS;

    switch (shown->choose) {
    case HERO_UI_GTK_CHOOSE_OPEN_MANY:
        value = hero_ui_gtk.file_dialog_open_multiple_finish(dialog, result, &error);
        break;
    case HERO_UI_GTK_CHOOSE_SAVE:
        value = hero_ui_gtk.file_dialog_save_finish(dialog, result, &error);
        break;
    case HERO_UI_GTK_CHOOSE_FOLDER:
        value = hero_ui_gtk.file_dialog_select_folder_finish(dialog, result, &error);
        break;
    case HERO_UI_GTK_CHOOSE_FOLDERS:
        value = hero_ui_gtk.file_dialog_select_multiple_folders_finish(dialog, result, &error);
        break;
    default:
        value = hero_ui_gtk.file_dialog_open_finish(dialog, result, &error);
        break;
    }
    if (error != NULL) {
        hero_ui_gtk.error_free(error);
    }
    hero_ui_gtk_dialog_chose(shown, value, many);
    if (value != NULL) {
        hero_ui_gtk.object_unref(value);
    }
}

static void hero_ui_gtk_chooser_responded(void *chooser, int response, void *data) {
    hero_ui_gtk_dialog *shown = data;
    void *files = response == HERO_GTK_RESPONSE_ACCEPT ? hero_ui_gtk.file_chooser_get_files(chooser) : NULL;

    hero_ui_gtk_dialog_chose(shown, files, true);
    if (files != NULL) {
        hero_ui_gtk.object_unref(files);
    }
}

/* The toplevel window GTK lists now and not in `before`, `count` of them:
 * the window a dialog just shown added. */
static void *hero_ui_gtk_added_toplevel(void *const *before, unsigned count) {
    void *toplevels = hero_ui_gtk.window_get_toplevels();
    unsigned now = toplevels != NULL ? hero_ui_gtk.list_model_get_n_items(toplevels) : 0;
    void *added = NULL;

    for (unsigned i = 0; i < now && added == NULL; i += 1) {
        void *each = hero_ui_gtk.list_model_get_item(toplevels, i);
        bool known = false;

        for (unsigned j = 0; j < count && !known; j += 1) {
            known = before[j] == each;
        }
        added = known ? NULL : each;
        hero_ui_gtk.object_unref(each);
    }
    return added;
}

/* The toplevel windows GTK lists now, into `into`, at most `most`; how many. */
static unsigned hero_ui_gtk_toplevels(void **into, unsigned most) {
    void *toplevels = hero_ui_gtk.window_get_toplevels();
    unsigned now = toplevels != NULL ? hero_ui_gtk.list_model_get_n_items(toplevels) : 0;
    unsigned made = 0;

    for (unsigned i = 0; i < now && made < most; i += 1) {
        void *each = hero_ui_gtk.list_model_get_item(toplevels, i);

        into[made++] = each;
        hero_ui_gtk.object_unref(each);
    }
    return made;
}

/* The most toplevel windows a dialog's window is told apart from. */
#define HERO_UI_GTK_TOPLEVELS_MOST 512

/* Shows message box or ask `shown`, over window `owner`, its heading,
 * words and tone given: GTK 4.10's alert where the library has it. */
static int64_t hero_ui_gtk_show_box(hero_ui_gtk_dialog *shown, hero_ui_native *owner, int64_t tone, const char *heading, const char *said) {
    if (hero_ui_gtk.alert_dialog_new != NULL && hero_ui_gtk.alert_dialog_choose != NULL && hero_ui_gtk.alert_dialog_choose_finish != NULL && hero_ui_gtk.alert_dialog_set_buttons != NULL && hero_ui_gtk.alert_dialog_set_detail != NULL &&
        hero_ui_gtk.alert_dialog_set_cancel_button != NULL && hero_ui_gtk.alert_dialog_set_default_button != NULL && hero_ui_gtk.alert_dialog_set_modal != NULL) {
        void *alert = hero_ui_gtk.alert_dialog_new("%s", heading);
        void *before[HERO_UI_GTK_TOPLEVELS_MOST];
        unsigned count;

        if (alert == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.alert_dialog_set_detail(alert, said);
        hero_ui_gtk.alert_dialog_set_buttons(alert, (const char *const *)shown->labels);
        hero_ui_gtk.alert_dialog_set_cancel_button(alert, (int)(shown->kind == HERO_UI_GTK_DIALOG_ASK ? shown->cancel : (shown->answers[0] == HERO_UI_ANSWER_CANCEL ? 0 : -1)));
        hero_ui_gtk.alert_dialog_set_default_button(alert, (int)(shown->kind == HERO_UI_GTK_DIALOG_ASK ? 0 : shown->shown - 1));
        hero_ui_gtk.alert_dialog_set_modal(alert, 1);
        count = hero_ui_gtk_toplevels(before, HERO_UI_GTK_TOPLEVELS_MOST);
        hero_ui_gtk.alert_dialog_choose(alert, owner->widget, shown->cancellable, hero_ui_gtk_alert_done, shown);
        shown->window_shown = hero_ui_gtk_added_toplevel(before, count);
        hero_ui_gtk.object_unref(alert);
        return HERO_UI_OK;
    }
    {
        int type = tone == HERO_UI_TONE_ERROR ? HERO_GTK_MESSAGE_ERROR : (tone == HERO_UI_TONE_WARNING ? HERO_GTK_MESSAGE_WARNING : HERO_GTK_MESSAGE_INFO);
        void *dialog = hero_ui_gtk.message_dialog_new(owner->widget, HERO_GTK_DIALOG_MODAL | HERO_GTK_DIALOG_DESTROY_WITH_PARENT, type, HERO_GTK_BUTTONS_NONE, "%s", heading);

        if (dialog == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.message_dialog_format_secondary_text(dialog, "%s", said);
        for (int64_t i = 0; i < shown->shown; i += 1) {
            (void)hero_ui_gtk.dialog_add_button(dialog, shown->labels[i], (int)i);
        }
        hero_ui_gtk.dialog_set_default_response(dialog, (int)(shown->kind == HERO_UI_GTK_DIALOG_ASK ? 0 : shown->shown - 1));
        (void)hero_ui_gtk.signal_connect_data(dialog, "response", (hero_ui_gtk_callback)hero_ui_gtk_message_responded, shown, NULL, 0);
        shown->object = dialog;
        shown->window_shown = dialog;
        hero_ui_gtk.window_present(dialog);
        return HERO_UI_OK;
    }
}

int64_t hero_ui_arm_show_message(int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count) {
    hero_ui_native *owner = hero_ui_gtk_window(window);
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *said = hero_ui_gtk_text(text, text_count, false);
    hero_ui_gtk_dialog *shown = owner != NULL ? hero_ui_gtk_dialog_new(dialog, window, HERO_UI_GTK_DIALOG_MESSAGE) : NULL;
    const char *msgids[3];
    int64_t answer = HERO_UI_LIMIT;

    if (owner == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return hero_ui_find(window) == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    }
    if (shown != NULL) {
        shown->labels = hero_layer_alloc_zero(4, sizeof *shown->labels);
    }
    if (heading != NULL && said != NULL && shown != NULL && shown->labels != NULL) {
        if (buttons == HERO_UI_BUTTONS_YES_NO_CANCEL || buttons == HERO_UI_BUTTONS_OK_CANCEL) {
            shown->answers[shown->shown] = HERO_UI_ANSWER_CANCEL;
            msgids[shown->shown++] = "_Cancel";
        }
        if (buttons == HERO_UI_BUTTONS_YES_NO || buttons == HERO_UI_BUTTONS_YES_NO_CANCEL) {
            shown->answers[shown->shown] = HERO_UI_ANSWER_NO;
            msgids[shown->shown++] = "_No";
            shown->answers[shown->shown] = HERO_UI_ANSWER_YES;
            msgids[shown->shown++] = "_Yes";
        } else {
            shown->answers[shown->shown] = HERO_UI_ANSWER_OK;
            msgids[shown->shown++] = "_OK";
        }
        answer = HERO_UI_OK;
        for (int64_t i = 0; i < shown->shown; i += 1) {
            char words[64];

            hero_ui_gtk_words_of(msgids[i], words, sizeof words);
            shown->labels[i] = hero_ui_gtk_text((const uint8_t *)words, (int64_t)strlen(words), false);
            answer = shown->labels[i] == NULL ? HERO_UI_LIMIT : answer;
        }
        if (answer == HERO_UI_OK) {
            answer = hero_ui_gtk_show_box(shown, owner, tone, heading, said);
        }
    }
    if (answer != HERO_UI_OK && shown != NULL) {
        hero_ui_gtk_dialog_free(shown);
    }
    hero_layer_free(heading);
    hero_layer_free(said);
    return answer;
}

int64_t hero_ui_arm_show_ask(int64_t dialog, int64_t window, int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel) {
    hero_ui_native *owner = hero_ui_gtk_window(window);
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *said = hero_ui_gtk_text(text, text_count, false);
    int64_t count = hero_ui_texts_in(buttons, buttons_count);
    hero_ui_gtk_dialog *shown = owner != NULL ? hero_ui_gtk_dialog_new(dialog, window, HERO_UI_GTK_DIALOG_ASK) : NULL;
    int64_t answer = HERO_UI_LIMIT;

    if (owner == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return hero_ui_find(window) == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    }
    if (shown != NULL) {
        shown->labels = hero_layer_alloc_zero((size_t)count + 1, sizeof *shown->labels);
        shown->cancel = cancel;
    }
    if (heading != NULL && said != NULL && shown != NULL && shown->labels != NULL) {
        answer = HERO_UI_OK;
        for (int64_t i = 0; i < count; i += 1) {
            int64_t length = 0;
            const uint8_t *label = hero_ui_text_at(buttons, buttons_count, i, &length);

            shown->labels[i] = hero_ui_gtk_text(label, length, false);
            shown->shown = i + 1;
            answer = shown->labels[i] == NULL ? HERO_UI_LIMIT : answer;
        }
        if (answer == HERO_UI_OK) {
            answer = hero_ui_gtk_show_box(shown, owner, tone, heading, said);
        }
    }
    if (answer != HERO_UI_OK && shown != NULL) {
        hero_ui_gtk_dialog_free(shown);
    }
    hero_layer_free(heading);
    hero_layer_free(said);
    return answer;
}

/* Shows file or folder dialog `shown` over window `owner`, titled
 * `heading`, its filters `store` (NULL for none) and its proposed name
 * `proposed` (empty for none): GTK 4.10's `GtkFileDialog` where the library
 * has it, else GTK 4.0's `GtkFileChooserNative`. */
static int64_t hero_ui_gtk_show_chooser(hero_ui_gtk_dialog *shown, hero_ui_native *owner, const char *heading, void *store, const char *proposed) {
    if (hero_ui_gtk.file_dialog_new != NULL && hero_ui_gtk.file_dialog_set_title != NULL && hero_ui_gtk.file_dialog_set_modal != NULL && hero_ui_gtk.file_dialog_set_initial_name != NULL && hero_ui_gtk.file_dialog_set_filters != NULL &&
        hero_ui_gtk.file_dialog_open != NULL && hero_ui_gtk.file_dialog_open_finish != NULL && hero_ui_gtk.file_dialog_open_multiple != NULL && hero_ui_gtk.file_dialog_open_multiple_finish != NULL && hero_ui_gtk.file_dialog_save != NULL &&
        hero_ui_gtk.file_dialog_save_finish != NULL && hero_ui_gtk.file_dialog_select_folder != NULL && hero_ui_gtk.file_dialog_select_folder_finish != NULL && hero_ui_gtk.file_dialog_select_multiple_folders != NULL &&
        hero_ui_gtk.file_dialog_select_multiple_folders_finish != NULL) {
        void *chooser = hero_ui_gtk.file_dialog_new();

        if (chooser == NULL) {
            return HERO_UI_LIMIT;
        }
        if (heading[0] != '\0') {
            hero_ui_gtk.file_dialog_set_title(chooser, heading);
        }
        hero_ui_gtk.file_dialog_set_modal(chooser, 1);
        if (store != NULL) {
            hero_ui_gtk.file_dialog_set_filters(chooser, store);
        }
        if (shown->choose == HERO_UI_GTK_CHOOSE_SAVE && proposed[0] != '\0') {
            hero_ui_gtk.file_dialog_set_initial_name(chooser, proposed);
        }
        switch (shown->choose) {
        case HERO_UI_GTK_CHOOSE_OPEN_MANY:
            hero_ui_gtk.file_dialog_open_multiple(chooser, owner->widget, shown->cancellable, hero_ui_gtk_files_done, shown);
            break;
        case HERO_UI_GTK_CHOOSE_SAVE:
            hero_ui_gtk.file_dialog_save(chooser, owner->widget, shown->cancellable, hero_ui_gtk_files_done, shown);
            break;
        case HERO_UI_GTK_CHOOSE_FOLDER:
            hero_ui_gtk.file_dialog_select_folder(chooser, owner->widget, shown->cancellable, hero_ui_gtk_files_done, shown);
            break;
        case HERO_UI_GTK_CHOOSE_FOLDERS:
            hero_ui_gtk.file_dialog_select_multiple_folders(chooser, owner->widget, shown->cancellable, hero_ui_gtk_files_done, shown);
            break;
        default:
            hero_ui_gtk.file_dialog_open(chooser, owner->widget, shown->cancellable, hero_ui_gtk_files_done, shown);
            break;
        }
        hero_ui_gtk.object_unref(chooser);
        return HERO_UI_OK;
    }
    {
        bool save = shown->choose == HERO_UI_GTK_CHOOSE_SAVE;
        bool folders = shown->choose == HERO_UI_GTK_CHOOSE_FOLDER || shown->choose == HERO_UI_GTK_CHOOSE_FOLDERS;
        char accept[64];
        char cancel[64];
        void *chooser;

        hero_ui_gtk_words_of(save ? "_Save" : (folders ? "_Select" : "_Open"), accept, sizeof accept);
        hero_ui_gtk_words_of("_Cancel", cancel, sizeof cancel);
        chooser = hero_ui_gtk.file_chooser_native_new(heading[0] != '\0' ? heading : NULL, owner->widget, save ? HERO_GTK_FILE_CHOOSER_ACTION_SAVE : (folders ? HERO_GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER : HERO_GTK_FILE_CHOOSER_ACTION_OPEN), accept, cancel);
        if (chooser == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.native_dialog_set_modal(chooser, 1);
        hero_ui_gtk.file_chooser_set_select_multiple(chooser, shown->choose == HERO_UI_GTK_CHOOSE_OPEN_MANY || shown->choose == HERO_UI_GTK_CHOOSE_FOLDERS ? 1 : 0);
        if (save && proposed[0] != '\0') {
            hero_ui_gtk.file_chooser_set_current_name(chooser, proposed);
        }
        for (unsigned i = 0; store != NULL && i < hero_ui_gtk.list_model_get_n_items(store); i += 1) {
            void *filter = hero_ui_gtk.list_model_get_item(store, i);

            hero_ui_gtk.file_chooser_add_filter(chooser, filter);
            hero_ui_gtk.object_unref(filter);
        }
        (void)hero_ui_gtk.signal_connect_data(chooser, "response", (hero_ui_gtk_callback)hero_ui_gtk_chooser_responded, shown, NULL, 0);
        shown->object = chooser;
        hero_ui_gtk.native_dialog_show(chooser);
        return HERO_UI_OK;
    }
}

int64_t hero_ui_arm_show_files(int64_t dialog, int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count) {
    hero_ui_native *owner = hero_ui_gtk_window(window);
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *proposed = hero_ui_gtk_text(name, name_count, false);
    int64_t made = 0;
    void *store = NULL;
    hero_ui_gtk_dialog *shown = NULL;
    int64_t answer = HERO_UI_LIMIT;

    if (owner == NULL) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        return hero_ui_find(window) == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    }
    if (heading != NULL && proposed != NULL) {
        store = hero_ui_gtk_filters(filters, filters_count, &made);
        shown = hero_ui_gtk_dialog_new(dialog, window, HERO_UI_GTK_DIALOG_FILES);
    }
    if (shown != NULL) {
        shown->choose = save == 1 ? HERO_UI_GTK_CHOOSE_SAVE : (multiple == 1 ? HERO_UI_GTK_CHOOSE_OPEN_MANY : HERO_UI_GTK_CHOOSE_OPEN);
        answer = hero_ui_gtk_show_chooser(shown, owner, heading, made > 0 ? store : NULL, proposed);
        if (answer != HERO_UI_OK) {
            hero_ui_gtk_dialog_free(shown);
        }
    }
    if (store != NULL) {
        hero_ui_gtk.object_unref(store);
    }
    hero_layer_free(heading);
    hero_layer_free(proposed);
    return answer;
}

int64_t hero_ui_arm_show_folders(int64_t dialog, int64_t window, int64_t multiple, const uint8_t *title, int64_t title_count) {
    hero_ui_native *owner = hero_ui_gtk_window(window);
    char *heading = hero_ui_gtk_text(title, title_count, false);
    hero_ui_gtk_dialog *shown = NULL;
    int64_t answer = HERO_UI_LIMIT;

    if (owner == NULL) {
        hero_layer_free(heading);
        return hero_ui_find(window) == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    }
    if (heading != NULL) {
        shown = hero_ui_gtk_dialog_new(dialog, window, HERO_UI_GTK_DIALOG_FOLDERS);
    }
    if (shown != NULL) {
        shown->choose = multiple == 1 ? HERO_UI_GTK_CHOOSE_FOLDERS : HERO_UI_GTK_CHOOSE_FOLDER;
        answer = hero_ui_gtk_show_chooser(shown, owner, heading, NULL, "");
        if (answer != HERO_UI_OK) {
            hero_ui_gtk_dialog_free(shown);
        }
    }
    hero_layer_free(heading);
    return answer;
}

/* The button labelled `label` in widget `root` and every widget inside it,
 * or NULL. */
static void *hero_ui_gtk_button_labelled(void *root, const char *label) {
    for (void *child = hero_ui_gtk.widget_get_first_child(root); child != NULL; child = hero_ui_gtk.widget_get_next_sibling(child)) {
        void *found;

        if (hero_ui_gtk.type_check_instance_is_a(child, hero_ui_gtk.button_get_type())) {
            const char *own = hero_ui_gtk.button_get_label(child);

            if (own != NULL && strcmp(own, label) == 0) {
                return child;
            }
        }
        found = hero_ui_gtk_button_labelled(child, label);
        if (found != NULL) {
            return found;
        }
    }
    return NULL;
}

/* Whether GTK still lists toplevel window `window`. */
static bool hero_ui_gtk_still_shown(void *window) {
    void *now[HERO_UI_GTK_TOPLEVELS_MOST];
    unsigned count = hero_ui_gtk_toplevels(now, HERO_UI_GTK_TOPLEVELS_MOST);

    for (unsigned i = 0; window != NULL && i < count; i += 1) {
        if (now[i] == window) {
            return true;
        }
    }
    return false;
}

int64_t hero_ui_arm_answer_dialog(int64_t dialog, int64_t value, const uint8_t *paths, int64_t count) {
    hero_ui_gtk_dialog *shown = hero_ui_gtk_dialogs;

    (void)paths;
    (void)count;
    while (shown != NULL && (shown->dialog != dialog || shown->ended)) {
        shown = shown->next;
    }
    if (shown == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (shown->kind == HERO_UI_GTK_DIALOG_FILES || shown->kind == HERO_UI_GTK_DIALOG_FOLDERS) {
        if (value != 0) {
            return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "GTK's file dialog takes its files from the person alone: no program chooses in it, though one may cancel it");
        }
        if (shown->object != NULL) {
            /* GTK 4.0's chooser, hidden: its response never comes. */
            hero_ui_gtk.native_dialog_hide(shown->object);
            hero_ui_gtk_dialog_told(shown, 0, NULL, 0);
        } else {
            hero_ui_gtk.cancellable_cancel(shown->cancellable);
        }
        return HERO_UI_OK;
    }
    {
        int64_t index = -1;
        void *button;

        for (int64_t i = 0; i < shown->shown; i += 1) {
            index = index < 0 && hero_ui_gtk_pressed(shown, i) == value ? i : index;
        }
        if (index < 0) {
            return HERO_UI_INVALID;
        }
        button = hero_ui_gtk_still_shown(shown->window_shown) ? hero_ui_gtk_button_labelled(shown->window_shown, shown->labels[index]) : NULL;
        if (button == NULL) {
            return hero_ui_gtk_said_so(HERO_UI_OTHER, "the dialog's window holds no button of that label");
        }
        /* Pressed as the person's click presses it. */
        (void)hero_ui_gtk.widget_activate(button);
        return HERO_UI_OK;
    }
}

/* Whether no dialog ended still waits for its callback. */
static bool hero_ui_gtk_ends_taken(void *data) {
    (void)data;
    for (hero_ui_gtk_dialog *each = hero_ui_gtk_dialogs; each != NULL; each = each->next) {
        if (each->ended) {
            return false;
        }
    }
    return true;
}

void hero_ui_arm_dialogs_end(int64_t window) {
    hero_ui_gtk_dialog *shown = hero_ui_gtk_dialogs;

    while (shown != NULL) {
        hero_ui_gtk_dialog *next = shown->next;

        if (!shown->ended && (window == 0 || shown->window == window)) {
            shown->ended = true;
            if (shown->kind <= HERO_UI_GTK_DIALOG_ASK && shown->object != NULL) {
                /* GTK 4.0's message box, destroyed: its response never
                 * comes. */
                hero_ui_gtk.window_destroy(shown->object);
                hero_ui_gtk_dialog_free(shown);
            } else if (shown->object != NULL) {
                hero_ui_gtk.native_dialog_hide(shown->object);
                hero_ui_gtk_dialog_free(shown);
            } else {
                /* Its callback comes once GTK took the cancel in. */
                hero_ui_gtk.cancellable_cancel(shown->cancellable);
            }
        }
        shown = next;
    }
    /* GTK's loop is given its turns until each callback came, a second at
     * most: a window destroyed before its alert took the cancel in is
     * destroyed twice by GTK 4.18.6 (its `gtk_window_destroy` assertion,
     * measured under Xvfb), and the window goes once this returns. */
    (void)hero_ui_gtk_spin(1000, hero_ui_gtk_ends_taken, NULL);
}

int64_t hero_ui_arm_dialogs_words(char *words, int64_t count) {
    return hero_ui_arm_windows_words(words, count);
}

/* -- THE DOCK, A NOTIFICATION, THE APPEARANCE, ACCESSIBLE NAMES -------------
 *
 * GTK 4 keeps no window out of the task bar, but X11 does: EWMH's
 * `_NET_WM_STATE_SKIP_TASKBAR`, asked of the window manager for every
 * window and each new one as it maps. A notification goes to the desktop's
 * notification server, `org.freedesktop.Notifications` on the session bus,
 * as libnotify hands it, so no application id is needed (GTK's
 * `GNotification` asks one). The appearance is GTK's: 4.20's
 * `gtk-interface-color-scheme` where GTK has it, else the dark variant GTK
 * draws (`gtk-application-prefer-dark-theme`, or a theme named dark);
 * turning it sets `gtk-application-prefer-dark-theme`, and its turns are told
 * from `GtkSettings`' notifications. An accessible name is GTK's own
 * `GTK_ACCESSIBLE_PROPERTY_LABEL`, and a label ties to its field by
 * `GTK_ACCESSIBLE_RELATION_LABELLED_BY`; GTK reads neither back, so the
 * arm keeps the name the program gave. */

/* Asks the window manager to keep window `window` out of the task bar and
 * the pager, or not. */
static void hero_ui_gtk_send_docked(hero_ui_native *window) {
    void *xd = hero_ui_gtk_xdisplay();
    void *surface = hero_ui_gtk_surface(window);
    hero_ui_x_event event;

    if (xd == NULL || surface == NULL) {
        return;
    }
    memset(&event, 0, sizeof event);
    event.client.type = HERO_X_CLIENT_MESSAGE;
    event.client.send_event = 1;
    event.client.window = hero_ui_gtk.x11_surface_get_xid(surface);
    event.client.message_type = hero_ui_gtk_atom("_NET_WM_STATE");
    event.client.format = 32;
    event.client.data[0] = hero_ui_gtk_undocked ? 1 : 0;
    event.client.data[1] = (long)hero_ui_gtk_atom("_NET_WM_STATE_SKIP_TASKBAR");
    event.client.data[2] = (long)hero_ui_gtk_atom("_NET_WM_STATE_SKIP_PAGER");
    event.client.data[3] = 1;
    (void)hero_ui_gtk.x_send_event(xd, hero_ui_gtk.x11_display_get_xrootwindow(hero_ui_gtk.display_get_default()), 0, HERO_X_SUBSTRUCTURE_MASKS, &event);
    (void)hero_ui_gtk.x_sync(xd, 0);
}

int64_t hero_ui_arm_set_docked(int64_t on) {
    void *xd = hero_ui_gtk_xdisplay();

    if (xd == NULL) {
        return hero_ui_gtk_not_x11("Wayland lets no program keep its windows out of the task bar: xdg_toplevel has no such request, and GTK 4 removed skip-taskbar");
    }
    if (!hero_ui_gtk_manager_runs(xd)) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, hero_ui_gtk_no_manager);
    }
    if (!hero_ui_gtk_manager_offers(xd, "_NET_WM_STATE_SKIP_TASKBAR")) {
        return hero_ui_gtk_said_so(HERO_UI_UNSUPPORTED, "the window manager running here does not offer _NET_WM_STATE_SKIP_TASKBAR, which keeps a window out of the task bar");
    }
    hero_ui_gtk_undocked = on == 0;
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *each = hero_ui_at(at);

        if (each != NULL && each->kind == HERO_UI_WINDOW && hero_ui_gtk_viewable(each)) {
            hero_ui_gtk_send_docked(each);
        }
    }
    return HERO_UI_OK;
}

/* The program's name, as the notification server shows it: GLib's, else
 * the executable's file name. */
static void hero_ui_gtk_program_name(char *into, size_t room) {
    const char *named = hero_ui_gtk.get_prgname();
    char path[4096];
    ssize_t length;

    if (named != NULL && named[0] != '\0') {
        (void)hero_ui_copy_text(into, room, named);
        return;
    }
    length = readlink("/proc/self/exe", path, sizeof path - 1);
    if (length <= 0) {
        (void)hero_ui_copy_text(into, room, "program");
        return;
    }
    path[length] = '\0';
    (void)hero_ui_copy_text(into, room, strrchr(path, '/') != NULL ? strrchr(path, '/') + 1 : path);
}

int64_t hero_ui_arm_notify(const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count) {
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *said = hero_ui_gtk_text(text, text_count, false);
    char name[256];
    char words[HERO_UI_WORDS_MOST];
    hero_ui_gerror *error = NULL;
    void *bus;
    void *reply;
    void *arguments;

    if (heading == NULL || said == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return HERO_UI_LIMIT;
    }
    bus = hero_ui_gtk.bus_get_sync(HERO_G_BUS_TYPE_SESSION, NULL, &error);
    if (bus == NULL) {
        (void)snprintf(words, sizeof words, "no D-Bus session bus answers (%s), and a notification reaches the desktop through org.freedesktop.Notifications on it", error != NULL && error->message != NULL ? error->message : "no words");
        if (error != NULL) {
            hero_ui_gtk.error_free(error);
        }
        hero_layer_free(heading);
        hero_layer_free(said);
        return hero_ui_refused(HERO_UI_UNSUPPORTED, words);
    }
    hero_ui_gtk_program_name(name, sizeof name);
    arguments = hero_ui_gtk.variant_new("(susss@as@a{sv}i)", name, 0u, "", heading, said, hero_ui_gtk.variant_new_strv(NULL, 0), hero_ui_gtk.variant_new_array("{sv}", NULL, 0), -1);
    reply = arguments != NULL ? hero_ui_gtk.dbus_connection_call_sync(bus, "org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify", arguments, NULL, 0, 5000, NULL, &error) : NULL;
    hero_ui_gtk.object_unref(bus);
    hero_layer_free(heading);
    hero_layer_free(said);
    if (reply == NULL) {
        (void)snprintf(words, sizeof words, "the desktop's notification server did not take it (%s)", error != NULL && error->message != NULL ? error->message : "no words");
        if (error != NULL) {
            hero_ui_gtk.error_free(error);
        }
        return hero_ui_refused(HERO_UI_UNSUPPORTED, words);
    }
    hero_ui_gtk.variant_unref(reply);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_system_words(char *words, int64_t count) {
    return hero_ui_arm_words(words, count);
}

/* GTK's settings' property `name`, where GTK has it. */
static bool hero_ui_gtk_has_setting(void *settings, const char *name) {
    /* An instance's first member is its class (`GTypeInstance`). */
    void *klass = *(void **)settings;

    return hero_ui_gtk.object_class_find_property(klass, name) != NULL;
}

/* The appearance GTK draws in now. */
static int64_t hero_ui_gtk_appearance_now(void) {
    void *settings = hero_ui_gtk.settings_get_default();
    int dark = 0;
    char *theme = NULL;
    bool named_dark = false;

    if (settings == NULL) {
        return HERO_UI_APPEARANCE_LIGHT;
    }
    if (hero_ui_gtk_has_setting(settings, "gtk-interface-color-scheme")) {
        int scheme = 0;

        hero_ui_gtk.object_get(settings, "gtk-interface-color-scheme", &scheme, NULL);
        if (scheme == HERO_GTK_INTERFACE_COLOR_SCHEME_DARK) {
            return HERO_UI_APPEARANCE_DARK;
        }
        if (scheme == HERO_GTK_INTERFACE_COLOR_SCHEME_LIGHT) {
            return HERO_UI_APPEARANCE_LIGHT;
        }
    }
    hero_ui_gtk.object_get(settings, "gtk-application-prefer-dark-theme", &dark, NULL);
    hero_ui_gtk.object_get(settings, "gtk-theme-name", &theme, NULL);
    for (const char *at = theme; at != NULL && *at != '\0' && !named_dark; at += 1) {
        named_dark = (at[0] == 'd' || at[0] == 'D') && strncmp(at + 1, "ark", 3) == 0;
    }
    hero_ui_gtk.g_free(theme);
    return dark || named_dark ? HERO_UI_APPEARANCE_DARK : HERO_UI_APPEARANCE_LIGHT;
}

/* The appearance told last, and whether the system's own dark preference
 * was read as the app opened, so the system's is given back. */
static _Thread_local int64_t hero_ui_gtk_appearance_told;
static _Thread_local int hero_ui_gtk_prefer_dark_first;
static _Thread_local bool hero_ui_gtk_appearance_watched;

static void hero_ui_gtk_appearance_turned(void *settings, void *spec, void *data) {
    int64_t now = hero_ui_gtk_appearance_now();

    (void)settings;
    (void)spec;
    (void)data;
    if (now != hero_ui_gtk_appearance_told) {
        hero_ui_gtk_appearance_told = now;
        hero_ui_tell(HERO_UI_EVENT_APPEARANCE, 0, now, 0);
    }
}

/* Watches GTK's settings for the appearance's turns, from the app's open. */
static void hero_ui_gtk_watch_appearance(void) {
    void *settings = hero_ui_gtk.settings_get_default();
    static const char *const watched[] = {"notify::gtk-application-prefer-dark-theme", "notify::gtk-theme-name", "notify::gtk-interface-color-scheme"};

    if (settings == NULL || hero_ui_gtk_appearance_watched) {
        return;
    }
    hero_ui_gtk.object_get(settings, "gtk-application-prefer-dark-theme", &hero_ui_gtk_prefer_dark_first, NULL);
    hero_ui_gtk_appearance_told = hero_ui_gtk_appearance_now();
    for (size_t i = 0; i < sizeof watched / sizeof watched[0]; i += 1) {
        if (i < 2 || hero_ui_gtk_has_setting(settings, "gtk-interface-color-scheme")) {
            (void)hero_ui_gtk.signal_connect_data(settings, watched[i], (hero_ui_gtk_callback)hero_ui_gtk_appearance_turned, NULL, NULL, 0);
        }
    }
    hero_ui_gtk_appearance_watched = true;
}

/* Lets go of GTK's settings as the app closes, the system's dark preference
 * given back. */
static void hero_ui_gtk_unwatch_appearance(void) {
    void *settings = hero_ui_gtk.settings_get_default();

    if (settings == NULL || !hero_ui_gtk_appearance_watched) {
        return;
    }
    (void)hero_ui_gtk.signal_handlers_disconnect_matched(settings, HERO_G_SIGNAL_MATCH_FUNC_DATA, 0, 0, NULL, (hero_ui_gtk_callback)hero_ui_gtk_appearance_turned, NULL);
    hero_ui_gtk.object_set(settings, "gtk-application-prefer-dark-theme", hero_ui_gtk_prefer_dark_first, NULL);
    hero_ui_gtk_appearance_watched = false;
}

int64_t hero_ui_arm_appearance(int64_t *appearance) {
    hero_ui_gtk_watch_appearance();
    *appearance = hero_ui_gtk_appearance_now();
    return HERO_UI_OK;
}

int64_t hero_ui_arm_appearance_set(int64_t appearance) {
    void *settings = hero_ui_gtk.settings_get_default();

    if (settings == NULL) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "GTK holds no settings on this display");
    }
    hero_ui_gtk_watch_appearance();
    hero_ui_gtk.object_set(settings, "gtk-application-prefer-dark-theme", appearance == 0 ? hero_ui_gtk_prefer_dark_first : (appearance == HERO_UI_APPEARANCE_DARK ? 1 : 0), NULL);
    /* GTK notifies a setting changed as it is set; a turn it did not tell
     * (the theme's own name already dark) is told here. */
    hero_ui_gtk_appearance_turned(settings, NULL, NULL);
    return HERO_UI_OK;
}

/* The GTK widget a screen reader reads for `widget`, or NULL for a kind
 * GTK draws none of (a menu and its items, a context menu). */
static void *hero_ui_gtk_accessible_of(const hero_ui_native *widget) {
    switch (widget->kind) {
    case HERO_UI_TEXT_AREA:
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
    case HERO_UI_TABLE:
    case HERO_UI_TREE:
        return widget->inner;
    case HERO_UI_MENU:
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
    case HERO_UI_CONTEXT_MENU:
        return NULL;
    default:
        return widget->widget;
    }
}

int64_t hero_ui_arm_set_accessible_name(int64_t id, const uint8_t *name, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    void *target;
    char *made;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    target = hero_ui_gtk_accessible_of(widget);
    if (target == NULL) {
        return HERO_UI_INVALID;
    }
    if (count == 0) {
        hero_ui_gtk.accessible_reset_property(target, HERO_GTK_ACCESSIBLE_PROPERTY_LABEL);
        hero_layer_free(widget->accessible);
        widget->accessible = NULL;
        return HERO_UI_OK;
    }
    made = hero_ui_gtk_text(name, count, false);
    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_gtk.accessible_update_property(target, HERO_GTK_ACCESSIBLE_PROPERTY_LABEL, made, -1);
    hero_layer_free(widget->accessible);
    widget->accessible = made;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_accessible_name(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);
    size_t bytes;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (hero_ui_gtk_accessible_of(widget) == NULL) {
        return HERO_UI_INVALID;
    }
    bytes = widget->accessible != NULL ? strlen(widget->accessible) : 0;
    *length = (int64_t)bytes;
    if (bytes > 0 && *length <= count) {
        memcpy(buffer, widget->accessible, bytes);
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_label_for(int64_t id, int64_t label) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *caption = label != 0 ? hero_ui_find(label) : NULL;
    void *target;

    if (widget == NULL || (label != 0 && caption == NULL)) {
        return HERO_UI_NOT_FOUND;
    }
    target = hero_ui_gtk_accessible_of(widget);
    if (target == NULL || (caption != NULL && caption->kind != HERO_UI_LABEL)) {
        return HERO_UI_INVALID;
    }
    if (caption == NULL) {
        hero_ui_gtk.accessible_reset_relation(target, HERO_GTK_ACCESSIBLE_RELATION_LABELLED_BY);
    } else if (hero_ui_gtk.get_minor_version() >= 14) {
        /* GTK 4.14 on takes a relation's references as a list NULL ends
         * (measured on 4.18.6); before it, as a `GList` (its documentation,
         * unrun here). */
        hero_ui_gtk.accessible_update_relation(target, HERO_GTK_ACCESSIBLE_RELATION_LABELLED_BY, caption->widget, NULL, -1);
    } else {
        void *labels = hero_ui_gtk.list_append(NULL, caption->widget);

        hero_ui_gtk.accessible_update_relation(target, HERO_GTK_ACCESSIBLE_RELATION_LABELLED_BY, labels, -1);
        hero_ui_gtk.list_free(labels);
    }
    return HERO_UI_OK;
}

/* -- WHAT THIS ARM DRAWS --------------------------------------------------- */

/* Every kind and act of `hero_ui.h` is drawn here, so neither switch
 * refuses one before `create` and `inject`, which check each. */
bool hero_ui_arm_makes(int64_t kind, int64_t parent) {
    (void)kind;
    (void)parent;
    return true;
}

bool hero_ui_arm_acts(int64_t act) {
    (void)act;
    return true;
}

/* What this part keeps for the app, given back as it closes. */
static void hero_ui_gtk_close_more(void) {
    /* Every dialog ended, its callback given GTK's turns to come. */
    hero_ui_arm_dialogs_end(0);
    hero_ui_gtk_unwatch_appearance();
    hero_ui_gtk_undocked = false;
    hero_ui_gtk_modal = 0;
    hero_layer_free(hero_ui_gtk_clip);
    hero_ui_gtk_clip = NULL;
    hero_ui_gtk_clip_count = 0;
    hero_ui_gtk_clip_kept = false;
    if (hero_ui_gtk_icon != NULL) {
        hero_ui_gtk.object_unref(hero_ui_gtk_icon);
        hero_ui_gtk_icon = NULL;
    }
    if (hero_ui_gtk_xevents) {
        (void)hero_ui_gtk.signal_handlers_disconnect_matched(hero_ui_gtk.display_get_default(), HERO_G_SIGNAL_MATCH_FUNC_DATA, 0, 0, NULL, (hero_ui_gtk_callback)hero_ui_gtk_x_event, NULL);
        hero_ui_gtk_xevents = false;
    }
    hero_ui_gtk_places_due = false;
    hero_ui_gtk_said[0] = '\0';
}

#endif
