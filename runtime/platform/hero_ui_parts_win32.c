/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_parts_win32.c: THE WIN32 ARM'S PARTS OF A DRAWING AREA FOR
 * A SCREEN READER, one member of the platform layer's archive
 * (`selfhost/cli/layer.hero`), compiled to nothing on every other system.
 * `hero_ui_win32.c` makes the area's window and hands this member its
 * `WM_GETOBJECT`, `WM_SETFOCUS` and `WM_DESTROY`; the list's reading, its
 * tree, the part under a point and the record written back are the pure
 * half, `hero_ui_win32_parts.h`.
 *
 * UI AUTOMATION'S FRAGMENTS. Once the program gives the area parts, its
 * window answers `WM_GETOBJECT` for `UiaRootObjectId` with a root of the
 * member's (`IRawElementProviderSimple`, `IRawElementProviderFragment`,
 * `IRawElementProviderFragmentRoot`), a group named as the area is, which
 * holds one object per part (`IRawElementProviderSimple` and
 * `IRawElementProviderFragment`, and the pattern its role has: Invoke for a
 * button and a link, Toggle for a check box, SelectionItem for a radio
 * button, RangeValue for a slider and a progress bar). A part `within`
 * another is its child in the reader's tree. Before any part the window
 * answers nothing, so the system's own element stands, the image
 * `hero_ui_win32.c` names; with the parts taken back the root stays, an
 * image again holding nothing.
 *
 * THE OBJECTS ARE CALLED ON ANY THREAD. Measured on the Windows box
 * (Windows Server, SDK 10.0.26100.0, a .NET `System.Windows.Automation`
 * client in another process): of 52 calls a reader's walk made, about a
 * third came on a thread of UI Automation's own and the rest on the UI
 * thread, although no object asks for COM's threading
 * (`ProviderOptions_UseComThreading`). So an object never reads the arm's
 * records, the UI thread's own state or a thread's own variable: the area
 * holds what its objects answer from (the parts, the name, the scale, the
 * functions of `UIAutomationCore.dll`) under a slim reader-writer lock,
 * which the UI thread alone takes to write and every object method takes to
 * read; UI Automation is called with no lock held, since it may call the
 * objects back. A reader's act is posted to the area's window (a message
 * registered for it) and told on the UI thread, where it is dropped if the
 * part's object was replaced meanwhile; keyboard focus is asked of the UI
 * thread's input state (`GetGUIThreadInfo`), never of the caller's.
 *
 * WHAT A READER DOES IS TOLD, NEVER APPLIED (`hero_ui.h`'s A SCREEN READER
 * ACTING ON A PART): Invoke, Toggle and Select queue a press, `SetFocus` on a
 * focusable part a focus, a slider's `SetValue` an increase above its value
 * and a decrease below it (the step is the program's; the reader's arrows
 * arrive as `SetValue` of the value plus or minus `SmallChange`), and the
 * parts stay as given until the program gives them again. `act`, the cases'
 * reader, calls the same pattern methods through the same interfaces.
 *
 * A PART'S OBJECT LIVES WHILE ITS NUMBER AND ITS ROLE DO. Given again, a
 * part of the same number and role keeps its object, so a reader standing on
 * a check box the program has just checked hears the change
 * (`UiaRaiseAutomationPropertyChangedEvent` of its toggle state) rather than
 * losing its place; a part whose role changed, or past the new list's end,
 * is disconnected (`UiaDisconnectProvider`) and a new object takes its
 * number, with a runtime id no object of the area had before. A change of
 * the tree is `StructureChangeType_ChildrenInvalidated` on the root; a
 * focused part that changes while the area has the keys, and the area taking
 * the keys, `UIA_AutomationFocusChangedEventId` on the focused part.
 *
 * A BOX IS ASKED ANEW at each reading: the window's client origin
 * (`ClientToScreen`) plus the box at the window's scale, which
 * `hero_ui_win32.c` keeps, so a part follows the window when it moves or
 * crosses to another monitor; the read-back goes the other way.
 *
 * THE READ-BACK asks the member's own objects what a client would read,
 * through their interfaces (the control type, the name, the help text, the
 * keyboard focusability, the parent by `Navigate`, the box, the toggle state,
 * the selection, the range), and the focused part by the root's `GetFocus`,
 * the answer UI Automation asks of a fragment root whose window has the
 * keys; never by a client's call from the UI thread into its own window,
 * which UI Automation documents can wait on itself.
 *
 * THE INTERFACES ARE THE SYSTEM'S: the SDK's `UIAutomationCore.h` is
 * MIDL's, which declares each interface for C beside C++ (read on the
 * Windows box, SDK 10.0.26100.0, as MinGW-w64's widl header does), unlike
 * Direct2D's (`hero_ui_win32_direct2d.h`). Their IIDs are written here, so
 * no import library of GUIDs is needed; each was read in both headers and
 * they agree. `UIAutomationCore.dll` is loaded from the system folder by its
 * full path at the first parts, so a program with none never loads it, and
 * each function's type is checked against the header's declaration at
 * compile time.
 *
 * Written from Microsoft's UI Automation documentation and the SDK's
 * headers; no other library's source.
 */

#include "hero_ui_parts_win32.h"

#if defined(_WIN32)
#if defined(_MSC_VER)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "oleaut32.lib")
#endif

/* The vtables below are constant data, and the headers' `lpVtbl` points at
 * them as such. */
#define CONST_VTABLE

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ole2.h>
#include <uiautomation.h>

#include "hero_layer_alloc.h"
#include "hero_layer_maths.h"
#include "platform/hero_ui_arm.h"

#include "hero_ui_win32_parts.h"

_Static_assert(sizeof(WCHAR) == sizeof(uint16_t), "a WCHAR is a UTF-16 unit");
_Static_assert(HERO_UI_WIN_CONTROL_BUTTON == UIA_ButtonControlTypeId && HERO_UI_WIN_CONTROL_CHECK_BOX == UIA_CheckBoxControlTypeId && HERO_UI_WIN_CONTROL_HYPERLINK == UIA_HyperlinkControlTypeId && HERO_UI_WIN_CONTROL_IMAGE == UIA_ImageControlTypeId && HERO_UI_WIN_CONTROL_PROGRESS_BAR == UIA_ProgressBarControlTypeId, "the control types are the header's");
_Static_assert(HERO_UI_WIN_CONTROL_RADIO_BUTTON == UIA_RadioButtonControlTypeId && HERO_UI_WIN_CONTROL_SLIDER == UIA_SliderControlTypeId && HERO_UI_WIN_CONTROL_TEXT == UIA_TextControlTypeId && HERO_UI_WIN_CONTROL_GROUP == UIA_GroupControlTypeId, "the control types are the header's");

/* -- UI Automation, loaded at the first parts ---------------------------------- */

/* UI Automation's two errors as the HRESULTs they are: the headers write
 * them as unsigned literals (`0x80040201`, `0x80131509`), which the layer's
 * `-Werror=sign-conversion` refuses where an HRESULT is answered. */
#define HERO_UIA_GONE ((HRESULT)UIA_E_ELEMENTNOTAVAILABLE)
#define HERO_UIA_REFUSED ((HRESULT)UIA_E_INVALIDOPERATION)

typedef LRESULT(WINAPI *hero_uia_return_fn)(HWND, WPARAM, LPARAM, IRawElementProviderSimple *);
typedef HRESULT(WINAPI *hero_uia_host_fn)(HWND, IRawElementProviderSimple **);
typedef HRESULT(WINAPI *hero_uia_event_fn)(IRawElementProviderSimple *, EVENTID);
typedef HRESULT(WINAPI *hero_uia_changed_fn)(IRawElementProviderSimple *, PROPERTYID, VARIANT, VARIANT);
typedef HRESULT(WINAPI *hero_uia_structure_fn)(IRawElementProviderSimple *, enum StructureChangeType, int *, int);
typedef HRESULT(WINAPI *hero_uia_disconnect_fn)(IRawElementProviderSimple *);
typedef BOOL(WINAPI *hero_uia_listening_fn)(void);

_Static_assert(__builtin_types_compatible_p(__typeof__(&UiaReturnRawElementProvider), hero_uia_return_fn) && __builtin_types_compatible_p(__typeof__(&UiaHostProviderFromHwnd), hero_uia_host_fn) && __builtin_types_compatible_p(__typeof__(&UiaRaiseAutomationEvent), hero_uia_event_fn), "the functions are the header's");
_Static_assert(__builtin_types_compatible_p(__typeof__(&UiaRaiseAutomationPropertyChangedEvent), hero_uia_changed_fn) && __builtin_types_compatible_p(__typeof__(&UiaRaiseStructureChangedEvent), hero_uia_structure_fn) && __builtin_types_compatible_p(__typeof__(&UiaDisconnectProvider), hero_uia_disconnect_fn) && __builtin_types_compatible_p(__typeof__(&UiaClientsAreListening), hero_uia_listening_fn), "the functions are the header's");

/* The functions an area calls, found once by the UI thread and copied into
 * each area at its making, so an object called on another thread finds them
 * in the area and never in a thread's own variable. */
typedef struct hero_uia_calls {
    hero_uia_return_fn give;
    hero_uia_host_fn host;
    hero_uia_event_fn event;
    hero_uia_changed_fn changed;
    hero_uia_structure_fn structure;
    hero_uia_disconnect_fn disconnect;
    hero_uia_listening_fn listening;
} hero_uia_calls;

/* The UI thread's, as every window of the arm is: the functions, and the
 * message a reader's act is posted to an area's window by. */
static _Thread_local hero_uia_calls hero_uia_loaded;
static _Thread_local UINT hero_uia_told;

/* `UIAutomationCore.h`'s IIDs, the SDK's and MinGW-w64's alike, and COM's
 * `IUnknown`. */
static const IID hero_uia_unknown_iid = {0x00000000, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const IID hero_uia_simple_iid = {0xd6dd68d1, 0x86fd, 0x4332, {0x86, 0x66, 0x9a, 0xbe, 0xde, 0xa2, 0xd2, 0x4c}};
static const IID hero_uia_fragment_iid = {0xf7063da8, 0x8359, 0x439c, {0x92, 0x97, 0xbb, 0xc5, 0x29, 0x9a, 0x7d, 0x87}};
static const IID hero_uia_root_iid = {0x620ce2a5, 0xab8f, 0x40a9, {0x86, 0xcb, 0xde, 0x3c, 0x75, 0x59, 0x9b, 0x58}};
static const IID hero_uia_invoke_iid = {0x54fcb24b, 0xe18e, 0x47a2, {0xb4, 0xd3, 0xec, 0xcb, 0xe7, 0x75, 0x99, 0xa2}};
static const IID hero_uia_toggle_iid = {0x56d00bd0, 0xc4f4, 0x433c, {0xa8, 0x36, 0x1a, 0x52, 0xa5, 0x7e, 0x08, 0x92}};
static const IID hero_uia_selection_iid = {0x2acad808, 0xb2d4, 0x452d, {0xa4, 0x07, 0x91, 0xff, 0x1a, 0xd1, 0x67, 0xb2}};
static const IID hero_uia_range_iid = {0x36dc7aef, 0x33e6, 0x4691, {0xaf, 0xe1, 0x2b, 0xe7, 0x27, 0x4b, 0x3d, 0x33}};

static bool hero_uia_same(REFIID one, const IID *other) {
    return memcmp(one, other, sizeof *other) == 0;
}

/* Copies the address `GetProcAddress` gives into the typed pointer `to`. */
static bool hero_uia_found(HMODULE core, const char *name, void *to, size_t size) {
    FARPROC found = GetProcAddress(core, name);

    if (found == NULL || size != sizeof found) {
        return false;
    }
    memcpy(to, &found, size);
    return true;
}

int64_t hero_ui_parts_win32_offered(void) {
    WCHAR folder[MAX_PATH];
    WCHAR path[MAX_PATH + 32];
    UINT length;
    HMODULE core;
    hero_uia_calls found;

    if (hero_uia_loaded.listening != NULL && hero_uia_told != 0) {
        return HERO_UI_OK;
    }
    length = GetSystemDirectoryW(folder, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area's parts need UI Automation, and Windows did not name its system folder");
    }
    (void)_snwprintf(path, sizeof path / sizeof path[0], L"%ls\\UIAutomationCore.dll", folder);
    path[sizeof path / sizeof path[0] - 1] = 0;
    core = LoadLibraryW(path);
    if (core == NULL) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area's parts need UI Automation (UIAutomationCore.dll), which this Windows does not have");
    }
    if (!hero_uia_found(core, "UiaReturnRawElementProvider", &found.give, sizeof found.give) || !hero_uia_found(core, "UiaHostProviderFromHwnd", &found.host, sizeof found.host) || !hero_uia_found(core, "UiaRaiseAutomationEvent", &found.event, sizeof found.event) || !hero_uia_found(core, "UiaRaiseAutomationPropertyChangedEvent", &found.changed, sizeof found.changed) || !hero_uia_found(core, "UiaRaiseStructureChangedEvent", &found.structure, sizeof found.structure) || !hero_uia_found(core, "UiaDisconnectProvider", &found.disconnect, sizeof found.disconnect) || !hero_uia_found(core, "UiaClientsAreListening", &found.listening, sizeof found.listening)) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area's parts need UiaReturnRawElementProvider, UiaHostProviderFromHwnd, UiaRaiseAutomationEvent, UiaRaiseAutomationPropertyChangedEvent, UiaRaiseStructureChangedEvent, UiaDisconnectProvider and UiaClientsAreListening, which this Windows' UIAutomationCore.dll does not all export");
    }
    hero_uia_told = RegisterWindowMessageW(L"HeroesUiPartActed");
    if (hero_uia_told == 0) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area's parts need a window message of their own, and Windows registered none (RegisterWindowMessageW)");
    }
    hero_uia_loaded = found;
    return HERO_UI_OK;
}

/* -- The records ----------------------------------------------------------------- */

typedef struct hero_ui_parts_element hero_ui_parts_element;

/* The area's parts, and UI Automation's root of them. `hwnd` is NULL once
 * the area is gone, and every object then answers that it is no longer
 * there. What `lock` guards (from `hwnd` on, and each part object's
 * `alive`) is written by the UI thread alone, holding it exclusive, and read
 * by the objects holding it shared; the UI thread reads it without it. */
struct hero_ui_parts_win32 {
    IRawElementProviderSimple simple;
    IRawElementProviderFragment fragment;
    IRawElementProviderFragmentRoot root;
    LONG references;
    SRWLOCK lock;
    /* Set at the making and never changed. */
    hero_uia_calls uia;
    DWORD thread;
    UINT told;
    int64_t id;
    hero_ui_parts_win32_scale scale;
    HWND hwnd;
    /* The window's scale as the arm keeps it, asked by the UI thread at
     * each placing of the area and each list. */
    uint32_t dpi;
    /* The list as given, which `parts` point into, and each part's object. */
    uint8_t *list;
    hero_ui_win_part *parts;
    hero_ui_parts_element **elements;
    size_t count;
    WCHAR *name;
    int serials;
    /* Whether the window answers `WM_GETOBJECT` with the root: from the
     * first parts on. */
    bool offered;
};

/* One part's object; `alive` until its number or role is given no more. */
struct hero_ui_parts_element {
    IRawElementProviderSimple simple;
    IRawElementProviderFragment fragment;
    IInvokeProvider invoke;
    IToggleProvider toggle;
    ISelectionItemProvider selection;
    IRangeValueProvider range;
    LONG references;
    hero_ui_parts_win32 *area;
    uint32_t number;
    uint8_t role;
    int serial;
    bool alive;
};

#define HERO_UI_PARTS_OF(type, field, face) ((type *)(void *)((char *)(face) - offsetof(type, field)))

static bool hero_ui_parts_live(const hero_ui_parts_element *element) {
    return element->alive && element->area->hwnd != NULL;
}

static const hero_ui_win_part *hero_ui_parts_given(const hero_ui_parts_element *element) {
    return &element->area->parts[element->number - 1];
}

/* -- IUnknown, for the root and the parts ----------------------------------------- */

static ULONG hero_ui_parts_area_add(hero_ui_parts_win32 *area) {
    return (ULONG)InterlockedIncrement(&area->references);
}

static ULONG hero_ui_parts_area_release(hero_ui_parts_win32 *area) {
    LONG left = InterlockedDecrement(&area->references);

    if (left == 0) {
        hero_layer_free(area->list);
        hero_layer_free(area->parts);
        hero_layer_free(area->elements);
        hero_layer_free(area->name);
        hero_layer_free(area);
    }
    return (ULONG)left;
}

static HRESULT hero_ui_parts_area_query(hero_ui_parts_win32 *area, REFIID iid, void **out) {
    if (out == NULL) {
        return E_POINTER;
    }
    if (hero_uia_same(iid, &hero_uia_unknown_iid) || hero_uia_same(iid, &hero_uia_simple_iid)) {
        *out = &area->simple;
    } else if (hero_uia_same(iid, &hero_uia_fragment_iid)) {
        *out = &area->fragment;
    } else if (hero_uia_same(iid, &hero_uia_root_iid)) {
        *out = &area->root;
    } else {
        *out = NULL;
        return E_NOINTERFACE;
    }
    (void)hero_ui_parts_area_add(area);
    return S_OK;
}

static ULONG hero_ui_parts_element_add(hero_ui_parts_element *element) {
    return (ULONG)InterlockedIncrement(&element->references);
}

static ULONG hero_ui_parts_element_release(hero_ui_parts_element *element) {
    LONG left = InterlockedDecrement(&element->references);

    if (left == 0) {
        (void)hero_ui_parts_area_release(element->area);
        hero_layer_free(element);
    }
    return (ULONG)left;
}

static HRESULT hero_ui_parts_element_query(hero_ui_parts_element *element, REFIID iid, void **out) {
    uint8_t role = element->role;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    if (hero_uia_same(iid, &hero_uia_unknown_iid) || hero_uia_same(iid, &hero_uia_simple_iid)) {
        *out = &element->simple;
    } else if (hero_uia_same(iid, &hero_uia_fragment_iid)) {
        *out = &element->fragment;
    } else if (hero_uia_same(iid, &hero_uia_invoke_iid) && (role == HERO_UI_PART_BUTTON || role == HERO_UI_PART_LINK)) {
        *out = &element->invoke;
    } else if (hero_uia_same(iid, &hero_uia_toggle_iid) && role == HERO_UI_PART_CHECK_BOX) {
        *out = &element->toggle;
    } else if (hero_uia_same(iid, &hero_uia_selection_iid) && role == HERO_UI_PART_RADIO) {
        *out = &element->selection;
    } else if (hero_uia_same(iid, &hero_uia_range_iid) && (role == HERO_UI_PART_SLIDER || role == HERO_UI_PART_PROGRESS)) {
        *out = &element->range;
    } else {
        return E_NOINTERFACE;
    }
    (void)hero_ui_parts_element_add(element);
    return S_OK;
}

/* Each face's three IUnknown methods, handed to its object's. */
#define HERO_UI_PARTS_UNKNOWN(prefix, face, type, field, owner)                                                                                                                      \
    static HRESULT STDMETHODCALLTYPE prefix##_query(face *self, REFIID iid, void **out) { return hero_ui_parts_##owner##_query(HERO_UI_PARTS_OF(type, field, self), iid, out); } \
    static ULONG STDMETHODCALLTYPE prefix##_add(face *self) { return hero_ui_parts_##owner##_add(HERO_UI_PARTS_OF(type, field, self)); }                                         \
    static ULONG STDMETHODCALLTYPE prefix##_release(face *self) { return hero_ui_parts_##owner##_release(HERO_UI_PARTS_OF(type, field, self)); }

HERO_UI_PARTS_UNKNOWN(hero_ui_parts_root_simple, IRawElementProviderSimple, hero_ui_parts_win32, simple, area)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_root_fragment, IRawElementProviderFragment, hero_ui_parts_win32, fragment, area)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_root_root, IRawElementProviderFragmentRoot, hero_ui_parts_win32, root, area)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_simple, IRawElementProviderSimple, hero_ui_parts_element, simple, element)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_fragment, IRawElementProviderFragment, hero_ui_parts_element, fragment, element)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_invoke, IInvokeProvider, hero_ui_parts_element, invoke, element)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_toggle, IToggleProvider, hero_ui_parts_element, toggle, element)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_selection, ISelectionItemProvider, hero_ui_parts_element, selection, element)
HERO_UI_PARTS_UNKNOWN(hero_ui_parts_range, IRangeValueProvider, hero_ui_parts_element, range, element)

/* -- Values ---------------------------------------------------------------------------- */

static void hero_ui_parts_bool(VARIANT *out, bool value) {
    out->vt = VT_BOOL;
    out->boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
}

/* The `count` bytes of UTF-8 `text` as a BSTR in `out`; false where
 * Windows reads no UTF-8 there or has no memory. */
static bool hero_ui_parts_text(VARIANT *out, const uint8_t *text, uint16_t count) {
    int units = count == 0 ? 0 : MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char *)text, (int)count, NULL, 0);
    BSTR made;

    if (count > 0 && units <= 0) {
        return false;
    }
    made = SysAllocStringLen(NULL, (UINT)units);
    if (made == NULL) {
        return false;
    }
    if (units > 0 && MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char *)text, (int)count, made, units) != units) {
        SysFreeString(made);
        return false;
    }
    out->vt = VT_BSTR;
    out->bstrVal = made;
    return true;
}

/* The runtime id `{UiaAppendRuntimeId, serial}`, which UI Automation joins
 * to the window's. */
static HRESULT hero_ui_parts_runtime_id(int serial, SAFEARRAY **out) {
    LONG ids[2] = {UiaAppendRuntimeId, serial};
    SAFEARRAY *array = SafeArrayCreateVector(VT_I4, 0, 2);

    *out = NULL;
    if (array == NULL) {
        return E_OUTOFMEMORY;
    }
    for (LONG at = 0; at < 2; at += 1) {
        if (FAILED(SafeArrayPutElement(array, &at, &ids[at]))) {
            (void)SafeArrayDestroy(array);
            return E_OUTOFMEMORY;
        }
    }
    *out = array;
    return S_OK;
}

/* The area's client origin on the screen and its scale, with the area read
 * (its lock held, or on the UI thread). */
static void hero_ui_parts_origin(const hero_ui_parts_win32 *area, POINT *origin, uint32_t *dpi) {
    origin->x = 0;
    origin->y = 0;
    (void)ClientToScreen(area->hwnd, origin);
    *dpi = area->dpi != 0 ? area->dpi : 96;
}

/* The area's lock taken to read it, from any thread, and given back. */
static void hero_ui_parts_reading(hero_ui_parts_win32 *area) {
    AcquireSRWLockShared(&area->lock);
}

static void hero_ui_parts_read_done(hero_ui_parts_win32 *area) {
    ReleaseSRWLockShared(&area->lock);
}

/* Whether the area's window has the keys of its UI thread, asked of that
 * thread's input state, so the answer is the same from any thread. */
static bool hero_ui_parts_has_keys(const hero_ui_parts_win32 *area) {
    GUITHREADINFO input;

    memset(&input, 0, sizeof input);
    input.cbSize = (DWORD)sizeof input;
    return area->hwnd != NULL && GetGUIThreadInfo(area->thread, &input) && input.hwndFocus == area->hwnd;
}

/* A reader's act `act` on part `number` (0 the area itself), whose object
 * is `serial`'s, posted to the area's window to be told on the UI thread;
 * with the area read. */
static HRESULT hero_ui_parts_posted(const hero_ui_parts_win32 *area, uint32_t number, int serial, int64_t act) {
    LPARAM words = (LPARAM)((int64_t)serial * 16 + act);

    return PostMessageW(area->hwnd, area->told, (WPARAM)number, words) ? S_OK : E_FAIL;
}

/* A step of slider `part` a reader's arrow takes: a hundredth of its range,
 * never so small that the value plus or minus it is the value. */
static double hero_ui_parts_step(const hero_ui_win_part *part) {
    double step = (part->most - part->least) / 100.0;
    double magnitude = fabs(part->value);
    double gap = hero_layer_next_toward(magnitude, INFINITY) - magnitude;

    return step >= gap ? step : gap;
}

static hero_ui_parts_element *hero_ui_parts_element_at(const hero_ui_parts_win32 *area, uint32_t number) {
    return number >= 1 && number <= area->count ? area->elements[number - 1] : NULL;
}

/* `number`'s fragment handed out with a reference, NULL for none. */
static IRawElementProviderFragment *hero_ui_parts_handed(const hero_ui_parts_win32 *area, uint32_t number) {
    hero_ui_parts_element *element = hero_ui_parts_element_at(area, number);

    if (element == NULL) {
        return NULL;
    }
    (void)hero_ui_parts_element_add(element);
    return &element->fragment;
}

/* The focused part's number, 0 for none. */
static uint32_t hero_ui_parts_focused(const hero_ui_win_part *parts, size_t count) {
    for (size_t number = 1; number <= count; number += 1) {
        if ((parts[number - 1].states & HERO_UI_PART_FOCUSED) != 0) {
            return (uint32_t)number;
        }
    }
    return 0;
}

/* -- A part: IRawElementProviderSimple -------------------------------------------------- */

/* Every method of a part below takes the area's lock to read it, answers
 * `HERO_UIA_GONE` where the part is given no more, and gives the lock back
 * at its one exit. */

static HRESULT STDMETHODCALLTYPE hero_ui_parts_options(IRawElementProviderSimple *self, enum ProviderOptions *out) {
    (void)self;
    if (out == NULL) {
        return E_POINTER;
    }
    *out = ProviderOptions_ServerSideProvider;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_pattern(IRawElementProviderSimple *self, PATTERNID pattern, IUnknown **out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, simple, self);
    uint8_t role = element->role;
    HRESULT done = S_OK;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(element->area);
    if (!hero_ui_parts_live(element)) {
        done = HERO_UIA_GONE;
    } else if (pattern == UIA_InvokePatternId && (role == HERO_UI_PART_BUTTON || role == HERO_UI_PART_LINK)) {
        *out = (IUnknown *)(void *)&element->invoke;
    } else if (pattern == UIA_TogglePatternId && role == HERO_UI_PART_CHECK_BOX) {
        *out = (IUnknown *)(void *)&element->toggle;
    } else if (pattern == UIA_SelectionItemPatternId && role == HERO_UI_PART_RADIO) {
        *out = (IUnknown *)(void *)&element->selection;
    } else if (pattern == UIA_RangeValuePatternId && (role == HERO_UI_PART_SLIDER || role == HERO_UI_PART_PROGRESS)) {
        *out = (IUnknown *)(void *)&element->range;
    }
    if (*out != NULL) {
        (void)hero_ui_parts_element_add(element);
    }
    hero_ui_parts_read_done(element->area);
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_property(IRawElementProviderSimple *self, PROPERTYID property, VARIANT *out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, simple, self);
    hero_ui_parts_win32 *area = element->area;
    const hero_ui_win_part *part;
    HRESULT done = S_OK;
    WCHAR id[24];

    if (out == NULL) {
        return E_POINTER;
    }
    VariantInit(out);
    hero_ui_parts_reading(area);
    if (!hero_ui_parts_live(element)) {
        hero_ui_parts_read_done(area);
        return HERO_UIA_GONE;
    }
    part = hero_ui_parts_given(element);
    switch (property) {
    case UIA_ControlTypePropertyId:
        out->vt = VT_I4;
        out->lVal = hero_ui_win_parts_control(part->role);
        break;
    case UIA_NamePropertyId:
        done = hero_ui_parts_text(out, part->name, part->named) ? S_OK : E_OUTOFMEMORY;
        break;
    case UIA_HelpTextPropertyId:
        done = part->described == 0 || hero_ui_parts_text(out, part->description, part->described) ? S_OK : E_OUTOFMEMORY;
        break;
    case UIA_AutomationIdPropertyId:
        /* Stable while the part's number is. */
        (void)_snwprintf(id, sizeof id / sizeof id[0], L"part %u", (unsigned)element->number);
        id[sizeof id / sizeof id[0] - 1] = 0;
        out->vt = VT_BSTR;
        out->bstrVal = SysAllocString(id);
        done = out->bstrVal != NULL ? S_OK : E_OUTOFMEMORY;
        break;
    case UIA_IsKeyboardFocusablePropertyId:
        hero_ui_parts_bool(out, (part->states & HERO_UI_PART_FOCUSABLE) != 0);
        break;
    case UIA_HasKeyboardFocusPropertyId:
        hero_ui_parts_bool(out, (part->states & HERO_UI_PART_FOCUSED) != 0 && hero_ui_parts_has_keys(area));
        break;
    case UIA_IsEnabledPropertyId:
        hero_ui_parts_bool(out, IsWindowEnabled(area->hwnd) != 0);
        break;
    case UIA_IsControlElementPropertyId:
    case UIA_IsContentElementPropertyId:
        hero_ui_parts_bool(out, true);
        break;
    default:
        break;
    }
    hero_ui_parts_read_done(area);
    if (FAILED(done)) {
        VariantInit(out);
    }
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_host(IRawElementProviderSimple *self, IRawElementProviderSimple **out) {
    (void)self;
    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    return S_OK;
}

static const IRawElementProviderSimpleVtbl hero_ui_parts_simple_table = {
    hero_ui_parts_simple_query, hero_ui_parts_simple_add, hero_ui_parts_simple_release, hero_ui_parts_options, hero_ui_parts_pattern, hero_ui_parts_property, hero_ui_parts_host,
};

/* -- A part: IRawElementProviderFragment -------------------------------------------- */

static HRESULT STDMETHODCALLTYPE hero_ui_parts_navigate(IRawElementProviderFragment *self, enum NavigateDirection direction, IRawElementProviderFragment **out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, fragment, self);
    hero_ui_parts_win32 *area = element->area;
    const hero_ui_win_part *part;
    uint32_t number = 0;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(area);
    if (!hero_ui_parts_live(element)) {
        hero_ui_parts_read_done(area);
        return HERO_UIA_GONE;
    }
    part = hero_ui_parts_given(element);
    switch (direction) {
    case NavigateDirection_Parent:
        if (part->within == 0) {
            (void)hero_ui_parts_area_add(area);
            *out = &area->fragment;
        }
        number = part->within;
        break;
    case NavigateDirection_NextSibling:
        number = part->next;
        break;
    case NavigateDirection_PreviousSibling:
        number = part->previous;
        break;
    case NavigateDirection_FirstChild:
        number = part->first;
        break;
    case NavigateDirection_LastChild:
        number = part->last;
        break;
    default:
        break;
    }
    if (*out == NULL) {
        *out = hero_ui_parts_handed(area, number);
    }
    hero_ui_parts_read_done(area);
    return S_OK;
}

/* A part's runtime id is its object's serial, which never changes, so it is
 * answered after the part is given no more too, as UI Automation asks it
 * while it disconnects the object. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_runtime(IRawElementProviderFragment *self, SAFEARRAY **out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, fragment, self);

    if (out == NULL) {
        return E_POINTER;
    }
    return hero_ui_parts_runtime_id(element->serial, out);
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_bounds(IRawElementProviderFragment *self, struct UiaRect *out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, fragment, self);
    hero_ui_parts_win32 *area = element->area;
    const hero_ui_win_part *part;
    POINT origin;
    uint32_t dpi;

    if (out == NULL) {
        return E_POINTER;
    }
    memset(out, 0, sizeof *out);
    hero_ui_parts_reading(area);
    if (!hero_ui_parts_live(element)) {
        hero_ui_parts_read_done(area);
        return HERO_UIA_GONE;
    }
    part = hero_ui_parts_given(element);
    hero_ui_parts_origin(area, &origin, &dpi);
    out->left = (double)origin.x + hero_ui_win_parts_pixels(part->box[0], dpi);
    out->top = (double)origin.y + hero_ui_win_parts_pixels(part->box[1], dpi);
    out->width = hero_ui_win_parts_pixels(part->box[2], dpi);
    out->height = hero_ui_win_parts_pixels(part->box[3], dpi);
    hero_ui_parts_read_done(area);
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_embedded(IRawElementProviderFragment *self, SAFEARRAY **out) {
    (void)self;
    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    return S_OK;
}

/* A reader asking a focusable part to take the keys: told, the program
 * moving its focus there or not. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_set_focus(IRawElementProviderFragment *self) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, fragment, self);
    HRESULT done;

    hero_ui_parts_reading(element->area);
    if (!hero_ui_parts_live(element)) {
        done = HERO_UIA_GONE;
    } else if ((hero_ui_parts_given(element)->states & HERO_UI_PART_FOCUSABLE) == 0) {
        done = HERO_UIA_REFUSED;
    } else {
        done = hero_ui_parts_posted(element->area, element->number, element->serial, HERO_UI_PART_FOCUS);
    }
    hero_ui_parts_read_done(element->area);
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_of(IRawElementProviderFragment *self, IRawElementProviderFragmentRoot **out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, fragment, self);
    HRESULT done = S_OK;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(element->area);
    if (!hero_ui_parts_live(element)) {
        done = HERO_UIA_GONE;
    } else {
        (void)hero_ui_parts_area_add(element->area);
        *out = &element->area->root;
    }
    hero_ui_parts_read_done(element->area);
    return done;
}

static const IRawElementProviderFragmentVtbl hero_ui_parts_fragment_table = {
    hero_ui_parts_fragment_query, hero_ui_parts_fragment_add, hero_ui_parts_fragment_release, hero_ui_parts_navigate, hero_ui_parts_runtime, hero_ui_parts_bounds, hero_ui_parts_embedded, hero_ui_parts_set_focus, hero_ui_parts_root_of,
};

/* -- A part's patterns: what a reader presses, toggles, selects and moves -------------- */

/* A press of `element`'s part, posted where it is live. */
static HRESULT hero_ui_parts_pressed(hero_ui_parts_element *element) {
    HRESULT done;

    hero_ui_parts_reading(element->area);
    done = hero_ui_parts_live(element) ? hero_ui_parts_posted(element->area, element->number, element->serial, HERO_UI_PART_PRESS) : HERO_UIA_GONE;
    hero_ui_parts_read_done(element->area);
    return done;
}

/* Whether `element`'s part is live and checked, `*checked`, under the
 * area's lock. */
static HRESULT hero_ui_parts_checked(hero_ui_parts_element *element, bool *checked) {
    HRESULT done = S_OK;

    *checked = false;
    hero_ui_parts_reading(element->area);
    if (hero_ui_parts_live(element)) {
        *checked = (hero_ui_parts_given(element)->states & HERO_UI_PART_CHECKED) != 0;
    } else {
        done = HERO_UIA_GONE;
    }
    hero_ui_parts_read_done(element->area);
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_invoked(IInvokeProvider *self) {
    return hero_ui_parts_pressed(HERO_UI_PARTS_OF(hero_ui_parts_element, invoke, self));
}

static const IInvokeProviderVtbl hero_ui_parts_invoke_table = {
    hero_ui_parts_invoke_query, hero_ui_parts_invoke_add, hero_ui_parts_invoke_release, hero_ui_parts_invoked,
};

static HRESULT STDMETHODCALLTYPE hero_ui_parts_toggled(IToggleProvider *self) {
    return hero_ui_parts_pressed(HERO_UI_PARTS_OF(hero_ui_parts_element, toggle, self));
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_toggle_state(IToggleProvider *self, enum ToggleState *out) {
    bool checked;
    HRESULT done;

    if (out == NULL) {
        return E_POINTER;
    }
    done = hero_ui_parts_checked(HERO_UI_PARTS_OF(hero_ui_parts_element, toggle, self), &checked);
    *out = checked ? ToggleState_On : ToggleState_Off;
    return done;
}

static const IToggleProviderVtbl hero_ui_parts_toggle_table = {
    hero_ui_parts_toggle_query, hero_ui_parts_toggle_add, hero_ui_parts_toggle_release, hero_ui_parts_toggled, hero_ui_parts_toggle_state,
};

static HRESULT STDMETHODCALLTYPE hero_ui_parts_selected(ISelectionItemProvider *self) {
    return hero_ui_parts_pressed(HERO_UI_PARTS_OF(hero_ui_parts_element, selection, self));
}

/* A radio part is one choice the program keeps; a reader adds none to a
 * selection and takes none from it. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_selection_refused(ISelectionItemProvider *self) {
    (void)self;
    return HERO_UIA_REFUSED;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_is_selected(ISelectionItemProvider *self, BOOL *out) {
    bool checked;
    HRESULT done;

    if (out == NULL) {
        return E_POINTER;
    }
    done = hero_ui_parts_checked(HERO_UI_PARTS_OF(hero_ui_parts_element, selection, self), &checked);
    *out = checked ? TRUE : FALSE;
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_container(ISelectionItemProvider *self, IRawElementProviderSimple **out) {
    (void)self;
    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    return S_OK;
}

static const ISelectionItemProviderVtbl hero_ui_parts_selection_table = {
    hero_ui_parts_selection_query, hero_ui_parts_selection_add, hero_ui_parts_selection_release, hero_ui_parts_selected, hero_ui_parts_selection_refused, hero_ui_parts_selection_refused, hero_ui_parts_is_selected, hero_ui_parts_container,
};

/* A slider moved to `value`: told as a step up above its value and down
 * below it, nothing where it is the value; a progress bar is read alone. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_set_value(IRangeValueProvider *self, double value) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, range, self);
    HRESULT done = S_OK;

    if (element->role != HERO_UI_PART_SLIDER) {
        return HERO_UIA_REFUSED;
    }
    if (isnan(value)) {
        return E_INVALIDARG;
    }
    hero_ui_parts_reading(element->area);
    if (!hero_ui_parts_live(element)) {
        done = HERO_UIA_GONE;
    } else if (value != hero_ui_parts_given(element)->value) {
        done = hero_ui_parts_posted(element->area, element->number, element->serial, value > hero_ui_parts_given(element)->value ? HERO_UI_PART_INCREASE : HERO_UI_PART_DECREASE);
    }
    hero_ui_parts_read_done(element->area);
    return done;
}

/* The range's numbers: 0 value, 1 least, 2 most, 3 a small step, 4 a large
 * one. */
static HRESULT hero_ui_parts_ranged(IRangeValueProvider *self, int which, double *out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, range, self);
    const hero_ui_win_part *part;
    HRESULT done = S_OK;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = 0;
    hero_ui_parts_reading(element->area);
    if (!hero_ui_parts_live(element)) {
        done = HERO_UIA_GONE;
    } else {
        part = hero_ui_parts_given(element);
        *out = which == 0 ? part->value : which == 1 ? part->least : which == 2 ? part->most : which == 3 ? hero_ui_parts_step(part) : hero_ui_parts_step(part) * 10.0;
    }
    hero_ui_parts_read_done(element->area);
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_value(IRangeValueProvider *self, double *out) {
    return hero_ui_parts_ranged(self, 0, out);
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_minimum(IRangeValueProvider *self, double *out) {
    return hero_ui_parts_ranged(self, 1, out);
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_maximum(IRangeValueProvider *self, double *out) {
    return hero_ui_parts_ranged(self, 2, out);
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_small(IRangeValueProvider *self, double *out) {
    return hero_ui_parts_ranged(self, 3, out);
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_large(IRangeValueProvider *self, double *out) {
    return hero_ui_parts_ranged(self, 4, out);
}

/* A slider takes a reader's value; a progress bar's is the program's to
 * show. The role is the object's own and never changes. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_read_only(IRangeValueProvider *self, BOOL *out) {
    hero_ui_parts_element *element = HERO_UI_PARTS_OF(hero_ui_parts_element, range, self);

    if (out == NULL) {
        return E_POINTER;
    }
    *out = element->role != HERO_UI_PART_SLIDER;
    return S_OK;
}

static const IRangeValueProviderVtbl hero_ui_parts_range_table = {
    hero_ui_parts_range_query, hero_ui_parts_range_add, hero_ui_parts_range_release, hero_ui_parts_set_value, hero_ui_parts_value, hero_ui_parts_read_only, hero_ui_parts_maximum, hero_ui_parts_minimum, hero_ui_parts_large, hero_ui_parts_small,
};

/* -- The root: the area as a group of its parts -------------------------------------- */

static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_pattern(IRawElementProviderSimple *self, PATTERNID pattern, IUnknown **out) {
    (void)self;
    (void)pattern;
    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    return S_OK;
}

/* What the root answers itself: a group while it holds parts and an image
 * again with none, named as the area is; the rest (its box, its focus, its
 * keyboard focusability) is the window's, which UI Automation reads from
 * the host provider. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_property(IRawElementProviderSimple *self, PROPERTYID property, VARIANT *out) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, simple, self);
    HRESULT done = S_OK;

    if (out == NULL) {
        return E_POINTER;
    }
    VariantInit(out);
    hero_ui_parts_reading(area);
    if (area->hwnd == NULL) {
        done = HERO_UIA_GONE;
    } else if (property == UIA_ControlTypePropertyId) {
        out->vt = VT_I4;
        out->lVal = area->count > 0 ? UIA_GroupControlTypeId : UIA_ImageControlTypeId;
    } else if (property == UIA_NamePropertyId && area->name != NULL) {
        out->vt = VT_BSTR;
        out->bstrVal = SysAllocString(area->name);
        if (out->bstrVal == NULL) {
            VariantInit(out);
            done = E_OUTOFMEMORY;
        }
    } else if (property == UIA_IsControlElementPropertyId || property == UIA_IsContentElementPropertyId) {
        hero_ui_parts_bool(out, true);
    }
    hero_ui_parts_read_done(area);
    return done;
}

/* The window's own provider, which UI Automation merges the root with. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_host(IRawElementProviderSimple *self, IRawElementProviderSimple **out) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, simple, self);
    HWND hwnd;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(area);
    hwnd = area->hwnd;
    hero_ui_parts_read_done(area);
    return hwnd != NULL ? area->uia.host(hwnd, out) : HERO_UIA_GONE;
}

static const IRawElementProviderSimpleVtbl hero_ui_parts_root_simple_table = {
    hero_ui_parts_root_simple_query, hero_ui_parts_root_simple_add, hero_ui_parts_root_simple_release, hero_ui_parts_options, hero_ui_parts_root_pattern, hero_ui_parts_root_property, hero_ui_parts_root_host,
};

static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_navigate(IRawElementProviderFragment *self, enum NavigateDirection direction, IRawElementProviderFragment **out) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, fragment, self);
    HRESULT done = S_OK;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(area);
    /* Its parent and siblings are the window's, which UI Automation knows. */
    if (area->hwnd == NULL) {
        done = HERO_UIA_GONE;
    } else if (direction == NavigateDirection_FirstChild) {
        *out = hero_ui_parts_handed(area, hero_ui_win_parts_first(area->parts, area->count));
    } else if (direction == NavigateDirection_LastChild) {
        *out = hero_ui_parts_handed(area, hero_ui_win_parts_last(area->parts, area->count));
    }
    hero_ui_parts_read_done(area);
    return done;
}

/* The root's runtime id, box and focus are its window's. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_runtime(IRawElementProviderFragment *self, SAFEARRAY **out) {
    (void)self;
    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_bounds(IRawElementProviderFragment *self, struct UiaRect *out) {
    (void)self;
    if (out == NULL) {
        return E_POINTER;
    }
    memset(out, 0, sizeof *out);
    return S_OK;
}

/* A reader giving the area the keys, as the window's own element gives
 * them: posted, since `SetFocus` takes the keys for its caller's thread. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_set_focus(IRawElementProviderFragment *self) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, fragment, self);
    HRESULT done;

    hero_ui_parts_reading(area);
    done = area->hwnd != NULL ? hero_ui_parts_posted(area, 0, 0, HERO_UI_PART_FOCUS) : HERO_UIA_GONE;
    hero_ui_parts_read_done(area);
    return done;
}

static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_root(IRawElementProviderFragment *self, IRawElementProviderFragmentRoot **out) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, fragment, self);

    if (out == NULL) {
        return E_POINTER;
    }
    (void)hero_ui_parts_area_add(area);
    *out = &area->root;
    return S_OK;
}

static const IRawElementProviderFragmentVtbl hero_ui_parts_root_fragment_table = {
    hero_ui_parts_root_fragment_query, hero_ui_parts_root_fragment_add, hero_ui_parts_root_fragment_release, hero_ui_parts_root_navigate, hero_ui_parts_root_runtime, hero_ui_parts_root_bounds, hero_ui_parts_embedded, hero_ui_parts_root_set_focus, hero_ui_parts_root_root,
};

/* The part under screen point `x`, `y`, NULL where it is the area's own. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_hit(IRawElementProviderFragmentRoot *self, double x, double y, IRawElementProviderFragment **out) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, root, self);
    HRESULT done = S_OK;
    POINT origin;
    uint32_t dpi;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(area);
    if (area->hwnd == NULL) {
        done = HERO_UIA_GONE;
    } else {
        hero_ui_parts_origin(area, &origin, &dpi);
        *out = hero_ui_parts_handed(area, hero_ui_win_parts_hit(area->parts, area->count, hero_ui_win_parts_points(x - (double)origin.x, dpi), hero_ui_win_parts_points(y - (double)origin.y, dpi)));
    }
    hero_ui_parts_read_done(area);
    return done;
}

/* The part the program's focus stands on: what UI Automation asks while the
 * area's window has the keys. */
static HRESULT STDMETHODCALLTYPE hero_ui_parts_root_focus(IRawElementProviderFragmentRoot *self, IRawElementProviderFragment **out) {
    hero_ui_parts_win32 *area = HERO_UI_PARTS_OF(hero_ui_parts_win32, root, self);
    HRESULT done = S_OK;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = NULL;
    hero_ui_parts_reading(area);
    if (area->hwnd == NULL) {
        done = HERO_UIA_GONE;
    } else {
        *out = hero_ui_parts_handed(area, hero_ui_parts_focused(area->parts, area->count));
    }
    hero_ui_parts_read_done(area);
    return done;
}

static const IRawElementProviderFragmentRootVtbl hero_ui_parts_root_table = {
    hero_ui_parts_root_root_query, hero_ui_parts_root_root_add, hero_ui_parts_root_root_release, hero_ui_parts_root_hit, hero_ui_parts_root_focus,
};

/* -- The member's calls ---------------------------------------------------------------- */

/* The area's lock taken by the UI thread to write it, and given back. */
static void hero_ui_parts_writing(hero_ui_parts_win32 *area) {
    AcquireSRWLockExclusive(&area->lock);
}

static void hero_ui_parts_write_done(hero_ui_parts_win32 *area) {
    ReleaseSRWLockExclusive(&area->lock);
}

/* The window's scale asked of the arm again, on the UI thread, and kept
 * where the objects read it. */
static void hero_ui_parts_rescaled(hero_ui_parts_win32 *area) {
    uint32_t dpi = area->scale != NULL ? area->scale(area->id) : 96;

    if (dpi != area->dpi) {
        hero_ui_parts_writing(area);
        area->dpi = dpi;
        hero_ui_parts_write_done(area);
    }
}

hero_ui_parts_win32 *hero_ui_parts_win32_new(int64_t id, void *window, hero_ui_parts_win32_scale scale, int64_t *answer) {
    hero_ui_parts_win32 *area;

    *answer = hero_ui_parts_win32_offered();
    if (*answer != HERO_UI_OK) {
        return NULL;
    }
    area = hero_layer_alloc_zero(1, sizeof *area);
    if (area == NULL) {
        *answer = HERO_UI_LIMIT;
        return NULL;
    }
    area->simple.lpVtbl = &hero_ui_parts_root_simple_table;
    area->fragment.lpVtbl = &hero_ui_parts_root_fragment_table;
    area->root.lpVtbl = &hero_ui_parts_root_table;
    area->references = 1;
    InitializeSRWLock(&area->lock);
    area->uia = hero_uia_loaded;
    area->thread = GetCurrentThreadId();
    area->told = hero_uia_told;
    area->id = id;
    area->scale = scale;
    area->hwnd = (HWND)window;
    area->dpi = scale != NULL ? scale(id) : 96;
    return area;
}

/* Part `number` of role `role`'s object, holding the area; NULL for no
 * memory. */
static hero_ui_parts_element *hero_ui_parts_element_new(hero_ui_parts_win32 *area, uint32_t number, uint8_t role) {
    hero_ui_parts_element *element = hero_layer_alloc_zero(1, sizeof *element);

    if (element == NULL) {
        return NULL;
    }
    element->simple.lpVtbl = &hero_ui_parts_simple_table;
    element->fragment.lpVtbl = &hero_ui_parts_fragment_table;
    element->invoke.lpVtbl = &hero_ui_parts_invoke_table;
    element->toggle.lpVtbl = &hero_ui_parts_toggle_table;
    element->selection.lpVtbl = &hero_ui_parts_selection_table;
    element->range.lpVtbl = &hero_ui_parts_range_table;
    element->references = 1;
    element->area = area;
    element->number = number;
    element->role = role;
    element->alive = true;
    (void)hero_ui_parts_area_add(area);
    return element;
}

/* `element`, marked given no more under the lock, disconnected from its
 * clients with no lock held, and the area's reference to it dropped. */
static void hero_ui_parts_element_end(hero_ui_parts_win32 *area, hero_ui_parts_element *element) {
    (void)area->uia.disconnect(&element->simple);
    (void)hero_ui_parts_element_release(element);
}

/* Tells the readers a property of a part kept across two lists changed. */
static void hero_ui_parts_changed(hero_ui_parts_element *element, PROPERTYID property, VARIANT *before, VARIANT *after) {
    (void)element->area->uia.changed(&element->simple, property, *before, *after);
    (void)VariantClear(before);
    (void)VariantClear(after);
}

static void hero_ui_parts_text_changed(hero_ui_parts_element *element, PROPERTYID property, const uint8_t *was, uint16_t was_count, const uint8_t *now, uint16_t now_count) {
    VARIANT before;
    VARIANT after;

    if (was_count == now_count && (now_count == 0 || memcmp(was, now, now_count) == 0)) {
        return;
    }
    VariantInit(&before);
    VariantInit(&after);
    if (hero_ui_parts_text(&before, was, was_count) && hero_ui_parts_text(&after, now, now_count)) {
        hero_ui_parts_changed(element, property, &before, &after);
    } else {
        (void)VariantClear(&before);
        (void)VariantClear(&after);
    }
}

static void hero_ui_parts_told_again(hero_ui_parts_element *element, const hero_ui_win_part *was) {
    const hero_ui_win_part *now = hero_ui_parts_given(element);
    VARIANT before;
    VARIANT after;

    hero_ui_parts_text_changed(element, UIA_NamePropertyId, was->name, was->named, now->name, now->named);
    hero_ui_parts_text_changed(element, UIA_HelpTextPropertyId, was->description, was->described, now->description, now->described);
    VariantInit(&before);
    VariantInit(&after);
    if ((was->states & HERO_UI_PART_FOCUSABLE) != (now->states & HERO_UI_PART_FOCUSABLE)) {
        hero_ui_parts_bool(&before, (was->states & HERO_UI_PART_FOCUSABLE) != 0);
        hero_ui_parts_bool(&after, (now->states & HERO_UI_PART_FOCUSABLE) != 0);
        hero_ui_parts_changed(element, UIA_IsKeyboardFocusablePropertyId, &before, &after);
    }
    if ((was->states & HERO_UI_PART_CHECKED) != (now->states & HERO_UI_PART_CHECKED)) {
        bool checked = (now->states & HERO_UI_PART_CHECKED) != 0;

        if (element->role == HERO_UI_PART_CHECK_BOX) {
            before.vt = VT_I4;
            before.lVal = checked ? ToggleState_Off : ToggleState_On;
            after.vt = VT_I4;
            after.lVal = checked ? ToggleState_On : ToggleState_Off;
            hero_ui_parts_changed(element, UIA_ToggleToggleStatePropertyId, &before, &after);
        } else {
            hero_ui_parts_bool(&before, !checked);
            hero_ui_parts_bool(&after, checked);
            hero_ui_parts_changed(element, UIA_SelectionItemIsSelectedPropertyId, &before, &after);
        }
    }
    if ((element->role == HERO_UI_PART_SLIDER || element->role == HERO_UI_PART_PROGRESS) && was->value != now->value) {
        before.vt = VT_R8;
        before.dblVal = was->value;
        after.vt = VT_R8;
        after.dblVal = now->value;
        hero_ui_parts_changed(element, UIA_RangeValueValuePropertyId, &before, &after);
    }
}

int64_t hero_ui_parts_win32_set(hero_ui_parts_win32 *area, const uint8_t *list, int64_t count) {
    size_t length = count > 0 ? (size_t)count : 0;
    size_t parts = length > 0 ? hero_ui_win_parts_counted(list, length) : 0;
    uint8_t *kept = NULL;
    hero_ui_win_part *read = NULL;
    hero_ui_parts_element **elements = NULL;
    uint8_t *was_list = area->list;
    hero_ui_win_part *was = area->parts;
    hero_ui_parts_element **was_elements = area->elements;
    size_t was_count = area->count;
    uint32_t was_focused = hero_ui_parts_focused(was, was_count);
    bool reshaped = parts != was_count || !area->offered;

    if (area->hwnd == NULL) {
        return hero_ui_refused(HERO_UI_INVALID, "the drawing area's window is gone");
    }
    if (length > 0 && parts == 0) {
        return hero_ui_refused(HERO_UI_INVALID, "parts the layer did not check reached the Win32 arm");
    }
    if (parts > 0) {
        kept = hero_layer_alloc(length);
        read = hero_layer_alloc_zero(parts, sizeof *read);
        elements = hero_layer_alloc_zero(parts, sizeof *elements);
        if (kept == NULL || read == NULL || elements == NULL) {
            goto no_memory;
        }
        memcpy(kept, list, length);
        if (hero_ui_win_parts_read(kept, length, read, parts) != parts) {
            hero_layer_free(kept);
            hero_layer_free(read);
            hero_layer_free(elements);
            return hero_ui_refused(HERO_UI_INVALID, "parts the layer did not check reached the Win32 arm");
        }
        /* A part of the same number and role keeps its object. */
        for (size_t number = 1; number <= parts; number += 1) {
            hero_ui_parts_element *old = number <= was_count ? was_elements[number - 1] : NULL;

            if (old != NULL && old->role == read[number - 1].role) {
                elements[number - 1] = old;
                reshaped = reshaped || was[number - 1].within != read[number - 1].within;
            } else {
                elements[number - 1] = hero_ui_parts_element_new(area, (uint32_t)number, read[number - 1].role);
                if (elements[number - 1] == NULL) {
                    goto no_memory;
                }
                area->serials += 1;
                elements[number - 1]->serial = area->serials;
                reshaped = true;
            }
        }
    }
    /* The new list in place, the objects given no more marked so, under the
     * lock; the scale asked again beside it. */
    hero_ui_parts_rescaled(area);
    hero_ui_parts_writing(area);
    area->list = kept;
    area->parts = read;
    area->elements = elements;
    area->count = parts;
    area->offered = area->offered || parts > 0;
    for (size_t number = 1; number <= was_count; number += 1) {
        if (number > parts || elements[number - 1] != was_elements[number - 1]) {
            was_elements[number - 1]->alive = false;
        }
    }
    hero_ui_parts_write_done(area);
    /* UI Automation told, with no lock held: it may call the objects back. */
    for (size_t number = 1; number <= was_count; number += 1) {
        if (number > parts || elements[number - 1] != was_elements[number - 1]) {
            hero_ui_parts_element_end(area, was_elements[number - 1]);
        }
    }
    if (area->offered && area->uia.listening()) {
        uint32_t focused = hero_ui_parts_focused(read, parts);

        for (size_t number = 1; number <= parts && number <= was_count; number += 1) {
            if (elements[number - 1] == was_elements[number - 1]) {
                hero_ui_parts_told_again(elements[number - 1], &was[number - 1]);
            }
        }
        if (reshaped) {
            (void)area->uia.structure(&area->simple, StructureChangeType_ChildrenInvalidated, NULL, 0);
        }
        if (focused != 0 && focused != was_focused && GetFocus() == area->hwnd) {
            (void)area->uia.event(&elements[focused - 1]->simple, UIA_AutomationFocusChangedEventId);
        }
    }
    hero_layer_free(was_list);
    hero_layer_free(was);
    hero_layer_free(was_elements);
    return HERO_UI_OK;

no_memory:
    if (elements != NULL) {
        for (size_t number = 1; number <= parts; number += 1) {
            if (elements[number - 1] != NULL && (number > was_count || elements[number - 1] != was_elements[number - 1])) {
                elements[number - 1]->alive = false;
                (void)hero_ui_parts_element_release(elements[number - 1]);
            }
        }
    }
    hero_layer_free(kept);
    hero_layer_free(read);
    hero_layer_free(elements);
    return hero_ui_refused(HERO_UI_LIMIT, "no memory for a drawing area's parts");
}

int64_t hero_ui_parts_win32_name(hero_ui_parts_win32 *area, const uint16_t *name) {
    WCHAR *copy = NULL;
    WCHAR *was;

    if (name != NULL) {
        size_t units = wcslen((const WCHAR *)name);

        copy = hero_layer_alloc((units + 1) * sizeof *copy);
        if (copy == NULL) {
            return hero_ui_refused(HERO_UI_LIMIT, "no memory for a drawing area's name for a screen reader");
        }
        memcpy(copy, name, (units + 1) * sizeof *copy);
    }
    hero_ui_parts_writing(area);
    was = area->name;
    area->name = copy;
    hero_ui_parts_write_done(area);
    hero_layer_free(was);
    return HERO_UI_OK;
}

/* The area gone: every object marked so under the lock, then UI Automation
 * told its window offers no root and every object disconnected from its
 * clients, with no lock held. */
static void hero_ui_parts_gone(hero_ui_parts_win32 *area) {
    HWND hwnd = area->hwnd;
    hero_ui_parts_element **elements = area->elements;
    size_t count = area->count;

    if (hwnd == NULL) {
        return;
    }
    hero_ui_parts_writing(area);
    area->hwnd = NULL;
    area->elements = NULL;
    area->count = 0;
    for (size_t number = 1; number <= count; number += 1) {
        elements[number - 1]->alive = false;
    }
    hero_ui_parts_write_done(area);
    if (area->offered) {
        (void)area->uia.give(hwnd, 0, 0, NULL);
    }
    for (size_t number = 1; number <= count; number += 1) {
        hero_ui_parts_element_end(area, elements[number - 1]);
    }
    hero_layer_free(elements);
    if (area->offered) {
        (void)area->uia.disconnect(&area->simple);
    }
}

void hero_ui_parts_win32_free(hero_ui_parts_win32 *area) {
    if (area == NULL) {
        return;
    }
    hero_ui_parts_gone(area);
    (void)hero_ui_parts_area_release(area);
}

/* A reader's act posted by an object (`hero_ui_parts_posted`), told on the
 * UI thread: part 0's focus gives the area the keys; a part's act is told
 * where the object it was asked of still holds that number. */
static void hero_ui_parts_act_told(hero_ui_parts_win32 *area, uintptr_t number, intptr_t words) {
    int64_t act = (int64_t)words % 16;
    int64_t serial = (int64_t)words / 16;

    if (area->hwnd == NULL) {
        return;
    }
    if (number == 0) {
        if (act == HERO_UI_PART_FOCUS) {
            (void)SetFocus(area->hwnd);
        }
        return;
    }
    if (number <= area->count && area->elements[number - 1]->serial == serial) {
        hero_ui_tell(HERO_UI_EVENT_PART, area->id, (int64_t)number, act);
    }
}

bool hero_ui_parts_win32_message(hero_ui_parts_win32 *area, uint32_t message, uintptr_t wparam, intptr_t lparam, intptr_t *result) {
    *result = 0;
    if (area->told != 0 && message == area->told) {
        hero_ui_parts_act_told(area, wparam, lparam);
        return true;
    }
    switch (message) {
    case WM_GETOBJECT:
        /* UI Automation's question alone; Active Accessibility's own
         * (`OBJID_CLIENT`) goes on to the window, which UI Automation's
         * bridge answers from the root. */
        if (area->hwnd != NULL && area->offered && (LONG)lparam == UiaRootObjectId) {
            *result = (intptr_t)area->uia.give(area->hwnd, (WPARAM)wparam, (LPARAM)lparam, &area->simple);
            return true;
        }
        return false;
    case WM_WINDOWPOSCHANGED:
    case WM_SHOWWINDOW:
    case 0x02E3: /* WM_DPICHANGED_AFTERPARENT, Windows 10 1703 */
        /* Placed again, perhaps at another scale. */
        if (area->hwnd != NULL) {
            hero_ui_parts_rescaled(area);
        }
        return false;
    case WM_SETFOCUS:
        /* The reader follows the keys to the part the program's focus
         * stands on. */
        if (area->hwnd != NULL) {
            hero_ui_parts_rescaled(area);
        }
        if (area->hwnd != NULL && area->offered && area->uia.listening()) {
            uint32_t focused = hero_ui_parts_focused(area->parts, area->count);

            if (focused != 0) {
                (void)area->uia.event(&area->elements[focused - 1]->simple, UIA_AutomationFocusChangedEventId);
            }
        }
        return false;
    case WM_DESTROY:
        hero_ui_parts_gone(area);
        return false;
    default:
        return false;
    }
}

/* -- Read back, and acted on, through the objects --------------------------------------- */

/* Pattern `pattern` of `element` as interface `iid`, NULL where it has none;
 * the caller releases it. */
static void *hero_ui_parts_face(hero_ui_parts_element *element, PATTERNID pattern, const IID *iid) {
    IUnknown *unknown = NULL;
    void *face = NULL;

    if (FAILED(element->simple.lpVtbl->GetPatternProvider(&element->simple, pattern, &unknown)) || unknown == NULL) {
        return NULL;
    }
    if (FAILED(unknown->lpVtbl->QueryInterface(unknown, iid, &face))) {
        face = NULL;
    }
    (void)unknown->lpVtbl->Release(unknown);
    return face;
}

/* Property `property` of `element` as text: UTF-8 in a block the caller
 * frees, its length through `count`; "" for none. False where it is no
 * text or longer than a record holds. */
static bool hero_ui_parts_asked_text(hero_ui_parts_element *element, PROPERTYID property, uint8_t **text, uint16_t *count) {
    VARIANT value;
    int bytes = 0;
    bool done = false;

    *text = NULL;
    *count = 0;
    if (FAILED(element->simple.lpVtbl->GetPropertyValue(&element->simple, property, &value))) {
        return false;
    }
    if (value.vt == VT_EMPTY) {
        return true;
    }
    if (value.vt == VT_BSTR) {
        UINT units = SysStringLen(value.bstrVal);

        bytes = units == 0 ? 0 : WideCharToMultiByte(CP_UTF8, 0, value.bstrVal, (int)units, NULL, 0, NULL, NULL);
        if (bytes >= 0 && bytes <= 65535 && (units == 0 || bytes > 0)) {
            *text = hero_layer_alloc((size_t)bytes);
            done = *text != NULL && (bytes == 0 || WideCharToMultiByte(CP_UTF8, 0, value.bstrVal, (int)units, (char *)*text, bytes, NULL, NULL) == bytes);
            *count = (uint16_t)bytes;
        }
    }
    (void)VariantClear(&value);
    return done;
}

/* Part `element` as its objects answer a reader, written to `out` (grown as
 * needed) at `*at`; false with words where an object answered no value. */
static bool hero_ui_parts_read_one(hero_ui_parts_win32 *area, hero_ui_parts_element *element, uint8_t **out, size_t *at, const char **words) {
    hero_ui_win_part got;
    VARIANT value;
    IRawElementProviderFragment *parent = NULL;
    IRawElementProviderFragment *focus = NULL;
    struct UiaRect rect;
    POINT origin;
    uint32_t dpi;
    uint8_t *name = NULL;
    uint8_t *description = NULL;
    size_t length;
    uint8_t *grown;
    bool read = false;

    memset(&got, 0, sizeof got);
    *words = "a part's object answered no control type, name, help text, focusability, parent, box or range";
    if (FAILED(element->simple.lpVtbl->GetPropertyValue(&element->simple, UIA_ControlTypePropertyId, &value)) || value.vt != VT_I4) {
        return false;
    }
    got.role = hero_ui_win_parts_role(value.lVal);
    if (FAILED(element->simple.lpVtbl->GetPropertyValue(&element->simple, UIA_IsKeyboardFocusablePropertyId, &value)) || value.vt != VT_BOOL) {
        return false;
    }
    got.states = value.boolVal != VARIANT_FALSE ? HERO_UI_PART_FOCUSABLE : 0;
    if (SUCCEEDED(area->root.lpVtbl->GetFocus(&area->root, &focus)) && focus != NULL) {
        got.states |= focus == &element->fragment ? HERO_UI_PART_FOCUSED : 0;
        (void)focus->lpVtbl->Release(focus);
    }
    if (got.role == HERO_UI_PART_CHECK_BOX) {
        IToggleProvider *toggle = hero_ui_parts_face(element, UIA_TogglePatternId, &hero_uia_toggle_iid);
        enum ToggleState state = ToggleState_Off;

        bool asked = toggle != NULL && SUCCEEDED(toggle->lpVtbl->get_ToggleState(toggle, &state));

        if (toggle != NULL) {
            (void)toggle->lpVtbl->Release(toggle);
        }
        if (!asked) {
            goto done;
        }
        got.states |= state == ToggleState_On ? HERO_UI_PART_CHECKED : 0;
    } else if (got.role == HERO_UI_PART_RADIO) {
        ISelectionItemProvider *selection = hero_ui_parts_face(element, UIA_SelectionItemPatternId, &hero_uia_selection_iid);
        BOOL selected = FALSE;

        bool asked = selection != NULL && SUCCEEDED(selection->lpVtbl->get_IsSelected(selection, &selected));

        if (selection != NULL) {
            (void)selection->lpVtbl->Release(selection);
        }
        if (!asked) {
            goto done;
        }
        got.states |= selected ? HERO_UI_PART_CHECKED : 0;
    } else if (got.role == HERO_UI_PART_SLIDER || got.role == HERO_UI_PART_PROGRESS) {
        IRangeValueProvider *range = hero_ui_parts_face(element, UIA_RangeValuePatternId, &hero_uia_range_iid);
        bool ranged = range != NULL && SUCCEEDED(range->lpVtbl->get_Value(range, &got.value)) && SUCCEEDED(range->lpVtbl->get_Minimum(range, &got.least)) && SUCCEEDED(range->lpVtbl->get_Maximum(range, &got.most));

        if (range != NULL) {
            (void)range->lpVtbl->Release(range);
        }
        if (!ranged) {
            goto done;
        }
    }
    /* The holder: the root, or a part of the area's. */
    if (FAILED(element->fragment.lpVtbl->Navigate(&element->fragment, NavigateDirection_Parent, &parent)) || parent == NULL) {
        goto done;
    }
    for (size_t number = 1; number <= area->count; number += 1) {
        if (parent == &area->elements[number - 1]->fragment) {
            got.within = (uint16_t)number;
        }
    }
    (void)parent->lpVtbl->Release(parent);
    if (FAILED(element->fragment.lpVtbl->get_BoundingRectangle(&element->fragment, &rect))) {
        goto done;
    }
    hero_ui_parts_origin(area, &origin, &dpi);
    got.box[0] = (float)hero_ui_win_parts_points(rect.left - (double)origin.x, dpi);
    got.box[1] = (float)hero_ui_win_parts_points(rect.top - (double)origin.y, dpi);
    got.box[2] = (float)hero_ui_win_parts_points(rect.width, dpi);
    got.box[3] = (float)hero_ui_win_parts_points(rect.height, dpi);
    if (!hero_ui_parts_asked_text(element, UIA_NamePropertyId, &name, &got.named) || !hero_ui_parts_asked_text(element, UIA_HelpTextPropertyId, &description, &got.described)) {
        goto done;
    }
    got.name = name;
    got.description = description;
    length = hero_ui_win_parts_put(NULL, 0, &got);
    grown = hero_layer_grow(*out, *at + length);
    if (grown == NULL) {
        *words = "no memory for a drawing area's parts read back";
        goto done;
    }
    *out = grown;
    (void)hero_ui_win_parts_put(*out + *at, length, &got);
    *at += length;
    read = true;
done:
    hero_layer_free(name);
    hero_layer_free(description);
    return read;
}

int64_t hero_ui_parts_win32_read(hero_ui_parts_win32 *area, uint8_t *buffer, int64_t count, int64_t *length) {
    uint8_t *out = NULL;
    size_t at = 0;
    const char *words = NULL;

    *length = 0;
    if (area->hwnd == NULL) {
        return hero_ui_refused(HERO_UI_INVALID, "the drawing area's window is gone");
    }
    hero_ui_parts_rescaled(area);
    for (size_t number = 1; number <= area->count; number += 1) {
        if (!hero_ui_parts_read_one(area, area->elements[number - 1], &out, &at, &words)) {
            hero_layer_free(out);
            return hero_ui_refused(HERO_UI_OTHER, words);
        }
    }
    if (buffer != NULL && count >= 0 && at <= (size_t)count && at > 0) {
        memcpy(buffer, out, at);
    }
    hero_layer_free(out);
    *length = (int64_t)at;
    return HERO_UI_OK;
}

int64_t hero_ui_parts_win32_act(hero_ui_parts_win32 *area, int64_t part, int64_t act) {
    hero_ui_parts_element *element;
    HRESULT done = E_FAIL;

    if (area->hwnd == NULL || part < 1 || (uint64_t)part > area->count) {
        return hero_ui_refused(HERO_UI_INVALID, "a screen reader's act on a part the drawing area does not have");
    }
    element = area->elements[part - 1];
    if (act == HERO_UI_PART_PRESS) {
        if (element->role == HERO_UI_PART_BUTTON || element->role == HERO_UI_PART_LINK) {
            IInvokeProvider *invoke = hero_ui_parts_face(element, UIA_InvokePatternId, &hero_uia_invoke_iid);

            done = invoke != NULL ? invoke->lpVtbl->Invoke(invoke) : E_NOINTERFACE;
            if (invoke != NULL) {
                (void)invoke->lpVtbl->Release(invoke);
            }
        } else if (element->role == HERO_UI_PART_CHECK_BOX) {
            IToggleProvider *toggle = hero_ui_parts_face(element, UIA_TogglePatternId, &hero_uia_toggle_iid);

            done = toggle != NULL ? toggle->lpVtbl->Toggle(toggle) : E_NOINTERFACE;
            if (toggle != NULL) {
                (void)toggle->lpVtbl->Release(toggle);
            }
        } else if (element->role == HERO_UI_PART_RADIO) {
            ISelectionItemProvider *selection = hero_ui_parts_face(element, UIA_SelectionItemPatternId, &hero_uia_selection_iid);

            done = selection != NULL ? selection->lpVtbl->Select(selection) : E_NOINTERFACE;
            if (selection != NULL) {
                (void)selection->lpVtbl->Release(selection);
            }
        } else {
            return hero_ui_refused(HERO_UI_INVALID, "a screen reader presses a button, a link, a check box or a radio button, and this part is none");
        }
    } else if (act == HERO_UI_PART_FOCUS) {
        done = element->fragment.lpVtbl->SetFocus(&element->fragment);
        if (done == HERO_UIA_REFUSED) {
            return hero_ui_refused(HERO_UI_INVALID, "a screen reader gives the keys to a focusable part, and this part is not focusable");
        }
    } else if (act == HERO_UI_PART_INCREASE || act == HERO_UI_PART_DECREASE) {
        IRangeValueProvider *range;

        if (element->role != HERO_UI_PART_SLIDER) {
            return hero_ui_refused(HERO_UI_INVALID, "a screen reader steps a slider, and this part is none");
        }
        range = hero_ui_parts_face(element, UIA_RangeValuePatternId, &hero_uia_range_iid);
        if (range != NULL) {
            /* The reader's arrow: the value plus or minus `SmallChange`. */
            double value = 0;
            double step = 0;

            done = range->lpVtbl->get_Value(range, &value);
            if (SUCCEEDED(done)) {
                done = range->lpVtbl->get_SmallChange(range, &step);
            }
            if (SUCCEEDED(done)) {
                done = range->lpVtbl->SetValue(range, act == HERO_UI_PART_INCREASE ? value + step : value - step);
            }
            (void)range->lpVtbl->Release(range);
        }
    } else {
        return hero_ui_refused(HERO_UI_INVALID, "a screen reader's act is HERO_UI_PART_PRESS to _DECREASE");
    }
    return SUCCEEDED(done) ? HERO_UI_OK : hero_ui_refused(HERO_UI_OTHER, "UI Automation's pattern refused a screen reader's act on a part");
}

#endif
