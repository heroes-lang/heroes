/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_proxy.h — the platform layer's reading of the proxies the system's own
 * settings name (M-rebel-library, 2026-11-23; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its function is defined in `platform/hero_proxy.c`, a member of the archive
 * on every program's link line, which the linker takes only into a program
 * that calls it; a group binds it by naming this header,
 * `extern "hero_proxy.h"`, with no `link` word and no flag.
 *
 * WHAT THE SYSTEMS HOLD. macOS: the dynamic store's proxies of the active
 * network service (`SCDynamicStoreCopyProxies`), what System Settings'
 * Network pane writes. Windows: the current user's Internet Options
 * (`WinHttpGetIEProxyConfigForCurrentUser`), what the Settings app's Proxy
 * page writes. Linux and the other systems: no store common to every desktop,
 * so the call answers `HERO_PROXY_UNSUPPORTED`; the environment's variables
 * are the face's to read.
 *
 * ONE CALL, ONE TEXT, NO STATE. The settings are written into the caller's
 * buffer as lines of UTF-8 ended by a line feed, each a key, a tab and its
 * values parted by tabs, a control byte in a value written as a space. The
 * keys are the platform's own, so nothing is decided here:
 *
 *   macOS    `http`, `https`, `socks`: host and port of each that is
 *            enabled (the port 0 where the store has none); `exception`:
 *            one host pattern that bypasses the proxy, one line each;
 *            `simple`: 1 where hosts with no dot bypass it; `pac`: the
 *            address of the automatic configuration script, "" where the
 *            script is held as text; `discover`: 1 where the proxy is
 *            discovered automatically (WPAD).
 *   Windows  `ie-proxy`: the proxy server list as the system writes it;
 *            `ie-bypass`: the bypass list; `pac`, `discover` as above.
 *
 * A line is written only for a setting that is on. A machine with no proxy
 * settings answers an empty text, which is no failure.
 *
 * THE ANSWER IS ONE NUMBER: its high 32 bits are the kind of the result,
 * `HERO_PROXY_OK` and the rest below, its low 32 bits the count of bytes
 * written into the buffer: the settings' text for `HERO_PROXY_OK`, the
 * system's own words for a failure. `HERO_PROXY_LIMIT` says the buffer was
 * too small: the text is cut at its last whole line, and a caller that wants
 * all of it calls again with a larger one.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against this prototype.
 */

#ifndef HERO_PROXY_H
#define HERO_PROXY_H

#include <stdint.h>

#define HERO_PROXY_OK 0
#define HERO_PROXY_UNSUPPORTED 1
#define HERO_PROXY_FAILED 2
#define HERO_PROXY_LIMIT 3

int64_t hero_proxy_settings(void *buffer, uint64_t count);

#endif
