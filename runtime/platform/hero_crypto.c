/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_crypto.c — cryptography by the system's own library, one
 * member of the archive the compiler puts on every program's link line
 * (M-rebel-library; `selfhost/cli/layer.hero` builds, keys and links it).
 * Its header, `hero_crypto.h`, says what each function answers; this file
 * says how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * WHAT EACH SYSTEM OFFERS, MEASURED 2026-11-17 by a C probe, not kept, of
 * each public, non-deprecated API, the macOS one compiled with
 * `-Werror=deprecated-declarations`, the Linux one with
 * `OPENSSL_NO_DEPRECATED`, every capability exercised and checked back:
 *
 *   capability                  macOS 27.2           Linux (OpenSSL 3.5.7)   Windows (CNG)
 *   AES-128/256-GCM             CryptoKit (1)        EVP_aes_*_gcm           BCrypt AES + GCM (3)
 *   ChaCha20-Poly1305           CryptoKit (1)        EVP_chacha20_poly1305   BCrypt CHACHA20_POLY1305 (3)
 *   Ed25519 sign, verify        Monocypher (2)       EVP_DigestSign          no algorithm (3)
 *   ECDSA P-256, P-384          SecKeyCreateSignature EVP_DigestSign          BCryptSignHash (3)
 *   RSA-PSS, PKCS #1 v1.5       SecKeyCreateSignature EVP_DigestSign          BCryptSignHash (3)
 *   RSA-OAEP                    SecKeyCreateEncryptedData EVP_PKEY_encrypt   BCryptEncrypt (3)
 *   X25519                      Monocypher (2)       EVP_PKEY_derive         curve25519, unmeasured (4)
 *   ECDH P-256, P-384           SecKeyCopyKeyExchangeResult EVP_PKEY_derive  BCryptSecretAgreement (3)
 *   key generation              SecKeyCreateRandomKey EVP_PKEY_generate      BCryptGenerateKeyPair (3)
 *   PKCS #8 private, DER        no (5)               i2d/d2i PKCS8           no (5)
 *   SPKI public, DER            SecItemExport (5)    i2d/d2i_PUBKEY          no (5)
 *   PEM                         SecItemExport, not PKCS #8 (5)  PEM_*         CryptStringToBinary (5)
 *
 *   (1) CommonCrypto's public modes are ECB, CBC, CFB, CTR, OFB, RC4 and
 *       CFB8 (`CommonCryptor.h`); its GCM is in a private header the SDK
 *       does not ship. CryptoKit has both ciphers and is Swift's alone, so
 *       since 2026-11-20 the layer holds a member written in Swift for them,
 *       `hero_crypto_cryptokit.swift` (panel 253's R3, the mixed route;
 *       `selfhost/cli/layer_swift.hero` compiles it on an Apple target).
 *   (2) `SecKey` takes RSA and the NIST curves (`kSecAttrKeyType*`), no
 *       Curve25519 key; CryptoKit's `Curve25519` is Swift's alone and its
 *       Ed25519 signs differently every time, never RFC 8032's bytes (panel
 *       253's ffi-pragmatist and critic, each by a build). So since
 *       2026-11-20 both are Monocypher 4.0.3's, vendored and compiled inside
 *       this member (THE APPLE ARM; decision 2594 admits a vendored C
 *       library for primitives that read no outside data).
 *   (3) From CNG's documentation, read 2026-11-17: ChaCha20-Poly1305 is
 *       listed from Windows 10, no identifier names Ed25519 or EdDSA. The
 *       Windows box was off: every Windows line is unrun.
 *   (4) CNG names `curve25519` for ECDH and documents neither its key
 *       blob's byte order nor its raw secret's for that curve, so the arm
 *       answers `UNSUPPORTED` for X25519 until the box measures it.
 *   (5) Form, not cryptography: `SecKeyCreateWithData` reads PKCS #1 and
 *       X9.63 alone (measured: PKCS #8 refused), and SecItemExport writes a
 *       private key in OpenSSL's older forms; CNG's blobs are its own. So
 *       this member reads and writes PKCS #8 and SPKI itself (THE FORMS
 *       below) on every platform, the same bytes everywhere; PEM's base64
 *       is the face's.
 *
 * WHAT THE ARMS ANSWERED, 2026-11-17, through a C probe of this member, not
 * kept, over Wycheproof's files (3,173 applicable cases) and NIST CAVP's
 * AES-GCM (1,500): Linux arm64 0 failed of all, under ASan and UBSan; macOS
 * 0 failed of every case of a kind it offers, under ASan and UBSan, and
 * `leaks --atExit` 0 leaks; Windows unrun that day, and run on 2026-11-18
 * (THE WINDOWS ARM, below). The Security framework held ECDSA signatures in
 * BER and an RSA signature past its modulus, Monocypher an Ed25519 R whose
 * encoding RFC 8032 § 5.1.3 does not decode, and Monocypher and OpenSSL
 * alike an Ed25519 key of such an encoding, so the common part reads a
 * signature's one form, and the key's, before any arm sees them
 * (`hero_crypto_signature_formed`, defect 2641).
 *
 * THREE ARMS, ONE SHAPE. The common part (the last failure, the table of
 * keys, THE FORMS, the checks of every argument) is written once; each arm
 * supplies the functions under THE ARMS' CONTRACT, and no arm reaches
 * another's state. A key's secret lives in the arm's own object (a
 * `SecKeyRef`, an `EVP_PKEY`) or in a block of the layer's, which is
 * overwritten with zeros before it is given back.
 *
 * THE LIBRARIES ARE LINKED BY THE MEMBER ITSELF, as `hero_tls.c` links
 * its own: on Windows `bcrypt` by `#pragma comment(lib)`, on macOS the
 * Security and CoreFoundation frameworks by a `.linker_option` the object
 * carries, and CryptoKit, Foundation and the Swift runtime by the entries the
 * CryptoKit member's Swift object carries; on Linux nothing, since
 * `libcrypto.so.3` is opened with `dlopen` at the first call that needs it.
 * A program that never calls this member links none of them.
 *
 * MEMORY. Every block is the layer's (`hero_layer_alloc.h`, given by
 * `parts/alloc.c`, the single allocation point); a block that is not there
 * is `HERO_CRYPTO_LIMIT`, never an abort. The table of keys is fixed, as
 * `hero_net.c`'s table of sockets is.
 *
 * Written from the platforms' documentation: Apple's headers for the
 * Security framework and CommonCrypto, OpenSSL's manual pages, the Win32
 * reference for CNG; RFC 5208, 5280, 5915, 5958, 8017 and 8410 for the
 * forms; Monocypher's manual pages for the five functions the Apple arm
 * calls, whose source is compiled in as released, not one byte edited
 * (`runtime/vendor/monocypher/README.md`); no other library's source.
 */

#include "hero_crypto.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>
/* What only a program that takes this member links. MSVC's word, which
 * clang reads on the MSVC target as `hero_net.c` has it; a MinGW target
 * links it by `-lbcrypt`. */
#if defined(_MSC_VER)
#pragma comment(lib, "bcrypt.lib")
#endif
/* CNG's documented identifier, which MinGW's `bcrypt.h` does not name. */
#ifndef BCRYPT_CHACHA20_POLY1305_ALGORITHM
#define BCRYPT_CHACHA20_POLY1305_ALGORITHM L"CHACHA20_POLY1305"
#endif
#elif defined(__APPLE__)
#include "hero_crypto_cryptokit.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#include <pthread.h>
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
#define HERO_CRYPTO_WORDS_MOST 512

static _Thread_local int64_t hero_crypto_kind_said;
static _Thread_local int64_t hero_crypto_os_said;
static _Thread_local char hero_crypto_words[HERO_CRYPTO_WORDS_MOST];

/* `text` copied into `into`, `most` bytes with the zero, cut where a
 * character ends; answers the length copied. */
static size_t hero_crypto_copy_text(char *into, size_t most, const char *text) {
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
static void hero_crypto_said(int64_t kind, int64_t os, const char *words) {
    hero_crypto_kind_said = kind;
    hero_crypto_os_said = os;
    (void)hero_crypto_copy_text(hero_crypto_words, sizeof hero_crypto_words, words);
}

static void hero_crypto_fine(void) {
    hero_crypto_said(HERO_CRYPTO_OK, 0, NULL);
}

int64_t hero_crypto_last_error(void) {
    return hero_crypto_kind_said;
}

int64_t hero_crypto_last_os_error(void) {
    return hero_crypto_os_said;
}

int64_t hero_crypto_last_message(void *buffer, uint64_t count) {
    if (buffer == NULL || count == 0) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "no room was given for the message");
        return -1;
    }
    return (int64_t)hero_crypto_copy_text((char *)buffer, count > SIZE_MAX ? SIZE_MAX : (size_t)count, hero_crypto_words);
}

/* -- SECRETS OVERWRITTEN ---------------------------------------------------- */

/* `count` bytes at `bytes` overwritten with zeros through a volatile
 * pointer, which the compiler may not drop as a store nobody reads. */
static void hero_crypto_wipe(void *bytes, size_t count) {
    volatile unsigned char *at = (volatile unsigned char *)bytes;

    if (bytes == NULL) {
        return;
    }
    while (count > 0) {
        *at = 0;
        at += 1;
        count -= 1;
    }
}

/* Bytes in a block of the layer's, overwritten before they are given back:
 * every blob may hold a secret. */
typedef struct hero_crypto_blob {
    unsigned char *bytes;
    size_t count;
} hero_crypto_blob;

static void hero_crypto_blob_free(hero_crypto_blob *b) {
    if (b->bytes != NULL) {
        hero_crypto_wipe(b->bytes, b->count);
        hero_layer_free(b->bytes);
    }
    b->bytes = NULL;
    b->count = 0;
}

/* A new blob of `n` parts joined, each `counts[i]` bytes; false with
 * `HERO_CRYPTO_LIMIT` where no block is left. */
static bool hero_crypto_join(hero_crypto_blob *out, const unsigned char *const *parts, const size_t *counts, size_t n) {
    size_t total = 0;

    for (size_t i = 0; i < n; i += 1) {
        total += counts[i];
    }
    out->bytes = hero_layer_alloc(total == 0 ? 1 : total);
    out->count = 0;
    if (out->bytes == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the key");
        return false;
    }
    for (size_t i = 0; i < n; i += 1) {
        if (counts[i] > 0) {
            memcpy(out->bytes + out->count, parts[i], counts[i]);
        }
        out->count += counts[i];
    }
    return true;
}

/* -- THE CHECKS EVERY CALL MAKES ------------------------------------------- */

/* Whether `count` bytes stand behind `bytes`, at most `most` of them;
 * refused as `what` where not. */
static bool hero_crypto_bytes_given(const void *bytes, uint64_t count, uint64_t most, const char *what) {
    char words[160];

    if (bytes == NULL && count > 0) {
        (void)snprintf(words, sizeof words, "%s: a count of %llu with no bytes behind it", what, (unsigned long long)count);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, words);
        return false;
    }
    if (count > most) {
        (void)snprintf(words, sizeof words, "%s: %llu bytes, past the %llu a call takes", what, (unsigned long long)count, (unsigned long long)most);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, words);
        return false;
    }
    return true;
}

static bool hero_crypto_is_aead(int64_t kind) {
    return kind == HERO_CRYPTO_AES_128_GCM || kind == HERO_CRYPTO_AES_256_GCM || kind == HERO_CRYPTO_CHACHA20_POLY1305;
}

static bool hero_crypto_is_pair(int64_t kind) {
    return kind == HERO_CRYPTO_ED25519 || kind == HERO_CRYPTO_X25519 || kind == HERO_CRYPTO_P256 || kind == HERO_CRYPTO_P384 || kind == HERO_CRYPTO_RSA;
}

static bool hero_crypto_is_scheme(int64_t what) {
    return (what >= HERO_CRYPTO_SIGN_ED25519 && what <= HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512) || what == HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256;
}

/* The key kind a scheme works with. */
static int64_t hero_crypto_scheme_kind(int64_t scheme) {
    if (scheme == HERO_CRYPTO_SIGN_ED25519) {
        return HERO_CRYPTO_ED25519;
    }
    if (scheme == HERO_CRYPTO_SIGN_ECDSA_SHA256 || scheme == HERO_CRYPTO_SIGN_ECDSA_SHA384) {
        return HERO_CRYPTO_P256;
    }
    return HERO_CRYPTO_RSA;
}

/* Whether a key of `kind` works with `scheme`: an ECDSA scheme on either
 * curve. */
static bool hero_crypto_scheme_fits(int64_t scheme, int64_t kind) {
    int64_t wanted = hero_crypto_scheme_kind(scheme);

    if (wanted == HERO_CRYPTO_P256) {
        return kind == HERO_CRYPTO_P256 || kind == HERO_CRYPTO_P384;
    }
    return kind == wanted;
}

static const char *hero_crypto_kind_name(int64_t kind) {
    switch (kind) {
    case HERO_CRYPTO_AES_128_GCM:
        return "AES-128-GCM";
    case HERO_CRYPTO_AES_256_GCM:
        return "AES-256-GCM";
    case HERO_CRYPTO_CHACHA20_POLY1305:
        return "ChaCha20-Poly1305";
    case HERO_CRYPTO_ED25519:
        return "Ed25519";
    case HERO_CRYPTO_X25519:
        return "X25519";
    case HERO_CRYPTO_P256:
        return "P-256";
    case HERO_CRYPTO_P384:
        return "P-384";
    case HERO_CRYPTO_RSA:
        return "RSA";
    default:
        return "no kind";
    }
}

/* -- THE FORMS: PKCS #8 AND SPKI, READ AND WRITTEN --------------------------
 *
 * DER as X.690 rules it, read strictly: a definite length in its shortest
 * form, nothing after the outermost element. What a key reads into is the
 * MATERIAL, the same fields on every platform; an arm makes its own object
 * from it and gives it back, so the bytes written are the same everywhere,
 * and the same as Go's `x509.MarshalPKCS8PrivateKey` and Python's
 * `PrivateFormat.PKCS8` (measured 2026-11-17 on a P-256 key: version 0, the
 * EC key's own version 1, its public point, its curve not repeated). */

typedef struct hero_crypto_span {
    const unsigned char *p;
    size_t n;
} hero_crypto_span;

/* The element at the front of `*s` whose tag is `tag`: its content into
 * `*inner`, `*s` moved past it; false where the front is no such element or
 * its length is not in DER's shortest form. */
static bool hero_crypto_der(hero_crypto_span *s, unsigned char tag, hero_crypto_span *inner) {
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

        if (bytes == 0 || bytes > 3 || s->n < 2 + bytes || s->p[2] == 0) {
            return false;
        }
        length = 0;
        for (size_t i = 0; i < bytes; i += 1) {
            length = (length << 8) | s->p[2 + i];
        }
        if (length < 0x80u) {
            return false;
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

static bool hero_crypto_span_is(hero_crypto_span s, const unsigned char *bytes, size_t count) {
    return s.n == count && memcmp(s.p, bytes, count) == 0;
}

/* A DER INTEGER that is not negative, its bytes without the zero DER puts
 * before a high bit; false for a negative one, or one not in its shortest
 * form. */
static bool hero_crypto_der_unsigned(hero_crypto_span *s, hero_crypto_span *value) {
    hero_crypto_span inner;

    if (!hero_crypto_der(s, 0x02, &inner) || inner.n == 0 || (inner.p[0] & 0x80u) != 0) {
        return false;
    }
    if (inner.n > 1 && inner.p[0] == 0) {
        if ((inner.p[1] & 0x80u) == 0) {
            return false;
        }
        inner.p += 1;
        inner.n -= 1;
    }
    *value = inner;
    return true;
}

/* The bits of a number written big-endian with no leading zero byte. */
static int64_t hero_crypto_bit_length(hero_crypto_span number) {
    int64_t bits;
    unsigned char top;

    if (number.n == 0) {
        return 0;
    }
    bits = (int64_t)(number.n - 1) * 8;
    top = number.p[0];
    while (top != 0) {
        bits += 1;
        top = (unsigned char)(top >> 1);
    }
    return bits;
}

/* A DER element's head, `tag` and `length`, written at `into`; answers its
 * length, at most 5. */
static size_t hero_crypto_der_head(unsigned char *into, unsigned char tag, size_t length) {
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

/* `inner` wrapped in an element of `tag`, a new blob. */
static bool hero_crypto_wrap(hero_crypto_blob *out, unsigned char tag, const unsigned char *inner, size_t n) {
    unsigned char head[6];
    size_t head_count = hero_crypto_der_head(head, tag, n);
    const unsigned char *parts[] = { head, inner };
    const size_t counts[] = { head_count, n };

    return hero_crypto_join(out, parts, counts, 2);
}

/* The object identifiers a key names, as their whole DER elements. */
static const unsigned char hero_crypto_oid_rsa[] = { 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01 };
static const unsigned char hero_crypto_oid_ec[] = { 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01 };
static const unsigned char hero_crypto_oid_p256[] = { 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07 };
static const unsigned char hero_crypto_oid_p384[] = { 0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x22 };
static const unsigned char hero_crypto_oid_x25519[] = { 0x06, 0x03, 0x2B, 0x65, 0x6E };
static const unsigned char hero_crypto_oid_ed25519[] = { 0x06, 0x03, 0x2B, 0x65, 0x70 };
static const unsigned char hero_crypto_der_null[] = { 0x05, 0x00 };

/* The bytes of one coordinate, or of a scalar, on each curve. */
#define HERO_CRYPTO_WIDTH_MOST 48

/* A key read, the same fields on every platform. */
typedef struct hero_crypto_material {
    int64_t kind;
    bool is_private;
    int64_t bits;
    /* EC: the field's bytes, 32 or 48; X25519 and Ed25519: 32. */
    size_t width;
    /* EC: 04 || X || Y; X25519 and Ed25519: the public key's 32 bytes;
     * `point_count` 0 where the key read holds none. */
    unsigned char point[1 + 2 * HERO_CRYPTO_WIDTH_MOST];
    size_t point_count;
    /* EC: the private scalar, `width` bytes big-endian; X25519 and Ed25519:
     * the private key's 32 bytes. */
    unsigned char scalar[HERO_CRYPTO_WIDTH_MOST];
    /* RSA: PKCS #1's RSAPrivateKey or RSAPublicKey. */
    hero_crypto_blob rsa;
} hero_crypto_material;

static void hero_crypto_material_free(hero_crypto_material *m) {
    hero_crypto_wipe(m->scalar, sizeof m->scalar);
    hero_crypto_blob_free(&m->rsa);
}

/* PKCS #1's RSAPrivateKey (nine INTEGERs, version 0) or RSAPublicKey (two),
 * each number into `fields` in the order written; false where it is not. */
static bool hero_crypto_rsa_fields(hero_crypto_span pkcs1, bool is_private, hero_crypto_span *fields) {
    hero_crypto_span seq;
    size_t n = is_private ? 9 : 2;

    if (!hero_crypto_der(&pkcs1, 0x30, &seq) || pkcs1.n != 0) {
        return false;
    }
    for (size_t i = 0; i < n; i += 1) {
        if (!hero_crypto_der_unsigned(&seq, &fields[i])) {
            return false;
        }
    }
    if (is_private && !(fields[0].n == 1 && fields[0].p[0] == 0)) {
        return false;
    }
    return seq.n == 0;
}

/* RSA's PKCS #1 key checked and kept in `m`; its modulus is the field at
 * `modulus`. */
static bool hero_crypto_rsa_take(hero_crypto_material *m, hero_crypto_span pkcs1, bool is_private) {
    hero_crypto_span fields[9];
    const unsigned char *parts[1];
    size_t counts[1];
    char words[160];

    if (!hero_crypto_rsa_fields(pkcs1, is_private, fields)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, is_private ? "the RSA key is not PKCS #1's RSAPrivateKey of two primes" : "the RSA key is not PKCS #1's RSAPublicKey");
        return false;
    }
    m->bits = hero_crypto_bit_length(fields[is_private ? 1 : 0]);
    if (m->bits < HERO_CRYPTO_RSA_BITS_LEAST || m->bits > HERO_CRYPTO_RSA_BITS_MOST) {
        (void)snprintf(words, sizeof words, "the RSA key has %lld bits, outside the %d to %d taken", (long long)m->bits, HERO_CRYPTO_RSA_BITS_LEAST, HERO_CRYPTO_RSA_BITS_MOST);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, words);
        return false;
    }
    parts[0] = pkcs1.p;
    counts[0] = pkcs1.n;
    return hero_crypto_join(&m->rsa, parts, counts, 1);
}

/* The curve an algorithm identifier's parameters name, `HERO_CRYPTO_P256`
 * or `HERO_CRYPTO_P384`, or 0. */
static int64_t hero_crypto_curve_of(hero_crypto_span parameters) {
    if (hero_crypto_span_is(parameters, hero_crypto_oid_p256, sizeof hero_crypto_oid_p256)) {
        return HERO_CRYPTO_P256;
    }
    if (hero_crypto_span_is(parameters, hero_crypto_oid_p384, sizeof hero_crypto_oid_p384)) {
        return HERO_CRYPTO_P384;
    }
    return 0;
}

/* An AlgorithmIdentifier: the kind it names, into `m->kind`, and `m->width`
 * and `m->bits` for a curve; false, said, for any other. */
static bool hero_crypto_algorithm(hero_crypto_span algorithm, hero_crypto_material *m) {
    hero_crypto_span oid;
    /* The identifier's whole element, its head with it, which the tables
     * above hold. */
    hero_crypto_span whole = algorithm;

    if (!hero_crypto_der(&algorithm, 0x06, &oid)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the key's algorithm identifier is not DER");
        return false;
    }
    {
        size_t oid_whole = (size_t)(oid.p + oid.n - whole.p);
        hero_crypto_span named = { whole.p, oid_whole };

        if (hero_crypto_span_is(named, hero_crypto_oid_rsa, sizeof hero_crypto_oid_rsa)) {
            /* The parameters are NULL, or absent as some write them. */
            if (algorithm.n != 0 && !hero_crypto_span_is(algorithm, hero_crypto_der_null, sizeof hero_crypto_der_null)) {
                hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the RSA key's parameters are not NULL");
                return false;
            }
            m->kind = HERO_CRYPTO_RSA;
            return true;
        }
        if (hero_crypto_span_is(named, hero_crypto_oid_ec, sizeof hero_crypto_oid_ec)) {
            m->kind = hero_crypto_curve_of(algorithm);
            if (m->kind == 0) {
                hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the EC key is on a curve other than P-256 and P-384, which no platform here takes");
                return false;
            }
            m->width = m->kind == HERO_CRYPTO_P256 ? 32 : 48;
            m->bits = m->kind == HERO_CRYPTO_P256 ? 256 : 384;
            return true;
        }
        if (hero_crypto_span_is(named, hero_crypto_oid_ed25519, sizeof hero_crypto_oid_ed25519) || hero_crypto_span_is(named, hero_crypto_oid_x25519, sizeof hero_crypto_oid_x25519)) {
            if (algorithm.n != 0) {
                hero_crypto_said(HERO_CRYPTO_INVALID, 0, "an Ed25519 or X25519 key's algorithm takes no parameters (RFC 8410)");
                return false;
            }
            m->kind = hero_crypto_span_is(named, hero_crypto_oid_ed25519, sizeof hero_crypto_oid_ed25519) ? HERO_CRYPTO_ED25519 : HERO_CRYPTO_X25519;
            m->width = 32;
            m->bits = m->kind == HERO_CRYPTO_ED25519 ? 256 : 255;
            return true;
        }
    }
    hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the key is none of Ed25519, X25519, P-256, P-384 and RSA");
    return false;
}

/* An uncompressed point of `m`'s curve, or a 25519 key's 32 bytes, kept. */
static bool hero_crypto_point_take(hero_crypto_material *m, hero_crypto_span point) {
    size_t wanted = m->kind == HERO_CRYPTO_P256 || m->kind == HERO_CRYPTO_P384 ? 1 + 2 * m->width : m->width;

    if (point.n != wanted || (wanted != m->width && point.p[0] != 0x04)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, wanted == m->width ? "the public key is not 32 bytes" : "the EC point is not uncompressed, or not of the key's curve");
        return false;
    }
    memcpy(m->point, point.p, point.n);
    m->point_count = point.n;
    return true;
}

/* PKCS #8 DER into `m`, strictly: version 0 or 1, the algorithm, the
 * private key, attributes passed over, a public key kept where it is the
 * key's own form. */
static bool hero_crypto_read_pkcs8(const unsigned char *der, size_t count, hero_crypto_material *m) {
    hero_crypto_span whole = { der, count };
    hero_crypto_span seq;
    hero_crypto_span version;
    hero_crypto_span algorithm;
    hero_crypto_span key;
    hero_crypto_span field;

    memset(m, 0, sizeof *m);
    m->is_private = true;
    if (!hero_crypto_der(&whole, 0x30, &seq) || whole.n != 0 || !hero_crypto_der(&seq, 0x02, &version) || version.n != 1 || version.p[0] > 1 || !hero_crypto_der(&seq, 0x30, &algorithm) || !hero_crypto_der(&seq, 0x04, &key)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the bytes are not PKCS #8's PrivateKeyInfo in DER (an encrypted key is not taken: decrypt it first)");
        return false;
    }
    if (hero_crypto_der(&seq, 0xA0, &field)) {
        /* attributes: passed over */
    }
    if (hero_crypto_der(&seq, 0x81, &field)) {
        /* RFC 5958's public key, which the private key's own form also
         * holds for EC and which a 25519 key does not need. */
    }
    if (seq.n != 0) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the PKCS #8 key has bytes after its last field");
        return false;
    }
    if (!hero_crypto_algorithm(algorithm, m)) {
        return false;
    }
    if (m->kind == HERO_CRYPTO_RSA) {
        return hero_crypto_rsa_take(m, key, true);
    }
    if (m->kind == HERO_CRYPTO_ED25519 || m->kind == HERO_CRYPTO_X25519) {
        hero_crypto_span seed;

        if (!hero_crypto_der(&key, 0x04, &seed) || key.n != 0 || seed.n != 32) {
            hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the 25519 key is not RFC 8410's CurvePrivateKey of 32 bytes");
            return false;
        }
        memcpy(m->scalar, seed.p, 32);
        return true;
    }
    {
        /* SEC 1's ECPrivateKey (RFC 5915). */
        hero_crypto_span ec;
        hero_crypto_span ec_version;
        hero_crypto_span scalar;

        if (!hero_crypto_der(&key, 0x30, &ec) || key.n != 0 || !hero_crypto_der(&ec, 0x02, &ec_version) || ec_version.n != 1 || ec_version.p[0] != 1 || !hero_crypto_der(&ec, 0x04, &scalar) || scalar.n == 0 || scalar.n > m->width) {
            hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the EC key is not SEC 1's ECPrivateKey of its curve");
            return false;
        }
        memset(m->scalar, 0, sizeof m->scalar);
        memcpy(m->scalar + (m->width - scalar.n), scalar.p, scalar.n);
        if (hero_crypto_der(&ec, 0xA0, &field)) {
            if (hero_crypto_curve_of(field) != m->kind) {
                hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the EC key names another curve inside than outside");
                return false;
            }
        }
        if (hero_crypto_der(&ec, 0xA1, &field)) {
            hero_crypto_span bits;

            if (!hero_crypto_der(&field, 0x03, &bits) || field.n != 0 || bits.n < 2 || bits.p[0] != 0) {
                hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the EC key's public point is not a BIT STRING");
                return false;
            }
            bits.p += 1;
            bits.n -= 1;
            if (!hero_crypto_point_take(m, bits)) {
                return false;
            }
        }
        if (ec.n != 0) {
            hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the EC key has bytes after its last field");
            return false;
        }
        return true;
    }
}

/* SubjectPublicKeyInfo DER into `m`, strictly. */
static bool hero_crypto_read_spki(const unsigned char *der, size_t count, hero_crypto_material *m) {
    hero_crypto_span whole = { der, count };
    hero_crypto_span seq;
    hero_crypto_span algorithm;
    hero_crypto_span bits;

    memset(m, 0, sizeof *m);
    m->is_private = false;
    if (!hero_crypto_der(&whole, 0x30, &seq) || whole.n != 0 || !hero_crypto_der(&seq, 0x30, &algorithm) || !hero_crypto_der(&seq, 0x03, &bits) || seq.n != 0 || bits.n < 2 || bits.p[0] != 0) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the bytes are not SubjectPublicKeyInfo in DER");
        return false;
    }
    if (!hero_crypto_algorithm(algorithm, m)) {
        return false;
    }
    bits.p += 1;
    bits.n -= 1;
    if (m->kind == HERO_CRYPTO_RSA) {
        return hero_crypto_rsa_take(m, bits, false);
    }
    return hero_crypto_point_take(m, bits);
}

/* The AlgorithmIdentifier of `m`'s kind, its contents written at `into`
 * (room for 32); answers their length. */
static size_t hero_crypto_algorithm_of(const hero_crypto_material *m, unsigned char *into) {
    size_t at = 0;

    if (m->kind == HERO_CRYPTO_RSA) {
        memcpy(into, hero_crypto_oid_rsa, sizeof hero_crypto_oid_rsa);
        at = sizeof hero_crypto_oid_rsa;
        memcpy(into + at, hero_crypto_der_null, sizeof hero_crypto_der_null);
        return at + sizeof hero_crypto_der_null;
    }
    if (m->kind == HERO_CRYPTO_P256 || m->kind == HERO_CRYPTO_P384) {
        const unsigned char *curve = m->kind == HERO_CRYPTO_P256 ? hero_crypto_oid_p256 : hero_crypto_oid_p384;
        size_t curve_count = m->kind == HERO_CRYPTO_P256 ? sizeof hero_crypto_oid_p256 : sizeof hero_crypto_oid_p384;

        memcpy(into, hero_crypto_oid_ec, sizeof hero_crypto_oid_ec);
        at = sizeof hero_crypto_oid_ec;
        memcpy(into + at, curve, curve_count);
        return at + curve_count;
    }
    if (m->kind == HERO_CRYPTO_ED25519) {
        memcpy(into, hero_crypto_oid_ed25519, sizeof hero_crypto_oid_ed25519);
        return sizeof hero_crypto_oid_ed25519;
    }
    memcpy(into, hero_crypto_oid_x25519, sizeof hero_crypto_oid_x25519);
    return sizeof hero_crypto_oid_x25519;
}

/* `m`, a private key, written as PKCS #8 DER into a new blob. */
static bool hero_crypto_write_pkcs8(const hero_crypto_material *m, hero_crypto_blob *out) {
    static const unsigned char version0[] = { 0x02, 0x01, 0x00 };
    static const unsigned char version1[] = { 0x02, 0x01, 0x01 };
    unsigned char algorithm[32];
    size_t algorithm_count = hero_crypto_algorithm_of(m, algorithm);
    hero_crypto_blob algorithm_seq = { NULL, 0 };
    hero_crypto_blob inner = { NULL, 0 };
    hero_crypto_blob octets = { NULL, 0 };
    hero_crypto_blob body = { NULL, 0 };
    bool good;

    if (m->kind == HERO_CRYPTO_RSA) {
        good = hero_crypto_wrap(&octets, 0x04, m->rsa.bytes, m->rsa.count);
    } else if (m->kind == HERO_CRYPTO_ED25519 || m->kind == HERO_CRYPTO_X25519) {
        good = hero_crypto_wrap(&inner, 0x04, m->scalar, 32) && hero_crypto_wrap(&octets, 0x04, inner.bytes, inner.count);
    } else {
        /* ECPrivateKey: version 1, the scalar, the public point as [1]. */
        unsigned char scalar_head[6];
        size_t scalar_head_count = hero_crypto_der_head(scalar_head, 0x04, m->width);
        unsigned char bits_head[6];
        size_t bits_head_count = hero_crypto_der_head(bits_head, 0x03, m->point_count + 1);
        unsigned char tagged_head[6];
        size_t tagged_head_count = hero_crypto_der_head(tagged_head, 0xA1, bits_head_count + m->point_count + 1);
        static const unsigned char unused_bits[] = { 0x00 };
        const unsigned char *parts[] = { version1, scalar_head, m->scalar, tagged_head, bits_head, unused_bits, m->point };
        const size_t counts[] = { sizeof version1, scalar_head_count, m->width, tagged_head_count, bits_head_count, 1, m->point_count };
        hero_crypto_blob ec_body = { NULL, 0 };

        good = hero_crypto_join(&ec_body, parts, counts, 7) && hero_crypto_wrap(&inner, 0x30, ec_body.bytes, ec_body.count) && hero_crypto_wrap(&octets, 0x04, inner.bytes, inner.count);
        hero_crypto_blob_free(&ec_body);
    }
    if (good) {
        good = hero_crypto_wrap(&algorithm_seq, 0x30, algorithm, algorithm_count);
    }
    if (good) {
        const unsigned char *parts[] = { version0, algorithm_seq.bytes, octets.bytes };
        const size_t counts[] = { sizeof version0, algorithm_seq.count, octets.count };

        good = hero_crypto_join(&body, parts, counts, 3) && hero_crypto_wrap(out, 0x30, body.bytes, body.count);
    }
    hero_crypto_blob_free(&algorithm_seq);
    hero_crypto_blob_free(&inner);
    hero_crypto_blob_free(&octets);
    hero_crypto_blob_free(&body);
    return good;
}

/* `m`'s public half written as SubjectPublicKeyInfo DER into a new blob;
 * an RSA key's is its RSAPublicKey, which `m->rsa` holds for a public key. */
static bool hero_crypto_write_spki(const hero_crypto_material *m, const unsigned char *key, size_t key_count, hero_crypto_blob *out) {
    unsigned char algorithm[32];
    size_t algorithm_count = hero_crypto_algorithm_of(m, algorithm);
    hero_crypto_blob algorithm_seq = { NULL, 0 };
    hero_crypto_blob bits = { NULL, 0 };
    hero_crypto_blob body = { NULL, 0 };
    static const unsigned char unused_bits[] = { 0x00 };
    bool good;

    {
        const unsigned char *parts[] = { unused_bits, key };
        const size_t counts[] = { 1, key_count };
        hero_crypto_blob content = { NULL, 0 };

        good = hero_crypto_join(&content, parts, counts, 2) && hero_crypto_wrap(&bits, 0x03, content.bytes, content.count);
        hero_crypto_blob_free(&content);
    }
    if (good) {
        good = hero_crypto_wrap(&algorithm_seq, 0x30, algorithm, algorithm_count);
    }
    if (good) {
        const unsigned char *parts[] = { algorithm_seq.bytes, bits.bytes };
        const size_t counts[] = { algorithm_seq.count, bits.count };

        good = hero_crypto_join(&body, parts, counts, 2) && hero_crypto_wrap(out, 0x30, body.bytes, body.count);
    }
    hero_crypto_blob_free(&algorithm_seq);
    hero_crypto_blob_free(&bits);
    hero_crypto_blob_free(&body);
    return good;
}

/* An unsigned number as a DER INTEGER appended to `parts`: its head into
 * `head`, a zero byte where its high bit is set. Numbers given without
 * leading zero bytes, at least one byte. */
static size_t hero_crypto_integer_head(unsigned char *head, hero_crypto_span number) {
    bool pad = number.n == 0 || (number.p[0] & 0x80u) != 0;
    size_t count = hero_crypto_der_head(head, 0x02, number.n + (pad ? 1 : 0));

    if (pad) {
        head[count] = 0x00;
        count += 1;
    }
    return count;
}

/* `number` with its leading zero bytes left out, one byte kept at least. */
static hero_crypto_span hero_crypto_trimmed(const unsigned char *bytes, size_t count) {
    hero_crypto_span s = { bytes, count };

    while (s.n > 1 && s.p[0] == 0) {
        s.p += 1;
        s.n -= 1;
    }
    return s;
}

/* PKCS #1's RSAPublicKey of `modulus` and `exponent`, or of the private
 * key's fields `n` numbers long, written into a new blob. */
static bool hero_crypto_rsa_write(const hero_crypto_span *numbers, size_t n, bool is_private, hero_crypto_blob *out) {
    unsigned char heads[9][7];
    const unsigned char *parts[19];
    size_t counts[19];
    size_t used = 0;
    static const unsigned char version0[] = { 0x02, 0x01, 0x00 };
    hero_crypto_blob body = { NULL, 0 };
    bool good;

    if (is_private) {
        parts[used] = version0;
        counts[used] = sizeof version0;
        used += 1;
    }
    for (size_t i = 0; i < n; i += 1) {
        hero_crypto_span number = hero_crypto_trimmed(numbers[i].p, numbers[i].n);

        counts[used] = hero_crypto_integer_head(heads[i], number);
        parts[used] = heads[i];
        used += 1;
        parts[used] = number.p;
        counts[used] = number.n;
        used += 1;
    }
    good = hero_crypto_join(&body, parts, counts, used) && hero_crypto_wrap(out, 0x30, body.bytes, body.count);
    hero_crypto_blob_free(&body);
    return good;
}

/* An RSA private key's RSAPublicKey, written into a new blob. */
static bool hero_crypto_rsa_public_of(const hero_crypto_blob *private_key, hero_crypto_blob *out) {
    hero_crypto_span fields[9];
    hero_crypto_span whole = { private_key->bytes, private_key->count };

    if (!hero_crypto_rsa_fields(whole, true, fields)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the RSA key is not PKCS #1's RSAPrivateKey");
        return false;
    }
    return hero_crypto_rsa_write(fields + 1, 2, false, out);
}

/* X9.62's DER of an ECDSA signature read into two numbers of `width` bytes
 * side by side; false for bytes of no such form, BER's longer lengths and
 * padded integers among them, or a number wider than the curve. Read in the
 * common part because the Security framework holds such bytes as a
 * signature (Wycheproof's `BerEncodedSignature` and `InvalidEncoding`
 * cases, measured 2026-11-17), and a signature has one form. */
static bool hero_crypto_der_to_rs(const unsigned char *der, size_t count, size_t width, unsigned char *rs) {
    hero_crypto_span whole = { der, count };
    hero_crypto_span seq;

    if (!hero_crypto_der(&whole, 0x30, &seq) || whole.n != 0) {
        return false;
    }
    memset(rs, 0, 2 * width);
    for (size_t i = 0; i < 2; i += 1) {
        hero_crypto_span number;

        if (!hero_crypto_der_unsigned(&seq, &number) || number.n > width) {
            return false;
        }
        memcpy(rs + i * width + (width - number.n), number.p, number.n);
    }
    return seq.n == 0;
}

/* -- THE TABLE OF KEYS ------------------------------------------------------
 *
 * A key is a handle the layer issues and checks, as `hero_net.c`'s socket
 * is: the handle carries this table's tag, a slot and the life the slot held
 * when it was issued, `HERO_HANDLE_TAG(HERO_HANDLE_TAG_CRYPTO) | (life <<
 * HERO_CRYPTO_INDEX_BITS) | (index + 1)`, never an address. A call on a key
 * holds its slot busy; a close marks the slot closing, so no later call
 * reaches it, and the last call to leave gives the key back. */

/* What a slot holds: the key, its kind and size, and the arm's object. */
typedef struct hero_crypto_held {
    int64_t kind;
    int64_t bits;
    bool is_public;
    /* How many texts the key has sealed, under the table's lock. */
    uint64_t sealed;
    /* A secret key's bytes; on macOS also an Ed25519 or X25519 private
     * key's 32, which Monocypher computes with. */
    hero_crypto_blob secret;
    /* An RSA key's modulus, as many bytes as a signature has, which every
     * check of a signature holds the signature below. */
    hero_crypto_blob modulus;
    /* An Ed25519 public key read whose 32 bytes RFC 8032 § 5.1.3 does not
     * decode (`hero_crypto_ed25519_canonical`): no signature holds under
     * it, on every platform. */
    bool undecodable;
#if defined(__APPLE__)
    /* The pair's half `SecKeyCreate*` made: RSA, P-256 and P-384. */
    SecKeyRef key;
    /* An Ed25519 or X25519 key's public 32 bytes, Monocypher's answer for
     * a private key, private or public alike. */
    unsigned char curve_public[32];
#elif defined(_WIN32)
    /* The pair's fields, from which each call makes CNG's blob for the
     * algorithm it runs: an EC key is one blob for ECDSA and another for
     * ECDH. */
    hero_crypto_material material;
#else
    /* OpenSSL's `EVP_PKEY *`. */
    void *pkey;
#endif
} hero_crypto_held;

#define HERO_CRYPTO_INDEX_BITS 17
#define HERO_CRYPTO_SLOTS 65536u
/* The last life a slot is given, so the life stays below the tag; a slot
 * at it is never issued again. */
#define HERO_CRYPTO_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_CRYPTO_INDEX_BITS)

typedef struct hero_crypto_slot {
    hero_crypto_held *held;
    uint64_t life;
    uint32_t busy;
    /* The next slot given back, its index plus one, 0 for none. */
    uint32_t next;
    bool taken;
    bool closing;
} hero_crypto_slot;

static hero_crypto_slot hero_crypto_slots[HERO_CRYPTO_SLOTS];
/* How many slots have ever held a key, under `hero_crypto_lock`. */
static uint32_t hero_crypto_fresh;
/* The first slot given back, its index plus one, under `hero_crypto_lock`. */
static uint32_t hero_crypto_given_back;

#if defined(_WIN32)
static SRWLOCK hero_crypto_lock = SRWLOCK_INIT;
#else
static pthread_mutex_t hero_crypto_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void hero_crypto_table_lock(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_crypto_lock);
#else
    (void)pthread_mutex_lock(&hero_crypto_lock);
#endif
}

static void hero_crypto_table_unlock(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_crypto_lock);
#else
    (void)pthread_mutex_unlock(&hero_crypto_lock);
#endif
}

/* The arm's own: gives back what `held` holds, its secret overwritten, and
 * `held` itself. Called without the table's lock. */
static void hero_crypto_arm_free(hero_crypto_held *held);

static void hero_crypto_held_free(hero_crypto_held *held) {
    if (held == NULL) {
        return;
    }
    hero_crypto_arm_free(held);
    hero_crypto_blob_free(&held->secret);
    hero_crypto_blob_free(&held->modulus);
    hero_crypto_wipe(held, sizeof *held);
    hero_layer_free(held);
}

/* A new, empty `held`, or NULL with `HERO_CRYPTO_LIMIT`. */
static hero_crypto_held *hero_crypto_held_new(int64_t kind) {
    hero_crypto_held *held = hero_layer_alloc_zero(1, sizeof *held);

    if (held == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for a key");
        return NULL;
    }
    held->kind = kind;
    return held;
}

/* A handle for `held`, or NULL with `HERO_CRYPTO_LIMIT`, `held` then given
 * back. */
static hero_crypto_key *hero_crypto_issue(hero_crypto_held *held) {
    hero_crypto_slot *slot = NULL;
    uint32_t index;

    hero_crypto_table_lock();
    if (hero_crypto_given_back != 0) {
        index = hero_crypto_given_back - 1;
        slot = &hero_crypto_slots[index];
        hero_crypto_given_back = slot->next;
    } else if (hero_crypto_fresh < HERO_CRYPTO_SLOTS) {
        index = hero_crypto_fresh;
        hero_crypto_fresh += 1;
        slot = &hero_crypto_slots[index];
    } else {
        hero_crypto_table_unlock();
        hero_crypto_held_free(held);
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "the layer holds as many keys as it can, 65536");
        return NULL;
    }
    slot->life += 1;
    slot->held = held;
    slot->busy = 0;
    slot->next = 0;
    slot->taken = true;
    slot->closing = false;
    hero_crypto_table_unlock();
    hero_crypto_fine();
    return (hero_crypto_key *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_CRYPTO) | (slot->life << HERO_CRYPTO_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot `k` names while it holds the life `k` was issued for, or NULL.
 * Under the lock. */
static hero_crypto_slot *hero_crypto_slot_of(hero_crypto_key *k) {
    uint64_t value = (uint64_t)(uintptr_t)k;
    uint64_t index = value & ((UINT64_C(1) << HERO_CRYPTO_INDEX_BITS) - 1);
    uint64_t life = (value & ~HERO_HANDLE_TAG_MASK) >> HERO_CRYPTO_INDEX_BITS;
    hero_crypto_slot *slot;

    /* Another table's handle is no key at any slot and any life. */
    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_CRYPTO) || index == 0 || index > HERO_CRYPTO_SLOTS || life > HERO_CRYPTO_LIFE_MOST) {
        return NULL;
    }
    slot = &hero_crypto_slots[index - 1];
    if (!slot->taken || slot->closing || slot->life != life) {
        return NULL;
    }
    return slot;
}

/* The slot given back to the free list. Under the lock. */
static void hero_crypto_give_back(hero_crypto_slot *slot) {
    slot->taken = false;
    slot->closing = false;
    slot->held = NULL;
    if (slot->life < HERO_CRYPTO_LIFE_MOST) {
        slot->next = hero_crypto_given_back;
        hero_crypto_given_back = (uint32_t)(slot - hero_crypto_slots) + 1;
    }
}

/* The key `k` names, held busy until `hero_crypto_leave`; NULL with
 * `HERO_CRYPTO_NOT_OPEN` where it names none open. */
static hero_crypto_held *hero_crypto_enter(hero_crypto_key *k, hero_crypto_slot **slot_out) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;

    hero_crypto_table_lock();
    slot = hero_crypto_slot_of(k);
    if (slot == NULL) {
        hero_crypto_table_unlock();
        hero_crypto_said(HERO_CRYPTO_NOT_OPEN, 0, "the key is closed, or was never one");
        return NULL;
    }
    slot->busy += 1;
    held = slot->held;
    *slot_out = slot;
    hero_crypto_table_unlock();
    return held;
}

static void hero_crypto_leave(hero_crypto_slot *slot) {
    hero_crypto_held *freed = NULL;

    hero_crypto_table_lock();
    slot->busy -= 1;
    if (slot->closing && slot->busy == 0) {
        freed = slot->held;
        hero_crypto_give_back(slot);
    }
    hero_crypto_table_unlock();
    hero_crypto_held_free(freed);
}

/* One more seal counted on `held`, the key `slot` holds; false with
 * `HERO_CRYPTO_LIMIT` past the bound. */
static bool hero_crypto_count_seal(hero_crypto_held *held) {
    bool fits;

    hero_crypto_table_lock();
    fits = held->sealed < HERO_CRYPTO_SEALS_MOST;
    if (fits) {
        held->sealed += 1;
    }
    hero_crypto_table_unlock();
    if (!fits) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "the key has sealed 2^32 texts with random nonces, as many as NIST SP 800-38D allows: make another key");
    }
    return fits;
}

int64_t hero_crypto_close(hero_crypto_key *k) {
    hero_crypto_slot *slot;
    hero_crypto_held *held = NULL;

    hero_crypto_table_lock();
    slot = hero_crypto_slot_of(k);
    if (slot == NULL) {
        hero_crypto_table_unlock();
        hero_crypto_said(HERO_CRYPTO_NOT_OPEN, 0, "the key is closed, or was never one");
        return -1;
    }
    slot->closing = true;
    if (slot->busy == 0) {
        held = slot->held;
        hero_crypto_give_back(slot);
    }
    hero_crypto_table_unlock();
    hero_crypto_held_free(held);
    hero_crypto_fine();
    return 0;
}

/* -- THE ARMS' CONTRACT -----------------------------------------------------
 *
 * Each arm defines these, and nothing else is called across the line. A
 * function answering `bool` says its failure itself; `open` and `verify`
 * answer 1 for a text or a signature that holds, 0 for one that does not,
 * -1 for a failure said. Every buffer an arm writes is the caller's, at
 * least as long as the common part checked. */

/* Whether the platform's library is there (Linux: `libcrypto.so.3`). */
static bool hero_crypto_arm_ready(void);
/* Whether the arm computes `what`, a kind or a scheme. */
static bool hero_crypto_arm_offers(int64_t what);
/* `count` bytes from the system's generator. */
static bool hero_crypto_arm_random(unsigned char *into, size_t count);
/* A new key pair of `held->kind` (and `bits`) into `held`. */
static bool hero_crypto_arm_generate(hero_crypto_held *held, int64_t bits);
/* `held` made from `m`, private or public as `m` is; `der` is the form `m`
 * was read from. */
static bool hero_crypto_arm_load(hero_crypto_held *held, const hero_crypto_material *m, const unsigned char *der, size_t der_count);
/* `held`'s key into `m`, its private half where `held` is private. */
static bool hero_crypto_arm_material(const hero_crypto_held *held, hero_crypto_material *m);
/* `to`, a new public key, made from `from`'s public half. */
static bool hero_crypto_arm_public(const hero_crypto_held *from, hero_crypto_held *to);
/* AEAD: `text` sealed into `out` (the ciphertext) and `tag`; and opened. */
static bool hero_crypto_arm_seal(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *text, size_t text_count, const unsigned char *associated, size_t associated_count, unsigned char *out, unsigned char *tag);
static int hero_crypto_arm_open(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *data, size_t data_count, const unsigned char *associated, size_t associated_count, const unsigned char *tag, unsigned char *out);
/* A signature into `out`, at most `most` bytes; answers its length or -1. */
static int64_t hero_crypto_arm_sign(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, unsigned char *out, size_t most);
static int hero_crypto_arm_verify(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, const unsigned char *signature, size_t signature_count);
/* The shared secret into `out`, the curve's width; answers it or -1. */
static int64_t hero_crypto_arm_agree(const hero_crypto_held *mine, const hero_crypto_held *theirs, unsigned char *out, size_t width);
/* RSA-OAEP: answers the bytes written or -1; decrypt answers -2 for bytes
 * that do not decrypt. */
static int64_t hero_crypto_arm_encrypt(const hero_crypto_held *held, const unsigned char *text, size_t count, unsigned char *out, size_t most);
static int64_t hero_crypto_arm_decrypt(const hero_crypto_held *held, const unsigned char *data, size_t count, unsigned char *out, size_t most);

/* -- THE COMMON CALLS ------------------------------------------------------- */

/* An RSA key's modulus kept in `held`, from `m` where given, else from the
 * arm's own export once; padded to its bytes. A key of another kind keeps
 * none. */
static bool hero_crypto_keep_modulus(hero_crypto_held *held, const hero_crypto_material *given) {
    hero_crypto_material exported;
    const hero_crypto_material *m = given;
    hero_crypto_span fields[9];
    size_t bytes = (size_t)((held->bits + 7) / 8);
    bool good;

    if (held->kind != HERO_CRYPTO_RSA) {
        return true;
    }
    memset(&exported, 0, sizeof exported);
    if (m == NULL) {
        if (!hero_crypto_arm_material(held, &exported)) {
            hero_crypto_material_free(&exported);
            return false;
        }
        m = &exported;
    }
    {
        hero_crypto_span whole = { m->rsa.bytes, m->rsa.count };

        good = hero_crypto_rsa_fields(whole, m->is_private, fields);
    }
    if (good) {
        hero_crypto_span n = m->is_private ? fields[1] : fields[0];

        good = n.n == bytes;
        if (good) {
            const unsigned char *parts[] = { n.p };
            const size_t counts[] = { n.n };

            good = hero_crypto_join(&held->modulus, parts, counts, 1);
        } else {
            hero_crypto_said(HERO_CRYPTO_OTHER, 0, "the RSA key's modulus is not as long as its bits say");
        }
    } else {
        hero_crypto_said(HERO_CRYPTO_OTHER, 0, "the RSA key is not PKCS #1");
    }
    hero_crypto_material_free(&exported);
    return good;
}

/* Whether `e`, 32 bytes, is an encoding of an Ed25519 point that RFC 8032
 * § 5.1.3 decodes: y, the bytes with bit 255 cleared read little-endian,
 * below p = 2^255 - 19, and bit 255, the sign of x, clear where x is 0,
 * which is where y is 1 or p - 1. Whether the point is on the curve is
 * the library's to answer, and both refuse one that is not. Neither
 * refuses the rest by itself: Monocypher 4.0.3 reduces y modulo p and
 * takes either sign of a zero x, for A and for R
 * (`crypto_eddsa_check_equation`), and OpenSSL 3.5 does the same for A,
 * so OpenSSL 3.5.7 and 3.5.9 held signatures under four such keys of five
 * built (measured 2026-11-21); only its byte comparison of R refuses such
 * an R. So the common part refuses both encodings before any arm is asked
 * (defect 2641). */
static bool hero_crypto_ed25519_canonical(const unsigned char e[32]) {
    bool y_is_one = e[0] == 0x01 && (e[31] & 0x7f) == 0x00;
    bool y_is_p_less_one = e[0] == 0xec && (e[31] & 0x7f) == 0x7f;
    bool middle_ones = true;
    size_t i;

    for (i = 1; i <= 30; i++) {
        y_is_one = y_is_one && e[i] == 0x00;
        middle_ones = middle_ones && e[i] == 0xff;
    }
    y_is_p_less_one = y_is_p_less_one && middle_ones;
    /* p is ed ff .. ff 7f little-endian, so y is at least p exactly where
     * bytes 1 to 30 are ff, byte 31 holds 7f below its sign bit, and byte
     * 0 is ed or above. */
    if (middle_ones && (e[31] & 0x7f) == 0x7f && e[0] >= 0xed) {
        return false;
    }
    return (e[31] & 0x80) == 0 || !(y_is_one || y_is_p_less_one);
}

/* The order of Ed25519's group, L = 2^252 +
 * 27742317777372353535851937790883648493 (RFC 8032 § 5.1), little-endian. */
static const unsigned char hero_crypto_ed25519_order[32] = {
    0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58, 0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
};

/* Whether `s`, 32 bytes little-endian, is below L: RFC 8032 § 5.1.7's
 * first check. Monocypher and OpenSSL make it too; made here, the form a
 * signature holds in is the common part's on every platform, as ECDSA's
 * and RSA's are. */
static bool hero_crypto_ed25519_below_order(const unsigned char s[32]) {
    size_t i = 32;

    while (i > 0) {
        i--;
        if (s[i] != hero_crypto_ed25519_order[i]) {
            return s[i] < hero_crypto_ed25519_order[i];
        }
    }
    return false;
}

/* Whether a signature's bytes are of the one form `held` checks: X9.62's
 * DER for ECDSA, exactly the modulus's bytes and below it for RSA (the
 * Security framework reduces one past it, Wycheproof's
 * `SignatureMalleability` case, measured 2026-11-17); for Ed25519 64 bytes
 * whose R is an encoding RFC 8032 § 5.1.3 decodes and whose S is below the
 * group's order, under a key whose own 32 bytes it decodes (Monocypher held
 * an R that is not, Wycheproof's `InvalidEncoding` tcId 151, defect 2641). */
static bool hero_crypto_signature_formed(const hero_crypto_held *held, const unsigned char *signature, size_t count) {
    if (held->kind == HERO_CRYPTO_P256 || held->kind == HERO_CRYPTO_P384) {
        unsigned char rs[2 * HERO_CRYPTO_WIDTH_MOST];

        return hero_crypto_der_to_rs(signature, count, held->kind == HERO_CRYPTO_P256 ? 32 : 48, rs);
    }
    if (held->kind == HERO_CRYPTO_RSA) {
        return count == held->modulus.count && memcmp(signature, held->modulus.bytes, count) < 0;
    }
    return count == 64 && !held->undecodable && hero_crypto_ed25519_canonical(signature) && hero_crypto_ed25519_below_order(signature + 32);
}

/* The bytes of an RSA key's modulus. */
static size_t hero_crypto_modulus_bytes(const hero_crypto_held *held) {
    return (size_t)((held->bits + 7) / 8);
}

/* The most bytes a signature of `held` by `scheme` takes: Ed25519's 64,
 * ECDSA's DER of two numbers of the curve's width, RSA's modulus. */
static size_t hero_crypto_signature_most(const hero_crypto_held *held) {
    if (held->kind == HERO_CRYPTO_ED25519) {
        return 64;
    }
    if (held->kind == HERO_CRYPTO_P256) {
        return 72;
    }
    if (held->kind == HERO_CRYPTO_P384) {
        return 104;
    }
    return hero_crypto_modulus_bytes(held);
}

/* Refused where the library is not there: `HERO_CRYPTO_UNSUPPORTED`. */
static bool hero_crypto_ready(void) {
    if (!hero_crypto_arm_ready()) {
        hero_crypto_said(HERO_CRYPTO_UNSUPPORTED, 0, "this machine has no libcrypto.so.3, the system's cryptography");
        return false;
    }
    return true;
}

/* Refused where the arm does not compute `what` on this platform. */
static bool hero_crypto_offered(int64_t what) {
    char words[160];

    if (!hero_crypto_arm_offers(what)) {
        (void)snprintf(words, sizeof words, "%s is not offered by this platform's own cryptography", hero_crypto_is_scheme(what) ? "this scheme" : hero_crypto_kind_name(what));
        hero_crypto_said(HERO_CRYPTO_UNSUPPORTED, 0, words);
        return false;
    }
    return true;
}

int64_t hero_crypto_offers(int64_t what) {
    if (!hero_crypto_is_aead(what) && !hero_crypto_is_pair(what) && !hero_crypto_is_scheme(what)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the number names no kind and no scheme");
        return -1;
    }
    hero_crypto_fine();
    return hero_crypto_arm_ready() && hero_crypto_arm_offers(what) ? 1 : 0;
}

hero_crypto_key *hero_crypto_generate(int64_t kind, int64_t bits) {
    hero_crypto_held *held;

    if (!hero_crypto_is_aead(kind) && !hero_crypto_is_pair(kind)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the number names no kind of key");
        return NULL;
    }
    if (kind == HERO_CRYPTO_RSA ? bits != 2048 && bits != 3072 && bits != 4096 : bits != 0) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, kind == HERO_CRYPTO_RSA ? "an RSA key is made of 2048, 3072 or 4096 bits" : "only an RSA key takes a size: give 0");
        return NULL;
    }
    if (!hero_crypto_ready() || !hero_crypto_offered(kind)) {
        return NULL;
    }
    held = hero_crypto_held_new(kind);
    if (held == NULL) {
        return NULL;
    }
    if (hero_crypto_is_aead(kind)) {
        size_t count = kind == HERO_CRYPTO_AES_128_GCM ? 16 : 32;

        held->bits = (int64_t)count * 8;
        held->secret.bytes = hero_layer_alloc(count);
        held->secret.count = count;
        if (held->secret.bytes == NULL) {
            held->secret.count = 0;
            hero_crypto_held_free(held);
            hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for a key");
            return NULL;
        }
        if (!hero_crypto_arm_random(held->secret.bytes, count)) {
            hero_crypto_held_free(held);
            return NULL;
        }
        return hero_crypto_issue(held);
    }
    if (!hero_crypto_arm_generate(held, bits) || !hero_crypto_keep_modulus(held, NULL)) {
        hero_crypto_held_free(held);
        return NULL;
    }
    return hero_crypto_issue(held);
}

hero_crypto_key *hero_crypto_from_secret(int64_t kind, const void *bytes, uint64_t count) {
    hero_crypto_held *held;
    uint64_t wanted = kind == HERO_CRYPTO_AES_128_GCM ? 16 : 32;
    const unsigned char *parts[1];
    size_t counts[1];
    char words[160];

    if (!hero_crypto_is_aead(kind)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "a secret key is AES-128-GCM, AES-256-GCM or ChaCha20-Poly1305");
        return NULL;
    }
    if (!hero_crypto_bytes_given(bytes, count, 32, "the key")) {
        return NULL;
    }
    if (count != wanted) {
        (void)snprintf(words, sizeof words, "an %s key is %llu bytes, not %llu", hero_crypto_kind_name(kind), (unsigned long long)wanted, (unsigned long long)count);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, words);
        return NULL;
    }
    if (!hero_crypto_ready() || !hero_crypto_offered(kind)) {
        return NULL;
    }
    held = hero_crypto_held_new(kind);
    if (held == NULL) {
        return NULL;
    }
    held->bits = (int64_t)wanted * 8;
    parts[0] = (const unsigned char *)bytes;
    counts[0] = (size_t)count;
    if (!hero_crypto_join(&held->secret, parts, counts, 1)) {
        hero_crypto_held_free(held);
        return NULL;
    }
    return hero_crypto_issue(held);
}

/* A key read from `der`: PKCS #8 where `is_private`, SPKI where not. */
static hero_crypto_key *hero_crypto_read(const void *der, uint64_t count, bool is_private) {
    hero_crypto_material m;
    hero_crypto_held *held;
    bool read;

    if (!hero_crypto_bytes_given(der, count, 1u << 20, "the key")) {
        return NULL;
    }
    if (!hero_crypto_ready()) {
        return NULL;
    }
    read = is_private ? hero_crypto_read_pkcs8((const unsigned char *)der, (size_t)count, &m) : hero_crypto_read_spki((const unsigned char *)der, (size_t)count, &m);
    if (!read) {
        hero_crypto_material_free(&m);
        return NULL;
    }
    if (!hero_crypto_offered(m.kind)) {
        hero_crypto_material_free(&m);
        return NULL;
    }
    held = hero_crypto_held_new(m.kind);
    if (held == NULL) {
        hero_crypto_material_free(&m);
        return NULL;
    }
    held->bits = m.bits;
    held->is_public = !is_private;
    /* Read, not refused, as a key whose bytes are no point on the curve
     * is: no signature holds under it. A private key's public half is
     * computed from its seed, and is canonical. */
    held->undecodable = m.kind == HERO_CRYPTO_ED25519 && !is_private && !hero_crypto_ed25519_canonical(m.point);
    if (!hero_crypto_arm_load(held, &m, (const unsigned char *)der, (size_t)count) || !hero_crypto_keep_modulus(held, &m)) {
        hero_crypto_material_free(&m);
        hero_crypto_held_free(held);
        return NULL;
    }
    hero_crypto_material_free(&m);
    return hero_crypto_issue(held);
}

hero_crypto_key *hero_crypto_read_private(const void *der, uint64_t count) {
    return hero_crypto_read(der, count, true);
}

hero_crypto_key *hero_crypto_read_public(const void *der, uint64_t count) {
    return hero_crypto_read(der, count, false);
}

hero_crypto_key *hero_crypto_public_of(hero_crypto_key *k) {
    hero_crypto_slot *slot;
    hero_crypto_held *held = hero_crypto_enter(k, &slot);
    hero_crypto_held *made;
    bool good;

    if (held == NULL) {
        return NULL;
    }
    if (hero_crypto_is_aead(held->kind)) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "a secret key has no public half");
        return NULL;
    }
    made = hero_crypto_held_new(held->kind);
    if (made == NULL) {
        hero_crypto_leave(slot);
        return NULL;
    }
    made->bits = held->bits;
    made->is_public = true;
    good = hero_crypto_arm_public(held, made);
    if (good && held->modulus.count > 0) {
        const unsigned char *parts[] = { held->modulus.bytes };
        const size_t counts[] = { held->modulus.count };

        good = hero_crypto_join(&made->modulus, parts, counts, 1);
    }
    hero_crypto_leave(slot);
    if (!good) {
        hero_crypto_held_free(made);
        return NULL;
    }
    return hero_crypto_issue(made);
}

/* One fact of `k`: its kind, its bits, or whether it is public. */
static int64_t hero_crypto_fact(hero_crypto_key *k, int which) {
    hero_crypto_slot *slot;
    hero_crypto_held *held = hero_crypto_enter(k, &slot);
    int64_t answer;

    if (held == NULL) {
        return -1;
    }
    answer = which == 0 ? held->kind : which == 1 ? held->bits : held->is_public ? 1 : 0;
    hero_crypto_leave(slot);
    hero_crypto_fine();
    return answer;
}

int64_t hero_crypto_kind(hero_crypto_key *k) {
    return hero_crypto_fact(k, 0);
}

int64_t hero_crypto_bits(hero_crypto_key *k) {
    return hero_crypto_fact(k, 1);
}

int64_t hero_crypto_is_public(hero_crypto_key *k) {
    return hero_crypto_fact(k, 2);
}

/* `bytes` handed to the caller by the fill form. */
static int64_t hero_crypto_fill(const hero_crypto_blob *bytes, void *buffer, uint64_t count) {
    if (count >= bytes->count && bytes->count > 0) {
        memcpy(buffer, bytes->bytes, bytes->count);
    }
    hero_crypto_fine();
    return (int64_t)bytes->count;
}

/* `k` written as PKCS #8 (`is_private`) or SPKI, by the fill form. */
static int64_t hero_crypto_write(hero_crypto_key *k, void *buffer, uint64_t count, bool is_private) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    hero_crypto_material m;
    hero_crypto_blob out = { NULL, 0 };
    hero_crypto_blob rsa_public = { NULL, 0 };
    bool good;
    int64_t answer = -1;

    if (!hero_crypto_bytes_given(buffer, count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    held = hero_crypto_enter(k, &slot);
    if (held == NULL) {
        return -1;
    }
    if (hero_crypto_is_aead(held->kind)) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "a secret key is written by neither PKCS #8 nor SPKI");
        return -1;
    }
    if (is_private && held->is_public) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "a public key has no private half to write");
        return -1;
    }
    memset(&m, 0, sizeof m);
    good = hero_crypto_arm_material(held, &m);
    hero_crypto_leave(slot);
    if (good && (m.kind == HERO_CRYPTO_P256 || m.kind == HERO_CRYPTO_P384) && m.point_count == 0) {
        hero_crypto_said(HERO_CRYPTO_UNSUPPORTED, 0, "this platform did not give the EC key's public point");
        good = false;
    }
    if (good && is_private) {
        good = hero_crypto_write_pkcs8(&m, &out);
    } else if (good) {
        if (m.kind == HERO_CRYPTO_RSA && m.is_private) {
            good = hero_crypto_rsa_public_of(&m.rsa, &rsa_public) && hero_crypto_write_spki(&m, rsa_public.bytes, rsa_public.count, &out);
        } else if (m.kind == HERO_CRYPTO_RSA) {
            good = hero_crypto_write_spki(&m, m.rsa.bytes, m.rsa.count, &out);
        } else if (m.point_count == 0) {
            hero_crypto_said(HERO_CRYPTO_UNSUPPORTED, 0, "this platform did not give the key's public half");
            good = false;
        } else {
            good = hero_crypto_write_spki(&m, m.point, m.point_count, &out);
        }
    }
    if (good) {
        answer = hero_crypto_fill(&out, buffer, count);
    }
    hero_crypto_blob_free(&out);
    hero_crypto_blob_free(&rsa_public);
    hero_crypto_material_free(&m);
    return answer;
}

int64_t hero_crypto_write_private(hero_crypto_key *k, void *buffer, uint64_t count) {
    return hero_crypto_write(k, buffer, count, true);
}

int64_t hero_crypto_write_public(hero_crypto_key *k, void *buffer, uint64_t count) {
    return hero_crypto_write(k, buffer, count, false);
}

/* The seal of `text`, with `nonce` given or drawn where it is NULL. */
static int64_t hero_crypto_sealing(hero_crypto_key *k, const void *nonce, const void *text, uint64_t text_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    uint64_t whole;
    unsigned char *out = (unsigned char *)buffer;
    bool good;

    if (!hero_crypto_bytes_given(text, text_count, HERO_CRYPTO_TEXT_MOST, "the text") || !hero_crypto_bytes_given(associated, associated_count, HERO_CRYPTO_TEXT_MOST, "the associated data") || !hero_crypto_bytes_given(buffer, count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    held = hero_crypto_enter(k, &slot);
    if (held == NULL) {
        return -1;
    }
    if (!hero_crypto_is_aead(held->kind)) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "only a secret key seals: AES-128-GCM, AES-256-GCM or ChaCha20-Poly1305");
        return -1;
    }
    whole = text_count + HERO_CRYPTO_NONCE_BYTES + HERO_CRYPTO_TAG_BYTES;
    if (count < whole) {
        hero_crypto_leave(slot);
        hero_crypto_fine();
        return (int64_t)whole;
    }
    if (!hero_crypto_count_seal(held)) {
        hero_crypto_leave(slot);
        return -1;
    }
    if (nonce != NULL) {
        memcpy(out, nonce, HERO_CRYPTO_NONCE_BYTES);
        good = true;
    } else {
        good = hero_crypto_arm_random(out, HERO_CRYPTO_NONCE_BYTES);
    }
    good = good && hero_crypto_arm_seal(held, out, (const unsigned char *)text, (size_t)text_count, (const unsigned char *)associated, (size_t)associated_count, out + HERO_CRYPTO_NONCE_BYTES, out + HERO_CRYPTO_NONCE_BYTES + text_count);
    hero_crypto_leave(slot);
    if (!good) {
        hero_crypto_wipe(out, (size_t)whole);
        return -1;
    }
    hero_crypto_fine();
    return (int64_t)whole;
}

int64_t hero_crypto_seal(hero_crypto_key *k, const void *text, uint64_t text_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count) {
    return hero_crypto_sealing(k, NULL, text, text_count, associated, associated_count, buffer, count);
}

int64_t hero_crypto_seal_nonce_for_tests(hero_crypto_key *k, const void *nonce, uint64_t nonce_count, const void *text, uint64_t text_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count) {
    if (nonce == NULL || nonce_count != HERO_CRYPTO_NONCE_BYTES) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "a nonce is 12 bytes");
        return -1;
    }
    return hero_crypto_sealing(k, nonce, text, text_count, associated, associated_count, buffer, count);
}

int64_t hero_crypto_open(hero_crypto_key *k, const void *sealed, uint64_t sealed_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    uint64_t text_count;
    const unsigned char *in = (const unsigned char *)sealed;
    /* Where no buffer is given for an empty text, a byte of its own, so no
     * arm adds even 0 to a null pointer. */
    unsigned char spare[1];
    unsigned char *out = buffer != NULL ? (unsigned char *)buffer : spare;
    int opened;

    if (!hero_crypto_bytes_given(sealed, sealed_count, HERO_CRYPTO_TEXT_MOST + HERO_CRYPTO_NONCE_BYTES + HERO_CRYPTO_TAG_BYTES, "the sealed bytes") || !hero_crypto_bytes_given(associated, associated_count, HERO_CRYPTO_TEXT_MOST, "the associated data") || !hero_crypto_bytes_given(buffer, count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    held = hero_crypto_enter(k, &slot);
    if (held == NULL) {
        return -1;
    }
    if (!hero_crypto_is_aead(held->kind)) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "only a secret key opens: AES-128-GCM, AES-256-GCM or ChaCha20-Poly1305");
        return -1;
    }
    if (sealed_count < HERO_CRYPTO_NONCE_BYTES + HERO_CRYPTO_TAG_BYTES) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the sealed bytes are fewer than a nonce and a tag, 28");
        return -1;
    }
    text_count = sealed_count - HERO_CRYPTO_NONCE_BYTES - HERO_CRYPTO_TAG_BYTES;
    if (count < text_count) {
        hero_crypto_leave(slot);
        hero_crypto_fine();
        return (int64_t)text_count;
    }
    opened = hero_crypto_arm_open(held, in, in + HERO_CRYPTO_NONCE_BYTES, (size_t)text_count, (const unsigned char *)associated, (size_t)associated_count, in + HERO_CRYPTO_NONCE_BYTES + text_count, out);
    hero_crypto_leave(slot);
    if (opened != 1) {
        hero_crypto_wipe(out, (size_t)text_count);
        if (opened == 0) {
            hero_crypto_said(HERO_CRYPTO_FORGED, 0, "the sealed bytes do not open under this key with this associated data");
        }
        return -1;
    }
    hero_crypto_fine();
    return (int64_t)text_count;
}

/* The key `k` checked for `scheme`: a scheme it knows, of `k`'s kind, the
 * private half where `private_needed`. NULL, said, where not. */
static hero_crypto_held *hero_crypto_enter_for(hero_crypto_key *k, int64_t scheme, bool private_needed, hero_crypto_slot **slot) {
    hero_crypto_held *held;

    if (!hero_crypto_is_scheme(scheme)) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the number names no scheme");
        return NULL;
    }
    held = hero_crypto_enter(k, slot);
    if (held == NULL) {
        return NULL;
    }
    if (!hero_crypto_scheme_fits(scheme, held->kind)) {
        char words[160];

        hero_crypto_leave(*slot);
        (void)snprintf(words, sizeof words, "the scheme is not one of %s's", hero_crypto_kind_name(held->kind));
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, words);
        return NULL;
    }
    if (private_needed && held->is_public) {
        hero_crypto_leave(*slot);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "a public key neither signs nor decrypts: the private half does");
        return NULL;
    }
    if (!hero_crypto_offered(scheme)) {
        hero_crypto_leave(*slot);
        return NULL;
    }
    return held;
}

int64_t hero_crypto_sign(hero_crypto_key *k, int64_t scheme, const void *message, uint64_t count, void *buffer, uint64_t buffer_count) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    size_t most;
    int64_t answer;

    if (scheme == HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "OAEP encrypts and does not sign");
        return -1;
    }
    if (!hero_crypto_bytes_given(message, count, HERO_CRYPTO_TEXT_MOST, "the message") || !hero_crypto_bytes_given(buffer, buffer_count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    held = hero_crypto_enter_for(k, scheme, true, &slot);
    if (held == NULL) {
        return -1;
    }
    most = hero_crypto_signature_most(held);
    if (buffer_count < most) {
        hero_crypto_leave(slot);
        hero_crypto_fine();
        return (int64_t)most;
    }
    answer = hero_crypto_arm_sign(held, scheme, (const unsigned char *)message, (size_t)count, (unsigned char *)buffer, most);
    hero_crypto_leave(slot);
    if (answer >= 0) {
        hero_crypto_fine();
    }
    return answer;
}

int64_t hero_crypto_verify(hero_crypto_key *k, int64_t scheme, const void *message, uint64_t count, const void *signature, uint64_t signature_count) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    int answer;

    if (scheme == HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "OAEP encrypts and does not sign");
        return -1;
    }
    if (!hero_crypto_bytes_given(message, count, HERO_CRYPTO_TEXT_MOST, "the message") || !hero_crypto_bytes_given(signature, signature_count, 1u << 16, "the signature")) {
        return -1;
    }
    held = hero_crypto_enter_for(k, scheme, false, &slot);
    if (held == NULL) {
        return -1;
    }
    answer = !hero_crypto_signature_formed(held, (const unsigned char *)signature, (size_t)signature_count) ? 0 : hero_crypto_arm_verify(held, scheme, (const unsigned char *)message, (size_t)count, (const unsigned char *)signature, (size_t)signature_count);
    hero_crypto_leave(slot);
    if (answer >= 0) {
        hero_crypto_fine();
    }
    return answer;
}

int64_t hero_crypto_agree(hero_crypto_key *mine, hero_crypto_key *theirs, void *buffer, uint64_t count) {
    hero_crypto_slot *mine_slot;
    hero_crypto_slot *theirs_slot;
    hero_crypto_held *mine_held;
    hero_crypto_held *theirs_held;
    size_t width;
    int64_t answer;

    if (!hero_crypto_bytes_given(buffer, count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    mine_held = hero_crypto_enter(mine, &mine_slot);
    if (mine_held == NULL) {
        return -1;
    }
    theirs_held = hero_crypto_enter(theirs, &theirs_slot);
    if (theirs_held == NULL) {
        hero_crypto_leave(mine_slot);
        return -1;
    }
    if (mine_held->kind != HERO_CRYPTO_X25519 && mine_held->kind != HERO_CRYPTO_P256 && mine_held->kind != HERO_CRYPTO_P384) {
        answer = -1;
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "only an X25519, P-256 or P-384 key agrees");
    } else if (mine_held->kind != theirs_held->kind) {
        answer = -1;
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the two keys are of different kinds");
    } else if (mine_held->is_public) {
        answer = -1;
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the first key must be private: a public key agrees with nothing");
    } else {
        width = mine_held->kind == HERO_CRYPTO_P384 ? 48 : 32;
        if (count < width) {
            answer = (int64_t)width;
            hero_crypto_fine();
        } else {
            answer = hero_crypto_arm_agree(mine_held, theirs_held, (unsigned char *)buffer, width);
            if (answer >= 0 && mine_held->kind == HERO_CRYPTO_X25519) {
                unsigned char any = 0;

                for (size_t i = 0; i < width; i += 1) {
                    any = (unsigned char)(any | ((unsigned char *)buffer)[i]);
                }
                if (any == 0) {
                    answer = -1;
                    hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the peer's X25519 key is of small order: the secret is all zeros (RFC 7748 6.1)");
                }
            }
            if (answer >= 0) {
                hero_crypto_fine();
            } else {
                hero_crypto_wipe(buffer, width);
            }
        }
    }
    hero_crypto_leave(theirs_slot);
    hero_crypto_leave(mine_slot);
    return answer;
}

int64_t hero_crypto_encrypt(hero_crypto_key *k, int64_t scheme, const void *text, uint64_t count, void *buffer, uint64_t buffer_count) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    size_t modulus;
    int64_t answer;

    if (scheme != HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the one scheme that encrypts is RSA-OAEP with SHA-256");
        return -1;
    }
    if (!hero_crypto_bytes_given(text, count, 1u << 16, "the text") || !hero_crypto_bytes_given(buffer, buffer_count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    held = hero_crypto_enter_for(k, scheme, false, &slot);
    if (held == NULL) {
        return -1;
    }
    modulus = hero_crypto_modulus_bytes(held);
    if (count + 66 > modulus) {
        char words[160];

        hero_crypto_leave(slot);
        (void)snprintf(words, sizeof words, "the text is %llu bytes; an RSA key of %lld bits encrypts at most %llu", (unsigned long long)count, (long long)held->bits, (unsigned long long)(modulus - 66));
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, words);
        return -1;
    }
    if (buffer_count < modulus) {
        hero_crypto_leave(slot);
        hero_crypto_fine();
        return (int64_t)modulus;
    }
    answer = hero_crypto_arm_encrypt(held, (const unsigned char *)text, (size_t)count, (unsigned char *)buffer, modulus);
    hero_crypto_leave(slot);
    if (answer >= 0) {
        hero_crypto_fine();
    }
    return answer;
}

int64_t hero_crypto_decrypt(hero_crypto_key *k, int64_t scheme, const void *data, uint64_t count, void *buffer, uint64_t buffer_count) {
    hero_crypto_slot *slot;
    hero_crypto_held *held;
    size_t most;
    int64_t answer;

    if (scheme != HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256) {
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "the one scheme that decrypts is RSA-OAEP with SHA-256");
        return -1;
    }
    if (!hero_crypto_bytes_given(data, count, 1u << 16, "the data") || !hero_crypto_bytes_given(buffer, buffer_count, UINT64_MAX, "the buffer")) {
        return -1;
    }
    held = hero_crypto_enter_for(k, scheme, true, &slot);
    if (held == NULL) {
        return -1;
    }
    most = hero_crypto_modulus_bytes(held) - 66;
    if (buffer_count < most) {
        hero_crypto_leave(slot);
        hero_crypto_fine();
        return (int64_t)most;
    }
    if (count != hero_crypto_modulus_bytes(held)) {
        hero_crypto_leave(slot);
        hero_crypto_said(HERO_CRYPTO_FORGED, 0, "the data do not decrypt under this key");
        return -1;
    }
    answer = hero_crypto_arm_decrypt(held, (const unsigned char *)data, (size_t)count, (unsigned char *)buffer, most);
    hero_crypto_leave(slot);
    if (answer == -2) {
        hero_crypto_wipe(buffer, most);
        hero_crypto_said(HERO_CRYPTO_FORGED, 0, "the data do not decrypt under this key");
        return -1;
    }
    if (answer >= 0) {
        hero_crypto_fine();
    }
    return answer;
}

/* ===========================================================================
 * THE APPLE ARM: the Security framework's `SecKey`.
 *
 * A key is a `SecKeyRef` made by `SecKeyCreateRandomKey` or
 * `SecKeyCreateWithData` and kept in no keychain. `SecKeyCreateWithData`
 * reads an RSA key as PKCS #1 and an EC key as X9.63's `04 || X || Y [||
 * K]`, which THE FORMS build from the material.
 *
 * ED25519 AND X25519 ARE MONOCYPHER'S, vendored
 * (`runtime/vendor/monocypher/README.md`; decision 2594): `SecKey` takes no
 * Curve25519 key, and CryptoKit's Ed25519 signs differently every time, so
 * it gives neither RFC 8032's signatures nor the deterministic one the face
 * promises (panel 253's R3).
 * Its two files are compiled inside this arm, every function and object of
 * theirs given internal linkage, so this object exports none of their
 * names and a program binding Monocypher or libsodium itself meets no second
 * definition (panel 253's R1); the second file's six macros of the same
 * names as the first's are taken back and its `hash_reduce` renamed, its
 * bytes untouched. Their unused functions are not compiled in, which is why
 * their warning is off here alone. The key's 32 private bytes live in the
 * layer's block (`secret`), overwritten when it is given back, and every
 * copy this arm makes on its stack is overwritten before it returns.
 * ========================================================================== */
#if defined(__APPLE__)

/* `<sys/param.h>`, which `Security.h` includes, defines `MIN` and `MAX` as
 * Monocypher does: the SDK's are kept aside and given back after it. */
#pragma push_macro("MIN")
#pragma push_macro("MAX")
#undef MIN
#undef MAX
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
/* The attribute stands on each name's first declaration, the headers', the
 * one object `crypto_argon2_no_extras` among them; over the sources it takes
 * functions alone, since an object subject there would also reach a
 * `static` inside a function, which clang then names as a file's object
 * nobody defines (`base_point`, `half_mod_L`: undefined at the link,
 * measured on this Mac). */
#pragma clang attribute push(__attribute__((internal_linkage)), apply_to = any(function, variable(is_global)))
#include "vendor/monocypher/monocypher.h"
#include "vendor/monocypher/monocypher-ed25519.h"
#pragma clang attribute pop
#pragma clang attribute push(__attribute__((internal_linkage)), apply_to = function)
#include "vendor/monocypher/monocypher.c"
#undef COPY
#undef FOR
#undef MIN
#undef WIPE_BUFFER
#undef WIPE_CTX
#undef ZERO
#define hash_reduce hero_crypto_monocypher_ed25519_hash_reduce
#include "vendor/monocypher/monocypher-ed25519.c"
#undef hash_reduce
#pragma clang attribute pop
#pragma clang diagnostic pop
/* Every macro the two files define, taken back, so none reaches the code
 * below: `monocypher.c`'s eighteen, the second file's six among them. */
#undef FOR_T
#undef FOR
#undef COPY
#undef ZERO
#undef WIPE_CTX
#undef WIPE_BUFFER
#undef MIN
#undef MAX
#undef QUARTERROUND
#undef BLAKE2_G
#undef BLAKE2_ROUND
#undef LSB
#undef G
#undef ROUND
#undef FE_CARRY
#undef P_W_WIDTH
#undef B_W_WIDTH
#undef P_W_SIZE
#pragma pop_macro("MAX")
#pragma pop_macro("MIN")

static bool hero_crypto_is_curve(int64_t kind) {
    return kind == HERO_CRYPTO_ED25519 || kind == HERO_CRYPTO_X25519;
}

/* A 25519 key's public half from its 32 private bytes, into
 * `held->curve_public`: Ed25519's by RFC 8032's key generation (the seed
 * hashed by SHA-512), X25519's by RFC 7748's base point. */
static void hero_crypto_curve_public_of(hero_crypto_held *held) {
    if (held->kind == HERO_CRYPTO_X25519) {
        crypto_x25519_public_key(held->curve_public, held->secret.bytes);
        return;
    }
    {
        /* Monocypher wipes the seed it is given and writes the seed and
         * the public key as its secret key: both copies are the stack's,
         * overwritten here. */
        unsigned char seed[32];
        unsigned char both[64];

        memcpy(seed, held->secret.bytes, 32);
        crypto_ed25519_key_pair(both, held->curve_public, seed);
        hero_crypto_wipe(seed, sizeof seed);
        hero_crypto_wipe(both, sizeof both);
    }
}

/* `held`'s 32 private bytes taken from `bytes`, and its public half. */
static bool hero_crypto_curve_take(hero_crypto_held *held, const unsigned char *bytes) {
    const unsigned char *parts[] = { bytes };
    const size_t counts[] = { 32 };

    if (!hero_crypto_join(&held->secret, parts, counts, 1)) {
        return false;
    }
    hero_crypto_curve_public_of(held);
    return true;
}

/* A CoreFoundation error said: its code and its description, which the
 * error's owner releases. */
static void hero_crypto_cf_said(int64_t kind, const char *what, CFErrorRef error) {
    char words[HERO_CRYPTO_WORDS_MOST];
    char described[384];
    int64_t code = 0;

    described[0] = '\0';
    if (error != NULL) {
        CFStringRef text = CFErrorCopyDescription(error);

        code = (int64_t)CFErrorGetCode(error);
        if (text != NULL) {
            if (!CFStringGetCString(text, described, sizeof described, kCFStringEncodingUTF8)) {
                described[0] = '\0';
            }
            CFRelease(text);
        }
        CFRelease(error);
    }
    if (described[0] != '\0') {
        (void)snprintf(words, sizeof words, "%s: %s", what, described);
    } else {
        (void)snprintf(words, sizeof words, "%s", what);
    }
    hero_crypto_said(kind, code, words);
}

/* Bytes lent to the framework as a CFData that copies nothing and frees
 * nothing. */
static CFDataRef hero_crypto_cf_lend(const unsigned char *bytes, size_t count) {
    static const unsigned char none[1] = { 0 };

    return CFDataCreateWithBytesNoCopy(NULL, count == 0 ? none : bytes, (CFIndex)count, kCFAllocatorNull);
}

static bool hero_crypto_arm_ready(void) {
    return true;
}

static bool hero_crypto_arm_offers(int64_t what) {
    return hero_crypto_is_aead(what) || hero_crypto_is_curve(what) || what == HERO_CRYPTO_P256 || what == HERO_CRYPTO_P384 || what == HERO_CRYPTO_RSA || (what >= HERO_CRYPTO_SIGN_ED25519 && what <= HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512) || what == HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256;
}

static bool hero_crypto_arm_random(unsigned char *into, size_t count) {
    int status = SecRandomCopyBytes(kSecRandomDefault, count, into);

    if (status != errSecSuccess) {
        hero_crypto_said(HERO_CRYPTO_OTHER, (int64_t)status, "SecRandomCopyBytes failed");
        return false;
    }
    return true;
}

/* A dictionary of a key's type, class or size; NULL with `LIMIT`. */
static CFMutableDictionaryRef hero_crypto_cf_attributes(int64_t kind, CFStringRef key_class, int64_t bits) {
    CFMutableDictionaryRef d = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    int size = (int)bits;

    if (d == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the key's attributes");
        return NULL;
    }
    CFDictionarySetValue(d, kSecAttrKeyType, kind == HERO_CRYPTO_RSA ? kSecAttrKeyTypeRSA : kSecAttrKeyTypeECSECPrimeRandom);
    if (key_class != NULL) {
        CFDictionarySetValue(d, kSecAttrKeyClass, key_class);
    }
    if (bits != 0) {
        CFNumberRef n = CFNumberCreate(NULL, kCFNumberIntType, &size);

        if (n == NULL) {
            CFRelease(d);
            hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the key's attributes");
            return NULL;
        }
        CFDictionarySetValue(d, kSecAttrKeySizeInBits, n);
        CFRelease(n);
    }
    return d;
}

static bool hero_crypto_arm_generate(hero_crypto_held *held, int64_t bits) {
    int64_t size = held->kind == HERO_CRYPTO_RSA ? bits : held->kind == HERO_CRYPTO_P256 ? 256 : 384;
    CFMutableDictionaryRef d;
    CFErrorRef error = NULL;

    if (hero_crypto_is_curve(held->kind)) {
        /* 32 bytes from the system's generator, as RFC 8032 § 5.1.5 and
         * RFC 7748 § 6.1 draw a private key. */
        unsigned char drawn[32];
        bool good = hero_crypto_arm_random(drawn, sizeof drawn) && hero_crypto_curve_take(held, drawn);

        hero_crypto_wipe(drawn, sizeof drawn);
        held->bits = held->kind == HERO_CRYPTO_ED25519 ? 256 : 255;
        return good;
    }
    d = hero_crypto_cf_attributes(held->kind, NULL, size);
    if (d == NULL) {
        return false;
    }
    held->key = SecKeyCreateRandomKey(d, &error);
    CFRelease(d);
    if (held->key == NULL) {
        hero_crypto_cf_said(HERO_CRYPTO_OTHER, "SecKeyCreateRandomKey failed", error);
        return false;
    }
    held->bits = size;
    return true;
}

static bool hero_crypto_arm_load(hero_crypto_held *held, const hero_crypto_material *m, const unsigned char *der, size_t der_count) {
    unsigned char own[1 + 3 * HERO_CRYPTO_WIDTH_MOST];
    const unsigned char *bytes;
    size_t count;
    CFMutableDictionaryRef d;
    CFDataRef data;
    CFErrorRef error = NULL;

    (void)der;
    (void)der_count;
    if (hero_crypto_is_curve(m->kind)) {
        /* A private key's public half is computed, as Linux's is: RFC
         * 8410's PKCS #8 holds none, and a version 2's is passed over by
         * THE FORMS on every platform. */
        if (m->is_private) {
            return hero_crypto_curve_take(held, m->scalar);
        }
        memcpy(held->curve_public, m->point, sizeof held->curve_public);
        return true;
    }
    if (m->kind == HERO_CRYPTO_RSA) {
        bytes = m->rsa.bytes;
        count = m->rsa.count;
    } else {
        if (m->point_count == 0) {
            hero_crypto_said(HERO_CRYPTO_UNSUPPORTED, 0, "the EC key holds no public point, which macOS needs and does not compute");
            return false;
        }
        memcpy(own, m->point, m->point_count);
        count = m->point_count;
        if (m->is_private) {
            memcpy(own + count, m->scalar, m->width);
            count += m->width;
        }
        bytes = own;
    }
    d = hero_crypto_cf_attributes(m->kind, m->is_private ? kSecAttrKeyClassPrivate : kSecAttrKeyClassPublic, 0);
    if (d == NULL) {
        hero_crypto_wipe(own, sizeof own);
        return false;
    }
    data = CFDataCreate(NULL, bytes, (CFIndex)count);
    hero_crypto_wipe(own, sizeof own);
    if (data == NULL) {
        CFRelease(d);
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the key");
        return false;
    }
    held->key = SecKeyCreateWithData(data, d, &error);
    CFRelease(data);
    CFRelease(d);
    if (held->key == NULL) {
        hero_crypto_cf_said(HERO_CRYPTO_INVALID, "the Security framework refused the key", error);
        return false;
    }
    return true;
}

static bool hero_crypto_arm_material(const hero_crypto_held *held, hero_crypto_material *m) {
    CFErrorRef error = NULL;
    CFDataRef data;
    const unsigned char *bytes;
    size_t count;
    bool good = true;

    memset(m, 0, sizeof *m);
    m->kind = held->kind;
    m->bits = held->bits;
    m->is_private = !held->is_public;
    if (hero_crypto_is_curve(held->kind)) {
        m->width = 32;
        memcpy(m->point, held->curve_public, 32);
        m->point_count = 32;
        if (m->is_private) {
            memcpy(m->scalar, held->secret.bytes, 32);
        }
        return true;
    }
    data = SecKeyCopyExternalRepresentation(held->key, &error);
    if (data == NULL) {
        hero_crypto_cf_said(HERO_CRYPTO_OTHER, "SecKeyCopyExternalRepresentation failed", error);
        return false;
    }
    bytes = CFDataGetBytePtr(data);
    count = (size_t)CFDataGetLength(data);
    if (held->kind == HERO_CRYPTO_RSA) {
        hero_crypto_span whole = { bytes, count };

        good = hero_crypto_rsa_take(m, whole, m->is_private);
    } else {
        m->width = held->kind == HERO_CRYPTO_P256 ? 32 : 48;
        if (count != 1 + 2 * m->width + (m->is_private ? m->width : 0) || bytes[0] != 0x04) {
            hero_crypto_said(HERO_CRYPTO_OTHER, 0, "the Security framework gave the EC key in an unknown form");
            good = false;
        } else {
            memcpy(m->point, bytes, 1 + 2 * m->width);
            m->point_count = 1 + 2 * m->width;
            if (m->is_private) {
                memcpy(m->scalar, bytes + m->point_count, m->width);
            }
        }
    }
    /* The framework's copy is its own, immutable: released, not wiped. */
    CFRelease(data);
    return good;
}

static bool hero_crypto_arm_public(const hero_crypto_held *from, hero_crypto_held *to) {
    if (hero_crypto_is_curve(from->kind)) {
        memcpy(to->curve_public, from->curve_public, sizeof to->curve_public);
        return true;
    }
    to->key = SecKeyCopyPublicKey(from->key);
    if (to->key == NULL) {
        hero_crypto_said(HERO_CRYPTO_OTHER, 0, "SecKeyCopyPublicKey answered no key");
        return false;
    }
    return true;
}

/* AES-GCM and ChaCha20-Poly1305 by CryptoKit, through the layer's Swift
 * member (`hero_crypto_cryptokit.swift`): macOS offers neither to C. Every
 * count is below `HERO_CRYPTO_TEXT_MOST`, the common part's check, so it is
 * an `int64_t` as Swift's `Int64` takes it. */
static bool hero_crypto_arm_seal(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *text, size_t text_count, const unsigned char *associated, size_t associated_count, unsigned char *out, unsigned char *tag) {
    char words[HERO_CRYPTO_WORDS_MOST];

    words[0] = '\0';
    if (hero_crypto_cryptokit_seal(held->kind, held->secret.bytes, (int64_t)held->secret.count, nonce, text, (int64_t)text_count, associated, (int64_t)associated_count, out, tag, words, (int64_t)sizeof words) != 1) {
        hero_crypto_said(HERO_CRYPTO_OTHER, 0, words[0] != '\0' ? words : "CryptoKit refused to seal");
        return false;
    }
    return true;
}

static int hero_crypto_arm_open(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *data, size_t data_count, const unsigned char *associated, size_t associated_count, const unsigned char *tag, unsigned char *out) {
    char words[HERO_CRYPTO_WORDS_MOST];
    int64_t opened;

    words[0] = '\0';
    opened = hero_crypto_cryptokit_open(held->kind, held->secret.bytes, (int64_t)held->secret.count, nonce, data, (int64_t)data_count, associated, (int64_t)associated_count, tag, out, words, (int64_t)sizeof words);
    if (opened == 1 || opened == 0) {
        return (int)opened;
    }
    hero_crypto_said(HERO_CRYPTO_OTHER, 0, words[0] != '\0' ? words : "CryptoKit refused to open");
    return -1;
}

static SecKeyAlgorithm hero_crypto_apple_algorithm(int64_t scheme) {
    switch (scheme) {
    case HERO_CRYPTO_SIGN_ECDSA_SHA256:
        return kSecKeyAlgorithmECDSASignatureMessageX962SHA256;
    case HERO_CRYPTO_SIGN_ECDSA_SHA384:
        return kSecKeyAlgorithmECDSASignatureMessageX962SHA384;
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA256:
        return kSecKeyAlgorithmRSASignatureMessagePSSSHA256;
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA384:
        return kSecKeyAlgorithmRSASignatureMessagePSSSHA384;
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA512:
        return kSecKeyAlgorithmRSASignatureMessagePSSSHA512;
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA256:
        return kSecKeyAlgorithmRSASignatureMessagePKCS1v15SHA256;
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA384:
        return kSecKeyAlgorithmRSASignatureMessagePKCS1v15SHA384;
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512:
        return kSecKeyAlgorithmRSASignatureMessagePKCS1v15SHA512;
    default:
        return kSecKeyAlgorithmRSAEncryptionOAEPSHA256;
    }
}

/* `data`, a CFData the framework answered, copied into `out` where it fits
 * in `most`; its length, or -1. Released either way. */
static int64_t hero_crypto_cf_give(CFDataRef data, unsigned char *out, size_t most) {
    size_t count = (size_t)CFDataGetLength(data);

    if (count > most) {
        CFRelease(data);
        hero_crypto_said(HERO_CRYPTO_OTHER, 0, "the Security framework answered more bytes than the call allows");
        return -1;
    }
    memcpy(out, CFDataGetBytePtr(data), count);
    CFRelease(data);
    return (int64_t)count;
}

static int64_t hero_crypto_arm_sign(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, unsigned char *out, size_t most) {
    CFDataRef lent;
    CFErrorRef error = NULL;
    CFDataRef signature;

    if (scheme == HERO_CRYPTO_SIGN_ED25519) {
        /* RFC 8032's signature, deterministic: Monocypher's secret key is
         * the seed then the public key, on the stack for this call alone.
         * `most` is 64, the common part's measure. */
        unsigned char both[64];

        (void)most;
        memcpy(both, held->secret.bytes, 32);
        memcpy(both + 32, held->curve_public, 32);
        crypto_ed25519_sign(out, both, message, count);
        hero_crypto_wipe(both, sizeof both);
        return 64;
    }
    lent = hero_crypto_cf_lend(message, count);
    if (lent == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the message");
        return -1;
    }
    signature = SecKeyCreateSignature(held->key, hero_crypto_apple_algorithm(scheme), lent, &error);
    CFRelease(lent);
    if (signature == NULL) {
        hero_crypto_cf_said(HERO_CRYPTO_OTHER, "SecKeyCreateSignature failed", error);
        return -1;
    }
    return hero_crypto_cf_give(signature, out, most);
}

static int hero_crypto_arm_verify(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, const unsigned char *signature, size_t signature_count) {
    SecKeyRef key;
    CFDataRef lent;
    CFDataRef sig;
    CFErrorRef error = NULL;
    int answer;

    if (scheme == HERO_CRYPTO_SIGN_ED25519) {
        /* The common part has checked the form: 64 bytes, R and the key
         * encodings RFC 8032 § 5.1.3 decodes and S below the group's order
         * (`hero_crypto_signature_formed`), since Monocypher reduces y
         * modulo p and takes either sign of a zero x in both. */
        (void)signature_count;
        return crypto_ed25519_check(signature, held->curve_public, message, count) == 0 ? 1 : 0;
    }
    key = held->is_public ? (SecKeyRef)CFRetain(held->key) : SecKeyCopyPublicKey(held->key);
    lent = hero_crypto_cf_lend(message, count);
    sig = hero_crypto_cf_lend(signature, signature_count);
    if (key == NULL || lent == NULL || sig == NULL) {
        answer = -1;
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left to check the signature");
    } else {
        answer = SecKeyVerifySignature(key, hero_crypto_apple_algorithm(scheme), lent, sig, &error) ? 1 : 0;
        if (error != NULL) {
            CFRelease(error);
        }
    }
    if (key != NULL) {
        CFRelease(key);
    }
    if (lent != NULL) {
        CFRelease(lent);
    }
    if (sig != NULL) {
        CFRelease(sig);
    }
    return answer;
}

static int64_t hero_crypto_arm_agree(const hero_crypto_held *mine, const hero_crypto_held *theirs, unsigned char *out, size_t width) {
    SecKeyRef peer;
    CFMutableDictionaryRef none;
    CFErrorRef error = NULL;
    CFDataRef secret = NULL;
    int64_t answer = -1;

    if (mine->kind == HERO_CRYPTO_X25519) {
        /* RFC 7748 § 5's function on the peer's public bytes; an all-zero
         * answer is the common part's refusal. `width` is 32. */
        crypto_x25519(out, mine->secret.bytes, theirs->curve_public);
        return (int64_t)width;
    }
    peer = theirs->is_public ? (SecKeyRef)CFRetain(theirs->key) : SecKeyCopyPublicKey(theirs->key);
    none = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    if (peer == NULL || none == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left to agree");
    } else {
        secret = SecKeyCopyKeyExchangeResult(mine->key, kSecKeyAlgorithmECDHKeyExchangeStandard, peer, none, &error);
        if (secret == NULL) {
            hero_crypto_cf_said(HERO_CRYPTO_OTHER, "SecKeyCopyKeyExchangeResult failed", error);
        } else if ((size_t)CFDataGetLength(secret) != width) {
            CFRelease(secret);
            hero_crypto_said(HERO_CRYPTO_OTHER, 0, "the Security framework answered a secret of another length");
        } else {
            answer = hero_crypto_cf_give(secret, out, width);
        }
    }
    if (peer != NULL) {
        CFRelease(peer);
    }
    if (none != NULL) {
        CFRelease(none);
    }
    return answer;
}

static int64_t hero_crypto_arm_encrypt(const hero_crypto_held *held, const unsigned char *text, size_t count, unsigned char *out, size_t most) {
    SecKeyRef key = held->is_public ? (SecKeyRef)CFRetain(held->key) : SecKeyCopyPublicKey(held->key);
    CFDataRef lent = hero_crypto_cf_lend(text, count);
    CFErrorRef error = NULL;
    CFDataRef data = NULL;
    int64_t answer = -1;

    if (key == NULL || lent == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left to encrypt");
    } else {
        data = SecKeyCreateEncryptedData(key, kSecKeyAlgorithmRSAEncryptionOAEPSHA256, lent, &error);
        if (data == NULL) {
            hero_crypto_cf_said(HERO_CRYPTO_OTHER, "SecKeyCreateEncryptedData failed", error);
        } else {
            answer = hero_crypto_cf_give(data, out, most);
        }
    }
    if (key != NULL) {
        CFRelease(key);
    }
    if (lent != NULL) {
        CFRelease(lent);
    }
    return answer;
}

static int64_t hero_crypto_arm_decrypt(const hero_crypto_held *held, const unsigned char *data, size_t count, unsigned char *out, size_t most) {
    CFDataRef lent = hero_crypto_cf_lend(data, count);
    CFErrorRef error = NULL;
    CFDataRef text;

    if (lent == NULL) {
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left to decrypt");
        return -1;
    }
    text = SecKeyCreateDecryptedData(held->key, kSecKeyAlgorithmRSAEncryptionOAEPSHA256, lent, &error);
    CFRelease(lent);
    if (text == NULL) {
        if (error != NULL) {
            CFRelease(error);
        }
        return -2;
    }
    /* The framework's copy of the text is its own and immutable: released,
     * not wiped (said in the face). */
    return hero_crypto_cf_give(text, out, most);
}

static void hero_crypto_arm_free(hero_crypto_held *held) {
    if (held->key != NULL) {
        CFRelease(held->key);
        held->key = NULL;
    }
}

#endif

/* ===========================================================================
 * THE LINUX ARM: OpenSSL 3's `libcrypto.so.3`, opened with `dlopen`.
 *
 * WHY `dlopen`, as `hero_tls.c` says it for `libssl`: the layer's archive is
 * compiled for every program a machine builds, so a member that included
 * `<openssl/evp.h>` would stop every build on a machine without OpenSSL's
 * headers, and one linked `-lcrypto` every program on one without the
 * library. The member declares the functions it calls below with OpenSSL
 * 3's own signatures (its ABI is kept across 3.x under `libcrypto.so.3`),
 * none deprecated (the probe compiled with `OPENSSL_NO_DEPRECATED`), and
 * finds each by `dlsym` once; one missing is the library missing, and then
 * every call answers `HERO_CRYPTO_UNSUPPORTED`. The library is never
 * closed, so no pointer outlives it.
 * ========================================================================== */
#if !defined(_WIN32) && !defined(__APPLE__)

/* OpenSSL's numbers this arm passes, as its headers name them (read in the
 * Linux arm64 image, OpenSSL 3.5.7, 2026-11-17). */
#define HERO_EVP_CTRL_AEAD_SET_IVLEN 0x9
#define HERO_EVP_CTRL_AEAD_GET_TAG 0x10
#define HERO_EVP_CTRL_AEAD_SET_TAG 0x11
#define HERO_RSA_PKCS1_PADDING 1
#define HERO_RSA_PKCS1_OAEP_PADDING 4
#define HERO_RSA_PKCS1_PSS_PADDING 6
#define HERO_RSA_PSS_SALTLEN_DIGEST (-1)

/* The functions this arm calls, found once. */
typedef struct hero_crypto_openssl {
    const void *(*EVP_aes_128_gcm)(void);
    const void *(*EVP_aes_256_gcm)(void);
    const void *(*EVP_chacha20_poly1305)(void);
    void *(*EVP_CIPHER_CTX_new)(void);
    void (*EVP_CIPHER_CTX_free)(void *ctx);
    int (*EVP_EncryptInit_ex)(void *ctx, const void *cipher, void *engine, const unsigned char *key, const unsigned char *iv);
    int (*EVP_EncryptUpdate)(void *ctx, unsigned char *out, int *out_count, const unsigned char *in, int in_count);
    int (*EVP_EncryptFinal_ex)(void *ctx, unsigned char *out, int *out_count);
    int (*EVP_DecryptInit_ex)(void *ctx, const void *cipher, void *engine, const unsigned char *key, const unsigned char *iv);
    int (*EVP_DecryptUpdate)(void *ctx, unsigned char *out, int *out_count, const unsigned char *in, int in_count);
    int (*EVP_DecryptFinal_ex)(void *ctx, unsigned char *out, int *out_count);
    int (*EVP_CIPHER_CTX_ctrl)(void *ctx, int type, int arg, void *ptr);
    int (*RAND_bytes)(unsigned char *buffer, int count);
    void *(*EVP_PKEY_CTX_new_from_name)(void *library, const char *name, const char *properties);
    void *(*EVP_PKEY_CTX_new_from_pkey)(void *library, void *pkey, const char *properties);
    void (*EVP_PKEY_CTX_free)(void *ctx);
    int (*EVP_PKEY_keygen_init)(void *ctx);
    int (*EVP_PKEY_CTX_set_group_name)(void *ctx, const char *name);
    int (*EVP_PKEY_CTX_set_rsa_keygen_bits)(void *ctx, int bits);
    int (*EVP_PKEY_generate)(void *ctx, void **pkey);
    void (*EVP_PKEY_free)(void *pkey);
    int (*EVP_PKEY_is_a)(const void *pkey, const char *name);
    void *(*EVP_MD_CTX_new)(void);
    void (*EVP_MD_CTX_free)(void *ctx);
    int (*EVP_DigestSignInit_ex)(void *ctx, void **pkey_ctx, const char *digest, void *library, const char *properties, void *pkey, const void *params);
    int (*EVP_DigestSign)(void *ctx, unsigned char *signature, size_t *signature_count, const unsigned char *message, size_t count);
    int (*EVP_DigestVerifyInit_ex)(void *ctx, void **pkey_ctx, const char *digest, void *library, const char *properties, void *pkey, const void *params);
    int (*EVP_DigestVerify)(void *ctx, const unsigned char *signature, size_t signature_count, const unsigned char *message, size_t count);
    int (*EVP_PKEY_CTX_set_rsa_padding)(void *ctx, int padding);
    int (*EVP_PKEY_CTX_set_rsa_pss_saltlen)(void *ctx, int length);
    int (*EVP_PKEY_CTX_set_rsa_oaep_md_name)(void *ctx, const char *digest, const char *properties);
    int (*EVP_PKEY_CTX_set_rsa_mgf1_md_name)(void *ctx, const char *digest, const char *properties);
    int (*EVP_PKEY_encrypt_init)(void *ctx);
    int (*EVP_PKEY_encrypt)(void *ctx, unsigned char *out, size_t *out_count, const unsigned char *in, size_t in_count);
    int (*EVP_PKEY_decrypt_init)(void *ctx);
    int (*EVP_PKEY_decrypt)(void *ctx, unsigned char *out, size_t *out_count, const unsigned char *in, size_t in_count);
    int (*EVP_PKEY_derive_init)(void *ctx);
    int (*EVP_PKEY_derive_set_peer)(void *ctx, void *peer);
    int (*EVP_PKEY_derive)(void *ctx, unsigned char *secret, size_t *count);
    void *(*d2i_PKCS8_PRIV_KEY_INFO)(void **out, const unsigned char **in, long count);
    void *(*EVP_PKCS82PKEY)(const void *info);
    void (*PKCS8_PRIV_KEY_INFO_free)(void *info);
    void *(*EVP_PKEY2PKCS8)(const void *pkey);
    int (*i2d_PKCS8_PRIV_KEY_INFO)(const void *info, unsigned char **out);
    void *(*d2i_PUBKEY)(void **out, const unsigned char **in, long count);
    int (*i2d_PUBKEY)(const void *pkey, unsigned char **out);
    unsigned long (*ERR_get_error)(void);
    void (*ERR_error_string_n)(unsigned long error, char *buffer, size_t count);
    void (*ERR_clear_error)(void);
} hero_crypto_openssl;

/* The functions, found once by `hero_crypto_load` under `hero_crypto_once`,
 * and read only after it: whether they were all found. */
static hero_crypto_openssl hero_crypto_ssl;
static bool hero_crypto_ssl_found;
static pthread_once_t hero_crypto_once = PTHREAD_ONCE_INIT;

/* One function by name into `*slot`; false where the library has none. C11
 * has no conversion between an object pointer and a function pointer, so
 * the address is copied by its bytes, which POSIX makes the same for both
 * (`dlsym`'s own page). */
static bool hero_crypto_find_function(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_CRYPTO_FIND(name) ok = ok && hero_crypto_find_function(library, #name, &hero_crypto_ssl.name, sizeof hero_crypto_ssl.name)

static void hero_crypto_load(void) {
    static const char *const names[] = { "libcrypto.so.3", "libcrypto.so.4" };
    bool ok = true;
    void *library = NULL;

    for (size_t i = 0; i < sizeof names / sizeof names[0] && library == NULL; i += 1) {
        library = dlopen(names[i], RTLD_NOW | RTLD_LOCAL);
    }
    if (library == NULL) {
        return;
    }
    HERO_CRYPTO_FIND(EVP_aes_128_gcm);
    HERO_CRYPTO_FIND(EVP_aes_256_gcm);
    HERO_CRYPTO_FIND(EVP_chacha20_poly1305);
    HERO_CRYPTO_FIND(EVP_CIPHER_CTX_new);
    HERO_CRYPTO_FIND(EVP_CIPHER_CTX_free);
    HERO_CRYPTO_FIND(EVP_EncryptInit_ex);
    HERO_CRYPTO_FIND(EVP_EncryptUpdate);
    HERO_CRYPTO_FIND(EVP_EncryptFinal_ex);
    HERO_CRYPTO_FIND(EVP_DecryptInit_ex);
    HERO_CRYPTO_FIND(EVP_DecryptUpdate);
    HERO_CRYPTO_FIND(EVP_DecryptFinal_ex);
    HERO_CRYPTO_FIND(EVP_CIPHER_CTX_ctrl);
    HERO_CRYPTO_FIND(RAND_bytes);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_new_from_name);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_new_from_pkey);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_free);
    HERO_CRYPTO_FIND(EVP_PKEY_keygen_init);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_set_group_name);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_set_rsa_keygen_bits);
    HERO_CRYPTO_FIND(EVP_PKEY_generate);
    HERO_CRYPTO_FIND(EVP_PKEY_free);
    HERO_CRYPTO_FIND(EVP_PKEY_is_a);
    HERO_CRYPTO_FIND(EVP_MD_CTX_new);
    HERO_CRYPTO_FIND(EVP_MD_CTX_free);
    HERO_CRYPTO_FIND(EVP_DigestSignInit_ex);
    HERO_CRYPTO_FIND(EVP_DigestSign);
    HERO_CRYPTO_FIND(EVP_DigestVerifyInit_ex);
    HERO_CRYPTO_FIND(EVP_DigestVerify);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_set_rsa_padding);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_set_rsa_pss_saltlen);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_set_rsa_oaep_md_name);
    HERO_CRYPTO_FIND(EVP_PKEY_CTX_set_rsa_mgf1_md_name);
    HERO_CRYPTO_FIND(EVP_PKEY_encrypt_init);
    HERO_CRYPTO_FIND(EVP_PKEY_encrypt);
    HERO_CRYPTO_FIND(EVP_PKEY_decrypt_init);
    HERO_CRYPTO_FIND(EVP_PKEY_decrypt);
    HERO_CRYPTO_FIND(EVP_PKEY_derive_init);
    HERO_CRYPTO_FIND(EVP_PKEY_derive_set_peer);
    HERO_CRYPTO_FIND(EVP_PKEY_derive);
    HERO_CRYPTO_FIND(d2i_PKCS8_PRIV_KEY_INFO);
    HERO_CRYPTO_FIND(EVP_PKCS82PKEY);
    HERO_CRYPTO_FIND(PKCS8_PRIV_KEY_INFO_free);
    HERO_CRYPTO_FIND(EVP_PKEY2PKCS8);
    HERO_CRYPTO_FIND(i2d_PKCS8_PRIV_KEY_INFO);
    HERO_CRYPTO_FIND(d2i_PUBKEY);
    HERO_CRYPTO_FIND(i2d_PUBKEY);
    HERO_CRYPTO_FIND(ERR_get_error);
    HERO_CRYPTO_FIND(ERR_error_string_n);
    HERO_CRYPTO_FIND(ERR_clear_error);
    hero_crypto_ssl_found = ok;
}

static bool hero_crypto_arm_ready(void) {
    (void)pthread_once(&hero_crypto_once, hero_crypto_load);
    return hero_crypto_ssl_found;
}

/* OpenSSL's newest reason for the failure on this thread, as words, the
 * queue emptied. */
static void hero_crypto_ssl_said(int64_t kind, const char *what) {
    char reason[256];
    char words[HERO_CRYPTO_WORDS_MOST];
    unsigned long error = hero_crypto_ssl.ERR_get_error();
    unsigned long last = error;

    while (error != 0) {
        last = error;
        error = hero_crypto_ssl.ERR_get_error();
    }
    if (last != 0) {
        hero_crypto_ssl.ERR_error_string_n(last, reason, sizeof reason);
        (void)snprintf(words, sizeof words, "%s: %s", what, reason);
    } else {
        (void)snprintf(words, sizeof words, "%s", what);
    }
    hero_crypto_said(kind, (int64_t)(last & 0x7FFFFFFFu), words);
}

static bool hero_crypto_arm_offers(int64_t what) {
    (void)what;
    return true;
}

static bool hero_crypto_arm_random(unsigned char *into, size_t count) {
    if (hero_crypto_ssl.RAND_bytes(into, (int)count) != 1) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "RAND_bytes failed");
        return false;
    }
    return true;
}

/* OpenSSL's name of a pair's kind. */
static const char *hero_crypto_ssl_name(int64_t kind) {
    switch (kind) {
    case HERO_CRYPTO_ED25519:
        return "ED25519";
    case HERO_CRYPTO_X25519:
        return "X25519";
    case HERO_CRYPTO_RSA:
        return "RSA";
    default:
        return "EC";
    }
}

static bool hero_crypto_arm_generate(hero_crypto_held *held, int64_t bits) {
    void *ctx = hero_crypto_ssl.EVP_PKEY_CTX_new_from_name(NULL, hero_crypto_ssl_name(held->kind), NULL);
    void *pkey = NULL;
    bool ok = ctx != NULL && hero_crypto_ssl.EVP_PKEY_keygen_init(ctx) == 1;

    if (ok && (held->kind == HERO_CRYPTO_P256 || held->kind == HERO_CRYPTO_P384)) {
        ok = hero_crypto_ssl.EVP_PKEY_CTX_set_group_name(ctx, held->kind == HERO_CRYPTO_P256 ? "P-256" : "P-384") == 1;
    }
    if (ok && held->kind == HERO_CRYPTO_RSA) {
        ok = hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, (int)bits) == 1;
    }
    ok = ok && hero_crypto_ssl.EVP_PKEY_generate(ctx, &pkey) == 1;
    if (ctx != NULL) {
        hero_crypto_ssl.EVP_PKEY_CTX_free(ctx);
    }
    if (!ok) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL made no key");
        return false;
    }
    held->pkey = pkey;
    held->bits = held->kind == HERO_CRYPTO_RSA ? bits : held->kind == HERO_CRYPTO_P256 ? 256 : held->kind == HERO_CRYPTO_P384 ? 384 : held->kind == HERO_CRYPTO_ED25519 ? 256 : 255;
    return true;
}

static bool hero_crypto_arm_load(hero_crypto_held *held, const hero_crypto_material *m, const unsigned char *der, size_t der_count) {
    const unsigned char *at = der;
    void *pkey = NULL;

    hero_crypto_ssl.ERR_clear_error();
    if (m->is_private) {
        void *info = hero_crypto_ssl.d2i_PKCS8_PRIV_KEY_INFO(NULL, &at, (long)der_count);

        if (info != NULL) {
            pkey = hero_crypto_ssl.EVP_PKCS82PKEY(info);
            hero_crypto_ssl.PKCS8_PRIV_KEY_INFO_free(info);
        }
    } else {
        pkey = hero_crypto_ssl.d2i_PUBKEY(NULL, &at, (long)der_count);
    }
    if (pkey == NULL) {
        hero_crypto_ssl_said(HERO_CRYPTO_INVALID, "OpenSSL refused the key");
        return false;
    }
    if (hero_crypto_ssl.EVP_PKEY_is_a(pkey, hero_crypto_ssl_name(m->kind)) != 1) {
        hero_crypto_ssl.EVP_PKEY_free(pkey);
        hero_crypto_said(HERO_CRYPTO_INVALID, 0, "OpenSSL read the key as another kind than its form names");
        return false;
    }
    held->pkey = pkey;
    return true;
}

/* An `i2d_*` writer's bytes in a new blob. */
static bool hero_crypto_ssl_der(const void *object, int (*writer)(const void *, unsigned char **), hero_crypto_blob *out) {
    int count = writer(object, NULL);
    unsigned char *at;

    if (count <= 0) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL wrote no key");
        return false;
    }
    out->bytes = hero_layer_alloc((size_t)count);
    out->count = (size_t)count;
    if (out->bytes == NULL) {
        out->count = 0;
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the key");
        return false;
    }
    at = out->bytes;
    if (writer(object, &at) != count) {
        hero_crypto_blob_free(out);
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL wrote the key twice differently");
        return false;
    }
    return true;
}

static bool hero_crypto_arm_material(const hero_crypto_held *held, hero_crypto_material *m) {
    hero_crypto_blob der = { NULL, 0 };
    bool good;

    if (held->is_public) {
        good = hero_crypto_ssl_der(held->pkey, hero_crypto_ssl.i2d_PUBKEY, &der) && hero_crypto_read_spki(der.bytes, der.count, m);
    } else {
        void *info = hero_crypto_ssl.EVP_PKEY2PKCS8(held->pkey);

        good = info != NULL && hero_crypto_ssl_der(info, hero_crypto_ssl.i2d_PKCS8_PRIV_KEY_INFO, &der) && hero_crypto_read_pkcs8(der.bytes, der.count, m);
        if (info == NULL) {
            hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL gave the key no PKCS #8 form");
        } else {
            hero_crypto_ssl.PKCS8_PRIV_KEY_INFO_free(info);
        }
        if (good && (m->kind == HERO_CRYPTO_ED25519 || m->kind == HERO_CRYPTO_X25519)) {
            /* RFC 8410's PKCS #8 holds no public key: OpenSSL's SPKI does. */
            hero_crypto_blob spki = { NULL, 0 };
            hero_crypto_material pub;

            memset(&pub, 0, sizeof pub);
            good = hero_crypto_ssl_der(held->pkey, hero_crypto_ssl.i2d_PUBKEY, &spki) && hero_crypto_read_spki(spki.bytes, spki.count, &pub);
            if (good) {
                memcpy(m->point, pub.point, pub.point_count);
                m->point_count = pub.point_count;
            }
            hero_crypto_material_free(&pub);
            hero_crypto_blob_free(&spki);
        }
    }
    hero_crypto_blob_free(&der);
    return good;
}

static bool hero_crypto_arm_public(const hero_crypto_held *from, hero_crypto_held *to) {
    hero_crypto_blob der = { NULL, 0 };
    const unsigned char *at;

    if (!hero_crypto_ssl_der(from->pkey, hero_crypto_ssl.i2d_PUBKEY, &der)) {
        return false;
    }
    at = der.bytes;
    to->pkey = hero_crypto_ssl.d2i_PUBKEY(NULL, &at, (long)der.count);
    hero_crypto_blob_free(&der);
    if (to->pkey == NULL) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL read back no public key");
        return false;
    }
    return true;
}

static const void *hero_crypto_ssl_cipher(int64_t kind) {
    if (kind == HERO_CRYPTO_AES_128_GCM) {
        return hero_crypto_ssl.EVP_aes_128_gcm();
    }
    if (kind == HERO_CRYPTO_AES_256_GCM) {
        return hero_crypto_ssl.EVP_aes_256_gcm();
    }
    return hero_crypto_ssl.EVP_chacha20_poly1305();
}

/* The most bytes one `EVP_*Update` is given, so a count fits an `int`. */
#define HERO_CRYPTO_SSL_PIECE ((size_t)1 << 30)

/* `count` bytes through `update` in pieces an `int` counts, into `out`
 * (NULL for associated data). */
static bool hero_crypto_ssl_pieces(void *ctx, int (*update)(void *, unsigned char *, int *, const unsigned char *, int), unsigned char *out, const unsigned char *in, size_t count) {
    size_t done = 0;

    while (done < count) {
        size_t piece = count - done > HERO_CRYPTO_SSL_PIECE ? HERO_CRYPTO_SSL_PIECE : count - done;
        int written = 0;

        if (update(ctx, out == NULL ? NULL : out + done, &written, in + done, (int)piece) != 1 || (out != NULL && (size_t)written != piece)) {
            return false;
        }
        done += piece;
    }
    return true;
}

static bool hero_crypto_arm_seal(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *text, size_t text_count, const unsigned char *associated, size_t associated_count, unsigned char *out, unsigned char *tag) {
    void *ctx = hero_crypto_ssl.EVP_CIPHER_CTX_new();
    int last = 0;
    bool ok;

    ok = ctx != NULL && hero_crypto_ssl.EVP_EncryptInit_ex(ctx, hero_crypto_ssl_cipher(held->kind), NULL, NULL, NULL) == 1 && hero_crypto_ssl.EVP_CIPHER_CTX_ctrl(ctx, HERO_EVP_CTRL_AEAD_SET_IVLEN, (int)HERO_CRYPTO_NONCE_BYTES, NULL) == 1 && hero_crypto_ssl.EVP_EncryptInit_ex(ctx, NULL, NULL, held->secret.bytes, nonce) == 1 && hero_crypto_ssl_pieces(ctx, hero_crypto_ssl.EVP_EncryptUpdate, NULL, associated, associated_count) && hero_crypto_ssl_pieces(ctx, hero_crypto_ssl.EVP_EncryptUpdate, out, text, text_count) && hero_crypto_ssl.EVP_EncryptFinal_ex(ctx, out + text_count, &last) == 1 && last == 0 && hero_crypto_ssl.EVP_CIPHER_CTX_ctrl(ctx, HERO_EVP_CTRL_AEAD_GET_TAG, (int)HERO_CRYPTO_TAG_BYTES, tag) == 1;
    if (ctx != NULL) {
        hero_crypto_ssl.EVP_CIPHER_CTX_free(ctx);
    }
    if (!ok) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL did not seal the text");
    }
    return ok;
}

static int hero_crypto_arm_open(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *data, size_t data_count, const unsigned char *associated, size_t associated_count, const unsigned char *tag, unsigned char *out) {
    void *ctx = hero_crypto_ssl.EVP_CIPHER_CTX_new();
    unsigned char expected[16];
    int last = 0;
    bool ok;
    int answer;

    memcpy(expected, tag, sizeof expected);
    ok = ctx != NULL && hero_crypto_ssl.EVP_DecryptInit_ex(ctx, hero_crypto_ssl_cipher(held->kind), NULL, NULL, NULL) == 1 && hero_crypto_ssl.EVP_CIPHER_CTX_ctrl(ctx, HERO_EVP_CTRL_AEAD_SET_IVLEN, (int)HERO_CRYPTO_NONCE_BYTES, NULL) == 1 && hero_crypto_ssl.EVP_DecryptInit_ex(ctx, NULL, NULL, held->secret.bytes, nonce) == 1 && hero_crypto_ssl_pieces(ctx, hero_crypto_ssl.EVP_DecryptUpdate, NULL, associated, associated_count) && hero_crypto_ssl_pieces(ctx, hero_crypto_ssl.EVP_DecryptUpdate, out, data, data_count) && hero_crypto_ssl.EVP_CIPHER_CTX_ctrl(ctx, HERO_EVP_CTRL_AEAD_SET_TAG, (int)HERO_CRYPTO_TAG_BYTES, expected) == 1;
    if (!ok) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL did not open the text");
        answer = -1;
    } else {
        /* The tag is checked here, and only here: a failure is a forgery. */
        answer = hero_crypto_ssl.EVP_DecryptFinal_ex(ctx, out + data_count, &last) == 1 && last == 0 ? 1 : 0;
        hero_crypto_ssl.ERR_clear_error();
    }
    if (ctx != NULL) {
        hero_crypto_ssl.EVP_CIPHER_CTX_free(ctx);
    }
    return answer;
}

/* The digest's name a scheme hashes with, NULL for Ed25519's none. */
static const char *hero_crypto_ssl_digest(int64_t scheme) {
    switch (scheme) {
    case HERO_CRYPTO_SIGN_ED25519:
        return NULL;
    case HERO_CRYPTO_SIGN_ECDSA_SHA384:
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA384:
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA384:
        return "SHA384";
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA512:
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512:
        return "SHA512";
    default:
        return "SHA256";
    }
}

/* The RSA padding of a scheme on the context `pkey_ctx`. */
static bool hero_crypto_ssl_padding(void *pkey_ctx, int64_t scheme) {
    if (scheme == HERO_CRYPTO_SIGN_RSA_PSS_SHA256 || scheme == HERO_CRYPTO_SIGN_RSA_PSS_SHA384 || scheme == HERO_CRYPTO_SIGN_RSA_PSS_SHA512) {
        return hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, HERO_RSA_PKCS1_PSS_PADDING) == 1 && hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_pss_saltlen(pkey_ctx, HERO_RSA_PSS_SALTLEN_DIGEST) == 1;
    }
    if (scheme == HERO_CRYPTO_SIGN_RSA_PKCS1_SHA256 || scheme == HERO_CRYPTO_SIGN_RSA_PKCS1_SHA384 || scheme == HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512) {
        return hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, HERO_RSA_PKCS1_PADDING) == 1;
    }
    return true;
}

static int64_t hero_crypto_arm_sign(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, unsigned char *out, size_t most) {
    void *ctx = hero_crypto_ssl.EVP_MD_CTX_new();
    void *pkey_ctx = NULL;
    size_t length = most;
    static const unsigned char none[1] = { 0 };
    bool ok;

    ok = ctx != NULL && hero_crypto_ssl.EVP_DigestSignInit_ex(ctx, &pkey_ctx, hero_crypto_ssl_digest(scheme), NULL, NULL, held->pkey, NULL) == 1 && hero_crypto_ssl_padding(pkey_ctx, scheme) && hero_crypto_ssl.EVP_DigestSign(ctx, out, &length, count == 0 ? none : message, count) == 1;
    if (ctx != NULL) {
        hero_crypto_ssl.EVP_MD_CTX_free(ctx);
    }
    if (!ok || length > most) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL did not sign the message");
        return -1;
    }
    return (int64_t)length;
}

static int hero_crypto_arm_verify(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, const unsigned char *signature, size_t signature_count) {
    void *ctx = hero_crypto_ssl.EVP_MD_CTX_new();
    void *pkey_ctx = NULL;
    static const unsigned char none[1] = { 0 };
    int answer;

    if (ctx == NULL || hero_crypto_ssl.EVP_DigestVerifyInit_ex(ctx, &pkey_ctx, hero_crypto_ssl_digest(scheme), NULL, NULL, held->pkey, NULL) != 1 || !hero_crypto_ssl_padding(pkey_ctx, scheme)) {
        if (ctx != NULL) {
            hero_crypto_ssl.EVP_MD_CTX_free(ctx);
        }
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL could not check the signature");
        return -1;
    }
    /* Anything but 1 is a signature that does not hold, bytes of no
     * signature's form among them. */
    answer = hero_crypto_ssl.EVP_DigestVerify(ctx, signature, signature_count, count == 0 ? none : message, count) == 1 ? 1 : 0;
    hero_crypto_ssl.ERR_clear_error();
    hero_crypto_ssl.EVP_MD_CTX_free(ctx);
    return answer;
}

static int64_t hero_crypto_arm_agree(const hero_crypto_held *mine, const hero_crypto_held *theirs, unsigned char *out, size_t width) {
    void *ctx = hero_crypto_ssl.EVP_PKEY_CTX_new_from_pkey(NULL, mine->pkey, NULL);
    size_t length = width;
    bool ok;

    ok = ctx != NULL && hero_crypto_ssl.EVP_PKEY_derive_init(ctx) == 1 && hero_crypto_ssl.EVP_PKEY_derive_set_peer(ctx, theirs->pkey) == 1 && hero_crypto_ssl.EVP_PKEY_derive(ctx, out, &length) == 1 && length == width;
    if (ctx != NULL) {
        hero_crypto_ssl.EVP_PKEY_CTX_free(ctx);
    }
    if (!ok) {
        hero_crypto_ssl_said(HERO_CRYPTO_INVALID, "OpenSSL agreed no secret");
        return -1;
    }
    return (int64_t)width;
}

/* An OAEP context of `pkey`, made for encryption or decryption. */
static void *hero_crypto_ssl_oaep(void *pkey, bool encrypting) {
    void *ctx = hero_crypto_ssl.EVP_PKEY_CTX_new_from_pkey(NULL, pkey, NULL);
    bool ok = ctx != NULL && (encrypting ? hero_crypto_ssl.EVP_PKEY_encrypt_init(ctx) : hero_crypto_ssl.EVP_PKEY_decrypt_init(ctx)) == 1 && hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_padding(ctx, HERO_RSA_PKCS1_OAEP_PADDING) == 1 && hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_oaep_md_name(ctx, "SHA256", NULL) == 1 && hero_crypto_ssl.EVP_PKEY_CTX_set_rsa_mgf1_md_name(ctx, "SHA256", NULL) == 1;

    if (!ok && ctx != NULL) {
        hero_crypto_ssl.EVP_PKEY_CTX_free(ctx);
        ctx = NULL;
    }
    return ctx;
}

static int64_t hero_crypto_arm_encrypt(const hero_crypto_held *held, const unsigned char *text, size_t count, unsigned char *out, size_t most) {
    void *ctx = hero_crypto_ssl_oaep(held->pkey, true);
    size_t length = most;
    static const unsigned char none[1] = { 0 };
    bool ok = ctx != NULL && hero_crypto_ssl.EVP_PKEY_encrypt(ctx, out, &length, count == 0 ? none : text, count) == 1;

    if (ctx != NULL) {
        hero_crypto_ssl.EVP_PKEY_CTX_free(ctx);
    }
    if (!ok) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL did not encrypt the text");
        return -1;
    }
    return (int64_t)length;
}

static int64_t hero_crypto_arm_decrypt(const hero_crypto_held *held, const unsigned char *data, size_t count, unsigned char *out, size_t most) {
    void *ctx = hero_crypto_ssl_oaep(held->pkey, false);
    unsigned char *room;
    size_t room_count = hero_crypto_modulus_bytes(held);
    size_t length = room_count;
    int64_t answer;

    if (ctx == NULL) {
        hero_crypto_ssl_said(HERO_CRYPTO_OTHER, "OpenSSL made no OAEP context");
        return -1;
    }
    /* OpenSSL asks room for a whole modulus before it knows the text's
     * length; the text is copied out of it and the room wiped. */
    room = hero_layer_alloc(room_count);
    if (room == NULL) {
        hero_crypto_ssl.EVP_PKEY_CTX_free(ctx);
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left to decrypt");
        return -1;
    }
    if (hero_crypto_ssl.EVP_PKEY_decrypt(ctx, room, &length, data, count) != 1 || length > most) {
        hero_crypto_ssl.ERR_clear_error();
        answer = -2;
    } else {
        memcpy(out, room, length);
        answer = (int64_t)length;
    }
    hero_crypto_wipe(room, room_count);
    hero_layer_free(room);
    hero_crypto_ssl.EVP_PKEY_CTX_free(ctx);
    return answer;
}

static void hero_crypto_arm_free(hero_crypto_held *held) {
    if (held->pkey != NULL) {
        /* `EVP_PKEY_free` clears a private key's numbers as it frees them. */
        hero_crypto_ssl.EVP_PKEY_free(held->pkey);
        held->pkey = NULL;
    }
}

#endif

/* ===========================================================================
 * THE WINDOWS ARM: CNG's BCrypt, written from the Win32 reference and first
 * compiled against MinGW-w64's headers on the Mac, the box being off on
 * 2026-11-17. Run on the box on 2026-11-18 at 1e0a4f82b (Windows 10.0.26100,
 * clang 23.1.1 for the MSVC target): it compiles with no word under C11 and
 * GNU11 `-Wpedantic -Werror`, the SDK's `BCRYPT_CHACHA20_POLY1305_ALGORITHM`
 * and the `#ifndef` below agreeing; CNG opens CHACHA20_POLY1305; the C probe
 * of this member held every Wycheproof and CAVP case of a kind it offers, 0
 * failed, plain and under ASan, with Linux arm64's counts but one (RSA PKCS
 * #1 v1.5's one acceptable case held, which Linux refused, either way
 * acceptable); `reach_hero_crypto`, `reach_crypto_seal` and
 * `reach_crypto_keypairs` passed, plain and under `--sanitize`. CNG refused
 * RFC 7748's Alice as a `BCRYPT_ECCPRIVATE_BLOB` on curve25519 in both byte
 * orders of the scalar (`STATUS_INVALID_PARAMETER`), so X25519 answers
 * `unsupported` here until the blob CNG takes is measured. Owed on the box:
 * that blob, RSA 4096's generation time, and a key written here read back on
 * macOS and Linux.
 *
 * A pair's key is its material (THE FORMS), from which each call makes the
 * blob CNG imports for the algorithm it runs (`BCRYPT_ECCPRIVATE_BLOB` for
 * ECDSA under one magic and for ECDH under another, `BCRYPT_RSAPRIVATE_BLOB`
 * of the public exponent, the modulus and the two primes), and destroys
 * after it. A secret key is its bytes, made a CNG key each call. CNG signs a
 * hash, which `BCryptHash` makes, and writes ECDSA's two numbers side by
 * side, which this arm puts in X9.62's DER and takes out of it. Ed25519 has
 * no CNG algorithm; X25519 waits for the box (the opening's note 4).
 * ========================================================================== */
#if defined(_WIN32)

#define HERO_STATUS_INVALID_SIGNATURE ((NTSTATUS)0xC000A000L)
#define HERO_STATUS_AUTH_TAG_MISMATCH ((NTSTATUS)0xC000A002L)

static bool hero_crypto_win_good(NTSTATUS status) {
    return status >= 0;
}

static void hero_crypto_win_said(int64_t kind, const char *what, NTSTATUS status) {
    char words[160];

    (void)snprintf(words, sizeof words, "%s answered 0x%08lX", what, (unsigned long)status);
    hero_crypto_said(kind, (int64_t)(uint32_t)status, words);
}

static bool hero_crypto_arm_ready(void) {
    return true;
}

static bool hero_crypto_arm_offers(int64_t what) {
    return what != HERO_CRYPTO_ED25519 && what != HERO_CRYPTO_SIGN_ED25519 && what != HERO_CRYPTO_X25519;
}

static bool hero_crypto_arm_random(unsigned char *into, size_t count) {
    NTSTATUS status = BCryptGenRandom(NULL, into, (ULONG)count, BCRYPT_USE_SYSTEM_PREFERRED_RNG);

    if (!hero_crypto_win_good(status)) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptGenRandom", status);
        return false;
    }
    return true;
}

/* The provider of an algorithm, opened; NULL, said, where it fails. */
static BCRYPT_ALG_HANDLE hero_crypto_win_open(LPCWSTR algorithm) {
    BCRYPT_ALG_HANDLE handle = NULL;
    NTSTATUS status = BCryptOpenAlgorithmProvider(&handle, algorithm, NULL, 0);

    if (!hero_crypto_win_good(status)) {
        hero_crypto_win_said(HERO_CRYPTO_UNSUPPORTED, "BCryptOpenAlgorithmProvider", status);
        return NULL;
    }
    return handle;
}

/* The provider for an EC key of `kind`, for signing or for agreeing. */
static LPCWSTR hero_crypto_win_ec_algorithm(int64_t kind, bool agreeing) {
    if (kind == HERO_CRYPTO_P256) {
        return agreeing ? BCRYPT_ECDH_P256_ALGORITHM : BCRYPT_ECDSA_P256_ALGORITHM;
    }
    return agreeing ? BCRYPT_ECDH_P384_ALGORITHM : BCRYPT_ECDSA_P384_ALGORITHM;
}

/* The magic of an EC blob. */
static ULONG hero_crypto_win_ec_magic(int64_t kind, bool agreeing, bool is_private) {
    if (kind == HERO_CRYPTO_P256) {
        if (agreeing) {
            return is_private ? BCRYPT_ECDH_PRIVATE_P256_MAGIC : BCRYPT_ECDH_PUBLIC_P256_MAGIC;
        }
        return is_private ? BCRYPT_ECDSA_PRIVATE_P256_MAGIC : BCRYPT_ECDSA_PUBLIC_P256_MAGIC;
    }
    if (agreeing) {
        return is_private ? BCRYPT_ECDH_PRIVATE_P384_MAGIC : BCRYPT_ECDH_PUBLIC_P384_MAGIC;
    }
    return is_private ? BCRYPT_ECDSA_PRIVATE_P384_MAGIC : BCRYPT_ECDSA_PUBLIC_P384_MAGIC;
}

/* A CNG key of `held`'s pair, imported into `alg`: for ECDSA or RSA where
 * not `agreeing`, for ECDH where it is; its private half where `held` is
 * private and `private_wanted`. NULL, said, where it fails. */
static BCRYPT_KEY_HANDLE hero_crypto_win_key(BCRYPT_ALG_HANDLE alg, const hero_crypto_held *held, bool agreeing, bool private_wanted) {
    const hero_crypto_material *m = &held->material;
    bool is_private = private_wanted && m->is_private;
    hero_crypto_blob blob = { NULL, 0 };
    BCRYPT_KEY_HANDLE key = NULL;
    LPCWSTR type;
    NTSTATUS status;

    if (m->kind == HERO_CRYPTO_RSA) {
        hero_crypto_span fields[9];
        hero_crypto_span whole = { m->rsa.bytes, m->rsa.count };
        hero_crypto_span e;
        hero_crypto_span n;
        BCRYPT_RSAKEY_BLOB head;

        if (!hero_crypto_rsa_fields(whole, m->is_private, fields)) {
            hero_crypto_said(HERO_CRYPTO_OTHER, 0, "the RSA key kept is not PKCS #1");
            return NULL;
        }
        n = m->is_private ? fields[1] : fields[0];
        e = m->is_private ? fields[2] : fields[1];
        memset(&head, 0, sizeof head);
        head.Magic = is_private ? BCRYPT_RSAPRIVATE_MAGIC : BCRYPT_RSAPUBLIC_MAGIC;
        head.BitLength = (ULONG)m->bits;
        head.cbPublicExp = (ULONG)e.n;
        head.cbModulus = (ULONG)n.n;
        if (is_private) {
            head.cbPrime1 = (ULONG)fields[4].n;
            head.cbPrime2 = (ULONG)fields[5].n;
        }
        {
            const unsigned char *parts[] = { (const unsigned char *)&head, e.p, n.p, fields[4].p, fields[5].p };
            const size_t counts[] = { sizeof head, e.n, n.n, is_private ? fields[4].n : 0, is_private ? fields[5].n : 0 };

            if (!hero_crypto_join(&blob, parts, counts, 5)) {
                return NULL;
            }
        }
        type = is_private ? BCRYPT_RSAPRIVATE_BLOB : BCRYPT_RSAPUBLIC_BLOB;
    } else {
        BCRYPT_ECCKEY_BLOB head;
        const unsigned char *parts[3];
        size_t counts[3];

        head.dwMagic = hero_crypto_win_ec_magic(m->kind, agreeing, is_private);
        head.cbKey = (ULONG)m->width;
        parts[0] = (const unsigned char *)&head;
        counts[0] = sizeof head;
        parts[1] = m->point + 1;
        counts[1] = 2 * m->width;
        parts[2] = m->scalar;
        counts[2] = is_private ? m->width : 0;
        if (!hero_crypto_join(&blob, parts, counts, 3)) {
            return NULL;
        }
        type = is_private ? BCRYPT_ECCPRIVATE_BLOB : BCRYPT_ECCPUBLIC_BLOB;
    }
    status = BCryptImportKeyPair(alg, NULL, type, &key, blob.bytes, (ULONG)blob.count, 0);
    hero_crypto_blob_free(&blob);
    if (!hero_crypto_win_good(status)) {
        hero_crypto_win_said(HERO_CRYPTO_INVALID, "BCryptImportKeyPair", status);
        return NULL;
    }
    return key;
}

static bool hero_crypto_arm_load(hero_crypto_held *held, const hero_crypto_material *m, const unsigned char *der, size_t der_count) {
    BCRYPT_ALG_HANDLE alg;
    BCRYPT_KEY_HANDLE key;
    const unsigned char *parts[1];
    size_t counts[1];

    (void)der;
    (void)der_count;
    if (m->kind != HERO_CRYPTO_RSA && m->point_count == 0) {
        hero_crypto_said(HERO_CRYPTO_UNSUPPORTED, 0, "the EC key holds no public point, which CNG needs and does not compute");
        return false;
    }
    held->material = *m;
    held->material.rsa.bytes = NULL;
    held->material.rsa.count = 0;
    if (m->kind == HERO_CRYPTO_RSA) {
        parts[0] = m->rsa.bytes;
        counts[0] = m->rsa.count;
        if (!hero_crypto_join(&held->material.rsa, parts, counts, 1)) {
            return false;
        }
    }
    /* Imported once now, so a key CNG refuses is refused at its reading. */
    alg = hero_crypto_win_open(m->kind == HERO_CRYPTO_RSA ? BCRYPT_RSA_ALGORITHM : hero_crypto_win_ec_algorithm(m->kind, false));
    if (alg == NULL) {
        return false;
    }
    key = hero_crypto_win_key(alg, held, false, true);
    if (key != NULL) {
        (void)BCryptDestroyKey(key);
    }
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    return key != NULL;
}

/* A blob CNG exports of `key`, in a new blob. */
static bool hero_crypto_win_export(BCRYPT_KEY_HANDLE key, LPCWSTR type, hero_crypto_blob *out) {
    ULONG size = 0;
    NTSTATUS status = BCryptExportKey(key, NULL, type, NULL, 0, &size, 0);

    if (!hero_crypto_win_good(status) || size == 0) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptExportKey", status);
        return false;
    }
    out->bytes = hero_layer_alloc(size);
    out->count = size;
    if (out->bytes == NULL) {
        out->count = 0;
        hero_crypto_said(HERO_CRYPTO_LIMIT, 0, "no memory left for the key");
        return false;
    }
    status = BCryptExportKey(key, NULL, type, out->bytes, size, &size, 0);
    if (!hero_crypto_win_good(status) || size != out->count) {
        hero_crypto_blob_free(out);
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptExportKey", status);
        return false;
    }
    return true;
}

static bool hero_crypto_arm_generate(hero_crypto_held *held, int64_t bits) {
    bool rsa = held->kind == HERO_CRYPTO_RSA;
    BCRYPT_ALG_HANDLE alg = hero_crypto_win_open(rsa ? BCRYPT_RSA_ALGORITHM : hero_crypto_win_ec_algorithm(held->kind, false));
    BCRYPT_KEY_HANDLE key = NULL;
    hero_crypto_blob blob = { NULL, 0 };
    hero_crypto_material *m = &held->material;
    ULONG size = rsa ? (ULONG)bits : held->kind == HERO_CRYPTO_P256 ? 256 : 384;
    NTSTATUS status;
    bool good;

    if (alg == NULL) {
        return false;
    }
    status = BCryptGenerateKeyPair(alg, &key, size, 0);
    if (hero_crypto_win_good(status)) {
        status = BCryptFinalizeKeyPair(key, 0);
    }
    good = hero_crypto_win_good(status);
    if (!good) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptGenerateKeyPair", status);
    }
    good = good && hero_crypto_win_export(key, rsa ? BCRYPT_RSAFULLPRIVATE_BLOB : BCRYPT_ECCPRIVATE_BLOB, &blob);
    memset(m, 0, sizeof *m);
    m->kind = held->kind;
    m->is_private = true;
    m->bits = (int64_t)size;
    if (good && rsa) {
        /* BCRYPT_RSAKEY_BLOB, then e, n, p, q, dp, dq, qinv and d. */
        BCRYPT_RSAKEY_BLOB head;
        hero_crypto_span numbers[8];
        size_t at = sizeof head;

        memcpy(&head, blob.bytes, sizeof head);
        {
            const size_t sizes[8] = { head.cbPublicExp, head.cbModulus, head.cbPrime1, head.cbPrime2, head.cbPrime1, head.cbPrime2, head.cbPrime1, head.cbModulus };

            for (size_t i = 0; i < 8 && good; i += 1) {
                if (sizes[i] > blob.count - at) {
                    good = false;
                } else {
                    numbers[i].p = blob.bytes + at;
                    numbers[i].n = sizes[i];
                    at += sizes[i];
                }
            }
        }
        if (good) {
            /* PKCS #1's order: n, e, d, p, q, dp, dq, qinv. */
            hero_crypto_span ordered[8] = { numbers[1], numbers[0], numbers[7], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6] };

            good = hero_crypto_rsa_write(ordered, 8, true, &m->rsa);
        } else {
            hero_crypto_said(HERO_CRYPTO_OTHER, 0, "CNG exported an RSA blob shorter than its head says");
        }
    } else if (good) {
        BCRYPT_ECCKEY_BLOB head;

        memcpy(&head, blob.bytes, sizeof head);
        m->width = held->kind == HERO_CRYPTO_P256 ? 32 : 48;
        if (head.cbKey != m->width || blob.count != sizeof head + 3 * m->width) {
            hero_crypto_said(HERO_CRYPTO_OTHER, 0, "CNG exported an EC blob of another size");
            good = false;
        } else {
            m->point[0] = 0x04;
            memcpy(m->point + 1, blob.bytes + sizeof head, 2 * m->width);
            m->point_count = 1 + 2 * m->width;
            memcpy(m->scalar, blob.bytes + sizeof head + 2 * m->width, m->width);
        }
    }
    hero_crypto_blob_free(&blob);
    if (key != NULL) {
        (void)BCryptDestroyKey(key);
    }
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (good) {
        held->bits = m->bits;
    }
    return good;
}

/* A copy of `from`'s material, its private half dropped where `public_only`. */
static bool hero_crypto_win_copy(const hero_crypto_material *from, hero_crypto_material *to, bool public_only) {
    const unsigned char *parts[1];
    size_t counts[1];

    *to = *from;
    to->rsa.bytes = NULL;
    to->rsa.count = 0;
    if (public_only) {
        to->is_private = false;
        hero_crypto_wipe(to->scalar, sizeof to->scalar);
    }
    if (from->kind != HERO_CRYPTO_RSA) {
        return true;
    }
    if (public_only && from->is_private) {
        return hero_crypto_rsa_public_of(&from->rsa, &to->rsa);
    }
    parts[0] = from->rsa.bytes;
    counts[0] = from->rsa.count;
    return hero_crypto_join(&to->rsa, parts, counts, 1);
}

static bool hero_crypto_arm_material(const hero_crypto_held *held, hero_crypto_material *m) {
    return hero_crypto_win_copy(&held->material, m, held->is_public);
}

static bool hero_crypto_arm_public(const hero_crypto_held *from, hero_crypto_held *to) {
    return hero_crypto_win_copy(&from->material, &to->material, true);
}

/* A CNG key of a secret key's bytes, its provider into `*alg`. */
static BCRYPT_KEY_HANDLE hero_crypto_win_secret(const hero_crypto_held *held, BCRYPT_ALG_HANDLE *alg) {
    BCRYPT_KEY_HANDLE key = NULL;
    NTSTATUS status;
    bool aes = held->kind != HERO_CRYPTO_CHACHA20_POLY1305;

    *alg = hero_crypto_win_open(aes ? BCRYPT_AES_ALGORITHM : BCRYPT_CHACHA20_POLY1305_ALGORITHM);
    if (*alg == NULL) {
        return NULL;
    }
    if (aes) {
        status = BCryptSetProperty(*alg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, (ULONG)sizeof BCRYPT_CHAIN_MODE_GCM, 0);
        if (!hero_crypto_win_good(status)) {
            hero_crypto_win_said(HERO_CRYPTO_UNSUPPORTED, "BCryptSetProperty(GCM)", status);
            (void)BCryptCloseAlgorithmProvider(*alg, 0);
            *alg = NULL;
            return NULL;
        }
    }
    status = BCryptGenerateSymmetricKey(*alg, &key, NULL, 0, held->secret.bytes, (ULONG)held->secret.count, 0);
    if (!hero_crypto_win_good(status)) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptGenerateSymmetricKey", status);
        (void)BCryptCloseAlgorithmProvider(*alg, 0);
        *alg = NULL;
        return NULL;
    }
    return key;
}

static bool hero_crypto_arm_seal(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *text, size_t text_count, const unsigned char *associated, size_t associated_count, unsigned char *out, unsigned char *tag) {
    BCRYPT_ALG_HANDLE alg;
    BCRYPT_KEY_HANDLE key = hero_crypto_win_secret(held, &alg);
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO info;
    ULONG done = 0;
    NTSTATUS status;

    if (key == NULL) {
        return false;
    }
    BCRYPT_INIT_AUTH_MODE_INFO(info);
    info.pbNonce = (PUCHAR)nonce;
    info.cbNonce = (ULONG)HERO_CRYPTO_NONCE_BYTES;
    info.pbAuthData = (PUCHAR)associated;
    info.cbAuthData = (ULONG)associated_count;
    info.pbTag = tag;
    info.cbTag = (ULONG)HERO_CRYPTO_TAG_BYTES;
    status = BCryptEncrypt(key, (PUCHAR)text, (ULONG)text_count, &info, NULL, 0, out, (ULONG)text_count, &done, 0);
    (void)BCryptDestroyKey(key);
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (!hero_crypto_win_good(status) || done != text_count) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptEncrypt", status);
        return false;
    }
    return true;
}

static int hero_crypto_arm_open(const hero_crypto_held *held, const unsigned char *nonce, const unsigned char *data, size_t data_count, const unsigned char *associated, size_t associated_count, const unsigned char *tag, unsigned char *out) {
    BCRYPT_ALG_HANDLE alg;
    BCRYPT_KEY_HANDLE key = hero_crypto_win_secret(held, &alg);
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO info;
    unsigned char expected[16];
    ULONG done = 0;
    NTSTATUS status;

    if (key == NULL) {
        return -1;
    }
    memcpy(expected, tag, sizeof expected);
    BCRYPT_INIT_AUTH_MODE_INFO(info);
    info.pbNonce = (PUCHAR)nonce;
    info.cbNonce = (ULONG)HERO_CRYPTO_NONCE_BYTES;
    info.pbAuthData = (PUCHAR)associated;
    info.cbAuthData = (ULONG)associated_count;
    info.pbTag = expected;
    info.cbTag = (ULONG)HERO_CRYPTO_TAG_BYTES;
    status = BCryptDecrypt(key, (PUCHAR)data, (ULONG)data_count, &info, NULL, 0, out, (ULONG)data_count, &done, 0);
    (void)BCryptDestroyKey(key);
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (status == HERO_STATUS_AUTH_TAG_MISMATCH) {
        return 0;
    }
    if (!hero_crypto_win_good(status) || done != data_count) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptDecrypt", status);
        return -1;
    }
    return 1;
}

/* The hash a scheme signs, CNG's name for it, and its bytes. */
static LPCWSTR hero_crypto_win_hash_name(int64_t scheme, size_t *bytes) {
    switch (scheme) {
    case HERO_CRYPTO_SIGN_ECDSA_SHA384:
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA384:
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA384:
        *bytes = 48;
        return BCRYPT_SHA384_ALGORITHM;
    case HERO_CRYPTO_SIGN_RSA_PSS_SHA512:
    case HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512:
        *bytes = 64;
        return BCRYPT_SHA512_ALGORITHM;
    default:
        *bytes = 32;
        return BCRYPT_SHA256_ALGORITHM;
    }
}

/* The hash of `message` by the scheme's algorithm into `out` (64 bytes);
 * its length or 0, said. */
static size_t hero_crypto_win_hash(int64_t scheme, const unsigned char *message, size_t count, unsigned char *out) {
    size_t bytes;
    LPCWSTR name = hero_crypto_win_hash_name(scheme, &bytes);
    BCRYPT_ALG_HANDLE alg = hero_crypto_win_open(name);
    NTSTATUS status;

    if (alg == NULL) {
        return 0;
    }
    status = BCryptHash(alg, NULL, 0, (PUCHAR)message, (ULONG)count, out, (ULONG)bytes);
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (!hero_crypto_win_good(status)) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptHash", status);
        return 0;
    }
    return bytes;
}

/* ECDSA's two numbers, `width` bytes each side by side, written as X9.62's
 * DER `SEQUENCE { r INTEGER, s INTEGER }` into `out`; its length. */
static size_t hero_crypto_win_rs_to_der(const unsigned char *rs, size_t width, unsigned char *out) {
    unsigned char heads[2][7];
    size_t head_counts[2];
    hero_crypto_span numbers[2];
    size_t body = 0;
    size_t at;

    for (size_t i = 0; i < 2; i += 1) {
        numbers[i] = hero_crypto_trimmed(rs + i * width, width);
        head_counts[i] = hero_crypto_integer_head(heads[i], numbers[i]);
        body += head_counts[i] + numbers[i].n;
    }
    at = hero_crypto_der_head(out, 0x30, body);
    for (size_t i = 0; i < 2; i += 1) {
        memcpy(out + at, heads[i], head_counts[i]);
        at += head_counts[i];
        memcpy(out + at, numbers[i].p, numbers[i].n);
        at += numbers[i].n;
    }
    return at;
}

/* The padding information of an RSA scheme and its flag. */
typedef struct hero_crypto_win_padding {
    BCRYPT_PSS_PADDING_INFO pss;
    BCRYPT_PKCS1_PADDING_INFO pkcs1;
    void *info;
    ULONG flags;
} hero_crypto_win_padding;

static void hero_crypto_win_pad(int64_t scheme, hero_crypto_win_padding *p) {
    size_t bytes;
    LPCWSTR name = hero_crypto_win_hash_name(scheme, &bytes);

    memset(p, 0, sizeof *p);
    if (scheme == HERO_CRYPTO_SIGN_RSA_PSS_SHA256 || scheme == HERO_CRYPTO_SIGN_RSA_PSS_SHA384 || scheme == HERO_CRYPTO_SIGN_RSA_PSS_SHA512) {
        p->pss.pszAlgId = name;
        p->pss.cbSalt = (ULONG)bytes;
        p->info = &p->pss;
        p->flags = BCRYPT_PAD_PSS;
    } else if (scheme == HERO_CRYPTO_SIGN_RSA_PKCS1_SHA256 || scheme == HERO_CRYPTO_SIGN_RSA_PKCS1_SHA384 || scheme == HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512) {
        p->pkcs1.pszAlgId = name;
        p->info = &p->pkcs1;
        p->flags = BCRYPT_PAD_PKCS1;
    }
}

static int64_t hero_crypto_arm_sign(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, unsigned char *out, size_t most) {
    bool rsa = held->kind == HERO_CRYPTO_RSA;
    BCRYPT_ALG_HANDLE alg = hero_crypto_win_open(rsa ? BCRYPT_RSA_ALGORITHM : hero_crypto_win_ec_algorithm(held->kind, false));
    BCRYPT_KEY_HANDLE key;
    unsigned char hash[64];
    unsigned char rs[2 * HERO_CRYPTO_WIDTH_MOST];
    size_t hash_count;
    hero_crypto_win_padding padding;
    ULONG done = 0;
    NTSTATUS status;

    if (alg == NULL) {
        return -1;
    }
    key = hero_crypto_win_key(alg, held, false, true);
    hash_count = key == NULL ? 0 : hero_crypto_win_hash(scheme, message, count, hash);
    if (hash_count == 0) {
        if (key != NULL) {
            (void)BCryptDestroyKey(key);
        }
        (void)BCryptCloseAlgorithmProvider(alg, 0);
        return -1;
    }
    hero_crypto_win_pad(scheme, &padding);
    if (rsa) {
        status = BCryptSignHash(key, padding.info, hash, (ULONG)hash_count, out, (ULONG)most, &done, padding.flags);
    } else {
        status = BCryptSignHash(key, NULL, hash, (ULONG)hash_count, rs, (ULONG)(2 * held->material.width), &done, 0);
    }
    (void)BCryptDestroyKey(key);
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (!hero_crypto_win_good(status)) {
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptSignHash", status);
        return -1;
    }
    if (rsa) {
        return (int64_t)done;
    }
    if (done != 2 * held->material.width) {
        hero_crypto_said(HERO_CRYPTO_OTHER, 0, "CNG answered an ECDSA signature of another length");
        return -1;
    }
    return (int64_t)hero_crypto_win_rs_to_der(rs, held->material.width, out);
}

static int hero_crypto_arm_verify(const hero_crypto_held *held, int64_t scheme, const unsigned char *message, size_t count, const unsigned char *signature, size_t signature_count) {
    bool rsa = held->kind == HERO_CRYPTO_RSA;
    BCRYPT_ALG_HANDLE alg;
    BCRYPT_KEY_HANDLE key;
    unsigned char hash[64];
    unsigned char rs[2 * HERO_CRYPTO_WIDTH_MOST];
    size_t hash_count;
    hero_crypto_win_padding padding;
    NTSTATUS status;

    if (!rsa && !hero_crypto_der_to_rs(signature, signature_count, held->material.width, rs)) {
        return 0;
    }
    alg = hero_crypto_win_open(rsa ? BCRYPT_RSA_ALGORITHM : hero_crypto_win_ec_algorithm(held->kind, false));
    if (alg == NULL) {
        return -1;
    }
    key = hero_crypto_win_key(alg, held, false, false);
    hash_count = key == NULL ? 0 : hero_crypto_win_hash(scheme, message, count, hash);
    if (hash_count == 0) {
        if (key != NULL) {
            (void)BCryptDestroyKey(key);
        }
        (void)BCryptCloseAlgorithmProvider(alg, 0);
        return -1;
    }
    hero_crypto_win_pad(scheme, &padding);
    if (rsa) {
        status = BCryptVerifySignature(key, padding.info, hash, (ULONG)hash_count, (PUCHAR)signature, (ULONG)signature_count, padding.flags);
    } else {
        status = BCryptVerifySignature(key, NULL, hash, (ULONG)hash_count, rs, (ULONG)(2 * held->material.width), 0);
    }
    (void)BCryptDestroyKey(key);
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (hero_crypto_win_good(status)) {
        return 1;
    }
    if (status == HERO_STATUS_INVALID_SIGNATURE || rsa) {
        /* A signature of the wrong length is refused by CNG with another
         * status than an invalid one: it does not hold either way. */
        return 0;
    }
    hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptVerifySignature", status);
    return -1;
}

static int64_t hero_crypto_arm_agree(const hero_crypto_held *mine, const hero_crypto_held *theirs, unsigned char *out, size_t width) {
    BCRYPT_ALG_HANDLE alg = hero_crypto_win_open(hero_crypto_win_ec_algorithm(mine->kind, true));
    BCRYPT_KEY_HANDLE mine_key;
    BCRYPT_KEY_HANDLE theirs_key;
    BCRYPT_SECRET_HANDLE secret = NULL;
    ULONG done = 0;
    NTSTATUS status;
    int64_t answer = -1;

    if (alg == NULL) {
        return -1;
    }
    mine_key = hero_crypto_win_key(alg, mine, true, true);
    theirs_key = mine_key == NULL ? NULL : hero_crypto_win_key(alg, theirs, true, false);
    if (theirs_key != NULL) {
        status = BCryptSecretAgreement(mine_key, theirs_key, &secret, 0);
        if (hero_crypto_win_good(status)) {
            status = BCryptDeriveKey(secret, BCRYPT_KDF_RAW_SECRET, NULL, out, (ULONG)width, &done, 0);
        }
        if (!hero_crypto_win_good(status) || done != width) {
            hero_crypto_win_said(HERO_CRYPTO_INVALID, "BCryptSecretAgreement", status);
        } else {
            /* CNG gives the raw secret little-endian (BCryptDeriveKey's
             * page); SEC 1 writes it big-endian. */
            for (size_t i = 0; i < width / 2; i += 1) {
                unsigned char b = out[i];

                out[i] = out[width - 1 - i];
                out[width - 1 - i] = b;
            }
            answer = (int64_t)width;
        }
    }
    if (secret != NULL) {
        (void)BCryptDestroySecret(secret);
    }
    if (theirs_key != NULL) {
        (void)BCryptDestroyKey(theirs_key);
    }
    if (mine_key != NULL) {
        (void)BCryptDestroyKey(mine_key);
    }
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    return answer;
}

/* RSA-OAEP through CNG, one way or the other; -2 for data that do not
 * decrypt. */
static int64_t hero_crypto_win_oaep(const hero_crypto_held *held, bool encrypting, const unsigned char *in, size_t count, unsigned char *out, size_t most) {
    BCRYPT_ALG_HANDLE alg = hero_crypto_win_open(BCRYPT_RSA_ALGORITHM);
    BCRYPT_KEY_HANDLE key;
    BCRYPT_OAEP_PADDING_INFO padding;
    ULONG done = 0;
    NTSTATUS status;

    if (alg == NULL) {
        return -1;
    }
    key = hero_crypto_win_key(alg, held, false, !encrypting);
    if (key == NULL) {
        (void)BCryptCloseAlgorithmProvider(alg, 0);
        return -1;
    }
    padding.pszAlgId = BCRYPT_SHA256_ALGORITHM;
    padding.pbLabel = NULL;
    padding.cbLabel = 0;
    if (encrypting) {
        status = BCryptEncrypt(key, (PUCHAR)in, (ULONG)count, &padding, NULL, 0, out, (ULONG)most, &done, BCRYPT_PAD_OAEP);
    } else {
        status = BCryptDecrypt(key, (PUCHAR)in, (ULONG)count, &padding, NULL, 0, out, (ULONG)most, &done, BCRYPT_PAD_OAEP);
    }
    (void)BCryptDestroyKey(key);
    (void)BCryptCloseAlgorithmProvider(alg, 0);
    if (!hero_crypto_win_good(status)) {
        if (!encrypting) {
            return -2;
        }
        hero_crypto_win_said(HERO_CRYPTO_OTHER, "BCryptEncrypt(OAEP)", status);
        return -1;
    }
    return (int64_t)done;
}

static int64_t hero_crypto_arm_encrypt(const hero_crypto_held *held, const unsigned char *text, size_t count, unsigned char *out, size_t most) {
    return hero_crypto_win_oaep(held, true, text, count, out, most);
}

static int64_t hero_crypto_arm_decrypt(const hero_crypto_held *held, const unsigned char *data, size_t count, unsigned char *out, size_t most) {
    return hero_crypto_win_oaep(held, false, data, count, out, most);
}

static void hero_crypto_arm_free(hero_crypto_held *held) {
    hero_crypto_material_free(&held->material);
}

#endif
