- [ ] **302 — a C loop header whose `(` is never closed takes the body into the bracket, and `missing_body` is told at the end of the block** | `for (i = 0; i < 3; i++` with its `)` left out, a body below: the bracket takes the body, and `missing_body` is told at the block's end rather than at the header (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/loop_habit.hero` · `selfhost/parse/unclosed.hero` · defect 194 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 194.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a mistake told far from where it stands, and a second message for it.
