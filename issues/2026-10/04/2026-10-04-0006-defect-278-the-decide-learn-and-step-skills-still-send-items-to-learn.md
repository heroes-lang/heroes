---
kind: defect
area: process
milestone: none
filed: 2026-10-04
commit: 743bfaabce9788268477c36a98c8f7453ff5a4a4
github: none
---

- [x] **278 — the `/decide`, `/learn` and `/step` skills still send items to `LEARN.md` and `SCHEDULED.md`, lists retired on 2026-09-12** | `git grep -n -E 'SCHEDULED\.md|LEARN\.md' -- .claude/skills/`: `decide/SKILL.md:25`, `:27` and `:84`, `learn/SKILL.md:69`, `:82` and `:86`, `step/SKILL.md:39` and `:57`, each naming a file that has not existed since `8715133c` (M-rotated-records step 5); `where/SKILL.md:81` is the one that says *retired 2026-09-12* (read by the coordinator, 2026-10-04) | the three skills' routing sentences · `docs/learn/` and `docs/work/milestones/`, the homes since that day · **class: improvement**

    **Origin:** lane b8-defects, 2026-10-04, rewording the texts that name the defect list (its reply's *found beside*, `/decide` and `/step`); `/learn` found by the coordinator's grep the same night.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a process text that names a retired list; a skill following it looks for a file that is not there; no program moves.

    Repaired at `743bfaab`, 2026-10-04, M-issue-files step 4, which rewrote the
    routing sentences of every skill for the issues; gated by the full net of
    that lane.

## The repair

Repaired at `743bfaab`. `/decide`, `/learn` and `/step` name the kinds an item is
filed as, `learn`, `decision`, `feature` and `task`, files of `issues/`, and no
list that is retired: `git grep -n -E 'SCHEDULED\.md|LEARN\.md' -- .claude/skills/`
finds one sentence, `/learn`'s, which says those files are retired and since
when. `/where` and `/panel` were rewritten the same way.

**Closed 2026-10-05** at M-issue-files's gate, on the lane's tree: the full net
5,323 passed and 1 failed, the one a citation in a comment of
`tests/harness/suite_records.hero` that the same lane wrote and repaired, after
which `spec` read 21 and 0, `records` 28 and 0, `canonical` 2 and 0 and `probe`
27 and 0; the net's own tests 268, the compiler's own 1,213, all passed. Ticked
where it stands, its file unmoved (`.claude/rules/records.md` § The issues).

