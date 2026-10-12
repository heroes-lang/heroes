---
kind: task
area: runtime
milestone: none
filed: 2026-08-10
commit: ec5558bef7a0e7934ec69fce6dab1b6d45dcb362
github: https://github.com/heroes-lang/heroes/issues/160
---

- [x] Covered | author draft 2026-08-10 | **PARTLY CLOSED, verified 2026-08-12 — invariant 2 HOLDS (no `static inline` in the header; the refcount entry points are a separate TU). Invariant 1 is false to the letter: design.md says *one* `malloc`, there are four. What holds is the rule that matters — generated C allocates **zero** times, measured over three programs. Queued: route the four through one wrapper, which is what Part 7.13's own reason asks for.** The two Part 7.13 invariants — allocation through a **single point**, refcount ops behind a **narrow never-inlined boundary** — are recorded in design.md §4.20 but **not** in CLAUDE.md §7, where the generated-C rules that get enforced live. Both are true today (verified: one `malloc`, one `free`, both in the `str` primitives; `incref`/`decref` in a separately compiled TU). Decide whether they become §7 rules with M5c's array and map allocation, which is the first thing that can break the first one | CLAUDE.md §7 · design.md §4.20, Part 7.13 · runtime/runtime.c | an invariant nothing checks is a comment, and this one is checkable by grep

## What was done

Verified 2026-08-12: invariant 2 holds, no `static inline` in the header and the refcount entry points in a separate TU; invariant 1 is false to the letter, four `malloc` calls where design.md said one, while generated C allocates zero times, measured over three programs. Routing the four through one wrapper was queued, as Part 7.13's own reason asks. Landed at `ec5558be`, "Panel 035: thirteen silences closed, and the spec got smaller".
