---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **395 — a field lent to C is counted in bytes where C counts in its own unit** | a `u8[16]` field lent through `f.ptr()` to a parameter C declares `int *`, `counted_by n` with `n: 16`: `check` 0, `build` 0, `run` 0 with the next field overwritten, printing 4702111234474983745 (the coordinator's re-run between 18:23 and 18:26, the clock read before and after, on the tree frozen for panel 194, `7a26a0a6`); `mbstowcs` over the same field with `n: 16` writes 64 bytes and exits 134 on Darwin, 135 and 139 on the Linux legs (the ffi-pragmatist's) | `selfhost/emit/lend_extent.hero` (the extent a `counted_by` lend is checked against) · `docs/panel/194-evidence/new-defects/field-unit/` · panel 194 R7 · **class: blocking**

    **Origin:** panel 194's compiler-engineer and ffi-pragmatist, 2026-10-06, each beside defect 092; reproduced by the completeness critic's second pass and by the coordinator on the frozen tree; panel 166's ergonomist had named it as an unrun guess.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a memory fault at exit 0 in a program the spec admits, § 13's own field lend; panel 194's route C, the count in C's own unit, repairs it with 092.
