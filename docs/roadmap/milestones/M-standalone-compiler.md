# M-standalone-compiler — `heroes` needs nothing else installed

**Scheduled 2026-10-26, at row 82, by the author's instruction**, immediately
before M-install-channels, meant as: *search for a C compiler to embed in the
Heroes compiler, compatible with Windows, Mac and Linux, so that Heroes is a
single executable; then we evaluate it and work out what we must do*. Asked
that day what must disappear from the machine, the author answered
**everything, as Zig does**: no clang, and no macOS SDK, no Visual Studio Build
Tools and no C library development package either. A library a program binds
through an `extern` group stays the program's own to install (spec § 13): its
header and its library are what that program is about. Asked how to go on, the
author chose this row and a measured probe of clang embedded, before the
opening.

**What the compiler asks of clang today, measured 2026-10-26 at `1b3bb242f`.**

- `heroes` runs the program `clang` from `PATH` by argument list
  (`runtime/parts/run.c:1061` on POSIX, `:1337` on Windows): one unit per
  module, the runtime, the link, and `clang -###` for its identity, held to a
  floor of 18 (`selfhost/cli/clang_floor.hero`).
- clang is also the FFI's oracle. The word `"clang"` stands **78** times in the
  code of **31** modules of `selfhost/`, `test` blocks and comments left out.
  Eleven of those calls ask what only clang's front end prints: `-Xclang
  -ast-dump` 8 times in 6 modules, `-fdump-record-layouts` twice and `-Xclang
  -ast-list` once. Beside them, `-fsyntax-only` 21 times, the preprocessor's
  `-E` 12, `-dM` 5, `-dD` 3, `-M` once, and `-fsanitize` 6.
- The emitted C leans on builtins clang and gcc share. The seed's code, its
  `HERO_STR_STATIC` lines left out, holds **5,811** `__builtin_add_overflow`,
  **1,587** `__builtin_sub_overflow` and **94** `__builtin_mul_overflow`, the emitter's form
  for `+`, `-` and `*`, since overflow aborts at every width (spec § 7). The
  runtime header every unit includes needs `__builtin_strlen` folded to a
  constant in each of the seed's **7,673** static string literals
  (`runtime/heroes_runtime.h:252`) and asks `__atomic_always_lock_free` inside a
  `_Static_assert` (`:193`). The 638 units of `tests/emission/` hold **1,200**
  `__builtin_classify_type` and **20,414** `#pragma clang diagnostic` lines, the
  regime under which the FFI's probes turn a warning into an error whose clang
  wording `selfhost/emit/ffi_answer.hero` reads back.
- `--sanitize` is clang's AddressSanitizer and UndefinedBehaviorSanitizer, and
  LeakSanitizer judges every program with an `extern` on the CI's Linux leg
  (CL-055).

**The candidates, read at their sources on 2026-10-26.** The list is the small
C compilers the coordinator knew and searched for, plus clang itself embedded,
so it is a measurement of the searcher's vocabulary (CLAUDE.md § RUN IT):

- **TinyCC**, its `mob` branch: code for i386, x86-64, arm, aarch64 and riscv64,
  and its own linker for ELF, PE and Mach-O (`tccelf.c`, `tccpe.c`,
  `tccmacho.c`). Its CI builds it and runs its tests on Linux x86-64 and
  aarch64, macOS 15 on Intel and on arm64, and Windows x86-64, i386 and arm64,
  and its ten newest runs, 2026-07-31 to 2026-10-16, passed. Its token table
  names no `__builtin_add_overflow`, `sub` or `mul`, no `__builtin_strlen`, no
  `__builtin_classify_type` and no `__atomic_always_lock_free`, and it has the
  C11 keywords. `tccmacho.c` writes no code signature (it runs `codesign -f -s
  -` when built with `CONFIG_CODESIGN`; on this Mac `/usr/bin/codesign` is on
  the system volume, not in the Command Line Tools) and reads the SDK's `.tbd`
  stubs. It prints no syntax tree and no record layout,
  and has no sanitizer. LGPL-2.1, with a relicensing to an MIT text that most
  of its authors have agreed to and not all (`RELICENSING`).
- **cproc**: Linux and FreeBSD, needing QBE, an assembler, a linker and an
  external preprocessor (its README).
- **xcc**: runs on Linux or macOS and writes ELF64 alone (its README).
- **MIR's c2m**: C11 into MIR's JIT, on Linux and macOS; its README names no
  Windows, and its output is not a native executable.
- **chibicc, slimcc, kefir**: Linux only to the coordinator's knowledge, not
  re-read that day.
- **clang and lld inside the binary**, the shape Zig ships as `zig cc`. On this
  Mac, zig 0.15.2 is one binary of **188,226,560** bytes beside a `lib/` of
  **209 MB**, **159 MB** of it `lib/libc/`: headers and stubs for glibc, musl,
  mingw-w64 and macOS, its own `libSystem.tbd` among them. Zig's own issue
  [16270](https://github.com/ziglang/zig/issues/16270) puts the cost of
  depending on LLVM, Clang and LLD at a binary of about 150 MiB against 5.
  LLVM's licence is Apache-2.0 with LLVM Exceptions.

**What the reading says, an inference until the probe runs.** TinyCC could
replace clang as the thing that compiles and links, and not as the oracle: the
eleven questions about syntax and layout have no TinyCC answer, the builtins
the emitted C and the runtime header use are missing, and robustness (§1.12,
CLAUDE.md § Precedence, rank 3) would lose its sanitizers. clang and lld inside
`heroes`, as Zig carries them, keep every question and every answer as they
are: `heroes` would run itself as clang wherever it runs `clang` today. And
design.md §3.2's *`tcc` is not usable on Apple Silicon* is not true of
TinyCC's `mob` branch on 2026-10-26, by its CI's own configuration (read, not
run here); correcting the sentence is the opening sitting's, design.md Part 3
being the panel's (CLAUDE.md § 4).

**The part no embedded compiler removes, measured on this Mac.**
`/usr/lib/libSystem.B.dylib` is not on the disk, macOS keeping it in the shared
cache, so any linker needs `libSystem.B.tbd`, which the Command Line Tools' SDK
holds. On Windows, clang's `lld-link` needs the MSVC C runtime's `libcmt.lib`
and the Windows SDK (`docs/platforms/windows/WINDOWS-MACHINE.md:403-411`). On
Linux, the C library's development package. *Everything, as Zig does* means
carrying those too, Zig's way, and that is where the licences of Apple's SDK
and of Microsoft's C runtime enter, and on Windows the target itself:
`x86_64-pc-windows-msvc` today, mingw-w64 in Zig's `windows-gnu`. Zig's own
measured on this Mac: `lib/libc/include/any-macos-any` 7.3 MB and
`lib/libc/darwin` 328 KB; `any-windows-any` 68 MB and `mingw` 18 MB;
`any-linux-any` 8.6 MB, `generic-glibc` 3.4 MB and `glibc` 2.1 MB.

**What it delivers.** On each of the four platforms, a machine with no C
toolchain installed builds and runs a Heroes program with `heroes` alone, and a
program that binds a library builds where that library is installed. Every
question the compiler asks clang today is still asked of a clang, so what a
program is told does not change with the machine it is built on.

**The probe before the opening**, the author's choice of 2026-10-26: measured
in a lane of its own from LLVM 23.1.4's own release, whose macOS arm64 archive
is 873,640,882 bytes, and never built from source on this Mac, which had 43 GiB
free that day. It asks four things: the size of a binary that carries clang and
lld for AArch64 and X86; whether clang's driver runs inside another program as
`zig cc` runs it; which runtime libraries the sanitizers need and where they
must stand on the disk; and what each platform still needs beside the binary.
Its numbers are measurement 055, and the sitting reads them: **run the same
day**, one binary of 191,569,360 bytes holding clang and lld, linked from the
release, stood in for this Mac's toolchain with `heroes` untouched, the
compiler's own C identical byte for byte and the compiler it built passing its
1,750 tests. **Read the same day from the archives' own listings**, each streamed and none written to
the disk: LLVM 23.1.4's release for macOS arm64, Linux x86-64, Linux arm64 and
Windows x86-64 each carries clang's 90 static libraries, lld's 6 (ELF, COFF and
MachO among them) and LLVM's 147 to 222, so no LLVM is built on any of the four
legs: a binary that holds clang and lld is a link. The macOS archive extracts
to 8,161,132,896 bytes, of which its static libraries are about 979 MB; its
`clang-23` alone is 196,018,976 bytes and its `lld` 146,159,616, with every
target in each.

**The opening convenes a sitting**, because what it decides is architecture:
the toolchain behind the backend, a word of `heroes` that runs clang, CLAUDE.md
§10's *never a second binary*, the seed's one clang line (`seed/README.md`),
and the licences of what travels with the binary.

**Why here.** Immediately before M-install-channels, whose premise this row
reverses: that page says no channel ships a prebuilt binary because `heroes`
without clang compiles nothing, and once this row is delivered a prebuilt
binary is the honest shape. The same premise holds up CL-067's *no uploaded
binary* for a release (`docs/records/contract/case-law.md`, which cites the
refusal of prebuilt binaries as a decoy without clang), so the sitting that
opens this row also reads whether a release uploads one. After M-deployable-binary, which asks what the
machine that runs a Heroes program needs; this row asks it of the machine that
builds one.

**What it does not deliver**: a second backend, which is M-qbe-backend's; the
latency question of M-latency-ceiling, whose page asks whether tcc compiles the
emitted C for a development build, a question this row leaves where it is; any
outward act.
