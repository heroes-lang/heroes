# ROADMAP — the milestone chain

Why this file exists: autonomous work sessions need the goal chain **in the
repo**. It is the distillation of the approved bootstrap plan (revision 2,
reviewed by panel 000); the full rationale for the build order is design.md
Part 10, and the language-level acceptance criteria live in design.md Part 0.
Revision 2 had deleted the ROADMAP ("tags say where you are"); it returns
because tags say where you *are*, not what is *next* — and "what is next"
must not live outside version control. Update the status line here at every
milestone close (the checklist is in `/step`).

**Status: M5b closed 2026-08-05, tag `m5b` — `str` reaches C and values have
lifetimes. `own.rs` is the first IR→IR pass: `incref`/`decref` as real instructions
visible in `--dump-ir`, `Program::phase` so the verifier names which pass to blame, and
canonical `f64` rendering (panel 021).

Three measurable outcomes. **The leak counter caught a real defect on its first run** —
`panic: 3 heap blocks still live at exit`, all three from one missing row, because `+`
on a `str` is an `Op::Binary` that reads like arithmetic and is a constructor. That is
the entire reason it replaced AddressSanitizer as the leak gate: ASan on Darwin arm64
answers `detect_leaks is not supported on this platform`, and a 999-block leak exits 0
in silence (measured by two judges independently). **The phase-indexed verifier refuted
the ownership pass's design twice, on real programs, before any test did** — end-of-block
release cannot survive `name + " scored " + got.must().to_str()`, where `.must()` opens
a block in the middle of an expression, and the repair cashes panel 019's slots decision
properly: an owning temporary is *moved* into a synthetic slot, so every release is a
slot's. **Two judges' findings composed into a better `f64` than either had**: with
gnulib's subnormal branch, `5e-324` renders `5e-324` and the warden's own counterexample
to "shortest" dissolves.

368 tests (was 363): 331 crate, 13 golden harnesses over 68 cases, 24 surface — and
every `run/` case now runs in **three** configurations (`-O0`, `-O2`, `--sanitize`),
with the leak balance asserted in every generated `main`. The runtime went from 50 lines
to 285: `HeroStr {ptr, len}` **by value** (an FFI decision — by pointer the wrong
`str`→`cstr` conversion compiles clean *with a cast* and hands a refcount word to
`sqlite3_open`), a magic word that catches a fabricated `HeroStr`, `hero_str_from_bytes`
(without which no `extern` may return `str`), and a locale-proof renderer. The spec
gained two sentences at +41 measured (2155 → 2196): `slice`'s `to` is excluded, and an
`f64` always prints a point or an exponent.
Next: M6, sugar and the library — `.must()`, function values, generics by monomorphisation, and the four built-ins with no entry point. Every TYPE in the language now emits (M5c aggregates, M5d `T?` and the map)
(`copy`/`drop`/`eq`/`hash` per reachable type, spike 04's frozen ABI), structural `==`,
and COW on the mutation primitives. Nothing blocks it. Carried in: `cow_check` was
struck from M5b for having zero call sites and `push` is what gives it one; **cycles are impossible** and the
reason is now cited rather than assumed — references cannot be stored in fields, so values
form disjoint trees (Abrahams et al., ICOOOLPS '21); this line previously said reference
counting leaks cycles, contradicting design.md §4.10 gift 2; `Op::Cast` is unreachable from source
until M7; and `len`'s unit is still derivable rather than stated, with `print(len("è"))`
as the discriminating probe.**

**M5a closed 2026-08-04, tag `m5a` — the compiler compiles.
`heroes run examples/gallery/00-first.hero` prints `20`. `emit/` is eight files and
1166 non-test lines: a printer over the M4 IR plus declaration ordering and the
mangler, with no analysis and no optimisation (the pipeline is clang's).

Three measurable outcomes. **Zero clang warnings across the whole corpus at both `-O0`
and `-O2`**, with warnings forwarded on success so they could fire — the number that
promoted `-Wconditional-uninitialized` to `-Werror=` in the same step that measured it,
resolving panel 020's disposition by a run rather than a discussion. **The double-emit
determinism diff lands and is green**, including the stronger form: identical bytes on
stdout and through `-o`, because the emitted C never mentions its own output path (GCC's
stage2-vs-stage3 comparison is the named ancestor, and the M8c fixpoint is the same test
thirty years later). **Panel 020 arrived with two vetoes and both were lifted by a
change, not an argument**: one table row (`extern` waits for M7 — §4.19's guarantee is
the `#include`, and measured, an emitter-invented prototype is verified by *nothing*:
`abs(-2147483649)` returned 2147483647 at exit 0 and `sqlite3_open` was accepted in total
silence under `-Weverything -pedantic`) and one exit code (an unsupported form is **1**;
given 2, a judge checked the version, ran doctor, grepped for the milestone identifier,
and told its user the toolchain was broken).

363 tests (was 331): 326 crate (+27 emit), 13 golden harnesses over 55 cases
(`run/` is born with 10 — 5 bulk, 5 adversarial marked UNVERIFIED — each compiled and
executed at **both** `-O0` and `-O2`; `emit/` pins the C byte for byte; `unsupported/`
pins what the backend refuses and inherits the `#~ <code>` annotation invariant, which
is *why* the refusal is a `Diagnostic` kind and not a parallel type), 24 CLI surface.
Two new diagnostic classes: `unsupported` (kind, 12 capability codes) and
`missing_return`, the latter having caught design.md's own appendix and earned §4.16's
file-wide hole exemption. The verifier gained dominance, and `verify.rs` split at the
seam. `--no-line` was **not** born (panel 016's stopping rule, applied at the milestone
panel 016 named). The spec gained panel 006's four-milestone-old print sentence:
2139 → **2155** binding, headroom 845.
Next: M5b, strings and the ownership pass — `str` in the runtime, visible
`incref`/`decref`/`cow_check` in `--dump-ir`, cleanup-label chains on every exit edge,
canonical `f64` rendering with its goldens, ASan/UBSan gating in `run/`. Nothing blocks
it. Carried in: the `unsupported` gate's `str` and `f64` rows are the ones M5b deletes;
a binding statement's span runs to the end of its line, so a trailing `#~` annotation
lands under the caret; `while true` in a value-returning function needs an unreachable
`return`; and the spec is silent on `INT64_MIN % -1`, which Heroes aborts.**

**M4 closed 2026-08-04, tag `m4` — the middle end exists. Part 5's sugar
table is erased on the way into a three-address IR with explicit basic blocks,
**slots and no phi nodes** (panel 019, unanimous, on LLVM's own advice to frontend
authors), and `heroes build [--dump-ir]` is the fifth verb.

Three measurable outcomes. **design.md's 320-line acceptance program lowers and
verifies** — 35 functions, 226 basic blocks, its one hole reported — and for the
first milestone in five it produced *no new defect in the program*: the frontend's
recorded answers were complete enough for a pass that reads them instead of
re-walking the tree. **The dump is an artifact, not debug output**: 4.77 IR lines
per source line against the panel's ceiling of 12, deterministic and byte-identical
twice (the cheap analogue of §7's double-emit diff, a milestone before there is any
C), explicitly **not** version-stable — LLVM's own stance on `.ll`. **Panel 019
changed the design in five places**, each because a judge compiled or read something
rather than argued: linkage on every callee (an unmangled Heroes `function open`
silently replaces libc's — compiled, prints 7, no diagnostic), at most one
destination on a call (`dst = call print(x)` is a hard clang error), a place as a
store's target, construction as an instruction, and an explicit cast at the FFI
boundary. Two vetoes were lifted by amending design.md §4.12 and Part 5 rather than
by argument (CLAUDE.md §12: spec beats compiler).

304 tests (was 259): 277 crate, 3 golden harnesses over 42 cases (`tests/golden/ir/`
is born with 14 — one per live sugar row plus 5 adversarial, marked UNVERIFIED —
and inherits `check/`'s `UPDATE_GOLDEN` ban), 21 CLI surface tests, 6 harness-level.
`ir/` is sixteen files, none over 300 lines, and `verify.rs` is GHC's Core Lint
without GHC's tree: it asserts §4.8's "copy-out happens always" on every exit edge,
including the error side of `?`.
Next: M5a, scalars run — `int`/`bool`/`if`/`while`/functions/`print` → C → binary,
the mangler, `#line` on change, the `-Werror` set, and the double-emit determinism
test that stays green forever. Nothing blocks it. Carried in: `???` reaching `build`
and the out-of-range `int` literal are both decided-by-default and queued as their
own diagnostic classes; a function that runs off its end waits for
`-Werror=return-type`; `-Wconditional-uninitialized` and `-Werror=format` want
adding to CLAUDE.md §7.**

**M3 closed 2026-08-04, tag `m3` — the frontend is complete. M3a the
resolver, M3b the bidirectional checker, M3c the data rules, M3d errors as a
product. Panels 015 (the resolver's rejection set), 016 (the command surface) and
017 (the type system's four edges) all decided and ratified; nine judges ran, and
three of them changed the design on evidence rather than argument.

Three measurable outcomes. **design.md's 317-line acceptance program type-checks
with zero diagnostics** — getting there found a twelfth `T`-where-`T?` site that
panels 002/006 had missed, the fourth defect that program has surfaced in four
milestones without ever being run. **Metric 3 ran for the first time**: 379
mutants, 94% caught against 76% in the control arm, recorded with provenance in
`docs/measurements/001-metric-3.md` — the project's first number about its own
thesis, with the columns that show *no* effect named as plainly as the ones that
do. **spec v1 landed and the freeze is over**: seven panels' amendments, 160
measured tokens of removals spent, 2139 binding max (from 2048 — the number read
2136 until panel 018's sweep landed, which cost +3 measured against a table that
had sold its shape as −4 on v0: deltas measured on v0 did not transfer), and the spec is
now *tested* against the compiler — because it had been briefing models into a
reserved-word error for four milestones.

259 tests (was 110 at M2): 240 crate, 3 golden harnesses over 28 cases (5
adversarial from M3a, 5 from M3b–d, 4 bulk, 12 inherited, 4 with `.fixed`
expectations), 16 CLI surface tests where there were none. `heroes` itself was
rebuilt on panel 016's verdict: one strict table-driven argv parser, exit codes
0/1/2 printed in `--help`, `--dump-<stage>` per design.md §3.5, `--in-place`
where `--write` used to be, and a stopping rule in CLAUDE.md §10 so the surface
does not sprawl.
Next: M4, desugar and lowering — Part 5's sugar table erased on the way into the IR, a
three-address IR with explicit basic blocks, `heroes build --dump-ir`. Nothing
blocks it. Carried in: `()` inside a container is still accepted, a `match` over
`bool` cannot be written exhaustively, and the same-typed-argument rule's cost on
the FFI boundary wants a panel.**

**M3a closed 2026-08-04, tag `m3a` — the resolver: order-free top
level, scopes with no shadowing, unused bindings with §4.16's file-wide hole
exemption, written types against primitives/declarations/generics, and
`heroes check [--dump-scopes]`, which the golden harness now runs through (the
frontend command's name stops moving here). Panel 015 settled the rejection set
and changed three of its five defaults on the judges' evidence: one tier of
built-in names, not two; the UFCS unknown narrowed to "not a field anywhere in
the file"; `_` as the unused rule's escape valve. design.md's appendix renamed
its `map`/`fold` to `apply`/`reduce` — the third rule that program has caught —
and **resolves with zero diagnostics**: 33 top-level names, 60 bindings, 293
resolved uses. 164 crate tests + 21 golden cases (12 inherited unchanged, 5
adversarial, 4 bulk).
Next: M3b, the bidirectional checker (⇐/⇒ on `int`/`bool`/functions). Carried
in: classify `break`/`continue`/`return` (NOT as `()` — RFC 1216), `+` on `str`
(unwritable per the spec's operator table, provided by §4.20's runtime — wants a
panel), the value-`match` rule, declarations out of inline arm bodies, and the
unrejected `()` in binding position.**

**M2 closed 2026-08-04, tag `m2` — parser, tree, `--dump-ast` and the
canonical formatter; panel 013 settled the function type's marker
(`(function(A, B) -> C)`, `fn` stays an error everywhere). The 317-line
acceptance program in design.md's appendix parses clean, formats idempotently
and its tree survives formatting. Golden `check/` now runs through `parse`;
104 crate tests + 11 golden cases (4 inherited from M1, 5 adversarial, 2 bulk).
M1 closed 2026-08-04 tag `m1`; M0 closed 2026-08-03, retro-tagged `m0`.
Debrief of the same day (after the tag): panels 013 and 014 ratified/resolved,
M2's five adversarial cases ratified, the layout cascade improved (5
diagnostics → 2), and two defects panel 014's costing exposed are fixed —
`heroes fmt` was deleting an arm's `if` branches and `heroes parse` could hang.
110 tests, 12 golden cases.
Next: M3a, step 1 (resolver — scopes, no shadowing, unused with the `???`
exemption, order-free top level). Nothing blocks it. Carried into M3: classify
`break`/`continue`/`return` (NOT as `()` — RFC 1216), the value-`match` rule,
declarations out of inline arm bodies, and the unrejected `()` in binding
position.**

## The chain

### M0 — Day zero ✅ (2026-08-03)
Git, Cargo workspace (library + `heroes` CLI), Cyclone tooling
(`clippy.toml`, `forbid(unsafe_code)`), golden harness green with zero cases,
`heroes doctor`, minimal runtime (`heroes_runtime.h` + `runtime.c`),
spec v0 (~1496 tokens, FROZEN as the pre-amendment baseline), harness
skeleton, four hand-written C spikes in `tools/spike/` (01 target shape ·
02 loop as goto+labels · 03 FFI via libm · 04 recursive variant + refcount,
ASan-clean — decided the container/descriptor ABI). Panels 000–006 all
decided; decisions applied to design.md.

### M1 — Lexer ✅ (2026-08-04, tag `m1`)
Rigid indentation (INDENT/DEDENT, tab = error), terminator insertion (as
amended by panel 007: full ender list, brackets-only continuation,
unclosed-opener diagnostic), str/char literals with the five escapes of
panel 008 (split by context, backslash reserved), comments retained,
reserved-word detection with prescribed errors and `Certain` fixes.
`heroes lex --dump-tokens [--json]` (spelled `--json` alone until panel 016). 30 crate-internal tests + 4 golden `check/` cases run
through the real binary. Lexer split into six single-concern files.
TextMate grammar (bonus): `editors/vscode/` (installable extension) — foreign
reserved words and unknown escapes scoped `invalid.illegal`, so the thesis
shows up while you type.
**Runnable:** `heroes lex examples/gallery/00-first.hero --dump-tokens`.

### M2 — Parser, AST, pretty printer ✅ (2026-08-04, tag `m2`)
Four entities plus `extern` and `test`, `=`/`@`, the §4.14 precedence table,
`???` as an AST node, the type grammar (function type per panel 013), `if` and
`match` as expressions, patterns. Two renderings of one tree: `--dump-ast`
prints every parenthesis, `heroes fmt` prints the fewest — comparing the two
is what proves the formatter preserved meaning. Formatter policies in
DESIGN-LOG (minimal parens, 88 columns inside brackets only, blank lines as
content). Recovery landmarks: brackets and lines, the two things the language
cannot lie about.
**Runnable:** `heroes parse examples/gallery/00-first.hero --dump-ast` ·
`heroes fmt examples/gallery/00-first.hero` (already canonical) · the appendix acceptance
program parses clean and formats idempotently (pinned by tests that read
design.md, so the appendix stays its single source).

### M3 — split in four (each with its own runnable artifact)
- **M3a — Resolver ✅** (2026-08-04, tag `m3a`)**:** scopes, no shadowing,
  unused (with the `???` exemption), order-free top level, written types, and
  the name-error diagnostic class settled by panel 015.
  **Runnable:** `heroes check examples/gallery/00-first.hero` (silent — it resolves) ·
  `heroes check tests/golden/check/shadowing.hero` (the diagnostic class) ·
  `--dump-scopes` prints the alphabetical top-level table and every binding
  nested by scope, for any file that has bindings. The witness is the appendix,
  read out of design.md by a test so it cannot drift: zero diagnostics.
- **M3b — Checker core ✅** (2026-08-04)**:** bidirectional ⇐/⇒, one shared join
  rule for every branching construct, and a jump with no type (panel 017 A).
  **Runnable:** `heroes check <file>` — the appendix type-checks clean.
- **M3c — Data ✅** (2026-08-04)**:** records, variants with payload,
  exhaustiveness, the `_` ban, the same-typed-argument rule, `ok`/`fail` in ⇐
  mode, and a declaration rejected as an inline arm body (panel 017 D).
- **M3d — Diagnostics as a product ✅** (2026-08-04)**:** §4.17's rich form (the
  line, the caret, the note carrying the other end, the tagged fixes), `.fixed`
  goldens that assert an applied `certain` fix checks clean, §4.16's `???` output
  capped at 5 and deterministic, `heroes check --json` (schema 1), `--brief`,
  `--apply`, and `--permissive` — the control arm metric 3 needs.
  **Metrics 1 and 3 have run**; metric 2 still needs a model or paced sampling
  with author-written tasks (`harness/README.md`), so the thesis keeps its
  measured mechanism and not yet its measured claim.

### M4 — Desugar + lowering ✅ (2026-08-04, tag `m4`)
Part 5's sugar table erased **on the way into the IR** — no desugared tree, and
`--dump-ir` is the evidence (one golden per row); three-address IR with explicit
basic blocks, **slots and no phi nodes** (panel 019). Named-arg check ordered
before monomorphisation, which is itself an IR→IR pass at M6. `heroes build
[--dump-ir]`, and an IR verifier that runs in tests — GHC's Core Lint without
GHC's tree.
**Runnable:** `heroes build examples/gallery/00-first.hero --dump-ir` · `heroes build
examples/gallery/05-mutation.hero --dump-ir` (the `@` copy-out chain on every exit
edge) · `heroes build <file>` alone, which says what it lowered and what does not
exist yet. The witness is the appendix, read out of design.md by a test: 320 lines,
35 functions, 226 basic blocks, lowered and verified. (Queued for the author:
hand-desugar three constructs, against the IR text.)

### M5 — split around the two passes
- **M5a — Scalars run ✅** (2026-08-04, tag `m5a`)**:** `int`/`bool`/`if`/`while`/
  functions/`print` → C → binary. Prototypes before definitions, the mangler (module
  component sanitised so `h_<module>_<name>` is injective), `#line` against the current
  *effective* line, the `-Werror` set with two flags added, `hero_unreachable`, and the
  `@` parameter's pointer ABI. **The double-emit determinism test lands here and stays
  green forever.** Panel 020 settled the backend, the artifact surface and the
  unsupported-form class; `--no-line` was refused rather than implemented.
  **Runnable:** `heroes run examples/gallery/00-first.hero` (prints 20) · `heroes build
  <file> --emit-c` (the C on stdout, byte-identical twice and through `-o`) · `heroes
  build examples/gallery/07-strings.hero` (the refusal: exit 1, the capability named,
  and nothing that resolves in another file).
- **M5b — Strings + ownership pass ✅** (2026-08-05, tag `m5b`)**:** `str` in the
  runtime; the first IR→IR pass, emitting visible `incref`/`decref` in `--dump-ir`
  (`cow_check` struck — zero call sites until M5c); the release sweep on every exit
  edge; `--sanitize` as a third `run/` configuration, and the **leak counter** that
  replaced ASan's, which does not exist on this platform. Canonical `f64` rendering
  fixed here with its goldens, round-trip-exact and locale-proof (panels 006, 021).
  **Runnable:** `heroes run tests/golden/run/strings.hero` · `heroes run
  tests/golden/run/f64-rendering.hero` · `heroes build tests/golden/emit/strings.hero
  --emit-c` (the refcounting, readable as text) · `heroes run --sanitize <file>`.
- **M5c — Aggregates ✅** (2026-08-10, tag `m5c`, panels 022 and 023)**:** records
  and variants **by value** (confirming spike 04), `[T]` through the descriptor pass at
  `HERO_RUNTIME_ABI 3`, structural `==` as a direct `h_T_eq`, and **COW as one unshare
  per array step of the place path** with write-back — the primitives take
  `HeroArrayHeader **`, because a caller cannot forget to store a result that does not
  exist. `Op::CowCheck` did **not** enter the IR: inside the primitive, C's
  argument-evaluation rule makes the hoisted order inexpressible, and the hoistable form
  is what a judge built a three-block cycle from.
  **The acceptance is one program:** `examples/gallery/11-trees.hero`, design.md §4.10's
  own recursive variant — `Expr` holding `[Expr]` — compiles, runs, prints 14, leak
  counter zero. A tree containing itself in a language with no pointer. The gallery goes
  from 3 building to 5, and nothing in it exits 2.
  **The veto ran**: `h = g` then `g.rows[0].cells[0] @ 7` prints 7 and 0, where the
  single-unshare version printed 7 and 7 with ASan clean, the leak counter at zero and
  exit 0 — a green harness on a program violating spec line 60. All three owed `run/`
  cases exist (`adversarial-cow-per-step.hero`).
  New in the front end: the **`no_size`** class (panel 023) with the topological type
  order it shares, and `Checked::counted` answering "does this type own a reference"
  where the AST is.
  **Runnable:** `heroes run examples/gallery/11-trees.hero` · `heroes run
  tests/golden/run/adversarial-cow-per-step.hero` · `heroes build
  tests/golden/emit/aggregates.hero --emit-c` (the descriptors and the tag switch, as
  text) · `heroes check tests/golden/check/no-size-mutual.hero`.
- **M5d — `T?` and the map ✅** (2026-08-10, tag `m5d`, author instruction over the
  M5c deferral)**:** `T?` as a tagged union **by value** with `HeroFailure` shipped in
  the runtime — so **every `T?` is reference-counted whatever `T` is**, `int?` included,
  because its error side is two `str`s. One generated option struct per distinct `T?`,
  named by index because `int?` and `[int]?` sanitise to the same identifier. The map is
  open addressing over three parallel regions with a **fixed seed** (panel 006:
  iteration order must be a function of the contents, or the fixpoint never closes) and
  **order-independent `==`** (spec line 58). `m[k]` yields a `V?`; the runtime returns an
  address or NULL because it cannot build an option struct generated per payload type.
  `HERO_RUNTIME_ABI` 5. **The gate now refuses no type at all.**
  **The disagreement, unresolved and recorded:** M5c deferred both on Principle 0
  because panel 022's R4 leaves `{K: V}` open — deleting it recovers **−57** spec tokens
  against **+29** to fund `set` plus `for k in m`. The author overruled that. If M6's
  audit deletes the container, this map goes with it; `T?` is not at risk under any
  reading.
  **Runnable:** `heroes run tests/golden/run/maps.hero` · `heroes run
  tests/golden/run/fallible.hero` (a refusal golden that became a running one, like
  `strings.hero` at M5b).
  **What is left in the language is not a type:** `sort`, `join`, `chars` and `range`
  have no runtime entry point, and `.must()` is an `Op::Abort` rather than a shape. All
  five are M6's, and they are what the last four gallery programs wait on.

### M6 — Sugar, tests, generics, library
`T?` operators, function values (C function pointers), generics by
monomorphisation, `test`/`assert` with source text, `outline`, `explain`.
Library in Heroes: map/filter/fold/find/any/all/range — `join` is Tier 1, in
the C runtime with its `Builder` (§1.11, §4.20; this line used to say
otherwise, and the ffi-pragmatist found the contradiction in panel 015).
**Acceptance: the appendix calculator (restored to `examples/`) compiles,
runs, its tests pass.** Harness: first *tests-pass* rates, both arms.
Principle 0 checkpoint: the closure-list audit, under panel 005's three
riders (mechanical; spec-coverage with named cuts; runtime tier for
file I/O / `args()` / `exit`).

### LSP — off the critical path
`heroes lsp`, ~250 lines JSON-RPC: diagnostics on save, formatting, hover,
documentSymbol. Any time after M3d; blocks nothing.

### M7 — FFI ladder
`extern` + header name → clang verifies against the real header.
printf → libm (spike 03 already proved it) → **SQLite** (the
architecture-holds test) → raylib. `heroes cc` for C++ shims.

### M8 — split: modules, port, fixpoint
- **M8a — Modules:** mangler namespace live (`h_<module>_…`), one `.c` per
  module, prototypes across TUs, per-module cache. File I/O + `args()` +
  `exit` (now trivial: `<stdio.h>` + declarations).
- **M8b — The port:** Rust → Heroes into `selfhost/` (directory born here),
  file by file, goldens as the net, `PORT-DEBT` count as the map.
- **M8c — Fixpoint:** A builds `B.c`, B builds `C.c`, `diff B.c C.c` empty
  (generated C, not binaries; clang version pinned and recorded). Then
  `crates/heroes` → `archive/bootstrap-rs/`: the third language dies here.

**Post-fixpoint, scheduled:** the QBE backend from the same IR (~500 lines)
— proves IR agnosticism, restores the register-allocation lesson.

## End-to-end verification (per milestone)

```sh
heroes doctor                                  # M0
cargo build && cargo test                      # M1+
heroes lex ex.hero --dump-tokens [--json]      # M1
heroes parse ex.hero --dump-ast                # M2
heroes fmt ex.hero [--in-place]                # M2 (the flag was --write until panel 016)
heroes check ex.hero --json [--permissive]     # M3a–d
heroes build ex.hero                           # M4: lowers, verifies, says so
heroes build ex.hero --dump-ir                 # M4 (M5b: increfs visible)
heroes build ex.hero --emit-c                  # M5a — and the determinism diff:
heroes build ex.hero --emit-c -o a.c && heroes build ex.hero --emit-c -o b.c && diff a.c b.c
heroes run examples/gallery/00-first.hero                 # M5a: first native binary (-O2)
heroes test examples/calculator.hero           # M6: acceptance
# M8c — the fixpoint, on generated C:
cargo run -- build selfhost/heroes.hero -o A
./A build selfhost/heroes.hero --emit-c -o B.c && clang … B.c -o B
./B build selfhost/heroes.hero --emit-c -o C.c && diff B.c C.c
```
