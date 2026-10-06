---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 395fe45e7dc5dba11d44c9382af96f28bdc48fc7
github: none
---

- [ ] **397 — a handle built with no argument is told a compiler bug at run time** | `record Opaque tag opaque` (a handle) and `h = Opaque()`: `check` 0, then `run` 134 with *panic: entered unreachable code — this is a compiler bug, please report it* (the coordinator's re-run on the frozen tree, `docs/panel/194-evidence/new-defects/handle-empty/`) | the construction of a handle record, `selfhost/check/` and `selfhost/emit/aggregate.hero` · spec § 13 (a handle) · **class: blocking**

    **Origin:** panel 194's compiler-engineer, 2026-10-06 (*found beside* 2), met while building R1; reproduced by the critic's second pass and the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a crash and a false message, the compiler blaming itself for a program `check` accepts; whether a handle may be built at all is what the repair rules (the spec says nothing of it; *never built* is the seat's inference).

    Repaired at `395fe45e`, 2026-10-06 (lane b13-bs), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
