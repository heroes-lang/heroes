---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **327 — a clang warning on a bound header is still printed once per program unit, `library.c` including the header where a group record is used** | since defect 248's repair (`50aaac0b`) the pointee and layout probes no longer print their own copy, and a header warning still reaches the author twice where the program uses a group record: once from the program's unit and once from `library.c`, which includes the header for it | how `selfhost/cli/` hands clang's output of each unit to the author · defect 248 · **class: adjacent**

    **Origin:** lane b10-cli, 2026-10-04, reproduced on its compiler at `86b29733` (its final reply's *Found beside*), beside 248.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake (the same warning twice).

    Repaired at `2697b8f9`, 2026-10-04 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shapes beside with this cause were repaired with it: two modules (three copies), the struct-tag round's second pass and its warm replay (three copies each), `--emit-c` (two).
