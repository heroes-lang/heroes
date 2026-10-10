# Panel 205, ffi-pragmatist

Copied by the coordinator at 03:12 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 205, ffi-pragmatist

Started 02:52 and reported at 03:11 (`date`). Everything was run in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/205-ffi-pragmatist/`: the tree was rsynced from `lane-panel-205`, its `.git` removed, no git run in it, and the compiler built from the seed (`heroes 0.2.0`). Toolchains: Apple clang 21.0.0 on this Mac, and Debian clang 22.1.8 in `heroes-linux-arm64:latest`. Running notes are in `notes.txt`.

## verdict

- **Defect 570, route C: approve. It is required under every route, and I veto any Q2 relaxation that lands without it.** C means raising again, after the groups' close, every warning a check rests on: the command line's eight `-Werror` plus clang's default errors (`-Wint-conversion`, `-Wincompatible-function-pointer-types`, `-Wimplicit-function-declaration`). The probe's four warnings stay inside the probe region. My first version raised them for the whole unit and made `examples/tally` exit 2 with an internal error, because the emitted code narrows `int64_t` to `uint8_t` at `:132`.
- **Q2: approve route P2.** The compiler's own guard headers push the diagnostic state and ignore the headers' style warnings: `-Wall`, `sign-conversion`, `shorten-64-to-32` and the two pointer-type ones. Inside that region, `return-type`, `uninitialized`, `conditional-uninitialized` and R2's `macro-redefined` stay errors. The guard pops after the groups, then route C runs.
  - **Object** to turning a package's `-I` into `-isystem`.
  - **Refuse on measurement** `-Wsystem-headers`.
- **Q1: approve the critic's route B**: a prefix that reads no libc header before the groups.
  - **Object** to a compiler-wide `-D_GNU_SOURCE` (panel 076's condition stands).
  - **Object** to a group form that names a macro.
  - Route A (a package's `-D`) already exists, but it applies to the whole program and its message is false. That should be filed.

## section

- design.md §4.19 `:2466-2476` (*every `extern` carries a `_Static_assert` … **Clang verifies the declared signature against the real header** is therefore a true sentence*).
- spec § 13 (*a parameter … that disagrees is refused*).
- §1.11 and §1.12.
- design.md `:3744` (*no warning level*).
- **design.md says nothing about feature-test macros, or about how a warning inside a header's own code is judged.** grep finds neither; `:2673` mentions `-isystem` only for `@file`. Panel 204 R1's §4.19 sentence (*a header of the program's own is the one place a switch goes*) is ratified but not landed, and it is false today for every switch that the first libc header locks in.

## experiment

**570, widened** (Mac, frozen compiler, `cases/w/`). `leak.h` is six `#pragma clang diagnostic ignored` lines with no push or pop, then `stdio/stdlib/string/dirent`. Each binding is shown with its result through a plain header, then through `leak.h`:

| binding | plain header | through `leak.h` |
|---|---|---|
| `abs(x: u32)` | `ffi_parameter_type` | exit 0, prints `5` |
| `fclose(f: Dir)`, where `Dir` is `tag DIR` | `ffi_parameter_type` | exit 0 (defect 029's class) |
| `abs(x: ptr)` | `ffi_parameter_type` | exit 0 |
| wrongly typed `atexit` callback | `ffi_callback_type` | exit 0, prints `0 0` |
| `getpid(x: f64) -> i32`, through a header declaring nothing but `ignored "-Wimplicit-function-declaration"` | `ffi_unknown_name` | exit 0, `true`, **0 warnings** |
| `strtok(s: cstr …)` | refused `ffi_writable_parameter` | still refused |

**Route C and route P as prototypes, with no compiler change.** They are copies of `runtime/` with edited `heroes_guard_open.h` and `heroes_guard_close.h`, selected through `HEROES_RUNTIME` (`rtC/`, `rtP/`, `rtP2/`).
- All six leak shapes are refused, with first lines byte-identical to the plain header's.
- `--emit-c` of `examples/sqlite` gives the same hash under both runtimes, so nothing crosses the boundary differently.
- All 20 `examples/` programs with an `extern` build at exit 0 with 0 warnings under rtC, rtP and rtP2 (built, not run).

**GMP and FFmpeg on this Mac** (`cases/g/`):

| case | today | route C alone | route P / P2 |
|---|---|---|---|
| `g`, version constant | refused `ffi_header_refused` | refused `ffi_header_refused` | prints `6` |
| `av`, `avutil_version` | refused | refused | prints `3998054` |
| right `__gmpz_init_set_si(r: ptr, v: i64)` | refused | refused | builds |
| wrong sign (`v: u64`) | refused | refused | `ffi_parameter_type` |
| wrong handle (`Mpq` passed to `__gmpz_clear`) | refused | refused | `ffi_parameter_type` |
| wrong sign `av_log_set_level(level: u32)` | refused | refused | `ffi_parameter_type` |
| `__gmpz_init_set_si(r: ptr, v: i32)`, against `long` | refused | refused | builds (an exact conversion spec § 13 admits) |

**The environment flip.** `gl.hero` (`link "gmp"`): today `CPATH` refuses it and `C_INCLUDE_PATH` builds it. Under rtP2 both print `6`.

**A program's own header.** Today `inl.h` is refused at line 2 (a sign conversion in its own code); under P2 it is refused at line 3 (a missing return, which stays an error). The critic's `wh.hero` raw warnings go from 2 to 0.

**C level** (`sys/arms.sh`):
- A callee that is an object-like macro alias keeps its sign and handle refusals under today's flags, `-isystem`, P and C.
- **A residual every arm misses:** `#define pragmafn _Pragma("clang diagnostic ignored \"-Wsign-conversion\"") realfn` defeats the sign check on its own probe and on every probe after it. grep finds 0 such macros in Homebrew's headers. A split probe (take the pointer, re-raise on its own line, then call) is refused (`sys/split.c`, rc 1). It is not built into the compiler.
- **The `EXTERN` pair** (two headers defining it differently): exit 0 with a raw warning today, exit 0 silent under `-isystem`, refused `macro-redefined` under P.

**Q1, Linux** (`q1/q1-linux.txt`). 25 (header, name) pairs were tried. `sigabbrev_np` was a bad pair: I named the wrong header, so no arm finds it. Of the 24 valid names, 18 are hidden with no macro:
- 14 need `_GNU_SOURCE`: `sched_getcpu`, `strverscmp`, `pipe2`, `dup3`, `execvpe`, `accept4`, `dladdr`, `O_DIRECT`, `O_TMPFILE`, `pthread_setname_np`, `memfd_create`, `mremap`, `ppoll`, `exp10`.
- 4 need `_XOPEN_SOURCE`: `strptime`, `wcwidth`, `ptsname`, `posix_openpt`.
- For each of the 18: a group naming an own header that defines the macro finds nothing. The same own header placed before the prefix finds it (`OK`), and so does `-D` (`OK`).
- With the header before the prefix, wrong bindings stay refused (wrong arity, `uint32_t` to `int`, `int64_t` to `int`), and right ones compile (`q1v-linux.txt`).

**Route B in plain C** (`rb/td.c`). Integer types are spelled with `typedef __INT64_TYPE__ int64_t;` and its siblings, then `stdbool`, `stddef`, the switch header, `<stdint.h>` and `<math.h>`, compiled with the flags:
- Mac: `_POSIX_C_SOURCE` first hides `strlcpy`, as C means. Without the switch it is visible, with 0 warnings.
- Linux: `sched_getcpu` is visible only with the switch.

**Route A's scope** (`scope/`, Linux). A package's `-D` applies to the whole program.
- `w2.hero`'s `wcwidth(c: u32)` builds and prints `1` only when `main` names `package "ncursesw"`. Without that it is refused with *`wchar.h` declares no `wcwidth`*, which is false.
- The cache keeps the verdict right in both build orders.

## census (a sample, said as a sample)

**Mac**: panel 204's 182 entry headers in the unit's shape (prefix and guard), `census/mac.tsv`:

| arm | fail | builds but prints raw warnings |
|---|---|---|
| today | 16 | 2 (`jq.h`, `jv.h`, `-Wunused-function`) |
| `-isystem` | 10 | 0 |
| P | 10 | 0 |
| P2 | 10 | 0 |
| no warning flags | 10 | 0 |
| `-Wsystem-headers` | 17 (adds `pa_mac_core.h`) | 55 of 165 (`stdio.h` alone 148 `nullability-completeness`) |

The 6 that `-isystem`, P and P2 let through are gmp, avutil, avcodec, avformat, swscale and libfdt, all failing today by `sign-conversion` inside their own code. Under P and P2 none newly fails. The other 10 fail in every arm, for missing prerequisites or configuration.

**Linux arm64**: 85 headers. For the user-directory arms, the headers of 35 entry headers' Debian packages (428 files) were copied into `census/linux-u`:

| arm | fail | notes |
|---|---|---|
| installed | 2 | `llvm-c`, `p11-kit` (not found) |
| copy as `-I` | 3 | adds `gmp.h`, by `sign-conversion` |
| copy as `-isystem` | 2 | |
| copy with P | 2 | |
| `-Wsystem-headers` | 4 | adds gmp and `linux/videodev2.h`; all 81 that build print raw warnings |

With `-Wsystem-headers`, every one of those 81 includes glibc's `stdint.h` redefining clang's own `__INT64_C`. So R2's flag together with `-Wsystem-headers` would refuse every Linux unit (an inference; the combination was not run).

**Pragmas in the wild:**
- Homebrew: 34 header files carry a diagnostic `ignored` or `warning` pragma, and 5 leave one unbalanced: SDL2 and SDL3 (`-Wpragma-pack`), lzo (`-Wundef`), and two NSS files.
- Debian `/usr/include`: 52 such files.
- Neither ignores a warning a check rests on.

**`.pc` files answering a feature macro:**
- Debian: 12 of 52 packages (the ncurses family) answer `-D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600`.
- Mac: 2 of 505 (`sdl2`, `sdl2-compat`) answer `-D_THREAD_SAFE`.
- Separately, glibc defines `_DEFAULT_SOURCE` itself under `-std=gnu11` with no macro set.

## argument

570 shows the probe is only as strong as the last pragma before it. One header line reopened five of six parameter checks, defect 029's handle mismatch among them, and accepted `getpid(x: f64)` with no warning. Route C closes all six with byte-identical messages, so any relaxation must come with it. With C in place, P2 lets GMP and FFmpeg through whatever the install route or environment variable. It keeps every wrong binding I wrote refused, keeps R2 and its own missing-return error, and leaves the emitted C byte-identical. `-isystem` blinds R2 and still depends on the install route. For Q1, the C a binding needs is `#define _GNU_SOURCE` before any header, as glibc documents. Route B makes that work with no new form, and wrong bindings are still refused.

## prediction

- With P2 and C landed, on this Mac:
  - `p205/gmpmac/g.hero` prints `6`.
  - `cases/g/gsign.hero` and `hole570/leak.hero` are refused `ffi_parameter_type`.
  - The 182-header census moves exactly 6 headers to OK and 0 to FAIL.
  - The 20 `examples/` programs with an `extern` build at exit 0 with 0 warnings.
- With route B landed, on Linux arm64, a module whose first group is an own header (`#define _GNU_SOURCE 1` then `<sched.h>`) binds `sched_getcpu() -> i32` and prints `true`, and `sched_getcpu(x: i32) -> i64` is refused.

## condition

- **P2 turns into an objection** if a real header that builds today is refused under P2 and C, or if a wrong binding refused today is accepted under them (0 of 182 + 85 headers and 0 of 10 shapes so far), or if Windows' clang treats the guard pragmas differently.
- **B turns into an objection** if the builtin integer typedefs disagree with libc on Windows. I would then take the own header before the prefix through another means.
- **C would narrow to the probe region** if the full net finds emitted code that the after-groups list refuses.

## what I did not run

- **Platforms:** Windows (`WIN32_LEAN_AND_MEAN`, `NOMINMAX`, MSVC `/external`) and Linux x86-64.
- **Not built into the compiler:** route B and the split probe (C level only). Route D was not run at all.
- **Prototypes not gated:** the full net, the compiler's own tests, the guard's test and the cache suite were not run under the prototypes.
- **Not run at the Heroes level on Linux:** P2 (C level only).
- **Censuses not run:**
  - every `pkg-config --list-all` package (I used panel 204's lists);
  - R2's pair census on user-directory copies;
  - R2 combined with `-Wsystem-headers`;
  - `heroes measure`.
- **Built, not run:** the 60 example builds.
- **Carried, not re-run:** `_FILE_OFFSET_BITS` (the critic's).
- **Rule notes:**
  - `mktemp` inside the container used the container's own `/tmp`.
  - Process count: twice, two builds ran at once, each `heroes` with its clang child, and the Linux census's docker client ran beside short greps. That came to about four processes for under a minute each time.
