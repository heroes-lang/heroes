/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_crypto.h — the platform layer's cryptography: texts sealed and
 * opened under a secret key, signatures made and checked, keys agreed, and
 * the keys themselves, by the SYSTEM's own library (M-rebel-library; spec
 * § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_tls.h`
 * is: its functions are defined in `platform/hero_crypto.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_crypto.h"`, with no `link` word and no flag.
 *
 * WHY THE SYSTEM'S, AND NEVER A CIPHER OF OUR OWN: a cipher must compute in
 * a time that its key and its text do not decide, and nothing proves that of
 * the C the compiler makes. The system's library keeps that promise and a
 * security update of the system reaches every program. So every
 * computation on a secret is the system's:
 *
 *   - Windows: CNG, `bcrypt.dll` (BCrypt), which only a program that takes
 *     this member links.
 *   - macOS: the Security framework's `SecKey`, and for sealing CryptoKit,
 *     reached by a member of the layer written in Swift, since CommonCrypto's
 *     public modes hold no authenticated one and CryptoKit is Swift's alone;
 *     no private header is read (panel 253's R3, 2026-11-20).
 *     **Ed25519 and X25519 are the one exception to the system's, on
 *     macOS alone**: `SecKey` takes no Curve25519 key and CryptoKit's
 *     Ed25519 signs differently every time, never RFC 8032's bytes, so both
 *     are Monocypher 4.0.3's, a small C library vendored into the runtime
 *     (`runtime/vendor/monocypher/README.md`), as decision 2594 admits for
 *     primitives that read no outside data; its manual says *"No secret
 *     dependent indices, no secret dependent branches"*, a claim of its
 *     authors this project has not measured. A security update of the
 *     system does not reach them; a release of Heroes does.
 *   - Linux: OpenSSL 3's `libcrypto.so.3`, loaded when a program first asks
 *     and never named on the link line, so the layer compiles and links on a
 *     machine with no OpenSSL, and there every call answers
 *     `HERO_CRYPTO_UNSUPPORTED`.
 *
 * What the member computes itself is FORM, never a secret's arithmetic: the
 * DER of PKCS #8 and SubjectPublicKeyInfo read and written, an ECDSA
 * signature's DER around the two numbers Windows answers, the order of a
 * secret's bytes Windows gives backwards.
 *
 * A KEY IS A HANDLE the layer issues and checks: one closed already, NULL,
 * or one this layer never issued is `HERO_CRYPTO_NOT_OPEN`, never a read of
 * freed memory. Its secret lives in the member's own memory from the call
 * that makes it to `hero_crypto_close`, which overwrites it with zeros
 * before it gives it back; a key made here never leaves but through
 * `hero_crypto_write_private`.
 *
 * NONCES ARE NEVER THE CALLER'S. `hero_crypto_seal` draws each nonce, 12
 * bytes, from the system's generator and writes it before the sealed text,
 * so no program reuses one by mistake. Random nonces of 96 bits collide
 * past about 2^32 texts under one key (NIST SP 800-38D § 8.3), so a key
 * seals at most `HERO_CRYPTO_SEALS_MOST` texts and then answers
 * `HERO_CRYPTO_LIMIT`: make another key.
 *
 * THE FILL FORM, as `hero_disk.h` says it: a function that answers bytes
 * takes the caller's `buffer` and its `count`, writes nothing past them, and
 * answers how many bytes the WHOLE answer has. Where that is above `count`
 * the buffer was short, nothing in it is to be read, and the caller asks
 * again with at least that many. A `count` of 0 only measures, and `buffer`
 * may then be NULL; for a signature and a decryption, whose length is known
 * only once made, the measure is the most it can be.
 *
 * NO PLATFORM TYPE AND NO CONDITIONAL BUT THE GUARD: a group sees
 * `int64_t`, `uint64_t`, `void *` and the key below.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a key
 * answers NULL, one answering a count -1, and `hero_crypto_last_error()`
 * then names the kind, `hero_crypto_last_message()` the system's words.
 */

#ifndef HERO_CRYPTO_H
#define HERO_CRYPTO_H

#include <stdint.h>

/* A key: secret, private or public. A group declares it as a handle,
 * `record Key tag hero_crypto_key`, which `hero_crypto_close` gives back. */
typedef struct hero_crypto_key hero_crypto_key;

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_crypto_last_os_error()`. */
#define HERO_CRYPTO_OK 0
/* An argument the layer refuses: a number that names no kind or scheme, a
 * count with no bytes behind it, a key of the wrong size, kind or half for
 * the call, bytes that are not the form asked, a text past its bound. */
#define HERO_CRYPTO_INVALID 1
/* The key is none this layer holds open. */
#define HERO_CRYPTO_NOT_OPEN 2
/* No memory or no slot left, or a key that has sealed
 * `HERO_CRYPTO_SEALS_MOST` texts. */
#define HERO_CRYPTO_LIMIT 8
/* This platform has no such thing: Ed25519 and X25519 on Windows
 * (`hero_crypto_offers`), or no `libcrypto.so.3` on this Linux. */
#define HERO_CRYPTO_UNSUPPORTED 11
/* The system failed for a reason none of these names; its words say it. */
#define HERO_CRYPTO_OTHER 12
/* The sealed bytes do not open under this key with this associated data:
 * changed, cut, or sealed under another key or with other data. */
#define HERO_CRYPTO_FORGED 40

/* What a key is for. The first three are secret keys that seal; the others
 * are key pairs, whose private half signs, agrees or decrypts and whose
 * public half checks, agrees or encrypts. */
#define HERO_CRYPTO_AES_128_GCM 1
#define HERO_CRYPTO_AES_256_GCM 2
#define HERO_CRYPTO_CHACHA20_POLY1305 3
#define HERO_CRYPTO_ED25519 10
#define HERO_CRYPTO_X25519 11
#define HERO_CRYPTO_P256 12
#define HERO_CRYPTO_P384 13
#define HERO_CRYPTO_RSA 14

/* How a signature is made and checked. ECDSA's signature is DER (X9.62),
 * either hash on either curve; RSA-PSS's salt is as long as its hash, on
 * both sides; PKCS #1 v1.5 is RFC 8017's. */
#define HERO_CRYPTO_SIGN_ED25519 20
#define HERO_CRYPTO_SIGN_ECDSA_SHA256 21
#define HERO_CRYPTO_SIGN_ECDSA_SHA384 22
#define HERO_CRYPTO_SIGN_RSA_PSS_SHA256 23
#define HERO_CRYPTO_SIGN_RSA_PSS_SHA384 24
#define HERO_CRYPTO_SIGN_RSA_PSS_SHA512 25
#define HERO_CRYPTO_SIGN_RSA_PKCS1_SHA256 26
#define HERO_CRYPTO_SIGN_RSA_PKCS1_SHA384 27
#define HERO_CRYPTO_SIGN_RSA_PKCS1_SHA512 28
/* How a text is encrypted to an RSA public key: OAEP with SHA-256 and MGF1
 * over SHA-256, no label. */
#define HERO_CRYPTO_ENCRYPT_RSA_OAEP_SHA256 30

/* The bytes of a nonce and of a tag: what `hero_crypto_seal` adds to a
 * text, the nonce before it and the tag after. */
#define HERO_CRYPTO_NONCE_BYTES UINT64_C(12)
#define HERO_CRYPTO_TAG_BYTES UINT64_C(16)
/* The most texts one key seals with nonces of its own drawing. */
#define HERO_CRYPTO_SEALS_MOST UINT64_C(4294967296)
/* The most bytes of a text, of associated data and of a message signed:
 * every platform's call takes this many at once. */
#define HERO_CRYPTO_TEXT_MOST UINT64_C(4294967040)
/* The bits an RSA key is made with: 2048, 3072 or 4096. A key read may hold
 * 1024 to 8192. */
#define HERO_CRYPTO_RSA_BITS_LEAST 1024
#define HERO_CRYPTO_RSA_BITS_MOST 8192

/* 1 where this machine computes `what`, a kind or a scheme above, 0 where
 * it does not (the kinds Windows lacks, every one on a Linux machine with
 * no `libcrypto.so.3`); -1 with `HERO_CRYPTO_INVALID` for a number that
 * names neither. */
int64_t hero_crypto_offers(int64_t what);

/* A new key of `kind`, drawn by the system: `bits` is an RSA key's size,
 * 2048, 3072 or 4096, and 0 for every other kind. A key pair answers its
 * private half. */
hero_crypto_key *hero_crypto_generate(int64_t kind, int64_t bits);

/* A secret key of `kind`, one of the three that seal, from its `count`
 * bytes: 16 for AES-128-GCM, 32 for the others. The bytes are copied into
 * the key; the caller's are never kept. */
hero_crypto_key *hero_crypto_from_secret(int64_t kind, const void *bytes, uint64_t count);

/* A private key from `count` bytes of PKCS #8 DER (RFC 5208, or RFC 5958's
 * version 2), unencrypted: Ed25519, X25519, P-256, P-384 or RSA of 1024 to
 * 8192 bits. An EC key must hold its public point on macOS and Windows,
 * which compute none (`HERO_CRYPTO_UNSUPPORTED` there), as every library
 * writes it. */
hero_crypto_key *hero_crypto_read_private(const void *der, uint64_t count);

/* A public key from `count` bytes of SubjectPublicKeyInfo DER (RFC 5280),
 * of the same kinds; an EC point must be uncompressed. */
hero_crypto_key *hero_crypto_read_public(const void *der, uint64_t count);

/* The public half of `k`, a new key the caller closes; a public key answers
 * a new handle on the same key. A secret key has none: `HERO_CRYPTO_INVALID`. */
hero_crypto_key *hero_crypto_public_of(hero_crypto_key *k);

/* `k`'s kind, its size in bits (128 or 256 for a secret key, the curve's or
 * the modulus's for a pair), and whether it is a public key (1) or not (0);
 * -1 with `HERO_CRYPTO_NOT_OPEN`. */
int64_t hero_crypto_kind(hero_crypto_key *k);
int64_t hero_crypto_bits(hero_crypto_key *k);
int64_t hero_crypto_is_public(hero_crypto_key *k);

/* `k`'s private half as PKCS #8 DER, version 0, unencrypted, by the fill
 * form; `HERO_CRYPTO_INVALID` for a public or a secret key. The bytes are
 * the same on every platform: an EC key holds its public point and not its
 * curve twice, as Go and Python write it. */
int64_t hero_crypto_write_private(hero_crypto_key *k, void *buffer, uint64_t count);

/* `k`'s public half as SubjectPublicKeyInfo DER, by the fill form;
 * `HERO_CRYPTO_INVALID` for a secret key. */
int64_t hero_crypto_write_public(hero_crypto_key *k, void *buffer, uint64_t count);

/* Closes `k`, its secret overwritten with zeros, and gives its handle back
 * whatever is answered: 0, or -1 where it was not open. A call using `k` on
 * another thread finishes first. */
int64_t hero_crypto_close(hero_crypto_key *k);

/* `text_count` bytes of `text` sealed under the secret key `k` with
 * `associated_count` bytes of `associated`, which are authenticated and not
 * encrypted: written into `buffer` as a nonce the call draws, the
 * ciphertext, as long as the text, and the tag, so `text_count` +
 * `HERO_CRYPTO_NONCE_BYTES` + `HERO_CRYPTO_TAG_BYTES` bytes, by the fill
 * form. A short buffer seals nothing and draws no nonce. Fails
 * `HERO_CRYPTO_INVALID` for a key that does not seal or a text past
 * `HERO_CRYPTO_TEXT_MOST`, `HERO_CRYPTO_LIMIT` past `HERO_CRYPTO_SEALS_MOST`. */
int64_t hero_crypto_seal(hero_crypto_key *k, const void *text, uint64_t text_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count);

/* FOR KNOWN-ANSWER TESTS ONLY: `hero_crypto_seal` with the
 * `HERO_CRYPTO_NONCE_BYTES` bytes of `nonce` instead of a nonce drawn. A
 * nonce used twice under one key gives away the two texts and lets anyone
 * forge, so a program that is not a test never calls it; it counts toward
 * the bound as a seal does. */
int64_t hero_crypto_seal_nonce_for_tests(hero_crypto_key *k, const void *nonce, uint64_t nonce_count, const void *text, uint64_t text_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count);

/* `sealed_count` bytes of `sealed`, as `hero_crypto_seal` writes them,
 * opened under `k` with `associated`: the text, `sealed_count` minus the
 * nonce and the tag, into `buffer` by the fill form. Fails
 * `HERO_CRYPTO_FORGED` where the tag does not hold, and then `buffer` holds
 * zeros, never a byte of an unchecked text; `HERO_CRYPTO_INVALID` for fewer
 * bytes than a nonce and a tag. */
int64_t hero_crypto_open(hero_crypto_key *k, const void *sealed, uint64_t sealed_count, const void *associated, uint64_t associated_count, void *buffer, uint64_t count);

/* The signature by the private key `k` of `count` bytes of `message`, by
 * `scheme`, into `buffer` by the fill form; a `buffer_count` of 0 measures
 * the most it can be. Ed25519 is deterministic, ECDSA and RSA-PSS draw. */
int64_t hero_crypto_sign(hero_crypto_key *k, int64_t scheme, const void *message, uint64_t count, void *buffer, uint64_t buffer_count);

/* 1 where `signature_count` bytes of `signature` are `k`'s signature of
 * `message` by `scheme`, 0 where they are not (bytes of no signature's
 * form included); -1 for a failure, a scheme not of `k`'s kind among them.
 * `k` is a public key or a private one, whose public half checks. */
int64_t hero_crypto_verify(hero_crypto_key *k, int64_t scheme, const void *message, uint64_t count, const void *signature, uint64_t signature_count);

/* The secret `mine`, a private X25519, P-256 or P-384 key, agrees with
 * `theirs`, a key of the same kind, into `buffer` by the fill form: 32
 * bytes for X25519 and P-256, 48 for P-384, the shared point's coordinate
 * as SEC 1 and RFC 7748 write it. Fails `HERO_CRYPTO_INVALID` where the
 * kinds differ, and where X25519 answers all zeros (a point of small order,
 * RFC 7748 § 6.1). Derive keys from it, never use it as one. */
int64_t hero_crypto_agree(hero_crypto_key *mine, hero_crypto_key *theirs, void *buffer, uint64_t count);

/* `count` bytes of `text` encrypted to the RSA key `k` by `scheme`, into
 * `buffer` by the fill form: as many bytes as the modulus. The text may be
 * at most the modulus's bytes less 66. */
int64_t hero_crypto_encrypt(hero_crypto_key *k, int64_t scheme, const void *text, uint64_t count, void *buffer, uint64_t buffer_count);

/* `count` bytes of `data` decrypted by the private RSA key `k`, into
 * `buffer` by the fill form, whose measure is the most a text can be. Fails
 * `HERO_CRYPTO_FORGED` for bytes that do not decrypt, saying no more. */
int64_t hero_crypto_decrypt(hero_crypto_key *k, int64_t scheme, const void *data, uint64_t count, void *buffer, uint64_t buffer_count);

/* The kind of the last call's failure on this thread, `HERO_CRYPTO_OK`
 * after a call that did not fail, and the platform's own number for it (0
 * where the layer itself refused). */
int64_t hero_crypto_last_error(void);
int64_t hero_crypto_last_os_error(void);

/* The last failure on this thread told in words, the system's own where it
 * gave some, written into `buffer` with a zero byte after it, cut at a
 * character's end where it does not fit; answers the length written
 * without that byte, 0 where there is none, -1 with `HERO_CRYPTO_INVALID`
 * where `count` is 0. */
int64_t hero_crypto_last_message(void *buffer, uint64_t count);

#endif
