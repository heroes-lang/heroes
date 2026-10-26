# 055: one binary that is clang and lld, linked from LLVM's own release, stands in for the machine's toolchain: the compiler's C identical byte for byte, its 1,750 tests passed, 18 programs printing the same, at 191,569,360 bytes

Measured 2026-10-26 by the session heroes-lang-3f, on the author's choice of
that day: M-standalone-compiler's row, and a measured probe of clang embedded
before it opens (`docs/roadmap/milestones/M-standalone-compiler.md`). On this
Mac, a `Mac15,13` with 24 GiB and four performance and four efficiency cores,
macOS 27.0.1, whose own toolchain is Apple clang 21.0.0 (clang-2100.3.34.2)
with the macOS 27.0 SDK; from the lane `lane-standalone-compiler` at
`762cdd5b7`, its compiler stage 2 (`sources 413da33e7969760a`). The evidence
is the archive beside this file, `055-one-binary-that-is-clang-and-lld-linked-from-llvms-own-release-stands-in-for-the-machines-toolchain-the-compilers-c-identical-byte-for-byte-and-its-1750-tests-passed.tar.xz`: the probe's scripts, its one C++
file, its logs and the four release archives' listings. LLVM's own driver
sources are not in it; they are LLVM's at the tag `llvmorg-23.1.4`:
`clang/tools/driver/driver.cpp`, `cc1_main.cpp`, `cc1as_main.cpp`,
`cc1gen_reproducer_main.cpp` and `lld/tools/lld/lld.cpp`. The working folder
`.claude/worktrees/standalone-probe/` (2026-10-26) is scratch.

## 1. The release archives, read from their listings

LLVM 23.1.4, published 2026-10-20, the newest release that day. Each archive
was streamed through `zstd -dc --long=31 | tar -tvf -` and nothing of it written
to the disk but its listing:

| archive | compressed | extracted | clang's `.a`/`.lib` | lld's | LLVM's | the release's own clang, lld |
|---|---|---|---|---|---|---|
| `LLVM-23.1.4-macOS-ARM64.tar.zst` | 873,640,882 | 7.60 GiB | 90, 434.2 MB | 6, 23.4 MB | 213, 521.2 MB | 196,018,976 and 146,159,616 |
| `LLVM-23.1.4-Linux-ARM64.tar.zst` | 1,101,887,514 | 11.21 GiB | 90, 238.0 MB | 6, 13.0 MB | 222, 329.2 MB | 342,358,624 and 202,116,680 |
| `LLVM-23.1.4-Linux-X64.tar.zst` | 1,190,060,068 | 11.56 GiB | 90, 237.2 MB | 6, 13.3 MB | 222, 338.3 MB | 282,507,120 and 213,124,752 |
| `clang+llvm-23.1.4-x86_64-pc-windows-msvc.tar.zst` | 276,626,313 | 3.99 GiB | 90, 284.5 MB | 6, 16.1 MB | 147, 296.2 MB | `clang.exe` 113,283,072 and `lld-link.exe` 76,339,712 |

So every leg of the CI has, in LLVM's own release, the static libraries a
program needs to hold clang and lld, and none of them builds LLVM. The macOS
archive downloaded matched GitHub's digest,
`sha256:0f8b0db925ab54a63028d788e85b14abaa25492e9608819728c17f6786948da4`.

**And every one of those libraries is LLVM bitcode, not machine code**: the
first member of each of the macOS archive's 309 static libraries reads `LLVM
bitcode, wrapper` to `file`. Apple's linker refuses it: *could not parse
bitcode object file ... 'Not an int attribute (Producer: 'LLVM23.1.4' Reader:
'LLVM APPLE_1_2100.3.34.2_0')*. A program holding them is linked by the
release's own clang and lld, as a ThinLTO (section 4).

## 2. The release's clang and lld in the machine's place, `heroes` untouched

`clang` first on `PATH` was the release's `clang-23`; beside it, `ld64.lld`
(the release's `lld`) and a two-line `ld` that runs it, because clang's Darwin
driver runs the `ld` that stands beside it. `SDKROOT` named the SDK `xcrun`
names, because the release's clang finds none by itself: without it
`#include <stdio.h>` fails, *'stdio.h' file not found*, where Apple's clang
finds the SDK alone.

- **`heroes doctor`**: every row `ok`, *clang 23 against a floor of 18*.
- **A C program**: linked by `ld64.lld`, signed by the linker itself
  (`codesign -dv`: `flags=0x20002(adhoc,linker-signed)`), linking
  `/usr/lib/libSystem.B.dylib` alone; the three link words `heroes` gives on
  Darwin, `-rdynamic`, `-Wl,-reproducible` and `-Wl,-dead_strip`, each taken
  with exit 0 and nothing said.
- **21 programs**, the gallery's `00-first`, `08-ffi` and `13-lease` and the 18
  examples whose `main.hero` declares an `extern` group, `sqlite` against the
  SDK's own `sqlite3.h` and `curl`, `raylib` and `sdl` among them: each built
  with Apple's toolchain and with the release's, 21 of 21 both ways and no
  warning in any build's output; the 18 that need neither the network nor a
  window run both ways under `timeout 20`, their output the same by `cmp`. The
  binaries' `LC_BUILD_VERSION` says `tool 3` (ld64) for Apple's and `tool 4`
  (LLD) for the release's.
- **The compiler's own C**: `heroes build selfhost/main.hero --emit-c` wrote
  27,000,911 bytes under each toolchain, identical by `cmp`. That emission
  holds clang's answers to the FFI's probes, so Apple's clang 21 and LLVM's 23
  answered the compiler's every question the same.
- **The compiler built by the release's clang and lld** (`tool 4`): `heroes
  0.5.0`, `sources 413da33e7969760a`; its own tests, run by itself with that
  toolchain: **1,750 tests, all passed**.
- **The sanitizers**: `heroes build --sanitize` built and ran the gallery's
  first program, and a C program writing past a heap block was told
  *AddressSanitizer: heap-buffer-overflow ... WRITE of size 4*. The sanitized
  binary loads `@rpath/libclang_rt.asan_osx_dynamic.dylib`, its `LC_RPATH` the
  resource directory's absolute `lib/darwin`, so that library must stand on
  the disk wherever a sanitized program runs.

These are the evidence's `b-` files. **A first pass is discarded, and why.** It gave clang the linker through
`CCC_OVERRIDE_OPTIONS='#+-fuse-ld=lld'`, which reaches every call; on every
call that only compiles, clang said *argument unused during compilation:
'-fuse-ld=lld'*, and three of the compiler's tests, which hold clang to
silence or to `-Werror`, failed on that sentence (1,750 tests, 3 failed). The
probe's artefact, not the toolchain's: with the `ld` beside clang instead and
no override, the same run read 1,750 passed, and both passes of the 21
programs read the same. The discarded pass is the `a-` files, its summary alone:
the log of its 1,750 tests was written over by the pass that replaced it.

## 3. What must stand on the disk beside such a binary, on this Mac

- **clang's resource directory**, which the driver finds beside itself: its
  `include/`, 325 files and 16,049,877 bytes (`stddef.h`, `stdatomic.h`, the
  headers clang supplies itself), and its `lib/`, 23 files and 31,365,152
  bytes, the sanitizers' runtimes among them (8 `.dylib`, 4,389,600 bytes;
  AddressSanitizer's 1,079,248, UndefinedBehaviorSanitizer's 421,248).
- **The SDK**: every build above together named 425 distinct files of it in
  the lane's build cache, 2,816,166 bytes, 424 of them under `usr/include`; a
  link reads `libSystem.B.tbd`, 347,931 bytes, since `/usr/lib/libSystem.B.dylib`
  is not on the disk. The whole SDK is 299 MB. Shipping those files instead
  of asking the machine for them is the question of their licence, unread.
- **For comparison, what Zig 0.15.2 ships for the same purpose**, measured in
  its `lib/libc/`: `include/any-macos-any` 7.3 MB and `darwin` 328 KB (its
  `libSystem.tbd` among them); `include/any-windows-any` 68 MB and `mingw`
  18 MB; `include/any-linux-any` 8.6 MB, `generic-glibc` 3.4 MB and `glibc`
  2.1 MB.

## 4. One binary that is clang and lld

- **The program**: `scripts/heroes_tools.cpp` in the evidence, 62 lines, whose
  `main` takes its role from the name it was started under (`clang`,
  `clang++`, `ld`, `ld64.lld`, `ld.lld`, `lld-link`), from a first word
  `clang`, the shape `heroes` itself would take as `zig cc` does, or from a
  first argument opening `-cc1`. That last is how clang's driver runs a compile
  it does not keep in its own process: with `-c` alone the compile ran
  in-process, and with a compile and a link in one command the driver ran this
  executable's real path with `-cc1` first, which a first version of the `main`
  refused. Beside it, the release's five driver sources, 1,916 lines. All six
  compiled by Apple's clang++ with `llvm-config --cxxflags`, and no warning.
- **The link**, by the release's `clang-23` with `lld`, ThinLTO, four jobs,
  `-dead_strip`: the release's static libraries of clang (90), lld (6) and LLVM
  (213), and Polly's two, 6,729,640 and 5,875,024 bytes, which the release
  links into its tools, so clang's CodeGen and LLVM's LTO call
  `getPollyPluginInfo()`: the first link, without them, failed on that symbol
  after 242.69 s, though no driver source names Polly. Then `-lm -lz -lxml2`
  and a static zstd that the release's `llvm-config` names by the path of the
  machine it was built on, `/opt/homebrew/lib/libzstd.a`, which this Mac has
  from Homebrew.
- **The binary**: 191,569,360 bytes with every target in it, 165,293,352
  after `strip -x`; the release's `clang-23` alone is 196,018,976, and `heroes`
  today 13,898,616. Compressed for a download it is 44,732,189 bytes by `zstd
  -19` and 37,829,816 by `xz -9`, where `heroes` today is 4,679,286 by `zstd -19`.
  The link that produced it took 280.61 s of wall and
  1,061.16 s of CPU, the one before it 317.20 and 1,175.29: single runs beside
  two other sessions' work, an order of magnitude and not a timing in CLAUDE.md
  § Verification's sense. The ThinLTO cache they leave is 261 MB.
- **The same battery as section 2, with this one binary standing as `clang`,
  `ld` and `ld64.lld`** and no other LLVM program on `PATH`: 21 programs built
  both ways with no warning, the 18 run printing the same by `cmp`, their
  binaries `tool 4`; the compiler's C, 27,000,911 bytes both ways, identical;
  the compiler built by the one binary, `tool 4`, `heroes 0.5.0`, `sources
  413da33e7969760a`; its own tests, run by itself with the one binary: **1,750,
  all passed**. The scripts' labels say *the release clang* for this run too:
  `STAGE` named the one binary's folder, and its logs are the evidence's `c-`
  files.

## 5. What this does not show

- **No run on Linux or Windows.** Their archives were read from their listings
  alone.
- **The size of a binary for AArch64 and X86 alone.** `lld`'s own code in the
  release's bitcode initialises every target for its LTO, so a two-target
  binary needs LLVM built from source with two targets, which this Mac, at 41
  GiB free that day, was not asked to do.
- **The licences** of Apple's SDK headers and stubs, of Microsoft's C runtime
  and of mingw-w64 and glibc's headers, which *everything, as Zig does* would
  ship: not read.
- **Windows' target**: `heroes` builds for `x86_64-pc-windows-msvc`, whose
  `lld-link` needs Microsoft's `libcmt.lib`; Zig's standalone shape is
  mingw-w64's `windows-gnu`. Unrun.
- **Every header of every library**: the two clangs answered this tree's 21
  programs and the compiler the same, which is not every C library a program
  may bind.
- **The `heroes` side**: a word that runs clang, the resource directory written
  to the disk once and kept, `-isysroot` given: unwritten, the opening
  sitting's.
