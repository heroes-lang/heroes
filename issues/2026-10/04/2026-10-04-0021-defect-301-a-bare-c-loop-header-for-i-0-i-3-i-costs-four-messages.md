---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: e08488a52a04851bd433960ac028973cc376525b
github: none
---

- [ ] **301 — a bare C loop header, `for i = 0; i < 3; i++`, costs four messages** | `for i = 0; i < 3; i++` with no parentheses: four messages, where defect 194's repair tells the parenthesised header once (lane b9-recovery's compiler, 2026-10-04) | `selfhost/scan.hero` (the lexer's `;`), `selfhost/parse/loop_habit.hero` · defect 194 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 194; by the lane's reading another cause, the bare header never reaching the loop habit's parenthesised reading.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): several messages for one mistake; if its cause is 194's own, 194 reopens.

    Repaired at `e08488a5`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
