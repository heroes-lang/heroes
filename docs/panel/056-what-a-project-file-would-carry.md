# 056 — What a project file would carry

**Status**: `RATIFIED 2026-08-15` (was `provisional — author ratification pending`).
**Convened** 2026-08-14 by author decision, from `docs/reasoning/004-a-project-file.md`
and in the same session. **Synthesised** 2026-08-15.
**Lane**: full, five judges, all reporting, all building in copies.

**The sitting did not answer the question it was asked, and that is its result.**
It was convened to decide *whether* Heroes should have a project file. Three
judges, working from different inputs and unaware of each other, converged on the
same discriminator — and it is not whether the file exists but **what it is allowed
to say**. The ballot's option C turned out to name two different artifacts, one of
which is refused by two vetoes and one of which nobody had written down.

## The proposal, verbatim

> Heroes has no project file. Everything a `Cargo.toml` carries is today either in
> the source (`use geom`; `extern "sqlite3.h" link "sqlite3"` / `package "raylib"`),
> on the argv table (`--include`, `--library`, `-O0`/`-O2`, `--sanitize`, `-o`), in
> the machine's environment (`CPATH`, `LIBRARY_PATH`, `PKG_CONFIG_PATH`), or nowhere.
> Should a per-project file that the compiler reads exist, and if so what may it
> contain?
>
> **A** — a Part 6 row: never, with the falsifier design.md:2317 requires.
> **B** — examined and deliberately **unplaced** (panel 039's precedent): a greppable
> paragraph naming the conditions that return it. Zero spec tokens.
> **C** — a minimal project file **now**, carrying only header/library search paths.
> **D** — deferred to **M-package-manager** and named there, giving design.md
> §3.5's already-promised `heroes add`/`heroes fetch` their unnamed object.

## The verdict table

| judge | A | B | C | D | rests on | prediction |
|---|---|---|---|---|---|---|
| llm-ergonomist | object | **approve** | approve-with-condition | object | spec 191-208, and locality | of 20 models given the spec, ≥12 write `link "SDL3"` rather than `package "sdl3"` and ≥11 of those fail to build; **silently-wrong rate 0%** |
| spec-warden | **veto** | approve-with-condition | **veto** | approve-with-condition | §1.0, §1.3, §1.12, §12 | at M-ffi-ladder rung 5's close, `find -name heroes.toml` returns 0 and 0 of 47 corpus programs need a persistent search path |
| compiler-engineer | object | **approve** | object *(not veto — see § jurisdiction)* | approve-with-condition | §1.12, CLAUDE.md §10, §12 | if C lands with **two** keys its diff exceeds +300 lines across ≥8 files and guards ≥10 of the 14 harness spawn sites |
| ffi-pragmatist | object | **approve** | **veto** | approve-with-condition | §1.11, §1.12, §4.19 | `M-struct-passing`'s SDL3 work needs no project file and no location shim on all three CI legs |
| historian | object | **approve** | object | approve-with-condition | precedent, sourced | if C ships, within six milestones the file holds a key that is not a search path, **or** `heroes` grows a flag to ignore it |

**A is refused** — one veto, four objections, and not one judge in favour.
**C is refused** — two vetoes, two objections, one conditional approval.
**B is the only option no judge opposed.**

## The convergence, which is the sitting's product

Three judges reached the same rule from three unshared inputs. None of them was
asked this question; each arrived at it while answering a different one.

- **llm-ergonomist** (spec only): the file may carry **no string that also appears
  in a `.hero` file**. Its veto is on a per-library stanza carrying `header`/`link`,
  because `extern "SDL3/SDL.h"` with no clause is **already a valid program** meaning
  *"these symbols need no library"* (spec 191-192). Such a file adds no syntax; it
  **repurposes a well-formed construct**, so nothing can notice the file went missing
  or was mistyped.
- **compiler-engineer** (the tree): the file may have **one key, not two**.
  `include` and `library` as separate keys reproduce panel 055's skew verbatim; a
  single `prefix <dir>` from which both are *derived* makes the skew
  **unrepresentable**, at 130 lines against 124 — six lines.
- **ffi-pragmatist** (C that compiles): the file may record **the question, never
  the answer** — `package "sdl3"`, optionally a version floor; never
  `-I/opt/homebrew/...`.

One rule in three vocabularies: **a project file may say where to look, never what
to find, and never in a shape that can disagree with itself.** The ballot had no
option for it, because option C as written is the two-key spelling the engineer
measured and the ergonomist's Form 2 is the one it vetoed.

## Why C is refused, measured rather than argued

**The skew is not a wrong number.** The spec-warden built header v1 declaring
`int64_t skew_value(int64_t)` against library v2 defining `double skew_value(double)`:

```
input 6   -> 6    exit 0        input 100 -> 100  exit 0
input 7   -> 7    exit 0        control (matched) -> 12
```

The failure **wears the costume of a correct no-op**: a test asserting
`skew_value(6) == 6` passes while the library doubles its argument. The
ffi-pragmatist reproduced the memory consequence on a staged library whose buffer
grew 16→64 between builds — **exit 134 with no output**, and under `--sanitize`
`AddressSanitizer: heap-buffer-overflow, WRITE of size 64 … 0 bytes after 16-byte
region`. The compiler-engineer reproduced it through a `heroes.project`: **11 where
the honest configuration prints 12**, exit 0, no diagnostic, `--sanitize` silent,
and `heroes mutate` structurally blind (`mutate.rs:39` filters to `.hero`).

**And `table.rs:38-41` was wrong about the hazard.** It said `-I` and `-L` named
*independently* were the only route to the skew. Measured on real SQLite — SDK
3.51.0 against Homebrew's keg-only 3.53.4 — **one header path is enough, because
the library side is never empty**:

```
--include …/opt/sqlite/include   -> header 3053004 / library 3.51.0   exit 0
CPATH=…/opt/sqlite/include       -> header 3053004 / library 3.51.0   exit 0
package "sqlite3"                -> header 3051000 / library 3.51.0   exit 0
```

The second line is the channel `machine_locked_path`'s **own repair note
recommends**. So the hazard predates the flags and is not created by them —
CLAUDE.md §11's class exactly, the premise dead and the argument around it still
reading as correct. The comment is repaired in its own commit citing this sitting.

**What the file adds over the flag is durability, and that is the objection.**
DESIGN-LOG:298 records the author accepting the skew for the flags with a stated
mitigation: *"`package` stays the shape to reach for first."* That mitigation is a
person, typing, per invocation, knowing they are reaching past `package`. **A
project file deletes the person** — read on every build, by everyone who clones,
with nobody typing anything. Under §1.12, of two admissible forms the one that can
be made to lie loses.

## Principle 0, and the burden nobody could discharge

The condition every judge was invited to meet was the same: **name one library that
is bindable with a project file and not bindable today.** None exists on this
machine.

`giflib` is the hardest case here — installed, **no `.pc` anywhere**, and
`/opt/homebrew/include` is not on clang's default path. Verified twice,
independently:

```
extern "gif_lib.h" link "gif"      no flags   -> error[ffi_missing_header]  exit 1
                                   --include  -> giflib major: 6            exit 0
```

The pragmatist checked §1.11's own library table: openssl, libcurl, libpcre2-8,
zlib, sqlite3, ncurses, raylib, sdl3, libzstd all ship a `.pc`; libm and libc are
`in_the_c_runtime`. The warden ran a program using a sibling `use` and the port's
only `extern` group under `env -i` from a foreign working directory with no flags —
**42, exit 0**. Not on the closure list; not compiler-need; no Part 11 effect.

## Precedent — the converged design is the one Heroes already has

Sourced by the historian, who also listed what it could not source and dropped it.

| language | artifact | year | what forced it | what it carries |
|---|---|---|---|---|
| Go | `go.mod` | 1.11, 2018-08-24 | **versioning, not configuration** | 10 directives, **zero flags, zero paths**; cgo's config lives in the source preamble, `// #cgo pkg-config:` |
| Zig | `build.zig.zon` | 0.11.0, 2023-08-04 | the package manager | 6 fields; the doc says *"no include paths, no library search paths, no compiler flags"* |
| Rust | `Cargo.toml` + `links` | 2014 | dependencies | `links` **requires** `build.rs`; search paths are emitted by a *program*, never written in the manifest |
| Nim | `nim.cfg` | — | compiler options | four config layers, each needing its own kill switch (`--skipCfg --skipUserCfg --skipParentCfg --skipProjCfg`); `config.nims` became NimScript, a language |
| Python | `.pth` | — | the one true search-paths-only file | grew code execution; the docs call the one-line limit *"a deliberate measure to discourage putting anything more complex here"* |
| CMake | `CMakeCache.txt` | — | — | *"contain full paths which make them unsuitable for moving between binary trees"* — and it is in the canonical gitignore |

Go ran nine years with positional resolution and no manifest, then added one for
**versioning** and still put no paths and no flags in it. `extern "raylib.h"
package "raylib"` **is** `// #cgo pkg-config: raylib`. Option C is the only option
on the ballot that moves Heroes *away* from the design three languages reached
independently.

One correction to our own text, sourced: **nimble is bundled with Nim**, not a
separate install. CLAUDE.md §6's *"a separate package binary: never"* describes it
imprecisely, and §6 is a bare rule with one citation and no argument behind it.

## Jurisdiction, and a judge that declined to inflate its seat

The compiler-engineer holds a veto for a core construct or a ceiling breach, and
recorded that option C is **neither**: nothing in Part 5, nothing in the type
checker, lowering or backend, `git diff --numstat -- crates/heroes/src/` returning
**0 lines**, and a total of **+247/−13 across 8 files** with 556 tests green.

> "On my own mandate's axis it is free, and I say so: §1.7 does not carry an
> objection here and I will not pretend it does. My objection is a robustness
> objection, and it belongs to §1.12."

Panel 055's option-A cost (+222/−34 across 14 files, reaching `lexer/keywords.rs`)
**does not transfer**, and the sitting says so rather than borrowing the number.

## Findings that are not about the ballot

Each is measured, and none was asked for.

1. **`table.rs:38-41`'s premise is false** — one header path skews, and `CPATH`
   does it too. Repaired in its own commit citing this sitting.
2. **A missing search directory is ignored in silence.** `clang -Wall -I/no/such/dir`
   says nothing; `-Wmissing-include-dirs` is not in `-Wall` nor in `FLAGS`, so a
   committed invocation that pins Homebrew's prefix builds against the SDK's headers
   on another machine, clean, exit 0. **The repair is not a one-line flag**: added to
   `FLAGS` it exits **2** — *"internal error: compiling the generated C failed"* —
   blaming the compiler for the author's flag, and it turns
   `a_search_path_reaches_clang_as_one_argv_word` red, the very test CLAUDE.md §11
   requires for the no-allow-list exemption. Classifying it exit 1 would break §7's
   stated narrowing, which is `declaration()`: every exit-1 class recovers a name
   **this program declared `extern`**, and a directory is not a declaration. Queued.
3. **`function malloc(size: i64) -> ptr` exits 2 with raw clang output**, because
   `unsigned long` is absent from `ffi_narrowed.rs::heroes_spelling`. `u64` binds
   `malloc` and `strlen` correctly. A fifth exit-1 class CLAUDE.md §7 does not have.
   Queued.
4. **CLAUDE.md §10's shape table has three slots** — subcommand, flag, nothing. A
   project file is a **fourth**: a new input class. §10 does not admit one without
   being amended.
5. **CLAUDE.md §9's `#~` invariant cannot reach a non-`.hero` file.**
   `golden.rs:344-372` collects `.hero` cases only, so a project file's diagnostics
   would be the one test class in the repository exempt from §9 — which panel 020's
   historian refused for `unsupported`.
6. **Discovery is a function of how the path was spelled, not of the file.** Same
   source, three spellings, three cache keys: from the root it finds
   `./heroes.project`; from inside the directory it finds nothing; canonicalising
   puts absolute paths in the pipeline, which design.md:655-659 forbids. From an
   absolute source path the ancestor walk **leaves the checkout** — a file one level
   above the repository changed the build.
7. **Hermeticity**: a malformed project file at the repository root turned
   `the_emitted_c_is_byte_identical_twice_and_through_o` and the golden run cases
   red, from one untracked file. There are **14** `CARGO_BIN_EXE_heroes` spawn sites
   and none guards against it. And the double-emit test **cannot see the feature at
   all** — `--emit-c` returns before `Toolchain::find()` — so its green means nothing
   here.

## Resolution — provisional, author ratification pending

**B is adopted**: compile-time project configuration is **examined and deliberately
unplaced** — neither Part 6 nor Part 7 — with a greppable paragraph in design.md
§3.5 naming what would return it. Zero spec tokens, zero code, and it is panel 039's
precedent applied to the same shape of question.

**A is refused.** The falsifier a Part 6 row owes is constructible today: *"a
program that cannot be built without a persistent path"* becomes writable the day
`heroes add` exists — and design.md §3.5 **already promises** that verb. A document
cannot permanently reject the object of a verb it commits to shipping.

**C is refused**, two vetoes, on §1.12 rather than on cost.

**D is adopted as B's companion and its condition**: the return conditions are
written into the **ROADMAP's M-package-manager entry**, not Part 7 — Part 7's
preamble claims its items lose *only* on simplicity, which is false of this shape,
and that is exactly why panel 039 vetoed Part 7 for comptime. It is **re-decided,
never renewed**.

**The three conditions that would return the question**, and they are the
convergence stated as tests:

1. **One key, not two.** A `prefix <dir>` from which `-I` and `-L` are derived, so
   the skew is unrepresentable. Two independent keys are refused.
2. **No string in the file may also appear in a `.hero` file.** Mechanical: grep the
   file's values against the sources; a match is a violation. Deleting the file must
   never change *which symbols are linked* — only whether the build succeeds.
3. **One named binding that `package "<name>"` and one command line cannot build.**
   Today that set is empty, and giflib — the hardest case on this machine — is not
   in it.

Meeting all three returns the question to a panel. Meeting fewer does not.

## Predictions to score

| judge | prediction | scored at |
|---|---|---|
| llm-ergonomist | ≥12/20 models write `link` over `package`; ≥11 of those fail to build; silently-wrong rate 0% | first Part 11 run with a model harness |
| spec-warden | `find -name 'heroes.toml'` returns 0 and 0 of 47 corpus programs need a persistent search path | M-ffi-ladder rung 5's close |
| compiler-engineer | two-key C exceeds +300 lines across ≥8 files and guards ≥10 of 14 spawn sites | C's landing, or M-selfhost-probe |
| ffi-pragmatist | SDL3 struct work needs no project file on all three CI legs | M-struct-passing |
| historian | within six milestones the file holds a non-search-path key, or a flag to ignore it appears | six milestones after any C |

## Process notes

- **All five judges built in copies**, and each said so with its path — the rule
  panel 054 paid for. Two improvements are owed to it: a copied `target/` leaves
  `env!("CARGO_MANIFEST_DIR")` pointing at the **real** repository, so a judge's
  golden run silently measures the wrong tree. The engineer found this, discarded
  that run and rebuilt. **`/panel` step 3 should say `rm -rf target build` after the
  copy.**
- **The coordinator changed the real working tree mid-sitting** — `-Werror=missing-include-dirs`
  was in `FLAGS` between 23:37 and 23:45 — and a judge observed the effect from
  inside its own measurement. It was reverted before any commit and the suite is
  green at 550 tests, but the rule the judges are held to should bind the
  coordinator during a sitting too.

## Ratification — 2026-08-15

**RATIFIED as it stands** (author instruction, `/decide`: *"le ratifiche ratifica
tutto per me"*). **B stands adopted with D as its companion**, **A stays refused**,
**C stays refused on its two vetoes**. The three return conditions are the
decision's operative half and are unchanged: one key not two, no string in the file
that also appears in a `.hero`, and one named binding that `package` plus one
command line cannot build. Meeting all three returns the question to a panel;
meeting fewer does not.

**The item put one thing to the author that the yes does not silently answer**, and
it is recorded here so it is not read as settled: if C is ever overturned, the shape
the judges would accept — **one key** — was **not on the ballot** and is six lines
dearer than the two-key form that was. A future sitting must price the one-key form
itself rather than inherit C's measurement.

**Panel 039 was ratified in the same instruction**, which matters for this file:
B is 039's precedent applied to a second shape of question, and it would have been
a decision standing on a provisional one for as long as the two were split.

The limit is the 2026-08-12 blanket's limit. Four of the five predictions above
name instruments that do not exist yet (the model harness, rung 5's close, C's
landing); the yes does not score them, and R2's *re-decided, never renewed* rule
governs them exactly as it did before.

## Appended 2026-09-04 — the nine jobs, moved here from the retired reasoning note

The note this sitting was convened from was retired on 2026-09-04 by author
instruction, with the rest of `docs/reasoning/`; its text stands at
`git show 2689d606:docs/reasoning/004-a-project-file.md`. **Two blocks move here
rather than into the record**, and the reason is this file's own § Process notes:
the note was legalised on 2026-08-15 (`docs/work/DONE.md:456`) precisely because
it held *"the path to the question, the inventory it was measured against, the
tension that produced the proposal"* — things the panel file does not say. Moving
them here rather than deleting them is what keeps that ruling true; the mechanical
test it set was *"if the note can be deleted without losing anything the panel
file does not already say, it should not have been written"*, and this is that
test answered in the only honest direction.

**The `.rs` paths in the table are the bootstrap's, as the note measured them on
2026-08-14** — five days before `crates/` became `archive/bootstrap-rs/`. They are
repointed rather than kept, because a citation into a moved tree reads as current
(CLAUDE.md §11); where a live counterpart exists it is named beside it.

### The nine jobs, and who holds each one today

| # | what a `Cargo.toml` carries | where Heroes puts it | the rule that puts it there |
|---|---|---|---|
| 1 | `[package]` — name, version, licence | **nowhere**; no publishable unit exists | — |
| 2 | `[dependencies]` | `use geom` reads `geom.hero` **beside the file** (`archive/bootstrap-rs/heroes/src/modules/mod.rs`; live: `selfhost/module/paths.hero`) | `design.md` §3.5 — *"a v1 'dependency' is a link flag declared in the source next to the `extern` that needs it"* |
| 3 | native linking (`links`, `build.rs`) | `link "sqlite3"` / `package "raylib"` in the group head | `design.md` §4.19; panels 036, 050 |
| 4 | search paths `-I` / `-L` | `--include`/`--library`, plus `CPATH`, `LIBRARY_PATH`, `PKG_CONFIG_PATH` | panel 055; `machine_locked_path`, live at `selfhost/parse/decl.hero:550` (was `syntax/externs.rs:110-136`) |
| 5 | `[profile]` — opt level, debug | per-verb defaults (`build` −O0, `run` −O2, `test` −O0) and `-O0`/`-O2`/`--sanitize` | `CLAUDE.md` §10 |
| 6 | `[[bin]]`, entry point | *"the file you compile holds `main`"*; one file operand, maximum (`archive/bootstrap-rs/heroes-cli/src/cli/table.rs`) | `design.md` §4.1 |
| 7 | `[workspace]`, module paths | beside the file: no path, no search list, no hierarchy | `design.md` Part 7 item 4 — *"no package hierarchy"* |
| 8 | `[target.'cfg(…)']` | `runtime/hero_os.h`; `package` is **one spelling per platform** | panel 049 — the platform axis refused at +144, with a veto |
| 9 | toolchain pinning | the flag list names `-std=gnu11` (`archive/bootstrap-rs/heroes-cli/src/commands/toolchain.rs`; live: `selfhost/cli/flags.hero`); clang pinned at the fixpoint | panel 047 — *"`rust-toolchain.toml` pins the Rust, the flag list names the C"* |

**The right-hand column is the finding.** For seven of the nine, what stands
against a manifest is not a preference about file formats: it is that the fact
already has a home and the home has a stated reason. Rows 1 and 7 are the
exceptions — they have no home at all, and both belong to the same unbuilt
milestone, which is `M-package-manager` and where this sitting's three return
conditions now live (`docs/ROADMAP.md` § M-package-manager).

### A project file was not on panel 055's ballot, and that is how the gap was found

Panel 055 put four options: **A** a path in the group head · **B** nothing in the
language, the machine is asked · **C** `--include`/`--library` on the argv table ·
**D** wait, because `package` may have met the need. A per-project file is none of
them.

That sitting's own reason for refusing A is what makes the gap visible. The
llm-ergonomist vetoed A on locality: *"one clang invocation means one `-I` set, so
a path in group 1's head changes how group 2's header resolves."* That is an
argument that a search path is a **whole-program fact wearing a per-group
spelling** — and it says nothing against a whole-program home. Of the three
whole-program homes that exist, the environment was adopted, the flags were queued
and then landed, and the third was never written down. The header comment of
`examples/sdl/main.hero` named the shape of the hole from the other side: *"There
was no third door."*
