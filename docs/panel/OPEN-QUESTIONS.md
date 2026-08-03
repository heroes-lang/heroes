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
- **Escape sequences do not exist (M1.3, needs a session before M6):**
  design.md never mentions them; the lexer implements the letter — a
  backslash in a string is an ordinary byte, and a char literal is one
  ASCII character. But the self-hosted lexer must express the tab and
  newline *characters* without magic numbers (`c == 9` is exactly what
  §4.3's char literals were introduced to kill), and `join`/`Builder` (M6
  library) will want them in strings. Design question for the panel: a
  minimal escape set (`\n \t \\ \" \'`?) vs named constants in the library
  vs something else. Compiler-need under §1.0, so it cannot be waved off.
