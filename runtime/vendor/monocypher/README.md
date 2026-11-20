# Monocypher 4.0.3, vendored

Loup Vaillant's Monocypher, a small cryptographic library in C, carried here
for the platform layer's macOS member (`runtime/platform/hero_crypto.c`), which
takes Ed25519 (RFC 8032) and X25519 (RFC 7748) from it: macOS offers neither to
C (`SecKey` takes no Curve25519 key, and CryptoKit's Ed25519 signs differently
every time, so it cannot give RFC 8032's signatures; panel 253's R3). Admitted
by the author's answer to decision 24 on 2026-11-19 (decision 2594): a vendored
C library for cryptographic primitives, which read no outside data.

## What is here, and from where

The four source files of the release, **byte for byte**, flattened into one
folder (upstream keeps the two Ed25519 files under `src/optional/`; each
`#include` is by a bare name, so it finds its neighbour in either layout), and
its licence:

| file | upstream path | sha256 |
|---|---|---|
| `monocypher.c` | `src/monocypher.c` | `57eb914fc88136119bd41655cccb8c250048bf54d470540625186f8ab16f64be` |
| `monocypher.h` | `src/monocypher.h` | `c494da712122da7ff679fdcf318a5317e84972b6c950fe9d896212947797facd` |
| `monocypher-ed25519.c` | `src/optional/monocypher-ed25519.c` | `60fce3578fb00b00da96490653d993c4cb427b1e1be38183285c66e04d22cc18` |
| `monocypher-ed25519.h` | `src/optional/monocypher-ed25519.h` | `abc4fad381879f5c29176ebe014b9189956b3dfe0a3e36459b6990bc57212380` |
| `LICENCE.md` | `LICENCE.md` | `5f8360e4c06ddcc584bdb4b210c6af824c4bb301e6a9a521869b6d90795ca4b3` |

Taken 2026-11-20 from `monocypher-4.0.3.tar.gz`, released 2026-06-15, the
latest release that day (GitHub's release list), whose SHA-512 is the one
`https://monocypher.org/download/monocypher-4.0.3.tar.gz.sha512` publishes,
`40904ada5c7ee4f7741733e38b69a30a4b0561cbffba5ffe7c2dce16136d540251ec0d9056ff606510d3b5b708fb8a40db7e0870d4a0b2dc17ba2bfb880f8965`,
and whose bytes are the same at monocypher.org and at
`https://github.com/LoupVaillant/Monocypher/releases/download/4.0.3/monocypher-4.0.3.tar.gz`
(`cmp`); its sha256 is
`8cc9bc341a66249016db9bd70e9142d8d0aef9945973744b1ac05dbc55d8ee66`. 4.0.3's
changelog opens with *"Fixed timing leak vulnerability in EdDSA/Ed25519
signatures"*, and the download page says the versions below it *"can have a
timing leak on public key cryptography ... depending on compiler settings"*.

Size: 3,952 lines of C, 145,813 bytes with the licence (`wc`). Compiled under
the layer's own words on this Mac (Apple clang 21), with no warning: 72 KB of
text for the whole of `monocypher.c` at `-O0`, of which the member keeps what
it calls, about 24 KB (`size`).

## The licence, and which half is taken

`LICENCE.md`, read whole: Monocypher is dual-licensed, *"Choose whichever
licence you want from the two"*, the 2-clause BSD licence or Creative Commons'
CC0 1.0, a dedication to the public domain with a public licence as its
fallback. **Heroes takes it under CC0 1.0**, so that a program built with
Heroes, which carries these functions where it takes the macOS member, owes
nothing for them, as `LICENSE-RUNTIME-EXCEPTION` promises of the runtime: the
BSD half would oblige every such binary to reproduce a notice. The licence
travels with the files all the same, and `NOTICE` names them.

## How it is compiled

Never alone: `hero_crypto.c` includes the two `.c` files on macOS alone, inside
one `#pragma clang attribute` giving every function and object of theirs
internal linkage, so the member's object exports none of Monocypher's names
and a program that binds Monocypher, libsodium or any other library with
`crypto_` names itself meets no second definition (panel 253's R1, the
ffi-pragmatist's veto: nothing may stop a program binding a C library
itself), and what the member does not call is not compiled into it. The two
files cannot share a unit as they stand: each defines the macros `COPY`,
`FOR`, `MIN`, `WIPE_BUFFER`, `WIPE_CTX` and `ZERO` and a `static`
`hash_reduce` of its own, so the member takes the first file's macros back
and renames the second's `hash_reduce` before it includes it. Nothing in
these files is edited; an upgrade replaces the five and this page, and a new
clash between the two is a compile error.

These files are named in `runtime/runtime.c`'s list, so the runtime's key
covers them and a copy of the runtime made from that list holds them; they
stand outside `platform/`, so the layer does not archive them as members.
