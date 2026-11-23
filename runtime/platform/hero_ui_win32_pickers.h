/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_win32_pickers.h: THE PURE HALF OF THE WIN32 ARM'S PICKERS (the date,
 * time and date and time pickers, the colour well and the font button of
 * `hero_ui.h`), the arithmetic and the text that need no window: a moment
 * as the seconds of its calendar fields read as UTC and back, which moments a
 * kind holds, the date pattern a picker shows, and the font descriptor
 * `<size>|<style>|<family>` read and written. The arm's own file keeps what
 * needs a control.
 *
 * NO TIME ZONE CROSSES THE LAYER. A picker's value is the seconds since
 * 1970-01-01T00:00:00 of the year, month, day, hour, minute and second the
 * control SHOWS, so the conversion is the proleptic Gregorian calendar's own
 * and neither `SystemTimeToFileTime` nor `TzSpecificLocalTimeToSystemTime`
 * stands between a `SYSTEMTIME` and the number: a `SYSTEMTIME`'s fields are
 * the calendar's fields.
 *
 * THE FUNCTIONS ARE `static`, for the file that includes this header, and need
 * `stdint.h`, `stdbool.h`, `wchar_t` and the `HERO_UI_*` kinds already
 * declared; a test of its logic on another system declares `wchar_t` as it
 * likes (the arm's is UTF-16) and includes `hero_ui.h` first.
 */

#ifndef HERO_UI_WIN32_PICKERS_H
#define HERO_UI_WIN32_PICKERS_H

#define HERO_UI_WIN_DAY_SECONDS INT64_C(86400)
/* The first day a `SYSTEMTIME` holds, 1601-01-01, and the last moments the
 * calendars of the controls reach: 9999-12-31 for a date, its last second for
 * a date and time. */
#define HERO_UI_WIN_STAMP_FLOOR (-INT64_C(11644473600))
#define HERO_UI_WIN_STAMP_DAY_MOST INT64_C(253402214400)
#define HERO_UI_WIN_STAMP_MOST INT64_C(253402300799)
/* The first day of the contract's own calendar, 1-01-01, where the ends of
 * the other arms' pickers and the face's own start. */
#define HERO_UI_WIN_STAMP_YEAR_ONE (-INT64_C(62135596800))
/* A `LOGFONTW`'s `lfFaceName`: 31 units and the zero. */
#define HERO_UI_WIN_FACE_UNITS 32

/* One moment's calendar fields. */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} hero_ui_win_fields;

/* The days from 1970-01-01 to the day `year`, `month` (1 to 12) and `day`
 * name, the proleptic Gregorian calendar's, before 1970 negative. */
static int64_t hero_ui_win_stamp_days(int64_t year, int month, int day) {
    int64_t y = month <= 2 ? year - 1 : year;
    int64_t era = (y >= 0 ? y : y - 399) / 400;
    int64_t of_era = y - era * 400;
    int64_t of_year = (153 * (int64_t)(month + (month > 2 ? -3 : 9)) + 2) / 5 + day - 1;
    int64_t in_era = of_era * 365 + of_era / 4 - of_era / 100 + of_year;

    return era * 146097 + in_era - 719468;
}

/* The year, month and day `days` days from 1970-01-01 is. */
static void hero_ui_win_stamp_civil(int64_t days, int *year, int *month, int *day) {
    int64_t z = days + 719468;
    int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    int64_t in_era = z - era * 146097;
    int64_t of_era = (in_era - in_era / 1460 + in_era / 36524 - in_era / 146096) / 365;
    int64_t of_year = in_era - (365 * of_era + of_era / 4 - of_era / 100);
    int64_t from_march = (5 * of_year + 2) / 153;
    int64_t last = of_year - (153 * from_march + 2) / 5 + 1;
    int64_t first = from_march < 10 ? from_march + 3 : from_march - 9;

    *year = (int)(first <= 2 ? of_era + era * 400 + 1 : of_era + era * 400);
    *month = (int)first;
    *day = (int)last;
}

/* The fields of the moment `seconds` is, which `hero_ui_win_stamp_refusal`
 * has judged: a year that fits an `int`. */
static void hero_ui_win_stamp_fields(int64_t seconds, hero_ui_win_fields *fields) {
    int64_t days = seconds / HERO_UI_WIN_DAY_SECONDS;
    int64_t rest = seconds % HERO_UI_WIN_DAY_SECONDS;

    if (rest < 0) {
        rest += HERO_UI_WIN_DAY_SECONDS;
        days -= 1;
    }
    hero_ui_win_stamp_civil(days, &fields->year, &fields->month, &fields->day);
    fields->hour = (int)(rest / 3600);
    fields->minute = (int)(rest % 3600 / 60);
    fields->second = (int)(rest % 60);
}

/* The seconds `fields` name. */
static int64_t hero_ui_win_stamp_seconds(const hero_ui_win_fields *fields) {
    return hero_ui_win_stamp_days(fields->year, fields->month, fields->day) * HERO_UI_WIN_DAY_SECONDS + (int64_t)fields->hour * 3600 + (int64_t)fields->minute * 60 + fields->second;
}

/* The words saying why a picker of `kind` holds no moment `seconds`, NULL
 * where it holds it: a date a whole day from the first a `SYSTEMTIME` has to
 * the last of the year 9999, a time the seconds of a day, a date and time any
 * second between those two days. */
static const char *hero_ui_win_stamp_refusal(int64_t kind, int64_t seconds) {
    if (kind == HERO_UI_TIME_PICKER) {
        return seconds >= 0 && seconds < HERO_UI_WIN_DAY_SECONDS ? NULL : "a time picker holds the seconds since midnight, 0 to 86399";
    }
    if (seconds < HERO_UI_WIN_STAMP_FLOOR) {
        return "Windows' date and time controls hold no moment before 1601-01-01 (-11644473600), the first day a SYSTEMTIME has";
    }
    if (seconds > (kind == HERO_UI_DATE_PICKER ? HERO_UI_WIN_STAMP_DAY_MOST : HERO_UI_WIN_STAMP_MOST)) {
        return "a date picker holds no day after 9999-12-31 (253402214400) and a date and time picker no second after 9999-12-31T23:59:59 (253402300799)";
    }
    if (kind == HERO_UI_DATE_PICKER && seconds % HERO_UI_WIN_DAY_SECONDS != 0) {
        return "a date picker holds a whole day, midnight of it, a multiple of 86400";
    }
    return NULL;
}

/* The lower end `least` of a range as the control holds it: the first day of
 * a date or a date and time picker's range may be any the contract's calendar
 * has, and one before the control's first day is the control's, since no
 * person could pick before it. A time picker's end is its own, and an end
 * before the contract's first day is left for `hero_ui_win_stamp_refusal` to
 * judge. */
static int64_t hero_ui_win_stamp_least(int64_t kind, int64_t least) {
    return kind != HERO_UI_TIME_PICKER && least >= HERO_UI_WIN_STAMP_YEAR_ONE && least < HERO_UI_WIN_STAMP_FLOOR ? HERO_UI_WIN_STAMP_FLOOR : least;
}

/* The ends a picker of `kind` has until its range is set. */
static void hero_ui_win_stamp_span(int64_t kind, int64_t *least, int64_t *most) {
    if (kind == HERO_UI_TIME_PICKER) {
        *least = 0;
        *most = HERO_UI_WIN_DAY_SECONDS - 1;
    } else {
        *least = HERO_UI_WIN_STAMP_FLOOR;
        *most = kind == HERO_UI_DATE_PICKER ? HERO_UI_WIN_STAMP_DAY_MOST : HERO_UI_WIN_STAMP_MOST;
    }
}

/* Writes unit `unit` into `out` at `*at` where `room` leaves one for the
 * zero; whether it fitted. */
static bool hero_ui_win_pattern_emit(wchar_t *out, int room, int *at, wchar_t unit) {
    if (*at + 1 >= room) {
        return false;
    }
    out[*at] = unit;
    *at += 1;
    return true;
}

/* The short date pattern `locale` (`LOCALE_SSHORTDATE`) made into the format
 * a date and time picker's `DTM_SETFORMAT` takes, in `out` of `room` units: a
 * run of `y` is a four-digit year (the value is the year as it is, never two
 * digits read into a century), `d` and `M` runs of one to four stay, text in
 * single quotes and every character that is no ASCII letter stay as written.
 * False where the pattern holds another letter outside its quotes (`g`, an
 * era, which the control's formats do not have), a run that is too long, an
 * unclosed quote, or no room: the caller shows `yyyy-MM-dd`. */
static bool hero_ui_win_pattern_date(const wchar_t *locale, wchar_t *out, int room) {
    int at = 0;
    bool quoted = false;

    for (const wchar_t *c = locale; *c != 0;) {
        if (*c == L'\'') {
            quoted = !quoted;
            if (!hero_ui_win_pattern_emit(out, room, &at, *c)) {
                return false;
            }
            c += 1;
        } else if (quoted || !((*c >= L'a' && *c <= L'z') || (*c >= L'A' && *c <= L'Z'))) {
            if (!hero_ui_win_pattern_emit(out, room, &at, *c)) {
                return false;
            }
            c += 1;
        } else if (*c == L'y' || *c == L'd' || *c == L'M') {
            wchar_t letter = *c;
            int run = 0;
            int shown;

            while (c[run] == letter) {
                run += 1;
            }
            if (letter != L'y' && run > 4) {
                return false;
            }
            shown = letter == L'y' ? 4 : run;
            for (int i = 0; i < shown; i += 1) {
                if (!hero_ui_win_pattern_emit(out, room, &at, letter)) {
                    return false;
                }
            }
            c += run;
        } else {
            return false;
        }
    }
    out[at] = 0;
    return !quoted && at > 0;
}

/* A font as a font button holds it: the size in points, the style and the
 * family's name. */
typedef struct {
    int size;
    bool bold;
    bool italic;
    wchar_t family[HERO_UI_WIN_FACE_UNITS];
} hero_ui_win_font_spec;

/* Reads descriptor `text`, `<size>|<style>|<family>`: a size of whole digits
 * from 1 to 1024 with no zero in front, the style `-`, `b`, `i` or `bi`, and
 * the family's name, the rest of the text after the second `|` (a `|` in it
 * is fine), 1 to 31 units with no line break. NULL where it is one, else the
 * words saying what it is not; the family is NOT looked for in the system. */
static const char *hero_ui_win_font_parse(const wchar_t *text, hero_ui_win_font_spec *spec) {
    const char *shape = "a font is `<size>|<style>|<family>`: the size in points, the style (`-`, `b`, `i` or `bi`) and the family's name";
    const wchar_t *at = text;
    int64_t size = 0;
    int digits = 0;
    int units = 0;

    for (; *at >= L'0' && *at <= L'9'; at += 1) {
        if (digits == 1 && size == 0) {
            return "a font's size is whole digits from 1 to 1024 with no zero in front";
        }
        size = size * 10 + (*at - L'0');
        digits += 1;
        if (size > 1024) {
            return "a font's size is from 1 to 1024 points";
        }
    }
    if (digits == 0 || *at != L'|') {
        return shape;
    }
    if (size < 1) {
        return "a font's size is from 1 to 1024 points";
    }
    at += 1;
    spec->size = (int)size;
    spec->bold = false;
    spec->italic = false;
    if (at[0] == L'-' && at[1] == L'|') {
        at += 2;
    } else if (at[0] == L'b' && at[1] == L'i' && at[2] == L'|') {
        spec->bold = true;
        spec->italic = true;
        at += 3;
    } else if ((at[0] == L'b' || at[0] == L'i') && at[1] == L'|') {
        spec->bold = at[0] == L'b';
        spec->italic = at[0] == L'i';
        at += 2;
    } else {
        return "a font's style is `-`, `b`, `i` or `bi`";
    }
    for (; at[units] != 0; units += 1) {
        if (at[units] == L'\r' || at[units] == L'\n') {
            return "a font's family has no line break in it";
        }
        if (units >= HERO_UI_WIN_FACE_UNITS - 1) {
            return "a font's family is at most 31 units long on Windows, which names no family longer";
        }
        spec->family[units] = at[units];
    }
    if (units == 0) {
        return "a font names a family";
    }
    spec->family[units] = 0;
    return NULL;
}

/* Writes the descriptor of `spec` into `out`, room for `size`'s digits, the
 * style, 31 units and the zero: 42 units. */
static void hero_ui_win_font_format(const hero_ui_win_font_spec *spec, wchar_t *out) {
    wchar_t reversed[8];
    int count = 0;
    int at = 0;

    for (int rest = spec->size; rest > 0 && count < 8; rest /= 10) {
        reversed[count++] = (wchar_t)(L'0' + rest % 10);
    }
    while (count > 0) {
        out[at++] = reversed[--count];
    }
    out[at++] = L'|';
    if (!spec->bold && !spec->italic) {
        out[at++] = L'-';
    } else {
        out[at++] = spec->bold ? L'b' : L'i';
        if (spec->bold && spec->italic) {
            out[at++] = L'i';
        }
    }
    out[at++] = L'|';
    for (int i = 0; spec->family[i] != 0 && i < HERO_UI_WIN_FACE_UNITS - 1; i += 1) {
        out[at++] = spec->family[i];
    }
    out[at] = 0;
}

/* Writes what a font button shows of `spec` into `out` of `room` units
 * (`Family, 13 pt, bold italic`), each `&` of the family doubled as a button
 * draws it. 110 units hold the longest. */
static void hero_ui_win_font_label(const hero_ui_win_font_spec *spec, wchar_t *out, int room) {
    wchar_t descriptor[48];
    int at = 0;
    int from = 0;
    const char *style = spec->bold && spec->italic ? ", bold italic" : (spec->bold ? ", bold" : (spec->italic ? ", italic" : ""));

    hero_ui_win_font_format(spec, descriptor);
    for (int i = 0; spec->family[i] != 0; i += 1) {
        if (spec->family[i] == L'&') {
            (void)hero_ui_win_pattern_emit(out, room, &at, L'&');
        }
        (void)hero_ui_win_pattern_emit(out, room, &at, spec->family[i]);
    }
    (void)hero_ui_win_pattern_emit(out, room, &at, L',');
    (void)hero_ui_win_pattern_emit(out, room, &at, L' ');
    while (descriptor[from] != L'|') {
        (void)hero_ui_win_pattern_emit(out, room, &at, descriptor[from++]);
    }
    (void)hero_ui_win_pattern_emit(out, room, &at, L' ');
    (void)hero_ui_win_pattern_emit(out, room, &at, L'p');
    (void)hero_ui_win_pattern_emit(out, room, &at, L't');
    for (int i = 0; style[i] != 0; i += 1) {
        (void)hero_ui_win_pattern_emit(out, room, &at, (wchar_t)style[i]);
    }
    out[at] = 0;
}

#endif
