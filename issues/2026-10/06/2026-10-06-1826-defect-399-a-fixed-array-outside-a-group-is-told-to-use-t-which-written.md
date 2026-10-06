---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **399 — a fixed array outside a group is told to use `[T]`, which written as said gives five more errors** | panel 194's blind readers of task 1 under two variants wrote a record's `u8[256]` fields where no group holds them; the `fixed_outside_a_group` note says *use `[T]` here*, and the program written as it says gives five `type_mismatch` (the completeness critic's second pass, compiling the readers' programs, `docs/panel/194-reports/blind/b194-t1-L.md` and `-t1-C.md`; not re-run by the coordinator) | the `fixed_outside_a_group` note, `selfhost/ffi_errors.hero` and `selfhost/check/ffi.hero` · design.md §4.17 · **class: adjacent**

    **Origin:** panel 194's completeness critic, its second pass, 2026-10-06.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a true refusal whose note points to a dead end, a message less exact than it could be. Into batch 13.
