# ROADMAP — the milestone chain

Why this file exists: autonomous work sessions need the goal chain **in the
repo**. It is the distillation of the approved bootstrap plan (revision 2,
reviewed by panel 000); the full rationale for the build order is design.md
Part 10, and the language-level acceptance criteria live in design.md Part 0.
Revision 2 had deleted the ROADMAP ("tags say where you are"); it returns
because tags say where you *are*, not what is *next* — and "what is next"
must not live outside version control. Update the status line here at every
milestone close (the checklist is in `/step`).

**Status: M1 closed 2026-08-04, tag `m1` (lexer complete: journal 001;
panels 007 and 008 resolved terminators and escapes; golden `check/` cases
now execute through the binary).
M0 closed 2026-08-03 (untagged — it predates the tagging habit).
Next: M2, step 1 (parser scaffold). Nothing blocks it; 007-bis and the
panel-009 governance ratification wait on the baseline, and the repeated-`@`
divergence needs its panel before M3c, not before M2.**

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
`heroes lex --json`. 29 crate-internal tests + 4 golden `check/` cases run
through the real binary. Lexer split into six single-concern files.
TextMate grammar (bonus): deferred, not dropped.
**Runnable:** `heroes lex examples/first.hero --json`.

### M2 — Parser, AST, pretty printer
Four entities, `=`/`@`, precedence table, `???` as an AST node,
reserved-word errors with pre-written fixes. Pretty printer immediately
(formatter + error renderer). `heroes parse --dump-ast`, `heroes fmt`.
**Runnable:** round-trip parse→print on the examples.

### M3 — split in four (each with its own runnable artifact)
- **M3a — Resolver:** scopes, no shadowing, unused (with the `???`
  exemption), order-free top level. Name-error diagnostics.
- **M3b — Checker core:** bidirectional ⇐/⇒ on `int`/`bool`/functions.
  (Queued for the author: the ⇐/⇒ paper exercise, before this lands.)
- **M3c — Data:** records, variants with payload, exhaustiveness, `_` ban,
  same-typed-argument rule, `ok`/`fail` checked against the expected type
  (panel 002/003 machinery).
- **M3d — Diagnostics as a product:** rich errors with `certain|guess`
  fixes, `x.fixed` goldens, `???` output (capped at 5, deterministic),
  `heroes check --json` (versioned schema) and `--permissive` (the control
  arm). First full harness run: metrics 1–3, both arms.

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
Library in Heroes: map/filter/fold/find/any/all/range/join.
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
heroes lex ex.hero --json                      # M1
heroes parse ex.hero --dump-ast                # M2
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
