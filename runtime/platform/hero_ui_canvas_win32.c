/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_canvas_win32.c: THE WIN32 ARM'S DRAWING AREA, one member
 * of the platform layer's archive (`selfhost/cli/layer.hero`), compiled to
 * nothing on every other system. `hero_ui_win32.c` makes the area's window
 * and hands this member its messages; the display list's reading, the path,
 * the transform and the pixels' arithmetic are the pure half,
 * `hero_ui_win32_canvas.h`.
 *
 * DRAWN BY DIRECT2D, in software, into a bitmap of the member's own. Of the
 * two Windows offers, Direct2D is the one kept: DirectWrite's text beside it
 * finds a font for every character the program's font lacks, where GDI+
 * draws a box, and reads OpenType fonts GDI+ does not; its rasteriser is the
 * system's current one. The target is a DC render target of type SOFTWARE,
 * bound to a 32-bit DIB section, so the pixels a snapshot reads and the ones
 * the window shows come from the same function into the same kind of bitmap,
 * a target never meets a lost device (a GPU reset, a driver update, a remote
 * session), and no COM apartment is needed. A paint replays the drawing into
 * the part of the window to paint, at the window's scale, and blends it over
 * the window's colour as a picture's pixels are; a snapshot replays it whole
 * at one pixel to a point.
 *
 * THE INTERFACES ARE `hero_ui_win32_direct2d.h`'s, declared there for C
 * because the SDK declares them for C++ alone, and checked there against
 * MinGW-w64's at compile time. The two libraries are loaded from the system
 * folder by their full path and stay loaded for the process.
 *
 * THE CLIP is one layer at a time: a clip is the current one intersected
 * with the new path (`CombineWithGeometry`, in the target's space), and a
 * restore pushes the kept one back, so a drawing of many clips holds one
 * layer's bitmap, never one per clip.
 *
 * THE POINTER (`hero_ui.h`'s THE POINTER IN THE AREA): a press takes the
 * mouse (`SetCapture`) until its button is let go, so a drag is told to the
 * area it started in wherever the pointer goes; the entry is the first move
 * inside, the exit `TrackMouseEvent`'s `WM_MOUSELEAVE` or a captured move out;
 * clicks are counted as Windows counts a double click (its time and its
 * rectangle), since a window class without `CS_DBLCLKS` is sent no double
 * click message and Windows counts no third. A press the area loses its
 * capture during (another window takes the mouse) is told released where the
 * pointer was last, so every press the program hears has its release. An act
 * of `inject` is sent to the area's window as Windows' own message, the
 * point, the clicks and the keys held riding beside it, since a message
 * carries whole pixels and no Alt; a move, an entry and an exit are told at
 * once, as the macOS arm tells them.
 *
 * THE KEYS (`hero_ui.h`'s THE KEYS IN THE AREA): `WM_KEYDOWN` and `WM_KEYUP`,
 * and the system keys' `WM_SYSKEYDOWN` and `WM_SYSKEYUP`, which go on to
 * Windows afterwards so Alt+F4 and the menu's Alt still work; Shift,
 * Control, Alt, the Windows keys and Caps Lock are told as no key, as on
 * macOS. What a key types is the `WM_CHAR`s its press posted, taken from the
 * queue with it, none while Control is held without Alt (Control and Alt
 * together are AltGr, which types); a release tells what its press typed,
 * and a release whose press the area was not told is not told.
 *
 * Written from Microsoft's Direct2D, DirectWrite and Win32 documentation and
 * the SDK's headers; no other library's source.
 */

#include "hero_ui_canvas_win32.h"

#if defined(_WIN32)

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "hero_layer_alloc.h"
#include "platform/hero_ui_arm.h"

#include "hero_ui_win32_canvas.h"

#if defined(_MSC_VER)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "msimg32.lib")
#endif

_Static_assert(sizeof(WCHAR) == sizeof(uint16_t), "a WCHAR is a UTF-16 unit");

#include "hero_ui_win32_direct2d.h"

/* What the areas share, made at the first area and given back at `close`:
 * the UI thread's, as every window of the arm is (`hero_ui_win32.c`'s EVERY
 * CALL IS ON THE UI THREAD). */
static _Thread_local hero_d2_factory *hero_ui_canvas_factory;
static _Thread_local hero_dw_factory *hero_ui_canvas_writer;
static _Thread_local hero_dw_fonts *hero_ui_canvas_fonts;
static _Thread_local hero_d2_unknown *hero_ui_canvas_strokes[3][3];
static _Thread_local WCHAR hero_ui_canvas_system_family[LF_FACESIZE];
static _Thread_local WCHAR hero_ui_canvas_locale[LOCALE_NAME_MAX_LENGTH];

static void hero_ui_canvas_release(void *object) {
    if (object != NULL) {
        ((hero_d2_unknown *)object)->lpVtbl->Release((hero_d2_unknown *)object);
    }
}

/* -- The record -------------------------------------------------------------- */

/* What a key's press typed, kept for its release. */
typedef struct hero_ui_canvas_typed {
    bool told;
    uint8_t count;
    uint8_t typed[32];
} hero_ui_canvas_typed;

struct hero_ui_canvas_win32 {
    uint8_t *list;
    size_t count;
    hero_d2_target *target;
    hero_ui_draw_path path;
    /* THE POINTER: the buttons this area holds pressed (bit 1 << button),
     * the one it took the mouse for, each one's clicks, the last press for
     * counting, whether the pointer is inside and where it was last told. */
    unsigned held;
    int64_t dragging;
    int64_t clicks_of[4];
    hero_ui_draw_clicks clicks;
    bool inside;
    int64_t last_x;
    int64_t last_y;
    /* The act `inject` is sending, which the window procedure tells in place
     * of what the message's words say. */
    bool injecting;
    bool injected;
    UINT injected_message;
    int64_t injected_x;
    int64_t injected_y;
    uint8_t injected_bytes[4];
    hero_ui_canvas_typed keys[256];
};

/* Loads Direct2D and DirectWrite from the system folder and makes what the
 * areas share; `HERO_UI_OK`, or `HERO_UI_UNSUPPORTED` with words. */
static int64_t hero_ui_canvas_start(void) {
    WCHAR folder[MAX_PATH];
    WCHAR path[MAX_PATH + 16];
    UINT length;
    HMODULE d2d;
    HMODULE dwrite;
    FARPROC found;
    hero_ui_canvas_d2d_create make_d2d;
    hero_ui_canvas_dwrite_create make_dwrite;
    NONCLIENTMETRICSW metrics;

    _Static_assert(sizeof found == sizeof make_d2d && sizeof found == sizeof make_dwrite, "a function's address fits a FARPROC");
    if (hero_ui_canvas_factory != NULL && hero_ui_canvas_writer != NULL && hero_ui_canvas_fonts != NULL) {
        return HERO_UI_OK;
    }
    length = GetSystemDirectoryW(folder, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area needs Direct2D, and Windows did not name its system folder");
    }
    (void)_snwprintf(path, sizeof path / sizeof path[0], L"%ls\\d2d1.dll", folder);
    path[sizeof path / sizeof path[0] - 1] = 0;
    d2d = LoadLibraryW(path);
    (void)_snwprintf(path, sizeof path / sizeof path[0], L"%ls\\dwrite.dll", folder);
    path[sizeof path / sizeof path[0] - 1] = 0;
    dwrite = LoadLibraryW(path);
    if (d2d == NULL || dwrite == NULL) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area needs Direct2D and DirectWrite (d2d1.dll, dwrite.dll), which this Windows does not have");
    }
    found = GetProcAddress(d2d, "D2D1CreateFactory");
    memcpy(&make_d2d, &found, sizeof make_d2d);
    found = GetProcAddress(dwrite, "DWriteCreateFactory");
    memcpy(&make_dwrite, &found, sizeof make_dwrite);
    if (make_d2d == NULL || make_dwrite == NULL) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "a drawing area needs D2D1CreateFactory and DWriteCreateFactory, which this Windows' libraries do not export");
    }
    if (hero_ui_canvas_factory == NULL && FAILED(make_d2d(HERO_D2_FACTORY_SINGLE_THREADED, &hero_ui_canvas_d2d_iid, NULL, (void **)&hero_ui_canvas_factory))) {
        hero_ui_canvas_factory = NULL;
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "Direct2D made no factory (D2D1CreateFactory failed)");
    }
    if (hero_ui_canvas_writer == NULL && FAILED(make_dwrite(HERO_DW_FACTORY_SHARED, &hero_ui_canvas_dwrite_iid, (void **)&hero_ui_canvas_writer))) {
        hero_ui_canvas_writer = NULL;
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "DirectWrite made no factory (DWriteCreateFactory failed)");
    }
    if (FAILED(hero_ui_canvas_writer->lpVtbl->GetSystemFontCollection(hero_ui_canvas_writer, &hero_ui_canvas_fonts, FALSE))) {
        hero_ui_canvas_fonts = NULL;
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "DirectWrite did not give the system's fonts");
    }
    /* The system's font is the one its dialogs' text is written in. */
    memset(&metrics, 0, sizeof metrics);
    metrics.cbSize = (UINT)sizeof metrics;
    if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0) && metrics.lfMessageFont.lfFaceName[0] != 0) {
        memcpy(hero_ui_canvas_system_family, metrics.lfMessageFont.lfFaceName, sizeof hero_ui_canvas_system_family);
        hero_ui_canvas_system_family[LF_FACESIZE - 1] = 0;
    } else {
        (void)wcscpy(hero_ui_canvas_system_family, L"Segoe UI");
    }
    if (GetUserDefaultLocaleName(hero_ui_canvas_locale, LOCALE_NAME_MAX_LENGTH) == 0) {
        (void)wcscpy(hero_ui_canvas_locale, L"en-us");
    }
    return HERO_UI_OK;
}

hero_ui_canvas_win32 *hero_ui_canvas_win32_new(int64_t *answer) {
    hero_ui_canvas_win32 *canvas;

    *answer = hero_ui_canvas_start();
    if (*answer != HERO_UI_OK) {
        return NULL;
    }
    canvas = hero_layer_alloc_zero(1, sizeof *canvas);
    if (canvas == NULL) {
        *answer = HERO_UI_LIMIT;
    }
    return canvas;
}

void hero_ui_canvas_win32_free(hero_ui_canvas_win32 *canvas) {
    if (canvas == NULL) {
        return;
    }
    hero_ui_canvas_release(canvas->target);
    hero_ui_draw_path_free(&canvas->path);
    hero_layer_free(canvas->list);
    hero_layer_free(canvas);
}

void hero_ui_canvas_win32_end(void) {
    for (size_t end = 0; end < 3; end += 1) {
        for (size_t join = 0; join < 3; join += 1) {
            hero_ui_canvas_release(hero_ui_canvas_strokes[end][join]);
            hero_ui_canvas_strokes[end][join] = NULL;
        }
    }
    hero_ui_canvas_release(hero_ui_canvas_fonts);
    hero_ui_canvas_release(hero_ui_canvas_writer);
    hero_ui_canvas_release(hero_ui_canvas_factory);
    hero_ui_canvas_fonts = NULL;
    hero_ui_canvas_writer = NULL;
    hero_ui_canvas_factory = NULL;
}

int64_t hero_ui_canvas_win32_set(hero_ui_canvas_win32 *canvas, void *window, const uint8_t *list, int64_t count) {
    HWND hwnd = (HWND)window;
    uint8_t *kept = NULL;

    if (count < 0 || (count > 0 && list == NULL) || !hero_ui_draw_whole(list, (size_t)count)) {
        return hero_ui_refused(HERO_UI_INVALID, "the drawing's bytes are no display list this arm reads");
    }
    if (count > 0) {
        kept = hero_layer_alloc((size_t)count);
        if (kept == NULL) {
            return HERO_UI_LIMIT;
        }
        memcpy(kept, list, (size_t)count);
    }
    hero_layer_free(canvas->list);
    canvas->list = kept;
    canvas->count = (size_t)count;
    (void)InvalidateRect(hwnd, NULL, FALSE);
    return HERO_UI_OK;
}

/* -- The replay ------------------------------------------------------------- */

/* A state `SAVE` keeps: the transform and the clip. */
typedef struct hero_ui_canvas_kept {
    hero_ui_draw_matrix transform;
    hero_d2_geometry *clip;
} hero_ui_canvas_kept;

/* One replay's state: the target, the transform below the drawing's (the
 * part of the window painted), the current transform and clip, the one layer
 * pushed, and the text format last made, for the next text in the same
 * font. */
typedef struct hero_ui_canvas_replay {
    hero_d2_target *target;
    hero_ui_draw_matrix base;
    hero_ui_draw_matrix transform;
    float tolerance;
    hero_d2_geometry *clip;
    hero_d2_unknown *layer;
    bool pushed;
    bool broken;
    hero_ui_canvas_kept kept[HERO_UI_DRAW_KEPT_MOST];
    size_t kept_count;
    hero_dw_format *format;
    hero_ui_draw_op format_op;
} hero_ui_canvas_replay;

static hero_d2_matrix hero_ui_canvas_matrix(hero_ui_draw_matrix m) {
    hero_d2_matrix made;

    hero_ui_draw_floats(m, made.m);
    return made;
}

/* The target's transform set to the drawing's at this point. */
static void hero_ui_canvas_place(hero_ui_canvas_replay *replay) {
    hero_d2_matrix m = hero_ui_canvas_matrix(hero_ui_draw_then(replay->transform, replay->base));

    replay->target->lpVtbl->SetTransform(replay->target, &m);
}

/* The path as a Direct2D geometry filled by `rule` (0 non-zero, 1 even-odd),
 * in the space its points were given in; NULL for a path of no step, which a
 * clip passes over as Core Graphics' does, and where Direct2D refused. A
 * figure of no segment is left out, since it marks nothing in Core Graphics. */
static hero_d2_geometry *hero_ui_canvas_geometry(const hero_ui_draw_path *path, uint8_t rule) {
    hero_d2_path *made = NULL;
    hero_d2_sink *sink = NULL;
    hero_d2_point start = {0.0f, 0.0f};
    bool open = false;

    if (path->failed || path->count == 0 || FAILED(hero_ui_canvas_factory->lpVtbl->CreatePathGeometry(hero_ui_canvas_factory, &made))) {
        return NULL;
    }
    if (FAILED(made->lpVtbl->Open(made, &sink))) {
        hero_ui_canvas_release(made);
        return NULL;
    }
    sink->lpVtbl->SetFillMode(sink, rule == 1 ? HERO_D2_FILL_ALTERNATE : HERO_D2_FILL_WINDING);
    for (size_t i = 0; i < path->count; i += 1) {
        const hero_ui_draw_step *step = &path->steps[i];

        if (step->code == HERO_UI_DRAW_MOVE) {
            if (open) {
                sink->lpVtbl->EndFigure(sink, HERO_D2_FIGURE_OPEN);
                open = false;
            }
            start.x = step->p[0];
            start.y = step->p[1];
            continue;
        }
        if (step->code == HERO_UI_DRAW_CLOSE) {
            if (open) {
                sink->lpVtbl->EndFigure(sink, HERO_D2_FIGURE_CLOSED);
                open = false;
            }
            continue;
        }
        if (!open) {
            sink->lpVtbl->BeginFigure(sink, start, HERO_D2_FIGURE_FILLED);
            open = true;
        }
        if (step->code == HERO_UI_DRAW_LINE) {
            hero_d2_point to = {step->p[0], step->p[1]};

            sink->lpVtbl->AddLine(sink, to);
        } else if (step->code == HERO_UI_DRAW_QUAD) {
            hero_d2_quadratic curve = {{step->p[0], step->p[1]}, {step->p[2], step->p[3]}};

            sink->lpVtbl->AddQuadraticBezier(sink, &curve);
        } else {
            hero_d2_bezier curve = {{step->p[0], step->p[1]}, {step->p[2], step->p[3]}, {step->p[4], step->p[5]}};

            sink->lpVtbl->AddBezier(sink, &curve);
        }
    }
    if (open) {
        sink->lpVtbl->EndFigure(sink, HERO_D2_FIGURE_OPEN);
    }
    if (FAILED(sink->lpVtbl->Close(sink))) {
        hero_ui_canvas_release(sink);
        hero_ui_canvas_release(made);
        return NULL;
    }
    hero_ui_canvas_release(sink);
    return (hero_d2_geometry *)made;
}

/* The clip pushed as the replay's one layer, the last one popped first. */
static void hero_ui_canvas_clip_again(hero_ui_canvas_replay *replay) {
    hero_d2_layer_parameters parameters;
    hero_d2_matrix identity = hero_ui_canvas_matrix(hero_ui_draw_identity());

    if (replay->pushed) {
        replay->target->lpVtbl->PopLayer(replay->target);
        replay->pushed = false;
    }
    if (replay->clip == NULL) {
        return;
    }
    if (replay->layer == NULL && FAILED(replay->target->lpVtbl->CreateLayer(replay->target, NULL, &replay->layer))) {
        /* Drawing on past a clip that could not be set would mark outside
         * it: nothing more is drawn. */
        replay->layer = NULL;
        replay->broken = true;
        return;
    }
    memset(&parameters, 0, sizeof parameters);
    parameters.content_bounds.left = -FLT_MAX;
    parameters.content_bounds.top = -FLT_MAX;
    parameters.content_bounds.right = FLT_MAX;
    parameters.content_bounds.bottom = FLT_MAX;
    parameters.geometric_mask = replay->clip;
    parameters.mask_antialias_mode = HERO_D2_ANTIALIAS_PER_PRIMITIVE;
    parameters.mask_transform = identity;
    parameters.opacity = 1.0f;
    /* The clip is in the target's own space already. */
    replay->target->lpVtbl->SetTransform(replay->target, &identity);
    replay->target->lpVtbl->PushLayer(replay->target, &parameters, replay->layer);
    replay->pushed = true;
}

/* `CLIP`: the clip intersected with the path, by `rule`, in the target's
 * space at the transform of this moment. */
static void hero_ui_canvas_clip(hero_ui_canvas_replay *replay, const hero_ui_draw_path *path, uint8_t rule) {
    hero_d2_geometry *shape = hero_ui_canvas_geometry(path, rule);
    hero_d2_geometry *placed = NULL;
    hero_d2_matrix m = hero_ui_canvas_matrix(hero_ui_draw_then(replay->transform, replay->base));

    if (shape == NULL) {
        if (path->count > 0) {
            replay->broken = true;
        }
        return;
    }
    if (FAILED(hero_ui_canvas_factory->lpVtbl->CreateTransformedGeometry(hero_ui_canvas_factory, shape, &m, &placed))) {
        hero_ui_canvas_release(shape);
        replay->broken = true;
        return;
    }
    hero_ui_canvas_release(shape);
    if (replay->clip != NULL) {
        hero_d2_path *both = NULL;
        hero_d2_sink *sink = NULL;
        hero_d2_matrix identity = hero_ui_canvas_matrix(hero_ui_draw_identity());
        bool made = SUCCEEDED(hero_ui_canvas_factory->lpVtbl->CreatePathGeometry(hero_ui_canvas_factory, &both)) && SUCCEEDED(both->lpVtbl->Open(both, &sink));

        made = made && SUCCEEDED(replay->clip->lpVtbl->CombineWithGeometry(replay->clip, placed, HERO_D2_COMBINE_INTERSECT, &identity, replay->tolerance, sink));
        made = made && SUCCEEDED(sink->lpVtbl->Close(sink));
        hero_ui_canvas_release(sink);
        hero_ui_canvas_release(placed);
        if (!made) {
            hero_ui_canvas_release(both);
            replay->broken = true;
            return;
        }
        placed = (hero_d2_geometry *)both;
    }
    /* A state kept holds its own reference to the clip it kept. */
    hero_ui_canvas_release(replay->clip);
    replay->clip = placed;
    hero_ui_canvas_clip_again(replay);
}

/* A colour of the list, straight 8-bit RGBA, as Direct2D's. */
static hero_d2_color hero_ui_canvas_color(const uint8_t rgba[4]) {
    hero_d2_color color = {(FLOAT)rgba[0] / 255.0f, (FLOAT)rgba[1] / 255.0f, (FLOAT)rgba[2] / 255.0f, (FLOAT)rgba[3] / 255.0f};

    return color;
}

/* `FILL` or `STROKE` of the path. */
static void hero_ui_canvas_mark(hero_ui_canvas_replay *replay, const hero_ui_draw_path *path, const hero_ui_draw_op *op) {
    hero_d2_geometry *shape = hero_ui_canvas_geometry(path, op->code == HERO_UI_DRAW_FILL ? op->rule : 0);
    hero_d2_color color = hero_ui_canvas_color(op->rgba);
    hero_d2_unknown *brush = NULL;

    if (shape == NULL) {
        return;
    }
    if (SUCCEEDED(replay->target->lpVtbl->CreateSolidColorBrush(replay->target, &color, NULL, &brush))) {
        hero_ui_canvas_place(replay);
        if (op->code == HERO_UI_DRAW_FILL) {
            replay->target->lpVtbl->FillGeometry(replay->target, shape, brush, NULL);
        } else {
            /* The list's ends are butt, round, square, its joins miter,
             * round, bevel. */
            static const UINT32 ends[3] = {HERO_D2_CAP_FLAT, HERO_D2_CAP_ROUND, HERO_D2_CAP_SQUARE};
            static const UINT32 joins[3] = {HERO_D2_JOIN_MITER_OR_BEVEL, HERO_D2_JOIN_ROUND, HERO_D2_JOIN_BEVEL};
            hero_d2_unknown **style = &hero_ui_canvas_strokes[op->end][op->join];

            if (*style == NULL) {
                hero_d2_stroke_properties properties = {ends[op->end], ends[op->end], HERO_D2_CAP_FLAT, joins[op->join], HERO_D2_MITER_LIMIT, 0, 0.0f};

                if (FAILED(hero_ui_canvas_factory->lpVtbl->CreateStrokeStyle(hero_ui_canvas_factory, &properties, NULL, 0, style))) {
                    *style = NULL;
                }
            }
            if (*style != NULL) {
                replay->target->lpVtbl->DrawGeometry(replay->target, shape, brush, op->real[0], *style);
            }
        }
    }
    hero_ui_canvas_release(brush);
    hero_ui_canvas_release(shape);
}

/* The text format of `TEXT` operation `op`: its family where the system has
 * it, else the system's, its size, bold and italic, on one line. The last
 * one made serves the next text in the same font. */
static hero_dw_format *hero_ui_canvas_format(hero_ui_canvas_replay *replay, const hero_ui_draw_op *op) {
    const hero_ui_draw_op *last = &replay->format_op;
    WCHAR *family = NULL;
    const WCHAR *name = hero_ui_canvas_system_family;
    hero_dw_format *made = NULL;

    if (replay->format != NULL && last->real[2] == op->real[2] && last->style == op->style && last->family_count == op->family_count && (op->family_count == 0 || memcmp(last->family, op->family, op->family_count) == 0)) {
        return replay->format;
    }
    hero_ui_canvas_release(replay->format);
    replay->format = NULL;
    if (op->family_count > 0) {
        family = hero_layer_alloc(((size_t)op->family_count + 1) * sizeof *family);
        if (family != NULL) {
            UINT32 index = 0;
            BOOL exists = FALSE;
            size_t units = hero_ui_draw_utf16(op->family, op->family_count, (uint16_t *)family, op->family_count);

            family[units] = 0;
            if (SUCCEEDED(hero_ui_canvas_fonts->lpVtbl->FindFamilyName(hero_ui_canvas_fonts, family, &index, &exists)) && exists) {
                name = family;
            }
        }
    }
    if (SUCCEEDED(hero_ui_canvas_writer->lpVtbl->CreateTextFormat(hero_ui_canvas_writer, name, NULL, (op->style & 1) != 0 ? HERO_DW_WEIGHT_BOLD : HERO_DW_WEIGHT_NORMAL, (op->style & 2) != 0 ? HERO_DW_STYLE_ITALIC : HERO_DW_STYLE_NORMAL, HERO_DW_STRETCH_NORMAL, op->real[2], hero_ui_canvas_locale, &made))) {
        (void)made->lpVtbl->SetWordWrapping(made, HERO_DW_NO_WRAP);
        replay->format = made;
        replay->format_op = *op;
    }
    hero_layer_free(family);
    return replay->format;
}

/* `TEXT`: one line, the top-left corner of its box at x, y. */
static void hero_ui_canvas_text(hero_ui_canvas_replay *replay, const hero_ui_draw_op *op) {
    hero_dw_format *format = hero_ui_canvas_format(replay, op);
    hero_d2_color color = hero_ui_canvas_color(op->rgba);
    hero_d2_unknown *brush = NULL;
    WCHAR *text;
    size_t units;
    /* Room for the line, never reached: it does not wrap. */
    hero_d2_rect box = {op->real[0], op->real[1], op->real[0] + 1048576.0f, op->real[1] + 1048576.0f};

    if (format == NULL || op->text_count == 0) {
        return;
    }
    text = hero_layer_alloc((size_t)op->text_count * sizeof *text);
    if (text == NULL) {
        return;
    }
    units = hero_ui_draw_utf16(op->text, op->text_count, (uint16_t *)text, op->text_count);
    if (SUCCEEDED(replay->target->lpVtbl->CreateSolidColorBrush(replay->target, &color, NULL, &brush))) {
        hero_ui_canvas_place(replay);
        replay->target->lpVtbl->DrawText(replay->target, text, (UINT32)units, format, &box, brush, 0, 0);
    }
    hero_ui_canvas_release(brush);
    hero_layer_free(text);
}

/* `IMAGE`: the picture filling its box, smoothly scaled. */
static void hero_ui_canvas_image(hero_ui_canvas_replay *replay, const hero_ui_draw_op *op) {
    size_t pixels = (size_t)op->columns * op->rows;
    uint8_t *bgra = hero_layer_alloc(pixels * 4);
    hero_d2_unknown *bitmap = NULL;
    hero_d2_size_u size = {op->columns, op->rows};
    hero_d2_bitmap_properties properties = {{HERO_D2_FORMAT_B8G8R8A8, HERO_D2_ALPHA_PREMULTIPLIED}, 96.0f, 96.0f};
    hero_d2_rect box = {op->real[0], op->real[1], op->real[0] + op->real[2], op->real[1] + op->real[3]};

    if (bgra == NULL) {
        return;
    }
    hero_ui_draw_premultiplied(op->pixels, bgra, pixels);
    if (SUCCEEDED(replay->target->lpVtbl->CreateBitmap(replay->target, size, bgra, op->columns * 4, &properties, &bitmap))) {
        hero_ui_canvas_place(replay);
        replay->target->lpVtbl->DrawBitmap(replay->target, bitmap, &box, 1.0f, HERO_D2_INTERPOLATION_LINEAR, NULL);
    }
    hero_ui_canvas_release(bitmap);
    hero_layer_free(bgra);
}

/* `SAVE` and `RESTORE`. */
static void hero_ui_canvas_keep(hero_ui_canvas_replay *replay, bool keep) {
    if (keep) {
        hero_ui_canvas_kept *kept;

        if (replay->kept_count == HERO_UI_DRAW_KEPT_MOST) {
            return;
        }
        kept = &replay->kept[replay->kept_count++];
        kept->transform = replay->transform;
        kept->clip = replay->clip;
        if (kept->clip != NULL) {
            kept->clip->lpVtbl->AddRef(kept->clip);
        }
        return;
    }
    if (replay->kept_count == 0) {
        return;
    }
    replay->kept_count -= 1;
    replay->transform = replay->kept[replay->kept_count].transform;
    if (replay->clip != replay->kept[replay->kept_count].clip) {
        hero_ui_canvas_release(replay->clip);
        replay->clip = replay->kept[replay->kept_count].clip;
        hero_ui_canvas_clip_again(replay);
    } else {
        hero_ui_canvas_release(replay->kept[replay->kept_count].clip);
    }
}

/* Replays `canvas`'s drawing into `target` between its `BeginDraw` and
 * `EndDraw`, `base` placing the area's space in the target's. */
static void hero_ui_canvas_replay_into(hero_ui_canvas_win32 *canvas, hero_d2_target *target, hero_ui_draw_matrix base, float tolerance) {
    hero_ui_canvas_replay *replay = hero_layer_alloc_zero(1, sizeof *replay);
    hero_d2_color clear = {0.0f, 0.0f, 0.0f, 0.0f};
    size_t at = 0;
    hero_ui_draw_op op;

    target->lpVtbl->SetAntialiasMode(target, HERO_D2_ANTIALIAS_PER_PRIMITIVE);
    target->lpVtbl->SetTextAntialiasMode(target, HERO_D2_TEXT_ANTIALIAS_GRAYSCALE);
    target->lpVtbl->Clear(target, &clear);
    if (replay == NULL) {
        return;
    }
    replay->target = target;
    replay->base = base;
    replay->transform = hero_ui_draw_identity();
    replay->tolerance = tolerance;
    hero_ui_draw_path_begin(&canvas->path);
    while (at < canvas->count && !replay->broken) {
        size_t next = hero_ui_draw_read(canvas->list, canvas->count, at, &op);

        if (next == 0) {
            break;
        }
        at = next;
        if (hero_ui_draw_path_take(&canvas->path, &op)) {
            continue;
        }
        switch (op.code) {
        case HERO_UI_DRAW_SAVE:
        case HERO_UI_DRAW_RESTORE:
            hero_ui_canvas_keep(replay, op.code == HERO_UI_DRAW_SAVE);
            break;
        case HERO_UI_DRAW_TRANSLATE:
        case HERO_UI_DRAW_SCALE:
        case HERO_UI_DRAW_ROTATE:
            replay->transform = hero_ui_draw_moved(replay->transform, &op);
            break;
        case HERO_UI_DRAW_FILL:
        case HERO_UI_DRAW_STROKE:
            hero_ui_canvas_mark(replay, &canvas->path, &op);
            break;
        case HERO_UI_DRAW_CLIP:
            hero_ui_canvas_clip(replay, &canvas->path, op.rule);
            break;
        case HERO_UI_DRAW_TEXT:
            hero_ui_canvas_text(replay, &op);
            break;
        case HERO_UI_DRAW_IMAGE:
            hero_ui_canvas_image(replay, &op);
            break;
        default:
            break;
        }
    }
    if (replay->pushed) {
        target->lpVtbl->PopLayer(target);
    }
    while (replay->kept_count > 0) {
        replay->kept_count -= 1;
        hero_ui_canvas_release(replay->kept[replay->kept_count].clip);
    }
    hero_ui_canvas_release(replay->clip);
    hero_ui_canvas_release(replay->layer);
    hero_ui_canvas_release(replay->format);
    /* The path's room is kept for the next replay, its steps not. */
    hero_ui_draw_path_begin(&canvas->path);
    hero_layer_free(replay);
}

/* -- The bitmap ---------------------------------------------------------------- */

/* A 32-bit top-down DIB section of `width` by `height` pixels selected into
 * a memory DC of its own, and its bits. */
typedef struct hero_ui_canvas_bitmap {
    HDC dc;
    HBITMAP bitmap;
    HGDIOBJ before;
    uint8_t *bits;
} hero_ui_canvas_bitmap;

static void hero_ui_canvas_bitmap_free(hero_ui_canvas_bitmap *made) {
    if (made->dc != NULL) {
        (void)SelectObject(made->dc, made->before);
        (void)DeleteDC(made->dc);
    }
    if (made->bitmap != NULL) {
        (void)DeleteObject(made->bitmap);
    }
    memset(made, 0, sizeof *made);
}

/* The drawing replayed into a new bitmap of `width` by `height` pixels at
 * `dpi`, the area's point `left`, `top` (in pixels) at its corner:
 * premultiplied BGRA, clear where nothing is drawn. `HERO_UI_OK`, or
 * `HERO_UI_OTHER` with words where Windows or Direct2D refused, nothing
 * made. */
static int64_t hero_ui_canvas_render(hero_ui_canvas_win32 *canvas, int width, int height, UINT dpi, int left, int top, hero_ui_canvas_bitmap *made) {
    BITMAPINFO info;
    RECT whole = {0, 0, width, height};
    hero_ui_draw_matrix base = hero_ui_draw_identity();
    HRESULT drawn;

    memset(made, 0, sizeof *made);
    memset(&info, 0, sizeof info);
    info.bmiHeader.biSize = (DWORD)sizeof info.bmiHeader;
    info.bmiHeader.biWidth = width;
    info.bmiHeader.biHeight = -height;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;
    made->bitmap = CreateDIBSection(NULL, &info, DIB_RGB_COLORS, (void **)&made->bits, NULL, 0);
    made->dc = made->bitmap != NULL ? CreateCompatibleDC(NULL) : NULL;
    if (made->dc == NULL || made->bits == NULL) {
        hero_ui_canvas_bitmap_free(made);
        return hero_ui_refused(HERO_UI_OTHER, "Windows made no bitmap the size of the drawing area to draw it in");
    }
    made->before = SelectObject(made->dc, made->bitmap);
    if (canvas->target == NULL) {
        hero_d2_target_properties properties = {HERO_D2_TARGET_SOFTWARE, {HERO_D2_FORMAT_B8G8R8A8, HERO_D2_ALPHA_PREMULTIPLIED}, 0.0f, 0.0f, 0, 0};

        if (FAILED(hero_ui_canvas_factory->lpVtbl->CreateDCRenderTarget(hero_ui_canvas_factory, &properties, &canvas->target))) {
            canvas->target = NULL;
            hero_ui_canvas_bitmap_free(made);
            return hero_ui_refused(HERO_UI_OTHER, "Direct2D made no target to draw the drawing area into (CreateDCRenderTarget failed)");
        }
    }
    canvas->target->lpVtbl->SetDpi(canvas->target, (FLOAT)dpi, (FLOAT)dpi);
    if (FAILED(canvas->target->lpVtbl->BindDC(canvas->target, made->dc, &whole))) {
        hero_ui_canvas_bitmap_free(made);
        return hero_ui_refused(HERO_UI_OTHER, "Direct2D could not draw into the drawing area's bitmap (BindDC failed)");
    }
    /* The points of the area, in the target's units (a point each, 1/96
     * inch), moved so the area's pixel `left`, `top` is the bitmap's corner. */
    base.e = -(double)left * 96.0 / (double)dpi;
    base.f = -(double)top * 96.0 / (double)dpi;
    canvas->target->lpVtbl->BeginDraw(canvas->target);
    /* A curve flattened for a clip strays at most a quarter of a pixel. */
    hero_ui_canvas_replay_into(canvas, canvas->target, base, (float)(0.25 * 96.0 / (double)dpi));
    drawn = canvas->target->lpVtbl->EndDraw(canvas->target, NULL, NULL);
    if (FAILED(drawn)) {
        /* A target to make again, which a software one is not told; one that
         * refused for any reason is made again at the next draw. */
        hero_ui_canvas_release(canvas->target);
        canvas->target = NULL;
        hero_ui_canvas_bitmap_free(made);
        return hero_ui_refused(HERO_UI_OTHER, drawn == HERO_D2_RECREATE_TARGET ? "Direct2D lost the drawing area's target while drawing it" : "Direct2D refused to draw the drawing area (EndDraw failed)");
    }
    (void)GdiFlush();
    return HERO_UI_OK;
}

/* `WM_PAINT`: the part to paint filled with the window's colour, the drawing
 * replayed over it at the window's scale and blended by its alpha, as a
 * picture's pixels are. */
static void hero_ui_canvas_paint(hero_ui_canvas_win32 *canvas, HWND hwnd, UINT dpi) {
    PAINTSTRUCT paint;
    HDC dc = BeginPaint(hwnd, &paint);
    RECT area = paint.rcPaint;
    int width = area.right - area.left;
    int height = area.bottom - area.top;
    hero_ui_canvas_bitmap made;

    if (dc == NULL) {
        return;
    }
    (void)FillRect(dc, &area, GetSysColorBrush(COLOR_3DFACE));
    if (canvas->count > 0 && width > 0 && height > 0 && hero_ui_canvas_render(canvas, width, height, dpi == 0 ? 96 : dpi, area.left, area.top, &made) == HERO_UI_OK) {
        BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

        (void)AlphaBlend(dc, area.left, area.top, width, height, made.dc, 0, 0, width, height, blend);
        hero_ui_canvas_bitmap_free(&made);
    }
    (void)EndPaint(hwnd, &paint);
}

int64_t hero_ui_canvas_win32_snapshot(hero_ui_canvas_win32 *canvas, int64_t width, int64_t height, uint8_t *rgba, int64_t count) {
    hero_ui_canvas_bitmap made;
    int64_t answer;

    if (width > 8192 || height > 8192) {
        return hero_ui_refused(HERO_UI_OTHER, "a drawing area is read back up to 8192 points a side, and this one is wider or taller");
    }
    if (width <= 0 || height <= 0 || rgba == NULL || count < width * height * 4) {
        return HERO_UI_OK;
    }
    answer = hero_ui_canvas_render(canvas, (int)width, (int)height, 96, 0, 0, &made);
    if (answer != HERO_UI_OK) {
        return answer;
    }
    hero_ui_draw_straight(made.bits, rgba, (size_t)(width * height));
    hero_ui_canvas_bitmap_free(&made);
    return HERO_UI_OK;
}

/* -- The pointer --------------------------------------------------------------- */

/* The keys held, as `hero_ui.h` adds them up, from a mouse message's `MK_*`
 * and Alt's own state, which no mouse message carries. */
static uint8_t hero_ui_canvas_mouse_keys(WPARAM wparam) {
    return (uint8_t)(((wparam & MK_CONTROL) != 0 ? HERO_UI_PRIMARY : 0) | ((wparam & MK_SHIFT) != 0 ? HERO_UI_SHIFT : 0) | (GetKeyState(VK_MENU) < 0 ? HERO_UI_ALT : 0));
}

static void hero_ui_canvas_tell(hero_ui_canvas_win32 *canvas, int64_t id, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, uint8_t keys) {
    uint8_t bytes[4] = {(uint8_t)phase, (uint8_t)button, (uint8_t)clicks, keys};

    canvas->last_x = x;
    canvas->last_y = y;
    hero_ui_tell_text(HERO_UI_EVENT_POINTER, id, x, y, bytes, 4);
}

/* Whether message `message` is the act `inject` is sending, which is then
 * told as the act says. */
static bool hero_ui_canvas_injected(hero_ui_canvas_win32 *canvas, UINT message) {
    if (canvas->injecting && !canvas->injected && canvas->injected_message == message) {
        canvas->injected = true;
        return true;
    }
    return false;
}

/* The area no longer holds button `button`; the mouse given back with the
 * last. */
static void hero_ui_canvas_let_go(hero_ui_canvas_win32 *canvas, HWND hwnd, int64_t button) {
    canvas->held &= ~(1u << button);
    if (canvas->held == 0) {
        canvas->dragging = 0;
        if (GetCapture() == hwnd) {
            (void)ReleaseCapture();
        }
    }
}

/* A press or a release of `button` (`HERO_UI_POINTER_LEFT`, `_RIGHT`,
 * `_OTHER`). */
static void hero_ui_canvas_button(hero_ui_canvas_win32 *canvas, int64_t id, HWND hwnd, UINT dpi, UINT message, WPARAM wparam, LPARAM lparam, int64_t button, bool pressed) {
    int64_t px = (int64_t)(short)LOWORD(lparam);
    int64_t py = (int64_t)(short)HIWORD(lparam);
    int64_t x = hero_ui_draw_sixty_fourths(px, dpi);
    int64_t y = hero_ui_draw_sixty_fourths(py, dpi);
    int64_t clicks;
    uint8_t keys = hero_ui_canvas_mouse_keys(wparam);

    if (hero_ui_canvas_injected(canvas, message)) {
        x = canvas->injected_x;
        y = canvas->injected_y;
        clicks = canvas->injected_bytes[2];
        keys = canvas->injected_bytes[3];
    } else if (pressed) {
        clicks = hero_ui_draw_click(&canvas->clicks, button, (uint32_t)GetMessageTime(), px, py, GetDoubleClickTime(), GetSystemMetrics(SM_CXDOUBLECLK), GetSystemMetrics(SM_CYDOUBLECLK));
    } else {
        clicks = canvas->clicks_of[button] > 0 ? canvas->clicks_of[button] : 1;
    }
    if (pressed) {
        if (canvas->held == 0) {
            canvas->dragging = button;
            (void)SetCapture(hwnd);
        }
        canvas->held |= 1u << button;
        canvas->clicks_of[button] = clicks;
        hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_DOWN, button, clicks, keys);
        return;
    }
    if ((canvas->held & (1u << button)) == 0 && !canvas->injecting) {
        /* The release of a press another window took. */
        return;
    }
    hero_ui_canvas_let_go(canvas, hwnd, button);
    hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_UP, button, clicks, keys);
}

/* `WM_MOUSEMOVE`: an entry where the pointer was outside, an exit where a
 * held button carries it out, and a move or a drag. */
static void hero_ui_canvas_moved(hero_ui_canvas_win32 *canvas, int64_t id, HWND hwnd, UINT dpi, WPARAM wparam, LPARAM lparam) {
    int64_t px = (int64_t)(short)LOWORD(lparam);
    int64_t py = (int64_t)(short)HIWORD(lparam);
    int64_t x = hero_ui_draw_sixty_fourths(px, dpi);
    int64_t y = hero_ui_draw_sixty_fourths(py, dpi);
    uint8_t keys = hero_ui_canvas_mouse_keys(wparam);
    RECT client;
    bool within;

    if (hero_ui_canvas_injected(canvas, WM_MOUSEMOVE)) {
        hero_ui_canvas_tell(canvas, id, canvas->injected_x, canvas->injected_y, HERO_UI_POINTER_DRAG, canvas->injected_bytes[1], 0, canvas->injected_bytes[3]);
        return;
    }
    within = GetClientRect(hwnd, &client) && px >= client.left && px < client.right && py >= client.top && py < client.bottom;
    if (within && !canvas->inside) {
        TRACKMOUSEEVENT track;

        memset(&track, 0, sizeof track);
        track.cbSize = (DWORD)sizeof track;
        track.dwFlags = TME_LEAVE;
        track.hwndTrack = hwnd;
        (void)TrackMouseEvent(&track);
        canvas->inside = true;
        hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_ENTER, 0, 0, keys);
        if (canvas->held == 0) {
            return;
        }
    } else if (!within && canvas->inside && canvas->held != 0) {
        canvas->inside = false;
        hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_EXIT, 0, 0, keys);
    }
    if (canvas->held != 0) {
        hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_DRAG, canvas->dragging, 0, keys);
    } else if (canvas->inside) {
        hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_MOVE, 0, 0, keys);
    }
}

/* `WM_MOUSELEAVE`: the exit, where the pointer is now. */
static void hero_ui_canvas_left(hero_ui_canvas_win32 *canvas, int64_t id, HWND hwnd, UINT dpi) {
    POINT at;
    int64_t x = canvas->last_x;
    int64_t y = canvas->last_y;

    if (!canvas->inside) {
        return;
    }
    canvas->inside = false;
    if (GetCursorPos(&at) && ScreenToClient(hwnd, &at)) {
        x = hero_ui_draw_sixty_fourths(at.x, dpi);
        y = hero_ui_draw_sixty_fourths(at.y, dpi);
    }
    hero_ui_canvas_tell(canvas, id, x, y, HERO_UI_POINTER_EXIT, 0, 0, hero_ui_canvas_mouse_keys((GetKeyState(VK_CONTROL) < 0 ? MK_CONTROL : 0) | (GetKeyState(VK_SHIFT) < 0 ? MK_SHIFT : 0)));
}

/* `WM_CAPTURECHANGED` while a button is held: another window took the
 * mouse, so every press the area holds is told released where the pointer
 * was last. */
static void hero_ui_canvas_lost(hero_ui_canvas_win32 *canvas, int64_t id) {
    unsigned held = canvas->held;

    canvas->held = 0;
    canvas->dragging = 0;
    for (int64_t button = HERO_UI_POINTER_LEFT; button <= HERO_UI_POINTER_OTHER; button += 1) {
        if ((held & (1u << button)) != 0) {
            hero_ui_canvas_tell(canvas, id, canvas->last_x, canvas->last_y, HERO_UI_POINTER_UP, button, canvas->clicks_of[button] > 0 ? canvas->clicks_of[button] : 1, 0);
        }
    }
}

int64_t hero_ui_canvas_win32_pointer(hero_ui_canvas_win32 *canvas, int64_t id, void *window, uint32_t dpi, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, int64_t modifiers) {
    HWND hwnd = (HWND)window;
    static const UINT downs[4] = {0, WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_MBUTTONDOWN};
    static const UINT ups[4] = {0, WM_LBUTTONUP, WM_RBUTTONUP, WM_MBUTTONUP};
    static const WPARAM masks[4] = {0, MK_LBUTTON, MK_RBUTTON, MK_MBUTTON};
    int64_t px = hero_ui_draw_pixels(x, dpi);
    int64_t py = hero_ui_draw_pixels(y, dpi);
    HWND target = hwnd;
    UINT message;
    WPARAM wparam = ((modifiers & HERO_UI_PRIMARY) != 0 ? MK_CONTROL : 0) | ((modifiers & HERO_UI_SHIFT) != 0 ? MK_SHIFT : 0);

    if (!IsWindowVisible(hwnd)) {
        return hero_ui_refused(HERO_UI_INVALID, "the drawing area's window is not shown, and Windows hands the pointer to a window it shows");
    }
    if (button < 0 || button > HERO_UI_POINTER_OTHER) {
        return HERO_UI_INVALID;
    }
    if (phase == HERO_UI_POINTER_MOVE || phase == HERO_UI_POINTER_ENTER || phase == HERO_UI_POINTER_EXIT) {
        /* What Windows tells of these follows the real pointer alone
         * (`TrackMouseEvent`), so they are told as they come. */
        if (phase != HERO_UI_POINTER_MOVE) {
            canvas->inside = phase == HERO_UI_POINTER_ENTER;
        }
        hero_ui_canvas_tell(canvas, id, x, y, phase, 0, 0, (uint8_t)modifiers);
        return HERO_UI_OK;
    }
    px = px < -32768 ? -32768 : px > 32767 ? 32767 : px;
    py = py < -32768 ? -32768 : py > 32767 ? 32767 : py;
    if (!((phase == HERO_UI_POINTER_DRAG || phase == HERO_UI_POINTER_UP) && GetCapture() == hwnd)) {
        /* The window's own hit test, as Windows finds the window a press at
         * that point goes to, its other top windows aside. */
        POINT screen = {(LONG)px, (LONG)py};
        HWND found = GetAncestor(hwnd, GA_ROOT);

        (void)ClientToScreen(hwnd, &screen);
        while (found != NULL) {
            POINT local = screen;
            HWND child;

            (void)ScreenToClient(found, &local);
            child = ChildWindowFromPointEx(found, local, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED | CWP_SKIPTRANSPARENT);
            if (child == NULL || child == found) {
                break;
            }
            found = child;
        }
        if (found != hwnd) {
            return hero_ui_refused(HERO_UI_OTHER, "the window's hit test gives that point to another window than the drawing area, so a person's pointer there would not reach it");
        }
    }
    if (phase == HERO_UI_POINTER_DOWN) {
        message = downs[button];
        wparam |= masks[button];
    } else if (phase == HERO_UI_POINTER_UP) {
        message = ups[button];
    } else {
        message = WM_MOUSEMOVE;
        wparam |= masks[button];
    }
    canvas->injecting = true;
    canvas->injected = false;
    canvas->injected_message = message;
    canvas->injected_x = x;
    canvas->injected_y = y;
    canvas->injected_bytes[0] = (uint8_t)phase;
    canvas->injected_bytes[1] = (uint8_t)button;
    canvas->injected_bytes[2] = (uint8_t)clicks;
    canvas->injected_bytes[3] = (uint8_t)modifiers;
    (void)SendMessageW(target, message, wparam, MAKELPARAM((WORD)(short)px, (WORD)(short)py));
    canvas->injecting = false;
    if (!canvas->injected) {
        return hero_ui_refused(HERO_UI_OTHER, "the drawing area's window did not take the pointer's message");
    }
    return HERO_UI_OK;
}

/* -- The keys ------------------------------------------------------------------ */

static uint8_t hero_ui_canvas_keys_held(void) {
    return (uint8_t)((GetKeyState(VK_CONTROL) < 0 ? HERO_UI_PRIMARY : 0) | (GetKeyState(VK_SHIFT) < 0 ? HERO_UI_SHIFT : 0) | (GetKeyState(VK_MENU) < 0 ? HERO_UI_ALT : 0));
}

/* A key pressed or let go in the area, told as `HERO_UI_EVENT_KEY`. */
static void hero_ui_canvas_key(hero_ui_canvas_win32 *canvas, int64_t id, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    uint32_t key = (uint32_t)(wparam & 0xFF);
    bool down = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
    uint8_t held = hero_ui_canvas_keys_held();
    hero_ui_canvas_typed *kept = &canvas->keys[key];
    char one[2];
    const char *name = hero_ui_draw_key_name(key, one);
    size_t named = name != NULL ? strlen(name) : 0;
    uint8_t text[16 + 1 + sizeof kept->typed];
    int64_t phase;

    if (hero_ui_draw_key_held_with(key)) {
        return;
    }
    if (down) {
        WCHAR units[16];
        size_t count = 0;
        MSG typed;

        phase = (lparam & (1L << 30)) != 0 ? 1 : 0;
        /* The characters `TranslateMessage` posted for this press, which stand
         * first in the queue for this window. */
        while (message == WM_KEYDOWN && PeekMessageW(&typed, hwnd, WM_CHAR, WM_CHAR, PM_REMOVE)) {
            if (count < sizeof units / sizeof units[0]) {
                units[count++] = (WCHAR)typed.wParam;
            }
        }
        if ((held & HERO_UI_PRIMARY) != 0 && (held & HERO_UI_ALT) == 0) {
            count = 0;
        }
        kept->told = true;
        kept->count = (uint8_t)hero_ui_draw_utf8((const uint16_t *)units, count, kept->typed, sizeof kept->typed);
    } else {
        if (!kept->told) {
            return;
        }
        kept->told = false;
        phase = 2;
    }
    memcpy(text, name != NULL ? name : "", named);
    text[named] = 0;
    memcpy(text + named + 1, kept->typed, kept->count);
    hero_ui_tell_text(HERO_UI_EVENT_KEY, id, held + 256 * phase, 0, text, (int64_t)(named + 1 + kept->count));
    if (!down) {
        kept->count = 0;
    }
}

bool hero_ui_canvas_win32_message(hero_ui_canvas_win32 *canvas, int64_t id, void *window, uint32_t dpi, uint32_t message, uintptr_t wparam, intptr_t lparam, intptr_t *result) {
    HWND hwnd = (HWND)window;

    *result = 0;
    dpi = dpi == 0 ? 96 : dpi;
    switch (message) {
    case WM_ERASEBKGND:
        /* The paint fills what it paints. */
        *result = 1;
        return true;
    case WM_PAINT:
        hero_ui_canvas_paint(canvas, hwnd, dpi);
        return true;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
        hero_ui_canvas_button(canvas, id, hwnd, dpi, message, wparam, lparam, message <= WM_LBUTTONDBLCLK ? HERO_UI_POINTER_LEFT : message <= WM_RBUTTONDBLCLK ? HERO_UI_POINTER_RIGHT : HERO_UI_POINTER_OTHER, true);
        *result = message >= WM_XBUTTONDOWN ? TRUE : 0;
        return true;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
        hero_ui_canvas_button(canvas, id, hwnd, dpi, message, wparam, lparam, message == WM_LBUTTONUP ? HERO_UI_POINTER_LEFT : message == WM_RBUTTONUP ? HERO_UI_POINTER_RIGHT : HERO_UI_POINTER_OTHER, false);
        *result = message == WM_XBUTTONUP ? TRUE : 0;
        return true;
    case WM_MOUSEMOVE:
        hero_ui_canvas_moved(canvas, id, hwnd, dpi, wparam, lparam);
        return true;
    case WM_MOUSELEAVE:
        hero_ui_canvas_left(canvas, id, hwnd, dpi);
        return true;
    case WM_CAPTURECHANGED:
        if ((HWND)lparam != hwnd && canvas->held != 0) {
            hero_ui_canvas_lost(canvas, id);
        }
        return true;
    case WM_KEYDOWN:
    case WM_KEYUP:
        hero_ui_canvas_key(canvas, id, hwnd, message, wparam, lparam);
        return true;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        /* Told, and then Windows' own: Alt+F4, the menu's Alt and F10. */
        hero_ui_canvas_key(canvas, id, hwnd, message, wparam, lparam);
        return false;
    case WM_CHAR:
        /* Its press took the characters it typed; one with no press (an
         * input method's) is no key of the area. */
        return true;
    default:
        return false;
    }
}

#endif
