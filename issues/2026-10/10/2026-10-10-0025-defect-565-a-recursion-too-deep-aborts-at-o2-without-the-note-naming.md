---
kind: defect
area: runtime
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **565 — a recursion too deep aborts at `-O2` without the note naming the recursion** | `param_value.hero` (a self-call through a parameter) aborts 134 at `-O0` with *stack exhausted in paramvalue.go, inside the recursion of paramvalue.go and paramvalue.step*, and at `-O2`, `heroes run`'s level, with *stack exhausted in paramvalue.step* alone; `viamap` loses its `map` frame the same way; every run aborts 134, so the stop holds and the message is less exact | the runtime's report of an exhausted stack and the frames it can read at `-O2` · panel 203 R2 · **class: adjacent**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 203 (`docs/panel/203-an-argument-with-no-type-of-its-own-waits-for-the-call-s-others-and-a-self-call-through-a-parameter-aborts-at-run-time.md`): the critic's second pass, its runs under `.claude/worktrees/scratch-b15/203-critic2/`, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it is at `-O0`.
