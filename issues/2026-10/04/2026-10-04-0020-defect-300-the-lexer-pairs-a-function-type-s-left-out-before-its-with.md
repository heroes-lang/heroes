---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 097cd4331cd1b91ff3b22f6e22d6242f3bd68073
github: none
---

- [ ] **300 — the lexer pairs a function type's `)` left out before its `->` with the parameters' `(`, so the repair asks three edits where two would do** | `function(i64 -> i64)` written for `function(i64) -> i64`: since defect 197's repair (`7dbbd811`) the type reader reads it where it stands and tells the `->` to move the `)`, and the lexer still pairs that `)` with the parameters' `(`, so the reproducer reads three true messages asking three edits (lane b9-recovery's compiler, 2026-10-04); closing it means the lexer closing a function type's `(` at its `->`, which about seven readers of the lexer's bracket offsets in `selfhost/parse/` would then tell apart | `selfhost/lexer.hero` and `selfhost/closers.hero` (the bracket pairing) · the readers of the lexer's offsets in `selfhost/parse/` · defect 197, its type-reader half repaired · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`); defect 197's lexer half, reported rather than forced, the parser's budget holding 8 lines.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second cause beside 197's (panel 187's R1: one item per cause), a true message asking more edits than the mistake needs.

    Repaired at `097cd433`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
