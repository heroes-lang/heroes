---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: f0404dac3c88eea570712ad45ded866cf7876ea5
github: none
---

- [ ] **390 — `run --sanitize` builds at `-O2`, where AddressSanitizer misses a stack overwrite that `-O0` catches** | a 4096-byte overwrite of the stack through `memset` in a header's `static inline`: `heroes run --sanitize` exits 0, `heroes build --sanitize` at `-O0` aborts with exit 134 (lane b12-ffi13's measurement, 2026-10-06, while gathering defect 092's evidence; not re-run by the coordinator); plain C at `-O2` misses it too, so it is the toolchain's, and the `run` suite's sanitizer leg can be blind to this shape | `selfhost/cli/` (the optimisation `--sanitize` builds at) · `tests/harness/suite_run.hero` (its sanitizer leg) · defect 092 · **class: improvement**

    **Origin:** lane b12-ffi13, 2026-10-06 (its report on defect 094, *Found beside* 2); filed by the coordinator at 13:44.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach, no program judged wrong by the compiler; the lane recommends the sanitizer leg run at `-O0` as well. Outside the batch under the author's instruction of 2026-10-05.

    Repaired at `f0404dac`, 2026-10-07 (lane b14-cli), gated by its cases and the net's own tests; the net is owed at the batch's close. Each `run` case is run again with `run --sanitize -O0`, `-O0` and `--sanitize` composing with no new flag; its control, `memset` eight bytes past a record's last field from a header's `static inline`, passes plain `-O0`, `-O2` and `--sanitize` at exit 0 and fails the new leg alone on *stack-buffer-overflow*, its exact twin passing every leg; `run` whole 372 passed on this Mac, the two legs +34.6% instructions on a sample of 93 cases.
