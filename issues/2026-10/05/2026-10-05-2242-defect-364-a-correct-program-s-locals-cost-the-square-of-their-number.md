---
kind: defect
area: resolve
milestone: none
filed: 2026-10-05
commit: 1883ca7d7030938bab247206c2b9fc5a308fed4a
github: none
---

- [ ] **364 — a correct program's locals cost the square of their number to resolve** | `function main()` with `x0 = 1` and N lines `x<i> = x<i-1> + 1`, a correct program: `check --brief` retires 1.67 billion instructions at N = 2,000 and 11.69 billion at 8,000 (the base's compiler at `00217c39`, measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `n2-*`) | `selfhost/resolve/state.hero:86`, `lookup_local`, which scans every open scope entry, called by `declare` for the shadowing check and once per name read (a sample at N = 16,000, lane b12-parse12) · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work on a correct program, the square of its locals, no message wrong.

    Repaired at `1883ca7d`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
