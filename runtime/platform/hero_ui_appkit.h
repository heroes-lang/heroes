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
 * of the last failure's words it wrote, and `dialog_paths` the bytes of the
 * last file dialog's paths, writing them where they fit. `open` takes, beside
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

int64_t hero_ui_appkit_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *bytes, int64_t count, int64_t *answer);

int64_t hero_ui_appkit_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes);

int64_t hero_ui_appkit_dialog_paths(uint8_t *buffer, int64_t count);

int64_t hero_ui_appkit_web_load(int64_t id, const uint8_t *url, int64_t count);

int64_t hero_ui_appkit_web_post(int64_t id, const uint8_t *text, int64_t count);

#endif
