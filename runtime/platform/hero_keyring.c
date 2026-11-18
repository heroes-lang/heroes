/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_keyring.c — secrets kept by the system's own keyring, one
 * member of the archive the compiler puts on every program's link line
 * (M-rebel-library, 2026-11-18; `selfhost/cli/layer.hero` builds, keys and
 * links it). Its header, `hero_keyring.h`, says what each function answers;
 * this file says how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * THREE ARMS. macOS: the Security framework's item calls (`SecItemAdd`,
 * `SecItemCopyMatching`, `SecItemUpdate`, `SecItemDelete`) on generic
 * passwords, none of the keychain calls Apple deprecated. Windows: the
 * Credential Manager (`CredWriteW`, `CredReadW`, `CredDeleteW`,
 * `CredEnumerateW`). Linux: libsecret's password calls, the library opened
 * with `dlopen` at the first call, as `hero_tls.c` opens OpenSSL, so a build
 * needs neither its headers nor the library, and a machine without it answers
 * `HERO_KEYRING_UNSUPPORTED`.
 *
 * THE LIBRARIES ARE LINKED BY THE MEMBER ITSELF, as `hero_tls.c` links
 * its own: on Windows `advapi32` by `#pragma comment(lib)`, on macOS the
 * Security and CoreFoundation frameworks by a `.linker_option` the object
 * carries, and on Linux by nothing. So a program that never calls this member
 * links none of them.
 *
 * NO SECRET IN A MESSAGE, AND AS FEW COPIES AS THE SYSTEM ALLOWS. A secret
 * is handed to the system from the caller's own buffer where the system
 * takes a pointer (macOS, by a `CFData` that does not copy) or through one
 * copy on the stack that is written zero after the call (Windows, whose
 * structure takes no `const`); a secret read is copied once, into the
 * caller's buffer, and the system's copy is written zero where it is ours to
 * free (Windows) or given back to the library that wipes it (libsecret).
 *
 * MEMORY: none of the layer's own. Names are bounded
 * (`HERO_KEYRING_NAME_MOST`), so every buffer here is on the stack.
 *
 * Written from the platforms' documentation: Apple's Security framework
 * headers and reference, the Win32 reference for the Credential Manager,
 * libsecret's reference and headers; no other library's source.
 */

#include "hero_keyring.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincred.h>
#include <wchar.h>
#if defined(_MSC_VER)
/* What only a program that takes this member links. MSVC's word; a MinGW
 * target links it by `-l`. */
#pragma comment(lib, "advapi32.lib")
#endif
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
/* The frameworks the arm calls, named in the object, so that the linker
 * links them into a program that takes this member and into no other. */
__asm__(".linker_option \"-framework\", \"Security\"");
__asm__(".linker_option \"-framework\", \"CoreFoundation\"");
#else
#include <dlfcn.h>
#include <pthread.h>
#endif

/* -- THE LAST FAILURE, PER THREAD ------------------------------------------ */

/* The words kept of a failure, with their zero byte. */
#define HERO_KEYRING_WORDS_MOST 512

static _Thread_local int64_t hero_keyring_kind;
static _Thread_local int64_t hero_keyring_os;
static _Thread_local char hero_keyring_words[HERO_KEYRING_WORDS_MOST];

/* `text` copied into `into`, `most` bytes with the zero, cut where a
 * character ends; answers the length copied. */
static size_t hero_keyring_copy_text(char *into, size_t most, const char *text) {
    size_t length = 0;

    if (most == 0) {
        return 0;
    }
    while (text != NULL && text[length] != '\0' && length < most - 1) {
        length += 1;
    }
    if (text != NULL && text[length] != '\0') {
        /* `text[length]` is the first byte left out: while it continues a
         * character, the character it belongs to is left out whole. */
        while (length > 0 && ((unsigned char)text[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    if (length > 0) {
        memcpy(into, text, length);
    }
    into[length] = '\0';
    return length;
}

/* The failure this call answers: its kind, the platform's number, and words,
 * the system's where it gave some; answers -1, what the call answers. */
static int64_t hero_keyring_said(int64_t kind, int64_t os, const char *words) {
    hero_keyring_kind = kind;
    hero_keyring_os = os;
    (void)hero_keyring_copy_text(hero_keyring_words, sizeof hero_keyring_words, words);
    return -1;
}

/* The words of a failure with the system's own after them: "what: theirs". */
static int64_t hero_keyring_said_with(int64_t kind, int64_t os, const char *what, const char *theirs) {
    char words[HERO_KEYRING_WORDS_MOST];

    if (theirs == NULL || theirs[0] == '\0') {
        return hero_keyring_said(kind, os, what);
    }
    (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
    return hero_keyring_said(kind, os, words);
}

static void hero_keyring_fine(void) {
    hero_keyring_kind = HERO_KEYRING_OK;
    hero_keyring_os = 0;
    hero_keyring_words[0] = '\0';
}

int64_t hero_keyring_last_error(void) {
    return hero_keyring_kind;
}

int64_t hero_keyring_last_os_error(void) {
    return hero_keyring_os;
}

int64_t hero_keyring_last_message(void *buffer, uint64_t count) {
    size_t length = strlen(hero_keyring_words);

    if (buffer == NULL || count == 0) {
        return hero_keyring_said(HERO_KEYRING_INVALID, 0, "no room was given for the message");
    }
    if ((uint64_t)length > count) {
        /* Cut to `count`, where a character ends: `words[length]` is the
         * first byte left out, and while it continues a character, the
         * character it belongs to is left out whole. */
        length = (size_t)count;
        while (length > 0 && ((unsigned char)hero_keyring_words[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    if (length > 0) {
        memcpy(buffer, hero_keyring_words, length);
    }
    return (int64_t)length;
}

/* -- WHAT EVERY ARM CHECKS FIRST ------------------------------------------- */

/* Whether the `length` bytes at `text` are UTF-8: no byte that begins no
 * character, no character cut, no longer form than its shortest, no
 * surrogate, nothing past U+10FFFF. */
static bool hero_keyring_is_utf8(const unsigned char *text, size_t length) {
    size_t at = 0;

    while (at < length) {
        unsigned char c = text[at];
        size_t more;
        uint32_t code;
        uint32_t least;

        if (c < 0x80u) {
            at += 1;
            continue;
        }
        if (c >= 0xC2u && c <= 0xDFu) {
            more = 1;
            code = c & 0x1Fu;
            least = 0x80u;
        } else if (c >= 0xE0u && c <= 0xEFu) {
            more = 2;
            code = c & 0x0Fu;
            least = 0x800u;
        } else if (c >= 0xF0u && c <= 0xF4u) {
            more = 3;
            code = c & 0x07u;
            least = 0x10000u;
        } else {
            return false;
        }
        if (length - at - 1 < more) {
            return false;
        }
        for (size_t i = 1; i <= more; i += 1) {
            if ((text[at + i] & 0xC0u) != 0x80u) {
                return false;
            }
            code = (code << 6) | (text[at + i] & 0x3Fu);
        }
        if (code < least || code > 0x10FFFFu || (code >= 0xD800u && code <= 0xDFFFu)) {
            return false;
        }
        at += more + 1;
    }
    return true;
}

/* The length of `name`, a service or an account, into `*length`, or false
 * with the kind said: NULL, longer than `HERO_KEYRING_NAME_MOST`, empty
 * where `may_be_empty` is false, or no UTF-8. Never reads past the bound. */
static bool hero_keyring_name(const char *name, bool may_be_empty, const char *what, size_t *length) {
    char words[128];
    size_t at = 0;

    if (name == NULL) {
        (void)snprintf(words, sizeof words, "no %s was given", what);
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, words);
        return false;
    }
    while (at <= HERO_KEYRING_NAME_MOST && name[at] != '\0') {
        at += 1;
    }
    if (at > HERO_KEYRING_NAME_MOST) {
        (void)snprintf(words, sizeof words, "the %s is longer than %d bytes", what, HERO_KEYRING_NAME_MOST);
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, words);
        return false;
    }
    if (at == 0 && !may_be_empty) {
        (void)snprintf(words, sizeof words, "the %s is empty", what);
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, words);
        return false;
    }
    if (!hero_keyring_is_utf8((const unsigned char *)name, at)) {
        (void)snprintf(words, sizeof words, "the %s is no UTF-8", what);
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, words);
        return false;
    }
    *length = at;
    return true;
}

/* A buffer of `count` bytes the caller hands for C to fill: NULL is taken
 * only with a count of 0. */
static bool hero_keyring_room(const void *buffer, uint64_t count, const char *what) {
    if (buffer == NULL && count > 0) {
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, what);
        return false;
    }
    return true;
}

/* The bytes of a secret: at most `HERO_KEYRING_SECRET_MOST`, never NULL with
 * a count. */
static bool hero_keyring_secret(const void *secret, uint64_t count) {
    char words[128];

    if (secret == NULL && count > 0) {
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, "the secret has a count and no bytes");
        return false;
    }
    if (count > HERO_KEYRING_SECRET_MOST) {
        (void)snprintf(words, sizeof words, "the secret is longer than %d bytes, the most a keyring holds on every platform", HERO_KEYRING_SECRET_MOST);
        (void)hero_keyring_said(HERO_KEYRING_INVALID, 0, words);
        return false;
    }
    return true;
}

/* Adds one account's `length` bytes and its zero to the listing: written at
 * `*used` where the buffer has room, counted either way. Answers false where
 * the count would pass what an `int64_t` holds, which no store reaches. */
static bool hero_keyring_list_one(void *buffer, uint64_t count, uint64_t *used, const unsigned char *account, size_t length) {
    uint64_t after = *used + (uint64_t)length + 1;

    if (after > (uint64_t)INT64_MAX) {
        return false;
    }
    if (after <= count && buffer != NULL) {
        if (length > 0) {
            memcpy((unsigned char *)buffer + *used, account, length);
        }
        ((unsigned char *)buffer)[*used + length] = 0;
    }
    *used = after;
    return true;
}

/* Whether an account the store gave can be listed: a name this layer could
 * be handed, so no zero byte, UTF-8, within the bound. */
static bool hero_keyring_listable(const unsigned char *account, size_t length) {
    if (length > HERO_KEYRING_NAME_MOST || memchr(account, 0, length) != NULL) {
        return false;
    }
    return hero_keyring_is_utf8(account, length);
}

/* The name of the store, by the fill form. */
static int64_t hero_keyring_named(void *buffer, uint64_t count, const char *name) {
    size_t length = strlen(name);

    if (length <= count) {
        memcpy(buffer, name, length);
    }
    hero_keyring_fine();
    return (int64_t)length;
}

#if defined(__APPLE__)
/* ==========================================================================
 * THE APPLE ARM: the login keychain's generic passwords, `kSecAttrService`
 * and `kSecAttrAccount` the pair. The data protection keychain
 * (`kSecUseDataProtectionKeychain`) is not asked: it needs an entitlement a
 * program built here does not carry, and answers `errSecMissingEntitlement`.
 * ========================================================================== */

/* The kind of a refusal, by the Security framework's own code. */
static int64_t hero_keyring_mac_kind(OSStatus status) {
    switch (status) {
    case errSecItemNotFound:
        return HERO_KEYRING_NOT_FOUND;
    case errSecAuthFailed:
    case errSecUserCanceled:
    case errSecInteractionNotAllowed:
    case errSecNoAccessForItem:
    case errSecReadOnly:
        return HERO_KEYRING_DENIED;
    case errSecNoSuchKeychain:
    case errSecNotAvailable:
    case errSecMissingEntitlement:
    case errSecUnimplemented:
        return HERO_KEYRING_UNSUPPORTED;
    case errSecAllocate:
    case errSecDiskFull:
        return HERO_KEYRING_LIMIT;
    case errSecParam:
        return HERO_KEYRING_INVALID;
    default:
        return HERO_KEYRING_OTHER;
    }
}

/* The failure of a Security call: its kind, its code, and the framework's own
 * words for it after `what`. Answers -1. */
static int64_t hero_keyring_mac_failed(OSStatus status, const char *what) {
    char theirs[HERO_KEYRING_WORDS_MOST];
    CFStringRef words = SecCopyErrorMessageString(status, NULL);

    theirs[0] = '\0';
    if (words != NULL) {
        if (!CFStringGetCString(words, theirs, (CFIndex)sizeof theirs, kCFStringEncodingUTF8)) {
            theirs[0] = '\0';
        }
        CFRelease(words);
    }
    return hero_keyring_said_with(hero_keyring_mac_kind(status), (int64_t)status, what, theirs);
}

/* A name as a `CFString`, or NULL where the system has no memory. */
static CFStringRef hero_keyring_mac_text(const char *name, size_t length) {
    return CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)name, (CFIndex)length, kCFStringEncodingUTF8, false);
}

/* A query of `count` keys and values, or NULL where the system has no
 * memory. */
static CFDictionaryRef hero_keyring_mac_query(const void **keys, const void **values, CFIndex count) {
    return CFDictionaryCreate(kCFAllocatorDefault, keys, values, count, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
}

static void hero_keyring_mac_release(CFTypeRef a, CFTypeRef b, CFTypeRef c) {
    if (a != NULL) {
        CFRelease(a);
    }
    if (b != NULL) {
        CFRelease(b);
    }
    if (c != NULL) {
        CFRelease(c);
    }
}

static int64_t hero_keyring_mac_no_memory(void) {
    return hero_keyring_said(HERO_KEYRING_LIMIT, 0, "the system had no memory for the keychain's query");
}

/* The service and the account as `CFString`s, both or neither. */
static bool hero_keyring_mac_pair(const char *service, size_t service_length, const char *account, size_t account_length, CFStringRef *service_text, CFStringRef *account_text) {
    *service_text = hero_keyring_mac_text(service, service_length);
    *account_text = account == NULL ? NULL : hero_keyring_mac_text(account, account_length);
    if (*service_text == NULL || (account != NULL && *account_text == NULL)) {
        hero_keyring_mac_release(*service_text, *account_text, NULL);
        (void)hero_keyring_mac_no_memory();
        return false;
    }
    return true;
}

int64_t hero_keyring_system(void *buffer, uint64_t count) {
    static const char probe[] = "hero_keyring.system: a service no program names";
    CFStringRef service;
    CFDictionaryRef query;
    CFTypeRef found = NULL;
    OSStatus status;

    if (!hero_keyring_room(buffer, count, "the buffer for the name has a count and no bytes")) {
        return -1;
    }
    service = hero_keyring_mac_text(probe, sizeof probe - 1);
    if (service == NULL) {
        return hero_keyring_mac_no_memory();
    }
    {
        const void *keys[] = { kSecClass, kSecAttrService, kSecReturnAttributes, kSecMatchLimit };
        const void *values[] = { kSecClassGenericPassword, service, kCFBooleanTrue, kSecMatchLimitOne };
        query = hero_keyring_mac_query(keys, values, 4);
    }
    if (query == NULL) {
        hero_keyring_mac_release(service, NULL, NULL);
        return hero_keyring_mac_no_memory();
    }
    status = SecItemCopyMatching(query, &found);
    hero_keyring_mac_release(found, query, service);
    if (status != errSecSuccess && status != errSecItemNotFound) {
        return hero_keyring_mac_failed(status, "the keychain did not answer");
    }
    return hero_keyring_named(buffer, count, "macOS Keychain");
}

int64_t hero_keyring_store(const char *service, const char *account, const void *secret, uint64_t count) {
    static const UInt8 nothing[1] = { 0 };
    size_t service_length = 0;
    size_t account_length = 0;
    CFStringRef service_text;
    CFStringRef account_text;
    CFDataRef data;
    CFDictionaryRef add;
    CFDictionaryRef match;
    CFDictionaryRef change;
    OSStatus status = errSecSuccess;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length) || !hero_keyring_secret(secret, count)) {
        return -1;
    }
    if (!hero_keyring_mac_pair(service, service_length, account, account_length, &service_text, &account_text)) {
        return -1;
    }
    /* The caller's bytes, not copied here: the data lives for this call. */
    data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, count > 0 ? (const UInt8 *)secret : nothing, (CFIndex)count, kCFAllocatorNull);
    {
        const void *add_keys[] = { kSecClass, kSecAttrService, kSecAttrAccount, kSecValueData };
        const void *add_values[] = { kSecClassGenericPassword, service_text, account_text, data };
        const void *match_keys[] = { kSecClass, kSecAttrService, kSecAttrAccount };
        const void *match_values[] = { kSecClassGenericPassword, service_text, account_text };
        const void *change_keys[] = { kSecValueData };
        const void *change_values[] = { data };

        add = data == NULL ? NULL : hero_keyring_mac_query(add_keys, add_values, 4);
        match = hero_keyring_mac_query(match_keys, match_values, 3);
        change = data == NULL ? NULL : hero_keyring_mac_query(change_keys, change_values, 1);
    }
    if (add == NULL || match == NULL || change == NULL) {
        hero_keyring_mac_release(add, match, change);
        hero_keyring_mac_release(data, service_text, account_text);
        return hero_keyring_mac_no_memory();
    }
    /* Added where none stood, else replaced; and where another program
     * removes it between the two, added again. Four rounds at most: a store
     * that loses four races in a row is told, never retried for ever. */
    for (int round = 0; round < 4; round += 1) {
        status = SecItemAdd(add, NULL);
        if (status != errSecDuplicateItem) {
            break;
        }
        status = SecItemUpdate(match, change);
        if (status != errSecItemNotFound) {
            break;
        }
    }
    hero_keyring_mac_release(add, match, change);
    hero_keyring_mac_release(data, service_text, account_text);
    if (status == errSecDuplicateItem || status == errSecItemNotFound) {
        return hero_keyring_said(HERO_KEYRING_OTHER, (int64_t)status, "the secret was added and removed by another program four times while it was stored");
    }
    if (status != errSecSuccess) {
        return hero_keyring_mac_failed(status, "the keychain did not store the secret");
    }
    hero_keyring_fine();
    return 0;
}

int64_t hero_keyring_read(const char *service, const char *account, void *buffer, uint64_t count) {
    size_t service_length = 0;
    size_t account_length = 0;
    CFStringRef service_text;
    CFStringRef account_text;
    CFDictionaryRef query;
    CFTypeRef found = NULL;
    OSStatus status;
    CFIndex length;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length) || !hero_keyring_room(buffer, count, "the buffer for the secret has a count and no bytes")) {
        return -1;
    }
    if (!hero_keyring_mac_pair(service, service_length, account, account_length, &service_text, &account_text)) {
        return -1;
    }
    {
        const void *keys[] = { kSecClass, kSecAttrService, kSecAttrAccount, kSecReturnData, kSecMatchLimit };
        const void *values[] = { kSecClassGenericPassword, service_text, account_text, kCFBooleanTrue, kSecMatchLimitOne };
        query = hero_keyring_mac_query(keys, values, 5);
    }
    if (query == NULL) {
        hero_keyring_mac_release(service_text, account_text, NULL);
        return hero_keyring_mac_no_memory();
    }
    status = SecItemCopyMatching(query, &found);
    hero_keyring_mac_release(query, service_text, account_text);
    if (status != errSecSuccess) {
        hero_keyring_mac_release(found, NULL, NULL);
        return hero_keyring_mac_failed(status, "the keychain did not read the secret");
    }
    if (found == NULL || CFGetTypeID(found) != CFDataGetTypeID()) {
        hero_keyring_mac_release(found, NULL, NULL);
        return hero_keyring_said(HERO_KEYRING_OTHER, 0, "the keychain answered something other than the secret's bytes");
    }
    length = CFDataGetLength((CFDataRef)found);
    if (length >= 0 && (uint64_t)length <= count && length > 0) {
        memcpy(buffer, CFDataGetBytePtr((CFDataRef)found), (size_t)length);
    }
    CFRelease(found);
    hero_keyring_fine();
    return (int64_t)length;
}

/* The most items one removal takes from the keychain after its first. */
#define HERO_KEYRING_DELETE_ROUNDS 65536

/* `SecItemDelete` over the service and, where `account` is not NULL, the
 * account; `all` takes "nothing stood" as done. */
static int64_t hero_keyring_mac_delete(const char *service, size_t service_length, const char *account, size_t account_length, bool all) {
    CFStringRef service_text;
    CFStringRef account_text;
    CFDictionaryRef query;
    OSStatus status;

    if (!hero_keyring_mac_pair(service, service_length, account, account_length, &service_text, &account_text)) {
        return -1;
    }
    {
        const void *keys[] = { kSecClass, kSecAttrService, kSecAttrAccount };
        const void *values[] = { kSecClassGenericPassword, service_text, account_text };
        query = hero_keyring_mac_query(keys, values, account_text == NULL ? 2 : 3);
    }
    if (query == NULL) {
        hero_keyring_mac_release(service_text, account_text, NULL);
        return hero_keyring_mac_no_memory();
    }
    /* One call removes one item of the login keychain, measured on macOS
     * 27.2 with two accounts of one service: so the call is made again until
     * none matches. Every round that does not end the loop removed an item,
     * and a bound stops a store that would answer success for ever. */
    status = SecItemDelete(query);
    if (status == errSecItemNotFound && all) {
        status = errSecSuccess;
    } else if (status == errSecSuccess) {
        OSStatus more = errSecSuccess;

        for (int round = 0; round < HERO_KEYRING_DELETE_ROUNDS && more == errSecSuccess; round += 1) {
            more = SecItemDelete(query);
        }
        if (more == errSecSuccess) {
            hero_keyring_mac_release(query, service_text, account_text);
            return hero_keyring_said(HERO_KEYRING_OTHER, 0, "the keychain still held items of the service after 65,536 removals");
        }
        if (more != errSecItemNotFound) {
            status = more;
        }
    }
    hero_keyring_mac_release(query, service_text, account_text);
    if (status != errSecSuccess) {
        return hero_keyring_mac_failed(status, "the keychain did not remove the secret");
    }
    hero_keyring_fine();
    return 0;
}

int64_t hero_keyring_remove(const char *service, const char *account) {
    size_t service_length = 0;
    size_t account_length = 0;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length)) {
        return -1;
    }
    return hero_keyring_mac_delete(service, service_length, account, account_length, false);
}

int64_t hero_keyring_remove_all(const char *service) {
    size_t service_length = 0;

    if (!hero_keyring_name(service, false, "service", &service_length)) {
        return -1;
    }
    return hero_keyring_mac_delete(service, service_length, NULL, 0, true);
}

/* One item's account into `into`, at most `HERO_KEYRING_NAME_MOST` bytes;
 * its length, or -1 where it holds none this layer can name. */
static int64_t hero_keyring_mac_account(CFTypeRef item, unsigned char *into) {
    CFTypeRef account;
    CFIndex used = 0;
    CFIndex length;

    if (item == NULL || CFGetTypeID(item) != CFDictionaryGetTypeID()) {
        return -1;
    }
    account = CFDictionaryGetValue((CFDictionaryRef)item, kSecAttrAccount);
    if (account == NULL) {
        return -1;
    }
    if (CFGetTypeID(account) == CFStringGetTypeID()) {
        length = CFStringGetLength((CFStringRef)account);
        if (CFStringGetBytes((CFStringRef)account, CFRangeMake(0, length), kCFStringEncodingUTF8, 0, false, into, HERO_KEYRING_NAME_MOST, &used) != length) {
            return -1;
        }
    } else if (CFGetTypeID(account) == CFDataGetTypeID()) {
        used = CFDataGetLength((CFDataRef)account);
        if (used < 0 || used > HERO_KEYRING_NAME_MOST) {
            return -1;
        }
        if (used > 0) {
            memcpy(into, CFDataGetBytePtr((CFDataRef)account), (size_t)used);
        }
    } else {
        return -1;
    }
    return hero_keyring_listable(into, (size_t)used) ? (int64_t)used : -1;
}

int64_t hero_keyring_accounts(const char *service, void *buffer, uint64_t count) {
    unsigned char one[HERO_KEYRING_NAME_MOST];
    size_t service_length = 0;
    CFStringRef service_text;
    CFDictionaryRef query;
    CFTypeRef found = NULL;
    OSStatus status;
    uint64_t used = 0;
    CFIndex items;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_room(buffer, count, "the buffer for the accounts has a count and no bytes")) {
        return -1;
    }
    service_text = hero_keyring_mac_text(service, service_length);
    if (service_text == NULL) {
        return hero_keyring_mac_no_memory();
    }
    {
        const void *keys[] = { kSecClass, kSecAttrService, kSecReturnAttributes, kSecMatchLimit };
        const void *values[] = { kSecClassGenericPassword, service_text, kCFBooleanTrue, kSecMatchLimitAll };
        query = hero_keyring_mac_query(keys, values, 4);
    }
    if (query == NULL) {
        hero_keyring_mac_release(service_text, NULL, NULL);
        return hero_keyring_mac_no_memory();
    }
    status = SecItemCopyMatching(query, &found);
    hero_keyring_mac_release(query, service_text, NULL);
    if (status == errSecItemNotFound) {
        hero_keyring_mac_release(found, NULL, NULL);
        hero_keyring_fine();
        return 0;
    }
    if (status != errSecSuccess) {
        hero_keyring_mac_release(found, NULL, NULL);
        return hero_keyring_mac_failed(status, "the keychain did not list the accounts");
    }
    if (found == NULL || CFGetTypeID(found) != CFArrayGetTypeID()) {
        hero_keyring_mac_release(found, NULL, NULL);
        return hero_keyring_said(HERO_KEYRING_OTHER, 0, "the keychain answered something other than a list of items");
    }
    items = CFArrayGetCount((CFArrayRef)found);
    for (CFIndex i = 0; i < items; i += 1) {
        int64_t length = hero_keyring_mac_account(CFArrayGetValueAtIndex((CFArrayRef)found, i), one);

        if (length >= 0 && !hero_keyring_list_one(buffer, count, &used, one, (size_t)length)) {
            CFRelease(found);
            return hero_keyring_said(HERO_KEYRING_LIMIT, 0, "the accounts take more bytes than a count holds");
        }
    }
    CFRelease(found);
    hero_keyring_fine();
    return (int64_t)used;
}

#elif defined(_WIN32)
/* ==========================================================================
 * THE WINDOWS ARM: the Credential Manager's generic credentials.
 *
 * ONE TARGET NAME PER PAIR, AND NO TWO PAIRS ON ONE. The Credential Manager
 * keys a credential by its target name alone and compares target names
 * without their case, so the pair is written into one ASCII name, `heroes-
 * keyring:` then the service then `:` then the account, each byte that is
 * not a lowercase ASCII letter, a digit, `.`, `_` or `-` written `%XX` in
 * uppercase hexadecimal. A capital letter is so written as its byte, so
 * `App` and `app` are two names whatever the comparison does; a `:` inside
 * a name is `%3A`, so the first `:` after the prefix ends the service; and
 * no `*` is ever written, so a service's own listing, the name up to its `:`
 * then `*`, matches no other service. The name is the layer's own: a
 * credential another program writes is never read here.
 * ========================================================================== */

#define HERO_KEYRING_PREFIX "heroes-keyring:"
/* The prefix, a service and an account at three characters a byte, the `:`
 * between them, a listing's `*` and the zero. */
#define HERO_KEYRING_TARGET_MOST (sizeof HERO_KEYRING_PREFIX + 6 * HERO_KEYRING_NAME_MOST + 4)

static bool hero_keyring_win_kept(unsigned char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
}

/* The `length` bytes of `name` written into `into` at `at`, encoded; answers
 * where the writing ends. The caller's buffer holds `HERO_KEYRING_TARGET_MOST`. */
static size_t hero_keyring_win_encode(wchar_t *into, size_t at, const char *name, size_t length) {
    static const char hex[] = "0123456789ABCDEF";

    for (size_t i = 0; i < length; i += 1) {
        unsigned char c = (unsigned char)name[i];

        if (hero_keyring_win_kept(c)) {
            into[at] = (wchar_t)c;
            at += 1;
        } else {
            into[at] = L'%';
            into[at + 1] = (wchar_t)hex[c >> 4];
            into[at + 2] = (wchar_t)hex[c & 0x0Fu];
            at += 3;
        }
    }
    return at;
}

/* The target name of `service` and, where `account` is not NULL, of the
 * pair; with `account` NULL, the listing's filter, the service then `:*`.
 * Answers the length of the part that names the service and its `:`. */
static size_t hero_keyring_win_target(wchar_t *into, const char *service, size_t service_length, const char *account, size_t account_length) {
    size_t at = 0;
    size_t head;

    for (size_t i = 0; i < sizeof HERO_KEYRING_PREFIX - 1; i += 1) {
        into[at] = (wchar_t)HERO_KEYRING_PREFIX[i];
        at += 1;
    }
    at = hero_keyring_win_encode(into, at, service, service_length);
    into[at] = L':';
    at += 1;
    head = at;
    if (account == NULL) {
        into[at] = L'*';
        at += 1;
    } else {
        at = hero_keyring_win_encode(into, at, account, account_length);
    }
    into[at] = L'\0';
    return head;
}

static int hero_keyring_win_hex(wchar_t c) {
    if (c >= L'0' && c <= L'9') {
        return (int)(c - L'0');
    }
    if (c >= L'A' && c <= L'F') {
        return (int)(c - L'A') + 10;
    }
    return -1;
}

/* The account a target name holds after its first `head` characters, which
 * are the service's, decoded into `into`; its length, or -1 where the name
 * is no target this layer writes. */
static int64_t hero_keyring_win_decode(const wchar_t *target, const wchar_t *head, size_t head_length, unsigned char *into) {
    size_t at = head_length;
    size_t length = 0;

    if (target == NULL || wcsncmp(target, head, head_length) != 0) {
        return -1;
    }
    while (target[at] != L'\0') {
        if (length >= HERO_KEYRING_NAME_MOST) {
            return -1;
        }
        if (target[at] == L'%') {
            int high = hero_keyring_win_hex(target[at + 1]);
            int low = high < 0 ? -1 : hero_keyring_win_hex(target[at + 2]);

            if (low < 0) {
                return -1;
            }
            into[length] = (unsigned char)(high * 16 + low);
            at += 3;
        } else if (target[at] < 0x80 && hero_keyring_win_kept((unsigned char)target[at])) {
            into[length] = (unsigned char)target[at];
            at += 1;
        } else {
            return -1;
        }
        length += 1;
    }
    return hero_keyring_listable(into, length) ? (int64_t)length : -1;
}

/* The kind of a refusal, by the system's error number. */
static int64_t hero_keyring_win_kind(DWORD error) {
    switch (error) {
    case ERROR_NOT_FOUND:
        return HERO_KEYRING_NOT_FOUND;
    case ERROR_NO_SUCH_LOGON_SESSION:
        return HERO_KEYRING_UNSUPPORTED;
    case ERROR_ACCESS_DENIED:
        return HERO_KEYRING_DENIED;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
        return HERO_KEYRING_LIMIT;
    case ERROR_INVALID_PARAMETER:
    case ERROR_INVALID_FLAGS:
    case ERROR_BAD_USERNAME:
        return HERO_KEYRING_INVALID;
    default:
        return HERO_KEYRING_OTHER;
    }
}

/* The failure of a Credential Manager call: its kind, its number, and the
 * system's words for it after `what`. Answers -1. */
static int64_t hero_keyring_win_failed(DWORD error, const char *what) {
    wchar_t wide[256];
    char theirs[HERO_KEYRING_WORDS_MOST];
    DWORD written = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, wide, (DWORD)(sizeof wide / sizeof wide[0]), NULL);
    int bytes = 0;

    while (written > 0 && (wide[written - 1] == L'\r' || wide[written - 1] == L'\n' || wide[written - 1] == L' ' || wide[written - 1] == L'.')) {
        written -= 1;
    }
    if (written > 0) {
        bytes = WideCharToMultiByte(CP_UTF8, 0, wide, (int)written, theirs, (int)sizeof theirs - 1, NULL, NULL);
    }
    theirs[bytes > 0 ? bytes : 0] = '\0';
    if (error == ERROR_NO_SUCH_LOGON_SESSION && bytes <= 0) {
        (void)hero_keyring_copy_text(theirs, sizeof theirs, "no logon session holds credentials here (a service or a network logon)");
    }
    return hero_keyring_said_with(hero_keyring_win_kind(error), (int64_t)error, what, theirs);
}

int64_t hero_keyring_system(void *buffer, uint64_t count) {
    static const char probe[] = "hero_keyring.system: a service no program names";
    wchar_t filter[HERO_KEYRING_TARGET_MOST];
    PCREDENTIALW *found = NULL;
    DWORD items = 0;

    if (!hero_keyring_room(buffer, count, "the buffer for the name has a count and no bytes")) {
        return -1;
    }
    (void)hero_keyring_win_target(filter, probe, sizeof probe - 1, NULL, 0);
    if (!CredEnumerateW(filter, 0, &items, &found)) {
        DWORD error = GetLastError();

        if (error != ERROR_NOT_FOUND) {
            return hero_keyring_win_failed(error, "the Credential Manager did not answer");
        }
    } else {
        CredFree(found);
    }
    return hero_keyring_named(buffer, count, "Windows Credential Manager");
}

int64_t hero_keyring_store(const char *service, const char *account, const void *secret, uint64_t count) {
    wchar_t target[HERO_KEYRING_TARGET_MOST];
    BYTE blob[HERO_KEYRING_SECRET_MOST];
    size_t service_length = 0;
    size_t account_length = 0;
    CREDENTIALW credential;
    BOOL written;
    DWORD error;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length) || !hero_keyring_secret(secret, count)) {
        return -1;
    }
    (void)hero_keyring_win_target(target, service, service_length, account, account_length);
    /* The structure's blob is no `const`, so the secret is copied once, here,
     * and that copy written zero after the call. */
    if (count > 0) {
        memcpy(blob, secret, (size_t)count);
    }
    memset(&credential, 0, sizeof credential);
    credential.Type = CRED_TYPE_GENERIC;
    credential.TargetName = target;
    credential.CredentialBlobSize = (DWORD)count;
    credential.CredentialBlob = count > 0 ? blob : NULL;
    credential.Persist = CRED_PERSIST_LOCAL_MACHINE;
    written = CredWriteW(&credential, 0);
    error = written ? ERROR_SUCCESS : GetLastError();
    SecureZeroMemory(blob, sizeof blob);
    if (!written) {
        return hero_keyring_win_failed(error, "the Credential Manager did not store the secret");
    }
    hero_keyring_fine();
    return 0;
}

int64_t hero_keyring_read(const char *service, const char *account, void *buffer, uint64_t count) {
    wchar_t target[HERO_KEYRING_TARGET_MOST];
    size_t service_length = 0;
    size_t account_length = 0;
    PCREDENTIALW credential = NULL;
    DWORD length;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length) || !hero_keyring_room(buffer, count, "the buffer for the secret has a count and no bytes")) {
        return -1;
    }
    (void)hero_keyring_win_target(target, service, service_length, account, account_length);
    if (!CredReadW(target, CRED_TYPE_GENERIC, 0, &credential)) {
        return hero_keyring_win_failed(GetLastError(), "the Credential Manager did not read the secret");
    }
    length = credential->CredentialBlobSize;
    if (length > 0 && credential->CredentialBlob != NULL) {
        if ((uint64_t)length <= count) {
            memcpy(buffer, credential->CredentialBlob, length);
        }
        SecureZeroMemory(credential->CredentialBlob, length);
    }
    CredFree(credential);
    hero_keyring_fine();
    return (int64_t)length;
}

int64_t hero_keyring_remove(const char *service, const char *account) {
    wchar_t target[HERO_KEYRING_TARGET_MOST];
    size_t service_length = 0;
    size_t account_length = 0;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length)) {
        return -1;
    }
    (void)hero_keyring_win_target(target, service, service_length, account, account_length);
    if (!CredDeleteW(target, CRED_TYPE_GENERIC, 0)) {
        return hero_keyring_win_failed(GetLastError(), "the Credential Manager did not remove the secret");
    }
    hero_keyring_fine();
    return 0;
}

/* The credentials of `service` this layer wrote, by its filter: `*found` and
 * `*items` to give back with `CredFree`, and the head the accounts follow.
 * Answers 0, also where none stands (`*items` 0), or -1. */
static int64_t hero_keyring_win_listed(const char *service, size_t service_length, wchar_t *filter, size_t *head, PCREDENTIALW **found, DWORD *items) {
    *head = hero_keyring_win_target(filter, service, service_length, NULL, 0);
    *found = NULL;
    *items = 0;
    if (!CredEnumerateW(filter, 0, items, found)) {
        DWORD error = GetLastError();

        *found = NULL;
        *items = 0;
        if (error != ERROR_NOT_FOUND) {
            return hero_keyring_win_failed(error, "the Credential Manager did not list the accounts");
        }
    }
    return 0;
}

int64_t hero_keyring_remove_all(const char *service) {
    wchar_t filter[HERO_KEYRING_TARGET_MOST];
    unsigned char one[HERO_KEYRING_NAME_MOST];
    size_t service_length = 0;
    size_t head = 0;
    PCREDENTIALW *found = NULL;
    DWORD items = 0;
    DWORD first = ERROR_SUCCESS;

    if (!hero_keyring_name(service, false, "service", &service_length)) {
        return -1;
    }
    if (hero_keyring_win_listed(service, service_length, filter, &head, &found, &items) < 0) {
        return -1;
    }
    for (DWORD i = 0; i < items; i += 1) {
        if (found[i] == NULL || hero_keyring_win_decode(found[i]->TargetName, filter, head, one) < 0) {
            continue;
        }
        if (!CredDeleteW(found[i]->TargetName, CRED_TYPE_GENERIC, 0)) {
            DWORD error = GetLastError();

            if (error != ERROR_NOT_FOUND && first == ERROR_SUCCESS) {
                first = error;
            }
        }
    }
    if (found != NULL) {
        CredFree(found);
    }
    if (first != ERROR_SUCCESS) {
        return hero_keyring_win_failed(first, "the Credential Manager did not remove every secret of the service");
    }
    hero_keyring_fine();
    return 0;
}

int64_t hero_keyring_accounts(const char *service, void *buffer, uint64_t count) {
    wchar_t filter[HERO_KEYRING_TARGET_MOST];
    unsigned char one[HERO_KEYRING_NAME_MOST];
    size_t service_length = 0;
    size_t head = 0;
    PCREDENTIALW *found = NULL;
    DWORD items = 0;
    uint64_t used = 0;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_room(buffer, count, "the buffer for the accounts has a count and no bytes")) {
        return -1;
    }
    if (hero_keyring_win_listed(service, service_length, filter, &head, &found, &items) < 0) {
        return -1;
    }
    for (DWORD i = 0; i < items; i += 1) {
        int64_t length = found[i] == NULL ? -1 : hero_keyring_win_decode(found[i]->TargetName, filter, head, one);

        if (length >= 0 && !hero_keyring_list_one(buffer, count, &used, one, (size_t)length)) {
            CredFree(found);
            return hero_keyring_said(HERO_KEYRING_LIMIT, 0, "the accounts take more bytes than a count holds");
        }
    }
    if (found != NULL) {
        CredFree(found);
    }
    hero_keyring_fine();
    return (int64_t)used;
}

#else
/* ==========================================================================
 * THE LINUX ARM: the Secret Service, through libsecret and GIO opened with
 * `dlopen`.
 *
 * WHY `dlopen`, as `hero_tls.c` says of OpenSSL: the layer's archive is
 * compiled for every program a machine builds, so a member that included
 * `<libsecret/secret.h>` would stop every build on a machine without its
 * headers, and one linked `-lsecret-1` every program on one without the
 * library. So the member names no header of libsecret or GLib: it declares
 * the functions it calls with their own signatures (libsecret's ABI is kept
 * under the one `libsecret-1.so.0`, GLib's under `libglib-2.0.so.0` and
 * `libgio-2.0.so.0`, which libsecret needs and so loads) and the two
 * structures it reads, `GError` and `SecretSchema`, as their headers lay them
 * out; the first call opens the library once, `RTLD_LOCAL`, and finds every
 * function by `dlsym` on its handle, which searches the library and the
 * libraries it needs. One missing is the library missing: then every call
 * answers `HERO_KEYRING_UNSUPPORTED`. The library is never closed.
 *
 * WHAT IS ASKED OF THE SESSION BUS ITSELF, never guessed from an error: first
 * whether a bus answers at all, `g_bus_get_sync`. With none every call is
 * `UNSUPPORTED`; measured 2026-11-18 in a container, the three ways a bus is
 * missing are three errors of three domains (no machine-id, `g-file-error`
 * 4; no `$DISPLAY` to start one, `g-io-error` 0; an address naming nothing,
 * `g-io-error` 1), so no code could have said it. Then, in
 * `hero_keyring_system`, whether the Secret Service has a default
 * collection, `ReadAlias("default")`: a gnome-keyring started with no login
 * keyring and no display to ask one of answered every search and refused
 * every store. And the accounts of a service, `SearchItems` then each item's
 * `Attributes`, the Secret Service's own methods: libsecret's
 * `secret_password_search_sync`, called as its documentation says, left 24
 * bytes per item found to LeakSanitizer (libsecret 0.21.7, measured
 * 2026-11-18, by a program linked against it), so the listing makes no item
 * object. A secret is stored, read and removed by libsecret's password
 * calls, which left nothing there.
 *
 * THE SCHEMA is `org.freedesktop.Secret.Generic` with two text attributes,
 * `service` and `username`, matched without its name
 * (`SECRET_SCHEMA_DONT_MATCH_NAME`), so an item another program stored with
 * those two attributes is the same item here.
 *
 * ONE CALL AT A TIME: every call below holds `hero_keyring_lock`, so the
 * library's synchronous calls, each running a main loop of its own, never
 * run side by side from two threads of the program.
 * ========================================================================== */

/* GLib's `GError`: a domain, a code and the words. */
typedef struct hero_keyring_gerror {
    uint32_t domain;
    int code;
    char *message;
} hero_keyring_gerror;

/* libsecret's `SecretSchemaAttribute` and `SecretSchema`, field for field. */
typedef struct {
    const char *name;
    int type;
} hero_keyring_attribute;

typedef struct {
    const char *name;
    int flags;
    hero_keyring_attribute attributes[32];
    int reserved;
    void *reserved1;
    void *reserved2;
    void *reserved3;
    void *reserved4;
    void *reserved5;
    void *reserved6;
    void *reserved7;
} hero_keyring_schema;

/* libsecret's numbers this arm reads, as its headers name them. */
#define HERO_SECRET_SCHEMA_DONT_MATCH_NAME 2
#define HERO_SECRET_SCHEMA_ATTRIBUTE_STRING 0
#define HERO_SECRET_ERROR_IS_LOCKED 2
/* GIO's numbers, each read from `gio/gioenums.h` by a program compiled
 * against it in the container, 2026-11-18. */
#define HERO_G_BUS_TYPE_SESSION 2
#define HERO_G_DBUS_CALL_FLAGS_NONE 0
#define HERO_G_DBUS_ERROR_SERVICE_UNKNOWN 2
#define HERO_G_DBUS_ERROR_NAME_HAS_NO_OWNER 3
#define HERO_G_DBUS_ERROR_ACCESS_DENIED 9
#define HERO_G_DBUS_ERROR_AUTH_FAILED 10
#define HERO_G_DBUS_ERROR_NO_SERVER 11
#define HERO_G_DBUS_ERROR_UNKNOWN_METHOD 19
#define HERO_G_DBUS_ERROR_UNKNOWN_OBJECT 41
#define HERO_G_IO_ERROR_PERMISSION_DENIED 14
#define HERO_G_IO_ERROR_CANCELLED 19

/* The Secret Service's names on the bus (freedesktop.org's Secret Service
 * API). */
#define HERO_SECRETS_NAME "org.freedesktop.secrets"
#define HERO_SECRETS_PATH "/org/freedesktop/secrets"
#define HERO_SECRETS_SERVICE "org.freedesktop.Secret.Service"
#define HERO_SECRETS_ITEM "org.freedesktop.Secret.Item"
/* How long one call on the bus may wait, in milliseconds: -1 is GIO's own
 * default, 25 seconds. */
#define HERO_SECRETS_WAIT (-1)

static const hero_keyring_schema hero_keyring_generic = {
    "org.freedesktop.Secret.Generic",
    HERO_SECRET_SCHEMA_DONT_MATCH_NAME,
    { { "service", HERO_SECRET_SCHEMA_ATTRIBUTE_STRING }, { "username", HERO_SECRET_SCHEMA_ATTRIBUTE_STRING } },
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

/* The functions, found once by `hero_keyring_load` under `hero_keyring_once`,
 * and read only after it. */
typedef struct {
    void *(*value_new)(const char *secret, ptrdiff_t length, const char *content_type);
    const char *(*value_get)(void *value, size_t *length);
    void (*value_unref)(void *value);
    int (*store_binary_sync)(const hero_keyring_schema *schema, const char *collection, const char *label, void *value, void *cancellable, hero_keyring_gerror **error, ...);
    void *(*lookup_binary_sync)(const hero_keyring_schema *schema, void *cancellable, hero_keyring_gerror **error, ...);
    int (*clear_sync)(const hero_keyring_schema *schema, void *cancellable, hero_keyring_gerror **error, ...);
    uint32_t (*secret_error_quark)(void);
    uint32_t (*dbus_error_quark)(void);
    uint32_t (*io_error_quark)(void);
    void *(*bus_get_sync)(int bus_type, void *cancellable, hero_keyring_gerror **error);
    void *(*call_sync)(void *connection, const char *bus_name, const char *object_path, const char *interface_name, const char *method_name, void *parameters, const char *reply_type, int flags, int timeout_msec, void *cancellable, hero_keyring_gerror **error);
    void *(*variant_new_parsed)(const char *format, ...);
    void (*variant_get)(void *value, const char *format, ...);
    int (*variant_lookup)(void *dictionary, const char *key, const char *format, ...);
    void (*variant_unref)(void *value);
    void (*strfreev)(char **strings);
    void (*free)(void *memory);
    void (*object_unref)(void *object);
    void (*error_free)(hero_keyring_gerror *error);
} hero_keyring_libsecret;

static hero_keyring_libsecret hero_keyring_functions;
static bool hero_keyring_found;
static pthread_once_t hero_keyring_once = PTHREAD_ONCE_INIT;
/* The lock every call of this arm holds (ONE CALL AT A TIME, above). */
static pthread_mutex_t hero_keyring_lock = PTHREAD_MUTEX_INITIALIZER;

/* One function by name into `*slot`; false where the library has none. C11
 * has no conversion between an object pointer and a function pointer, so the
 * address is copied by its bytes, which POSIX makes the same for both
 * (`dlsym`'s own page). */
static bool hero_keyring_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_KEYRING_FIND(field, name) ok = ok && hero_keyring_find(library, name, &hero_keyring_functions.field, sizeof hero_keyring_functions.field)

static void hero_keyring_load(void) {
    bool ok = true;
    void *library = dlopen("libsecret-1.so.0", RTLD_NOW | RTLD_LOCAL);

    if (library == NULL) {
        return;
    }
    HERO_KEYRING_FIND(value_new, "secret_value_new");
    HERO_KEYRING_FIND(value_get, "secret_value_get");
    HERO_KEYRING_FIND(value_unref, "secret_value_unref");
    HERO_KEYRING_FIND(store_binary_sync, "secret_password_store_binary_sync");
    HERO_KEYRING_FIND(lookup_binary_sync, "secret_password_lookup_binary_sync");
    HERO_KEYRING_FIND(clear_sync, "secret_password_clear_sync");
    HERO_KEYRING_FIND(secret_error_quark, "secret_error_get_quark");
    HERO_KEYRING_FIND(dbus_error_quark, "g_dbus_error_quark");
    HERO_KEYRING_FIND(io_error_quark, "g_io_error_quark");
    HERO_KEYRING_FIND(bus_get_sync, "g_bus_get_sync");
    HERO_KEYRING_FIND(call_sync, "g_dbus_connection_call_sync");
    HERO_KEYRING_FIND(variant_new_parsed, "g_variant_new_parsed");
    HERO_KEYRING_FIND(variant_get, "g_variant_get");
    HERO_KEYRING_FIND(variant_lookup, "g_variant_lookup");
    HERO_KEYRING_FIND(variant_unref, "g_variant_unref");
    HERO_KEYRING_FIND(strfreev, "g_strfreev");
    HERO_KEYRING_FIND(free, "g_free");
    HERO_KEYRING_FIND(object_unref, "g_object_unref");
    HERO_KEYRING_FIND(error_free, "g_error_free");
    hero_keyring_found = ok;
}

/* The kind of a failure of libsecret or of the bus, by its domain and code. */
static int64_t hero_keyring_linux_kind(const hero_keyring_gerror *error) {
    const hero_keyring_libsecret *f = &hero_keyring_functions;

    if (error->domain == f->secret_error_quark()) {
        return error->code == HERO_SECRET_ERROR_IS_LOCKED ? HERO_KEYRING_DENIED : HERO_KEYRING_OTHER;
    }
    if (error->domain == f->dbus_error_quark()) {
        switch (error->code) {
        case HERO_G_DBUS_ERROR_SERVICE_UNKNOWN:
        case HERO_G_DBUS_ERROR_NAME_HAS_NO_OWNER:
        case HERO_G_DBUS_ERROR_NO_SERVER:
            return HERO_KEYRING_UNSUPPORTED;
        case HERO_G_DBUS_ERROR_ACCESS_DENIED:
        case HERO_G_DBUS_ERROR_AUTH_FAILED:
            return HERO_KEYRING_DENIED;
        default:
            return HERO_KEYRING_OTHER;
        }
    }
    if (error->domain == f->io_error_quark()) {
        switch (error->code) {
        case HERO_G_IO_ERROR_PERMISSION_DENIED:
        case HERO_G_IO_ERROR_CANCELLED:
            return HERO_KEYRING_DENIED;
        default:
            return HERO_KEYRING_OTHER;
        }
    }
    return HERO_KEYRING_OTHER;
}

/* The failure a call left in `error`, said after `what` with `kind`, or, where
 * `kind` is `HERO_KEYRING_OK`, with the kind its domain and code give; the
 * error given back. With no error, `OTHER`. Answers -1. */
static int64_t hero_keyring_linux_said(hero_keyring_gerror *error, int64_t kind, const char *what) {
    int64_t code;

    if (error == NULL) {
        return hero_keyring_said(HERO_KEYRING_OTHER, 0, what);
    }
    if (kind == HERO_KEYRING_OK) {
        kind = hero_keyring_linux_kind(error);
    }
    code = (int64_t)error->code;
    (void)hero_keyring_said_with(kind, code, what, error->message);
    hero_keyring_functions.error_free(error);
    return -1;
}

static int64_t hero_keyring_linux_failed(hero_keyring_gerror *error, const char *what) {
    return hero_keyring_linux_said(error, HERO_KEYRING_OK, what);
}

/* Whether libsecret is there and a session bus answers: the bus's
 * connection into `*bus` where the caller asks for it, the lock taken; false
 * with `UNSUPPORTED` said where either is missing. */
static bool hero_keyring_enter(void **bus) {
    hero_keyring_gerror *error = NULL;
    void *connection;

    (void)pthread_once(&hero_keyring_once, hero_keyring_load);
    if (!hero_keyring_found) {
        (void)hero_keyring_said(HERO_KEYRING_UNSUPPORTED, 0, "no Secret Service here: libsecret-1.so.0 is not installed, or lacks a function of 0.19 or later");
        return false;
    }
    (void)pthread_mutex_lock(&hero_keyring_lock);
    connection = hero_keyring_functions.bus_get_sync(HERO_G_BUS_TYPE_SESSION, NULL, &error);
    if (connection == NULL) {
        (void)pthread_mutex_unlock(&hero_keyring_lock);
        (void)hero_keyring_linux_said(error, HERO_KEYRING_UNSUPPORTED, "no Secret Service here: no session bus answers");
        return false;
    }
    if (bus != NULL) {
        *bus = connection;
    } else {
        hero_keyring_functions.object_unref(connection);
    }
    return true;
}

static void hero_keyring_leave(void *bus) {
    if (bus != NULL) {
        hero_keyring_functions.object_unref(bus);
    }
    (void)pthread_mutex_unlock(&hero_keyring_lock);
}

int64_t hero_keyring_system(void *buffer, uint64_t count) {
    hero_keyring_gerror *error = NULL;
    void *bus = NULL;
    void *reply;
    const char *collection = NULL;
    bool none;

    if (!hero_keyring_room(buffer, count, "the buffer for the name has a count and no bytes") || !hero_keyring_enter(&bus)) {
        return -1;
    }
    reply = hero_keyring_functions.call_sync(bus, HERO_SECRETS_NAME, HERO_SECRETS_PATH, HERO_SECRETS_SERVICE, "ReadAlias", hero_keyring_functions.variant_new_parsed("(%s,)", "default"), "(o)", HERO_G_DBUS_CALL_FLAGS_NONE, HERO_SECRETS_WAIT, NULL, &error);
    if (reply == NULL) {
        hero_keyring_leave(bus);
        return hero_keyring_linux_failed(error, "the Secret Service did not answer");
    }
    /* `&o` borrows the path from the reply, which is given back after it is
     * read. The Secret Service answers "/" for an alias that names none. */
    hero_keyring_functions.variant_get(reply, "(&o)", &collection);
    none = collection == NULL || strcmp(collection, "/") == 0;
    hero_keyring_functions.variant_unref(reply);
    hero_keyring_leave(bus);
    if (none) {
        return hero_keyring_said(HERO_KEYRING_UNSUPPORTED, 0, "the Secret Service has no default collection to keep secrets in, and none can be made here without asking the person");
    }
    return hero_keyring_named(buffer, count, "Secret Service");
}

int64_t hero_keyring_store(const char *service, const char *account, const void *secret, uint64_t count) {
    char label[2 * HERO_KEYRING_NAME_MOST + 8];
    size_t service_length = 0;
    size_t account_length = 0;
    hero_keyring_gerror *error = NULL;
    void *value;
    int stored;
    /* Text where the bytes are UTF-8 with no zero, so a program that reads
     * the Secret Service's text (Python's keyring, `secret-tool`) reads it. */
    bool text;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length) || !hero_keyring_secret(secret, count)) {
        return -1;
    }
    text = count == 0 || (memchr(secret, 0, (size_t)count) == NULL && hero_keyring_is_utf8((const unsigned char *)secret, (size_t)count));
    if (account_length > 0) {
        (void)snprintf(label, sizeof label, "%s: %s", service, account);
    } else {
        (void)snprintf(label, sizeof label, "%s", service);
    }
    if (!hero_keyring_enter(NULL)) {
        return -1;
    }
    value = hero_keyring_functions.value_new(count > 0 ? (const char *)secret : "", (ptrdiff_t)count, text ? "text/plain" : "application/octet-stream");
    if (value == NULL) {
        hero_keyring_leave(NULL);
        return hero_keyring_said(HERO_KEYRING_LIMIT, 0, "libsecret made no value of the secret");
    }
    stored = hero_keyring_functions.store_binary_sync(&hero_keyring_generic, NULL, label, value, NULL, &error, "service", service, "username", account, (char *)NULL);
    hero_keyring_functions.value_unref(value);
    hero_keyring_leave(NULL);
    if (error != NULL && error->domain == hero_keyring_functions.dbus_error_quark() && (error->code == HERO_G_DBUS_ERROR_UNKNOWN_METHOD || error->code == HERO_G_DBUS_ERROR_UNKNOWN_OBJECT)) {
        /* The default collection's object is not there (measured: a
         * gnome-keyring with no login keyring and no display to ask for one
         * answers UnknownMethod at `/org/freedesktop/secrets/collection/login`). */
        return hero_keyring_linux_said(error, HERO_KEYRING_UNSUPPORTED, "the Secret Service has no default collection to keep the secret in");
    }
    if (!stored || error != NULL) {
        return hero_keyring_linux_failed(error, "the Secret Service did not store the secret");
    }
    hero_keyring_fine();
    return 0;
}

int64_t hero_keyring_read(const char *service, const char *account, void *buffer, uint64_t count) {
    size_t service_length = 0;
    size_t account_length = 0;
    hero_keyring_gerror *error = NULL;
    const char *bytes;
    size_t length = 0;
    void *value;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length) || !hero_keyring_room(buffer, count, "the buffer for the secret has a count and no bytes")) {
        return -1;
    }
    if (!hero_keyring_enter(NULL)) {
        return -1;
    }
    value = hero_keyring_functions.lookup_binary_sync(&hero_keyring_generic, NULL, &error, "service", service, "username", account, (char *)NULL);
    if (value == NULL) {
        hero_keyring_leave(NULL);
        if (error != NULL) {
            return hero_keyring_linux_failed(error, "the Secret Service did not read the secret");
        }
        return hero_keyring_said(HERO_KEYRING_NOT_FOUND, 0, "no secret stands under that service and account");
    }
    bytes = hero_keyring_functions.value_get(value, &length);
    if (bytes != NULL && length > 0 && (uint64_t)length <= count) {
        memcpy(buffer, bytes, length);
    }
    hero_keyring_functions.value_unref(value);
    hero_keyring_leave(NULL);
    if (length > (size_t)INT64_MAX) {
        return hero_keyring_said(HERO_KEYRING_LIMIT, 0, "the secret takes more bytes than a count holds");
    }
    hero_keyring_fine();
    return (int64_t)length;
}

/* `secret_password_clear_sync` over the service and, where `account` is not
 * NULL, the account; `all` takes "nothing stood" as done. */
static int64_t hero_keyring_linux_clear(const char *service, const char *account, bool all) {
    hero_keyring_gerror *error = NULL;
    int cleared;

    if (!hero_keyring_enter(NULL)) {
        return -1;
    }
    if (account == NULL) {
        cleared = hero_keyring_functions.clear_sync(&hero_keyring_generic, NULL, &error, "service", service, (char *)NULL);
    } else {
        cleared = hero_keyring_functions.clear_sync(&hero_keyring_generic, NULL, &error, "service", service, "username", account, (char *)NULL);
    }
    hero_keyring_leave(NULL);
    if (error != NULL) {
        return hero_keyring_linux_failed(error, "the Secret Service did not remove the secret");
    }
    if (!cleared && !all) {
        return hero_keyring_said(HERO_KEYRING_NOT_FOUND, 0, "no secret stands under that service and account");
    }
    hero_keyring_fine();
    return 0;
}

int64_t hero_keyring_remove(const char *service, const char *account) {
    size_t service_length = 0;
    size_t account_length = 0;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_name(account, true, "account", &account_length)) {
        return -1;
    }
    return hero_keyring_linux_clear(service, account, false);
}

int64_t hero_keyring_remove_all(const char *service) {
    size_t service_length = 0;

    if (!hero_keyring_name(service, false, "service", &service_length)) {
        return -1;
    }
    return hero_keyring_linux_clear(service, NULL, true);
}

/* The account of the item at `path`, by its `Attributes` property, added to
 * the listing; false only where the count would pass an `int64_t`. An item
 * that is gone by now, or holds no account this layer can name, adds
 * nothing. */
static bool hero_keyring_linux_item(void *bus, const char *path, void *buffer, uint64_t count, uint64_t *used) {
    const hero_keyring_libsecret *f = &hero_keyring_functions;
    hero_keyring_gerror *error = NULL;
    void *reply;
    void *attributes = NULL;
    char *account = NULL;
    bool fits = true;

    reply = f->call_sync(bus, HERO_SECRETS_NAME, path, "org.freedesktop.DBus.Properties", "Get", f->variant_new_parsed("(%s, %s)", HERO_SECRETS_ITEM, "Attributes"), "(v)", HERO_G_DBUS_CALL_FLAGS_NONE, HERO_SECRETS_WAIT, NULL, &error);
    if (reply == NULL) {
        if (error != NULL) {
            f->error_free(error);
        }
        return true;
    }
    f->variant_get(reply, "(v)", &attributes);
    f->variant_unref(reply);
    if (attributes == NULL) {
        return true;
    }
    /* A lookup of the wrong type answers false rather than reading, so an
     * item whose attributes are no `a{ss}` adds nothing. */
    if (f->variant_lookup(attributes, "username", "s", &account) && account != NULL) {
        size_t length = 0;

        while (length <= HERO_KEYRING_NAME_MOST && account[length] != '\0') {
            length += 1;
        }
        if (hero_keyring_listable((const unsigned char *)account, length)) {
            fits = hero_keyring_list_one(buffer, count, used, (const unsigned char *)account, length);
        }
        f->free(account);
    }
    f->variant_unref(attributes);
    return fits;
}

int64_t hero_keyring_accounts(const char *service, void *buffer, uint64_t count) {
    const hero_keyring_libsecret *f = &hero_keyring_functions;
    size_t service_length = 0;
    hero_keyring_gerror *error = NULL;
    void *bus = NULL;
    void *reply;
    char **unlocked = NULL;
    char **locked = NULL;
    uint64_t used = 0;
    bool fits = true;

    if (!hero_keyring_name(service, false, "service", &service_length) || !hero_keyring_room(buffer, count, "the buffer for the accounts has a count and no bytes")) {
        return -1;
    }
    if (!hero_keyring_enter(&bus)) {
        return -1;
    }
    /* `%s` writes the service as one string value, whatever it holds, so no
     * name is read as the text of the format. */
    reply = f->call_sync(bus, HERO_SECRETS_NAME, HERO_SECRETS_PATH, HERO_SECRETS_SERVICE, "SearchItems", f->variant_new_parsed("({'service': %s},)", service), "(aoao)", HERO_G_DBUS_CALL_FLAGS_NONE, HERO_SECRETS_WAIT, NULL, &error);
    if (reply == NULL) {
        hero_keyring_leave(bus);
        return hero_keyring_linux_failed(error, "the Secret Service did not list the accounts");
    }
    f->variant_get(reply, "(^ao^ao)", &unlocked, &locked);
    f->variant_unref(reply);
    for (size_t i = 0; fits && unlocked != NULL && unlocked[i] != NULL; i += 1) {
        fits = hero_keyring_linux_item(bus, unlocked[i], buffer, count, &used);
    }
    for (size_t i = 0; fits && locked != NULL && locked[i] != NULL; i += 1) {
        fits = hero_keyring_linux_item(bus, locked[i], buffer, count, &used);
    }
    f->strfreev(unlocked);
    f->strfreev(locked);
    hero_keyring_leave(bus);
    if (!fits) {
        return hero_keyring_said(HERO_KEYRING_LIMIT, 0, "the accounts take more bytes than a count holds");
    }
    hero_keyring_fine();
    return (int64_t)used;
}

#endif
