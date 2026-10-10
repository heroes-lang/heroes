---
kind: defect
area: check
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **614 — the control arm reads a cell on the wildcard as a cell** | under `check --permissive`, which drops `never_rebound` and so defect 608's refusal, `_ @= nullptr` is refused `cannot_infer`, *`nullptr` alone says `ptr`, not which handle `_` will hold*, with a guess writing `_: <Handle> @= nullptr`, where `_ = nullptr` is accepted in both arms; `_ @= []` is told `cannot_infer` with the guess *annotate the cell: `_: [i64] @= []`* where `_ = []` is told the binding's; `_ @= f(1)`, `_ @= h(1)` (a `T?`) and `_ @= g(1)` (a `()`) exit 0 as their `=` lines do in that arm | `selfhost/check/walk.hero`'s `.declare` arm, `born(…, cell: true)` for any name, `_` included; `selfhost/check/empty_binding.hero` (`poisoned`'s `bare_cell`, `retell`'s `cell`) · **class: adjacent**

    **Origin:** found by lane b20-check at 01:23 on 2026-10-11, measuring defect 608's shapes (`_ @= nullptr`, `_ @= []`) under `--permissive` with the lane's compiler after that repair; the normal arm never reaches the checker on such a line, the resolver's `never_rebound` telling it first.

    **Class: adjacent**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): found beside the work in the control arm alone, as defects 193 and 251 were; the program the arm judges is refused there for a reason that is not a thesis rule's, a `cannot_infer` whose words treat `_` as a cell, so the arm's count reads one refusal the language would not make of `_ = nullptr`.
