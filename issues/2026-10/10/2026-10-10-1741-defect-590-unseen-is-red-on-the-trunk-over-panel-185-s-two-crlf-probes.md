---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **590 — `unseen` is red on the trunk over panel 185's two CRLF probes** | `./heroes run tests/harness/main.hero -- ./heroes unseen` on the trunk at `87794631` (and at `0cc01326`, the files unchanged) reads *2 passed, 1 failed*: `docs/panel/185-briefs/probes/q4/s5h_crlf.hero` and `s5h_crlf_applied.hero`, lines 1 to 6 each, `U+000D`; the two files were added on 2026-10-02 (`b391c774`) as probe inputs whose CRLF line ends are the thing they probe, and the suite began refusing `U+000D` on 2026-10-07 (`8ef1684a`, batch 14's gate), so the trunk's `unseen` has been red since, or the gate that ran it did not read it | `tests/harness/suite_unseen.hero` (its `REFUSED` list and what `shell.project_files` hands it); the probes under `docs/panel/185-briefs/probes/q4/`, a record (`.claude/rules/records.md` § A record is never rewritten); the repair is either an exemption for a probe input whose refused byte is its point, said in the suite with the file named, or the probe kept as the escape the message asks for where the probe's instrument reads escapes · **class: blocking**

    **Origin:** found by panel 209's coordinator at 17:35 on 2026-10-10, running `records` and `unseen` over the tree before the sitting's commit (`.claude/worktrees/scratch-b15/209-coordinator/suites.txt`, ignored by git); the sitting's own files hold no such character (they are not among the lines named). Not re-run on the trunk without the sitting's files, which cannot reach the two paths named.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a suite red on the trunk, the same as a red CI; it turns a batch's gate red for a cause no batch introduced.
