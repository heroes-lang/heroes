---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 4156ee2946729c46567e9abecd0462a0b5140541
github: none
---

- [ ] **553 — `m = {}` is offered an array's annotation as its fix** | an empty map literal with nothing to type it gets the fix *annotate the binding: `xs: [i64] = []`*, an array's example for a map (lane b17-check) | the empty-literal fix of `selfhost/check/` · **class: adjacent**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a fix less exact than it could be.

    Repaired at `4156ee29`, 2026-10-09 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each empty literal has its own constructor in `contextless_errors.hero`, `empty_array_literal` and `empty_map_literal`, the map's guess *annotate the binding: `m: {str: i64} = {}`*. Case `full/fixedbugs-553-…`, five map shapes red on the base and the array's example standing; `check` 648, `full` 32, `permissive` 16, the compiler's 1,544 tests, 0 failed.
