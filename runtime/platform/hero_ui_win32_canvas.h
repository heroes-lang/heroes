/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_win32_canvas.h: THE PURE HALF OF THE WIN32 ARM'S DRAWING AREA
 * (`hero_ui.h`'s A DRAWING AREA, WITH ITS MOUSE AND ITS KEYS), what needs no
 * window and no Direct2D: one operation of a display list read, every length
 * asked before a byte is taken; the current path kept as Core Graphics keeps
 * it; the transform's arithmetic; pixels turned between the list's straight
 * RGBA and the premultiplied BGRA Windows draws; a point between pixels and
 * 64ths of a point; clicks counted; a virtual key's name; UTF-8 and UTF-16
 * each into the other. `hero_ui_canvas_win32.c` keeps what needs the system.
 *
 * THE PATH IS CORE GRAPHICS' (the macOS arm replays the same list into a
 * `CGMutablePath`, and the two must mark the same pixels), measured with
 * `CGPathApply` on macOS 27: a rectangle is a closed subpath from its x, y
 * corner along the top edge first; an ellipse is a closed subpath of four
 * cubic curves from the middle of its right side, down first (both turn
 * clockwise on a screen whose y goes down, so a non-zero fill of the two
 * nested leaves no hole); a line after a close starts a new subpath where the
 * closed one started; a line on an empty path starts one, and a curve or a
 * close on an empty path is left out. Its points stay in the space they were
 * given in, so the transform at the fill, the stroke or the clip places
 * them.
 *
 * THE FUNCTIONS ARE `static`, for the file that includes this header, and
 * need `stdint.h`, `stdbool.h`, `stddef.h`, `string.h` and `math.h` already
 * included and the allocator `hero_layer_grow` and `hero_layer_free`
 * declared; a test of its logic on another system includes them first. */

#ifndef HERO_UI_WIN32_CANVAS_H
#define HERO_UI_WIN32_CANVAS_H

/* The operation codes of the display list. */
#define HERO_UI_DRAW_SAVE 0x01
#define HERO_UI_DRAW_RESTORE 0x02
#define HERO_UI_DRAW_TRANSLATE 0x03
#define HERO_UI_DRAW_SCALE 0x04
#define HERO_UI_DRAW_ROTATE 0x05
#define HERO_UI_DRAW_BEGIN 0x10
#define HERO_UI_DRAW_MOVE 0x11
#define HERO_UI_DRAW_LINE 0x12
#define HERO_UI_DRAW_QUAD 0x13
#define HERO_UI_DRAW_CUBIC 0x14
#define HERO_UI_DRAW_CLOSE 0x15
#define HERO_UI_DRAW_RECT 0x16
#define HERO_UI_DRAW_ELLIPSE 0x17
#define HERO_UI_DRAW_FILL 0x20
#define HERO_UI_DRAW_STROKE 0x21
#define HERO_UI_DRAW_CLIP 0x22
#define HERO_UI_DRAW_TEXT 0x30
#define HERO_UI_DRAW_IMAGE 0x31

/* The most states a list keeps at once (`hero_ui.c`'s check refuses a
 * 257th). */
#define HERO_UI_DRAW_KEPT_MOST 256

/* Core Graphics' constant for a quarter of an ellipse as one cubic curve,
 * 4 (sqrt 2 - 1) / 3: the control points of `CGPathAddEllipseInRect`
 * measured stand 0.552285 of a radius from the ends. */
#define HERO_UI_DRAW_KAPPA 0.55228474983079340

/* ONE OPERATION, its operands read: `real` its numbers in the list's order
 * (x, y, then the rest), `rule` a fill's or a clip's (0 non-zero, 1
 * even-odd), `end` and `join` a stroke's, `style` a text's, `rgba` a fill's,
 * a stroke's or a text's colour, and the text's, the family's and the
 * picture's bytes where they stand in the list. */
typedef struct hero_ui_draw_op {
    uint8_t code;
    float real[6];
    uint8_t rule;
    uint8_t end;
    uint8_t join;
    uint8_t style;
    uint8_t rgba[4];
    const uint8_t *family;
    uint32_t family_count;
    const uint8_t *text;
    uint32_t text_count;
    const uint8_t *pixels;
    uint32_t columns;
    uint32_t rows;
} hero_ui_draw_op;

static float hero_ui_draw_real(const uint8_t *at) {
    uint32_t bits = (uint32_t)at[0] | (uint32_t)at[1] << 8 | (uint32_t)at[2] << 16 | (uint32_t)at[3] << 24;
    float value;

    memcpy(&value, &bits, sizeof value);
    return value;
}

static uint32_t hero_ui_draw_u32(const uint8_t *at) {
    return (uint32_t)at[0] | (uint32_t)at[1] << 8 | (uint32_t)at[2] << 16 | (uint32_t)at[3] << 24;
}

/* Reads the operation at byte `at` of the `count` bytes of `list` into `op`
 * and answers the byte past it; 0 where the bytes there are no operation:
 * a code that is none, an operand past the end, a number that is not
 * finite, an enumeration past its range, a size that is not above 0 where
 * the list says it must be, a picture whose bytes are not its columns *
 * rows * 4. `hero_ui.c` checked the list whole before; this reads it as
 * though it had not. */
static size_t hero_ui_draw_read(const uint8_t *list, size_t count, size_t at, hero_ui_draw_op *op) {
    size_t fixed;
    size_t reals = 0;
    const uint8_t *p;

    if (list == NULL || at >= count) {
        return 0;
    }
    memset(op, 0, sizeof *op);
    op->code = list[at];
    switch (op->code) {
    case HERO_UI_DRAW_SAVE:
    case HERO_UI_DRAW_RESTORE:
    case HERO_UI_DRAW_BEGIN:
    case HERO_UI_DRAW_CLOSE:
        fixed = 0;
        break;
    case HERO_UI_DRAW_ROTATE:
        fixed = 4;
        reals = 1;
        break;
    case HERO_UI_DRAW_TRANSLATE:
    case HERO_UI_DRAW_SCALE:
    case HERO_UI_DRAW_MOVE:
    case HERO_UI_DRAW_LINE:
        fixed = 8;
        reals = 2;
        break;
    case HERO_UI_DRAW_QUAD:
    case HERO_UI_DRAW_RECT:
    case HERO_UI_DRAW_ELLIPSE:
        fixed = 16;
        reals = 4;
        break;
    case HERO_UI_DRAW_CUBIC:
        fixed = 24;
        reals = 6;
        break;
    case HERO_UI_DRAW_FILL:
        fixed = 5;
        break;
    case HERO_UI_DRAW_CLIP:
        fixed = 1;
        break;
    case HERO_UI_DRAW_STROKE:
        fixed = 10;
        reals = 1;
        break;
    case HERO_UI_DRAW_TEXT:
        fixed = 23;
        reals = 3;
        break;
    case HERO_UI_DRAW_IMAGE:
        fixed = 24;
        reals = 4;
        break;
    default:
        return 0;
    }
    if (count - at - 1 < fixed) {
        return 0;
    }
    p = list + at + 1;
    for (size_t i = 0; i < reals; i += 1) {
        op->real[i] = hero_ui_draw_real(p + 4 * i);
        if (!isfinite(op->real[i])) {
            return 0;
        }
    }
    switch (op->code) {
    case HERO_UI_DRAW_RECT:
    case HERO_UI_DRAW_ELLIPSE:
        if (op->real[2] < 0.0f || op->real[3] < 0.0f) {
            return 0;
        }
        break;
    case HERO_UI_DRAW_FILL:
    case HERO_UI_DRAW_CLIP:
        op->rule = p[0];
        if (op->rule > 1) {
            return 0;
        }
        if (op->code == HERO_UI_DRAW_FILL) {
            memcpy(op->rgba, p + 1, 4);
        }
        break;
    case HERO_UI_DRAW_STROKE:
        op->end = p[4];
        op->join = p[5];
        memcpy(op->rgba, p + 6, 4);
        if (!(op->real[0] > 0.0f) || op->end > 2 || op->join > 2) {
            return 0;
        }
        break;
    case HERO_UI_DRAW_TEXT:
        op->style = p[12];
        memcpy(op->rgba, p + 13, 4);
        op->family_count = (uint32_t)p[17] | (uint32_t)p[18] << 8;
        op->text_count = hero_ui_draw_u32(p + 19);
        if (!(op->real[2] > 0.0f) || op->style > 3 || count - at - 1 - fixed < (size_t)op->family_count + (size_t)op->text_count) {
            return 0;
        }
        op->family = p + fixed;
        op->text = p + fixed + op->family_count;
        return at + 1 + fixed + op->family_count + op->text_count;
    case HERO_UI_DRAW_IMAGE:
        op->columns = hero_ui_draw_u32(p + 16);
        op->rows = hero_ui_draw_u32(p + 20);
        if (!(op->real[2] > 0.0f) || !(op->real[3] > 0.0f) || op->columns < 1 || op->columns > 8192 || op->rows < 1 || op->rows > 8192) {
            return 0;
        }
        if (count - at - 1 - fixed < (size_t)op->columns * op->rows * 4) {
            return 0;
        }
        op->pixels = p + fixed;
        return at + 1 + fixed + (size_t)op->columns * op->rows * 4;
    default:
        break;
    }
    return at + 1 + fixed;
}

/* Whether the `count` bytes of `list` read to their end, operation by
 * operation, with every restore finding a state kept. */
static bool hero_ui_draw_whole(const uint8_t *list, size_t count) {
    size_t at = 0;
    int64_t kept = 0;
    hero_ui_draw_op op;

    while (at < count) {
        size_t next = hero_ui_draw_read(list, count, at, &op);

        if (next == 0) {
            return false;
        }
        if (op.code == HERO_UI_DRAW_SAVE) {
            kept += 1;
            if (kept > HERO_UI_DRAW_KEPT_MOST) {
                return false;
            }
        } else if (op.code == HERO_UI_DRAW_RESTORE) {
            if (kept == 0) {
                return false;
            }
            kept -= 1;
        }
        at = next;
    }
    return true;
}

/* -- The current path ------------------------------------------------------ */

/* A step of the path: `MOVE` starts a figure at p[0], p[1]; `LINE` goes to
 * p[0], p[1]; `QUAD` through p[0], p[1] to p[2], p[3]; `CUBIC` through p[0]
 * to p[3] to p[4], p[5]; `CLOSE` closes the figure. */
typedef struct hero_ui_draw_step {
    uint8_t code;
    float p[6];
} hero_ui_draw_step;

/* The path: its steps, whether a figure stands open, the current point
 * (none on an empty path) and where the figure began. `failed` is set where
 * a step found no memory, and the path is then marked nothing. */
typedef struct hero_ui_draw_path {
    hero_ui_draw_step *steps;
    size_t count;
    size_t room;
    bool open;
    bool has_point;
    bool failed;
    float x;
    float y;
    float start_x;
    float start_y;
} hero_ui_draw_path;

static void hero_ui_draw_path_free(hero_ui_draw_path *path) {
    hero_layer_free(path->steps);
    memset(path, 0, sizeof *path);
}

/* `BEGIN`: the path emptied, its room kept. */
static void hero_ui_draw_path_begin(hero_ui_draw_path *path) {
    path->count = 0;
    path->open = false;
    path->has_point = false;
    path->failed = false;
}

static void hero_ui_draw_path_add(hero_ui_draw_path *path, uint8_t code, const float *p, size_t reals) {
    hero_ui_draw_step *step;

    if (path->failed) {
        return;
    }
    if (path->count == path->room) {
        size_t room = path->room == 0 ? 64 : path->room * 2;
        hero_ui_draw_step *grown = room > SIZE_MAX / sizeof *grown ? NULL : hero_layer_grow(path->steps, room * sizeof *grown);

        if (grown == NULL) {
            path->failed = true;
            return;
        }
        path->steps = grown;
        path->room = room;
    }
    step = &path->steps[path->count];
    memset(step, 0, sizeof *step);
    step->code = code;
    for (size_t i = 0; i < reals; i += 1) {
        step->p[i] = p[i];
    }
    path->count += 1;
}

static void hero_ui_draw_path_move(hero_ui_draw_path *path, float x, float y) {
    float p[2] = {x, y};

    hero_ui_draw_path_add(path, HERO_UI_DRAW_MOVE, p, 2);
    path->open = true;
    path->has_point = true;
    path->x = path->start_x = x;
    path->y = path->start_y = y;
}

/* A segment begins a figure where the last one closed, as Core Graphics'
 * does: the point it closed at, which is where it began. */
static void hero_ui_draw_path_segment(hero_ui_draw_path *path, uint8_t code, const float *p, size_t reals) {
    if (!path->open) {
        hero_ui_draw_path_move(path, path->x, path->y);
    }
    hero_ui_draw_path_add(path, code, p, reals);
    path->x = p[reals - 2];
    path->y = p[reals - 1];
}

static void hero_ui_draw_path_line(hero_ui_draw_path *path, float x, float y) {
    float p[2] = {x, y};

    if (!path->has_point) {
        hero_ui_draw_path_move(path, x, y);
        return;
    }
    hero_ui_draw_path_segment(path, HERO_UI_DRAW_LINE, p, 2);
}

static void hero_ui_draw_path_curve(hero_ui_draw_path *path, uint8_t code, const float *p) {
    if (path->has_point) {
        hero_ui_draw_path_segment(path, code, p, code == HERO_UI_DRAW_QUAD ? 4 : 6);
    }
}

static void hero_ui_draw_path_close(hero_ui_draw_path *path) {
    if (path->has_point && path->open) {
        hero_ui_draw_path_add(path, HERO_UI_DRAW_CLOSE, NULL, 0);
        path->open = false;
        path->x = path->start_x;
        path->y = path->start_y;
    }
}

static void hero_ui_draw_path_rect(hero_ui_draw_path *path, float x, float y, float width, float height) {
    hero_ui_draw_path_move(path, x, y);
    hero_ui_draw_path_line(path, x + width, y);
    hero_ui_draw_path_line(path, x + width, y + height);
    hero_ui_draw_path_line(path, x, y + height);
    hero_ui_draw_path_close(path);
}

static void hero_ui_draw_path_ellipse(hero_ui_draw_path *path, float x, float y, float width, float height) {
    double cx = (double)x + (double)width / 2.0;
    double cy = (double)y + (double)height / 2.0;
    double kx = (double)width / 2.0 * HERO_UI_DRAW_KAPPA;
    double ky = (double)height / 2.0 * HERO_UI_DRAW_KAPPA;
    double right = (double)x + (double)width;
    double bottom = (double)y + (double)height;
    const double quarters[4][6] = {
        {right, cy + ky, cx + kx, bottom, cx, bottom},
        {cx - kx, bottom, x, cy + ky, x, cy},
        {x, cy - ky, cx - kx, y, cx, y},
        {cx + kx, y, right, cy - ky, right, cy},
    };

    hero_ui_draw_path_move(path, (float)right, (float)cy);
    for (size_t i = 0; i < 4; i += 1) {
        float p[6];

        for (size_t j = 0; j < 6; j += 1) {
            p[j] = (float)quarters[i][j];
        }
        hero_ui_draw_path_curve(path, HERO_UI_DRAW_CUBIC, p);
    }
    hero_ui_draw_path_close(path);
}

/* Takes path operation `op` into `path`; false for an operation that is no
 * path's. */
static bool hero_ui_draw_path_take(hero_ui_draw_path *path, const hero_ui_draw_op *op) {
    switch (op->code) {
    case HERO_UI_DRAW_BEGIN:
        hero_ui_draw_path_begin(path);
        return true;
    case HERO_UI_DRAW_MOVE:
        hero_ui_draw_path_move(path, op->real[0], op->real[1]);
        return true;
    case HERO_UI_DRAW_LINE:
        hero_ui_draw_path_line(path, op->real[0], op->real[1]);
        return true;
    case HERO_UI_DRAW_QUAD:
    case HERO_UI_DRAW_CUBIC:
        hero_ui_draw_path_curve(path, op->code, op->real);
        return true;
    case HERO_UI_DRAW_CLOSE:
        hero_ui_draw_path_close(path);
        return true;
    case HERO_UI_DRAW_RECT:
        hero_ui_draw_path_rect(path, op->real[0], op->real[1], op->real[2], op->real[3]);
        return true;
    case HERO_UI_DRAW_ELLIPSE:
        hero_ui_draw_path_ellipse(path, op->real[0], op->real[1], op->real[2], op->real[3]);
        return true;
    default:
        return false;
    }
}

/* -- The transform ----------------------------------------------------------- */

/* A point x, y goes to x * a + y * c + e, x * b + y * d + f: Direct2D's
 * `D2D1_MATRIX_3X2_F` in its order (_11 _12 _21 _22 _31 _32), a point a row
 * multiplied on the left. Kept in doubles and handed over in floats. */
typedef struct hero_ui_draw_matrix {
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
} hero_ui_draw_matrix;

static hero_ui_draw_matrix hero_ui_draw_identity(void) {
    hero_ui_draw_matrix m = {1.0, 0.0, 0.0, 1.0, 0.0, 0.0};

    return m;
}

/* `first` then `then`: a point through `first`, the result through `then`. */
static hero_ui_draw_matrix hero_ui_draw_then(hero_ui_draw_matrix first, hero_ui_draw_matrix then) {
    hero_ui_draw_matrix m;

    m.a = first.a * then.a + first.b * then.c;
    m.b = first.a * then.b + first.b * then.d;
    m.c = first.c * then.a + first.d * then.c;
    m.d = first.c * then.b + first.d * then.d;
    m.e = first.e * then.a + first.f * then.c + then.e;
    m.f = first.e * then.b + first.f * then.d + then.f;
    return m;
}

/* `TRANSLATE`, `SCALE` and `ROTATE` applied to the transform `m`, as Core
 * Graphics' `translateBy`, `scaleBy` and `rotate(by:)` are: the new step
 * acts on a point first, then the transform it is added to. A positive angle
 * turns clockwise on a screen whose y goes down. */
static hero_ui_draw_matrix hero_ui_draw_moved(hero_ui_draw_matrix m, const hero_ui_draw_op *op) {
    hero_ui_draw_matrix step = hero_ui_draw_identity();

    if (op->code == HERO_UI_DRAW_TRANSLATE) {
        step.e = op->real[0];
        step.f = op->real[1];
    } else if (op->code == HERO_UI_DRAW_SCALE) {
        step.a = op->real[0];
        step.d = op->real[1];
    } else if (op->code == HERO_UI_DRAW_ROTATE) {
        double cosine = cos((double)op->real[0]);
        double sine = sin((double)op->real[0]);

        step.a = cosine;
        step.b = sine;
        step.c = -sine;
        step.d = cosine;
    } else {
        return m;
    }
    return hero_ui_draw_then(step, m);
}

static void hero_ui_draw_floats(hero_ui_draw_matrix m, float out[6]) {
    out[0] = (float)m.a;
    out[1] = (float)m.b;
    out[2] = (float)m.c;
    out[3] = (float)m.d;
    out[4] = (float)m.e;
    out[5] = (float)m.f;
}

/* -- Pixels -------------------------------------------------------------------- */

/* The `count` pixels of `rgba`, 8-bit RGBA with straight alpha, written to
 * `bgra` as Direct2D's `DXGI_FORMAT_B8G8R8A8_UNORM` premultiplied holds
 * them: each colour times its alpha, rounded. */
static void hero_ui_draw_premultiplied(const uint8_t *rgba, uint8_t *bgra, size_t count) {
    for (size_t i = 0; i < count; i += 1) {
        uint32_t alpha = rgba[4 * i + 3];

        bgra[4 * i] = (uint8_t)((rgba[4 * i + 2] * alpha + 127) / 255);
        bgra[4 * i + 1] = (uint8_t)((rgba[4 * i + 1] * alpha + 127) / 255);
        bgra[4 * i + 2] = (uint8_t)((rgba[4 * i] * alpha + 127) / 255);
        bgra[4 * i + 3] = (uint8_t)alpha;
    }
}

/* The `count` premultiplied BGRA pixels of `bgra` written to `rgba` with
 * straight alpha: each colour divided by its alpha, rounded, and never past
 * 255 (a colour a rasteriser left above its alpha); a clear pixel is 0, 0,
 * 0, 0. */
static void hero_ui_draw_straight(const uint8_t *bgra, uint8_t *rgba, size_t count) {
    for (size_t i = 0; i < count; i += 1) {
        uint32_t alpha = bgra[4 * i + 3];

        for (size_t c = 0; c < 3; c += 1) {
            uint32_t value = alpha == 0 ? 0 : (bgra[4 * i + 2 - c] * 255u + alpha / 2) / alpha;

            rgba[4 * i + c] = (uint8_t)(value > 255 ? 255 : value);
        }
        rgba[4 * i + 3] = (uint8_t)alpha;
    }
}

/* -- The pointer --------------------------------------------------------------- */

/* `n` / `d` rounded half away from 0; `d` above 0. */
static int64_t hero_ui_draw_divided(int64_t n, int64_t d) {
    return n >= 0 ? (n + d / 2) / d : -((-n + d / 2) / d);
}

/* Pixel `pixels` of a window at `dpi` dots an inch, as 64ths of a point (a
 * point is 1/96 inch, Windows' own unit at 100 %). */
static int64_t hero_ui_draw_sixty_fourths(int64_t pixels, uint32_t dpi) {
    return hero_ui_draw_divided(pixels * 6144, dpi == 0 ? 96 : (int64_t)dpi);
}

/* `sixty_fourths` of a point as the nearest pixel at `dpi`. */
static int64_t hero_ui_draw_pixels(int64_t sixty_fourths, uint32_t dpi) {
    return hero_ui_draw_divided(sixty_fourths * (dpi == 0 ? 96 : (int64_t)dpi), 6144);
}

/* The last press, for counting clicks as Windows counts a double click:
 * the same button again within the system's double-click time and inside
 * its double-click rectangle around the last press. */
typedef struct hero_ui_draw_clicks {
    int64_t button;
    int64_t clicks;
    uint32_t time;
    int64_t x;
    int64_t y;
} hero_ui_draw_clicks;

/* The clicks of a press of `button` at pixel x, y at time `time` (ms, as
 * `GetMessageTime`, which wraps), given the double-click time `within` and
 * rectangle `width` by `height`; `last` becomes this press. */
static int64_t hero_ui_draw_click(hero_ui_draw_clicks *last, int64_t button, uint32_t time, int64_t x, int64_t y, uint32_t within, int64_t width, int64_t height) {
    int64_t dx = x - last->x;
    int64_t dy = y - last->y;
    bool again = last->clicks > 0 && last->button == button && (uint32_t)(time - last->time) <= within && dx <= width / 2 && -dx <= width / 2 && dy <= height / 2 && -dy <= height / 2;

    last->clicks = again ? (last->clicks < 255 ? last->clicks + 1 : 255) : 1;
    last->button = button;
    last->time = time;
    last->x = x;
    last->y = y;
    return last->clicks;
}

/* -- The keys ------------------------------------------------------------------ */

/* The name `hero_ui_set_shortcut` gives virtual key `key` (Windows' numbers,
 * `winuser.h`), or NULL for one it names none: a letter in lower case, a
 * digit of the row or of the keypad, F1 to F12 and the named keys. Where it
 * is a letter or a digit `one` holds it and is answered. */
static const char *hero_ui_draw_key_name(uint32_t key, char one[2]) {
    static const struct {
        uint32_t key;
        const char *name;
    } named[] = {
        {0x08, "Backspace"}, {0x09, "Tab"},  {0x0D, "Enter"}, {0x1B, "Escape"}, {0x20, "Space"}, {0x21, "PageUp"}, {0x22, "PageDown"},
        {0x23, "End"},       {0x24, "Home"}, {0x25, "Left"},  {0x26, "Up"},     {0x27, "Right"}, {0x28, "Down"},   {0x2E, "Delete"},
    };
    static const char *const functions[12] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};

    one[1] = '\0';
    if (key >= 'A' && key <= 'Z') {
        one[0] = (char)(key - 'A' + 'a');
        return one;
    }
    if (key >= '0' && key <= '9') {
        one[0] = (char)key;
        return one;
    }
    if (key >= 0x60 && key <= 0x69) {
        one[0] = (char)('0' + (key - 0x60));
        return one;
    }
    if (key >= 0x70 && key <= 0x7B) {
        return functions[key - 0x70];
    }
    for (size_t i = 0; i < sizeof named / sizeof named[0]; i += 1) {
        if (named[i].key == key) {
            return named[i].name;
        }
    }
    return NULL;
}

/* Whether virtual key `key` is a key held with others (Shift, Control, Alt,
 * the Windows keys, Caps Lock), which macOS tells no key event of and this
 * arm tells none of either. */
static bool hero_ui_draw_key_held_with(uint32_t key) {
    return key == 0x10 || key == 0x11 || key == 0x12 || key == 0x14 || key == 0x5B || key == 0x5C || (key >= 0xA0 && key <= 0xA5);
}

/* -- Text ---------------------------------------------------------------------- */

/* The `count` UTF-16 units of `units` written to `out` as UTF-8 where they
 * fit in `room`, a lone surrogate as U+FFFD; answers the bytes written,
 * whole characters only. */
static size_t hero_ui_draw_utf8(const uint16_t *units, size_t count, uint8_t *out, size_t room) {
    size_t made = 0;

    for (size_t i = 0; i < count; i += 1) {
        uint32_t c = units[i];
        uint8_t bytes[4];
        size_t n;

        if (c >= 0xD800 && c <= 0xDBFF && i + 1 < count && units[i + 1] >= 0xDC00 && units[i + 1] <= 0xDFFF) {
            c = 0x10000 + ((c - 0xD800) << 10) + (units[i + 1] - 0xDC00u);
            i += 1;
        } else if (c >= 0xD800 && c <= 0xDFFF) {
            c = 0xFFFD;
        }
        if (c < 0x80) {
            bytes[0] = (uint8_t)c;
            n = 1;
        } else if (c < 0x800) {
            bytes[0] = (uint8_t)(0xC0 | c >> 6);
            bytes[1] = (uint8_t)(0x80 | (c & 0x3F));
            n = 2;
        } else if (c < 0x10000) {
            bytes[0] = (uint8_t)(0xE0 | c >> 12);
            bytes[1] = (uint8_t)(0x80 | (c >> 6 & 0x3F));
            bytes[2] = (uint8_t)(0x80 | (c & 0x3F));
            n = 3;
        } else {
            bytes[0] = (uint8_t)(0xF0 | c >> 18);
            bytes[1] = (uint8_t)(0x80 | (c >> 12 & 0x3F));
            bytes[2] = (uint8_t)(0x80 | (c >> 6 & 0x3F));
            bytes[3] = (uint8_t)(0x80 | (c & 0x3F));
            n = 4;
        }
        if (room - made < n) {
            break;
        }
        memcpy(out + made, bytes, n);
        made += n;
    }
    return made;
}

/* The `count` bytes of UTF-8 `text` written to `out` as UTF-16 where they
 * fit in `room` units; a byte that begins no character, or a character cut
 * short, as U+FFFD. Answers the units written. */
static size_t hero_ui_draw_utf16(const uint8_t *text, size_t count, uint16_t *out, size_t room) {
    size_t made = 0;
    size_t i = 0;

    while (i < count) {
        uint32_t c = text[i];
        size_t n = c < 0x80 ? 1 : (c & 0xE0) == 0xC0 ? 2 : (c & 0xF0) == 0xE0 ? 3 : (c & 0xF8) == 0xF0 ? 4 : 0;
        bool whole = n > 0 && count - i >= n;

        for (size_t j = 1; whole && j < n; j += 1) {
            whole = (text[i + j] & 0xC0) == 0x80;
        }
        if (!whole) {
            c = 0xFFFD;
            n = 1;
        } else if (n > 1) {
            c &= n == 2 ? 0x1Fu : n == 3 ? 0x0Fu : 0x07u;
            for (size_t j = 1; j < n; j += 1) {
                c = c << 6 | (text[i + j] & 0x3Fu);
            }
            if (c > 0x10FFFF || (c >= 0xD800 && c <= 0xDFFF)) {
                c = 0xFFFD;
            }
        }
        if (c >= 0x10000) {
            if (room - made < 2) {
                break;
            }
            out[made++] = (uint16_t)(0xD800 + ((c - 0x10000) >> 10));
            out[made++] = (uint16_t)(0xDC00 + ((c - 0x10000) & 0x3FF));
        } else {
            if (room - made < 1) {
                break;
            }
            out[made++] = (uint16_t)c;
        }
        i += n;
    }
    return made;
}

#endif
