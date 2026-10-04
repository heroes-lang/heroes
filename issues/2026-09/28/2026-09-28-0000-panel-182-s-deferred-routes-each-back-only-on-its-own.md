---
kind: feature
area: emit
milestone: M-deployable-binary
filed: 2026-09-28
commit: none
github: none
---

- [ ] **M-deployable-binary** | panel 182's deferred routes, each back only on its own condition: the consuming store (b), the initialising stores (f), a value declared where it is defined (g), MemorySanitizer as a Linux instrument leg, and one slot for the synthetic slots of mutually exclusive arms | `docs/panel/182-a-value-is-never-zeroed-a-slot-is-and-a-definition-is-whole.md` · `docs/panel/182-reports/completeness-critic.md` · `selfhost/emit/`

    **Origin:** panel 182, 2026-09-28, deferred and not refused; ratified by
    the author the same evening, and scheduled here by the coordinator under
    CLAUDE.md § 3's delegated default, because this is the one scheduled
    milestone whose deliverable is what a built program costs when it runs,
    and that is what the routes buy. The author may move it.

    **The conditions, as the sitting wrote them.** (b) and (f) come back when
    the compiler itself crosses a leak gate (its runs report live blocks at
    exit: the critic's differential probe, or a runtime mode in which
    `hero_exit` checks) **and** (b)'s fault injection is shown to fire; their
    gain over the landed route is 6 to 9 points of user time, and their
    failure modes, a double release, a leak and for (f)'s consumed-store half
    a use after free, have no instrument today that sees them in the one
    program with 12,897 synthetic slots. (g), the critic's unlisted route,
    removes the value prologue outright and its gain is unrun. MemorySanitizer
    was measured working on the Linux arm64 image by the critic, and becomes a
    leg once the cache keys let it build its own objects, which defect 122's
    repair made them do (the C compiler and its flags in every key, closed
    2026-09-28). Coalescing rests on a premise nobody ran.

    **What opening this item owes first**: re-measure each gain on the
    compiler of that day, since 6 to 9 points is a measurement of
    2026-09-28's, and say which condition is met, in the item, before any
    route is built.
