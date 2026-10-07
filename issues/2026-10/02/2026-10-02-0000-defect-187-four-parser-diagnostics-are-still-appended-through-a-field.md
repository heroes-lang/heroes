---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: 885fad916908fe0eba3665970deadf75209ae3b8
github: none
---

- [ ] **187 — four parser diagnostics are still appended through a field place, against defect 146's rule and `cursor.hero`'s own comment** | `git grep -n 'c.diagnostics @ c.diagnostics.push' -- selfhost/parse/` prints `loop_habit.hero:62`, `line_end.hero:251`, `type.hero:257`, `type.hero:320`, while `selfhost/cursor.hero:272` says *Every parser module appends through this* | the four lines · `cursor.push_diagnostic` (`selfhost/cursor.hero:273`) · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/field-place-push/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), which finds three more modules appending the same way, their cost unmeasured. No cost is measured for any: at `loop_habit.hero:62` the append is not what makes defect 184 slow.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form nobody needs to be right.

    Repaired at `885fad91` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The four lines went before the batch, at `bd43a92c` (defect 184) and `e09550e7` (defect 343), and the modules found beside them at `f50b9102` and `36396b71` (defect 350), whose `layout` check refuses a list of reports grown through a field; the same append left in `parse/rest_words`, `a.rests @ a.rests.push(arg)`, goes through `ast.push_rest`, and the words' cost to `parse` 16,000 constructions falls from 19.9 billion instructions retired to 0.14, linear (1,000 to 16,000: 98, 327, 1,283, 5,030, 19,941 million before; 14, 17, 35, 70, 145 after).
