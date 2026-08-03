# Panel 007 — terminator ender-list and continuation lines

Date: 2026-08-03. Trigger: M1 step 1 implemented design.md §4.15 by the
letter and found the gap (docs/panel/OPEN-QUESTIONS.md watch item, now
resolved). Five judges, differentiated inputs; the compiler-engineer judged
against the real lexer (crates/heroes/src/lexer/mod.rs, 480 lines at review
time), the llm-ergonomist ran a blind A/B writing four programs from the
spec alone, the spec-warden verified the header count and the v1 package
arithmetic, the ffi-pragmatist compiled the real C surface (libm, SQLite,
cblas_dgemm), the historian verified every precedent at the source.

## Proposal (as put to the judges)

design.md §4.15 reads: "The lexer inserts a terminator when a line ends
with an identifier, a literal, `return`, `)`, `]`, or `}`. (Go's rule.)"

- **Amendment A** — complete the ender list to: identifier · literal (int,
  float, str, char, `true`, `false`) · `return` · `break` · `continue` ·
  `???` · postfix `?` · `)` · `]` · `}`.
- **Amendment B** — continuation: inside `(` `[` `{`, leading whitespace is
  not structural (no Indent/Dedent). At bracket depth 0, a line that
  emitted no terminator continues onto the next line. Terminators inserted
  unchanged everywhere.
- Open spec question: whether spec v1 carries one ~28-word sentence about
  breaking long lines (spec v0 frozen regardless).

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| compiler-engineer | approve-with-changes | A: +4 match arms, zero snapshot churn; B-bracket: ~30 src lines + ~70 test lines; **B depth-0: rejected as written** | lexer ≤545 lines at M2 close, 12 snapshots byte-identical, parser has zero depth-conditional layout code; if depth-0 ships, the first `record` golden fails | depth-0 clause struck or rewritten as an explicit continuator set; unclosed-opener diagnostic ships with the bracket clause |
| llm-ergonomist | approve (on the v1 sentence) | ~30 spec tokens retire 3 of 4 layout guesses; 1-in-3 plausible encodings fails under Variant 1 | on tasks forcing an expression break outside brackets, the sentence raises first-try compile ≥25pp (falsified if <5pp) | mechanism must match the sentence; bare expression statements must be illegal (holds — panel 003) |
| spec-warden | approve-with-changes (provisional, counts estimated) | A+B: 0 spec tokens; v1 package ~1494 est. without the sentence, **~1531 with it — breach** | zero baseline completions will contain a depth-0 illegal line break (falsified if ≥1) | sentence stays OUT of v1; may enter later only with baseline evidence + a named removal + a real tokenizer count |
| ffi-pragmatist | approve | none — lexer only; compiled libm/SQLite/cblas_dgemm surface unaffected | every §4.19 ladder signature lexes with zero continuation escapes; multi-line externs emit byte-identical C | terminators harmless at depth ≥1; emitted C never depends on source line breaks |
| historian (advisory) | approve-with-changes | n/a | the rule's first bite is a line broken after postfix `?` (falsified if the first termination bug is elsewhere) | statement-position restriction (panel 003) must hold so stranded continuations are loud |

## Disagreements, unsmoothed

1. **Ergonomist vs engineer on depth-0 continuation.** The ergonomist's
   sanctioned encoding (trailing `||`, unparenthesized) requires depth-0
   continuation; the engineer showed the proposed clause is unsound — four
   legal block headers end in non-enders (`Point = record`,
   `Token = variant`, bare `else`, `=>` block arms) and would have their
   body's Indent silently suppressed: every record, variant and else-block
   in the language breaks. The safe form (explicit continuator-token set,
   Nim's precedent per the historian) is more surface, is not needed for
   self-hosting (parenthesize), and has no measured demand yet.
2. **Ergonomist vs warden on the spec sentence.** Opposite falsifiable
   predictions about the same future baseline run (≥25pp gain vs zero
   illegal breaks). Both are on record below; the baseline scores them.

## Resolution — RATIFIED by the author, 2026-08-03

The panel's conservative synthesis, adopted as put:

1. **Ender list completed** (Amendment A): identifier · literals (int,
   float; str/char when they land) · `true`/`false` · `return` · `break` ·
   `continue` · `???` · postfix `?` · `)` · `]` · `}`. Go's rule
   faithfully; `fallthrough` omitted deliberately (no such keyword). The
   one departure from Go on record: `++`/`--` are statements in Go, `?` is
   an expression in Heroes — safe because panel 003's statement-position
   rule makes any stranded continuation a loud error (historian's and
   ergonomist's shared condition, verified).
2. **Continuation inside brackets only** (Amendment B, bracket clause):
   within `( [ {`, leading whitespace is not structural — no Indent/Dedent
   tokens; terminator insertion unchanged everywhere (§4.9's multi-line
   literals rely on terminators as separators). At depth 0 every line's
   indentation is structural: long expressions are broken inside
   parentheses or not at all (Python's discipline). An unclosed opener is
   a compile error at EOF citing the opener (engineer's condition — without
   it one missing `)` silently swallows the rest of the file's layout).
3. **Depth-0 trailing-operator continuation (Nim's rule): deferred.**
   Enters only if the baseline shows models actually produce that break
   shape. Reconvene as 007-bis with the explicit continuator set and the
   spec sentence as one package.
4. **Spec untouched**: v0 frozen (standing), and the ~28-word sentence
   stays out of the v1 package (warden's budget arithmetic: it would breach
   1500 with the header cut already spent on 002/003/006).

## Predictions to score (at the baseline run, then M2/M7)

- ergonomist: tasks forcing a depth-0 expression break lose ≥25pp first-try
  under a spec that says nothing — **scored against the baseline**.
- spec-warden: zero baseline completions contain a depth-0 illegal break —
  **scored against the same baseline** (these two cannot both hold).
- compiler-engineer: lexer ≤545 lines at M2 close, zero snapshot churn,
  zero layout code in the parser — **scored at M2 close**.
- ffi-pragmatist: §4.19 ladder signatures lex clean; multi-line externs
  emit byte-identical C — **scored at M7**.
- historian: first termination-rule bug is a break after postfix `?` —
  **scored at first bug report**.
