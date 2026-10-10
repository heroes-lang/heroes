---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **576 — a negated parenthesised literal that does not fit is quoted without its closing parenthesis** | `x: i8 @ -(-128)` is refused *`` `-(-128` `` does not fit an `i8`*, the span's caret one character short under `-(-128)`; reproduced by the coordinator at 04:33 with the trunk's compiler (`.claude/worktrees/scratch-b15/r576/neg.hero`); the base before batch 18 the same | the span of a unary minus over a parenthesised operand, `selfhost/check/` and the parser's span of `-(...)` · **class: adjacent**

    **Origin:** found by lane b18-infer beside defect 564 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-infer/notes.txt`, ignored by git), filed by the coordinator at 04:34 on 2026-10-10.

    **Class: adjacent**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message whose quote and span are less exact than they could be.
