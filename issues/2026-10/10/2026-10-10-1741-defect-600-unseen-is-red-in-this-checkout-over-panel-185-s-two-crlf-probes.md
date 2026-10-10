---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: self
github: none
---

- [x] **600 — `unseen` is red in this checkout over panel 185's two CRLF probes, and the tree is green** | `./heroes run tests/harness/main.hero -- ./heroes unseen` in the trunk's checkout at `87794631` (and at `0cc01326`) reads *2 passed, 1 failed*: `docs/panel/185-briefs/probes/q4/s5h_crlf.hero` and `s5h_crlf_applied.hero`, lines 1 to 6 each, `U+000D`. **Not a defect of the tree**: `git ls-files --eol` on both reads `i/lf w/crlf attr/text=auto eol=lf`, so git stores LF and only this checkout's working copy holds the CR bytes; a fresh clone, the CI's among them, reads the files as LF and `unseen` green there (session heroes-lang-98's reading, 17:44, re-run by the coordinator at 17:46) | the two working copies; restoring them is `git checkout -- <the two paths>`, a destructive operation the author approves (CLAUDE.md § Hard stops); `tests/harness/suite_unseen.hero` is right as it stands · **class: adjacent**

    **Origin:** found by panel 209's coordinator at 17:35 on 2026-10-10, running `records` and `unseen` over the tree before the sitting's commit (`.claude/worktrees/scratch-b15/209-coordinator/suites.txt`, ignored by git). Filed at 17:41 as defect 590, class `blocking`, *a suite red on the trunk*, in commit `bcbca3fd`.

    **Class: adjacent**, 2026-10-10: real in this checkout, none of the blocking shapes once the storage is read; it stops no gate run from a clean clone.

    **Corrected and closed, 2026-10-10 17:47.** The number 590 was taken by lane b19-pack's branch at 16:47 (session heroes-lang-98's message of 17:44, verified by `git ls-tree -r --name-only lane-b19-pack -- issues`), so this item takes 600, the file renamed with its number and its stamp kept. The reading *a suite red on the trunk* was false: the suite reads the working copy, and the working copy of an LF-stored file held CR bytes left by panel 185's probe tool on 2026-10-02, which git's `text=auto eol=lf` attribute hides from `git status`. The repair is the restore above, owed to the author's yes; nothing in git moves. The commit message of `bcbca3fd` says *defect 590, blocking* and stands as history.
