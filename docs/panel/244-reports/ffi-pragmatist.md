# Panel 244, the ffi-pragmatist

Written as I go, 2026-11-10. Worktree `.claude/worktrees/244-ffi-pragmatist`, detached at
`89db843472cf1c39450f4a2fc22ff99ff3d716f7`. Stage 1 `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (real 24.88, user 19.15), stage 2 `./heroes build selfhost/main.hero -o
heroes-next` (real 271.19, user 155.39, sys 32.45, the load at 77 when it started, so the duration
is discarded); `./heroes-next --version` prints `heroes 0.6.0` / `sources 14e9e5ce168a2336`, the
base's. Every probe below is run with that `heroes-next`; clang is `Apple clang version 21.0.0
(clang-2100.4.1.1)`. Probes in `.claude/worktrees/p244-scratch/ffi-pragmatist/`.

Complete as of this writing; the verdict is at the end, in the seat's shape.

## 1. `p18`: a header constant a fetched package binds (C5)

### 1.1 Reproduced, and the five entry shapes beside it

`e1/` is the critic's `p18`, `own18` and `idx` copied (never built in the critic's folder).
`heroes-next fetch` placed `cpkg@1.0.0-a2554f96d3f86ca5`; `heroes-next run main.hero` exits 1 with
`error[ffi_constant_type]: HALF in c.h cannot be read without running its header's code, which a
fetched package's header never does at build: it is a float, which no enumerator holds`; `own18`
prints `2`, `0.5`, exit 0. The shapes beside it, same two files, same compiler:

| route the package reaches the program by | probe | result |
|---|---|---|
| the program's own tree (`own18`) | `e1/own18` | `0.5`, exit 0 |
| the package's own folder, `heroes-next test lib.hero` with a `test` asserting `half() == 0.5` | `e2/cpkg` | `1 test, all passed`, exit 0 |
| a `path` entry, `heroes-next add --path ../cpkg` | `e2/app` | note *a `path` entry ... unverified*, `0.5`, exit 0 |
| an index entry (`p18`) | `e1/p18` | `ffi_constant_type`, exit 1 |
| a `git` entry, a local repository tagged `v1.0.0`, `add file://... --tag v1.0.0` | `e3/app` | `ffi_constant_type`, exit 1 |

**So the refusal lands on the consumer and never on the author.** Every route a package's author
develops and tests by (its own folder, a `path` entry) builds; only the copy a stranger fetches is
refused. `heroes-next --help` lists 15 verbs and none publishes or checks a package as fetched
(the critic's local-index route is the only way an author can see it first, and nothing names it).

### 1.2 Four real libraries as fetched packages

Each module is first built in the program's own tree (baseline, all exit 0), then published to a
hand-made index (`e4/idx/`, the critic's format: `versions` holds `shasum -a 256` of `list`,
verified against the critic's `cpkg` first), added with `add <p> --index file://../idx` and run.

| package | header | constants bound | own tree | fetched | refused |
|---|---|---|---|---|---|
| `hsql` = `examples/ledger/db/sqlite.hero` verbatim | SDK `sqlite3.h` 3.54.0 | 7 `#define` integers, `SQLITE_TRANSIENT: ptr` | exit 0 | exit 0, prints `0` | **0 of 8** |
| `hmath` | SDK `math.h` | `M_PI`, `M_E` (`#define` floats), `FP_NAN` (`#define 1`) | `3.141592653589793` ... exit 0 | exit 1 | **2 of 3** (the floats) |
| `hcurl` = `examples/curl/main.hero`'s group and `curl_bound.h` | SDK `curl/curl.h` 8.7.1 | `CURLOPT_URL`, `CURLE_OK`, `CURLE_UNSUPPORTED_PROTOCOL` (enumerators) | `10002`, `0`, exit 0 | exit 1 | **3 of 3** |
| `hray` | Homebrew `raylib.h` 6.2, `pkgconfig "raylib"` | `KEY_A`, `FLAG_WINDOW_RESIZABLE` (enumerators), `PI` (`#define` float), `RAYWHITE` (record) | `65`, `4`, `3.1415927`, `-168430081`, exit 0 | exit 1 | **4 of 4** |

Panel 235's prediction P1, its first half (a fetched copy of `examples/ledger/db/sqlite.hero` builds with no `--include`; the planted `heroes_standard.h` half unrun here), holds:
SQLite spells every constant as an integer `#define`. **Three of the four ladder libraries do not.**
The note the refusal gives is *Write the value so in the package's header, or compute it in
Heroes*: for `CURLOPT_URL` that means a `#define` of `10002` in the package's header, which is the
exact state `examples/curl/main.hero`'s own comment records leaving: *This file used to say
`10002`, with that derivation in a comment and nothing checking either (panel 038; `heroes mutate`
scored it at 5 sites, 0 caught)*. A fetched libcurl package is sent back to before
M-header-constants.

### 1.3 Who can act on the refusal: nobody who reads it

The consumer reads the note *Write the value so in the package's header*. Followed (`e5/`, the
placed `hmath` given a header `hm.h` with `#define HM_PI 3.14159...` and the module pointed at it):
`error[package_unread]: <hmath> cannot be read: the tree placed at ... a file of it was changed`,
exit 1. The placed tree is held to its sum, so the person the note speaks to cannot apply it. And
for an `f64` no header the author writes rescues it either: `value_fetched.hero`'s `refusals`
refuses by the declared kind before any header is read (`if kind == "integer" || kind ==
"unsigned"`, else `it is <kind>, which no enumerator holds`; read in the code, the float `#define`
case not run separately). The consumer's real routes are to ask the author, or to copy the package
into a folder and name it by `path`, which builds it as the program's own code (row 3 above) and
so runs its header's C at build, which is what C5 exists to stop.

### 1.4 The binding C5 forces today, compiled

What a package author must write instead: one `static inline` accessor per refused constant in a
header the package ships, bound as a `function` (`e6/src/`). Published to `e6/idx`, fetched, run:

| package | shim lines of C | fetched result |
|---|---|---|
| `smath` (`smath_pi`, `smath_e`) | 2 | `3.141592653589793`, `2.718281828459045`, `1`, `1.4142135623730951`, exit 0 |
| `sray` (`KEY_A`, `FLAG_WINDOW_RESIZABLE`, `PI`, `RAYWHITE`) | 4 | `65`, `4`, `3.1415927`, `-168430081`, exit 0 |
| `scurl` (`CURLOPT_URL`, `CURLE_OK`, `CURLE_UNSUPPORTED_PROTOCOL`) | 3 | `10002`, `0`, exit 0 |

Every value equals the own tree's. **The cost is not the line count; it is the check that leaves.**
A shim's return type is the author's hand claim, and C converts silently into it (`e7/`):

- own tree, `constant CURLOPT_URL: i8`: `error[ffi_constant_type]: CURLOPT_URL is int in
  curl_bound.h, not i8 — clang read the header and the types disagree`, exit 1;
- shim, `static inline signed char narrow_url(void) { return CURLOPT_URL; }` bound `-> i8`: clang's
  `-Wconstant-conversion` warning passes through, **exit 0, prints `18`**.

So under C5 a fetched libm, raylib or libcurl package either copies digits (the note's route,
unverified, `mutate`'s 0 of 5 per `examples/curl/main.hero`) or writes a shim whose width is
nobody's check. Both are §4.19's ladder going back down a rung for the one kind of code §1.11 now
says carries bindings (*over a C library through a binding it carries where it cannot*,
`docs/design.md` §1.11, the paragraph on the core packages).

### 1.5 Feature 2358's C, compiled: every refused kind valued with nothing run

The C5 restriction's reason is defect 2109: `enum { E_SHIFT = 1 << 31 }` folds silently. Measured
with clang 21 on a header installed with `-isystem` (where clang suppresses its warnings), under
the three `-Werror=` words C5 already passes (`shift-sign-overflow`, `integer-overflow`,
`gnu-folding-constant`), `-fsyntax-only`, nothing linked or run (`c2358/`):

| probe line | exit | reads |
|---|---|---|
| `enum { hero_q = (E_SHIFT) };` (today's dump route, through the name) | 0 | **hides the UB**: C5's reason, confirmed |
| `enum { hero_q = (1 << 31) };` (the initialiser spliced onto the probe's line) | 1 | refused on `q.c:2` |
| `enum { hero_q = (E_OK) };` / spliced `(10000 + 2)` for `CURLOPT_URL` | 0 / 0 | valued; `_Static_assert(hero_q == CURLOPT_URL)` holds |
| C11 `static const double hero_q = (F_OVER);` (`(double)(2147483647 + 1)`) | 1 | refused on the probe's line |
| C11 `static const double hero_q = (F_GOOD);` | 0 | accepted |
| C11 `static const P hero_q = (P_BAD);` (`{ 1 << 31, 0.5 }`) / `(P_OVER)` | 1 / 1 | refused on the probe's line |
| C11 `static const P hero_q = (P_GOOD);` | 0 | accepted |
| C23 `constexpr double/float/long/Color` of `M_PI`, `PI`, `DEG2RAD`, `KEY_A`, `CURLOPT_URL`, `RAYWHITE` | 0 | dump prints `value: Float 3.141593e+00` ... **seven digits, lossy**; `-ast-dump=json`'s `value`s are the literals, not the fold (`DEG2RAD` shows `3.14159274` and `180`) |
| C23 `constexpr long hero_q = (E_SHIFT);` | 0 | hides the UB too: `constexpr` is no route for an enumerator |
| `-Wsystem-headers` added | 1 on every probe, `P_GOOD` included | the header's own `E_SHIFT` fires everywhere: no route |

The JSON dump the C2 route already reads holds `CURLOPT_URL`'s initialiser as
`BinaryOperator "+"` over `IntegerLiteral 10000` and `2`, the leaves C2 splices. So 2358 is C2's
splice for an enumerator and a C11 static initialiser of the declared type for a float or a record,
both `-fsyntax-only`; neither needs a value printed, since the build itself emits the C name. **What
I could not run**: clang 18, the floor (`selfhost/cli/clang_floor.hero:36-37`); this Mac has Apple
clang 21 and Homebrew clang 22.1.8 only and Docker is off. The C11 route uses nothing newer than
C11 and a compound literal in a static initialiser; C23 `constexpr` (not proposed) would need the
floor raised. Whether `1.0 / 0.0` should be refused (C11 accepts it, Annex F's infinity; C23
`constexpr` refuses it) is a question for 2358, not settled here.

**Nothing run, witnessed** (`c2358/w.h`): a header whose `__attribute__((constructor))` appends
`ran` to `witness.txt`, put through the C11 route (a float static initialiser, a spliced
enumerator and its `_Static_assert`) and the C23 one, `-fsyntax-only`: exit 0 both, `ls
witness.txt` *No such file*; the control, the same header built and run, wrote `ran`.

## 2. `package_reads_outside` and where a fetched unit looks (C1, C2)

What C2 allows, read in `selfhost/cli/package_reads.hero:17-28`: the package's root, the runtime,
the toolchain's own `<...>` directories *(the C library, clang's resource directory, a `CPATH` the
machine sets)*, clang's own listed files, and the `-I`/`-F` of the package's own `pkgconfig`
answers; never the program's root, an `--include` folder or another package. Run (`e8/` to `e10/`):

| shape | own tree | fetched | message |
|---|---|---|---|
| header `#include "../../shared.h"`, reaching the program's folder | n/a | exit 1 | `package_reads_outside`, naming the file and the four allowed places |
| header `#include "<absolute path>/abs.h"` | n/a | exit 1 | `package_reads_outside`, the same |
| group `"inc.h"`, consumer's `--include incdir` | (builds, by `--include`) | exit 1 | `ffi_missing_header`, listing where the unit looks |
| the same, consumer's `CPATH=incdir` | | **exit 0, `2`** | none: `CPATH` is the machine's by C2's ruling |
| `extern "raylib.h" link "raylib"`, Homebrew's `/opt/homebrew/include` | `--include`: `-16776961`, exit 0 | `--include`: exit 1, `ffi_missing_header`; `CPATH=/opt/homebrew/include`: `-16776961`, exit 0 | |
| `extern "readline/readline.h" link "readline"` after a `stdio.h` group, `--library` the keg | `CPATH` = keg: `2051` | `CPATH` = keg: `2051`, exit 0; no `CPATH`: **`1026`, exit 0** | none |
| the same with `pkgconfig "readline"`, `PKG_CONFIG_PATH` = keg | `2051` | `2051`, exit 0 | |
| `examples/ledger/db/sqlite.hero`, `link "sqlite3"`, SDK header | exit 0 | exit 0 (§ 1.2) | |

Three readings:

- **Every refusal here carries its rule in its own words.** Each names the file or the header, and
  the places a fetched unit reads; a reader can act on it. The one inexactness is the shared NOTE
  (`package_units.hero`, `constant NOTE`): *searches the runtime's directory, the folders its own
  `pkgconfig` groups' answer names and its own root, and reads nothing else*. It omits the
  toolchain's own directories, which the message above it names and which `hsql`'s `link
  "sqlite3"` builds through, and it omits `CPATH`, which builds both `inc` and `rayl` above. A
  consumer told `ffi_missing_header` for `raylib.h` is not told the one variable that builds the
  program; a package author reading *a library's that a `pkgconfig` group names* is not told that a
  header of the platform's own needs nothing. An `adjacent` message defect, not filed by me.
- **`1026` at exit 0 is not a package defect**: the same program in its own tree prints it too.
  libedit's SDK header against GNU readline's library is the hazard panel 237 measured and kept
  (*clearing `CPATH` ... read libedit's header at exit 0*), and the wrapper form of it is panel
  243's Q2 (`e8/own`, a wrapper `rl.h` including `<readline/readline.h>` under `CPATH`, refused
  `shadowed_header`); I leave both to that sitting.
- **Predictable from spec § 13 alone?** No sentence of § 13 or § 1 says where a fetched package's
  header is looked for. But the plausible mistakes are the author's (a header that climbs, an
  absolute include, a library header no `pkgconfig` names), the author meets them at the
  consumer's build as in § 1.1, and every one of them is refused with a message that names the
  allowed places. None is a wrong value. By panel 237's R8's standard (*a sentence would cost every
  prompt for a case no tracked program meets*): `git ls-files 'packages/*.h'` printed nothing on
  this base, so no tracked package ships a header at all.

## 3. The package-folder stand-in refusals (panel 237's R2 and R3, defect 2357)

`e11/`, fetched through `e11/idx`:

| shape | result |
|---|---|
| package `sint` ships `stdint.h` (`#include_next`, then `INT64_MAX` 7), binds `PK_TOP` = `INT64_MAX` | exit 1, `shadowed_header: heroes.packages/sint@.../stdint.h stands in for the C library's <stdint.h>`, note: *rename the file, or move it out of heroes.packages/sint@...: the package sint is placed in that folder* |
| package `pt` ships `pt.h` (`int x, y`); the program's own folder holds a `pt.h` with `long x, y` | exit 1, `shadowed_header: pt.h stands in for the package pt's pt.h, which a unit reads for that package's group record`, note: *rename the file, or move it out of .* |
| the same, the program's `pt.h` moved away | `3`, exit 0 |

**Settled by panel 237's R8** (*No spec text for items 1 and 2. The specification names no
diagnostic code ... a sentence would cost every prompt for a case no tracked program meets*), and
the premise still holds on this base: 0 tracked `.h` under `packages/` (§ 2), so no package
can meet it, and `grep -c 'error\[' spec/heroes-spec.md` is 0 (run below). The second shape is a
real ABI split (one record laid out `int` and `long`) refused before anything compiles, which is
the property this seat guards; the program's reader can apply its note. The first shape's note,
like C5's in § 1.3, is addressed to the package's author and read by the consumer, who cannot
rename a file of a tree held to its sum: the same message class, one line in the same defect.

## 4. The brief's question, answered by refusal

| refusal (code) | predictable from spec § 13 alone | a sentence? | settled by |
|---|---|---|---|
| a fetched package's float, record or enumerator `constant` (`ffi_constant_type`, `p18`) | **No, and worse: § 13:428 says the opposite**, *A group's `constant` has no body: the header holds the value*, with no exception for a fetched package | **No, by CLAUDE.md § 12** (*Spec beats compiler: the compiler has the bug*): the repair is the compiler's, feature 2358, whose C compiles (§ 1.5). The conservative alternative is draft C below | R9's C5 rules it **provisionally** (*Provisional until measured on the four CI legs at the first version*, `docs/panel/235-*.md`, R9 item 5); feature 2358 lifts every kind I measured |
| a header reading outside the package (`package_reads_outside`), a header not found where a fetched unit looks (`ffi_missing_header`) | No sentence says where a fetched unit looks | **No**: every message names the allowed places; no wrong value; 0 tracked packages ship a header | R9's C1 and C2, and the coordinator's ruling of 2026-11-07 (`pkg-config` folders the machine's); `CPATH` the machine's by `package_reads.hero:21` |
| a file of a package's folder or the program's standing in (`shadowed_header`) | No | **No** | panel 237's R8, its premise re-run (0 package headers, 0 `error[` in the spec) |

**Pricing the sentence I argue against** (CLAUDE.md § 4: the robust route adopted, the conservative
one recorded). In place in my worktree's spec, `heroes-next measure spec/heroes-spec.md` from the
worktree's root, then reverted (`cmp` identical); base `claude-legacy 8167`, `cl100k_base 8314`,
`real 10898`. Each delta is **a lower bound, in those words**; the real count is a paid
`--refresh`, the author's.

| draft | text at § 13:428 | `claude-legacy` | `cl100k_base` | true of every shape? |
|---|---|---|---|---|
| A, appended | `In a fetched package it is an integer or bool reading no enumerator or object: nothing fetched runs at build.` | +26 | +25 | **no**: `cstr` and `ptr` constants build fetched (`e12/`, `hsql`) |
| B, merged | `..., in a fetched package an integer or bool reading no enumerator or object.` | +17 | +16 | **no**, the same two shapes |
| C, merged | `..., in a fetched package never a float, a record or one read through an enumerator or object.` | **+20** | **+19** | yes, for the eight shapes run (`i64` `#define`, `u64`, `bool`, `cstr`, `ptr`, `f64`, `f32`, record, enumerator) |

Draft B, the cheapest, was false on two of nine shapes, which is why it is attacked before it is
priced. The diffs are `p244-scratch/ffi-pragmatist/draft{A,B,C}.diff`.

**Who the sentence would help, measured**: it reaches the package's author, the only person who can
act (§ 1.1), and it would teach them the two routes § 1.4 measured: digits, or a shim whose width
nobody checks (`18` at exit 0). There is a third, the only interim route that keeps the check, and
no message names it (`e13/`): the digits with an assertion in the package's header,

```c
#include <curl/curl.h>
#define HCURLOPT_URL 10002
_Static_assert(HCURLOPT_URL == CURLOPT_URL, "CURLOPT_URL moved");
```

fetched: `10002`, exit 0; the mutant `10003`: `error[ffi_header_refused]: hc.h ... static
assertion failed due to requirement '10003 == CURLOPT_URL'`, exit 1, nothing run. It works for an
enumerator only: for a float the assertion is a GNU fold (`-Werror=gnu-folding-constant` refuses
it) and a fetched `f64` is refused by its kind anyway, and C compares no record.

## 5. Overlap with panel 243 (Q2, defect 2428), data only

Defect 2428's class reaches readline and a fetched package (`e8/`): a wrapper `rl.h` including
`<readline/readline.h>` is refused `shadowed_header` in the program's own tree under
`CPATH=/opt/homebrew/opt/readline/include`, and as a fetched package under `pkgconfig "readline"`
with `PKG_CONFIG_PATH` the keg's, whose note tells the consumer to *move it out of
/opt/homebrew/Cellar/readline/8.3.6/include*. The same binding with no wrapper (a `stdio.h` group,
then `readline/readline.h`) builds and prints `2051` in both trees, and without `CPATH` prints
`1026`, libedit's header against GNU readline's library, at exit 0 in both trees.

## Verdict

- **verdict**: **object**: to any § 13 sentence for the C boundary of packages, the conservative
  draft C included. No sentence for C1/C2 or the stand-ins; C5's refusal closed in the compiler by
  feature 2358, re-classed as the contradiction defect 2347 was (*the specification admits a
  program the compiler refuses*, panel 237's R7: `blocking`), before M-core-packages' tag.
- **section**: design.md §1.11 (*over a C library through a binding it carries where it cannot, and
  verified ... by clang against the header it names*, the core-packages paragraph) and §4.19; spec
  § 13:428; CLAUDE.md § 12. The document does not cover who a fetched package's refusal reaches:
  I found nothing in design.md, R9 or the spec on a refusal that only the consumer can meet.
- **experiment**: §§ 1.1 to 1.5, 2, 3, 4: 18 real constants of four libraries fetched, 9 refused,
  every one building in the program's own tree; shims for all three refused libraries, built
  fetched; a narrowing shim at exit 0; feature 2358's C (splice and C11 static initialiser)
  accepted by clang 21 with defect 2109's shapes refused; a `_Static_assert` copy verified.
  Clang accepted every binding it was meant to accept and refused every one it was meant to refuse.
- **argument**: Only C5's refusal fails the criterion, and not because § 13 is silent: § 13:428
  admits what the compiler refuses, so by CLAUDE.md § 12 the compiler is wrong. It refuses 9 of 18
  constants across four ladder libraries (libm 2/3, raylib 4/4, libcurl 3/3, SQLite 0/8). Only the
  consumer meets it, since the author's `test` and `path` builds pass, and its note's repair is
  refused `package_unread` for the person reading it. A sentence would teach shims, which lose the
  width check (`18` at exit 0), and would write R9's provisional rule into the promised text. Its
  cheapest wording was false for `cstr` and `ptr`. 2358's C compiles with nothing run.
- **prediction**: feature 2358 built as C2's splice for an enumerator and a C11 static initialiser
  of the declared type for a float or a record, `-fsyntax-only`, makes the fetched `hmath`, `hcurl`
  and `hray` of `p244-scratch/ffi-pragmatist/e4/idx` build and print their own trees' values
  (`3.141592653589793`; `10002`, `0`; `65`, `4`, `3.1415927`, `-168430081`), keeps defect 2109's
  `1 << 31` and `P_BAD` refused, and a fetched header's `__attribute__((constructor))` writes no
  file during a cold build. The same on clang 18, which is unrun.
- **condition**: I approve draft C (+20 vendored, a lower bound) at § 13:428 if 2358 is measured
  unbuildable on one of the four CI legs (clang 18 is the floor, unrun here), or the author rules C5
  permanent as Go's stance; either way its shape list is re-run first. I approve no sentence and no
  2358 if the author extends panel 216's yes to fetched headers (R9's recorded conservative
  route). I would veto a sentence or a note that makes bare copied digits the sanctioned repair:
  that is the header verification M-header-constants won, given up.

## Found, not repaired, not filed (proposed classes)

1. **The notes of a fetched package's refusals speak to its author and are read by its consumer**
   (`adjacent`; `blocking` if the synthesis reads *a false message*). `ffi_constant_type`'s C5 NOTE
   (*Write the value so in the package's header*) and a package folder's `shadowed_header` note
   (*rename the file, or move it out of heroes.packages/...*) name repairs that the reader is
   refused: `e5/` applies the first and gets `package_unread`, exit 1. Neither note names what the
   consumer can do: tell the author, or name a copy of the package by `path`. And the first note's
   repair is bare digits, where `e13/`'s asserted copy keeps the check. Reproducers `e1/p18`, `e5/`,
   `e11/f-sint`.
2. **`package_reads_outside`'s NOTE says a fetched unit reads nothing beyond its root, the runtime
   and its `pkgconfig` folders**, and the unit also reads the toolchain's own directories (which the
   message above the note names) and `CPATH`'s folders (`adjacent`, *a true message less exact than
   it could be*). `e10/f`: `raylib.h` with `link` is refused `ffi_missing_header` under `--include
   /opt/homebrew/include`, and the same program builds with `CPATH=/opt/homebrew/include`, which no
   line of either message says. `e9/f-inc` is the same shape with a header of my own.
3. **C5's refusal reaches only the consumer** (the class is the synthesis's: proposed `blocking` by
   defect 2347's precedent, a correct program by spec § 13:428 refused). Its repair is feature
   2358. Reproducer `e2/` (author's `test` and `path`: exit 0) beside `e1/p18` and `e3/` (index and
   `git`: exit 1).

No prototype edit was made to the compiler; the spec edits of § 4 were reverted in my worktree
(`cmp` identical), and their diffs are kept in the scratch folder.
