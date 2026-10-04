---
kind: defect
area: process
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **278 — the `/decide`, `/learn` and `/step` skills still send items to `LEARN.md` and `SCHEDULED.md`, lists retired on 2026-09-12** | `git grep -n -E 'SCHEDULED\.md|LEARN\.md' -- .claude/skills/`: `decide/SKILL.md:25`, `:27` and `:84`, `learn/SKILL.md:69`, `:82` and `:86`, `step/SKILL.md:39` and `:57`, each naming a file that has not existed since `8715133c` (M-rotated-records step 5); `where/SKILL.md:81` is the one that says *retired 2026-09-12* (read by the coordinator, 2026-10-04) | the three skills' routing sentences · `docs/learn/` and `docs/work/milestones/`, the homes since that day · **class: improvement**

    **Origin:** lane b8-defects, 2026-10-04, rewording the texts that name the defect list (its reply's *found beside*, `/decide` and `/step`); `/learn` found by the coordinator's grep the same night.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a process text that names a retired list; a skill following it looks for a file that is not there; no program moves.
