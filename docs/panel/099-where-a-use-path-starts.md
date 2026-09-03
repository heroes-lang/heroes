# Panel 099 — Where a `use` path starts: the file you compile, from every seat at once

**Convened** 2026-09-02, opening M-package-layout. **Trigger** surface syntax and
semantics, `spec/**` (CLAUDE.md §4). **Lane** full — the spec must say a sentence,
so all five seats sat. **Status** `provisional — author ratification pending`.

**This sitting reopens a ruling, not a silence**: panel 032 (RATIFIED 2026-08-12)
adopted S (stay flat) and fixed R4 as the landing form if the author overruled.
The author scheduled this milestone on 2026-08-25 to rule on exactly that
(`DESIGN-LOG:435`), so what 032 fixed is inherited, not re-derived: R4 (the form
and the qualifier — `use syntax/decl` binds `decl`, last parts unique), R5 (the C
component is the whole path), R6 (`use` is scoped to the program).

## The proposal, verbatim

> R4 does not say where a path starts. Two variants, each one sentence added to
> § Files and layout after R4's line:
>
> **ROOT** — `A path is read from the compiled file's directory, whichever file
> writes it.` (measured 3636, +44 over today's 3592)
> **FILE** — `A path is read from the directory of the file that writes it;
> `..` climbs one level.` (measured 3641, +49)
>
> The ergonomist received the two label-stripped as variant-2/variant-1, after a
> blind phase 0 on a spec carrying only R4's line (3618, +26 — re-confirming
> 032 R4's own measurement on today's spec).

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **approve ROOT · object FILE** | the root is **already the only base in the code**: `modules.hero:144` opens every module at the compiled file's directory + name — "beside the file" coincides only because the tree is flat. ROOT ≈95 non-test lines, five frontend files, **zero** in the mangler, the TU cache, the emitter; FILE ≈180 plus a canonicaliser five comparison sites must never forget |
| llm-ergonomist | **approve variant-2 (ROOT) with condition · approve variant-1 (FILE) second** | blind phase 0 wrote root-relative spellings unprompted; ROOT's one defect as tabled: a bare `use util` from a subdirectory is claimed by two sentences at once, so a root module has no licensed spelling — one sentence fixes it |
| spec-warden | **approve w-root-min only (3630, +38) · object both as tabled** | the removal of ", beside this file," (−4) is **mandatory** under ROOT — the clause becomes false for every importer below the root; `..` alone prices at +7 with three silences unclosed (repetition, mid-path, escape) |
| ffi-pragmatist | **approve ROOT · object FILE** | moving the SQLite binding into a directory changes **zero** of the 7 linker-resolved symbols (compiled, `nm`-diffed); FILE's two collision shapes compiled to `redefinition of 'h_token_next'` and an undefined-symbol link error, both exit 2 **on legal programs** — 032 D1's class |
| historian (advisory) | **support ROOT · oppose FILE** | no language on record ever moved anchored→relative; every documented removal (PEP 328, Rust RFC 2126, Go's module-mode ban) killed a spelling with two candidate meanings. The nearest shape is Zig #216, and Zig's recorded pain is entirely the **moving root** |

Five seats, no seat for FILE. The disagreements below are about conditions, not
direction.

## What was measured, seat by seat

**The engineer's map of module identity** — one string, one funnel, and ROOT
preserves it. The use line's text is the module's identity from `span_text`
through `seen` (`modules.hero:126`), the cycle graph, `unknown_modules`, and
`FileEntry.component` (`source.hero:175`). The `/` in a ROOT path is already the
separator `load_text` opens with, so discovery's open path changes zero lines.
FILE splits spelling from identity: five sites today would each need
canonicalisation, and a missed one is a measured double-load — one disk file,
two modules, two TUs, exit 0. Lexer cost of `..`: zero either way (`..` is two
`.dot` tokens); backslash cannot lex, so `/` is forced on every platform. The
cache key and the on-disk TU names carry only the sanitised alnum component
(`cli_units.hero:81-92`), so Windows path mapping is untouched.

**The ergonomist's experiment.** Phase 0, blind, on the R4-only spec: it
designed the 6-file 3-directory program with `use lex/token` written from
`syntax/parse.hero` — the root-relative spelling, unprompted — and flagged its
own `use util` as a guess. Phase 1: under FILE its program is illegal (two
lines, both loud); under ROOT `use lex/token` becomes licensed verbatim and
`use util` stays torn between two sentences — its condition. It also built the
cross-variant witness: a program legal under **both** variants that prints 2
under FILE and 3 under ROOT (a same-basename decoy one level apart), which is
the proof the spec cannot stay silent on the base. And it named FILE's silent
shape: `..` miscounts are loud, but a same-basename decoy at the wrong level
binds at exit 0.

**The warden's ledger** (every number run with `./heroes measure`, spread 76):
today 3592 · R4 alone 3618 · ROOT as tabled 3636 · FILE as tabled 3641 ·
removal alone 3588 (−4) · **w-root-min 3630 (+38)** · w-file-min 3635 · FILE
without `..` 3628 (so the climb operator alone is +7, before its silences are
closed). Its finding on ROOT as tabled: "the compiled file's directory" is
ambiguous the day per-module TUs exist — the spec's own defined term is "the
file you compile" — and left beside the untouched `use geom` bullet it
specifies a **mixed-base** system. w-root-min fixes both and is cheaper.

**The pragmatist's compiled artifacts.** Baseline `examples/sqlite` runs (exit
0, `rows: 3`). The naive two-file split was refused by the compiler itself
(`extern_across_modules`), so every real binding ships wrappers — the binding
module has the program's highest fan-in. Renaming module `sqlite` →
`db_sqlite` (byte-identical component to what `db/sqlite.hero` yields under
R5): the 7 undefined symbols are identical before and after; the `extern
constant` accessor moves `_h_sqlite_SQLITE_OK` → `_h_dbsqlite_SQLITE_OK`
(panel 038's exception, behaving as designed). R5 re-measured on today's tree:
a plausible `selfhost/` layout gives **13 last-part collision groups over 32
modules vs 0 whole-path collisions** (repo-wide: 12 groups — `main` ×20 across
separate programs, R6's caveat — vs 0). And the wire where a FILE spelling
would leak into the mangler is one line: `source.hero:175`.

**The historian's asymmetry, sourced.** PEP 328 killed Python's
relative-then-absolute *fallback* (kept explicit `from . import`); Rust 2018
killed one-syntax-two-rules by making every path carry an anchor; Go forbids
relative imports in module mode ("you need to know what they are relative to",
plus no canonical name for crash dumps). Node's relief valve for `../../`
chains was a manifest (`#` imports in package.json) — the thing §10 refuses.
Nim's search-path scheme grew `std/`/`pkg/` pseudo-directories as an ambiguity
patch. Nothing documented ever moved anchored→relative. Decisive for Heroes'
exact shape (no manifest, no search path): the mangler needs one canonical
module identity, and the only canonical name available without a manifest is
the root-relative path.

## The one real hazard, named by three seats and not smoothed over

The historian's flip condition (b) is live in this repository: `heroes test
selfhost/lexer.hero` hands the compiler a non-main file of a multi-file tree
today — the ROADMAP's own command list carries it. Under ROOT, the compiled
file's directory **is** the root (the engineer confirmed no other root notion
exists anywhere in the code, and §10/panel 056 forbids the project file that
could define one), so testing a leaf that sits **below** a program's root
re-bases every path in it. Today that is harmless — `selfhost/` is flat, leaf
and root share a directory — and the day the tree nests, the historian predicts
this as the first defect, in Zig #13970's shape. The sitting does not solve it
by silence: R5 below makes it a stated rule with a diagnostic and a golden, and
the prediction is registered to be scored.

## Resolution — `provisional — author ratification pending`

**R1 — ROOT is adopted: a `use` path starts at the directory of the file you
compile, the same from every file of the program.** Five seats, none for FILE.
The engineer's invariant (spelling **is** identity, no canonicaliser anywhere),
the pragmatist's compiled collision pair under FILE, the warden's cheaper
ledger, the ergonomist's blind phase 0, and the historian's one-way record all
point the same direction independently.

**R2 — the landing form is the warden's w-root-min, measured 3630 (+38), and
the removal is part of the form.** Three edits in one commit to § Files and
layout: drop ", beside this file," from the `use geom` bullet (it becomes false
for any importer below the root — keeping it writes a contradiction §12 would
then enforce); add R4's line unchanged; add `Every `use` starts at the
directory of the file you compile.` This one-base wording also discharges the
ergonomist's condition: a bare `use util` is root-relative like every other
`use`, so a root module has exactly one licensed spelling from everywhere.

**R3 — spelling is identity, and the C component is the sanitised whole path.**
`FileEntry.module` carries the path as written (under ROOT that text is already
canonical); `module_of` loses its `stem_of` (`source.hero:95-96`, `:175`) and
becomes sanitised segment concatenation; `module_names_collide` compares
**components**, not names — 032 D1's repair, extended. The pragmatist's
approval is conditional on exactly this, and the engineer's prediction is
falsified if the funnel that ships is not the funnel it measured.

**R4 — there is no climb operator.** `..` has no spelling, deliberately: the
warden priced it at +7 with three silences unclosed, the pragmatist showed
`module_of` sanitises it into a silent collision, and a path that can leave the
compiled file's directory is the escape §1.12 leans against. A module above the
root is a module of a different program.

**R5 — the leaf-test hazard is a stated rule, a diagnostic and a golden, not a
silence.** Compiling a file below a program's root re-bases its paths; that is
the rule's own consequence and the spec sentence in R2 states the rule. The
milestone owes: `unknown_module`'s message saying **where the path was read
from** (the compiled file's directory, named), and one golden case exercising
a nested leaf compiled directly. The historian's prediction on this is
registered below.

**R6 — the premise at `resolve_top.hero:147-148` dies with the landing.** "The
key's `/` cannot occur in a module name" is an expiring premise the moment a
module name is a path (CLAUDE.md §11): the landing changes the join or the
comment, and either way it gains a test that fires when the premise dies.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | the landing commits show **zero** changed lines in `selfhost/emit_*.hero` and `selfhost/cli_units.hero`, and ≤120 added non-test lines total | M-package-layout close |
| llm-ergonomist | harness task "write syntax/parse.hero importing root `util` and `lex/token`, no compiler run": under w-root-min ≥90% first-try compile and 0 silent decoy bindings among path uses; under FILE ≥25% `..` miscounts and ≥10% decoy bindings | harness run |
| spec-warden | `./heroes measure spec/heroes-spec.md` reads exactly **3630** at the landing commit if w-root-min lands verbatim; and `selfhost/`'s total `use`-line count at the migration commit equals the flat count at the commit before, ±0 (032 R1's zero-savings claim, re-scored) | landing commit · migration step |
| ffi-pragmatist | moving `examples/sqlite`'s extern group into `db/sqlite.hero` changes **zero** undefined symbols in `nm -u` versus the flat build, and §4.19's ladder needs no shim | M-package-layout |
| historian | if Heroes ever compiles a non-main file below a nested program's root, the first defect filed is Zig #13970's shape, within two milestones of that invocation existing | standing |

## Scoring 032, done in this sitting rather than owed

Panel 032's ergonomist predicted (checkable at "harness run"): *≥50% of `use`
lines from a subdirectory to a peer take the root-relative spelling; if the
compiler is file-relative, a 6-file 3-directory program first-try-compiles ~0%
of the time.* This sitting's blind phase 0 is that run, at n=1 seat: the one
subdirectory-to-peer line written was `use lex/token` from `syntax/parse.hero`
— root-relative, 1 of 1 — and the phase-0 program is illegal under FILE (two
`use` lines wrong), a 0% first-try. **Both halves confirmed on this sample**,
recorded in 032's own file with today's date; the sample is one seat and the
harness prediction above supersedes it at scale.

## What a veto would compel

No seat vetoed. The warden moves to veto only if a landing measurement exceeds
4010 (the pessimistic edge of §1.6's spread) or the removal is dropped without
a registered prediction. The pragmatist's FILE objection — moot under R1 —
lifts only on a single-funnel canonicaliser plus a root-escape refusal. The
engineer moves to object if the landing diff grows a separator-mapping branch
in the component or the cache key. The historian flips to Zig's hybrid only on
a documented manifest-less root-relative language whose users were forced to
add a manifest — it searched and found none.

## Author's verdict

**Ratified 2026-09-03** (author instruction, *"ratifica tutto"*, in a `/decide`
sitting that put ten items to the author at once; the instruction that followed
— *"scegli le soluzioni più robuste e complete rispetto a quelle più
economiche"* — changed nothing here, because ROOT was at once the cheaper form,
the smaller diff and the only one with precedent, and the alternative had no
seat). The item that asked for this stood open in `docs/work/DECIDE.md` from
2026-09-02 to 2026-09-03 and is in `docs/work/DONE.md` with the verdict.

**The leaf-test hazard now has a home.** A grep on the day of ratification found
R5's owed diagnostic and golden in no list — `SCHEDULED.md` 0, `DONE.md` 0 — so
it is scheduled in `docs/work/SCHEDULED.md` under the milestone that takes the
robustness repairs.

**What a yes would settle**: ROOT as the base, in w-root-min's exact wording at a
measured 3630, with the ", beside this file," removal in the same commit; no
climb operator, ever; and the component built by sanitised segment concatenation
so that spelling stays identity.

**What a yes would not settle**: the leaf-test hazard's eventual shape (R5 states
the rule and owes the diagnostic and the golden, but the historian's prediction
about the first defect is standing, not answered), and every prediction in the
table above, which is scored rather than ratified.

## Found in passing, owed to the coordinator

- **A dead citation, CLAUDE.md §11's class** (spec-warden): CLAUDE.md §10 and
  `DESIGN-LOG:435` cite `design.md:637` for the `heroes add` promise; the
  sentence lives at `design.md:772`. Corrected in this milestone, its own
  commit.
- **The witness program** (ergonomist): legal under both variants, prints 2 or
  3 depending on the base. Kept here as the standing proof that a base-less
  path rule is not a smaller spec but a wrong one — the shape 031's silence
  already charged this project one declined subdirectory for.
