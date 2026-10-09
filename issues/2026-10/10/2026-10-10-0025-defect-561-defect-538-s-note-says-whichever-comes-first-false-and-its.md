---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **561 — defect 538's note says *whichever comes first*, false, and its remedy led two readers to edit a correct program** | `rlmath1` (raymath.h bound first, raylib.h second) fails `test` with the note *whichever comes first*, and `rlmath2`, the same two swapped, passes `test`, and so `tasn1a` and `tasn1b` (libtasn1 and OpenSSL); the blind seat's two readers given the message on `fp` both edited a correct program, one writing `use left` and `left.twice(...)`, which `check` refuses `extern_across_modules`, and both argued its second remedy, *a header of your own*, cannot work where the other header defines the name | `selfhost/cli/headers_together.hero` and `header_refused.hero` · panel 202 R2 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): the ffi-pragmatist's `rlmath` and `tasn1` cases, and the blind seat's `d3-a` and `d3-b` as the critic's second pass read them, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message.
