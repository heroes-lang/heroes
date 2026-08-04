# ROADMAP — the milestone chain

Why this file exists: autonomous work sessions need the goal chain **in the
repo**. It is the distillation of the approved bootstrap plan (revision 2,
reviewed by panel 000); the full rationale for the build order is design.md
Part 10, and the language-level acceptance criteria live in design.md Part 0.
Revision 2 had deleted the ROADMAP ("tags say where you are"); it returns
because tags say where you *are*, not what is *next* — and "what is next"
must not live outside version control. Update the status line here at every
milestone close (the checklist is in `/step`).

**Status: M3 closed 2026-08-04, tag `m3` — the frontend is complete. M3a the
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
measured tokens of removals spent, 2136 binding max (from 2048), and the spec is
now *tested* against the compiler — because it had been briefing models into a
reserved-word error for four milestones.

259 tests (was 110 at M2): 240 crate, 3 golden harnesses over 28 cases (5
adversarial from M3a, 5 from M3b–d, 4 bulk, 12 inherited, 4 with `.fixed`
expectations), 16 CLI surface tests where there were none. `heroes` itself was
rebuilt on panel 016's verdict: one strict table-driven argv parser, exit codes
0/1/2 printed in `--help`, `--dump-<stage>` per design.md §3.5, `--in-place`
where `--write` used to be, and a stopping rule in CLAUDE.md §10 so the surface
does not sprawl.
Next: M4, desugar and lowering — Part 5's sugar table erased in the frontend, a
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
**Runnable:** `heroes lex examples/first.hero --dump-tokens`.

### M2 — Parser, AST, pretty printer ✅ (2026-08-04, tag `m2`)
Four entities plus `extern` and `test`, `=`/`@`, the §4.14 precedence table,
`???` as an AST node, the type grammar (function type per panel 013), `if` and
`match` as expressions, patterns. Two renderings of one tree: `--dump-ast`
prints every parenthesis, `heroes fmt` prints the fewest — comparing the two
is what proves the formatter preserved meaning. Formatter policies in
DESIGN-LOG (minimal parens, 88 columns inside brackets only, blank lines as
content). Recovery landmarks: brackets and lines, the two things the language
cannot lie about.
**Runnable:** `heroes parse examples/first.hero --dump-ast` ·
`heroes fmt examples/first.hero` (already canonical) · the appendix acceptance
program parses clean and formats idempotently (pinned by tests that read
design.md, so the appendix stays its single source).

### M3 — split in four (each with its own runnable artifact)
- **M3a — Resolver ✅** (2026-08-04, tag `m3a`)**:** scopes, no shadowing,
  unused (with the `???` exemption), order-free top level, written types, and
  the name-error diagnostic class settled by panel 015.
  **Runnable:** `heroes check examples/first.hero` (silent — it resolves) ·
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

### M4 — Desugar + lowering
Part 5's sugar table erased in the frontend; three-address IR with explicit
basic blocks. Named-arg check ordered before monomorphisation.
`heroes build --dump-ir`. (Queued for the author: hand-desugar three
constructs, before this lands.)

### M5 — split around the two passes
- **M5a — Scalars run:** `int`/`bool`/`if`/`for cond`/functions/`print` →
  C → binary. Decl-ordering pass + mangler + `#line`-on-change + `-Werror`
  set + `hero_unreachable`. **Double-emit determinism test lands here and
  stays green forever.** Compare with spike 01. Celebrate.
- **M5b — Strings + ownership pass:** `str` in the runtime; ownership pass
  emitting visible `incref`/`decref`/`cow_check` in `--dump-ir`;
  cleanup-label chains on every exit edge; ASan/UBSan gating in `run/`
  goldens. Canonical `f64` rendering fixed here with its goldens (panel 006).
- **M5c — Aggregates:** records/variants/arrays/maps via the descriptor pass
  (`copy/drop/eq/hash` per reachable type — spike 04's ABI); structural
  `==`; COW on mutation primitives.

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
heroes build ex.hero --dump-ir                 # M4 (M5b: increfs visible)
heroes build ex.hero --emit-c                  # M5a — and the determinism diff:
heroes build ex.hero --emit-c -o a.c && heroes build ex.hero --emit-c -o b.c && diff a.c b.c
heroes run examples/first.hero                 # M5a: first native binary (-O2)
heroes test examples/calculator.hero           # M6: acceptance
# M8c — the fixpoint, on generated C:
cargo run -- build selfhost/heroes.hero -o A
./A build selfhost/heroes.hero --emit-c -o B.c && clang … B.c -o B
./B build selfhost/heroes.hero --emit-c -o C.c && diff B.c C.c
```
