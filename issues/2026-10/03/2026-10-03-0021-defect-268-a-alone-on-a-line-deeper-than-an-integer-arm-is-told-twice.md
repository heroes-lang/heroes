---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 1684bab5c853c64b2c162cc166700cec7c6aa69a
github: none
---

- [ ] **268 — a `-` alone on a line deeper than an integer arm is told twice, the second an `unexpected_block` true only if the `-` joins the next line** | `k = match n` over `2 => "two"`, a line `-` one level deeper, then `1 => "one"` and `_ => "many"`: `check` exit 1, `continuation_outside_brackets` at 5:13, then `unexpected_block` at 5:1 (batch 8's round compiler at `1eb854c3`, 2026-10-04, `<scratchpad>/batch8/recovery/shapes/s182_minus_over_int.hero`) | `selfhost/parse/` (the stray operator's margin, beside defect 182's repair) · **class: adjacent**

    **Origin:** batch 8's recovery lane, 2026-10-03 (its report's *Found beside*); reproduced by the coordinator, 2026-10-04.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, the stray `-`.

    **Cause found 2026-10-04, lane b11-parse**: where both readings stand, `sign_above.offer` hands the parser the join with the `-` line's margin, an indent the arm below is joined into, so `grammar_expr.arms_of` names an orphan (`parse/orphans.read`); two levels deeper the lexer's own `indentation_jump` stands too, three messages, and the join fix, from the `-`'s end to the arm's first byte, writes the arm at the `-`'s margin, a block deeper than anything that opens one. Defect 182's move, the margin taken back, made for two readings is the lexer's (`selfhost/sign_above.hero`), not this lane's file; a hold in the parser could not take back the lexer's own margin reports. Not repaired.

    Repaired at `1684bab5`, 2026-10-05 (lane b11-parse, its files widened to `sign_above.hero` that day), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A tab margin under the `-` keeps its reports, as defect 182's does, and two stray `-` lines in a row are a shape beside, not at depth one.
