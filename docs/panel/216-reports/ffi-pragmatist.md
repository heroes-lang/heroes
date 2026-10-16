# Panel 216, the ffi-pragmatist

Written 2026-10-16 from 22:10, in a detached worktree of `6e2690b50` at
`<scratchpad>/216-ffi-pragmatist/` (`git log -1 --format=%H` there printed
`6e2690b5058b663a768f06db91abe73cd804793a`), `build/` removed, the compiler built there from the
seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` under `/usr/bin/time -p`,
`real 7.92 user 7.74 sys 0.12`. Every number names its command; what could not be run says *unrun*.

Every measurement file and script named below is in `<scratchpad>/216-ffi-m/`, kept; the
worktree is removed at the end.

## A route nobody listed: clang's own constant evaluator, asked inside gnu11 (route K)

The C23 and C++ routes change the dialect only to reach clang's *constant evaluator*, the one that
refuses an undefined value where C11's folding keeps it. Clang exposes that evaluator inside C11
already: an `enable_if` or `diagnose_if` attribute's condition must be a potential constant
expression, judged by the evaluator C++ `constexpr` uses. Asked of the critic's 44 shapes (`s.h`,
`d.h`, copied to `<scratchpad>/216-ffi-m/alt/`), one unit per shape, `clang -std=gnu11
-fsyntax-only -ferror-limit=0`, with

```c
#include "s.h"
int q_di(int x) __attribute__((diagnose_if(!(((S_DIV) == (S_DIV)) || 1), "ub", "error")));
```

(`gen_alt.sh` there; the `|| 1` makes the condition true for every value, so only a failed
evaluation can refuse):

- **Apple clang 21.0.0 (this Mac)**: refused 21 of the 23 undefined shapes, every one but
  `__builtin_abs(INT_MIN)` (refused today by the constancy assertion, `ffi_not_constant`) and
  `1LL << 63` (refused today by the region's `-Wshift-sign-overflow`); kept 18 of the 21 defined
  ones, refusing the same three C23 and C++ refuse: `D_OFFSET_HAND`, `D_PTRINT`, `D_STMT`.
  `enable_if` and `diagnose_if` agree on every row.
- **Ubuntu clang 18.1.3** (`ubuntu:24.04`, its `clang` package, the floor and the CI's two Linux
  legs): the same split, `diff` of the verdicts differing on two rows only: `S_ABS` refused too
  (stricter, and refused today anyway), and `D_STMT`'s error count (refused on both).
- **Debian clang 22.1.8** (`heroes-linux-arm64`): identical to Apple 21 but `D_STMT`'s count.

Clang's words separate the two kinds of refusal by their note:
an undefined value reads *note: value 2147483648 is outside the range of representable values of
type 'int'* (`S_DIV`, `S_HIDDEN`) or *value 1.0E+20 is outside the range …* (`S_FLT`); the two
defined pointer shapes read *note: this conversion is not allowed in a constant expression*.
The error itself is *'diagnose_if' attribute expression never produces a constant expression*.

## Clang 19 is the first clang with C23 `constexpr` (the critic's open question, answered)

`ubuntu:24.04` with `clang`, `clang-19` and `clang-20` from its own archive, the coordinator's
`c23.c` (copied from the critic's folder), `clang -fsyntax-only -std=c23 -ferror-limit=0`:
**Ubuntu clang 18.1.3**: four errors, *unknown type name 'constexpr'*; **Ubuntu clang 19.1.1**
and **20.1.2**: two errors, *constexpr variable 'v_div' (and 'v_flt') must be initialized by a
constant expression*, `K_COMMA` and `K_FDIV` kept. So route C needs the floor raised from 18 to
19 at least (`selfhost/cli/clang_floor.hero:36-37`), and the CI's two Linux legs to install
`clang-19`, which the Ubuntu 24.04 archive carries (`apt-cache policy clang-19` printed
`Candidate: 1:19.1.1-1ubuntu1~24.04.2`).

## The corpus: every integer constant the tree binds, asked in its own unit's context

**The population, enumerated from the tree.** `git grep -l -E '^[[:space:]]*extern ' 6e2690b50 --
'*.hero' ':!docs'`: 932 files. An awk over them (`<scratchpad>/216-ffi-m/consts.awk`: an `extern
"` line at column 0, then its indented `constant NAME: TYPE` lines) found 164 constants in groups,
**129 declared `i8` to `u64` or `bool` in 62 files** (105 `i64`, 17 `i32`, 4 `u64`, 2 `u32`, 1
`i16`, none `bool`). The headers: `hero_os.h` 50, `sqlite3.h` 11, `sys/socket.h` 4,
`curl/curl.h` 3, `signal.h` 3, `stdio.h`, `poll.h`, `sys/prctl.h` 2 each, `sys/resource.h`,
`stdint.h`, `regex.h`, `math.h`, `limits.h`, `errno.h` 1 each, and 26 headers of golden cases.

**How each was asked.** Every program holding one (54 with a `main`, plus `selfhost/main.hero`,
`examples/ledger/main.hero` and `tests/harness/main.hero` for the eight modules; `archive/` is
built by nothing) was built with `heroes build` under a `clang` wrapper on `PATH` that logs each
invocation, response files expanded (`<scratchpad>/216-ffi-m/wrapbin/clang`); 38 built, the 19
refused being cases built to be refused (`check/`, `fixedbugs/`, `unsupported/`) and the two whose
header is Linux's (`sys/prctl.h`, 437 and 589). Then `routes.py` took each unit that holds
`value_check`'s region, its text through `#include "heroes_standard.h"` as the context (the
groups' headers in the unit's own order, the library's standard headers after them as the unit
reads them: defect 568's `CASE_LOW` names `INT64_MIN`, which exists only after that line), its
exact clang words, and asked each constant in a unit of its own, the question at `#line 900001
"heroes_question"` so a refusal is read by position:

| route | the question, `K` the constant | dialect and flags |
|---|---|---|
| B | `_Static_assert((K) == (K), "q");` | the unit's, `-Werror=gnu-folding-constant` |
| C | `constexpr __typeof__(K) q_c = (K);` | `-std=gnu23` |
| D | `constexpr decltype(K) q_d = (K);` | `-x c++ -std=gnu++17` |
| F | `__typeof__(K) q_f(void) { return (K); }`, a trap in `q_f`'s IR | the unit's at `-O1`, `-fsanitize` and `-fsanitize-trap` of `signed-integer-overflow,float-cast-overflow,shift,integer-divide-by-zero`, `-S -emit-llvm` |
| K | `int q_k(int x) __attribute__((diagnose_if(!(((K) == (K)) \|\| 1), "q", "error")));` | the unit's |

Every route's context alone (no question) compiled in 55 of the 57 units, as gnu23, as C++ and
with the sanitizers; the two failures are 437's and 589's, whose `sys/prctl.h` this Mac lacks.
**So no header the tree binds fails to compile as C23 or as C++ on this Mac** (Apple clang
21.0.0), `heroes_runtime.h` and `hero_os.h` included.

**This Mac, Apple clang 21.0.0, 121 (unit, constant) pairs** (`python3 -I routes.py`, 776
compiles, `real 5.42`):

| route | refused | of which a program that builds today |
|---|---|---|
| B | 15 | **1: `FROM_A_FLOAT`, `((int)(1000.0 / 60))`, in `tests/golden/run/fixedbugs-2072-values-c-defines-still-build.hero`** |
| C | 12 | 0 |
| D | 12 | 0 |
| F | 7 refused by an error, 6 trapped | 0 |
| K | 12 | 0 |

Every other refusal is a constant the trunk already refuses: the five planted undefined values
of `unsupported/fixedbugs-2072-…` (F traps the sixth, `SHIFT_INTO_SIGN`, `1 << 31`, which C, D
and K keep and the region refuses), the unreadable ones of 2061, 2071, 143 and 563, and two
declared with the wrong type (`M_PI`, `HALF`), which B also refuses. **The corpus holds no
constant whose value C leaves undefined outside the planted cases**, so on this Mac it cannot
tell C, D, F and K apart; it does tell B apart, which refuses a correct program the net runs.

**The same corpus on Linux** (`linux_corpus.sh`: the worktree copied into the container, its
compiler built there from the seed, the 57 programs built under the same logging wrapper, 40
built and 17 refused as designed, then `routes.py`): every context compiled under every dialect,
57 of 57, on both.

| route | Debian clang 22.1.8 (`heroes-linux-arm64`) | Ubuntu clang 18.1.3 (`ubuntu:24.04`) |
|---|---|---|
| B | 15 refused, `FROM_A_FLOAT` among them | 12: clang 18 keeps three planted shifts (`SHIFT_PAST`, `SHIFT_NEGATIVE`, `SHIFT_BY_NEGATIVE`) as constant expressions, which the region refuses |
| C | 12 | **121 of 121 refused**, *unknown type name 'constexpr'* |
| D | 12 | 12 |
| F | 7 by an error, 6 by a trap | the same |
| K | 12 | 12 |

`diff` of each route's refused set across Apple 21, Debian 22 and Ubuntu 18 printed nothing for
D, F and K. Two defects of my own harness were found and repaired on the way, and no number
above comes from before the repair: a response file read without its posix quoting, and the IR
written to `/dev/stdout`, which clang on Linux replaces through a temporary file and a rename
(`-o -` since).

## The real headers: every object-like macro of 110 to 135 libraries, each route asked

The corpus binds few real libraries, so `sweep.py` (`<scratchpad>/216-ffi-m/`) takes each header
of the C library and of the libraries design.md §1.11's table names and more (sqlite3, curl,
zlib, ncurses, PCRE2, OpenSSL, raylib on the Mac, SDL2, SDL3, libuv, GLFW, GMP, libarchive, yaml,
png, lz4, zstd, lzma, libffi, readline, json-c, cJSON on Linux, sndfile, portaudio, opus, vorbis,
freetype, harfbuzz, cairo, fontconfig, expat, libxml2, glib, libssh, nghttp2, c-ares, GnuTLS,
libgcrypt, libsodium on Linux, ICU, webp, tiff, jpeg, gif, mpg123, libusb, FFmpeg, X11, pcap,
libedit, oniguruma, lcms2, uuid, libpq, ZeroMQ, libevent, libbsd, libcap, seccomp, systemd, and
`hero_os.h`), each with its `pkg-config --cflags`, in the context a unit reads a group's header
in (`heroes_unit.h`, `hero_os.h`, the runtime, the guard, the header, the guard's close,
`heroes_standard.h`); lists the object-like macros the header brings (`clang -E -dM`, minus the
context's own, `__` names left out); holds each to **today's gate** (an integer type by
`_Generic`, `__builtin_constant_p`, and `value_check`'s region around `return N;`); and asks each
route of every macro that passes, one block of 100 lines per macro under `#line`, refusals read
by position, `-ferror-limit=0`.

| | this Mac, Apple clang 21.0.0 | Debian clang 22.1.8 | Ubuntu clang 18.1.3 |
|---|---|---|---|
| headers whose context compiled | 110 of 115 (5 not found by their words) | 135 of 136 (raylib not packaged) | 133 of 136 (raylib, SDL3, mpdecimal) |
| object-like macros | 53,464 | 57,223 | 55,164 |
| **pass today's gate** (a correct binding builds) | **41,135** | **43,848** | **42,107** |
| B refuses | 0 | 0 | 0 |
| C refuses | 0 | 0 | **42,107 (all)** |
| D refuses | **26** | **38** | **36** |
| F refuses | 0 | 0 | 0 |
| K refuses | 0 | 0 | 0 |
| context fails as gnu23 / as C++ | 0 / 0 | 0 / 0 | 0 / 0 |

**The control**: the critic's `s.h` and `d.h` through the same sweep. Of the 23 undefined shapes,
the 17 that pass today's gate are refused by every route, B, C, D, F and K alike, on all three
clangs (C on 18 refusing everything); of the 19 defined ones that pass it, B refuses 9, C, D and K
the two pointer shapes (`D_OFFSET_HAND`, `D_PTRINT`), F none. `diff` of the verdicts across the
three clangs: only C's column on 18, and `PTHREAD_STACK_MIN` in D's on glibc (below). So the
zeros over the real headers are zeros of a working instrument.

**Every header compiles as C23 and as C++**, on all three platforms: the fear that route D or C
cannot read a header did not come true over 110 to 135 libraries. **What came true instead is
worse: the header compiles, and says something else.**

- **Route D's 26 on this Mac** are one name, `_USE_FORTIFY_LEVEL` (`stdio.h`, `string.h` and 22
  libraries through them), and glib's `G_C_STD_VERSION`: the SDK's `_stdio.h:505` reads `#if
  defined (__GNUC__) && _FORTIFY_SOURCE > 0 && !defined (__cplusplus)`, and glib defines
  `G_C_STD_VERSION` as `__STDC_VERSION__` in C only. Both build today as a C binding; as C++ they
  are *use of undeclared identifier*.
- **Route D's 38 on Debian and 36 on Ubuntu** add glibc's **`SIGSTKSZ`, `MINSIGSTKSZ` and
  `PTHREAD_STACK_MIN`**, the constants a `sigaltstack` or a thread binding names, and a few
  include guards. In the container: `clang -std=gnu11 -E` of `SIGSTKSZ MINSIGSTKSZ
  PTHREAD_STACK_MIN` printed `16384 5120 131072`; `clang -x c++ -std=gnu++17 -E` printed
  `sysconf (_SC_SIGSTKSZ) sysconf (_SC_SIGSTKSZ) __sysconf (75)`, because **clang++ predefines
  `_GNU_SOURCE 1`** (`clang -x c++ -dM -E /dev/null`) and clang does not, and glibc 2.34's
  `bits/sigstksz.h:28` then makes the value a call. The C++ unit asks a value the program never
  reads, and refuses a correct binding.
- **The expansions compared**, every gate-passing macro preprocessed in the three dialects
  (`@@i@@ NAME @@end@@` lines, `-E -P`): as gnu23, **9 differ** on this Mac (`true` and `false`
  become C23's keywords of type `bool` where the C11 unit reads `1` and `0`, and
  `G_C_STD_VERSION` reads `202311L` where the unit reads `201112L`), 9 on both Linux clangs; as
  C++, **92** on this Mac (ncurses, gmp and freetype switch to `static_cast`, `INTn_MIN` is
  spelt otherwise, the two names above vanish), 107 on Debian, 95 on Ubuntu.

So a route that reads the header in another dialect does not ask the program's value; it asks a
value of another program, which is equal by luck on most macros and provably not on these.

**The binding behind D's refusal, built.** In `heroes-linux-arm64` (glibc 2.41, `ldd --version`),
`extern "signal.h"` with `constant SIGSTKSZ: i64` and `constant MINSIGSTKSZ: i64`, `extern
"limits.h"` with `constant PTHREAD_STACK_MIN: i64`, and `print` of the three: `heroes build`
exit 0, the program printed `16384`, `5120` and `131072` (run together by `print`), exit 0. A
correct program today; under D, refused. Spec § 13 tells the author to write *one defining
`_GNU_SOURCE` first*: the program decides `_GNU_SOURCE` by the order of its groups, and a C++
unit decides it alone.

## Three more shapes of "a value C defines", beside the ones measured

**33 edge shapes** (`<scratchpad>/216-ffi-m/alt/e.h`): a `static const int` read, a compound
literal `((int){5})`, an unevaluated `1 / 0` and `INT_MIN / -1` in a conditional's dead arm,
`_Alignof`, `(int)(char)300`, `-1u`, builtins (`__builtin_bswap32`, `__builtin_popcount`,
`__builtin_choose_expr`, `__builtin_constant_p`, `__builtin_types_compatible_p`), `((int)-2.9)`,
`(1.0L < 2.0L)`, `(NULL == 0)`, `((uint64_t)1 << 63)`, `(SIZE_MAX / 2)`, `((_Bool)42)`, and a
null-pointer difference `((int)(&((char *)0)[8] - (char *)0))`. Through the sweep, on all three
clangs:

- **B refuses nine, eight of them values C defines** (`E_COMPOUND_INT`, `E_FLOAT_TRUNC`,
  `E_INF_CMP`, `E_LDBL_CMP`, `E_NEG_ZERO_FLT`, `E_NULL_CMP`, `E_STATIC_CONST`,
  `E_STATIC_CONST_ARITH`; the ninth is `E_PTRDIFF`, below), the same nine on all three clangs;
  and the corpus showed clang 18 keeping three shifts B refuses on 21 and 22, so B is not even
  one rule across the floor.
- **C refuses the two `static const` reads** (C23's `constexpr` cannot read a `const` object
  that is not `constexpr`) and `E_PTRDIFF`; on clang 18, all 33.
- **D refuses `__builtin_types_compatible_p(int, int)`**, a C-only builtin (*expected '(' for
  function-style cast*), and `E_PTRDIFF`.
- **K refuses one, `E_PTRDIFF`**, with *note: cannot perform pointer arithmetic on null
  pointer*: arithmetic on a null pointer, which C leaves undefined too (C11 6.5.6p8). **K keeps
  the `static const` reads, the compound literal, every builtin and every float comparison.**
- **F refuses none**, `E_PTRDIFF` included (no sanitizer of the four reads pointer arithmetic).

**The type neighbours** (the critic's `t.h`, copied, plus four defined twins): K and F both
reach the `f64` and widened-`i64` values the brief's routes leave out. K, with the same
`(K) == (K)` question, refuses `T_F64`, `T_F64B` and `T_I64` with the value-naming note and keeps
`((double)(1000 / 60))` and `((int64_t)(INT_MAX / -1))`; for a record, `(((struct pt)T_PT), 1)`
(a brace list) and `((T_PTX), 1)` (a compound literal) refuse the two undefined records with
*value 2147483648 (1.0E+20) is outside the range …* and keep `{7, 2}` and `((struct pt){ 100, 3 })`,
on clang 21 and 18 alike. F traps the four undefined ones (two traps each) and none of the four
defined.

## Found beside the sitting: a header value's own `_Pragma` disarms today's region

`value_check`'s region rests on clang's warnings made errors, and a header's value may carry a
`_Pragma` that turns a warning off where it is expanded. Built with the trunk's compiler from
the seed (`<scratchpad>/216-ffi-m/prag/`), this Mac:

| `#define` in `p.h`, bound `constant …: i64` | `heroes build` | three runs | K (errors) | F (traps) |
|---|---|---|---|---|
| `P_PLAIN (1 << 40)` | exit 1, `ffi_constant_type` | — | | |
| `P_SHIFT (_Pragma("clang diagnostic ignored \"-Wshift-count-overflow\"") 1 << 40)` | **exit 0** | `6090925004 6090974156 6158377932` | 1 | 2 |
| `P_EVERY` (the same with `-Weverything`) | **exit 0** | `6169027532 6127821772 6127592396` | 1 | 2 |
| `P_SIGN (_Pragma("… \"-Wshift-sign-overflow\"") 1 << 31)` | **exit 0** | `-2147483648` ×3 | **0** | 2 |
| `P_DIVZ (_Pragma("… \"-Wdivision-by-zero\"") 1 / 0)` | exit 1, `ffi_not_constant` | — | 1 | 2 |

So defect 2075's symptom (exit 0, a value that changes per run) is reachable through a third
door, and **B is disarmed the same way on clang 18** (`P_SHIFT`: 0 errors under
`-Werror=gnu-folding-constant`). K's error (*attribute expression never produces a constant
expression*) is an error, not a warning, and no diagnostic pragma lowered it on 21 or 18; F's
trap is code, not a diagnostic. **Searched before calling it new**: `gh issue list --state all
--search` for *_Pragma region*, *pragma value check*, *pragma undefined value*, *pragma constant
shift*, *Pragma inside the value*; the nearest is defect 570 (issue 1930, closed), a header's
pragma turning a *sign* check off for the rest of the unit. A question for the coordinator, not
filed by me; by `.claude/rules/verification.md` § Bounded discovery it reads as `blocking` (a
wrong program accepted).

## Route E, built: it traps, and says the wrong thing

The coordinator's four programs, `--emit-c`, compiled with `clang -std=gnu11` and the runtime,
the four sanitizers in trap mode over the whole unit: `K_DIV` and `K_FLT` **exit 133** (SIGTRAP)
at `-O0` and at `-O2`; `K_COMMA` and `K_FDIV` print `2` and `16`, exit 0. What the program says:
*panic: the process is dying of SIGTRAP, under KDIV.K_DIV, and this runtime did not raise it. A C
library ends a program this way when it catches misuse, memory given back twice among them*; at
`-O2` the frame is gone and no constant is named. That is design.md §1.12's own falsifier, *an
abort that stops a program at a place that tells the reader nothing*, and its words blame a C
library for a double free. And sanitizing the whole unit instruments every header `static
inline` function the program calls, library code whose behaviour the route then changes; only a
unit of accessors alone would avoid that, which is route F moved to run time.

## What K and F cost

`cost.py` (`<scratchpad>/216-ffi-m/`): each unit holding a constant, its exact command timed as
it is, then with K's declarations appended, then F's probe unit; CPU seconds of the children
(`getrusage`), median of five on this Mac, three in each container. **The Mac was not still**
(load averages 11.97, 33.34 and 27.96 at the start, 10.43 30.61 27.19 at the end; another
session's clang at the top of `ps`), which is why CPU seconds and not `real` are given.

| | units | the units' compiles | with K appended | F's probe units |
|---|---|---|---|---|
| this Mac, Apple clang 21 | 42 | 1.63 s | 1.65 s (+1.5%) | 1.13 s (+69.6%) |
| Debian clang 22.1.8 | 44 | 2.16 s | 2.15 s (−0.5%) | 1.39 s (+64.4%) |
| Ubuntu clang 18.1.3 | 44 | 2.03 s | 1.92 s (−5.5%) | 1.16 s (+57.0%) |

K's difference is inside the noise on all three; F is one more clang process per unit holding a
constant. Against a whole cold build: `heroes build` of `examples/sqlite/main.hero` in a fresh
directory, three runs, `user 0.37 sys 0.15` each; F's probe of that unit, 0.026 s, is about +5%
of it. **And every unit of a program that builds today compiled with K's declarations appended,
on all three clangs**; the 13 to 15 units skipped are those of programs the trunk refuses.

**F's one premise about headers, measured.** A probe at `-O1` sees `__OPTIMIZE__`; over every
gate-passing macro of every header, the expansions at `-O0` against `-O1` with the sanitizers
differ on exactly one name, SDL's `SDL_ASSERT_LEVEL` (`2` at `-O0`, `1` at `-O1` and `-O2`, SDL2
and SDL3, all three clangs). Both values are defined, so no verdict moved; and `-O1
-U__OPTIMIZE__ -D__NO_INLINE__` reads `2` and still traps `S_DIV`, `S_FLT`, `S_HIDDEN` and none of
`D_FDIV`, `D_FLOATCAST`, `D_LDBL`, `D_FINRANGE`, `D_COMMA`. So F can be held to the build's view,
at the price of a list of level macros. K is asked in the unit itself, at the unit's level and in
its dialect, and has no such list.

## Windows: unrun

`timeout 20 ssh -o ConnectTimeout=8 win 'clang --version'` printed *connect to host apponfly-vps
port 22: Operation timed out*. What I could ask without the box, an inference about it and not a
measurement of it: Apple clang 21 with `--target=x86_64-pc-windows-msvc -ffreestanding` (clang's
own headers, `long` of 32 bits) over the 44 shapes gives K's split identical to the Mac's (`diff`
printed nothing) and F's traps on the same 23 undefined shapes; `D_STMT`'s F unit did not compile
there. The Windows SDK's headers and clang 23: unrun.

## Verdicts

`section`: design.md §1.11 (*FFI ergonomics rank alongside comprehension*, and its point 3:
*FFI signatures are verified by clang against the real header*), §1.12 (*any C library must be
bindable*; *a guarantee that ends quietly is not one*), §4.19 (*a `constant`'s value is the
header's*); spec § 13 (*clang checks every … constant … against that header*; *one defining
`_GNU_SOURCE` first*). **What design.md does not say**: in which dialect the header is read for
a check. §4.19's *the header's value* reads, to me, as the value the program's own unit
computes; routes C and D ask another unit's, and the document does not cover that question.

| route | verdict | the reason, measured |
|---|---|---|
| A, nothing | object | no binding refused, but `K_DIV` and the pragma-borne `P_SHIFT` print a new value per run at exit 0 (§1.12) |
| B, the ICE rule | **veto** | refuses `FROM_A_FLOAT` of a `run` golden on all three clangs, eight more values C defines (`static const` reads, `((int){5})`, `((int)-2.9)`, `(NULL == 0)`), and is itself disarmed by a value's pragma on clang 18 |
| C, C23 `constexpr` | **veto** | refuses 42,107 of 42,107 real macros and 121 of 121 corpus constants on clang 18.1.3, the floor and two CI legs; on 19 and later it still refuses `static const` reads and reads 9 macros as another program does |
| D, C++ `constexpr` | **veto** | refuses 26 (Mac), 38 (Debian), 36 (Ubuntu) real macros a C binding builds today: `SIGSTKSZ` built and printed `16384` on Debian arm64; clang++ predefines `_GNU_SOURCE`, headers branch on `__cplusplus` |
| E, a trap at run time | object | refuses no binding, but exit 133 with words blaming a C library's double free, no constant named at `-O2`; changes what a program does (a spec sentence) and the library's inline code |
| F, the trap read at build time | **approve** | refuses 0 of 127,090 gate-passing real macros and 0 corpus constants on three clangs, keeps every defined shape found, traps every arithmetic undefined value including `1 << 31` under a pragma; +1 clang per unit holding a constant; its level premise is measured and repairable |
| G, defining flags | object | refuses nothing, closes the float half only (critic), and changes the semantics of every header's inline code |
| H, B as a filter | object | the filter refuses no real macro, so it saves F's cost everywhere; but on clang 18 a value's pragma passes it (`P_SHIFT`), and the second question is never asked |
| I, the folded literal | object | refuses nothing and accepts the undefined value; the literal is a copy, and `SDL_ASSERT_LEVEL` shows a value moving with the level a copy must follow |
| J, our own evaluator | object | against §1.11 point 3: a second C evaluator beside clang; unbuilt |
| K, `diagnose_if` in gnu11 (new) | object alone; **approve as F's words** | in-unit, clang 18 to 22, +0% CPU, 0 real refusals, the note names the value; but alone it refuses `(int)(intptr_t)(void *)16` (a value C defines) and keeps `1 << 31` under a pragma |

**What I recommend**: F decides, at the build's own dialect, words and level macros, in a unit
of accessors' probes per unit that holds an integer (or `f64`, or record) constant; K's note,
asked in the unit itself, gives the message its words (*value 2147483648 is outside the range of
representable values of type 'int'*), and the region's warnings stay as they are until F
supersedes them. The conservative route, which the synthesis may prefer: K alone, with its
measured hole (`1 << 31` under a pragma) and its one refusal of a defined pointer shape named.

**Conditions.** F turns to object if the Windows box's clang 23, over the Windows SDK headers a
binding names, traps a macro C defines, or if a header the tree binds is found whose value moves
with `__OPTIMIZE__` into an undefined one; F turns to veto if any gate-passing real macro traps
on any leg. D and C lose their veto only if a unit in another dialect is shown to expand every
gate-passing macro of the measured headers as the gnu11 unit does (today: 92 to 107 and 9
differ), and C also needs the floor at 19 (a tool-surface sitting). B loses its veto if
`fixedbugs-2072-values-c-defines-still-build` and the eight edge values are ruled not correct
programs, which spec § 13 does not say.

# Pass 2, 2026-10-17 (00:39 to 01:20): route Ux-whole, preprocessed first, over the real headers

Written after the critic's pass 2 and the compiler-engineer's final section, both read whole first.
Worktree `.claude/worktrees/216-ffi-pass2` at `6e2690b50` (`git log -1 --format=%H` printed
`6e2690b5058b663a768f06db91abe73cd804793a`), `build/` removed, the compiler from the seed (`real
7.89 user 7.74 sys 0.12`). Containers `heroes-linux-arm64:latest` (Debian clang 22.1.8, no
`--platform`) and `ubuntu:24.04` (Ubuntu clang 18.1.3), pass 1's libraries installed with `apt-get
install --no-install-recommends` (`install.sh`), the worktree copied in and its compiler built from
the seed in each. Every script and output below is in `<scratchpad>/216-ffi-m2/`.

## The recipe, reused and not redesigned

- **The 96 cells**: the compiler-engineer's `ux48/ux2.sh` copied with its paths alone changed
  (`c96/ux2.sh`); its `pre` verdicts on this Mac against `216-ce-m2/ux48/pre48.v`: `diff` empty.
- **The real headers** (`uxcorpus.py`): per library, pass 1's gate-passing macros, ONE Heroes
  program binding every one as a `constant` (`i64`, `u64` where the value is an unsigned 64-bit
  type) in a group whose header holds the library's headers and `-D` words, `heroes build
  --emit-c`, then `ux2.sh`'s `pre` recipe verbatim (its words, `-E` with
  `-Dmain=hero_probe_hidden_main`, `-x cpp-output` with the four checks, the link with
  `runtime.c`), the main calling every accessor and printing a last line when it finished, each
  report read by its `<file>.hero:<line>:` position. Three probe forms: the compiler-engineer's (one
  process, every accessor in turn), one forked child per accessor, and **trap mode** (the same `.i`
  compiled with `-fsanitize-trap=<the four>`, one child per accessor, the verdict its signal).
- **The corpus** (`corpusux.py`): every program pass 1 listed, built cold under the logging
  `clang`; each unit that defines an accessor asked with its OWN logged words, the level replaced.

## The counts

| | Apple clang 21.0.0 | Debian clang 22.1.8 | Ubuntu clang 18.1.3 |
|---|---|---|---|
| object-like macros / pass today's gate | 53,464 / 41,135 | 57,223 / 43,848 | 55,164 / 42,107 |
| bound in a program that builds | **41,114** (108 libraries) | **43,827** (133) | **42,086** (131) |
| refused by the build by their name (not Ux) | 21 | 21 | 21 |
| **flagged by Ux-whole, `-O0` / `-O2`** | **0 / 0** | **0 / 0** | **0 / 0** |
| trapped by trap mode, `-O0` / `-O2` | 0 / 0 | 0 / 0 | 0 / 0 |
| verdicts that differ between `-O0` and `-O2` | 0 | 0 | 0 |
| control `s.h`, 17 undefined values that pass the gate, report `-O0` | 17 | 17 | 17 |
| the same, report `-O2`, one process | **8, then SIGTRAP** | **9, then hung to the 120 s bound** | 17 |
| the same, trap mode `-O0` / `-O2` | 17 / 17 | 17 / 17 | 17 / 17 |
| control `d.h`, 19 defined values | 0 everywhere | 0 everywhere | 0 everywhere |
| corpus: units / accessors / constants | 42 / 96 / 62 | 44 / 100 / 63 | 44 / 100 / 63 |
| **corpus flagged**, report and trap, `-O0` and `-O2` | **0** | **0** | **0** |

The 21 are the build's, not the route's: `true` and `false` (8, `expected_name`), `INT8_MIN` to
`INT64_MIN` that my pass-1 sweep counted under `math.h`, `stdbool.h` and `stddef.h` (12,
`ffi_unknown_name`), X11's `None` (`reserved_word`). Every probe of every library finished. **So
Ux-whole refuses no real macro and no corpus constant on any leg, at either level**, the whole
population and not a sample.

## Trap mode: the same verdicts, no sanitizer runtime

- **The 96 cells** (`c96/ux2trap.sh`: `pre` with `-fsanitize-trap` on the compile and the link):
  trap verdicts against report verdicts, `diff` empty on this Mac, Debian 22 and Ubuntu 18; 44
  flagged, each by signal 5 (exit 133), 52 kept; report on both Linux clangs equals `pre48.v`
  too. The 9 attack shapes at both levels (18 cells per clang): trap equals report on all three.
- **No runtime.** On this Mac the trap probe's `otool -L` names `libSystem` alone and `nm` finds 0
  `ubsan` symbols; the report probe loads `@rpath/libclang_rt.ubsan_osx_dynamic.dylib`. **On
  `ubuntu:24.04` with `clang` installed without recommends, report mode cannot link at all: 131 of
  131 libraries, *cannot find /usr/lib/llvm-18/lib/clang/18/lib/linux/libclang_rt.ubsan_standalone-aarch64.a*;
  trap mode: 131 of 131 clean, the controls 17 and 0.** `libclang-rt-18-dev` is a *Recommends* of
  `libclang-common-18-dev` (`apt-cache depends`), so that clang compiles, links and builds every
  Heroes program today (`K_DIV` built, exit 0) and a report-mode Ux would break every program
  binding a constant on it. Found beside: there `heroes run --sanitize K_DIV.hero` exits **2**,
  *internal error: the runtime did not compile ... 'sanitizer/common_interface_defs.h' file not
  found*; searched (`gh issue list --search` for *sanitize internal error*, *compiler-rt*,
  *sanitizer runtime missing*, *ubsan_standalone*, *no-install-recommends*), not found: a
  question for the coordinator, not filed by me.
- **Windows: unrun** (the box was not tried). Cross-compiled here for `x86_64-pc-windows-msvc`
  (`-ffreestanding`, `win/w.sh`): the trap object holds 0 `ubsan` symbols and lowers each check
  to `ud1`; the report object references `__ubsan_handle_divrem_overflow` and
  `__ubsan_handle_float_cast_overflow`. An inference, not a measurement of the box: trap mode
  needs no UBSan runtime there either.
- **What trap mode loses is the words**: no operation, no value. The kind survives in the trap
  instruction (`brk #0x5503` for a division, `#0x5505` for a float conversion, read from this
  Mac's object), which a handler could name; the value would come from K's note (pass 1).
- **Isolation without `fork`**, which Windows lacks (`resume.py`): one process calls every
  accessor, writing each index before the call; a death by signal names the accessor, and the next
  process starts after it. OpenSSL's 7,425 accessors: **1 run, 0.22 to 0.32 s** on this Mac (a new
  binary's first run included), 7,186 in 0.00 s on Debian; the control: 18 runs, the same 17, on
  all three. A child per accessor costs **2.71 to 2.78 s** for the same 7,425 here (`fork`, load
  3.03), so resuming is the form to build.

## FOUND: report mode at `-O2` runs code nobody asked it to run

`S_HIDDEN`'s shape, `((INT_MIN / -1) * 0)`: after the handler LLVM treats the rest as
unreachable. On this Mac that is `brk #0x1`, so a one-process probe stops at the first such
constant (8 of 17). **On Debian clang 22.1.8 no instruction follows the handler's call and
execution falls into the next function**: the one-process probe loops to its time bound (9 of 17),
and `ux2.sh`'s own one-constant probe falls into the program's renamed `main` and dies of SIGSEGV
in `h_0opt_f87774a_retain` (`objdump` shows the accessor ending at the `bl`). Built to see what
that means (`fall/fall.sh`, a constant of that shape and a `main` whose first call is a header
function appending to a file): **report mode wrote the file during the probe, the program's own
code run at build time**; trap mode exit 133 and nothing written; report with
`-fno-sanitize-recover`: the report, exit 1, nothing written. Ubuntu clang 18 does not fall
through. So recovering report mode is unsound at `-O2` on the newest clang the project uses.

## The attacks, each built and run

| attack (header, bound `i32`) | today | Ux, report read by grep | with the canary or a finished line | trap mode |
|---|---|---|---|---|
| constructor `abort()`, `AB_K = INT_MIN / -1` | build 0, program 134 | probe SIGABRT before `main`: **kept, silent** | canary silent: build fails | parent dies: fails only if a finished line is required |
| constructor `exit(0)`, `EX_K` | build 0, program prints nothing | probe exit 0, no report: **kept, silent** | build fails | the same |
| constructor `atexit` of `_exit(0)`, `AE_K` | prints `1832690748` | flagged | flagged | flagged once the probe writes unbuffered (my first harness lost its own buffered lines) |
| constructor `dup2(/dev/null, 2)`, `QU_K` | **prints `1801167932`, exit 0** | **kept, silent: defect 2075 passes** | canary lost too: build fails; `UBSAN_OPTIONS=log_path=<file>`: flagged | **flagged** (signal 5) |
| constructor `read(0, ...)`, `RD_K` | | stdin an open pipe: blocked to the bound (exit 124); stdin `/dev/null`: flagged | | flagged |
| a header's global: `static const int` plus 1 (`GK_OVF`), minus 1 (`GK_OK`), a non-const `static` (`GK_VAR`) | `GK_OVF` refused (`ffi_constant_type`, *overflow in expression*), `GK_VAR` refused (`ffi_not_constant`) | `GK_OK` kept | | kept |
| a unit that calls a bound function (corpus; real raylib, SDL3, sqlite3) | builds | **`ux2.sh`'s link fails: 42 of 84 unit-levels here, 44 of 88 on each Linux**, other modules' functions or the library's, through `hero_extern_refs` (`emit/extern_refs.hero`, `used`); the program's own link: 84 of 84, 88 of 88 clean; undefined left to the loader: 10 of 84 abort here (*symbol not found in flat namespace*), 44 of 88 exit 127 | | the same |
| headers present, library absent (`package` answering `-lraylib_absent`, SDL3, libjpeg, sqlite3) | `build` exit 1, `ffi_missing_library`; `--emit-c` and `check` exit 0 | a unit calling a bound function: `ux2.sh`'s link *Undefined symbols*, the package's libraries *library 'raylib_absent' not found*; constants alone (libjpeg) link | | the same |
| library found at link, not at load (`@rpath` dylib here, a `.so` outside every loader path on Debian) | **build 0** for `TK_K` and for `TK_BAD = INT_MIN / -1` | no probe can load (*Library not loaded*, *error while loading shared libraries*): **`TK_BAD` kept, silent** | **`TK_K`, a correct program, refused** | the same |
| a library's own initializers | | `libcrypto` and `libglib` carry one each (`otool -l`), which a probe linked as the program is would load and run at build time (an inference, not run); raylib, SDL3, libjpeg, sqlite3 none | | |

**The repair, measured** (`corpusux-s.py`, variant S): the unit's own text with `hero_extern_refs`
no longer `used`, `-ffunction-sections -fdata-sections`, linked against `runtime.c` alone with
`-Wl,-dead_strip` (`--gc-sections` on Linux) from a main that calls only the accessors. **No
library and no other module: 84 of 84 unit-levels here, 88 of 88 on Debian and on Ubuntu, link
and run, 0 flagged, in report mode with `-fno-sanitize-recover` and in trap mode**; `TK_K` kept
and `TK_BAD` flagged with the library absent at load, here and on Debian, at both levels; the four
library-absent `--emit-c` units link and run (this Mac). The resumed traps name the same 17
constants as the forked ones (checked by name here). So the probe can ask without the program's
libraries, and Ux can then run under `check` and `--emit-c` too without refusing what they accept
today.

## Verdict on Ux-whole

`section`: design.md §1.11 (*anything a real program needs comes from C libraries through the
FFI*), §1.12 (*any C library must be bindable*; *a guarantee that ends quietly is not one*),
§4.19. **What design.md does not say**: whether `heroes build` may require a bound library to be
loadable, or the sanitizer runtime installed, on the build machine; nor whether a build may run
the program's or its headers' code (§4.19's pkg-config repair treated *a linker the file named run
during the build* as a defect, which is the nearest ruling, and it is not this one).

**object**, on two measured costs to bindings and one to soundness, each repairable and repaired
in the scratchpad:

1. As the compiler-engineer's recipe links it, the probe cannot link half the corpus's units;
   linked as the program is, it must also LOAD the program's libraries at build time, so a
   correct binding whose library is not loadable on the build machine is refused (`TK_K`) or,
   read by grep alone, an undefined one passes in silence (`TK_BAD`). **I veto that form**:
   categorically harder bindings, §1.12.
2. Report mode needs the UBSan runtime, which `ubuntu:24.04`'s `clang` without recommends lacks:
   every program binding a constant would stop building there. **I veto a form that requires it.**
3. Recovering report mode at `-O2` executes the program's own `main` during the build on clang 22.

**Conditions, which turn me to approve**: (a) trap mode decides, each accessor isolated by
resuming after a trap; (b) the probe is variant S: no library, no other module, no `used`
reference table; (c) a finished line and the compiler-engineer's two-sided canary in every probe,
a probe that does not finish refusing the build with words naming the probe's death and the
header, never *C leaves the value undefined*; (d) stdin `/dev/null`, output to a file, the time
and byte bounds; (e) if report mode is kept anywhere for its words, `-fno-sanitize-recover` and
`UBSAN_OPTIONS=log_path`; (f) the Windows box runs the resuming trap probe before landing.

**Prediction**: built as (a) to (e), over the 41,114, 43,827 and 42,086 bound real macros and
the corpus, Ux-whole flags 0 at `-O0` and `-O2` on all three clangs and the control 17 and 0;
`TK_K` builds and `TK_BAD` is refused with its library absent at load, on macOS and Linux; on
`ubuntu:24.04` without `libclang-rt-18-dev` every program that builds today still builds. Any one
failing falsifies it.

**What would turn me further**: to approve the recipe as specified, an author's ruling that a
build may require every bound library loadable and the sanitizer runtime installed (then `heroes
doctor` checks both); to veto the repaired form, one gate-passing real macro trapped on any leg,
or the Windows box unable to run a trap probe.

## Housekeeping (pass 2)

Worktree `.claude/worktrees/216-ffi-pass2` removed at the end (`git worktree remove --force`);
containers `ffi216p2-d22` and `ffi216p2-u18` stopped and removed. The probes' own writes (the
attack headers' constructors) went to the scratchpad and to the containers' `/tmp`. No paid run,
no API call, no credential printed; `gh` used for read-only searches. Nothing written in the trunk
but this section.
