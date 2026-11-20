/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_crypto_cryptokit.swift: the authenticated ciphers of macOS,
 * AES-GCM and ChaCha20-Poly1305, by the system's CryptoKit, for
 * `hero_crypto.c`'s Apple arm (panel 253's R3, the mixed route;
 * M-rebel-library's steps 69). macOS offers neither cipher to C:
 * CommonCrypto's public modes hold no authenticated one and its GCM is in a
 * header the SDK does not ship; CryptoKit has both and is Swift's alone. So
 * this file is Swift, and exports two functions to C by `@c`, which `swiftc`
 * 6.4.2 prints in its generated header's C section, outside `#if
 * defined(__OBJC__)`.
 *
 * ITS C DECLARATIONS ARE `hero_crypto_cryptokit.h`'s, and the layer holds the
 * two to each other at every compile: it compiles a C unit that includes the
 * header `swiftc` generated from this file beside that one, so a parameter of
 * another type is clang's `conflicting types`, a refusal of the compiler's own
 * (`selfhost/cli/layer_swift.hero`). The layer compiles it on an Apple target
 * alone, archives it beside the C members, and the linker takes it only into a
 * program that takes `hero_crypto.c`; CryptoKit, Foundation and the Swift
 * runtime are linked by the object's own entries.
 *
 * WHAT IT COMPUTES IS THE CIPHER, NOTHING ELSE: the common part of
 * `hero_crypto.c` has checked every argument, drawn the nonce or taken the
 * test's, counted the seal and holds the key. A key is copied into
 * CryptoKit's `SymmetricKey`, which keeps its bytes in memory CryptoKit
 * overwrites when it is freed; a text opened is copied out and its copy
 * here overwritten before the call answers. CryptoKit's own buffers are its
 * own.
 *
 * EVERY FAILURE IS A VALUE, NEVER A TRAP: no `!`, no `try!`, and every
 * length checked before a buffer is made of a pointer, so a Swift runtime
 * trap, which would end the program with no word of the runtime's, has no
 * place to arise. A refusal answers -1 with CryptoKit's words in `words`,
 * cut at a character's end; a tag that does not hold answers 0.
 */

import CryptoKit
import Foundation

// The kinds `hero_crypto.h` numbers: `HERO_CRYPTO_AES_128_GCM`,
// `HERO_CRYPTO_AES_256_GCM` and `HERO_CRYPTO_CHACHA20_POLY1305`.
private let aes128Gcm: Int64 = 1
private let aes256Gcm: Int64 = 2
private let chacha20Poly1305: Int64 = 3

// The bytes of a nonce and a tag (`HERO_CRYPTO_NONCE_BYTES`,
// `HERO_CRYPTO_TAG_BYTES`).
private let nonceBytes = 12
private let tagBytes = 16

// `text` into `words`, at most `wordsCount` bytes with the zero after them,
// cut where a character ends.
private func said(_ text: String, _ words: UnsafeMutablePointer<CChar>?, _ wordsCount: Int64) {
    guard let words = words, wordsCount > 0 else {
        return
    }
    let most = Int(wordsCount) - 1
    var at = 0
    for character in text {
        let bytes = Array(String(character).utf8)
        if at + bytes.count > most {
            break
        }
        for byte in bytes {
            words[at] = CChar(bitPattern: byte)
            at += 1
        }
    }
    words[at] = 0
}

// Whether `keyCount` bytes are a key of `kind`.
private func fits(_ kind: Int64, _ keyCount: Int64) -> Bool {
    return (kind == aes128Gcm && keyCount == 16) || (kind == aes256Gcm && keyCount == 32) || (kind == chacha20Poly1305 && keyCount == 32)
}

// One byte a buffer of no bytes starts at, never read and never given back.
// **CryptoKit's `ChaChaPoly.seal` traps on associated data of no bytes whose
// start is nil** (SIGTRAP inside CryptoKit, measured 2026-11-20 on macOS
// 27.2: `crypto/seal`'s test sealing three bytes with no associated data
// died of it), and takes the same buffer with a start; so no buffer handed
// to CryptoKit here has a nil start.
private let nothing: UnsafeMutableRawPointer = {
    let at = UnsafeMutableRawPointer.allocate(byteCount: 1, alignment: 1)
    at.storeBytes(of: 0, as: UInt8.self)
    return at
}()

// `count` bytes at `bytes`, or none at `nothing` where `count` is 0, as
// CryptoKit reads them; the caller has checked that bytes stand behind a
// count above 0.
private func lent(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> UnsafeRawBufferPointer {
    if count == 0 {
        return UnsafeRawBufferPointer(start: UnsafeRawPointer(nothing), count: 0)
    }
    return UnsafeRawBufferPointer(start: bytes, count: Int(count))
}

// `textCount` bytes of `text` sealed under the key of `kind` with
// `associated`: the ciphertext into `out`, as long as the text, and the tag
// into `tag`. 1, or -1 with the words said.
@c(hero_crypto_cryptokit_seal)
public func heroCryptoCryptokitSeal(
    _ kind: Int64,
    _ key: UnsafePointer<UInt8>?,
    _ keyCount: Int64,
    _ nonce: UnsafePointer<UInt8>?,
    _ text: UnsafePointer<UInt8>?,
    _ textCount: Int64,
    _ associated: UnsafePointer<UInt8>?,
    _ associatedCount: Int64,
    _ out: UnsafeMutablePointer<UInt8>?,
    _ tag: UnsafeMutablePointer<UInt8>?,
    _ words: UnsafeMutablePointer<CChar>?,
    _ wordsCount: Int64
) -> Int64 {
    guard fits(kind, keyCount), let key = key, let nonce = nonce, let tag = tag, textCount >= 0, associatedCount >= 0,
        textCount == 0 || (text != nil && out != nil), associatedCount == 0 || associated != nil
    else {
        said("CryptoKit was given no key, nonce, tag or bytes where the call needs them", words, wordsCount)
        return -1
    }
    let secret = SymmetricKey(data: UnsafeRawBufferPointer(start: key, count: Int(keyCount)))
    let drawn = UnsafeRawBufferPointer(start: nonce, count: nonceBytes)
    do {
        let ciphertext: Data
        let mac: Data
        if kind == chacha20Poly1305 {
            let box = try ChaChaPoly.seal(lent(text, textCount), using: secret, nonce: try ChaChaPoly.Nonce(data: drawn), authenticating: lent(associated, associatedCount))
            ciphertext = box.ciphertext
            mac = box.tag
        } else {
            let box = try AES.GCM.seal(lent(text, textCount), using: secret, nonce: try AES.GCM.Nonce(data: drawn), authenticating: lent(associated, associatedCount))
            ciphertext = box.ciphertext
            mac = box.tag
        }
        guard ciphertext.count == Int(textCount), mac.count == tagBytes else {
            said("CryptoKit sealed to another length than the text and a tag of 16 bytes", words, wordsCount)
            return -1
        }
        if textCount > 0, let out = out {
            _ = ciphertext.copyBytes(to: UnsafeMutableRawBufferPointer(start: out, count: Int(textCount)))
        }
        _ = mac.copyBytes(to: UnsafeMutableRawBufferPointer(start: tag, count: tagBytes))
        return 1
    } catch {
        said("CryptoKit refused to seal: " + String(describing: error), words, wordsCount)
        return -1
    }
}

// `dataCount` bytes of ciphertext opened under the key of `kind` with
// `associated` and `tag`: the text into `out`. 1; 0 where the tag does not
// hold, `out` untouched; -1 with the words said.
@c(hero_crypto_cryptokit_open)
public func heroCryptoCryptokitOpen(
    _ kind: Int64,
    _ key: UnsafePointer<UInt8>?,
    _ keyCount: Int64,
    _ nonce: UnsafePointer<UInt8>?,
    _ data: UnsafePointer<UInt8>?,
    _ dataCount: Int64,
    _ associated: UnsafePointer<UInt8>?,
    _ associatedCount: Int64,
    _ tag: UnsafePointer<UInt8>?,
    _ out: UnsafeMutablePointer<UInt8>?,
    _ words: UnsafeMutablePointer<CChar>?,
    _ wordsCount: Int64
) -> Int64 {
    guard fits(kind, keyCount), let key = key, let nonce = nonce, let tag = tag, dataCount >= 0, associatedCount >= 0,
        dataCount == 0 || (data != nil && out != nil), associatedCount == 0 || associated != nil
    else {
        said("CryptoKit was given no key, nonce, tag or bytes where the call needs them", words, wordsCount)
        return -1
    }
    let secret = SymmetricKey(data: UnsafeRawBufferPointer(start: key, count: Int(keyCount)))
    let drawn = UnsafeRawBufferPointer(start: nonce, count: nonceBytes)
    let mac = UnsafeRawBufferPointer(start: tag, count: tagBytes)
    do {
        var text: Data
        if kind == chacha20Poly1305 {
            let box = try ChaChaPoly.SealedBox(nonce: try ChaChaPoly.Nonce(data: drawn), ciphertext: lent(data, dataCount), tag: mac)
            text = try ChaChaPoly.open(box, using: secret, authenticating: lent(associated, associatedCount))
        } else {
            let box = try AES.GCM.SealedBox(nonce: try AES.GCM.Nonce(data: drawn), ciphertext: lent(data, dataCount), tag: mac)
            text = try AES.GCM.open(box, using: secret, authenticating: lent(associated, associatedCount))
        }
        // The copy here is this function's, overwritten on every way out.
        defer {
            text.resetBytes(in: 0..<text.count)
        }
        guard text.count == Int(dataCount) else {
            said("CryptoKit opened to another length than the ciphertext", words, wordsCount)
            return -1
        }
        if dataCount > 0, let out = out {
            _ = text.copyBytes(to: UnsafeMutableRawBufferPointer(start: out, count: Int(dataCount)))
        }
        return 1
    } catch CryptoKitError.authenticationFailure {
        return 0
    } catch {
        said("CryptoKit refused to open: " + String(describing: error), words, wordsCount)
        return -1
    }
}
