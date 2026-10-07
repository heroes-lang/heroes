---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: 80f8824f15749d3d0894377b343e3a8139abbb7a
github: none
---

- [ ] **460 — a type parameter named like a built-in is told three times** | `g<len>(x: len)`: `builtin_name_taken` and two more messages, because the refused parameter is never bound (lane b14-resolve) | `selfhost/resolve/decls.hero` · defect 455 · **class: adjacent**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told three times.

    Repaired at `80f8824f`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The refused type parameter is kept, marked told, and each use of it stays unresolved and silent, as an unknown type's uses are; a primitive's name was already read as the primitive, and the same built-in twice is told at each. The case reads 6 messages where the base read 17, in a signature, nested, in a body and beside a library's name; fixedbugs-455's type parameter moves from three to one; `check` 590, `full` 20, the compiler's own tests 1,369, all passed.
