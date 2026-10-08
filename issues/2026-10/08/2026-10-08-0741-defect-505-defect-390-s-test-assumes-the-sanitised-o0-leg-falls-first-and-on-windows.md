---
kind: defect
area: harness
milestone: none
filed: 2026-10-08
commit: e607e48920e16d914fe9e9b171b129026d7931c9
github: none
---

- [ ] **505 — defect 390's test assumes the sanitised `-O0` leg falls first, and on Windows the plain `-O0` leg does** | the CI's Windows leg of run 37703883667 on `02256c1e` read the full net 6,648 passed and 0 failed and the net's own tests 308 with 1 failed, *fixedbugs: a C write past a field is seen by the sanitiser at -O0, the one leg that sees it (defect 390)*, at `assert strings.starts_with(text: over.failures[0], prefix: "FAIL run/over at --sanitize -O0\n")`; on the Windows box (clang 23.1.1, the trunk's compiler built there, `/c/w/b14-fix390-77876`) the control writing 16 bytes from a record's last 8-byte field reads: `-O0` prints `0` and `done` then exits 139 (segmentation fault), `-O2` exits 0, `--sanitize` and `--sanitize -O0` both exit 1 with AddressSanitizer's *stack-buffer-overflow* (run by the coordinator before 07:41 on 2026-10-08) | `tests/harness/suite_run.hero:349`, the test's premise that one leg alone sees the write · defect 390 · **class: blocking**

    **Origin:** filed by the coordinator at 07:41 on 2026-10-08 from the CI's Windows leg after batch 14's push, reproduced on the box before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): a red CI. The compiler and the sanitiser leg are right on Windows; the test's premise is a fact of Darwin and Linux, a narrowing that asks the world (`.claude/rules/module-shape.md` § A narrowing asks the value).

    Repaired at `e607e489`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests (318 passed); the net is owed at the batch's close, and Windows is the CI's leg after the push. The test asks the sanitised `-O0` leg alone (`sanitised_failed` with `--sanitize -O0`), which must fail naming `stack-buffer-overflow` for the over-writing program and pass the control, then that the whole case fails at some leg and its control passes every leg; in scratch copies on this Mac it is red with `-O0` removed from that leg's build, and green where a plain `-O0` leg falls first, the old test red there at `over.failures[0]` as on Windows.
