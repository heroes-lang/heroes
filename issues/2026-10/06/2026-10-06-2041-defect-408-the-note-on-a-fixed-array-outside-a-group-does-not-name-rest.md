---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **408 — the note on a fixed array outside a group does not name `rest: zero`** | defect 399's repaired note tells a reader meant to build a C struct to write the elements out where it is built, while panel 194's R1, landed by lane bs in the same round, ends such a construction with `rest: zero`; naming the program's own record field needs `check/ffi.hero` and `check/ffi_sweep.hero`, lane bs's files | `selfhost/ffi_errors.hero`, `selfhost/check/ffi.hero` · **class: adjacent**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a message that does not name the repair the language now has for the program in hand (design.md §4.17).
