---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: f90ee44e19ef804dd4b3d1871d93266f3b078607
github: none
---

- [ ] **409 — a push onto a record's array field copies the whole array** | `r.f @ r.f.push(x)` copies the array at every push (by design, panel 037: only a plain name grows in place): 3.0, 12.1 and 48.1 billion instructions at 10,000, 20,000 and 40,000 pushes, against 21.6, 27.3 and 38.6 million for a local; 368 such lines in `selfhost/`, of which defect 393's three in `print/elements.hero` were quadratic | `grep` of `@ .*\.push(` on a field in `selfhost/` · **class: improvement**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost, no program judged wrong; an audit of the lines inside loops.

    Repaired at `f90ee44e`, the last of four commits with `c22c5a15`, `2a26fde2` and `a3e3a138`, 2026-10-07 (lane b14-p409), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The audit, from the world: 404 lines of `selfhost/` store a push into a place with a field or an index, 402 of them a push of that same place, 230 onto a diagnostic's `notes` and 123 onto its `fixes`; counted per call site in an instrumented runtime over the compiler's own source and 14 shapes at four sizes each, twelve lines grew with the program and are lent now, so each grows in place and copies nothing at any size: the C writer's chunk and chunks (`--emit-c` of the compiler's own source copied 259,772,868 and 64,663,810 elements), the type table's nodes, flags and parameter and name runs (11,397,925 ids, 4,959,675 nodes; the runs' find-or-append moved to `check/type_runs.hero`), the checker's held reports, route M's reports and its holes (7,998,000 copies each at 4,000), and a compilation's faults (799,980,000 copies at 40,000 bytes that are not UTF-8). Instructions retired, base then repair, every output byte-identical: `check` of the compiler's own source 67.47 billion to 65.51, `--dump-ir` 152.39 to 150.42, `--emit-c` 1,310.95 to 1,245.50; the faults' 48.49 to 0.36 at 40,000 bytes.
