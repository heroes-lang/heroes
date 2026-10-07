---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **428 — two writable `cstr` parameters of one declaration are told one at a time** | a declaration with two `cstr` parameters C writes is told `ffi_writable_parameter` for the first, the second only once the first is repaired, the diagnostic spanning the whole declaration and kept once per span; the same on the base | `selfhost/emit/ffi_mutable.hero` · **class: adjacent**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-w411's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed.
