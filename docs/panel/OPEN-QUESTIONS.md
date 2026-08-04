# Open panel sessions — awaiting the author's decision

**None.** All four sessions opened by panel 000 were decided by the author
on 2026-08-03 (all on the recommended resolution) and confirmed by full
five-judge sessions the same day:

- 002 — `ok(x)` constructor and `fail`'s typing → `002-ok-constructor.md`
- 003 — statement-position rule → `003-statement-position.md`
- 005 — Principle 0 formal adoption → `005-principle-0.md`
- 006 — map iteration order and `print`'s contract → `006-map-order-print.md`

Decisions are applied to design.md (the living document). **Spec v0 stays
frozen** until the pre-amendment baseline run; the v1 amendment texts are
recorded in each session file, and the spec-warden's package condition binds
them: v1 lands together with the cut of the spec's 53-word header comment,
or it breaches the 1500-token budget.

## Watch list (not open sessions — conditions and gaps on the record)

- `()` is used by the 003 rule but never defined in the spec's type table;
  owes a row if the baseline shows confusion (spec-warden, 003).
- The M6 closure-list audit is scoped by three riders: mechanical,
  spec-coverage with named cuts, tier assignment for file I/O / `args()` /
  `exit(code)` (panel 005).
- Ordering-sensitive `BTreeMap` walks in the Rust bootstrap get `// ORDER:`
  markers as they are written; each becomes an explicit sort in the port
  (compiler-engineer, 006).
- Canonical `f64` rendering: deterministic, locale-independent; exact
  algorithm fixed at M5b with its goldens (ffi-pragmatist, 006).
- ~~CLAUDE.md's "Nim ≈150k lines"~~ — resolved by removal (2026-08-04): the
  claim was dropped in CLAUDE.md's single-sourcing rewrite; the repo no
  longer asserts a line count for Nim anywhere (historian, 005, satisfied).
- ~~Terminator ender-list gap (M1.1)~~ — **resolved by panel 007** (author
  ratified 2026-08-03): ender list completed, continuation inside brackets
  only, unclosed opener = EOF error.
- **Depth-0 trailing-operator continuation + the spec-v1 layout sentence
  (007-bis, keyed to the baseline run):** deferred as one package. Score at
  the baseline: ergonomist predicts ≥25pp first-try loss on depth-0
  expression breaks with a silent spec; spec-warden predicts zero baseline
  completions contain such a break. If the warden is wrong, reconvene with
  Nim's explicit continuator set + the sentence (+ a named spec removal and
  a real tokenizer count).
- ~~Escape sequences do not exist (M1.3)~~ — **resolved by panel 008**
  (author ratified 2026-08-04): five escapes split by context, backslash
  reserved, set frozen.
- **Raw string literals (no panel yet, no urgency):** panel 008's
  implementation found that `"C:\temp"` cannot be made loud — `\t` is legal,
  so the path silently becomes `C:<TAB>emp`. Every C-style-escape language
  carries this; the standard remedy is raw strings (Go backquotes, Rust
  `r#"…"#`, Swift SE-0200), which are v2 material at best. Recorded so it is
  not rediscovered as a bug.
- ~~Repeated `@` arguments are a silent divergence~~ — **resolved by panel
  010** (adopted provisionally 2026-08-04, ratification queued): two `@`
  arguments sharing a root binding are a compile error. Original statement: `shift(a @ n, b @ n)` — the
  same variable passed twice as a mutable argument — compiles under both
  readings of §4.8 and yields *different answers* (copy-in/copy-out:
  last write wins; reference semantics: the writes see each other). Every
  mainstream language installs the reference model, so a model will guess
  wrong silently. Proposed fix is not spec prose but a compile error on
  repeated `@` arguments.
- **The harness needs a compiles-but-wrong-output bucket** separate from the
  compile-error bucket (ergonomist, 009): without it the thesis's central
  claim — that silent errors are what the design eliminates — has no
  instrument, and panels 008/009's predictions are untestable.
