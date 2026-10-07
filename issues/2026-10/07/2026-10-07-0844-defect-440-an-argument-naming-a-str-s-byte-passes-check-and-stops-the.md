---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 02bea033eef8fe4f878c9fda35497becdd7060dc
github: none
---

- [ ] **440 — an `@` argument naming a `str`'s byte passes `check` and stops the build** | `f(@s[0])`, `s` a `str`: `check` 0, then `build` exit 2 with clang refusing the generated C, where `s[0] @ 65` is refused by the checker (lane b13-land-addr's measurement on its branch, not re-run by the coordinator) | the checker's reading of an `@` argument that is an index into a `str`, `selfhost/check/` · **class: blocking**

    **Origin:** filed by the coordinator at 08:44 on 2026-10-07, from lane b13-land-addr's report of the night before (*found beside*, landing panel 196's R1); the lane's measurement.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, `check` accepting what the build cannot make.

    Repaired at `02bea033`, 2026-10-07 (lane b13-land-addr): each marked argument of a call is asked `access.refuse_str_element` once typed, the check `s[0] @ 65` goes through, so `@s[i]` on a `str` is told `str_element_write` at `s[0]`, to a Heroes function, an extern, through UFCS, a field's byte, an array's string's byte and a generic `@x: T` alike; a `[u8]` element and a `str` lent whole stay legal; cases `check/fixedbugs-440-*` and `run/fixedbugs-440-*`; gated by its cases, `check` whole and `corpus`, and the compiler's own tests (the lane's); the card filled by the coordinator.
