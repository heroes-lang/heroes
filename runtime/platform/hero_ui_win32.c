/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_win32.c: the native user interface's arm on Windows,
 * Win32's own controls, one member of the archive the compiler puts on every
 * program's link line (`selfhost/cli/layer.hero`), compiled to nothing on
 * every other system. `hero_ui.c` calls its functions, THE ARM of
 * `platform/hero_ui_arm.h`, on the UI thread with every argument checked;
 * it calls back what `hero_ui.c` keeps for both C arms, through the same
 * header.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled. */

#include "hero_ui_win32.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <cderr.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <oleacc.h>
#include <wchar.h>
#include "hero_ui_canvas_win32.h"
#include "hero_ui_parts_win32.h"
#include "hero_ui_print_win32.h"
#include "hero_ui_web_win32.h"
#include "hero_ui_win32_watch.h"
#include "hero_ui_win32_pickers.h"

/* ==========================================================================
 * THE WIN32 ARM: the system's own controls, `BUTTON`, `EDIT`, `STATIC`,
 * `LISTBOX`, `COMBOBOX` and comctl32's trackbar and progress bar, children of
 * top-level windows of this arm's own class, `HeroesUiWindow`, whose client
 * area is the window's content.
 *
 * EVERY CALL IS ON THE UI THREAD, the thread that opened the process's first
 * app (THE THREAD): Win32 delivers a window's messages to the thread that
 * made it, so every window here is that thread's and `pump` runs its queue.
 *
 * POINTS ARE MAPPED TO PIXELS PER MONITOR. The UI thread is made aware of
 * each monitor's scale while an app is open (`SetThreadDpiAwarenessContext`,
 * per-monitor version 2 where Windows has it), so each window is drawn at its
 * monitor's own scale: a window keeps its scale, `dpi`, and every place,
 * size and font of it is its points times `dpi / 96`, done again when the
 * window moves to a monitor of another scale (`WM_DPICHANGED`). The scaling
 * functions came with Windows 10 1607 and 1703, so each is found by
 * `GetProcAddress` and an older Windows draws at the system's one scale.
 *
 * THE SYSTEM'S CURRENT LOOK: a program's controls take version 6 of the
 * common controls, the look of the system's own programs, only through an
 * activation context asking for it, which the program's executable carries
 * none of. So `open` makes one from a manifest of a few lines written to a
 * file of the system's temporary folder and removed once read, and keeps it
 * active on the UI thread while the app is open; where no such file can be
 * made the controls are drawn in the classic look and work the same.
 *
 * TEXT crosses into UTF-16 at this boundary (`MultiByteToWideChar` of the
 * UTF-8 `hero_ui.c` checked) and back (`WideCharToMultiByte`, an unpaired
 * surrogate a person typed read as U+FFFD). A multi-line edit control and a
 * label hold lines ended by CR LF, so a line feed is written as CR LF and CR
 * LF read as a line feed; a button's, a checkbox's, a radio item's and a
 * menu's `&` is written `&&`, which Win32 draws as one `&` and not as the
 * underline of a key.
 *
 * WHAT THE PERSON DOES arrives as the notifications each control sends its
 * parent (`BN_CLICKED`, `EN_CHANGE`, `LBN_SELCHANGE`, `CBN_SELCHANGE`, a
 * trackbar's `WM_HSCROLL`), a menu's or a shortcut's `WM_COMMAND`, the close
 * button's `WM_CLOSE` and a new size's `WM_SIZE`; what the program sets is
 * muted where Win32 tells it too (an edit control's `EN_CHANGE` after
 * `SetWindowTextW`). A control's own number (`GWLP_ID`) is 0 and its
 * `GWLP_USERDATA` the widget's number, so a notification finds its widget
 * through the table and a stale one finds none: `WM_COMMAND` carries a
 * 16-bit number, which no 64-bit widget number fits.
 *
 * WHAT `inject` DOES is the control's own path: `BM_CLICK` presses a button,
 * a checkbox or a radio item, and where Win32 dropped it (its documentation
 * says a click of an inactive window's control may fail), the `BN_CLICKED`
 * the control sends is sent for it, so an injected act always makes its one
 * event; an edit is `EM_REPLACESEL` over the whole text, undoable as typing
 * is; a list's, a combo box's and a slider's choice is set and the
 * notification the control sends for a person's choice sent to its parent;
 * a menu item is the `WM_COMMAND` its menu sends; the close button is
 * `SC_CLOSE`.
 *
 * MENUS AND SHORTCUTS: a window's menu bar holds its menus' pop-ups, each
 * filled again, in the order made, when one of its items changes; Win32 has
 * no hidden menu item, so a hidden one is left out. A menu item is known to
 * Win32 by a 16-bit command number this arm gives, from 1000 up, past the
 * dialog manager's `IDOK` to `IDCONTINUE`; its shortcut is an entry of the
 * window's accelerator table, made again when a shortcut changes and handed
 * to `TranslateAcceleratorW` by `pump`, and shown after a tab in the item's
 * text by the keys' own names (`GetKeyNameTextW`). `pump` hands each message
 * to `IsDialogMessageW` too, so Tab and the arrows move between controls as
 * in a dialog.
 *
 * THE LOOP: `pump` takes the thread's messages until one queues an event or
 * its time passes, waiting in `MsgWaitForMultipleObjectsEx`, which also waits
 * on the events the poll sets the app's wait watches give
 * (`hero_ui_win32_watch.h`). While the person drags a window's border or holds
 * a menu open, Windows runs a loop of its own inside `DispatchMessageW`, and
 * `pump` returns when it ends.
 *
 * DIALOGS stand over their window, each shown by a thread of its own, so
 * the program goes on while the person answers (DIALOGS ANSWERED THROUGH
 * THE QUEUE).
 *
 * THE LIBRARIES ARE NAMED BY THE MEMBER, as `hero_net.c` names `ws2_32`:
 * `user32`, `gdi32`, `comctl32`, `comdlg32` and `msimg32` (`AlphaBlend`, a
 * picture's pixels drawn over the window's colour by their alpha), by
 * `#pragma comment(lib)`, so only a program that takes this member links them.
 *
 * Written from Microsoft's Win32 documentation; no other library's source.
 * ========================================================================== */

#if defined(_MSC_VER)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "dwmapi.lib")
#endif

_Static_assert(sizeof(LONG_PTR) == sizeof(int64_t), "a control's GWLP_USERDATA holds a widget's 64-bit number");

/* Win32's numbers this arm reads that an older SDK does not name. */
#define HERO_UI_WIN_DPICHANGED 0x02E0u
#define HERO_UI_WIN_AWARE_V2 ((HANDLE)(intptr_t)-4)
#define HERO_UI_WIN_AWARE ((HANDLE)(intptr_t)-3)
/* The first command number of a menu item, past `IDOK` to `IDCONTINUE`,
 * which the dialog manager sends as `WM_COMMAND` of its own. */
#define HERO_UI_WIN_FIRST_COMMAND 1000u
#define HERO_UI_WIN_LAST_COMMAND 0xEFFFu
/* The farthest a place or a size reaches, in points, so its pixels fit an
 * `int` at every scale Windows offers (500 %). */
#define HERO_UI_WIN_POINTS_MOST (INT64_C(1) << 24)
/* A slider's positions where its range is wider than a trackbar's 32 bits. */
#define HERO_UI_WIN_STEPS 1000000
/* The room a file dialog's answer has, in UTF-16 units. */
#define HERO_UI_WIN_PATHS_ROOM (1u << 20)
/* DWM's system backdrop, Windows 11 22H2's (`dwmapi.h` of an older SDK
 * names neither): the attribute and its Mica. */
#define HERO_UI_WIN_BACKDROP_TYPE 38
#define HERO_UI_WIN_BACKDROP_MICA 2
/* A monitor's own scale, `GetDpiForMonitor`'s effective one. */
#define HERO_UI_WIN_EFFECTIVE_DPI 0
/* The message a table posts itself to tell the rows chosen once a click's
 * notifications have all come (A TABLE). */
#define HERO_UI_WIN_SETTLE (WM_APP + 0x48)
/* The message a dialog's thread posts the UI thread's hidden window once
 * its dialog is answered or ended (DIALOGS ANSWERED THROUGH THE QUEUE). */
#define HERO_UI_WIN_DIALOG_DONE (WM_APP + 0x4A)
/* The message the thread that listens for a later launch posts the UI
 * thread's hidden window when it has queued what the launch handed over
 * (THE APP'S IDENTITY, ONE INSTANCE, AND DOCUMENTS FROM THE SYSTEM). */
#define HERO_UI_WIN_HANDED (WM_APP + 0x4B)
/* How long `press_key` waits for the key it sent to come back through the
 * queue, and how long the clipboard is asked of another program holding it. */
#define HERO_UI_WIN_KEY_WAIT_MS 2000u
#define HERO_UI_WIN_CLIPBOARD_TRIES 50
/* The first command number of a message box's button (`TaskDialogIndirect`). */
#define HERO_UI_WIN_ASK_FIRST 100

static const wchar_t hero_ui_win_window_class[] = L"HeroesUiWindow";
static const wchar_t hero_ui_win_panel_class[] = L"HeroesUiPanel";
static const wchar_t hero_ui_win_picture_class[] = L"HeroesUiPicture";
/* A drawing area's window, which paints itself. */
static const wchar_t hero_ui_win_canvas_class[] = L"HeroesUiCanvas";

/* The manifest of the activation context (THE SYSTEM'S CURRENT LOOK). */
static const char hero_ui_win_manifest[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
                                           "<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\r\n"
                                           "<dependency><dependentAssembly><assemblyIdentity type=\"win32\" "
                                           "name=\"Microsoft.Windows.Common-Controls\" version=\"6.0.0.0\" processorArchitecture=\"*\" "
                                           "publicKeyToken=\"6595b64144ccf1df\" language=\"*\"/></dependentAssembly></dependency>\r\n"
                                           "</assembly>\r\n";

struct hero_ui_native {
    int64_t id;
    int64_t kind;
    int64_t parent;
    /* The window it stands in; a window's own number, a menu item's its
     * menu's window. */
    int64_t window;
    /* The window, the control, a radio group's panel, a picture's view. */
    HWND hwnd;
    /* Shown as the program asks; a placed widget shows once placed too. */
    bool visible;
    /* A menu's, an item's and a radio group's, kept to fill a menu again
     * or to make the group's items alike. */
    bool enabled;
    bool placed;
    /* Where the program placed it, in points. */
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
    /* A radio group's items and where each was placed, four numbers an item,
     * a width below 0 where it was not. */
    HWND *radios;
    int64_t *radio_places;
    int64_t radio_count;
    /* A list's or a combo box's items. */
    int64_t items;
    /* A slider's or a progress bar's range and value, exact. */
    int64_t least;
    int64_t most;
    int64_t value;
    /* A window's menu bar, a menu's pop-up. */
    HMENU menu;
    /* A window's menus, a menu's items and separators, in the order made. */
    hero_ui_ids order;
    /* A menu's or an item's text, kept to fill its menu again. */
    wchar_t *label;
    /* An item's or a separator's command number. */
    WORD command;
    /* An item's shortcut: its virtual key, 0 for none, the keys held
     * (`FCONTROL`, `FSHIFT`, `FALT`) and whether the key is an extended one. */
    WORD key;
    BYTE held;
    bool key_extended;
    /* A window's scale, font, content size and least size in points, and its
     * accelerator table, made again where `accelerators_stale`. */
    UINT dpi;
    HFONT font;
    int64_t content_width;
    int64_t content_height;
    int64_t least_width;
    int64_t least_height;
    HACCEL accelerators;
    bool accelerators_stale;
    /* A picture's pixels, premultiplied as `AlphaBlend` takes them. */
    HBITMAP bitmap;
    int64_t bitmap_width;
    int64_t bitmap_height;
    /* What a widget whose parent this is stands in (CONTAINERS): a
     * window's client area, a page's, a pane's or a scrolled box's content
     * window; NULL for a widget that holds none. */
    HWND inside;
    /* The control beside the one `hwnd` names that a widget of two controls
     * keeps: a group box's frame, the `BUTTON` that draws it, and a spin's
     * arrows, the up-down control (A GROUP BOX, AN EDITABLE COMBO BOX, A
     * SPIN), and the system's own toolbar or status bar that a bar's items
     * stand over (A TOOLBAR, A STATUS BAR, THE PICKERS). */
    HWND part;
    /* A window's place among the screens (WINDOWS ON THE SCREEN): whether
     * it is watched, the state and the screen last told, in full screen with
     * the style and the place to give back, framed, made modal, disabled by
     * another window's modality or by a dialog standing over none, its
     * backdrop. */
    bool watched;
    int64_t state_told;
    int64_t screen_told;
    int64_t moved_x;
    int64_t moved_y;
    bool full_screen;
    LONG_PTR style_before;
    LONG_PTR extended_before;
    WINDOWPLACEMENT placement_before;
    bool unframed;
    bool resizable;
    bool modal;
    bool held_by_modal;
    /* Whether a window takes files dropped on it (FILES DROPPED ON A
     * WINDOW). */
    bool takes_files;
    int64_t backdrop;
    /* A window's control that had the keys when it was left, given them
     * again when it is activated (THE WIDGET WITH THE KEYS); whether it was
     * shown once; whether its controls' Tab order wants making again; its
     * buttons for Return and for Escape. */
    HWND focus;
    bool shown_once;
    bool tabs_stale;
    int64_t enter_button;
    int64_t escape_button;
    /* Its name for a screen reader, the program's own (NULL for none), and
     * the label tied to it, 0 for none (ACCESSIBLE NAMES). */
    wchar_t *access_name;
    int64_t label_for;
    /* Its tip, UTF-16 as a tooltip shows it (NULL for none), and, of a
     * window, the tooltip control its widgets' tips are tools of (A TIP OVER
     * A WIDGET). */
    wchar_t *tip;
    HWND tooltips;
    /* A tray's icon of its own (NULL for the app's) and whether Windows
     * shows it (AN ICON IN THE TRAY). */
    HICON tray_icon;
    bool tray_shown;
    /* A menu item's check mode, `HERO_UI_CHECK_*`, and its state. */
    int64_t check;
    bool checked;
    /* A split's divider dragged by the person: the offset of the mouse in
     * it, in pixels. */
    bool dragging;
    int drag_offset;
    /* A scrolled box's content size in points and its scroll, in pixels. */
    int64_t scroll_width;
    int64_t scroll_height;
    int scrolled;
    /* A table's columns, its cells row after row as UTF-16, the column it
     * is shown sorted by (-1 for none) and its direction, the rows last told
     * chosen, and whether a choice waits to be told. */
    int64_t columns;
    wchar_t **cells;
    int64_t cell_count;
    int64_t sort_column;
    bool sort_ascending;
    int64_t *told;
    int64_t told_count;
    bool choice_waits;
    /* A tree's items, in the order added. */
    struct hero_ui_win_node *nodes;
    /* A drawing area's drawing, its pointer and its keys
     * (`hero_ui_canvas_win32.c`). */
    hero_ui_canvas_win32 *canvas;
    /* Its parts for a screen reader, made at the first parts the program
     * gives (`hero_ui_parts_win32.c`). */
    hero_ui_parts_win32 *parts;
    int64_t node_count;
    int64_t node_room;
    /* A text widget's text as its control last held it, UTF-16 as the
     * control keeps it (`known_count` -1 where it could not be kept), and
     * the edits kept for its undo, the first `step_done` done and the rest
     * undone; whether the last is a person's run of typed characters the
     * next one may join (UNDO AND THE SELECTION IN TEXT WIDGETS). */
    wchar_t *known;
    int64_t known_count;
    struct hero_ui_win_step *steps;
    int64_t step_count;
    int64_t step_room;
    int64_t step_done;
    bool typing;
    /* A colour well's sixteen custom colours, kept for the chooser it opens
     * (A TOOLBAR, A STATUS BAR, THE PICKERS); NULL until it opens one. */
    COLORREF *customs;
};

/* A tree's item: the program's number for it, the item it stands under (0
 * the top), the tree view's handle, whether it shows its triangle while it
 * holds none, and whether the person unfolded it while it held none. */
typedef struct hero_ui_win_node {
    int64_t item;
    int64_t under;
    HTREEITEM handle;
    bool foldable;
    bool waiting;
    /* Folded by the person with the choice under it let go, told after the
     * fold. */
    bool let_go;
} hero_ui_win_node;

/* An edit kept for undo: the `gone_count` units it took out of a text at
 * `at` and the `came_count` units it put in their place, each block ended by
 * a zero so it can be put back as it stands. */
typedef struct hero_ui_win_step {
    int64_t at;
    wchar_t *gone;
    int64_t gone_count;
    wchar_t *came;
    int64_t came_count;
} hero_ui_win_step;

/* The scaling functions of `user32` (POINTS ARE MAPPED), NULL where this
 * Windows has none. */
typedef UINT(WINAPI *hero_ui_win_dpi_of_window)(HWND);
typedef BOOL(WINAPI *hero_ui_win_adjust_for_dpi)(LPRECT, DWORD, BOOL, DWORD, UINT);
typedef BOOL(WINAPI *hero_ui_win_parameters_for_dpi)(UINT, UINT, PVOID, UINT, UINT);
typedef int(WINAPI *hero_ui_win_metrics_for_dpi)(int, UINT);
typedef HANDLE(WINAPI *hero_ui_win_thread_awareness)(HANDLE);

static _Thread_local hero_ui_win_dpi_of_window hero_ui_win_dpi_for_window;
static _Thread_local hero_ui_win_adjust_for_dpi hero_ui_win_adjust;
static _Thread_local hero_ui_win_parameters_for_dpi hero_ui_win_parameters;
static _Thread_local hero_ui_win_metrics_for_dpi hero_ui_win_metrics;
static _Thread_local hero_ui_win_thread_awareness hero_ui_win_awareness;

/* Whether the classes are registered and the functions found, which `open`
 * does once for the process's life. */
static _Thread_local bool hero_ui_win_ready;
/* The activation context, NULL before it was asked for and
 * `INVALID_HANDLE_VALUE` where none could be made; its cookie while active. */
static _Thread_local HANDLE hero_ui_win_context;
static _Thread_local ULONG_PTR hero_ui_win_cookie;
static _Thread_local bool hero_ui_win_activated;
/* The thread's awareness of scale before `open`, given back at `close`. */
static _Thread_local HANDLE hero_ui_win_awareness_before;
static _Thread_local bool hero_ui_win_aware;
/* The next command number a menu item may take. */
static _Thread_local UINT hero_ui_win_next_command;
/* Above 0 while `inject` acts, so a slider moved to where it stands is told. */
static _Thread_local int64_t hero_ui_win_injecting;

/* `shcore`'s scale of a monitor (Windows 8.1) and `comctl32` version 6's
 * task dialog, each found by `GetProcAddress` where this Windows has them,
 * since a program's import of either would stop it loading where it does
 * not (version 5 of the common controls, which a program with no manifest
 * binds, has no task dialog). */
typedef HRESULT(WINAPI *hero_ui_win_monitor_dpi)(HMONITOR, int, UINT *, UINT *);
typedef HRESULT(WINAPI *hero_ui_win_task_dialog)(const TASKDIALOGCONFIG *, int *, int *, BOOL *);
static _Thread_local hero_ui_win_monitor_dpi hero_ui_win_dpi_for_monitor;
static _Thread_local hero_ui_win_task_dialog hero_ui_win_task;
static _Thread_local bool hero_ui_win_task_asked;
/* Above 0 while the program's own call moves a window or changes its
 * state, so WINDOWS ON THE SCREEN tells none of it as the person's. */
static _Thread_local int64_t hero_ui_win_quiet;
/* Whether an ask of the session's end, and one of another program's, is
 * held for the program's answer, and the window whose reason Windows
 * shows (THE SESSION'S END ASKING THE PROGRAM). */
static _Thread_local bool hero_ui_win_session_held;
static _Thread_local bool hero_ui_win_closing_held;
static _Thread_local HWND hero_ui_win_blocking;
/* The appearance a program injected, 0 for the system's own; the one last
 * told. */
static _Thread_local int64_t hero_ui_win_appearance_forced;
static _Thread_local int64_t hero_ui_win_appearance_told;
/* The message Explorer sends every top-level window when it starts again,
 * its tray empty. */
static _Thread_local UINT hero_ui_win_taskbar_created;
/* The window made modal, 0 for none. */
static _Thread_local int64_t hero_ui_win_modal;
/* The app's icon, NULL for the system's own. */
static _Thread_local HICON hero_ui_win_icon;
/* The hidden window a dialog's thread tells once done, made at the first
 * dialog (DIALOGS ANSWERED THROUGH THE QUEUE). */
static _Thread_local HWND hero_ui_win_dialog_host;
/* The hidden window the clipboard is opened by, made at its first use. */
static _Thread_local HWND hero_ui_win_clip_owner;
/* What the last read of the clipboard found, in `hero_ui_clipboard_write`'s
 * form; NULL with -1 bytes where no read found anything. */
static _Thread_local uint8_t *hero_ui_win_clip;
static _Thread_local int64_t hero_ui_win_clip_count = -1;

/* -- Words, numbers and text ------------------------------------------------ */

/* The WebView2 member's answer `answer`, its words kept where it refused
 * `HERO_UI_OTHER` or `HERO_UI_UNSUPPORTED`, so the failure says which
 * part of WebView2 is missing. */
static int64_t hero_ui_win_web(int64_t answer) {
    char words[HERO_UI_WORDS_MOST];

    if (answer != HERO_UI_OTHER && answer != HERO_UI_UNSUPPORTED) {
        return answer;
    }
    if (hero_ui_web_win32_words(words, (int64_t)sizeof words) <= 0) {
        (void)snprintf(words, sizeof words, "WebView2 refused, giving no words");
    }
    return hero_ui_refused(answer, words);
}

/* Answers `HERO_UI_OTHER` with `what` and the system's words for `code`. */
static int64_t hero_ui_win_failed(const char *what, DWORD code) {
    char words[HERO_UI_WORDS_MOST];
    wchar_t theirs[256];
    char narrow[HERO_UI_WORDS_MOST];
    DWORD length = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, 0, theirs, (DWORD)(sizeof theirs / sizeof theirs[0]), NULL);
    int made;

    while (length > 0 && (theirs[length - 1] == L'\r' || theirs[length - 1] == L'\n' || theirs[length - 1] == L' ' || theirs[length - 1] == L'.')) {
        length -= 1;
    }
    made = length == 0 ? 0 : WideCharToMultiByte(CP_UTF8, 0, theirs, (int)length, narrow, (int)sizeof narrow - 1, NULL, NULL);
    narrow[made > 0 ? made : 0] = '\0';
    (void)snprintf(words, sizeof words, "%s: Windows error %lu%s%s", what, (unsigned long)code, made > 0 ? ", " : "", narrow);
    return hero_ui_refused(HERO_UI_OTHER, words);
}

/* `points` as pixels at `dpi`, rounded to the nearest. */
static int hero_ui_win_px(int64_t points, UINT dpi) {
    int64_t bounded = hero_ui_between(points, -HERO_UI_WIN_POINTS_MOST, HERO_UI_WIN_POINTS_MOST);

    return MulDiv((int)bounded, (int)dpi, 96);
}

/* `pixels` as points at `dpi`, rounded to the nearest. */
static int64_t hero_ui_win_pt(int pixels, UINT dpi) {
    return dpi == 0 ? pixels : (int64_t)MulDiv(pixels, 96, (int)dpi);
}

/* `pixels` as points at `dpi`, rounded up, so a size measured is never cut. */
static int64_t hero_ui_win_pt_up(int pixels, UINT dpi) {
    int64_t scaled = (int64_t)(pixels < 0 ? 0 : pixels) * 96;

    return dpi == 0 ? pixels : (scaled + (int64_t)dpi - 1) / (int64_t)dpi;
}

/* What `hero_ui_win_wide` writes besides the UTF-16 of the text. */
#define HERO_UI_WIN_LINES 1u
#define HERO_UI_WIN_PREFIX 2u
#define HERO_UI_WIN_TABS 4u

/* `count` bytes of checked UTF-8 at `text` as UTF-16 with its zero, in a
 * block of `hero_layer_alloc`, or NULL where memory ran out: a line feed as
 * CR LF where `HERO_UI_WIN_LINES`, an `&` as `&&` where `HERO_UI_WIN_PREFIX`,
 * a tab as a space where `HERO_UI_WIN_TABS`. UTF-8 takes at least as many
 * bytes as UTF-16 takes units, so `count` units hold the text. */
static wchar_t *hero_ui_win_wide(const uint8_t *text, int64_t count, unsigned ways) {
    wchar_t *plain = hero_layer_alloc(((size_t)count + 1) * sizeof(wchar_t));
    wchar_t *written;
    int made = 0;
    size_t more = 0;
    size_t at = 0;

    if (plain == NULL) {
        return NULL;
    }
    if (count > 0) {
        made = MultiByteToWideChar(CP_UTF8, 0, (const char *)text, (int)count, plain, (int)count);
        if (made <= 0) {
            hero_layer_free(plain);
            return NULL;
        }
    }
    plain[made] = 0;
    for (int i = 0; i < made; i += 1) {
        more += ((ways & HERO_UI_WIN_LINES) != 0 && plain[i] == L'\n') || ((ways & HERO_UI_WIN_PREFIX) != 0 && plain[i] == L'&') ? 1u : 0u;
    }
    if (more == 0 && (ways & HERO_UI_WIN_TABS) == 0) {
        return plain;
    }
    written = hero_layer_alloc(((size_t)made + more + 1) * sizeof(wchar_t));
    if (written == NULL) {
        hero_layer_free(plain);
        return NULL;
    }
    for (int i = 0; i < made; i += 1) {
        if ((ways & HERO_UI_WIN_LINES) != 0 && plain[i] == L'\n') {
            written[at++] = L'\r';
        } else if ((ways & HERO_UI_WIN_PREFIX) != 0 && plain[i] == L'&') {
            written[at++] = L'&';
        }
        written[at++] = (ways & HERO_UI_WIN_TABS) != 0 && plain[i] == L'\t' ? L' ' : plain[i];
    }
    written[at] = 0;
    hero_layer_free(plain);
    return written;
}

/* `units` units of UTF-16 at `wide` as UTF-8 in a block of
 * `hero_layer_alloc`, its length through `*bytes`, CR LF read as a line feed
 * where `lines` (the one CR `hero_ui_win_wide` writes before every line feed,
 * so a text written and read back is the text written); NULL where memory
 * ran out. */
static uint8_t *hero_ui_win_narrow(const wchar_t *wide, int units, bool lines, int64_t *bytes) {
    uint8_t *narrow;
    int made = 0;
    int64_t kept = 0;

    *bytes = 0;
    if (units > 0) {
        made = WideCharToMultiByte(CP_UTF8, 0, wide, units, NULL, 0, NULL, NULL);
    }
    narrow = hero_layer_alloc(made > 0 ? (size_t)made : 1u);
    if (narrow == NULL) {
        return NULL;
    }
    if (made > 0) {
        made = WideCharToMultiByte(CP_UTF8, 0, wide, units, (char *)narrow, made, NULL, NULL);
    }
    for (int i = 0; i < made; i += 1) {
        if (lines && narrow[i] == '\r' && i + 1 < made && narrow[i + 1] == '\n') {
            continue;
        }
        narrow[kept] = narrow[i];
        kept += 1;
    }
    *bytes = kept;
    return narrow;
}

/* The text of control `hwnd` as UTF-8 in a block of `hero_layer_alloc`, its
 * length through `*bytes`, CR LF read as a line feed where `lines`; NULL
 * where memory ran out. */
static uint8_t *hero_ui_win_text_of(HWND hwnd, bool lines, int64_t *bytes) {
    int units = GetWindowTextLengthW(hwnd);
    wchar_t *wide;
    uint8_t *narrow;
    int read;

    *bytes = 0;
    if (units < 0) {
        units = 0;
    }
    wide = hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t));
    if (wide == NULL) {
        return NULL;
    }
    read = GetWindowTextW(hwnd, wide, units + 1);
    narrow = hero_ui_win_narrow(wide, read > 0 ? read : 0, lines, bytes);
    hero_layer_free(wide);
    return narrow;
}

/* The text of control `hwnd` as it holds it, UTF-16 with its zero, in a
 * block of `hero_layer_alloc`, or NULL where memory ran out. */
static wchar_t *hero_ui_win_wide_of(HWND hwnd) {
    int units = GetWindowTextLengthW(hwnd);
    wchar_t *text = hero_layer_alloc(((size_t)(units > 0 ? units : 0) + 1) * sizeof(wchar_t));

    if (text != NULL) {
        text[0] = 0;
        (void)GetWindowTextW(hwnd, text, (units > 0 ? units : 0) + 1);
    }
    return text;
}

/* The widget whose window or control is `hwnd`, by the number it carries,
 * or NULL: a radio item answers its group. */
static hero_ui_native *hero_ui_win_of(HWND hwnd) {
    LONG_PTR id = hwnd == NULL ? 0 : GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    hero_ui_native *widget = id > 0 ? hero_ui_find((int64_t)id) : NULL;

    if (widget == NULL) {
        return NULL;
    }
    if (widget->hwnd == hwnd) {
        return widget;
    }
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        if (widget->radios[i] == hwnd) {
            return widget;
        }
    }
    return NULL;
}

/* The window of number `id`, or NULL. */
static hero_ui_native *hero_ui_win_window(int64_t id) {
    hero_ui_native *window = hero_ui_find(id);

    return window != NULL && window->kind == HERO_UI_WINDOW ? window : NULL;
}

/* The window or the tray of number `id`, what a menu's items stand in, or
 * NULL. */
static hero_ui_native *hero_ui_win_top(int64_t id) {
    hero_ui_native *top = hero_ui_find(id);

    return top != NULL && (top->kind == HERO_UI_WINDOW || top->kind == HERO_UI_TRAY) ? top : NULL;
}

/* The scale `hwnd` is drawn at, the system's where Windows names no window's
 * own. */
static UINT hero_ui_win_dpi(HWND hwnd) {
    HDC screen;
    int dpi = 0;

    if (hero_ui_win_dpi_for_window != NULL && hwnd != NULL) {
        UINT own = hero_ui_win_dpi_for_window(hwnd);

        if (own > 0) {
            return own;
        }
    }
    screen = GetDC(NULL);
    if (screen != NULL) {
        dpi = GetDeviceCaps(screen, LOGPIXELSX);
        (void)ReleaseDC(NULL, screen);
    }
    return dpi > 0 ? (UINT)dpi : 96u;
}

/* The system's metric `which` at `dpi`. */
static int hero_ui_win_metric(int which, UINT dpi) {
    if (hero_ui_win_metrics != NULL) {
        return hero_ui_win_metrics(which, dpi);
    }
    return MulDiv(GetSystemMetrics(which), (int)dpi, (int)hero_ui_win_dpi(NULL));
}

/* The font the system's own dialogs write with, at `dpi`. */
static HFONT hero_ui_win_font(UINT dpi) {
    NONCLIENTMETRICSW metrics;
    HFONT made = NULL;

    memset(&metrics, 0, sizeof metrics);
    metrics.cbSize = (UINT)sizeof metrics;
    if (hero_ui_win_parameters != NULL && hero_ui_win_parameters(SPI_GETNONCLIENTMETRICS, (UINT)sizeof metrics, &metrics, 0, dpi)) {
        made = CreateFontIndirectW(&metrics.lfMessageFont);
    } else if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, (UINT)sizeof metrics, &metrics, 0)) {
        metrics.lfMessageFont.lfHeight = MulDiv(metrics.lfMessageFont.lfHeight, (int)dpi, (int)hero_ui_win_dpi(NULL));
        made = CreateFontIndirectW(&metrics.lfMessageFont);
    }
    /* A stock object is never deleted, and `DeleteObject` of one is
     * harmless (its documentation), so this font is given back as a made
     * one is. */
    return made != NULL ? made : (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

/* The size `text` takes in `hwnd`'s font, in pixels: `DrawTextW`'s answer. */
static SIZE hero_ui_win_text_size(HWND hwnd, const wchar_t *text, UINT format) {
    SIZE size = {0, 0};
    RECT area = {0, 0, 0, 0};
    HDC dc = GetDC(hwnd);
    HGDIOBJ font;
    HGDIOBJ before = NULL;

    if (dc == NULL) {
        return size;
    }
    font = (HGDIOBJ)SendMessageW(hwnd, WM_GETFONT, 0, 0);
    if (font != NULL) {
        before = SelectObject(dc, font);
    }
    (void)DrawTextW(dc, text, -1, &area, format | DT_CALCRECT | DT_NOPREFIX);
    if (before != NULL) {
        (void)SelectObject(dc, before);
    }
    (void)ReleaseDC(hwnd, dc);
    size.cx = area.right - area.left;
    size.cy = area.bottom - area.top;
    return size;
}

/* The height of one line of `hwnd`'s font, in pixels. */
static int hero_ui_win_line(HWND hwnd) {
    TEXTMETRICW metrics;
    HDC dc = GetDC(hwnd);
    HGDIOBJ font;
    HGDIOBJ before = NULL;
    int height = 16;

    if (dc == NULL) {
        return height;
    }
    font = (HGDIOBJ)SendMessageW(hwnd, WM_GETFONT, 0, 0);
    if (font != NULL) {
        before = SelectObject(dc, font);
    }
    if (GetTextMetricsW(dc, &metrics)) {
        height = (int)metrics.tmHeight;
    }
    if (before != NULL) {
        (void)SelectObject(dc, before);
    }
    (void)ReleaseDC(hwnd, dc);
    return height;
}

/* -- Windows: their frame, their content, their scale ----------------------- */

/* `area`, a window's client rectangle, grown to the window's frame. */
static void hero_ui_win_frame_of(const hero_ui_native *window, RECT *area) {
    DWORD style = (DWORD)GetWindowLongPtrW(window->hwnd, GWL_STYLE);
    DWORD extended = (DWORD)GetWindowLongPtrW(window->hwnd, GWL_EXSTYLE);
    BOOL bar = GetMenu(window->hwnd) != NULL;

    if (hero_ui_win_adjust != NULL) {
        (void)hero_ui_win_adjust(area, style, bar, extended, window->dpi);
    } else {
        (void)AdjustWindowRectEx(area, style, bar, extended);
    }
}

/* Gives window `window` the content size it keeps, queuing nothing. */
static void hero_ui_win_size_content(hero_ui_native *window) {
    int width = hero_ui_win_px(window->content_width, window->dpi);
    int height = hero_ui_win_px(window->content_height, window->dpi);
    RECT frame = {0, 0, width, height};
    RECT client;

    hero_ui_win_frame_of(window, &frame);
    hero_ui_mute(1);
    (void)SetWindowPos(window->hwnd, NULL, 0, 0, frame.right - frame.left, frame.bottom - frame.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    /* A menu bar wrapped onto more lines than the frame counts takes its
     * height out of the content: given back once. */
    if (GetClientRect(window->hwnd, &client) && client.bottom != height && client.right == width) {
        (void)SetWindowPos(window->hwnd, NULL, 0, 0, frame.right - frame.left, frame.bottom - frame.top + (height - client.bottom), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    hero_ui_mute(-1);
}

/* Places `widget` where the program placed it, at its window's scale, and
 * shows it where it is placed and asked to be. */
static void hero_ui_win_lay_box(hero_ui_native *widget);

static void hero_ui_win_put(hero_ui_native *widget) {
    hero_ui_native *window = hero_ui_win_window(widget->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    int height = hero_ui_win_px(widget->height, dpi);
    UINT shown = widget->placed && widget->visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;

    if (widget->kind == HERO_UI_PAGE || widget->kind == HERO_UI_PANE) {
        /* The system's box places it (BOXES THE SYSTEM DRAWS). */
        return;
    }

    if (widget->kind == HERO_UI_WEBVIEW) {
        /* WebView2 draws into the window's client area where its member
         * puts it, in that window's pixels. */
        if (widget->placed) {
            (void)hero_ui_web_win32_place(widget->id, hero_ui_win_px(widget->x, dpi), hero_ui_win_px(widget->y, dpi), hero_ui_win_px(widget->width, dpi), height);
        }
        (void)hero_ui_web_win32_show(widget->id, widget->placed && widget->visible ? 1 : 0);
        return;
    }
    if (widget->hwnd == NULL) {
        return;
    }
    if (widget->kind == HERO_UI_COMBO || widget->kind == HERO_UI_EDITABLE_COMBO) {
        /* A combo box's height is its field's and its list's together; the
         * field keeps the height of its font, and its list room for 12 items. */
        int item = (int)SendMessageW(widget->hwnd, CB_GETITEMHEIGHT, 0, 0);
        RECT field;

        if (GetWindowRect(widget->hwnd, &field) && field.bottom - field.top > height) {
            height = field.bottom - field.top;
        }
        height += (item > 0 ? item : 16) * 12 + 2;
    }
    (void)SetWindowPos(widget->hwnd, NULL, hero_ui_win_px(widget->x, dpi), hero_ui_win_px(widget->y, dpi), hero_ui_win_px(widget->width, dpi), height, SWP_NOZORDER | SWP_NOACTIVATE | shown);
    hero_ui_win_lay_box(widget);
    if (widget->kind == HERO_UI_SPIN && widget->part != NULL) {
        /* The arrows take their width out of the field's again, as `UDM_SETBUDDY`
         * does, so field and arrows together are the place given. */
        (void)SendMessageW(widget->part, UDM_SETBUDDY, (WPARAM)widget->hwnd, 0);
        (void)ShowWindow(widget->part, shown == SWP_SHOWWINDOW ? SW_SHOWNA : SW_HIDE);
    }
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        const int64_t *place = &widget->radio_places[i * 4];

        if (place[2] < 0) {
            (void)SetWindowPos(widget->radios[i], NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
            continue;
        }
        (void)SetWindowPos(widget->radios[i], NULL, hero_ui_win_px(place[0] - widget->x, dpi), hero_ui_win_px(place[1] - widget->y, dpi), hero_ui_win_px(place[2], dpi), hero_ui_win_px(place[3], dpi), SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

/* Gives every control of window `window` its font, `SendMessageW` reaching
 * a radio group's items too. */
static void hero_ui_win_give_font(hero_ui_native *window) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        if (widget == NULL || widget->window != window->id || widget->kind == HERO_UI_WINDOW || widget->hwnd == NULL) {
            continue;
        }
        (void)SendMessageW(widget->hwnd, WM_SETFONT, (WPARAM)window->font, TRUE);
        if (widget->part != NULL) {
            (void)SendMessageW(widget->part, WM_SETFONT, (WPARAM)window->font, TRUE);
        }
        if (widget->kind == HERO_UI_GROUP && widget->inside != NULL) {
            (void)SendMessageW(widget->inside, WM_SETFONT, (WPARAM)window->font, TRUE);
        }
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            (void)SendMessageW(widget->radios[i], WM_SETFONT, (WPARAM)window->font, TRUE);
        }
    }
}

/* Window `hwnd` moved to a monitor of scale `dpi`: its font, its size and
 * its controls' places are made again at it, the content keeping its points. */
static void hero_ui_win_rescaled(HWND hwnd, UINT dpi, const RECT *suggested) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    HFONT before;

    if (window == NULL || window->kind != HERO_UI_WINDOW || dpi == 0) {
        return;
    }
    window->dpi = dpi;
    before = window->font;
    window->font = hero_ui_win_font(dpi);
    hero_ui_win_give_font(window);
    if (before != NULL) {
        (void)DeleteObject(before);
    }
    hero_ui_mute(1);
    if (suggested != NULL) {
        (void)SetWindowPos(hwnd, NULL, suggested->left, suggested->top, suggested->right - suggested->left, suggested->bottom - suggested->top, SWP_NOZORDER | SWP_NOACTIVATE);
    }
    hero_ui_mute(-1);
    hero_ui_win_size_content(window);
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        if (widget != NULL && widget->window == window->id && widget->kind != HERO_UI_WINDOW && (widget->hwnd != NULL || widget->kind == HERO_UI_WEBVIEW) && widget->placed) {
            hero_ui_win_put(widget);
        }
    }
}

/* Window `hwnd`'s content took a new size: told where the person gave it,
 * kept where the program set it. */
static void hero_ui_win_sized(HWND hwnd) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    RECT client;

    if (window == NULL || window->kind != HERO_UI_WINDOW || !GetClientRect(hwnd, &client)) {
        return;
    }
    if (client.right == hero_ui_win_px(window->content_width, window->dpi) && client.bottom == hero_ui_win_px(window->content_height, window->dpi)) {
        return;
    }
    if (hero_ui_is_muted()) {
        return;
    }
    window->content_width = hero_ui_win_pt(client.right, window->dpi);
    window->content_height = hero_ui_win_pt(client.bottom, window->dpi);
    hero_ui_tell(HERO_UI_EVENT_RESIZE, window->id, window->content_width, window->content_height);
}

/* The least frame the person may give window `hwnd`. */
static bool hero_ui_win_least(HWND hwnd, MINMAXINFO *limits) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    RECT frame;

    if (window == NULL || window->kind != HERO_UI_WINDOW || limits == NULL) {
        return false;
    }
    frame.left = 0;
    frame.top = 0;
    frame.right = hero_ui_win_px(window->least_width, window->dpi);
    frame.bottom = hero_ui_win_px(window->least_height, window->dpi);
    hero_ui_win_frame_of(window, &frame);
    if (frame.right - frame.left > limits->ptMinTrackSize.x) {
        limits->ptMinTrackSize.x = frame.right - frame.left;
    }
    if (frame.bottom - frame.top > limits->ptMinTrackSize.y) {
        limits->ptMinTrackSize.y = frame.bottom - frame.top;
    }
    return true;
}

/* -- Menus and shortcuts ---------------------------------------------------- */

/* Whether widget `widget` and every container it stands in is shown and
 * enabled as the program set them. */
static bool hero_ui_win_reachable(const hero_ui_native *widget) {
    for (const hero_ui_native *at = widget; at != NULL && at->kind != HERO_UI_WINDOW; at = hero_ui_find(at->parent)) {
        if (!at->visible || !at->enabled) {
            return false;
        }
    }
    return true;
}

/* Whether menu item `item`, every menu it stands in and its window let the
 * person choose it: a context menu's item while the widget it belongs to is
 * shown and enabled. */
static bool hero_ui_win_usable(const hero_ui_native *item) {
    if (item->kind != HERO_UI_MENU_ITEM || !item->enabled || !item->visible) {
        return false;
    }
    for (const hero_ui_native *at = hero_ui_find(item->parent); at != NULL; at = hero_ui_find(at->parent)) {
        if (at->kind == HERO_UI_MENU) {
            if (!at->enabled || !at->visible) {
                return false;
            }
            continue;
        }
        if (at->kind == HERO_UI_CONTEXT_MENU) {
            const hero_ui_native *target = hero_ui_find(at->parent);

            return target != NULL && hero_ui_win_reachable(target);
        }
        return at->kind == HERO_UI_WINDOW;
    }
    return false;
}

/* The menu item of command number `command`, or NULL. */
static hero_ui_native *hero_ui_win_item_of(UINT command) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *item = hero_ui_at(at);

        if (item != NULL && (item->kind == HERO_UI_MENU_ITEM || item->kind == HERO_UI_MENU_SEPARATOR) && item->command == command) {
            return item;
        }
    }
    return NULL;
}

/* A command number no menu item holds, or 0 where all are taken. */
static WORD hero_ui_win_new_command(void) {
    UINT span = HERO_UI_WIN_LAST_COMMAND - HERO_UI_WIN_FIRST_COMMAND + 1;

    for (UINT tried = 0; tried < span; tried += 1) {
        UINT command = hero_ui_win_next_command;

        if (command < HERO_UI_WIN_FIRST_COMMAND || command > HERO_UI_WIN_LAST_COMMAND) {
            command = HERO_UI_WIN_FIRST_COMMAND;
        }
        hero_ui_win_next_command = command + 1;
        if (hero_ui_win_item_of(command) == NULL) {
            return (WORD)command;
        }
    }
    return 0;
}

/* The name Windows gives virtual key `key` on this keyboard, into `name`
 * of `room` units; `extended` for the keys of the arrows' block. */
static int hero_ui_win_key_name(UINT key, bool extended, wchar_t *name, int room) {
    UINT scan = MapVirtualKeyW(key, MAPVK_VK_TO_VSC);
    LONG word = (LONG)((scan & 0xFFu) << 16) | (extended ? (LONG)1 << 24 : 0);

    return scan == 0 ? 0 : GetKeyNameTextW(word, name, room);
}

/* Appends `text` to `into` of `room` units at `*used`, as much as fits with
 * the zero after it. */
static void hero_ui_win_append(wchar_t *into, size_t room, size_t *used, const wchar_t *text) {
    for (size_t i = 0; text[i] != 0 && *used + 1 < room; i += 1) {
        into[*used] = text[i];
        *used += 1;
    }
    into[*used] = 0;
}

/* `label` as a menu shows it, `&` doubled and a tab a space, then a tab and
 * `keys` where they are not empty; in a block of `hero_layer_alloc`, or
 * NULL. */
static wchar_t *hero_ui_win_menu_text(const wchar_t *label, const wchar_t *keys) {
    size_t length = label != NULL ? wcslen(label) : 0;
    wchar_t *text = hero_layer_alloc((length * 2 + wcslen(keys) + 2) * sizeof(wchar_t));
    size_t at = 0;

    if (text == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < length; i += 1) {
        if (label[i] == L'&') {
            text[at++] = L'&';
        }
        text[at++] = label[i] == L'\t' ? L' ' : label[i];
    }
    if (keys[0] != 0) {
        text[at++] = L'\t';
        for (size_t i = 0; keys[i] != 0; i += 1) {
            text[at++] = keys[i];
        }
    }
    text[at] = 0;
    return text;
}

/* Item `item`'s text as its menu shows it: its label, then its shortcut's
 * keys by the names this keyboard gives them, `Ctrl+Shift+O`. */
static wchar_t *hero_ui_win_item_text(const hero_ui_native *item) {
    static const struct {
        BYTE held;
        UINT key;
        const wchar_t *otherwise;
    } held_names[] = {{FCONTROL, VK_CONTROL, L"Ctrl"}, {FSHIFT, VK_SHIFT, L"Shift"}, {FALT, VK_MENU, L"Alt"}};
    wchar_t keys[160];
    size_t used = 0;

    keys[0] = 0;
    if (item->key != 0) {
        wchar_t name[48];

        for (size_t i = 0; i < sizeof held_names / sizeof held_names[0]; i += 1) {
            if ((item->held & held_names[i].held) == 0) {
                continue;
            }
            hero_ui_win_append(keys, 160, &used, hero_ui_win_key_name(held_names[i].key, false, name, 48) > 0 ? name : held_names[i].otherwise);
            hero_ui_win_append(keys, 160, &used, L"+");
        }
        hero_ui_win_append(keys, 160, &used, hero_ui_win_key_name(item->key, item->key_extended, name, 48) > 0 ? name : L"?");
    }
    return hero_ui_win_menu_text(item->label, keys);
}

/* Fills menu `menu`'s pop-up again, a menu's or a context menu's, with its
 * items, separators and menus shown, in their order: a check item with its
 * mark, a radio item's drawn as a dot. `RemoveMenu` takes an inner menu out
 * without destroying it, so each inner menu stays its own until its widget
 * is destroyed. */
static int64_t hero_ui_win_fill_menu(hero_ui_native *menu) {
    hero_ui_native *window = hero_ui_win_window(menu->window);
    int64_t answer = HERO_UI_OK;

    while (GetMenuItemCount(menu->menu) > 0) {
        if (!RemoveMenu(menu->menu, 0, MF_BYPOSITION)) {
            break;
        }
    }
    for (int64_t i = 0; i < menu->order.count; i += 1) {
        hero_ui_native *item = hero_ui_find(menu->order.at[i]);
        MENUITEMINFOW info;
        wchar_t *text = NULL;
        int at = GetMenuItemCount(menu->menu);

        if (item == NULL || !item->visible) {
            continue;
        }
        memset(&info, 0, sizeof info);
        info.cbSize = (UINT)sizeof info;
        if (item->kind == HERO_UI_MENU_SEPARATOR) {
            info.fMask = MIIM_FTYPE | MIIM_ID;
            info.fType = MFT_SEPARATOR;
            info.wID = item->command;
        } else {
            text = item->kind == HERO_UI_MENU ? hero_ui_win_menu_text(item->label, L"") : hero_ui_win_item_text(item);
            if (text == NULL) {
                answer = HERO_UI_LIMIT;
                continue;
            }
            info.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_STATE | (item->kind == HERO_UI_MENU ? MIIM_SUBMENU : MIIM_ID);
            info.fType = MFT_STRING | (item->check == HERO_UI_CHECK_RADIO ? MFT_RADIOCHECK : 0);
            info.fState = (item->enabled ? MFS_ENABLED : MFS_GRAYED) | (item->check != HERO_UI_CHECK_NONE && item->checked ? MFS_CHECKED : MFS_UNCHECKED);
            info.dwTypeData = text;
            info.wID = item->command;
            info.hSubMenu = item->kind == HERO_UI_MENU ? item->menu : NULL;
        }
        if (!InsertMenuItemW(menu->menu, (UINT)(at < 0 ? 0 : at), TRUE, &info) && answer == HERO_UI_OK) {
            answer = hero_ui_win_failed("adding an item to a menu", GetLastError());
        }
        hero_layer_free(text);
    }
    if (window != NULL) {
        window->accelerators_stale = true;
        if (GetMenu(window->hwnd) != NULL) {
            (void)DrawMenuBar(window->hwnd);
        }
    }
    return answer;
}

/* Fills again what menu, item or separator `widget` stands in: a window's
 * menu bar, a menu or a context menu. */
static int64_t hero_ui_win_fill_bar(hero_ui_native *window);
static int64_t hero_ui_win_refill_owner(const hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);

    if (owner == NULL) {
        return HERO_UI_OK;
    }
    return owner->kind == HERO_UI_WINDOW ? hero_ui_win_fill_bar(owner) : hero_ui_win_fill_menu(owner);
}

/* Fills window `window`'s menu bar again with its menus shown, in their
 * order; the bar is taken off the window while it holds none, and the
 * content keeps its size. */
static int64_t hero_ui_win_fill_bar(hero_ui_native *window) {
    int64_t answer = HERO_UI_OK;
    int shown = 0;
    bool had = GetMenu(window->hwnd) != NULL;

    if (window->menu == NULL) {
        window->menu = CreateMenu();
        if (window->menu == NULL) {
            return hero_ui_win_failed("making a menu bar", GetLastError());
        }
    }
    while (GetMenuItemCount(window->menu) > 0) {
        if (!RemoveMenu(window->menu, 0, MF_BYPOSITION)) {
            break;
        }
    }
    for (int64_t i = 0; i < window->order.count; i += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[i]);
        wchar_t *text;

        if (menu == NULL || !menu->visible) {
            continue;
        }
        text = hero_ui_win_menu_text(menu->label, L"");
        if (text == NULL) {
            answer = HERO_UI_LIMIT;
            continue;
        }
        if (!AppendMenuW(window->menu, MF_POPUP | (menu->enabled ? MF_ENABLED : MF_GRAYED), (UINT_PTR)menu->menu, text)) {
            if (answer == HERO_UI_OK) {
                answer = hero_ui_win_failed("adding a menu to a menu bar", GetLastError());
            }
        } else {
            shown += 1;
        }
        hero_layer_free(text);
    }
    if ((shown > 0) != had) {
        /* Taking the bar on or off sends the window a `WM_SIZE` of the
         * content the bar's height shorter or taller: the program's own
         * change, muted, and the content given its size again (measured on
         * Windows Server 2025, 2026-11-22: unmuted, a content set to 420 by
         * 300 was told resized to 420 by 280 as a menu was made). */
        hero_ui_mute(1);
        (void)SetMenu(window->hwnd, shown > 0 ? window->menu : NULL);
        hero_ui_mute(-1);
        hero_ui_win_size_content(window);
    } else if (had) {
        (void)DrawMenuBar(window->hwnd);
    }
    window->accelerators_stale = true;
    return answer;
}

/* Counts, where `table` is NULL, or writes into `table` from `*count`, the
 * shortcuts of menu `menu`'s items shown and of the menus inside it. */
static void hero_ui_win_gather_keys(const hero_ui_native *menu, ACCEL *table, int64_t *count) {
    for (int64_t i = 0; i < menu->order.count; i += 1) {
        const hero_ui_native *item = hero_ui_find(menu->order.at[i]);

        if (item == NULL || !item->visible) {
            continue;
        }
        if (item->kind == HERO_UI_MENU) {
            hero_ui_win_gather_keys(item, table, count);
            continue;
        }
        if (item->kind != HERO_UI_MENU_ITEM || item->key == 0) {
            continue;
        }
        if (table != NULL) {
            table[*count].fVirt = (BYTE)(FVIRTKEY | item->held);
            table[*count].key = item->key;
            table[*count].cmd = item->command;
        }
        *count += 1;
    }
}

/* Makes window `window`'s accelerator table again from the shortcuts of the
 * items shown of its menu bar's menus and the menus inside them; a context
 * menu's shortcuts are shown and take no key. */
static void hero_ui_win_make_accelerators(hero_ui_native *window) {
    ACCEL *table;
    int64_t most = 0;
    int64_t count = 0;

    if (window->accelerators != NULL) {
        (void)DestroyAcceleratorTable(window->accelerators);
        window->accelerators = NULL;
    }
    window->accelerators_stale = false;
    for (int64_t m = 0; m < window->order.count; m += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[m]);

        if (menu != NULL && menu->visible) {
            hero_ui_win_gather_keys(menu, NULL, &most);
        }
    }
    if (most == 0 || most > INT32_MAX) {
        return;
    }
    table = hero_layer_alloc((size_t)most * sizeof *table);
    if (table == NULL) {
        /* No memory for the table: the shortcuts stay unmade and are asked
         * for again at the next message. */
        window->accelerators_stale = true;
        return;
    }
    for (int64_t m = 0; m < window->order.count; m += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[m]);

        if (menu != NULL && menu->visible) {
            hero_ui_win_gather_keys(menu, table, &count);
        }
    }
    if (count > 0) {
        window->accelerators = CreateAcceleratorTableW(table, (int)count);
    }
    hero_layer_free(table);
}

/* -- What the person does ---------------------------------------------------- */

/* The position of `value` on slider `slider`'s trackbar: the value itself
 * where its range fits the trackbar's 32 bits, else its share of
 * `HERO_UI_WIN_STEPS`. */
static int hero_ui_win_position(const hero_ui_native *slider, int64_t value) {
    if (slider->least >= INT32_MIN && slider->most <= INT32_MAX) {
        return (int)hero_ui_between(value, slider->least, slider->most);
    }
    return (int)(hero_ui_fraction(hero_ui_between(value, slider->least, slider->most), slider->least, slider->most) * HERO_UI_WIN_STEPS + 0.5);
}

/* The value standing at trackbar position `position`. */
static int64_t hero_ui_win_value_at(const hero_ui_native *slider, int position) {
    if (slider->least >= INT32_MIN && slider->most <= INT32_MAX) {
        return hero_ui_between(position, slider->least, slider->most);
    }
    return hero_ui_at_fraction((double)position / HERO_UI_WIN_STEPS, slider->least, slider->most);
}

/* Gives a trackbar or a progress bar the range and the value `widget` keeps. */
static void hero_ui_win_show_range(hero_ui_native *widget) {
    bool direct = widget->least >= INT32_MIN && widget->most <= INT32_MAX;
    int low = direct ? (int)widget->least : 0;
    int high = direct ? (int)widget->most : HERO_UI_WIN_STEPS;
    int position = widget->kind == HERO_UI_SLIDER ? hero_ui_win_position(widget, widget->value) : direct ? (int)hero_ui_between(widget->value, widget->least, widget->most) : (int)(hero_ui_fraction(widget->value, widget->least, widget->most) * HERO_UI_WIN_STEPS + 0.5);

    if (widget->kind == HERO_UI_SLIDER) {
        (void)SendMessageW(widget->hwnd, TBM_SETRANGEMIN, FALSE, (LPARAM)low);
        (void)SendMessageW(widget->hwnd, TBM_SETRANGEMAX, FALSE, (LPARAM)high);
        (void)SendMessageW(widget->hwnd, TBM_SETPOS, TRUE, (LPARAM)position);
    } else {
        (void)SendMessageW(widget->hwnd, PBM_SETRANGE32, (WPARAM)low, (LPARAM)high);
        (void)SendMessageW(widget->hwnd, PBM_SETPOS, (WPARAM)position, 0);
    }
}

/* Checks the `index`th item of radio group `group` and no other; -1 none. */
static void hero_ui_win_check_radio(hero_ui_native *group, int64_t index) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        (void)SendMessageW(group->radios[i], BM_SETCHECK, i == index ? BST_CHECKED : BST_UNCHECKED, 0);
    }
}

/* The item radio group `group` has checked, -1 for none. */
static int64_t hero_ui_win_checked_radio(const hero_ui_native *group) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        if (SendMessageW(group->radios[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
            return i;
        }
    }
    return -1;
}

/* Whether widget `id` is a radio item. */
static bool hero_ui_win_is_radio(int64_t id) {
    const hero_ui_native *item = hero_ui_find(id);

    return item != NULL && item->kind == HERO_UI_MENU_ITEM && item->check == HERO_UI_CHECK_RADIO;
}

/* Checks radio item `item` and unchecks the other radio items of its group:
 * those of its menu no other item or separator stands between, in the
 * order the program made them. */
static void hero_ui_win_check_in_group(hero_ui_native *item) {
    hero_ui_native *menu = hero_ui_find(item->parent);
    int64_t at = -1;
    int64_t first;
    int64_t last;

    item->checked = true;
    if (menu == NULL) {
        return;
    }
    for (int64_t i = 0; i < menu->order.count && at < 0; i += 1) {
        at = menu->order.at[i] == item->id ? i : -1;
    }
    if (at < 0) {
        return;
    }
    first = at;
    last = at;
    while (first > 0 && hero_ui_win_is_radio(menu->order.at[first - 1])) {
        first -= 1;
    }
    while (last + 1 < menu->order.count && hero_ui_win_is_radio(menu->order.at[last + 1])) {
        last += 1;
    }
    for (int64_t i = first; i <= last; i += 1) {
        hero_ui_native *other = hero_ui_find(menu->order.at[i]);

        if (other != NULL && other != item) {
            other->checked = false;
        }
    }
}

/* Above 0 while an undo or a redo changes a text control's text, so the
 * change is no new edit to keep. */
static _Thread_local int64_t hero_ui_win_replaying;
static void hero_ui_win_edit_changed(hero_ui_native *widget, bool person);
static void hero_ui_win_edit_reset(hero_ui_native *widget);
static void hero_ui_win_edit_end(hero_ui_native *widget);
static bool hero_ui_win_tray_show(hero_ui_native *tray, DWORD how);
static void hero_ui_win_tip_end(hero_ui_native *widget);
static bool hero_ui_win_tip_apply(hero_ui_native *widget);
static void hero_ui_win_handed_take(void);
static void hero_ui_win_spin_commit(hero_ui_native *spin);
static void hero_ui_win_spin_show(const hero_ui_native *spin);
static LRESULT hero_ui_win_spin_stepped(hero_ui_native *spin, const NMUPDOWN *step);
static void hero_ui_win_combo_complete(hero_ui_native *combo);
static void hero_ui_win_combo_chosen(hero_ui_native *combo);
static int64_t hero_ui_win_combo_match(const hero_ui_native *combo);
static void hero_ui_win_lay_group(hero_ui_native *group);
static void hero_ui_win_lay_bar(hero_ui_native *bar);
static int64_t hero_ui_win_bar_height(const hero_ui_native *bar);
static bool hero_ui_win_bar_holds(const hero_ui_native *bar, int64_t kind);
static void hero_ui_win_stamp_setup(hero_ui_native *picker);
static void hero_ui_win_stamp_show(const hero_ui_native *picker);
static void hero_ui_win_stamp_changed(hero_ui_native *picker);
static bool hero_ui_win_stamp_kind(int64_t kind);
static int64_t hero_ui_win_stamp_set(hero_ui_native *picker, int64_t value, bool person);
static int64_t hero_ui_win_stamp_range(hero_ui_native *picker, int64_t least, int64_t most);
static int64_t hero_ui_win_well_set(hero_ui_native *well, int64_t value, bool person);
static void hero_ui_win_well_choose(hero_ui_native *well);
static bool hero_ui_win_swatch_draw(const DRAWITEMSTRUCT *item);
static bool hero_ui_win_font_start(hero_ui_native *button);
static int64_t hero_ui_win_font_apply(hero_ui_native *button, const wchar_t *text, bool store, bool *changed);
static void hero_ui_win_font_choose(hero_ui_native *button);

/* A control's notification `code` to its parent, or a menu's or a shortcut's
 * command where `control` is NULL. */
static void hero_ui_win_command(UINT code, UINT command, HWND control) {
    hero_ui_native *widget;

    if (control == NULL) {
        hero_ui_native *item = code <= 1 ? hero_ui_win_item_of(command) : NULL;

        if (item != NULL && hero_ui_win_usable(item)) {
            /* A check item turned over, a radio item checked alone in its
             * group, before its event, which carries the state after. */
            if (item->check == HERO_UI_CHECK_BOX) {
                item->checked = !item->checked;
                (void)hero_ui_win_refill_owner(item);
            } else if (item->check == HERO_UI_CHECK_RADIO) {
                hero_ui_win_check_in_group(item);
                (void)hero_ui_win_refill_owner(item);
            }
            hero_ui_tell(HERO_UI_EVENT_MENU, item->id, item->check != HERO_UI_CHECK_NONE && item->checked ? 1 : 0, 0);
        }
        return;
    }
    widget = hero_ui_win_of(control);
    if (widget == NULL) {
        return;
    }
    switch (widget->kind) {
    case HERO_UI_BUTTON:
        if (code == BN_CLICKED) {
            hero_ui_tell(HERO_UI_EVENT_CLICK, widget->id, 0, 0);
        }
        break;
    case HERO_UI_CHECKBOX:
        if (code == BN_CLICKED) {
            hero_ui_tell(HERO_UI_EVENT_TOGGLE, widget->id, SendMessageW(control, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0, 0);
        }
        break;
    case HERO_UI_RADIO_GROUP:
        if (code == BN_CLICKED) {
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                if (widget->radios[i] == control) {
                    hero_ui_win_check_radio(widget, i);
                    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, i, 0);
                }
            }
        }
        break;
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
        if (code == EN_CHANGE) {
            if (hero_ui_win_replaying == 0 && hero_ui_win_injecting == 0 && !hero_ui_is_muted()) {
                hero_ui_win_edit_changed(widget, true);
            }
            hero_ui_tell(HERO_UI_EVENT_TEXT, widget->id, 0, 0);
        }
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        if (code == LBN_SELCHANGE) {
            hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, 0, 0);
        } else if (code == LBN_DBLCLK) {
            /* The row double-clicked, which holds the list's caret; its
             * first click's choice came before. */
            LRESULT row = SendMessageW(control, LB_GETCARETINDEX, 0, 0);

            if (row >= 0 && row < widget->items) {
                hero_ui_tell(HERO_UI_EVENT_ACTIVATE, widget->id, (int64_t)row, 0);
            }
        }
        break;
    case HERO_UI_COMBO:
        if (code == CBN_SELCHANGE) {
            LRESULT chosen = SendMessageW(control, CB_GETCURSEL, 0, 0);

            hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, chosen == CB_ERR ? -1 : (int64_t)chosen, 0);
        }
        break;
    case HERO_UI_EDITABLE_COMBO:
        if (code == CBN_EDITUPDATE) {
            hero_ui_win_combo_complete(widget);
        } else if (code == CBN_EDITCHANGE) {
            hero_ui_tell(HERO_UI_EVENT_TEXT, widget->id, 0, 0);
        } else if (code == CBN_SELCHANGE) {
            hero_ui_win_combo_chosen(widget);
        }
        break;
    case HERO_UI_SPIN:
        if (code == EN_KILLFOCUS) {
            hero_ui_win_spin_commit(widget);
        }
        break;
    case HERO_UI_COLOR_WELL:
        if (code == BN_CLICKED) {
            hero_ui_win_well_choose(widget);
        }
        break;
    case HERO_UI_FONT_BUTTON:
        if (code == BN_CLICKED) {
            hero_ui_win_font_choose(widget);
        }
        break;
    default:
        break;
    }
}

/* Trackbar `control` moved. */
static void hero_ui_win_slid(HWND control) {
    hero_ui_native *slider = hero_ui_win_of(control);
    int64_t value;

    if (slider == NULL || slider->kind != HERO_UI_SLIDER) {
        return;
    }
    value = hero_ui_win_value_at(slider, (int)SendMessageW(control, TBM_GETPOS, 0, 0));
    if (value != slider->value || hero_ui_win_injecting > 0) {
        slider->value = value;
        hero_ui_tell(HERO_UI_EVENT_VALUE, slider->id, value, 0);
    }
}

/* Paints picture `hwnd`: the window's colour, and its pixels in the middle,
 * each pixel a point. */
static void hero_ui_win_paint(HWND hwnd) {
    PAINTSTRUCT paint;
    HDC dc = BeginPaint(hwnd, &paint);
    hero_ui_native *picture = hero_ui_win_of(hwnd);
    hero_ui_native *window = picture != NULL ? hero_ui_win_window(picture->window) : NULL;
    RECT area;

    if (dc == NULL) {
        return;
    }
    if (GetClientRect(hwnd, &area)) {
        (void)FillRect(dc, &area, GetSysColorBrush(COLOR_3DFACE));
    }
    if (picture != NULL && picture->bitmap != NULL && window != NULL) {
        int width = hero_ui_win_px(picture->bitmap_width, window->dpi);
        int height = hero_ui_win_px(picture->bitmap_height, window->dpi);
        HDC memory = CreateCompatibleDC(dc);

        if (memory != NULL) {
            HGDIOBJ before = SelectObject(memory, picture->bitmap);
            BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

            (void)AlphaBlend(dc, (area.right - width) / 2, (area.bottom - height) / 2, width, height, memory, 0, 0, (int)picture->bitmap_width, (int)picture->bitmap_height, blend);
            (void)SelectObject(memory, before);
            (void)DeleteDC(memory);
        }
    }
    (void)EndPaint(hwnd, &paint);
}

/* The system's quit (`WM_QUIT`): every window is asked to close, in the
 * order made, as AppKit's Quit asks. */
static void hero_ui_win_quit(void) {
    int64_t last = 0;

    for (;;) {
        int64_t next = 0;

        for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
            hero_ui_native *window = hero_ui_at(at);

            if (window != NULL && window->kind == HERO_UI_WINDOW && window->id > last && (next == 0 || window->id < next)) {
                next = window->id;
            }
        }
        if (next == 0) {
            return;
        }
        hero_ui_tell(HERO_UI_EVENT_CLOSE, next, 0, 0);
        last = next;
    }
}

/* Where on window `window`, which has no frame, the point `at` of the
 * screen lands: on an edge or a corner, within the sizing border Windows
 * gives a frame at its scale, where it may be resized and is neither
 * maximized nor in full screen, so the person resizes it there; elsewhere
 * on its background, its caption, so the person moves it. */
static LRESULT hero_ui_win_edge(const hero_ui_native *window, LPARAM at) {
    POINT point = {(short)LOWORD(at), (short)HIWORD(at)};
    RECT area;
    int border;
    bool left;
    bool right;
    bool top;
    bool bottom;

    if (!window->resizable || window->full_screen || IsZoomed(window->hwnd) || !GetWindowRect(window->hwnd, &area)) {
        return HTCAPTION;
    }
    border = hero_ui_win_metric(SM_CXSIZEFRAME, window->dpi) + hero_ui_win_metric(SM_CXPADDEDBORDER, window->dpi);
    left = point.x < area.left + border;
    right = point.x >= area.right - border;
    top = point.y < area.top + border;
    bottom = point.y >= area.bottom - border;
    if (top) {
        return left ? HTTOPLEFT : right ? HTTOPRIGHT : HTTOP;
    }
    if (bottom) {
        return left ? HTBOTTOMLEFT : right ? HTBOTTOMRIGHT : HTBOTTOM;
    }
    return left ? HTLEFT : right ? HTRIGHT : HTCAPTION;
}

#define HERO_UI_WIN_TRAY_MESSAGE (WM_APP + 0x49)

/* What the parts below the procedure handle, each defined in its part. */
static void hero_ui_win_moved(HWND hwnd, bool moved);
static void hero_ui_win_state_changed(HWND hwnd);
static bool hero_ui_win_window_activated(HWND hwnd, WPARAM state);
static void hero_ui_win_dialog_key(HWND hwnd, UINT which);
static bool hero_ui_win_show_context(HWND target, LPARAM at, bool inherit);
static bool hero_ui_win_scroll_by(HWND hwnd, int request, int lines);
static bool hero_ui_win_divider_mouse(HWND hwnd, UINT message, LPARAM lparam);
static void hero_ui_win_page_chosen(hero_ui_native *tabs);
static LRESULT hero_ui_win_table_notified(hero_ui_native *table, NMHDR *head);
static LRESULT hero_ui_win_tree_notified(hero_ui_native *tree, NMHDR *head);
static void hero_ui_win_settle_rows(hero_ui_native *table);
static void hero_ui_win_lay_box(hero_ui_native *widget);
static hero_ui_native *hero_ui_win_widget_at(HWND hwnd);
static void hero_ui_win_setting_changed(LPARAM name);
static void hero_ui_win_tray_told(HWND hwnd, WPARAM at, LPARAM what);
static void hero_ui_win_trays_again(void);
static void hero_ui_win_dialogs_reap(void);
static LRESULT hero_ui_win_end_asked(HWND hwnd, LPARAM why);
static void hero_ui_win_quits_answered(void);
static void hero_ui_win_files_dropped(HWND hwnd, HDROP drop);

/* A notification from a control to its parent (`WM_NOTIFY`): the person's
 * tab, a table's or a tree's. */
static LRESULT hero_ui_win_notified(NMHDR *head) {
    hero_ui_native *widget = head != NULL ? hero_ui_win_of(head->hwndFrom) : NULL;

    if (widget == NULL) {
        return 0;
    }
    switch (widget->kind) {
    case HERO_UI_TABS:
        if (head->code == TCN_SELCHANGE) {
            hero_ui_win_page_chosen(widget);
        }
        return 0;
    case HERO_UI_TABLE:
        return hero_ui_win_table_notified(widget, head);
    case HERO_UI_TREE:
        return hero_ui_win_tree_notified(widget, head);
    case HERO_UI_SPIN:
        return head->code == UDN_DELTAPOS ? hero_ui_win_spin_stepped(widget, (const NMUPDOWN *)head) : 0;
    case HERO_UI_DATE_PICKER:
    case HERO_UI_TIME_PICKER:
    case HERO_UI_DATE_TIME_PICKER:
        if (head->code == DTN_DATETIMECHANGE) {
            hero_ui_win_stamp_changed(widget);
        }
        return 0;
    case HERO_UI_CALENDAR:
        /* Every change of the day shown chosen is read back and told where
         * it changed the value: a click's `MCN_SELECT` and an arrow key's
         * `MCN_SELCHANGE` alike, so a pick by the keyboard is told whether or
         * not Windows sends the first for it; a notice that leaves the day
         * as it was (the program's own `MCM_SETCURSEL`, muted, a month turned
         * with the day kept) tells nothing. */
        if (head->code == MCN_SELECT || head->code == MCN_SELCHANGE) {
            hero_ui_win_stamp_changed(widget);
        }
        return 0;
    default:
        return 0;
    }
}

/* -- A drawing area's window ------------------------------------------------- */

static void hero_ui_win_give_keys(hero_ui_native *window, HWND control);
static hero_ui_native *hero_ui_win_key_button(int64_t id);
static void hero_ui_win_canvas_role(const hero_ui_native *area);

/* The scale of drawing area `area`'s window, as the window keeps it. */
static UINT hero_ui_win_canvas_dpi(const hero_ui_native *area) {
    const hero_ui_native *window = hero_ui_win_window(area->window);

    return window != NULL && window->dpi != 0 ? window->dpi : 96;
}

/* `WM_GETDLGCODE` of drawing area `area`: every key, Tab and the arrows
 * among them, as the macOS arm's area takes every key it is given; Return
 * and Escape alone go on to the dialog manager where the window has a button
 * they press, as AppKit's key equivalents press it before the area hears
 * them. */
static LRESULT hero_ui_win_canvas_keys(const hero_ui_native *area, const MSG *message) {
    const hero_ui_native *window = hero_ui_win_window(area->window);

    if (message != NULL && window != NULL && message->message == WM_KEYDOWN && ((message->wParam == VK_RETURN && hero_ui_win_key_button(window->enter_button) != NULL) || (message->wParam == VK_ESCAPE && hero_ui_win_key_button(window->escape_button) != NULL))) {
        return DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTCHARS;
    }
    return DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTCHARS;
}

static LRESULT CALLBACK hero_ui_win_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    hero_ui_native *widget;
    LRESULT canvas_answer;

    widget = hero_ui_win_of(hwnd);
    if (widget != NULL && widget->kind == HERO_UI_CANVAS && widget->canvas != NULL && widget->hwnd == hwnd) {
        if (message == WM_GETDLGCODE) {
            return hero_ui_win_canvas_keys(widget, (const MSG *)lparam);
        }
        if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_XBUTTONDOWN) {
            /* A press in the area gives it the keys, as a person's click in
             * a field does. */
            hero_ui_native *window = hero_ui_win_window(widget->window);

            if (window != NULL) {
                hero_ui_win_give_keys(window, hwnd);
            }
        }
        if (widget->parts != NULL && hero_ui_parts_win32_message(widget->parts, message, wparam, lparam, &canvas_answer)) {
            return canvas_answer;
        }
        if (hero_ui_canvas_win32_message(widget->canvas, widget->id, hwnd, hero_ui_win_canvas_dpi(widget), message, wparam, lparam, &canvas_answer)) {
            return canvas_answer;
        }
    }
    if (message == hero_ui_win_taskbar_created && message != 0) {
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_TRAY) {
            hero_ui_win_trays_again();
        }
    }
    switch (message) {
    case HERO_UI_WIN_TRAY_MESSAGE:
        hero_ui_win_tray_told(hwnd, wparam, lparam);
        return 0;
    case WM_COMMAND:
        /* Return or Escape the dialog manager gives the window, from no
         * control of this arm's (RETURN AND ESCAPE). */
        if (lparam == 0 && HIWORD(wparam) == 0 && (LOWORD(wparam) == IDOK || LOWORD(wparam) == IDCANCEL)) {
            hero_ui_win_dialog_key(hwnd, LOWORD(wparam));
            return 0;
        }
        hero_ui_win_command(HIWORD(wparam), LOWORD(wparam), (HWND)lparam);
        return 0;
    case WM_NOTIFY:
        return hero_ui_win_notified((NMHDR *)lparam);
    case WM_DRAWITEM:
        if (hero_ui_win_swatch_draw((const DRAWITEMSTRUCT *)lparam)) {
            return TRUE;
        }
        break;
    case WM_HSCROLL:
        if (lparam != 0) {
            hero_ui_win_slid((HWND)lparam);
        }
        return 0;
    case WM_VSCROLL:
        if (lparam == 0 && hero_ui_win_scroll_by(hwnd, LOWORD(wparam), 0)) {
            return 0;
        }
        break;
    case WM_MOUSEWHEEL:
        if (hero_ui_win_scroll_by(hwnd, -1, MulDiv((short)HIWORD(wparam), 3, WHEEL_DELTA))) {
            return 0;
        }
        break;
    case WM_SETCURSOR:
        if ((HWND)wparam == hwnd && LOWORD(lparam) == HTCLIENT && hero_ui_win_divider_mouse(hwnd, message, lparam)) {
            return TRUE;
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
    case WM_LBUTTONUP:
    case WM_CAPTURECHANGED:
        if (hero_ui_win_divider_mouse(hwnd, message, lparam)) {
            return 0;
        }
        break;
    case WM_CONTEXTMENU:
        if (hero_ui_win_show_context((HWND)wparam, lparam, true)) {
            return 0;
        }
        break;
    case WM_QUERYENDSESSION:
        return hero_ui_win_end_asked(hwnd, lparam);
    case WM_ENDSESSION:
        if (wparam == FALSE) {
            hero_ui_win_quits_answered();
        }
        return 0;
    case WM_DROPFILES:
        hero_ui_win_files_dropped(hwnd, (HDROP)wparam);
        return 0;
    case HERO_UI_WIN_DIALOG_DONE:
        hero_ui_win_dialogs_reap();
        return 0;
    case HERO_UI_WIN_HANDED:
        hero_ui_win_handed_take();
        return 0;
    case HERO_UI_WIN_SETTLE:
        widget = hero_ui_find((int64_t)lparam);
        if (widget != NULL && widget->kind == HERO_UI_TABLE) {
            hero_ui_win_settle_rows(widget);
        }
        return 0;
    case WM_CLOSE:
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_WINDOW) {
            hero_ui_tell(HERO_UI_EVENT_CLOSE, widget->id, 0, 0);
            return 0;
        }
        break;
    case WM_ACTIVATE:
        if (hero_ui_win_window_activated(hwnd, wparam)) {
            return 0;
        }
        break;
    case WM_MOVE:
        hero_ui_win_moved(hwnd, true);
        break;
    case WM_DISPLAYCHANGE:
        hero_ui_win_moved(hwnd, false);
        break;
    case WM_SETTINGCHANGE:
        hero_ui_win_setting_changed(lparam);
        break;
    case WM_SIZE:
        if (wparam != SIZE_MINIMIZED) {
            hero_ui_win_sized(hwnd);
        }
        hero_ui_win_state_changed(hwnd);
        break;
    case WM_GETMINMAXINFO:
        if (hero_ui_win_least(hwnd, (MINMAXINFO *)lparam)) {
            return 0;
        }
        break;
    case HERO_UI_WIN_DPICHANGED:
        hero_ui_win_rescaled(hwnd, HIWORD(wparam), (const RECT *)lparam);
        hero_ui_win_moved(hwnd, false);
        return 0;
    case WM_NCHITTEST:
        /* A window with no frame is moved by a drag on its background, and
         * a box's background stands aside for it. */
        widget = hero_ui_win_widget_at(hwnd);
        if (widget != NULL) {
            hero_ui_native *window = hero_ui_win_window(widget->window);

            if (window != NULL && window->unframed) {
                LRESULT hit = DefWindowProcW(hwnd, message, wparam, lparam);

                if (hit != HTCLIENT || widget->kind == HERO_UI_SPLIT_ROW || widget->kind == HERO_UI_SPLIT_COLUMN || widget->kind == HERO_UI_CANVAS) {
                    return hit;
                }
                return widget->kind == HERO_UI_WINDOW && widget->hwnd == hwnd ? hero_ui_win_edge(window, lparam) : HTTRANSPARENT;
            }
        }
        break;
    case WM_NCLBUTTONDBLCLK:
        /* A double click on a window's background is no maximize. */
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_WINDOW && widget->unframed && wparam == HTCAPTION) {
            return 0;
        }
        break;
    case WM_ERASEBKGND:
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_IMAGE) {
            return 1;
        }
        if (widget != NULL && widget->kind == HERO_UI_WINDOW && widget->backdrop == HERO_UI_BACKDROP_TRANSLUCENT) {
            /* Black is what DWM draws its material through. */
            RECT area;

            if (GetClientRect(hwnd, &area)) {
                (void)FillRect((HDC)wparam, &area, (HBRUSH)GetStockObject(BLACK_BRUSH));
            }
            return 1;
        }
        break;
    case WM_PAINT:
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_IMAGE) {
            hero_ui_win_paint(hwnd);
            return 0;
        }
        break;
    default:
        break;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

/* -- The arm ------------------------------------------------------------------ */

/* Whether the process's window station shows what is drawn and takes a
 * person's input: an interactive one does, and a service's or a remote
 * shell's session (`ssh`) does not. */
int64_t hero_ui_arm_here(void) {
    USEROBJECTFLAGS flags;
    DWORD needed = 0;
    HWINSTA station = GetProcessWindowStation();

    memset(&flags, 0, sizeof flags);
    if (station == NULL || !GetUserObjectInformationW(station, UOI_FLAGS, &flags, (DWORD)sizeof flags, &needed)) {
        return 0;
    }
    return (flags.dwFlags & WSF_VISIBLE) != 0 ? 1 : 0;
}

void hero_ui_arm_absent(char *words, size_t count) {
    (void)hero_ui_copy_text(words, count, "this process's window station shows nothing and takes no input, as a service's or a remote shell's does");
}

/* The function of `user32` named `name` into `slot`, left NULL where this
 * Windows has none; C11 converts no object pointer to a function pointer, so
 * the address is copied by its bytes. */
static void hero_ui_win_find_function(HMODULE user32, const char *name, void *slot, size_t size) {
    FARPROC found = user32 != NULL ? GetProcAddress(user32, name) : NULL;

    if (found != NULL && size == sizeof found) {
        memcpy(slot, &found, sizeof found);
    }
}

/* Registers class `name` of this arm's procedure, where it is not yet. */
static bool hero_ui_win_register(const wchar_t *name, HBRUSH background, HICON icon) {
    WNDCLASSEXW request;
    HINSTANCE module = GetModuleHandleW(NULL);

    memset(&request, 0, sizeof request);
    request.cbSize = (UINT)sizeof request;
    if (GetClassInfoExW(module, name, &request)) {
        return true;
    }
    memset(&request, 0, sizeof request);
    request.cbSize = (UINT)sizeof request;
    request.lpfnWndProc = hero_ui_win_proc;
    request.hInstance = module;
    request.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    request.hbrBackground = background;
    request.hIcon = icon;
    request.lpszClassName = name;
    return RegisterClassExW(&request) != 0;
}

/* The activation context of the common controls' version 6, made once and
 * activated (THE SYSTEM'S CURRENT LOOK). */
static void hero_ui_win_take_styles(void) {
    if (hero_ui_win_context == NULL) {
        wchar_t folder[MAX_PATH + 1];
        wchar_t file[MAX_PATH + 1];
        DWORD length = GetTempPathW(MAX_PATH + 1, folder);

        hero_ui_win_context = INVALID_HANDLE_VALUE;
        if (length > 0 && length <= MAX_PATH && GetTempFileNameW(folder, L"hui", 0, file) != 0) {
            HANDLE out = CreateFileW(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
            DWORD written = 0;
            DWORD wanted = (DWORD)(sizeof hero_ui_win_manifest - 1);
            bool whole = false;

            if (out != INVALID_HANDLE_VALUE) {
                whole = WriteFile(out, hero_ui_win_manifest, wanted, &written, NULL) && written == wanted;
                (void)CloseHandle(out);
            }
            if (whole) {
                ACTCTXW request;

                memset(&request, 0, sizeof request);
                request.cbSize = (ULONG)sizeof request;
                request.lpSource = file;
                hero_ui_win_context = CreateActCtxW(&request);
            }
            (void)DeleteFileW(file);
        }
    }
    if (hero_ui_win_context != INVALID_HANDLE_VALUE && !hero_ui_win_activated) {
        hero_ui_win_activated = ActivateActCtx(hero_ui_win_context, &hero_ui_win_cookie) != FALSE;
    }
}

static int64_t hero_ui_win_appearance_now(void);
static void hero_ui_win_props_end(void);
static void hero_ui_win_notifier_end(void);
static _Thread_local bool hero_ui_win_undocked;

int64_t hero_ui_arm_open(void) {
    INITCOMMONCONTROLSEX controls;

    if (!hero_ui_win_ready) {
        HMODULE user32 = GetModuleHandleW(L"user32.dll");

        hero_ui_win_find_function(user32, "GetDpiForWindow", &hero_ui_win_dpi_for_window, sizeof hero_ui_win_dpi_for_window);
        hero_ui_win_find_function(user32, "AdjustWindowRectExForDpi", &hero_ui_win_adjust, sizeof hero_ui_win_adjust);
        hero_ui_win_find_function(user32, "SystemParametersInfoForDpi", &hero_ui_win_parameters, sizeof hero_ui_win_parameters);
        hero_ui_win_find_function(user32, "GetSystemMetricsForDpi", &hero_ui_win_metrics, sizeof hero_ui_win_metrics);
        hero_ui_win_find_function(user32, "SetThreadDpiAwarenessContext", &hero_ui_win_awareness, sizeof hero_ui_win_awareness);
        hero_ui_win_find_function(LoadLibraryW(L"shcore.dll"), "GetDpiForMonitor", &hero_ui_win_dpi_for_monitor, sizeof hero_ui_win_dpi_for_monitor);
    }
    hero_ui_win_take_styles();
    if (hero_ui_win_awareness != NULL && !hero_ui_win_aware) {
        HANDLE before = hero_ui_win_awareness(HERO_UI_WIN_AWARE_V2);

        if (before == NULL) {
            before = hero_ui_win_awareness(HERO_UI_WIN_AWARE);
        }
        hero_ui_win_awareness_before = before;
        hero_ui_win_aware = before != NULL;
    }
    memset(&controls, 0, sizeof controls);
    controls.dwSize = (DWORD)sizeof controls;
    controls.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS | ICC_DATE_CLASSES;
    (void)InitCommonControlsEx(&controls);
    if (!hero_ui_win_ready) {
        HBRUSH face = (HBRUSH)(INT_PTR)(COLOR_3DFACE + 1);

        if (!hero_ui_win_register(hero_ui_win_window_class, face, LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION)) || !hero_ui_win_register(hero_ui_win_panel_class, face, NULL) || !hero_ui_win_register(hero_ui_win_picture_class, NULL, NULL) || !hero_ui_win_register(hero_ui_win_canvas_class, NULL, NULL)) {
            return hero_ui_win_failed("registering the window classes", GetLastError());
        }
        hero_ui_win_ready = true;
    }
    hero_ui_unmute();
    hero_ui_win_injecting = 0;
    hero_ui_win_quiet = 0;
    hero_ui_win_modal = 0;
    hero_ui_win_appearance_forced = 0;
    hero_ui_win_taskbar_created = RegisterWindowMessageW(L"TaskbarCreated");
    hero_ui_win_appearance_told = hero_ui_win_appearance_now();
    hero_ui_make_host();
    return HERO_UI_OK;
}

static void hero_ui_win_hold_others(int64_t except, bool hold);
static void hero_ui_win_label_changed(int64_t label, bool gone);
static void hero_ui_win_tray_hide(hero_ui_native *tray);
static int hero_ui_win_page_index(const hero_ui_native *tabs, int64_t page);
static void hero_ui_win_drop_cells(hero_ui_native *table);

/* Destroys `widget`'s system objects and gives back its record; its children
 * are gone already. */
static void hero_ui_win_drop(hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);
    /* The box a page or a pane leaves, laid out again once it is gone. */
    hero_ui_native *box = widget->kind == HERO_UI_PAGE || widget->kind == HERO_UI_PANE ? owner : NULL;

    hero_ui_win_tip_end(widget);
    switch (widget->kind) {
    case HERO_UI_WINDOW:
        if (hero_ui_win_modal == widget->id) {
            hero_ui_win_modal = 0;
            hero_ui_win_hold_others(widget->id, false);
        }
        if (widget->hwnd != NULL && widget->hwnd == hero_ui_win_blocking) {
            /* Its reason goes with it. */
            hero_ui_win_blocking = NULL;
        }
        if (widget->hwnd != NULL) {
            (void)SetWindowLongPtrW(widget->hwnd, GWLP_USERDATA, 0);
            if (GetMenu(widget->hwnd) != NULL) {
                (void)SetMenu(widget->hwnd, NULL);
            }
            (void)DestroyWindow(widget->hwnd);
        }
        if (widget->menu != NULL) {
            (void)DestroyMenu(widget->menu);
        }
        if (widget->accelerators != NULL) {
            (void)DestroyAcceleratorTable(widget->accelerators);
        }
        if (widget->font != NULL) {
            (void)DeleteObject(widget->font);
        }
        break;
    case HERO_UI_MENU:
        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_win_refill_owner(widget);
        }
        if (widget->menu != NULL) {
            (void)DestroyMenu(widget->menu);
        }
        break;
    case HERO_UI_TRAY:
        hero_ui_win_tray_hide(widget);
        if (widget->tray_icon != NULL) {
            (void)DestroyIcon(widget->tray_icon);
        }
        (void)SetWindowLongPtrW(widget->hwnd, GWLP_USERDATA, 0);
        (void)DestroyWindow(widget->hwnd);
        break;
    case HERO_UI_CONTEXT_MENU:
        /* Taken from its widget with it: a right click finds none now. */
        if (widget->menu != NULL) {
            (void)DestroyMenu(widget->menu);
        }
        break;
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_win_fill_menu(owner);
        }
        break;
    case HERO_UI_WEBVIEW:
        /* Before its window goes: the member closes WebView2's controller
         * and calls the host's `gone`. */
        (void)hero_ui_web_win32_destroy(widget->id);
        break;
    case HERO_UI_PAGE:
        if (owner != NULL && owner->hwnd != NULL) {
            int index = hero_ui_win_page_index(owner, widget->id);
            int chosen = (int)SendMessageW(owner->hwnd, TCM_GETCURSEL, 0, 0);

            if (index >= 0) {
                int left;

                hero_ui_mute(1);
                (void)SendMessageW(owner->hwnd, TCM_DELETEITEM, (WPARAM)index, 0);
                left = (int)SendMessageW(owner->hwnd, TCM_GETITEMCOUNT, 0, 0);
                if (index == chosen && left > 0) {
                    (void)SendMessageW(owner->hwnd, TCM_SETCURSEL, (WPARAM)(index < left ? index : left - 1), 0);
                }
                hero_ui_mute(-1);
            }
        }
        (void)SetWindowLongPtrW(widget->hwnd, GWLP_USERDATA, 0);
        (void)DestroyWindow(widget->hwnd);
        break;
    default:
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            (void)SetWindowLongPtrW(widget->radios[i], GWLP_USERDATA, 0);
        }
        if ((widget->kind == HERO_UI_TOOLBAR || widget->kind == HERO_UI_STATUS_BAR) && widget->part != NULL) {
            /* The bar's own control goes with its panel and tells it nothing
             * on the way. */
            (void)SetWindowLongPtrW(widget->part, GWLP_USERDATA, 0);
        }
        if (widget->kind == HERO_UI_SPIN && widget->part != NULL) {
            /* The arrows stand beside the field, not in it. */
            (void)SetWindowLongPtrW(widget->part, GWLP_USERDATA, 0);
            (void)DestroyWindow(widget->part);
        }
        if (widget->hwnd != NULL) {
            (void)SetWindowLongPtrW(widget->hwnd, GWLP_USERDATA, 0);
            (void)DestroyWindow(widget->hwnd);
        }
        if (widget->bitmap != NULL) {
            (void)DeleteObject(widget->bitmap);
        }
        break;
    }
    hero_ui_parts_win32_free(widget->parts);
    widget->parts = NULL;
    hero_ui_canvas_win32_free(widget->canvas);
    widget->canvas = NULL;
    if (widget->kind == HERO_UI_TABLE) {
        hero_ui_win_drop_cells(widget);
    }
    if (widget->kind == HERO_UI_LABEL) {
        hero_ui_win_label_changed(widget->id, true);
    }
    hero_ui_win_edit_end(widget);
    hero_layer_free(widget->access_name);
    hero_layer_free(widget->radios);
    hero_layer_free(widget->radio_places);
    hero_layer_free(widget->label);
    hero_layer_free(widget->customs);
    hero_layer_free(widget->told);
    hero_layer_free(widget->nodes);
    hero_ui_ids_free(&widget->order);
    hero_ui_forget(widget->id);
    hero_layer_free(widget);
    if (box != NULL) {
        hero_ui_win_lay_box(box);
    }
}

/* Destroys widget `id` and every widget whose parent it is, children first. */
static void hero_ui_win_destroy_tree(int64_t id) {
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
        hero_ui_win_destroy_tree(child);
    }
    hero_ui_win_drop(widget);
}

void hero_ui_arm_close(void) {
    hero_ui_win_quits_answered();
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
        hero_ui_win_destroy_tree(first);
    }
    hero_ui_free_map();
    hero_ui_canvas_win32_end();
    hero_ui_win_props_end();
    hero_ui_win_notifier_end();
    hero_ui_win_undocked = false;
    hero_layer_free(hero_ui_win_clip);
    hero_ui_win_clip = NULL;
    hero_ui_win_clip_count = -1;
    hero_ui_win_modal = 0;
    if (hero_ui_win_clip_owner != NULL) {
        (void)DestroyWindow(hero_ui_win_clip_owner);
        hero_ui_win_clip_owner = NULL;
    }
    /* Every dialog ended already (`dialogs_end`, before `close`). */
    if (hero_ui_win_dialog_host != NULL) {
        (void)DestroyWindow(hero_ui_win_dialog_host);
        hero_ui_win_dialog_host = NULL;
    }
    if (hero_ui_win_icon != NULL) {
        (void)DestroyIcon(hero_ui_win_icon);
        hero_ui_win_icon = NULL;
    }
    if (hero_ui_win_activated) {
        (void)DeactivateActCtx(0, hero_ui_win_cookie);
        hero_ui_win_activated = false;
    }
    if (hero_ui_win_aware && hero_ui_win_awareness != NULL) {
        (void)hero_ui_win_awareness(hero_ui_win_awareness_before);
        hero_ui_win_aware = false;
    }
}

/* Makes control `kind` of class `class` in `parent`, at no place and hidden,
 * carrying widget number `id` and its window's font. */
static HWND hero_ui_win_control(const hero_ui_native *window, HWND parent, const wchar_t *class, DWORD style, DWORD extended, int64_t id) {
    HWND made = CreateWindowExW(extended, class, L"", WS_CHILD | WS_CLIPSIBLINGS | style, 0, 0, 0, 0, parent, NULL, GetModuleHandleW(NULL), NULL);

    if (made != NULL) {
        (void)SetWindowLongPtrW(made, GWLP_USERDATA, (LONG_PTR)id);
        (void)SendMessageW(made, WM_SETFONT, (WPARAM)window->font, FALSE);
    }
    return made;
}

/* Makes web view `id` in window `window`, standing in `inside`, its client
 * area or a container's: THE WEB VIEW, kind 17, is `hero_ui_web_win32.c`'s,
 * which draws WebView2 there; this arm keeps its record, its place and
 * whether it is shown. */
static int64_t hero_ui_win_make_web(int64_t id, hero_ui_native *window, HWND inside) {
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
    answer = hero_ui_win_web(hero_ui_web_win32_create(id, hero_ui_web_host_of(), inside));
    if (answer != HERO_UI_OK) {
        hero_ui_forget(id);
        hero_layer_free(made);
    }
    return answer;
}

static void hero_ui_win_give_icon(HWND hwnd);
static void hero_ui_win_dress(HWND hwnd);
static int64_t hero_ui_win_make_tray(hero_ui_native *made);
static void hero_ui_win_tray_hide(hero_ui_native *tray);
static bool hero_ui_win_subclass_edit(HWND hwnd);
static void hero_ui_win_lay_pages(hero_ui_native *tabs);
static void hero_ui_win_lay_panes(hero_ui_native *split);

/* Whether `owner` holds a widget of `kind`: a menu bar's menu, a menu's
 * or a context menu's items and menus, a page in tabs, at most two panes in
 * a split, a context menu on a widget a right click lands on (none on a
 * menu, an item, a page or a pane, one a widget), and every other kind in a
 * window or a container. */
static bool hero_ui_win_holds_kind(const hero_ui_native *owner, int64_t kind) {
    switch (kind) {
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        return owner->kind == HERO_UI_MENU || owner->kind == HERO_UI_CONTEXT_MENU;
    case HERO_UI_MENU:
        return owner->kind == HERO_UI_WINDOW || owner->kind == HERO_UI_MENU || owner->kind == HERO_UI_CONTEXT_MENU;
    case HERO_UI_PAGE:
        return owner->kind == HERO_UI_TABS;
    case HERO_UI_PANE: {
        int panes = 0;

        if (owner->kind != HERO_UI_SPLIT_ROW && owner->kind != HERO_UI_SPLIT_COLUMN) {
            return false;
        }
        for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
            const hero_ui_native *pane = hero_ui_at(at);

            panes += pane != NULL && pane->kind == HERO_UI_PANE && pane->parent == owner->id ? 1 : 0;
        }
        return panes < 2;
    }
    case HERO_UI_CONTEXT_MENU:
        if (owner->kind == HERO_UI_MENU || owner->kind == HERO_UI_MENU_ITEM || owner->kind == HERO_UI_MENU_SEPARATOR || owner->kind == HERO_UI_CONTEXT_MENU || owner->kind == HERO_UI_PAGE || owner->kind == HERO_UI_PANE) {
            return false;
        }
        for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
            const hero_ui_native *menu = hero_ui_at(at);

            if (menu != NULL && menu->kind == HERO_UI_CONTEXT_MENU && menu->parent == owner->id) {
                return false;
            }
        }
        return true;
    case HERO_UI_TOOLBAR:
    case HERO_UI_STATUS_BAR:
        /* In a window, one of each kind to a window. */
        if (owner->kind != HERO_UI_WINDOW) {
            return false;
        }
        for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
            const hero_ui_native *bar = hero_ui_at(at);

            if (bar != NULL && bar->kind == kind && bar->parent == owner->id) {
                return false;
            }
        }
        return true;
    default:
        return hero_ui_win_bar_holds(owner, kind) && owner->inside != NULL;
    }
}

int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    hero_ui_native *owner = NULL;
    hero_ui_native *window = NULL;
    hero_ui_native *made;
    HWND inside;
    int64_t answer = HERO_UI_OK;

    if (hero_ui_find(id) != NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (kind != HERO_UI_WINDOW && kind != HERO_UI_TRAY) {
        owner = hero_ui_find(parent);
        if (owner == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        if (!hero_ui_win_holds_kind(owner, kind)) {
            return HERO_UI_INVALID;
        }
        window = hero_ui_win_top(owner->window);
        if (window == NULL) {
            return HERO_UI_NOT_FOUND;
        }
    }
    if (kind == HERO_UI_WEBVIEW) {
        return hero_ui_win_make_web(id, window, owner->inside);
    }
    inside = owner != NULL ? owner->inside : NULL;
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
        made->hwnd = CreateWindowExW(WS_EX_CONTROLPARENT, hero_ui_win_window_class, L"", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, NULL, NULL, GetModuleHandleW(NULL), NULL);
        if (made->hwnd != NULL) {
            (void)SetWindowLongPtrW(made->hwnd, GWLP_USERDATA, (LONG_PTR)id);
            made->dpi = hero_ui_win_dpi(made->hwnd);
            made->font = hero_ui_win_font(made->dpi);
            made->content_width = 100;
            made->content_height = 100;
            made->inside = made->hwnd;
            made->resizable = true;
        }
        break;
    case HERO_UI_LABEL:
        made->hwnd = hero_ui_win_control(window, inside, L"STATIC", SS_LEFT | SS_NOPREFIX, 0, id);
        break;
    case HERO_UI_BUTTON:
        made->hwnd = hero_ui_win_control(window, inside, L"BUTTON", BS_PUSHBUTTON | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_TEXT_FIELD:
        made->hwnd = hero_ui_win_control(window, inside, L"EDIT", ES_LEFT | ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_TEXT_AREA:
        made->hwnd = hero_ui_win_control(window, inside, L"EDIT", ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_NOHIDESEL | WS_VSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_CHECKBOX:
        made->hwnd = hero_ui_win_control(window, inside, L"BUTTON", BS_AUTOCHECKBOX | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_RADIO_GROUP:
        made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_panel_class, 0, WS_EX_CONTROLPARENT, id);
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        made->hwnd = hero_ui_win_control(window, inside, L"LISTBOX", LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP | (kind == HERO_UI_LIST_MULTIPLE ? LBS_EXTENDEDSEL : 0), WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_COMBO:
        made->hwnd = hero_ui_win_control(window, inside, L"COMBOBOX", CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_PROGRESS:
        made->hwnd = hero_ui_win_control(window, inside, PROGRESS_CLASSW, 0, 0, id);
        break;
    case HERO_UI_SEPARATOR:
        made->hwnd = hero_ui_win_control(window, inside, L"STATIC", SS_ETCHEDHORZ, 0, id);
        break;
    case HERO_UI_GROUP:
        /* A panel holding the frame and, over it, the content the widgets
         * are made in. */
        made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_panel_class, WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        if (made->hwnd != NULL) {
            made->part = hero_ui_win_control(window, made->hwnd, L"BUTTON", BS_GROUPBOX | WS_VISIBLE, 0, id);
            made->inside = hero_ui_win_control(window, made->hwnd, hero_ui_win_panel_class, WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
            if (made->part == NULL || made->inside == NULL) {
                (void)DestroyWindow(made->hwnd);
                made->hwnd = NULL;
                made->part = NULL;
                made->inside = NULL;
            }
        }
        break;
    case HERO_UI_EDITABLE_COMBO:
        made->hwnd = hero_ui_win_control(window, inside, L"COMBOBOX", CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_SPIN:
        made->hwnd = hero_ui_win_control(window, inside, L"EDIT", ES_RIGHT | ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        if (made->hwnd != NULL) {
            UDACCEL one = {0, 1};

            made->part = hero_ui_win_control(window, inside, UPDOWN_CLASSW, UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS, 0, id);
            if (made->part == NULL) {
                (void)DestroyWindow(made->hwnd);
                made->hwnd = NULL;
            } else {
                /* Each arrow changes the value by 1, however long it is held. */
                (void)SendMessageW(made->part, UDM_SETBUDDY, (WPARAM)made->hwnd, 0);
                (void)SendMessageW(made->part, UDM_SETRANGE32, (WPARAM)made->least, (LPARAM)made->most);
                (void)SendMessageW(made->part, UDM_SETACCEL, 1, (LPARAM)&one);
                hero_ui_win_spin_show(made);
            }
        }
        break;
    case HERO_UI_SLIDER:
        made->hwnd = hero_ui_win_control(window, inside, TRACKBAR_CLASSW, TBS_HORZ | TBS_NOTICKS | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_TOOLBAR:
    case HERO_UI_STATUS_BAR:
        /* A panel the items are made in, and under them the system's own bar,
         * which draws what a toolbar or a status bar looks like and holds no
         * item of ours. */
        made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_panel_class, WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        if (made->hwnd != NULL) {
            made->inside = made->hwnd;
            made->part = hero_ui_win_control(window, made->hwnd, kind == HERO_UI_TOOLBAR ? TOOLBARCLASSNAMEW : STATUSCLASSNAMEW, WS_VISIBLE | CCS_NOPARENTALIGN | CCS_NORESIZE | (kind == HERO_UI_TOOLBAR ? TBSTYLE_FLAT : 0), 0, id);
            if (made->part == NULL) {
                (void)DestroyWindow(made->hwnd);
                made->hwnd = NULL;
                made->inside = NULL;
            } else if (kind == HERO_UI_TOOLBAR) {
                (void)SendMessageW(made->part, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
            } else {
                /* One pane across the bar and no border round it. */
                (void)SendMessageW(made->part, SB_SIMPLE, TRUE, 0);
                (void)SendMessageW(made->part, SB_SETTEXTW, SB_SIMPLEID | SBT_NOBORDERS, (LPARAM)L"");
            }
        }
        break;
    case HERO_UI_DATE_PICKER:
    case HERO_UI_TIME_PICKER:
    case HERO_UI_DATE_TIME_PICKER:
        made->hwnd = hero_ui_win_control(window, inside, DATETIMEPICK_CLASSW, WS_TABSTOP | (kind == HERO_UI_TIME_PICKER ? DTS_UPDOWN : 0), 0, id);
        if (made->hwnd != NULL) {
            hero_ui_win_stamp_setup(made);
        }
        break;
    case HERO_UI_CALENDAR:
        /* One day at a time: never `MCS_MULTISELECT`. */
        made->hwnd = hero_ui_win_control(window, inside, MONTHCAL_CLASSW, WS_TABSTOP, 0, id);
        if (made->hwnd != NULL) {
            hero_ui_win_stamp_setup(made);
        }
        break;
    case HERO_UI_COLOR_WELL:
        made->hwnd = hero_ui_win_control(window, inside, L"BUTTON", BS_OWNERDRAW | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_FONT_BUTTON:
        made->hwnd = hero_ui_win_control(window, inside, L"BUTTON", BS_PUSHBUTTON | WS_TABSTOP, 0, id);
        if (made->hwnd != NULL && !hero_ui_win_font_start(made)) {
            answer = HERO_UI_LIMIT;
        }
        break;
    case HERO_UI_IMAGE:
        made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_picture_class, 0, 0, id);
        break;
    case HERO_UI_CANVAS:
        made->canvas = hero_ui_canvas_win32_new(&answer);
        if (made->canvas != NULL) {
            made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_canvas_class, WS_TABSTOP, 0, id);
        }
        break;
    case HERO_UI_MENU:
    case HERO_UI_CONTEXT_MENU:
        made->menu = CreatePopupMenu();
        if (made->menu == NULL) {
            answer = hero_ui_win_failed("making a menu", GetLastError());
        }
        break;
    case HERO_UI_TABS:
        made->hwnd = hero_ui_win_control(window, inside, WC_TABCONTROLW, WS_TABSTOP | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        break;
    case HERO_UI_PAGE:
        made->hwnd = hero_ui_win_control(window, owner->hwnd, hero_ui_win_panel_class, WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        made->inside = made->hwnd;
        made->placed = true;
        break;
    case HERO_UI_PANE:
        made->hwnd = hero_ui_win_control(window, owner->hwnd, hero_ui_win_panel_class, WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        made->inside = made->hwnd;
        made->placed = true;
        break;
    case HERO_UI_SCROLL:
        made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_panel_class, WS_VSCROLL | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        if (made->hwnd != NULL) {
            /* Its content: moved up and down within it, carrying its number
             * so a click on it finds the box. */
            made->inside = hero_ui_win_control(window, made->hwnd, hero_ui_win_panel_class, WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
            if (made->inside == NULL) {
                (void)DestroyWindow(made->hwnd);
                made->hwnd = NULL;
            }
        }
        break;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        made->hwnd = hero_ui_win_control(window, inside, hero_ui_win_panel_class, WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, id);
        made->most = HERO_UI_WIN_POINTS_MOST;
        break;
    case HERO_UI_TABLE:
        made->hwnd = hero_ui_win_control(window, inside, WC_LISTVIEWW, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        made->sort_column = -1;
        made->sort_ascending = true;
        if (made->hwnd != NULL) {
            (void)SendMessageW(made->hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
        }
        break;
    case HERO_UI_TRAY:
        answer = hero_ui_win_make_tray(made);
        break;
    case HERO_UI_TREE:
        made->hwnd = hero_ui_win_control(window, inside, WC_TREEVIEWW, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        made->command = hero_ui_win_new_command();
        if (made->command == 0) {
            answer = hero_ui_refused(HERO_UI_OTHER, "every command number Win32 gives menu items is taken");
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    if (answer == HERO_UI_OK && made->hwnd == NULL && kind != HERO_UI_MENU && kind != HERO_UI_MENU_ITEM && kind != HERO_UI_MENU_SEPARATOR && kind != HERO_UI_CONTEXT_MENU) {
        answer = hero_ui_win_failed("making a widget", GetLastError());
    }
    if (answer == HERO_UI_OK && !hero_ui_keep(id, made)) {
        answer = HERO_UI_LIMIT;
    }
    if (answer != HERO_UI_OK) {
        if (made->kind == HERO_UI_TRAY) {
            hero_ui_win_tray_hide(made);
        }
        if (made->hwnd != NULL) {
            (void)DestroyWindow(made->hwnd);
        }
        if (made->menu != NULL) {
            (void)DestroyMenu(made->menu);
        }
        if (made->font != NULL) {
            (void)DeleteObject(made->font);
        }
        hero_ui_canvas_win32_free(made->canvas);
        hero_layer_free(made->label);
        hero_layer_free(made);
        return answer;
    }
    if (window != NULL) {
        window->tabs_stale = true;
    }
    switch (kind) {
    case HERO_UI_WINDOW:
        hero_ui_win_size_content(made);
        if (hero_ui_win_icon != NULL) {
            hero_ui_win_give_icon(made->hwnd);
        }
        hero_ui_win_dress(made->hwnd);
        if (hero_ui_win_modal != 0) {
            /* Made while another window is modal: held as the others are. */
            made->held_by_modal = true;
            (void)EnableWindow(made->hwnd, FALSE);
        }
        break;
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
        (void)SendMessageW(made->hwnd, EM_SETLIMITTEXT, 0, 0);
        /* Where it fails the field shows its own menu alone. */
        (void)hero_ui_win_subclass_edit(made->hwnd);
        break;
    case HERO_UI_CANVAS:
        hero_ui_win_canvas_role(made);
        break;
    case HERO_UI_PAGE: {
        TCITEMW item;
        LRESULT at;

        memset(&item, 0, sizeof item);
        item.mask = TCIF_TEXT | TCIF_PARAM;
        item.pszText = L"";
        item.lParam = (LPARAM)id;
        hero_ui_mute(1);
        at = SendMessageW(owner->hwnd, TCM_INSERTITEMW, (WPARAM)SendMessageW(owner->hwnd, TCM_GETITEMCOUNT, 0, 0), (LPARAM)&item);
        if (at >= 0 && SendMessageW(owner->hwnd, TCM_GETCURSEL, 0, 0) < 0) {
            (void)SendMessageW(owner->hwnd, TCM_SETCURSEL, (WPARAM)at, 0);
        }
        hero_ui_mute(-1);
        if (at < 0) {
            hero_ui_win_drop(made);
            return hero_ui_win_failed("adding a page to tabs", GetLastError());
        }
        hero_ui_win_lay_pages(owner);
        break;
    }
    case HERO_UI_PANE:
        hero_ui_win_lay_panes(owner);
        break;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        hero_ui_win_show_range(made);
        break;
    case HERO_UI_MENU:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_win_drop(made);
            return HERO_UI_LIMIT;
        }
        return owner->kind == HERO_UI_WINDOW ? hero_ui_win_fill_bar(owner) : hero_ui_win_fill_menu(owner);
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_win_drop(made);
            return HERO_UI_LIMIT;
        }
        return hero_ui_win_fill_menu(owner);
    default:
        break;
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_destroy(int64_t id) {
    if (hero_ui_find(id) == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_win_destroy_tree(id);
    return HERO_UI_OK;
}

static int64_t hero_ui_win_set_cells(hero_ui_native *table, const uint8_t *items, int64_t count);
static int64_t hero_ui_win_choose_in_box(hero_ui_native *widget, const int64_t *indices, int64_t count);
static bool hero_ui_win_rows_chosen(const hero_ui_native *table, int64_t **rows, int64_t *count);
static int64_t hero_ui_win_item_chosen(const hero_ui_native *tree);
static int64_t hero_ui_win_divider_at(const hero_ui_native *split);

static void hero_ui_win_first_shown(hero_ui_native *window);
static void hero_ui_win_lay_scroll(hero_ui_native *scroll);
static int64_t hero_ui_win_divider(UINT dpi);

static void hero_ui_win_label_changed(int64_t label, bool gone);
static bool hero_ui_win_tray_show(hero_ui_native *tray, DWORD how);
static int64_t hero_ui_win_tray_image(hero_ui_native *tray, const uint8_t *rgba, int64_t width, int64_t height);
static void hero_ui_win_shown_in_taskbar(const hero_ui_native *window);

int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    unsigned ways = 0;
    wchar_t *wide;
    BOOL set;
    DWORD code;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_LABEL:
    case HERO_UI_TEXT_AREA:
        ways = HERO_UI_WIN_LINES;
        break;
    case HERO_UI_BUTTON:
    case HERO_UI_CHECKBOX:
        ways = HERO_UI_WIN_PREFIX;
        break;
    case HERO_UI_PAGE:
        ways = HERO_UI_WIN_PREFIX | HERO_UI_WIN_TABS;
        break;
    case HERO_UI_GROUP:
        ways = HERO_UI_WIN_PREFIX;
        break;
    case HERO_UI_FONT_BUTTON:
    case HERO_UI_EDITABLE_COMBO:
    case HERO_UI_WINDOW:
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_MENU:
    case HERO_UI_MENU_ITEM:
    case HERO_UI_TRAY:
        break;
    default:
        return HERO_UI_INVALID;
    }
    wide = hero_ui_win_wide(text, count, ways);
    if (wide == NULL) {
        return HERO_UI_LIMIT;
    }
    if (widget->kind == HERO_UI_PAGE) {
        /* Its tab's title, which the tab control draws with `&` as the
         * underline of a key, so it is written doubled. */
        hero_ui_native *tabs = hero_ui_find(widget->parent);
        int index = tabs != NULL ? hero_ui_win_page_index(tabs, id) : -1;
        TCITEMW item;

        memset(&item, 0, sizeof item);
        item.mask = TCIF_TEXT;
        item.pszText = wide;
        set = index >= 0 && SendMessageW(tabs->hwnd, TCM_SETITEMW, (WPARAM)index, (LPARAM)&item) != 0;
        code = GetLastError();
        hero_layer_free(wide);
        if (set) {
            /* A title of another width may change the room the tabs take. */
            hero_ui_win_lay_pages(tabs);
        }
        return set ? HERO_UI_OK : hero_ui_win_failed("setting a tab's title", code);
    }
    if (widget->kind == HERO_UI_MENU || widget->kind == HERO_UI_MENU_ITEM) {
        hero_layer_free(widget->label);
        widget->label = wide;
        return hero_ui_win_refill_owner(widget);
    }
    if (widget->kind == HERO_UI_TRAY) {
        /* Its tip, which Windows shows of its first 127 units. */
        hero_layer_free(widget->label);
        widget->label = wide;
        return !widget->tray_shown || hero_ui_win_tray_show(widget, NIM_MODIFY) ? HERO_UI_OK : hero_ui_win_failed("giving a tray icon its words", GetLastError());
    }
    if (widget->kind == HERO_UI_FONT_BUTTON) {
        /* Its text is the font as `<size>|<style>|<family>`, shown as the
         * family and the size. */
        int64_t answer = hero_ui_win_font_apply(widget, wide, true, NULL);

        hero_layer_free(wide);
        return answer;
    }
    hero_ui_mute(1);
    set = SetWindowTextW(widget->kind == HERO_UI_GROUP ? widget->part : widget->hwnd, wide);
    code = GetLastError();
    if (set && widget->kind == HERO_UI_LABEL) {
        hero_ui_win_label_changed(id, false);
    }
    if (set && (widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA)) {
        (void)SendMessageW(widget->hwnd, EM_SETSEL, wcslen(wide), (LPARAM)wcslen(wide));
        (void)SendMessageW(widget->hwnd, EM_SCROLLCARET, 0, 0);
        hero_ui_win_edit_reset(widget);
    }
    hero_ui_mute(-1);
    hero_layer_free(wide);
    return set ? HERO_UI_OK : hero_ui_win_failed("setting a text", code);
}

/* Makes radio group `group`'s items anew, the `count` bytes of `items`. */
static int64_t hero_ui_win_make_radios(hero_ui_native *group, const uint8_t *items, int64_t count) {
    hero_ui_native *window = hero_ui_win_window(group->window);
    int64_t texts = hero_ui_texts_in(items, count);
    HWND *radios = NULL;
    int64_t *places = NULL;

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (texts > 0) {
        radios = hero_layer_alloc_zero((size_t)texts, sizeof *radios);
        places = hero_layer_alloc_zero((size_t)texts * 4, sizeof *places);
        if (radios == NULL || places == NULL) {
            hero_layer_free(radios);
            hero_layer_free(places);
            return HERO_UI_LIMIT;
        }
    }
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        (void)SetWindowLongPtrW(group->radios[i], GWLP_USERDATA, 0);
        (void)DestroyWindow(group->radios[i]);
    }
    hero_layer_free(group->radios);
    hero_layer_free(group->radio_places);
    group->radios = radios;
    group->radio_places = places;
    group->radio_count = 0;
    for (int64_t i = 0; i < texts; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(items, count, i, &length);
        wchar_t *wide = hero_ui_win_wide(text, length, HERO_UI_WIN_PREFIX);
        HWND radio = hero_ui_win_control(window, group->hwnd, L"BUTTON", BS_AUTORADIOBUTTON | WS_VISIBLE | (i == 0 ? WS_GROUP | WS_TABSTOP : 0), 0, group->id);

        if (radio == NULL || wide == NULL) {
            DWORD code = GetLastError();

            hero_layer_free(wide);
            if (radio != NULL) {
                (void)DestroyWindow(radio);
            }
            return wide == NULL ? HERO_UI_LIMIT : hero_ui_win_failed("making a radio group's item", code);
        }
        (void)SetWindowTextW(radio, wide);
        hero_layer_free(wide);
        (void)EnableWindow(radio, group->enabled);
        radios[i] = radio;
        places[i * 4 + 2] = -1;
        group->radio_count = i + 1;
    }
    hero_ui_win_put(group);
    if (group->tip != NULL) {
        (void)hero_ui_win_tip_apply(group);
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t texts = hero_ui_texts_in(items, count);
    UINT reset;
    UINT add;
    wchar_t *kept = NULL;
    int64_t answer = HERO_UI_OK;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        return hero_ui_win_make_radios(widget, items, count);
    }
    if (widget->kind == HERO_UI_TABLE) {
        return hero_ui_win_set_cells(widget, items, count);
    }
    if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
        reset = LB_RESETCONTENT;
        add = LB_ADDSTRING;
    } else if (widget->kind == HERO_UI_COMBO || widget->kind == HERO_UI_EDITABLE_COMBO) {
        reset = CB_RESETCONTENT;
        add = CB_ADDSTRING;
    } else {
        return HERO_UI_INVALID;
    }
    hero_ui_mute(1);
    if (widget->kind == HERO_UI_EDITABLE_COMBO) {
        /* Emptying the items empties the field: its text is kept. */
        kept = hero_ui_win_wide_of(widget->hwnd);
    }
    (void)SendMessageW(widget->hwnd, reset, 0, 0);
    widget->items = 0;
    for (int64_t i = 0; i < texts && answer == HERO_UI_OK; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(items, count, i, &length);
        wchar_t *wide = hero_ui_win_wide(text, length, 0);
        LRESULT added;

        if (wide == NULL) {
            answer = HERO_UI_LIMIT;
            break;
        }
        added = SendMessageW(widget->hwnd, add, 0, (LPARAM)wide);
        hero_layer_free(wide);
        if (added < 0) {
            answer = HERO_UI_LIMIT;
            break;
        }
        widget->items += 1;
    }
    if (answer != HERO_UI_OK) {
        (void)SendMessageW(widget->hwnd, reset, 0, 0);
        widget->items = 0;
    }
    if (widget->kind == HERO_UI_COMBO) {
        (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)-1, 0);
    }
    if (widget->kind == HERO_UI_EDITABLE_COMBO) {
        if (kept != NULL) {
            (void)SetWindowTextW(widget->hwnd, kept);
        }
        hero_layer_free(kept);
    }
    hero_ui_mute(-1);
    return answer;
}

int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        if (value != 0 && value != 1) {
            return HERO_UI_INVALID;
        }
        (void)SendMessageW(widget->hwnd, BM_SETCHECK, value == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
        return HERO_UI_OK;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        if (value < widget->least || value > widget->most) {
            return HERO_UI_INVALID;
        }
        widget->value = value;
        hero_ui_win_show_range(widget);
        return HERO_UI_OK;
    case HERO_UI_COMBO:
        if (value < -1 || value >= widget->items) {
            return HERO_UI_INVALID;
        }
        (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)value, 0);
        return HERO_UI_OK;
    case HERO_UI_EDITABLE_COMBO:
        /* An item's index sets the text to it; -1 leaves the text. */
        if (value < -1 || value >= widget->items) {
            return HERO_UI_INVALID;
        }
        if (value >= 0) {
            hero_ui_mute(1);
            (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)value, 0);
            hero_ui_mute(-1);
        }
        return HERO_UI_OK;
    case HERO_UI_SPIN:
        if (value < widget->least || value > widget->most) {
            return HERO_UI_INVALID;
        }
        widget->value = value;
        hero_ui_win_spin_show(widget);
        return HERO_UI_OK;
    case HERO_UI_DATE_PICKER:
    case HERO_UI_TIME_PICKER:
    case HERO_UI_DATE_TIME_PICKER:
    case HERO_UI_CALENDAR:
        return hero_ui_win_stamp_set(widget, value, false);
    case HERO_UI_COLOR_WELL:
        return hero_ui_win_well_set(widget, value, false);
    case HERO_UI_RADIO_GROUP:
        if (value < -1 || value >= widget->radio_count) {
            return HERO_UI_INVALID;
        }
        hero_ui_win_check_radio(widget, value);
        return HERO_UI_OK;
    case HERO_UI_MENU_ITEM:
        /* A check or radio item's state; checking a radio item unchecks
         * the others of its group. */
        if (widget->check == HERO_UI_CHECK_NONE || (value != 0 && value != 1)) {
            return HERO_UI_INVALID;
        }
        if (value == 1 && widget->check == HERO_UI_CHECK_RADIO) {
            hero_ui_win_check_in_group(widget);
        } else {
            widget->checked = value == 1;
        }
        return hero_ui_win_refill_owner(widget);
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        if (value < 0) {
            return HERO_UI_INVALID;
        }
        widget->value = hero_ui_between(value, 0, HERO_UI_WIN_POINTS_MOST);
        hero_ui_win_lay_panes(widget);
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_SPLIT_ROW || widget->kind == HERO_UI_SPLIT_COLUMN) {
        /* Where the person may drag the divider; the program's own place is
         * kept as set. */
        if (least < 0) {
            return HERO_UI_INVALID;
        }
        widget->least = least;
        widget->most = most;
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_SPIN) {
        /* The value is not moved by the range: the face clamps it and gives
         * it again. */
        if (least > most) {
            return HERO_UI_INVALID;
        }
        widget->least = least;
        widget->most = most;
        (void)SendMessageW(widget->part, UDM_SETRANGE32, (WPARAM)(int)hero_ui_between(least, INT32_MIN, INT32_MAX), (LPARAM)(int)hero_ui_between(most, INT32_MIN, INT32_MAX));
        hero_ui_win_spin_show(widget);
        return HERO_UI_OK;
    }
    if (hero_ui_win_stamp_kind(widget->kind)) {
        return hero_ui_win_stamp_range(widget, least, most);
    }
    if (widget->kind == HERO_UI_COLOR_WELL) {
        return hero_ui_refused(HERO_UI_INVALID, "a colour well has no range: its value is the colour's four bytes");
    }
    if (widget->kind != HERO_UI_SLIDER && widget->kind != HERO_UI_PROGRESS) {
        return HERO_UI_INVALID;
    }
    widget->least = least;
    widget->most = most;
    widget->value = hero_ui_between(widget->value, least, most);
    hero_ui_win_show_range(widget);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_TABS || widget->kind == HERO_UI_TABLE || widget->kind == HERO_UI_TREE) {
        return hero_ui_win_choose_in_box(widget, indices, count);
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
    if (widget->kind == HERO_UI_LIST) {
        (void)SendMessageW(widget->hwnd, LB_SETCURSEL, count == 0 ? (WPARAM)-1 : (WPARAM)indices[0], 0);
    } else {
        (void)SendMessageW(widget->hwnd, LB_SETSEL, FALSE, -1);
        for (int64_t i = 0; i < count; i += 1) {
            (void)SendMessageW(widget->hwnd, LB_SETSEL, TRUE, (LPARAM)indices[i]);
        }
    }
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (which == HERO_UI_FLAG_RESIZABLE) {
        LONG_PTR style;

        if (widget->kind != HERO_UI_WINDOW) {
            return HERO_UI_INVALID;
        }
        widget->resizable = on == 1;
        if (widget->unframed || widget->full_screen) {
            /* No frame to resize it by: its edges answer the person's
             * drag while it is resizable (WM_NCHITTEST), or the frame it
             * is given back takes the flag. */
            if (widget->full_screen) {
                widget->style_before = on == 1 ? (widget->style_before | WS_THICKFRAME | WS_MAXIMIZEBOX) : (widget->style_before & ~(LONG_PTR)(WS_THICKFRAME | WS_MAXIMIZEBOX));
            }
            return HERO_UI_OK;
        }
        style = GetWindowLongPtrW(widget->hwnd, GWL_STYLE);
        style = on == 1 ? (style | WS_THICKFRAME | WS_MAXIMIZEBOX) : (style & ~(LONG_PTR)(WS_THICKFRAME | WS_MAXIMIZEBOX));
        /* Muted from the style on, which Windows may answer by a resize at
         * once (`hero_ui_win_set_framed`). */
        hero_ui_mute(1);
        (void)SetWindowLongPtrW(widget->hwnd, GWL_STYLE, style);
        (void)SetWindowPos(widget->hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        hero_ui_mute(-1);
        hero_ui_win_size_content(widget);
        return HERO_UI_OK;
    }
    if (which == HERO_UI_FLAG_VISIBLE) {
        widget->visible = on == 1;
        switch (widget->kind) {
        case HERO_UI_WINDOW:
            if (on == 1) {
                bool first = !widget->shown_once;

                (void)ShowWindow(widget->hwnd, first ? SW_SHOWNORMAL : SW_SHOW);
                (void)SetForegroundWindow(widget->hwnd);
                hero_ui_win_shown_in_taskbar(widget);
                if (first) {
                    hero_ui_win_first_shown(widget);
                }
            } else {
                (void)ShowWindow(widget->hwnd, SW_HIDE);
            }
            return HERO_UI_OK;
        case HERO_UI_PAGE:
        case HERO_UI_PANE:
            hero_ui_win_lay_box(hero_ui_find(widget->parent));
            return HERO_UI_OK;
        case HERO_UI_CONTEXT_MENU:
            return HERO_UI_OK;
        case HERO_UI_TRAY:
            if (on == 1 && !widget->tray_shown) {
                return hero_ui_win_tray_show(widget, NIM_ADD) ? HERO_UI_OK : hero_ui_win_failed("adding an icon to the notification area", GetLastError());
            }
            if (on == 0) {
                hero_ui_win_tray_hide(widget);
            }
            return HERO_UI_OK;
        case HERO_UI_MENU:
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_win_refill_owner(widget);
        case HERO_UI_WEBVIEW:
            hero_ui_win_put(widget);
            return HERO_UI_OK;
        default:
            (void)ShowWindow(widget->hwnd, widget->visible && widget->placed ? SW_SHOWNA : SW_HIDE);
            if (widget->kind == HERO_UI_SPIN) {
                (void)ShowWindow(widget->part, widget->visible && widget->placed ? SW_SHOWNA : SW_HIDE);
            }
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
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_win_refill_owner(widget);
        case HERO_UI_CONTEXT_MENU:
            return HERO_UI_OK;
        default:
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                (void)EnableWindow(widget->radios[i], on == 1);
            }
            (void)EnableWindow(widget->hwnd, on == 1);
            if (widget->part != NULL) {
                (void)EnableWindow(widget->part, on == 1);
            }
            if (widget->kind == HERO_UI_TOOLBAR || widget->kind == HERO_UI_STATUS_BAR) {
                /* A disabled window ignores the person but draws its children
                 * as they were: the items are greyed with their bar, and take
                 * back their own state with it. */
                for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
                    const hero_ui_native *item = hero_ui_at(at);

                    if (item != NULL && item->parent == widget->id && item->hwnd != NULL) {
                        (void)EnableWindow(item->hwnd, on == 1 && item->enabled);
                    }
                }
            }
            return HERO_UI_OK;
        }
    }
    return HERO_UI_INVALID;
}

/* The virtual key of the key `hero_ui.h` names, and whether it is an
 * extended one; false for a name it does not list. */
static bool hero_ui_win_key_of(const uint8_t *key, int64_t count, WORD *code, bool *extended) {
    static const struct {
        const char *name;
        WORD code;
        bool extended;
    } named[] = {
        {"Enter", VK_RETURN, false}, {"Escape", VK_ESCAPE, false}, {"Tab", VK_TAB, false},     {"Space", VK_SPACE, false}, {"Delete", VK_DELETE, true}, {"Backspace", VK_BACK, false}, {"Up", VK_UP, true},          {"Down", VK_DOWN, true},
        {"Left", VK_LEFT, true},     {"Right", VK_RIGHT, true},    {"Home", VK_HOME, true},     {"End", VK_END, true},      {"PageUp", VK_PRIOR, true},  {"PageDown", VK_NEXT, true},
    };

    *extended = false;
    if (count == 1 && key[0] >= 'a' && key[0] <= 'z') {
        *code = (WORD)(key[0] - 'a' + 'A');
        return true;
    }
    if (count == 1 && key[0] >= '0' && key[0] <= '9') {
        *code = (WORD)key[0];
        return true;
    }
    if (count >= 2 && count <= 3 && key[0] == 'F') {
        int64_t n = 0;

        for (int64_t i = 1; i < count; i += 1) {
            if (key[i] < '0' || key[i] > '9') {
                return false;
            }
            n = n * 10 + (key[i] - '0');
        }
        if (n >= 1 && n <= 12 && !(count == 3 && key[1] == '0')) {
            *code = (WORD)(VK_F1 + n - 1);
            return true;
        }
        return false;
    }
    for (size_t i = 0; i < sizeof named / sizeof named[0]; i += 1) {
        if (strlen(named[i].name) == (size_t)count && memcmp(named[i].name, key, (size_t)count) == 0) {
            *code = named[i].code;
            *extended = named[i].extended;
            return true;
        }
    }
    return false;
}

int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    hero_ui_native *item = hero_ui_find(id);
    WORD code = 0;
    bool extended = false;

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM || (count > 0 && !hero_ui_win_key_of(key, count, &code, &extended))) {
        return HERO_UI_INVALID;
    }
    item->key = code;
    item->key_extended = extended;
    item->held = (BYTE)(((modifiers & HERO_UI_PRIMARY) != 0 ? FCONTROL : 0) | ((modifiers & HERO_UI_SHIFT) != 0 ? FSHIFT : 0) | ((modifiers & HERO_UI_ALT) != 0 ? FALT : 0));
    return hero_ui_win_refill_owner(item);
}

int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    hero_ui_native *picture = hero_ui_find(id);
    BITMAPINFO format;
    void *bits = NULL;
    HBITMAP made;
    uint8_t *into;

    if (picture == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (picture->kind == HERO_UI_TRAY) {
        return hero_ui_win_tray_image(picture, rgba, width, height);
    }
    if (picture->kind != HERO_UI_IMAGE) {
        return HERO_UI_INVALID;
    }
    if (picture->bitmap != NULL) {
        (void)DeleteObject(picture->bitmap);
        picture->bitmap = NULL;
    }
    picture->bitmap_width = 0;
    picture->bitmap_height = 0;
    if (width == 0 || height == 0) {
        (void)InvalidateRect(picture->hwnd, NULL, TRUE);
        return HERO_UI_OK;
    }
    if (width > INT32_MAX / 4 || height > INT32_MAX / 4) {
        return HERO_UI_LIMIT;
    }
    memset(&format, 0, sizeof format);
    format.bmiHeader.biSize = (DWORD)sizeof format.bmiHeader;
    format.bmiHeader.biWidth = (LONG)width;
    format.bmiHeader.biHeight = -(LONG)height;
    format.bmiHeader.biPlanes = 1;
    format.bmiHeader.biBitCount = 32;
    format.bmiHeader.biCompression = BI_RGB;
    made = CreateDIBSection(NULL, &format, DIB_RGB_COLORS, &bits, NULL, 0);
    if (made == NULL || bits == NULL) {
        if (made != NULL) {
            (void)DeleteObject(made);
        }
        return HERO_UI_LIMIT;
    }
    into = bits;
    for (int64_t i = 0; i < width * height; i += 1) {
        const uint8_t *from = rgba + i * 4;
        unsigned alpha = from[3];

        into[i * 4 + 0] = (uint8_t)(((unsigned)from[2] * alpha + 127u) / 255u);
        into[i * 4 + 1] = (uint8_t)(((unsigned)from[1] * alpha + 127u) / 255u);
        into[i * 4 + 2] = (uint8_t)(((unsigned)from[0] * alpha + 127u) / 255u);
        into[i * 4 + 3] = (uint8_t)alpha;
    }
    picture->bitmap = made;
    picture->bitmap_width = width;
    picture->bitmap_height = height;
    (void)InvalidateRect(picture->hwnd, NULL, TRUE);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    hero_ui_native *window = hero_ui_find(id);
    RECT client;

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (window->kind == HERO_UI_SCROLL && which == HERO_UI_SIZE_CONTENT) {
        /* A scrolled box's content, which scrolls within it. */
        window->scroll_width = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
        window->scroll_height = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
        hero_ui_win_lay_scroll(window);
        return HERO_UI_OK;
    }
    if (window->kind != HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    if (which == HERO_UI_SIZE_LEAST) {
        window->least_width = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
        window->least_height = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
        return HERO_UI_OK;
    }
    width = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
    height = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
    if (width == window->content_width && height == window->content_height && GetClientRect(window->hwnd, &client) && client.right == hero_ui_win_px(width, window->dpi) && client.bottom == hero_ui_win_px(height, window->dpi)) {
        return HERO_UI_OK;
    }
    window->content_width = width;
    window->content_height = height;
    hero_ui_win_size_content(window);
    if (GetClientRect(window->hwnd, &client) && (client.right != hero_ui_win_px(width, window->dpi) || client.bottom != hero_ui_win_px(height, window->dpi))) {
        /* Windows kept the frame inside what the screen or the least size
         * allow: the size the content took is the one kept. */
        window->content_width = hero_ui_win_pt(client.right, window->dpi);
        window->content_height = hero_ui_win_pt(client.bottom, window->dpi);
    }
    return HERO_UI_OK;
}

/* Turns separator `line` to run along the longer side of the rectangle it
 * was placed in, across where the two are equal: the system's etched line
 * is a style of its control, horizontal or vertical. */
static void hero_ui_win_turn_separator(hero_ui_native *line) {
    LONG_PTR style = GetWindowLongPtrW(line->hwnd, GWL_STYLE);
    LONG_PTR turned = (style & ~(LONG_PTR)SS_TYPEMASK) | (line->height > line->width ? SS_ETCHEDVERT : SS_ETCHEDHORZ);

    if (turned != style) {
        (void)SetWindowLongPtrW(line->hwnd, GWL_STYLE, turned);
        (void)InvalidateRect(line->hwnd, NULL, TRUE);
    }
}

int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    /* A page and a pane are placed by their box. */
    if ((widget->hwnd == NULL && widget->kind != HERO_UI_WEBVIEW) || widget->kind == HERO_UI_WINDOW || widget->kind == HERO_UI_PAGE || widget->kind == HERO_UI_PANE || widget->kind == HERO_UI_TRAY) {
        return HERO_UI_INVALID;
    }
    if (index < 0 && (widget->x != x || widget->y != y)) {
        hero_ui_native *window = hero_ui_win_window(widget->window);

        if (window != NULL) {
            window->tabs_stale = true;
        }
    }
    if (index >= 0) {
        int64_t *place;

        if (widget->kind != HERO_UI_RADIO_GROUP || index >= widget->radio_count) {
            return HERO_UI_INVALID;
        }
        place = &widget->radio_places[index * 4];
        place[0] = hero_ui_between(x, -HERO_UI_WIN_POINTS_MOST, HERO_UI_WIN_POINTS_MOST);
        place[1] = hero_ui_between(y, -HERO_UI_WIN_POINTS_MOST, HERO_UI_WIN_POINTS_MOST);
        place[2] = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
        place[3] = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
    } else {
        widget->x = x;
        widget->y = y;
        widget->width = width;
        widget->height = height;
        widget->placed = true;
        if (widget->kind == HERO_UI_SEPARATOR) {
            hero_ui_win_turn_separator(widget);
        }
    }
    hero_ui_win_put(widget);
    return HERO_UI_OK;
}

/* The size a button, a checkbox or a radio item asks, in pixels: the
 * system's ideal (version 6 of the common controls), else its text's with
 * the room around it. */
static SIZE hero_ui_win_button_size(HWND button, bool box, UINT dpi) {
    SIZE ideal = {0, 0};
    SIZE measured = {0, 0};
    wchar_t *text = hero_ui_win_wide_of(button);

    if (text != NULL) {
        measured = hero_ui_win_text_size(button, text, DT_SINGLELINE);
        hero_layer_free(text);
    }
    if (!SendMessageW(button, BCM_GETIDEALSIZE, 0, (LPARAM)&ideal)) {
        ideal.cx = 0;
        ideal.cy = 0;
    }
    if (box) {
        int mark = hero_ui_win_metric(SM_CXMENUCHECK, dpi);

        ideal.cx = ideal.cx > measured.cx + mark + MulDiv(8, (int)dpi, 96) ? ideal.cx : measured.cx + mark + MulDiv(8, (int)dpi, 96);
        ideal.cy = ideal.cy > MulDiv(17, (int)dpi, 96) ? ideal.cy : MulDiv(17, (int)dpi, 96);
        return ideal;
    }
    /* A push button is 75 by 23 pixels at least at 96 dots an inch, the
     * size Windows' own dialogs give their buttons. */
    ideal.cx = ideal.cx > measured.cx + MulDiv(20, (int)dpi, 96) ? ideal.cx : measured.cx + MulDiv(20, (int)dpi, 96);
    ideal.cx = ideal.cx > MulDiv(75, (int)dpi, 96) ? ideal.cx : MulDiv(75, (int)dpi, 96);
    ideal.cy = ideal.cy > MulDiv(23, (int)dpi, 96) ? ideal.cy : MulDiv(23, (int)dpi, 96);
    return ideal;
}

int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;
    UINT dpi;
    SIZE size = {0, 0};

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_win_window(widget->window);
    dpi = window != NULL ? window->dpi : 96u;
    *width = 0;
    *height = 0;
    switch (widget->kind) {
    case HERO_UI_LABEL: {
        wchar_t *text = hero_ui_win_wide_of(widget->hwnd);

        if (text == NULL) {
            return HERO_UI_LIMIT;
        }
        /* An empty label still takes a line's height, as AppKit's does. */
        size = hero_ui_win_text_size(widget->hwnd, text[0] == 0 ? L" " : text, DT_LEFT);
        size.cx = text[0] == 0 ? 0 : size.cx;
        hero_layer_free(text);
        break;
    }
    case HERO_UI_BUTTON:
        size = hero_ui_win_button_size(widget->hwnd, false, dpi);
        break;
    case HERO_UI_CHECKBOX:
        size = hero_ui_win_button_size(widget->hwnd, true, dpi);
        break;
    case HERO_UI_RADIO_GROUP:
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            SIZE item = hero_ui_win_button_size(widget->radios[i], true, dpi);

            size.cx = item.cx > size.cx ? item.cx : size.cx;
            size.cy += item.cy;
        }
        break;
    case HERO_UI_TEXT_FIELD:
        /* The headless side's width, and a line of the font with the edit
         * control's borders: 23 pixels at 96 dots an inch in Windows' own
         * dialogs. */
        *width = 160;
        size.cy = hero_ui_win_line(widget->hwnd) + 2 * hero_ui_win_metric(SM_CYEDGE, dpi) + MulDiv(3, (int)dpi, 96);
        break;
    case HERO_UI_TEXT_AREA:
        *width = 240;
        *height = 96;
        return HERO_UI_OK;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        *width = 160;
        *height = 96;
        return HERO_UI_OK;
    case HERO_UI_SPIN: {
        wchar_t widest[44];
        int digits = 1;
        int64_t longer = widget->most > -widget->least ? widget->most : -widget->least;

        for (int64_t shown = longer; shown >= 10; shown /= 10) {
            digits += 1;
        }
        digits += widget->least < 0 ? 1 : 0;
        digits = digits > 20 ? 20 : digits;
        for (int i = 0; i < digits; i += 1) {
            widest[i] = L'0';
        }
        widest[digits] = 0;
        size = hero_ui_win_text_size(widget->hwnd, widest, DT_SINGLELINE);
        size.cx += hero_ui_win_metric(SM_CXVSCROLL, dpi) + 2 * hero_ui_win_metric(SM_CXEDGE, dpi) + MulDiv(8, (int)dpi, 96);
        size.cy = hero_ui_win_line(widget->hwnd) + 2 * hero_ui_win_metric(SM_CYEDGE, dpi) + MulDiv(3, (int)dpi, 96);
        break;
    }
    case HERO_UI_EDITABLE_COMBO:
    case HERO_UI_COMBO: {
        RECT field;
        int widest = 0;

        for (int64_t i = 0; i < widget->items; i += 1) {
            LRESULT units = SendMessageW(widget->hwnd, CB_GETLBTEXTLEN, (WPARAM)i, 0);
            wchar_t *item = units >= 0 ? hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t)) : NULL;

            if (item == NULL) {
                continue;
            }
            item[0] = 0;
            if (SendMessageW(widget->hwnd, CB_GETLBTEXT, (WPARAM)i, (LPARAM)item) >= 0) {
                SIZE text = hero_ui_win_text_size(widget->hwnd, item, DT_SINGLELINE);

                widest = text.cx > widest ? text.cx : widest;
            }
            hero_layer_free(item);
        }
        size.cx = widest + hero_ui_win_metric(SM_CXVSCROLL, dpi) + 2 * hero_ui_win_metric(SM_CXEDGE, dpi) + MulDiv(12, (int)dpi, 96);
        size.cx = size.cx > MulDiv(80, (int)dpi, 96) ? size.cx : MulDiv(80, (int)dpi, 96);
        size.cy = GetWindowRect(widget->hwnd, &field) ? field.bottom - field.top : MulDiv(23, (int)dpi, 96);
        break;
    }
    case HERO_UI_PROGRESS:
        *width = 160;
        *height = 16;
        return HERO_UI_OK;
    case HERO_UI_SEPARATOR:
        *width = 1;
        *height = 1;
        return HERO_UI_OK;
    case HERO_UI_TOOLBAR:
    case HERO_UI_STATUS_BAR:
        /* Docked across the window, so no width of its own. */
        *height = hero_ui_win_bar_height(widget);
        return HERO_UI_OK;
    case HERO_UI_DATE_PICKER:
    case HERO_UI_TIME_PICKER:
    case HERO_UI_DATE_TIME_PICKER:
        /* The widest text it shows, its arrow and a text field's height. */
        size = hero_ui_win_text_size(widget->hwnd, widget->kind == HERO_UI_DATE_PICKER ? L"00/00/0000" : (widget->kind == HERO_UI_TIME_PICKER ? L"00:00:00" : L"00/00/0000 00:00:00"), DT_SINGLELINE);
        size.cx += hero_ui_win_metric(SM_CXVSCROLL, dpi) + 2 * hero_ui_win_metric(SM_CXEDGE, dpi) + MulDiv(12, (int)dpi, 96);
        size.cy = hero_ui_win_line(widget->hwnd) + 2 * hero_ui_win_metric(SM_CYEDGE, dpi) + MulDiv(3, (int)dpi, 96);
        break;
    case HERO_UI_CALENDAR: {
        /* A month's grid, its title and its today line, as the control
         * measures them in its font. */
        RECT least;

        memset(&least, 0, sizeof least);
        if (SendMessageW(widget->hwnd, MCM_GETMINREQRECT, 0, (LPARAM)&least)) {
            size.cx = least.right - least.left;
            size.cy = least.bottom - least.top;
        }
        break;
    }
    case HERO_UI_COLOR_WELL:
        *width = 44;
        *height = 23;
        return HERO_UI_OK;
    case HERO_UI_FONT_BUTTON:
        size = hero_ui_win_button_size(widget->hwnd, false, dpi);
        break;
    case HERO_UI_SLIDER: {
        LRESULT thumb = SendMessageW(widget->hwnd, TBM_GETTHUMBLENGTH, 0, 0);

        *width = 160;
        size.cy = (int)(thumb > 0 ? thumb : MulDiv(20, (int)dpi, 96)) + MulDiv(10, (int)dpi, 96);
        break;
    }
    case HERO_UI_IMAGE:
        *width = widget->bitmap_width;
        *height = widget->bitmap_height;
        return HERO_UI_OK;
    case HERO_UI_WEBVIEW:
        /* The headless side's size of a web view, where nothing else decides
         * it (`ui/internal/sizes.hero`). */
        *width = 320;
        *height = 240;
        return HERO_UI_OK;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        /* The divider's thickness on both axes; a box's other room the
         * layout finds from what it holds. */
        *width = hero_ui_win_divider(dpi);
        *height = *width;
        return HERO_UI_OK;
    case HERO_UI_TABLE: {
        /* Its columns at their width, and a list's height with its header. */
        HWND header = (HWND)SendMessageW(widget->hwnd, LVM_GETHEADER, 0, 0);
        RECT bar = {0, 0, 0, 0};
        int wide = 0;

        for (int64_t i = 0; i < widget->columns; i += 1) {
            wide += (int)SendMessageW(widget->hwnd, LVM_GETCOLUMNWIDTH, (WPARAM)i, 0);
        }
        if (header != NULL) {
            (void)GetWindowRect(header, &bar);
        }
        *width = hero_ui_win_pt_up(wide + 2 * hero_ui_win_metric(SM_CXEDGE, dpi), dpi);
        *width = *width > 160 ? *width : 160;
        *height = 96 + hero_ui_win_pt_up(bar.bottom - bar.top, dpi);
        return HERO_UI_OK;
    }
    case HERO_UI_TREE:
        *width = 160;
        *height = 96;
        return HERO_UI_OK;
    default:
        return HERO_UI_OK;
    }
    if (*width == 0) {
        *width = hero_ui_win_pt_up(size.cx, dpi);
    }
    *height = hero_ui_win_pt_up(size.cy, dpi);
    return HERO_UI_OK;
}

static uint8_t *hero_ui_win_group_title(const hero_ui_native *group, int64_t *bytes);

int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);
    uint8_t *text;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_TEXT_FIELD && widget->kind != HERO_UI_TEXT_AREA && widget->kind != HERO_UI_GROUP && widget->kind != HERO_UI_EDITABLE_COMBO && widget->kind != HERO_UI_FONT_BUTTON) {
        return HERO_UI_INVALID;
    }
    if (widget->kind == HERO_UI_FONT_BUTTON) {
        /* The descriptor the arm keeps, which the button's own text is not. */
        text = widget->label != NULL ? hero_ui_win_narrow(widget->label, (int)wcslen(widget->label), false, length) : NULL;
    } else {
        text = widget->kind == HERO_UI_GROUP ? hero_ui_win_group_title(widget, length) : hero_ui_win_text_of(widget->hwnd, widget->kind == HERO_UI_TEXT_AREA, length);
    }
    if (text == NULL) {
        return HERO_UI_LIMIT;
    }
    if (*length > 0 && *length <= count) {
        memcpy(buffer, text, (size_t)*length);
    }
    hero_layer_free(text);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    hero_ui_native *widget = hero_ui_find(id);
    LRESULT chosen;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        *value = SendMessageW(widget->hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0;
        return HERO_UI_OK;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
    case HERO_UI_SPIN:
    case HERO_UI_DATE_PICKER:
    case HERO_UI_TIME_PICKER:
    case HERO_UI_DATE_TIME_PICKER:
    case HERO_UI_CALENDAR:
    case HERO_UI_COLOR_WELL:
        *value = widget->value;
        return HERO_UI_OK;
    case HERO_UI_COMBO:
        chosen = SendMessageW(widget->hwnd, CB_GETCURSEL, 0, 0);
        *value = chosen == CB_ERR ? -1 : (int64_t)chosen;
        return HERO_UI_OK;
    case HERO_UI_RADIO_GROUP:
        *value = hero_ui_win_checked_radio(widget);
        return HERO_UI_OK;
    case HERO_UI_MENU_ITEM:
        if (widget->check == HERO_UI_CHECK_NONE) {
            return HERO_UI_INVALID;
        }
        *value = widget->checked ? 1 : 0;
        return HERO_UI_OK;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        *value = hero_ui_win_divider_at(widget);
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
    switch (widget->kind) {
    case HERO_UI_LIST_MULTIPLE: {
        LRESULT selected = SendMessageW(widget->hwnd, LB_GETSELCOUNT, 0, 0);
        int *read;
        LRESULT got;

        if (selected <= 0) {
            *chosen = 0;
            return HERO_UI_OK;
        }
        read = hero_layer_alloc((size_t)selected * sizeof *read);
        if (read == NULL) {
            return HERO_UI_LIMIT;
        }
        got = SendMessageW(widget->hwnd, LB_GETSELITEMS, (WPARAM)selected, (LPARAM)read);
        got = got < 0 ? 0 : got;
        /* Win32 does not promise their order: ascending, as the header does. */
        for (LRESULT i = 1; i < got; i += 1) {
            int held = read[i];
            LRESULT j = i;

            while (j > 0 && read[j - 1] > held) {
                read[j] = read[j - 1];
                j -= 1;
            }
            read[j] = held;
        }
        *chosen = (int64_t)got;
        if (got <= count) {
            for (LRESULT i = 0; i < got; i += 1) {
                indices[i] = read[i];
            }
        }
        hero_layer_free(read);
        return HERO_UI_OK;
    }
    case HERO_UI_LIST:
        one = (int64_t)SendMessageW(widget->hwnd, LB_GETCURSEL, 0, 0);
        break;
    case HERO_UI_COMBO:
        one = (int64_t)SendMessageW(widget->hwnd, CB_GETCURSEL, 0, 0);
        break;
    case HERO_UI_EDITABLE_COMBO:
        one = hero_ui_win_combo_match(widget);
        break;
    case HERO_UI_RADIO_GROUP:
        one = hero_ui_win_checked_radio(widget);
        break;
    case HERO_UI_TABS:
        one = (int64_t)SendMessageW(widget->hwnd, TCM_GETCURSEL, 0, 0);
        break;
    case HERO_UI_TREE:
        one = hero_ui_win_item_chosen(widget);
        one = one > 0 ? one : -1;
        break;
    case HERO_UI_TABLE: {
        int64_t *rows = NULL;
        int64_t got = 0;

        if (!hero_ui_win_rows_chosen(widget, &rows, &got)) {
            return HERO_UI_LIMIT;
        }
        *chosen = got;
        if (got <= count && got > 0) {
            memcpy(indices, rows, (size_t)got * sizeof *rows);
        }
        hero_layer_free(rows);
        return HERO_UI_OK;
    }
    default:
        return HERO_UI_INVALID;
    }
    *chosen = one >= 0 ? 1 : 0;
    if (one >= 0 && count >= 1) {
        indices[0] = one;
    }
    return HERO_UI_OK;
}

static void hero_ui_win_settle_tabs(hero_ui_native *window);

/* Whether `message` was a window's shortcut or a key the dialog manager
 * moves between controls by, and so is handled. */
static bool hero_ui_win_translated(MSG *message) {
    HWND root = message->hwnd != NULL ? GetAncestor(message->hwnd, GA_ROOT) : NULL;
    hero_ui_native *window = root != NULL ? hero_ui_win_of(root) : NULL;

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return false;
    }
    if (message->message >= WM_KEYFIRST && message->message <= WM_KEYLAST) {
        if (message->message == WM_KEYDOWN && message->wParam == VK_TAB) {
            hero_ui_win_settle_tabs(window);
        }
        if (window->accelerators_stale) {
            hero_ui_win_make_accelerators(window);
        }
        if (window->accelerators != NULL && TranslateAcceleratorW(root, window->accelerators, message)) {
            return true;
        }
    }
    return IsDialogMessageW(root, message) != FALSE;
}

int64_t hero_ui_arm_pump(int64_t ms) {
    uint64_t before = hero_ui_pushed_count();
    ULONGLONG deadline = GetTickCount64() + (ULONGLONG)hero_ui_between(ms, 0, INT32_MAX);
    MSG message;

    for (;;) {
        ULONGLONG now;

        while (hero_ui_pushed_count() == before && PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                hero_ui_win_quit();
                continue;
            }
            if (hero_ui_win_translated(&message)) {
                continue;
            }
            (void)TranslateMessage(&message);
            (void)DispatchMessageW(&message);
        }
        now = GetTickCount64();
        if (hero_ui_pushed_count() != before || now >= deadline) {
            return HERO_UI_OK;
        }
        hero_ui_win_watch_wait((DWORD)(deadline - now));
    }
}

int64_t hero_ui_arm_watch_fd(int64_t fd, int64_t id) {
    return hero_ui_win_watch_arm(fd, id);
}

void hero_ui_arm_unwatch_fd(int64_t id) {
    hero_ui_win_watch_end(id);
}

static int64_t hero_ui_win_table_inject(hero_ui_native *table, int64_t kind, int64_t value, int64_t second);
static int64_t hero_ui_win_tree_inject(hero_ui_native *tree, int64_t kind, int64_t value, int64_t second);
static hero_ui_native *hero_ui_win_key_button(int64_t id);
static int64_t hero_ui_win_choose_tab(hero_ui_native *tabs, int64_t index);
static int64_t hero_ui_win_drag_to(hero_ui_native *split, int64_t value);

/* Presses button `button`, a push button's, a checkbox's or a radio item's,
 * by `BM_CLICK`, and where Win32 dropped the press sends the `BN_CLICKED`
 * the control would have (WHAT `inject` DOES). */
static void hero_ui_win_press(HWND button, bool toggles) {
    uint64_t before = hero_ui_pushed_count();
    LRESULT state = SendMessageW(button, BM_GETCHECK, 0, 0);

    if (!IsWindowEnabled(button)) {
        return;
    }
    (void)SendMessageW(button, BM_CLICK, 0, 0);
    if (hero_ui_pushed_count() != before) {
        return;
    }
    if (toggles && SendMessageW(button, BM_GETCHECK, 0, 0) == state) {
        (void)SendMessageW(button, BM_SETCHECK, state == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED, 0);
    }
    (void)SendMessageW(GetParent(button), WM_COMMAND, MAKEWPARAM(0, BN_CLICKED), (LPARAM)button);
}

/* Opens row `row` of list `widget` as the person's double click does: the
 * row chosen first where it is not, its choice told by the list's own
 * notification, then the row told opened. */
static int64_t hero_ui_win_open_row(hero_ui_native *widget, int64_t row) {
    bool chosen;

    if (widget->kind != HERO_UI_LIST && widget->kind != HERO_UI_LIST_MULTIPLE) {
        return HERO_UI_INVALID;
    }
    if (row >= widget->items) {
        return HERO_UI_INVALID;
    }
    if (!IsWindowEnabled(widget->hwnd)) {
        return HERO_UI_OK;
    }
    chosen = widget->kind == HERO_UI_LIST ? SendMessageW(widget->hwnd, LB_GETCURSEL, 0, 0) == (LRESULT)row : SendMessageW(widget->hwnd, LB_GETSEL, (WPARAM)row, 0) > 0;
    if (!chosen) {
        if (widget->kind == HERO_UI_LIST) {
            (void)SendMessageW(widget->hwnd, LB_SETCURSEL, (WPARAM)row, 0);
        } else {
            (void)SendMessageW(widget->hwnd, LB_SETSEL, FALSE, -1);
            (void)SendMessageW(widget->hwnd, LB_SETSEL, TRUE, (LPARAM)row);
        }
        (void)SendMessageW(GetParent(widget->hwnd), WM_COMMAND, MAKEWPARAM(0, LBN_SELCHANGE), (LPARAM)widget->hwnd);
    }
    (void)SendMessageW(widget->hwnd, LB_SETCARETINDEX, (WPARAM)row, FALSE);
    hero_ui_tell(HERO_UI_EVENT_ACTIVATE, widget->id, row, 0);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t answer = HERO_UI_OK;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_TABLE || widget->kind == HERO_UI_TREE) {
        hero_ui_win_injecting += 1;
        answer = widget->kind == HERO_UI_TABLE ? hero_ui_win_table_inject(widget, kind, value, second) : hero_ui_win_tree_inject(widget, kind, value, second);
        hero_ui_win_injecting -= 1;
        return answer;
    }
    hero_ui_win_injecting += 1;
    switch (kind) {
    case HERO_UI_EVENT_ACTIVATE:
        answer = hero_ui_win_open_row(widget, value);
        break;
    case HERO_UI_EVENT_SUBMIT:
        if (widget->kind != HERO_UI_TEXT_FIELD) {
            answer = HERO_UI_INVALID;
        } else if (IsWindowEnabled(widget->hwnd)) {
            /* The window's button for Return takes it first, as the
             * person's Return reaches it. */
            hero_ui_native *window = hero_ui_win_window(widget->window);
            hero_ui_native *button = window != NULL ? hero_ui_win_key_button(window->enter_button) : NULL;

            if (button != NULL) {
                hero_ui_win_press(button->hwnd, false);
            } else {
                hero_ui_tell(HERO_UI_EVENT_SUBMIT, widget->id, 0, 0);
            }
        }
        break;
    case HERO_UI_EVENT_CLICK:
    case HERO_UI_EVENT_TOGGLE:
        if (widget->kind == HERO_UI_BUTTON || widget->kind == HERO_UI_CHECKBOX) {
            hero_ui_win_press(widget->hwnd, widget->kind == HERO_UI_CHECKBOX);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_TEXT:
        if (widget->kind == HERO_UI_EDITABLE_COMBO) {
            wchar_t *wide = hero_ui_win_wide(text, count, 0);

            if (wide == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (IsWindowEnabled(widget->hwnd)) {
                /* The text typed, told once as the person's typing is. */
                hero_ui_mute(1);
                (void)SetWindowTextW(widget->hwnd, wide);
                widget->value = (int64_t)wcslen(wide);
                hero_ui_mute(-1);
                (void)SendMessageW(GetParent(widget->hwnd), WM_COMMAND, MAKEWPARAM(0, CBN_EDITCHANGE), (LPARAM)widget->hwnd);
            }
            hero_layer_free(wide);
        } else if (widget->kind == HERO_UI_FONT_BUTTON) {
            /* The font as the person chose it, told once where it changes. */
            wchar_t *wide = hero_ui_win_wide(text, count, 0);
            bool changed = false;

            if (wide == NULL) {
                answer = HERO_UI_LIMIT;
            } else {
                answer = hero_ui_win_font_apply(widget, wide, IsWindowEnabled(widget->hwnd) != FALSE, &changed);
                if (answer == HERO_UI_OK && changed) {
                    hero_ui_tell(HERO_UI_EVENT_TEXT, widget->id, 0, 0);
                }
            }
            hero_layer_free(wide);
        } else if (widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA) {
            wchar_t *wide = hero_ui_win_wide(text, count, widget->kind == HERO_UI_TEXT_AREA ? HERO_UI_WIN_LINES : 0);

            if (wide == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (IsWindowEnabled(widget->hwnd)) {
                (void)SendMessageW(widget->hwnd, EM_SETSEL, 0, -1);
                (void)SendMessageW(widget->hwnd, EM_REPLACESEL, TRUE, (LPARAM)wide);
                /* However many notifications the control sent for it, an
                 * injected edit is one edit. */
                hero_ui_win_edit_changed(widget, false);
            }
            hero_layer_free(wide);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_SELECT:
        if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
            if (value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (IsWindowEnabled(widget->hwnd)) {
                if (widget->kind == HERO_UI_LIST) {
                    (void)SendMessageW(widget->hwnd, LB_SETCURSEL, value < 0 ? (WPARAM)-1 : (WPARAM)value, 0);
                } else {
                    if (value < 0 || second != 1) {
                        (void)SendMessageW(widget->hwnd, LB_SETSEL, FALSE, -1);
                    }
                    if (value >= 0) {
                        (void)SendMessageW(widget->hwnd, LB_SETSEL, TRUE, (LPARAM)value);
                    }
                }
                (void)SendMessageW(GetParent(widget->hwnd), WM_COMMAND, MAKEWPARAM(0, LBN_SELCHANGE), (LPARAM)widget->hwnd);
            }
        } else if (widget->kind == HERO_UI_COMBO || widget->kind == HERO_UI_EDITABLE_COMBO) {
            if (value < 0 || value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (IsWindowEnabled(widget->hwnd)) {
                hero_ui_mute(1);
                (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)value, 0);
                hero_ui_mute(-1);
                (void)SendMessageW(GetParent(widget->hwnd), WM_COMMAND, MAKEWPARAM(0, CBN_SELCHANGE), (LPARAM)widget->hwnd);
            }
        } else if (widget->kind == HERO_UI_RADIO_GROUP) {
            if (value < 0 || value >= widget->radio_count) {
                answer = HERO_UI_INVALID;
            } else {
                hero_ui_win_press(widget->radios[value], false);
            }
        } else if (widget->kind == HERO_UI_TABS) {
            answer = hero_ui_win_choose_tab(widget, value);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_VALUE:
        if (widget->kind == HERO_UI_SPLIT_ROW || widget->kind == HERO_UI_SPLIT_COLUMN) {
            answer = hero_ui_win_drag_to(widget, value);
        } else if (widget->kind == HERO_UI_SPIN) {
            if (value < widget->least || value > widget->most) {
                answer = HERO_UI_INVALID;
            } else if (IsWindowEnabled(widget->hwnd) && value != widget->value) {
                widget->value = value;
                hero_ui_win_spin_show(widget);
                hero_ui_tell(HERO_UI_EVENT_VALUE, widget->id, value, 0);
            }
        } else if (hero_ui_win_stamp_kind(widget->kind)) {
            answer = hero_ui_win_stamp_set(widget, value, true);
        } else if (widget->kind == HERO_UI_COLOR_WELL) {
            answer = hero_ui_win_well_set(widget, value, true);
        } else if (widget->kind != HERO_UI_SLIDER) {
            answer = HERO_UI_INVALID;
        } else if (IsWindowEnabled(widget->hwnd)) {
            int position = hero_ui_win_position(widget, value);

            (void)SendMessageW(widget->hwnd, TBM_SETPOS, TRUE, (LPARAM)position);
            (void)SendMessageW(GetParent(widget->hwnd), WM_HSCROLL, MAKEWPARAM(TB_THUMBPOSITION, (WORD)position), (LPARAM)widget->hwnd);
        }
        break;
    case HERO_UI_EVENT_MENU:
        if (widget->kind != HERO_UI_MENU_ITEM) {
            answer = HERO_UI_INVALID;
        } else if (hero_ui_win_usable(widget)) {
            /* A window's or a tray's, where its menu sends its command. */
            hero_ui_native *window = hero_ui_win_top(widget->window);

            if (window != NULL) {
                (void)SendMessageW(window->hwnd, WM_COMMAND, MAKEWPARAM(widget->command, 0), 0);
            }
        }
        break;
    case HERO_UI_EVENT_CLOSE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else {
            (void)SendMessageW(widget->hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        }
        break;
    case HERO_UI_EVENT_MESSAGE:
        /* The page sends `text` by its own bridge, so the message comes as
         * the page's would. */
        answer = widget->kind == HERO_UI_WEBVIEW ? hero_ui_win_web(hero_ui_web_win32_send(id, text, count)) : HERO_UI_INVALID;
        break;
    case HERO_UI_EVENT_RESIZE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else if (widget->resizable && !widget->full_screen) {
            int width = hero_ui_win_px(hero_ui_between(value, widget->least_width, HERO_UI_WIN_POINTS_MOST), widget->dpi);
            int height = hero_ui_win_px(hero_ui_between(second, widget->least_height, HERO_UI_WIN_POINTS_MOST), widget->dpi);
            RECT frame = {0, 0, width, height};

            hero_ui_win_frame_of(widget, &frame);
            (void)SetWindowPos(widget->hwnd, NULL, 0, 0, frame.right - frame.left, frame.bottom - frame.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    hero_ui_win_injecting -= 1;
    return answer;
}

/* Keeps the UTF-16 path `wide` of `units` units, followed by one zero byte,
 * at the end of the paths answered; one that is no UTF-16 (an unpaired
 * surrogate) is passed over, since no UTF-8 names it. */
static bool hero_ui_win_keep_path(const wchar_t *wide, int units, uint8_t **paths, int64_t *count, int64_t *room) {
    int bytes = units > 0 ? WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, units, NULL, 0, NULL, NULL) : 0;

    if (bytes <= 0) {
        return true;
    }
    if (*count + bytes + 1 > *room) {
        int64_t grown_room = (*room + bytes + 1) * 2;
        uint8_t *grown = hero_layer_grow(*paths, (size_t)grown_room);

        if (grown == NULL) {
            return false;
        }
        *paths = grown;
        *room = grown_room;
    }
    (void)WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, units, (char *)*paths + *count, bytes, NULL, NULL);
    *count += bytes;
    (*paths)[*count] = 0;
    *count += 1;
    return true;
}

int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return widget->kind == HERO_UI_WEBVIEW ? hero_ui_win_web(hero_ui_web_win32_load(id, url, count)) : HERO_UI_INVALID;
}

int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return widget->kind == HERO_UI_WEBVIEW ? hero_ui_win_web(hero_ui_web_win32_post(id, text, count)) : HERO_UI_INVALID;
}

/* -- THE KINDS AND ACTS THIS ARM DRAWS -------------------------------------
 *
 * The kinds of `hero_ui.h` up to the context menu, and the tray, a
 * separator, a group box, an editable combo box, a spin, a toolbar, a status
 * bar, the three pickers of a date and a time, a colour well and a font
 * button, so a kind the header gains later is told `hero_ui_elsewhere`'s words
 * until this arm draws it, never `INVALID` by a `create` that knows none of
 * it; and no context menu on a web view, whose menu is WebView2's own,
 * which this arm does not replace. */
bool hero_ui_arm_makes(int64_t kind, int64_t parent) {
    const hero_ui_native *owner = hero_ui_find(parent);

    if (kind > HERO_UI_CONTEXT_MENU && kind != HERO_UI_TRAY && kind != HERO_UI_SEPARATOR && kind != HERO_UI_GROUP && kind != HERO_UI_EDITABLE_COMBO && kind != HERO_UI_SPIN && kind != HERO_UI_TOOLBAR && kind != HERO_UI_STATUS_BAR && !hero_ui_win_stamp_kind(kind) && kind != HERO_UI_COLOR_WELL && kind != HERO_UI_FONT_BUTTON && kind != HERO_UI_CANVAS) {
        return false;
    }
    return kind != HERO_UI_CONTEXT_MENU || owner == NULL || owner->kind != HERO_UI_WEBVIEW;
}

bool hero_ui_arm_acts(int64_t act) {
    (void)act;
    return true;
}

/* Every `HERO_UI_OTHER` and `HERO_UI_UNSUPPORTED` of this arm is said
 * through `hero_ui_refused`, so the words kept are the last refusal's own. */
int64_t hero_ui_arm_windows_words(char *words, int64_t count) {
    return hero_ui_arm_words(words, count);
}

/* -- WINDOWS ON THE SCREEN -------------------------------------------------
 *
 * A screen is a monitor, `EnumDisplayMonitors`' order with the primary one
 * first. Windows gives a thread aware of each monitor's scale every place
 * in physical pixels, so a monitor's area and work area are given in points
 * at its own scale (`GetDpiForMonitor`), its origin at the primary's scale,
 * so that screens side by side stay side by side in the primary's points.
 * A window's frame is `GetWindowRect`'s, as wxWidgets' `GetPosition` reads
 * it, the border Windows keeps around a frame for resizing included: one
 * rectangle a window has shown or hidden, so a place set and read back is
 * the place set. Its position is from its screen's top-left corner at that
 * screen's scale, its size at the window's own; a minimized window's frame
 * is the one it is restored to, and a full screen's the monitor's.
 *
 * WHAT THE PERSON DOES is read from the window's own messages once it is
 * watched (`watch`, at every show): `WM_MOVE` tells a new position, a new
 * monitor before it, `WM_SIZE` a new state, `WM_DISPLAYCHANGE` and
 * `WM_DPICHANGED` a monitor that changed; each only where it differs from
 * the one last told, and none while the program's own call runs (`quiet`).
 * A minimized window's move to Windows' parking place is no move. */

/* The monitors, the primary one first; `failed` where memory ran out. */
typedef struct hero_ui_win_monitors {
    HMONITOR *at;
    int64_t count;
    int64_t room;
    bool failed;
} hero_ui_win_monitors;

static BOOL CALLBACK hero_ui_win_monitor_found(HMONITOR monitor, HDC dc, LPRECT area, LPARAM data) {
    hero_ui_win_monitors *list = (hero_ui_win_monitors *)data;
    MONITORINFO info;

    (void)dc;
    (void)area;
    if (list->count == list->room) {
        int64_t room = list->room == 0 ? 8 : list->room * 2;
        HMONITOR *grown = hero_layer_grow(list->at, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            list->failed = true;
            return FALSE;
        }
        list->at = grown;
        list->room = room;
    }
    memset(&info, 0, sizeof info);
    info.cbSize = (DWORD)sizeof info;
    if (GetMonitorInfoW(monitor, &info) && (info.dwFlags & MONITORINFOF_PRIMARY) != 0) {
        memmove(list->at + 1, list->at, (size_t)list->count * sizeof *list->at);
        list->at[0] = monitor;
    } else {
        list->at[list->count] = monitor;
    }
    list->count += 1;
    return TRUE;
}

/* Lists the monitors into `list`, which the caller frees; false where
 * Windows or memory refused. */
static bool hero_ui_win_list_monitors(hero_ui_win_monitors *list) {
    memset(list, 0, sizeof *list);
    if (!EnumDisplayMonitors(NULL, NULL, hero_ui_win_monitor_found, (LPARAM)list) || list->failed || list->count == 0) {
        hero_layer_free(list->at);
        memset(list, 0, sizeof *list);
        return false;
    }
    return true;
}

/* Monitor `monitor`'s scale, its dots an inch; the system's where this
 * Windows names no monitor's own. */
static UINT hero_ui_win_monitor_scale(HMONITOR monitor) {
    UINT x = 0;
    UINT y = 0;

    if (hero_ui_win_dpi_for_monitor != NULL && hero_ui_win_dpi_for_monitor(monitor, HERO_UI_WIN_EFFECTIVE_DPI, &x, &y) == S_OK && x > 0) {
        return x;
    }
    return hero_ui_win_dpi(NULL);
}

/* Monitor `monitor`'s whole area and work area, in pixels. */
static bool hero_ui_win_monitor_areas(HMONITOR monitor, RECT *whole, RECT *work) {
    MONITORINFO info;

    memset(&info, 0, sizeof info);
    info.cbSize = (DWORD)sizeof info;
    if (!GetMonitorInfoW(monitor, &info)) {
        return false;
    }
    *whole = info.rcMonitor;
    *work = info.rcWork;
    return true;
}

/* The index of `monitor` among `list`'s, 0 where it is not among them. */
static int64_t hero_ui_win_screen_index(const hero_ui_win_monitors *list, HMONITOR monitor) {
    for (int64_t i = 0; i < list->count; i += 1) {
        if (list->at[i] == monitor) {
            return i;
        }
    }
    return 0;
}

/* The frame window `window` has or is restored to, in pixels: a minimized
 * or maximized window's restored frame through its placement, whose
 * workspace coordinates are moved by its monitor's task bar to the screen's. */
static bool hero_ui_win_frame_rect(const hero_ui_native *window, RECT *frame) {
    WINDOWPLACEMENT placement;

    if (window->full_screen || !IsIconic(window->hwnd)) {
        return GetWindowRect(window->hwnd, frame) != FALSE;
    }
    memset(&placement, 0, sizeof placement);
    placement.length = (UINT)sizeof placement;
    if (!GetWindowPlacement(window->hwnd, &placement)) {
        return false;
    }
    *frame = placement.rcNormalPosition;
    if ((GetWindowLongPtrW(window->hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0) {
        RECT whole;
        RECT work;

        if (hero_ui_win_monitor_areas(MonitorFromRect(frame, MONITOR_DEFAULTTONEAREST), &whole, &work)) {
            OffsetRect(frame, work.left - whole.left, work.top - whole.top);
        }
    }
    return true;
}

/* The monitor window `window` stands on most, its frame's as it is restored
 * where it is minimized. */
static HMONITOR hero_ui_win_monitor_of(const hero_ui_native *window) {
    RECT frame;

    if (IsIconic(window->hwnd) && hero_ui_win_frame_rect(window, &frame)) {
        return MonitorFromRect(&frame, MONITOR_DEFAULTTONEAREST);
    }
    return MonitorFromWindow(window->hwnd, MONITOR_DEFAULTTONEAREST);
}

/* Window `window`'s screen and its frame's position on it, in points. */
static bool hero_ui_win_place_of(const hero_ui_native *window, int64_t *screen, int64_t *x, int64_t *y) {
    hero_ui_win_monitors list;
    HMONITOR monitor = hero_ui_win_monitor_of(window);
    RECT frame;
    RECT whole;
    RECT work;
    UINT scale;

    if (!hero_ui_win_frame_rect(window, &frame) || !hero_ui_win_monitor_areas(monitor, &whole, &work)) {
        return false;
    }
    scale = hero_ui_win_monitor_scale(monitor);
    *screen = 0;
    if (hero_ui_win_list_monitors(&list)) {
        *screen = hero_ui_win_screen_index(&list, monitor);
        hero_layer_free(list.at);
    }
    *x = hero_ui_win_pt(frame.left - whole.left, scale);
    *y = hero_ui_win_pt(frame.top - whole.top, scale);
    return true;
}

/* The state window `window` stands in now, `HERO_UI_STATE_*`. */
static int64_t hero_ui_win_state_now(const hero_ui_native *window) {
    if (IsIconic(window->hwnd)) {
        return HERO_UI_STATE_MINIMIZED;
    }
    if (window->full_screen) {
        return HERO_UI_STATE_FULL_SCREEN;
    }
    return IsZoomed(window->hwnd) ? HERO_UI_STATE_MAXIMIZED : HERO_UI_STATE_NORMAL;
}

/* Notes where window `window` stands as told, so the person's next act is
 * told where it differs from this. */
static void hero_ui_win_note_place(hero_ui_native *window) {
    int64_t screen = 0;
    int64_t x = 0;
    int64_t y = 0;

    window->state_told = hero_ui_win_state_now(window);
    if (hero_ui_win_place_of(window, &screen, &x, &y)) {
        window->screen_told = screen;
        if (!IsIconic(window->hwnd)) {
            window->moved_x = x;
            window->moved_y = y;
        }
    }
}

/* Whether what window `hwnd` is told now is the person's to tell: a window
 * watched, no program's call under way. */
static hero_ui_native *hero_ui_win_watched(HWND hwnd) {
    hero_ui_native *window = hero_ui_win_of(hwnd);

    if (window == NULL || window->kind != HERO_UI_WINDOW || !window->watched || hero_ui_win_quiet > 0 || hero_ui_is_muted()) {
        return NULL;
    }
    return window;
}

/* Tells window `hwnd`'s screen where it changed, then its position where
 * `moved` and it changed (`WM_MOVE`), each against the one last told. */
static void hero_ui_win_moved(HWND hwnd, bool moved) {
    hero_ui_native *window = hero_ui_win_watched(hwnd);
    int64_t screen = 0;
    int64_t x = 0;
    int64_t y = 0;

    if (window == NULL || IsIconic(hwnd) || !hero_ui_win_place_of(window, &screen, &x, &y)) {
        return;
    }
    if (screen != window->screen_told) {
        window->screen_told = screen;
        hero_ui_tell(HERO_UI_EVENT_SCREEN, window->id, screen, 0);
    }
    if (moved && (x != window->moved_x || y != window->moved_y)) {
        window->moved_x = x;
        window->moved_y = y;
        hero_ui_tell(HERO_UI_EVENT_MOVE, window->id, x, y);
    }
}

/* Tells window `hwnd`'s state where it changed (`WM_SIZE`). */
static void hero_ui_win_state_changed(HWND hwnd) {
    hero_ui_native *window = hero_ui_win_watched(hwnd);
    int64_t now;

    if (window == NULL) {
        return;
    }
    now = hero_ui_win_state_now(window);
    if (now != window->state_told) {
        window->state_told = now;
        hero_ui_tell(HERO_UI_EVENT_STATE, window->id, now, 0);
    }
}

int64_t hero_ui_arm_watch(int64_t id) {
    hero_ui_native *window = hero_ui_win_window(id);

    if (window != NULL && !window->watched) {
        hero_ui_win_note_place(window);
        window->watched = true;
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_screens(int64_t *values, int64_t count, int64_t *screens) {
    hero_ui_win_monitors list;
    RECT primary;
    RECT unused;
    UINT primary_scale;

    if (!hero_ui_win_list_monitors(&list)) {
        return hero_ui_win_failed("listing the monitors", GetLastError());
    }
    primary_scale = hero_ui_win_monitor_scale(list.at[0]);
    if (!hero_ui_win_monitor_areas(list.at[0], &primary, &unused)) {
        hero_layer_free(list.at);
        return hero_ui_win_failed("reading the primary monitor", GetLastError());
    }
    *screens = list.count;
    if (list.count * HERO_UI_SCREEN_FIELDS <= count) {
        for (int64_t i = 0; i < list.count; i += 1) {
            int64_t *at = values + i * HERO_UI_SCREEN_FIELDS;
            UINT scale = hero_ui_win_monitor_scale(list.at[i]);
            RECT whole;
            RECT work;

            if (!hero_ui_win_monitor_areas(list.at[i], &whole, &work)) {
                hero_layer_free(list.at);
                return hero_ui_win_failed("reading a monitor", GetLastError());
            }
            at[HERO_UI_SCREEN_LEFT] = hero_ui_win_pt(whole.left - primary.left, primary_scale);
            at[HERO_UI_SCREEN_TOP] = hero_ui_win_pt(whole.top - primary.top, primary_scale);
            at[HERO_UI_SCREEN_WIDTH] = hero_ui_win_pt(whole.right - whole.left, scale);
            at[HERO_UI_SCREEN_HEIGHT] = hero_ui_win_pt(whole.bottom - whole.top, scale);
            at[HERO_UI_SCREEN_WORK_LEFT] = at[HERO_UI_SCREEN_LEFT] + hero_ui_win_pt(work.left - whole.left, scale);
            at[HERO_UI_SCREEN_WORK_TOP] = at[HERO_UI_SCREEN_TOP] + hero_ui_win_pt(work.top - whole.top, scale);
            at[HERO_UI_SCREEN_WORK_WIDTH] = hero_ui_win_pt(work.right - work.left, scale);
            at[HERO_UI_SCREEN_WORK_HEIGHT] = hero_ui_win_pt(work.bottom - work.top, scale);
            at[HERO_UI_SCREEN_SCALE] = (int64_t)scale * 100 / 96;
        }
    }
    hero_layer_free(list.at);
    return HERO_UI_OK;
}

/* Window `id` for a call of this part: NOT_FOUND where no widget is `id`,
 * INVALID where it is no window. */
static hero_ui_native *hero_ui_win_window_of(int64_t id, int64_t *kind) {
    hero_ui_native *widget = hero_ui_find(id);

    *kind = widget == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    return widget != NULL && widget->kind == HERO_UI_WINDOW ? widget : NULL;
}

int64_t hero_ui_arm_frame_of(int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);
    RECT frame;

    if (window == NULL) {
        return kind;
    }
    if (!hero_ui_win_frame_rect(window, &frame) || !hero_ui_win_place_of(window, screen, x, y)) {
        return hero_ui_win_failed("reading a window's frame", GetLastError());
    }
    *width = hero_ui_win_pt(frame.right - frame.left, window->dpi);
    *height = hero_ui_win_pt(frame.bottom - frame.top, window->dpi);
    return HERO_UI_OK;
}

/* Puts window `window`'s frame's top-left corner at `left`, `top` pixels:
 * the frame itself where it stands as it is drawn, the frame it is restored
 * to where it is minimized or maximized, and the one it leaves full screen
 * for where it is in full screen. */
static bool hero_ui_win_frame_to(hero_ui_native *window, int left, int top) {
    WINDOWPLACEMENT *kept = &window->placement_before;
    WINDOWPLACEMENT placement;
    RECT frame;

    if (!window->full_screen && !IsIconic(window->hwnd) && !IsZoomed(window->hwnd)) {
        if (!SetWindowPos(window->hwnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE)) {
            return false;
        }
        /* A move onto a monitor of another scale resizes the frame by
         * `WM_DPICHANGED`'s rectangle, which may move its corner: put back. */
        if (GetWindowRect(window->hwnd, &frame) && (frame.left != left || frame.top != top)) {
            (void)SetWindowPos(window->hwnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
        return true;
    }
    if (!window->full_screen) {
        memset(&placement, 0, sizeof placement);
        placement.length = (UINT)sizeof placement;
        if (!GetWindowPlacement(window->hwnd, &placement)) {
            return false;
        }
        kept = &placement;
    }
    frame = kept->rcNormalPosition;
    if ((GetWindowLongPtrW(window->hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0) {
        RECT whole;
        RECT work;
        RECT target = {left, top, left + (frame.right - frame.left), top + (frame.bottom - frame.top)};

        if (hero_ui_win_monitor_areas(MonitorFromRect(&target, MONITOR_DEFAULTTONEAREST), &whole, &work)) {
            left -= work.left - whole.left;
            top -= work.top - whole.top;
        }
    }
    OffsetRect(&kept->rcNormalPosition, left - frame.left, top - frame.top);
    return window->full_screen || SetWindowPlacement(window->hwnd, kept) != FALSE;
}

int64_t hero_ui_arm_set_position(int64_t id, int64_t screen, int64_t x, int64_t y, int64_t person) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);
    hero_ui_win_monitors list;
    RECT whole;
    RECT work;
    UINT scale;
    bool moved;
    DWORD code;

    if (window == NULL) {
        return kind;
    }
    if (!hero_ui_win_list_monitors(&list)) {
        return hero_ui_win_failed("listing the monitors", GetLastError());
    }
    if (screen >= list.count || !hero_ui_win_monitor_areas(list.at[screen], &whole, &work)) {
        hero_layer_free(list.at);
        return screen >= list.count ? HERO_UI_INVALID : hero_ui_win_failed("reading a monitor", GetLastError());
    }
    scale = hero_ui_win_monitor_scale(list.at[screen]);
    hero_layer_free(list.at);
    hero_ui_win_quiet += person == 1 ? 0 : 1;
    moved = hero_ui_win_frame_to(window, whole.left + hero_ui_win_px(x, scale), whole.top + hero_ui_win_px(y, scale));
    code = GetLastError();
    hero_ui_win_quiet -= person == 1 ? 0 : 1;
    if (person != 1) {
        hero_ui_win_note_place(window);
    }
    return moved ? HERO_UI_OK : hero_ui_win_failed("moving a window", code);
}

int64_t hero_ui_arm_center(int64_t id, int64_t over) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);
    hero_ui_native *anchor = NULL;
    RECT frame;
    RECT around;
    RECT whole;
    RECT work;
    int width;
    int height;
    int left;
    int top;
    bool moved;
    DWORD code;

    if (window == NULL) {
        return kind;
    }
    if (over != 0) {
        anchor = hero_ui_win_window_of(over, &kind);
        if (anchor == NULL) {
            return kind;
        }
    }
    if (!hero_ui_win_frame_rect(window, &frame) || (anchor != NULL && !hero_ui_win_frame_rect(anchor, &around))) {
        return hero_ui_win_failed("reading a window's frame", GetLastError());
    }
    if (!hero_ui_win_monitor_areas(anchor != NULL ? MonitorFromRect(&around, MONITOR_DEFAULTTONEAREST) : hero_ui_win_monitor_of(window), &whole, &work)) {
        return hero_ui_win_failed("reading a monitor", GetLastError());
    }
    if (anchor == NULL) {
        around = work;
    }
    width = frame.right - frame.left;
    height = frame.bottom - frame.top;
    left = around.left + ((around.right - around.left) - width) / 2;
    top = around.top + ((around.bottom - around.top) - height) / 2;
    /* Within the work area as far as the frame's size allows, its top-left
     * corner kept on it first. */
    left = left + width > work.right ? work.right - width : left;
    top = top + height > work.bottom ? work.bottom - height : top;
    left = left < work.left ? work.left : left;
    top = top < work.top ? work.top : top;
    hero_ui_win_quiet += 1;
    moved = hero_ui_win_frame_to(window, left, top);
    code = GetLastError();
    hero_ui_win_quiet -= 1;
    hero_ui_win_note_place(window);
    return moved ? HERO_UI_OK : hero_ui_win_failed("centring a window", code);
}

/* Takes window `window` into full screen: its style and placement kept,
 * then no frame and the whole of its monitor. */
static bool hero_ui_win_enter_full(hero_ui_native *window) {
    RECT whole;
    RECT work;
    LONG_PTR style;

    if (!hero_ui_win_monitor_areas(hero_ui_win_monitor_of(window), &whole, &work)) {
        return false;
    }
    memset(&window->placement_before, 0, sizeof window->placement_before);
    window->placement_before.length = (UINT)sizeof window->placement_before;
    if (!GetWindowPlacement(window->hwnd, &window->placement_before)) {
        return false;
    }
    window->style_before = GetWindowLongPtrW(window->hwnd, GWL_STYLE);
    window->extended_before = GetWindowLongPtrW(window->hwnd, GWL_EXSTYLE);
    style = (window->style_before & ~(LONG_PTR)(WS_OVERLAPPEDWINDOW | WS_BORDER | WS_DLGFRAME)) | WS_POPUP;
    window->full_screen = true;
    (void)SetWindowLongPtrW(window->hwnd, GWL_STYLE, style);
    (void)SetWindowLongPtrW(window->hwnd, GWL_EXSTYLE, window->extended_before & ~(LONG_PTR)(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
    return SetWindowPos(window->hwnd, NULL, whole.left, whole.top, whole.right - whole.left, whole.bottom - whole.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) != FALSE;
}

/* Takes window `window` out of full screen, its style and placement given
 * back. */
static bool hero_ui_win_leave_full(hero_ui_native *window) {
    window->full_screen = false;
    (void)SetWindowLongPtrW(window->hwnd, GWL_STYLE, window->style_before);
    (void)SetWindowLongPtrW(window->hwnd, GWL_EXSTYLE, window->extended_before);
    (void)SetWindowPos(window->hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    return SetWindowPlacement(window->hwnd, &window->placement_before) != FALSE;
}

/* Shows window `window` as `program` asks, or, for the person, presses the
 * button of the title bar that does it. */
static void hero_ui_win_show_as(hero_ui_native *window, int program, WPARAM button, bool person) {
    if (person) {
        (void)SendMessageW(window->hwnd, WM_SYSCOMMAND, button, 0);
    } else {
        (void)ShowWindow(window->hwnd, program);
    }
}

/* Brings window `window` to `target`, leaving the state it stands in first:
 * restored from the task bar, out of full screen, out of maximized. */
static int64_t hero_ui_win_bring(hero_ui_native *window, int64_t target, bool person) {
    if (hero_ui_win_state_now(window) == target) {
        return HERO_UI_OK;
    }
    if (IsIconic(window->hwnd) && target != HERO_UI_STATE_MINIMIZED) {
        hero_ui_win_show_as(window, SW_RESTORE, SC_RESTORE, person);
    }
    if (window->full_screen && target != HERO_UI_STATE_FULL_SCREEN && !hero_ui_win_leave_full(window)) {
        return hero_ui_win_failed("taking a window out of full screen", GetLastError());
    }
    if (IsZoomed(window->hwnd) && target != HERO_UI_STATE_MAXIMIZED && target != HERO_UI_STATE_MINIMIZED) {
        hero_ui_win_show_as(window, SW_RESTORE, SC_RESTORE, person);
    }
    switch (target) {
    case HERO_UI_STATE_MINIMIZED:
        if (!IsIconic(window->hwnd)) {
            hero_ui_win_show_as(window, SW_MINIMIZE, SC_MINIMIZE, person);
        }
        break;
    case HERO_UI_STATE_MAXIMIZED:
        if (!IsZoomed(window->hwnd)) {
            hero_ui_win_show_as(window, SW_MAXIMIZE, SC_MAXIMIZE, person);
        }
        break;
    case HERO_UI_STATE_FULL_SCREEN:
        if (!window->full_screen && !hero_ui_win_enter_full(window)) {
            return hero_ui_win_failed("taking a window into full screen", GetLastError());
        }
        break;
    default:
        break;
    }
    if (hero_ui_win_state_now(window) != target) {
        return hero_ui_refused(HERO_UI_OTHER, "Windows did not bring the window to that state");
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_state(int64_t id, int64_t state, int64_t person) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);
    int64_t answer;

    if (window == NULL) {
        return kind;
    }
    if (!IsWindowVisible(window->hwnd)) {
        /* `ShowWindow` would show it, which only the program does. */
        return hero_ui_refused(HERO_UI_OTHER, "the window is hidden, and Windows brings only a window it shows to a state");
    }
    hero_ui_win_quiet += person == 1 ? 0 : 1;
    answer = hero_ui_win_bring(window, state, person == 1);
    hero_ui_win_quiet -= person == 1 ? 0 : 1;
    if (person != 1) {
        hero_ui_win_note_place(window);
    }
    return answer;
}

int64_t hero_ui_arm_state(int64_t id, int64_t *state) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);

    if (window == NULL) {
        return kind;
    }
    *state = hero_ui_win_state_now(window);
    return HERO_UI_OK;
}

/* Gives window `window` its frame, or takes it away, keeping its content's
 * size and its frame's top-left corner: with no frame the window is its
 * content alone, no sizing border either, and a window that may be resized
 * is resized by its content's edges (`hero_ui_win_edge`). */
static int64_t hero_ui_win_set_framed(hero_ui_native *window, bool on) {
    LONG_PTR style = GetWindowLongPtrW(window->hwnd, GWL_STYLE);

    if (window->full_screen) {
        return hero_ui_refused(HERO_UI_OTHER, "a window in full screen has no frame to give or take");
    }
    if (on) {
        style = (style & ~(LONG_PTR)WS_POPUP) | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | (window->resizable ? WS_THICKFRAME | WS_MAXIMIZEBOX : 0);
    } else {
        style = (style & ~(LONG_PTR)(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME)) | WS_POPUP;
    }
    window->unframed = !on;
    /* The style changed is the program's own change, and Windows may resize
     * the window for it at once: measured on Windows Server 2025, a window
     * that had left full screen took a `WM_SIZE` from `SetWindowLongPtrW`
     * itself, which unmuted was told as the person's resize. */
    hero_ui_mute(1);
    (void)SetWindowLongPtrW(window->hwnd, GWL_STYLE, style);
    (void)SetWindowPos(window->hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    hero_ui_mute(-1);
    hero_ui_win_size_content(window);
    return HERO_UI_OK;
}

/* Puts Windows 11's translucent material, Mica, behind window `window`'s
 * widgets, or takes it away: the frame extended over the whole client
 * area, which this arm then paints black, DWM's transparent. */
static int64_t hero_ui_win_set_backdrop(hero_ui_native *window, int64_t backdrop) {
    int type = backdrop == HERO_UI_BACKDROP_TRANSLUCENT ? HERO_UI_WIN_BACKDROP_MICA : 1;
    MARGINS margins = {0, 0, 0, 0};
    HRESULT done = DwmSetWindowAttribute(window->hwnd, HERO_UI_WIN_BACKDROP_TYPE, &type, (DWORD)sizeof type);

    if (FAILED(done)) {
        if (backdrop == HERO_UI_BACKDROP_NONE) {
            window->backdrop = HERO_UI_BACKDROP_NONE;
            return HERO_UI_OK;
        }
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "this Windows has no translucent material for a window (DWM's system backdrop came with Windows 11 22H2)");
    }
    if (backdrop == HERO_UI_BACKDROP_TRANSLUCENT) {
        margins.cxLeftWidth = -1;
        margins.cxRightWidth = -1;
        margins.cyTopHeight = -1;
        margins.cyBottomHeight = -1;
    }
    (void)DwmExtendFrameIntoClientArea(window->hwnd, &margins);
    window->backdrop = backdrop;
    (void)InvalidateRect(window->hwnd, NULL, TRUE);
    return HERO_UI_OK;
}

/* Every window of the app, but `except`, enabled, `on`, or disabled: a
 * window made modal holds the others so, and gives back only the ones it
 * held. */
static void hero_ui_win_hold_others(int64_t except, bool hold) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *window = hero_ui_at(at);

        if (window == NULL || window->kind != HERO_UI_WINDOW || window->id == except) {
            continue;
        }
        if (hold && IsWindowEnabled(window->hwnd)) {
            window->held_by_modal = true;
            (void)EnableWindow(window->hwnd, FALSE);
        } else if (!hold && window->held_by_modal) {
            window->held_by_modal = false;
            (void)EnableWindow(window->hwnd, TRUE);
        }
    }
}

/* Makes window `window` modal, or ends its modality. */
static int64_t hero_ui_win_set_modal(hero_ui_native *window, bool on) {
    if (on) {
        if (hero_ui_win_modal != 0 && hero_ui_win_modal != window->id) {
            return hero_ui_refused(HERO_UI_OTHER, "another window is modal already, and one is at a time");
        }
        hero_ui_win_modal = window->id;
        hero_ui_win_hold_others(window->id, true);
        (void)EnableWindow(window->hwnd, TRUE);
        if (IsWindowVisible(window->hwnd)) {
            (void)SetForegroundWindow(window->hwnd);
        }
        return HERO_UI_OK;
    }
    if (hero_ui_win_modal == window->id) {
        hero_ui_win_modal = 0;
        hero_ui_win_hold_others(window->id, false);
    }
    return HERO_UI_OK;
}

int64_t hero_ui_arm_window_set(int64_t id, int64_t which, int64_t value) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);
    LONG_PTR extended;

    if (window == NULL) {
        return kind;
    }
    switch (which) {
    case HERO_UI_WINDOW_SET_ON_TOP:
        if (!SetWindowPos(window->hwnd, value == 1 ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE)) {
            return hero_ui_win_failed("keeping a window on top", GetLastError());
        }
        return HERO_UI_OK;
    case HERO_UI_WINDOW_SET_FRAMED: {
        int64_t answer;

        hero_ui_win_quiet += 1;
        answer = hero_ui_win_set_framed(window, value == 1);
        hero_ui_win_quiet -= 1;
        hero_ui_win_note_place(window);
        return answer;
    }
    case HERO_UI_WINDOW_SET_OPACITY:
        extended = GetWindowLongPtrW(window->hwnd, GWL_EXSTYLE);
        if (value == 255) {
            (void)SetWindowLongPtrW(window->hwnd, GWL_EXSTYLE, extended & ~(LONG_PTR)WS_EX_LAYERED);
            (void)RedrawWindow(window->hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
            return HERO_UI_OK;
        }
        (void)SetWindowLongPtrW(window->hwnd, GWL_EXSTYLE, extended | WS_EX_LAYERED);
        if (!SetLayeredWindowAttributes(window->hwnd, 0, (BYTE)value, LWA_ALPHA)) {
            return hero_ui_win_failed("setting a window's opacity", GetLastError());
        }
        return HERO_UI_OK;
    case HERO_UI_WINDOW_SET_BACKDROP:
        return hero_ui_win_set_backdrop(window, value);
    case HERO_UI_WINDOW_SET_MODAL:
        return hero_ui_win_set_modal(window, value == 1);
    default:
        return HERO_UI_INVALID;
    }
}

/* Gives every window the app's icon, `hero_ui_win_icon`, or the class's,
 * the system's own, where it is NULL. */
static void hero_ui_win_give_icon(HWND hwnd) {
    (void)SendMessageW(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hero_ui_win_icon);
    (void)SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hero_ui_win_icon);
}

/* An icon of `width` by `height` pixels of straight RGBA at `rgba`, through
 * `*made`. */
static int64_t hero_ui_win_make_icon(const uint8_t *rgba, int64_t width, int64_t height, HICON *made) {
    BITMAPV5HEADER format;
    ICONINFO parts;
    void *bits = NULL;
    HBITMAP color;
    HBITMAP mask;
    uint8_t *into;

    *made = NULL;
    if (width > 4096 || height > 4096) {
        return hero_ui_refused(HERO_UI_OTHER, "an icon of Windows is at most 4096 pixels a side");
    }
    memset(&format, 0, sizeof format);
    format.bV5Size = (DWORD)sizeof format;
    format.bV5Width = (LONG)width;
    format.bV5Height = -(LONG)height;
    format.bV5Planes = 1;
    format.bV5BitCount = 32;
    format.bV5Compression = BI_BITFIELDS;
    format.bV5RedMask = 0x00FF0000u;
    format.bV5GreenMask = 0x0000FF00u;
    format.bV5BlueMask = 0x000000FFu;
    format.bV5AlphaMask = 0xFF000000u;
    color = CreateDIBSection(NULL, (BITMAPINFO *)&format, DIB_RGB_COLORS, &bits, NULL, 0);
    mask = CreateBitmap((int)width, (int)height, 1, 1, NULL);
    if (color == NULL || bits == NULL || mask == NULL) {
        if (color != NULL) {
            (void)DeleteObject(color);
        }
        if (mask != NULL) {
            (void)DeleteObject(mask);
        }
        return HERO_UI_LIMIT;
    }
    /* An icon's colour bitmap holds its alpha straight, as given. */
    into = bits;
    for (int64_t i = 0; i < width * height; i += 1) {
        into[i * 4 + 0] = rgba[i * 4 + 2];
        into[i * 4 + 1] = rgba[i * 4 + 1];
        into[i * 4 + 2] = rgba[i * 4 + 0];
        into[i * 4 + 3] = rgba[i * 4 + 3];
    }
    memset(&parts, 0, sizeof parts);
    parts.fIcon = TRUE;
    parts.hbmColor = color;
    parts.hbmMask = mask;
    *made = CreateIconIndirect(&parts);
    (void)DeleteObject(color);
    (void)DeleteObject(mask);
    return *made != NULL ? HERO_UI_OK : hero_ui_win_failed("making an icon", GetLastError());
}

int64_t hero_ui_arm_set_icon(const uint8_t *rgba, int64_t width, int64_t height) {
    HICON made = NULL;
    HICON before = hero_ui_win_icon;

    if (width > 0 && height > 0) {
        int64_t answer = hero_ui_win_make_icon(rgba, width, height, &made);

        if (answer != HERO_UI_OK) {
            return answer;
        }
    }
    hero_ui_win_icon = made;
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *window = hero_ui_at(at);

        if (window != NULL && window->kind == HERO_UI_WINDOW) {
            hero_ui_win_give_icon(window->hwnd);
        }
        if (window != NULL && window->kind == HERO_UI_TRAY && window->tray_icon == NULL && window->tray_shown) {
            (void)hero_ui_win_tray_show(window, NIM_MODIFY);
        }
    }
    if (before != NULL) {
        (void)DestroyIcon(before);
    }
    return HERO_UI_OK;
}

/* The modal session is AppKit's alone: here a modal window's others are
 * disabled and the ordinary loop serves. */
bool hero_ui_arm_modal_here(void) {
    return false;
}

int64_t hero_ui_arm_modal_pump(int64_t ms) {
    (void)ms;
    return HERO_UI_UNSUPPORTED;
}

/* -- KEYS PRESSED BY THE PROGRAM, AND THE WIDGET WITH THE KEYS -------------
 *
 * A key is pressed by `SendInput`, the person's keyboard's own path, so the
 * accelerator table, the dialog manager and the control see it as theirs;
 * Windows hands it to the foreground window alone, so the window is brought
 * there first and a refusal by its foreground lock is told. The loop runs
 * until the key's release has come back through the queue.
 *
 * THE KEYS IN A WINDOW: Windows gives the keys to one control of the thread's
 * active window. A window keeps the control that had them when it was left
 * (`WM_ACTIVATE`) and gives them back when it is activated again, as a
 * dialog does, so each window answers its own control whether it is active
 * or not; a window shown the first time gives them to its first control in
 * the Tab order, which is the order its controls stand in, left to right and
 * top to bottom (each container's children put in that order before a Tab
 * moves the keys).
 *
 * RETURN AND ESCAPE are the dialog manager's (`IsDialogMessageW` in `pump`):
 * a control that does not take Return itself (a field, a list, a table, a
 * tree) gives its window `IDOK`, and Escape gives it `IDCANCEL`; the window
 * presses its button for that key where it has one the person reaches, and
 * otherwise Return submits the field or opens the row chosen. A text area
 * takes Return as a new line, and a focused push button is pressed by it,
 * the dialog manager's own. */

/* The widget whose control `hwnd` is, or stands in: a radio item's group, a
 * list view's header's table; NULL for none of this arm's. */
static hero_ui_native *hero_ui_win_widget_at(HWND hwnd) {
    for (HWND at = hwnd; at != NULL; at = GetParent(at)) {
        hero_ui_native *widget = hero_ui_win_of(at);

        if (widget != NULL) {
            return widget;
        }
        if ((GetWindowLongPtrW(at, GWL_STYLE) & WS_CHILD) == 0) {
            break;
        }
    }
    return NULL;
}

/* Whether a widget of `kind` takes the keys. */
static bool hero_ui_win_takes_keys(int64_t kind) {
    switch (kind) {
    case HERO_UI_BUTTON:
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
    case HERO_UI_CHECKBOX:
    case HERO_UI_RADIO_GROUP:
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
    case HERO_UI_COMBO:
    case HERO_UI_SLIDER:
    case HERO_UI_TABS:
    case HERO_UI_TABLE:
    case HERO_UI_TREE:
    case HERO_UI_DATE_PICKER:
    case HERO_UI_TIME_PICKER:
    case HERO_UI_DATE_TIME_PICKER:
    case HERO_UI_CALENDAR:
    case HERO_UI_COLOR_WELL:
    case HERO_UI_FONT_BUTTON:
    case HERO_UI_EDITABLE_COMBO:
    case HERO_UI_SPIN:
    case HERO_UI_CANVAS:
        return true;
    default:
        return false;
    }
}

/* The control of widget `widget` that takes the keys: a radio group's item
 * checked, else its first. */
static HWND hero_ui_win_key_control(const hero_ui_native *widget) {
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        int64_t checked = hero_ui_win_checked_radio(widget);

        return widget->radio_count == 0 ? widget->hwnd : widget->radios[checked >= 0 ? checked : 0];
    }
    return widget->hwnd;
}

/* Puts the controls standing in `parent` in the order they stand, left to
 * right and top to bottom, which is the dialog manager's Tab order, then
 * those of each container among them. */
static void hero_ui_win_order_tabs(HWND parent) {
    int64_t count = 0;
    hero_ui_native **row;

    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        count += widget != NULL && widget->kind != HERO_UI_WINDOW && widget->hwnd != NULL && GetParent(widget->hwnd) == parent ? 1 : 0;
    }
    if (count == 0) {
        return;
    }
    row = hero_layer_alloc((size_t)count * sizeof *row);
    if (row == NULL) {
        return;
    }
    count = 0;
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        if (widget != NULL && widget->kind != HERO_UI_WINDOW && widget->hwnd != NULL && GetParent(widget->hwnd) == parent) {
            int64_t i = count;

            /* By top, then left, then the order made. */
            while (i > 0 && (row[i - 1]->y > widget->y || (row[i - 1]->y == widget->y && (row[i - 1]->x > widget->x || (row[i - 1]->x == widget->x && row[i - 1]->id > widget->id))))) {
                row[i] = row[i - 1];
                i -= 1;
            }
            row[i] = widget;
            count += 1;
        }
    }
    for (int64_t i = 0; i < count; i += 1) {
        (void)SetWindowPos(row[i]->hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
    }
    /* A bar's own control draws what its items stand over: lowest of all. */
    if (hero_ui_win_of(parent) != NULL && hero_ui_win_of(parent)->part != NULL && (hero_ui_win_of(parent)->kind == HERO_UI_TOOLBAR || hero_ui_win_of(parent)->kind == HERO_UI_STATUS_BAR)) {
        (void)SetWindowPos(hero_ui_win_of(parent)->part, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
    }
    for (int64_t i = 0; i < count; i += 1) {
        if (row[i]->inside != NULL) {
            hero_ui_win_order_tabs(row[i]->inside);
        }
        if (row[i]->kind == HERO_UI_TABS) {
            hero_ui_win_order_tabs(row[i]->hwnd);
        }
    }
    hero_layer_free(row);
}

/* Window `window`'s controls in their Tab order, where a place changed. */
static void hero_ui_win_settle_tabs(hero_ui_native *window) {
    if (window->tabs_stale) {
        window->tabs_stale = false;
        hero_ui_win_order_tabs(window->hwnd);
    }
}

/* The control of window `window` that has the keys: Windows' own where the
 * window is active, else the one it keeps; NULL for none. */
static HWND hero_ui_win_focus_in(const hero_ui_native *window) {
    HWND focus = GetFocus();

    if (focus != NULL && IsChild(window->hwnd, focus)) {
        return focus;
    }
    return window->focus != NULL && IsWindow(window->focus) && IsChild(window->hwnd, window->focus) ? window->focus : NULL;
}

/* Gives control `control` of window `window` the keys: kept by the window,
 * and given now where the window is active. */
static void hero_ui_win_give_keys(hero_ui_native *window, HWND control) {
    window->focus = control;
    if (control != NULL && GetActiveWindow() == window->hwnd) {
        (void)SetFocus(control);
    }
}

/* Window `hwnd` activated or left (`WM_ACTIVATE`): left, the control with
 * the keys kept; activated, given them back. Answers whether it handled it. */
static bool hero_ui_win_window_activated(HWND hwnd, WPARAM state) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    HWND focus;

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return false;
    }
    if (LOWORD(state) == WA_INACTIVE) {
        focus = GetFocus();
        if (focus != NULL && IsChild(hwnd, focus)) {
            window->focus = focus;
        }
        return false;
    }
    if (HIWORD(state) != 0) {
        return false;
    }
    focus = hero_ui_win_focus_in(window);
    if (focus == NULL) {
        hero_ui_win_settle_tabs(window);
        focus = GetNextDlgTabItem(hwnd, NULL, FALSE);
    }
    if (focus != NULL && focus != hwnd) {
        window->focus = focus;
        (void)SetFocus(focus);
        return true;
    }
    return false;
}

/* Window `window` shown the first time: its first control in the Tab order
 * given the keys, where the program gave them to none. */
static void hero_ui_win_first_shown(hero_ui_native *window) {
    HWND first;

    window->shown_once = true;
    window->tabs_stale = true;
    hero_ui_win_settle_tabs(window);
    if (hero_ui_win_focus_in(window) != NULL) {
        hero_ui_win_give_keys(window, hero_ui_win_focus_in(window));
        return;
    }
    first = GetNextDlgTabItem(window->hwnd, NULL, FALSE);
    if (first != NULL && first != window->hwnd && hero_ui_win_widget_at(first) != NULL) {
        hero_ui_win_give_keys(window, first);
    }
}

/* The whole text of editable combo box or spin `widget` chosen, as the dialog
 * manager chooses a field's text when the person's Tab gives it the keys, so
 * that what is typed next replaces it, as on the other arms; the choice
 * stays while the keys are elsewhere. */
static void hero_ui_win_select_all(const hero_ui_native *widget) {
    if (widget->kind == HERO_UI_EDITABLE_COMBO) {
        (void)SendMessageW(widget->hwnd, CB_SETEDITSEL, 0, MAKELPARAM(0, -1));
    } else if (widget->kind == HERO_UI_SPIN) {
        (void)SendMessageW(widget->hwnd, EM_SETSEL, 0, -1);
    }
}

int64_t hero_ui_arm_set_focus(int64_t id) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_win_window(widget->window);
    if (window == NULL || widget->hwnd == NULL || !hero_ui_win_reachable(widget)) {
        return HERO_UI_INVALID;
    }
    switch (widget->kind) {
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
    case HERO_UI_TABLE:
    case HERO_UI_TREE:
    case HERO_UI_CANVAS:
        hero_ui_win_give_keys(window, widget->hwnd);
        return HERO_UI_OK;
    case HERO_UI_EDITABLE_COMBO:
    case HERO_UI_SPIN:
        hero_ui_win_give_keys(window, widget->hwnd);
        hero_ui_win_select_all(widget);
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

int64_t hero_ui_arm_focus(int64_t id, int64_t *focus) {
    hero_ui_native *window = hero_ui_find(id);
    HWND control;
    hero_ui_native *found;

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (window->kind != HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    control = hero_ui_win_focus_in(window);
    found = control != NULL ? hero_ui_win_widget_at(control) : NULL;
    /* A container takes the keys only where it holds no control that does:
     * the window is answered for it. */
    *focus = found != NULL && found->inside == NULL ? found->id : window->id;
    return HERO_UI_OK;
}

/* The `INPUT` of virtual key `key` going down or up. */
static INPUT hero_ui_win_key_input(WORD key, bool extended, bool up) {
    INPUT input;

    memset(&input, 0, sizeof input);
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.wScan = (WORD)MapVirtualKeyW(key, MAPVK_VK_TO_VSC);
    input.ki.dwFlags = (extended ? KEYEVENTF_EXTENDEDKEY : 0) | (up ? KEYEVENTF_KEYUP : 0);
    return input;
}

/* Runs the loop until virtual key `last` comes back released, or
 * `HERO_UI_WIN_KEY_WAIT_MS` pass, then takes what it left queued; whatever
 * the key queued stays queued for `wait`. */
static void hero_ui_win_key_back(WORD last) {
    ULONGLONG deadline = GetTickCount64() + HERO_UI_WIN_KEY_WAIT_MS;
    bool came = false;
    MSG message;

    while (!came) {
        ULONGLONG now = GetTickCount64();

        while (!came && PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
            came = (message.message == WM_KEYUP || message.message == WM_SYSKEYUP) && message.wParam == last;
            if (message.message == WM_QUIT) {
                hero_ui_win_quit();
                continue;
            }
            if (!hero_ui_win_translated(&message)) {
                (void)TranslateMessage(&message);
                (void)DispatchMessageW(&message);
            }
        }
        if (came || now >= deadline) {
            break;
        }
        (void)MsgWaitForMultipleObjectsEx(0, NULL, (DWORD)(deadline - now), QS_ALLINPUT, MWMO_INPUTAVAILABLE);
    }
    while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            hero_ui_win_quit();
            continue;
        }
        if (!hero_ui_win_translated(&message)) {
            (void)TranslateMessage(&message);
            (void)DispatchMessageW(&message);
        }
    }
}

int64_t hero_ui_arm_press_key(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    static const struct {
        int64_t held;
        WORD key;
    } helds[] = {{HERO_UI_PRIMARY, VK_CONTROL}, {HERO_UI_SHIFT, VK_SHIFT}, {HERO_UI_ALT, VK_MENU}};
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;
    INPUT inputs[8];
    UINT made = 0;
    WORD code = 0;
    bool extended = false;
    WORD last;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = widget->kind == HERO_UI_WINDOW ? widget : hero_ui_win_window(widget->window);
    if (window == NULL || !hero_ui_win_key_of(key, count, &code, &extended) || (widget != window && (!hero_ui_win_takes_keys(widget->kind) || widget->hwnd == NULL || !hero_ui_win_reachable(widget)))) {
        return HERO_UI_INVALID;
    }
    if (!IsWindowVisible(window->hwnd)) {
        return hero_ui_refused(HERO_UI_OTHER, "the window is hidden, and Windows hands a key to a window it shows");
    }
    if (GetForegroundWindow() != window->hwnd) {
        (void)SetForegroundWindow(window->hwnd);
    }
    if (GetForegroundWindow() != window->hwnd) {
        return hero_ui_refused(HERO_UI_OTHER, "Windows keeps another window in front (its foreground lock), so a key pressed would go there");
    }
    if (widget != window) {
        window->focus = hero_ui_win_key_control(widget);
        (void)SetFocus(window->focus);
    }
    hero_ui_win_settle_tabs(window);
    for (size_t i = 0; i < sizeof helds / sizeof helds[0]; i += 1) {
        if ((modifiers & helds[i].held) != 0) {
            inputs[made++] = hero_ui_win_key_input(helds[i].key, false, false);
        }
    }
    inputs[made++] = hero_ui_win_key_input(code, extended, false);
    inputs[made++] = hero_ui_win_key_input(code, extended, true);
    last = code;
    for (size_t i = sizeof helds / sizeof helds[0]; i > 0; i -= 1) {
        if ((modifiers & helds[i - 1].held) != 0) {
            inputs[made++] = hero_ui_win_key_input(helds[i - 1].key, false, true);
            last = helds[i - 1].key;
        }
    }
    if (SendInput(made, inputs, (int)sizeof inputs[0]) != made) {
        return hero_ui_win_failed("pressing a key (SendInput, which a program of a higher integrity's window refuses)", GetLastError());
    }
    hero_ui_win_key_back(last);
    return HERO_UI_OK;
}

/* Whether button `button` is one the person presses by a key now: shown,
 * enabled, in a window. */
static hero_ui_native *hero_ui_win_key_button(int64_t id) {
    hero_ui_native *button = id != 0 ? hero_ui_find(id) : NULL;

    return button != NULL && button->kind == HERO_UI_BUTTON && hero_ui_win_reachable(button) && IsWindowEnabled(button->hwnd) ? button : NULL;
}

/* The first row table `table` has chosen, -1 for none. */
static int64_t hero_ui_win_first_chosen(const hero_ui_native *table);
/* The item tree `tree` has chosen, 0 for none. */
static int64_t hero_ui_win_item_chosen(const hero_ui_native *tree);

/* Return, `IDOK`, or Escape, `IDCANCEL`, the dialog manager gave window
 * `hwnd`: its button for that key pressed where the person reaches it; else
 * for Return the field with the keys submitted, or the row or item chosen
 * of the list, table or tree with them opened. */
static void hero_ui_win_dialog_key(HWND hwnd, UINT which) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    hero_ui_native *button;
    hero_ui_native *focused;
    HWND focus;

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return;
    }
    /* A number typed in a spin is committed by Return, whatever then takes
     * the key. */
    focus = which == IDOK ? GetFocus() : NULL;
    focused = focus != NULL && IsChild(hwnd, focus) ? hero_ui_win_widget_at(focus) : NULL;
    if (focused != NULL && focused->kind == HERO_UI_SPIN) {
        hero_ui_win_spin_commit(focused);
    }
    button = hero_ui_win_key_button(which == IDCANCEL ? window->escape_button : window->enter_button);
    if (button != NULL) {
        hero_ui_win_press(button->hwnd, false);
        return;
    }
    focus = GetFocus();
    focused = which == IDOK && focus != NULL && IsChild(hwnd, focus) ? hero_ui_win_widget_at(focus) : NULL;
    if (focused == NULL || !IsWindowEnabled(focused->hwnd)) {
        return;
    }
    switch (focused->kind) {
    case HERO_UI_TEXT_FIELD:
        hero_ui_tell(HERO_UI_EVENT_SUBMIT, focused->id, 0, 0);
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
    case HERO_UI_TABLE: {
        /* The first row chosen, which the header says Return opens. */
        int64_t first = -1;

        if (focused->kind == HERO_UI_TABLE) {
            first = hero_ui_win_first_chosen(focused);
        } else if (focused->kind == HERO_UI_LIST) {
            first = (int64_t)SendMessageW(focused->hwnd, LB_GETCURSEL, 0, 0);
        } else {
            for (int64_t i = 0; i < focused->items && first < 0; i += 1) {
                first = SendMessageW(focused->hwnd, LB_GETSEL, (WPARAM)i, 0) > 0 ? i : -1;
            }
        }
        if (first >= 0) {
            hero_ui_tell(HERO_UI_EVENT_ACTIVATE, focused->id, first, 0);
        }
        break;
    }
    case HERO_UI_TREE: {
        int64_t item = hero_ui_win_item_chosen(focused);

        if (item > 0) {
            hero_ui_tell(HERO_UI_EVENT_ACTIVATE, focused->id, item, 0);
        }
        break;
    }
    default:
        break;
    }
}

int64_t hero_ui_arm_set_button_key(int64_t id, int64_t key) {
    hero_ui_native *button = hero_ui_find(id);
    hero_ui_native *window;
    hero_ui_native *before;

    if (button == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_win_window(button->window);
    if (button->kind != HERO_UI_BUTTON || window == NULL) {
        return HERO_UI_INVALID;
    }
    if (window->enter_button == id) {
        window->enter_button = 0;
        (void)SendMessageW(button->hwnd, BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
    }
    if (window->escape_button == id) {
        window->escape_button = 0;
    }
    if (key == HERO_UI_BUTTON_KEY_ENTER) {
        before = hero_ui_find(window->enter_button);
        if (before != NULL && before->hwnd != NULL) {
            (void)SendMessageW(before->hwnd, BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
        }
        window->enter_button = id;
        /* Drawn as its window's default, as the system's dialogs draw it. */
        (void)SendMessageW(button->hwnd, BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
    } else if (key == HERO_UI_BUTTON_KEY_ESCAPE) {
        window->escape_button = id;
    }
    return HERO_UI_OK;
}

/* -- A MENU ITEM'S ROLE ---------------------------------------------------- */

/* Windows holds no application menu, so an item keeps the place the program
 * gave it and its role asks nothing more of it. */
int64_t hero_ui_arm_set_role(int64_t id, int64_t role) {
    hero_ui_native *item = hero_ui_find(id);

    (void)role;
    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return item->kind == HERO_UI_MENU_ITEM ? HERO_UI_OK : HERO_UI_INVALID;
}

/* -- CHECK AND RADIO MENU ITEMS -------------------------------------------- */

int64_t hero_ui_arm_set_check(int64_t id, int64_t mode) {
    hero_ui_native *item = hero_ui_find(id);

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM) {
        return HERO_UI_INVALID;
    }
    item->check = mode;
    item->checked = false;
    return hero_ui_win_refill_owner(item);
}

/* -- UNDO AND THE SELECTION IN TEXT WIDGETS ---------------------------------
 *
 * An `EDIT` control keeps one level of undo, which its second undo turns into
 * a redo, so the arm keeps the edits itself: a text area's, every edit since
 * the program last set its text, and a text field's, those made while it
 * has the keys and until they pass to another control. An edit is kept as
 * what it took out of the text and what it put there, found by comparing the
 * text the control held before with the one it holds after (`EN_CHANGE`), so
 * the person's typing, a paste and an injected edit are kept alike and the
 * kept steps hold the changed units alone. A run of characters typed one
 * after another is one edit, as the person's undo takes a word at a time;
 * an injected edit is one of its own. The control's own undo is never
 * reached: its keys (Ctrl+Z, Ctrl+Y, Alt+Backspace) and its messages
 * (`WM_UNDO`, `EM_UNDO`, `EM_CANUNDO`) answer from the kept steps, so the
 * person's undo and the program's are one.
 *
 * THE SELECTION is in the control's UTF-16 units (a CR LF is two), told in
 * the bytes of the UTF-8 text the program reads (a CR LF is one line feed);
 * an end inside a character is refused and a unit inside one is told at its
 * start. A text area shows its selection while it has not the keys
 * (`ES_NOHIDESEL`); a text field takes them, where its own does. */

/* The edits one text widget keeps, and the units of text they may hold. */
#define HERO_UI_WIN_UNDO_STEPS 256
#define HERO_UI_WIN_UNDO_UNITS ((int64_t)1 << 22)

/* Whether `count` units at `units` are one character a person types: one
 * unit, or a surrogate pair. */
static bool hero_ui_win_typed_one(const wchar_t *units, int64_t count) {
    return count == 1 || (count == 2 && IS_HIGH_SURROGATE(units[0]) && IS_LOW_SURROGATE(units[1]));
}

/* Forgets the steps of `widget` from the one numbered `from` on. */
static void hero_ui_win_steps_drop(hero_ui_native *widget, int64_t from) {
    for (int64_t i = from; i < widget->step_count; i += 1) {
        hero_layer_free(widget->steps[i].gone);
        hero_layer_free(widget->steps[i].came);
    }
    if (from < widget->step_count) {
        widget->step_count = from;
    }
    if (widget->step_done > widget->step_count) {
        widget->step_done = widget->step_count;
    }
}

/* Forgets every step of `widget` and the run of typing it may join. */
static void hero_ui_win_steps_forget(hero_ui_native *widget) {
    hero_ui_win_steps_drop(widget, 0);
    widget->typing = false;
}

static void hero_ui_win_edit_end(hero_ui_native *widget) {
    hero_ui_win_steps_forget(widget);
    hero_layer_free(widget->steps);
    hero_layer_free(widget->known);
    widget->steps = NULL;
    widget->step_room = 0;
    widget->known = NULL;
    widget->known_count = 0;
}

/* Takes the text control `widget` holds now as its text, no edit of it
 * kept: the program set it. */
static void hero_ui_win_edit_reset(hero_ui_native *widget) {
    wchar_t *text = hero_ui_win_wide_of(widget->hwnd);

    hero_ui_win_steps_forget(widget);
    hero_layer_free(widget->known);
    widget->known = text;
    widget->known_count = text != NULL ? (int64_t)wcslen(text) : -1;
}

/* Whether `widget`'s control has the keys of its window. */
static bool hero_ui_win_has_keys(const hero_ui_native *widget) {
    hero_ui_native *window = hero_ui_win_window(widget->window);

    return window != NULL && widget->hwnd != NULL && hero_ui_win_focus_in(window) == widget->hwnd;
}

/* Copies `count` units at `units` with a zero after them, or NULL. */
static wchar_t *hero_ui_win_units_copy(const wchar_t *units, int64_t count) {
    wchar_t *copy = hero_layer_alloc(((size_t)count + 1) * sizeof(wchar_t));

    if (copy != NULL) {
        if (count > 0) {
            memcpy(copy, units, (size_t)count * sizeof(wchar_t));
        }
        copy[count] = 0;
    }
    return copy;
}

/* Drops the oldest steps of `widget` while it keeps more than the limits
 * let it. */
static void hero_ui_win_steps_trim(hero_ui_native *widget) {
    for (;;) {
        int64_t units = 0;

        for (int64_t i = 0; i < widget->step_count; i += 1) {
            units += widget->steps[i].gone_count + widget->steps[i].came_count;
        }
        if (widget->step_count <= 1 || (widget->step_count <= HERO_UI_WIN_UNDO_STEPS && units <= HERO_UI_WIN_UNDO_UNITS)) {
            return;
        }
        hero_layer_free(widget->steps[0].gone);
        hero_layer_free(widget->steps[0].came);
        memmove(widget->steps, widget->steps + 1, (size_t)(widget->step_count - 1) * sizeof widget->steps[0]);
        widget->step_count -= 1;
        if (widget->step_done > 0) {
            widget->step_done -= 1;
        }
    }
}

/* Keeps one edit of `widget`: `gone_count` units taken out at `at` and
 * `came_count` put there. The edits undone are given up, since a new edit
 * ends their redo; a typed character joins the run before it where it
 * stands just after it. False where memory ran out. */
static bool hero_ui_win_step_keep(hero_ui_native *widget, int64_t at, const wchar_t *gone, int64_t gone_count, const wchar_t *came, int64_t came_count, bool person) {
    bool typed = person && hero_ui_win_typed_one(came, came_count);
    struct hero_ui_win_step *last;
    struct hero_ui_win_step *step;
    wchar_t *took;
    wchar_t *put;

    hero_ui_win_steps_drop(widget, widget->step_done);
    last = widget->step_count > 0 ? &widget->steps[widget->step_count - 1] : NULL;
    if (typed && widget->typing && last != NULL && gone_count == 0 && at == last->at + last->came_count) {
        wchar_t *grown = hero_layer_grow(last->came, ((size_t)(last->came_count + came_count) + 1) * sizeof(wchar_t));

        if (grown == NULL) {
            return false;
        }
        memcpy(grown + last->came_count, came, (size_t)came_count * sizeof(wchar_t));
        last->came_count += came_count;
        grown[last->came_count] = 0;
        last->came = grown;
        return true;
    }
    if (widget->step_count == widget->step_room) {
        int64_t room = widget->step_room == 0 ? 16 : widget->step_room * 2;
        struct hero_ui_win_step *more = hero_layer_grow(widget->steps, (size_t)room * sizeof widget->steps[0]);

        if (more == NULL) {
            return false;
        }
        widget->steps = more;
        widget->step_room = room;
    }
    took = hero_ui_win_units_copy(gone, gone_count);
    put = hero_ui_win_units_copy(came, came_count);
    if (took == NULL || put == NULL) {
        hero_layer_free(took);
        hero_layer_free(put);
        return false;
    }
    step = &widget->steps[widget->step_count];
    step->at = at;
    step->gone = took;
    step->gone_count = gone_count;
    step->came = put;
    step->came_count = came_count;
    widget->step_count += 1;
    widget->step_done = widget->step_count;
    widget->typing = typed;
    hero_ui_win_steps_trim(widget);
    return true;
}

/* The control of `widget` changed its text: keeps the edit that did it,
 * `person` where the person made it, and takes the new text as the one the
 * control holds. Nothing is kept where memory ran out, and no step then
 * stands that the text could have left behind. */
static void hero_ui_win_edit_changed(hero_ui_native *widget, bool person) {
    wchar_t *text = hero_ui_win_wide_of(widget->hwnd);
    const wchar_t *before = widget->known != NULL ? widget->known : L"";
    int64_t before_count = widget->known_count;
    int64_t count;
    int64_t same;
    int64_t tail = 0;
    int64_t most;

    if (text == NULL) {
        hero_ui_win_steps_forget(widget);
        hero_layer_free(widget->known);
        widget->known = NULL;
        widget->known_count = -1;
        return;
    }
    count = (int64_t)wcslen(text);
    if (before_count < 0) {
        hero_ui_win_steps_forget(widget);
        hero_layer_free(widget->known);
        widget->known = text;
        widget->known_count = count;
        return;
    }
    most = before_count < count ? before_count : count;
    same = 0;
    while (same < most && before[same] == text[same]) {
        same += 1;
    }
    /* A change never starts or ends between the two units of a character. */
    if (same > 0 && (IS_HIGH_SURROGATE(text[same - 1]) || text[same - 1] == L'\r')) {
        same -= 1;
    }
    while (tail < most - same && before[before_count - 1 - tail] == text[count - 1 - tail]) {
        tail += 1;
    }
    if (tail > 0 && (IS_LOW_SURROGATE(text[count - tail]) || text[count - tail] == L'\n')) {
        tail -= 1;
    }
    if (before_count - same - tail > 0 || count - same - tail > 0) {
        if (!(widget->kind == HERO_UI_TEXT_AREA || hero_ui_win_has_keys(widget)) || !hero_ui_win_step_keep(widget, same, before + same, before_count - same - tail, text + same, count - same - tail, person)) {
            hero_ui_win_steps_forget(widget);
        }
    }
    hero_layer_free(widget->known);
    widget->known = text;
    widget->known_count = count;
}

/* Undoes the last edit `widget` keeps, or redoes the last undone; false
 * where there is none. The change is told as the person's where `told`. */
static bool hero_ui_win_edit_replay(hero_ui_native *widget, bool undo, bool told) {
    struct hero_ui_win_step *step;
    const wchar_t *with;
    int64_t out;
    int64_t in;
    int64_t whole;
    int64_t held;

    if (undo ? widget->step_done == 0 : widget->step_done == widget->step_count) {
        return false;
    }
    step = &widget->steps[undo ? widget->step_done - 1 : widget->step_done];
    out = undo ? step->came_count : step->gone_count;
    in = undo ? step->gone_count : step->came_count;
    with = undo ? step->gone : step->came;
    held = (int64_t)GetWindowTextLengthW(widget->hwnd);
    /* The steps stand on the text the control held when they were kept; a
     * text changed with no notice leaves none that can be trusted. */
    if (widget->known_count < 0 || held != widget->known_count || step->at + out > held) {
        hero_ui_win_steps_forget(widget);
        return false;
    }
    whole = held - out + in;
    widget->typing = false;
    hero_ui_win_replaying += 1;
    if (!told) {
        hero_ui_mute(1);
    }
    (void)SendMessageW(widget->hwnd, EM_SETSEL, (WPARAM)step->at, (LPARAM)(step->at + out));
    (void)SendMessageW(widget->hwnd, EM_REPLACESEL, FALSE, (LPARAM)with);
    (void)SendMessageW(widget->hwnd, EM_SETSEL, (WPARAM)(step->at + in), (LPARAM)(step->at + in));
    (void)SendMessageW(widget->hwnd, EM_SCROLLCARET, 0, 0);
    if (!told) {
        hero_ui_mute(-1);
    }
    hero_ui_win_replaying -= 1;
    if ((int64_t)GetWindowTextLengthW(widget->hwnd) != whole) {
        /* The control refused some of it: its text is no longer the steps'. */
        hero_ui_win_steps_forget(widget);
    } else {
        widget->step_done += undo ? -1 : 1;
    }
    hero_layer_free(widget->known);
    widget->known = hero_ui_win_wide_of(widget->hwnd);
    widget->known_count = widget->known != NULL ? (int64_t)wcslen(widget->known) : -1;
    return true;
}

/* The key a control's `WM_CHAR` or `WM_SYSKEYDOWN` carries, answered where
 * it is an undo or a redo of a text widget: true where it was one. */
static bool hero_ui_win_edit_key(hero_ui_native *widget, UINT message, WPARAM key) {
    bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

    if (message == WM_CHAR && key == 0x1A) {
        (void)hero_ui_win_edit_replay(widget, !shift, true);
        return true;
    }
    if (message == WM_CHAR && key == 0x19) {
        (void)hero_ui_win_edit_replay(widget, false, true);
        return true;
    }
    if (message == WM_SYSKEYDOWN && key == VK_BACK) {
        (void)hero_ui_win_edit_replay(widget, !shift, true);
        return true;
    }
    return false;
}

/* The bytes of the UTF-8 of one UTF-16 unit that stands alone: a lone
 * surrogate is read as U+FFFD, three bytes, as `hero_ui_win_narrow` reads it. */
static int64_t hero_ui_win_utf8_length(wchar_t unit) {
    if (IS_HIGH_SURROGATE(unit) || IS_LOW_SURROGATE(unit) || unit >= 0x800) {
        return 3;
    }
    return unit >= 0x80 ? 2 : 1;
}

/* The unit of the UTF-16 text `wide` (`units` of them, CR LF one character
 * where `lines`) at which byte `byte` of its UTF-8 text starts, -1 where
 * that byte is inside a character or past the text. */
static int64_t hero_ui_win_unit_of_byte(const wchar_t *wide, int64_t units, bool lines, int64_t byte) {
    int64_t unit = 0;
    int64_t bytes = 0;

    while (unit < units) {
        wchar_t c = wide[unit];

        if (bytes == byte) {
            return unit;
        }
        if (lines && c == L'\r' && unit + 1 < units && wide[unit + 1] == L'\n') {
            bytes += 1;
            unit += 2;
        } else if (IS_HIGH_SURROGATE(c) && unit + 1 < units && IS_LOW_SURROGATE(wide[unit + 1])) {
            bytes += 4;
            unit += 2;
        } else {
            bytes += hero_ui_win_utf8_length(c);
            unit += 1;
        }
        if (bytes > byte) {
            return -1;
        }
    }
    return bytes == byte ? units : -1;
}

/* The byte of the UTF-8 text at which the character holding unit `unit` of
 * `wide` starts (see `hero_ui_win_unit_of_byte`). */
static int64_t hero_ui_win_byte_of_unit(const wchar_t *wide, int64_t units, bool lines, int64_t unit) {
    int64_t at = 0;
    int64_t bytes = 0;

    while (at < units && at < unit) {
        wchar_t c = wide[at];
        int64_t step = 1;

        if (lines && c == L'\r' && at + 1 < units && wide[at + 1] == L'\n') {
            if (at + 2 > unit) {
                break;
            }
            bytes += 1;
            step = 2;
        } else if (IS_HIGH_SURROGATE(c) && at + 1 < units && IS_LOW_SURROGATE(wide[at + 1])) {
            if (at + 2 > unit) {
                break;
            }
            bytes += 4;
            step = 2;
        } else {
            bytes += hero_ui_win_utf8_length(c);
        }
        at += step;
    }
    return bytes;
}

static bool hero_ui_win_is_text(const hero_ui_native *widget) {
    return widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA;
}

int64_t hero_ui_arm_can_undo(int64_t id, int64_t which, int64_t *can) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!hero_ui_win_is_text(widget)) {
        return HERO_UI_INVALID;
    }
    *can = (which == HERO_UI_UNDO ? widget->step_done > 0 : widget->step_done < widget->step_count) ? 1 : 0;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_undo(int64_t id, int64_t which, int64_t *done) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!hero_ui_win_is_text(widget)) {
        return HERO_UI_INVALID;
    }
    *done = hero_ui_win_edit_replay(widget, which == HERO_UI_UNDO, false) ? 1 : 0;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_text_selection(int64_t id, int64_t start, int64_t end) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;
    wchar_t *wide;
    int64_t from;
    int64_t to;
    bool lines;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!hero_ui_win_is_text(widget)) {
        return HERO_UI_INVALID;
    }
    window = hero_ui_win_window(widget->window);
    if (widget->kind == HERO_UI_TEXT_FIELD && (window == NULL || !hero_ui_win_reachable(widget))) {
        return HERO_UI_INVALID;
    }
    wide = hero_ui_win_wide_of(widget->hwnd);
    if (wide == NULL) {
        return HERO_UI_LIMIT;
    }
    lines = widget->kind == HERO_UI_TEXT_AREA;
    from = hero_ui_win_unit_of_byte(wide, (int64_t)wcslen(wide), lines, start);
    to = hero_ui_win_unit_of_byte(wide, (int64_t)wcslen(wide), lines, end);
    hero_layer_free(wide);
    if (from < 0 || to < 0) {
        return HERO_UI_INVALID;
    }
    /* A field shows its selection by having the keys, which it takes before
     * the selection is set, a control taking the keys leaving its own. */
    if (widget->kind == HERO_UI_TEXT_FIELD) {
        hero_ui_win_give_keys(window, widget->hwnd);
    }
    (void)SendMessageW(widget->hwnd, EM_SETSEL, (WPARAM)from, (LPARAM)to);
    (void)SendMessageW(widget->hwnd, EM_SCROLLCARET, 0, 0);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_text_selection(int64_t id, int64_t *start, int64_t *end) {
    hero_ui_native *widget = hero_ui_find(id);
    wchar_t *wide;
    DWORD from = 0;
    DWORD to = 0;
    int64_t units;
    bool lines;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!hero_ui_win_is_text(widget)) {
        return HERO_UI_INVALID;
    }
    wide = hero_ui_win_wide_of(widget->hwnd);
    if (wide == NULL) {
        return HERO_UI_LIMIT;
    }
    (void)SendMessageW(widget->hwnd, EM_GETSEL, (WPARAM)&from, (LPARAM)&to);
    units = (int64_t)wcslen(wide);
    lines = widget->kind == HERO_UI_TEXT_AREA;
    *start = hero_ui_win_byte_of_unit(wide, units, lines, (int64_t)from);
    *end = hero_ui_win_byte_of_unit(wide, units, lines, (int64_t)to);
    hero_layer_free(wide);
    return HERO_UI_OK;
}

/* -- A CONTEXT MENU ---------------------------------------------------------
 *
 * A context menu is a pop-up menu kept by the widget it belongs to, shown by
 * `TrackPopupMenuEx` where Windows asks for one (`WM_CONTEXTMENU`: the
 * person's right click, Shift+F10 or the menu key) over that widget, or over
 * a widget standing in it that has none of its own; its items' `WM_COMMAND`
 * reaches the window as a menu bar's does. An edit control shows a menu of
 * its own (Undo, Cut, Copy, Paste), so a field or a text area is subclassed
 * and its own menu gives way to the program's where it has one. */

/* The context menu of widget `id`, or NULL. */
static hero_ui_native *hero_ui_win_context_of(int64_t id) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *menu = hero_ui_at(at);

        if (menu != NULL && menu->kind == HERO_UI_CONTEXT_MENU && menu->parent == id) {
            return menu;
        }
    }
    return NULL;
}

/* Shows the context menu the person asked for over control `target`, at
 * `at` on the screen, or under the control for the keyboard's (-1, -1): the
 * control's widget's own, else that of the nearest container or window it
 * stands in, where `inherit`. Answers whether one was shown. */
static bool hero_ui_win_show_context(HWND target, LPARAM at, bool inherit) {
    hero_ui_native *widget = hero_ui_win_widget_at(target);
    hero_ui_native *menu = NULL;
    hero_ui_native *window;
    POINT point;

    for (hero_ui_native *each = widget; each != NULL && menu == NULL; each = hero_ui_find(each->parent)) {
        menu = hero_ui_win_context_of(each->id);
        if (!inherit || each->kind == HERO_UI_WINDOW) {
            break;
        }
    }
    window = menu != NULL ? hero_ui_win_top(menu->window) : NULL;
    if (menu == NULL || window == NULL || menu->menu == NULL) {
        return false;
    }
    point.x = (short)LOWORD(at);
    point.y = (short)HIWORD(at);
    if (at == (LPARAM)-1 || (point.x == -1 && point.y == -1)) {
        RECT area;

        if (!GetWindowRect(target, &area)) {
            return false;
        }
        point.x = area.left + (area.right - area.left) / 2;
        point.y = area.top + (area.bottom - area.top) / 2;
    }
    (void)hero_ui_win_fill_menu(menu);
    if (window->kind == HERO_UI_TRAY) {
        /* A tray's hidden window in front, so the menu closes when the
         * person clicks away; the message after it lets the menu end. */
        (void)SetForegroundWindow(window->hwnd);
    }
    (void)TrackPopupMenuEx(menu->menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, point.x, point.y, window->hwnd, NULL);
    if (window->kind == HERO_UI_TRAY) {
        (void)PostMessageW(window->hwnd, WM_NULL, 0, 0);
    }
    return true;
}

/* The property an edit control keeps its own procedure in. */
static const wchar_t hero_ui_win_base_name[] = L"HeroesUiBase";

static LRESULT CALLBACK hero_ui_win_edit_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    WNDPROC base = (WNDPROC)GetPropW(hwnd, hero_ui_win_base_name);
    hero_ui_native *widget = message == WM_NCDESTROY ? NULL : hero_ui_win_of(hwnd);

    if (message == WM_CONTEXTMENU && hero_ui_win_show_context(hwnd, lparam, false)) {
        return 0;
    }
    /* The control's own undo gives way to the one this arm keeps (UNDO AND
     * THE SELECTION IN TEXT WIDGETS). */
    if (widget != NULL && hero_ui_win_is_text(widget)) {
        switch (message) {
        case WM_CHAR:
        case WM_SYSKEYDOWN:
            if (hero_ui_win_edit_key(widget, message, wparam)) {
                return 0;
            }
            break;
        case WM_UNDO:
        case EM_UNDO:
            return hero_ui_win_edit_replay(widget, true, true) ? TRUE : FALSE;
        case EM_CANUNDO:
            return widget->step_done > 0 ? TRUE : FALSE;
        case WM_KILLFOCUS: {
            /* A field's edits are those made while it has the keys, so they
             * end as the keys pass to another control of its window. */
            HWND top = GetAncestor(hwnd, GA_ROOT);

            widget->typing = false;
            if (widget->kind == HERO_UI_TEXT_FIELD && wparam != 0 && top != NULL && IsChild(top, (HWND)wparam)) {
                hero_ui_win_steps_forget(widget);
            }
            break;
        }
        default:
            break;
        }
    }
    if (message == WM_NCDESTROY) {
        (void)SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)base);
        (void)RemovePropW(hwnd, hero_ui_win_base_name);
    }
    return base != NULL ? CallWindowProcW(base, hwnd, message, wparam, lparam) : DefWindowProcW(hwnd, message, wparam, lparam);
}

/* Subclasses edit control `hwnd` (A CONTEXT MENU); false where Windows
 * refused, the control then left as it was. */
static bool hero_ui_win_subclass_edit(HWND hwnd) {
    LONG_PTR base = GetWindowLongPtrW(hwnd, GWLP_WNDPROC);

    if (base == 0 || !SetPropW(hwnd, hero_ui_win_base_name, (HANDLE)base)) {
        return false;
    }
    if (SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)hero_ui_win_edit_proc) == 0) {
        (void)RemovePropW(hwnd, hero_ui_win_base_name);
        return false;
    }
    return true;
}

/* -- A MASKED FIELD, AND A PROGRESS BAR WITH NO VALUE ----------------------- */

/* The character an edit control of the common controls' current look masks
 * a password with (version 6's own, a black circle). */
#define HERO_UI_WIN_MASK 0x25CF

int64_t hero_ui_arm_set_masked(int64_t id, int64_t on) {
    hero_ui_native *field = hero_ui_find(id);

    if (field == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (field->kind != HERO_UI_TEXT_FIELD) {
        return HERO_UI_INVALID;
    }
    (void)SendMessageW(field->hwnd, EM_SETPASSWORDCHAR, on == 1 ? (WPARAM)HERO_UI_WIN_MASK : 0, 0);
    (void)InvalidateRect(field->hwnd, NULL, TRUE);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_moving(int64_t id, int64_t on) {
    hero_ui_native *bar = hero_ui_find(id);
    LONG_PTR style;

    if (bar == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (bar->kind != HERO_UI_PROGRESS) {
        return HERO_UI_INVALID;
    }
    style = GetWindowLongPtrW(bar->hwnd, GWL_STYLE);
    if (on == 1) {
        (void)SetWindowLongPtrW(bar->hwnd, GWL_STYLE, style | PBS_MARQUEE);
        (void)SendMessageW(bar->hwnd, PBM_SETMARQUEE, TRUE, 0);
    } else {
        (void)SendMessageW(bar->hwnd, PBM_SETMARQUEE, FALSE, 0);
        (void)SetWindowLongPtrW(bar->hwnd, GWL_STYLE, style & ~(LONG_PTR)PBS_MARQUEE);
        hero_ui_win_show_range(bar);
    }
    return HERO_UI_OK;
}

/* -- THE CLIPBOARD -----------------------------------------------------------
 *
 * Opened by a hidden window of this arm's own, since a clipboard emptied by
 * no window takes no data; another program holding it open is waited for a
 * little, then told. Text is `CF_UNICODETEXT`, its lines ended by CR LF as
 * Windows' programs write them and read back as a line feed (the arm's one
 * rule for text, so a text written is the text read); paths are `CF_HDROP`,
 * a `DROPFILES` of UTF-16 paths, each as given; an image is `CF_DIBV5`,
 * 32 bits a pixel with its alpha straight, rows from the top, and an image
 * another program put is read from it, else from `CF_DIB` (24 or 32 bits,
 * either way up), else from `CF_BITMAP` through GDI. Windows makes
 * `CF_DIB` and `CF_BITMAP` of a `CF_DIBV5`, and each text format of
 * another, but never text of paths or of an image, so what a write puts is
 * found as its own kind alone. */

/* Opens the clipboard by this arm's hidden window, waiting a little for
 * another program that holds it; false with the failure said. */
static bool hero_ui_win_open_clipboard(int64_t *answer) {
    if (hero_ui_win_clip_owner == NULL) {
        hero_ui_win_clip_owner = CreateWindowExW(0, hero_ui_win_panel_class, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandleW(NULL), NULL);
        if (hero_ui_win_clip_owner == NULL) {
            *answer = hero_ui_win_failed("making the window the clipboard is opened by", GetLastError());
            return false;
        }
    }
    for (int tried = 0; tried < HERO_UI_WIN_CLIPBOARD_TRIES; tried += 1) {
        if (OpenClipboard(hero_ui_win_clip_owner)) {
            return true;
        }
        Sleep(10);
    }
    *answer = hero_ui_win_failed("opening the clipboard, which another program holds", GetLastError());
    return false;
}

/* A movable global block of `bytes` bytes, zeroed, or NULL. */
static HGLOBAL hero_ui_win_block(size_t bytes) {
    return GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, bytes);
}

/* The global block of what `kind` puts, the bytes checked by the entry
 * point: NULL with `*answer` set where it cannot be made. */
static HGLOBAL hero_ui_win_clip_block(int64_t kind, const uint8_t *bytes, int64_t count, int64_t width, int64_t height, UINT *format, int64_t *answer) {
    HGLOBAL block = NULL;
    uint8_t *into;

    *answer = HERO_UI_LIMIT;
    if (kind == HERO_UI_CLIP_TEXT) {
        wchar_t *wide = hero_ui_win_wide(bytes, count, HERO_UI_WIN_LINES);
        size_t units = wide != NULL ? wcslen(wide) + 1 : 0;

        *format = CF_UNICODETEXT;
        block = wide != NULL ? hero_ui_win_block(units * sizeof(wchar_t)) : NULL;
        if (block != NULL && (into = GlobalLock(block)) != NULL) {
            memcpy(into, wide, units * sizeof(wchar_t));
            (void)GlobalUnlock(block);
        }
        hero_layer_free(wide);
        return block;
    }
    if (kind == HERO_UI_CLIP_FILES) {
        /* The paths' bytes as they are, each zero after a path a zero unit
         * after it, and the block's own zero, one more, after the last. */
        int units = MultiByteToWideChar(CP_UTF8, 0, (const char *)bytes, (int)count, NULL, 0);

        *format = CF_HDROP;
        if (units <= 0) {
            *answer = hero_ui_win_failed("writing the clipboard's paths as UTF-16", GetLastError());
            return NULL;
        }
        block = hero_ui_win_block(sizeof(DROPFILES) + ((size_t)units + 1) * sizeof(wchar_t));
        if (block != NULL && (into = GlobalLock(block)) != NULL) {
            DROPFILES *head = (DROPFILES *)(void *)into;

            head->pFiles = (DWORD)sizeof(DROPFILES);
            head->fWide = TRUE;
            (void)MultiByteToWideChar(CP_UTF8, 0, (const char *)bytes, (int)count, (wchar_t *)(void *)(into + sizeof(DROPFILES)), units);
            (void)GlobalUnlock(block);
        }
        return block;
    }
    {
        BITMAPV5HEADER head;
        size_t pixels = (size_t)width * (size_t)height;

        *format = CF_DIBV5;
        memset(&head, 0, sizeof head);
        head.bV5Size = (DWORD)sizeof head;
        head.bV5Width = (LONG)width;
        head.bV5Height = -(LONG)height;
        head.bV5Planes = 1;
        head.bV5BitCount = 32;
        head.bV5Compression = BI_BITFIELDS;
        head.bV5SizeImage = (DWORD)(pixels * 4);
        head.bV5RedMask = 0x00FF0000u;
        head.bV5GreenMask = 0x0000FF00u;
        head.bV5BlueMask = 0x000000FFu;
        head.bV5AlphaMask = 0xFF000000u;
        head.bV5CSType = LCS_sRGB;
        head.bV5Intent = LCS_GM_IMAGES;
        if (width > INT32_MAX || height > INT32_MAX) {
            return NULL;
        }
        block = hero_ui_win_block(sizeof head + pixels * 4);
        if (block != NULL && (into = GlobalLock(block)) != NULL) {
            memcpy(into, &head, sizeof head);
            for (size_t i = 0; i < pixels; i += 1) {
                into[sizeof head + i * 4 + 0] = bytes[i * 4 + 2];
                into[sizeof head + i * 4 + 1] = bytes[i * 4 + 1];
                into[sizeof head + i * 4 + 2] = bytes[i * 4 + 0];
                into[sizeof head + i * 4 + 3] = bytes[i * 4 + 3];
            }
            (void)GlobalUnlock(block);
        }
        return block;
    }
}

int64_t hero_ui_arm_clipboard_set(int64_t kind, const uint8_t *bytes, int64_t count, int64_t width, int64_t height) {
    int64_t answer = HERO_UI_OK;
    HGLOBAL block = NULL;
    UINT format = 0;

    if (kind != HERO_UI_CLIP_NOTHING) {
        block = hero_ui_win_clip_block(kind, bytes, count, width, height, &format, &answer);
        if (block == NULL) {
            return answer;
        }
        answer = HERO_UI_OK;
    }
    if (!hero_ui_win_open_clipboard(&answer)) {
        if (block != NULL) {
            (void)GlobalFree(block);
        }
        return answer;
    }
    if (!EmptyClipboard()) {
        answer = hero_ui_win_failed("emptying the clipboard", GetLastError());
    } else if (block != NULL && SetClipboardData(format, block) == NULL) {
        answer = hero_ui_win_failed("putting data on the clipboard", GetLastError());
    } else {
        /* The system owns the block now. */
        block = NULL;
    }
    (void)CloseClipboard();
    if (block != NULL) {
        (void)GlobalFree(block);
    }
    return answer;
}

/* Keeps `count` bytes at `kept`, a block of `hero_layer_alloc`, as what the
 * last read found. */
static void hero_ui_win_keep_clip(uint8_t *kept, int64_t count) {
    hero_layer_free(hero_ui_win_clip);
    hero_ui_win_clip = kept;
    hero_ui_win_clip_count = count;
}

/* The paths of a `DROPFILES` block of `size` bytes at `head`, each as UTF-8
 * followed by one zero byte, into `*paths`; `HERO_UI_NOT_FOUND` where it
 * holds none. A path no UTF-8 names (an unpaired surrogate) is read with
 * U+FFFD in its place. */
static int64_t hero_ui_win_read_drop(const uint8_t *head, size_t size, uint8_t **paths, int64_t *count) {
    const DROPFILES *drop = (const DROPFILES *)(const void *)head;
    int64_t room = 0;

    *paths = NULL;
    *count = 0;
    if (size < sizeof(DROPFILES) || drop->pFiles < sizeof(DROPFILES) || drop->pFiles >= size) {
        return HERO_UI_NOT_FOUND;
    }
    if (drop->fWide) {
        const wchar_t *at = (const wchar_t *)(const void *)(head + drop->pFiles);
        size_t units = (size - drop->pFiles) / sizeof(wchar_t);
        size_t start = 0;

        for (size_t i = 0; i < units; i += 1) {
            if (at[i] != 0) {
                continue;
            }
            if (i == start) {
                break;
            }
            if (!hero_ui_win_keep_path(at + start, (int)(i - start), paths, count, &room)) {
                hero_layer_free(*paths);
                *paths = NULL;
                return HERO_UI_LIMIT;
            }
            start = i + 1;
        }
    } else {
        /* Paths of the system's code page, as an older program writes. */
        const char *at = (const char *)head + drop->pFiles;
        size_t bytes = size - drop->pFiles;
        size_t start = 0;

        for (size_t i = 0; i < bytes; i += 1) {
            int units;
            wchar_t *wide;

            if (at[i] != 0) {
                continue;
            }
            if (i == start) {
                break;
            }
            units = MultiByteToWideChar(CP_ACP, 0, at + start, (int)(i - start), NULL, 0);
            wide = units > 0 ? hero_layer_alloc((size_t)units * sizeof(wchar_t)) : NULL;
            if (wide == NULL || MultiByteToWideChar(CP_ACP, 0, at + start, (int)(i - start), wide, units) != units || !hero_ui_win_keep_path(wide, units, paths, count, &room)) {
                hero_layer_free(wide);
                hero_layer_free(*paths);
                *paths = NULL;
                return HERO_UI_LIMIT;
            }
            hero_layer_free(wide);
            start = i + 1;
        }
    }
    return *count > 0 ? HERO_UI_OK : HERO_UI_NOT_FOUND;
}

/* The pixels of a device-independent bitmap of `size` bytes at `head`, its
 * header a `BITMAPINFOHEADER` or a later one, as RGBA rows from the top
 * into `*pixels`: 32 bits a pixel, by its masks where it names them, its
 * alpha taken where any pixel has one (a bitmap of `BI_RGB` writes 0 in the
 * fourth byte and means opaque), or 24. `HERO_UI_NOT_FOUND` for any other
 * depth, which the caller then reads through GDI. */
static int64_t hero_ui_win_read_dib(const uint8_t *head, size_t size, uint8_t **pixels, int64_t *width, int64_t *height) {
    const BITMAPINFOHEADER *info = (const BITMAPINFOHEADER *)(const void *)head;
    DWORD masks[4] = {0x00FF0000u, 0x0000FF00u, 0x000000FFu, 0};
    size_t offset;
    size_t stride;
    int64_t wide;
    int64_t high;
    bool down;
    bool alpha = false;
    uint8_t *out;

    if (size < sizeof *info || info->biSize < sizeof *info || info->biSize > size || info->biWidth <= 0 || info->biHeight == 0 || info->biPlanes != 1) {
        return HERO_UI_NOT_FOUND;
    }
    if ((info->biBitCount != 32 && info->biBitCount != 24) || (info->biCompression != BI_RGB && info->biCompression != BI_BITFIELDS)) {
        return HERO_UI_NOT_FOUND;
    }
    offset = info->biSize;
    if (info->biCompression == BI_BITFIELDS) {
        if (info->biSize >= sizeof(BITMAPV4HEADER)) {
            const BITMAPV4HEADER *v4 = (const BITMAPV4HEADER *)(const void *)head;

            masks[0] = v4->bV4RedMask;
            masks[1] = v4->bV4GreenMask;
            masks[2] = v4->bV4BlueMask;
            masks[3] = v4->bV4AlphaMask;
        } else {
            if (size < offset + 12) {
                return HERO_UI_NOT_FOUND;
            }
            memcpy(masks, head + offset, 12);
            offset += 12;
        }
    }
    offset += (size_t)info->biClrUsed * 4;
    wide = info->biWidth;
    high = info->biHeight < 0 ? -(int64_t)info->biHeight : info->biHeight;
    down = info->biHeight < 0;
    stride = (((size_t)wide * info->biBitCount + 31) / 32) * 4;
    if (wide > 32768 || high > 32768 || offset > size || stride * (size_t)high > size - offset) {
        return HERO_UI_NOT_FOUND;
    }
    if ((uint64_t)wide * (uint64_t)high * 4 > (uint64_t)HERO_UI_CLIP_MOST) {
        return HERO_UI_LIMIT;
    }
    out = hero_layer_alloc((size_t)wide * (size_t)high * 4);
    if (out == NULL) {
        return HERO_UI_LIMIT;
    }
    for (int64_t y = 0; y < high; y += 1) {
        const uint8_t *row = head + offset + stride * (size_t)(down ? y : high - 1 - y);

        for (int64_t x = 0; x < wide; x += 1) {
            uint8_t *to = out + ((size_t)y * (size_t)wide + (size_t)x) * 4;

            if (info->biBitCount == 24) {
                to[0] = row[x * 3 + 2];
                to[1] = row[x * 3 + 1];
                to[2] = row[x * 3 + 0];
                to[3] = 255;
                continue;
            }
            {
                DWORD value;

                memcpy(&value, row + x * 4, 4);
                for (int c = 0; c < 4; c += 1) {
                    DWORD mask = c == 3 && masks[3] == 0 && info->biCompression == BI_RGB ? 0xFF000000u : masks[c];
                    unsigned shift = 0;

                    if (mask == 0) {
                        to[c] = 255;
                        continue;
                    }
                    while (((mask >> shift) & 1u) == 0) {
                        shift += 1;
                    }
                    to[c] = (uint8_t)(((value & mask) >> shift) * 255u / (mask >> shift));
                }
                alpha = alpha || to[3] != 0;
            }
        }
    }
    if (info->biBitCount == 32 && !alpha) {
        /* No pixel holds an alpha: the fourth byte is unused, as `BI_RGB`
         * writes it, and the image is opaque. */
        for (size_t i = 0; i < (size_t)wide * (size_t)high; i += 1) {
            out[i * 4 + 3] = 255;
        }
    }
    *pixels = out;
    *width = wide;
    *height = high;
    return HERO_UI_OK;
}

/* The pixels of the clipboard's `CF_BITMAP` read through GDI as 32 bits,
 * opaque, for a depth `hero_ui_win_read_dib` does not read. */
static int64_t hero_ui_win_read_bitmap(HBITMAP bitmap, uint8_t **pixels, int64_t *width, int64_t *height) {
    BITMAP shape;
    BITMAPINFO want;
    HDC screen;
    uint8_t *out;
    int rows;

    if (GetObjectW(bitmap, (int)sizeof shape, &shape) != (int)sizeof shape || shape.bmWidth <= 0 || shape.bmHeight <= 0) {
        return HERO_UI_NOT_FOUND;
    }
    if ((uint64_t)shape.bmWidth * (uint64_t)shape.bmHeight * 4 > (uint64_t)HERO_UI_CLIP_MOST) {
        return HERO_UI_LIMIT;
    }
    out = hero_layer_alloc((size_t)shape.bmWidth * (size_t)shape.bmHeight * 4);
    if (out == NULL) {
        return HERO_UI_LIMIT;
    }
    memset(&want, 0, sizeof want);
    want.bmiHeader.biSize = (DWORD)sizeof want.bmiHeader;
    want.bmiHeader.biWidth = shape.bmWidth;
    want.bmiHeader.biHeight = -shape.bmHeight;
    want.bmiHeader.biPlanes = 1;
    want.bmiHeader.biBitCount = 32;
    want.bmiHeader.biCompression = BI_RGB;
    screen = GetDC(NULL);
    rows = screen != NULL ? GetDIBits(screen, bitmap, 0, (UINT)shape.bmHeight, out, &want, DIB_RGB_COLORS) : 0;
    if (screen != NULL) {
        (void)ReleaseDC(NULL, screen);
    }
    if (rows != shape.bmHeight) {
        hero_layer_free(out);
        return HERO_UI_NOT_FOUND;
    }
    for (size_t i = 0; i < (size_t)shape.bmWidth * (size_t)shape.bmHeight; i += 1) {
        uint8_t blue = out[i * 4 + 0];

        out[i * 4 + 0] = out[i * 4 + 2];
        out[i * 4 + 2] = blue;
        out[i * 4 + 3] = 255;
    }
    *pixels = out;
    *width = shape.bmWidth;
    *height = shape.bmHeight;
    return HERO_UI_OK;
}

/* What the open clipboard holds of `kind`, into `*kept`. */
static int64_t hero_ui_win_read_clip(int64_t kind, uint8_t **kept, int64_t *bytes, int64_t *width, int64_t *height) {
    static const UINT images[] = {CF_DIBV5, CF_DIB};
    HANDLE data;
    const uint8_t *head;
    int64_t answer = HERO_UI_NOT_FOUND;

    if (kind == HERO_UI_CLIP_TEXT) {
        size_t size;

        data = IsClipboardFormatAvailable(CF_UNICODETEXT) ? GetClipboardData(CF_UNICODETEXT) : NULL;
        head = data != NULL ? GlobalLock(data) : NULL;
        if (head == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        size = GlobalSize(data) / sizeof(wchar_t);
        if (size > (size_t)HERO_UI_CLIP_MOST) {
            (void)GlobalUnlock(data);
            return HERO_UI_LIMIT;
        }
        {
            const wchar_t *text = (const wchar_t *)(const void *)head;
            size_t units = 0;

            while (units < size && text[units] != 0) {
                units += 1;
            }
            *kept = hero_ui_win_narrow(text, (int)units, true, bytes);
        }
        (void)GlobalUnlock(data);
        return *kept == NULL ? HERO_UI_LIMIT : *bytes > HERO_UI_CLIP_MOST ? HERO_UI_LIMIT : HERO_UI_OK;
    }
    if (kind == HERO_UI_CLIP_FILES) {
        data = IsClipboardFormatAvailable(CF_HDROP) ? GetClipboardData(CF_HDROP) : NULL;
        head = data != NULL ? GlobalLock(data) : NULL;
        if (head == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        answer = hero_ui_win_read_drop(head, GlobalSize(data), kept, bytes);
        (void)GlobalUnlock(data);
        return answer;
    }
    for (size_t i = 0; i < sizeof images / sizeof images[0] && answer == HERO_UI_NOT_FOUND; i += 1) {
        data = IsClipboardFormatAvailable(images[i]) ? GetClipboardData(images[i]) : NULL;
        head = data != NULL ? GlobalLock(data) : NULL;
        if (head != NULL) {
            answer = hero_ui_win_read_dib(head, GlobalSize(data), kept, width, height);
            (void)GlobalUnlock(data);
        }
    }
    if (answer == HERO_UI_NOT_FOUND && IsClipboardFormatAvailable(CF_BITMAP)) {
        data = GetClipboardData(CF_BITMAP);
        answer = data != NULL ? hero_ui_win_read_bitmap((HBITMAP)data, kept, width, height) : HERO_UI_NOT_FOUND;
    }
    if (answer == HERO_UI_OK) {
        *bytes = *width * *height * 4;
    }
    return answer;
}

int64_t hero_ui_arm_clipboard_get(int64_t kind, int64_t *bytes, int64_t *width, int64_t *height) {
    uint8_t *kept = NULL;
    int64_t answer = HERO_UI_OK;

    *bytes = 0;
    if (!hero_ui_win_open_clipboard(&answer)) {
        return answer;
    }
    answer = hero_ui_win_read_clip(kind, &kept, bytes, width, height);
    (void)CloseClipboard();
    if (answer != HERO_UI_OK) {
        hero_layer_free(kept);
        *width = 0;
        *height = 0;
        *bytes = 0;
        return answer;
    }
    hero_ui_win_keep_clip(kept, *bytes);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_clipboard_bytes(uint8_t *buffer, int64_t count) {
    if (hero_ui_win_clip_count < 0) {
        return -1;
    }
    if (hero_ui_win_clip_count > 0 && hero_ui_win_clip_count <= count) {
        memcpy(buffer, hero_ui_win_clip, (size_t)hero_ui_win_clip_count);
    }
    return hero_ui_win_clip_count;
}

/* -- DIALOGS ANSWERED THROUGH THE QUEUE -------------------------------------
 *
 * Every dialog of Win32 waits for the person inside the call that shows it,
 * running a loop of its own until answered: `MessageBoxW`; version 6 of the
 * common controls' task dialog, `TaskDialogIndirect`, a message box of the
 * program's buttons, found in the module the activation context gives (THE
 * SYSTEM'S CURRENT LOOK), so a program on a Windows without it still loads
 * and is told; `GetOpenFileNameW` and `GetSaveFileNameW`, which show the
 * system's current file dialog where no hook or template is asked and are
 * told `OFN_NOCHANGEDIR`, so the process's folder stays where it was; and
 * the shell's Common Item Dialog, `IFileOpenDialog` told `FOS_PICKFOLDERS`,
 * for a folder. So each dialog is shown by a thread of its own, ITS THREAD,
 * which waits for the person while the UI thread goes on, and `show_*`
 * returns once that thread is made. The dialog is owned by the window it
 * stands over, so Windows holds that window, and no other, disabled while
 * it stands, as a sheet holds its window on macOS; a second dialog over the
 * same window waits for its turn until the first is answered, as a second
 * sheet waits on macOS. Its thread joins COM as a single-threaded apartment
 * and takes the UI thread's activation context and awareness of scale,
 * which are each thread's own.
 *
 * WHAT CROSSES BETWEEN THE THREADS: what a dialog shows is made as UTF-16 by
 * the UI thread before its thread starts, and read by that thread alone
 * after; the answer is written by its thread alone, which then marks the
 * dialog `finished` and posts the UI thread's hidden window
 * `HERO_UI_WIN_DIALOG_DONE`, where the UI thread tells the answer and gives
 * the record back once the thread has ended. No record is given back while
 * its thread runs. Whether a dialog was ended unanswered is the UI thread's
 * to write and its thread's to read when its turn comes.
 *
 * NO WAIT OF THE UI THREAD FOR A DIALOG LEAVES THE MESSAGES SENT TO IT
 * UNREAD: a dialog owned by a window of the UI thread sends that window
 * messages and waits for them (`WM_ENABLE` as Windows disables it), so every
 * such wait here reads the messages sent to the UI thread meanwhile
 * (`PM_QS_SENDMESSAGE`), and never the ones posted, which `pump` reads.
 *
 * AN ANSWER AS THE PERSON'S is a press of the dialog's button through the
 * dialog's own window, the visible top-level dialog its thread shows: a task
 * dialog's `TDM_CLICK_BUTTON`, and the `WM_COMMAND` a click of a message
 * box's or a file dialog's button sends; the call returns once the dialog
 * has closed. A file or folder dialog is only cancelled so, no program
 * choosing in it. A dialog still waiting for its turn is answered at once,
 * as the person who answers it the moment it shows. ENDING a dialog marks it
 * ended, so its answer is told to no one, and closes it the same way. */

/* The class and the interface of the Common Item Dialog that opens, as
 * `shobjidl.h` names them, written here so no import library is needed. */
static const CLSID hero_ui_win_open_dialog_class = {0xDC1C5A9C, 0xE88A, 0x4DDE, {0xA5, 0xA1, 0x60, 0xF8, 0x2A, 0x20, 0xAE, 0xF7}};
static const IID hero_ui_win_open_dialog_face = {0xD57C7288, 0xD4AD, 0x4768, {0xBE, 0x02, 0x9D, 0x96, 0x95, 0x32, 0xD9, 0x60}};

#define HERO_UI_WIN_DIALOG_MESSAGE 1
#define HERO_UI_WIN_DIALOG_ASK 2
#define HERO_UI_WIN_DIALOG_FILES 3
#define HERO_UI_WIN_DIALOG_FOLDERS 4
#define HERO_UI_WIN_DIALOG_PRINT 5
/* How long the UI thread waits for a dialog's thread to put its dialog on
 * the screen, and for a dialog to close and its thread to end once its
 * button is pressed. */
#define HERO_UI_WIN_DIALOG_WAIT_MS 10000u

/* A dialog shown, or waiting for its turn over its window. */
typedef struct hero_ui_win_dialog {
    /* What it shows, made by the UI thread before its thread starts and read
     * by its thread alone after: its number and its window's, its kind, the
     * window it stands over, the UI thread's hidden window it tells once
     * done, its words, a message box's tone and buttons, an ask's buttons
     * and cancel button (-1 for none), a file dialog's ways, its filters in
     * Win32's spelling, the name it proposes and the first extension. */
    int64_t dialog;
    int64_t window;
    int kind;
    HWND owner;
    HWND host;
    int64_t tone;
    int64_t buttons;
    int64_t count;
    int64_t cancel;
    bool save;
    bool multiple;
    wchar_t *heading;
    wchar_t *said;
    wchar_t *labels[HERO_UI_ASK_BUTTONS_MOST];
    wchar_t *filters;
    wchar_t *name;
    wchar_t extension[64];
    /* A print's or a page setup's: 1 for the page setup, 0 for the print, the
     * web view whose page it is asked for first (0 for none), and the
     * settings' text, copied before its thread starts. */
    int64_t page;
    int64_t view;
    uint8_t *settings;
    int64_t settings_count;
    /* The UI thread's activation context (NULL for none), its awareness of
     * scale (NULL to leave the thread's) and the task dialog's function,
     * each a thread-local of the UI thread its thread cannot read. */
    HANDLE context;
    hero_ui_win_thread_awareness awareness;
    hero_ui_win_task_dialog task;
    /* Its thread, that thread's number, and the event giving it its turn. */
    HANDLE thread;
    DWORD thread_id;
    HANDLE turn;
    /* The answer, a button or how many paths were chosen, and the paths,
     * each followed by one zero byte: written by its thread before it marks
     * the dialog `finished`. */
    int64_t answer;
    uint8_t *paths;
    int64_t paths_count;
    atomic_bool finished;
    /* Ended unanswered by the UI thread. */
    atomic_bool ended;
    /* A print answered as its person would (`print_answer`): the answer and
     * the settings text the UI thread tells in place of what the dialog's
     * own thread read, written by the UI thread before it sets `injected`
     * and the dialog is closed. */
    atomic_bool injected;
    int64_t injected_value;
    uint8_t *injected_text;
    int64_t injected_count;
    /* The UI thread's own: whether its turn was given, whether its answer
     * was told. */
    bool started;
    bool told;
    struct hero_ui_win_dialog *next;
} hero_ui_win_dialog;

/* The dialogs shown or waiting, in the order shown. */
static _Thread_local hero_ui_win_dialog *hero_ui_win_dialogs;

/* Gives back dialog `dialog`'s record, its thread ended or never made. */
static void hero_ui_win_dialog_free(hero_ui_win_dialog *dialog) {
    if (dialog->thread != NULL) {
        (void)CloseHandle(dialog->thread);
    }
    if (dialog->turn != NULL) {
        (void)CloseHandle(dialog->turn);
    }
    hero_layer_free(dialog->heading);
    hero_layer_free(dialog->said);
    for (int64_t i = 0; i < HERO_UI_ASK_BUTTONS_MOST; i += 1) {
        hero_layer_free(dialog->labels[i]);
    }
    hero_layer_free(dialog->filters);
    hero_layer_free(dialog->name);
    hero_layer_free(dialog->paths);
    hero_layer_free(dialog->settings);
    hero_layer_free(dialog->injected_text);
    hero_layer_free(dialog);
}

/* Waits up to `ms` milliseconds for `handle`, reading the messages sent to
 * the UI thread meanwhile and none posted (NO WAIT ... UNREAD); whether it
 * came. */
static bool hero_ui_win_await(HANDLE handle, DWORD ms) {
    ULONGLONG deadline = GetTickCount64() + ms;

    for (;;) {
        MSG message;
        ULONGLONG now;
        DWORD woke;

        (void)PeekMessageW(&message, NULL, 0, 0, PM_NOREMOVE | PM_QS_SENDMESSAGE);
        now = GetTickCount64();
        woke = MsgWaitForMultipleObjectsEx(1, &handle, now >= deadline ? 0 : (DWORD)(deadline - now), QS_SENDMESSAGE, MWMO_INPUTAVAILABLE);
        if (woke == WAIT_OBJECT_0) {
            return true;
        }
        if (woke != WAIT_OBJECT_0 + 1) {
            return false;
        }
    }
}

/* How many paths `bytes` bytes of paths hold, each followed by one zero. */
static int64_t hero_ui_win_paths_in(const uint8_t *paths, int64_t bytes) {
    int64_t count = 0;

    for (int64_t at = 0; at < bytes; at += 1) {
        count += paths[at] == 0 ? 1 : 0;
    }
    return count;
}

/* Keeps the file-system path of shell item `item` at the end of the paths
 * answered; false where memory ran out. An item with no such path is passed
 * over, `FOS_FORCEFILESYSTEM` letting the person choose none. */
static bool hero_ui_win_keep_item(IShellItem *item, uint8_t **paths, int64_t *count, int64_t *room) {
    wchar_t *path = NULL;
    bool kept = true;

    if (SUCCEEDED(item->lpVtbl->GetDisplayName(item, SIGDN_FILESYSPATH, &path)) && path != NULL) {
        kept = hero_ui_win_keep_path(path, (int)wcslen(path), paths, count, room);
        CoTaskMemFree(path);
    }
    return kept;
}

/* Shows message box `dialog` on its thread: its answer `HERO_UI_ANSWER_*`,
 * Cancel where Windows answered none. A box of OK alone answers OK to
 * Escape, as `MessageBoxW` does. */
static void hero_ui_win_show_box(hero_ui_win_dialog *dialog) {
    UINT style = dialog->tone == HERO_UI_TONE_ERROR ? MB_ICONERROR : (dialog->tone == HERO_UI_TONE_WARNING ? MB_ICONWARNING : MB_ICONINFORMATION);
    int pressed;

    style |= dialog->buttons == HERO_UI_BUTTONS_OK_CANCEL ? MB_OKCANCEL : dialog->buttons == HERO_UI_BUTTONS_YES_NO ? MB_YESNO : dialog->buttons == HERO_UI_BUTTONS_YES_NO_CANCEL ? MB_YESNOCANCEL : MB_OK;
    pressed = MessageBoxW(dialog->owner, dialog->said, dialog->heading, style | MB_SETFOREGROUND);
    dialog->answer = pressed == IDOK ? HERO_UI_ANSWER_OK : pressed == IDYES ? HERO_UI_ANSWER_YES : pressed == IDNO ? HERO_UI_ANSWER_NO : HERO_UI_ANSWER_CANCEL;
}

/* Shows ask `dialog` on its thread: its answer the index of the button
 * pressed, its cancel button for Escape and the close box, and that, -1
 * where it has none, where no task dialog could be shown. */
static void hero_ui_win_show_ask(hero_ui_win_dialog *dialog) {
    TASKDIALOG_BUTTON choices[HERO_UI_ASK_BUTTONS_MOST];
    TASKDIALOGCONFIG config;
    int button = 0;

    for (int64_t i = 0; i < dialog->count; i += 1) {
        choices[i].nButtonID = (int)(HERO_UI_WIN_ASK_FIRST + i);
        choices[i].pszButtonText = dialog->labels[i];
    }
    memset(&config, 0, sizeof config);
    config.cbSize = (UINT)sizeof config;
    config.hwndParent = dialog->owner;
    config.hInstance = GetModuleHandleW(NULL);
    config.dwFlags = TDF_POSITION_RELATIVE_TO_WINDOW | (dialog->cancel >= 0 ? TDF_ALLOW_DIALOG_CANCELLATION : 0);
    config.pszWindowTitle = dialog->heading;
    config.pszMainIcon = dialog->tone == HERO_UI_TONE_ERROR ? TD_ERROR_ICON : dialog->tone == HERO_UI_TONE_WARNING ? TD_WARNING_ICON : TD_INFORMATION_ICON;
    config.pszContent = dialog->said;
    config.cButtons = (UINT)dialog->count;
    config.pButtons = choices;
    config.nDefaultButton = HERO_UI_WIN_ASK_FIRST;
    if (FAILED(dialog->task(&config, &button, NULL, NULL))) {
        button = IDCANCEL;
    }
    dialog->answer = button >= HERO_UI_WIN_ASK_FIRST && button < HERO_UI_WIN_ASK_FIRST + dialog->count ? button - HERO_UI_WIN_ASK_FIRST : dialog->cancel;
}

/* Shows file dialog `dialog` on its thread: the paths chosen, none where
 * the person cancelled or Windows refused. Several files chosen are their
 * folder, then each name, each followed by a zero, and one more after the
 * last; one file is its whole path. */
static void hero_ui_win_show_files(hero_ui_win_dialog *dialog) {
    OPENFILENAMEW request;
    wchar_t *answer = hero_layer_alloc_zero((size_t)HERO_UI_WIN_PATHS_ROOM, sizeof(wchar_t));
    int64_t room = 0;
    bool kept = true;

    if (answer == NULL) {
        return;
    }
    if (dialog->save && wcslen(dialog->name) < HERO_UI_WIN_PATHS_ROOM) {
        memcpy(answer, dialog->name, (wcslen(dialog->name) + 1) * sizeof(wchar_t));
    }
    memset(&request, 0, sizeof request);
    request.lStructSize = (DWORD)sizeof request;
    request.hwndOwner = dialog->owner;
    request.lpstrFilter = dialog->filters;
    request.nFilterIndex = dialog->filters != NULL ? 1 : 0;
    request.lpstrFile = answer;
    request.nMaxFile = HERO_UI_WIN_PATHS_ROOM;
    request.lpstrTitle = dialog->heading[0] != 0 ? dialog->heading : NULL;
    request.lpstrDefExt = dialog->save && dialog->extension[0] != 0 ? dialog->extension : NULL;
    request.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | (dialog->save ? OFN_OVERWRITEPROMPT : OFN_FILEMUSTEXIST | (dialog->multiple ? OFN_ALLOWMULTISELECT : 0));
    if (dialog->save ? GetSaveFileNameW(&request) : GetOpenFileNameW(&request)) {
        size_t first = wcslen(answer);
        bool several = dialog->multiple && !dialog->save && answer[first + 1] != 0;

        if (!several) {
            kept = hero_ui_win_keep_path(answer, (int)first, &dialog->paths, &dialog->paths_count, &room);
        }
        for (size_t at = first + 1; several && kept && answer[at] != 0; at += wcslen(answer + at) + 1) {
            size_t length = wcslen(answer + at);
            wchar_t *joined = hero_layer_alloc((first + length + 2) * sizeof(wchar_t));

            if (joined == NULL) {
                kept = false;
                break;
            }
            memcpy(joined, answer, first * sizeof(wchar_t));
            joined[first] = first > 0 && answer[first - 1] == L'\\' ? 0 : L'\\';
            memcpy(joined + first + (joined[first] == 0 ? 0 : 1), answer + at, length * sizeof(wchar_t));
            kept = hero_ui_win_keep_path(joined, (int)(first + length + (joined[first] == 0 ? 0 : 1)), &dialog->paths, &dialog->paths_count, &room);
            hero_layer_free(joined);
        }
    }
    if (!kept) {
        hero_layer_free(dialog->paths);
        dialog->paths = NULL;
        dialog->paths_count = 0;
    }
    hero_layer_free(answer);
}

/* Shows folder dialog `dialog` on its thread: the folders chosen, none
 * where the person cancelled or Windows refused. */
static void hero_ui_win_show_folders(hero_ui_win_dialog *dialog) {
    IFileOpenDialog *shell = NULL;
    FILEOPENDIALOGOPTIONS options = 0;
    HRESULT shown = CoCreateInstance(&hero_ui_win_open_dialog_class, NULL, CLSCTX_INPROC_SERVER, &hero_ui_win_open_dialog_face, (void **)&shell);
    int64_t room = 0;
    bool kept = true;

    if (FAILED(shown) || shell == NULL) {
        return;
    }
    (void)shell->lpVtbl->GetOptions(shell, &options);
    options |= FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR | (dialog->multiple ? FOS_ALLOWMULTISELECT : 0);
    (void)shell->lpVtbl->SetOptions(shell, options);
    if (dialog->heading[0] != 0) {
        (void)shell->lpVtbl->SetTitle(shell, dialog->heading);
    }
    if (SUCCEEDED(shell->lpVtbl->Show(shell, dialog->owner))) {
        IShellItemArray *chosen = NULL;
        DWORD each = 0;

        if (SUCCEEDED(shell->lpVtbl->GetResults(shell, &chosen)) && chosen != NULL) {
            (void)chosen->lpVtbl->GetCount(chosen, &each);
            for (DWORD i = 0; i < each && kept; i += 1) {
                IShellItem *item = NULL;

                if (SUCCEEDED(chosen->lpVtbl->GetItemAt(chosen, i, &item)) && item != NULL) {
                    kept = hero_ui_win_keep_item(item, &dialog->paths, &dialog->paths_count, &room);
                    item->lpVtbl->Release(item);
                }
            }
            chosen->lpVtbl->Release(chosen);
        }
    }
    shell->lpVtbl->Release(shell);
    if (!kept) {
        hero_layer_free(dialog->paths);
        dialog->paths = NULL;
        dialog->paths_count = 0;
    }
}

/* The dialog whose print the thread running it shows, which the print
 * member's `push` writes the answer into: the member queues its answer where
 * this arm keeps it for the UI thread to tell. */
static _Thread_local hero_ui_win_dialog *hero_ui_win_printing;

static void hero_ui_win_print_kept(int64_t kind, int64_t dialog, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_win_dialog *own = hero_ui_win_printing;

    (void)kind;
    (void)dialog;
    (void)second;
    if (own == NULL) {
        return;
    }
    hero_layer_free(own->paths);
    own->paths = NULL;
    own->paths_count = 0;
    own->answer = value == 1 ? 1 : 0;
    if (value == 1 && text != NULL && count > 0) {
        own->paths = hero_layer_alloc((size_t)count);
        if (own->paths == NULL) {
            /* Settings no memory held are no choice that can be told whole. */
            own->answer = 0;
            return;
        }
        memcpy(own->paths, text, (size_t)count);
        own->paths_count = count;
    }
}

/* Shows print dialog or page setup `dialog` on its thread, which the
 * member's own call waits in until the person answers: the choice and the
 * settings chosen, or a cancel, a failure being told as one. */
static void hero_ui_win_show_print(hero_ui_win_dialog *dialog) {
    hero_ui_win_printing = dialog;
    if (hero_ui_print_win32_show(dialog->owner, dialog->dialog, dialog->page, dialog->settings, dialog->settings_count, hero_ui_win_print_kept) != HERO_UI_OK) {
        dialog->answer = 0;
    }
    hero_ui_win_printing = NULL;
}

/* A dialog's thread (ITS THREAD): takes the UI thread's look and scale,
 * waits for its turn, shows its dialog unless it was ended meanwhile, and
 * tells the UI thread it is done. Past `finished` it reads nothing of the
 * record, which the UI thread may give back once the thread has ended. */
static DWORD WINAPI hero_ui_win_dialog_thread(LPVOID given) {
    hero_ui_win_dialog *dialog = given;
    HWND host = dialog->host;
    HRESULT joined = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ULONG_PTR cookie = 0;
    bool activated = dialog->context != NULL && ActivateActCtx(dialog->context, &cookie) != FALSE;

    if (dialog->awareness != NULL && dialog->awareness(HERO_UI_WIN_AWARE_V2) == NULL) {
        (void)dialog->awareness(HERO_UI_WIN_AWARE);
    }
    if (WaitForSingleObject(dialog->turn, INFINITE) == WAIT_OBJECT_0 && !atomic_load(&dialog->ended)) {
        if (dialog->kind == HERO_UI_WIN_DIALOG_MESSAGE) {
            hero_ui_win_show_box(dialog);
        } else if (dialog->kind == HERO_UI_WIN_DIALOG_ASK) {
            hero_ui_win_show_ask(dialog);
        } else if (dialog->kind == HERO_UI_WIN_DIALOG_PRINT) {
            hero_ui_win_show_print(dialog);
        } else {
            if (dialog->kind == HERO_UI_WIN_DIALOG_FILES) {
                hero_ui_win_show_files(dialog);
            } else {
                hero_ui_win_show_folders(dialog);
            }
            dialog->answer = hero_ui_win_paths_in(dialog->paths, dialog->paths_count);
        }
    }
    if (activated) {
        (void)DeactivateActCtx(0, cookie);
    }
    if (SUCCEEDED(joined)) {
        CoUninitialize();
    }
    atomic_store(&dialog->finished, true);
    (void)PostMessageW(host, HERO_UI_WIN_DIALOG_DONE, 0, 0);
    return 0;
}

/* Gives its turn to each dialog waiting where no dialog shown before it
 * over its window still stands. */
static void hero_ui_win_dialogs_turn(void) {
    for (hero_ui_win_dialog *each = hero_ui_win_dialogs; each != NULL; each = each->next) {
        bool waits = false;

        if (each->started) {
            continue;
        }
        for (hero_ui_win_dialog *before = hero_ui_win_dialogs; before != each && !waits; before = before->next) {
            waits = before->window == each->window && !atomic_load(&before->finished);
        }
        if (!waits) {
            each->started = true;
            (void)SetEvent(each->turn);
        }
    }
}

static bool hero_ui_win_print_page(int64_t view, int64_t dialog, const uint8_t *settings, int64_t count);

/* Tells the answer of dialog `each`, whose thread finished: what its thread
 * read, or what a print answered as its person (`print_answer`) gave in its
 * place. A web view's print asked first is not told a choice: the page goes
 * to WebView2 with the settings chosen, which tells how it went once it has
 * handed the page on; a failure to start that is told as a cancel. */
static void hero_ui_win_dialog_told(hero_ui_win_dialog *each) {
    int64_t answer = each->answer;
    const uint8_t *text = each->paths;
    int64_t count = each->paths_count;

    if (each->kind == HERO_UI_WIN_DIALOG_PRINT) {
        if (atomic_load(&each->injected)) {
            answer = each->injected_value;
            text = each->injected_text;
            count = each->injected_count;
        }
        if (each->view != 0 && answer == 1) {
            if (hero_ui_win_print_page(each->view, each->dialog, text, count)) {
                return;
            }
            answer = 0;
        }
    }
    if (count > HERO_UI_TEXT_MOST) {
        answer = 0;
    }
    hero_ui_tell_text(HERO_UI_EVENT_DIALOG, each->dialog, answer, 0, answer > 0 && count > 0 ? text : NULL, answer > 0 ? count : 0);
}

/* Tells the answer of each dialog whose thread finished, unless it was
 * ended or told already, gives back each whose thread has ended, and gives
 * the dialogs waiting behind them their turn (`HERO_UI_WIN_DIALOG_DONE`).
 * Paths past what an event's text holds are told as a cancel, which no
 * program then mistakes for the whole choice. */
static void hero_ui_win_dialogs_reap(void) {
    hero_ui_win_dialog **at = &hero_ui_win_dialogs;

    while (*at != NULL) {
        hero_ui_win_dialog *each = *at;

        if (!atomic_load(&each->finished)) {
            at = &each->next;
            continue;
        }
        if (!each->told && !atomic_load(&each->ended)) {
            hero_ui_win_dialog_told(each);
        }
        each->told = true;
        /* Its thread does nothing past `finished` but post and return. */
        if (!hero_ui_win_await(each->thread, HERO_UI_WIN_DIALOG_WAIT_MS)) {
            at = &each->next;
            continue;
        }
        *at = each->next;
        hero_ui_win_dialog_free(each);
    }
    hero_ui_win_dialogs_turn();
}

/* Whether the visible, enabled top-level dialog (`#32770`) of a thread is
 * `hwnd`, which `found` then holds: so a box a file dialog shows over
 * itself, which holds the file dialog disabled, is found before it. */
static BOOL CALLBACK hero_ui_win_dialog_found(HWND hwnd, LPARAM given) {
    wchar_t name[8];
    HWND *found = (HWND *)given;

    if (IsWindowVisible(hwnd) && IsWindowEnabled(hwnd) && GetClassNameW(hwnd, name, 8) == 6 && wcscmp(name, L"#32770") == 0) {
        *found = hwnd;
        return FALSE;
    }
    return TRUE;
}

/* Dialog `dialog`'s own window, waited for until its thread has put it on
 * the screen; NULL where its thread finished first or the wait passed. */
static HWND hero_ui_win_dialog_window(hero_ui_win_dialog *dialog) {
    ULONGLONG deadline = GetTickCount64() + HERO_UI_WIN_DIALOG_WAIT_MS;

    for (;;) {
        HWND found = NULL;

        if (atomic_load(&dialog->finished)) {
            return NULL;
        }
        (void)EnumThreadWindows(dialog->thread_id, hero_ui_win_dialog_found, (LPARAM)&found);
        if (found != NULL) {
            return found;
        }
        if (GetTickCount64() >= deadline) {
            return NULL;
        }
        (void)hero_ui_win_await(dialog->thread, 10);
    }
}

/* Presses button `command` of dialog `dialog`, on the screen as `shown`, as
 * the person's click does, and waits until the dialog has closed and its
 * thread ended; whether it did. */
static bool hero_ui_win_dialog_press(hero_ui_win_dialog *dialog, HWND shown, int command) {
    DWORD_PTR result = 0;

    if (dialog->kind == HERO_UI_WIN_DIALOG_ASK) {
        (void)SendMessageTimeoutW(shown, TDM_CLICK_BUTTON, (WPARAM)command, 0, SMTO_NORMAL, HERO_UI_WIN_DIALOG_WAIT_MS, &result);
    } else {
        (void)SendMessageTimeoutW(shown, WM_COMMAND, MAKEWPARAM(command, BN_CLICKED), (LPARAM)GetDlgItem(shown, command), SMTO_NORMAL, HERO_UI_WIN_DIALOG_WAIT_MS, &result);
    }
    return hero_ui_win_await(dialog->thread, HERO_UI_WIN_DIALOG_WAIT_MS);
}

/* The command of the button answering `answer` in a message box of buttons
 * `buttons`, 0 where it holds none. A box of OK alone gives its one button
 * `IDCANCEL`'s command, which closes it answering `IDOK`, so that Escape
 * answers OK (measured on Windows Server 2025, 2026-11-23). */
static int hero_ui_win_box_button(int64_t buttons, int64_t answer) {
    switch (answer) {
    case HERO_UI_ANSWER_OK:
        return buttons == HERO_UI_BUTTONS_OK ? IDCANCEL : buttons == HERO_UI_BUTTONS_OK_CANCEL ? IDOK : 0;
    case HERO_UI_ANSWER_CANCEL:
        return buttons == HERO_UI_BUTTONS_OK_CANCEL || buttons == HERO_UI_BUTTONS_YES_NO_CANCEL ? IDCANCEL : 0;
    case HERO_UI_ANSWER_YES:
        return buttons == HERO_UI_BUTTONS_YES_NO || buttons == HERO_UI_BUTTONS_YES_NO_CANCEL ? IDYES : 0;
    case HERO_UI_ANSWER_NO:
        return buttons == HERO_UI_BUTTONS_YES_NO || buttons == HERO_UI_BUTTONS_YES_NO_CANCEL ? IDNO : 0;
    default:
        return 0;
    }
}

/* The command that closes dialog `dialog` unanswered: an ask's cancel
 * button, else its first; a message box's No where it holds no Cancel,
 * else the command of its Cancel or of its one OK; a file or folder
 * dialog's Cancel. */
static int hero_ui_win_dismissal(const hero_ui_win_dialog *dialog) {
    if (dialog->kind == HERO_UI_WIN_DIALOG_ASK) {
        return (int)(HERO_UI_WIN_ASK_FIRST + (dialog->cancel >= 0 ? dialog->cancel : 0));
    }
    return dialog->kind == HERO_UI_WIN_DIALOG_MESSAGE && dialog->buttons == HERO_UI_BUTTONS_YES_NO ? IDNO : IDCANCEL;
}

/* Whether version 6 of the common controls' task dialog was found, looked
 * for once: under the activation context the name is version 6's module. */
static bool hero_ui_win_task_found(void) {
    if (!hero_ui_win_task_asked) {
        HMODULE controls = LoadLibraryW(L"comctl32.dll");

        hero_ui_win_task_asked = true;
        if (controls != NULL) {
            hero_ui_win_find_function(controls, "TaskDialogIndirect", &hero_ui_win_task, sizeof hero_ui_win_task);
        }
    }
    return hero_ui_win_task != NULL;
}

/* The record of a dialog `dialog` of kind `kind` over window `window`,
 * nothing of it shown yet: NULL with `*answer` the kind where `window` is
 * no window or a dialog of that number still stands, or memory ran out. */
static hero_ui_win_dialog *hero_ui_win_dialog_new(int64_t dialog, int64_t window, int kind, int64_t *answer) {
    hero_ui_native *over = hero_ui_find(window);
    hero_ui_win_dialog *made;

    for (hero_ui_win_dialog *each = hero_ui_win_dialogs; each != NULL; each = each->next) {
        if (each->dialog == dialog && !each->told && !atomic_load(&each->ended)) {
            *answer = HERO_UI_NOT_FOUND;
            return NULL;
        }
    }
    if (over == NULL || over->kind != HERO_UI_WINDOW || over->hwnd == NULL) {
        *answer = over == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
        return NULL;
    }
    made = hero_layer_alloc_zero(1, sizeof *made);
    if (made == NULL) {
        *answer = HERO_UI_LIMIT;
        return NULL;
    }
    made->dialog = dialog;
    made->window = window;
    made->kind = kind;
    made->owner = over->hwnd;
    made->cancel = -1;
    made->context = hero_ui_win_activated ? hero_ui_win_context : NULL;
    made->awareness = hero_ui_win_aware ? hero_ui_win_awareness : NULL;
    made->task = hero_ui_win_task;
    atomic_init(&made->finished, false);
    atomic_init(&made->ended, false);
    atomic_init(&made->injected, false);
    return made;
}

/* Gives back dialog `made`, which memory ran out making: `HERO_UI_LIMIT`. */
static int64_t hero_ui_win_dialog_unmade(hero_ui_win_dialog *made) {
    hero_ui_win_dialog_free(made);
    return HERO_UI_LIMIT;
}

/* Makes dialog `made`'s thread, keeps the dialog at the end of the dialogs
 * and gives it its turn where none stands over its window: `HERO_UI_OK`,
 * else the record given back and the failure said. */
static int64_t hero_ui_win_dialog_start(hero_ui_win_dialog *made) {
    hero_ui_win_dialog **at = &hero_ui_win_dialogs;
    int64_t answer = HERO_UI_OK;

    if (hero_ui_win_dialog_host == NULL) {
        hero_ui_win_dialog_host = CreateWindowExW(0, hero_ui_win_panel_class, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandleW(NULL), NULL);
    }
    if (hero_ui_win_dialog_host == NULL) {
        answer = hero_ui_win_failed("making the window a dialog's answer comes to", GetLastError());
    } else if ((made->turn = CreateEventW(NULL, TRUE, FALSE, NULL)) == NULL) {
        answer = hero_ui_win_failed("making the event a dialog waits for its turn on", GetLastError());
    } else {
        made->host = hero_ui_win_dialog_host;
        made->thread = CreateThread(NULL, 0, hero_ui_win_dialog_thread, made, 0, &made->thread_id);
        if (made->thread == NULL) {
            answer = hero_ui_win_failed("making the thread a dialog is shown by", GetLastError());
        }
    }
    if (answer != HERO_UI_OK) {
        hero_ui_win_dialog_free(made);
        return answer;
    }
    while (*at != NULL) {
        at = &(*at)->next;
    }
    *at = made;
    hero_ui_win_dialogs_turn();
    return HERO_UI_OK;
}

/* Win32's spelling of the filters `filters` holds, each a name and its
 * extensions with `;` between, as `lpstrFilter` takes them, into `made`:
 * each filter's name, then each extension as `*.ext`, `;` between, each
 * followed by a zero, and one more after the last; an extension written
 * with its dot keeps one dot. The first extension named is the one a save
 * adds to a name the person typed without one. None where the filters are
 * fewer than one name and its list; false where memory ran out. */
static bool hero_ui_win_spell_filters(const uint8_t *filters, int64_t filters_count, hero_ui_win_dialog *made) {
    int64_t texts = hero_ui_texts_in(filters, filters_count);
    size_t room = (size_t)filters_count * 3 + 4;
    size_t at = 0;
    wchar_t *spelled;

    made->extension[0] = 0;
    if (texts < 2) {
        return true;
    }
    spelled = hero_layer_alloc(room * sizeof(wchar_t));
    for (int64_t f = 0; spelled != NULL && f + 1 < texts; f += 2) {
        int64_t name_length = 0;
        int64_t list_length = 0;
        const uint8_t *filter_name = hero_ui_text_at(filters, filters_count, f, &name_length);
        const uint8_t *list = hero_ui_text_at(filters, filters_count, f + 1, &list_length);
        wchar_t *wide_name = hero_ui_win_wide(filter_name, name_length, 0);
        wchar_t *wide_list = hero_ui_win_wide(list, list_length, 0);

        if (wide_name == NULL || wide_list == NULL) {
            hero_layer_free(wide_name);
            hero_layer_free(wide_list);
            hero_layer_free(spelled);
            spelled = NULL;
            break;
        }
        for (size_t c = 0; wide_name[c] != 0; c += 1) {
            spelled[at++] = wide_name[c];
        }
        spelled[at++] = 0;
        for (size_t c = 0, start = 0;; c += 1) {
            if (wide_list[c] == L';' || wide_list[c] == 0) {
                size_t from = start < c && wide_list[start] == L'.' ? start + 1 : start;

                if (from < c) {
                    if (spelled[at - 1] != 0) {
                        spelled[at++] = L';';
                    }
                    spelled[at++] = L'*';
                    spelled[at++] = L'.';
                    for (size_t e = from; e < c; e += 1) {
                        spelled[at++] = wide_list[e];
                    }
                    if (made->extension[0] == 0 && c - from < 63) {
                        memcpy(made->extension, wide_list + from, (c - from) * sizeof(wchar_t));
                        made->extension[c - from] = 0;
                    }
                }
                start = c + 1;
            }
            if (wide_list[c] == 0) {
                break;
            }
        }
        if (spelled[at - 1] == 0) {
            spelled[at++] = L'*';
            spelled[at++] = L'.';
            spelled[at++] = L'*';
        }
        spelled[at++] = 0;
        hero_layer_free(wide_name);
        hero_layer_free(wide_list);
    }
    if (spelled == NULL) {
        return false;
    }
    spelled[at] = 0;
    made->filters = spelled;
    return true;
}

int64_t hero_ui_arm_show_message(int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count) {
    int64_t answer = HERO_UI_OK;
    hero_ui_win_dialog *made = hero_ui_win_dialog_new(dialog, window, HERO_UI_WIN_DIALOG_MESSAGE, &answer);

    if (made == NULL) {
        return answer;
    }
    made->tone = tone;
    made->buttons = buttons;
    made->heading = hero_ui_win_wide(title, title_count, 0);
    made->said = hero_ui_win_wide(text, text_count, HERO_UI_WIN_LINES);
    return made->heading == NULL || made->said == NULL ? hero_ui_win_dialog_unmade(made) : hero_ui_win_dialog_start(made);
}

int64_t hero_ui_arm_show_ask(int64_t dialog, int64_t window, int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel) {
    int64_t answer = HERO_UI_OK;
    int64_t count = hero_ui_texts_in(buttons, buttons_count);
    hero_ui_win_dialog *made;
    bool whole;

    if (!hero_ui_win_task_found()) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "Windows' task dialog, which shows buttons of the program's words, is version 6 of the common controls', which this process could not load");
    }
    if (count < 1 || count > HERO_UI_ASK_BUTTONS_MOST) {
        return HERO_UI_INVALID;
    }
    made = hero_ui_win_dialog_new(dialog, window, HERO_UI_WIN_DIALOG_ASK, &answer);
    if (made == NULL) {
        return answer;
    }
    made->tone = tone;
    made->count = count;
    made->cancel = cancel;
    made->heading = hero_ui_win_wide(title, title_count, 0);
    made->said = hero_ui_win_wide(text, text_count, HERO_UI_WIN_LINES);
    whole = made->heading != NULL && made->said != NULL;
    for (int64_t i = 0; i < count; i += 1) {
        int64_t length = 0;
        const uint8_t *label = hero_ui_text_at(buttons, buttons_count, i, &length);

        made->labels[i] = hero_ui_win_wide(label, length, HERO_UI_WIN_PREFIX);
        whole = whole && made->labels[i] != NULL;
    }
    return whole ? hero_ui_win_dialog_start(made) : hero_ui_win_dialog_unmade(made);
}

int64_t hero_ui_arm_show_files(int64_t dialog, int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count) {
    int64_t answer = HERO_UI_OK;
    hero_ui_win_dialog *made = hero_ui_win_dialog_new(dialog, window, HERO_UI_WIN_DIALOG_FILES, &answer);
    bool whole;

    if (made == NULL) {
        return answer;
    }
    made->save = save == 1;
    made->multiple = multiple == 1;
    made->heading = hero_ui_win_wide(title, title_count, 0);
    made->name = hero_ui_win_wide(name, name_count, 0);
    whole = made->heading != NULL && made->name != NULL && hero_ui_win_spell_filters(filters, filters_count, made);
    return whole ? hero_ui_win_dialog_start(made) : hero_ui_win_dialog_unmade(made);
}

int64_t hero_ui_arm_show_folders(int64_t dialog, int64_t window, int64_t multiple, const uint8_t *title, int64_t title_count) {
    int64_t answer = HERO_UI_OK;
    hero_ui_win_dialog *made = hero_ui_win_dialog_new(dialog, window, HERO_UI_WIN_DIALOG_FOLDERS, &answer);

    if (made == NULL) {
        return answer;
    }
    made->multiple = multiple == 1;
    made->heading = hero_ui_win_wide(title, title_count, 0);
    return made->heading == NULL ? hero_ui_win_dialog_unmade(made) : hero_ui_win_dialog_start(made);
}

int64_t hero_ui_arm_answer_dialog(int64_t dialog, int64_t value, const uint8_t *paths, int64_t count) {
    hero_ui_win_dialog *shown = NULL;
    HWND window;
    int command;

    (void)paths;
    (void)count;
    for (hero_ui_win_dialog *each = hero_ui_win_dialogs; each != NULL && shown == NULL; each = each->next) {
        if (each->dialog == dialog && !each->told && !atomic_load(&each->ended) && !atomic_load(&each->finished)) {
            shown = each;
        }
    }
    if (shown == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (shown->kind == HERO_UI_WIN_DIALOG_FILES || shown->kind == HERO_UI_WIN_DIALOG_FOLDERS) {
        if (value != 0) {
            return hero_ui_refused(HERO_UI_UNSUPPORTED, "Windows' file and folder dialogs take their choice from the person alone: no program chooses in them, though one may cancel them");
        }
        command = IDCANCEL;
    } else if (shown->kind == HERO_UI_WIN_DIALOG_MESSAGE) {
        command = hero_ui_win_box_button(shown->buttons, value);
    } else {
        command = value >= 0 && value < shown->count ? (int)(HERO_UI_WIN_ASK_FIRST + value) : 0;
    }
    if (command == 0) {
        return HERO_UI_INVALID;
    }
    if (!shown->started) {
        /* Still waiting for its turn: told now, and its thread ends without
         * showing it. */
        hero_ui_tell_text(HERO_UI_EVENT_DIALOG, shown->dialog, value, 0, NULL, 0);
        shown->told = true;
        atomic_store(&shown->ended, true);
        shown->started = true;
        (void)SetEvent(shown->turn);
        return HERO_UI_OK;
    }
    window = hero_ui_win_dialog_window(shown);
    if (window == NULL) {
        return atomic_load(&shown->finished) ? HERO_UI_NOT_FOUND : hero_ui_refused(HERO_UI_OTHER, "the dialog's thread put no dialog on the screen within ten seconds");
    }
    if (!hero_ui_win_dialog_press(shown, window, command)) {
        return hero_ui_refused(HERO_UI_OTHER, "the dialog did not close within ten seconds of its button pressed");
    }
    return HERO_UI_OK;
}

void hero_ui_arm_dialogs_end(int64_t window) {
    for (hero_ui_win_dialog *each = hero_ui_win_dialogs; each != NULL; each = each->next) {
        if ((window != 0 && each->window != window) || atomic_load(&each->ended)) {
            continue;
        }
        atomic_store(&each->ended, true);
        if (!each->started) {
            each->started = true;
            (void)SetEvent(each->turn);
        } else if (!atomic_load(&each->finished)) {
            HWND shown = hero_ui_win_dialog_window(each);

            if (shown != NULL) {
                (void)hero_ui_win_dialog_press(each, shown, hero_ui_win_dismissal(each));
            }
        }
    }
    /* Each gone before its window goes, which owns it. */
    for (hero_ui_win_dialog *each = hero_ui_win_dialogs; each != NULL; each = each->next) {
        if (window == 0 || each->window == window) {
            (void)hero_ui_win_await(each->thread, HERO_UI_WIN_DIALOG_WAIT_MS);
        }
    }
    hero_ui_win_dialogs_reap();
    if (window == 0) {
        /* A thread that would not end within the waits keeps its record,
         * which nothing gives back, so nothing it reads is freed under it. */
        hero_ui_win_dialogs = NULL;
    }
}

int64_t hero_ui_arm_dialogs_words(char *words, int64_t count) {
    return hero_ui_arm_words(words, count);
}

/* -- THE PRINT AND PAGE-SETUP DIALOGS ---------------------------------------
 *
 * The system's print dialog and page setup are `hero_ui_print_win32.c`'s, and
 * each waits for its person inside its call (`PrintDlgExW`, `PageSetupDlgW`),
 * so this arm shows each on a thread of its own, as every dialog above is: a
 * dialog of kind `HERO_UI_WIN_DIALOG_PRINT` over its window, in the same list,
 * its turn and its ending, which tells `HERO_UI_EVENT_DIALOG` with the
 * settings chosen (value 1) or none (value 0, a cancel or a failure to show).
 * The settings are checked on the calling thread first, so a text that is no
 * settings and a printer no spooler names are refused by the call itself.
 * An answer as the person's (`print_answer`) closes the dialog the way a
 * Cancel does and tells the answer given, the settings text of its own: the
 * dialog's own fields are the person's to set, and no program sets them.
 *
 * A WEB VIEW'S PAGE is WebView2's `Print`, which runs on the UI thread, whose
 * completion `hero_ui_print_win32_web` queues itself: where the person is
 * asked first, the dialog above is shown with the view's number kept, and its
 * choice, not told, starts the print. Whether the words of the last refusal
 * are the print member's or this arm's is `hero_ui_win_print_member`'s. */

static _Thread_local bool hero_ui_win_print_member;

/* Refuses with the print member's words, `kind` being what it answered. */
static int64_t hero_ui_win_print_refused(int64_t kind) {
    hero_ui_win_print_member = true;
    return kind;
}

/* Starts print or page setup `made`, the settings' text copied for its
 * thread, once the text is checked: `HERO_UI_OK`, else the record given back
 * and the member's refusal. */
static int64_t hero_ui_win_print_start(hero_ui_win_dialog *made, int64_t page, int64_t view, const uint8_t *settings, int64_t count) {
    int64_t kind = hero_ui_print_win32_check(settings, count);

    if (kind != HERO_UI_OK) {
        hero_ui_win_dialog_free(made);
        return hero_ui_win_print_refused(kind);
    }
    made->page = page;
    made->view = view;
    made->settings = hero_layer_alloc((size_t)count + 1);
    if (made->settings == NULL) {
        return hero_ui_win_dialog_unmade(made);
    }
    if (count > 0) {
        memcpy(made->settings, settings, (size_t)count);
    }
    made->settings[count] = 0;
    made->settings_count = count;
    return hero_ui_win_dialog_start(made);
}

int64_t hero_ui_arm_print_show(int64_t dialog, int64_t window, int64_t page, const uint8_t *settings, int64_t count) {
    int64_t answer = HERO_UI_OK;
    hero_ui_win_dialog *made;

    hero_ui_win_print_member = false;
    made = hero_ui_win_dialog_new(dialog, window, HERO_UI_WIN_DIALOG_PRINT, &answer);
    if (made == NULL) {
        return answer;
    }
    return hero_ui_win_print_start(made, page, 0, settings, count);
}

/* Hands the page of web view `view` to WebView2 as dialog `dialog`, with
 * the settings; whether WebView2 took it, its completion told later. */
static bool hero_ui_win_print_page(int64_t view, int64_t dialog, const uint8_t *settings, int64_t count) {
    const hero_ui_native *web = hero_ui_find(view);
    const hero_ui_native *top = web != NULL && web->kind == HERO_UI_WEBVIEW ? hero_ui_win_top(web->window) : NULL;
    void *core = top != NULL ? hero_ui_web_win32_core(view) : NULL;

    return core != NULL && top->hwnd != NULL && hero_ui_print_win32_web(core, top->hwnd, dialog, 0, settings, count, hero_ui_tell_text) == HERO_UI_OK;
}

int64_t hero_ui_arm_print_web(int64_t dialog, int64_t view, int64_t ask, const uint8_t *settings, int64_t count) {
    const hero_ui_native *web = hero_ui_find(view);
    const hero_ui_native *top;
    void *core;
    int64_t answer = HERO_UI_OK;
    hero_ui_win_dialog *made;

    hero_ui_win_print_member = false;
    top = web != NULL && web->kind == HERO_UI_WEBVIEW ? hero_ui_win_top(web->window) : NULL;
    core = top != NULL ? hero_ui_web_win32_core(view) : NULL;
    if (core == NULL || top->hwnd == NULL) {
        return hero_ui_refused(HERO_UI_NOT_FOUND, "WebView2 holds no such web view to print, or it stands in no window");
    }
    if (ask == 1) {
        made = hero_ui_win_dialog_new(dialog, top->id, HERO_UI_WIN_DIALOG_PRINT, &answer);
        return made == NULL ? answer : hero_ui_win_print_start(made, 0, view, settings, count);
    }
    answer = hero_ui_print_win32_web(core, top->hwnd, dialog, 0, settings, count, hero_ui_tell_text);
    return answer == HERO_UI_OK ? answer : hero_ui_win_print_refused(answer);
}

int64_t hero_ui_arm_print_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count) {
    hero_ui_win_dialog *shown = NULL;
    HWND window;

    hero_ui_win_print_member = false;
    for (hero_ui_win_dialog *each = hero_ui_win_dialogs; each != NULL && shown == NULL; each = each->next) {
        if (each->kind == HERO_UI_WIN_DIALOG_PRINT && each->dialog == dialog && !each->told && !atomic_load(&each->ended) && !atomic_load(&each->finished)) {
            shown = each;
        }
    }
    if (shown == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (value == 1) {
        /* The person's OK gives the settings the dialog was shown with where
         * none are given, and the given ones are held to the same text. */
        int64_t kind = hero_ui_print_win32_check(count > 0 ? settings : shown->settings, count > 0 ? count : shown->settings_count);

        if (kind != HERO_UI_OK) {
            return hero_ui_win_print_refused(kind);
        }
        if (count == 0) {
            settings = shown->settings;
            count = shown->settings_count;
        }
        shown->injected_text = hero_layer_alloc((size_t)count + 1);
        if (shown->injected_text == NULL) {
            return HERO_UI_LIMIT;
        }
        memcpy(shown->injected_text, settings, (size_t)count);
        shown->injected_count = count;
    }
    shown->injected_value = value == 1 ? 1 : 0;
    atomic_store(&shown->injected, true);
    if (!shown->started) {
        /* Still waiting for its turn: told now, and its thread ends without
         * showing it. */
        hero_ui_win_dialog_told(shown);
        shown->told = true;
        atomic_store(&shown->ended, true);
        shown->started = true;
        (void)SetEvent(shown->turn);
        return HERO_UI_OK;
    }
    window = hero_ui_win_dialog_window(shown);
    if (window == NULL) {
        return atomic_load(&shown->finished) ? HERO_UI_NOT_FOUND : hero_ui_refused(HERO_UI_OTHER, "the dialog's thread put no print dialog on the screen within ten seconds");
    }
    if (!hero_ui_win_dialog_press(shown, window, IDCANCEL)) {
        return hero_ui_refused(HERO_UI_OTHER, "the print dialog did not close within ten seconds of its Cancel pressed");
    }
    return HERO_UI_OK;
}

void hero_ui_arm_print_end(int64_t window) {
    const hero_ui_native *over = window == 0 ? NULL : hero_ui_find(window);

    /* A print over a window ends with it (`dialogs_end` ends the dialogs of
     * its list); WebView2's completions still to come are told to no one.
     * A widget that is no window has no print over it. */
    if (window == 0) {
        hero_ui_print_win32_end(NULL);
    } else if (over != NULL && over->kind == HERO_UI_WINDOW && over->hwnd != NULL) {
        hero_ui_print_win32_end(over->hwnd);
    }
}

int64_t hero_ui_arm_print_words(char *words, int64_t count) {
    return hero_ui_win_print_member ? hero_ui_print_win32_words(words, count) : hero_ui_arm_words(words, count);
}

/* -- BOXES THE SYSTEM DRAWS: PAGES, A SCROLLED BOX, TWO PANES ---------------
 *
 * CONTAINERS: a page, a pane and a scrolled box's content are windows of
 * this arm's panel class, so the controls standing in them send their
 * notifications to this arm's procedure as a window's do, and each is
 * `WS_EX_CONTROLPARENT`, so the dialog manager's Tab goes into it. A widget
 * whose parent is one is made in it and placed from its top-left corner.
 *
 * TABS are the tab control, `WC_TABCONTROL`, each page a child of it laid
 * over its display area (`TCM_ADJUSTRECT`), the page chosen shown alone; a
 * tab's item carries its page's number. A SCROLLED BOX is a panel with its
 * vertical scroll bar always drawn (`SIF_DISABLENOSCROLL`), so the room it
 * takes beside the content never changes, and its content a panel moved up
 * and down within it. A SPLIT is a panel holding its two panes, the divider
 * the strip of the split itself between them, which the person drags with
 * the mouse held (`SetCapture`) within the range the program set; this arm
 * lays the panes out wherever the split or its divider moves. */

/* The divider's thickness, in points: Windows' own sizing border's. */
static int64_t hero_ui_win_divider(UINT dpi) {
    int64_t thickness = hero_ui_win_pt_up(hero_ui_win_metric(SM_CXSIZEFRAME, dpi), dpi);

    return thickness > 0 ? thickness : 4;
}

/* The index of page `page` among tabs `tabs`' items, -1 where it is none. */
static int hero_ui_win_page_index(const hero_ui_native *tabs, int64_t page) {
    int count = (int)SendMessageW(tabs->hwnd, TCM_GETITEMCOUNT, 0, 0);

    for (int i = 0; i < count; i += 1) {
        TCITEMW item;

        memset(&item, 0, sizeof item);
        item.mask = TCIF_PARAM;
        if (SendMessageW(tabs->hwnd, TCM_GETITEMW, (WPARAM)i, (LPARAM)&item) && item.lParam == (LPARAM)page) {
            return i;
        }
    }
    return -1;
}

/* Lays tabs `tabs`' pages over its display area, the one chosen shown. */
static void hero_ui_win_lay_pages(hero_ui_native *tabs) {
    RECT area;
    int chosen = (int)SendMessageW(tabs->hwnd, TCM_GETCURSEL, 0, 0);
    int count = (int)SendMessageW(tabs->hwnd, TCM_GETITEMCOUNT, 0, 0);

    if (!GetClientRect(tabs->hwnd, &area)) {
        return;
    }
    (void)SendMessageW(tabs->hwnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&area);
    for (int i = 0; i < count; i += 1) {
        TCITEMW item;
        hero_ui_native *page;

        memset(&item, 0, sizeof item);
        item.mask = TCIF_PARAM;
        if (!SendMessageW(tabs->hwnd, TCM_GETITEMW, (WPARAM)i, (LPARAM)&item)) {
            continue;
        }
        page = hero_ui_find((int64_t)item.lParam);
        if (page == NULL || page->hwnd == NULL) {
            continue;
        }
        (void)SetWindowPos(page->hwnd, NULL, area.left, area.top, area.right > area.left ? area.right - area.left : 0, area.bottom > area.top ? area.bottom - area.top : 0, SWP_NOZORDER | SWP_NOACTIVATE | (i == chosen && page->visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
    }
}

/* The divider's place in split `split`, its first pane's width or height in
 * points, within what its place holds. */
static int64_t hero_ui_win_divider_at(const hero_ui_native *split) {
    hero_ui_native *window = hero_ui_win_window(split->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    int64_t across = split->kind == HERO_UI_SPLIT_ROW ? split->width : split->height;
    int64_t room = across - hero_ui_win_divider(dpi);

    if (!split->placed) {
        return split->value < 0 ? 0 : split->value;
    }
    return hero_ui_between(split->value, 0, room > 0 ? room : 0);
}

/* The panes of split `split`, the first made first; NULL where it has none. */
static void hero_ui_win_panes(const hero_ui_native *split, hero_ui_native **first, hero_ui_native **second) {
    *first = NULL;
    *second = NULL;
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *pane = hero_ui_at(at);

        if (pane == NULL || pane->kind != HERO_UI_PANE || pane->parent != split->id) {
            continue;
        }
        if (*first == NULL || pane->id < (*first)->id) {
            *second = *first;
            *first = pane;
        } else if (*second == NULL || pane->id < (*second)->id) {
            *second = pane;
        }
    }
}

/* Lays split `split`'s panes out on either side of its divider. */
static void hero_ui_win_lay_panes(hero_ui_native *split) {
    hero_ui_native *window = hero_ui_win_window(split->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    hero_ui_native *first;
    hero_ui_native *second;
    int64_t at = hero_ui_win_divider_at(split);
    int64_t thickness = hero_ui_win_divider(dpi);
    bool row = split->kind == HERO_UI_SPLIT_ROW;

    hero_ui_win_panes(split, &first, &second);
    if (first != NULL && second == NULL) {
        /* One pane fills the split. */
        (void)SetWindowPos(first->hwnd, NULL, 0, 0, hero_ui_win_px(split->width, dpi), hero_ui_win_px(split->height, dpi), SWP_NOZORDER | SWP_NOACTIVATE | (first->visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
        return;
    }
    if (first != NULL) {
        (void)SetWindowPos(first->hwnd, NULL, 0, 0, hero_ui_win_px(row ? at : split->width, dpi), hero_ui_win_px(row ? split->height : at, dpi), SWP_NOZORDER | SWP_NOACTIVATE | (first->visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
    }
    if (second != NULL) {
        int64_t from = at + thickness;
        int64_t left = (row ? split->width : split->height) - from;

        (void)SetWindowPos(second->hwnd, NULL, hero_ui_win_px(row ? from : 0, dpi), hero_ui_win_px(row ? 0 : from, dpi), hero_ui_win_px(row ? (left > 0 ? left : 0) : split->width, dpi), hero_ui_win_px(row ? split->height : (left > 0 ? left : 0), dpi), SWP_NOZORDER | SWP_NOACTIVATE | (second->visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
    }
}

/* Gives scrolled box `scroll`'s bar its range and its content its place. */
static void hero_ui_win_lay_scroll(hero_ui_native *scroll) {
    hero_ui_native *window = hero_ui_win_window(scroll->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    SCROLLINFO bar;
    RECT shown;
    int content = hero_ui_win_px(scroll->scroll_height, dpi);
    int most;

    if (!GetClientRect(scroll->hwnd, &shown)) {
        return;
    }
    most = content - shown.bottom;
    scroll->scrolled = most > 0 ? (scroll->scrolled > most ? most : scroll->scrolled) : 0;
    memset(&bar, 0, sizeof bar);
    bar.cbSize = (UINT)sizeof bar;
    bar.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
    bar.nMin = 0;
    bar.nMax = content > 0 ? content - 1 : 0;
    bar.nPage = (UINT)(shown.bottom > 0 ? shown.bottom : 0);
    bar.nPos = scroll->scrolled;
    (void)SetScrollInfo(scroll->hwnd, SB_VERT, &bar, TRUE);
    /* A content whose size the program never set is the box's own. */
    (void)SetWindowPos(scroll->inside, NULL, 0, -scroll->scrolled, scroll->scroll_width > 0 ? hero_ui_win_px(scroll->scroll_width, dpi) : shown.right, content > 0 ? content : shown.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
}

/* Lays out what box `widget` holds, where it is one. */
static void hero_ui_win_lay_box(hero_ui_native *widget) {
    if (widget == NULL) {
        return;
    }
    switch (widget->kind) {
    case HERO_UI_TABS:
        hero_ui_win_lay_pages(widget);
        break;
    case HERO_UI_SPLIT_ROW:
    case HERO_UI_SPLIT_COLUMN:
        hero_ui_win_lay_panes(widget);
        break;
    case HERO_UI_SCROLL:
        hero_ui_win_lay_scroll(widget);
        break;
    case HERO_UI_GROUP:
        hero_ui_win_lay_group(widget);
        break;
    case HERO_UI_TOOLBAR:
    case HERO_UI_STATUS_BAR:
        hero_ui_win_lay_bar(widget);
        break;
    default:
        break;
    }
}

/* Scrolls scrolled box `hwnd` as its bar's `request` asks (`WM_VSCROLL`),
 * or by `lines` lines of the wheel where `request` is -1. */
static bool hero_ui_win_scroll_by(HWND hwnd, int request, int lines) {
    hero_ui_native *scroll = hero_ui_win_of(hwnd);
    SCROLLINFO bar;
    int line;
    int to;

    if (scroll == NULL || scroll->kind != HERO_UI_SCROLL) {
        return false;
    }
    memset(&bar, 0, sizeof bar);
    bar.cbSize = (UINT)sizeof bar;
    bar.fMask = SIF_ALL;
    if (!GetScrollInfo(hwnd, SB_VERT, &bar)) {
        return true;
    }
    line = hero_ui_win_line(hwnd);
    to = bar.nPos;
    switch (request) {
    case -1:
        to -= lines * line;
        break;
    case SB_LINEUP:
        to -= line;
        break;
    case SB_LINEDOWN:
        to += line;
        break;
    case SB_PAGEUP:
        to -= (int)bar.nPage;
        break;
    case SB_PAGEDOWN:
        to += (int)bar.nPage;
        break;
    case SB_THUMBTRACK:
    case SB_THUMBPOSITION:
        to = bar.nTrackPos;
        break;
    case SB_TOP:
        to = bar.nMin;
        break;
    case SB_BOTTOM:
        to = bar.nMax;
        break;
    default:
        return true;
    }
    {
        int most = bar.nMax - (int)bar.nPage + 1;

        to = to > most ? most : to;
        to = to < 0 ? 0 : to;
    }
    if (to != scroll->scrolled) {
        scroll->scrolled = to;
        hero_ui_win_lay_scroll(scroll);
    }
    return true;
}

/* What the mouse does on split `hwnd`'s own strip, the divider: its cursor,
 * and a drag within the range the program set, told as the person's
 * `HERO_UI_EVENT_VALUE` at each new place. Answers whether it handled it. */
static bool hero_ui_win_divider_mouse(HWND hwnd, UINT message, LPARAM lparam) {
    hero_ui_native *split = hero_ui_win_of(hwnd);
    hero_ui_native *window;
    bool row;
    int at;

    if (split == NULL || (split->kind != HERO_UI_SPLIT_ROW && split->kind != HERO_UI_SPLIT_COLUMN)) {
        return false;
    }
    window = hero_ui_win_window(split->window);
    row = split->kind == HERO_UI_SPLIT_ROW;
    at = row ? (short)LOWORD(lparam) : (short)HIWORD(lparam);
    switch (message) {
    case WM_SETCURSOR:
        (void)SetCursor(LoadCursorW(NULL, (LPCWSTR)(row ? IDC_SIZEWE : IDC_SIZENS)));
        return true;
    case WM_LBUTTONDOWN:
        if (!split->enabled || window == NULL) {
            return true;
        }
        split->dragging = true;
        split->drag_offset = at - hero_ui_win_px(hero_ui_win_divider_at(split), window->dpi);
        (void)SetCapture(hwnd);
        return true;
    case WM_MOUSEMOVE:
        if (split->dragging && window != NULL) {
            int64_t place = hero_ui_win_pt(at - split->drag_offset, window->dpi);
            int64_t across = (row ? split->width : split->height) - hero_ui_win_divider(window->dpi);

            place = hero_ui_between(place, split->least, split->most);
            place = hero_ui_between(place, 0, across > 0 ? across : 0);
            if (place != split->value) {
                split->value = place;
                hero_ui_win_lay_panes(split);
                hero_ui_tell(HERO_UI_EVENT_VALUE, split->id, place, 0);
            }
        }
        return true;
    case WM_LBUTTONUP:
    case WM_CAPTURECHANGED:
        if (split->dragging) {
            split->dragging = false;
            if (message == WM_LBUTTONUP) {
                (void)ReleaseCapture();
            }
        }
        return true;
    default:
        return false;
    }
}

/* Tabs `tabs` shows the page the person or the program chose: laid out,
 * and the person's choice told. */
static void hero_ui_win_page_chosen(hero_ui_native *tabs) {
    hero_ui_win_lay_pages(tabs);
    hero_ui_tell(HERO_UI_EVENT_SELECT, tabs->id, (int64_t)SendMessageW(tabs->hwnd, TCM_GETCURSEL, 0, 0), 0);
}

/* -- A GROUP BOX, AN EDITABLE COMBO BOX, A SPIN -----------------------------
 *
 * A GROUP BOX is a panel of this arm's class holding two windows: the system's
 * own group box, a `BUTTON` of `BS_GROUPBOX` (`part`), which draws the frame
 * and the title but is no container, and over it the content, a panel that
 * the widgets are made in, so what a window does with a box's content it does
 * with this one's, and the panel clips both. The room the frame and the title
 * leave around the content (`hero_ui_insets`) is one value, in points, that
 * the layout and the drawing both read: the title's height from the text
 * metrics of the control's own font, and 8 points on the other three sides,
 * which is the arm's own and unmeasured against what the theme draws.
 *
 * AN EDITABLE COMBO BOX is a `COMBOBOX` of `CBS_DROPDOWN`. What it has chosen
 * is never a state of its own: the first item equal to its text. The person's
 * typing is `CBN_EDITCHANGE`; a choice in its list is `CBN_SELCHANGE`, told as
 * the item's index and then as the text it has become. The items complete
 * what is typed in place (`CBN_EDITUPDATE`): the first item that begins with
 * the text, case apart, is written over it with the part the person did not
 * type selected, so the next key replaces it, and only text that grew is
 * completed, or a Backspace could never take the completion back. `value`
 * holds the length of the text last typed.
 *
 * A SPIN is an `EDIT` and an up-down control (`part`) bought to it by
 * `UDM_SETBUDDY`, which takes the arrows' width out of the field's, so the
 * place the program gave is the two together and `put` gives it again with
 * each placing. The control's own stepping is stopped (`UDN_DELTAPOS`
 * answered true) and the arm does it: the number typed first committed, then
 * a step of 1 within the range, told as `HERO_UI_EVENT_VALUE` where it
 * changes the value. A typed number is committed where the field gives the
 * keys up and at Return; a number past the range is clamped, a text that is
 * no whole number put back as the value held, and nothing told where the
 * value did not change. The field takes any text and the commit judges it:
 * `ES_NUMBER` would refuse the minus sign of a range that holds negatives. */

static void hero_ui_win_group_insets(const hero_ui_native *group, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom) {
    hero_ui_native *window = hero_ui_win_window(group->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    TEXTMETRICW metrics;
    HDC dc = group->part != NULL ? GetDC(group->part) : NULL;
    int title = 0;

    memset(&metrics, 0, sizeof metrics);
    if (dc != NULL) {
        HGDIOBJ before = window != NULL && window->font != NULL ? SelectObject(dc, window->font) : NULL;

        title = GetTextMetricsW(dc, &metrics) ? metrics.tmHeight : 0;
        if (before != NULL) {
            (void)SelectObject(dc, before);
        }
        (void)ReleaseDC(group->part, dc);
    }
    *left = 8;
    *right = 8;
    *bottom = 8;
    *top = hero_ui_win_pt_up(title > 0 ? title : MulDiv(16, (int)dpi, 96), dpi) + 4;
}

static void hero_ui_win_lay_group(hero_ui_native *group) {
    hero_ui_native *window = hero_ui_win_window(group->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    RECT shown;
    int64_t left, top, right, bottom;
    int width, height;

    if (group->part == NULL || group->inside == NULL || !GetClientRect(group->hwnd, &shown)) {
        return;
    }
    hero_ui_win_group_insets(group, &left, &top, &right, &bottom);
    width = shown.right - hero_ui_win_px(left, dpi) - hero_ui_win_px(right, dpi);
    height = shown.bottom - hero_ui_win_px(top, dpi) - hero_ui_win_px(bottom, dpi);
    (void)SetWindowPos(group->part, NULL, 0, 0, shown.right, shown.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
    (void)SetWindowPos(group->inside, NULL, hero_ui_win_px(left, dpi), hero_ui_win_px(top, dpi), width > 0 ? width : 0, height > 0 ? height : 0, SWP_NOZORDER | SWP_NOACTIVATE);
}

/* The title of group box `group` as the program gave it: the system's text
 * with each `&&` the arm wrote for a literal `&` turned back. */
static uint8_t *hero_ui_win_group_title(const hero_ui_native *group, int64_t *bytes) {
    wchar_t *wide = hero_ui_win_wide_of(group->part);
    uint8_t *narrow;
    int at = 0;

    *bytes = 0;
    if (wide == NULL) {
        return NULL;
    }
    for (int i = 0; wide[i] != 0; i += 1) {
        wide[at++] = wide[i];
        if (wide[i] == L'&' && wide[i + 1] == L'&') {
            i += 1;
        }
    }
    wide[at] = 0;
    narrow = hero_ui_win_narrow(wide, at, false, bytes);
    hero_layer_free(wide);
    return narrow;
}

/* The first item of editable combo `combo` equal to its text, -1 for none. */
static int64_t hero_ui_win_combo_match(const hero_ui_native *combo) {
    wchar_t *text = hero_ui_win_wide_of(combo->hwnd);
    int64_t found = -1;

    if (text == NULL) {
        return -1;
    }
    for (int64_t i = 0; i < combo->items && found < 0; i += 1) {
        LRESULT units = SendMessageW(combo->hwnd, CB_GETLBTEXTLEN, (WPARAM)i, 0);
        wchar_t *item = units >= 0 ? hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t)) : NULL;

        if (item == NULL) {
            continue;
        }
        item[0] = 0;
        if (SendMessageW(combo->hwnd, CB_GETLBTEXT, (WPARAM)i, (LPARAM)item) >= 0 && wcscmp(item, text) == 0) {
            found = i;
        }
        hero_layer_free(item);
    }
    hero_layer_free(text);
    return found;
}

/* The person chose an item in editable combo `combo`'s list: its index, then
 * the text it has become. The field is given the item's text here, since
 * the control writes it only after its notification. */
static void hero_ui_win_combo_chosen(hero_ui_native *combo) {
    LRESULT chosen = SendMessageW(combo->hwnd, CB_GETCURSEL, 0, 0);
    LRESULT units;
    wchar_t *item;

    if (chosen == CB_ERR) {
        return;
    }
    units = SendMessageW(combo->hwnd, CB_GETLBTEXTLEN, (WPARAM)chosen, 0);
    item = units >= 0 ? hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t)) : NULL;
    if (item != NULL) {
        item[0] = 0;
        if (SendMessageW(combo->hwnd, CB_GETLBTEXT, (WPARAM)chosen, (LPARAM)item) >= 0) {
            hero_ui_mute(1);
            (void)SetWindowTextW(combo->hwnd, item);
            hero_ui_mute(-1);
            combo->value = (int64_t)wcslen(item);
        }
        hero_layer_free(item);
    }
    hero_ui_tell(HERO_UI_EVENT_SELECT, combo->id, (int64_t)chosen, 0);
    hero_ui_tell(HERO_UI_EVENT_TEXT, combo->id, 0, 0);
}

/* Completes the text the person has just typed in editable combo `combo`
 * from its items (A GROUP BOX, AN EDITABLE COMBO BOX, A SPIN). */
static void hero_ui_win_combo_complete(hero_ui_native *combo) {
    int units = GetWindowTextLengthW(combo->hwnd);
    DWORD from = 0;
    DWORD to = 0;
    wchar_t *typed;
    LRESULT found;

    if (hero_ui_win_injecting > 0 || hero_ui_is_muted() || units <= 0) {
        combo->value = units > 0 ? units : 0;
        return;
    }
    if (units <= combo->value) {
        combo->value = units;
        return;
    }
    combo->value = units;
    (void)SendMessageW(combo->hwnd, CB_GETEDITSEL, (WPARAM)&from, (LPARAM)&to);
    typed = hero_ui_win_wide_of(combo->hwnd);
    if (typed == NULL || to != (DWORD)units) {
        hero_layer_free(typed);
        return;
    }
    found = SendMessageW(combo->hwnd, CB_FINDSTRING, (WPARAM)-1, (LPARAM)typed);
    if (found != CB_ERR) {
        LRESULT length = SendMessageW(combo->hwnd, CB_GETLBTEXTLEN, (WPARAM)found, 0);
        wchar_t *item = length > units ? hero_layer_alloc(((size_t)length + 1) * sizeof(wchar_t)) : NULL;

        if (item != NULL) {
            item[0] = 0;
            if (SendMessageW(combo->hwnd, CB_GETLBTEXT, (WPARAM)found, (LPARAM)item) >= 0) {
                /* What the person typed keeps its own case. */
                memcpy(item, typed, (size_t)units * sizeof(wchar_t));
                hero_ui_mute(1);
                (void)SetWindowTextW(combo->hwnd, item);
                (void)SendMessageW(combo->hwnd, CB_SETEDITSEL, 0, MAKELPARAM(units, length));
                hero_ui_mute(-1);
            }
            hero_layer_free(item);
        }
    }
    hero_layer_free(typed);
}

/* Writes the digits of `number` into `out`, room for 22 units. */
static void hero_ui_win_digits(int64_t number, wchar_t *out) {
    wchar_t reversed[24];
    uint64_t magnitude = number < 0 ? (uint64_t)0 - (uint64_t)number : (uint64_t)number;
    int count = 0;
    int at = 0;

    do {
        reversed[count++] = (wchar_t)(L'0' + (int)(magnitude % 10u));
        magnitude /= 10u;
    } while (magnitude != 0);
    if (number < 0) {
        out[at++] = L'-';
    }
    while (count > 0) {
        out[at++] = reversed[--count];
    }
    out[at] = 0;
}

/* Shows spin `spin`'s held value in its field. */
static void hero_ui_win_spin_show(const hero_ui_native *spin) {
    wchar_t digits[24];

    hero_ui_win_digits(spin->value, digits);
    (void)SetWindowTextW(spin->hwnd, digits);
}

/* The whole number `text` is, spaces and tabs around it passed over and one
 * sign allowed, clamped where it has more digits than 64 bits hold; false
 * where it is anything else. */
static bool hero_ui_win_number_of(const wchar_t *text, int64_t *number) {
    bool negative = false;
    uint64_t magnitude = 0;
    int digits = 0;

    while (*text == L' ' || *text == L'\t') {
        text += 1;
    }
    if (*text == L'-' || *text == L'+') {
        negative = *text == L'-';
        text += 1;
    }
    for (; *text >= L'0' && *text <= L'9'; text += 1) {
        uint64_t digit = (uint64_t)(*text - L'0');

        magnitude = magnitude > (UINT64_MAX - digit) / 10u ? UINT64_MAX : magnitude * 10u + digit;
        digits += 1;
    }
    while (*text == L' ' || *text == L'\t') {
        text += 1;
    }
    if (digits == 0 || *text != 0) {
        return false;
    }
    if (negative) {
        *number = magnitude >= (uint64_t)INT64_MAX + 1u ? INT64_MIN : -(int64_t)magnitude;
    } else {
        *number = magnitude > (uint64_t)INT64_MAX ? INT64_MAX : (int64_t)magnitude;
    }
    return true;
}

static void hero_ui_win_spin_commit(hero_ui_native *spin) {
    wchar_t text[64];
    int units = GetWindowTextLengthW(spin->hwnd);
    int64_t typed = spin->value;
    int64_t before = spin->value;

    if (units > 0 && units < 63 && GetWindowTextW(spin->hwnd, text, 64) > 0 && hero_ui_win_number_of(text, &typed)) {
        spin->value = hero_ui_between(typed, spin->least, spin->most);
    }
    hero_ui_win_spin_show(spin);
    if (spin->value != before) {
        hero_ui_tell(HERO_UI_EVENT_VALUE, spin->id, spin->value, 0);
    }
}

static LRESULT hero_ui_win_spin_stepped(hero_ui_native *spin, const NMUPDOWN *step) {
    int64_t next;

    hero_ui_win_spin_commit(spin);
    next = hero_ui_between(spin->value + (step->iDelta < 0 ? -1 : 1), spin->least, spin->most);
    if (next != spin->value) {
        spin->value = next;
        hero_ui_win_spin_show(spin);
        hero_ui_tell(HERO_UI_EVENT_VALUE, spin->id, next, 0);
    }
    /* True: the control does not step itself. */
    return TRUE;
}

/* -- A TOOLBAR, A STATUS BAR, THE PICKERS ----------------------------------
 *
 * A TOOLBAR AND A STATUS BAR are each a panel of this arm's class that the
 * bar's items are made in, as a page's content is, and under the items, the
 * system's own control (`part`: `TOOLBARCLASSNAME` flat, `STATUSCLASSNAME` in
 * its simple form with no border), which draws what a bar looks like. One
 * route for the whole bar: the system's toolbar holds only buttons and
 * separators of its own, its text fields, checkboxes and combo boxes being
 * child windows, and a status bar holds only texts, and a child window of
 * either would send its notifications to a control that forwards none to the
 * window, so every item is the ordinary control it is anywhere, a child of
 * the panel, and the face places it (`hero_ui_place`) as it does a page's. The
 * control is the lowest window of the panel (`hero_ui_win_order_tabs` keeps it
 * so). `preferred` answers the bar's height, the tallest item and the padding
 * round it, never less than a floor of the arm's own; the bar is placed by
 * the window across its width, so it has no width to answer.
 *
 * THE THREE PICKERS are the system's date and time picker
 * (`DATETIMEPICK_CLASS`) with the format each shows: a date's, the user's
 * short date pattern with a four-digit year (`hero_ui_win_pattern_date`), a
 * time's `HH:mm:ss` with its up-down arrows, and a date and time's the two
 * together. The value held is the calendar's fields as `SYSTEMTIME` holds
 * them, read as UTC in seconds (`hero_ui_win32_pickers.h`), and the control
 * is its view: the arm keeps the value and the ends, `DTM_SETRANGE` bounds the
 * calendar and every change the control tells (`DTN_DATETIMECHANGE`) is read
 * back, held to the ends and told as `HERO_UI_EVENT_VALUE` where it changed
 * the value, which also holds a time of day, that no range of the control
 * does. A program's own `DTM_SETSYSTEMTIME` is muted. A `SYSTEMTIME` has no
 * year before 1601, so a value before it is `invalid` and the ends of a new
 * picker start there.
 *
 * A MONTH'S CALENDAR is the system's month calendar (`MONTHCAL_CLASS`), one
 * day chosen, held and bounded as the date picker's value is
 * (`MCM_SETCURSEL`, `MCM_GETCURSEL`, `MCM_SETRANGE`), its size the control's
 * own least (`MCM_GETMINREQRECT`).
 *
 * A COLOUR WELL is a button the arm draws (`BS_OWNERDRAW`): a swatch of the
 * colour over a push button's frame, the colour shown over a checkerboard
 * where it is not opaque. Its click opens `ChooseColorW` over its window, which
 * has no alpha, so the pick keeps the alpha byte the well held and the dialog's
 * custom colours stay in the widget. The chooser is modal in the UI thread
 * as a context menu is, since only a person's click opens it; the pick is told
 * as `HERO_UI_EVENT_VALUE` once, for the dialog's OK.
 *
 * A FONT BUTTON is a push button showing the family and the size, whose text
 * for the program is the descriptor `<size>|<style>|<family>` the arm keeps
 * (`label`). A descriptor is read by `hero_ui_win_font_parse` and its family
 * is looked for among the system's (`EnumFontFamiliesExW`), spelt as the
 * system spells it. GDI makes a bold or an italic face that a family lacks, so
 * the style reads back as it was set. A click opens `ChooseFontW` the same
 * way, its size the dialog's `iPointSize` in tenths of a point rounded to a
 * whole one. */

/* The padding round a bar's items, in points, as the face's own, and the
 * least height an empty bar takes. */
#define HERO_UI_WIN_BAR_PADDING 4
#define HERO_UI_WIN_TOOLBAR_FLOOR 28
#define HERO_UI_WIN_STATUS_FLOOR 22

static bool hero_ui_win_bar_holds(const hero_ui_native *bar, int64_t kind) {
    if (bar->kind == HERO_UI_TOOLBAR) {
        return kind == HERO_UI_BUTTON || kind == HERO_UI_CHECKBOX || kind == HERO_UI_TEXT_FIELD || kind == HERO_UI_COMBO || kind == HERO_UI_SEPARATOR;
    }
    if (bar->kind == HERO_UI_STATUS_BAR) {
        return kind == HERO_UI_LABEL || kind == HERO_UI_PROGRESS || kind == HERO_UI_BUTTON || kind == HERO_UI_IMAGE || kind == HERO_UI_SEPARATOR;
    }
    return true;
}

static void hero_ui_win_lay_bar(hero_ui_native *bar) {
    RECT shown;

    if (bar->part != NULL && GetClientRect(bar->hwnd, &shown)) {
        (void)SetWindowPos(bar->part, NULL, 0, 0, shown.right, shown.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

static int64_t hero_ui_win_bar_height(const hero_ui_native *bar) {
    int64_t tallest = 0;

    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        const hero_ui_native *item = hero_ui_at(at);
        int64_t wide = 0;
        int64_t high = 0;

        if (item != NULL && item->parent == bar->id && hero_ui_arm_preferred(item->id, &wide, &high) == HERO_UI_OK && high > tallest) {
            tallest = high;
        }
    }
    tallest += 2 * HERO_UI_WIN_BAR_PADDING;
    return tallest > (bar->kind == HERO_UI_TOOLBAR ? HERO_UI_WIN_TOOLBAR_FLOOR : HERO_UI_WIN_STATUS_FLOOR) ? tallest : (bar->kind == HERO_UI_TOOLBAR ? HERO_UI_WIN_TOOLBAR_FLOOR : HERO_UI_WIN_STATUS_FLOOR);
}

static bool hero_ui_win_stamp_kind(int64_t kind) {
    return kind == HERO_UI_DATE_PICKER || kind == HERO_UI_TIME_PICKER || kind == HERO_UI_DATE_TIME_PICKER || kind == HERO_UI_CALENDAR;
}

/* The kind the pure half reads `kind` as: a month's calendar is a date
 * picker shown open (`hero_ui.h`'s A MONTH'S CALENDAR), every rule of its
 * value and its ends the date picker's. */
static int64_t hero_ui_win_stamp_as(int64_t kind) {
    return kind == HERO_UI_CALENDAR ? HERO_UI_DATE_PICKER : kind;
}

/* `fields` as the `SYSTEMTIME` a picker takes. */
static void hero_ui_win_stamp_system(const hero_ui_win_fields *fields, SYSTEMTIME *out) {
    int64_t days = hero_ui_win_stamp_days(fields->year, fields->month, fields->day);

    memset(out, 0, sizeof *out);
    out->wYear = (WORD)fields->year;
    out->wMonth = (WORD)fields->month;
    out->wDay = (WORD)fields->day;
    out->wHour = (WORD)fields->hour;
    out->wMinute = (WORD)fields->minute;
    out->wSecond = (WORD)fields->second;
    /* 1970-01-01 was a Thursday. */
    out->wDayOfWeek = (WORD)((days % 7 + 11) % 7);
}

/* The moment picker `picker` shows, in seconds; false where it tells none. */
static bool hero_ui_win_stamp_read(const hero_ui_native *picker, int64_t *seconds) {
    SYSTEMTIME shown;
    hero_ui_win_fields fields;

    memset(&shown, 0, sizeof shown);
    if (picker->kind == HERO_UI_CALENDAR ? !SendMessageW(picker->hwnd, MCM_GETCURSEL, 0, (LPARAM)&shown) : SendMessageW(picker->hwnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&shown) != GDT_VALID) {
        return false;
    }
    fields.year = shown.wYear;
    fields.month = shown.wMonth;
    fields.day = shown.wDay;
    fields.hour = shown.wHour;
    fields.minute = shown.wMinute;
    fields.second = shown.wSecond;
    if (picker->kind == HERO_UI_TIME_PICKER) {
        /* Only the time of day is its value, whatever day the control holds. */
        fields.year = 1970;
        fields.month = 1;
        fields.day = 1;
    } else if (hero_ui_win_stamp_as(picker->kind) == HERO_UI_DATE_PICKER) {
        fields.hour = 0;
        fields.minute = 0;
        fields.second = 0;
    }
    *seconds = hero_ui_win_stamp_seconds(&fields);
    return true;
}

/* Shows picker `picker`'s ends and value in its control, told to no one. */
static void hero_ui_win_stamp_show(const hero_ui_native *picker) {
    hero_ui_win_fields fields;
    SYSTEMTIME shown;
    SYSTEMTIME ends[2];

    hero_ui_mute(1);
    if (picker->kind != HERO_UI_TIME_PICKER) {
        hero_ui_win_stamp_fields(picker->least, &fields);
        hero_ui_win_stamp_system(&fields, &ends[0]);
        hero_ui_win_stamp_fields(picker->most, &fields);
        hero_ui_win_stamp_system(&fields, &ends[1]);
        (void)SendMessageW(picker->hwnd, picker->kind == HERO_UI_CALENDAR ? MCM_SETRANGE : DTM_SETRANGE, GDTR_MIN | GDTR_MAX, (LPARAM)ends);
    }
    hero_ui_win_stamp_fields(picker->value, &fields);
    hero_ui_win_stamp_system(&fields, &shown);
    if (picker->kind == HERO_UI_CALENDAR) {
        /* The month shown follows the day chosen. */
        (void)SendMessageW(picker->hwnd, MCM_SETCURSEL, 0, (LPARAM)&shown);
    } else {
        (void)SendMessageW(picker->hwnd, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&shown);
    }
    hero_ui_mute(-1);
}

/* Gives a new picker its format, its ends and the value 0. */
static void hero_ui_win_stamp_setup(hero_ui_native *picker) {
    wchar_t locale[85];
    wchar_t date[96];
    wchar_t format[128];
    size_t used = 0;
    const wchar_t clock[] = L"HH':'mm':'ss";
    const wchar_t standard[] = L"yyyy-MM-dd";

    hero_ui_win_stamp_span(hero_ui_win_stamp_as(picker->kind), &picker->least, &picker->most);
    picker->value = 0;
    format[0] = 0;
    if (picker->kind == HERO_UI_CALENDAR) {
        hero_ui_win_stamp_show(picker);
        return;
    }
    if (picker->kind != HERO_UI_TIME_PICKER) {
        if (GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SSHORTDATE, locale, (int)(sizeof locale / sizeof locale[0])) <= 0 || !hero_ui_win_pattern_date(locale, date, (int)(sizeof date / sizeof date[0]))) {
            memcpy(date, standard, sizeof standard);
        }
        used = wcslen(date);
        memcpy(format, date, (used + 1) * sizeof(wchar_t));
    }
    if (picker->kind != HERO_UI_DATE_PICKER) {
        if (used > 0) {
            format[used++] = L' ';
        }
        memcpy(format + used, clock, sizeof clock);
    }
    hero_ui_mute(1);
    (void)SendMessageW(picker->hwnd, DTM_SETFORMATW, 0, (LPARAM)format);
    hero_ui_mute(-1);
    hero_ui_win_stamp_show(picker);
}

/* The control of `picker` told a change: read back, held to the ends, and
 * told where it changed the value. */
static void hero_ui_win_stamp_changed(hero_ui_native *picker) {
    int64_t seconds;
    int64_t held;
    bool changed;

    if (hero_ui_is_muted() || !hero_ui_win_stamp_read(picker, &seconds)) {
        return;
    }
    held = hero_ui_between(seconds, picker->least, picker->most);
    changed = held != picker->value;
    picker->value = held;
    if (held != seconds) {
        /* Past an end the control stays at it, as an arrow past one does. */
        hero_ui_win_stamp_show(picker);
    }
    if (changed) {
        hero_ui_tell(HERO_UI_EVENT_VALUE, picker->id, held, 0);
    }
}

/* Sets picker `picker`'s value as the program does (`person` false, told to
 * no one) or as the person does (told where it changes the value, nothing
 * where the picker is greyed out); `invalid` for a moment its kind does not
 * hold and for one outside its ends. */
static int64_t hero_ui_win_stamp_set(hero_ui_native *picker, int64_t value, bool person) {
    const char *refusal = hero_ui_win_stamp_refusal(hero_ui_win_stamp_as(picker->kind), value);

    if (refusal != NULL) {
        return hero_ui_refused(HERO_UI_INVALID, refusal);
    }
    if (value < picker->least || value > picker->most) {
        return hero_ui_refused(HERO_UI_INVALID, "the moment is outside the ends the picker was given");
    }
    if (person && !IsWindowEnabled(picker->hwnd)) {
        return HERO_UI_OK;
    }
    if (person && value == picker->value) {
        return HERO_UI_OK;
    }
    picker->value = value;
    hero_ui_win_stamp_show(picker);
    if (person) {
        hero_ui_tell(HERO_UI_EVENT_VALUE, picker->id, value, 0);
    }
    return HERO_UI_OK;
}

/* Gives picker `picker` the ends `least` and `most`, a lower end before the
 * control's first day being that day; a value outside them moves to the
 * nearest end, told to no one. */
static int64_t hero_ui_win_stamp_range(hero_ui_native *picker, int64_t least, int64_t most) {
    const char *refusal;

    least = hero_ui_win_stamp_least(hero_ui_win_stamp_as(picker->kind), least);
    refusal = hero_ui_win_stamp_refusal(hero_ui_win_stamp_as(picker->kind), least);
    if (refusal == NULL) {
        refusal = hero_ui_win_stamp_refusal(hero_ui_win_stamp_as(picker->kind), most);
    }
    if (refusal != NULL) {
        return hero_ui_refused(HERO_UI_INVALID, refusal);
    }
    if (least >= most) {
        return hero_ui_refused(HERO_UI_INVALID, "the range ends where the control's first day, 1601-01-01, begins it: no moment lies between its ends");
    }
    picker->least = least;
    picker->most = most;
    picker->value = hero_ui_between(picker->value, least, most);
    hero_ui_win_stamp_show(picker);
    return HERO_UI_OK;
}

/* Gives well `well` the colour `value`, four sRGB bytes `0xRRGGBBAA`, as the
 * program does or as the person does (told where it changes the colour,
 * nothing where the well is greyed out). */
static int64_t hero_ui_win_well_set(hero_ui_native *well, int64_t value, bool person) {
    if (value < 0 || value > INT64_C(4294967295)) {
        return hero_ui_refused(HERO_UI_INVALID, "a colour is four bytes, 0xRRGGBBAA, from 0 to 4294967295");
    }
    if (person && (!IsWindowEnabled(well->hwnd) || value == well->value)) {
        return HERO_UI_OK;
    }
    well->value = value;
    (void)InvalidateRect(well->hwnd, NULL, TRUE);
    if (person) {
        hero_ui_tell(HERO_UI_EVENT_VALUE, well->id, value, 0);
    }
    return HERO_UI_OK;
}

/* Above 0 while a colour or font chooser is open, so a click that arrives
 * through its own message loop opens no second. */
static _Thread_local bool hero_ui_win_choosing;

/* The colour `value` over `backdrop`, each channel by the alpha it holds. */
static COLORREF hero_ui_win_over(uint32_t value, COLORREF backdrop) {
    uint32_t alpha = value & 255u;
    uint32_t red = (value >> 24) & 255u;
    uint32_t green = (value >> 16) & 255u;
    uint32_t blue = (value >> 8) & 255u;

    return RGB((red * alpha + GetRValue(backdrop) * (255u - alpha) + 127u) / 255u, (green * alpha + GetGValue(backdrop) * (255u - alpha) + 127u) / 255u, (blue * alpha + GetBValue(backdrop) * (255u - alpha) + 127u) / 255u);
}

/* Paints the colour well `item` is the owner-drawn button of: the button's
 * frame and, inside it, the colour, over a checkerboard where it is not
 * opaque; false where `item` is no well's. */
static bool hero_ui_win_swatch_draw(const DRAWITEMSTRUCT *item) {
    hero_ui_native *well = item != NULL && item->CtlType == ODT_BUTTON ? hero_ui_win_of(item->hwndItem) : NULL;
    hero_ui_native *window;
    RECT frame;
    RECT inner;
    int cell;
    uint32_t value;
    UINT state = DFCS_BUTTONPUSH;

    if (well == NULL || well->kind != HERO_UI_COLOR_WELL) {
        return false;
    }
    window = hero_ui_win_window(well->window);
    frame = item->rcItem;
    if ((item->itemState & ODS_SELECTED) != 0) {
        state |= DFCS_PUSHED;
    }
    if ((item->itemState & ODS_DISABLED) != 0) {
        state |= DFCS_INACTIVE;
    }
    (void)DrawFrameControl(item->hDC, &frame, DFC_BUTTON, state);
    inner = frame;
    (void)InflateRect(&inner, -hero_ui_win_px(4, window != NULL ? window->dpi : 96u), -hero_ui_win_px(4, window != NULL ? window->dpi : 96u));
    value = (uint32_t)well->value;
    cell = hero_ui_win_px(4, window != NULL ? window->dpi : 96u);
    if ((value & 255u) == 255u || cell < 1) {
        HBRUSH solid = CreateSolidBrush(RGB((value >> 24) & 255u, (value >> 16) & 255u, (value >> 8) & 255u));

        if (solid != NULL) {
            (void)FillRect(item->hDC, &inner, solid);
            (void)DeleteObject(solid);
        }
    } else {
        for (int y = inner.top; y < inner.bottom; y += cell) {
            for (int x = inner.left; x < inner.right; x += cell) {
                RECT square = {x, y, x + cell < inner.right ? x + cell : inner.right, y + cell < inner.bottom ? y + cell : inner.bottom};
                bool light = (((x - inner.left) / cell) + ((y - inner.top) / cell)) % 2 == 0;
                HBRUSH brush = CreateSolidBrush(hero_ui_win_over(value, light ? RGB(255, 255, 255) : RGB(192, 192, 192)));

                if (brush != NULL) {
                    (void)FillRect(item->hDC, &square, brush);
                    (void)DeleteObject(brush);
                }
            }
        }
    }
    (void)FrameRect(item->hDC, &inner, (HBRUSH)GetStockObject(BLACK_BRUSH));
    if ((item->itemState & ODS_FOCUS) != 0 && (item->itemState & ODS_NOFOCUSRECT) == 0) {
        RECT focus = frame;

        (void)InflateRect(&focus, -2, -2);
        (void)DrawFocusRect(item->hDC, &focus);
    }
    return true;
}

/* The person clicked well `well`: the system's colour chooser over its
 * window, and the colour it gives, the alpha kept, told once for its OK. */
static void hero_ui_win_well_choose(hero_ui_native *well) {
    hero_ui_native *window = hero_ui_win_top(well->window);
    CHOOSECOLORW choose;
    uint32_t held = (uint32_t)well->value;
    int64_t id = well->id;
    BOOL picked;

    if (hero_ui_win_choosing || window == NULL) {
        return;
    }
    if (well->customs == NULL) {
        well->customs = hero_layer_alloc(16 * sizeof *well->customs);
        if (well->customs == NULL) {
            return;
        }
        for (int i = 0; i < 16; i += 1) {
            well->customs[i] = RGB(255, 255, 255);
        }
    }
    memset(&choose, 0, sizeof choose);
    choose.lStructSize = (DWORD)sizeof choose;
    choose.hwndOwner = window->hwnd;
    choose.rgbResult = RGB((held >> 24) & 255u, (held >> 16) & 255u, (held >> 8) & 255u);
    choose.lpCustColors = well->customs;
    choose.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;
    hero_ui_win_choosing = true;
    picked = ChooseColorW(&choose);
    hero_ui_win_choosing = false;
    /* The well is looked for again: the chooser's own loop ran meanwhile. */
    well = hero_ui_find(id);
    if (!picked || well == NULL || well->kind != HERO_UI_COLOR_WELL) {
        return;
    }
    well->value = ((int64_t)GetRValue(choose.rgbResult) << 24) | ((int64_t)GetGValue(choose.rgbResult) << 16) | ((int64_t)GetBValue(choose.rgbResult) << 8) | (int64_t)(held & 255u);
    (void)InvalidateRect(well->hwnd, NULL, TRUE);
    hero_ui_tell(HERO_UI_EVENT_VALUE, well->id, well->value, 0);
}

/* The name of the family the system draws its dialogs in, in `family` of
 * `HERO_UI_WIN_FACE_UNITS` units. */
static void hero_ui_win_system_family(wchar_t *family) {
    NONCLIENTMETRICSW metrics;
    const wchar_t *name = L"Segoe UI";
    int units = 0;

    memset(&metrics, 0, sizeof metrics);
    metrics.cbSize = (UINT)sizeof metrics;
    if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0) && metrics.lfMessageFont.lfFaceName[0] != 0) {
        name = metrics.lfMessageFont.lfFaceName;
    }
    for (; name[units] != 0 && units < HERO_UI_WIN_FACE_UNITS - 1; units += 1) {
        family[units] = name[units];
    }
    family[units] = 0;
}

/* Keeps and shows font `spec` in font button `button`: its descriptor in
 * `label`, its family and size on the button. */
static bool hero_ui_win_font_store(hero_ui_native *button, const hero_ui_win_font_spec *spec) {
    wchar_t descriptor[48];
    wchar_t shown[128];
    wchar_t *kept;
    size_t units;

    hero_ui_win_font_format(spec, descriptor);
    hero_ui_win_font_label(spec, shown, (int)(sizeof shown / sizeof shown[0]));
    units = wcslen(descriptor) + 1;
    kept = hero_layer_alloc(units * sizeof(wchar_t));
    if (kept == NULL) {
        return false;
    }
    memcpy(kept, descriptor, units * sizeof(wchar_t));
    hero_layer_free(button->label);
    button->label = kept;
    hero_ui_mute(1);
    (void)SetWindowTextW(button->hwnd, shown);
    hero_ui_mute(-1);
    return true;
}

/* A new font button holds the system's own font at 13 points. */
static bool hero_ui_win_font_start(hero_ui_native *button) {
    hero_ui_win_font_spec spec;

    memset(&spec, 0, sizeof spec);
    spec.size = 13;
    hero_ui_win_system_family(spec.family);
    return hero_ui_win_font_store(button, &spec);
}

/* One family `EnumFontFamiliesExW` found: its name, as the system spells it,
 * into the `wchar_t` block `found` points at, and the enumeration stopped. */
static int CALLBACK hero_ui_win_family_seen(const LOGFONTW *font, const TEXTMETRICW *metrics, DWORD type, LPARAM found) {
    wchar_t *into = (wchar_t *)found;
    int units = 0;

    (void)metrics;
    (void)type;
    for (; font->lfFaceName[units] != 0 && units < HERO_UI_WIN_FACE_UNITS - 1; units += 1) {
        into[units] = font->lfFaceName[units];
    }
    into[units] = 0;
    return 0;
}

/* Whether the system has the family `family` names, case apart; where it
 * has, the name is written back as the system spells it. */
static bool hero_ui_win_family_known(wchar_t *family) {
    LOGFONTW ask;
    wchar_t seen[HERO_UI_WIN_FACE_UNITS];
    HDC screen = GetDC(NULL);
    bool known;

    if (screen == NULL) {
        return false;
    }
    memset(&ask, 0, sizeof ask);
    ask.lfCharSet = DEFAULT_CHARSET;
    for (int i = 0; family[i] != 0 && i < HERO_UI_WIN_FACE_UNITS - 1; i += 1) {
        ask.lfFaceName[i] = family[i];
    }
    seen[0] = 0;
    (void)EnumFontFamiliesExW(screen, &ask, hero_ui_win_family_seen, (LPARAM)seen, 0);
    (void)ReleaseDC(NULL, screen);
    known = seen[0] != 0 && CompareStringOrdinal(seen, -1, family, -1, TRUE) == CSTR_EQUAL;
    if (known) {
        memcpy(family, seen, sizeof seen);
    }
    return known;
}

/* Reads descriptor `text` for font button `button` and, where `store`, makes it
 * the button's font; `changed` (where not NULL) says whether that was another
 * font than it held. `invalid` for a descriptor of another shape or a family
 * the system does not have, nothing kept. */
static int64_t hero_ui_win_font_apply(hero_ui_native *button, const wchar_t *text, bool store, bool *changed) {
    hero_ui_win_font_spec spec;
    wchar_t descriptor[48];
    const char *words = hero_ui_win_font_parse(text, &spec);

    if (words != NULL) {
        return hero_ui_refused(HERO_UI_INVALID, words);
    }
    if (!hero_ui_win_family_known(spec.family)) {
        return hero_ui_refused(HERO_UI_INVALID, "Windows has no font family of that name (EnumFontFamiliesExW lists none)");
    }
    hero_ui_win_font_format(&spec, descriptor);
    if (changed != NULL) {
        *changed = button->label == NULL || wcscmp(descriptor, button->label) != 0;
    }
    if (store && !hero_ui_win_font_store(button, &spec)) {
        return HERO_UI_LIMIT;
    }
    return HERO_UI_OK;
}

/* The person clicked font button `button`: the system's font chooser over its
 * window, and the font it gives told once for its OK. */
static void hero_ui_win_font_choose(hero_ui_native *button) {
    hero_ui_native *window = hero_ui_win_top(button->window);
    hero_ui_win_font_spec spec;
    CHOOSEFONTW choose;
    LOGFONTW face;
    HDC screen;
    int dpi = 96;
    int64_t id = button->id;
    int size;
    BOOL picked;

    if (hero_ui_win_choosing || window == NULL || button->label == NULL || hero_ui_win_font_parse(button->label, &spec) != NULL) {
        return;
    }
    /* The dialog reads a size from the height it is given against the
     * screen's own scale, so the height is made against the same. */
    screen = GetDC(NULL);
    if (screen != NULL) {
        dpi = GetDeviceCaps(screen, LOGPIXELSY);
        (void)ReleaseDC(NULL, screen);
    }
    memset(&face, 0, sizeof face);
    face.lfHeight = -MulDiv(spec.size, dpi > 0 ? dpi : 96, 72);
    face.lfWeight = spec.bold ? FW_BOLD : FW_NORMAL;
    face.lfItalic = spec.italic ? TRUE : FALSE;
    face.lfCharSet = DEFAULT_CHARSET;
    for (int i = 0; spec.family[i] != 0 && i < HERO_UI_WIN_FACE_UNITS - 1; i += 1) {
        face.lfFaceName[i] = spec.family[i];
    }
    memset(&choose, 0, sizeof choose);
    choose.lStructSize = (DWORD)sizeof choose;
    choose.hwndOwner = window->hwnd;
    choose.lpLogFont = &face;
    choose.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_NOVERTFONTS | CF_FORCEFONTEXIST | CF_LIMITSIZE;
    choose.nSizeMin = 1;
    choose.nSizeMax = 1024;
    hero_ui_win_choosing = true;
    picked = ChooseFontW(&choose);
    hero_ui_win_choosing = false;
    button = hero_ui_find(id);
    if (!picked || button == NULL || button->kind != HERO_UI_FONT_BUTTON || face.lfFaceName[0] == 0) {
        return;
    }
    size = (choose.iPointSize + 5) / 10;
    spec.size = size < 1 ? 1 : (size > 1024 ? 1024 : size);
    spec.bold = face.lfWeight >= FW_BOLD;
    spec.italic = face.lfItalic != 0;
    for (int i = 0; i < HERO_UI_WIN_FACE_UNITS; i += 1) {
        spec.family[i] = i < HERO_UI_WIN_FACE_UNITS - 1 ? face.lfFaceName[i] : 0;
        if (spec.family[i] == 0) {
            break;
        }
    }
    if (hero_ui_win_font_store(button, &spec)) {
        hero_ui_tell(HERO_UI_EVENT_TEXT, button->id, 0, 0);
    }
}

int64_t hero_ui_arm_insets(int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;
    UINT dpi;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_win_window(widget->window);
    dpi = window != NULL ? window->dpi : 96u;
    if (widget->kind == HERO_UI_TABS) {
        /* Measured as the tab control's own display area in a place of 400
         * by 300 points, the room its tabs and border take around it. */
        int wide = hero_ui_win_px(400, dpi);
        int high = hero_ui_win_px(300, dpi);
        RECT area = {0, 0, wide, high};

        (void)SendMessageW(widget->hwnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&area);
        *left = hero_ui_win_pt_up(area.left, dpi);
        *top = hero_ui_win_pt_up(area.top, dpi);
        *right = hero_ui_win_pt_up(wide - area.right, dpi);
        *bottom = hero_ui_win_pt_up(high - area.bottom, dpi);
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_SCROLL) {
        *right = hero_ui_win_pt_up(hero_ui_win_metric(SM_CXVSCROLL, dpi), dpi);
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_GROUP) {
        hero_ui_win_group_insets(widget, left, top, right, bottom);
        return HERO_UI_OK;
    }
    return HERO_UI_INVALID;
}

/* -- A TABLE ----------------------------------------------------------------
 *
 * The list view in report mode, `WC_LISTVIEW`, with `LVS_OWNERDATA`: the
 * arm keeps the cells as UTF-16 and the list view asks for each one it
 * draws (`LVN_GETDISPINFOW`), so a table of many rows is given at once and
 * drawn as it scrolls. A click changes the rows chosen by several
 * notifications (the old let go, the new chosen), so the arm tells one
 * `SELECT` once they have all come: the first posts `HERO_UI_WIN_SETTLE` to
 * the table's parent, and its handling tells the rows chosen where they
 * differ from the ones last told. A header clicked turns the column already
 * sorted by over, or sorts another ascending, shows the arrow at once
 * (`HDF_SORTUP`, `HDF_SORTDOWN`) and is told; the rows are never reordered
 * here, the program giving them again. */

/* The cell of table `table` at `row` and `column`, an empty one where none. */
static wchar_t *hero_ui_win_cell(const hero_ui_native *table, int64_t row, int64_t column) {
    static _Thread_local wchar_t none[1];
    int64_t at = row * table->columns + column;

    return table->columns > 0 && column < table->columns && at >= 0 && at < table->cell_count && table->cells[at] != NULL ? table->cells[at] : none;
}

/* Gives back table `table`'s cells. */
static void hero_ui_win_drop_cells(hero_ui_native *table) {
    for (int64_t i = 0; i < table->cell_count; i += 1) {
        hero_layer_free(table->cells[i]);
    }
    hero_layer_free(table->cells);
    table->cells = NULL;
    table->cell_count = 0;
    table->items = 0;
}

/* Shows table `table`'s sort in its header's arrows. */
static void hero_ui_win_show_sort(const hero_ui_native *table) {
    HWND header = (HWND)SendMessageW(table->hwnd, LVM_GETHEADER, 0, 0);

    for (int64_t i = 0; header != NULL && i < table->columns; i += 1) {
        HDITEMW item;

        memset(&item, 0, sizeof item);
        item.mask = HDI_FORMAT;
        if (!SendMessageW(header, HDM_GETITEMW, (WPARAM)i, (LPARAM)&item)) {
            continue;
        }
        item.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
        if (i == table->sort_column) {
            item.fmt |= table->sort_ascending ? HDF_SORTUP : HDF_SORTDOWN;
        }
        (void)SendMessageW(header, HDM_SETITEMW, (WPARAM)i, (LPARAM)&item);
    }
}

/* The rows table `table` has chosen, ascending, in a block of
 * `hero_layer_alloc` (NULL for none), their count through `*count`; false
 * where memory ran out. */
static bool hero_ui_win_rows_chosen(const hero_ui_native *table, int64_t **rows, int64_t *count) {
    LRESULT selected = SendMessageW(table->hwnd, LVM_GETSELECTEDCOUNT, 0, 0);
    int at = -1;

    *rows = NULL;
    *count = 0;
    if (selected <= 0) {
        return true;
    }
    *rows = hero_layer_alloc((size_t)selected * sizeof **rows);
    if (*rows == NULL) {
        return false;
    }
    while (*count < (int64_t)selected && (at = (int)SendMessageW(table->hwnd, LVM_GETNEXTITEM, (WPARAM)at, LVNI_SELECTED)) >= 0) {
        (*rows)[*count] = at;
        *count += 1;
    }
    return true;
}

static int64_t hero_ui_win_first_chosen(const hero_ui_native *table) {
    return (int64_t)SendMessageW(table->hwnd, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);
}

/* Notes table `table`'s rows chosen now as the ones told. */
static void hero_ui_win_note_rows(hero_ui_native *table) {
    int64_t *rows = NULL;
    int64_t count = 0;

    if (hero_ui_win_rows_chosen(table, &rows, &count)) {
        hero_layer_free(table->told);
        table->told = rows;
        table->told_count = count;
    }
    table->choice_waits = false;
}

/* Tells table `table`'s rows chosen where they differ from the ones last
 * told: the person's choice, once its notifications have all come. */
static void hero_ui_win_settle_rows(hero_ui_native *table) {
    int64_t *rows = NULL;
    int64_t count = 0;
    bool same;

    if (!table->choice_waits) {
        return;
    }
    table->choice_waits = false;
    if (!hero_ui_win_rows_chosen(table, &rows, &count)) {
        return;
    }
    same = count == table->told_count && (count == 0 || memcmp(rows, table->told, (size_t)count * sizeof *rows) == 0);
    hero_layer_free(table->told);
    table->told = rows;
    table->told_count = count;
    if (!same) {
        hero_ui_tell(HERO_UI_EVENT_SELECT, table->id, count > 0 ? rows[0] : -1, 0);
    }
}

/* The rows of table `table` chosen anew by the person or by an act given
 * as theirs: told once, after the notifications. */
static void hero_ui_win_rows_changed(hero_ui_native *table) {
    if (hero_ui_is_muted() || table->choice_waits) {
        return;
    }
    table->choice_waits = true;
    (void)PostMessageW(GetParent(table->hwnd), HERO_UI_WIN_SETTLE, 0, (LPARAM)table->id);
}

/* Chooses row `row` of table `table` and no other, the focus with it, or
 * adds it to the rows chosen where `adding`; -1 lets every row go. */
static void hero_ui_win_choose_row(hero_ui_native *table, int64_t row, bool adding) {
    LVITEMW state;

    memset(&state, 0, sizeof state);
    state.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
    if (!adding || row < 0) {
        (void)SendMessageW(table->hwnd, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)&state);
    }
    if (row >= 0) {
        state.state = LVIS_SELECTED | LVIS_FOCUSED;
        (void)SendMessageW(table->hwnd, LVM_SETITEMSTATE, (WPARAM)row, (LPARAM)&state);
        (void)SendMessageW(table->hwnd, LVM_ENSUREVISIBLE, (WPARAM)row, FALSE);
    }
}

int64_t hero_ui_arm_set_columns(int64_t id, const uint8_t *titles, int64_t count) {
    hero_ui_native *table = hero_ui_find(id);
    hero_ui_native *window;
    int64_t columns = hero_ui_texts_in(titles, count);

    if (table == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_win_window(table->window);
    if (table->kind != HERO_UI_TABLE || window == NULL || columns < 1 || columns > HERO_UI_COLUMNS_MOST) {
        return HERO_UI_INVALID;
    }
    hero_ui_mute(1);
    (void)SendMessageW(table->hwnd, LVM_SETITEMCOUNT, 0, 0);
    hero_ui_win_drop_cells(table);
    while (SendMessageW(table->hwnd, LVM_DELETECOLUMN, 0, 0)) {
    }
    table->columns = 0;
    for (int64_t i = 0; i < columns; i += 1) {
        int64_t length = 0;
        const uint8_t *title = hero_ui_text_at(titles, count, i, &length);
        wchar_t *wide = hero_ui_win_wide(title, length, 0);
        LVCOLUMNW column;
        SIZE size;

        if (wide == NULL) {
            hero_ui_mute(-1);
            return HERO_UI_LIMIT;
        }
        size = hero_ui_win_text_size(table->hwnd, wide, DT_SINGLELINE);
        memset(&column, 0, sizeof column);
        column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        column.pszText = wide;
        column.iSubItem = (int)i;
        /* Its title's width with the room a header gives it, at least 80
         * points; the person widens it. */
        column.cx = size.cx + hero_ui_win_px(24, window->dpi);
        column.cx = column.cx > hero_ui_win_px(80, window->dpi) ? column.cx : hero_ui_win_px(80, window->dpi);
        if (SendMessageW(table->hwnd, LVM_INSERTCOLUMNW, (WPARAM)i, (LPARAM)&column) >= 0) {
            table->columns += 1;
        }
        hero_layer_free(wide);
    }
    table->sort_column = -1;
    table->sort_ascending = true;
    hero_ui_win_note_rows(table);
    hero_ui_mute(-1);
    return table->columns == columns ? HERO_UI_OK : hero_ui_win_failed("giving a table its columns", GetLastError());
}

/* Gives table `table` its cells, row after row. */
static int64_t hero_ui_win_set_cells(hero_ui_native *table, const uint8_t *items, int64_t count) {
    int64_t texts = hero_ui_texts_in(items, count);
    wchar_t **cells;

    if (table->columns == 0 || texts % table->columns != 0) {
        return HERO_UI_INVALID;
    }
    cells = texts > 0 ? hero_layer_alloc_zero((size_t)texts, sizeof *cells) : NULL;
    if (texts > 0 && cells == NULL) {
        return HERO_UI_LIMIT;
    }
    for (int64_t i = 0; i < texts; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(items, count, i, &length);

        /* A cell shows one line: a line feed or a tab drawn as a space. */
        cells[i] = hero_ui_win_wide(text, length, HERO_UI_WIN_TABS);
        if (cells[i] == NULL) {
            for (int64_t j = 0; j < i; j += 1) {
                hero_layer_free(cells[j]);
            }
            hero_layer_free(cells);
            return HERO_UI_LIMIT;
        }
        for (wchar_t *at = cells[i]; *at != 0; at += 1) {
            *at = *at == L'\n' || *at == L'\r' ? L' ' : *at;
        }
    }
    hero_ui_mute(1);
    hero_ui_win_choose_row(table, -1, false);
    hero_ui_win_drop_cells(table);
    table->cells = cells;
    table->cell_count = texts;
    table->items = texts / table->columns;
    (void)SendMessageW(table->hwnd, LVM_SETITEMCOUNT, (WPARAM)table->items, LVSICF_NOINVALIDATEALL);
    (void)InvalidateRect(table->hwnd, NULL, TRUE);
    hero_ui_win_note_rows(table);
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_sort(int64_t id, int64_t column, int64_t ascending) {
    hero_ui_native *table = hero_ui_find(id);

    if (table == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (table->kind != HERO_UI_TABLE || column >= table->columns) {
        return HERO_UI_INVALID;
    }
    table->sort_column = column;
    table->sort_ascending = ascending == 1;
    hero_ui_win_show_sort(table);
    return HERO_UI_OK;
}

/* Sorts table `table` by `column` as the person's click on its header asks,
 * shown and told. */
static void hero_ui_win_sort_asked(hero_ui_native *table, int64_t column, bool ascending) {
    table->sort_column = column;
    table->sort_ascending = ascending;
    hero_ui_win_show_sort(table);
    hero_ui_tell(HERO_UI_EVENT_SORT, table->id, column, ascending ? 1 : 0);
}

/* What table `table` notifies, `NMHDR` `head`; answers what its parent's
 * procedure answers. */
static LRESULT hero_ui_win_table_notified(hero_ui_native *table, NMHDR *head) {
    switch (head->code) {
    case LVN_GETDISPINFOW: {
        LVITEMW *item = &((NMLVDISPINFOW *)(void *)head)->item;

        if ((item->mask & LVIF_TEXT) != 0) {
            item->pszText = hero_ui_win_cell(table, item->iItem, item->iSubItem);
        }
        return 0;
    }
    case LVN_ITEMCHANGED: {
        const NMLISTVIEW *changed = (const NMLISTVIEW *)(void *)head;

        if ((changed->uChanged & LVIF_STATE) != 0 && ((changed->uOldState ^ changed->uNewState) & LVIS_SELECTED) != 0) {
            hero_ui_win_rows_changed(table);
        }
        return 0;
    }
    case LVN_ODSTATECHANGED: {
        const NMLVODSTATECHANGE *changed = (const NMLVODSTATECHANGE *)(void *)head;

        if (((changed->uOldState ^ changed->uNewState) & LVIS_SELECTED) != 0) {
            hero_ui_win_rows_changed(table);
        }
        return 0;
    }
    case LVN_ITEMACTIVATE: {
        const NMITEMACTIVATE *opened = (const NMITEMACTIVATE *)(void *)head;

        hero_ui_win_settle_rows(table);
        if (opened->iItem >= 0 && opened->iItem < table->items) {
            hero_ui_tell(HERO_UI_EVENT_ACTIVATE, table->id, opened->iItem, 0);
        }
        return 0;
    }
    case LVN_COLUMNCLICK: {
        int64_t column = ((const NMLISTVIEW *)(void *)head)->iSubItem;

        hero_ui_win_sort_asked(table, column, column == table->sort_column ? !table->sort_ascending : true);
        return 0;
    }
    default:
        return 0;
    }
}

/* What the person does to table `table`, given as theirs: `SELECT` a row
 * chosen, or added where `second` is 1, -1 none; `ACTIVATE` a row opened,
 * chosen first; `SORT` a column clicked to sort as `second` asks. */
static int64_t hero_ui_win_table_inject(hero_ui_native *table, int64_t kind, int64_t value, int64_t second) {
    switch (kind) {
    case HERO_UI_EVENT_SELECT:
        if (value >= table->items) {
            return HERO_UI_INVALID;
        }
        if (IsWindowEnabled(table->hwnd)) {
            hero_ui_win_choose_row(table, value, second == 1);
        }
        return HERO_UI_OK;
    case HERO_UI_EVENT_ACTIVATE:
        if (value >= table->items) {
            return HERO_UI_INVALID;
        }
        if (IsWindowEnabled(table->hwnd)) {
            if ((SendMessageW(table->hwnd, LVM_GETITEMSTATE, (WPARAM)value, LVIS_SELECTED) & LVIS_SELECTED) == 0) {
                hero_ui_win_choose_row(table, value, false);
            }
            hero_ui_win_settle_rows(table);
            hero_ui_tell(HERO_UI_EVENT_ACTIVATE, table->id, value, 0);
        }
        return HERO_UI_OK;
    case HERO_UI_EVENT_SORT:
        if (value >= table->columns) {
            return HERO_UI_INVALID;
        }
        if (IsWindowEnabled(table->hwnd)) {
            hero_ui_win_sort_asked(table, value, second == 1);
        }
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

/* -- A TREE -----------------------------------------------------------------
 *
 * The tree view, `WC_TREEVIEW`, each item carrying the program's number in
 * its `lParam`, and a foldable item its button while it holds none
 * (`cChildren` 1). The arm keeps the items, their numbers and what each
 * stands under, so it finds an item's handle and the items under it without
 * asking the tree view. The person's choice is `TVN_SELCHANGEDW`, an item
 * unfolded or folded `TVN_ITEMEXPANDEDW`; an item unfolded while it holds
 * none is told at `TVN_ITEMEXPANDINGW`, since the tree view has nothing to
 * show and may tell no more, and the items the program then adds under it
 * are shown unfolded. Folding an item whose descendant is chosen lets the
 * choice go before the fold, where the tree view would move it to the
 * folded item, and tells it gone after the fold, `UNFOLD` then `SELECT`, as
 * AppKit's outline view was measured to; a choice the tree view would make of its own
 * accord (its first item when it takes the keys with none chosen) is
 * refused, and an item taken out takes the choice under it with it. A
 * double click or Return opens the item chosen. */

/* The item of tree `tree` numbered `item`, or NULL; 0, the top, is none. */
static hero_ui_win_node *hero_ui_win_node_of(const hero_ui_native *tree, int64_t item) {
    for (int64_t i = 0; i < tree->node_count; i += 1) {
        if (tree->nodes[i].item == item) {
            return &tree->nodes[i];
        }
    }
    return NULL;
}

/* The item of tree `tree` whose handle is `handle`, or NULL. */
static hero_ui_win_node *hero_ui_win_node_at(const hero_ui_native *tree, HTREEITEM handle) {
    for (int64_t i = 0; handle != NULL && i < tree->node_count; i += 1) {
        if (tree->nodes[i].handle == handle) {
            return &tree->nodes[i];
        }
    }
    return NULL;
}

/* Whether item `item` stands under item `above`, at any depth. */
static bool hero_ui_win_under(const hero_ui_native *tree, int64_t item, int64_t above) {
    const hero_ui_win_node *node = hero_ui_win_node_of(tree, item);

    for (int64_t steps = 0; node != NULL && steps <= tree->node_count; steps += 1) {
        if (node->under == above) {
            return true;
        }
        node = hero_ui_win_node_of(tree, node->under);
    }
    return false;
}

/* Whether item `item` holds an item. */
static bool hero_ui_win_holds(const hero_ui_native *tree, int64_t item) {
    for (int64_t i = 0; i < tree->node_count; i += 1) {
        if (tree->nodes[i].under == item) {
            return true;
        }
    }
    return false;
}

/* Gives item `node` its button where it is foldable or holds items. */
static void hero_ui_win_show_button(const hero_ui_native *tree, const hero_ui_win_node *node) {
    TVITEMW change;

    memset(&change, 0, sizeof change);
    change.mask = TVIF_CHILDREN | TVIF_HANDLE;
    change.hItem = node->handle;
    change.cChildren = node->foldable || hero_ui_win_holds(tree, node->item) ? 1 : 0;
    (void)SendMessageW(tree->hwnd, TVM_SETITEMW, 0, (LPARAM)&change);
}

/* Whether item `item` is unfolded in the tree view. */
static bool hero_ui_win_unfolded(const hero_ui_native *tree, const hero_ui_win_node *node) {
    return (SendMessageW(tree->hwnd, TVM_GETITEMSTATE, (WPARAM)node->handle, TVIS_EXPANDED) & TVIS_EXPANDED) != 0;
}

/* Whether the person sees item `item`: every item over it unfolded. */
static bool hero_ui_win_seen(const hero_ui_native *tree, int64_t item) {
    const hero_ui_win_node *node = hero_ui_win_node_of(tree, item);

    if (node == NULL) {
        return false;
    }
    for (const hero_ui_win_node *over = hero_ui_win_node_of(tree, node->under); over != NULL; over = hero_ui_win_node_of(tree, over->under)) {
        if (!hero_ui_win_unfolded(tree, over)) {
            return false;
        }
    }
    return true;
}

static int64_t hero_ui_win_item_chosen(const hero_ui_native *tree) {
    HTREEITEM chosen = (HTREEITEM)SendMessageW(tree->hwnd, TVM_GETNEXTITEM, TVGN_CARET, 0);
    const hero_ui_win_node *node = hero_ui_win_node_at(tree, chosen);

    return node != NULL ? node->item : 0;
}

/* Lets the choice of tree `tree` go where it stands under item `item`,
 * telling nothing; answers whether it let one go. */
static bool hero_ui_win_let_go_under(hero_ui_native *tree, int64_t item) {
    int64_t chosen = hero_ui_win_item_chosen(tree);

    if (chosen == 0 || !hero_ui_win_under(tree, chosen, item)) {
        return false;
    }
    hero_ui_mute(1);
    (void)SendMessageW(tree->hwnd, TVM_SELECTITEM, TVGN_CARET, 0);
    hero_ui_mute(-1);
    return true;
}

int64_t hero_ui_arm_tree_add(int64_t id, int64_t item, int64_t under, const uint8_t *text, int64_t count, int64_t foldable) {
    hero_ui_native *tree = hero_ui_find(id);
    hero_ui_win_node *over;
    hero_ui_win_node *made;
    TVINSERTSTRUCTW insert;
    wchar_t *wide;
    HTREEITEM handle;

    if (tree == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (tree->kind != HERO_UI_TREE) {
        return HERO_UI_INVALID;
    }
    over = under != 0 ? hero_ui_win_node_of(tree, under) : NULL;
    if ((under != 0 && over == NULL) || hero_ui_win_node_of(tree, item) != NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (tree->node_count == tree->node_room) {
        int64_t room = tree->node_room == 0 ? 16 : tree->node_room * 2;
        hero_ui_win_node *grown = hero_layer_grow(tree->nodes, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            return HERO_UI_LIMIT;
        }
        tree->nodes = grown;
        tree->node_room = room;
        over = under != 0 ? hero_ui_win_node_of(tree, under) : NULL;
    }
    wide = hero_ui_win_wide(text, count, HERO_UI_WIN_TABS);
    if (wide == NULL) {
        return HERO_UI_LIMIT;
    }
    memset(&insert, 0, sizeof insert);
    insert.hParent = over != NULL ? over->handle : TVI_ROOT;
    insert.hInsertAfter = TVI_LAST;
    insert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
    insert.item.pszText = wide;
    insert.item.lParam = (LPARAM)item;
    insert.item.cChildren = foldable == 1 ? 1 : 0;
    hero_ui_mute(1);
    handle = (HTREEITEM)SendMessageW(tree->hwnd, TVM_INSERTITEMW, 0, (LPARAM)&insert);
    hero_layer_free(wide);
    if (handle == NULL) {
        hero_ui_mute(-1);
        return hero_ui_win_failed("adding a tree's item", GetLastError());
    }
    made = &tree->nodes[tree->node_count];
    tree->node_count += 1;
    made->item = item;
    made->under = under;
    made->handle = handle;
    made->foldable = foldable == 1;
    made->waiting = false;
    if (over != NULL) {
        hero_ui_win_show_button(tree, over);
        if (over->waiting) {
            /* The person unfolded it while it held none: what the program
             * adds now is shown so. */
            over->waiting = false;
            (void)SendMessageW(tree->hwnd, TVM_EXPAND, TVE_EXPAND, (LPARAM)over->handle);
        }
    }
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_tree_remove(int64_t id, int64_t item) {
    hero_ui_native *tree = hero_ui_find(id);
    hero_ui_win_node *node;
    int64_t under;
    int64_t kept = 0;

    if (tree == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (tree->kind != HERO_UI_TREE) {
        return HERO_UI_INVALID;
    }
    node = hero_ui_win_node_of(tree, item);
    if (node == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    under = node->under;
    hero_ui_mute(1);
    /* The choice of the item or of one under it goes with it, where the
     * tree view would move it to a neighbour. */
    if (hero_ui_win_item_chosen(tree) == item || hero_ui_win_under(tree, hero_ui_win_item_chosen(tree), item)) {
        (void)SendMessageW(tree->hwnd, TVM_SELECTITEM, TVGN_CARET, 0);
    }
    (void)SendMessageW(tree->hwnd, TVM_DELETEITEM, 0, (LPARAM)node->handle);
    /* The tree view took the items under it with it: so does the arm. */
    for (int64_t i = 0; i < tree->node_count; i += 1) {
        if (tree->nodes[i].item != item && !hero_ui_win_under(tree, tree->nodes[i].item, item)) {
            tree->nodes[kept] = tree->nodes[i];
            kept += 1;
        }
    }
    tree->node_count = kept;
    node = hero_ui_win_node_of(tree, under);
    if (node != NULL) {
        hero_ui_win_show_button(tree, node);
    }
    hero_ui_mute(-1);
    return HERO_UI_OK;
}

/* Unfolds item `node`, `on`, or folds it, telling nothing; a fold lets go
 * of a choice under it first, and answers whether it did. */
static bool hero_ui_win_fold(hero_ui_native *tree, hero_ui_win_node *node, bool on) {
    bool gone = false;

    hero_ui_mute(1);
    if (!on) {
        gone = hero_ui_win_let_go_under(tree, node->item);
    }
    node->waiting = on && !hero_ui_win_holds(tree, node->item);
    (void)SendMessageW(tree->hwnd, TVM_EXPAND, on ? TVE_EXPAND : TVE_COLLAPSE, (LPARAM)node->handle);
    hero_ui_win_show_button(tree, node);
    hero_ui_mute(-1);
    return gone;
}

int64_t hero_ui_arm_tree_unfold(int64_t id, int64_t item, int64_t on) {
    hero_ui_native *tree = hero_ui_find(id);
    hero_ui_win_node *node;

    if (tree == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (tree->kind != HERO_UI_TREE) {
        return HERO_UI_INVALID;
    }
    node = hero_ui_win_node_of(tree, item);
    if (node == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (!node->foldable && !hero_ui_win_holds(tree, item)) {
        return HERO_UI_INVALID;
    }
    (void)hero_ui_win_fold(tree, node, on == 1);
    return HERO_UI_OK;
}

/* Chooses item `item` of tree `tree`, 0 none, the items over it unfolded;
 * told where the person's act, not where the program's. */
static void hero_ui_win_choose_item(hero_ui_native *tree, int64_t item) {
    hero_ui_win_node *node = hero_ui_win_node_of(tree, item);

    if (node != NULL) {
        hero_ui_mute(1);
        (void)SendMessageW(tree->hwnd, TVM_ENSUREVISIBLE, 0, (LPARAM)node->handle);
        hero_ui_mute(-1);
    }
    (void)SendMessageW(tree->hwnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)(node != NULL ? node->handle : NULL));
}

/* What tree `tree` notifies; answers what its parent's procedure answers. */
static LRESULT hero_ui_win_tree_notified(hero_ui_native *tree, NMHDR *head) {
    switch (head->code) {
    case TVN_SELCHANGINGW:
    {
        /* An item chosen by no person's click or key, no call of the
         * program's and no act given as the person's is the tree view's
         * own: its first item when it takes the keys with none chosen,
         * which Windows Server 2025 was measured to notify with an action
         * of 0x1000, none of the documented `TVC_*`. Refused, so the choice
         * is the person's and the program's alone. */
        UINT action = ((const NMTREEVIEWW *)(void *)head)->action;

        return action == TVC_BYMOUSE || action == TVC_BYKEYBOARD || hero_ui_is_muted() || hero_ui_win_injecting > 0 ? FALSE : TRUE;
    }
    case TVN_SELCHANGEDW:
        if (!hero_ui_is_muted()) {
            hero_ui_tell(HERO_UI_EVENT_SELECT, tree->id, hero_ui_win_item_chosen(tree), 0);
        }
        return 0;
    case TVN_ITEMEXPANDINGW: {
        const NMTREEVIEWW *change = (const NMTREEVIEWW *)(void *)head;
        hero_ui_win_node *node = hero_ui_win_node_at(tree, change->itemNew.hItem);

        if (node == NULL || hero_ui_is_muted()) {
            return FALSE;
        }
        if ((change->action & TVE_COLLAPSE) != 0) {
            node->let_go = hero_ui_win_let_go_under(tree, node->item);
        } else if ((change->action & TVE_EXPAND) != 0 && !hero_ui_win_holds(tree, node->item)) {
            /* Nothing to show yet: told now, its items shown as they come. */
            node->waiting = true;
            hero_ui_tell(HERO_UI_EVENT_UNFOLD, tree->id, node->item, 1);
        }
        return FALSE;
    }
    case TVN_ITEMEXPANDEDW: {
        const NMTREEVIEWW *change = (const NMTREEVIEWW *)(void *)head;
        hero_ui_win_node *node = hero_ui_win_node_at(tree, change->itemNew.hItem);

        if (node == NULL || hero_ui_is_muted() || !hero_ui_win_holds(tree, node->item)) {
            return 0;
        }
        hero_ui_tell(HERO_UI_EVENT_UNFOLD, tree->id, node->item, (change->action & TVE_EXPAND) != 0 ? 1 : 0);
        if (node->let_go) {
            /* The choice the fold let go of, told after the fold, as the
             * layer's other systems tell it. */
            node->let_go = false;
            hero_ui_tell(HERO_UI_EVENT_SELECT, tree->id, 0, 0);
        }
        return 0;
    }
    case NM_DBLCLK: {
        TVHITTESTINFO hit;
        DWORD at = GetMessagePos();
        hero_ui_win_node *node;

        memset(&hit, 0, sizeof hit);
        hit.pt.x = (short)LOWORD(at);
        hit.pt.y = (short)HIWORD(at);
        (void)ScreenToClient(tree->hwnd, &hit.pt);
        node = hero_ui_win_node_at(tree, (HTREEITEM)SendMessageW(tree->hwnd, TVM_HITTEST, 0, (LPARAM)&hit));
        if (node != NULL && (hit.flags & TVHT_ONITEM) != 0) {
            hero_ui_tell(HERO_UI_EVENT_ACTIVATE, tree->id, node->item, 0);
        }
        return 0;
    }
    default:
        return 0;
    }
}

/* What the person does to tree `tree`, given as theirs, reaching only an
 * item they see: `SELECT` an item chosen (-1 none), `ACTIVATE` one opened,
 * chosen first, `UNFOLD` one unfolded or folded. */
static int64_t hero_ui_win_tree_inject(hero_ui_native *tree, int64_t kind, int64_t value, int64_t second) {
    hero_ui_win_node *node = value > 0 ? hero_ui_win_node_of(tree, value) : NULL;

    if (!(kind == HERO_UI_EVENT_SELECT && value < 0) && (node == NULL || !hero_ui_win_seen(tree, value))) {
        return HERO_UI_INVALID;
    }
    if (kind == HERO_UI_EVENT_UNFOLD && !node->foldable && !hero_ui_win_holds(tree, value)) {
        return HERO_UI_INVALID;
    }
    if (kind != HERO_UI_EVENT_SELECT && kind != HERO_UI_EVENT_ACTIVATE && kind != HERO_UI_EVENT_UNFOLD) {
        return HERO_UI_INVALID;
    }
    if (!IsWindowEnabled(tree->hwnd)) {
        return HERO_UI_OK;
    }
    switch (kind) {
    case HERO_UI_EVENT_SELECT:
        if (value != hero_ui_win_item_chosen(tree)) {
            hero_ui_win_choose_item(tree, value < 0 ? 0 : value);
        }
        return HERO_UI_OK;
    case HERO_UI_EVENT_ACTIVATE:
        if (value != hero_ui_win_item_chosen(tree)) {
            hero_ui_win_choose_item(tree, value);
        }
        hero_ui_tell(HERO_UI_EVENT_ACTIVATE, tree->id, value, 0);
        return HERO_UI_OK;
    default: {
        /* Unfolded where the person unfolded it while it held none, or
         * where it holds items the tree view shows. */
        bool now = node->waiting || (hero_ui_win_holds(tree, value) && hero_ui_win_unfolded(tree, node));

        if (now == (second == 1)) {
            return HERO_UI_OK;
        }
        if (hero_ui_win_fold(tree, node, second == 1)) {
            hero_ui_tell(HERO_UI_EVENT_UNFOLD, tree->id, value, 0);
            hero_ui_tell(HERO_UI_EVENT_SELECT, tree->id, 0, 0);
            return HERO_UI_OK;
        }
        hero_ui_tell(HERO_UI_EVENT_UNFOLD, tree->id, value, second == 1 ? 1 : 0);
        return HERO_UI_OK;
    }
    }
}

/* -- WHAT THE PERSON DOES TO TABS AND A SPLIT ------------------------------ */

/* Chooses page `index` of tabs `tabs` as the person's click does: the tab
 * control's focus moved to it, which chooses it and notifies its parent
 * (`TCM_SETCURFOCUS`), the page told by that notification; where Windows
 * chose it and sent none, it is told here. */
static int64_t hero_ui_win_choose_tab(hero_ui_native *tabs, int64_t index) {
    int64_t before = (int64_t)SendMessageW(tabs->hwnd, TCM_GETCURSEL, 0, 0);
    uint64_t told = hero_ui_pushed_count();

    if (index < 0 || index >= (int64_t)SendMessageW(tabs->hwnd, TCM_GETITEMCOUNT, 0, 0)) {
        return HERO_UI_INVALID;
    }
    if (!IsWindowEnabled(tabs->hwnd) || index == before) {
        return HERO_UI_OK;
    }
    (void)SendMessageW(tabs->hwnd, TCM_SETCURFOCUS, (WPARAM)index, 0);
    if (hero_ui_pushed_count() == told) {
        if ((int64_t)SendMessageW(tabs->hwnd, TCM_GETCURSEL, 0, 0) != index) {
            (void)SendMessageW(tabs->hwnd, TCM_SETCURSEL, (WPARAM)index, 0);
        }
        hero_ui_win_page_chosen(tabs);
    }
    return HERO_UI_OK;
}

/* Drags split `split`'s divider to `value` as the person's drag does: kept
 * within the range the program set and what the split holds, laid out and
 * told. */
static int64_t hero_ui_win_drag_to(hero_ui_native *split, int64_t value) {
    hero_ui_native *window = hero_ui_win_window(split->window);
    int64_t across;
    int64_t place;
    hero_ui_native *first;
    hero_ui_native *second;

    hero_ui_win_panes(split, &first, &second);
    if (window == NULL || second == NULL) {
        return HERO_UI_INVALID;
    }
    if (!split->enabled) {
        return HERO_UI_OK;
    }
    across = (split->kind == HERO_UI_SPLIT_ROW ? split->width : split->height) - hero_ui_win_divider(window->dpi);
    place = hero_ui_between(value, split->least, split->most);
    place = split->placed ? hero_ui_between(place, 0, across > 0 ? across : 0) : place;
    split->value = place;
    hero_ui_win_lay_panes(split);
    hero_ui_tell(HERO_UI_EVENT_VALUE, split->id, place, 0);
    return HERO_UI_OK;
}

/* -- THE PROGRAM'S CHOICE IN A BOX ----------------------------------------- */

/* The program's choice of the page tabs `widget` shows, of a table's rows
 * or of a tree's item, telling nothing: a page by its index, one; rows of
 * the table, any number; an item of the tree by its number, at most one,
 * the items over it unfolded. */
static int64_t hero_ui_win_choose_in_box(hero_ui_native *widget, const int64_t *indices, int64_t count) {
    switch (widget->kind) {
    case HERO_UI_TABS:
        if (count != 1 || indices[0] >= (int64_t)SendMessageW(widget->hwnd, TCM_GETITEMCOUNT, 0, 0)) {
            return HERO_UI_INVALID;
        }
        hero_ui_mute(1);
        (void)SendMessageW(widget->hwnd, TCM_SETCURSEL, (WPARAM)indices[0], 0);
        hero_ui_mute(-1);
        hero_ui_win_lay_pages(widget);
        return HERO_UI_OK;
    case HERO_UI_TABLE:
        for (int64_t i = 0; i < count; i += 1) {
            if (indices[i] >= widget->items) {
                return HERO_UI_INVALID;
            }
        }
        hero_ui_mute(1);
        hero_ui_win_choose_row(widget, -1, false);
        for (int64_t i = 0; i < count; i += 1) {
            hero_ui_win_choose_row(widget, indices[i], i > 0);
        }
        hero_ui_win_note_rows(widget);
        hero_ui_mute(-1);
        return HERO_UI_OK;
    case HERO_UI_TREE:
        if (count > 1 || (count == 1 && hero_ui_win_node_of(widget, indices[0]) == NULL)) {
            return HERO_UI_INVALID;
        }
        hero_ui_mute(1);
        hero_ui_win_choose_item(widget, count == 1 ? indices[0] : 0);
        hero_ui_mute(-1);
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

/* -- THE DARK OR LIGHT APPEARANCE --------------------------------------------
 *
 * The system's appearance is the person's choice for apps in Windows'
 * settings, `AppsUseLightTheme` under the current user's `Personalize` key
 * (light where Windows names none, as before Windows 10 1809), read again
 * when Windows tells every window `WM_SETTINGCHANGE` of "ImmersiveColorSet".
 * The app's windows follow it in what Windows draws for a program: their
 * title bars and frames, by DWM's dark mode (`DWMWA_USE_IMMERSIVE_DARK_MODE`,
 * attribute 20, 19 before Windows 10 20H1); Win32's own controls have no
 * documented dark look and keep the system's light one. An appearance the
 * program injects holds until it injects 0, the system's own again. */

#define HERO_UI_WIN_DARK_TITLE 20
#define HERO_UI_WIN_DARK_TITLE_BEFORE 19


/* The appearance the person chose for apps in Windows' settings. */
static int64_t hero_ui_win_system_appearance(void) {
    DWORD light = 1;
    DWORD size = (DWORD)sizeof light;

    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &light, &size) != ERROR_SUCCESS) {
        return HERO_UI_APPEARANCE_LIGHT;
    }
    return light == 0 ? HERO_UI_APPEARANCE_DARK : HERO_UI_APPEARANCE_LIGHT;
}

/* The appearance the app's windows are drawn in now. */
static int64_t hero_ui_win_appearance_now(void) {
    return hero_ui_win_appearance_forced != 0 ? hero_ui_win_appearance_forced : hero_ui_win_system_appearance();
}

/* Draws window `hwnd`'s title bar and frame in the appearance now. */
static void hero_ui_win_dress(HWND hwnd) {
    BOOL dark = hero_ui_win_appearance_now() == HERO_UI_APPEARANCE_DARK;

    if (FAILED(DwmSetWindowAttribute(hwnd, HERO_UI_WIN_DARK_TITLE, &dark, (DWORD)sizeof dark))) {
        (void)DwmSetWindowAttribute(hwnd, HERO_UI_WIN_DARK_TITLE_BEFORE, &dark, (DWORD)sizeof dark);
    }
}

/* The appearance may have turned: every window drawn in it, and told where
 * it differs from the one last told. */
static void hero_ui_win_appearance_turned(void) {
    int64_t now = hero_ui_win_appearance_now();

    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *window = hero_ui_at(at);

        if (window != NULL && window->kind == HERO_UI_WINDOW) {
            hero_ui_win_dress(window->hwnd);
        }
    }
    if (now != hero_ui_win_appearance_told) {
        hero_ui_win_appearance_told = now;
        hero_ui_tell(HERO_UI_EVENT_APPEARANCE, 0, now, 0);
    }
}

/* Windows tells every top-level window a setting changed: the appearance's
 * is "ImmersiveColorSet". */
static void hero_ui_win_setting_changed(LPARAM name) {
    if (name != 0 && wcscmp((const wchar_t *)name, L"ImmersiveColorSet") == 0 && hero_ui_win_appearance_forced == 0) {
        hero_ui_win_appearance_turned();
    }
}

int64_t hero_ui_arm_appearance(int64_t *appearance) {
    *appearance = hero_ui_win_appearance_now();
    return HERO_UI_OK;
}

int64_t hero_ui_arm_appearance_set(int64_t appearance) {
    hero_ui_win_appearance_forced = appearance;
    hero_ui_win_appearance_turned();
    return HERO_UI_OK;
}

/* -- ACCESSIBLE NAMES --------------------------------------------------------
 *
 * A control's name for a screen reader is the one Active Accessibility and
 * UI Automation give it: its own words, or for a field the label standing
 * before it. A name of the program's, or a tied label's words, is set over
 * them by the accessibility property service (`IAccPropServices`'
 * `SetHwndPropStr` with `PROPID_ACC_NAME`) and taken back by
 * `ClearHwndProps`; the name is read back as the system holds it, by the
 * control's `IAccessible` (`get_accName`). The program's own name comes
 * first, then a tied label's words, kept again whenever the label's text
 * changes. */

#if defined(_MSC_VER)
#pragma comment(lib, "oleacc.lib")
#pragma comment(lib, "oleaut32.lib")
#endif

/* `oleacc.h`'s service, its interface, its name property and
 * `IAccessible`, written here so no import library of GUIDs is needed. */
static const CLSID hero_ui_win_props_class = {0xb5f8350b, 0x0548, 0x48b1, {0xa6, 0xee, 0x88, 0xbd, 0x00, 0xb4, 0xa5, 0xe7}};
static const IID hero_ui_win_props_face = {0x6e26e776, 0x04f0, 0x495d, {0x80, 0xe4, 0x33, 0x30, 0x35, 0x2e, 0x31, 0x69}};
static const GUID hero_ui_win_name_property = {0x608d3df8, 0x8128, 0x4aa7, {0xa4, 0x28, 0xf5, 0x5e, 0x49, 0x26, 0x72, 0x91}};
static const IID hero_ui_win_accessible_face = {0x618736e0, 0x3c3d, 0x11cf, {0x81, 0x0c, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

/* The accessibility property service, made once the UI thread is in COM;
 * NULL where it cannot be. */
static _Thread_local IAccPropServices *hero_ui_win_props;
static _Thread_local bool hero_ui_win_props_joined;

static IAccPropServices *hero_ui_win_props_of(void) {
    if (hero_ui_win_props == NULL) {
        HRESULT joined = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        hero_ui_win_props_joined = SUCCEEDED(joined);
        if (FAILED(joined) && joined != RPC_E_CHANGED_MODE) {
            return NULL;
        }
        if (FAILED(CoCreateInstance(&hero_ui_win_props_class, NULL, CLSCTX_INPROC_SERVER, &hero_ui_win_props_face, (void **)&hero_ui_win_props))) {
            hero_ui_win_props = NULL;
            if (hero_ui_win_props_joined) {
                CoUninitialize();
                hero_ui_win_props_joined = false;
            }
        }
    }
    return hero_ui_win_props;
}

/* Gives the service back at `close`. */
static void hero_ui_win_props_end(void) {
    if (hero_ui_win_props != NULL) {
        hero_ui_win_props->lpVtbl->Release(hero_ui_win_props);
        hero_ui_win_props = NULL;
    }
    if (hero_ui_win_props_joined) {
        CoUninitialize();
        hero_ui_win_props_joined = false;
    }
}

/* Sets widget `widget`'s name as its record says: the program's own, else
 * its tied label's words, else none of the arm's, the system's own then. */
/* Drawing area `area`'s name handed to the root of its parts, which UI
 * Automation reads in place of the system's once the area has parts: the
 * program's name, or its tied label's words, as below. */
static int64_t hero_ui_win_parts_named(const hero_ui_native *area) {
    hero_ui_native *label = area->label_for != 0 ? hero_ui_find(area->label_for) : NULL;
    wchar_t *words = NULL;
    int64_t answer;

    if (area->parts == NULL) {
        return HERO_UI_OK;
    }
    if (area->access_name == NULL && label != NULL && label->hwnd != NULL) {
        words = hero_ui_win_wide_of(label->hwnd);
    }
    answer = hero_ui_parts_win32_name(area->parts, (const uint16_t *)(area->access_name != NULL ? area->access_name : words));
    hero_layer_free(words);
    return answer;
}

static int64_t hero_ui_win_name_again(hero_ui_native *widget) {
    IAccPropServices *props = hero_ui_win_props_of();
    hero_ui_native *label = widget->label_for != 0 ? hero_ui_find(widget->label_for) : NULL;
    wchar_t *words = NULL;
    HRESULT done;

    if (hero_ui_win_parts_named(widget) != HERO_UI_OK) {
        return HERO_UI_LIMIT;
    }
    if (props == NULL) {
        return hero_ui_refused(HERO_UI_OTHER, "Windows' accessibility property service (IAccPropServices) could not be made");
    }
    if (widget->access_name == NULL && label != NULL && label->hwnd != NULL) {
        words = hero_ui_win_wide_of(label->hwnd);
    }
    if (widget->access_name != NULL || words != NULL) {
        done = props->lpVtbl->SetHwndPropStr(props, widget->hwnd, (DWORD)OBJID_CLIENT, CHILDID_SELF, hero_ui_win_name_property, widget->access_name != NULL ? widget->access_name : words);
    } else {
        MSAAPROPID name = hero_ui_win_name_property;

        done = props->lpVtbl->ClearHwndProps(props, widget->hwnd, (DWORD)OBJID_CLIENT, CHILDID_SELF, &name, 1);
    }
    hero_layer_free(words);
    return FAILED(done) ? hero_ui_win_failed("naming a widget for a screen reader", (DWORD)done) : HERO_UI_OK;
}

/* A drawing area is one element for a screen reader, of role image
 * (`ROLE_SYSTEM_GRAPHIC`, which a window of this arm's own class is not
 * given by the system), its name the program's, until the program gives it
 * parts: from then on UI Automation reads the root of its parts in its place
 * (`hero_ui_parts_win32.c`). Where the service could not be made it keeps
 * the system's role. */
static void hero_ui_win_canvas_role(const hero_ui_native *area) {
    static const GUID role_property = {0xcb905ff2, 0x7bd1, 0x4c05, {0xb3, 0xc8, 0xe6, 0xc2, 0x41, 0x36, 0x4d, 0x70}};
    IAccPropServices *props = hero_ui_win_props_of();
    VARIANT role;

    if (props == NULL || area->hwnd == NULL) {
        return;
    }
    VariantInit(&role);
    role.vt = VT_I4;
    role.lVal = ROLE_SYSTEM_GRAPHIC;
    (void)props->lpVtbl->SetHwndProp(props, area->hwnd, (DWORD)OBJID_CLIENT, CHILDID_SELF, role_property, role);
}

/* Label `label`'s words changed or it is gone: every widget it names is
 * named again. */
static void hero_ui_win_label_changed(int64_t label, bool gone) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        if (widget != NULL && widget->label_for == label && widget->id != label) {
            if (gone) {
                widget->label_for = 0;
            }
            (void)hero_ui_win_name_again(widget);
        }
    }
}

int64_t hero_ui_arm_set_accessible_name(int64_t id, const uint8_t *name, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    wchar_t *wide = NULL;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->hwnd == NULL) {
        return HERO_UI_INVALID;
    }
    if (count > 0) {
        wide = hero_ui_win_wide(name, count, HERO_UI_WIN_TABS);
        if (wide == NULL) {
            return HERO_UI_LIMIT;
        }
    }
    hero_layer_free(widget->access_name);
    widget->access_name = wide;
    return hero_ui_win_name_again(widget);
}

int64_t hero_ui_arm_accessible_name(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);
    IAccessible *object = NULL;
    VARIANT self;
    BSTR name = NULL;
    HRESULT done;
    uint8_t *narrow;

    *length = 0;
    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->hwnd == NULL) {
        return HERO_UI_INVALID;
    }
    done = AccessibleObjectFromWindow(widget->hwnd, (DWORD)OBJID_CLIENT, &hero_ui_win_accessible_face, (void **)&object);
    if (FAILED(done) || object == NULL) {
        return hero_ui_win_failed("reading a widget's accessible name", (DWORD)done);
    }
    VariantInit(&self);
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;
    done = object->lpVtbl->get_accName(object, self, &name);
    object->lpVtbl->Release(object);
    if (FAILED(done)) {
        return hero_ui_win_failed("reading a widget's accessible name", (DWORD)done);
    }
    if (name == NULL) {
        return HERO_UI_OK;
    }
    narrow = hero_ui_win_narrow(name, (int)SysStringLen(name), true, length);
    SysFreeString(name);
    if (narrow == NULL) {
        return HERO_UI_LIMIT;
    }
    if (*length > 0 && *length <= count) {
        memcpy(buffer, narrow, (size_t)*length);
    }
    hero_layer_free(narrow);
    return HERO_UI_OK;
}

int64_t hero_ui_arm_set_label_for(int64_t id, int64_t label) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *caption = label != 0 ? hero_ui_find(label) : NULL;

    if (widget == NULL || (label != 0 && caption == NULL)) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->hwnd == NULL || (caption != NULL && caption->kind != HERO_UI_LABEL) || label == id) {
        return HERO_UI_INVALID;
    }
    widget->label_for = label;
    return hero_ui_win_name_again(widget);
}

/* -- AN ICON IN THE TRAY, THE TASK BAR, AND A NOTIFICATION --------------------
 *
 * A TRAY is a hidden top-level window of the arm's owning one icon of the
 * notification area (`Shell_NotifyIconW`, version 4), its tip its text and
 * its icon its image or the app's. A click on it, the left button's
 * (`NIN_SELECT`, `NIN_KEYSELECT`) or the menu's (`WM_CONTEXTMENU`), shows the
 * tray's context menu at the point Windows gives, the hidden window brought
 * to the foreground first so the menu closes when the person clicks away,
 * Windows' rule for a tray's menu; an item chosen comes to that window as a
 * menu's `WM_COMMAND`. Explorer restarted tells every top-level window
 * "TaskbarCreated", and each tray adds its icon again.
 *
 * THE TASK BAR: an app out of it has no task bar button for any of its
 * windows (`ITaskbarList`'s `DeleteTab`), each one taken out again whenever
 * it is shown, since showing a window gives it its button back.
 *
 * A NOTIFICATION is a tray icon's balloon (`NIF_INFO`), which Windows 10 and
 * later show as a toast under the program's name: the app's first tray's,
 * else one of the arm's own, a tray icon added for it and kept until the app
 * closes. A toast of the program's own (WinRT's `ToastNotificationManager`)
 * needs an AppUserModelID the Start menu knows, which a program the
 * compiler built does not carry. */

#if defined(_MSC_VER)
#pragma comment(lib, "shell32.lib")
#endif

/* `shobjidl.h`'s task bar list, written here so no import library of GUIDs
 * is needed. */
static const CLSID hero_ui_win_taskbar_class = {0x56fdf344, 0xfd6d, 0x11d0, {0x95, 0x8a, 0x00, 0x60, 0x97, 0xc9, 0xa0, 0x90}};
static const IID hero_ui_win_taskbar_face = {0x56fdf342, 0xfd6d, 0x11d0, {0x95, 0x8a, 0x00, 0x60, 0x97, 0xc9, 0xa0, 0x90}};

/* Copies the UTF-16 `from` into `into` of `room` units with its zero, cut
 * where it does not fit, never inside a surrogate pair. */
static void hero_ui_win_copy_units(wchar_t *into, size_t room, const wchar_t *from) {
    size_t at = 0;

    while (from != NULL && from[at] != 0 && at + 1 < room) {
        into[at] = from[at];
        at += 1;
    }
    if (at > 0 && into[at - 1] >= 0xD800 && into[at - 1] <= 0xDBFF && from[at] != 0) {
        at -= 1;
    }
    into[at] = 0;
}

/* Tray `tray`'s icon in the notification area: added, `NIM_ADD`, or its
 * tip and icon given again, `NIM_MODIFY`; false where Windows refused. */
static bool hero_ui_win_tray_show(hero_ui_native *tray, DWORD how) {
    NOTIFYICONDATAW icon;

    memset(&icon, 0, sizeof icon);
    icon.cbSize = (DWORD)sizeof icon;
    icon.hWnd = tray->hwnd;
    icon.uID = 1;
    icon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
    icon.uCallbackMessage = HERO_UI_WIN_TRAY_MESSAGE;
    icon.hIcon = tray->tray_icon != NULL ? tray->tray_icon : hero_ui_win_icon != NULL ? hero_ui_win_icon : LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION);
    hero_ui_win_copy_units(icon.szTip, sizeof icon.szTip / sizeof icon.szTip[0], tray->tip != NULL ? tray->tip : tray->label);
    if (!Shell_NotifyIconW(how, &icon)) {
        return false;
    }
    if (how == NIM_ADD) {
        icon.uVersion = NOTIFYICON_VERSION_4;
        (void)Shell_NotifyIconW(NIM_SETVERSION, &icon);
        tray->tray_shown = true;
    }
    return true;
}

/* Takes tray `tray`'s icon out of the notification area. */
static void hero_ui_win_tray_hide(hero_ui_native *tray) {
    NOTIFYICONDATAW icon;

    if (!tray->tray_shown) {
        return;
    }
    memset(&icon, 0, sizeof icon);
    icon.cbSize = (DWORD)sizeof icon;
    icon.hWnd = tray->hwnd;
    icon.uID = 1;
    (void)Shell_NotifyIconW(NIM_DELETE, &icon);
    tray->tray_shown = false;
}

/* What tray window `hwnd`'s icon tells (`HERO_UI_WIN_TRAY_MESSAGE`): a click
 * shows its context menu at the point `at` packs. */
static void hero_ui_win_tray_told(HWND hwnd, WPARAM at, LPARAM what) {
    UINT event = LOWORD(what);

    if (event == WM_CONTEXTMENU || event == NIN_SELECT || event == NIN_KEYSELECT) {
        (void)hero_ui_win_show_context(hwnd, (LPARAM)at, false);
    }
}

/* Explorer restarted: every tray shown adds its icon again. */
static void hero_ui_win_trays_again(void) {
    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *tray = hero_ui_at(at);

        if (tray != NULL && tray->kind == HERO_UI_TRAY && tray->tray_shown) {
            tray->tray_shown = false;
            (void)hero_ui_win_tray_show(tray, NIM_ADD);
        }
    }
}

/* Makes tray `made`'s hidden window and adds its icon. */
static int64_t hero_ui_win_make_tray(hero_ui_native *made) {
    made->hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, hero_ui_win_panel_class, L"", WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandleW(NULL), NULL);
    if (made->hwnd == NULL) {
        return hero_ui_win_failed("making a tray icon's window", GetLastError());
    }
    (void)SetWindowLongPtrW(made->hwnd, GWLP_USERDATA, (LONG_PTR)made->id);
    if (!hero_ui_win_tray_show(made, NIM_ADD)) {
        DWORD code = GetLastError();

        (void)DestroyWindow(made->hwnd);
        made->hwnd = NULL;
        return hero_ui_win_failed("adding an icon to the notification area", code);
    }
    return HERO_UI_OK;
}

/* Tray `tray`'s icon of `rgba` pixels, `width` by `height`, or the app's
 * for none, 0 by 0. */
static int64_t hero_ui_win_tray_image(hero_ui_native *tray, const uint8_t *rgba, int64_t width, int64_t height) {
    HICON made = NULL;
    int64_t answer = width > 0 && height > 0 ? hero_ui_win_make_icon(rgba, width, height, &made) : HERO_UI_OK;

    if (answer != HERO_UI_OK) {
        return answer;
    }
    if (tray->tray_icon != NULL) {
        (void)DestroyIcon(tray->tray_icon);
    }
    tray->tray_icon = made;
    return !tray->tray_shown || hero_ui_win_tray_show(tray, NIM_MODIFY) ? HERO_UI_OK : hero_ui_win_failed("giving a tray icon its image", GetLastError());
}

/* Takes window `hwnd`'s task bar button out where the app stands out of the
 * task bar, or puts it back, `ITaskbarList` made for the call. */
static int64_t hero_ui_win_taskbar_tab(HWND hwnd, bool shown) {
    HRESULT joined = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ITaskbarList *list = NULL;
    HRESULT done;

    if (FAILED(joined) && joined != RPC_E_CHANGED_MODE) {
        return hero_ui_win_failed("joining COM for the task bar", (DWORD)joined);
    }
    done = CoCreateInstance(&hero_ui_win_taskbar_class, NULL, CLSCTX_INPROC_SERVER, &hero_ui_win_taskbar_face, (void **)&list);
    if (SUCCEEDED(done) && list != NULL) {
        done = list->lpVtbl->HrInit(list);
        if (SUCCEEDED(done)) {
            done = shown ? list->lpVtbl->AddTab(list, hwnd) : list->lpVtbl->DeleteTab(list, hwnd);
        }
        list->lpVtbl->Release(list);
    }
    if (SUCCEEDED(joined)) {
        CoUninitialize();
    }
    return FAILED(done) ? hero_ui_win_failed("changing the task bar", (DWORD)done) : HERO_UI_OK;
}

/* Window `window` was shown: out of the task bar again where the app is. */
static void hero_ui_win_shown_in_taskbar(const hero_ui_native *window) {
    if (hero_ui_win_undocked) {
        (void)hero_ui_win_taskbar_tab(window->hwnd, false);
    }
}

int64_t hero_ui_arm_set_docked(int64_t on) {
    int64_t answer = HERO_UI_OK;

    hero_ui_win_undocked = on == 0;
    for (size_t at = 0; at < hero_ui_map_room() && answer == HERO_UI_OK; at += 1) {
        hero_ui_native *window = hero_ui_at(at);

        if (window != NULL && window->kind == HERO_UI_WINDOW && IsWindowVisible(window->hwnd)) {
            answer = hero_ui_win_taskbar_tab(window->hwnd, on == 1);
        }
    }
    return answer;
}

/* The tray icon a notification is shown by: the app's first tray, else one
 * of the arm's own, added for it and kept until the app closes. */
static _Thread_local HWND hero_ui_win_notifier;

static int64_t hero_ui_win_notifier_of(HWND *hwnd) {
    NOTIFYICONDATAW icon;

    for (size_t at = 0; at < hero_ui_map_room(); at += 1) {
        hero_ui_native *tray = hero_ui_at(at);

        if (tray != NULL && tray->kind == HERO_UI_TRAY && tray->tray_shown) {
            *hwnd = tray->hwnd;
            return HERO_UI_OK;
        }
    }
    if (hero_ui_win_notifier == NULL) {
        hero_ui_win_notifier = CreateWindowExW(WS_EX_TOOLWINDOW, hero_ui_win_panel_class, L"", WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandleW(NULL), NULL);
        if (hero_ui_win_notifier == NULL) {
            return hero_ui_win_failed("making the window a notification is shown by", GetLastError());
        }
        memset(&icon, 0, sizeof icon);
        icon.cbSize = (DWORD)sizeof icon;
        icon.hWnd = hero_ui_win_notifier;
        icon.uID = 1;
        icon.uFlags = NIF_ICON;
        icon.hIcon = hero_ui_win_icon != NULL ? hero_ui_win_icon : LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION);
        if (!Shell_NotifyIconW(NIM_ADD, &icon)) {
            DWORD code = GetLastError();

            (void)DestroyWindow(hero_ui_win_notifier);
            hero_ui_win_notifier = NULL;
            return hero_ui_refused(HERO_UI_UNSUPPORTED, code == 0 ? "Windows shows a program's notification by its icon in the notification area, which this session has none of (no Explorer runs)" : "Windows refused an icon in the notification area to show a notification by");
        }
    }
    *hwnd = hero_ui_win_notifier;
    return HERO_UI_OK;
}

/* Takes the arm's own notifying icon away at `close`. */
static void hero_ui_win_notifier_end(void) {
    NOTIFYICONDATAW icon;

    if (hero_ui_win_notifier == NULL) {
        return;
    }
    memset(&icon, 0, sizeof icon);
    icon.cbSize = (DWORD)sizeof icon;
    icon.hWnd = hero_ui_win_notifier;
    icon.uID = 1;
    (void)Shell_NotifyIconW(NIM_DELETE, &icon);
    (void)DestroyWindow(hero_ui_win_notifier);
    hero_ui_win_notifier = NULL;
}

int64_t hero_ui_arm_notify(const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count) {
    wchar_t *heading = hero_ui_win_wide(title, title_count, HERO_UI_WIN_TABS);
    wchar_t *said = hero_ui_win_wide(text, text_count, HERO_UI_WIN_LINES);
    NOTIFYICONDATAW icon;
    HWND by = NULL;
    int64_t answer;

    if (heading == NULL || said == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return HERO_UI_LIMIT;
    }
    answer = hero_ui_win_notifier_of(&by);
    if (answer == HERO_UI_OK) {
        memset(&icon, 0, sizeof icon);
        icon.cbSize = (DWORD)sizeof icon;
        icon.hWnd = by;
        icon.uID = 1;
        icon.uFlags = NIF_INFO;
        icon.dwInfoFlags = NIIF_INFO | NIIF_RESPECT_QUIET_TIME;
        /* Windows shows at most 63 units of a title and 255 of its words. */
        hero_ui_win_copy_units(icon.szInfoTitle, sizeof icon.szInfoTitle / sizeof icon.szInfoTitle[0], heading);
        hero_ui_win_copy_units(icon.szInfo, sizeof icon.szInfo / sizeof icon.szInfo[0], said[0] != 0 ? said : L" ");
        if (!Shell_NotifyIconW(NIM_MODIFY, &icon)) {
            answer = hero_ui_win_failed("showing a notification", GetLastError());
        }
    }
    hero_layer_free(heading);
    hero_layer_free(said);
    return answer;
}


/* -- THE SESSION'S END ASKING THE PROGRAM ----------------------------------
 *
 * Windows asks each top-level window of the app to let the session end, at
 * the person's logout, restart or shutdown, and asks it to end at another
 * program's request through the Restart Manager (`ENDSESSION_CLOSEAPP`):
 * `WM_QUERYENDSESSION`, which is answered at once. The first window asked
 * queues the ask, `QUIT` 1 for the session's end and 0 for another
 * program's, and every window answers FALSE while an ask of that kind is
 * held, so nothing ends until the program answers: Windows shows the person
 * the app as one keeping the session from ending, with the reason this arm
 * registers on the window first asked (`ShutdownBlockReasonCreate`), and
 * the person may end the session anyway or cancel it. `answer_quit` takes
 * the reason back (`ShutdownBlockReasonDestroy`) and holds nothing more:
 * with `go` 1 the program then closes the app, and the session's end goes
 * on without it; with `go` 0 the app stays, and the session's end it held
 * waits for the person's Cancel on Windows' screen, since no call of an app
 * cancels it. A session's end Windows forces (`ENDSESSION_CRITICAL`) is
 * told and not held, since nothing holds it; one cancelled, by the person
 * or by another program (`WM_ENDSESSION` with FALSE), answers every ask
 * held as staying. Windows lets no app without a visible window hold the
 * session's end: it ends such an app. `inject` sends a window of the app
 * the same `WM_QUERYENDSESSION`, so the ask takes the system's path. */

#ifndef ENDSESSION_CLOSEAPP
#define ENDSESSION_CLOSEAPP 0x00000001
#endif
#ifndef ENDSESSION_CRITICAL
#define ENDSESSION_CRITICAL 0x40000000
#endif

/* `WM_QUERYENDSESSION` to window `hwnd`, `why` its reasons: the ask queued
 * where none of its kind is held, and FALSE, nothing ending, while the
 * program has not answered, but where Windows forces the end. Told by the
 * path no setting of the program mutes, so no ask is lost. */
static LRESULT hero_ui_win_end_asked(HWND hwnd, LPARAM why) {
    bool session = ((DWORD)why & ENDSESSION_CLOSEAPP) == 0;
    bool forced = ((DWORD)why & ENDSESSION_CRITICAL) != 0;
    bool *held = session ? &hero_ui_win_session_held : &hero_ui_win_closing_held;

    if (!*held) {
        *held = true;
        if (session && !forced && hero_ui_win_blocking == NULL && ShutdownBlockReasonCreate(hwnd, L"Saving what would be lost before it ends")) {
            hero_ui_win_blocking = hwnd;
        }
        hero_ui_tell_text(HERO_UI_EVENT_QUIT, 0, session ? 1 : 0, 0, NULL, 0);
    }
    return forced ? TRUE : FALSE;
}

/* Answers every ask held, the reason Windows shows taken back. */
static void hero_ui_win_quits_answered(void) {
    if (hero_ui_win_blocking != NULL) {
        (void)ShutdownBlockReasonDestroy(hero_ui_win_blocking);
        hero_ui_win_blocking = NULL;
    }
    hero_ui_win_session_held = false;
    hero_ui_win_closing_held = false;
}

int64_t hero_ui_arm_answer_quit(int64_t go) {
    /* Either way the app holds nothing more: `go` 1 is the program's close
     * to come, and 0 the app staying (THE SESSION'S END). */
    (void)go;
    hero_ui_win_quits_answered();
    return HERO_UI_OK;
}

int64_t hero_ui_arm_ask_quit(int64_t session_end) {
    hero_ui_native *asked = NULL;

    /* A window of the app, else its tray's hidden window, each top-level
     * and so asked by Windows. */
    for (int round = 0; round < 2 && asked == NULL; round += 1) {
        for (size_t at = 0; at < hero_ui_map_room() && asked == NULL; at += 1) {
            hero_ui_native *each = hero_ui_at(at);

            if (each != NULL && each->hwnd != NULL && each->kind == (round == 0 ? HERO_UI_WINDOW : HERO_UI_TRAY)) {
                asked = each;
            }
        }
    }
    if (asked == NULL) {
        return hero_ui_refused(HERO_UI_OTHER, "the app shows no window and no tray icon, and Windows asks an app to end through its top-level windows alone");
    }
    (void)SendMessageW(asked->hwnd, WM_QUERYENDSESSION, 0, session_end == 1 ? 0 : (LPARAM)ENDSESSION_CLOSEAPP);
    return HERO_UI_OK;
}


/* -- FILES DROPPED ON A WINDOW ---------------------------------------------
 *
 * A window that takes files is registered with the shell's drop of files,
 * `DragAcceptFiles`, which marks it `WS_EX_ACCEPTFILES`: a drop on it, or on
 * a control of it that takes no drop of its own (an `EDIT` control takes
 * none), comes to the window as `WM_DROPFILES`, whose `HDROP` holds the
 * paths (`DragQueryFileW`) and the point in the window's client area
 * (`DragQueryPoint`), in pixels, told in points of the window's scale; a web
 * view takes its drops itself, through WebView2's own drop target. `inject`
 * sends the window the same message with an `HDROP` of the arm's making, a
 * `DROPFILES` of UTF-16 paths in a movable global block, the layout
 * `DragQueryFileW` reads in a drop and in the clipboard alike; the arm gives
 * that block back itself, and `DragFinish` the shell's. */

/* The drop `inject` is sending, which the arm gives back itself. */
static _Thread_local HDROP hero_ui_win_own_drop;

/* `WM_DROPFILES` to window `hwnd`: the paths and the point told where the
 * window takes files, and the shell's drop given back. A path no UTF-8
 * names (an unpaired surrogate) is passed over, and a drop left with none
 * tells nothing. */
static void hero_ui_win_files_dropped(HWND hwnd, HDROP drop) {
    hero_ui_native *window = hero_ui_win_of(hwnd);

    if (window != NULL && window->kind == HERO_UI_WINDOW && window->takes_files) {
        UINT files = DragQueryFileW(drop, 0xFFFFFFFFu, NULL, 0);
        uint8_t *paths = NULL;
        int64_t count = 0;
        int64_t room = 0;
        POINT at = {0, 0};
        bool kept = true;

        for (UINT i = 0; i < files && kept; i += 1) {
            UINT units = DragQueryFileW(drop, i, NULL, 0);
            wchar_t *path = units > 0 ? hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t)) : NULL;

            if (units > 0 && path == NULL) {
                kept = false;
            } else if (path != NULL && DragQueryFileW(drop, i, path, units + 1) == units) {
                kept = hero_ui_win_keep_path(path, (int)units, &paths, &count, &room);
            }
            hero_layer_free(path);
        }
        (void)DragQueryPoint(drop, &at);
        if (kept && count > 0) {
            hero_ui_tell_text(HERO_UI_EVENT_DROP, window->id, hero_ui_win_pt(at.x > 0 ? at.x : 0, window->dpi), hero_ui_win_pt(at.y > 0 ? at.y : 0, window->dpi), paths, count);
        }
        hero_layer_free(paths);
    }
    if (drop != hero_ui_win_own_drop) {
        DragFinish(drop);
    }
}

int64_t hero_ui_arm_set_drop(int64_t id, int64_t on) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);

    if (window == NULL) {
        return kind;
    }
    DragAcceptFiles(window->hwnd, on == 1 ? TRUE : FALSE);
    window->takes_files = on == 1;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_drop(int64_t id, int64_t x, int64_t y, const uint8_t *paths, int64_t count) {
    int64_t kind;
    hero_ui_native *window = hero_ui_win_window_of(id, &kind);
    RECT content;
    HGLOBAL block;
    UINT format = 0;
    int64_t answer = HERO_UI_OK;
    uint8_t *into;
    int across;
    int down;

    if (window == NULL) {
        return kind;
    }
    if (!window->takes_files) {
        return HERO_UI_INVALID;
    }
    if (!GetClientRect(window->hwnd, &content)) {
        return hero_ui_win_failed("reading a window's content to drop on", GetLastError());
    }
    across = hero_ui_win_px(x, window->dpi);
    down = hero_ui_win_px(y, window->dpi);
    if (across >= content.right || down >= content.bottom) {
        return HERO_UI_INVALID;
    }
    block = hero_ui_win_clip_block(HERO_UI_CLIP_FILES, paths, count, 0, 0, &format, &answer);
    if (block == NULL) {
        return answer;
    }
    into = GlobalLock(block);
    if (into == NULL) {
        answer = hero_ui_win_failed("writing the paths dropped", GetLastError());
        (void)GlobalFree(block);
        return answer;
    }
    ((DROPFILES *)(void *)into)->pt.x = across;
    ((DROPFILES *)(void *)into)->pt.y = down;
    ((DROPFILES *)(void *)into)->fNC = FALSE;
    (void)GlobalUnlock(block);
    hero_ui_win_own_drop = (HDROP)block;
    (void)SendMessageW(window->hwnd, WM_DROPFILES, (WPARAM)block, 0);
    hero_ui_win_own_drop = NULL;
    (void)GlobalFree(block);
    return HERO_UI_OK;
}

/* -- THE APP'S IDENTITY, ONE INSTANCE, AND DOCUMENTS FROM THE SYSTEM --------
 *
 * THE IDENTITY is the process's explicit AppUserModelID
 * (`SetCurrentProcessExplicitAppUserModelID`), by which the task bar groups
 * the app's windows and pins its icon; its name is taken and shown nowhere,
 * since Windows has no application menu and the program titles its own
 * windows.
 *
 * ONE INSTANCE is a named mutex of the session (`Local\hero-ui-<identifier>`),
 * which the system takes back with the process, so a process that ended
 * leaves no claim behind. The first instance also keeps a named pipe of the
 * machine (`\\.\pipe\hero-ui-<identifier>-<session>`) that a thread of its
 * own listens on, one client at a time, each read held to a deadline and the
 * text it hands bounded; the pipe takes no client of another machine
 * (`PIPE_REJECT_REMOTE_CLIENTS`) and, by the default access the system gives
 * it, none that writes from another user's account. A later launch writes
 * its arguments to it, and the thread queues them and posts the UI thread's
 * hidden window, which tells them as `HERO_UI_EVENT_OPEN`. Only the session's
 * own launches meet: a user's second session claims a second instance.
 *
 * DOCUMENTS reach an app on Windows as the command line of a new process,
 * which is a later launch's arguments handed over; the system makes no open
 * request of its own to a running app. */

/* How long a later launch waits for the first instance's pipe, and the first
 * instance for a client's bytes, in milliseconds. */
#define HERO_UI_WIN_CLAIM_WAIT_MS 5000u
#define HERO_UI_WIN_CLAIM_READ_MS 2000u
#define HERO_UI_WIN_CLAIM_NAME_ROOM 320

/* One text a later launch handed over, waiting for the UI thread. */
typedef struct hero_ui_win_handed {
    struct hero_ui_win_handed *next;
    int64_t count;
    uint8_t bytes[1];
} hero_ui_win_handed;

/* This process's claim of its one instance: the mutex that holds it, the
 * pipe and the thread that listens, the events that stop the thread and
 * that its reads wait on, the window it posts, the texts it queued (under
 * `lock`) and the pipe's name. */
typedef struct hero_ui_win_claim {
    HANDLE mutex;
    HANDLE pipe;
    HANDLE thread;
    HANDLE stop;
    HANDLE io;
    HWND host;
    CRITICAL_SECTION lock;
    hero_ui_win_handed *first;
    hero_ui_win_handed *last;
    wchar_t pipe_name[HERO_UI_WIN_CLAIM_NAME_ROOM];
} hero_ui_win_claim;

static _Thread_local hero_ui_win_claim *hero_ui_win_claimed;

int64_t hero_ui_arm_set_identity(const uint8_t *identifier, int64_t identifier_count, const uint8_t *name, int64_t name_count) {
    wchar_t wide[HERO_UI_IDENTITY_MOST + 1];
    HRESULT made;

    (void)name;
    (void)name_count;
    for (int64_t at = 0; at < identifier_count && at < HERO_UI_IDENTITY_MOST; at += 1) {
        wide[at] = (wchar_t)identifier[at];
    }
    wide[identifier_count < HERO_UI_IDENTITY_MOST ? identifier_count : HERO_UI_IDENTITY_MOST] = 0;
    made = SetCurrentProcessExplicitAppUserModelID(wide);
    if (FAILED(made)) {
        return hero_ui_win_failed("giving the app its identity (SetCurrentProcessExplicitAppUserModelID)", (DWORD)HRESULT_CODE(made));
    }
    return HERO_UI_OK;
}

/* Writes the claim's names for `identifier`: the mutex's into `mutex` and
 * the pipe's into `pipe`, each `HERO_UI_WIN_CLAIM_NAME_ROOM` units. */
static void hero_ui_win_claim_names(const uint8_t *identifier, int64_t count, wchar_t *mutex, wchar_t *pipe) {
    static const wchar_t local[] = L"Local\\hero-ui-";
    static const wchar_t machine[] = L"\\\\.\\pipe\\hero-ui-";
    DWORD session = 0;
    size_t used;

    if (!ProcessIdToSessionId(GetCurrentProcessId(), &session)) {
        session = 0;
    }
    wcscpy(mutex, local);
    wcscpy(pipe, machine);
    used = wcslen(machine);
    for (int64_t at = 0; at < count && at < HERO_UI_IDENTITY_MOST; at += 1) {
        mutex[wcslen(local) + (size_t)at] = (wchar_t)identifier[at];
        pipe[used + (size_t)at] = (wchar_t)identifier[at];
    }
    mutex[wcslen(local) + (size_t)(count < HERO_UI_IDENTITY_MOST ? count : HERO_UI_IDENTITY_MOST)] = 0;
    pipe[used + (size_t)(count < HERO_UI_IDENTITY_MOST ? count : HERO_UI_IDENTITY_MOST)] = 0;
    used = wcslen(pipe);
    (void)swprintf(pipe + used, HERO_UI_WIN_CLAIM_NAME_ROOM - used, L"-%lu", (unsigned long)session);
}

/* Reads `count` bytes the client wrote, through the claim's event, within
 * `HERO_UI_WIN_CLAIM_READ_MS` and no longer than the claim lives. */
static bool hero_ui_win_claim_read(hero_ui_win_claim *claim, void *into, DWORD count) {
    ULONGLONG deadline = GetTickCount64() + HERO_UI_WIN_CLAIM_READ_MS;
    uint8_t *at = into;
    DWORD got = 0;

    while (got < count) {
        OVERLAPPED over;
        DWORD chunk = 0;

        memset(&over, 0, sizeof over);
        over.hEvent = claim->io;
        (void)ResetEvent(claim->io);
        if (!ReadFile(claim->pipe, at + got, count - got, NULL, &over)) {
            ULONGLONG now = GetTickCount64();
            HANDLE both[2];

            if (GetLastError() != ERROR_IO_PENDING) {
                return false;
            }
            both[0] = claim->stop;
            both[1] = claim->io;
            if (WaitForMultipleObjects(2, both, FALSE, now >= deadline ? 0 : (DWORD)(deadline - now)) != WAIT_OBJECT_0 + 1) {
                (void)CancelIoEx(claim->pipe, &over);
                (void)GetOverlappedResult(claim->pipe, &over, &chunk, TRUE);
                return false;
            }
        }
        if (!GetOverlappedResult(claim->pipe, &over, &chunk, FALSE) || chunk == 0) {
            return false;
        }
        got += chunk;
    }
    return true;
}

/* Takes the one text a connected client wrote, a length of four bytes and
 * that many bytes of UTF-8 whose every item ends in a zero, queues it for
 * the UI thread and tells it; anything else is dropped. */
static void hero_ui_win_claim_take(hero_ui_win_claim *claim) {
    uint8_t head[4];
    uint32_t length;
    hero_ui_win_handed *text;

    if (!hero_ui_win_claim_read(claim, head, sizeof head)) {
        return;
    }
    length = (uint32_t)head[0] | ((uint32_t)head[1] << 8) | ((uint32_t)head[2] << 16) | ((uint32_t)head[3] << 24);
    if (length > (uint32_t)HERO_UI_TEXT_MOST) {
        return;
    }
    text = hero_layer_alloc(sizeof *text + length);
    if (text == NULL) {
        return;
    }
    text->next = NULL;
    text->count = (int64_t)length;
    if ((length > 0 && !hero_ui_win_claim_read(claim, text->bytes, length)) || !hero_ui_is_utf8(text->bytes, length) || (length > 0 && text->bytes[length - 1] != 0)) {
        hero_layer_free(text);
        return;
    }
    EnterCriticalSection(&claim->lock);
    if (claim->last != NULL) {
        claim->last->next = text;
    } else {
        claim->first = text;
    }
    claim->last = text;
    LeaveCriticalSection(&claim->lock);
    (void)PostMessageW(claim->host, HERO_UI_WIN_HANDED, 0, 0);
}

/* The thread that listens on the claim's pipe, one client at a time, until
 * the claim's stop event is set. */
static DWORD WINAPI hero_ui_win_claim_serve(LPVOID given) {
    hero_ui_win_claim *claim = given;

    for (;;) {
        OVERLAPPED over;
        DWORD ignored = 0;
        BOOL connected;

        memset(&over, 0, sizeof over);
        over.hEvent = claim->io;
        (void)ResetEvent(claim->io);
        connected = ConnectNamedPipe(claim->pipe, &over);
        if (!connected && GetLastError() == ERROR_IO_PENDING) {
            HANDLE both[2];

            both[0] = claim->stop;
            both[1] = claim->io;
            if (WaitForMultipleObjects(2, both, FALSE, INFINITE) != WAIT_OBJECT_0 + 1) {
                (void)CancelIoEx(claim->pipe, &over);
                (void)GetOverlappedResult(claim->pipe, &over, &ignored, TRUE);
                return 0;
            }
            connected = GetOverlappedResult(claim->pipe, &over, &ignored, FALSE);
        } else if (!connected && GetLastError() == ERROR_PIPE_CONNECTED) {
            connected = TRUE;
        }
        if (connected) {
            hero_ui_win_claim_take(claim);
        }
        (void)DisconnectNamedPipe(claim->pipe);
        if (WaitForSingleObject(claim->stop, connected ? 0 : 100) == WAIT_OBJECT_0) {
            return 0;
        }
    }
}

/* Writes `count` bytes of arguments to the pipe `name` as a later launch
 * does, waiting for the first instance's pipe up to
 * `HERO_UI_WIN_CLAIM_WAIT_MS`; 0 where they were taken, else the Windows
 * error. */
static DWORD hero_ui_win_claim_send(const wchar_t *name, const uint8_t *arguments, int64_t count) {
    ULONGLONG deadline = GetTickCount64() + HERO_UI_WIN_CLAIM_WAIT_MS;
    HANDLE pipe = INVALID_HANDLE_VALUE;
    uint8_t head[4];
    DWORD written = 0;
    DWORD code = 0;
    int64_t sent = 0;

    for (;;) {
        ULONGLONG now = GetTickCount64();

        /* The identification level lets the pipe's owner know who writes and
         * nothing more: it may not act as the writer. */
        pipe = CreateFileW(name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION, NULL);
        if (pipe != INVALID_HANDLE_VALUE) {
            break;
        }
        code = GetLastError();
        if (now >= deadline || (code != ERROR_PIPE_BUSY && code != ERROR_FILE_NOT_FOUND)) {
            return code != 0 ? code : ERROR_TIMEOUT;
        }
        if (code == ERROR_PIPE_BUSY) {
            (void)WaitNamedPipeW(name, 100);
        } else {
            Sleep(20);
        }
    }
    head[0] = (uint8_t)(count & 0xff);
    head[1] = (uint8_t)((count >> 8) & 0xff);
    head[2] = (uint8_t)((count >> 16) & 0xff);
    head[3] = (uint8_t)((count >> 24) & 0xff);
    code = 0;
    if (!WriteFile(pipe, head, sizeof head, &written, NULL) || written != sizeof head) {
        code = GetLastError() != 0 ? GetLastError() : ERROR_WRITE_FAULT;
    }
    while (code == 0 && sent < count) {
        DWORD part = (DWORD)(count - sent > (int64_t)(1u << 20) ? (int64_t)(1u << 20) : count - sent);

        if (!WriteFile(pipe, arguments + sent, part, &written, NULL) || written == 0) {
            code = GetLastError() != 0 ? GetLastError() : ERROR_WRITE_FAULT;
        } else {
            sent += written;
        }
    }
    /* What is written stays readable by the first instance after this end
     * closes, so nothing waits here for it. */
    (void)CloseHandle(pipe);
    return code;
}

/* Ends claim `claim`: the thread stopped, every handle given back, the
 * texts still queued dropped. */
static void hero_ui_win_claim_free(hero_ui_win_claim *claim) {
    hero_ui_win_handed *text;

    if (claim->thread != NULL) {
        (void)SetEvent(claim->stop);
        (void)WaitForSingleObject(claim->thread, INFINITE);
        (void)CloseHandle(claim->thread);
    }
    if (claim->pipe != INVALID_HANDLE_VALUE && claim->pipe != NULL) {
        (void)CloseHandle(claim->pipe);
    }
    if (claim->stop != NULL) {
        (void)CloseHandle(claim->stop);
    }
    if (claim->io != NULL) {
        (void)CloseHandle(claim->io);
    }
    if (claim->mutex != NULL) {
        (void)CloseHandle(claim->mutex);
    }
    for (text = claim->first; text != NULL;) {
        hero_ui_win_handed *next = text->next;

        hero_layer_free(text);
        text = next;
    }
    DeleteCriticalSection(&claim->lock);
    hero_layer_free(claim);
}

int64_t hero_ui_arm_claim_instance(const uint8_t *identifier, int64_t identifier_count, const uint8_t *arguments, int64_t count, int64_t *first) {
    wchar_t mutex_name[HERO_UI_WIN_CLAIM_NAME_ROOM];
    wchar_t pipe_name[HERO_UI_WIN_CLAIM_NAME_ROOM];
    hero_ui_win_claim *claim;
    HANDLE mutex;
    DWORD code;
    bool taken;

    if (hero_ui_win_claimed != NULL) {
        *first = 1;
        return HERO_UI_OK;
    }
    hero_ui_win_claim_names(identifier, identifier_count, mutex_name, pipe_name);
    mutex = CreateMutexW(NULL, FALSE, mutex_name);
    code = GetLastError();
    /* A mutex made by a process of another elevation refuses to be opened,
     * which says as much as finding it made. */
    taken = (mutex != NULL && code == ERROR_ALREADY_EXISTS) || (mutex == NULL && code == ERROR_ACCESS_DENIED);
    if (taken) {
        if (mutex != NULL) {
            (void)CloseHandle(mutex);
        }
        code = hero_ui_win_claim_send(pipe_name, arguments, count);
        if (code != 0) {
            return hero_ui_win_failed("handing the arguments to the app's first instance", code);
        }
        *first = 0;
        return HERO_UI_OK;
    }
    if (mutex == NULL) {
        return hero_ui_win_failed("claiming the app's one instance (CreateMutexW)", code);
    }
    claim = hero_layer_alloc_zero(1, sizeof *claim);
    if (claim == NULL) {
        (void)CloseHandle(mutex);
        return HERO_UI_LIMIT;
    }
    InitializeCriticalSection(&claim->lock);
    claim->mutex = mutex;
    claim->pipe = INVALID_HANDLE_VALUE;
    wcscpy(claim->pipe_name, pipe_name);
    if (hero_ui_win_dialog_host == NULL) {
        hero_ui_win_dialog_host = CreateWindowExW(0, hero_ui_win_panel_class, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandleW(NULL), NULL);
    }
    claim->host = hero_ui_win_dialog_host;
    claim->stop = CreateEventW(NULL, TRUE, FALSE, NULL);
    claim->io = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (claim->host == NULL || claim->stop == NULL || claim->io == NULL) {
        code = GetLastError();
        hero_ui_win_claim_free(claim);
        return hero_ui_win_failed("making what the app's one instance listens with", code);
    }
    claim->pipe = CreateNamedPipeW(pipe_name, PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS, 1, 4096, 4096, 0, NULL);
    if (claim->pipe == INVALID_HANDLE_VALUE) {
        code = GetLastError();
        hero_ui_win_claim_free(claim);
        return hero_ui_win_failed("making the pipe a later launch hands its arguments to", code);
    }
    claim->thread = CreateThread(NULL, 0, hero_ui_win_claim_serve, claim, 0, NULL);
    if (claim->thread == NULL) {
        code = GetLastError();
        hero_ui_win_claim_free(claim);
        return hero_ui_win_failed("making the thread that listens for a later launch", code);
    }
    hero_ui_win_claimed = claim;
    *first = 1;
    return HERO_UI_OK;
}

int64_t hero_ui_arm_hand_over(const uint8_t *arguments, int64_t count) {
    DWORD code;

    if (hero_ui_win_claimed == NULL) {
        return HERO_UI_INVALID;
    }
    code = hero_ui_win_claim_send(hero_ui_win_claimed->pipe_name, arguments, count);
    return code == 0 ? HERO_UI_OK : hero_ui_win_failed("handing a later launch's arguments to the app", code);
}

int64_t hero_ui_arm_open_items(int64_t what, const uint8_t *items, int64_t count) {
    (void)what;
    (void)items;
    (void)count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "Windows opens a document in an app as the command line of a new process, which the app's one instance takes as a later launch's arguments, and makes no open request of its own to a running app");
}

void hero_ui_arm_instance_end(void) {
    if (hero_ui_win_claimed != NULL) {
        hero_ui_win_claim_free(hero_ui_win_claimed);
        hero_ui_win_claimed = NULL;
    }
}

/* The texts a later launch handed over, told to the program as the
 * arguments of an opening (`HERO_UI_WIN_HANDED`). */
static void hero_ui_win_handed_take(void) {
    hero_ui_win_claim *claim = hero_ui_win_claimed;

    while (claim != NULL) {
        hero_ui_win_handed *text;
        int64_t items = 0;

        EnterCriticalSection(&claim->lock);
        text = claim->first;
        if (text != NULL) {
            claim->first = text->next;
            if (claim->first == NULL) {
                claim->last = NULL;
            }
        }
        LeaveCriticalSection(&claim->lock);
        if (text == NULL) {
            return;
        }
        for (int64_t at = 0; at < text->count; at += 1) {
            items += text->bytes[at] == 0 ? 1 : 0;
        }
        hero_ui_tell_text(HERO_UI_EVENT_OPEN, 0, items, HERO_UI_OPEN_ARGUMENTS, text->bytes, text->count);
        hero_layer_free(text);
    }
}

/* -- A TIP OVER A WIDGET ------------------------------------------------------
 *
 * A top-level window keeps one tooltip control (`TOOLTIPS_CLASS`, made at its
 * first tip) and a widget's control is one of its tools, by its handle
 * (`TTF_IDISHWND`) and with the tooltip subclassing it (`TTF_SUBCLASS`), so
 * the system shows the tip when the pointer rests on the control, after its
 * own delay. A tooltip breaks a line wider than its limit unless it is given
 * one, so the limit stands at a width that keeps a line of words; the
 * program's own line feeds break lines too. The tip is also kept in the
 * widget, UTF-16 with CR LF, and read back from there.
 *
 * A radio group's tip is shown over its panel and each of its items; a tray
 * icon's is the text Windows shows over it, which the icon's own text gives
 * where it was given no tip (at most its first 127 units are held); a menu
 * item has none, Windows showing no tip over a menu item, and a web view
 * leaves its tips to its page. */

/* The width a tip may take before it breaks a line, in points. */
#define HERO_UI_WIN_TIP_WIDTH 320

/* The kinds whose control the pointer rests on: not a window, a menu, a menu
 * item or separator, a context menu, a page, a timer, a tray icon, which
 * `hero_ui_arm_set_tip` tells apart. */
static bool hero_ui_win_tips_over(const hero_ui_native *widget) {
    switch (widget->kind) {
    case HERO_UI_WINDOW:
    case HERO_UI_MENU:
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
    case HERO_UI_CONTEXT_MENU:
    case HERO_UI_PAGE:
    case HERO_UI_TRAY:
        return false;
    default:
        return widget->hwnd != NULL;
    }
}

/* The tooltip control of `window`, made where it has none; NULL where
 * Windows would not. */
static HWND hero_ui_win_tooltips_of(hero_ui_native *window) {
    if (window->tooltips == NULL) {
        window->tooltips = CreateWindowExW(WS_EX_TOPMOST, TOOLTIPS_CLASSW, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, window->hwnd, NULL, GetModuleHandleW(NULL), NULL);
        if (window->tooltips != NULL) {
            (void)SendMessageW(window->tooltips, TTM_SETMAXTIPWIDTH, 0, (LPARAM)hero_ui_win_px(HERO_UI_WIN_TIP_WIDTH, window->dpi));
        }
    }
    return window->tooltips;
}

/* Makes `control` a tool of `tooltips` showing `text`, or takes it out
 * where `text` is NULL; whether Windows did. */
static bool hero_ui_win_tool(HWND tooltips, HWND owner, HWND control, wchar_t *text) {
    TOOLINFOW tool;

    memset(&tool, 0, sizeof tool);
    tool.cbSize = (UINT)CCSIZEOF_STRUCT(TTTOOLINFOW, lpszText);
    tool.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    tool.hwnd = owner;
    tool.uId = (UINT_PTR)control;
    (void)SendMessageW(tooltips, TTM_DELTOOLW, 0, (LPARAM)&tool);
    if (text == NULL) {
        return true;
    }
    tool.lpszText = text;
    return SendMessageW(tooltips, TTM_ADDTOOLW, 0, (LPARAM)&tool) != FALSE;
}

/* Gives every control of `widget` its tip, or takes the tool of each away
 * where it has none; whether Windows did. */
static bool hero_ui_win_tip_apply(hero_ui_native *widget) {
    hero_ui_native *window = hero_ui_win_top(widget->window);
    HWND tooltips = window != NULL ? (widget->tip != NULL ? hero_ui_win_tooltips_of(window) : window->tooltips) : NULL;
    bool done = true;

    if (tooltips == NULL) {
        return widget->tip == NULL;
    }
    /* A static control lets the mouse through to its window unless it is
     * asked to be notified, and a tip is shown where the mouse rests on
     * the control. */
    if (widget->tip != NULL && (widget->kind == HERO_UI_LABEL || widget->kind == HERO_UI_SEPARATOR)) {
        (void)SetWindowLongPtrW(widget->hwnd, GWL_STYLE, GetWindowLongPtrW(widget->hwnd, GWL_STYLE) | SS_NOTIFY);
    }
    done = hero_ui_win_tool(tooltips, window->hwnd, widget->hwnd, widget->tip);
    if (widget->part != NULL) {
        done = hero_ui_win_tool(tooltips, window->hwnd, widget->part, widget->tip) && done;
    }
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        done = hero_ui_win_tool(tooltips, window->hwnd, widget->radios[i], widget->tip) && done;
    }
    return done;
}

/* Takes `widget`'s tip and its tools away, as it is destroyed. */
static void hero_ui_win_tip_end(hero_ui_native *widget) {
    if (widget->tip == NULL) {
        return;
    }
    if (widget->kind != HERO_UI_TRAY) {
        hero_layer_free(widget->tip);
        widget->tip = NULL;
        (void)hero_ui_win_tip_apply(widget);
        return;
    }
    hero_layer_free(widget->tip);
    widget->tip = NULL;
}

int64_t hero_ui_arm_set_tip(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    wchar_t *wide;
    bool done;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_MENU_ITEM) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "Windows shows no tip over a menu item: a menu draws its items as it has them and gives them none");
    }
    if (widget->kind == HERO_UI_WEBVIEW) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a web view leaves its tips to its page, which shows an element's own (its title), and the layer gives the view none");
    }
    if (widget->kind != HERO_UI_TRAY && !hero_ui_win_tips_over(widget)) {
        return HERO_UI_INVALID;
    }
    wide = count > 0 ? hero_ui_win_wide(text, count, widget->kind == HERO_UI_TRAY ? 0u : HERO_UI_WIN_LINES) : NULL;
    if (count > 0 && wide == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_layer_free(widget->tip);
    widget->tip = wide;
    if (widget->kind == HERO_UI_TRAY) {
        return !widget->tray_shown || hero_ui_win_tray_show(widget, NIM_MODIFY) ? HERO_UI_OK : hero_ui_win_failed("giving a tray icon its tip", GetLastError());
    }
    done = hero_ui_win_tip_apply(widget);
    return done ? HERO_UI_OK : hero_ui_win_failed("giving a widget its tip (TTM_ADDTOOLW)", GetLastError());
}

int64_t hero_ui_arm_tip(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);
    wchar_t held[128];
    uint8_t *narrow;
    int64_t bytes = 0;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_MENU_ITEM) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "Windows shows no tip over a menu item: a menu draws its items as it has them and gives them none");
    }
    if (widget->kind == HERO_UI_WEBVIEW) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a web view leaves its tips to its page, which shows an element's own (its title), and the layer gives the view none");
    }
    if (widget->kind != HERO_UI_TRAY && !hero_ui_win_tips_over(widget)) {
        return HERO_UI_INVALID;
    }
    *length = 0;
    if (widget->tip == NULL) {
        return HERO_UI_OK;
    }
    /* The tray holds the first 127 units of its tip. */
    if (widget->kind == HERO_UI_TRAY) {
        hero_ui_win_copy_units(held, sizeof held / sizeof held[0], widget->tip);
    }
    narrow = hero_ui_win_narrow(widget->kind == HERO_UI_TRAY ? held : widget->tip, (int)wcslen(widget->kind == HERO_UI_TRAY ? held : widget->tip), widget->kind != HERO_UI_TRAY, &bytes);
    if (narrow == NULL) {
        return HERO_UI_LIMIT;
    }
    if (bytes <= count && bytes > 0) {
        memcpy(buffer, narrow, (size_t)bytes);
    }
    hero_layer_free(narrow);
    *length = bytes;
    return HERO_UI_OK;
}

/* -- A DRAWING AREA (`hero_ui_canvas_win32.c`) ------------------------------
 *
 * The area's window is this arm's; its drawing, its pointer and its keys are
 * the member's, which these calls reach with the window and its scale. */

/* Drawing area `id`, or NULL with `*answer` `NOT_FOUND` or `INVALID`. */
static hero_ui_native *hero_ui_win_canvas_of(int64_t id, int64_t *answer) {
    hero_ui_native *area = hero_ui_find(id);

    *answer = area == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    return area != NULL && area->kind == HERO_UI_CANVAS && area->canvas != NULL && area->hwnd != NULL ? area : NULL;
}

int64_t hero_ui_arm_set_drawing(int64_t id, const uint8_t *list, int64_t count) {
    int64_t answer;
    hero_ui_native *area = hero_ui_win_canvas_of(id, &answer);

    return area == NULL ? answer : hero_ui_canvas_win32_set(area->canvas, area->hwnd, list, count);
}

int64_t hero_ui_arm_snapshot(int64_t id, uint8_t *rgba, int64_t count, int64_t *width, int64_t *height) {
    int64_t answer;
    hero_ui_native *area = hero_ui_win_canvas_of(id, &answer);

    if (area == NULL) {
        return answer;
    }
    /* The size the layer placed it at, in points. */
    *width = area->width;
    *height = area->height;
    return hero_ui_canvas_win32_snapshot(area->canvas, area->width, area->height, rgba, count);
}

int64_t hero_ui_arm_pointer(int64_t id, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, int64_t modifiers) {
    int64_t answer;
    hero_ui_native *area = hero_ui_win_canvas_of(id, &answer);

    return area == NULL ? answer : hero_ui_canvas_win32_pointer(area->canvas, area->id, area->hwnd, hero_ui_win_canvas_dpi(area), x, y, phase, button, clicks, modifiers);
}

/* -- A DRAWING AREA'S PARTS (`hero_ui_parts_win32.c`) -----------------------
 *
 * The parts are the member's, made at the first parts the program gives, so
 * an area that never has any loads no UI Automation; the area's window, its
 * scale and its name are this arm's, handed to them. */

/* Drawing area `id`'s scale, asked at each reading of a part's box. */
static uint32_t hero_ui_win_parts_scale(int64_t id) {
    const hero_ui_native *area = hero_ui_find(id);

    return area != NULL ? hero_ui_win_canvas_dpi(area) : 96;
}

int64_t hero_ui_arm_set_parts(int64_t id, const uint8_t *list, int64_t count) {
    int64_t answer;
    hero_ui_native *area = hero_ui_win_canvas_of(id, &answer);

    if (area == NULL) {
        return answer;
    }
    if (area->parts == NULL) {
        if (count == 0) {
            /* None given and none to take back. */
            return hero_ui_parts_win32_offered();
        }
        area->parts = hero_ui_parts_win32_new(area->id, area->hwnd, hero_ui_win_parts_scale, &answer);
        if (area->parts == NULL) {
            return answer;
        }
        answer = hero_ui_win_parts_named(area);
        if (answer != HERO_UI_OK) {
            return answer;
        }
    }
    return hero_ui_parts_win32_set(area->parts, list, count);
}

int64_t hero_ui_arm_parts(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    int64_t answer;
    hero_ui_native *area = hero_ui_win_canvas_of(id, &answer);

    *length = 0;
    if (area == NULL) {
        return answer;
    }
    return area->parts == NULL ? hero_ui_parts_win32_offered() : hero_ui_parts_win32_read(area->parts, buffer, count, length);
}

int64_t hero_ui_arm_part_act(int64_t id, int64_t part, int64_t act) {
    int64_t answer;
    hero_ui_native *area = hero_ui_win_canvas_of(id, &answer);

    if (area == NULL) {
        return answer;
    }
    if (area->parts == NULL) {
        answer = hero_ui_parts_win32_offered();
        return answer != HERO_UI_OK ? answer : hero_ui_refused(HERO_UI_INVALID, "a screen reader's act on a part of a drawing area given no parts");
    }
    return hero_ui_parts_win32_act(area->parts, part, act);
}

#endif
