---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: de7ed3273286d93e7643671645713c30ebad1497
github: none
---

- [ ] **426 — a `cstr` lent to a C function declared with no prototype is written by C at exit 0** | `int x_kr();` with a K&R definition taking `unsigned char *p`, bound `x_kr(p: cstr lent)` and lent a shared `str`: C writes it (`Zbc`) at exit 0, on the base and after defect 411's repair, on this Mac and Linux arm64; there is no prototype to ask, and the pointee check's call checks nothing | `selfhost/cli/pointee_wants.hero`, the question asked of a function with no prototype · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-w411's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): memory written behind the program's back at exit 0 (design.md §1.12), the shape 411 closed for a prototype.

    Repaired at `de7ed327`, 2026-10-07 (lane b13-w411): an extern with parameters whose header gives no prototype is refused on its declaration as `ffi_parameter_type`, detected by `!__builtin_types_compatible_p(__typeof__(*f), __typeof__((f)()) (void *))`, the message naming a header of the author's own that adds the documented prototype; cases `unsupported/fixedbugs-426-*`; gated by its cases, the compiler's own tests, every form whole on this Mac and its cases on Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.
