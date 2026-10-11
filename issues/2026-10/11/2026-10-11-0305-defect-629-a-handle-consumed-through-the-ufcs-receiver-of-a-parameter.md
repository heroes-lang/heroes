---
kind: defect
area: check
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **629 — a handle consumed through the UFCS receiver of a parameter without `@` is accepted, where the named call is refused** | `function finish(n: Node)` whose body is `n.node_free()`, `node_free` a C function whose parameter `consumes`, is accepted, and a later use of the caller's handle aborts 134 at run time; the named call `node_free(n)` in the same body is refused `consumed_borrowed_handle`, which the checker asks only on the named-call path (`selfhost/check/walk.hero:1202`); `h.h_into(...)` for `transfers` the same | `selfhost/check/walk.hero` and the UFCS call's lowering of a consuming parameter; panel 210 R4 · **class: blocking**

    **Origin:** filed by the coordinator at 03:05 on 2026-10-11 from panel 210's completeness critic (`docs/panel/210-reports/critic.md`), its builds under `.claude/worktrees/scratch-b15/210-critic/` (ignored by git), the seat's measurement on the tree frozen at `1dd890751`, not re-run by the coordinator.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted (memory stays safe: the abort is the runtime's).
