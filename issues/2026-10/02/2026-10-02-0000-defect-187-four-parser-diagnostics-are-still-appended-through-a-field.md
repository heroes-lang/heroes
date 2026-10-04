---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: none
github: none
---

- [ ] **187 — four parser diagnostics are still appended through a field place, against defect 146's rule and `cursor.hero`'s own comment** | `git grep -n 'c.diagnostics @ c.diagnostics.push' -- selfhost/parse/` prints `loop_habit.hero:62`, `line_end.hero:251`, `type.hero:257`, `type.hero:320`, while `selfhost/cursor.hero:272` says *Every parser module appends through this* | the four lines · `cursor.push_diagnostic` (`selfhost/cursor.hero:273`) · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/field-place-push/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), which finds three more modules appending the same way, their cost unmeasured. No cost is measured for any: at `loop_habit.hero:62` the append is not what makes defect 184 slow.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form nobody needs to be right.
