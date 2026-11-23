/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_proxy.c — the proxies the system's settings name, one member
 * of the archive the compiler puts on every program's link line
 * (M-rebel-library, 2026-11-23; `selfhost/cli/layer.hero` builds, keys and
 * links it). Its header, `hero_proxy.h`, says what the call answers and the
 * text it writes; this file says how each system gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types`.
 *
 * TWO ARMS AND A REFUSAL. macOS: `SCDynamicStoreCopyProxies`, SystemConfiguration's
 * call for the current internet proxy settings, a dictionary read by the keys
 * `SCSchemaDefinitions.h` names (`kSCPropNetProxies...`). Windows:
 * `WinHttpGetIEProxyConfigForCurrentUser`, which fills a
 * `WINHTTP_CURRENT_USER_IE_PROXY_CONFIG` whose strings the caller frees with
 * `GlobalFree`; the function is found in `winhttp.dll` at the call, loaded
 * from the system folder alone (`LOAD_LIBRARY_SEARCH_SYSTEM32`), so no
 * import library is named and a program that never calls it links none.
 * Every other system: `HERO_PROXY_UNSUPPORTED`.
 *
 * THE LIBRARIES ARE LINKED BY THE MEMBER ITSELF on macOS, as
 * `hero_keyring.c` links its own: the frameworks by a `.linker_option` the
 * object carries.
 *
 * NO STATE AND NO MEMORY OF ITS OWN. The text is written straight into the
 * caller's buffer (a line that does not fit is not written at all, and the
 * answer says `HERO_PROXY_LIMIT`), so the member holds no global, no lock and
 * no allocation.
 *
 * Written from the platforms' documentation: Apple's SystemConfiguration
 * headers (`SCDynamicStoreCopySpecific.h`, `SCSchemaDefinitions.h`) and
 * Microsoft's reference for `WinHttpGetIEProxyConfigForCurrentUser` and
 * `WINHTTP_CURRENT_USER_IE_PROXY_CONFIG`; no other library's source.
 */

#include "hero_proxy.h"

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
#include <SystemConfiguration/SystemConfiguration.h>
/* The frameworks the arm calls, named in the object, so that the linker
 * links them into a program that takes this member and into no other. */
__asm__(".linker_option \"-framework\", \"SystemConfiguration\"");
__asm__(".linker_option \"-framework\", \"CoreFoundation\"");
#endif

/* The most bytes a call writes: the answer's count is 32 bits. */
#define HERO_PROXY_MOST 0x7fffffffu

/* The text being written into the caller's buffer. */
typedef struct {
    char *out;
    uint64_t room;
    uint64_t used;
    /* Where the line being written began, so a line that does not fit leaves no part of itself. */
    uint64_t mark;
    bool full;
} hero_proxy_text;

/* Appends `n` bytes of `s` as they are, or marks the text full and takes the
 * line back where they do not fit. A value's control bytes are made spaces
 * where the value is written (`hero_proxy_string`, `hero_proxy_wide`), never
 * here, since the tab and the line feed that shape a line pass through. */
static void hero_proxy_put(hero_proxy_text *t, const char *s, size_t n) {
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

#if defined(__APPLE__) || defined(_WIN32)

/* Begins a line: nothing of an earlier line is touched. */
static void hero_proxy_begin(hero_proxy_text *t) {
    t->mark = t->used;
}

/* Appends the tab that parts a key from its values and its values from each other. */
static void hero_proxy_tab(hero_proxy_text *t) {
    hero_proxy_put(t, "\t", 1);
}

/* Ends the line begun: its line feed, or the whole line taken back. */
static void hero_proxy_end(hero_proxy_text *t) {
    hero_proxy_put(t, "\n", 1);
}

#endif

#if defined(__APPLE__)

/* Appends a number written in decimal. */
static void hero_proxy_number(hero_proxy_text *t, int64_t n) {
    char digits[24];
    int len = snprintf(digits, sizeof digits, "%lld", (long long)n);

    hero_proxy_put(t, digits, (size_t)len);
}

#endif

#if !defined(__APPLE__)

/* A failure's words, in place of whatever was written. */
static void hero_proxy_words(hero_proxy_text *t, const char *words) {
    t->used = 0;
    t->mark = 0;
    t->full = false;
    hero_proxy_put(t, words, strlen(words));
}

#endif

#if defined(__APPLE__)

/* Whether `key` holds a number other than 0. */
static bool hero_proxy_on(CFDictionaryRef d, CFStringRef key) {
    CFTypeRef v = CFDictionaryGetValue(d, key);
    int32_t n = 0;

    return v != NULL && CFGetTypeID(v) == CFNumberGetTypeID() && CFNumberGetValue((CFNumberRef)v, kCFNumberSInt32Type, &n) && n != 0;
}

/* The number `key` holds, 0 where it holds none. */
static int64_t hero_proxy_count(CFDictionaryRef d, CFStringRef key) {
    CFTypeRef v = CFDictionaryGetValue(d, key);
    int64_t n = 0;

    if (v == NULL || CFGetTypeID(v) != CFNumberGetTypeID() || !CFNumberGetValue((CFNumberRef)v, kCFNumberSInt64Type, &n)) {
        return 0;
    }
    return n;
}

/* Appends `s` as UTF-8, written in the room that is left. */
static void hero_proxy_string(hero_proxy_text *t, CFStringRef s) {
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
    for (CFIndex i = 0; i < written; i++) {
        unsigned char c = (unsigned char)t->out[t->used + (uint64_t)i];

        if (c < 0x20 || c == 0x7f) {
            t->out[t->used + (uint64_t)i] = ' ';
        }
    }
    t->used += (uint64_t)written;
}

/* The line `name`, host and port of a proxy that is enabled. */
static void hero_proxy_server(hero_proxy_text *t, CFDictionaryRef d, const char *name, CFStringRef enable, CFStringRef host, CFStringRef port) {
    CFTypeRef h = CFDictionaryGetValue(d, host);

    if (!hero_proxy_on(d, enable) || h == NULL || CFGetTypeID(h) != CFStringGetTypeID() || CFStringGetLength((CFStringRef)h) == 0) {
        return;
    }
    hero_proxy_begin(t);
    hero_proxy_put(t, name, strlen(name));
    hero_proxy_tab(t);
    hero_proxy_string(t, (CFStringRef)h);
    hero_proxy_tab(t);
    hero_proxy_number(t, hero_proxy_count(d, port));
    hero_proxy_end(t);
}

static int hero_proxy_fill(hero_proxy_text *t) {
    /* No store session of our own: NULL makes a temporary one. NULL comes
     * back where no setting is defined, which is no failure. */
    CFDictionaryRef d = SCDynamicStoreCopyProxies(NULL);

    if (d == NULL) {
        return HERO_PROXY_OK;
    }
    hero_proxy_server(t, d, "http", kSCPropNetProxiesHTTPEnable, kSCPropNetProxiesHTTPProxy, kSCPropNetProxiesHTTPPort);
    hero_proxy_server(t, d, "https", kSCPropNetProxiesHTTPSEnable, kSCPropNetProxiesHTTPSProxy, kSCPropNetProxiesHTTPSPort);
    hero_proxy_server(t, d, "socks", kSCPropNetProxiesSOCKSEnable, kSCPropNetProxiesSOCKSProxy, kSCPropNetProxiesSOCKSPort);

    CFTypeRef list = CFDictionaryGetValue(d, kSCPropNetProxiesExceptionsList);

    if (list != NULL && CFGetTypeID(list) == CFArrayGetTypeID()) {
        CFIndex n = CFArrayGetCount((CFArrayRef)list);

        for (CFIndex i = 0; i < n; i++) {
            CFTypeRef one = CFArrayGetValueAtIndex((CFArrayRef)list, i);

            if (one == NULL || CFGetTypeID(one) != CFStringGetTypeID() || CFStringGetLength((CFStringRef)one) == 0) {
                continue;
            }
            hero_proxy_begin(t);
            hero_proxy_put(t, "exception\t", 10);
            hero_proxy_string(t, (CFStringRef)one);
            hero_proxy_end(t);
        }
    }
    if (hero_proxy_on(d, kSCPropNetProxiesExcludeSimpleHostnames)) {
        hero_proxy_begin(t);
        hero_proxy_put(t, "simple\t1", 8);
        hero_proxy_end(t);
    }
    if (hero_proxy_on(d, kSCPropNetProxiesProxyAutoConfigEnable)) {
        CFTypeRef where = CFDictionaryGetValue(d, kSCPropNetProxiesProxyAutoConfigURLString);

        hero_proxy_begin(t);
        hero_proxy_put(t, "pac\t", 4);
        if (where != NULL && CFGetTypeID(where) == CFStringGetTypeID()) {
            hero_proxy_string(t, (CFStringRef)where);
        }
        hero_proxy_end(t);
    }
    if (hero_proxy_on(d, kSCPropNetProxiesProxyAutoDiscoveryEnable)) {
        hero_proxy_begin(t);
        hero_proxy_put(t, "discover\t1", 10);
        hero_proxy_end(t);
    }
    CFRelease(d);
    return HERO_PROXY_OK;
}

#elif defined(_WIN32)

/* WINHTTP_CURRENT_USER_IE_PROXY_CONFIG, written out so that no header of
 * WinHTTP is needed: its layout is the documented one. */
typedef struct {
    BOOL fAutoDetect;
    LPWSTR lpszAutoConfigUrl;
    LPWSTR lpszProxy;
    LPWSTR lpszProxyBypass;
} hero_proxy_ie;

typedef BOOL(WINAPI *hero_proxy_ie_fn)(hero_proxy_ie *);

/* Appends `w`, UTF-16, as UTF-8 in the room that is left. */
static bool hero_proxy_wide(hero_proxy_text *t, const wchar_t *w) {
    int need = WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);

    if (need == 0) {
        return false;
    }
    if (t->full || need == 1) {
        return true;
    }
    if ((uint64_t)(need - 1) > t->room - t->used) {
        t->full = true;
        t->used = t->mark;
        return true;
    }
    if (WideCharToMultiByte(CP_UTF8, 0, w, -1, t->out + t->used, need, NULL, NULL) == 0) {
        return false;
    }
    for (int i = 0; i < need - 1; i++) {
        unsigned char c = (unsigned char)t->out[t->used + (uint64_t)i];

        if (c < 0x20 || c == 0x7f) {
            t->out[t->used + (uint64_t)i] = ' ';
        }
    }
    t->used += (uint64_t)(need - 1);
    return true;
}

/* The line `key` and `value`, where `value` is there. */
static bool hero_proxy_line(hero_proxy_text *t, const char *key, const wchar_t *value) {
    if (value == NULL || value[0] == L'\0') {
        return true;
    }
    hero_proxy_begin(t);
    hero_proxy_put(t, key, strlen(key));
    hero_proxy_tab(t);
    bool good = hero_proxy_wide(t, value);

    hero_proxy_end(t);
    return good;
}

static int hero_proxy_fill(hero_proxy_text *t) {
    HMODULE winhttp = LoadLibraryExW(L"winhttp.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (winhttp == NULL) {
        hero_proxy_words(t, "winhttp.dll cannot be loaded from the system folder");
        return HERO_PROXY_FAILED;
    }
    FARPROC found = GetProcAddress(winhttp, "WinHttpGetIEProxyConfigForCurrentUser");
    hero_proxy_ie_fn call = NULL;

    memcpy(&call, &found, sizeof call);
    if (call == NULL) {
        FreeLibrary(winhttp);
        hero_proxy_words(t, "winhttp.dll has no WinHttpGetIEProxyConfigForCurrentUser");
        return HERO_PROXY_UNSUPPORTED;
    }
    hero_proxy_ie ie;

    memset(&ie, 0, sizeof ie);
    if (!call(&ie)) {
        DWORD code = GetLastError();

        FreeLibrary(winhttp);
        if (code == ERROR_FILE_NOT_FOUND) {
            /* "No Internet Explorer proxy settings can be found": none are set. */
            return HERO_PROXY_OK;
        }
        char words[96];
        int len = snprintf(words, sizeof words, "WinHttpGetIEProxyConfigForCurrentUser failed with the system error %lu", (unsigned long)code);

        hero_proxy_words(t, len > 0 ? words : "WinHttpGetIEProxyConfigForCurrentUser failed");
        return HERO_PROXY_FAILED;
    }
    bool good = hero_proxy_line(t, "ie-proxy", ie.lpszProxy) && hero_proxy_line(t, "ie-bypass", ie.lpszProxyBypass);

    if (ie.lpszAutoConfigUrl != NULL && ie.lpszAutoConfigUrl[0] != L'\0') {
        good = hero_proxy_line(t, "pac", ie.lpszAutoConfigUrl) && good;
    }
    if (ie.fAutoDetect) {
        hero_proxy_begin(t);
        hero_proxy_put(t, "discover\t1", 10);
        hero_proxy_end(t);
    }
    if (ie.lpszProxy != NULL) {
        GlobalFree(ie.lpszProxy);
    }
    if (ie.lpszProxyBypass != NULL) {
        GlobalFree(ie.lpszProxyBypass);
    }
    if (ie.lpszAutoConfigUrl != NULL) {
        GlobalFree(ie.lpszAutoConfigUrl);
    }
    FreeLibrary(winhttp);
    if (!good) {
        hero_proxy_words(t, "a string of the Internet Options is not text WideCharToMultiByte converts");
        return HERO_PROXY_FAILED;
    }
    return HERO_PROXY_OK;
}

#else

static int hero_proxy_fill(hero_proxy_text *t) {
    hero_proxy_words(t, "this system keeps no proxy settings a program can read; its environment's variables are the program's to read");
    return HERO_PROXY_UNSUPPORTED;
}

#endif

int64_t hero_proxy_settings(void *buffer, uint64_t count) {
    hero_proxy_text t;

    t.out = (char *)buffer;
    t.room = (buffer == NULL) ? 0 : (count > HERO_PROXY_MOST ? HERO_PROXY_MOST : count);
    t.used = 0;
    t.mark = 0;
    t.full = false;
    int kind = hero_proxy_fill(&t);

    if (kind == HERO_PROXY_OK && t.full) {
        kind = HERO_PROXY_LIMIT;
    }
    return ((int64_t)kind << 32) | (int64_t)t.used;
}
