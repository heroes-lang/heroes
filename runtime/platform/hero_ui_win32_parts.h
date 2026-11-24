/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_win32_parts.h: THE PURE HALF OF THE WIN32 ARM'S PARTS OF A DRAWING
 * AREA (`hero_ui.h`'s A DRAWING AREA'S PARTS, READ BY A SCREEN READER), what
 * needs no window and no UI Automation: a list of parts read into records
 * whose tree (`within`) is linked both ways, the part under a point, a part's
 * box between the area's points and the screen's pixels, the record written
 * back from what the system's objects answer, and a role's UI Automation
 * control type and back. `hero_ui_parts_win32.c` keeps what needs the system.
 *
 * THE LIST IS `hero_ui.c`'S, checked whole before an arm sees it
 * (`hero_ui_parts_checked`): every record whole, every `within` an earlier
 * part, every number finite. The reader below asks every length all the
 * same and answers 0 parts for a list it cannot read whole, so a list that
 * reached it some other way is no part rather than a read past its end.
 *
 * THE FUNCTIONS ARE `static`, for the file that includes this header, and
 * need `stdint.h`, `stdbool.h`, `stddef.h` and `string.h` already included;
 * a test of its logic on another system includes them first. */

#ifndef HERO_UI_WIN32_PARTS_H
#define HERO_UI_WIN32_PARTS_H

/* A record's fixed bytes before its name and description (`hero_ui.c`'s
 * `HERO_UI_PART_FIXED`). */
#define HERO_UI_WIN_PART_FIXED 48

/* UI Automation's control types for the nine roles, `UIAutomationClient.h`'s
 * numbers, which the member checks against the system's header at compile
 * time. */
#define HERO_UI_WIN_CONTROL_BUTTON 50000
#define HERO_UI_WIN_CONTROL_CHECK_BOX 50002
#define HERO_UI_WIN_CONTROL_HYPERLINK 50005
#define HERO_UI_WIN_CONTROL_IMAGE 50006
#define HERO_UI_WIN_CONTROL_PROGRESS_BAR 50012
#define HERO_UI_WIN_CONTROL_RADIO_BUTTON 50013
#define HERO_UI_WIN_CONTROL_SLIDER 50015
#define HERO_UI_WIN_CONTROL_TEXT 50020
#define HERO_UI_WIN_CONTROL_GROUP 50026

/* One part as given: its role, states and holder (`within`, 0 the area),
 * its box in the area's points, its value and ends, its name and
 * description where they stand in the list; and the tree, each a part's
 * number (the first is 1) or 0 for none: its first and last part held, the
 * next and the previous part of the same holder, and its depth (1 for a
 * part of the area itself). */
typedef struct hero_ui_win_part {
    uint8_t role;
    uint8_t states;
    uint16_t within;
    float box[4];
    double value;
    double least;
    double most;
    const uint8_t *name;
    uint16_t named;
    const uint8_t *description;
    uint16_t described;
    uint32_t first;
    uint32_t last;
    uint32_t next;
    uint32_t previous;
    uint32_t depth;
} hero_ui_win_part;

static uint16_t hero_ui_win_part_u16(const uint8_t *at) {
    return (uint16_t)(at[0] | (uint16_t)at[1] << 8);
}

static float hero_ui_win_part_f32(const uint8_t *at) {
    uint32_t bits = (uint32_t)at[0] | (uint32_t)at[1] << 8 | (uint32_t)at[2] << 16 | (uint32_t)at[3] << 24;
    float real;

    memcpy(&real, &bits, sizeof real);
    return real;
}

static double hero_ui_win_part_f64(const uint8_t *at) {
    uint64_t bits = 0;
    double real;

    for (int shift = 0; shift < 8; shift += 1) {
        bits |= (uint64_t)at[shift] << (8 * shift);
    }
    memcpy(&real, &bits, sizeof real);
    return real;
}

/* The number of records the `count` bytes of `list` hold, or 0 where a
 * record runs past its end. */
static size_t hero_ui_win_parts_counted(const uint8_t *list, size_t count) {
    size_t at = 0;
    size_t parts = 0;

    while (at < count) {
        size_t named;
        size_t described;

        if (count - at < HERO_UI_WIN_PART_FIXED) {
            return 0;
        }
        named = hero_ui_win_part_u16(list + at + 44);
        described = hero_ui_win_part_u16(list + at + 46);
        if (count - at - HERO_UI_WIN_PART_FIXED < named + described) {
            return 0;
        }
        at += HERO_UI_WIN_PART_FIXED + named + described;
        parts += 1;
    }
    return parts;
}

/* Reads the `count` bytes of `list` into `parts`, which holds `room`
 * records, and links their tree; answers the parts read, 0 where the list
 * is not `room` records whole or a `within` names no earlier part. The
 * names point into `list`, which must outlive `parts`. */
static size_t hero_ui_win_parts_read(const uint8_t *list, size_t count, hero_ui_win_part *parts, size_t room) {
    size_t at = 0;
    size_t made = 0;

    if (hero_ui_win_parts_counted(list, count) != room) {
        return 0;
    }
    while (made < room) {
        hero_ui_win_part *part = &parts[made];
        const uint8_t *record = list + at;

        memset(part, 0, sizeof *part);
        part->role = record[0];
        part->states = record[1];
        part->within = hero_ui_win_part_u16(record + 2);
        for (size_t side = 0; side < 4; side += 1) {
            part->box[side] = hero_ui_win_part_f32(record + 4 + 4 * side);
        }
        part->value = hero_ui_win_part_f64(record + 20);
        part->least = hero_ui_win_part_f64(record + 28);
        part->most = hero_ui_win_part_f64(record + 36);
        part->named = hero_ui_win_part_u16(record + 44);
        part->described = hero_ui_win_part_u16(record + 46);
        part->name = record + HERO_UI_WIN_PART_FIXED;
        part->description = part->name + part->named;
        if (part->within > made) {
            return 0;
        }
        made += 1;
        /* Linked under its holder, after the holder's last part. */
        if (part->within == 0) {
            part->depth = 1;
        } else {
            hero_ui_win_part *holder = &parts[part->within - 1];

            part->depth = holder->depth + 1;
            part->previous = holder->last;
            if (holder->last != 0) {
                parts[holder->last - 1].next = (uint32_t)made;
            } else {
                holder->first = (uint32_t)made;
            }
            holder->last = (uint32_t)made;
        }
        at += HERO_UI_WIN_PART_FIXED + part->named + part->described;
    }
    /* The parts of the area itself, linked in their order. */
    {
        uint32_t before = 0;

        for (size_t number = 1; number <= made; number += 1) {
            if (parts[number - 1].within == 0) {
                parts[number - 1].previous = before;
                if (before != 0) {
                    parts[before - 1].next = (uint32_t)number;
                }
                before = (uint32_t)number;
            }
        }
    }
    return made;
}

/* The first and the last part of the area itself, 0 for none. */
static uint32_t hero_ui_win_parts_first(const hero_ui_win_part *parts, size_t count) {
    for (size_t number = 1; number <= count; number += 1) {
        if (parts[number - 1].within == 0) {
            return (uint32_t)number;
        }
    }
    return 0;
}

static uint32_t hero_ui_win_parts_last(const hero_ui_win_part *parts, size_t count) {
    for (size_t number = count; number >= 1; number -= 1) {
        if (parts[number - 1].within == 0) {
            return (uint32_t)number;
        }
    }
    return 0;
}

/* The part under point `x`, `y` of the area, in its points: the deepest in
 * the tree whose box holds it, of two as deep the later given (drawn last,
 * so on top); 0 for none. A box holds its left and top edges, not its right
 * and bottom ones, so two parts side by side never both hold a point. */
static uint32_t hero_ui_win_parts_hit(const hero_ui_win_part *parts, size_t count, double x, double y) {
    uint32_t found = 0;
    uint32_t deepest = 0;

    for (size_t number = 1; number <= count; number += 1) {
        const hero_ui_win_part *part = &parts[number - 1];
        double left = part->box[0];
        double top = part->box[1];

        if (x >= left && y >= top && x < left + (double)part->box[2] && y < top + (double)part->box[3] && part->depth >= deepest) {
            found = (uint32_t)number;
            deepest = part->depth;
        }
    }
    return found;
}

/* A length in the area's points as pixels at `dpi` (96 one pixel to a
 * point), and back. */
static double hero_ui_win_parts_pixels(double points, uint32_t dpi) {
    return points * (double)dpi / 96.0;
}

static double hero_ui_win_parts_points(double pixels, uint32_t dpi) {
    return pixels * 96.0 / (double)dpi;
}

/* Role `role`'s UI Automation control type, 0 for none. */
static int32_t hero_ui_win_parts_control(uint8_t role) {
    static const int32_t controls[10] = {0, HERO_UI_WIN_CONTROL_GROUP, HERO_UI_WIN_CONTROL_TEXT, HERO_UI_WIN_CONTROL_IMAGE, HERO_UI_WIN_CONTROL_BUTTON, HERO_UI_WIN_CONTROL_CHECK_BOX, HERO_UI_WIN_CONTROL_RADIO_BUTTON, HERO_UI_WIN_CONTROL_SLIDER, HERO_UI_WIN_CONTROL_PROGRESS_BAR, HERO_UI_WIN_CONTROL_HYPERLINK};

    return role < 10 ? controls[role] : 0;
}

/* The role whose control type is `control`, 0 for none. */
static uint8_t hero_ui_win_parts_role(int32_t control) {
    for (uint8_t role = 1; role < 10; role += 1) {
        if (hero_ui_win_parts_control(role) == control) {
            return role;
        }
    }
    return 0;
}

static void hero_ui_win_parts_put_u16(uint8_t *at, uint16_t value) {
    at[0] = (uint8_t)(value & 0xFF);
    at[1] = (uint8_t)(value >> 8);
}

/* Writes one record of the read-back at `out`, which holds `room` bytes,
 * from `part` (its name and description where its pointers say); answers
 * the record's length, nothing written where `room` does not hold it. */
static size_t hero_ui_win_parts_put(uint8_t *out, size_t room, const hero_ui_win_part *part) {
    size_t length = HERO_UI_WIN_PART_FIXED + (size_t)part->named + part->described;
    double values[3];

    if (out == NULL || room < length) {
        return length;
    }
    out[0] = part->role;
    out[1] = part->states;
    hero_ui_win_parts_put_u16(out + 2, part->within);
    for (size_t side = 0; side < 4; side += 1) {
        uint32_t bits;

        memcpy(&bits, &part->box[side], sizeof bits);
        for (size_t byte = 0; byte < 4; byte += 1) {
            out[4 + 4 * side + byte] = (uint8_t)(bits >> (8 * byte));
        }
    }
    values[0] = part->value;
    values[1] = part->least;
    values[2] = part->most;
    for (size_t at = 0; at < 3; at += 1) {
        uint64_t bits;

        memcpy(&bits, &values[at], sizeof bits);
        for (size_t byte = 0; byte < 8; byte += 1) {
            out[20 + 8 * at + byte] = (uint8_t)(bits >> (8 * byte));
        }
    }
    hero_ui_win_parts_put_u16(out + 44, part->named);
    hero_ui_win_parts_put_u16(out + 46, part->described);
    if (part->named > 0) {
        memcpy(out + HERO_UI_WIN_PART_FIXED, part->name, part->named);
    }
    if (part->described > 0) {
        memcpy(out + HERO_UI_WIN_PART_FIXED + part->named, part->description, part->described);
    }
    return length;
}

#endif
