# 057 — The path pair

**Status**: `provisional — author ratification pending`.
**Convened** 2026-08-15 by author instruction, from `docs/debrief/DECIDE.md`'s
path-pair item (queued by author instruction 2026-08-14, shaped in `/decide`
2026-08-15).
**Lane**: full, five judges, all reporting, all building in copies.

**Not one judge took the proposal as written, and the two defects that killed it
were found by running it rather than by reading it.** Three vetoes, from three
different sections. What survives is not a narrowed version of the proposal — it is
a different function with a different return value, which no ballot listed, plus a
placement the proposal never considered.

## The proposal, verbatim

> Adopt two built-ins:
> - `path_join(dir: str, name: str) -> str` — the two pieces with exactly one `/`
>   between them, unless `dir` already ends in `/` or `\`.
> - `path_parent(p: str) -> str?` — the text before the last `/` or `\`, absent when
>   there is none.
>
> Neither normalises: no `\`→`/` rewriting, no `..` collapsing, no case folding, no
> symlink resolution.
>
> Measured spec cost: the pair **+79** (3208→3287); `path_parent` alone **+41**
> (3208→3249). Headroom 888 of 4096.
>
> §1.0 argument exists for `path_parent` only: the port's module loader must find a
> path's directory part, which `modules/mod.rs::directory_text` does today with
> `rfind(['/', '\\'])`. `path_join` has no §1.0 argument — no corpus program builds
> a path, and `dir + "/" + name` is already correct on all three platforms.

## The verdict table

| judge | pair | `path_parent` alone | neither | section | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto** | **veto** (as built-in) | **adopt** | §1.11 (design.md:466–478), on §1.7 and §1.0 | a compiling Heroes port of `modules/mod.rs` that calls it *and* keeps the Windows-root spelling |
| spec-warden | **veto** | refuse as spelled | — | §1.0's burden, on §4.6, §1.11, §1.2, §1.6 | re-put as `path_dir`, separator **kept**, `""` on absence (+41 measured) |
| ffi-pragmatist | **veto** (the `\` clause) | refuse the *placement* | **adopt** (as Tier 2) | §1.11, §1.12, CLAUDE.md §11 | separator set is `/` **only** |
| llm-ergonomist | refuse `path_join` | adopt-with-condition | second choice | — (blind: spec only) | `path_parent` returns a **directory**, not a substring: `.` when there is none |
| historian *(advisory)* | adopt-with-condition | — | — | precedent | `path_parent("/a")` → `/`; the `\` clause gets a falsifier or goes |

**Nothing enters as a built-in.** Three vetoes, and the two non-vetoing seats both
refuse `path_join`.

## The two defects, both found by executing

Neither was in the proposal's own risk list, and both are silent.

**1. The empty parent composes to an absolute path** (llm-ergonomist, writing
blind from the spec). Asked to read a file sitting beside a configuration file, it
wrote — as its *first draft*, under both proposed variants:

```
sibling = path_join(path_parent(a[0]), "defaults.txt")
```

For `prog app.conf` — a bare filename, the ordinary way a tool is run in the
directory it configures — `path_parent` yields nothing and the line produces
**`/defaults.txt`**, an absolute path at the filesystem root. It compiles, it
type-checks, and it reads exactly like the task sentence.

**Its program written with no built-in at all was correct**, because writing the
scan by hand made it return `"."`. That inversion is the sitting's sharpest single
result: the proposed built-in made a blind reader's program worse than the language
without it. `path_join` does not rescue it — `path_join("", "defaults.txt")` is
`/defaults.txt` by the proposal's own rule — and it makes the line *harder* to
review, because the `/` moves out of the source into the runtime and there is
nothing left to look at.

The cause is precise: the proposal defines `path_parent` as **text extraction**, so
`"/foo"` and `"foo"` both yield the empty string — and those two need opposite join
behaviour. POSIX `dirname` returns `/` and `.`, two different values.

**2. One directory, spelled two ways in one string** (compiler-engineer, executing
the pair):

```
path_join(path_parent("examples\calculator\main.hero").default(""), "lex.hero")
  → examples\calculator/lex.hero
```

This is the defect `modules/mod.rs:78-85` records as found by the **third CI leg on
2026-08-14** and fixed by *not joining*. The port needs the separator **retained**
and `""` on absence; `path_parent` gives neither.

## What the `\` clause actually costs, measured through a real library

The ffi-pragmatist ran §4.19's ladder rung 3 against a real POSIX directory named
`back\` — a legal name on Linux and Darwin:

```
A path_join   path=[e2root/back\l.db]   OPENED
B join_posix  path=[e2root/back\/l.db]  OPENED
```

Both returned `SQLITE_OK`. `find` afterwards shows **two different databases**,
both opened successfully, **no diagnostic anywhere**. The same split appears through
`hero_file_read`: `path_join` gets `No such file or directory` where `dir + "/" +
name` reads the file.

That is §1.12's class exactly — not a crash, a *correctly-formed string naming a
different object* — and it is handed to every path-taking C function the language
reaches: `sqlite3_open`, `fopen`, curl, raylib's loaders. Every one returns success.

**The historian found the same hazard shipped, twice, in both directions.**
filebrowser's fix for CVE-2026-54093 rewrote `\`→`/` in archive entry names; on
POSIX that rewrite **manufactured** a traversal out of `..\..\x`, and they replaced
it with an inert character (GHSA-83xp-526h-j3ww). Git paid the mirror defect
(CVE-2019-1354) and its fix then broke legitimate repositories holding legal
backslash filenames (git-for-windows#2435). Zig splits `dirnamePosix` (only `/`)
from `dirnameWindows` (both); Node splits `path.posix` from `path.win32` for the
same reason.

**And the clause is CLAUDE.md §11's named failure mode**: *"`\` is a separator"* is
a premise about the world, welded into a value-level operation, placed in the one
file whose recorded claim is that it holds none (`runtime/parts/os.c:70`).

## The branch the proposal called "neither", priced by two seats

Both building judges independently wrote it in ordinary Heroes and ran it.

| | compiler-engineer | ffi-pragmatist |
|---|---|---|
| where | `library/source.hero` (Tier 2) | Tier 2 |
| size | **8 lines** | **25 non-blank lines** |
| compiler lines | 0 | 0 |
| runtime lines | 0 | 0 |
| ABI bump | none | none |
| spec tokens | 0 | 0 |
| verified | ran, correct on all three shapes | byte-identical to the C; `str?` falls out of `ok`/`fail`; **rung 3 opens a computed path through real `sqlite3.h` and reads back `42`** |

The two line counts differ because the scopes do — the smaller is the pair alone,
the larger carries the POSIX-only variants and their exercises. **Both are recorded
rather than reconciled**: the panel's finding is that the branch costs nothing in
the compiler, and the exact figure is a drafting question for whoever writes it.

**§1.11 already settled this class**, and the compiler-engineer quotes it against
the proposal: a `T?`-returning edge is Tier 2 and not a built-in, because *"a `T?`
is a per-translation-unit generated struct … the built-in route needs a second
composition arm in the emitter and an ABI bump, and the Tier-2 route needs zero
backend lines."* That is `read_file -> str?`'s own sentence, and `path_parent ->
str?` is it verbatim. The pragmatist reproduced the argument from the other side by
trying: it **could not write `path_parent` returning `str?`** at all, and had to
fall back to `hero_file_read`'s out-parameter shape.

**Cost as a built-in, for contrast** (compiler-engineer, per file): `path_join`
~83 non-test lines and ABI 13→14; `path_parent` ~140–160, *dearer rather than
cheaper*, because the runtime cannot return a `T?` — so no `builtins::entry` row, an
inline composition arm in `emit/ops.rs`, a new emitter file, and
`hero_failure_no_parent()` in `failure.c`. It would also take `types/builtins.rs`
from 285 to **310**, past CLAUDE.md §11.

## The C alternative is closed, and it is closed on §12

§1.11 says everything comes from C, so the strongest form of *neither* is to bind
`dirname(3)`/`basename(3)` and add nothing. The ffi-pragmatist priced it and it does
not survive:

- `dirname` is `char *` on glibc, musl, mingw **and** Darwin — all four headers
  checked — and glibc additionally `#define basename __xpg_basename`, shipping
  **two incompatible functions** selected by which header is included.
- A literal's `.cstr()` is `HERO_STR_STATIC`, i.e. read-only: the write **faulted**,
  `SIGSEGV/SIGBUS`.
- A heap `str`'s `.cstr()` **accepted** the write and left `len=17` over 9 bytes of
  content — a `str` silently desynced from its own length.
- `libgen.h` is `fatal error: file not found` under `-target x86_64-windows-msvc`,
  and the CI's Windows leg runs plain `clang`.

A C function that segfaults on a static string is what CLAUDE.md §12 forbids
reaching for. **The built-in is the safer of the two available options, not the more
indulgent one** — which is the opposite of how the proposal framed the trade.

## Where the judges disagree, unsmoothed

**Three seats gave three different answers to *what does it return when there is no
separator*, and all three are reasoned from evidence.**

| seat | `path_x("app.conf")` | `path_x("/a")` | why |
|---|---|---|---|
| llm-ergonomist | `.` | `/` | the only value that composes without producing an absolute path — measured on its own first-draft programs |
| spec-warden | `""`, separator **kept** | `/` | it is what `directory_text` actually computes, and it makes the join `+` |
| historian | **absent** | `/` | Zig's `dirname` and Java's `getParent`, sourced, with Zig's test suite verbatim |

They agree on `/a` and split on the bare name. The warden's shape is the one that
dissolves the ergonomist's defect *without* needing `.`: if the separator stays
attached, `"" + "defaults.txt"` is `defaults.txt` and `"/etc/" + "defaults.txt"` is
`/etc/defaults.txt`, both correct, and `path_join` becomes unnecessary — which is
why the same seat strikes it.

**This convergence is the sitting's constructive result and it was on nobody's
ballot**: a function that keeps the separator, spelled `path_dir`, measured **+41**.
It is not adopted here, because the seats that would have to build it vetoed the
*placement* rather than the shape, and a shape with no agreed home is not a
decision.

## Resolution — provisional, author ratification pending

**Neither built-in enters.** Three vetoes and no seat in favour of the pair as
proposed. Principle 0 is the ground: `path_join` is conceded to have no closure-list
claim, and *"the inverse of the function that does"* is the elegance argument §1.0
exists to refuse. `path_parent`'s claim was `directory_text` — **and it does not
compute it**, measured twice.

**The `\`-as-separator clause is refused outright**, and this is the part with a
falsifier already fired: a real POSIX file named `back\` opens two different
databases with no diagnostic. Any future path work in this language uses `/` only,
or ships the platform split Zig and Node both arrived at.

**Nothing lands, and the port writes the scan once** — option (c) of the original
item, in `modules/`, with `directory_text`'s comment carried over, at **0 spec
tokens**. The Tier-2 alternative is measured, recorded above, and available at zero
compiler cost whenever it is wanted; whether `library/source.hero` gains it is a
smaller question than this sitting and is queued rather than decided, because a
library addition is still a spec line and the seats disagreed about the shape it
would carry.

**What a veto compels**, per seat, recorded so the author can overturn knowingly:

- *compiler-engineer*: a written answer to why the class §1.11 settled at panel 036
  — a `str?`-returning edge is Tier 2 — is reopened for a helper not on the closure
  list.
- *ffi-pragmatist*: if the `\` clause is kept anyway, `runtime/parts/os.c:70`'s
  claim must be **struck, not amended** — the file would then do something to a path
  — and a `tests/fixedbugs` case named after the two-databases experiment is owed,
  because §11 requires a premise about the world to ship with a test that fires when
  it dies.
- *spec-warden*: the pair does not land as a unit; `path_join` returns to `DECIDE.md`
  under §1.0 and `path_parent` is either re-put as `path_dir` or waits with it.

**Two drafting defects in the proposal are recorded rather than quietly fixed**, both
found by the warden: *"or nothing when there is none"* names a value the language
does not have — `T?` is `ok`/`err` (§4.6), and "absent" is not a Heroes value — and
the honest wording measures **cheaper** (+40 against +41); and the clause spelled the
separator `` `\\` ``, which is the spelling spec line 67 already uses for the *escape
sequence*.

## Findings that are not about the ballot

- **`.cstr()` handed to a C function that writes is a live hazard, and it is wider
  than this sitting.** Measured here: a literal's `.cstr()` is `HERO_STR_STATIC` and
  faults on write; a heap `str`'s `.cstr()` **accepts** the write and leaves the
  `str` desynced from its own length — `len=17` over 9 bytes. Panel 053 settled the
  *inbound* null `cstr`; this is the outbound mutable one, and nothing in §4.19
  addresses it. Queued.
- **`directory_text` has the refused clause today.** `modules/mod.rs:91` does
  `rfind(['/', '\\'])` on every platform. Its blast radius is small and stated in
  its own comment — it decides the *display name*, and the filesystem is asked
  through a real `Path` — but the premise is the one this sitting refused, and it
  should carry a falsifier or lose the `\`. Queued.
- **The proposal's non-normalising half is well-precedented and should be reused if
  path work ever returns.** Zig, Rust and .NET all ship non-normalising joins, and
  .NET moved *toward* that form for stated security reasons. Go's `filepath.Clean`
  cites Rob Pike's Plan 9 paper — **and Plan 9 has no symlinks**, which is the
  enabling condition that did not travel; it produced CVE-2022-41722, where
  normalising *created* the traversal. Go has since shipped `IsLocal`, `Localize`
  and `os.Root`.
- **The precedent running against the whole shape**: Java (`java.nio.file.Path`),
  Python (`pathlib`, PEP 428) and Rust all replaced string paths with a path
  *type*, and Go's 2025 endpoint stops manipulating path text at all. Principle 0
  forbids proposing that here. It is recorded so nobody later calls it an oversight.
- **`types/builtins.rs` is 285 lines** and any built-in addition takes it past
  §11's ~300 — which meets the §11 drift item already queued 2026-08-15.

## Predictions to score

| judge | prediction | scored at |
|---|---|---|
| llm-ergonomist | Harness T3 (`prog app.conf`, no separator), 20 first-try programs per variant: pair-as-written ≥10/20 read the absolute `/defaults.txt`; parent-only ≥8/20; base ≤6/20; corrected (`.`) ≤2/20. Compile errors catch **0** in every variant. **The counter-intuitive clause: base beats both proposed variants** | first Part 11 run with a model harness |
| compiler-engineer | At M-selfhost-probe the Heroes port of `modules/mod.rs` calls `path_parent` **zero** times — its one consumer needs the separator retained and `""` on absence. Falsified if the port calls it without re-appending a separator and a `.default("")` | M-selfhost-probe |
| spec-warden | Adopt the pair and at M-selfhost-probe `grep -c path_join` over `library/*.hero` + `examples/` + `tests/golden/run/` stays **0** outside the built-in's own test. Instrument and its arm: the Windows CI leg, which fired on this class in `fad0cd2`. Price of being wrong: +79 measured tokens for 0 call sites | M-selfhost-probe |
| ffi-pragmatist | Under `path_join`, `heroes build examples\calculator\main.hero` on Windows emits sibling module names spelled `examples\calculator/lex.hero`; the file **opens** (Win32 normalises `/`) and the failure is a golden `.expected` byte mismatch, not file-not-found. Standing: SQLite rung 3 needs no built-in and no shim under the *neither* branch — already checked, exit 0 | M-selfhost-probe |
| historian | If the `\` clause is ever kept unconditionally, its first defect is **not** a traversal but a Heroes source or cache path legitimately containing `\` on macOS/Linux resolving to the wrong parent — a spurious "module not found" or a cache miss. Settleable **today**: create a file whose name contains a literal `\` and run the rule over its absolute path | any time; no milestone needed |

## Process notes

- **All five judges built in copies and removed `target/` first**, the rule this
  repository amended into `/panel` step 3 earlier the same day after panel 056 paid
  for it. No judge reported the `CARGO_MANIFEST_DIR` symptom.
- **The working tree was frozen for the whole sitting** — the other half of the same
  amendment, and its first observance. No commit between the briefs going out and
  this synthesis.
- **The coordinator's own proposal is the one that lost.** It was drafted in
  `/decide` on 2026-08-15 and put to the panel unchanged, including the `str?` return
  and the `\` clause that three seats refused. That is the instrument working: the
  measurements were run by seats that did not write it.
