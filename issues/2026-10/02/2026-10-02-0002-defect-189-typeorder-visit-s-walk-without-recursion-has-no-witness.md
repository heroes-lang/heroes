---
kind: defect
area: emit
milestone: none
filed: 2026-10-02
commit: none
github: none
---

- [ ] **189 — `typeorder.visit`'s walk without recursion has no witness** | the record chains the run cases emit are 1,000 deep (`run/fixedbugs-140-records-` and `-extern-records-a-thousand-deep-build`), and at 1,000 and 5,000 lane ci140's mutant, `visit` restored to its recursion, emits the trunk's bytes (666,776 and 3,358,806, `cmp` equal) | `selfhost/emit/typeorder.hero:130` and its one test at `:206` · **class: improvement**

    **Origin:** lane ci140's report, 2026-10-02 (`scratchpad/lane-ci140/mut/emit-typeorder/heroes-mut`, 2026-10-02, *the mutant that no case catches*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/typeorder-witness/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). At 20,000 deep the mutant reached clang, which died (defect 170), so a unit test is the witness the lane names.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): coverage.
