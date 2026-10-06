---
kind: decision
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **panel 196** | ratify, amend or overturn R1 to R9 (an `@` argument reaches C as its own place, never a copy, through an element and a Heroes function's own `@` parameter; one element unless an extent is stated; `lent` legal on `@`, and a temporary only for `lent`; the buffer C fills owned by the emitter in a per-thread guarded region, with a constant, a literal or a sibling as its extent; the header's own extent read; a local lent in place inside the guard; the fixes that drop an extent repaired; S1, S3, S4 as a copy, S5′, S6, S0 and nothing refused) | `docs/panel/196-an-argument-reaches-c-as-its-own-place-one-element-unless-an-extent-is-stated-and-a-buffer-c-fills-is-lent-and-guarded.md`

    **Origin:** panel 196's synthesis, 2026-10-06 from 23:41, on the tree frozen at `39935f7c`, convened for defect 396 under the author's goal of that day.

    **Recommendation: ratify R1 to R9**, the robust route at each disagreement, on what was built and run: S7 built by the compiler-engineer and run from one `.hero` file; the guard page's per-thread form taken because the critic measured the canary and the one-region page failing (a pattern equal to the canary's, two threads, N above a page); R1 because the critic measured one spelling handing C four different addresses, and zlib and libuv failing in three of them; R3 because S7's buffer, kept by `setvbuf`, reads a dead frame.

    **The conservative alternative, the author's to choose instead**: R2 and S7 with constant extents only, a 16-byte canary with a per-process random pattern on the stack, R1 limited to array elements and group records lent through a wrapper, no `lent` on `@`, and R6 and R7 waiting.
