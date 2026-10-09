---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **556 — two non-static definitions of one C name in bound headers make `build` exit 2** | the critic's `onedef` (one header defining a non-static function, named by two modules) and `extdef` (two headers each defining a non-static `twice`): `build` exits 2, *internal error: linking failed: duplicate symbol '_twice'*, blaming the compiler; `onedef` passes `test`, the reverse of the two-header case | the link step's reading of a duplicate symbol, `selfhost/cli/` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.
