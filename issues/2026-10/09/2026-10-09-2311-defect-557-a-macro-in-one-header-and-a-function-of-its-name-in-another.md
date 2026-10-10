---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **557 — a macro in one header and a function of its name in another are told that one header does not compile** | the critic's `macro`: `a.h` `#define twice(x)`, `b.h` `static inline int twice`; `test` exits 1 with the old false message *`b.h` ... does not compile: expected identifier or '('*, and with the `use` lines swapped the program passes every tool: a false message, and a verdict that depends on `use` order | `selfhost/cli/headers_together.hero` · defects 538 and 550 · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the shape beside defect 550.
