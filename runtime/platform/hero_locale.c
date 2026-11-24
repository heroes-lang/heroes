/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_locale.c — the languages and the formats the system's
 * settings name, one member of the archive the compiler puts on every
 * program's link line (M-rebel-library, 2026-11-23; `selfhost/cli/layer.hero`
 * builds, keys and links it). Its header, `hero_locale.h`, says what the call
 * answers and the text it writes; this file says how each system gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types`.
 *
 * TWO ARMS AND A REFUSAL. macOS: CoreFoundation's
 * `CFLocaleCopyPreferredLanguages`, an array of strings, and
 * `CFLocaleCopyCurrent`, whose identifier `CFLocaleGetIdentifier` names; both
 * read the user's preferences, never the environment. Windows:
 * `GetUserPreferredUILanguages` with `MUI_LANGUAGE_NAME`, a list of UTF-16
 * names each ended by a zero and the list by another, asked once for its
 * size and again for its text, and asked anew where it grew in between; and
 * `GetUserDefaultLocaleName`, at most `LOCALE_NAME_MAX_LENGTH` characters.
 * Both are kernel32's, linked into every program. Every other system:
 * `HERO_LOCALE_UNSUPPORTED`.
 *
 * THE LIBRARIES ARE LINKED BY THE MEMBER ITSELF on macOS, as
 * `hero_proxy.c` links its own: CoreFoundation by a `.linker_option` the
 * object carries.
 *
 * NO STATE OF ITS OWN. The text is written straight into the caller's
 * buffer (a line that does not fit is not written at all, and the answer
 * says `HERO_LOCALE_LIMIT`); the Windows arm's list of languages is the one
 * allocation, freed before the call returns.
 *
 * Written from the platforms' documentation: Apple's `CFLocale.h` and
 * Microsoft's reference for `GetUserPreferredUILanguages`,
 * `GetUserDefaultLocaleName` and `WideCharToMultiByte`; no other library's
 * source.
 */

#include "hero_locale.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
/* The framework the arm calls, named in the object, so that the linker
 * links it into a program that takes this member and into no other. */
__asm__(".linker_option \"-framework\", \"CoreFoundation\"");
#endif

/* The most bytes a call writes: the answer's count is 32 bits. */
#define HERO_LOCALE_MOST 0x7fffffffu

/* The text being written into the caller's buffer. */
typedef struct {
    char *out;
    uint64_t room;
    uint64_t used;
    /* Where the line being written began, so a line that does not fit leaves no part of itself. */
    uint64_t mark;
    bool full;
} hero_locale_text;

/* Appends `n` bytes of `s` as they are, or marks the text full and takes the
 * line back where they do not fit. */
static void hero_locale_put(hero_locale_text *t, const char *s, size_t n) {
    if (t->full) {
        return;
    }
    if (n > t->room - t->used) {
        t->full = true;
        t->used = t->mark;
        return;
    }
    if (n > 0) {
        memcpy(t->out + t->used, s, n);
    }
    t->used += n;
}

#if !defined(__APPLE__)

/* A failure's words, in place of whatever was written. */
static void hero_locale_words(hero_locale_text *t, const char *words) {
    t->used = 0;
    t->mark = 0;
    t->full = false;
    hero_locale_put(t, words, strlen(words));
}

#endif

#if defined(__APPLE__) || defined(_WIN32)

/* Begins a line with its key and the tab after it: nothing of an earlier line
 * is touched. */
static void hero_locale_begin(hero_locale_text *t, const char *key) {
    t->mark = t->used;
    hero_locale_put(t, key, strlen(key));
    hero_locale_put(t, "\t", 1);
}

/* Ends the line begun: its line feed, or the whole line taken back. */
static void hero_locale_end(hero_locale_text *t) {
    hero_locale_put(t, "\n", 1);
}

/* Writes a space for each control byte of the `n` bytes just written, so a
 * name never parts a line or a key from its value. */
static void hero_locale_clean(hero_locale_text *t, uint64_t n) {
    for (uint64_t i = t->used - n; i < t->used; i++) {
        unsigned char c = (unsigned char)t->out[i];

        if (c < 0x20 || c == 0x7f) {
            t->out[i] = ' ';
        }
    }
}

#endif

#if defined(__APPLE__)

/* Appends `s` as UTF-8, written in the room that is left. */
static void hero_locale_string(hero_locale_text *t, CFStringRef s) {
    CFIndex length = CFStringGetLength(s);
    CFIndex written = 0;

    if (t->full || length == 0) {
        return;
    }
    if (t->room == t->used) {
        t->full = true;
        t->used = t->mark;
        return;
    }
    CFIndex done = CFStringGetBytes(s, CFRangeMake(0, length), kCFStringEncodingUTF8, '?', false, (UInt8 *)(t->out + t->used), (CFIndex)(t->room - t->used), &written);

    if (done < length) {
        t->full = true;
        t->used = t->mark;
        return;
    }
    t->used += (uint64_t)written;
    hero_locale_clean(t, (uint64_t)written);
}

/* The line `key` and `value`, where `value` is a string that is not empty. */
static void hero_locale_line(hero_locale_text *t, const char *key, CFTypeRef value) {
    if (value == NULL || CFGetTypeID(value) != CFStringGetTypeID() || CFStringGetLength((CFStringRef)value) == 0) {
        return;
    }
    hero_locale_begin(t, key);
    hero_locale_string(t, (CFStringRef)value);
    hero_locale_end(t);
}

static int hero_locale_fill(hero_locale_text *t) {
    /* Both calls follow the Create rule: the caller releases what they
     * answer. Neither is documented to answer NULL; a NULL writes no line. */
    CFArrayRef languages = CFLocaleCopyPreferredLanguages();

    if (languages != NULL) {
        CFIndex count = CFArrayGetCount(languages);

        for (CFIndex i = 0; i < count; i++) {
            hero_locale_line(t, "language", CFArrayGetValueAtIndex(languages, i));
        }
        CFRelease(languages);
    }

    CFLocaleRef current = CFLocaleCopyCurrent();

    if (current != NULL) {
        hero_locale_line(t, "formats", CFLocaleGetIdentifier(current));
        CFRelease(current);
    }
    return HERO_LOCALE_OK;
}

#elif defined(_WIN32)

/* The most times the list of languages is asked again where it grew between
 * the call that sized it and the call that read it. */
#define HERO_LOCALE_TRIES 4

/* The failure of `call`, the system's error number named. */
static int hero_locale_failed(hero_locale_text *t, const char *call) {
    char words[160];

    snprintf(words, sizeof words, "%s failed, Windows error %lu", call, (unsigned long)GetLastError());
    hero_locale_words(t, words);
    return HERO_LOCALE_FAILED;
}

/* Appends the `n` UTF-16 units of `s` as UTF-8, written in the room that is
 * left; an unpaired surrogate becomes U+FFFD, as `WideCharToMultiByte` writes
 * it without `WC_ERR_INVALID_CHARS`. */
static void hero_locale_wide(hero_locale_text *t, const WCHAR *s, int n) {
    if (t->full || n <= 0) {
        return;
    }
    int need = WideCharToMultiByte(CP_UTF8, 0, s, n, NULL, 0, NULL, NULL);

    if (need <= 0 || (uint64_t)need > t->room - t->used) {
        t->full = true;
        t->used = t->mark;
        return;
    }
    int wrote = WideCharToMultiByte(CP_UTF8, 0, s, n, t->out + t->used, need, NULL, NULL);

    if (wrote != need) {
        t->full = true;
        t->used = t->mark;
        return;
    }
    t->used += (uint64_t)wrote;
    hero_locale_clean(t, (uint64_t)wrote);
}

/* The lines of the preferred languages: 0, or the kind of a failure. */
static int hero_locale_languages(hero_locale_text *t) {
    for (int attempt = 0; attempt < HERO_LOCALE_TRIES; attempt++) {
        ULONG count = 0;
        ULONG size = 0;

        if (!GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, NULL, &size)) {
            return hero_locale_failed(t, "GetUserPreferredUILanguages");
        }
        if (size == 0) {
            return HERO_LOCALE_OK;
        }
        WCHAR *list = (WCHAR *)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)size * sizeof(WCHAR));

        if (list == NULL) {
            hero_locale_words(t, "the list of the preferred languages could not be allocated");
            return HERO_LOCALE_FAILED;
        }
        ULONG room = size;

        if (!GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, list, &room)) {
            DWORD error = GetLastError();

            HeapFree(GetProcessHeap(), 0, list);
            if (error == ERROR_INSUFFICIENT_BUFFER) {
                continue;
            }
            SetLastError(error);
            return hero_locale_failed(t, "GetUserPreferredUILanguages");
        }
        /* `room` now counts the units written, both final zeros included;
         * each name is read up to its zero and never past `room`. */
        ULONG at = 0;

        while (at < room && list[at] != 0) {
            ULONG end = at;

            while (end < room && list[end] != 0) {
                end++;
            }
            hero_locale_begin(t, "language");
            hero_locale_wide(t, list + at, (int)(end - at));
            hero_locale_end(t);
            at = end + 1;
        }
        HeapFree(GetProcessHeap(), 0, list);
        return HERO_LOCALE_OK;
    }
    hero_locale_words(t, "the list of the preferred languages kept growing while it was read");
    return HERO_LOCALE_FAILED;
}

static int hero_locale_fill(hero_locale_text *t) {
    int kind = hero_locale_languages(t);

    if (kind != HERO_LOCALE_OK) {
        return kind;
    }
    WCHAR name[LOCALE_NAME_MAX_LENGTH];
    int n = GetUserDefaultLocaleName(name, LOCALE_NAME_MAX_LENGTH);

    if (n == 0) {
        return hero_locale_failed(t, "GetUserDefaultLocaleName");
    }
    /* `n` counts the final zero. */
    if (n > 1) {
        hero_locale_begin(t, "formats");
        hero_locale_wide(t, name, n - 1);
        hero_locale_end(t);
    }
    return HERO_LOCALE_OK;
}

#else

static int hero_locale_fill(hero_locale_text *t) {
    hero_locale_words(t, "this system keeps its languages and formats in the environment, LANG and LC_*, and no settings of its own");
    return HERO_LOCALE_UNSUPPORTED;
}

#endif

int64_t hero_locale_settings(void *buffer, uint64_t count) {
    hero_locale_text t;

    t.out = (char *)buffer;
    t.room = (buffer == NULL) ? 0 : (count > HERO_LOCALE_MOST ? HERO_LOCALE_MOST : count);
    t.used = 0;
    t.mark = 0;
    t.full = false;
    int kind = hero_locale_fill(&t);

    if (kind == HERO_LOCALE_OK && t.full) {
        kind = HERO_LOCALE_LIMIT;
    }
    return ((int64_t)kind << 32) | (int64_t)t.used;
}
