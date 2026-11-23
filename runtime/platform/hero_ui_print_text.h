/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_print_text.h — the print dialogs' settings read from and
 * written to THE SETTINGS' TEXT (`hero_ui_print_win32.h`), which the Win32
 * and GTK 4 arms of the print dialogs share; the macOS arm, in Swift, reads
 * the same text its own way, and `printing.format_settings` and
 * `printing.parse_settings` are its Heroes side. A header of the members alone: no Heroes group
 * names it, and its functions are `static`, each member compiling its own.
 */

#ifndef HERO_UI_PRINT_TEXT_H
#define HERO_UI_PRINT_TEXT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hero_printing.h"

/* A job's settings as the dialogs exchange them. */
typedef struct {
    char printer[HERO_PRINTING_NAME_MOST + 1];
    int64_t copies;
    char paper[HERO_PRINTING_NAME_MOST + 1];
    int64_t orientation;
    int64_t sides;
    int64_t color;
    int64_t collate;
    int64_t pages[2 * HERO_PRINTING_RANGES_MOST];
    int64_t range_count;
} hero_ui_print_settings;

/* IPP's keywords of the sides, the colour and the collation, at the
 * header's numbers (`hero_printing.h`): index 0 unused for the sides. */
static const char *const hero_ui_print_sides[] = {"", "one-sided", "two-sided-long-edge", "two-sided-short-edge"};
static const char *const hero_ui_print_colors[] = {"auto", "color", "monochrome"};
static const char *const hero_ui_print_collations[] = {"separate-documents-uncollated-copies", "separate-documents-collated-copies"};

/* The index of `word` among the `count` words, or -1. */
static int64_t hero_ui_print_word(const char *word, const char *const *words, int64_t count) {
    for (int64_t i = 0; i < count; i++) {
        if (strcmp(word, words[i]) == 0 && words[i][0] != '\0') {
            return i;
        }
    }
    return -1;
}

/* A whole decimal number of `text`, `*out` set; false for another text. */
static bool hero_ui_print_number(const char *text, int64_t *out) {
    char *end = NULL;
    long long n;

    if (text[0] == '\0') {
        return false;
    }
    n = strtoll(text, &end, 10);
    if (end == NULL || *end != '\0') {
        return false;
    }
    *out = (int64_t)n;
    return true;
}

/* The `count` bytes of `text` read into `out`, defaults first; false where
 * a line is no `<key>=<value>` of the eight, a value out of its bounds or
 * none of its keywords, or a line past `HERO_PRINTING_NAME_MOST` and the
 * pages' room. */
static bool hero_ui_print_read(const uint8_t *text, int64_t count, hero_ui_print_settings *out) {
    static const char *const keys[] = {"printer", "copies", "media", "orientation-requested", "sides", "print-color-mode", "multiple-document-handling", "page-ranges"};
    char line[HERO_PRINTING_RANGES_MOST * 24 + 16];
    unsigned seen = 0;
    int64_t at = 0;

    memset(out, 0, sizeof *out);
    out->copies = 1;
    out->orientation = HERO_PRINTING_PORTRAIT;
    out->sides = HERO_PRINTING_ONE_SIDED;
    out->color = HERO_PRINTING_COLOR_DEFAULT;
    out->collate = 1;
    if (count < 0 || (text == NULL && count > 0)) {
        return false;
    }
    while (at < count) {
        int64_t end = at;
        char *equals;
        size_t length;

        while (end < count && text[end] != '\n') {
            end++;
        }
        length = (size_t)(end - at);
        if (length == 0 || length >= sizeof line) {
            return false;
        }
        memcpy(line, text + at, length);
        line[length] = '\0';
        if (memchr(line, '\0', length) != NULL) {
            return false;
        }
        at = end + 1;
        equals = strchr(line, '=');
        if (equals == NULL) {
            return false;
        }
        *equals = '\0';
        /* A key given twice is refused, as `printing.parse_settings`
         * refuses it. */
        for (unsigned k = 0; k < sizeof keys / sizeof keys[0]; k++) {
            if (strcmp(line, keys[k]) == 0) {
                if ((seen & (1u << k)) != 0) {
                    return false;
                }
                seen |= 1u << k;
            }
        }
        if (strcmp(line, "printer") == 0 || strcmp(line, "media") == 0) {
            char *into = line[0] == 'p' ? out->printer : out->paper;

            if (strlen(equals + 1) > HERO_PRINTING_NAME_MOST) {
                return false;
            }
            memcpy(into, equals + 1, strlen(equals + 1) + 1);
        } else if (strcmp(line, "sides") == 0 || strcmp(line, "print-color-mode") == 0 || strcmp(line, "multiple-document-handling") == 0) {
            int64_t found = line[0] == 's' ? hero_ui_print_word(equals + 1, hero_ui_print_sides, 4) : line[0] == 'p' ? hero_ui_print_word(equals + 1, hero_ui_print_colors, 3) : hero_ui_print_word(equals + 1, hero_ui_print_collations, 2);

            if (found < 0) {
                return false;
            }
            if (line[0] == 's') {
                out->sides = found;
            } else if (line[0] == 'p') {
                out->color = found;
            } else {
                out->collate = found;
            }
        } else if (strcmp(line, "page-ranges") == 0) {
            char *cursor = equals + 1;

            out->range_count = 0;
            while (*cursor != '\0') {
                char *dash = strchr(cursor, '-');
                char *comma = strchr(cursor, ',');
                char *stop = comma != NULL ? comma : cursor + strlen(cursor);
                int64_t first, last;

                if (dash == NULL || dash > stop || out->range_count + 2 > 2 * HERO_PRINTING_RANGES_MOST) {
                    return false;
                }
                *dash = '\0';
                if (comma != NULL) {
                    *comma = '\0';
                }
                if (!hero_ui_print_number(cursor, &first) || !hero_ui_print_number(dash + 1, &last) || first < 1 || last < first) {
                    return false;
                }
                out->pages[out->range_count++] = first;
                out->pages[out->range_count++] = last;
                cursor = comma != NULL ? comma + 1 : stop;
            }
        } else {
            int64_t n;

            if (!hero_ui_print_number(equals + 1, &n)) {
                return false;
            }
            if (strcmp(line, "copies") == 0 && n >= 1 && n <= HERO_PRINTING_COPIES_MOST) {
                out->copies = n;
            } else if (strcmp(line, "orientation-requested") == 0 && n >= HERO_PRINTING_PORTRAIT && n <= HERO_PRINTING_REVERSE_PORTRAIT) {
                out->orientation = n;
            } else {
                return false;
            }
        }
    }
    return true;
}

/* `settings` written as the text into `out` of `room` bytes; its length,
 * or -1 where it does not fit. */
static int64_t hero_ui_print_write(const hero_ui_print_settings *settings, char *out, size_t room) {
    size_t used;
    int n = snprintf(out, room, "printer=%s\ncopies=%lld\nmedia=%s\norientation-requested=%lld\nsides=%s\nprint-color-mode=%s\nmultiple-document-handling=%s\npage-ranges=", settings->printer, (long long)settings->copies, settings->paper, (long long)settings->orientation, hero_ui_print_sides[settings->sides], hero_ui_print_colors[settings->color], hero_ui_print_collations[settings->collate]);

    if (n < 0 || (size_t)n >= room) {
        return -1;
    }
    used = (size_t)n;
    for (int64_t i = 0; i + 1 < settings->range_count; i += 2) {
        n = snprintf(out + used, room - used, "%s%lld-%lld", i == 0 ? "" : ",", (long long)settings->pages[i], (long long)settings->pages[i + 1]);
        if (n < 0 || (size_t)n >= room - used) {
            return -1;
        }
        used += (size_t)n;
    }
    if (used + 2 > room) {
        return -1;
    }
    out[used++] = '\n';
    out[used] = '\0';
    return (int64_t)used;
}

#endif
