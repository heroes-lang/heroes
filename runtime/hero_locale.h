/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_locale.h — the platform layer's reading of the languages and the
 * formats the person chose in the system's own settings (M-rebel-library,
 * 2026-11-23; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its function is defined in `platform/hero_locale.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls it; a group binds it by naming this header,
 * `extern "hero_locale.h"`, with no `link` word and no flag.
 *
 * WHAT THE SYSTEMS HOLD. macOS: the preferred languages of System Settings'
 * Language & Region pane (`CFLocaleCopyPreferredLanguages`) and the locale
 * its region and formats make (`CFLocaleCopyCurrent`), which a program
 * started from the Finder reads as one started from a terminal, whatever
 * its environment holds. Windows: the user's preferred languages of the
 * display (`GetUserPreferredUILanguages`) and the regional format of the
 * Settings app (`GetUserDefaultLocaleName`). Linux and the other systems:
 * the session's environment, `LANG` and `LC_*`, is the store the desktop
 * writes, so the call answers `HERO_LOCALE_UNSUPPORTED` and the face reads
 * the environment.
 *
 * ONE CALL, ONE TEXT, NO STATE. The settings are written into the caller's
 * buffer as lines of UTF-8 ended by a line feed, each a key, a tab and the
 * system's own name, a control byte in a name written as a space:
 *
 *   `language`  one preferred language, the most preferred first, one line
 *               each: `it-IT`, `zh-Hans-CN` on macOS, `it-IT` on Windows.
 *   `formats`   the locale numbers, dates and currencies are written in, one
 *               line: ICU's identifier on macOS, `en_US@rg=itzzzz` for
 *               English written with Italy's formats; the locale's name on
 *               Windows, `it-IT`, `de-DE_phoneb` where a sort is chosen.
 *
 * The names are the platform's own, so nothing is decided here; a setting
 * the system does not hold writes no line.
 *
 * THE ANSWER IS ONE NUMBER: its high 32 bits are the kind of the result,
 * `HERO_LOCALE_OK` and the rest below, its low 32 bits the count of bytes
 * written into the buffer: the settings' text for `HERO_LOCALE_OK`, the
 * system's own words for a failure. `HERO_LOCALE_LIMIT` says the buffer was
 * too small: the text is cut at its last whole line, and a caller that wants
 * all of it calls again with a larger one.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against this prototype.
 */

#ifndef HERO_LOCALE_H
#define HERO_LOCALE_H

#include <stdint.h>

#define HERO_LOCALE_OK 0
#define HERO_LOCALE_UNSUPPORTED 1
#define HERO_LOCALE_FAILED 2
#define HERO_LOCALE_LIMIT 3

int64_t hero_locale_settings(void *buffer, uint64_t count);

#endif
