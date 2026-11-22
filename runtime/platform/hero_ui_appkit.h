/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit.h: the functions `hero_ui_appkit.swift` exports to
 * C, which `hero_ui.c`'s macOS arm calls to draw the system's own widgets by
 * AppKit. A header of the member's own, as `hero_handle_tag.h` is, and no
 * group's: a program binds `hero_ui.h`.
 *
 * HELD TO SWIFT'S OWN AT EVERY COMPILE OF THE LAYER: `swiftc` writes the
 * header its `@c` functions mean, and the layer compiles a C unit including
 * that header and this one, so a declaration here that differs from the Swift
 * is `conflicting types` and the layer does not build
 * (`selfhost/cli/layer_swift.hero`). Swift's `Int64` is `int64_t`, an
 * `UnsafePointer<UInt8>` `const uint8_t *`, an `UnsafeMutablePointer<CChar>`
 * `char *`, a `@convention(c)` function a pointer to a C function.
 *
 * Each answers a kind of `hero_ui.h`, `HERO_UI_OK` where done, and is called
 * on the main thread alone, with the arguments `hero_ui.c` has checked
 * (THE ARM there says which and what each means); `words` answers the bytes
 * of the last failure's words it wrote, and `wake` ends the wait of the
 * system's loop `pump` runs once another member has queued an event, which
 * that loop would not see. `open` takes, beside
 * `push`, the three functions a web view asks and the bridge's script, as
 * `hero_ui_web.h`'s `hero_ui_web_host` names them, each a parameter of its
 * own since Swift reads no C structure here.
 */

#ifndef HERO_UI_APPKIT_H
#define HERO_UI_APPKIT_H

#include <stdint.h>

int64_t hero_ui_appkit_here(void);

int64_t hero_ui_appkit_open(void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t), int64_t (*serve)(int64_t, const uint8_t *, int64_t, const uint8_t *, int64_t, int64_t *, const uint8_t **), int64_t (*ask)(int64_t, int64_t, const uint8_t *, int64_t), void (*gone)(int64_t), const uint8_t *bridge, int64_t bridge_count);

void hero_ui_appkit_close(void);

int64_t hero_ui_appkit_words(char *words, int64_t count);

/* Which AppKit object of a widget `hero_ui_appkit_object` answers: its
 * window (a window's own, or the window a widget or a menu stands in), the
 * view placed in its window's content, the view the person types into or
 * chooses in (a text area's `NSTextView`, a list's `NSTableView`, else the
 * placed view), a menu's `NSMenu`, or a menu's or a menu item's
 * `NSMenuItem`; or a window's own `NSWindow`, NULL for every other kind. */
#define HERO_UI_APPKIT_OBJECT_WINDOW 1
#define HERO_UI_APPKIT_OBJECT_VIEW 2
#define HERO_UI_APPKIT_OBJECT_TYPED 3
#define HERO_UI_APPKIT_OBJECT_MENU 4
#define HERO_UI_APPKIT_OBJECT_ITEM 5
#define HERO_UI_APPKIT_OBJECT_OWN_WINDOW 6

/* The AppKit object `which` of widget `id`, unretained, or NULL where the
 * native side holds no such widget or the widget no such object: what
 * `hero_ui.c` hands the arm's other Swift members, each a module of its own,
 * within the call that asked it, while the widget stands. */
void *hero_ui_appkit_object(int64_t id, int64_t which);

int64_t hero_ui_appkit_set_role(int64_t id, int64_t role);

int64_t hero_ui_appkit_set_button_key(int64_t id, int64_t key);

int64_t hero_ui_appkit_set_focus(int64_t id);

int64_t hero_ui_appkit_focus(int64_t id, int64_t *focus);

int64_t hero_ui_appkit_set_check(int64_t id, int64_t mode);

int64_t hero_ui_appkit_set_masked(int64_t id, int64_t on);

int64_t hero_ui_appkit_set_moving(int64_t id, int64_t on);

int64_t hero_ui_appkit_insets(int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom);

int64_t hero_ui_appkit_set_columns(int64_t id, const uint8_t *titles, int64_t count);

int64_t hero_ui_appkit_set_sort(int64_t id, int64_t column, int64_t ascending);

int64_t hero_ui_appkit_tree_add(int64_t id, int64_t item, int64_t under, const uint8_t *text, int64_t count, int64_t foldable);

int64_t hero_ui_appkit_tree_remove(int64_t id, int64_t item);

int64_t hero_ui_appkit_tree_unfold(int64_t id, int64_t item, int64_t on);

int64_t hero_ui_appkit_create(int64_t id, int64_t kind, int64_t parent);

int64_t hero_ui_appkit_destroy(int64_t id);

int64_t hero_ui_appkit_set_text(int64_t id, const uint8_t *bytes, int64_t count);

int64_t hero_ui_appkit_set_items(int64_t id, const uint8_t *bytes, int64_t count);

int64_t hero_ui_appkit_set_value(int64_t id, int64_t value);

int64_t hero_ui_appkit_set_range(int64_t id, int64_t least, int64_t most);

int64_t hero_ui_appkit_set_selection(int64_t id, const int64_t *indices, int64_t count);

int64_t hero_ui_appkit_set_flag(int64_t id, int64_t which, int64_t on);

int64_t hero_ui_appkit_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers);

int64_t hero_ui_appkit_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height);

int64_t hero_ui_appkit_set_size(int64_t id, int64_t which, int64_t width, int64_t height);

int64_t hero_ui_appkit_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height);

int64_t hero_ui_appkit_preferred(int64_t id, int64_t *width, int64_t *height);

int64_t hero_ui_appkit_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);

int64_t hero_ui_appkit_value(int64_t id, int64_t *value);

int64_t hero_ui_appkit_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen);

int64_t hero_ui_appkit_pump(int64_t ms);

int64_t hero_ui_appkit_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *bytes, int64_t count);

void hero_ui_appkit_wake(void);

int64_t hero_ui_appkit_set_docked(int64_t on);

int64_t hero_ui_appkit_set_accessible_name(int64_t id, const uint8_t *name, int64_t count);

int64_t hero_ui_appkit_accessible_name(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);

int64_t hero_ui_appkit_set_label_for(int64_t id, int64_t label);

int64_t hero_ui_appkit_web_load(int64_t id, const uint8_t *url, int64_t count);

int64_t hero_ui_appkit_web_post(int64_t id, const uint8_t *text, int64_t count);

#endif
