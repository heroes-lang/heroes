---
kind: defect
area: emit
milestone: none
filed: 2026-10-02
commit: d83ef0b1e7ec1f77b7b07b82d59c0cb59a9bafa8
github: none
---

- [ ] **189 — `typeorder.visit`'s walk without recursion has no witness** | the record chains the run cases emit are 1,000 deep (`run/fixedbugs-140-records-` and `-extern-records-a-thousand-deep-build`), and at 1,000 and 5,000 lane ci140's mutant, `visit` restored to its recursion, emits the trunk's bytes (666,776 and 3,358,806, `cmp` equal) | `selfhost/emit/typeorder.hero:130` and its one test at `:206` · **class: improvement**

    **Origin:** lane ci140's report, 2026-10-02 (`scratchpad/lane-ci140/mut/emit-typeorder/heroes-mut`, 2026-10-02, *the mutant that no case catches*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/typeorder-witness/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). At 20,000 deep the mutant reached clang, which died (defect 170), so a unit test is the witness the lane names.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): coverage.

    Repaired at `d83ef0b1`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. No program can reach a depth where a recursion dies (the front end costs 233.4 billion instructions at 20,000 records), so `typeorder.visit` walks a graph given as data, built by `emit/type_needs.hero`'s queue, and its test walks a chain of 100,000, past the 6,000 to 7,000 where a recursion of it dies on an 8 MiB stack at -O0; the mutant restoring the recursion fails that test with *panic: stack exhausted*, the emitted C is byte-identical at 500, 1,000 and 2,000 records, and `--emit-c` there costs 4.12, 5.80 and 10.00 billion instructions where it cost 4.49, 7.02 and 14.86.
