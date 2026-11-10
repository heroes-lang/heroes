# Panel 241, the ffi-pragmatist's report

Written as it goes, 2026-11-10. Worktree `.claude/worktrees/241-ffi-pragmatist`, detached at
`87d77dec2`. Probes in `.claude/worktrees/p241-scratch/ffi/`.

## The compiler

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, `--version`
  `sources 5aae00294f7cd6fa` (9.79 s total by zsh `time`).
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, exit 0, `sources 110fad075b8d3bca`
  (the brief's key), `real 113.07 user 86.31 sys 18.99` (load average 3.43 when read after; a
  planning figure only).

## The three files

`p241-scratch/ffi/data/`: `bin.dat` 3,001 bytes of `/dev/urandom` (not UTF-8 at byte 2, by Python's
decoder), `empty.dat` 0 bytes, `big.dat` 4,194,304 bytes of `/dev/urandom` (not UTF-8 at byte 5),
larger than the 65,536-byte read every package below makes. `shasum -a 256`:

```
d138cc92e48f6066eadef38dc2ba7abc0e609dbc731e978c64cb05507c0184ef  bin.dat
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855  empty.dat
4660fc41d7b1dc872e0be5b7d23b553336d42d78d4453bca0726c5dc672c047d  big.dat
```

## Where `libcrypto` resolves on this Mac

`pkg-config --version` 3.0.10 (`/opt/homebrew/bin/pkg-config`); `pkg-config --modversion libcrypto`
4.1.0; `--cflags --libs`: `-I/opt/homebrew/Cellar/openssl@4/4.1.0/include
-L/opt/homebrew/Cellar/openssl@4/4.1.0/lib -lcrypto`; `brew --prefix openssl`
`/opt/homebrew/opt/openssl@4`, its `lib/pkgconfig` holding `libcrypto.pc`, found through the default
`pc_path` (`PKG_CONFIG_PATH` empty). In that header (`openssl/sha.h`) `SHA256_Init`, `_Update`,
`_Final` are `OSSL_DEPRECATEDIN_3_0` (lines 74 to 77); the one-shot `SHA256` (line 83) and `evp.h`'s
`EVP_MD_CTX_new`, `EVP_DigestInit_ex`, `EVP_DigestUpdate`, `EVP_DigestFinal_ex`, `EVP_sha256` (lines
627 to 878) are not.

## The critic's probes, reproduced first

With my stage 2, in `p241-scratch/ffi/repro/`: `bytes/main.hero` (stdio.h's `fopen("rb")`, `fread`
with `@buf: [u8] counted_by n lent`) printed `5`, `0 1 2 255 254`, exit 0, on a 5-byte file written
by `printf '\x00\x01\x02\xff\xfe'`; `sha/main.hero` (binding `hero_compiler.h`) printed `0
aa5cd9ac...e9f60a` and `ba7816bf...15ad`, equal to `shasum -a 256 blob.bin` and `printf abc |
shasum -a 256`, and `read_file` gave `not_text`. Both as the critic wrote.

## How each route was built

Each route is a package in a scratch index of my own, `p241-scratch/ffi/index/<name>/1.0.0/`, its
`list` and `versions` written by `mkindex.sh` in the core index's format (the script reproduces
`packages/core/0.2.0/list` byte for byte, `cmp` silent), placed by `heroes-next add <name> --index
file://../index -C .` and reached as `use <name>/crypto/sha256`. Each package module binds
`stdio.h` itself (`fopen`, `fread` with the fill form, `ferror`, `fclose`) and reads 65,536 bytes at a
time; `sum_file(path) -> str?` and `sum256(data: [u8])` are its two functions. Every program prints
`<digest>  <path>` per argument, so `diff` against `shasum -a 256` judges it, then the digest of the
`[u8]` `[97, 98, 99]`.

## Route (a), `pkgconfig "libcrypto"`: this Mac

Package `shaa`, `crypto/sha256.hero`, **96 lines of Heroes, 0 lines of C**. The group:

```
extern "openssl/evp.h" pkgconfig "libcrypto"
    record MdCtx tag EVP_MD_CTX
    record Md tag EVP_MD
    function EVP_MD_CTX_new() -> MdCtx acquires EVP_MD_CTX_free
    function EVP_MD_CTX_free(ctx: MdCtx consumes)
    function EVP_MD_fetch(libctx: ptr, algorithm: cstr lent, properties: ptr) -> Md acquires EVP_MD_free
    function EVP_MD_free(md: Md consumes)
    function EVP_DigestInit_ex(ctx: MdCtx, kind: Md, impl: ptr) -> i32
    function EVP_DigestUpdate(ctx: MdCtx, @d: [u8] counted_by cnt lent, cnt: u64) -> i32
    function EVP_DigestFinal_ex(ctx: MdCtx, @md: [u8] counted_by 32 lent, @s: u32) -> i32
```

- Built and ran: `diff` of the three digests against `shasum -a 256` silent; `abc` gave
  `ba7816bf...15ad`; a missing file `file_not_found`; `--sanitize` (ASan and UBSan) the same three
  digests, exit 0. `otool -L` names `/opt/homebrew/opt/openssl@4/lib/libcrypto.4.dylib`: the binary
  runs only where Homebrew's `openssl@4` is installed at that path.
- **What the binding cost, compiled**: the first spelling, `EVP_sha256() -> Md`, was refused,
  `error[ffi_return_type]: EVP_sha256 returns const EVP_MD * ... not Md`. The spec has no spelling for
  a `const`-qualified handle result (searched: `const` in `spec/heroes-spec.md`, 15 lines, none
  about a result; `returns \`const` in `tests/golden/`, none). `EVP_sha256() -> ptr` builds and runs
  (`constprobe/`, printed `true`), and loses the handle's type; OpenSSL 3's own `EVP_MD_fetch`
  returns a non-const `EVP_MD *` the program frees, so the typed spelling exists here. A library
  whose only door is a `const T *` result is bound as `ptr` today. Two more checker refusals, each
  with its repair drafted: `consumed_borrowed_handle` (a helper freeing a handle it was lent, repaired
  `@ctx`) and `never_rebound`.
- `sum256` returns `str?` on this route alone: libcrypto can fail (no context, no provider), and the
  binding says so; routes (b) to (d) cannot fail.

## Route (b), the package's own C header: this Mac

Package `shab`, `crypto/sha256.hero` **69 lines of Heroes** and `crypto/sha256.h` **122 lines of
C**, beside it in the package, named `extern "crypto/sha256.h"` from the package's root as
`fixedbugs-2351` names `inner/deep.h`. The C is portable C11, every function `static inline`, no
`#ifdef`, no platform arm; `clang -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Werror
-fsyntax-only` on a file including it alone: exit 0. The state is a group record held by the
program, no allocation:

```
extern "crypto/sha256.h"
    record State tag crypto_sha256
        h: u32[8]
        block: u8[64]
        used: u64
        total: u64
    function crypto_sha256_start() -> State
    function crypto_sha256_update(@s: State, @p: [u8] counted_by n lent, n: u64)
    function crypto_sha256_finish(@s: State, @out: [u8] counted_by 32 lent)
```

Built and ran: the three digests equal `shasum -a 256` (`diff` silent), `abc` equal, `--sanitize`
equal, exit 0; `otool -L` names `libSystem` alone, so the binary runs on any Mac.

## The thesis at the package boundary, both C routes

A wrong width that only the header can refute, the call sites kept consistent, in a copy of each
package (`shaam`, `shabm`), placed and built:

- (a) `@s: u64` where OpenSSL says `unsigned int *`: exit 1, `error[ffi_parameter_type]: s of
  EVP_DigestFinal_ex is declared u64, and the header's unsigned int * points at a different width or
  sign`, at the `.hero` line, `fix (guess): declare s as @s: u32`.
- (b) the field `used: u32` where the package's struct says `uint64_t`: exit 1,
  `error[ffi_field_type]: State.used is uint64_t in crypto/sha256.h ... not u32`, at the `.hero`
  line.

So clang verifies a placed package's binding against a system header and against the package's own,
the same way. (A first pair of mutants, the count `u64` to `u32` with the calls unchanged, was refused
earlier by Heroes' own `type_mismatch` at the calls: also an error, but not the header's.)

## Route (c), the runtime's third door: this Mac

The door does not exist, so I wrote it, in a copy of my worktree's runtime
(`p241-scratch/ffi/runtime-c/`), linked by `HEROES_RUNTIME`: **7 lines** in `parts/sha256.c`, a
public `HeroStr hero_bytes_sha256(const uint8_t *p, uint64_t n)` calling the `static`
`hero_sha256_bytes`, and **2 lines** in `hero_compiler.h` (`diff -u`, lines added). Package `shac`,
`crypto/sha256.hero` **44 lines of Heroes**, binding

```
extern "hero_compiler.h"
    function hero_bytes_sha256(@p: [u8] counted_by n lent, n: u64) -> str
```

The door is one-shot, so the package holds the whole file in one `[u8]`: its size by `fseek(SEEK_END)`
and `ftell`, one `fread` of that size. (`fseek`'s `long` offset is declared `i32`, which C converts
exactly on LP64 and which is `long`'s own width on Windows; `ftell -> i64`, a result wider than C's.)
A streaming door would put a state struct into the runtime's header, which this probe did not write.

- Built against the scratch runtime and ran: three digests equal `shasum -a 256`, `abc` equal,
  `--sanitize` equal, exit 0.
- **Built against the unmodified runtime, the same package is refused**: `error[ffi_unknown_name]:
  hero_compiler.h declares no hero_bytes_sha256 - clang read the header and could not find it`,
  exit 1. So a package on this route builds only with a compiler release whose runtime carries the
  door: its version is tied to the compiler's, which routes (a), (b) and (d) are not (they need only
  the fill form, in the spec since panel 234).

## Route (d), Heroes alone: this Mac

Package `shad`, `crypto/sha256.hero` **288 lines of Heroes, 0 of C**: the constants, `compress` and
`hex8` copied from `selfhost/cli/sha256.hero` as they stand at `87d77dec2` (`sed -n 22,108p` and
`192,256p`), a streaming `feed` over `[u8]` and `finish` written for the probe. Three digests and
`abc` equal `shasum -a 256` at `-O0` and at `-O2`.

## Every route at the block and read edges

Eleven more files of `/dev/urandom`, 55, 56, 63, 64, 65, 119, 120, 65,535, 65,536, 65,537 and
131,073 bytes (`data/edges/`): routes (a), (b), (c) and (d) at `-O2`, and (d) at `-O0`, each 11 of 11
equal to `shasum -a 256` (`diff` silent). So 14 files per route.

## The clocks, this Mac

`/usr/bin/time -p`, 7 runs each, the first discarded, a run with `real` above `user + sys + 0.1`
discarded as waiting (the latency instrument's rule), best `real` of the rest (`timeit.sh`). **The
machine was not still**: load average 3.10 to 4.08 across the runs (`uptime` before and after),
another seat's `heroes-next` at 52.9% CPU and an editor helper at 98.5% (`ps`). Noise floor, each
binary with no file: 0.00 s for all eight (below the clock's 0.01 s). `heroes build` is `-O0` by
default, `heroes run` `-O2` (`--help`).

| route | 4 MiB `-O0` | 4 MiB `-O2` | 64 MiB `-O0` | 64 MiB `-O2` |
|---|---|---|---|---|
| (a) libcrypto | 0.02 | 0.01 | 0.33 | 0.20 |
| (b) package header | 0.07 | 0.02 | 1.24 | 0.37 |
| (c) runtime door | 0.07 | 0.02 | 1.16 | 0.33 |
| (d) Heroes alone | 0.60 | 0.05 | 9.60 | 0.81 |

Every run kept but one of (d) 64 MiB `-O0` (5 of 6). Spreads (worst kept less best): at most 0.08 s below 2 s ((c) 64 MiB `-O0`, 1.16 to 1.24), 0.33 s for (d) 64 MiB `-O0`. Read: at `-O2` Heroes alone is 2.2 times (b), 2.5 times (c)
and 4.0 times (a) on 64 MiB (0.81 against 0.37, 0.33, 0.20); at `-O0` it is 7.7 times (b) and 29
times (a). Routes (b) and (c) run the same C at the program's level, so they track each other;
libcrypto is compiled once, optimised, by its packager, so its `-O0` cost is the package's Heroes
glue alone (0.13 s of 0.33 on 64 MiB). The carried figure, *0.52 s on 4 MB against 0.06 s at
`-O0`*, reads here 0.60 against 0.07 (c) and 0.02 (a).

## Question 3, the environment's write: this Mac

What the runtime does today (`runtime/parts/os.c:906`): `hero_env_put` is `SetEnvironmentVariableW`
over a UTF-8 to UTF-16 conversion on Windows, leaving the CRT's narrow copy as it was (its own
comment), and `setenv(name, value, 1)` elsewhere. Five compiler modules bind it or the two SHA-256
doors through `hero_compiler.h` (`git grep -A12 'extern "hero_compiler.h"'`: `cli/symbolized`,
`fetch/made_repo` and `fetch/transport` for `hero_env_put`; `fetch/index_source` and
`module/fingerprint` for the two digests).

Three programs, in `p241-scratch/ffi/`:

- `prog-env-setenv`: POSIX `setenv` bound straight from `stdlib.h`, no header of the program's own.
  Builds and prints `0` here.
- `prog-env-putenv`: the UCRT's `_putenv_s` bound the same way. Refused here,
  `error[ffi_unknown_name]: stdlib.h declares no _putenv_s`, with `fix (guess): the header declares
  putenv`.
- `prog-env` over a package `osenv` whose own header `env.h` (16 lines of C) holds the platform arm,
  `#if defined(_WIN32) _putenv_s #else setenv`, bound as `osenv_set(name: cstr lent, value: cstr
  lent) -> i32` (`env.hero`, 8 lines). The program sets `P241_PROBE` to `café-1` through it and
  `P241_RUNTIME` to `café-2` through `hero_env_put` (bound from `hero_compiler.h`), reads each back
  with `getenv` and runs itself as a child through `system`. Here: `package header set: true`, parent
  sees `café-1`; `hero_env_put: 0`, parent sees `café-2`; the child sees both, UTF-8 intact; exit 0.

So one `.hero` binding of the environment's write cannot build on all three platforms without a C
arm somewhere: the spec has no platform condition on a group (none found: `grep -n '_WIN32\|platform'
spec/heroes-spec.md` names no such form), so the arm is a package's own header, the runtime's
`hero_env_put`, or feature 2295's archive.

## Route (a) on this Mac without OpenSSL, and under the compiler

- `PKG_CONFIG_LIBDIR` an empty folder, `PKG_CONFIG_PATH` empty (a Mac with no Homebrew OpenSSL):
  exit 1, `error[ffi_pkgconfig]: the library libcrypto is not installed on this machine`, at the
  `pkgconfig "libcrypto"` string of the placed package's group, pkg-config's own words beneath and
  `install its development files, or name the library directly with link`. A clean refusal at
  build, not a link failure.
- **A compiler that took its digest from route (a) could not be built from C alone.** Route (a)'s
  emitted C (`--emit-c`, 60,806 bytes) carries `#include <openssl/evp.h>` at its line 23. Compiled as
  CLAUDE.md builds the seed, `clang -I runtime <emitted.c> runtime/runtime.c` (the committed runtime,
  `git archive HEAD runtime`): `fatal error: 'openssl/evp.h' file not found`, exit 1; with
  Homebrew's `-I` added, five `Undefined symbols` (`_EVP_DigestFinal_ex` ...), exit 1; with `-I`,
  `-L` and `-lcrypto`, exit 0 and the right digest. So if the package manager were rewritten over a
  `crypto/sha256` of route (a), the seed's build line gains OpenSSL's three flags on every machine
  that bootstraps, the Windows box among them, where none is installed (below).

## Linux arm64, the container

`heroes-linux-arm64:latest` (Debian clang 22.1.8), my worktree copied in with the committed runtime
unpacked over it (`git archive HEAD runtime`, since my worktree's runtime was then mid-experiment),
stage 1 `sources 5aae00294f7cd6fa`, stage 2 `sources 110fad075b8d3bca` (`real 110.595`). Two things the
container refused first, both of the harness and not of a route: `heroes add` from a `file://` index
is `error[source_refused]` outside a git repository (`git init` fixed it), and then
`error[no_curl]`, since `heroes add` runs `curl` for a `file://` index too and the image has libcurl
but no `curl` binary; so the container took the Mac's placed trees, `heroes.toml` and
`heroes.packages/`, unchanged (a placed tree's fingerprint is its files' digests, so it travels).

- `pkg-config` 1.8.1 resolves `libcrypto` to **3.5.7**, `-lcrypto`, from `libssl-dev`, which the
  image has only because `libcurl4-openssl-dev` pulls it in (`dpkg -l`; the Dockerfile names no
  `libssl-dev`).
- **All four routes, both levels: 14 of 14 files equal `sha256sum`**, `abc` equal. Route (a)'s
  binding, written against OpenSSL 4.1.0 on this Mac, built and ran unchanged against 3.5.7; `ldd`
  names `libcrypto.so.3`.
- Clocks on 4 MiB, bash `time`, six runs after one discarded, best `real`: (a) 0.024 / 0.015 s,
  (b) 0.085 / 0.025, (c) 0.080 / 0.024, (d) 0.593 / 0.052 at `-O0` / `-O2`; `real` within 0.004 s of
  `user + sys` in every run. The same order as this Mac.
- Question 3, the same as this Mac: the package header's write and `hero_env_put` each seen by the
  parent's `getenv` and by the child, `café-1` and `café-2`; `setenv` bound from `stdlib.h` builds
  and prints `0`; `_putenv_s` is `ffi_unknown_name`.

## The Windows box

A folder of my own, `C:/w/p241-ffi/` (made by me; no other folder of `C:/w/` touched, listed once
with `ls`). My worktree's `seed/` and `selfhost/` and the committed `runtime/` (`git archive HEAD
runtime`) went by `scp` as three gzip archives (22.4 s; a first, uncompressed `tar | ssh` moved 16 MB
in about eight minutes and was stopped). clang 23.1.1, target `x86_64-pc-windows-msvc`. Stage 1 with
the seed README's `-Wl,/STACK:67108864`, `sources 5aae00294f7cd6fa`; stage 2 `sources
110fad075b8d3bca` (`real 182.779`). The data written on the box by `head -c /dev/urandom`, the same
sizes, and judged by the box's own `sha256sum`. `heroes add` from the `file://` index worked there
(Git for Windows brings `curl`), after `git init` of my folder.

- **Route (a)**: no `pkg-config` and no `pkgconf` on the box's `PATH` (`command -v`), no OpenSSL
  under `Program Files`. Both levels exit 1, `error[ffi_pkgconfig]: the library libcrypto cannot be
  looked up: pkg-config is not on this machine`, at the group's `pkgconfig` string, `note: install
  pkg-config, or name the library directly with link`. A clean refusal at build. Whether OpenSSL
  plus a `pkg-config` installed on the box would build it is **unrun**: installing them is outside my
  folder.
- **Routes (b), (c) and (d), both levels: 14 of 14 equal `sha256sum`**, `abc` equal. Route (c)'s
  `fseek(stream: File, offset: i32, whence: i32)` binds against the UCRT's 32-bit `long` and builds.
- Clocks on 4 MiB, bash `time` (whose `user` and `sys` read 0 for a native process under Git Bash,
  so only `real` is read), six after one discarded, best: (b) 0.245 / 0.043 s, (c) 0.251 / 0.040,
  (d) 0.492 / 0.105 at `-O0` / `-O2`. The box is a shared VPS; these are not comparable with this
  Mac's.
- **Question 3 on Windows**: through the package header's `_putenv_s` arm, `set: true`, the parent's
  `getenv` sees `café-1`, the child sees `café-1`: UTF-8 intact both ways. Through `hero_env_put`:
  `0` (HERO_OS_OK), **the parent's own `getenv` reads `null_cstr`**, the child sees `café-2`. So on
  Windows a value written by `hero_env_put` is invisible to every C library the program binds that
  reads the environment through the CRT's `getenv` in the same process (its own comment says the
  CRT's copy is not updated; this is that, run). `setenv` bound from `stdlib.h` is refused,
  `error[ffi_unknown_name]: stdlib.h declares no setenv`, `fix (guess): the header declares getenv`;
  `_putenv_s` bound from `stdlib.h` builds and prints `0`.

## The dead arm, planted (question 3's header shape)

Package `osenvbad`, `osenv`'s copy with one planted mistake in the Windows arm of `env.h`:
`_putenv_s(name, value, 1)`, three arguments where the UCRT takes two.

- **This Mac: build exit 0, the program runs and prints the same six lines as the correct package.**
  The mistake is invisible here.
- **The Windows box: build exit 1**, `error[ffi_header_refused]: env.h and stdlib.h, the headers two
  groups name, cannot be compiled in one unit: clang refuses them together, at env.h line 10 and
  stdlib.h line 1226: too many arguments to function call, expected 2, have 3`, quoting both lines.
  Caught, but told as two headers refusing each other, where the mistake is `env.h`'s alone: a
  diagnostic-quality note (design.md §4.17), not filed by me.

This is panel 114's R3 measured again on a package: the arm a machine does not compile is verified
on no machine but its own, so a package that ships platform C ships a binding that is green where it
is wrong.

## What moving the doors into `hero_os.h` costs, performed

In my worktree, the originals saved to `p241-scratch/ffi/orig/` first: `hero_env_put`,
`hero_file_sha256` and `hero_text_sha256` cut from `hero_compiler.h` with their comments, pasted
into `hero_os.h`, and the third door `hero_bytes_sha256` declared there and defined in
`parts/sha256.c`. Counted by `diff` against the saved originals:

| file | lines |
|---|---|
| `runtime/hero_compiler.h` | 19 removed |
| `runtime/hero_os.h` | 23 added (19 moved, 2 the new door's, a heading and a blank) |
| `runtime/parts/sha256.c` | 8 added (the door) |
| `runtime/heroes_guard_open.h` | 4 added, one `#pragma push_macro` per name (647 lines before) |
| `runtime/heroes_guard_close.h` | 4 added, one `#pragma pop_macro` per name (647 before) |
| `selfhost/emit/guarded_names.hero` | 4 words, in 4 existing lines of `NAMES`, sorted by bytes |

- Before the guard was paid, `heroes-next test selfhost/main.hero --only "the list holds every word"`
  failed, exactly: `left: "hero_bytes_sha256 hero_env_put hero_file_sha256 hero_text_sha256"`. After:
  that test and `the runtime's guard files hold the list` each `1 test, all passed`. The compiler's
  whole own suite on the moved tree: below.
- **`HERO_RUNTIME_ABI` did not move** (I left it at 32; the test build, which links the moved
  runtime, and every program above built); adding a function is an undefined symbol at an older
  runtime's link, not a stamp (panel 089's reading, `hero_compiler.h:15`). An older runtime is in
  fact refused earlier, at clang's header check: route (c) against the committed runtime is
  `ffi_unknown_name`.
- **No binding has to change**: the five compiler modules whose groups name `hero_compiler.h` for the
  moved functions compiled unchanged, because every unit reads `hero_os.h` before its groups. Measured
  apart (`wrongheader/`): a group naming `stdio.h` for `hero_env_shown` builds and runs (`true`); the
  same with a wrong result type is refused, `ffi_return_type`, so the signature is still held to the
  real declaration and only the header's name goes unchecked for a function a seed declares (told
  as `returns HeroStr in stdio.h`, which names the wrong header; not filed by me).
- Per-unit cost of four more `push_macro` and `pop_macro` pairs in every unit: **unmeasured**.
- **The promise**: design.md:3641-3645 (decision 2228) makes every `hero_os.h` function a door
  *"its functions' stability is promised with the rest at M-compatibility-promise"*. Moving the two
  digests there promises a SHA-256 in the runtime from `v1.0.0` on.

## `hero_env_put` over `_wputenv_s`, the condition compiled

In a scratch copy of the committed runtime (`runtime-e/`), one line of `hero_env_put`'s Windows arm:
`SetEnvironmentVariableW(wide_name, wide_value)` became `_wputenv_s(wide_name, wide_value) == 0`.
The probe now also reads each name through `hero_os.h`'s `hero_env_shown`, which reads the
process's own block (`GetEnvironmentVariableW`), the one `hero_run_go`'s children inherit. On the box:

| | parent's `getenv` | the process block (`hero_env_shown`) | the child |
|---|---|---|---|
| package header, `_putenv_s` | `café-1` | 0 `café-1` | `café-1` |
| committed `hero_env_put` | **`null_cstr`** | 0 `café-2` | `café-2` |
| `hero_env_put` over `_wputenv_s` | `café-2` | 0 `café-2` | `café-2` |

On this Mac and in the container every cell reads the value. So the UCRT's `_wputenv_s` writes both
the CRT's copy and the process block, and one changed line makes `hero_env_put` what a bound C
library expects on Windows.

The full suite on the moved tree: `heroes-next test selfhost/main.hero`, **1992 tests, all
passed** (the critic's count at `87d77dec2`). My worktree was then restored from the saved originals;
`git status --short` prints nothing.

The planted Windows arm in the Linux container, rerun on the committed tree (`git archive HEAD seed
runtime selfhost`, stage 2 `sources 110fad075b8d3bca`; a first run had copied my worktree mid-move
and read `sources 1da29aca4144daf5`, discarded): **build exit 0, runs green**, as on this Mac.

## Verdicts

### Question 2, where `crypto/sha256`'s C comes from

- **(d) Heroes alone: approve.** Section: design.md:568-569, *"written in Heroes alone where Heroes
  can say it, over a C library through a binding it carries where it cannot"*, ratified by panel 234
  after the milestone page's row (line 32, measured 2026-09-03) was written. 288 lines, 0 of C,
  correct on 14 files at both levels on this Mac, Linux arm64 and the Windows box, no install step,
  no dynamic dependency. Its cost is speed, 4.0 times (a) at `-O2` and 29 times at `-O0` on 64 MiB
  here, and CLAUDE.md § 13 makes speed no justification unless it stops a needed program; 0.60 s for
  4 MiB at the default `-O0` stops none I can name.
- **(a) `pkgconfig "libcrypto"`: object, not veto.** The binding is the best FFI in this sitting:
  nine members, 0 lines of C, verified by clang (the `unsigned int *` mutant refused), unchanged from
  OpenSSL 4.1.0 to 3.5.7. As core's route it is wrong on design.md:568 (Heroes can say it, measured)
  and on the platforms: refused `ffi_pkgconfig` on the Windows box, and on this Mac the binary names
  Homebrew's absolute `libcrypto.4.dylib` path (`otool -L`; not run on a Mac without it). Its two
  traps are recorded for whoever binds it: `EVP_sha256() -> Md` is refused (`const EVP_MD *`, and
  the spec has no const handle result; `-> ptr` builds untyped), so `EVP_MD_fetch`; and `SHA256_Init`
  and its siblings are `OSSL_DEPRECATEDIN_3_0`. It stays the route a program takes by binding
  libcrypto itself.
- **(b) the package's own C header: object, not veto.** Sound and verified (the `ffi_field_type`
  mutant refused at the package's own struct), 69 + 122 lines, portable. **design.md:571-575 does
  not cover it**: it refuses *"platform C of its own"*, and this C has no platform arm; I searched
  design.md for a package's own header (`header of the package's own`, line 2684, which sanctions a
  `static inline` *rename*) and found no sentence on a package carrying an algorithm in C. What does
  reach it is design.md:599-601, *"A thin C shim remains standard practice for C++ libraries and
  awkward struct-passing ... shims are for the hard cases only"*: 122 lines of hand-written SHA-256 is
  no shim and no hard case, it is a standard library written in C, §1.11's refusal, and C beneath the
  language's robustness (§1.12) that no upstream maintains. The sitting should say so in a sentence.
- **(c) the runtime's third door: object, not veto.** design.md:575, *"the language still ships its
  runtime and nothing a program could carry instead"*: (a), (b) and (d) are three programs carrying
  it. Measured besides: a package on (c) is `ffi_unknown_name` against any runtime without the door,
  so its version is chained to the compiler's; moved into `hero_os.h` it costs the lines above and
  decision 2228's promise of a SHA-256 in the runtime from `v1.0.0`. No veto: `HERO_RUNTIME_ABI`
  moves by 0 and the full suite passes.

**The compiler keeps what it has**: `hero_file_sha256` and `hero_text_sha256` stay in
`hero_compiler.h`, the compiler's own and unpromised, where panel 235's R6 put them for the `-O0`
cost; nothing in this sitting needs them moved.

### Question 3, the environment's write

- **An `os` package's own header with a platform arm: veto.** Section: design.md:572 (*"none ships
  platform C of its own"*) and panel 234's R6, which rests on this seat's veto of panel 114's R3.
  Measured again: the planted `_putenv_s(name, value, 1)` builds at exit 0 and runs green on this Mac
  and on Linux arm64, and is refused only on the Windows box. §4.19's promise that a wrong signature
  is a compile error is unkept on two platforms of three. The program that breaks: any `os` package
  whose dead arm is wrong, shipped green from every Mac and Linux leg to its Windows users.
- **A package binding `hero_env_put` from `hero_compiler.h` as it stands: object.** That header
  says *"which no program needs"* and is unpromised, and on Windows its value is invisible to the
  program's own `getenv` (`null_cstr`, measured).
- **`hero_env_put` moved into `hero_os.h`, with its Windows arm over `_wputenv_s`: approve.** Spec
  § 13 already names `hero_os.h` for processes, its read `hero_env_shown` is already there, and panel
  234's R6 keeps *"hero_os.h's process"* in the runtime, so no spec token moves and no archive is
  needed. Its cost, an inference from the four-name run: one line in each of `guarded_names.hero`,
  `heroes_guard_open.h` and `heroes_guard_close.h`, its five-line declaration moved, ABI 0, no
  binding edited. The `_wputenv_s` line is measured: parent, process block and child all read
  `café-2` on the box.
- **Feature 2295's archive** is not needed by any route I approve, so this sitting owes the spec
  nothing and stays in its lane.

### Question 1, from the boundary only

**No `core` module over C may enter the compiler's closure** (object; it is not on the table today):
route (a)'s emitted C under plain `clang -I runtime ... runtime/runtime.c` is `'openssl/evp.h' file
not found`, then five undefined `_EVP_*` symbols, so a compiler over it could not be built from C
alone on the Windows box, where no OpenSSL is installed. A Heroes-only module such as `core/strings`
costs the boundary nothing.

### Prediction

At M-core-packages' close, a `crypto/sha256` in Heroes alone gives `shasum -a 256`'s digest for
every file of its tests on all four CI legs, Windows included, with no line of C and nothing
installed, and runs within 5 times a nine-member libcrypto binding at `-O2` on 64 MiB on the macOS leg
(4.05 here: 0.81 s against 0.20); and `hero_env_put`, moved into `hero_os.h` over `_wputenv_s`, lands
with `HERO_RUNTIME_ABI` unmoved, the compiler's own tests all passing, and a program's `getenv` on the
Windows leg reading what it wrote.

### What would change these verdicts

- (d) to object: a named program the milestone needs that a Heroes SHA-256 at `-O0` stops from
  running (a budget it cannot meet), measured.
- (a) to approve: the sitting holding the page's row over design.md:568, and the Windows leg given
  OpenSSL and a `pkg-config` by the project; then with `EVP_MD_fetch`, `sum256 -> str?` and the
  platform refusal on the package's card.
- (b) to approve: a ruling, in design.md, that a package may carry portable C beyond a shim, with
  what holds that C to §1.12.
- The veto on question 3's header: a compiler that verifies every arm of a header on every machine
  (panel 114's R3 instrument), which does not exist.

## What stayed unrun

- Route (a) on Windows with OpenSSL and `pkg-config` installed (installing is outside my folder).
- Route (a)'s binary on a Mac without Homebrew's `openssl@4` (the dependency is read from `otool -L`).
- Any CI leg; the x86-64 Linux container.
- The per-unit cost of four more guard pairs; the latency instrument (the machine was never still:
  load 3.1 to 5.0).
- `hero_env_put` alone moved (the four-name move was run; one name's three lines are arithmetic).
- A child started by the runtime's own `hero_run_go` rather than `system()`; the process block it
  inherits was read through `hero_env_shown` instead.
