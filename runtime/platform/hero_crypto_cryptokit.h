/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_crypto_cryptokit.h: the two functions
 * `hero_crypto_cryptokit.swift` exports to C, which `hero_crypto.c`'s Apple
 * arm calls to seal and open by CryptoKit. A header of the member's own, as
 * `hero_handle_tag.h` is, and no group's: a program binds `hero_crypto.h`.
 *
 * HELD TO SWIFT'S OWN AT EVERY COMPILE OF THE LAYER: `swiftc` writes the
 * header its `@c` functions mean, and the layer compiles a C unit including
 * that header and this one, so a declaration here that differs from the Swift
 * is `conflicting types` and the layer does not build
 * (`selfhost/cli/layer_swift.hero`). Swift's `Int64` is `int64_t`, a typed
 * pointer `uint8_t *`, `UnsafeMutablePointer<CChar>` `char *`.
 *
 * `kind` is `HERO_CRYPTO_AES_128_GCM`, `_AES_256_GCM` or
 * `_CHACHA20_POLY1305`; the nonce is 12 bytes and the tag 16. Each answers 1
 * done, -1 refused with CryptoKit's words in `words` (at most `words_count`
 * bytes with a zero after them); `open` answers 0 where the tag does not
 * hold, writing nothing into `out`.
 */

#ifndef HERO_CRYPTO_CRYPTOKIT_H
#define HERO_CRYPTO_CRYPTOKIT_H

#include <stdint.h>

int64_t hero_crypto_cryptokit_seal(int64_t kind, const uint8_t *key, int64_t key_count, const uint8_t *nonce, const uint8_t *text, int64_t text_count, const uint8_t *associated, int64_t associated_count, uint8_t *out, uint8_t *tag, char *words, int64_t words_count);

int64_t hero_crypto_cryptokit_open(int64_t kind, const uint8_t *key, int64_t key_count, const uint8_t *nonce, const uint8_t *data, int64_t data_count, const uint8_t *associated, int64_t associated_count, const uint8_t *tag, uint8_t *out, char *words, int64_t words_count);

#endif
