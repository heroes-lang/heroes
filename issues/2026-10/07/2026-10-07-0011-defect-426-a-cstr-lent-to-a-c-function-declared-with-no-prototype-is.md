---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **426 — a `cstr` lent to a C function declared with no prototype is written by C at exit 0** | `int x_kr();` with a K&R definition taking `unsigned char *p`, bound `x_kr(p: cstr lent)` and lent a shared `str`: C writes it (`Zbc`) at exit 0, on the base and after defect 411's repair, on this Mac and Linux arm64; there is no prototype to ask, and the pointee check's call checks nothing | `selfhost/cli/pointee_wants.hero`, the question asked of a function with no prototype · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-w411's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): memory written behind the program's back at exit 0 (design.md §1.12), the shape 411 closed for a prototype.
