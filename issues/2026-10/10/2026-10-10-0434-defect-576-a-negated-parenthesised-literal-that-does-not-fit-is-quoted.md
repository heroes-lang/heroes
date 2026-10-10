---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: 79e557b5fde0d177c05576676e3b5192e2807d45
github: none
---

- [ ] **576 — a negated parenthesised literal that does not fit is quoted without its closing parenthesis** | `x: i8 @ -(-128)` is refused *`` `-(-128` `` does not fit an `i8`*, the span's caret one character short under `-(-128)`; reproduced by the coordinator at 04:33 with the trunk's compiler (`.claude/worktrees/scratch-b15/r576/neg.hero`); the base before batch 18 the same | the span of a unary minus over a parenthesised operand, `selfhost/check/` and the parser's span of `-(...)` · **class: adjacent**

    **Origin:** found by lane b18-infer beside defect 564 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-infer/notes.txt`, ignored by git), filed by the coordinator at 04:34 on 2026-10-10.

    **Class: adjacent**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message whose quote and span are less exact than they could be.

    Repaired at `79e557b5`, 2026-10-10 (lane b18-infer), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Not a minus's alone: a parenthesised expression is its contents, and every operator built its span from its operands', so a `!`, a binary operator at either edge, and a field, an index, a call, a `?` or a `::` after a parenthesised base lost a parenthesis too; each now spans from the first token its form read to the last. `-(-128)` and `-((200))` are quoted whole, `(1 + 2) * 3` and `(p).x` underlined from the `(`. Cases `check/` and `full/fixedbugs-576-…`, three goldens told one column earlier and corrected beneath; `check` 653, `full` 34, `fixes` 937, `annotations` 945, `emission` 1096 with nothing moved, the compiler's 1,547 tests, 0 failed; no code moves in the census and the IR of the 1,643 programs both compilers accept is identical.
