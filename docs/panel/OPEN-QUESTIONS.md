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
- CLAUDE.md's "Nim ≈150k lines" is an unverified assumption (historian, 005).
- **Terminator ender-list gap (M1.1, needs a session before M2):** design.md
  §4.15 inserts the terminator only after an identifier, a literal, `return`,
  `)`, `]`, `}` — the list omits `break`, `continue` and `???`, so by the
  letter `break` alone on a line gets no terminator, and `x = ???` doesn't
  either. Related and also unspecified: how a continuation line (e.g.
  `b = (2 +` … `3)`) interacts with Indent/Dedent tokens. The lexer
  implements the letter and the parser will need the answer.
