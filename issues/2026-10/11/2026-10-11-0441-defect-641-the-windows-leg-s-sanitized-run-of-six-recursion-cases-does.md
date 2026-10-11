---
kind: defect
area: runtime
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **641 — the Windows leg's sanitized run of six recursion cases does not say `stack-overflow`** | `run/fixedbugs-508-*`, `fixedbugs-520-*` (2) and `fixedbugs-547-*` (3) at `--sanitize` on Windows x86-64: *must be caught by the sanitiser saying `stack-overflow`, and it said:* nothing; the same cases pass at -O0 and -O2 there and on the three other legs; AddressSanitizer on Windows reports a stack overflow differently or not at all, a question for the box | `tests/harness/suite_run.hero`'s sanitizer expectation and the runtime's stack guard under ASan on Windows; the Windows box · **class: blocking**

    **Origin:** filed by the coordinator at 04:41 on 2026-10-11 from the CI's Windows x86-64 leg of batch 20's push (`4fb8bd98`, run 38101444166, job 114358017019, its log through the API at `.claude/worktrees/scratch-b15/ci-4fb8/`, ignored by git), the first run of that leg to reach the net past the compiler's own tests (1,607 passed there), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a red CI.
