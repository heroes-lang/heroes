---
kind: decision
area: none
milestone: none
filed: 2026-08-05
commit: a3045b79169eb63dc0d34bb2a2eb5c3e8e1b72a7
github: none
---

2026-08-05 | **`Op::CowCheck` does not return to the IR**, and the reason is a cycle a judge actually built. As an instruction "inserted before every mutation" it can be hoisted above the argument, and in that order the unshare is a no-op and `xs` ends up reaching itself: three blocks, ASan silent, only the block counter catching it. Kept inside the runtime primitive, C's own rule that all arguments exist before the callee's first statement makes the bad order **inexpressible** — which is better than a check, because a check can be forgotten | the same instruction was struck at M5b for having no call sites; it now has one and still does not belong in the IR | Part 5, §4.10 | 021, 022 |
