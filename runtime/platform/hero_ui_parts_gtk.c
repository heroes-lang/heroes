/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_parts_gtk.c: a drawing area's parts read by a screen reader on GTK
 * 4, `hero_ui_parts_gtk.h`'s calls; what GTK offers and refuses is that
 * header's. design.md §1.11: everything comes from C, here GTK 4, GLib and
 * GObject, found by `dlsym` on GTK's library as the GTK arm finds its own, so
 * a build needs none of their headers.
 *
 * THREE TYPES OF THIS MEMBER'S, registered once with GObject:
 * `HeroUiCanvasArea`, a `GtkDrawingArea` whose `GtkAccessible` lists the
 * parts as its children; `HeroUiCanvasPart`, one `GObject` per part
 * implementing `GtkAccessible` (its role, its context made by
 * `gtk_at_context_create`, its parent, children and next sibling by the
 * `within` tree, its bounds from its box, its focus); and
 * `HeroUiCanvasRangePart`, a part that is also a `GtkAccessibleRange`, for a
 * slider and a progress bar, whose value setter is a reader's step. Their
 * fields stand after the parent type's instance, at the offset GObject's
 * `g_type_query` gives, since no header of GTK's is read.
 *
 * WHO OUTLIVES WHOM. Each area holds one record (`area`); the record holds a
 * reference to each part and to the image standing for the area, and each
 * part one hold on the record, so a part GTK keeps a moment longer (a
 * reader's call in flight) never reads a record that is gone. The record is
 * freed when the area is finalised and no part holds it. A part's GTK
 * context is its own, released as the part is disposed, which takes the part
 * back from the reader.
 *
 * WHAT CHANGES WHEN THE PARTS ARE GIVEN AGAIN. A list of the same parts by
 * role and by holder (a value, a name, a box or a state changed) updates the
 * parts in place, so a reader keeps its place and hears the change; any
 * other list replaces them all. The reader's focus follows the part marked
 * focused while the area has the keys, told when that part's number changes
 * and when the area takes the keys.
 */

#include "hero_ui_parts_gtk.h"

/* This member is GTK 4's: `runtime.c` includes it on every system, so everything below, the POSIX
 * headers (`<dlfcn.h>`, `<pthread.h>`) first of all, stands inside the guard `hero_ui_canvas_gtk.c`,
 * its only caller, keeps, and the file compiles to nothing on Windows and macOS. Defect d-gap-os2-1: it
 * stood unguarded and no program built on Windows. */
#if !defined(_WIN32) && !defined(__APPLE__)

#include <dlfcn.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"
#include "hero_layer_maths.h"
#include "hero_ui.h"
#include "hero_ui_arm.h"

/* The numbers of GTK's headers this member passes, read in GTK 4.18.6's. */
#define HERO_GTK_ROLE_BUTTON 3
#define HERO_GTK_ROLE_CHECKBOX 6
#define HERO_GTK_ROLE_GROUP 18
#define HERO_GTK_ROLE_IMG 20
#define HERO_GTK_ROLE_LABEL 22
#define HERO_GTK_ROLE_LINK 25
#define HERO_GTK_ROLE_PROGRESS_BAR 44
#define HERO_GTK_ROLE_RADIO 45
#define HERO_GTK_ROLE_SLIDER 59
#define HERO_GTK_PROPERTY_DESCRIPTION 1
#define HERO_GTK_PROPERTY_LABEL 4
#define HERO_GTK_PROPERTY_READ_ONLY 11
#define HERO_GTK_PROPERTY_VALUE_MAX 15
#define HERO_GTK_PROPERTY_VALUE_MIN 16
#define HERO_GTK_PROPERTY_VALUE_NOW 17
#define HERO_GTK_STATE_CHECKED 1
#define HERO_GTK_PLATFORM_FOCUSABLE 0
#define HERO_GTK_PLATFORM_FOCUSED 1

/* A part's record in `hero_ui.h`'s list: role, states, within, the box, the
 * three numbers and the two lengths. */
#define HERO_UI_PARTS_GTK_RECORD 48

/* The property id this member gives `GtkAccessible`'s "accessible-role". */
#define HERO_UI_PARTS_GTK_ROLE_PROPERTY 1u

/* GObject's public structures, as `gtype.h` and `gobject.h` lay them out, and
 * the two interfaces' as `gtkaccessible.h` and `gtkaccessiblerange.h` do; the
 * sizes asserted are the ones measured in the Linux image with the headers. */
typedef struct {
    size_t type;
    const char *type_name;
    unsigned class_size;
    unsigned instance_size;
} hero_gtype_query;

typedef struct {
    uint16_t class_size;
    void (*base_init)(void *klass);
    void (*base_finalize)(void *klass);
    void (*class_init)(void *klass, void *data);
    void (*class_finalize)(void *klass, void *data);
    const void *class_data;
    uint16_t instance_size;
    uint16_t n_preallocs;
    void (*instance_init)(void *instance, void *klass);
    const void *value_table;
} hero_gtype_info;

typedef struct {
    void (*interface_init)(void *iface, void *data);
    void (*interface_finalize)(void *iface, void *data);
    void *interface_data;
} hero_ginterface_info;

typedef struct {
    size_t g_type;
    void *construct_properties;
    void *(*constructor)(size_t type, unsigned count, void *properties);
    void (*set_property)(void *object, unsigned id, const void *value, void *spec);
    void (*get_property)(void *object, unsigned id, void *value, void *spec);
    void (*dispose)(void *object);
    void (*finalize)(void *object);
} hero_gobject_class_head;

typedef struct {
    size_t g_type;
    size_t g_instance_type;
    void *(*get_at_context)(void *self);
    int (*get_platform_state)(void *self, int state);
    void *(*get_accessible_parent)(void *self);
    void *(*get_first_accessible_child)(void *self);
    void *(*get_next_accessible_sibling)(void *self);
    int (*get_bounds)(void *self, int *x, int *y, int *width, int *height);
} hero_gtk_accessible_iface;

typedef struct {
    size_t g_type;
    size_t g_instance_type;
    int (*set_current_value)(void *self, double value);
} hero_gtk_range_iface;

_Static_assert(sizeof(void *) == 8 && sizeof(size_t) == 8, "the layouts below are LP64's, GObject's own on every system this member builds for");
_Static_assert(sizeof(hero_gtype_query) == 24 && sizeof(hero_gtype_info) == 72 && sizeof(hero_ginterface_info) == 24, "GObject's type structures as the image's headers measured them");
_Static_assert(offsetof(hero_gobject_class_head, set_property) == 24 && offsetof(hero_gobject_class_head, get_property) == 32 && offsetof(hero_gobject_class_head, finalize) == 48, "GObjectClass's members as the image's headers measured them");
_Static_assert(sizeof(hero_gtk_accessible_iface) == 64 && offsetof(hero_gtk_accessible_iface, get_bounds) == 56 && sizeof(hero_gtk_range_iface) == 24, "GtkAccessibleInterface and GtkAccessibleRangeInterface as GTK 4.18.6's headers measured them");

typedef struct hero_ui_parts_gtk_area hero_ui_parts_gtk_area;

/* One part, or the image standing for an area with none (number 0). */
typedef struct {
    hero_ui_parts_gtk_area *owner;
    void *context;
    void *parent;
    void *first;
    void *next;
    int64_t number;
    int gtk_role;
    uint8_t role;
    uint8_t states;
    uint16_t within;
    float box[4];
    double value;
    double least;
    double most;
    uint8_t *name;
    uint16_t name_length;
    uint8_t *description;
    uint16_t description_length;
    /* Set as the part is disposed: GTK asks a part's role again while its
     * context is taken back, and a disposed part makes no new one. */
    bool disposed;
} hero_ui_parts_gtk_part;

struct hero_ui_parts_gtk_area {
    /* The area, NULL once it is finalised; its number. */
    void *area;
    int64_t id;
    /* The area's own hold, one per live part and one per pending turn. */
    int64_t holds;
    bool forgotten;
    /* The parts in the order given, a reference each; the image standing
     * for the area while it has none, a reference; the area's first child. */
    void **parts;
    int64_t count;
    void *image;
    void *first;
    /* The area's accessible name, NUL-terminated, the image's. */
    char *name;
    /* The number of the part marked focused, 0 for none. */
    int64_t focused;
};

typedef void (*hero_ui_parts_gtk_callback)(void);

typedef struct {
    size_t (*type_register_static)(size_t parent, const char *name, const hero_gtype_info *info, int flags);
    void (*type_add_interface_static)(size_t type, size_t iface, const hero_ginterface_info *info);
    void (*type_query)(size_t type, hero_gtype_query *query);
    void *(*type_interface_peek)(void *klass, size_t iface);
    void *(*type_interface_peek_parent)(void *iface);
    void *(*type_class_peek)(size_t type);
    size_t (*object_get_type)(void);
    void (*object_class_override_property)(void *klass, unsigned id, const char *name);
    int (*value_get_enum)(const void *value);
    void (*value_set_enum)(void *value, int number);
    void *(*object_new)(size_t type, const char *first, ...);
    void *(*object_ref)(void *object);
    void (*object_unref)(void *object);
    unsigned (*idle_add)(int (*run)(void *data), void *data);
    unsigned long (*signal_connect_data)(void *instance, const char *signal, hero_ui_parts_gtk_callback handler, void *data, void *destroy, int flags);
    size_t (*drawing_area_get_type)(void);
    size_t (*accessible_get_type)(void);
    size_t (*accessible_range_get_type)(void);
    void *(*at_context_create)(int role, void *accessible, void *display);
    void *(*widget_get_display)(void *widget);
    int (*widget_has_focus)(void *widget);
    int (*widget_get_width)(void *widget);
    int (*widget_get_height)(void *widget);
    void *(*event_controller_focus_new)(void);
    void (*widget_add_controller)(void *widget, void *controller);
    void (*accessible_set_accessible_parent)(void *self, void *parent, void *next);
    void (*accessible_update_property)(void *self, int first, ...);
    void (*accessible_reset_property)(void *self, int property);
    void (*accessible_update_state)(void *self, int first, ...);
    void (*accessible_update_platform_state)(void *self, int state);
    int (*accessible_get_accessible_role)(void *self);
    void *(*accessible_get_accessible_parent)(void *self);
    void *(*accessible_get_first_accessible_child)(void *self);
    void *(*accessible_get_next_accessible_sibling)(void *self);
    int (*accessible_get_platform_state)(void *self, int state);
    int (*accessible_get_bounds)(void *self, int *x, int *y, int *width, int *height);
    /* The three types, and where each type's fields stand in its instance. */
    size_t area_type;
    size_t part_type;
    size_t range_type;
    size_t area_offset;
    size_t part_offset;
} hero_ui_parts_gtk_functions;

/* Found and registered once inside `pthread_once` and only read after it,
 * as `hero_ui_gtk.c`'s functions are: the functions and the types are the
 * process's. */
static hero_ui_parts_gtk_functions hero_ui_parts_gtk_found_functions;
static bool hero_ui_parts_gtk_found;
static pthread_once_t hero_ui_parts_gtk_once = PTHREAD_ONCE_INIT;

static const hero_ui_parts_gtk_functions *hero_ui_parts_gtk_loaded(void);

/* -- FINDING THE CALLS, AND THE TYPES ---------------------------------------- */

/* One function by name into `*slot`, its address copied by its bytes, as
 * `hero_ui_canvas_gtk.c` does (POSIX makes them the same for both). */
static bool hero_ui_parts_gtk_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_UI_PARTS_GTK_FIND(field, name) ok = hero_ui_parts_gtk_find(gtk, name, &hero_ui_parts_gtk_found_functions.field, sizeof hero_ui_parts_gtk_found_functions.field) && ok

/* The record a part's or an area's instance holds, past its parent type's. */
static hero_ui_parts_gtk_part *hero_ui_parts_gtk_part_of(void *object) {
    return (hero_ui_parts_gtk_part *)(void *)((char *)object + hero_ui_parts_gtk_found_functions.part_offset);
}

static hero_ui_parts_gtk_area **hero_ui_parts_gtk_area_slot(void *area) {
    return (hero_ui_parts_gtk_area **)(void *)((char *)area + hero_ui_parts_gtk_found_functions.area_offset);
}

/* `size` rounded up to a multiple of 16, where a type's own fields start. */
static size_t hero_ui_parts_gtk_aligned(size_t size) {
    return (size + 15u) & ~(size_t)15u;
}

static void hero_ui_parts_gtk_release(hero_ui_parts_gtk_area *record);

/* -- THE PART'S GtkAccessible ------------------------------------------------ */

static void *hero_ui_parts_gtk_ref(void *object) {
    return object != NULL ? hero_ui_parts_gtk_found_functions.object_ref(object) : NULL;
}

static void *hero_ui_parts_gtk_part_context(void *self) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(self);

    if (part->context == NULL && !part->disposed && part->owner != NULL && part->owner->area != NULL) {
        /* NULL where GTK's accessibility is turned off (`GTK_A11Y=none`):
         * the part then answers through this interface alone. */
        part->context = f->at_context_create(part->gtk_role, self, f->widget_get_display(part->owner->area));
    }
    return hero_ui_parts_gtk_ref(part->context);
}

static int hero_ui_parts_gtk_part_state(void *self, int state) {
    const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(self);
    const void *area = part->owner != NULL ? part->owner->area : NULL;

    if (state == HERO_GTK_PLATFORM_FOCUSABLE) {
        return (part->states & HERO_UI_PART_FOCUSABLE) != 0;
    }
    if (state == HERO_GTK_PLATFORM_FOCUSED) {
        return (part->states & HERO_UI_PART_FOCUSED) != 0 && area != NULL && hero_ui_parts_gtk_found_functions.widget_has_focus((void *)area);
    }
    return 0;
}

static void *hero_ui_parts_gtk_part_parent(void *self) {
    const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(self);

    /* The area's own parent link is dropped as it is finalised. */
    if (part->owner == NULL || part->owner->area == NULL) {
        return NULL;
    }
    return hero_ui_parts_gtk_ref(part->parent);
}

static void *hero_ui_parts_gtk_part_first(void *self) {
    return hero_ui_parts_gtk_ref(hero_ui_parts_gtk_part_of(self)->first);
}

static void *hero_ui_parts_gtk_part_next(void *self) {
    return hero_ui_parts_gtk_ref(hero_ui_parts_gtk_part_of(self)->next);
}

/* The part's box in whole points relative to its holder's (the area's own
 * for a part of the area itself), as GTK asks it, every edge's point
 * inside; the image standing for an area is the area whole. */
static int hero_ui_parts_gtk_part_bounds(void *self, int *x, int *y, int *width, int *height) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(self);
    double left = part->box[0];
    double top = part->box[1];
    double right;
    double bottom;

    if (part->owner == NULL || part->owner->area == NULL) {
        return 0;
    }
    if (part->number == 0) {
        *x = 0;
        *y = 0;
        *width = f->widget_get_width(part->owner->area);
        *height = f->widget_get_height(part->owner->area);
        return 1;
    }
    right = left + part->box[2];
    bottom = top + part->box[3];
    if (part->within > 0 && part->within <= part->owner->count) {
        const hero_ui_parts_gtk_part *holder = hero_ui_parts_gtk_part_of(part->owner->parts[part->within - 1]);

        left -= holder->box[0];
        right -= holder->box[0];
        top -= holder->box[1];
        bottom -= holder->box[1];
    }
    /* `hero_ui.c` bounds every number of a part to a binary32's, within an
     * int's reach once floored and ceiled at points this size. */
    *x = (int)hero_layer_floor(left);
    *y = (int)hero_layer_floor(top);
    *width = (int)hero_layer_ceil(right) - *x;
    *height = (int)hero_layer_ceil(bottom) - *y;
    return 1;
}

static void hero_ui_parts_gtk_part_accessible(void *iface, void *data) {
    hero_gtk_accessible_iface *accessible = iface;

    (void)data;
    accessible->get_at_context = hero_ui_parts_gtk_part_context;
    accessible->get_platform_state = hero_ui_parts_gtk_part_state;
    accessible->get_accessible_parent = hero_ui_parts_gtk_part_parent;
    accessible->get_first_accessible_child = hero_ui_parts_gtk_part_first;
    accessible->get_next_accessible_sibling = hero_ui_parts_gtk_part_next;
    accessible->get_bounds = hero_ui_parts_gtk_part_bounds;
}

/* A reader setting a slider's value: a step up where it is above the value
 * the program gave, down where below, told and never applied (the step is
 * the program's); nothing for an equal value, and a progress bar's value is
 * read alone. */
static int hero_ui_parts_gtk_part_set_value(void *self, double value) {
    const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(self);

    if (part->role != HERO_UI_PART_SLIDER || isnan(value) || part->owner == NULL || part->owner->forgotten) {
        return 0;
    }
    if (value > part->value) {
        hero_ui_tell(HERO_UI_EVENT_PART, part->owner->id, part->number, HERO_UI_PART_INCREASE);
    } else if (value < part->value) {
        hero_ui_tell(HERO_UI_EVENT_PART, part->owner->id, part->number, HERO_UI_PART_DECREASE);
    }
    return 1;
}

static void hero_ui_parts_gtk_part_range(void *iface, void *data) {
    (void)data;
    ((hero_gtk_range_iface *)iface)->set_current_value = hero_ui_parts_gtk_part_set_value;
}

/* "accessible-role", which `GtkAccessible` asks every implementation for:
 * the part's, fixed as the part is made. */
static void hero_ui_parts_gtk_part_set_property(void *object, unsigned id, const void *value, void *spec) {
    (void)spec;
    if (id == HERO_UI_PARTS_GTK_ROLE_PROPERTY && hero_ui_parts_gtk_part_of(object)->context == NULL) {
        hero_ui_parts_gtk_part_of(object)->gtk_role = hero_ui_parts_gtk_found_functions.value_get_enum(value);
    }
}

static void hero_ui_parts_gtk_part_get_property(void *object, unsigned id, void *value, void *spec) {
    (void)spec;
    if (id == HERO_UI_PARTS_GTK_ROLE_PROPERTY) {
        hero_ui_parts_gtk_found_functions.value_set_enum(value, hero_ui_parts_gtk_part_of(object)->gtk_role);
    }
}

/* The context goes first, while the part still answers it: disposing it
 * takes the part back from the reader. */
static void hero_ui_parts_gtk_part_dispose(void *object) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(object);
    void *context = part->context;

    part->disposed = true;
    part->context = NULL;
    if (context != NULL) {
        f->object_unref(context);
    }
    ((hero_gobject_class_head *)f->type_class_peek(f->object_get_type()))->dispose(object);
}

static void hero_ui_parts_gtk_part_finalize(void *object) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(object);

    if (part->context != NULL) {
        f->object_unref(part->context);
        part->context = NULL;
    }
    hero_layer_free(part->name);
    hero_layer_free(part->description);
    if (part->owner != NULL) {
        hero_ui_parts_gtk_release(part->owner);
    }
    ((hero_gobject_class_head *)f->type_class_peek(f->object_get_type()))->finalize(object);
}

static void hero_ui_parts_gtk_part_class(void *klass, void *data) {
    hero_gobject_class_head *head = klass;

    (void)data;
    head->set_property = hero_ui_parts_gtk_part_set_property;
    head->get_property = hero_ui_parts_gtk_part_get_property;
    head->dispose = hero_ui_parts_gtk_part_dispose;
    head->finalize = hero_ui_parts_gtk_part_finalize;
    hero_ui_parts_gtk_found_functions.object_class_override_property(klass, HERO_UI_PARTS_GTK_ROLE_PROPERTY, "accessible-role");
}

/* -- THE AREA'S GtkAccessible ------------------------------------------------ */

/* The area's first child for a reader: its first part of its own, or the
 * image standing for it; a drawing area holds no widget child. */
static void *hero_ui_parts_gtk_area_first(void *self) {
    const hero_ui_parts_gtk_area *record = *hero_ui_parts_gtk_area_slot(self);

    return record != NULL ? hero_ui_parts_gtk_ref(record->first) : NULL;
}

/* Every other answer is the widget's, GtkWidget's own implementation. */
static void hero_ui_parts_gtk_area_accessible(void *iface, void *data) {
    hero_gtk_accessible_iface *accessible = iface;
    const hero_gtk_accessible_iface *widget = hero_ui_parts_gtk_found_functions.type_interface_peek_parent(iface);

    (void)data;
    if (widget != NULL) {
        accessible->get_at_context = widget->get_at_context;
        accessible->get_platform_state = widget->get_platform_state;
        accessible->get_accessible_parent = widget->get_accessible_parent;
        accessible->get_next_accessible_sibling = widget->get_next_accessible_sibling;
        accessible->get_bounds = widget->get_bounds;
    }
    accessible->get_first_accessible_child = hero_ui_parts_gtk_area_first;
}

static void hero_ui_parts_gtk_drop_all(hero_ui_parts_gtk_area *record);

static void hero_ui_parts_gtk_area_finalize(void *object) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    hero_ui_parts_gtk_area *record = *hero_ui_parts_gtk_area_slot(object);

    if (record != NULL) {
        /* The parts' parent link reads NULL from here, so none asks this
         * area again as it is taken back. */
        record->area = NULL;
        record->forgotten = true;
        hero_ui_parts_gtk_drop_all(record);
        *hero_ui_parts_gtk_area_slot(object) = NULL;
        hero_ui_parts_gtk_release(record);
    }
    ((hero_gobject_class_head *)f->type_class_peek(f->drawing_area_get_type()))->finalize(object);
}

static void hero_ui_parts_gtk_area_class(void *klass, void *data) {
    (void)data;
    ((hero_gobject_class_head *)klass)->finalize = hero_ui_parts_gtk_area_finalize;
}

/* Registers the three types, false where GObject refused one. */
static bool hero_ui_parts_gtk_register(hero_ui_parts_gtk_functions *f) {
    hero_gtype_query area = {0, NULL, 0, 0};
    hero_gtype_query object = {0, NULL, 0, 0};
    hero_gtype_info info;
    const hero_ginterface_info area_accessible = {hero_ui_parts_gtk_area_accessible, NULL, NULL};
    const hero_ginterface_info part_accessible = {hero_ui_parts_gtk_part_accessible, NULL, NULL};
    const hero_ginterface_info part_range = {hero_ui_parts_gtk_part_range, NULL, NULL};
    size_t area_size;
    size_t part_size;

    f->type_query(f->drawing_area_get_type(), &area);
    f->type_query(f->object_get_type(), &object);
    f->area_offset = hero_ui_parts_gtk_aligned(area.instance_size);
    f->part_offset = hero_ui_parts_gtk_aligned(object.instance_size);
    area_size = f->area_offset + sizeof(hero_ui_parts_gtk_area *);
    part_size = f->part_offset + sizeof(hero_ui_parts_gtk_part);
    if (area.class_size == 0 || object.class_size == 0 || area.class_size > UINT16_MAX || object.class_size > UINT16_MAX || area_size > UINT16_MAX || part_size > UINT16_MAX) {
        return false;
    }
    memset(&info, 0, sizeof info);
    info.class_size = (uint16_t)area.class_size;
    info.class_init = hero_ui_parts_gtk_area_class;
    info.instance_size = (uint16_t)area_size;
    f->area_type = f->type_register_static(f->drawing_area_get_type(), "HeroUiCanvasArea", &info, 0);
    memset(&info, 0, sizeof info);
    info.class_size = (uint16_t)object.class_size;
    info.class_init = hero_ui_parts_gtk_part_class;
    info.instance_size = (uint16_t)part_size;
    f->part_type = f->type_register_static(f->object_get_type(), "HeroUiCanvasPart", &info, 0);
    if (f->area_type == 0 || f->part_type == 0) {
        return false;
    }
    memset(&info, 0, sizeof info);
    info.class_size = (uint16_t)object.class_size;
    info.instance_size = (uint16_t)part_size;
    f->range_type = f->type_register_static(f->part_type, "HeroUiCanvasRangePart", &info, 0);
    if (f->range_type == 0) {
        return false;
    }
    f->type_add_interface_static(f->area_type, f->accessible_get_type(), &area_accessible);
    f->type_add_interface_static(f->part_type, f->accessible_get_type(), &part_accessible);
    f->type_add_interface_static(f->range_type, f->accessible_range_get_type(), &part_range);
    return true;
}

static void hero_ui_parts_gtk_load(void) {
    bool ok = true;
    void *gtk = dlopen("libgtk-4.so.1", RTLD_NOW | RTLD_LOCAL);

    if (gtk == NULL) {
        return;
    }
    HERO_UI_PARTS_GTK_FIND(type_register_static, "g_type_register_static");
    HERO_UI_PARTS_GTK_FIND(type_add_interface_static, "g_type_add_interface_static");
    HERO_UI_PARTS_GTK_FIND(type_query, "g_type_query");
    HERO_UI_PARTS_GTK_FIND(type_interface_peek, "g_type_interface_peek");
    HERO_UI_PARTS_GTK_FIND(type_interface_peek_parent, "g_type_interface_peek_parent");
    HERO_UI_PARTS_GTK_FIND(type_class_peek, "g_type_class_peek");
    HERO_UI_PARTS_GTK_FIND(object_get_type, "g_object_get_type");
    HERO_UI_PARTS_GTK_FIND(object_class_override_property, "g_object_class_override_property");
    HERO_UI_PARTS_GTK_FIND(value_get_enum, "g_value_get_enum");
    HERO_UI_PARTS_GTK_FIND(value_set_enum, "g_value_set_enum");
    HERO_UI_PARTS_GTK_FIND(object_new, "g_object_new");
    HERO_UI_PARTS_GTK_FIND(object_ref, "g_object_ref");
    HERO_UI_PARTS_GTK_FIND(object_unref, "g_object_unref");
    HERO_UI_PARTS_GTK_FIND(idle_add, "g_idle_add");
    HERO_UI_PARTS_GTK_FIND(signal_connect_data, "g_signal_connect_data");
    HERO_UI_PARTS_GTK_FIND(drawing_area_get_type, "gtk_drawing_area_get_type");
    HERO_UI_PARTS_GTK_FIND(accessible_get_type, "gtk_accessible_get_type");
    HERO_UI_PARTS_GTK_FIND(accessible_range_get_type, "gtk_accessible_range_get_type");
    HERO_UI_PARTS_GTK_FIND(at_context_create, "gtk_at_context_create");
    HERO_UI_PARTS_GTK_FIND(widget_get_display, "gtk_widget_get_display");
    HERO_UI_PARTS_GTK_FIND(widget_has_focus, "gtk_widget_has_focus");
    HERO_UI_PARTS_GTK_FIND(widget_get_width, "gtk_widget_get_width");
    HERO_UI_PARTS_GTK_FIND(widget_get_height, "gtk_widget_get_height");
    HERO_UI_PARTS_GTK_FIND(event_controller_focus_new, "gtk_event_controller_focus_new");
    HERO_UI_PARTS_GTK_FIND(widget_add_controller, "gtk_widget_add_controller");
    HERO_UI_PARTS_GTK_FIND(accessible_set_accessible_parent, "gtk_accessible_set_accessible_parent");
    HERO_UI_PARTS_GTK_FIND(accessible_update_property, "gtk_accessible_update_property");
    HERO_UI_PARTS_GTK_FIND(accessible_reset_property, "gtk_accessible_reset_property");
    HERO_UI_PARTS_GTK_FIND(accessible_update_state, "gtk_accessible_update_state");
    HERO_UI_PARTS_GTK_FIND(accessible_update_platform_state, "gtk_accessible_update_platform_state");
    HERO_UI_PARTS_GTK_FIND(accessible_get_accessible_role, "gtk_accessible_get_accessible_role");
    HERO_UI_PARTS_GTK_FIND(accessible_get_accessible_parent, "gtk_accessible_get_accessible_parent");
    HERO_UI_PARTS_GTK_FIND(accessible_get_first_accessible_child, "gtk_accessible_get_first_accessible_child");
    HERO_UI_PARTS_GTK_FIND(accessible_get_next_accessible_sibling, "gtk_accessible_get_next_accessible_sibling");
    HERO_UI_PARTS_GTK_FIND(accessible_get_platform_state, "gtk_accessible_get_platform_state");
    HERO_UI_PARTS_GTK_FIND(accessible_get_bounds, "gtk_accessible_get_bounds");
    hero_ui_parts_gtk_found = ok && hero_ui_parts_gtk_register(&hero_ui_parts_gtk_found_functions);
}

/* The functions, or NULL where GTK 4's library or one of them is missing. */
static const hero_ui_parts_gtk_functions *hero_ui_parts_gtk_loaded(void) {
    (void)pthread_once(&hero_ui_parts_gtk_once, hero_ui_parts_gtk_load);
    return hero_ui_parts_gtk_found ? &hero_ui_parts_gtk_found_functions : NULL;
}

/* `kind`, its words written into `words`. */
static int64_t hero_ui_parts_gtk_said(int64_t kind, char *words, size_t room, const char *said) {
    if (words != NULL && room > 0) {
        (void)snprintf(words, room, "%s", said);
    }
    return kind;
}

static const char hero_ui_parts_gtk_older[] = "GTK 4's library lacks a call a drawing area's parts make for a screen reader (GTK 4.10's gtk_accessible_get_bounds and its custom accessibles among them)";

/* The record of `area`, NULL where it is of no type of this member's. */
static hero_ui_parts_gtk_area *hero_ui_parts_gtk_record(void *area) {
    const hero_ui_parts_gtk_functions *f = hero_ui_parts_gtk_loaded();

    if (f == NULL || area == NULL || *(size_t *)*(void **)area != f->area_type) {
        return NULL;
    }
    return *hero_ui_parts_gtk_area_slot(area);
}

/* -- THE RECORD -------------------------------------------------------------- */

static void hero_ui_parts_gtk_release(hero_ui_parts_gtk_area *record) {
    record->holds -= 1;
    if (record->holds == 0) {
        hero_layer_free(record->parts);
        hero_layer_free(record->name);
        hero_layer_free(record);
    }
}

/* A part of `owner` of role `role` (`hero_ui.h`'s, 0 for the image standing
 * for the area), the GTK type and role its role takes; NULL where GObject
 * made none. */
static void *hero_ui_parts_gtk_new_part(hero_ui_parts_gtk_area *owner, uint8_t role) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    static const int gtk_roles[] = {HERO_GTK_ROLE_IMG, HERO_GTK_ROLE_GROUP, HERO_GTK_ROLE_LABEL, HERO_GTK_ROLE_IMG, HERO_GTK_ROLE_BUTTON, HERO_GTK_ROLE_CHECKBOX, HERO_GTK_ROLE_RADIO, HERO_GTK_ROLE_SLIDER, HERO_GTK_ROLE_PROGRESS_BAR, HERO_GTK_ROLE_LINK};
    bool ranged = role == HERO_UI_PART_SLIDER || role == HERO_UI_PART_PROGRESS;
    void *made;
    hero_ui_parts_gtk_part *part;

    if (role > HERO_UI_PART_LINK) {
        return NULL;
    }
    made = f->object_new(ranged ? f->range_type : f->part_type, (const char *)NULL);
    if (made == NULL) {
        return NULL;
    }
    part = hero_ui_parts_gtk_part_of(made);
    part->owner = owner;
    part->gtk_role = gtk_roles[role];
    part->role = role;
    owner->holds += 1;
    return made;
}

/* Gives part `object` its name, description, numbers and checked state as
 * GTK's properties and states, which GTK tells a reader where they change. */
static void hero_ui_parts_gtk_describe(void *object, const char *name, const char *description) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(object);

    if (name != NULL && name[0] != '\0') {
        f->accessible_update_property(object, HERO_GTK_PROPERTY_LABEL, name, -1);
    } else {
        f->accessible_reset_property(object, HERO_GTK_PROPERTY_LABEL);
    }
    if (description != NULL && description[0] != '\0') {
        f->accessible_update_property(object, HERO_GTK_PROPERTY_DESCRIPTION, description, -1);
    } else {
        f->accessible_reset_property(object, HERO_GTK_PROPERTY_DESCRIPTION);
    }
    if (part->role == HERO_UI_PART_SLIDER || part->role == HERO_UI_PART_PROGRESS) {
        f->accessible_update_property(object, HERO_GTK_PROPERTY_VALUE_MIN, part->least, HERO_GTK_PROPERTY_VALUE_MAX, part->most, HERO_GTK_PROPERTY_VALUE_NOW, part->value, -1);
    }
    if (part->role == HERO_UI_PART_PROGRESS) {
        f->accessible_update_property(object, HERO_GTK_PROPERTY_READ_ONLY, 1, -1);
    }
    if (part->role == HERO_UI_PART_CHECK_BOX || part->role == HERO_UI_PART_RADIO) {
        f->accessible_update_state(object, HERO_GTK_STATE_CHECKED, (part->states & HERO_UI_PART_CHECKED) != 0 ? 1 : 0, -1);
    }
}

/* Takes every part and the image back from the reader: each unlinked, and
 * its reference given back, which disposes its context. */
static void hero_ui_parts_gtk_drop_all(hero_ui_parts_gtk_area *record) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    void **parts = record->parts;
    int64_t count = record->count;
    void *image = record->image;

    record->parts = NULL;
    record->count = 0;
    record->image = NULL;
    record->first = NULL;
    record->focused = 0;
    for (int64_t i = 0; i < count; i += 1) {
        hero_ui_parts_gtk_part_of(parts[i])->next = NULL;
        hero_ui_parts_gtk_part_of(parts[i])->first = NULL;
    }
    for (int64_t i = 0; i < count; i += 1) {
        f->object_unref(parts[i]);
    }
    hero_layer_free(parts);
    if (image != NULL) {
        f->object_unref(image);
    }
}

/* Makes the image standing for an area with no parts, named by the area's
 * accessible name, and ties it under the area; false where memory is
 * short. */
static bool hero_ui_parts_gtk_stand_in(hero_ui_parts_gtk_area *record) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    void *image = hero_ui_parts_gtk_new_part(record, 0);

    if (image == NULL) {
        return false;
    }
    hero_ui_parts_gtk_part_of(image)->parent = record->area;
    hero_ui_parts_gtk_describe(image, record->name, NULL);
    record->image = image;
    record->first = image;
    f->accessible_set_accessible_parent(image, record->area, NULL);
    return true;
}

/* Tells the reader where its focus stands: the part marked focused, once
 * the area has the keys. GTK tells the area's own focus first, and this
 * runs a turn after it, so the reader lands on the part. */
static int hero_ui_parts_gtk_focus_turn(void *data) {
    hero_ui_parts_gtk_area *record = data;

    if (!record->forgotten && record->area != NULL && record->focused > 0 && record->focused <= record->count) {
        hero_ui_parts_gtk_found_functions.accessible_update_platform_state(record->parts[record->focused - 1], HERO_GTK_PLATFORM_FOCUSED);
    }
    hero_ui_parts_gtk_release(record);
    return 0;
}

static void hero_ui_parts_gtk_focus_later(hero_ui_parts_gtk_area *record) {
    record->holds += 1;
    (void)hero_ui_parts_gtk_found_functions.idle_add(hero_ui_parts_gtk_focus_turn, record);
}

/* The area took the keys, or gave them up: the focused part's state read
 * anew. */
static void hero_ui_parts_gtk_area_focus(void *controller, void *data) {
    hero_ui_parts_gtk_area *record = data;

    (void)controller;
    if (!record->forgotten && record->focused > 0) {
        hero_ui_parts_gtk_focus_later(record);
    }
}

/* -- READING THE RECORDS ----------------------------------------------------- */

static uint16_t hero_ui_parts_gtk_u16(const uint8_t *at) {
    return (uint16_t)(at[0] | (unsigned)at[1] << 8);
}

static float hero_ui_parts_gtk_r32(const uint8_t *at) {
    uint32_t bits = (uint32_t)at[0] | (uint32_t)at[1] << 8 | (uint32_t)at[2] << 16 | (uint32_t)at[3] << 24;
    float value;

    memcpy(&value, &bits, sizeof value);
    return value;
}

static double hero_ui_parts_gtk_d64(const uint8_t *at) {
    uint64_t bits = 0;
    double value;

    for (int i = 7; i >= 0; i -= 1) {
        bits = bits << 8 | at[i];
    }
    memcpy(&value, &bits, sizeof value);
    return value;
}

static void hero_ui_parts_gtk_put_u16(uint8_t *at, uint16_t value) {
    at[0] = (uint8_t)(value & 0xFFu);
    at[1] = (uint8_t)(value >> 8);
}

static void hero_ui_parts_gtk_put_r32(uint8_t *at, float value) {
    uint32_t bits;

    memcpy(&bits, &value, sizeof bits);
    for (int i = 0; i < 4; i += 1) {
        at[i] = (uint8_t)(bits >> (8 * i) & 0xFFu);
    }
}

static void hero_ui_parts_gtk_put_d64(uint8_t *at, double value) {
    uint64_t bits;

    memcpy(&bits, &value, sizeof bits);
    for (int i = 0; i < 8; i += 1) {
        at[i] = (uint8_t)(bits >> (8 * i) & 0xFFu);
    }
}

/* One record of the list at `at`, `left` bytes on, into `part` (its name and
 * description copied, NUL-terminated); its length, 0 where the list is
 * malformed or memory short. */
static int64_t hero_ui_parts_gtk_parse(const uint8_t *at, int64_t left, hero_ui_parts_gtk_part *part, bool *short_of_memory) {
    uint16_t name_length;
    uint16_t description_length;

    if (left < HERO_UI_PARTS_GTK_RECORD) {
        return 0;
    }
    name_length = hero_ui_parts_gtk_u16(at + 44);
    description_length = hero_ui_parts_gtk_u16(at + 46);
    if (left - HERO_UI_PARTS_GTK_RECORD < (int64_t)name_length + description_length) {
        return 0;
    }
    part->role = at[0];
    part->states = at[1];
    part->within = hero_ui_parts_gtk_u16(at + 2);
    for (int i = 0; i < 4; i += 1) {
        part->box[i] = hero_ui_parts_gtk_r32(at + 4 + 4 * i);
    }
    part->value = hero_ui_parts_gtk_d64(at + 20);
    part->least = hero_ui_parts_gtk_d64(at + 28);
    part->most = hero_ui_parts_gtk_d64(at + 36);
    part->name = hero_layer_alloc_zero((size_t)name_length + 1, 1);
    part->description = hero_layer_alloc_zero((size_t)description_length + 1, 1);
    if (part->name == NULL || part->description == NULL) {
        *short_of_memory = true;
        return 0;
    }
    memcpy(part->name, at + HERO_UI_PARTS_GTK_RECORD, name_length);
    memcpy(part->description, at + HERO_UI_PARTS_GTK_RECORD + name_length, description_length);
    part->name_length = name_length;
    part->description_length = description_length;
    return HERO_UI_PARTS_GTK_RECORD + (int64_t)name_length + description_length;
}

/* -- THE CALLS --------------------------------------------------------------- */

size_t hero_ui_parts_gtk_area_type(void) {
    const hero_ui_parts_gtk_functions *f = hero_ui_parts_gtk_loaded();

    return f != NULL ? f->area_type : 0;
}

int hero_ui_parts_gtk_area_role(void) {
    return HERO_GTK_ROLE_GROUP;
}

int64_t hero_ui_parts_gtk_attach(void *area, int64_t id, char *words, size_t room) {
    const hero_ui_parts_gtk_functions *f = hero_ui_parts_gtk_loaded();
    hero_ui_parts_gtk_area *record;
    void *focus;

    if (f == NULL || area == NULL || *(size_t *)*(void **)area != f->area_type) {
        return HERO_UI_OK;
    }
    record = hero_layer_alloc_zero(1, sizeof *record);
    if (record == NULL) {
        return hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing area's parts could not be had");
    }
    record->area = area;
    record->id = id;
    record->holds = 1;
    *hero_ui_parts_gtk_area_slot(area) = record;
    if (!hero_ui_parts_gtk_stand_in(record)) {
        return hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for the image standing for a drawing area could not be had");
    }
    focus = f->event_controller_focus_new();
    if (focus != NULL) {
        (void)f->signal_connect_data(focus, "enter", (hero_ui_parts_gtk_callback)hero_ui_parts_gtk_area_focus, record, NULL, 0);
        (void)f->signal_connect_data(focus, "leave", (hero_ui_parts_gtk_callback)hero_ui_parts_gtk_area_focus, record, NULL, 0);
        f->widget_add_controller(area, focus);
    }
    return HERO_UI_OK;
}

void hero_ui_parts_gtk_forget(void *area) {
    hero_ui_parts_gtk_area *record = hero_ui_parts_gtk_record(area);

    if (record != NULL) {
        record->forgotten = true;
        hero_ui_parts_gtk_drop_all(record);
    }
}

void hero_ui_parts_gtk_named(void *area, const char *name) {
    hero_ui_parts_gtk_area *record = hero_ui_parts_gtk_record(area);
    char *kept = NULL;

    if (record == NULL) {
        return;
    }
    if (name != NULL && name[0] != '\0') {
        kept = hero_layer_alloc_zero(strlen(name) + 1, 1);
        if (kept == NULL) {
            return;
        }
        memcpy(kept, name, strlen(name));
    }
    hero_layer_free(record->name);
    record->name = kept;
    if (record->image != NULL) {
        hero_ui_parts_gtk_describe(record->image, record->name, NULL);
    }
}

/* Whether the parts read from the list are the parts held, role by role and
 * holder by holder, so they change in place. */
static bool hero_ui_parts_gtk_same(const hero_ui_parts_gtk_area *record, const hero_ui_parts_gtk_part *given, int64_t count) {
    if (count == 0 || count != record->count) {
        return false;
    }
    for (int64_t i = 0; i < count; i += 1) {
        const hero_ui_parts_gtk_part *held = hero_ui_parts_gtk_part_of(record->parts[i]);

        if (held->role != given[i].role || held->within != given[i].within) {
            return false;
        }
    }
    return true;
}

/* Copies the given part's fields into the part object's own, the name and
 * description passing to it. */
static void hero_ui_parts_gtk_take(void *object, hero_ui_parts_gtk_part *given, int64_t number) {
    hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(object);

    hero_layer_free(part->name);
    hero_layer_free(part->description);
    part->number = number;
    part->states = given->states;
    part->within = given->within;
    memcpy(part->box, given->box, sizeof part->box);
    part->value = given->value;
    part->least = given->least;
    part->most = given->most;
    part->name = given->name;
    part->name_length = given->name_length;
    part->description = given->description;
    part->description_length = given->description_length;
    given->name = NULL;
    given->description = NULL;
}

/* Ties the new parts `made` into the reader's tree by their `within`, in
 * the order given: each part's holder and next sibling, then each tied
 * under its holder, which presents it to the reader. */
static void hero_ui_parts_gtk_tie(hero_ui_parts_gtk_area *record, void **made, int64_t count) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    void *area_last = NULL;

    for (int64_t i = 0; i < count; i += 1) {
        hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(made[i]);
        void *holder = part->within == 0 ? record->area : made[part->within - 1];

        part->parent = holder;
        if (part->within == 0) {
            if (area_last == NULL) {
                record->first = made[i];
            } else {
                hero_ui_parts_gtk_part_of(area_last)->next = made[i];
            }
            area_last = made[i];
        } else {
            hero_ui_parts_gtk_part *holding = hero_ui_parts_gtk_part_of(holder);
            void *last = holding->first;

            if (last == NULL) {
                holding->first = made[i];
            } else {
                while (hero_ui_parts_gtk_part_of(last)->next != NULL) {
                    last = hero_ui_parts_gtk_part_of(last)->next;
                }
                hero_ui_parts_gtk_part_of(last)->next = made[i];
            }
        }
    }
    for (int64_t i = 0; i < count; i += 1) {
        const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(made[i]);

        f->accessible_set_accessible_parent(made[i], part->parent, part->next);
    }
}

int64_t hero_ui_parts_gtk_set(void *area, const uint8_t *list, int64_t count, char *words, size_t room) {
    const hero_ui_parts_gtk_functions *f = hero_ui_parts_gtk_loaded();
    hero_ui_parts_gtk_area *record = hero_ui_parts_gtk_record(area);
    hero_ui_parts_gtk_part *given = NULL;
    void **made = NULL;
    int64_t parts = 0;
    int64_t focused = 0;
    int64_t answer = HERO_UI_OK;
    bool short_of_memory = false;
    bool in_place;

    if (record == NULL) {
        return hero_ui_parts_gtk_said(HERO_UI_UNSUPPORTED, words, room, hero_ui_parts_gtk_older);
    }
    given = hero_layer_alloc_zero(HERO_UI_PARTS_MOST, sizeof *given);
    if (given == NULL) {
        return hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing area's parts could not be had");
    }
    for (int64_t at = 0; at < count;) {
        int64_t length = parts < HERO_UI_PARTS_MOST ? hero_ui_parts_gtk_parse(list + at, count - at, &given[parts], &short_of_memory) : 0;

        if (length == 0 || given[parts].role < HERO_UI_PART_GROUP || given[parts].role > HERO_UI_PART_LINK || given[parts].within > parts) {
            answer = short_of_memory ? hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing area's parts could not be had") : hero_ui_parts_gtk_said(HERO_UI_INVALID, words, room, "a drawing area's parts this member reads as malformed");
            /* Counted, so the name and description it may hold are freed. */
            parts += 1;
            goto done;
        }
        if ((given[parts].states & HERO_UI_PART_FOCUSED) != 0) {
            focused = parts + 1;
        }
        at += length;
        parts += 1;
    }
    if (parts == 0 && record->count == 0 && record->image != NULL) {
        /* None again: the image stands as it is, and the reader hears
         * nothing. */
        goto done;
    }
    in_place = hero_ui_parts_gtk_same(record, given, parts);
    if (!in_place && parts > 0) {
        /* Every new part made before any old one goes, so memory short
         * leaves the last parts. */
        made = hero_layer_alloc_zero((size_t)parts, sizeof *made);
        for (int64_t i = 0; made != NULL && i < parts; i += 1) {
            made[i] = hero_ui_parts_gtk_new_part(record, given[i].role);
            if (made[i] == NULL) {
                for (int64_t j = 0; j < i; j += 1) {
                    f->object_unref(made[j]);
                }
                hero_layer_free(made);
                made = NULL;
            }
        }
        if (made == NULL) {
            answer = hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing area's parts could not be had");
            goto done;
        }
    }
    if (in_place) {
        int64_t before = record->focused;

        for (int64_t i = 0; i < parts; i += 1) {
            hero_ui_parts_gtk_take(record->parts[i], &given[i], i + 1);
            hero_ui_parts_gtk_describe(record->parts[i], (const char *)hero_ui_parts_gtk_part_of(record->parts[i])->name, (const char *)hero_ui_parts_gtk_part_of(record->parts[i])->description);
        }
        record->focused = focused;
        if (before != focused && before > 0 && f->widget_has_focus(area)) {
            f->accessible_update_platform_state(record->parts[before - 1], HERO_GTK_PLATFORM_FOCUSED);
        }
        if (before != focused && focused > 0 && f->widget_has_focus(area)) {
            hero_ui_parts_gtk_focus_later(record);
        }
        goto done;
    }
    hero_ui_parts_gtk_drop_all(record);
    if (parts == 0) {
        if (!hero_ui_parts_gtk_stand_in(record)) {
            answer = hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for the image standing for a drawing area could not be had");
        }
        goto done;
    }
    for (int64_t i = 0; i < parts; i += 1) {
        hero_ui_parts_gtk_take(made[i], &given[i], i + 1);
        hero_ui_parts_gtk_describe(made[i], (const char *)hero_ui_parts_gtk_part_of(made[i])->name, (const char *)hero_ui_parts_gtk_part_of(made[i])->description);
    }
    record->parts = made;
    record->count = parts;
    record->focused = focused;
    hero_ui_parts_gtk_tie(record, made, parts);
    if (focused > 0 && f->widget_has_focus(area)) {
        hero_ui_parts_gtk_focus_later(record);
    }
done:
    for (int64_t i = 0; i < parts && i < HERO_UI_PARTS_MOST; i += 1) {
        hero_layer_free(given[i].name);
        hero_layer_free(given[i].description);
    }
    hero_layer_free(given);
    return answer;
}

/* GTK's role `role` as `hero_ui.h`'s part role, 0 for none of them. */
static uint8_t hero_ui_parts_gtk_role_back(int role) {
    switch (role) {
    case HERO_GTK_ROLE_GROUP:
        return HERO_UI_PART_GROUP;
    case HERO_GTK_ROLE_LABEL:
        return HERO_UI_PART_TEXT;
    case HERO_GTK_ROLE_IMG:
        return HERO_UI_PART_IMAGE;
    case HERO_GTK_ROLE_BUTTON:
        return HERO_UI_PART_BUTTON;
    case HERO_GTK_ROLE_CHECKBOX:
        return HERO_UI_PART_CHECK_BOX;
    case HERO_GTK_ROLE_RADIO:
        return HERO_UI_PART_RADIO;
    case HERO_GTK_ROLE_SLIDER:
        return HERO_UI_PART_SLIDER;
    case HERO_GTK_ROLE_PROGRESS_BAR:
        return HERO_UI_PART_PROGRESS;
    case HERO_GTK_ROLE_LINK:
        return HERO_UI_PART_LINK;
    default:
        return 0;
    }
}

/* Walks the reader's tree from `holder` down through GTK's own getters,
 * marking each part of the area's found in `seen` and checking each one's
 * holder is `holder`; false where GTK answers a part this member did not
 * give, a role or a holder other than the one given, or no bounds. */
static bool hero_ui_parts_gtk_walk(const hero_ui_parts_gtk_area *record, void *holder, bool *seen, int depth) {
    const hero_ui_parts_gtk_functions *f = &hero_ui_parts_gtk_found_functions;
    void *child = f->accessible_get_first_accessible_child(holder);
    bool ok = depth <= HERO_UI_PARTS_MOST;

    while (child != NULL) {
        void *next;

        if (ok) {
            const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(child);
            void *parent = f->accessible_get_accessible_parent(child);
            int x;
            int y;
            int width;
            int height;

            ok = part->number >= 1 && part->number <= record->count && record->parts[part->number - 1] == child && !seen[part->number - 1] && parent == holder &&
                 hero_ui_parts_gtk_role_back(f->accessible_get_accessible_role(child)) == part->role && f->accessible_get_bounds(child, &x, &y, &width, &height) != 0;
            if (parent != NULL) {
                f->object_unref(parent);
            }
            if (ok) {
                seen[part->number - 1] = true;
                ok = hero_ui_parts_gtk_walk(record, child, seen, depth + 1);
            }
        }
        next = f->accessible_get_next_accessible_sibling(child);
        f->object_unref(child);
        child = next;
    }
    return ok;
}

int64_t hero_ui_parts_gtk_read(void *area, uint8_t *buffer, int64_t count, int64_t *length, char *words, size_t room) {
    const hero_ui_parts_gtk_functions *f = hero_ui_parts_gtk_loaded();
    const hero_ui_parts_gtk_area *record = hero_ui_parts_gtk_record(area);
    int64_t total = 0;
    bool *seen;
    bool whole;

    if (record == NULL) {
        return hero_ui_parts_gtk_said(HERO_UI_UNSUPPORTED, words, room, hero_ui_parts_gtk_older);
    }
    for (int64_t i = 0; i < record->count; i += 1) {
        const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(record->parts[i]);

        total += HERO_UI_PARTS_GTK_RECORD + part->name_length + part->description_length;
    }
    *length = total;
    if (buffer == NULL || total > count) {
        return HERO_UI_OK;
    }
    seen = hero_layer_alloc_zero(record->count > 0 ? (size_t)record->count : 1, sizeof *seen);
    if (seen == NULL) {
        return hero_ui_parts_gtk_said(HERO_UI_LIMIT, words, room, "the memory for reading a drawing area's parts could not be had");
    }
    whole = record->count == 0 || hero_ui_parts_gtk_walk(record, area, seen, 0);
    for (int64_t i = 0; whole && i < record->count; i += 1) {
        whole = seen[i];
    }
    hero_layer_free(seen);
    if (!whole) {
        return hero_ui_parts_gtk_said(HERO_UI_OTHER, words, room, "GTK's tree of the drawing area's accessibles is not the parts given: a part missing, out of its holder, of another role, or without bounds");
    }
    for (int64_t i = 0, at = 0; i < record->count; i += 1) {
        void *object = record->parts[i];
        const hero_ui_parts_gtk_part *part = hero_ui_parts_gtk_part_of(object);
        uint8_t states = part->states & (uint8_t)(HERO_UI_PART_FOCUSED | HERO_UI_PART_CHECKED);

        /* The role and whether it is focusable are GTK's answers; GTK has no
         * getter for a property or a state, so the rest is the part's own. */
        if (f->accessible_get_platform_state(object, HERO_GTK_PLATFORM_FOCUSABLE) != 0) {
            states |= HERO_UI_PART_FOCUSABLE;
        }
        buffer[at] = hero_ui_parts_gtk_role_back(f->accessible_get_accessible_role(object));
        buffer[at + 1] = states;
        hero_ui_parts_gtk_put_u16(buffer + at + 2, part->within);
        for (int j = 0; j < 4; j += 1) {
            hero_ui_parts_gtk_put_r32(buffer + at + 4 + 4 * j, part->box[j]);
        }
        hero_ui_parts_gtk_put_d64(buffer + at + 20, part->value);
        hero_ui_parts_gtk_put_d64(buffer + at + 28, part->least);
        hero_ui_parts_gtk_put_d64(buffer + at + 36, part->most);
        hero_ui_parts_gtk_put_u16(buffer + at + 44, part->name_length);
        hero_ui_parts_gtk_put_u16(buffer + at + 46, part->description_length);
        memcpy(buffer + at + HERO_UI_PARTS_GTK_RECORD, part->name, part->name_length);
        memcpy(buffer + at + HERO_UI_PARTS_GTK_RECORD + part->name_length, part->description, part->description_length);
        at += HERO_UI_PARTS_GTK_RECORD + part->name_length + part->description_length;
    }
    return HERO_UI_OK;
}

int64_t hero_ui_parts_gtk_act(void *area, int64_t part, int64_t act, char *words, size_t room) {
    const hero_ui_parts_gtk_functions *f = hero_ui_parts_gtk_loaded();
    const hero_ui_parts_gtk_area *record = hero_ui_parts_gtk_record(area);
    void *object;
    const hero_ui_parts_gtk_part *held;
    const hero_gtk_range_iface *range;
    char said[160];

    if (record == NULL) {
        return hero_ui_parts_gtk_said(HERO_UI_UNSUPPORTED, words, room, hero_ui_parts_gtk_older);
    }
    if (part < 1 || part > record->count) {
        (void)snprintf(said, sizeof said, "part %lld is none of the drawing area's %lld parts", (long long)part, (long long)record->count);
        return hero_ui_parts_gtk_said(HERO_UI_INVALID, words, room, said);
    }
    object = record->parts[part - 1];
    held = hero_ui_parts_gtk_part_of(object);
    switch (act) {
    case HERO_UI_PART_PRESS:
        if (held->role != HERO_UI_PART_BUTTON && held->role != HERO_UI_PART_LINK && held->role != HERO_UI_PART_CHECK_BOX && held->role != HERO_UI_PART_RADIO) {
            return hero_ui_parts_gtk_said(HERO_UI_INVALID, words, room, "a press is a button's, a link's, a check box's or a radio button's");
        }
        return hero_ui_parts_gtk_said(HERO_UI_UNSUPPORTED, words, room, "GTK 4 gives a screen reader's press, AT-SPI's Action interface, to widgets alone (gtk/a11y/gtkatspiaction.c), and a drawing area's part is no widget");
    case HERO_UI_PART_FOCUS:
        if ((held->states & HERO_UI_PART_FOCUSABLE) == 0) {
            return hero_ui_parts_gtk_said(HERO_UI_INVALID, words, room, "a focus is asked of a focusable part alone");
        }
        return hero_ui_parts_gtk_said(HERO_UI_UNSUPPORTED, words, room, "GTK 4 answers a screen reader's request for the focus, AT-SPI's Component.GrabFocus, as not supported for every object (gtk/a11y/gtkatspicomponent.c)");
    case HERO_UI_PART_INCREASE:
    case HERO_UI_PART_DECREASE:
        if (held->role != HERO_UI_PART_SLIDER) {
            return hero_ui_parts_gtk_said(HERO_UI_INVALID, words, room, "a step is a slider's alone");
        }
        /* The setter GTK's Value interface calls for a reader, through the
         * interface GObject holds for the part's class. */
        range = f->type_interface_peek(*(void **)object, f->accessible_range_get_type());
        if (range == NULL || range->set_current_value(object, hero_layer_next_toward(held->value, act == HERO_UI_PART_INCREASE ? HUGE_VAL : -HUGE_VAL)) == 0) {
            return hero_ui_parts_gtk_said(HERO_UI_OTHER, words, room, "GTK's value setter refused a step of the slider");
        }
        return HERO_UI_OK;
    default:
        return hero_ui_parts_gtk_said(HERO_UI_INVALID, words, room, "an act is HERO_UI_PART_PRESS to _DECREASE");
    }
}

#endif /* !defined(_WIN32) && !defined(__APPLE__) */
