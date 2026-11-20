/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_tls.c — TLS by the system's own library, one member of the
 * archive the compiler puts on every program's link line (M-rebel-library;
 * `selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_tls.h`, says what each function answers; this file says how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * THREE ARMS, AND TWO SHAPES. Windows and Linux run TLS over a stream: their
 * libraries, SChannel and OpenSSL, read and write bytes the caller moves, so
 * one driver below (THE DRIVER) moves them through `hero_stream.h`, and each
 * arm supplies an ENGINE that never waits: bytes fed in, records taken out,
 * plaintext opened and sealed. A connection made by name is `hero_net.h`'s
 * socket made a stream by `hero_stream_net.h`, then the same driver. macOS
 * is the other shape: Network.framework makes its own socket and runs TLS
 * on its own queue, so its arm (THE APPLE ARM) waits on that queue's blocks
 * with a semaphore and has no engine; TLS over a stream the program holds
 * is Secure Transport's alone there, deprecated, so it answers
 * `HERO_TLS_UNSUPPORTED`.
 *
 * THE LIBRARIES ARE LINKED BY THE MEMBER ITSELF, as `hero_net.c` links
 * `ws2_32`: on Windows by `#pragma comment(lib)`, on macOS by a
 * `.linker_option` the object carries (measured 2026-11-16: ld64 reads it
 * from an archive member it takes, and only then), and on Linux by nothing,
 * since OpenSSL is opened with `dlopen` at the first call that needs it.
 * So a program that never calls this member links none of them.
 *
 * MEMORY. A connection's state is taken from the layer's own blocks
 * (`hero_layer_alloc.h`, given by `parts/alloc.c`, the single allocation
 * point) and given back at its close: a TLS session holds records of 16 KB
 * and the library's own objects, which no fixed table could hold. A block
 * that is not there is `HERO_TLS_LIMIT`, never an abort. The listener table
 * is fixed, as `hero_net.c`'s socket table is.
 *
 * Written from the platforms' documentation: the Win32 reference for SSPI
 * and CryptoAPI, Apple's headers for Network.framework and the Security
 * framework, OpenSSL's manual pages; no other library's source.
 */

#include "hero_tls.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"
#include "hero_net.h"
#include "hero_stream_net.h"
#include "hero_time.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define SECURITY_WIN32
/* SChannel's `SCH_CREDENTIALS`, which names `UNICODE_STRING`. */
#define SCHANNEL_USE_BLACKLISTS
#include <windows.h>
#include <subauth.h>
#include <wincrypt.h>
#include <sspi.h>
#include <schannel.h>
#include <ncrypt.h>
/* What only a program that takes this member links. MSVC's word; a MinGW
 * target links them by `-l`. */
#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "ncrypt.lib")
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <Network/Network.h>
#include <Security/Security.h>
#include <arpa/inet.h>
#include <dispatch/dispatch.h>
#include <errno.h>
#include <pthread.h>
/* The frameworks the arm calls, named in the object, so that the linker
 * links them into a program that takes this member and into no other. */
__asm__(".linker_option \"-framework\", \"Network\"");
__asm__(".linker_option \"-framework\", \"Security\"");
__asm__(".linker_option \"-framework\", \"CoreFoundation\"");
#else
#include <arpa/inet.h>
#include <dlfcn.h>
#include <pthread.h>
#endif

/* -- THE LAST FAILURE, PER THREAD ------------------------------------------ */

/* The words kept of a failure, with their zero byte. */
#define HERO_TLS_WORDS_MOST 640

static _Thread_local int64_t hero_tls_kind;
static _Thread_local int64_t hero_tls_os;
static _Thread_local char hero_tls_words[HERO_TLS_WORDS_MOST];

/* `text` copied into `into`, `most` bytes with the zero, cut where a
 * character ends; answers the length copied. */
static size_t hero_tls_copy_text(char *into, size_t most, const char *text) {
    size_t length = text == NULL ? 0 : strlen(text);

    if (most == 0) {
        return 0;
    }
    if (length > most - 1) {
        length = most - 1;
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

/* The failure this call answers: its kind, the platform's number, and
 * words, the system's where it gave some, or NULL for none. */
static void hero_tls_said(int64_t kind, int64_t os, const char *words) {
    hero_tls_kind = kind;
    hero_tls_os = os;
    (void)hero_tls_copy_text(hero_tls_words, sizeof hero_tls_words, words);
}

static void hero_tls_fine(void) {
    hero_tls_said(HERO_TLS_OK, 0, NULL);
}

int64_t hero_tls_last_error(void) {
    return hero_tls_kind;
}

int64_t hero_tls_last_os_error(void) {
    return hero_tls_os;
}

int64_t hero_tls_last_message(void *buffer, uint64_t count) {
    if (buffer == NULL || count == 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "no room was given for the message");
        return -1;
    }
    return (int64_t)hero_tls_copy_text((char *)buffer, count > SIZE_MAX ? SIZE_MAX : (size_t)count, hero_tls_words);
}

/* -- LOCKS ------------------------------------------------------------------ */

#if defined(_WIN32)
typedef SRWLOCK hero_tls_mutex;
static void hero_tls_mutex_init(hero_tls_mutex *m) {
    InitializeSRWLock(m);
}
static void hero_tls_mutex_lock(hero_tls_mutex *m) {
    AcquireSRWLockExclusive(m);
}
static bool hero_tls_mutex_try(hero_tls_mutex *m) {
    return TryAcquireSRWLockExclusive(m) != 0;
}
static void hero_tls_mutex_unlock(hero_tls_mutex *m) {
    ReleaseSRWLockExclusive(m);
}
static void hero_tls_mutex_end(hero_tls_mutex *m) {
    (void)m;
}
/* The lock over the listener table. */
static SRWLOCK hero_tls_lock = SRWLOCK_INIT;
#else
#if !defined(__APPLE__)
/* The driver's locks; the Apple arm has no driver. */
typedef pthread_mutex_t hero_tls_mutex;
static void hero_tls_mutex_init(hero_tls_mutex *m) {
    (void)pthread_mutex_init(m, NULL);
}
static void hero_tls_mutex_lock(hero_tls_mutex *m) {
    (void)pthread_mutex_lock(m);
}
static bool hero_tls_mutex_try(hero_tls_mutex *m) {
    return pthread_mutex_trylock(m) == 0;
}
static void hero_tls_mutex_unlock(hero_tls_mutex *m) {
    (void)pthread_mutex_unlock(m);
}
static void hero_tls_mutex_end(hero_tls_mutex *m) {
    (void)pthread_mutex_destroy(m);
}
#endif
/* The lock over the listener table. */
static pthread_mutex_t hero_tls_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

/* -- ARGUMENTS ------------------------------------------------------------- */

/* The most bytes of PEM a call reads: far past any chain of certificates. */
#define HERO_TLS_PEM_MOST (UINT64_C(1) << 24)
/* The longest host name the layer takes, `hero_net.c`'s. */
#define HERO_TLS_HOST_MOST 1024u
/* The most bytes of an ALPN list, as text. */
#define HERO_TLS_PROTOCOLS_MOST 1024u

/* Whether `count` bytes stand behind `bytes`; says `HERO_TLS_INVALID`
 * where they do not. */
static bool hero_tls_bytes_given(const void *bytes, uint64_t count, const char *what) {
    char words[160];

    if (count > 0 && bytes == NULL) {
        (void)snprintf(words, sizeof words, "%s: a count of %llu with no bytes behind it", what, (unsigned long long)count);
        hero_tls_said(HERO_TLS_INVALID, 0, words);
        return false;
    }
    if (count > HERO_TLS_PEM_MOST) {
        (void)snprintf(words, sizeof words, "%s: %llu bytes, more than the 16 MiB a PEM is read up to", what, (unsigned long long)count);
        hero_tls_said(HERO_TLS_INVALID, 0, words);
        return false;
    }
    return true;
}

static bool hero_tls_host_given(const char *host) {
    size_t length;

    if (host == NULL || host[0] == '\0') {
        hero_tls_said(HERO_TLS_INVALID, 0, "no host was given");
        return false;
    }
    length = strlen(host);
    if (length > HERO_TLS_HOST_MOST) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the host name is longer than 1024 bytes");
        return false;
    }
    return true;
}

static bool hero_tls_port_given(int64_t port, bool zero) {
    if (port < (zero ? 0 : 1) || port > 65535) {
        hero_tls_said(HERO_TLS_INVALID, 0, zero ? "the port is outside 0 to 65535" : "the port is outside 1 to 65535");
        return false;
    }
    return true;
}

/* Whether `protocols` is a list of ALPN names, each 1 to 255 bytes, joined
 * by `,`; NULL and "" are the empty list. */
static bool hero_tls_protocols_given(const char *protocols) {
    size_t length;
    size_t run = 0;

    if (protocols == NULL || protocols[0] == '\0') {
        return true;
    }
    length = strlen(protocols);
    if (length > HERO_TLS_PROTOCOLS_MOST) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the list of application protocols is longer than 1024 bytes");
        return false;
    }
    for (size_t at = 0; at <= length; at += 1) {
        if (at == length || protocols[at] == ',') {
            if (run == 0 || run > 255) {
                hero_tls_said(HERO_TLS_INVALID, 0, "an application protocol in the list is empty or longer than 255 bytes");
                return false;
            }
            run = 0;
        } else {
            run += 1;
        }
    }
    return true;
}

/* The ALPN list as the protocol carries it, each name after its length in
 * one byte; answers the length written, which `most` bounds. */
#if !defined(__APPLE__)
static size_t hero_tls_protocols_wire(const char *protocols, unsigned char *into, size_t most) {
    size_t written = 0;
    size_t start = 0;
    size_t length = protocols == NULL ? 0 : strlen(protocols);

    if (length == 0) {
        return 0;
    }
    for (size_t at = 0; at <= length; at += 1) {
        if (at == length || protocols[at] == ',') {
            size_t run = at - start;

            if (written + 1 + run > most) {
                return written;
            }
            into[written] = (unsigned char)run;
            memcpy(into + written + 1, protocols + start, run);
            written += 1 + run;
            start = at + 1;
        }
    }
    return written;
}
#endif

static bool hero_tls_checks_given(int64_t checks) {
    if (checks != HERO_TLS_CHECK_ALL && checks != HERO_TLS_CHECK_NOTHING_FOR_TESTS) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the checks are none of HERO_TLS_CHECK_ALL and HERO_TLS_CHECK_NOTHING_FOR_TESTS");
        return false;
    }
    return true;
}

/* -- PEM AND DER ------------------------------------------------------------
 *
 * The certificates and the key arrive as PEM (RFC 7468): base 64 between a
 * `-----BEGIN <label>-----` line and its `-----END <label>-----`. Each block
 * is decoded here to its DER, the bytes every platform's library reads. A
 * key becomes PKCS #8 (RFC 5208) whatever form it came in, which Windows'
 * CNG and OpenSSL import, and also the form Apple's `SecKeyCreateWithData`
 * reads: RSA's own (PKCS #1, RFC 8017) and, for an EC key, the public point
 * followed by the secret scalar (ANSI X9.63), both read out of SEC 1's
 * `ECPrivateKey` (RFC 5915). */

typedef struct hero_tls_blob {
    unsigned char *bytes;
    size_t count;
} hero_tls_blob;

static void hero_tls_blob_free(hero_tls_blob *b) {
    hero_layer_free(b->bytes);
    b->bytes = NULL;
    b->count = 0;
}

/* Where `needle` first stands in `text[from, count)`, or `count`. */
static size_t hero_tls_find(const unsigned char *text, size_t count, size_t from, const char *needle) {
    size_t length = strlen(needle);

    if (length == 0 || count < length) {
        return count;
    }
    for (size_t at = from; at + length <= count; at += 1) {
        if (memcmp(text + at, needle, length) == 0) {
            return at;
        }
    }
    return count;
}

/* The value of one base 64 character, or -1. */
static int hero_tls_sextet(unsigned char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    return -1;
}

/* `text[from, to)` decoded from base 64 into a new blob, white space passed
 * over; false where a character is no base 64 or the padding is wrong. */
static bool hero_tls_unbase64(const unsigned char *text, size_t from, size_t to, hero_tls_blob *out) {
    size_t most = (to - from) / 4 * 3 + 3;
    unsigned char *bytes = hero_layer_alloc(most == 0 ? 1 : most);
    size_t written = 0;
    uint32_t held = 0;
    int bits = 0;
    int padding = 0;

    if (bytes == NULL) {
        return false;
    }
    for (size_t at = from; at < to; at += 1) {
        unsigned char c = text[at];
        int value;

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            continue;
        }
        if (c == '=') {
            padding += 1;
            continue;
        }
        value = hero_tls_sextet(c);
        if (value < 0 || padding > 0) {
            hero_layer_free(bytes);
            return false;
        }
        held = (held << 6) | (uint32_t)value;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            bytes[written] = (unsigned char)((held >> bits) & 0xFFu);
            written += 1;
        }
    }
    if (padding > 2 || written == 0) {
        hero_layer_free(bytes);
        return false;
    }
    out->bytes = bytes;
    out->count = written;
    return true;
}

/* The next PEM block of `text` at or after `*at`: its label into `label`,
 * its bytes decoded into `out`, `*at` moved past it. Answers 1, 0 where no
 * block is left, -1 where one is not PEM (said `HERO_TLS_INVALID`). */
static int hero_tls_pem_next(const unsigned char *text, size_t count, size_t *at, char *label, size_t label_most, hero_tls_blob *out) {
    size_t begin = hero_tls_find(text, count, *at, "-----BEGIN ");
    size_t name;
    size_t name_end;
    size_t body;
    size_t end;
    char closing[96];

    if (begin == count) {
        return 0;
    }
    name = begin + strlen("-----BEGIN ");
    name_end = hero_tls_find(text, count, name, "-----");
    if (name_end == count || name_end - name >= label_most || name_end - name > 64) {
        hero_tls_said(HERO_TLS_INVALID, 0, "a PEM block's first line does not end in -----");
        return -1;
    }
    memcpy(label, text + name, name_end - name);
    label[name_end - name] = '\0';
    body = name_end + 5;
    (void)snprintf(closing, sizeof closing, "-----END %s-----", label);
    end = hero_tls_find(text, count, body, closing);
    if (end == count) {
        hero_tls_said(HERO_TLS_INVALID, 0, "a PEM block has no line that ends it");
        return -1;
    }
    /* A header line inside the block (`Proc-Type:`) is how an encrypted
     * key of the old form is written: nothing here decrypts. */
    for (size_t i = body; i < end; i += 1) {
        if (text[i] == ':') {
            hero_tls_said(HERO_TLS_INVALID, 0, "a PEM block holds headers, as an encrypted key does: decrypt the key first");
            return -1;
        }
    }
    if (!hero_tls_unbase64(text, body, end, out)) {
        hero_tls_said(HERO_TLS_INVALID, 0, "a PEM block's body is not base 64");
        return -1;
    }
    *at = end + strlen(closing);
    return 1;
}

/* The most certificates a chain or a list of roots holds. */
#define HERO_TLS_CERTS_MOST 512u

typedef struct hero_tls_certs {
    hero_tls_blob each[HERO_TLS_CERTS_MOST];
    size_t count;
} hero_tls_certs;

static void hero_tls_certs_free(hero_tls_certs *c) {
    for (size_t i = 0; i < c->count; i += 1) {
        hero_tls_blob_free(&c->each[i]);
    }
    c->count = 0;
}

/* Every `CERTIFICATE` block of the PEM, in its order, other blocks passed
 * over; false where there is none or one is not PEM (said). The caller
 * frees what was read with `hero_tls_certs_free`. */
static bool hero_tls_certs_of(const void *pem, uint64_t pem_count, hero_tls_certs *out, const char *what) {
    size_t at = 0;
    char label[80];
    char words[160];

    out->count = 0;
    for (;;) {
        hero_tls_blob block = { NULL, 0 };
        int found = hero_tls_pem_next((const unsigned char *)pem, (size_t)pem_count, &at, label, sizeof label, &block);

        if (found < 0) {
            hero_tls_certs_free(out);
            return false;
        }
        if (found == 0) {
            break;
        }
        if (strcmp(label, "CERTIFICATE") != 0) {
            hero_tls_blob_free(&block);
            continue;
        }
        if (out->count == HERO_TLS_CERTS_MOST) {
            hero_tls_blob_free(&block);
            hero_tls_certs_free(out);
            (void)snprintf(words, sizeof words, "%s: more than %u certificates", what, HERO_TLS_CERTS_MOST);
            hero_tls_said(HERO_TLS_INVALID, 0, words);
            return false;
        }
        out->each[out->count] = block;
        out->count += 1;
    }
    if (out->count == 0) {
        (void)snprintf(words, sizeof words, "%s: the PEM holds no CERTIFICATE block", what);
        hero_tls_said(HERO_TLS_INVALID, 0, words);
        return false;
    }
    return true;
}

/* One DER element of a span. */
typedef struct hero_tls_span {
    const unsigned char *p;
    size_t n;
} hero_tls_span;

/* The element at the front of `*s` whose tag is `tag`: its content into
 * `*inner`, `*s` moved past it; false where the front is no such element. */
static bool hero_tls_der(hero_tls_span *s, unsigned char tag, hero_tls_span *inner) {
    size_t length;
    size_t head;

    if (s->n < 2 || s->p[0] != tag) {
        return false;
    }
    if (s->p[1] < 0x80u) {
        length = s->p[1];
        head = 2;
    } else {
        size_t bytes = s->p[1] & 0x7Fu;

        if (bytes == 0 || bytes > 4 || s->n < 2 + bytes) {
            return false;
        }
        length = 0;
        for (size_t i = 0; i < bytes; i += 1) {
            length = (length << 8) | s->p[2 + i];
        }
        head = 2 + bytes;
    }
    if (length > s->n - head) {
        return false;
    }
    inner->p = s->p + head;
    inner->n = length;
    s->p += head + length;
    s->n -= head + length;
    return true;
}

/* A DER element's head, `tag` and `length`, written at `into`; answers its
 * length, at most 6. */
static size_t hero_tls_der_head(unsigned char *into, unsigned char tag, size_t length) {
    into[0] = tag;
    if (length < 0x80u) {
        into[1] = (unsigned char)length;
        return 2;
    }
    if (length <= 0xFFu) {
        into[1] = 0x81;
        into[2] = (unsigned char)length;
        return 3;
    }
    if (length <= 0xFFFFu) {
        into[1] = 0x82;
        into[2] = (unsigned char)(length >> 8);
        into[3] = (unsigned char)length;
        return 4;
    }
    into[1] = 0x83;
    into[2] = (unsigned char)(length >> 16);
    into[3] = (unsigned char)(length >> 8);
    into[4] = (unsigned char)length;
    return 5;
}

/* The object identifiers a key names, as their DER contents. */
static const unsigned char hero_tls_oid_rsa[] = { 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01 };
static const unsigned char hero_tls_oid_ec[] = { 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01 };
static const unsigned char hero_tls_oid_p256[] = { 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07 };
static const unsigned char hero_tls_oid_p384[] = { 0x2B, 0x81, 0x04, 0x00, 0x22 };
static const unsigned char hero_tls_oid_p521[] = { 0x2B, 0x81, 0x04, 0x00, 0x23 };

static bool hero_tls_span_is(hero_tls_span s, const unsigned char *bytes, size_t count) {
    return s.n == count && memcmp(s.p, bytes, count) == 0;
}

#define HERO_TLS_KEY_RSA 1
#define HERO_TLS_KEY_EC 2

/* A private key, read. */
typedef struct hero_tls_key {
    int kind;
    /* An EC key's curve, by the bytes of its scalar: 32, 48 or 66. */
    size_t scalar;
    /* The key as PKCS #8. */
    hero_tls_blob pkcs8;
    /* RSA: PKCS #1; EC: 04 || X || Y || K, which `SecKeyCreateWithData`
     * reads. */
    hero_tls_blob own;
} hero_tls_key;

static void hero_tls_key_free(hero_tls_key *k) {
    hero_tls_blob_free(&k->pkcs8);
    hero_tls_blob_free(&k->own);
}

/* A new blob of `parts` joined, each `count` bytes. */
static bool hero_tls_join(hero_tls_blob *out, const unsigned char *const *parts, const size_t *counts, size_t n) {
    size_t total = 0;

    for (size_t i = 0; i < n; i += 1) {
        total += counts[i];
    }
    out->bytes = hero_layer_alloc(total == 0 ? 1 : total);
    if (out->bytes == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for the key");
        return false;
    }
    out->count = 0;
    for (size_t i = 0; i < n; i += 1) {
        memcpy(out->bytes + out->count, parts[i], counts[i]);
        out->count += counts[i];
    }
    return true;
}

/* PKCS #8 around `inner`, a key of the algorithm whose identifier's DER
 * contents are `algorithm` (the SEQUENCE's contents, already written). */
static bool hero_tls_pkcs8(hero_tls_blob *out, const unsigned char *algorithm, size_t algorithm_count, hero_tls_span inner) {
    static const unsigned char version[] = { 0x02, 0x01, 0x00 };
    unsigned char algorithm_head[6];
    unsigned char octets_head[6];
    unsigned char outer_head[6];
    size_t algorithm_head_count = hero_tls_der_head(algorithm_head, 0x30, algorithm_count);
    size_t octets_head_count = hero_tls_der_head(octets_head, 0x04, inner.n);
    size_t body = sizeof version + algorithm_head_count + algorithm_count + octets_head_count + inner.n;
    size_t outer_head_count = hero_tls_der_head(outer_head, 0x30, body);
    const unsigned char *parts[] = { outer_head, version, algorithm_head, algorithm, octets_head, inner.p };
    const size_t counts[] = { outer_head_count, sizeof version, algorithm_head_count, algorithm_count, octets_head_count, inner.n };

    return hero_tls_join(out, parts, counts, 6);
}

/* An EC key's own form, 04 || X || Y || K, from SEC 1's `ECPrivateKey`;
 * its curve's identifier into `*curve` where it names one. */
static bool hero_tls_ec_own(hero_tls_span sec1, hero_tls_key *key, hero_tls_span *curve) {
    hero_tls_span seq;
    hero_tls_span version;
    hero_tls_span scalar;
    hero_tls_span point = { NULL, 0 };
    unsigned char padded[66];

    if (!hero_tls_der(&sec1, 0x30, &seq) || !hero_tls_der(&seq, 0x02, &version) || !hero_tls_der(&seq, 0x04, &scalar)) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the EC key is not SEC 1's ECPrivateKey");
        return false;
    }
    while (seq.n > 0) {
        hero_tls_span field;

        if (hero_tls_der(&seq, 0xA0, &field)) {
            hero_tls_span oid;

            if (curve != NULL && hero_tls_der(&field, 0x06, &oid)) {
                *curve = oid;
            }
        } else if (hero_tls_der(&seq, 0xA1, &field)) {
            hero_tls_span bits;

            if (hero_tls_der(&field, 0x03, &bits) && bits.n > 1 && bits.p[0] == 0) {
                point.p = bits.p + 1;
                point.n = bits.n - 1;
            }
        } else {
            break;
        }
    }
    if (point.n == 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the EC key holds no public point; OpenSSL writes it with one");
        return false;
    }
    key->scalar = point.n == 65 ? 32 : point.n == 97 ? 48 : point.n == 133 ? 66 : 0;
    if (key->scalar == 0 || point.p[0] != 0x04 || scalar.n > key->scalar) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the EC key is on none of P-256, P-384 and P-521, or its point is compressed");
        return false;
    }
    memset(padded, 0, sizeof padded);
    memcpy(padded + (key->scalar - scalar.n), scalar.p, scalar.n);
    {
        const unsigned char *parts[] = { point.p, padded };
        const size_t counts[] = { point.n, key->scalar };

        return hero_tls_join(&key->own, parts, counts, 2);
    }
}

/* The first private key of the PEM, read into `*key`; false where there is
 * none, it is encrypted, or it is of no kind taken here (said). */
static bool hero_tls_key_of(const void *pem, uint64_t pem_count, hero_tls_key *key) {
    size_t at = 0;
    char label[80];

    memset(key, 0, sizeof *key);
    for (;;) {
        hero_tls_blob block = { NULL, 0 };
        int found = hero_tls_pem_next((const unsigned char *)pem, (size_t)pem_count, &at, label, sizeof label, &block);
        hero_tls_span whole;
        bool good = false;

        if (found < 0) {
            return false;
        }
        if (found == 0) {
            hero_tls_said(HERO_TLS_INVALID, 0, "the PEM holds no PRIVATE KEY, RSA PRIVATE KEY or EC PRIVATE KEY block");
            return false;
        }
        whole.p = block.bytes;
        whole.n = block.count;
        if (strcmp(label, "ENCRYPTED PRIVATE KEY") == 0) {
            hero_tls_blob_free(&block);
            hero_tls_said(HERO_TLS_INVALID, 0, "the key is encrypted: decrypt it first");
            return false;
        } else if (strcmp(label, "RSA PRIVATE KEY") == 0) {
            static const unsigned char algorithm[] = { 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00 };
            const unsigned char *parts[] = { block.bytes };
            const size_t counts[] = { block.count };

            key->kind = HERO_TLS_KEY_RSA;
            good = hero_tls_pkcs8(&key->pkcs8, algorithm, sizeof algorithm, whole) && hero_tls_join(&key->own, parts, counts, 1);
        } else if (strcmp(label, "EC PRIVATE KEY") == 0) {
            hero_tls_span curve = { NULL, 0 };

            key->kind = HERO_TLS_KEY_EC;
            good = hero_tls_ec_own(whole, key, &curve);
            if (good && curve.n == 0) {
                hero_tls_said(HERO_TLS_INVALID, 0, "the EC key does not name its curve");
                good = false;
            }
            if (good) {
                unsigned char algorithm[2 + sizeof hero_tls_oid_ec + 2 + 16];
                size_t written = 0;

                algorithm[written++] = 0x06;
                algorithm[written++] = (unsigned char)sizeof hero_tls_oid_ec;
                memcpy(algorithm + written, hero_tls_oid_ec, sizeof hero_tls_oid_ec);
                written += sizeof hero_tls_oid_ec;
                if (curve.n > 16) {
                    hero_tls_said(HERO_TLS_INVALID, 0, "the EC key names a curve not taken here");
                    good = false;
                } else {
                    algorithm[written++] = 0x06;
                    algorithm[written++] = (unsigned char)curve.n;
                    memcpy(algorithm + written, curve.p, curve.n);
                    written += curve.n;
                    good = hero_tls_pkcs8(&key->pkcs8, algorithm, written, whole);
                }
            }
        } else if (strcmp(label, "PRIVATE KEY") == 0) {
            hero_tls_span outer = whole;
            hero_tls_span seq;
            hero_tls_span version;
            hero_tls_span algorithm;
            hero_tls_span oid;
            hero_tls_span inner;

            if (!hero_tls_der(&outer, 0x30, &seq) || !hero_tls_der(&seq, 0x02, &version) || !hero_tls_der(&seq, 0x30, &algorithm) || !hero_tls_der(&algorithm, 0x06, &oid) || !hero_tls_der(&seq, 0x04, &inner)) {
                hero_tls_said(HERO_TLS_INVALID, 0, "the PRIVATE KEY block is not PKCS #8");
            } else if (hero_tls_span_is(oid, hero_tls_oid_rsa, sizeof hero_tls_oid_rsa)) {
                const unsigned char *parts[] = { inner.p };
                const size_t counts[] = { inner.n };
                const unsigned char *whole_parts[] = { block.bytes };
                const size_t whole_counts[] = { block.count };

                key->kind = HERO_TLS_KEY_RSA;
                good = hero_tls_join(&key->own, parts, counts, 1) && hero_tls_join(&key->pkcs8, whole_parts, whole_counts, 1);
            } else if (hero_tls_span_is(oid, hero_tls_oid_ec, sizeof hero_tls_oid_ec)) {
                hero_tls_span curve;
                const unsigned char *whole_parts[] = { block.bytes };
                const size_t whole_counts[] = { block.count };

                key->kind = HERO_TLS_KEY_EC;
                if (!hero_tls_der(&algorithm, 0x06, &curve) || !(hero_tls_span_is(curve, hero_tls_oid_p256, sizeof hero_tls_oid_p256) || hero_tls_span_is(curve, hero_tls_oid_p384, sizeof hero_tls_oid_p384) || hero_tls_span_is(curve, hero_tls_oid_p521, sizeof hero_tls_oid_p521))) {
                    hero_tls_said(HERO_TLS_INVALID, 0, "the EC key is on none of P-256, P-384 and P-521");
                } else {
                    good = hero_tls_ec_own(inner, key, NULL) && hero_tls_join(&key->pkcs8, whole_parts, whole_counts, 1);
                }
            } else {
                hero_tls_said(HERO_TLS_INVALID, 0, "the key is neither RSA nor EC: Ed25519 and others are not taken here");
            }
        } else {
            hero_tls_blob_free(&block);
            continue;
        }
        hero_tls_blob_free(&block);
        if (!good) {
            hero_tls_key_free(key);
        }
        return good;
    }
}

/* -- THE LISTENER TABLE -----------------------------------------------------
 *
 * A listener is a handle the layer issues and checks, as `hero_net.c`'s
 * socket is: the handle carries this table's tag, a slot and the life the
 * slot held when it was issued, `HERO_HANDLE_TAG(HERO_HANDLE_TAG_TLS) |
 * (life << HERO_TLS_INDEX_BITS) | (index + 1)`, never an address. The tag
 * (`hero_handle_tag.h`) keeps it apart from every other table's handle at
 * any slot and any life, which the runtime's set of ended handles needs,
 * since it remembers an address with no type beside it; a value without
 * this table's tag is no listener, `HERO_TLS_NOT_OPEN` as the header says
 * of one the layer never issued. A call on a listener holds its slot busy; a close marks the slot
 * closing, so no later call reaches it, holds it busy too while it wakes the
 * calls inside it and waits until the port listens no more, and the last of
 * them to leave, the close among them, gives the arm's listener back. */

/* The arm's listener: an `nw_listener_t` on macOS, a socket and the
 * settings every connection shares elsewhere. */
typedef struct hero_tls_spot hero_tls_spot;

#define HERO_TLS_INDEX_BITS 13
#define HERO_TLS_SLOTS 4096u
/* The last life a slot is given, 2^45 - 1, so the life stays below the
 * tag; a slot at it is never issued again. */
#define HERO_TLS_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_TLS_INDEX_BITS)

typedef struct hero_tls_slot {
    hero_tls_spot *spot;
    uint64_t life;
    uint32_t busy;
    /* The next slot given back, its index plus one, 0 for none. */
    uint32_t next;
    bool held;
    bool closing;
} hero_tls_slot;

static hero_tls_slot hero_tls_slots[HERO_TLS_SLOTS];
/* How many slots have ever held a listener, under `hero_tls_lock`. */
static uint32_t hero_tls_fresh;
/* The first slot given back, its index plus one, under `hero_tls_lock`. */
static uint32_t hero_tls_given_back;

static void hero_tls_table_lock(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_tls_lock);
#else
    (void)pthread_mutex_lock(&hero_tls_lock);
#endif
}

static void hero_tls_table_unlock(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_tls_lock);
#else
    (void)pthread_mutex_unlock(&hero_tls_lock);
#endif
}

/* The arm's own: wakes every call waiting on `spot`, which then answers
 * `HERO_TLS_NOT_OPEN`, and returns once nothing listens on its port any
 * more; and gives back what `spot` holds, once nothing is inside it. Both
 * are called without the table's lock. */
static void hero_tls_spot_wake(hero_tls_spot *spot);
static void hero_tls_spot_free(hero_tls_spot *spot);

/* A handle for `spot`, or NULL with `HERO_TLS_LIMIT`. */
static hero_tls_listener *hero_tls_issue(hero_tls_spot *spot) {
    hero_tls_slot *slot = NULL;
    uint32_t index;

    hero_tls_table_lock();
    if (hero_tls_given_back != 0) {
        index = hero_tls_given_back - 1;
        slot = &hero_tls_slots[index];
        hero_tls_given_back = slot->next;
    } else if (hero_tls_fresh < HERO_TLS_SLOTS) {
        index = hero_tls_fresh;
        hero_tls_fresh += 1;
        slot = &hero_tls_slots[index];
    } else {
        hero_tls_table_unlock();
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many TLS listeners as it can, 4096");
        return NULL;
    }
    slot->life += 1;
    slot->spot = spot;
    slot->busy = 0;
    slot->next = 0;
    slot->held = true;
    slot->closing = false;
    hero_tls_table_unlock();
    return (hero_tls_listener *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_TLS) | (slot->life << HERO_TLS_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot `l` names while it holds the life `l` was issued for, or NULL.
 * Under the lock. */
static hero_tls_slot *hero_tls_slot_of(hero_tls_listener *l) {
    uint64_t value = (uint64_t)(uintptr_t)l;
    uint64_t index = value & ((UINT64_C(1) << HERO_TLS_INDEX_BITS) - 1);
    uint64_t life = (value & ~HERO_HANDLE_TAG_MASK) >> HERO_TLS_INDEX_BITS;
    hero_tls_slot *slot;

    /* Another table's handle, a stream's or a socket's, is no listener at
     * any slot and any life. */
    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_TLS) || index == 0 || index > HERO_TLS_SLOTS || life > HERO_TLS_LIFE_MOST) {
        return NULL;
    }
    slot = &hero_tls_slots[index - 1];
    if (!slot->held || slot->closing || slot->life != life) {
        return NULL;
    }
    return slot;
}

/* The slot given back to the free list. Under the lock. */
static void hero_tls_give_back(hero_tls_slot *slot) {
    slot->held = false;
    slot->closing = false;
    slot->spot = NULL;
    if (slot->life < HERO_TLS_LIFE_MOST) {
        slot->next = hero_tls_given_back;
        hero_tls_given_back = (uint32_t)(slot - hero_tls_slots) + 1;
    }
}

/* The listener `l` names, held busy until `hero_tls_leave`; NULL with
 * `HERO_TLS_NOT_OPEN` where it names none open. */
static hero_tls_spot *hero_tls_enter(hero_tls_listener *l, hero_tls_slot **held) {
    hero_tls_slot *slot;
    hero_tls_spot *spot;

    hero_tls_table_lock();
    slot = hero_tls_slot_of(l);
    if (slot == NULL) {
        hero_tls_table_unlock();
        hero_tls_said(HERO_TLS_NOT_OPEN, 0, "the TLS listener is closed, or was never one");
        return NULL;
    }
    slot->busy += 1;
    spot = slot->spot;
    *held = slot;
    hero_tls_table_unlock();
    return spot;
}

static void hero_tls_leave(hero_tls_slot *slot) {
    hero_tls_spot *freed = NULL;

    hero_tls_table_lock();
    slot->busy -= 1;
    if (slot->closing && slot->busy == 0) {
        freed = slot->spot;
        hero_tls_give_back(slot);
    }
    hero_tls_table_unlock();
    if (freed != NULL) {
        hero_tls_spot_free(freed);
    }
}

/* Whether the listener held as `slot` is being closed: a waiting call
 * asks it between its slices. */
static bool hero_tls_closing(hero_tls_slot *slot) {
    bool closing;

    hero_tls_table_lock();
    closing = slot->closing;
    hero_tls_table_unlock();
    return closing;
}

/* The close holds the slot busy while it wakes the calls inside and waits
 * for the arm's listener to stop listening, so the listener is given back
 * by whichever leaves last, this close where no call is inside, and never
 * while the close still reads it. */
int64_t hero_tls_close_listener(hero_tls_listener *l) {
    hero_tls_slot *slot;
    hero_tls_spot *spot;

    hero_tls_table_lock();
    slot = hero_tls_slot_of(l);
    if (slot == NULL) {
        hero_tls_table_unlock();
        hero_tls_said(HERO_TLS_NOT_OPEN, 0, "the TLS listener is closed, or was never one");
        return -1;
    }
    slot->closing = true;
    slot->busy += 1;
    spot = slot->spot;
    hero_tls_table_unlock();
    hero_tls_spot_wake(spot);
    hero_tls_leave(slot);
    hero_tls_fine();
    return 0;
}

/* -- WHAT A CONNECTION'S FAILURE IS CALLED ---------------------------------- */

#if !defined(__APPLE__)
/* A socket's kind of failure as this layer's. */
static int64_t hero_tls_from_net(int64_t kind) {
    switch (kind) {
    case HERO_NET_INVALID:
    case HERO_NET_ADDRESS:
    case HERO_NET_TOO_LONG:
        return HERO_TLS_INVALID;
    case HERO_NET_NOT_OPEN:
        return HERO_TLS_NOT_OPEN;
    case HERO_NET_NOT_FOUND:
        return HERO_TLS_NOT_FOUND;
    case HERO_NET_IN_USE:
        return HERO_TLS_IN_USE;
    case HERO_NET_REFUSED:
        return HERO_TLS_REFUSED;
    case HERO_NET_RESET:
        return HERO_TLS_CLOSED_PEER;
    case HERO_NET_TIMED_OUT:
        return HERO_TLS_TIMED_OUT;
    case HERO_NET_UNREACHABLE:
        return HERO_TLS_UNREACHABLE;
    case HERO_NET_LIMIT:
        return HERO_TLS_LIMIT;
    case HERO_NET_DENIED:
        return HERO_TLS_DENIED;
    case HERO_NET_STOPPED:
        return HERO_TLS_STOPPED;
    default:
        return HERO_TLS_OTHER;
    }
}

/* A stream's kind of failure, met while a handshake read or wrote, as this
 * layer's. */
static int64_t hero_tls_from_stream(int64_t kind) {
    switch (kind) {
    case HERO_STREAM_NOT_OPEN:
        return HERO_TLS_NOT_OPEN;
    case HERO_STREAM_LIMIT:
        return HERO_TLS_LIMIT;
    case HERO_STREAM_CLOSED_PEER:
        return HERO_TLS_CLOSED_PEER;
    case HERO_STREAM_TIMED_OUT:
        return HERO_TLS_TIMED_OUT;
    case HERO_STREAM_STOPPED:
        return HERO_TLS_STOPPED;
    case HERO_STREAM_UNSUPPORTED:
    case HERO_STREAM_INVALID:
        return HERO_TLS_INVALID;
    default:
        return HERO_TLS_OTHER;
    }
}

/* The words for a connection's failure the socket layer gives no words for. */
static const char *hero_tls_net_words(int64_t kind) {
    switch (kind) {
    case HERO_TLS_NOT_FOUND:
        return "the host name has no address";
    case HERO_TLS_REFUSED:
        return "nothing listens at that address and port";
    case HERO_TLS_UNREACHABLE:
        return "no route reaches the host";
    case HERO_TLS_TIMED_OUT:
        return "the time given passed first";
    case HERO_TLS_STOPPED:
        return "the program was asked to stop";
    case HERO_TLS_IN_USE:
        return "something listens at that address and port already";
    case HERO_TLS_DENIED:
        return "the system refused the address or the port to this program";
    case HERO_TLS_CLOSED_PEER:
        return "the peer closed the connection";
    case HERO_TLS_LIMIT:
        return "the process has no socket or memory left";
    default:
        return "the connection failed";
    }
}
#endif

/* -- THE EXCHANGE BEFORE TLS --------------------------------------------------
 *
 * The steps `hero_tls_connect_after` and `hero_tls_listen_after` run on the
 * plain connection before TLS begins (the header gives their form). One
 * machine, fed one byte at a time, serves the four places that run them: a
 * client's and a listener's framer on macOS, a client's and a server's
 * socket elsewhere. It asks for a byte only while a step expects one, so it
 * never reads past the last step and the peer's next byte is TLS's. Every
 * byte it takes moves it, and a line and a step have bounds, so a peer that
 * never ends a line is told, never followed. */

/* The steps as handed, checked and copied, counted: whoever runs them holds
 * a count, a listener for its connections to come. */
typedef struct hero_tls_script {
    int64_t refs;
    size_t count;
    unsigned char bytes[];
} hero_tls_script;

static void hero_tls_script_keep(hero_tls_script *s) {
    if (s != NULL) {
        (void)__atomic_add_fetch(&s->refs, 1, __ATOMIC_ACQ_REL);
    }
}

static void hero_tls_script_drop(hero_tls_script *s) {
    if (s != NULL && __atomic_sub_fetch(&s->refs, 1, __ATOMIC_ACQ_REL) == 0) {
        hero_layer_free(s);
    }
}

/* A step's length, four bytes, most significant first. */
static size_t hero_tls_step_length(const unsigned char *at) {
    return ((size_t)at[0] << 24) | ((size_t)at[1] << 16) | ((size_t)at[2] << 8) | (size_t)at[3];
}

/* The `count` bytes of `steps` checked against their form and bounds and
 * copied into `*out`, NULL where there are none; false said
 * `HERO_TLS_INVALID` or `HERO_TLS_LIMIT`. */
static bool hero_tls_script_of(const void *steps, uint64_t count, hero_tls_script **out) {
    const unsigned char *bytes = steps;
    size_t at = 0;
    size_t number = 0;
    char words[200];
    hero_tls_script *s;

    *out = NULL;
    if (!hero_tls_bytes_given(steps, count, "the steps")) {
        return false;
    }
    while (at < count) {
        size_t length;
        int kind;

        number += 1;
        if (count - at < 5) {
            (void)snprintf(words, sizeof words, "the steps end inside the head of step %zu", number);
            hero_tls_said(HERO_TLS_INVALID, 0, words);
            return false;
        }
        kind = bytes[at];
        length = hero_tls_step_length(bytes + at + 1);
        if (number > HERO_TLS_STEPS_MOST) {
            hero_tls_said(HERO_TLS_INVALID, 0, "there are more than 64 steps");
            return false;
        }
        if (kind != HERO_TLS_STEP_SEND && kind != HERO_TLS_STEP_EXPECT && kind != HERO_TLS_STEP_EXPECT_LINE) {
            (void)snprintf(words, sizeof words, "step %zu is of no kind the layer knows (%d)", number, kind);
            hero_tls_said(HERO_TLS_INVALID, 0, words);
            return false;
        }
        if (length == 0 || length > HERO_TLS_STEP_BYTES_MOST || (kind == HERO_TLS_STEP_EXPECT_LINE && length > HERO_TLS_PREFIX_MOST)) {
            (void)snprintf(words, sizeof words, "step %zu holds %zu bytes, where it holds 1 to %llu", number, length, (unsigned long long)(kind == HERO_TLS_STEP_EXPECT_LINE ? HERO_TLS_PREFIX_MOST : HERO_TLS_STEP_BYTES_MOST));
            hero_tls_said(HERO_TLS_INVALID, 0, words);
            return false;
        }
        if (length > count - at - 5) {
            (void)snprintf(words, sizeof words, "step %zu runs past the end of the steps", number);
            hero_tls_said(HERO_TLS_INVALID, 0, words);
            return false;
        }
        at += 5 + length;
    }
    if (count == 0) {
        return true;
    }
    s = hero_layer_alloc(sizeof *s + (size_t)count);
    if (s == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for the steps");
        return false;
    }
    s->refs = 1;
    s->count = (size_t)count;
    memcpy(s->bytes, steps, (size_t)count);
    *out = s;
    return true;
}

/* The room `hero_tls_shown` writes into: 64 bytes at four characters each,
 * the quotes, `...` and the zero. */
#define HERO_TLS_SHOWN_MOST 272u

/* `count` bytes as a reader sees them, between quotes and never raw:
 * printable ASCII as itself, `"` and `\` behind a `\`, `\r`, `\n` and `\t`,
 * every other byte `\xHH`; the first 64 of them, then `...` where there are
 * more, or where `more` says more were heard than kept. */
static void hero_tls_shown(const unsigned char *bytes, size_t count, bool more, char *into) {
    static const char digits[] = "0123456789abcdef";
    size_t w = 0;
    size_t shown = count > 64 ? 64 : count;

    into[w++] = '"';
    for (size_t i = 0; i < shown; i += 1) {
        unsigned char c = bytes[i];

        if (c == '"' || c == '\\') {
            into[w++] = '\\';
            into[w++] = (char)c;
        } else if (c == '\r' || c == '\n' || c == '\t') {
            into[w++] = '\\';
            into[w++] = c == '\r' ? 'r' : c == '\n' ? 'n' : 't';
        } else if (c >= 0x20 && c < 0x7F) {
            into[w++] = (char)c;
        } else {
            into[w++] = '\\';
            into[w++] = 'x';
            into[w++] = digits[c >> 4];
            into[w++] = digits[c & 15u];
        }
    }
    into[w++] = '"';
    if (count > shown || more) {
        into[w++] = '.';
        into[w++] = '.';
        into[w++] = '.';
    }
    into[w] = '\0';
}

#define HERO_TLS_EXCHANGE_MORE 0
#define HERO_TLS_EXCHANGE_DONE 1
#define HERO_TLS_EXCHANGE_FAILED 2
/* The first bytes of an answer or a line kept, for the words and for a
 * line's beginning, which is at most 64 bytes and the byte after it. */
#define HERO_TLS_HEARD_MOST 80u

typedef struct hero_tls_exchange {
    const hero_tls_script *script;
    /* Where the step after this one begins, and this one's number, from 1. */
    size_t next;
    size_t number;
    /* This step: its kind (0 once every step is met), its bytes. */
    int kind;
    const unsigned char *bytes;
    size_t length;
    /* Of an answer, the bytes heard; of a line, the bytes of this line. */
    size_t matched;
    /* Every byte this step has read, every line of it. */
    size_t step_read;
    unsigned char heard[HERO_TLS_HEARD_MOST];
    size_t heard_count;
    bool heard_more;
    int state;
    int64_t failed_kind;
    char words[HERO_TLS_WORDS_MOST];
} hero_tls_exchange;

static void hero_tls_exchange_load(hero_tls_exchange *e) {
    e->matched = 0;
    e->step_read = 0;
    e->heard_count = 0;
    e->heard_more = false;
    if (e->script == NULL || e->next >= e->script->count) {
        e->kind = 0;
        e->bytes = NULL;
        e->length = 0;
        e->state = HERO_TLS_EXCHANGE_DONE;
        return;
    }
    e->kind = e->script->bytes[e->next];
    e->length = hero_tls_step_length(e->script->bytes + e->next + 1);
    e->bytes = e->script->bytes + e->next + 5;
    e->next += 5 + e->length;
    e->number += 1;
}

static void hero_tls_exchange_begin(hero_tls_exchange *e, const hero_tls_script *script) {
    memset(e, 0, sizeof *e);
    e->script = script;
    e->state = HERO_TLS_EXCHANGE_MORE;
    hero_tls_exchange_load(e);
}

/* The bytes of this step where it sends, the machine moved past it. */
static bool hero_tls_exchange_sending(hero_tls_exchange *e, const unsigned char **bytes, size_t *length) {
    if (e->state != HERO_TLS_EXCHANGE_MORE || e->kind != HERO_TLS_STEP_SEND) {
        return false;
    }
    *bytes = e->bytes;
    *length = e->length;
    hero_tls_exchange_load(e);
    return true;
}

static void hero_tls_exchange_fail(hero_tls_exchange *e, int64_t kind, const char *words) {
    e->state = HERO_TLS_EXCHANGE_FAILED;
    e->failed_kind = kind;
    (void)hero_tls_copy_text(e->words, sizeof e->words, words);
}

/* The peer closed the connection while this step waited for it. */
static void hero_tls_exchange_closed(hero_tls_exchange *e) {
    char heard[HERO_TLS_SHOWN_MOST];
    char words[HERO_TLS_WORDS_MOST];

    hero_tls_shown(e->heard, e->heard_count, e->heard_more, heard);
    (void)snprintf(words, sizeof words, "the peer closed the connection during step %zu of the exchange before TLS, having sent %s", e->number, heard);
    hero_tls_exchange_fail(e, HERO_TLS_CLOSED_PEER, words);
}

/* One byte the peer sent, taken by the step that expects it; answers the
 * state after it. */
static int hero_tls_exchange_byte(hero_tls_exchange *e, unsigned char b) {
    char expected[HERO_TLS_SHOWN_MOST];
    char heard[HERO_TLS_SHOWN_MOST];
    char words[HERO_TLS_WORDS_MOST];

    if (e->heard_count < HERO_TLS_HEARD_MOST) {
        e->heard[e->heard_count] = b;
        e->heard_count += 1;
    } else {
        e->heard_more = true;
    }
    e->step_read += 1;
    if (e->kind == HERO_TLS_STEP_EXPECT) {
        if (b != e->bytes[e->matched]) {
            hero_tls_shown(e->bytes, e->length, false, expected);
            hero_tls_shown(e->heard, e->heard_count, e->heard_more, heard);
            (void)snprintf(words, sizeof words, "step %zu: expected %s, heard %s", e->number, expected, heard);
            hero_tls_exchange_fail(e, HERO_TLS_PREAMBLE, words);
            return e->state;
        }
        e->matched += 1;
        if (e->matched == e->length) {
            hero_tls_exchange_load(e);
        }
        return e->state;
    }
    e->matched += 1;
    if (b == '\n') {
        if (e->heard_count < e->length || memcmp(e->heard, e->bytes, e->length) != 0) {
            hero_tls_shown(e->bytes, e->length, false, expected);
            hero_tls_shown(e->heard, e->heard_count, e->heard_more, heard);
            (void)snprintf(words, sizeof words, "step %zu: expected a line beginning with %s, heard %s", e->number, expected, heard);
            hero_tls_exchange_fail(e, HERO_TLS_PREAMBLE, words);
            return e->state;
        }
        if (e->heard_count > e->length && e->heard[e->length] == '-') {
            /* A continuation, SMTP's `250-`: the next line is read. */
            e->matched = 0;
            e->heard_count = 0;
            e->heard_more = false;
            return e->state;
        }
        hero_tls_exchange_load(e);
        return e->state;
    }
    if (e->matched >= HERO_TLS_LINE_MOST || e->step_read >= HERO_TLS_STEP_BYTES_MOST) {
        hero_tls_shown(e->heard, e->heard_count, e->heard_more, heard);
        (void)snprintf(words, sizeof words, "step %zu: %s %llu bytes with no end, the line beginning %s", e->number, e->matched >= HERO_TLS_LINE_MOST ? "a line passed" : "the answer passed", (unsigned long long)(e->matched >= HERO_TLS_LINE_MOST ? HERO_TLS_LINE_MOST : HERO_TLS_STEP_BYTES_MOST), heard);
        hero_tls_exchange_fail(e, HERO_TLS_PREAMBLE, words);
    }
    return e->state;
}

/* -- THE ENGINES' COMMON SHAPE (Windows, Linux) ------------------------------
 *
 * An engine never waits. The driver feeds it what the peer sent, takes what
 * it wrote for the peer, and asks it to step a handshake, open a record or
 * seal plaintext. Each function that fails says so with `hero_tls_said`. A
 * `hero_tls_config` is what connections share, a listener's identity or a
 * client's trust, counted, since a connection accepted outlives its
 * listener. */

#if !defined(__APPLE__)

typedef struct hero_tls_config hero_tls_config;
typedef struct hero_tls_engine hero_tls_engine;

/* A handshake step's answers, and an open's that wants bytes. */
#define HERO_TLS_STEP_DONE 0
#define HERO_TLS_STEP_MORE (-2)
#define HERO_TLS_STEP_FAILED (-1)

/* The most plaintext one record carries, and so a seal's step. */
#define HERO_TLS_RECORD_MOST 16384u
/* What one read of the inner stream takes at most. */
#define HERO_TLS_INPUT_MOST (16384u + 2048u)

static bool hero_tls_arm_ready(void);
static hero_tls_config *hero_tls_config_client(const hero_tls_certs *roots, const unsigned char *alpn, size_t alpn_count, int64_t checks);
static hero_tls_config *hero_tls_config_server(const hero_tls_certs *chain, const hero_tls_key *key, const unsigned char *alpn, size_t alpn_count);
static void hero_tls_config_drop(hero_tls_config *c);
static hero_tls_engine *hero_tls_engine_new(hero_tls_config *c, const char *host);
static int hero_tls_engine_handshake(hero_tls_engine *e);
static bool hero_tls_engine_feed(hero_tls_engine *e, const unsigned char *bytes, size_t count);
static size_t hero_tls_engine_output(hero_tls_engine *e, unsigned char *buffer, size_t count);
static bool hero_tls_engine_pending(hero_tls_engine *e);
static int64_t hero_tls_engine_open(hero_tls_engine *e, unsigned char *buffer, size_t count);
static int64_t hero_tls_engine_seal(hero_tls_engine *e, const unsigned char *data, size_t count);
static void hero_tls_engine_close_notify(hero_tls_engine *e);
static int64_t hero_tls_engine_version(hero_tls_engine *e);
static size_t hero_tls_engine_protocol(hero_tls_engine *e, char *buffer, size_t count);
static void hero_tls_engine_free(hero_tls_engine *e);

#endif

/* ============================================================================
 * THE WINDOWS ENGINE: SChannel through SSPI.
 * ========================================================================== */
#if defined(_WIN32)
/* ==== BEGIN WINDOWS ENGINE ==== */

/* What a connection of this arm needs of Windows, written from the Win32
 * reference: SSPI's `AcquireCredentialsHandleW`, `InitializeSecurityContextW`,
 * `AcceptSecurityContext`, `EncryptMessage`, `DecryptMessage`,
 * `ApplyControlToken` and `QueryContextAttributesW`; CryptoAPI's
 * `CertGetCertificateChain` and `CertVerifyCertificateChainPolicy` for the
 * check; CNG's `NCryptImportKey` for a server's key.
 *
 * TLS 1.2 AT THE LEAST: `SCH_CREDENTIALS` (Windows 10 1809 and later) with
 * one `TLS_PARAMETERS` disabling every protocol but TLS 1.2 and 1.3; where a
 * Windows refuses that form, `SCHANNEL_CRED` enabling TLS 1.2 alone, the
 * newest that form names.
 *
 * THE CHECK IS THIS ARM'S, AFTER THE HANDSHAKE (`SCH_CRED_MANUAL_CRED_VALIDATION`):
 * SChannel's own check would answer one number for every refusal, so the
 * peer's chain is built by `CertGetCertificateChain`, with an engine whose
 * only roots are the ones given where some were given, and judged by
 * `CertVerifyCertificateChainPolicy` with the SSL policy and the host name,
 * whose error tells expired, wrong host and unknown authority apart. No
 * revocation is asked, as OpenSSL's arm asks none: a revocation check
 * reaches the network, which a check with roots of its own must not need.
 * The handshake has ended when the check runs, so a refused peer sees its
 * connection closed rather than an alert. */

/* The most bytes the peer may send before a record or a handshake message
 * ends: past it the peer is refused `HERO_TLS_LIMIT`. */
#define HERO_TLS_WIN_INPUT_MOST (UINT32_C(1) << 20)

/* `BCryptVerifySignature`, for the key's check below. */
#pragma comment(lib, "bcrypt.lib")

struct hero_tls_config {
    int64_t refs;
    bool server;
    int64_t checks;
    CredHandle credentials;
    /* The client's roots as a store, and the chain engine that trusts them
     * alone; NULL both for the system's own. */
    HCERTSTORE roots;
    HCERTCHAINENGINE chain_engine;
    /* The ALPN list as SChannel takes it, `SEC_APPLICATION_PROTOCOLS`;
     * `alpn_count` 0 for none. Words, for the structure's alignment. */
    unsigned long alpn[(HERO_TLS_PROTOCOLS_MOST + 64) / sizeof(unsigned long) + 1];
    unsigned long alpn_count;
    /* A server's identity: its certificate, in a store holding the chain,
     * and its key. */
    PCCERT_CONTEXT leaf;
    HCERTSTORE chain;
    NCRYPT_PROV_HANDLE provider;
    NCRYPT_KEY_HANDLE key;
    /* The name the key is kept under while the config lives; "" for none. */
    wchar_t key_name[96];
};

/* A growable run of bytes: `bytes[at, count)` is what is held. */
typedef struct hero_tls_win_bytes {
    unsigned char *bytes;
    size_t at;
    size_t count;
    size_t most;
} hero_tls_win_bytes;

struct hero_tls_engine {
    hero_tls_config *config;
    CtxtHandle context;
    bool have_context;
    bool server;
    /* The handshake has ended, and the context speaks records. */
    bool done;
    /* A post-handshake message (`SEC_I_RENEGOTIATE`) is being answered:
     * the handshake's calls take the next bytes, not `DecryptMessage`. */
    bool again;
    /* The peer's close_notify was read. */
    bool ended;
    /* The host, as the check names it, and as SNI names it (NULL for an
     * address, which SNI never names). */
    wchar_t *host;
    wchar_t *sni;
    SecPkgContext_StreamSizes sizes;
    /* What the peer sent and nothing has taken; what this end wrote for
     * the peer; plaintext opened and not yet read; a record being sealed. */
    hero_tls_win_bytes in;
    hero_tls_win_bytes out;
    hero_tls_win_bytes plain;
    unsigned char *seal;
    int64_t version;
    char protocol[HERO_TLS_PROTOCOL_MOST];
};

static bool hero_tls_arm_ready(void) {
    return true;
}

/* Windows' words for `code`, after `what`, as UTF-8. */
static void hero_tls_win_said(int64_t kind, DWORD code, const char *what) {
    wchar_t wide[400];
    char said[HERO_TLS_WORDS_MOST];
    char words[HERO_TLS_WORDS_MOST];
    DWORD length = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, 0, wide, (DWORD)(sizeof wide / sizeof wide[0]), NULL);

    said[0] = '\0';
    while (length > 0 && (wide[length - 1] == L'\r' || wide[length - 1] == L'\n' || wide[length - 1] == L' ' || wide[length - 1] == L'.')) {
        length -= 1;
    }
    if (length > 0) {
        int written = WideCharToMultiByte(CP_UTF8, 0, wide, (int)length, said, (int)sizeof said - 1, NULL, NULL);

        said[written > 0 ? written : 0] = '\0';
    }
    if (said[0] != '\0') {
        (void)snprintf(words, sizeof words, "%s: %s (0x%08lX)", what, said, (unsigned long)code);
    } else {
        (void)snprintf(words, sizeof words, "%s (0x%08lX)", what, (unsigned long)code);
    }
    hero_tls_said(kind, (int64_t)code, words);
}

/* `count` more bytes at the end of `b`, its held bytes first moved to its
 * start; false said `HERO_TLS_LIMIT` where no room is left. */
static bool hero_tls_win_append(hero_tls_win_bytes *b, const void *data, size_t count) {
    if (b->at > 0) {
        memmove(b->bytes, b->bytes + b->at, b->count - b->at);
        b->count -= b->at;
        b->at = 0;
    }
    if (count > HERO_TLS_WIN_INPUT_MOST || b->count > HERO_TLS_WIN_INPUT_MOST - count) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "the peer sent more than a megabyte that ends no TLS record");
        return false;
    }
    if (b->count + count > b->most) {
        size_t most = b->most == 0 ? 4096 : b->most;
        unsigned char *grown;

        while (most < b->count + count) {
            most *= 2;
        }
        grown = hero_layer_grow(b->bytes, most);
        if (grown == NULL) {
            hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
            return false;
        }
        b->bytes = grown;
        b->most = most;
    }
    if (count > 0) {
        memcpy(b->bytes + b->count, data, count);
    }
    b->count += count;
    return true;
}

/* `b`'s held bytes, the first `taken` of them gone. */
static void hero_tls_win_take(hero_tls_win_bytes *b, size_t taken) {
    b->at += taken;
    if (b->at >= b->count) {
        b->at = 0;
        b->count = 0;
    }
}

static size_t hero_tls_win_held(const hero_tls_win_bytes *b) {
    return b->count - b->at;
}

/* `text`, UTF-8, as a new UTF-16 string; NULL said where it is not UTF-8. */
static wchar_t *hero_tls_win_wide(const char *text) {
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);
    wchar_t *wide;

    if (length <= 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the host name is not UTF-8");
        return NULL;
    }
    wide = hero_layer_alloc((size_t)length * sizeof *wide);
    if (wide == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    (void)MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, wide, length);
    return wide;
}

/* Whether `host` is an address, IPv4's four numbers or anything holding a
 * `:` as IPv6 does, which SNI never names (RFC 6066 § 3). A host name's last
 * label is never all digits, so the test needs nothing finer. */
static bool hero_tls_win_is_address(const char *host) {
    int dots = 0;

    if (strchr(host, ':') != NULL) {
        return true;
    }
    for (const char *c = host; *c != '\0'; c += 1) {
        if (*c == '.') {
            dots += 1;
        } else if (*c < '0' || *c > '9') {
            return false;
        }
    }
    return dots == 3;
}

/* The credentials of one side: `SCH_CREDENTIALS` first, `SCHANNEL_CRED`
 * where Windows refuses it; false said where neither is taken. */
static bool hero_tls_win_credentials(hero_tls_config *c) {
    SCH_CREDENTIALS modern;
    TLS_PARAMETERS parameters;
    SCHANNEL_CRED legacy;
    TimeStamp expiry;
    DWORD flags = c->server ? SCH_USE_STRONG_CRYPTO : SCH_CRED_MANUAL_CRED_VALIDATION | SCH_CRED_NO_DEFAULT_CREDS | SCH_USE_STRONG_CRYPTO;
    DWORD allowed = c->server ? SP_PROT_TLS1_2_SERVER | SP_PROT_TLS1_3_SERVER : SP_PROT_TLS1_2_CLIENT | SP_PROT_TLS1_3_CLIENT;
    SECURITY_STATUS status;

    memset(&modern, 0, sizeof modern);
    memset(&parameters, 0, sizeof parameters);
    parameters.grbitDisabledProtocols = ~allowed;
    modern.dwVersion = SCH_CREDENTIALS_VERSION;
    modern.dwFlags = flags;
    modern.cTlsParameters = 1;
    modern.pTlsParameters = &parameters;
    if (c->leaf != NULL) {
        modern.cCreds = 1;
        modern.paCred = &c->leaf;
    }
    status = AcquireCredentialsHandleW(NULL, (LPWSTR)(uintptr_t)UNISP_NAME_W, c->server ? SECPKG_CRED_INBOUND : SECPKG_CRED_OUTBOUND, NULL, &modern, NULL, NULL, &c->credentials, &expiry);
    if (status == SEC_E_OK) {
        return true;
    }
    memset(&legacy, 0, sizeof legacy);
    legacy.dwVersion = SCHANNEL_CRED_VERSION;
    legacy.dwFlags = flags;
    legacy.grbitEnabledProtocols = c->server ? SP_PROT_TLS1_2_SERVER : SP_PROT_TLS1_2_CLIENT;
    if (c->leaf != NULL) {
        legacy.cCreds = 1;
        legacy.paCred = &c->leaf;
    }
    status = AcquireCredentialsHandleW(NULL, (LPWSTR)(uintptr_t)UNISP_NAME_W, c->server ? SECPKG_CRED_INBOUND : SECPKG_CRED_OUTBOUND, NULL, &legacy, NULL, NULL, &c->credentials, &expiry);
    if (status == SEC_E_OK) {
        return true;
    }
    hero_tls_win_said(c->server ? HERO_TLS_INVALID : HERO_TLS_OTHER, (DWORD)status, "SChannel took no credentials");
    return false;
}

/* The ALPN list in the form SChannel takes. */
static void hero_tls_win_alpn(hero_tls_config *c, const unsigned char *alpn, size_t alpn_count) {
    SEC_APPLICATION_PROTOCOLS *protocols = (SEC_APPLICATION_PROTOCOLS *)(void *)c->alpn;
    size_t room = sizeof c->alpn - offsetof(SEC_APPLICATION_PROTOCOLS, ProtocolLists) - offsetof(SEC_APPLICATION_PROTOCOL_LIST, ProtocolList);

    c->alpn_count = 0;
    if (alpn_count == 0 || alpn_count > room) {
        return;
    }
    memset(c->alpn, 0, sizeof c->alpn);
    protocols->ProtocolListsSize = (unsigned long)(offsetof(SEC_APPLICATION_PROTOCOL_LIST, ProtocolList) + alpn_count);
    protocols->ProtocolLists[0].ProtoNegoExt = SecApplicationProtocolNegotiationExt_ALPN;
    protocols->ProtocolLists[0].ProtocolListSize = (unsigned short)alpn_count;
    memcpy(protocols->ProtocolLists[0].ProtocolList, alpn, alpn_count);
    c->alpn_count = (unsigned long)(offsetof(SEC_APPLICATION_PROTOCOLS, ProtocolLists) + protocols->ProtocolListsSize);
}

static hero_tls_config *hero_tls_win_config_new(bool server, int64_t checks) {
    hero_tls_config *c = hero_layer_alloc_zero(1, sizeof *c);

    if (c == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    SecInvalidateHandle(&c->credentials);
    c->server = server;
    c->checks = checks;
    c->refs = 1;
    return c;
}

static void hero_tls_config_drop(hero_tls_config *c) {
    if (c == NULL || __atomic_sub_fetch(&c->refs, 1, __ATOMIC_ACQ_REL) != 0) {
        return;
    }
    if (SecIsValidHandle(&c->credentials)) {
        (void)FreeCredentialsHandle(&c->credentials);
    }
    if (c->chain_engine != NULL) {
        CertFreeCertificateChainEngine(c->chain_engine);
    }
    if (c->roots != NULL) {
        (void)CertCloseStore(c->roots, 0);
    }
    if (c->leaf != NULL) {
        (void)CertFreeCertificateContext(c->leaf);
    }
    if (c->chain != NULL) {
        (void)CertCloseStore(c->chain, 0);
    }
    if (c->key != 0 && c->key_name[0] != L'\0') {
        /* The key's file in the user's key store goes with the config. */
        (void)NCryptDeleteKey(c->key, 0);
    } else if (c->key != 0) {
        (void)NCryptFreeObject(c->key);
    }
    if (c->provider != 0) {
        (void)NCryptFreeObject(c->provider);
    }
    hero_layer_free(c);
}

static hero_tls_config *hero_tls_config_client(const hero_tls_certs *roots, const unsigned char *alpn, size_t alpn_count, int64_t checks) {
    hero_tls_config *c = hero_tls_win_config_new(false, checks);

    if (c == NULL) {
        return NULL;
    }
    if (checks == HERO_TLS_CHECK_ALL && roots->count > 0) {
        CERT_CHAIN_ENGINE_CONFIG engine;

        c->roots = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, 0, CERT_STORE_CREATE_NEW_FLAG, NULL);
        if (c->roots == NULL) {
            hero_tls_win_said(HERO_TLS_LIMIT, GetLastError(), "Windows made no store for the roots");
            hero_tls_config_drop(c);
            return NULL;
        }
        for (size_t i = 0; i < roots->count; i += 1) {
            if (!CertAddEncodedCertificateToStore(c->roots, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, roots->each[i].bytes, (DWORD)roots->each[i].count, CERT_STORE_ADD_ALWAYS, NULL)) {
                hero_tls_win_said(HERO_TLS_INVALID, GetLastError(), "a root given is no certificate Windows reads");
                hero_tls_config_drop(c);
                return NULL;
            }
        }
        /* The roots given are the only ones the chain may end at. */
        memset(&engine, 0, sizeof engine);
        engine.cbSize = sizeof engine;
        engine.hExclusiveRoot = c->roots;
        if (!CertCreateCertificateChainEngine(&engine, &c->chain_engine)) {
            hero_tls_win_said(HERO_TLS_OTHER, GetLastError(), "Windows made no chain engine for the roots");
            hero_tls_config_drop(c);
            return NULL;
        }
    }
    hero_tls_win_alpn(c, alpn, alpn_count);
    if (!hero_tls_win_credentials(c)) {
        hero_tls_config_drop(c);
        return NULL;
    }
    return c;
}

/* The server's key, imported into the Microsoft Software Key Storage
 * Provider from its PKCS #8 under a name of its own, and tied to the leaf
 * by `CERT_KEY_PROV_INFO_PROP_ID`. SChannel signs a server's handshake in
 * the system's security process, which reaches a key by its name: MEASURED
 * on the box (Windows 10.0.26100, 2026-11-16), an ephemeral key tied by
 * `CERT_NCRYPT_KEY_HANDLE_PROP_ID` is refused by `AcquireCredentialsHandleW`,
 * `SEC_E_NO_CREDENTIALS` (0x8009030E), with `SCH_CREDENTIALS` and with
 * `SCHANNEL_CRED` alike. So the key is persisted, as a file of the user's
 * key store, under a name no other process or config takes (the process,
 * the config's address, the performance counter), for as long as the
 * config lives: `hero_tls_config_drop` deletes it. A process killed before
 * leaves the file behind, named `heroes-tls-...`. */
static bool hero_tls_win_key(hero_tls_config *c, const hero_tls_key *key) {
    LARGE_INTEGER now;
    NCryptBuffer name;
    NCryptBufferDesc names;
    CRYPT_KEY_PROV_INFO info;
    SECURITY_STATUS status = NCryptOpenStorageProvider(&c->provider, MS_KEY_STORAGE_PROVIDER, 0);

    if (status != ERROR_SUCCESS) {
        hero_tls_win_said(HERO_TLS_OTHER, (DWORD)status, "Windows opened no key storage provider");
        return false;
    }
    (void)QueryPerformanceCounter(&now);
    (void)swprintf(c->key_name, sizeof c->key_name / sizeof c->key_name[0], L"heroes-tls-%lu-%llx-%llx", (unsigned long)GetCurrentProcessId(), (unsigned long long)(uintptr_t)c, (unsigned long long)now.QuadPart);
    name.BufferType = NCRYPTBUFFER_PKCS_KEY_NAME;
    name.pvBuffer = c->key_name;
    name.cbBuffer = (ULONG)((wcslen(c->key_name) + 1) * sizeof(wchar_t));
    names.ulVersion = NCRYPTBUFFER_VERSION;
    names.cBuffers = 1;
    names.pBuffers = &name;
    status = NCryptImportKey(c->provider, 0, NCRYPT_PKCS8_PRIVATE_KEY_BLOB, &names, &c->key, key->pkcs8.bytes, (DWORD)key->pkcs8.count, NCRYPT_OVERWRITE_KEY_FLAG);
    if (status != ERROR_SUCCESS) {
        c->key = 0;
        c->key_name[0] = L'\0';
        hero_tls_win_said(HERO_TLS_INVALID, (DWORD)status, "Windows could not read the key");
        return false;
    }
    memset(&info, 0, sizeof info);
    info.pwszContainerName = c->key_name;
    info.pwszProvName = (LPWSTR)(uintptr_t)MS_KEY_STORAGE_PROVIDER;
    if (!CertSetCertificateContextProperty(c->leaf, CERT_KEY_PROV_INFO_PROP_ID, 0, &info)) {
        hero_tls_win_said(HERO_TLS_OTHER, GetLastError(), "Windows did not tie the key to the certificate");
        return false;
    }
    return true;
}

/* Whether the key is the certificate's: the key signs a digest and the
 * certificate's public key, read by `CryptImportPublicKeyInfoEx2`, verifies
 * it. SChannel takes a mismatched pair and fails only at a handshake, so the
 * listener asks first and refuses it `HERO_TLS_INVALID`. */
static bool hero_tls_win_key_matches(hero_tls_config *c) {
    BCRYPT_KEY_HANDLE public_key = NULL;
    unsigned char digest[32];
    unsigned char signature[1024];
    DWORD signed_count = 0;
    BCRYPT_PKCS1_PADDING_INFO pkcs1;
    void *padding = NULL;
    DWORD padding_flag = 0;
    SECURITY_STATUS status;
    NTSTATUS verified;
    bool rsa = strcmp(c->leaf->pCertInfo->SubjectPublicKeyInfo.Algorithm.pszObjId, szOID_RSA_RSA) == 0;

    memset(digest, 0x5A, sizeof digest);
    if (rsa) {
        pkcs1.pszAlgId = BCRYPT_SHA256_ALGORITHM;
        padding = &pkcs1;
        padding_flag = BCRYPT_PAD_PKCS1;
    }
    status = NCryptSignHash(c->key, padding, digest, sizeof digest, signature, sizeof signature, &signed_count, padding_flag);
    if (status != ERROR_SUCCESS) {
        hero_tls_win_said(HERO_TLS_INVALID, (DWORD)status, "the key signs nothing");
        return false;
    }
    if (!CryptImportPublicKeyInfoEx2(X509_ASN_ENCODING, &c->leaf->pCertInfo->SubjectPublicKeyInfo, 0, NULL, &public_key)) {
        hero_tls_win_said(HERO_TLS_INVALID, GetLastError(), "Windows could not read the certificate's key");
        return false;
    }
    verified = BCryptVerifySignature(public_key, padding, digest, sizeof digest, signature, signed_count, padding_flag);
    (void)BCryptDestroyKey(public_key);
    if (verified != 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the key is not the certificate's");
        return false;
    }
    return true;
}

static hero_tls_config *hero_tls_config_server(const hero_tls_certs *chain, const hero_tls_key *key, const unsigned char *alpn, size_t alpn_count) {
    hero_tls_config *c = hero_tls_win_config_new(true, HERO_TLS_CHECK_ALL);

    if (c == NULL) {
        return NULL;
    }
    /* The chain in one store, which the leaf's context belongs to, so the
     * chain SChannel builds to send finds the authorities given there. */
    c->chain = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, 0, CERT_STORE_CREATE_NEW_FLAG, NULL);
    if (c->chain == NULL) {
        hero_tls_win_said(HERO_TLS_LIMIT, GetLastError(), "Windows made no store for the chain");
        hero_tls_config_drop(c);
        return NULL;
    }
    for (size_t i = 0; i < chain->count; i += 1) {
        if (!CertAddEncodedCertificateToStore(c->chain, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, chain->each[i].bytes, (DWORD)chain->each[i].count, CERT_STORE_ADD_ALWAYS, i == 0 ? &c->leaf : NULL)) {
            hero_tls_win_said(HERO_TLS_INVALID, GetLastError(), "a certificate of the chain is no certificate Windows reads");
            hero_tls_config_drop(c);
            return NULL;
        }
    }
    if (!hero_tls_win_key(c, key) || !hero_tls_win_key_matches(c)) {
        hero_tls_config_drop(c);
        return NULL;
    }
    hero_tls_win_alpn(c, alpn, alpn_count);
    if (!hero_tls_win_credentials(c)) {
        hero_tls_config_drop(c);
        return NULL;
    }
    return c;
}

static hero_tls_engine *hero_tls_engine_new(hero_tls_config *c, const char *host) {
    hero_tls_engine *e = hero_layer_alloc_zero(1, sizeof *e);

    if (e == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    SecInvalidateHandle(&e->context);
    e->server = c->server;
    e->version = -1;
    if (!c->server) {
        e->host = hero_tls_win_wide(host);
        if (e->host == NULL) {
            hero_layer_free(e);
            return NULL;
        }
        if (!hero_tls_win_is_address(host)) {
            e->sni = e->host;
        }
    }
    (void)__atomic_add_fetch(&c->refs, 1, __ATOMIC_ACQ_REL);
    e->config = c;
    return e;
}

static void hero_tls_engine_free(hero_tls_engine *e) {
    if (e == NULL) {
        return;
    }
    if (e->have_context) {
        (void)DeleteSecurityContext(&e->context);
    }
    hero_layer_free(e->host);
    hero_layer_free(e->in.bytes);
    hero_layer_free(e->out.bytes);
    hero_layer_free(e->plain.bytes);
    hero_layer_free(e->seal);
    hero_tls_config_drop(e->config);
    hero_layer_free(e);
}

/* The kind of a refused chain, by the policy's error. */
static int64_t hero_tls_win_policy_kind(DWORD error) {
    switch (error) {
    case (DWORD)CERT_E_EXPIRED:
    case (DWORD)CERT_E_VALIDITYPERIODNESTING:
        return HERO_TLS_CERT_EXPIRED;
    case (DWORD)CERT_E_CN_NO_MATCH:
        return HERO_TLS_CERT_HOST;
    case (DWORD)CERT_E_UNTRUSTEDROOT:
    case (DWORD)CERT_E_CHAINING:
    case (DWORD)CERT_E_UNTRUSTEDTESTROOT:
        return HERO_TLS_CERT_UNTRUSTED;
    case (DWORD)CRYPT_E_REVOKED:
        return HERO_TLS_CERT_REVOKED;
    default:
        return HERO_TLS_CERT_INVALID;
    }
}

/* The peer's certificate checked: false said where it is refused. */
static bool hero_tls_win_check(hero_tls_engine *e) {
    PCCERT_CONTEXT remote = NULL;
    PCCERT_CHAIN_CONTEXT chain = NULL;
    CERT_CHAIN_PARA chain_para;
    LPSTR usages[] = { (LPSTR)(uintptr_t)szOID_PKIX_KP_SERVER_AUTH };
    SSL_EXTRA_CERT_CHAIN_POLICY_PARA ssl;
    CERT_CHAIN_POLICY_PARA policy;
    CERT_CHAIN_POLICY_STATUS status;
    SECURITY_STATUS queried = QueryContextAttributesW(&e->context, SECPKG_ATTR_REMOTE_CERT_CONTEXT, &remote);

    if (queried != SEC_E_OK || remote == NULL) {
        hero_tls_win_said(HERO_TLS_CERT_INVALID, (DWORD)queried, "the peer presented no certificate");
        return false;
    }
    memset(&chain_para, 0, sizeof chain_para);
    chain_para.cbSize = sizeof chain_para;
    chain_para.RequestedUsage.dwType = USAGE_MATCH_TYPE_AND;
    chain_para.RequestedUsage.Usage.cUsageIdentifier = 1;
    chain_para.RequestedUsage.Usage.rgpszUsageIdentifier = usages;
    if (!CertGetCertificateChain(e->config->chain_engine, remote, NULL, remote->hCertStore, &chain_para, 0, NULL, &chain)) {
        hero_tls_win_said(HERO_TLS_CERT_INVALID, GetLastError(), "the peer's certificate was refused: Windows built no chain for it");
        (void)CertFreeCertificateContext(remote);
        return false;
    }
    memset(&ssl, 0, sizeof ssl);
    ssl.cbSize = sizeof ssl;
    ssl.dwAuthType = AUTHTYPE_SERVER;
    ssl.pwszServerName = e->host;
    memset(&policy, 0, sizeof policy);
    policy.cbSize = sizeof policy;
    policy.pvExtraPolicyPara = &ssl;
    memset(&status, 0, sizeof status);
    status.cbSize = sizeof status;
    if (!CertVerifyCertificateChainPolicy(CERT_CHAIN_POLICY_SSL, chain, &policy, &status)) {
        status.dwError = GetLastError();
    }
    CertFreeCertificateChain(chain);
    (void)CertFreeCertificateContext(remote);
    if (status.dwError != 0) {
        hero_tls_win_said(hero_tls_win_policy_kind(status.dwError), status.dwError, "the peer's certificate was refused");
        return false;
    }
    return true;
}

/* What the handshake chose, read once it has ended. */
static void hero_tls_win_chosen(hero_tls_engine *e) {
    SecPkgContext_ConnectionInfo info;
    SecPkgContext_ApplicationProtocol chosen;

    (void)QueryContextAttributesW(&e->context, SECPKG_ATTR_STREAM_SIZES, &e->sizes);
    if (QueryContextAttributesW(&e->context, SECPKG_ATTR_CONNECTION_INFO, &info) == SEC_E_OK) {
        e->version = (info.dwProtocol & (SP_PROT_TLS1_3_CLIENT | SP_PROT_TLS1_3_SERVER)) != 0 ? 13 : (info.dwProtocol & (SP_PROT_TLS1_2_CLIENT | SP_PROT_TLS1_2_SERVER)) != 0 ? 12 : -1;
    }
    e->protocol[0] = '\0';
    memset(&chosen, 0, sizeof chosen);
    if (e->config->alpn_count > 0 && QueryContextAttributesW(&e->context, SECPKG_ATTR_APPLICATION_PROTOCOL, &chosen) == SEC_E_OK && chosen.ProtoNegoStatus == SecApplicationProtocolNegotiationStatus_Success && chosen.ProtoNegoExt == SecApplicationProtocolNegotiationExt_ALPN) {
        /* A protocol id is at most 255 bytes, which `e->protocol` holds with its zero. */
        memcpy(e->protocol, chosen.ProtocolId, chosen.ProtocolIdSize);
        e->protocol[chosen.ProtocolIdSize] = '\0';
    }
}

/* One call of the handshake's function, `InitializeSecurityContextW` or
 * `AcceptSecurityContext`, over the bytes held: what it wrote is kept for
 * the peer, what it read taken, what it left kept. Answers its status. */
static SECURITY_STATUS hero_tls_win_step(hero_tls_engine *e, bool control) {
    SecBuffer in[3];
    SecBuffer out[2];
    SecBufferDesc in_desc;
    SecBufferDesc out_desc;
    unsigned long in_count = 0;
    unsigned long attributes = 0;
    size_t held = hero_tls_win_held(&e->in);
    SECURITY_STATUS status;
    bool first = !e->have_context;

    if (!control && (held > 0 || e->server || e->again)) {
        in[0].BufferType = SECBUFFER_TOKEN;
        in[0].pvBuffer = held > 0 ? e->in.bytes + e->in.at : NULL;
        in[0].cbBuffer = (unsigned long)held;
        in[1].BufferType = SECBUFFER_EMPTY;
        in[1].pvBuffer = NULL;
        in[1].cbBuffer = 0;
        in_count = 2;
    }
    /* The ALPN list goes with the first call: it is the hello's. */
    if (!control && first && e->config->alpn_count > 0) {
        in[in_count].BufferType = SECBUFFER_APPLICATION_PROTOCOLS;
        in[in_count].pvBuffer = e->config->alpn;
        in[in_count].cbBuffer = e->config->alpn_count;
        in_count += 1;
    }
    in_desc.ulVersion = SECBUFFER_VERSION;
    in_desc.cBuffers = in_count;
    in_desc.pBuffers = in;
    out[0].BufferType = SECBUFFER_TOKEN;
    out[0].pvBuffer = NULL;
    out[0].cbBuffer = 0;
    out[1].BufferType = SECBUFFER_ALERT;
    out[1].pvBuffer = NULL;
    out[1].cbBuffer = 0;
    out_desc.ulVersion = SECBUFFER_VERSION;
    out_desc.cBuffers = 2;
    out_desc.pBuffers = out;
    if (e->server) {
        status = AcceptSecurityContext(&e->config->credentials, first ? NULL : &e->context, in_count > 0 ? &in_desc : NULL, ASC_REQ_SEQUENCE_DETECT | ASC_REQ_REPLAY_DETECT | ASC_REQ_CONFIDENTIALITY | ASC_REQ_EXTENDED_ERROR | ASC_REQ_ALLOCATE_MEMORY | ASC_REQ_STREAM, 0, &e->context, &out_desc, &attributes, NULL);
    } else {
        status = InitializeSecurityContextW(&e->config->credentials, first ? NULL : &e->context, e->sni, ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY | ISC_REQ_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM | ISC_REQ_MANUAL_CRED_VALIDATION, 0, 0, in_count > 0 ? &in_desc : NULL, 0, &e->context, &out_desc, &attributes, NULL);
    }
    if (first && SecIsValidHandle(&e->context) && status != SEC_E_INCOMPLETE_MESSAGE) {
        e->have_context = true;
    }
    for (int i = 0; i < 2; i += 1) {
        if (out[i].pvBuffer != NULL) {
            if (out[i].cbBuffer > 0 && !hero_tls_win_append(&e->out, out[i].pvBuffer, out[i].cbBuffer)) {
                status = SEC_E_INSUFFICIENT_MEMORY;
            }
            (void)FreeContextBuffer(out[i].pvBuffer);
        }
    }
    if (status == SEC_E_INCOMPLETE_MESSAGE || control || in_count < 2) {
        return status;
    }
    /* What the call left for the next: SECBUFFER_EXTRA names the last
     * bytes of the input; with none, it took them all. */
    if (in[1].BufferType == SECBUFFER_EXTRA && in[1].cbBuffer > 0 && in[1].cbBuffer <= held) {
        hero_tls_win_take(&e->in, held - in[1].cbBuffer);
    } else {
        hero_tls_win_take(&e->in, held);
    }
    return status;
}

/* The handshake, or a post-handshake message's answer, stepped while the
 * bytes held allow. */
static int hero_tls_win_handshake(hero_tls_engine *e) {
    for (;;) {
        SECURITY_STATUS status;

        /* Nothing to hand the function: a server before the hello, a
         * client after its own, both wait for the peer's bytes. */
        if (hero_tls_win_held(&e->in) == 0 && !e->again && (e->server || e->have_context)) {
            return HERO_TLS_STEP_MORE;
        }
        status = hero_tls_win_step(e, false);

        switch (status) {
        case SEC_E_OK:
            if (!e->done) {
                hero_tls_win_chosen(e);
                if (!e->server && e->config->checks == HERO_TLS_CHECK_ALL && !hero_tls_win_check(e)) {
                    return HERO_TLS_STEP_FAILED;
                }
                e->done = true;
            }
            e->again = false;
            return HERO_TLS_STEP_DONE;
        case SEC_I_CONTINUE_NEEDED:
            if (hero_tls_win_held(&e->in) == 0) {
                return HERO_TLS_STEP_MORE;
            }
            break;
        case SEC_E_INCOMPLETE_MESSAGE:
            return HERO_TLS_STEP_MORE;
        case SEC_I_INCOMPLETE_CREDENTIALS:
            /* The server asked for a certificate this client has none of:
             * asked again, SChannel goes on without one. */
            break;
        default:
            hero_tls_win_said(HERO_TLS_HANDSHAKE, (DWORD)status, "the TLS handshake failed");
            return HERO_TLS_STEP_FAILED;
        }
    }
}

static int hero_tls_engine_handshake(hero_tls_engine *e) {
    return hero_tls_win_handshake(e);
}

static bool hero_tls_engine_feed(hero_tls_engine *e, const unsigned char *bytes, size_t count) {
    return hero_tls_win_append(&e->in, bytes, count);
}

static size_t hero_tls_engine_output(hero_tls_engine *e, unsigned char *buffer, size_t count) {
    size_t held = hero_tls_win_held(&e->out);
    size_t taken = held < count ? held : count;

    if (taken > 0) {
        memcpy(buffer, e->out.bytes + e->out.at, taken);
        hero_tls_win_take(&e->out, taken);
    }
    return taken;
}

static bool hero_tls_engine_pending(hero_tls_engine *e) {
    return hero_tls_win_held(&e->out) > 0;
}

/* Plaintext opened earlier, given first. */
static size_t hero_tls_win_give(hero_tls_engine *e, unsigned char *buffer, size_t count) {
    size_t held = hero_tls_win_held(&e->plain);
    size_t given = held < count ? held : count;

    if (given > 0) {
        memcpy(buffer, e->plain.bytes + e->plain.at, given);
        hero_tls_win_take(&e->plain, given);
    }
    return given;
}

static int64_t hero_tls_engine_open(hero_tls_engine *e, unsigned char *buffer, size_t count) {
    for (;;) {
        SecBuffer buffers[4];
        SecBufferDesc desc;
        SECURITY_STATUS status;
        size_t held;
        size_t given = hero_tls_win_give(e, buffer, count);
        unsigned char *extra = NULL;
        size_t extra_count = 0;

        if (given > 0) {
            return (int64_t)given;
        }
        if (e->ended) {
            return 0;
        }
        if (e->again) {
            int step = hero_tls_win_handshake(e);

            if (step != HERO_TLS_STEP_DONE) {
                return step;
            }
            continue;
        }
        held = hero_tls_win_held(&e->in);
        if (held == 0) {
            return HERO_TLS_STEP_MORE;
        }
        buffers[0].BufferType = SECBUFFER_DATA;
        buffers[0].pvBuffer = e->in.bytes + e->in.at;
        buffers[0].cbBuffer = (unsigned long)held;
        for (int i = 1; i < 4; i += 1) {
            buffers[i].BufferType = SECBUFFER_EMPTY;
            buffers[i].pvBuffer = NULL;
            buffers[i].cbBuffer = 0;
        }
        desc.ulVersion = SECBUFFER_VERSION;
        desc.cBuffers = 4;
        desc.pBuffers = buffers;
        status = DecryptMessage(&e->context, &desc, 0, NULL);
        if (status == SEC_E_INCOMPLETE_MESSAGE) {
            return HERO_TLS_STEP_MORE;
        }
        if (status != SEC_E_OK && status != SEC_I_RENEGOTIATE && status != SEC_I_CONTEXT_EXPIRED) {
            hero_tls_win_said(HERO_TLS_OTHER, (DWORD)status, "a TLS record could not be read");
            return HERO_TLS_STEP_FAILED;
        }
        /* The plaintext lies inside the input, decrypted in place: it is
         * copied out before the bytes after it are moved. */
        for (int i = 1; i < 4; i += 1) {
            if (buffers[i].BufferType == SECBUFFER_DATA && buffers[i].cbBuffer > 0) {
                size_t plain = buffers[i].cbBuffer;
                size_t direct = plain < count ? plain : count;

                memcpy(buffer, buffers[i].pvBuffer, direct);
                given = direct;
                if (plain > direct && !hero_tls_win_append(&e->plain, (unsigned char *)buffers[i].pvBuffer + direct, plain - direct)) {
                    return HERO_TLS_STEP_FAILED;
                }
            } else if (buffers[i].BufferType == SECBUFFER_EXTRA && buffers[i].cbBuffer > 0) {
                extra = buffers[i].pvBuffer;
                extra_count = buffers[i].cbBuffer;
            }
        }
        if (extra != NULL && extra_count <= held && extra == e->in.bytes + e->in.at + (held - extra_count)) {
            hero_tls_win_take(&e->in, held - extra_count);
        } else if (extra != NULL && extra_count <= held) {
            /* Extra bytes SChannel names elsewhere than the input's end:
             * moved to the input's start. */
            memmove(e->in.bytes, extra, extra_count);
            e->in.at = 0;
            e->in.count = extra_count;
        } else {
            hero_tls_win_take(&e->in, held);
        }
        if (status == SEC_I_CONTEXT_EXPIRED) {
            e->ended = true;
        } else if (status == SEC_I_RENEGOTIATE) {
            /* A message of the handshake after its end (TLS 1.3's tickets
             * and key updates): the bytes after it go to the handshake's
             * function, which answers it, and the records go on. */
            e->again = true;
        }
        if (given > 0) {
            return (int64_t)given;
        }
    }
}

static int64_t hero_tls_engine_seal(hero_tls_engine *e, const unsigned char *data, size_t count) {
    size_t most = e->sizes.cbMaximumMessage == 0 ? HERO_TLS_RECORD_MOST : e->sizes.cbMaximumMessage;
    size_t step = count < most ? count : most;
    SecBuffer buffers[4];
    SecBufferDesc desc;
    SECURITY_STATUS status;

    if (e->seal == NULL) {
        e->seal = hero_layer_alloc((size_t)e->sizes.cbHeader + most + e->sizes.cbTrailer);
        if (e->seal == NULL) {
            hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
            return HERO_TLS_STEP_FAILED;
        }
    }
    memcpy(e->seal + e->sizes.cbHeader, data, step);
    buffers[0].BufferType = SECBUFFER_STREAM_HEADER;
    buffers[0].pvBuffer = e->seal;
    buffers[0].cbBuffer = e->sizes.cbHeader;
    buffers[1].BufferType = SECBUFFER_DATA;
    buffers[1].pvBuffer = e->seal + e->sizes.cbHeader;
    buffers[1].cbBuffer = (unsigned long)step;
    buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
    buffers[2].pvBuffer = e->seal + e->sizes.cbHeader + step;
    buffers[2].cbBuffer = e->sizes.cbTrailer;
    buffers[3].BufferType = SECBUFFER_EMPTY;
    buffers[3].pvBuffer = NULL;
    buffers[3].cbBuffer = 0;
    desc.ulVersion = SECBUFFER_VERSION;
    desc.cBuffers = 4;
    desc.pBuffers = buffers;
    status = EncryptMessage(&e->context, 0, &desc, 0);
    if (status != SEC_E_OK) {
        hero_tls_win_said(HERO_TLS_OTHER, (DWORD)status, "a TLS record could not be written");
        return HERO_TLS_STEP_FAILED;
    }
    if (!hero_tls_win_append(&e->out, e->seal, (size_t)buffers[0].cbBuffer + buffers[1].cbBuffer + buffers[2].cbBuffer)) {
        return HERO_TLS_STEP_FAILED;
    }
    return (int64_t)step;
}

static void hero_tls_engine_close_notify(hero_tls_engine *e) {
    DWORD type = SCHANNEL_SHUTDOWN;
    SecBuffer token;
    SecBufferDesc desc;

    if (!e->have_context || !e->done) {
        return;
    }
    token.BufferType = SECBUFFER_TOKEN;
    token.pvBuffer = &type;
    token.cbBuffer = sizeof type;
    desc.ulVersion = SECBUFFER_VERSION;
    desc.cBuffers = 1;
    desc.pBuffers = &token;
    if (ApplyControlToken(&e->context, &desc) == SEC_E_OK) {
        /* The alert itself is what the handshake's function writes next. */
        (void)hero_tls_win_step(e, true);
    }
}

static int64_t hero_tls_engine_version(hero_tls_engine *e) {
    return e->version;
}

static size_t hero_tls_engine_protocol(hero_tls_engine *e, char *buffer, size_t count) {
    return hero_tls_copy_text(buffer, count, e->protocol);
}

/* ==== END WINDOWS ENGINE ==== */
#endif

/* ============================================================================
 * THE LINUX ENGINE: OpenSSL 3, opened with `dlopen`.
 *
 * WHY `dlopen` AND NOT A LINK, AND WHY IT IS SAFE. The layer's archive is
 * compiled for every program a machine builds, so a member that included
 * `<openssl/ssl.h>` would stop every build on a machine without OpenSSL's
 * headers, and one linked `-lssl` every program on one without the library.
 * So the member names no OpenSSL header and no library: it declares the
 * functions it calls below, with OpenSSL 3's own signatures (its ABI is
 * kept across 3.x under the one `libssl.so.3`), and the first call that
 * needs TLS opens `libssl.so.3` and `libcrypto.so.3` once, `RTLD_LOCAL` so
 * nothing of theirs takes another library's name, and finds every function
 * by `dlsym`. One missing is the library missing: then every call answers
 * `HERO_TLS_UNSUPPORTED`, and none of the functions is ever called. The
 * library is never closed, so no pointer outlives it. A weak symbol was the
 * other route, and it would have linked the program to `libssl` anyway.
 * ========================================================================== */
#if !defined(_WIN32) && !defined(__APPLE__)

/* OpenSSL's numbers this engine reads, as its manual pages name them. */
#define HERO_SSL_ERROR_NONE 0
#define HERO_SSL_ERROR_SSL 1
#define HERO_SSL_ERROR_WANT_READ 2
#define HERO_SSL_ERROR_WANT_WRITE 3
#define HERO_SSL_ERROR_SYSCALL 5
#define HERO_SSL_ERROR_ZERO_RETURN 6
#define HERO_SSL_VERIFY_NONE 0
#define HERO_SSL_VERIFY_PEER 1
#define HERO_SSL_CTRL_SET_TLSEXT_HOSTNAME 55
#define HERO_SSL_CTRL_CHAIN_CERT 89
#define HERO_SSL_CTRL_SET_MIN_PROTO_VERSION 123
#define HERO_TLSEXT_NAMETYPE_HOST_NAME 0
#define HERO_TLS1_2_VERSION 0x0303
#define HERO_TLS1_3_VERSION 0x0304
#define HERO_SSL_TLSEXT_ERR_OK 0
#define HERO_SSL_TLSEXT_ERR_NOACK 3
#define HERO_BIO_CTRL_PENDING 10
#define HERO_BIO_C_SET_BUF_MEM_EOF_RETURN 130
#define HERO_X509_V_OK 0
#define HERO_X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT 2
#define HERO_X509_V_ERR_CERT_SIGNATURE_FAILURE 7
#define HERO_X509_V_ERR_CERT_NOT_YET_VALID 9
#define HERO_X509_V_ERR_CERT_HAS_EXPIRED 10
#define HERO_X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT 18
#define HERO_X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN 19
#define HERO_X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY 20
#define HERO_X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE 21
#define HERO_X509_V_ERR_CERT_REVOKED 23
#define HERO_X509_V_ERR_CERT_UNTRUSTED 27
#define HERO_X509_V_ERR_HOSTNAME_MISMATCH 62
#define HERO_X509_V_ERR_IP_ADDRESS_MISMATCH 64

typedef int (*hero_tls_alpn_pick_fn)(void *ssl, const unsigned char **out, unsigned char *out_count, const unsigned char *in, unsigned int in_count, void *arg);

/* The functions this engine calls, found once. */
typedef struct hero_tls_openssl {
    const void *(*TLS_client_method)(void);
    const void *(*TLS_server_method)(void);
    void *(*SSL_CTX_new)(const void *method);
    void (*SSL_CTX_free)(void *ctx);
    long (*SSL_CTX_ctrl)(void *ctx, int cmd, long larg, void *parg);
    int (*SSL_CTX_set_default_verify_paths)(void *ctx);
    void (*SSL_CTX_set_verify)(void *ctx, int mode, void *callback);
    void *(*SSL_CTX_get_cert_store)(const void *ctx);
    int (*SSL_CTX_set_alpn_protos)(void *ctx, const unsigned char *protos, unsigned int count);
    void (*SSL_CTX_set_alpn_select_cb)(void *ctx, hero_tls_alpn_pick_fn callback, void *arg);
    int (*SSL_CTX_use_certificate)(void *ctx, void *x509);
    int (*SSL_CTX_use_PrivateKey)(void *ctx, void *pkey);
    int (*SSL_CTX_check_private_key)(const void *ctx);
    void *(*SSL_new)(void *ctx);
    void (*SSL_free)(void *ssl);
    void (*SSL_set_connect_state)(void *ssl);
    void (*SSL_set_accept_state)(void *ssl);
    void (*SSL_set_bio)(void *ssl, void *rbio, void *wbio);
    int (*SSL_do_handshake)(void *ssl);
    int (*SSL_read)(void *ssl, void *buffer, int count);
    int (*SSL_write)(void *ssl, const void *data, int count);
    int (*SSL_shutdown)(void *ssl);
    int (*SSL_get_error)(const void *ssl, int answer);
    int (*SSL_set1_host)(void *ssl, const char *host);
    long (*SSL_ctrl)(void *ssl, int cmd, long larg, void *parg);
    long (*SSL_get_verify_result)(const void *ssl);
    int (*SSL_version)(const void *ssl);
    void (*SSL_get0_alpn_selected)(const void *ssl, const unsigned char **data, unsigned int *count);
    void *(*SSL_get0_param)(void *ssl);
    int (*X509_VERIFY_PARAM_set1_ip_asc)(void *param, const char *ip);
    const void *(*BIO_s_mem)(void);
    void *(*BIO_new)(const void *method);
    int (*BIO_read)(void *bio, void *buffer, int count);
    int (*BIO_write)(void *bio, const void *data, int count);
    long (*BIO_ctrl)(void *bio, int cmd, long larg, void *parg);
    int (*BIO_free)(void *bio);
    void *(*d2i_X509)(void **out, const unsigned char **in, long count);
    void (*X509_free)(void *x509);
    void *(*d2i_AutoPrivateKey)(void **out, const unsigned char **in, long count);
    void (*EVP_PKEY_free)(void *pkey);
    int (*X509_STORE_add_cert)(void *store, void *x509);
    unsigned long (*ERR_get_error)(void);
    void (*ERR_error_string_n)(unsigned long error, char *buffer, size_t count);
    void (*ERR_clear_error)(void);
    const char *(*X509_verify_cert_error_string)(long error);
} hero_tls_openssl;

/* The functions, found once by `hero_tls_load` under `hero_tls_once`, and
 * read only after it: whether they were all found. */
static hero_tls_openssl hero_tls_ssl;
static bool hero_tls_ssl_found;
static pthread_once_t hero_tls_once = PTHREAD_ONCE_INIT;

/* One function by name into `*slot`, from `ssl` or `crypto`; false where
 * neither has it. C11 has no conversion between an object pointer and a
 * function pointer, so the address is copied by its bytes, which POSIX
 * makes the same for both (`dlsym`'s own page). */
static bool hero_tls_find_function(void *ssl, void *crypto, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(ssl, name);

    if (found == NULL && crypto != NULL) {
        found = dlsym(crypto, name);
    }
    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_TLS_FIND(name) ok = ok && hero_tls_find_function(ssl, crypto, #name, &hero_tls_ssl.name, sizeof hero_tls_ssl.name)

static void hero_tls_load(void) {
    static const char *const ssl_names[] = { "libssl.so.3", "libssl.so.4" };
    static const char *const crypto_names[] = { "libcrypto.so.3", "libcrypto.so.4" };
    bool ok = true;
    void *ssl = NULL;
    void *crypto = NULL;

    for (size_t i = 0; i < sizeof ssl_names / sizeof ssl_names[0] && ssl == NULL; i += 1) {
        ssl = dlopen(ssl_names[i], RTLD_NOW | RTLD_LOCAL);
        if (ssl != NULL) {
            crypto = dlopen(crypto_names[i], RTLD_NOW | RTLD_LOCAL);
        }
    }
    if (ssl == NULL) {
        return;
    }
    HERO_TLS_FIND(TLS_client_method);
    HERO_TLS_FIND(TLS_server_method);
    HERO_TLS_FIND(SSL_CTX_new);
    HERO_TLS_FIND(SSL_CTX_free);
    HERO_TLS_FIND(SSL_CTX_ctrl);
    HERO_TLS_FIND(SSL_CTX_set_default_verify_paths);
    HERO_TLS_FIND(SSL_CTX_set_verify);
    HERO_TLS_FIND(SSL_CTX_get_cert_store);
    HERO_TLS_FIND(SSL_CTX_set_alpn_protos);
    HERO_TLS_FIND(SSL_CTX_set_alpn_select_cb);
    HERO_TLS_FIND(SSL_CTX_use_certificate);
    HERO_TLS_FIND(SSL_CTX_use_PrivateKey);
    HERO_TLS_FIND(SSL_CTX_check_private_key);
    HERO_TLS_FIND(SSL_new);
    HERO_TLS_FIND(SSL_free);
    HERO_TLS_FIND(SSL_set_connect_state);
    HERO_TLS_FIND(SSL_set_accept_state);
    HERO_TLS_FIND(SSL_set_bio);
    HERO_TLS_FIND(SSL_do_handshake);
    HERO_TLS_FIND(SSL_read);
    HERO_TLS_FIND(SSL_write);
    HERO_TLS_FIND(SSL_shutdown);
    HERO_TLS_FIND(SSL_get_error);
    HERO_TLS_FIND(SSL_set1_host);
    HERO_TLS_FIND(SSL_ctrl);
    HERO_TLS_FIND(SSL_get_verify_result);
    HERO_TLS_FIND(SSL_version);
    HERO_TLS_FIND(SSL_get0_alpn_selected);
    HERO_TLS_FIND(SSL_get0_param);
    HERO_TLS_FIND(X509_VERIFY_PARAM_set1_ip_asc);
    HERO_TLS_FIND(BIO_s_mem);
    HERO_TLS_FIND(BIO_new);
    HERO_TLS_FIND(BIO_read);
    HERO_TLS_FIND(BIO_write);
    HERO_TLS_FIND(BIO_ctrl);
    HERO_TLS_FIND(BIO_free);
    HERO_TLS_FIND(d2i_X509);
    HERO_TLS_FIND(X509_free);
    HERO_TLS_FIND(d2i_AutoPrivateKey);
    HERO_TLS_FIND(EVP_PKEY_free);
    HERO_TLS_FIND(X509_STORE_add_cert);
    HERO_TLS_FIND(ERR_get_error);
    HERO_TLS_FIND(ERR_error_string_n);
    HERO_TLS_FIND(ERR_clear_error);
    HERO_TLS_FIND(X509_verify_cert_error_string);
    hero_tls_ssl_found = ok;
}

static bool hero_tls_arm_ready(void) {
    (void)pthread_once(&hero_tls_once, hero_tls_load);
    return hero_tls_ssl_found;
}

/* OpenSSL's newest reason for the failure on this thread, as words, the
 * queue emptied. */
static void hero_tls_ssl_said(int64_t kind, const char *what) {
    char reason[256];
    char words[HERO_TLS_WORDS_MOST];
    unsigned long error = hero_tls_ssl.ERR_get_error();
    unsigned long last = error;

    while (error != 0) {
        last = error;
        error = hero_tls_ssl.ERR_get_error();
    }
    if (last != 0) {
        hero_tls_ssl.ERR_error_string_n(last, reason, sizeof reason);
        (void)snprintf(words, sizeof words, "%s: %s", what, reason);
    } else {
        (void)snprintf(words, sizeof words, "%s", what);
    }
    hero_tls_said(kind, (int64_t)(last & 0x7FFFFFFFu), words);
}

struct hero_tls_config {
    void *ctx;
    bool server;
    int64_t checks;
    /* A server's ALPN list, as the protocol carries it. */
    unsigned char alpn[HERO_TLS_PROTOCOLS_MOST + 2];
    size_t alpn_count;
    /* Counted under its own atomics: a listener's and each of its
     * connections'. */
    int64_t refs;
};

struct hero_tls_engine {
    hero_tls_config *config;
    void *ssl;
    /* The two memory BIOs `ssl` owns: what the peer sent, and what it
     * writes for the peer. */
    void *in;
    void *out;
    bool server;
};

/* The first of the server's protocols the client offers, ALPN's choice. */
static int hero_tls_alpn_pick(void *ssl, const unsigned char **out, unsigned char *out_count, const unsigned char *in, unsigned int in_count, void *arg) {
    const hero_tls_config *c = arg;

    (void)ssl;
    for (size_t at = 0; at < c->alpn_count; at += 1u + c->alpn[at]) {
        size_t length = c->alpn[at];

        for (size_t them = 0; them < in_count; them += 1u + in[them]) {
            if (in[them] == length && them + 1 + length <= in_count && memcmp(in + them + 1, c->alpn + at + 1, length) == 0) {
                *out = in + them + 1;
                *out_count = (unsigned char)length;
                return HERO_SSL_TLSEXT_ERR_OK;
            }
        }
    }
    return HERO_SSL_TLSEXT_ERR_NOACK;
}

static hero_tls_config *hero_tls_config_new(bool server, int64_t checks) {
    hero_tls_config *c = hero_layer_alloc_zero(1, sizeof *c);

    if (c == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    hero_tls_ssl.ERR_clear_error();
    c->ctx = hero_tls_ssl.SSL_CTX_new(server ? hero_tls_ssl.TLS_server_method() : hero_tls_ssl.TLS_client_method());
    if (c->ctx == NULL) {
        hero_tls_ssl_said(HERO_TLS_LIMIT, "OpenSSL made no context");
        hero_layer_free(c);
        return NULL;
    }
    if (hero_tls_ssl.SSL_CTX_ctrl(c->ctx, HERO_SSL_CTRL_SET_MIN_PROTO_VERSION, HERO_TLS1_2_VERSION, NULL) != 1) {
        hero_tls_ssl_said(HERO_TLS_OTHER, "OpenSSL refused TLS 1.2 as the least version");
        hero_tls_ssl.SSL_CTX_free(c->ctx);
        hero_layer_free(c);
        return NULL;
    }
    c->server = server;
    c->checks = checks;
    c->refs = 1;
    return c;
}

static void hero_tls_config_drop(hero_tls_config *c) {
    if (c != NULL && __atomic_sub_fetch(&c->refs, 1, __ATOMIC_ACQ_REL) == 0) {
        hero_tls_ssl.SSL_CTX_free(c->ctx);
        hero_layer_free(c);
    }
}

static hero_tls_config *hero_tls_config_client(const hero_tls_certs *roots, const unsigned char *alpn, size_t alpn_count, int64_t checks) {
    hero_tls_config *c = hero_tls_config_new(false, checks);

    if (c == NULL) {
        return NULL;
    }
    if (checks == HERO_TLS_CHECK_ALL) {
        hero_tls_ssl.SSL_CTX_set_verify(c->ctx, HERO_SSL_VERIFY_PEER, NULL);
        if (roots->count == 0) {
            if (hero_tls_ssl.SSL_CTX_set_default_verify_paths(c->ctx) != 1) {
                hero_tls_ssl_said(HERO_TLS_OTHER, "OpenSSL could not read the system's authorities");
                hero_tls_config_drop(c);
                return NULL;
            }
        } else {
            void *store = hero_tls_ssl.SSL_CTX_get_cert_store(c->ctx);

            for (size_t i = 0; i < roots->count; i += 1) {
                const unsigned char *at = roots->each[i].bytes;
                void *x509 = hero_tls_ssl.d2i_X509(NULL, &at, (long)roots->each[i].count);
                int added;

                if (x509 == NULL) {
                    hero_tls_ssl_said(HERO_TLS_INVALID, "a root given is no certificate");
                    hero_tls_config_drop(c);
                    return NULL;
                }
                added = hero_tls_ssl.X509_STORE_add_cert(store, x509);
                hero_tls_ssl.X509_free(x509);
                if (added != 1) {
                    hero_tls_ssl_said(HERO_TLS_INVALID, "OpenSSL refused a root given");
                    hero_tls_config_drop(c);
                    return NULL;
                }
            }
        }
    } else {
        hero_tls_ssl.SSL_CTX_set_verify(c->ctx, HERO_SSL_VERIFY_NONE, NULL);
    }
    if (alpn_count > 0 && hero_tls_ssl.SSL_CTX_set_alpn_protos(c->ctx, alpn, (unsigned int)alpn_count) != 0) {
        hero_tls_ssl_said(HERO_TLS_INVALID, "OpenSSL refused the application protocols");
        hero_tls_config_drop(c);
        return NULL;
    }
    return c;
}

static hero_tls_config *hero_tls_config_server(const hero_tls_certs *chain, const hero_tls_key *key, const unsigned char *alpn, size_t alpn_count) {
    hero_tls_config *c = hero_tls_config_new(true, HERO_TLS_CHECK_ALL);
    const unsigned char *at;
    void *pkey;
    int used;

    if (c == NULL) {
        return NULL;
    }
    hero_tls_ssl.SSL_CTX_set_verify(c->ctx, HERO_SSL_VERIFY_NONE, NULL);
    for (size_t i = 0; i < chain->count; i += 1) {
        void *x509;

        at = chain->each[i].bytes;
        x509 = hero_tls_ssl.d2i_X509(NULL, &at, (long)chain->each[i].count);
        if (x509 == NULL) {
            hero_tls_ssl_said(HERO_TLS_INVALID, "a certificate of the chain is not one");
            hero_tls_config_drop(c);
            return NULL;
        }
        used = i == 0 ? hero_tls_ssl.SSL_CTX_use_certificate(c->ctx, x509) : (int)hero_tls_ssl.SSL_CTX_ctrl(c->ctx, HERO_SSL_CTRL_CHAIN_CERT, 1, x509);
        hero_tls_ssl.X509_free(x509);
        if (used != 1) {
            hero_tls_ssl_said(HERO_TLS_INVALID, "OpenSSL refused a certificate of the chain");
            hero_tls_config_drop(c);
            return NULL;
        }
    }
    at = key->pkcs8.bytes;
    pkey = hero_tls_ssl.d2i_AutoPrivateKey(NULL, &at, (long)key->pkcs8.count);
    if (pkey == NULL) {
        hero_tls_ssl_said(HERO_TLS_INVALID, "OpenSSL could not read the key");
        hero_tls_config_drop(c);
        return NULL;
    }
    used = hero_tls_ssl.SSL_CTX_use_PrivateKey(c->ctx, pkey);
    hero_tls_ssl.EVP_PKEY_free(pkey);
    if (used != 1 || hero_tls_ssl.SSL_CTX_check_private_key(c->ctx) != 1) {
        hero_tls_ssl_said(HERO_TLS_INVALID, "the key is not the certificate's");
        hero_tls_config_drop(c);
        return NULL;
    }
    if (alpn_count > sizeof c->alpn) {
        alpn_count = sizeof c->alpn;
    }
    memcpy(c->alpn, alpn, alpn_count);
    c->alpn_count = alpn_count;
    if (alpn_count > 0) {
        hero_tls_ssl.SSL_CTX_set_alpn_select_cb(c->ctx, hero_tls_alpn_pick, c);
    }
    return c;
}

/* Whether `host` is an address, IPv4 or IPv6, rather than a name. */
static bool hero_tls_is_address(const char *host) {
    unsigned char bytes[16];

    return inet_pton(AF_INET, host, bytes) == 1 || inet_pton(AF_INET6, host, bytes) == 1;
}

static hero_tls_engine *hero_tls_engine_new(hero_tls_config *c, const char *host) {
    hero_tls_engine *e = hero_layer_alloc_zero(1, sizeof *e);

    if (e == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    hero_tls_ssl.ERR_clear_error();
    e->ssl = hero_tls_ssl.SSL_new(c->ctx);
    e->in = hero_tls_ssl.BIO_new(hero_tls_ssl.BIO_s_mem());
    e->out = hero_tls_ssl.BIO_new(hero_tls_ssl.BIO_s_mem());
    if (e->ssl == NULL || e->in == NULL || e->out == NULL) {
        hero_tls_ssl_said(HERO_TLS_LIMIT, "OpenSSL made no connection");
        if (e->in != NULL) {
            (void)hero_tls_ssl.BIO_free(e->in);
        }
        if (e->out != NULL) {
            (void)hero_tls_ssl.BIO_free(e->out);
        }
        if (e->ssl != NULL) {
            hero_tls_ssl.SSL_free(e->ssl);
        }
        hero_layer_free(e);
        return NULL;
    }
    /* An empty memory BIO answers "try again", not its end: the bytes are
     * still to come from the peer. */
    (void)hero_tls_ssl.BIO_ctrl(e->in, HERO_BIO_C_SET_BUF_MEM_EOF_RETURN, -1, NULL);
    (void)hero_tls_ssl.BIO_ctrl(e->out, HERO_BIO_C_SET_BUF_MEM_EOF_RETURN, -1, NULL);
    hero_tls_ssl.SSL_set_bio(e->ssl, e->in, e->out);
    e->server = c->server;
    if (c->server) {
        hero_tls_ssl.SSL_set_accept_state(e->ssl);
    } else {
        bool address = hero_tls_is_address(host);

        hero_tls_ssl.SSL_set_connect_state(e->ssl);
        /* SNI names a host, never an address (RFC 6066 § 3). */
        if (!address && hero_tls_ssl.SSL_ctrl(e->ssl, HERO_SSL_CTRL_SET_TLSEXT_HOSTNAME, HERO_TLSEXT_NAMETYPE_HOST_NAME, (void *)(uintptr_t)host) != 1) {
            hero_tls_ssl_said(HERO_TLS_INVALID, "OpenSSL refused the host name");
            hero_tls_ssl.SSL_free(e->ssl);
            hero_layer_free(e);
            return NULL;
        }
        if (c->checks == HERO_TLS_CHECK_ALL) {
            int set = address ? hero_tls_ssl.X509_VERIFY_PARAM_set1_ip_asc(hero_tls_ssl.SSL_get0_param(e->ssl), host) : hero_tls_ssl.SSL_set1_host(e->ssl, host);

            if (set != 1) {
                hero_tls_ssl_said(HERO_TLS_INVALID, "OpenSSL refused the host to check");
                hero_tls_ssl.SSL_free(e->ssl);
                hero_layer_free(e);
                return NULL;
            }
        }
    }
    (void)__atomic_add_fetch(&c->refs, 1, __ATOMIC_ACQ_REL);
    e->config = c;
    return e;
}

/* The kind of a refused certificate, by OpenSSL's verify result. */
static int64_t hero_tls_verify_kind(long result) {
    switch (result) {
    case HERO_X509_V_ERR_CERT_HAS_EXPIRED:
    case HERO_X509_V_ERR_CERT_NOT_YET_VALID:
        return HERO_TLS_CERT_EXPIRED;
    case HERO_X509_V_ERR_HOSTNAME_MISMATCH:
    case HERO_X509_V_ERR_IP_ADDRESS_MISMATCH:
        return HERO_TLS_CERT_HOST;
    case HERO_X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
    case HERO_X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
    case HERO_X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
    case HERO_X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
    case HERO_X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE:
    case HERO_X509_V_ERR_CERT_UNTRUSTED:
        return HERO_TLS_CERT_UNTRUSTED;
    case HERO_X509_V_ERR_CERT_REVOKED:
        return HERO_TLS_CERT_REVOKED;
    default:
        return HERO_TLS_CERT_INVALID;
    }
}

static int hero_tls_engine_handshake(hero_tls_engine *e) {
    int answer;
    int error;

    hero_tls_ssl.ERR_clear_error();
    answer = hero_tls_ssl.SSL_do_handshake(e->ssl);
    if (answer == 1) {
        return HERO_TLS_STEP_DONE;
    }
    error = hero_tls_ssl.SSL_get_error(e->ssl, answer);
    if (error == HERO_SSL_ERROR_WANT_READ || error == HERO_SSL_ERROR_WANT_WRITE) {
        return HERO_TLS_STEP_MORE;
    }
    if (!e->server && e->config->checks == HERO_TLS_CHECK_ALL) {
        long result = hero_tls_ssl.SSL_get_verify_result(e->ssl);

        if (result != HERO_X509_V_OK) {
            char words[HERO_TLS_WORDS_MOST];

            (void)snprintf(words, sizeof words, "the peer's certificate was refused: %s", hero_tls_ssl.X509_verify_cert_error_string(result));
            hero_tls_ssl.ERR_clear_error();
            hero_tls_said(hero_tls_verify_kind(result), (int64_t)result, words);
            return HERO_TLS_STEP_FAILED;
        }
    }
    hero_tls_ssl_said(HERO_TLS_HANDSHAKE, "the TLS handshake failed");
    return HERO_TLS_STEP_FAILED;
}

static bool hero_tls_engine_feed(hero_tls_engine *e, const unsigned char *bytes, size_t count) {
    if (count > (size_t)INT32_MAX || hero_tls_ssl.BIO_write(e->in, bytes, (int)count) != (int)count) {
        hero_tls_ssl_said(HERO_TLS_LIMIT, "OpenSSL took no more bytes");
        return false;
    }
    return true;
}

static size_t hero_tls_engine_output(hero_tls_engine *e, unsigned char *buffer, size_t count) {
    int got = hero_tls_ssl.BIO_read(e->out, buffer, count > (size_t)INT32_MAX ? INT32_MAX : (int)count);

    return got > 0 ? (size_t)got : 0;
}

static bool hero_tls_engine_pending(hero_tls_engine *e) {
    return hero_tls_ssl.BIO_ctrl(e->out, HERO_BIO_CTRL_PENDING, 0, NULL) > 0;
}

static int64_t hero_tls_engine_open(hero_tls_engine *e, unsigned char *buffer, size_t count) {
    int answer;
    int error;

    hero_tls_ssl.ERR_clear_error();
    answer = hero_tls_ssl.SSL_read(e->ssl, buffer, count > (size_t)INT32_MAX ? INT32_MAX : (int)count);
    if (answer > 0) {
        return answer;
    }
    error = hero_tls_ssl.SSL_get_error(e->ssl, answer);
    if (error == HERO_SSL_ERROR_ZERO_RETURN) {
        return 0;
    }
    if (error == HERO_SSL_ERROR_WANT_READ || error == HERO_SSL_ERROR_WANT_WRITE) {
        return HERO_TLS_STEP_MORE;
    }
    hero_tls_ssl_said(HERO_TLS_OTHER, "a TLS record could not be read");
    return HERO_TLS_STEP_FAILED;
}

static int64_t hero_tls_engine_seal(hero_tls_engine *e, const unsigned char *data, size_t count) {
    int answer;

    hero_tls_ssl.ERR_clear_error();
    answer = hero_tls_ssl.SSL_write(e->ssl, data, count > (size_t)INT32_MAX ? INT32_MAX : (int)count);
    if (answer > 0) {
        return answer;
    }
    hero_tls_ssl_said(HERO_TLS_OTHER, "a TLS record could not be written");
    return HERO_TLS_STEP_FAILED;
}

static void hero_tls_engine_close_notify(hero_tls_engine *e) {
    hero_tls_ssl.ERR_clear_error();
    (void)hero_tls_ssl.SSL_shutdown(e->ssl);
    hero_tls_ssl.ERR_clear_error();
}

static int64_t hero_tls_engine_version(hero_tls_engine *e) {
    int version = hero_tls_ssl.SSL_version(e->ssl);

    return version == HERO_TLS1_3_VERSION ? 13 : version == HERO_TLS1_2_VERSION ? 12 : -1;
}

static size_t hero_tls_engine_protocol(hero_tls_engine *e, char *buffer, size_t count) {
    const unsigned char *chosen = NULL;
    unsigned int length = 0;

    hero_tls_ssl.SSL_get0_alpn_selected(e->ssl, &chosen, &length);
    if (chosen == NULL || length == 0 || length + 1u > count) {
        buffer[0] = '\0';
        return 0;
    }
    memcpy(buffer, chosen, length);
    buffer[length] = '\0';
    return length;
}

static void hero_tls_engine_free(hero_tls_engine *e) {
    if (e != NULL) {
        /* `SSL_free` frees the two BIOs `SSL_set_bio` gave it. */
        hero_tls_ssl.SSL_free(e->ssl);
        hero_tls_config_drop(e->config);
        hero_layer_free(e);
    }
}

#endif

/* ============================================================================
 * THE DRIVER (Windows, Linux): TLS over a stream, through an engine.
 *
 * A session is a stream's payload, made by `hero_stream_make`. Its reads
 * and writes may run on two threads at once, as `hero_stream.h` allows, so
 * three locks order it: `engine` over the engine, held only while it works
 * and never across a wait; `in`, one reader at a time, held across the wait
 * for the peer's bytes; `out`, one writer at a time, held across the write
 * of records, so records reach the peer in the order the engine wrote them.
 * The order is `in` or `out` before `engine`, and `in` before `out`. A
 * reader whose open left records for the peer (a key update's answer)
 * writes them only where `out` is free; whoever lets `out` go asks the
 * engine again, so no record is left behind (`hero_tls_settle`).
 * ========================================================================== */
#if !defined(__APPLE__)

typedef struct hero_tls_session {
    hero_stream *inner;
    hero_tls_engine *engine;
    /* The socket under `inner` where this layer made it (a connection by
     * name, or one accepted), whose deadline bounds the handshake; NULL for
     * a stream the program handed. */
    hero_net_socket *socket;
    hero_tls_mutex engine_lock;
    hero_tls_mutex in_lock;
    hero_tls_mutex out_lock;
    /* A write lost part of a record to the inner stream's failure, a
     * deadline's among them: every later write is refused. Under
     * `out_lock`. */
    bool write_broken;
    unsigned char input[HERO_TLS_INPUT_MOST];
    unsigned char output[HERO_TLS_INPUT_MOST];
} hero_tls_session;

static hero_tls_session *hero_tls_session_new(hero_stream *inner, hero_tls_engine *engine, hero_net_socket *socket) {
    hero_tls_session *s = hero_layer_alloc_zero(1, sizeof *s);

    if (s == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    s->inner = inner;
    s->engine = engine;
    s->socket = socket;
    hero_tls_mutex_init(&s->engine_lock);
    hero_tls_mutex_init(&s->in_lock);
    hero_tls_mutex_init(&s->out_lock);
    return s;
}

/* The session given back; its inner stream closed where `close_inner`. */
static void hero_tls_session_free(hero_tls_session *s, bool close_inner) {
    hero_tls_engine_free(s->engine);
    if (close_inner) {
        (void)hero_stream_close(s->inner);
    }
    hero_tls_mutex_end(&s->engine_lock);
    hero_tls_mutex_end(&s->in_lock);
    hero_tls_mutex_end(&s->out_lock);
    hero_layer_free(s);
}

/* All `count` bytes of `data` written to the inner stream; -1 with the
 * stream's failure on this thread where it failed. */
static int64_t hero_tls_write_all(hero_stream *inner, const unsigned char *data, size_t count) {
    size_t written = 0;

    while (written < count) {
        int64_t sent = hero_stream_write(inner, data + written, count - written);

        if (sent < 0) {
            return -1;
        }
        if (sent == 0) {
            hero_stream_set_error(HERO_STREAM_CLOSED_PEER, 0);
            return -1;
        }
        written += (size_t)sent;
    }
    return 0;
}

/* Every record the engine holds for the peer written, in order. Under
 * `out_lock`. */
static int64_t hero_tls_flush(hero_tls_session *s) {
    for (;;) {
        size_t count;

        hero_tls_mutex_lock(&s->engine_lock);
        count = hero_tls_engine_output(s->engine, s->output, sizeof s->output);
        hero_tls_mutex_unlock(&s->engine_lock);
        if (count == 0) {
            return 0;
        }
        if (hero_tls_write_all(s->inner, s->output, count) < 0) {
            return -1;
        }
    }
}

/* After letting `out_lock` go: records the engine still holds are written
 * by this thread where `out_lock` is free, and otherwise by its holder,
 * which asks again after it lets go. */
static void hero_tls_settle(hero_tls_session *s) {
    for (;;) {
        bool pending;

        hero_tls_mutex_lock(&s->engine_lock);
        pending = hero_tls_engine_pending(s->engine);
        hero_tls_mutex_unlock(&s->engine_lock);
        if (!pending || !hero_tls_mutex_try(&s->out_lock)) {
            return;
        }
        (void)hero_tls_flush(s);
        hero_tls_mutex_unlock(&s->out_lock);
    }
}

/* The handshake, driven to its end over the inner stream; 0, or -1 said.
 * Before the session is a stream, so on one thread and with no lock. */
static int64_t hero_tls_handshake(hero_tls_session *s) {
    for (;;) {
        int step = hero_tls_engine_handshake(s->engine);
        int64_t got;

        /* What the step wrote goes first, an alert on a failure included. */
        if (hero_tls_flush(s) < 0) {
            if (step != HERO_TLS_STEP_FAILED) {
                int64_t kind = hero_tls_from_stream(hero_stream_last_error());

                hero_tls_said(kind, hero_stream_last_os_error(), hero_tls_net_words(kind));
            }
            return -1;
        }
        if (step == HERO_TLS_STEP_DONE) {
            return 0;
        }
        if (step == HERO_TLS_STEP_FAILED) {
            return -1;
        }
        got = hero_stream_read(s->inner, s->input, sizeof s->input);
        if (got == 0) {
            hero_tls_said(HERO_TLS_CLOSED_PEER, 0, "the peer closed the connection during the TLS handshake");
            return -1;
        }
        if (got < 0) {
            int64_t kind = hero_tls_from_stream(hero_stream_last_error());

            hero_tls_said(kind, hero_stream_last_os_error(), hero_tls_net_words(kind));
            return -1;
        }
        if (!hero_tls_engine_feed(s->engine, s->input, (size_t)got)) {
            return -1;
        }
    }
}

static int64_t hero_tls_stream_read(void *payload, void *buffer, uint64_t count) {
    hero_tls_session *s = payload;
    int64_t answer;

    hero_tls_mutex_lock(&s->in_lock);
    for (;;) {
        int64_t got;

        hero_tls_mutex_lock(&s->engine_lock);
        answer = hero_tls_engine_open(s->engine, (unsigned char *)buffer, (size_t)count);
        hero_tls_mutex_unlock(&s->engine_lock);
        hero_tls_settle(s);
        if (answer >= 0) {
            break;
        }
        if (answer == HERO_TLS_STEP_FAILED) {
            hero_stream_set_error(HERO_STREAM_OTHER, hero_tls_last_error());
            break;
        }
        got = hero_stream_read(s->inner, s->input, sizeof s->input);
        if (got <= 0) {
            /* The transport's end with no close_notify before it is
             * answered as the end, as Python's `ssl` answers it by default:
             * a protocol above TLS that counts its bytes sees what is
             * missing. A failure is the inner stream's, said already. */
            answer = got;
            break;
        }
        hero_tls_mutex_lock(&s->engine_lock);
        if (!hero_tls_engine_feed(s->engine, s->input, (size_t)got)) {
            hero_tls_mutex_unlock(&s->engine_lock);
            hero_stream_set_error(HERO_STREAM_LIMIT, 0);
            answer = -1;
            break;
        }
        hero_tls_mutex_unlock(&s->engine_lock);
    }
    hero_tls_mutex_unlock(&s->in_lock);
    return answer;
}

static int64_t hero_tls_stream_write(void *payload, const void *data, uint64_t count) {
    hero_tls_session *s = payload;
    const unsigned char *bytes = data;
    uint64_t taken = 0;
    int64_t answer = 0;

    hero_tls_mutex_lock(&s->out_lock);
    if (s->write_broken) {
        hero_tls_mutex_unlock(&s->out_lock);
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    while (taken < count) {
        size_t step = count - taken > HERO_TLS_RECORD_MOST ? HERO_TLS_RECORD_MOST : (size_t)(count - taken);
        int64_t sealed;

        hero_tls_mutex_lock(&s->engine_lock);
        sealed = hero_tls_engine_seal(s->engine, bytes + taken, step);
        hero_tls_mutex_unlock(&s->engine_lock);
        if (sealed < 0) {
            hero_stream_set_error(HERO_STREAM_OTHER, hero_tls_last_error());
            answer = -1;
            break;
        }
        if (hero_tls_flush(s) < 0) {
            /* A record may have left in part: no later record could be
             * read by the peer after it. */
            s->write_broken = true;
            answer = -1;
            break;
        }
        taken += (uint64_t)sealed;
    }
    hero_tls_mutex_unlock(&s->out_lock);
    hero_tls_settle(s);
    return answer < 0 ? -1 : (int64_t)taken;
}

static int64_t hero_tls_stream_close(void *payload) {
    hero_tls_session *s = payload;
    int64_t closed;

    /* The close_notify, as a courtesy: the peer learns the end was meant.
     * Where the layer made the socket, one second bounds it. */
    if (s->socket != NULL) {
        (void)hero_net_set_deadline(s->socket, 1000);
    }
    hero_tls_engine_close_notify(s->engine);
    (void)hero_tls_flush(s);
    hero_tls_engine_free(s->engine);
    s->engine = NULL;
    closed = hero_stream_close(s->inner);
    {
        int64_t kind = hero_stream_last_error();
        int64_t os = hero_stream_last_os_error();

        hero_tls_mutex_end(&s->engine_lock);
        hero_tls_mutex_end(&s->in_lock);
        hero_tls_mutex_end(&s->out_lock);
        hero_layer_free(s);
        if (closed < 0) {
            hero_stream_set_error(kind, os);
        }
    }
    return closed;
}

/* The table every TLS stream of this arm is made with. */
static const hero_stream_ops hero_tls_ops = { hero_tls_stream_read, hero_tls_stream_write, hero_tls_stream_close };

/* The engine's handshake run over `inner` and, done, the TLS stream; NULL
 * said where it failed. `inner` is closed on a failure where `owned` (the
 * layer made it) and left to the caller where not. */
static hero_stream *hero_tls_run(hero_stream *inner, hero_tls_engine *engine, hero_net_socket *socket, bool owned) {
    hero_tls_session *s = hero_tls_session_new(inner, engine, socket);
    hero_stream *stream;

    if (s == NULL) {
        hero_tls_engine_free(engine);
        if (owned) {
            (void)hero_stream_close(inner);
        }
        return NULL;
    }
    if (hero_tls_handshake(s) < 0) {
        int64_t kind = hero_tls_kind;
        int64_t os = hero_tls_os;
        char words[HERO_TLS_WORDS_MOST];

        (void)hero_tls_copy_text(words, sizeof words, hero_tls_words);
        hero_tls_session_free(s, owned);
        hero_tls_said(kind, os, words);
        return NULL;
    }
    if (socket != NULL) {
        (void)hero_net_set_deadline(socket, 0);
    }
    stream = hero_stream_make(s, &hero_tls_ops, HERO_STREAM_KIND_SOCKET);
    if (stream == NULL) {
        hero_tls_session_free(s, owned);
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many streams as it can");
        return NULL;
    }
    hero_tls_fine();
    return stream;
}

/* The engine of a stream this arm made, held until `hero_stream_release`;
 * NULL said `HERO_TLS_NOT_OPEN` where `s` is none. */
static hero_tls_session *hero_tls_held(hero_stream *s) {
    void *payload = NULL;

    if (hero_stream_hold_payload(s, &hero_tls_ops, &payload) < 0) {
        hero_tls_said(HERO_TLS_NOT_OPEN, 0, "the stream is no open TLS stream");
        return NULL;
    }
    return payload;
}

int64_t hero_tls_version(hero_stream *s) {
    hero_tls_session *session = hero_tls_held(s);
    int64_t version;

    if (session == NULL) {
        return -1;
    }
    hero_tls_mutex_lock(&session->engine_lock);
    version = hero_tls_engine_version(session->engine);
    hero_tls_mutex_unlock(&session->engine_lock);
    (void)hero_stream_release(s);
    hero_tls_fine();
    return version;
}

int64_t hero_tls_protocol(hero_stream *s, void *buffer, uint64_t count) {
    hero_tls_session *session;
    size_t length;

    if (buffer == NULL || count < HERO_TLS_PROTOCOL_MOST) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the buffer is shorter than HERO_TLS_PROTOCOL_MOST");
        return -1;
    }
    session = hero_tls_held(s);
    if (session == NULL) {
        return -1;
    }
    hero_tls_mutex_lock(&session->engine_lock);
    length = hero_tls_engine_protocol(session->engine, (char *)buffer, HERO_TLS_PROTOCOL_MOST);
    hero_tls_mutex_unlock(&session->engine_lock);
    (void)hero_stream_release(s);
    hero_tls_fine();
    return (int64_t)length;
}

int64_t hero_tls_set_deadline(hero_stream *s, int64_t ms) {
    hero_tls_session *session;
    int64_t answer;
    int64_t kind;
    int64_t os;

    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return -1;
    }
    session = hero_tls_held(s);
    if (session == NULL) {
        return -1;
    }
    /* The socket's under the stream, whoever made it: `hero_stream_net.h`
     * reaches it and moves a wait already begun. */
    answer = hero_stream_net_set_deadline(session->inner, ms);
    kind = hero_stream_last_error();
    os = hero_stream_last_os_error();
    (void)hero_stream_release(s);
    if (answer < 0) {
        if (kind == HERO_STREAM_INVALID) {
            hero_tls_said(HERO_TLS_UNSUPPORTED, os, "the stream under this TLS stream is no socket, and keeps no deadline");
        } else {
            int64_t tls_kind = hero_tls_from_stream(kind);

            hero_tls_said(tls_kind, os, hero_tls_net_words(tls_kind));
        }
        return -1;
    }
    hero_tls_fine();
    return 0;
}

int64_t hero_tls_available(void) {
    return hero_tls_arm_ready() ? 1 : 0;
}

static bool hero_tls_ready_or_said(void) {
    if (!hero_tls_arm_ready()) {
#if defined(_WIN32)
        hero_tls_said(HERO_TLS_UNSUPPORTED, 0, "SChannel is not ready on this machine");
#else
        hero_tls_said(HERO_TLS_UNSUPPORTED, 0, "this machine has no OpenSSL 3: libssl.so.3 could not be opened");
#endif
        return false;
    }
    return true;
}

/* A client's settings from the call's arguments, said where refused. */
static hero_tls_config *hero_tls_client_config(const void *roots, uint64_t roots_count, const char *protocols, int64_t checks) {
    hero_tls_certs *certs;
    unsigned char alpn[HERO_TLS_PROTOCOLS_MOST + 2];
    size_t alpn_count;
    hero_tls_config *c;

    if (!hero_tls_bytes_given(roots, roots_count, "the roots") || !hero_tls_protocols_given(protocols) || !hero_tls_checks_given(checks) || !hero_tls_ready_or_said()) {
        return NULL;
    }
    certs = hero_layer_alloc_zero(1, sizeof *certs);
    if (certs == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    if (roots_count > 0 && !hero_tls_certs_of(roots, roots_count, certs, "the roots")) {
        hero_layer_free(certs);
        return NULL;
    }
    alpn_count = hero_tls_protocols_wire(protocols, alpn, sizeof alpn);
    c = hero_tls_config_client(certs, alpn, alpn_count, checks);
    hero_tls_certs_free(certs);
    hero_layer_free(certs);
    return c;
}

/* A server's settings from the call's arguments, said where refused. */
static hero_tls_config *hero_tls_server_config(const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols) {
    hero_tls_certs *certs;
    hero_tls_key read_key;
    unsigned char alpn[HERO_TLS_PROTOCOLS_MOST + 2];
    size_t alpn_count;
    hero_tls_config *c;

    if (!hero_tls_bytes_given(chain, chain_count, "the chain") || !hero_tls_bytes_given(key, key_count, "the key") || !hero_tls_protocols_given(protocols) || !hero_tls_ready_or_said()) {
        return NULL;
    }
    certs = hero_layer_alloc_zero(1, sizeof *certs);
    if (certs == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    if (!hero_tls_certs_of(chain, chain_count, certs, "the chain")) {
        hero_layer_free(certs);
        return NULL;
    }
    if (!hero_tls_key_of(key, key_count, &read_key)) {
        hero_tls_certs_free(certs);
        hero_layer_free(certs);
        return NULL;
    }
    alpn_count = hero_tls_protocols_wire(protocols, alpn, sizeof alpn);
    c = hero_tls_config_server(certs, &read_key, alpn, alpn_count);
    hero_tls_key_free(&read_key);
    hero_tls_certs_free(certs);
    hero_layer_free(certs);
    return c;
}

/* The steps of `script` run on `socket`, the plain connection, before TLS:
 * each send written whole, each expected byte read one at a time, so no byte
 * past the last step is taken; the socket's deadline bounds every wait.
 * 0, or -1 said. */
static int64_t hero_tls_exchange_on(hero_net_socket *socket, const hero_tls_script *script) {
    hero_tls_exchange *e;

    if (script == NULL) {
        return 0;
    }
    e = hero_layer_alloc_zero(1, sizeof *e);
    if (e == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for the steps");
        return -1;
    }
    hero_tls_exchange_begin(e, script);
    for (;;) {
        const unsigned char *bytes;
        size_t length;
        unsigned char b;
        int64_t got;

        while (hero_tls_exchange_sending(e, &bytes, &length)) {
            size_t sent = 0;

            while (sent < length) {
                int64_t wrote = hero_net_send(socket, bytes + sent, length - sent);

                if (wrote <= 0) {
                    int64_t kind = wrote == 0 ? HERO_TLS_CLOSED_PEER : hero_tls_from_net(hero_net_last_error());

                    hero_tls_said(kind, hero_net_last_os_error(), hero_tls_net_words(kind));
                    hero_layer_free(e);
                    return -1;
                }
                sent += (size_t)wrote;
            }
        }
        if (e->state == HERO_TLS_EXCHANGE_DONE) {
            hero_layer_free(e);
            return 0;
        }
        got = hero_net_recv(socket, &b, 1);
        if (got <= 0) {
            if (got == 0) {
                hero_tls_exchange_closed(e);
                hero_tls_said(e->failed_kind, 0, e->words);
            } else {
                int64_t kind = hero_tls_from_net(hero_net_last_error());

                hero_tls_said(kind, hero_net_last_os_error(), hero_tls_net_words(kind));
            }
            hero_layer_free(e);
            return -1;
        }
        if (hero_tls_exchange_byte(e, b) == HERO_TLS_EXCHANGE_FAILED) {
            hero_tls_said(e->failed_kind, 0, e->words);
            hero_layer_free(e);
            return -1;
        }
    }
}

hero_stream *hero_tls_client(hero_stream *inner, const char *host, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks) {
    hero_tls_config *c;
    hero_tls_engine *e;

    if (!hero_tls_host_given(host)) {
        return NULL;
    }
    c = hero_tls_client_config(roots, roots_count, protocols, checks);
    if (c == NULL) {
        return NULL;
    }
    e = hero_tls_engine_new(c, host);
    hero_tls_config_drop(c);
    if (e == NULL) {
        return NULL;
    }
    return hero_tls_run(inner, e, NULL, false);
}

hero_stream *hero_tls_server(hero_stream *inner, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols) {
    hero_tls_config *c = hero_tls_server_config(chain, chain_count, key, key_count, protocols);
    hero_tls_engine *e;

    if (c == NULL) {
        return NULL;
    }
    e = hero_tls_engine_new(c, NULL);
    hero_tls_config_drop(c);
    if (e == NULL) {
        return NULL;
    }
    return hero_tls_run(inner, e, NULL, false);
}

/* The milliseconds left of `ms` since `start` (monotonic nanoseconds), at
 * least 1 while some is left; 0 where `ms` is 0, no limit; -1 where none is
 * left. */
static int64_t hero_tls_left(int64_t ms, int64_t start) {
    int64_t spent;

    if (ms == 0) {
        return 0;
    }
    spent = (hero_time_monotonic_ns() - start) / 1000000;
    if (spent >= ms) {
        return -1;
    }
    return ms - spent;
}

/* A client's connection by name, the steps of `script` (NULL for none) run
 * on it before TLS: `hero_tls_connect` and `hero_tls_connect_after`. */
static hero_stream *hero_tls_connect_with(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks, hero_tls_script *script) {
    int64_t start = hero_time_monotonic_ns();
    hero_tls_config *c;
    hero_tls_engine *e;
    hero_net_socket *socket;
    hero_stream *inner;
    int64_t left;

    if (!hero_tls_host_given(host) || !hero_tls_port_given(port, false)) {
        return NULL;
    }
    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return NULL;
    }
    c = hero_tls_client_config(roots, roots_count, protocols, checks);
    if (c == NULL) {
        return NULL;
    }
    e = hero_tls_engine_new(c, host);
    hero_tls_config_drop(c);
    if (e == NULL) {
        return NULL;
    }
    socket = hero_net_connect_within(host, port, ms);
    if (socket == NULL) {
        int64_t kind = hero_tls_from_net(hero_net_last_error());

        hero_tls_engine_free(e);
        hero_tls_said(kind, hero_net_last_os_error(), hero_tls_net_words(kind));
        return NULL;
    }
    left = hero_tls_left(ms, start);
    if (left < 0 || hero_net_set_deadline(socket, left) < 0) {
        hero_tls_engine_free(e);
        (void)hero_net_close(socket);
        hero_tls_said(HERO_TLS_TIMED_OUT, 0, hero_tls_net_words(HERO_TLS_TIMED_OUT));
        return NULL;
    }
    if (hero_tls_exchange_on(socket, script) < 0) {
        hero_tls_engine_free(e);
        (void)hero_net_close(socket);
        return NULL;
    }
    inner = hero_stream_net_of(socket);
    if (inner == NULL) {
        hero_tls_engine_free(e);
        (void)hero_net_close(socket);
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many streams as it can");
        return NULL;
    }
    return hero_tls_run(inner, e, socket, true);
}

hero_stream *hero_tls_connect(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks) {
    return hero_tls_connect_with(host, port, ms, roots, roots_count, protocols, checks, NULL);
}

hero_stream *hero_tls_connect_after(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks, const void *steps, uint64_t steps_count) {
    hero_tls_script *script;
    hero_stream *stream;

    if (!hero_tls_script_of(steps, steps_count, &script)) {
        return NULL;
    }
    stream = hero_tls_connect_with(host, port, ms, roots, roots_count, protocols, checks, script);
    hero_tls_script_drop(script);
    return stream;
}

/* A listener of this arm: the socket, the settings its connections share,
 * and the steps each runs before TLS (NULL for none). */
struct hero_tls_spot {
    hero_net_socket *socket;
    hero_tls_config *config;
    hero_tls_script *script;
};

static void hero_tls_spot_wake(hero_tls_spot *spot) {
    /* An `accept` another thread waits in on the socket ends at once. */
    (void)hero_net_close(spot->socket);
}

static void hero_tls_spot_free(hero_tls_spot *spot) {
    hero_tls_config_drop(spot->config);
    hero_tls_script_drop(spot->script);
    hero_layer_free(spot);
}

/* A listener whose connections run the steps of `script` (NULL for none)
 * before TLS: `hero_tls_listen` and `hero_tls_listen_after`. It takes the
 * caller's count of `script`, also where it fails. */
static hero_tls_listener *hero_tls_listen_with(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols, hero_tls_script *script) {
    hero_tls_spot *spot;
    hero_tls_listener *l;

    if (address == NULL) {
        hero_tls_script_drop(script);
        hero_tls_said(HERO_TLS_INVALID, 0, "no address was given");
        return NULL;
    }
    if (!hero_tls_port_given(port, true)) {
        hero_tls_script_drop(script);
        return NULL;
    }
    spot = hero_layer_alloc_zero(1, sizeof *spot);
    if (spot == NULL) {
        hero_tls_script_drop(script);
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    spot->script = script;
    spot->config = hero_tls_server_config(chain, chain_count, key, key_count, protocols);
    if (spot->config == NULL) {
        hero_tls_script_drop(spot->script);
        hero_layer_free(spot);
        return NULL;
    }
    spot->socket = hero_net_listen(address, port);
    if (spot->socket == NULL) {
        int64_t kind = hero_tls_from_net(hero_net_last_error());

        hero_tls_said(kind, hero_net_last_os_error(), hero_tls_net_words(kind));
        hero_tls_spot_free(spot);
        return NULL;
    }
    l = hero_tls_issue(spot);
    if (l == NULL) {
        (void)hero_net_close(spot->socket);
        hero_tls_spot_free(spot);
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many TLS listeners as it can, 4096");
        return NULL;
    }
    hero_tls_fine();
    return l;
}

hero_tls_listener *hero_tls_listen(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols) {
    return hero_tls_listen_with(address, port, chain, chain_count, key, key_count, protocols, NULL);
}

hero_tls_listener *hero_tls_listen_after(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols, const void *steps, uint64_t steps_count) {
    hero_tls_script *script;

    if (!hero_tls_script_of(steps, steps_count, &script)) {
        return NULL;
    }
    return hero_tls_listen_with(address, port, chain, chain_count, key, key_count, protocols, script);
}

int64_t hero_tls_port(hero_tls_listener *l) {
    hero_tls_slot *slot;
    hero_tls_spot *spot = hero_tls_enter(l, &slot);
    int64_t port;

    if (spot == NULL) {
        return -1;
    }
    port = hero_net_port(spot->socket);
    hero_tls_leave(slot);
    if (port < 0) {
        hero_tls_said(HERO_TLS_NOT_OPEN, 0, "the TLS listener is closed");
        return -1;
    }
    hero_tls_fine();
    return port;
}

int64_t hero_tls_set_accept_deadline(hero_tls_listener *l, int64_t ms) {
    hero_tls_slot *slot;
    hero_tls_spot *spot;
    int64_t answer;

    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return -1;
    }
    spot = hero_tls_enter(l, &slot);
    if (spot == NULL) {
        return -1;
    }
    /* The listening socket's own deadline, which ends an `accept` waiting
     * on it at once (`hero_net.h`). */
    answer = hero_net_set_deadline(spot->socket, ms);
    hero_tls_leave(slot);
    if (answer < 0) {
        hero_tls_said(HERO_TLS_NOT_OPEN, hero_net_last_os_error(), "the TLS listener is closed");
        return -1;
    }
    hero_tls_fine();
    return 0;
}

hero_stream *hero_tls_accept(hero_tls_listener *l, int64_t ms) {
    hero_tls_slot *slot;
    hero_tls_spot *spot;
    hero_net_socket *socket;
    hero_stream *inner;
    hero_tls_engine *e;
    hero_stream *stream;
    hero_tls_script *script;
    int64_t ran;

    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return NULL;
    }
    spot = hero_tls_enter(l, &slot);
    if (spot == NULL) {
        return NULL;
    }
    socket = hero_net_accept(spot->socket);
    if (socket == NULL) {
        int64_t kind = hero_tls_closing(slot) ? HERO_TLS_NOT_OPEN : hero_tls_from_net(hero_net_last_error());
        int64_t os = hero_net_last_os_error();

        hero_tls_leave(slot);
        hero_tls_said(kind, os, kind == HERO_TLS_NOT_OPEN ? "the TLS listener was closed" : hero_tls_net_words(kind));
        return NULL;
    }
    e = hero_tls_engine_new(spot->config, NULL);
    script = spot->script;
    hero_tls_script_keep(script);
    hero_tls_leave(slot);
    if (e == NULL) {
        hero_tls_script_drop(script);
        (void)hero_net_close(socket);
        return NULL;
    }
    if (ms > 0) {
        (void)hero_net_set_deadline(socket, ms);
    }
    ran = hero_tls_exchange_on(socket, script);
    hero_tls_script_drop(script);
    if (ran < 0) {
        hero_tls_engine_free(e);
        (void)hero_net_close(socket);
        return NULL;
    }
    inner = hero_stream_net_of(socket);
    if (inner == NULL) {
        hero_tls_engine_free(e);
        (void)hero_net_close(socket);
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many streams as it can");
        return NULL;
    }
    stream = hero_tls_run(inner, e, socket, true);
    return stream;
}

#endif

/* ============================================================================
 * THE APPLE ARM: Network.framework, and the Security framework's trust.
 *
 * A connection is an `nw_connection_t` whose parameters carry TLS: TLS 1.2
 * at the least, the ALPN list, and on the client a verify block that asks
 * `SecTrustEvaluateWithError` itself, with the SSL policy and the host, and
 * with the roots given as the only anchors where some were given; so a
 * refusal is known by its own code (`errSecCertificateExpired`,
 * `errSecHostNameMismatch`, `errSecNotTrusted`), which the connection's own
 * failure, a TLS alert's number, would not tell. Every block runs on the
 * connection's own serial queue; a call of the program waits for a block's
 * answer on a semaphore, in slices of 50 ms that see the stop asked.
 *
 * WHAT OUTLIVES WHAT. A connection's state is counted: the stream holds it,
 * and so do the connection's handlers until the connection reports itself
 * cancelled, after which none runs. A read or a write hands its block a
 * request of its own, counted too, so a call that stops waiting leaves the
 * block nothing of the caller's to write into.
 * ========================================================================== */
#if defined(__APPLE__)

#define HERO_TLS_APPLE_WAITING 0
#define HERO_TLS_APPLE_READY 1
#define HERO_TLS_APPLE_FAILED 2
#define HERO_TLS_APPLE_CANCELLED 3

/* The slice a wait sleeps between two looks at the stop, 50 ms. */
#define HERO_TLS_SLICE_NS INT64_C(50000000)

typedef struct hero_tls_apple {
    nw_connection_t connection;
    dispatch_queue_t queue;
    dispatch_semaphore_t changed;
    pthread_mutex_t lock;
    int state;
    int64_t refs;
    /* Why the connection failed, and what the verify block refused. */
    int64_t kind;
    int64_t os;
    char words[HERO_TLS_WORDS_MOST];
    int64_t cert_kind;
    int64_t cert_os;
    char cert_words[HERO_TLS_WORDS_MOST];
    /* The steps run before TLS, counted (NULL for none), and what a step
     * not met told, which the framer keeps for a client. */
    hero_tls_script *script;
    int64_t step_kind;
    char step_words[HERO_TLS_WORDS_MOST];
    /* The deadline of the stream's waits, monotonic, 0 for none; a write it
     * ended, after which every write is refused; the read it ended, still
     * pending, which the next read waits for; and bytes a read took past
     * its caller's count, given to the next. Under `lock`. */
    int64_t until;
    bool write_broken;
    struct hero_tls_apple_request *pending;
    unsigned char *held;
    size_t held_count;
    size_t held_at;
    /* What a client checks: the host, the only roots where given. */
    CFStringRef host;
    CFArrayRef roots;
    int64_t checks;
    /* What the handshake chose, read once it was ready. */
    int64_t version;
    char protocol[HERO_TLS_PROTOCOL_MOST];
} hero_tls_apple;

static hero_tls_apple *hero_tls_apple_new(void) {
    hero_tls_apple *a = hero_layer_alloc_zero(1, sizeof *a);

    if (a == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    a->queue = dispatch_queue_create("org.heroes-lang.tls", DISPATCH_QUEUE_SERIAL);
    a->changed = dispatch_semaphore_create(0);
    if (a->queue == NULL || a->changed == NULL) {
        if (a->queue != NULL) {
            dispatch_release(a->queue);
        }
        if (a->changed != NULL) {
            dispatch_release(a->changed);
        }
        hero_layer_free(a);
        hero_tls_said(HERO_TLS_LIMIT, 0, "no queue or semaphore left for TLS");
        return NULL;
    }
    (void)pthread_mutex_init(&a->lock, NULL);
    a->refs = 1;
    a->checks = HERO_TLS_CHECK_ALL;
    return a;
}

static void hero_tls_apple_request_drop(struct hero_tls_apple_request *r);

static void hero_tls_apple_drop(hero_tls_apple *a) {
    if (__atomic_sub_fetch(&a->refs, 1, __ATOMIC_ACQ_REL) != 0) {
        return;
    }
    if (a->pending != NULL) {
        hero_tls_apple_request_drop(a->pending);
    }
    hero_layer_free(a->held);
    if (a->connection != NULL) {
        nw_release(a->connection);
    }
    if (a->host != NULL) {
        CFRelease(a->host);
    }
    if (a->roots != NULL) {
        CFRelease(a->roots);
    }
    hero_tls_script_drop(a->script);
    dispatch_release(a->queue);
    dispatch_release(a->changed);
    (void)pthread_mutex_destroy(&a->lock);
    hero_layer_free(a);
}

/* A CFString as UTF-8 words into `into`, or `fallback` where it cannot be. */
static void hero_tls_cf_words(CFStringRef text, char *into, size_t most, const char *fallback) {
    if (text == NULL || !CFStringGetCString(text, into, (CFIndex)most, kCFStringEncodingUTF8)) {
        (void)hero_tls_copy_text(into, most, fallback);
    }
}

/* A CFError's own description, with `what` before it. */
static void hero_tls_cf_error_words(CFErrorRef error, const char *what, char *into, size_t most) {
    char described[HERO_TLS_WORDS_MOST];
    CFStringRef text = error == NULL ? NULL : CFErrorCopyDescription(error);

    hero_tls_cf_words(text, described, sizeof described, "no reason was given");
    if (text != NULL) {
        CFRelease(text);
    }
    (void)snprintf(into, most, "%s: %s", what, described);
}

/* The kind of a refusal, by the Security framework's code. */
static int64_t hero_tls_trust_kind(CFIndex code) {
    switch (code) {
    case errSecCertificateExpired:
    case errSecCertificateNotValidYet:
        return HERO_TLS_CERT_EXPIRED;
    case errSecHostNameMismatch:
        return HERO_TLS_CERT_HOST;
    case errSecNotTrusted:
    case errSecCreateChainFailed:
        return HERO_TLS_CERT_UNTRUSTED;
    case errSecCertificateRevoked:
        return HERO_TLS_CERT_REVOKED;
    default:
        return HERO_TLS_CERT_INVALID;
    }
}

/* The connection's own failure as this layer's kind, with words. Under the
 * state's lock. */
static void hero_tls_apple_failed(hero_tls_apple *a, nw_error_t error) {
    nw_error_domain_t domain = error == NULL ? nw_error_domain_invalid : nw_error_get_error_domain(error);
    int code = error == NULL ? 0 : nw_error_get_error_code(error);
    char words[HERO_TLS_WORDS_MOST];

    a->os = code;
    if (a->cert_kind != HERO_TLS_OK) {
        a->kind = a->cert_kind;
        a->os = a->cert_os;
        (void)hero_tls_copy_text(a->words, sizeof a->words, a->cert_words);
        return;
    }
    if (a->step_kind != HERO_TLS_OK) {
        a->kind = a->step_kind;
        a->os = 0;
        (void)hero_tls_copy_text(a->words, sizeof a->words, a->step_words);
        return;
    }
    if (domain == nw_error_domain_posix) {
        switch (code) {
        case ECONNREFUSED:
            a->kind = HERO_TLS_REFUSED;
            break;
        case ENETUNREACH:
        case EHOSTUNREACH:
        case ENETDOWN:
        case EHOSTDOWN:
            a->kind = HERO_TLS_UNREACHABLE;
            break;
        case ETIMEDOUT:
            a->kind = HERO_TLS_TIMED_OUT;
            break;
        case ECONNRESET:
        case EPIPE:
        case ECONNABORTED:
            a->kind = HERO_TLS_CLOSED_PEER;
            break;
        case EADDRINUSE:
            a->kind = HERO_TLS_IN_USE;
            break;
        case EACCES:
        case EPERM:
            a->kind = HERO_TLS_DENIED;
            break;
        case EADDRNOTAVAIL:
            a->kind = HERO_TLS_INVALID;
            break;
        case EPROTO:
            /* A listener's framer failed a client's exchange before TLS:
             * its words stay with the framer, which serves every
             * connection of the listener. */
            a->kind = HERO_TLS_PREAMBLE;
            break;
        case ENOMEM:
        case ENOBUFS:
        case EMFILE:
        case ENFILE:
            a->kind = HERO_TLS_LIMIT;
            break;
        default:
            a->kind = HERO_TLS_OTHER;
            break;
        }
        if (a->kind == HERO_TLS_PREAMBLE) {
            (void)snprintf(words, sizeof words, "the exchange before TLS was not met: a step's answer was not the one expected, or passed its bound");
        } else {
            (void)snprintf(words, sizeof words, "%s", strerror(code));
        }
    } else if (domain == nw_error_domain_dns) {
        a->kind = HERO_TLS_NOT_FOUND;
        (void)snprintf(words, sizeof words, "the host name has no address (DNS error %d)", code);
    } else if (domain == nw_error_domain_tls) {
        CFStringRef text = SecCopyErrorMessageString((OSStatus)code, NULL);
        char said[HERO_TLS_WORDS_MOST];

        a->kind = HERO_TLS_HANDSHAKE;
        hero_tls_cf_words(text, said, sizeof said, "no reason was given");
        if (text != NULL) {
            CFRelease(text);
        }
        (void)snprintf(words, sizeof words, "the TLS handshake failed: %s (%d)", said, code);
    } else {
        a->kind = HERO_TLS_OTHER;
        (void)snprintf(words, sizeof words, "the connection failed (error %d)", code);
    }
    (void)hero_tls_copy_text(a->words, sizeof a->words, words);
}

/* Reads what the handshake chose. On the connection's queue, once ready. */
static void hero_tls_apple_chosen(hero_tls_apple *a) {
    nw_protocol_definition_t definition = nw_protocol_copy_tls_definition();
    nw_protocol_metadata_t metadata = nw_connection_copy_protocol_metadata(a->connection, definition);

    a->version = -1;
    a->protocol[0] = '\0';
    if (metadata != NULL) {
        sec_protocol_metadata_t sec = nw_tls_copy_sec_protocol_metadata(metadata);

        if (sec != NULL) {
            tls_protocol_version_t version = sec_protocol_metadata_get_negotiated_tls_protocol_version(sec);
            const char *chosen = sec_protocol_metadata_copy_negotiated_protocol(sec);

            a->version = version == tls_protocol_version_TLSv13 ? 13 : version == tls_protocol_version_TLSv12 ? 12 : -1;
            if (chosen != NULL) {
                (void)hero_tls_copy_text(a->protocol, sizeof a->protocol, chosen);
                hero_layer_free((void *)(uintptr_t)chosen);
            }
            sec_release(sec);
        }
        nw_release(metadata);
    }
    nw_release(definition);
}

/* The handler of every state the connection reaches, holding a count of
 * the state until it is cancelled. */
static void hero_tls_apple_watch(hero_tls_apple *a) {
    (void)__atomic_add_fetch(&a->refs, 1, __ATOMIC_ACQ_REL);
    nw_connection_set_queue(a->connection, a->queue);
    nw_connection_set_state_changed_handler(a->connection, ^(nw_connection_state_t state, nw_error_t error) {
        bool cancelled = false;

        (void)pthread_mutex_lock(&a->lock);
        switch (state) {
        case nw_connection_state_ready:
            if (a->state == HERO_TLS_APPLE_WAITING) {
                hero_tls_apple_chosen(a);
                a->state = HERO_TLS_APPLE_READY;
            }
            break;
        case nw_connection_state_waiting:
        case nw_connection_state_failed:
            /* Waiting is a connection that cannot be made now (refused,
             * no route), which Network.framework would retry when the
             * network changes: the program is answered at once instead. */
            if (a->state == HERO_TLS_APPLE_WAITING || a->state == HERO_TLS_APPLE_READY) {
                hero_tls_apple_failed(a, error);
                a->state = HERO_TLS_APPLE_FAILED;
            }
            break;
        case nw_connection_state_cancelled:
            a->state = HERO_TLS_APPLE_CANCELLED;
            cancelled = true;
            break;
        default:
            break;
        }
        (void)pthread_mutex_unlock(&a->lock);
        dispatch_semaphore_signal(a->changed);
        if (cancelled) {
            nw_connection_set_state_changed_handler(a->connection, NULL);
            hero_tls_apple_drop(a);
        }
    });
}

/* A wait's answers besides a state. */
#define HERO_TLS_WAIT_STOPPED (-1)
#define HERO_TLS_WAIT_TIMED_OUT (-2)

/* Sleeps on `sem` until `done` reads true, the stop is asked, or
 * `until_ns` (monotonic, 0 for none) passes. */
static int hero_tls_apple_sleep(dispatch_semaphore_t sem, int64_t until_ns) {
    int64_t slice = HERO_TLS_SLICE_NS;

    if (hero_net_stop_asked() != 0) {
        return HERO_TLS_WAIT_STOPPED;
    }
    if (until_ns != 0) {
        int64_t left = until_ns - hero_time_monotonic_ns();

        if (left <= 0) {
            return HERO_TLS_WAIT_TIMED_OUT;
        }
        if (left < slice) {
            slice = left;
        }
    }
    (void)dispatch_semaphore_wait(sem, dispatch_time(DISPATCH_TIME_NOW, slice));
    return 0;
}

/* Waits until the connection is ready or failed, or the stop or the time. */
static int hero_tls_apple_settle(hero_tls_apple *a, int64_t until_ns) {
    for (;;) {
        int state;
        int slept;

        (void)pthread_mutex_lock(&a->lock);
        state = a->state;
        (void)pthread_mutex_unlock(&a->lock);
        if (state != HERO_TLS_APPLE_WAITING) {
            return state;
        }
        slept = hero_tls_apple_sleep(a->changed, until_ns);
        if (slept != 0) {
            return slept;
        }
    }
}

/* The verify block of a client: the peer's chain checked by the Security
 * framework with the SSL policy for `a->host`, against `a->roots` alone
 * where given, its refusal kept with its own code. */
static void hero_tls_apple_verify(hero_tls_apple *a, sec_protocol_options_t sec) {
    sec_protocol_options_set_verify_block(sec, ^(sec_protocol_metadata_t metadata, sec_trust_t trust_ref, sec_protocol_verify_complete_t complete) {
        SecTrustRef trust;
        SecPolicyRef policy;
        CFErrorRef error = NULL;
        bool good;

        (void)metadata;
        if (a->checks == HERO_TLS_CHECK_NOTHING_FOR_TESTS) {
            complete(true);
            return;
        }
        trust = sec_trust_copy_ref(trust_ref);
        policy = SecPolicyCreateSSL(true, a->host);
        good = trust != NULL && policy != NULL && SecTrustSetPolicies(trust, policy) == errSecSuccess;
        if (good && a->roots != NULL) {
            good = SecTrustSetAnchorCertificates(trust, a->roots) == errSecSuccess && SecTrustSetAnchorCertificatesOnly(trust, true) == errSecSuccess;
        }
        if (good) {
            good = SecTrustEvaluateWithError(trust, &error);
        }
        if (!good) {
            CFIndex code = error == NULL ? 0 : CFErrorGetCode(error);

            (void)pthread_mutex_lock(&a->lock);
            a->cert_kind = error == NULL ? HERO_TLS_CERT_INVALID : hero_tls_trust_kind(code);
            a->cert_os = (int64_t)code;
            hero_tls_cf_error_words(error, "the peer's certificate was refused", a->cert_words, sizeof a->cert_words);
            (void)pthread_mutex_unlock(&a->lock);
        }
        if (error != NULL) {
            CFRelease(error);
        }
        if (policy != NULL) {
            CFRelease(policy);
        }
        if (trust != NULL) {
            CFRelease(trust);
        }
        complete(good);
    }, a->queue);
}

/* Each name of an ALPN list offered or taken. */
static void hero_tls_apple_protocols(sec_protocol_options_t sec, const char *protocols) {
    char name[256];
    size_t start = 0;
    size_t length = protocols == NULL ? 0 : strlen(protocols);

    for (size_t at = 0; length > 0 && at <= length; at += 1) {
        if (at == length || protocols[at] == ',') {
            size_t run = at - start;

            if (run > 0 && run < sizeof name) {
                memcpy(name, protocols + start, run);
                name[run] = '\0';
                sec_protocol_options_add_tls_application_protocol(sec, name);
            }
            start = at + 1;
        }
    }
}

/* Every send the machine has reached, written below TLS. */
static void hero_tls_apple_framer_send(nw_framer_t framer, hero_tls_exchange *e) {
    const unsigned char *bytes;
    size_t length;

    while (hero_tls_exchange_sending(e, &bytes, &length)) {
        nw_framer_write_output(framer, bytes, length);
    }
}

/* A step not met: its words kept for the client whose session `a` is (a
 * listener's framer has none), the connection failed. */
static void hero_tls_apple_framer_failed(nw_framer_t framer, hero_tls_exchange *e, hero_tls_apple *a) {
    if (a != NULL) {
        (void)pthread_mutex_lock(&a->lock);
        a->step_kind = e->failed_kind;
        (void)hero_tls_copy_text(a->step_words, sizeof a->step_words, e->words);
        (void)pthread_mutex_unlock(&a->lock);
    }
    nw_framer_mark_failed_with_error(framer, e->failed_kind == HERO_TLS_CLOSED_PEER ? ECONNRESET : EPROTO);
}

/* What the peer sent, fed to the machine while a step expects it, never a
 * byte past the last step; the sends it reaches written; once every step
 * is met, the framer stands aside and TLS above it begins. Each pass of the
 * loop takes a byte, writes a step, or returns. */
static size_t hero_tls_apple_framer_input(nw_framer_t framer, hero_tls_exchange *e, hero_tls_apple *a) {
    for (;;) {
        __block bool ended = false;
        bool parsed;

        if (e->state != HERO_TLS_EXCHANGE_MORE) {
            return 0;
        }
        parsed = nw_framer_parse_input(framer, 1, HERO_TLS_LINE_MOST, NULL, ^size_t(uint8_t *buffer, size_t length, bool is_complete) {
            size_t used = 0;

            if (buffer == NULL || length == 0) {
                ended = is_complete;
                return 0;
            }
            while (used < length && e->state == HERO_TLS_EXCHANGE_MORE && (e->kind == HERO_TLS_STEP_EXPECT || e->kind == HERO_TLS_STEP_EXPECT_LINE)) {
                (void)hero_tls_exchange_byte(e, buffer[used]);
                used += 1;
            }
            return used;
        });
        if (!parsed && ended && e->state == HERO_TLS_EXCHANGE_MORE) {
            hero_tls_exchange_closed(e);
        }
        hero_tls_apple_framer_send(framer, e);
        if (e->state == HERO_TLS_EXCHANGE_FAILED) {
            hero_tls_apple_framer_failed(framer, e, a);
            return 0;
        }
        if (e->state == HERO_TLS_EXCHANGE_DONE) {
            nw_framer_pass_through_input(framer);
            nw_framer_pass_through_output(framer);
            nw_framer_mark_ready(framer);
            return 0;
        }
        if (!parsed) {
            return 1;
        }
    }
}

/* The framer below TLS that runs `script`, a client's where `a` is its
 * session, whose words it keeps, a listener's connections' where `a` is
 * NULL. Each instance of it, one per connection, runs a machine of its own,
 * made at its start and given back at its cleanup; `script` lives while the
 * connection does, held by its session. NULL where none could be made. */
static nw_protocol_options_t hero_tls_apple_framer(hero_tls_script *script, hero_tls_apple *a) {
    nw_protocol_definition_t definition;
    nw_protocol_options_t options;

    definition = nw_framer_create_definition("org.heroes-lang.tls.before", NW_FRAMER_CREATE_FLAGS_DEFAULT, ^nw_framer_start_result_t(nw_framer_t framer) {
        hero_tls_exchange *e = hero_layer_alloc_zero(1, sizeof *e);

        if (e == NULL) {
            nw_framer_mark_failed_with_error(framer, ENOMEM);
            return nw_framer_start_result_will_mark_ready;
        }
        hero_tls_exchange_begin(e, script);
        nw_framer_set_cleanup_handler(framer, ^(nw_framer_t ended) {
            (void)ended;
            hero_layer_free(e);
        });
        nw_framer_set_output_handler(framer, ^(nw_framer_t f, nw_framer_message_t message, size_t message_length, bool is_complete) {
            /* Reached only before every step is met, since the framer then
             * passes output through: TLS above writing before that would
             * send TLS ahead of the exchange, so the connection fails. */
            (void)message;
            (void)message_length;
            (void)is_complete;
            nw_framer_mark_failed_with_error(f, EPROTO);
        });
        nw_framer_set_input_handler(framer, ^size_t(nw_framer_t f) {
            return hero_tls_apple_framer_input(f, e, a);
        });
        hero_tls_apple_framer_send(framer, e);
        if (e->state == HERO_TLS_EXCHANGE_DONE) {
            nw_framer_pass_through_input(framer);
            nw_framer_pass_through_output(framer);
            return nw_framer_start_result_ready;
        }
        return nw_framer_start_result_will_mark_ready;
    });
    if (definition == NULL) {
        return NULL;
    }
    options = nw_framer_create_options(definition);
    nw_release(definition);
    return options;
}

/* The parameters of TCP with TLS: a client's checks where `client`, with
 * `a`'s host and roots, a server's `identity` where not. The TLS options
 * are made here and put on top of TCP, so no block of this call outlives
 * it: only the verify block does, which holds `a`, counted. */
static nw_parameters_t hero_tls_apple_parameters(hero_tls_apple *a, const char *protocols, sec_identity_t identity, bool client, bool address, nw_protocol_options_t below) {
    nw_parameters_t parameters = nw_parameters_create_secure_tcp(NW_PARAMETERS_DISABLE_PROTOCOL, NW_PARAMETERS_DEFAULT_CONFIGURATION);
    nw_protocol_stack_t stack;
    nw_protocol_options_t tls;
    sec_protocol_options_t sec;

    if (parameters == NULL) {
        return NULL;
    }
    stack = nw_parameters_copy_default_protocol_stack(parameters);
    tls = nw_tls_create_options();
    sec = tls == NULL ? NULL : nw_tls_copy_sec_protocol_options(tls);
    if (stack == NULL || tls == NULL || sec == NULL) {
        if (stack != NULL) {
            nw_release(stack);
        }
        if (tls != NULL) {
            nw_release(tls);
        }
        nw_release(parameters);
        return NULL;
    }
    sec_protocol_options_set_min_tls_protocol_version(sec, tls_protocol_version_TLSv12);
    hero_tls_apple_protocols(sec, protocols);
    if (client) {
        char name[HERO_TLS_HOST_MOST + 1];

        /* SNI names a host, never an address (RFC 6066 § 3). */
        hero_tls_cf_words(a->host, name, sizeof name, "");
        if (!address && name[0] != '\0') {
            sec_protocol_options_set_tls_server_name(sec, name);
        }
        hero_tls_apple_verify(a, sec);
    } else if (identity != NULL) {
        sec_protocol_options_set_local_identity(sec, identity);
    }
    /* The framer first, then TLS put above it: TCP, the framer, TLS. */
    if (below != NULL) {
        nw_protocol_stack_prepend_application_protocol(stack, below);
    }
    nw_protocol_stack_prepend_application_protocol(stack, tls);
    sec_release(sec);
    nw_release(tls);
    nw_release(stack);
    return parameters;
}

/* A CFArray of the certificates, NULL said where one is not. */
static CFArrayRef hero_tls_apple_certificates(const hero_tls_certs *certs, const char *what) {
    CFMutableArrayRef array = CFArrayCreateMutable(NULL, (CFIndex)certs->count, &kCFTypeArrayCallBacks);
    char words[160];

    if (array == NULL) {
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    for (size_t i = 0; i < certs->count; i += 1) {
        CFDataRef data = CFDataCreate(NULL, certs->each[i].bytes, (CFIndex)certs->each[i].count);
        SecCertificateRef cert = data == NULL ? NULL : SecCertificateCreateWithData(NULL, data);

        if (data != NULL) {
            CFRelease(data);
        }
        if (cert == NULL) {
            CFRelease(array);
            (void)snprintf(words, sizeof words, "%s: block %zu is no certificate the Security framework reads", what, i + 1);
            hero_tls_said(HERO_TLS_INVALID, 0, words);
            return NULL;
        }
        CFArrayAppendValue(array, cert);
        CFRelease(cert);
    }
    return array;
}

/* The answer to a connection's failure, its state given back. */
static hero_stream *hero_tls_apple_give_up(hero_tls_apple *a, int settled) {
    if (settled == HERO_TLS_WAIT_STOPPED) {
        hero_tls_said(HERO_TLS_STOPPED, 0, "the program was asked to stop");
    } else if (settled == HERO_TLS_WAIT_TIMED_OUT) {
        hero_tls_said(HERO_TLS_TIMED_OUT, 0, "the time given passed first");
    } else {
        (void)pthread_mutex_lock(&a->lock);
        hero_tls_said(a->kind == HERO_TLS_OK ? HERO_TLS_OTHER : a->kind, a->os, a->words);
        (void)pthread_mutex_unlock(&a->lock);
    }
    nw_connection_cancel(a->connection);
    hero_tls_apple_drop(a);
    return NULL;
}

static int64_t hero_tls_apple_read(void *payload, void *buffer, uint64_t count);
static int64_t hero_tls_apple_write(void *payload, const void *data, uint64_t count);
static int64_t hero_tls_apple_close(void *payload);

static const hero_stream_ops hero_tls_apple_ops = { hero_tls_apple_read, hero_tls_apple_write, hero_tls_apple_close };

/* The connection, once ready, as a stream; NULL said where it is not. */
static hero_stream *hero_tls_apple_stream(hero_tls_apple *a, int64_t until_ns) {
    int settled = hero_tls_apple_settle(a, until_ns);
    hero_stream *stream;

    if (settled != HERO_TLS_APPLE_READY) {
        return hero_tls_apple_give_up(a, settled);
    }
    stream = hero_stream_make(a, &hero_tls_apple_ops, HERO_STREAM_KIND_SOCKET);
    if (stream == NULL) {
        nw_connection_cancel(a->connection);
        hero_tls_apple_drop(a);
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many streams as it can");
        return NULL;
    }
    hero_tls_fine();
    return stream;
}

/* Whether `host` is an address, IPv4 or IPv6, rather than a name. */
static bool hero_tls_is_address(const char *host) {
    unsigned char bytes[16];

    return inet_pton(AF_INET, host, bytes) == 1 || inet_pton(AF_INET6, host, bytes) == 1;
}

/* The monotonic time `ms` milliseconds from now, 0 for no limit: where `ms`
 * is 0, or so long that its end does not fit the clock's 64 bits (past
 * about 290 years), which no wait meets either. */
static int64_t hero_tls_until(int64_t ms) {
    int64_t now = hero_time_monotonic_ns();

    if (ms <= 0 || ms > (INT64_MAX - now) / INT64_C(1000000)) {
        return 0;
    }
    return now + ms * INT64_C(1000000);
}

/* A client's connection by name, the steps of `script` (NULL for none) run
 * by a framer below TLS: `hero_tls_connect` and `hero_tls_connect_after`. */
static hero_stream *hero_tls_connect_with(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks, hero_tls_script *script) {
    int64_t until = hero_tls_until(ms);
    nw_protocol_options_t below = NULL;
    hero_tls_apple *a;
    char port_text[8];
    nw_endpoint_t endpoint;
    nw_parameters_t parameters;

    if (!hero_tls_host_given(host) || !hero_tls_port_given(port, false) || !hero_tls_bytes_given(roots, roots_count, "the roots") || !hero_tls_protocols_given(protocols) || !hero_tls_checks_given(checks)) {
        return NULL;
    }
    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return NULL;
    }
    a = hero_tls_apple_new();
    if (a == NULL) {
        return NULL;
    }
    a->checks = checks;
    a->host = CFStringCreateWithCString(NULL, host, kCFStringEncodingUTF8);
    if (a->host == NULL) {
        hero_tls_apple_drop(a);
        hero_tls_said(HERO_TLS_INVALID, 0, "the host name is not UTF-8");
        return NULL;
    }
    if (roots_count > 0) {
        hero_tls_certs *certs = hero_layer_alloc_zero(1, sizeof *certs);

        if (certs == NULL || !hero_tls_certs_of(roots, roots_count, certs, "the roots")) {
            if (certs == NULL) {
                hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
            }
            hero_layer_free(certs);
            hero_tls_apple_drop(a);
            return NULL;
        }
        a->roots = hero_tls_apple_certificates(certs, "the roots");
        hero_tls_certs_free(certs);
        hero_layer_free(certs);
        if (a->roots == NULL) {
            hero_tls_apple_drop(a);
            return NULL;
        }
    }
    (void)snprintf(port_text, sizeof port_text, "%d", (int)port);
    endpoint = nw_endpoint_create_host(host, port_text);
    if (script != NULL) {
        hero_tls_script_keep(script);
        a->script = script;
        below = hero_tls_apple_framer(script, a);
        if (below == NULL) {
            if (endpoint != NULL) {
                nw_release(endpoint);
            }
            hero_tls_apple_drop(a);
            hero_tls_said(HERO_TLS_LIMIT, 0, "Network.framework made no framer for the steps");
            return NULL;
        }
    }
    parameters = hero_tls_apple_parameters(a, protocols, NULL, true, hero_tls_is_address(host), below);
    if (below != NULL) {
        nw_release(below);
    }
    if (endpoint == NULL || parameters == NULL) {
        if (endpoint != NULL) {
            nw_release(endpoint);
        }
        if (parameters != NULL) {
            nw_release(parameters);
        }
        hero_tls_apple_drop(a);
        hero_tls_said(HERO_TLS_INVALID, 0, "Network.framework took neither the host nor the parameters");
        return NULL;
    }
    a->connection = nw_connection_create(endpoint, parameters);
    nw_release(endpoint);
    nw_release(parameters);
    if (a->connection == NULL) {
        hero_tls_apple_drop(a);
        hero_tls_said(HERO_TLS_LIMIT, 0, "Network.framework made no connection");
        return NULL;
    }
    hero_tls_apple_watch(a);
    nw_connection_start(a->connection);
    return hero_tls_apple_stream(a, until);
}

hero_stream *hero_tls_connect(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks) {
    return hero_tls_connect_with(host, port, ms, roots, roots_count, protocols, checks, NULL);
}

hero_stream *hero_tls_connect_after(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks, const void *steps, uint64_t steps_count) {
    hero_tls_script *script;
    hero_stream *stream;

    if (!hero_tls_script_of(steps, steps_count, &script)) {
        return NULL;
    }
    stream = hero_tls_connect_with(host, port, ms, roots, roots_count, protocols, checks, script);
    hero_tls_script_drop(script);
    return stream;
}

/* What a read or a write hands its block: counted, the block's and the
 * caller's, so the one that leaves last frees it. */
typedef struct hero_tls_apple_request {
    dispatch_semaphore_t done;
    int64_t refs;
    bool finished;
    bool end;
    int64_t kind;
    int64_t os;
    size_t got;
    size_t most;
    unsigned char bytes[];
} hero_tls_apple_request;

static hero_tls_apple_request *hero_tls_apple_request_new(size_t most) {
    hero_tls_apple_request *r = hero_layer_alloc_zero(1, sizeof *r + most);

    if (r == NULL) {
        return NULL;
    }
    r->done = dispatch_semaphore_create(0);
    if (r->done == NULL) {
        hero_layer_free(r);
        return NULL;
    }
    r->refs = 2;
    r->most = most;
    return r;
}

static void hero_tls_apple_request_drop(hero_tls_apple_request *r) {
    if (__atomic_sub_fetch(&r->refs, 1, __ATOMIC_ACQ_REL) == 0) {
        dispatch_release(r->done);
        hero_layer_free(r);
    }
}

/* A connection's failure in a read or a write, as the stream's kind. */
static void hero_tls_apple_request_failed(hero_tls_apple_request *r, nw_error_t error) {
    nw_error_domain_t domain = nw_error_get_error_domain(error);
    int code = nw_error_get_error_code(error);

    r->os = code;
    if (domain == nw_error_domain_posix && (code == ECONNRESET || code == EPIPE || code == ENOTCONN || code == ECONNABORTED)) {
        r->kind = HERO_STREAM_CLOSED_PEER;
    } else if (domain == nw_error_domain_posix && code == ECANCELED) {
        r->kind = HERO_STREAM_NOT_OPEN;
    } else if (domain == nw_error_domain_posix && code == ETIMEDOUT) {
        r->kind = HERO_STREAM_TIMED_OUT;
    } else {
        r->kind = HERO_STREAM_OTHER;
    }
}

/* Waits for a request's block until the stream's deadline, read at every
 * slice so a new one counts at once: 0 once it has run, or
 * `HERO_TLS_WAIT_STOPPED`, the stop having cancelled the connection (its
 * block, run then, finds the request still there), or
 * `HERO_TLS_WAIT_TIMED_OUT`. */
static int hero_tls_apple_await(hero_tls_apple *a, hero_tls_apple_request *r) {
    for (;;) {
        int64_t until;
        int slept;

        if (__atomic_load_n(&r->finished, __ATOMIC_ACQUIRE)) {
            return 0;
        }
        (void)pthread_mutex_lock(&a->lock);
        until = a->until;
        (void)pthread_mutex_unlock(&a->lock);
        slept = hero_tls_apple_sleep(r->done, until);
        if (slept == HERO_TLS_WAIT_STOPPED) {
            nw_connection_cancel(a->connection);
            return slept;
        }
        if (slept == HERO_TLS_WAIT_TIMED_OUT) {
            return __atomic_load_n(&r->finished, __ATOMIC_ACQUIRE) ? 0 : slept;
        }
    }
}

/* Bytes a read holds past an earlier caller's count, given first. Under
 * `a->lock`; answers how many, 0 where none are held. */
static size_t hero_tls_apple_from_held(hero_tls_apple *a, void *buffer, uint64_t count) {
    size_t left = a->held_count - a->held_at;
    size_t given = left < count ? left : (size_t)count;

    if (given == 0) {
        return 0;
    }
    memcpy(buffer, a->held + a->held_at, given);
    a->held_at += given;
    if (a->held_at == a->held_count) {
        hero_layer_free(a->held);
        a->held = NULL;
        a->held_count = 0;
        a->held_at = 0;
    }
    return given;
}

static void hero_tls_apple_receive(hero_tls_apple *a, hero_tls_apple_request *r);

static int64_t hero_tls_apple_read(void *payload, void *buffer, uint64_t count) {
    hero_tls_apple *a = payload;
    hero_tls_apple_request *r;
    int64_t answer;
    size_t given;
    int awaited;

    (void)pthread_mutex_lock(&a->lock);
    given = hero_tls_apple_from_held(a, buffer, count);
    r = a->pending;
    a->pending = NULL;
    (void)pthread_mutex_unlock(&a->lock);
    if (given > 0) {
        if (r != NULL) {
            (void)pthread_mutex_lock(&a->lock);
            a->pending = r;
            (void)pthread_mutex_unlock(&a->lock);
        }
        return (int64_t)given;
    }
    if (r == NULL) {
        r = hero_tls_apple_request_new((size_t)count);
        if (r == NULL) {
            hero_stream_set_error(HERO_STREAM_LIMIT, 0);
            return -1;
        }
        hero_tls_apple_receive(a, r);
    }
    awaited = hero_tls_apple_await(a, r);
    if (awaited == HERO_TLS_WAIT_TIMED_OUT) {
        /* The receive stays asked for, and its bytes are the next read's. */
        (void)pthread_mutex_lock(&a->lock);
        a->pending = r;
        (void)pthread_mutex_unlock(&a->lock);
        hero_stream_set_error(HERO_STREAM_TIMED_OUT, 0);
        return -1;
    }
    if (awaited == HERO_TLS_WAIT_STOPPED) {
        hero_tls_apple_request_drop(r);
        hero_stream_set_error(HERO_STREAM_STOPPED, 0);
        return -1;
    }
    if (r->got > 0) {
        size_t now = r->got < count ? r->got : (size_t)count;

        memcpy(buffer, r->bytes, now);
        if (now < r->got) {
            unsigned char *rest = hero_layer_alloc(r->got - now);

            if (rest == NULL) {
                hero_tls_apple_request_drop(r);
                hero_stream_set_error(HERO_STREAM_LIMIT, 0);
                return -1;
            }
            memcpy(rest, r->bytes + now, r->got - now);
            (void)pthread_mutex_lock(&a->lock);
            a->held = rest;
            a->held_count = r->got - now;
            a->held_at = 0;
            (void)pthread_mutex_unlock(&a->lock);
        }
        answer = (int64_t)now;
    } else if (r->end || r->kind == HERO_STREAM_OK) {
        /* The peer's end, with close_notify or not, answered as the end. */
        answer = 0;
    } else {
        hero_stream_set_error(r->kind, r->os);
        answer = -1;
    }
    hero_tls_apple_request_drop(r);
    return answer;
}

/* Asks the connection for up to `r->most` bytes, which the block writes
 * into `r`, holding its own count of it. */
static void hero_tls_apple_receive(hero_tls_apple *a, hero_tls_apple_request *r) {
    nw_connection_receive(a->connection, 1, (uint32_t)r->most, ^(dispatch_data_t content, nw_content_context_t context, bool is_complete, nw_error_t error) {
        (void)context;
        if (content != NULL) {
            (void)dispatch_data_apply(content, ^bool(dispatch_data_t region, size_t offset, const void *bytes, size_t size) {
                (void)region;
                if (offset > r->most || size > r->most - offset) {
                    return false;
                }
                memcpy(r->bytes + offset, bytes, size);
                if (offset + size > r->got) {
                    r->got = offset + size;
                }
                return true;
            });
        }
        if (r->got == 0) {
            if (error != NULL) {
                hero_tls_apple_request_failed(r, error);
            } else if (is_complete) {
                r->end = true;
            }
        }
        __atomic_store_n(&r->finished, true, __ATOMIC_RELEASE);
        dispatch_semaphore_signal(r->done);
        hero_tls_apple_request_drop(r);
    });
}

static int64_t hero_tls_apple_write(void *payload, const void *data, uint64_t count) {
    hero_tls_apple *a = payload;
    hero_tls_apple_request *r;
    dispatch_data_t content;
    int64_t answer;
    bool broken;
    int awaited;

    (void)pthread_mutex_lock(&a->lock);
    broken = a->write_broken;
    (void)pthread_mutex_unlock(&a->lock);
    if (broken) {
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    r = hero_tls_apple_request_new(0);
    if (r == NULL) {
        hero_stream_set_error(HERO_STREAM_LIMIT, 0);
        return -1;
    }
    /* The default destructor copies the bytes now: the caller's buffer is
     * not read after this call. */
    content = dispatch_data_create(data, (size_t)count, a->queue, DISPATCH_DATA_DESTRUCTOR_DEFAULT);
    if (content == NULL) {
        hero_tls_apple_request_drop(r);
        hero_tls_apple_request_drop(r);
        hero_stream_set_error(HERO_STREAM_LIMIT, 0);
        return -1;
    }
    nw_connection_send(a->connection, content, NW_CONNECTION_DEFAULT_MESSAGE_CONTEXT, false, ^(nw_error_t error) {
        if (error != NULL) {
            hero_tls_apple_request_failed(r, error);
        }
        __atomic_store_n(&r->finished, true, __ATOMIC_RELEASE);
        dispatch_semaphore_signal(r->done);
        hero_tls_apple_request_drop(r);
    });
    dispatch_release(content);
    awaited = hero_tls_apple_await(a, r);
    if (awaited != 0) {
        /* A write the deadline ended may still leave in part: no later one
         * is sent after it. */
        if (awaited == HERO_TLS_WAIT_TIMED_OUT) {
            (void)pthread_mutex_lock(&a->lock);
            a->write_broken = true;
            (void)pthread_mutex_unlock(&a->lock);
        }
        hero_tls_apple_request_drop(r);
        hero_stream_set_error(awaited == HERO_TLS_WAIT_TIMED_OUT ? HERO_STREAM_TIMED_OUT : HERO_STREAM_STOPPED, 0);
        return -1;
    }
    if (r->kind != HERO_STREAM_OK) {
        hero_stream_set_error(r->kind, r->os);
        answer = -1;
    } else {
        answer = (int64_t)count;
    }
    hero_tls_apple_request_drop(r);
    return answer;
}

static int64_t hero_tls_apple_close(void *payload) {
    hero_tls_apple *a = payload;
    hero_tls_apple_request *r = hero_tls_apple_request_new(0);

    /* The close_notify and the end of the stream, as a courtesy, waited
     * for at most one second; then the connection is cancelled. */
    if (r != NULL) {
        int64_t until = hero_time_monotonic_ns() + INT64_C(1000000000);

        nw_connection_send(a->connection, NULL, NW_CONNECTION_FINAL_MESSAGE_CONTEXT, true, ^(nw_error_t error) {
            (void)error;
            __atomic_store_n(&r->finished, true, __ATOMIC_RELEASE);
            dispatch_semaphore_signal(r->done);
            hero_tls_apple_request_drop(r);
        });
        while (!__atomic_load_n(&r->finished, __ATOMIC_ACQUIRE) && hero_tls_apple_sleep(r->done, until) == 0) {
        }
        hero_tls_apple_request_drop(r);
    }
    nw_connection_cancel(a->connection);
    hero_tls_apple_drop(a);
    return 0;
}

/* The state of a stream this arm made, held until `hero_stream_release`. */
static hero_tls_apple *hero_tls_apple_held(hero_stream *s) {
    void *payload = NULL;

    if (hero_stream_hold_payload(s, &hero_tls_apple_ops, &payload) < 0) {
        hero_tls_said(HERO_TLS_NOT_OPEN, 0, "the stream is no open TLS stream");
        return NULL;
    }
    return payload;
}

int64_t hero_tls_version(hero_stream *s) {
    hero_tls_apple *a = hero_tls_apple_held(s);
    int64_t version;

    if (a == NULL) {
        return -1;
    }
    (void)pthread_mutex_lock(&a->lock);
    version = a->version;
    (void)pthread_mutex_unlock(&a->lock);
    (void)hero_stream_release(s);
    hero_tls_fine();
    return version;
}

int64_t hero_tls_protocol(hero_stream *s, void *buffer, uint64_t count) {
    hero_tls_apple *a;
    size_t length;

    if (buffer == NULL || count < HERO_TLS_PROTOCOL_MOST) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the buffer is shorter than HERO_TLS_PROTOCOL_MOST");
        return -1;
    }
    a = hero_tls_apple_held(s);
    if (a == NULL) {
        return -1;
    }
    (void)pthread_mutex_lock(&a->lock);
    length = hero_tls_copy_text((char *)buffer, HERO_TLS_PROTOCOL_MOST, a->protocol);
    (void)pthread_mutex_unlock(&a->lock);
    (void)hero_stream_release(s);
    hero_tls_fine();
    return (int64_t)length;
}

int64_t hero_tls_set_deadline(hero_stream *s, int64_t ms) {
    hero_tls_apple *a;

    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return -1;
    }
    a = hero_tls_apple_held(s);
    if (a == NULL) {
        return -1;
    }
    (void)pthread_mutex_lock(&a->lock);
    a->until = hero_tls_until(ms);
    (void)pthread_mutex_unlock(&a->lock);
    (void)hero_stream_release(s);
    hero_tls_fine();
    return 0;
}

int64_t hero_tls_available(void) {
    return 1;
}

hero_stream *hero_tls_client(hero_stream *inner, const char *host, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks) {
    (void)inner;
    (void)host;
    (void)roots;
    (void)roots_count;
    (void)protocols;
    (void)checks;
    hero_tls_said(HERO_TLS_UNSUPPORTED, 0, "on macOS TLS runs only over a connection Network.framework makes (hero_tls_connect): TLS over a stream the program holds is Secure Transport's alone, deprecated since macOS 10.15");
    return NULL;
}

hero_stream *hero_tls_server(hero_stream *inner, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols) {
    (void)inner;
    (void)chain;
    (void)chain_count;
    (void)key;
    (void)key_count;
    (void)protocols;
    hero_tls_said(HERO_TLS_UNSUPPORTED, 0, "on macOS TLS runs only over a connection Network.framework makes (hero_tls_accept): TLS over a stream the program holds is Secure Transport's alone, deprecated since macOS 10.15");
    return NULL;
}

/* The most connections a listener holds that nobody has accepted yet; one
 * past it is turned away, as a full backlog turns one away. */
#define HERO_TLS_WAITING_MOST 128u

struct hero_tls_spot {
    nw_listener_t listener;
    dispatch_queue_t queue;
    dispatch_semaphore_t changed;
    pthread_mutex_t lock;
    int state;
    bool closed;
    int64_t refs;
    int64_t kind;
    int64_t os;
    char words[HERO_TLS_WORDS_MOST];
    nw_connection_t waiting[HERO_TLS_WAITING_MOST];
    size_t head;
    size_t count;
    sec_identity_t identity;
    /* The steps each connection runs before TLS, counted (NULL for none). */
    hero_tls_script *script;
    /* The deadline of the wait for a client, monotonic, 0 for none. Under
     * `lock`. */
    int64_t accept_until;
    /* Whether `nw_listener_cancel` was asked, so it is asked once. Under
     * `lock`. */
    bool cancel_asked;
};

static void hero_tls_spot_drop(hero_tls_spot *spot) {
    if (__atomic_sub_fetch(&spot->refs, 1, __ATOMIC_ACQ_REL) != 0) {
        return;
    }
    if (spot->listener != NULL) {
        nw_release(spot->listener);
    }
    if (spot->identity != NULL) {
        sec_release(spot->identity);
    }
    hero_tls_script_drop(spot->script);
    dispatch_release(spot->queue);
    dispatch_release(spot->changed);
    (void)pthread_mutex_destroy(&spot->lock);
    hero_layer_free(spot);
}

/* Asks the listener's cancel, the first time only. Without `lock`. */
static void hero_tls_spot_cancel(hero_tls_spot *spot) {
    bool first;

    (void)pthread_mutex_lock(&spot->lock);
    first = !spot->cancel_asked;
    spot->cancel_asked = true;
    (void)pthread_mutex_unlock(&spot->lock);
    if (first && spot->listener != NULL) {
        nw_listener_cancel(spot->listener);
    }
}

/* Marks the listener closed, so a call waiting in it answers
 * `HERO_TLS_NOT_OPEN`, and returns once it has stopped listening: at the
 * cancelled state its handler reports, or at the stop, the cancel then
 * going on alone. `nw_listener_cancel` only asks: until the listener's
 * queue reaches the cancelled state its port still takes a connection and
 * drops it unaccepted, so a client connecting there after the close had
 * returned saw its TLS handshake broken off, `failed` ("server closed
 * session with no notification", -9816), where nothing listened (defect
 * 2623: 7 connects of 200, and a plain TCP connect taken 1 time in 200, on
 * a loaded Mac). The socket arm's close is a socket's close, done before it
 * returns. The caller holds the slot busy, so the table's count keeps the
 * spot while this waits. */
static void hero_tls_spot_wake(hero_tls_spot *spot) {
    (void)pthread_mutex_lock(&spot->lock);
    spot->closed = true;
    (void)pthread_mutex_unlock(&spot->lock);
    dispatch_semaphore_signal(spot->changed);
    hero_tls_spot_cancel(spot);
    for (;;) {
        int state;

        (void)pthread_mutex_lock(&spot->lock);
        state = spot->state;
        (void)pthread_mutex_unlock(&spot->lock);
        if (state == HERO_TLS_APPLE_CANCELLED || hero_tls_apple_sleep(spot->changed, 0) != 0) {
            return;
        }
    }
}

static void hero_tls_spot_free(hero_tls_spot *spot) {
    (void)pthread_mutex_lock(&spot->lock);
    spot->closed = true;
    while (spot->count > 0) {
        nw_connection_t c = spot->waiting[spot->head];

        spot->head = (spot->head + 1) % HERO_TLS_WAITING_MOST;
        spot->count -= 1;
        nw_connection_set_queue(c, spot->queue);
        nw_connection_cancel(c);
        nw_release(c);
    }
    (void)pthread_mutex_unlock(&spot->lock);
    hero_tls_spot_cancel(spot);
    hero_tls_spot_drop(spot);
}

/* The identity a listener presents, from the chain and the key read. */
static sec_identity_t hero_tls_apple_identity(const hero_tls_certs *chain, const hero_tls_key *key) {
    CFArrayRef certificates = hero_tls_apple_certificates(chain, "the chain");
    CFDataRef data;
    CFMutableDictionaryRef attributes;
    SecKeyRef private_key;
    SecIdentityRef identity;
    sec_identity_t answer;
    CFErrorRef error = NULL;

    if (certificates == NULL) {
        return NULL;
    }
    data = CFDataCreate(NULL, key->own.bytes, (CFIndex)key->own.count);
    attributes = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    if (data == NULL || attributes == NULL) {
        if (data != NULL) {
            CFRelease(data);
        }
        if (attributes != NULL) {
            CFRelease(attributes);
        }
        CFRelease(certificates);
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    CFDictionarySetValue(attributes, kSecAttrKeyType, key->kind == HERO_TLS_KEY_RSA ? kSecAttrKeyTypeRSA : kSecAttrKeyTypeECSECPrimeRandom);
    CFDictionarySetValue(attributes, kSecAttrKeyClass, kSecAttrKeyClassPrivate);
    private_key = SecKeyCreateWithData(data, attributes, &error);
    CFRelease(data);
    CFRelease(attributes);
    if (private_key == NULL) {
        char words[HERO_TLS_WORDS_MOST];

        hero_tls_cf_error_words(error, "the Security framework could not read the key", words, sizeof words);
        hero_tls_said(HERO_TLS_INVALID, error == NULL ? 0 : (int64_t)CFErrorGetCode(error), words);
        if (error != NULL) {
            CFRelease(error);
        }
        CFRelease(certificates);
        return NULL;
    }
    identity = SecIdentityCreate(NULL, (SecCertificateRef)CFArrayGetValueAtIndex(certificates, 0), private_key);
    CFRelease(private_key);
    if (identity == NULL) {
        CFRelease(certificates);
        hero_tls_said(HERO_TLS_INVALID, 0, "the key is not the certificate's");
        return NULL;
    }
    answer = sec_identity_create_with_certificates(identity, certificates);
    CFRelease(identity);
    CFRelease(certificates);
    if (answer == NULL) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the Security framework made no identity of the chain and the key");
    }
    return answer;
}

/* A listener whose connections run the steps of `script` (NULL for none)
 * by a framer below TLS. `*taken` says whether the listener took the
 * caller's count of `script`. */
static hero_tls_listener *hero_tls_listen_made(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols, hero_tls_script *script, bool *taken) {
    nw_protocol_options_t below = NULL;
    hero_tls_spot *spot;
    hero_tls_certs *certs;
    hero_tls_key read_key;
    hero_tls_apple helper;
    nw_parameters_t parameters;
    char port_text[8];
    int settled;
    hero_tls_listener *l;

    if (address == NULL) {
        hero_tls_said(HERO_TLS_INVALID, 0, "no address was given");
        return NULL;
    }
    if (!hero_tls_port_given(port, true) || !hero_tls_bytes_given(chain, chain_count, "the chain") || !hero_tls_bytes_given(key, key_count, "the key") || !hero_tls_protocols_given(protocols)) {
        return NULL;
    }
    certs = hero_layer_alloc_zero(1, sizeof *certs);
    spot = hero_layer_alloc_zero(1, sizeof *spot);
    if (certs == NULL || spot == NULL) {
        hero_layer_free(certs);
        hero_layer_free(spot);
        hero_tls_said(HERO_TLS_LIMIT, 0, "no memory left for TLS");
        return NULL;
    }
    if (!hero_tls_certs_of(chain, chain_count, certs, "the chain")) {
        hero_layer_free(certs);
        hero_layer_free(spot);
        return NULL;
    }
    if (!hero_tls_key_of(key, key_count, &read_key)) {
        hero_tls_certs_free(certs);
        hero_layer_free(certs);
        hero_layer_free(spot);
        return NULL;
    }
    spot->identity = hero_tls_apple_identity(certs, &read_key);
    hero_tls_key_free(&read_key);
    hero_tls_certs_free(certs);
    hero_layer_free(certs);
    if (spot->identity == NULL) {
        hero_layer_free(spot);
        return NULL;
    }
    spot->queue = dispatch_queue_create("org.heroes-lang.tls.listener", DISPATCH_QUEUE_SERIAL);
    spot->changed = dispatch_semaphore_create(0);
    (void)pthread_mutex_init(&spot->lock, NULL);
    /* The listener's handlers hold one count until it is cancelled, the
     * table the other. */
    spot->refs = 2;
    spot->script = script;
    *taken = true;
    if (script != NULL) {
        below = hero_tls_apple_framer(script, NULL);
    }
    memset(&helper, 0, sizeof helper);
    parameters = script != NULL && below == NULL ? NULL : hero_tls_apple_parameters(&helper, protocols, spot->identity, false, false, below);
    if (below != NULL) {
        nw_release(below);
    }
    (void)snprintf(port_text, sizeof port_text, "%d", (int)port);
    if (parameters != NULL && address[0] != '\0') {
        nw_endpoint_t local = nw_endpoint_create_host(address, port_text);

        if (local != NULL) {
            nw_parameters_set_local_endpoint(parameters, local);
            nw_release(local);
        }
        spot->listener = nw_listener_create(parameters);
    } else if (parameters != NULL) {
        spot->listener = nw_listener_create_with_port(port_text, parameters);
    }
    if (parameters != NULL) {
        nw_release(parameters);
    }
    if (spot->listener == NULL || spot->queue == NULL || spot->changed == NULL) {
        spot->refs = 1;
        hero_tls_spot_drop(spot);
        hero_tls_said(HERO_TLS_INVALID, 0, "Network.framework made no listener on that address and port");
        return NULL;
    }
    nw_listener_set_queue(spot->listener, spot->queue);
    nw_listener_set_new_connection_handler(spot->listener, ^(nw_connection_t connection) {
        bool kept = false;

        (void)pthread_mutex_lock(&spot->lock);
        if (!spot->closed && spot->count < HERO_TLS_WAITING_MOST) {
            nw_retain(connection);
            spot->waiting[(spot->head + spot->count) % HERO_TLS_WAITING_MOST] = connection;
            spot->count += 1;
            kept = true;
        }
        (void)pthread_mutex_unlock(&spot->lock);
        if (!kept) {
            nw_connection_set_queue(connection, spot->queue);
            nw_connection_cancel(connection);
        }
        dispatch_semaphore_signal(spot->changed);
    });
    nw_listener_set_state_changed_handler(spot->listener, ^(nw_listener_state_t state, nw_error_t error) {
        bool cancelled = false;

        (void)pthread_mutex_lock(&spot->lock);
        if (state == nw_listener_state_ready && spot->state == HERO_TLS_APPLE_WAITING) {
            spot->state = HERO_TLS_APPLE_READY;
        } else if ((state == nw_listener_state_failed || state == nw_listener_state_waiting) && spot->state != HERO_TLS_APPLE_CANCELLED) {
            hero_tls_apple helper_failed;

            memset(&helper_failed, 0, sizeof helper_failed);
            hero_tls_apple_failed(&helper_failed, error);
            spot->kind = helper_failed.kind;
            spot->os = helper_failed.os;
            (void)hero_tls_copy_text(spot->words, sizeof spot->words, helper_failed.words);
            spot->state = HERO_TLS_APPLE_FAILED;
        } else if (state == nw_listener_state_cancelled) {
            spot->state = HERO_TLS_APPLE_CANCELLED;
            cancelled = true;
        }
        (void)pthread_mutex_unlock(&spot->lock);
        dispatch_semaphore_signal(spot->changed);
        if (cancelled) {
            nw_listener_set_state_changed_handler(spot->listener, NULL);
            nw_listener_set_new_connection_handler(spot->listener, NULL);
            hero_tls_spot_drop(spot);
        }
    });
    nw_listener_start(spot->listener);
    for (;;) {
        (void)pthread_mutex_lock(&spot->lock);
        settled = spot->state;
        (void)pthread_mutex_unlock(&spot->lock);
        if (settled != HERO_TLS_APPLE_WAITING) {
            break;
        }
        settled = hero_tls_apple_sleep(spot->changed, 0);
        if (settled != 0) {
            break;
        }
    }
    if (settled != HERO_TLS_APPLE_READY) {
        if (settled == HERO_TLS_WAIT_STOPPED) {
            hero_tls_said(HERO_TLS_STOPPED, 0, "the program was asked to stop");
        } else {
            (void)pthread_mutex_lock(&spot->lock);
            hero_tls_said(spot->kind == HERO_TLS_OK ? HERO_TLS_OTHER : spot->kind, spot->os, spot->words);
            (void)pthread_mutex_unlock(&spot->lock);
        }
        hero_tls_spot_free(spot);
        return NULL;
    }
    l = hero_tls_issue(spot);
    if (l == NULL) {
        hero_tls_spot_free(spot);
        hero_tls_said(HERO_TLS_LIMIT, 0, "the layer holds as many TLS listeners as it can, 4096");
        return NULL;
    }
    hero_tls_fine();
    return l;
}

/* `hero_tls_listen_made`, the caller's count of `script` given back where
 * the listener did not take it. */
static hero_tls_listener *hero_tls_listen_with(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols, hero_tls_script *script) {
    bool taken = false;
    hero_tls_listener *l = hero_tls_listen_made(address, port, chain, chain_count, key, key_count, protocols, script, &taken);

    if (!taken) {
        hero_tls_script_drop(script);
    }
    return l;
}

hero_tls_listener *hero_tls_listen(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols) {
    return hero_tls_listen_with(address, port, chain, chain_count, key, key_count, protocols, NULL);
}

hero_tls_listener *hero_tls_listen_after(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols, const void *steps, uint64_t steps_count) {
    hero_tls_script *script;

    if (!hero_tls_script_of(steps, steps_count, &script)) {
        return NULL;
    }
    return hero_tls_listen_with(address, port, chain, chain_count, key, key_count, protocols, script);
}

int64_t hero_tls_port(hero_tls_listener *l) {
    hero_tls_slot *slot;
    hero_tls_spot *spot = hero_tls_enter(l, &slot);
    int64_t port;

    if (spot == NULL) {
        return -1;
    }
    port = (int64_t)nw_listener_get_port(spot->listener);
    hero_tls_leave(slot);
    hero_tls_fine();
    return port;
}

int64_t hero_tls_set_accept_deadline(hero_tls_listener *l, int64_t ms) {
    hero_tls_slot *slot;
    hero_tls_spot *spot;

    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return -1;
    }
    spot = hero_tls_enter(l, &slot);
    if (spot == NULL) {
        return -1;
    }
    (void)pthread_mutex_lock(&spot->lock);
    spot->accept_until = hero_tls_until(ms);
    (void)pthread_mutex_unlock(&spot->lock);
    /* A wait already begun looks at it at once. */
    dispatch_semaphore_signal(spot->changed);
    hero_tls_leave(slot);
    hero_tls_fine();
    return 0;
}

hero_stream *hero_tls_accept(hero_tls_listener *l, int64_t ms) {
    hero_tls_slot *slot;
    hero_tls_spot *spot;
    nw_connection_t connection = NULL;
    hero_tls_apple *a;

    if (ms < 0) {
        hero_tls_said(HERO_TLS_INVALID, 0, "the time given is below 0");
        return NULL;
    }
    spot = hero_tls_enter(l, &slot);
    if (spot == NULL) {
        return NULL;
    }
    for (;;) {
        bool closed;
        int64_t until;
        int slept;

        (void)pthread_mutex_lock(&spot->lock);
        closed = spot->closed;
        until = spot->accept_until;
        if (!closed && spot->count > 0) {
            connection = spot->waiting[spot->head];
            spot->head = (spot->head + 1) % HERO_TLS_WAITING_MOST;
            spot->count -= 1;
        }
        (void)pthread_mutex_unlock(&spot->lock);
        if (closed || hero_tls_closing(slot)) {
            hero_tls_leave(slot);
            hero_tls_said(HERO_TLS_NOT_OPEN, 0, "the TLS listener was closed");
            return NULL;
        }
        if (connection != NULL) {
            break;
        }
        slept = hero_tls_apple_sleep(spot->changed, until);
        if (slept == HERO_TLS_WAIT_STOPPED) {
            hero_tls_leave(slot);
            hero_tls_said(HERO_TLS_STOPPED, 0, "the program was asked to stop");
            return NULL;
        }
        if (slept == HERO_TLS_WAIT_TIMED_OUT) {
            hero_tls_leave(slot);
            hero_tls_said(HERO_TLS_TIMED_OUT, 0, "the time given passed first");
            return NULL;
        }
    }
    a = hero_tls_apple_new();
    if (a == NULL) {
        /* Turned away on the listener's queue, while the slot still holds
         * the listener. */
        nw_connection_set_queue(connection, spot->queue);
        nw_connection_cancel(connection);
        nw_release(connection);
        hero_tls_leave(slot);
        return NULL;
    }
    /* The listener's steps, which its framer runs at this connection's
     * start, held by the connection, since the listener may close first. */
    hero_tls_script_keep(spot->script);
    a->script = spot->script;
    hero_tls_leave(slot);
    a->connection = connection;
    hero_tls_apple_watch(a);
    nw_connection_start(a->connection);
    return hero_tls_apple_stream(a, hero_tls_until(ms));
}

#endif
