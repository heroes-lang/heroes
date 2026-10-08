---
kind: defect
area: examples
milestone: none
filed: 2026-10-07
commit: 16f27d696a827b82c8e04b22e7b9ee68288a00f1
github: none
---

- [ ] **466 — `examples/calculator/whole.hero`'s `apply` doc comment sits cut off from `apply`** | lines 27 to 31: the doc comment and its `## Generic library` heading above `## Error codes`; two of four blind sessions moved it unasked (lane b14-m212's measurement 040) | `examples/calculator/whole.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-m212's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): cosmetic, outward-facing (`examples/`).

    Repaired at `16f27d69`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests (318 passed); the net is owed at the batch's close. The `## Error codes` block, inserted by `b70aa5a6` (2026-08-13) between `apply`'s doc comment and `apply`, stands above `## Generic library`, so the heading, the comment and the function stand together as design.md's appendix has them; a net test reads the 120 modules of `examples/` for a comment running straight into a `##` heading (red on the base, this one), and the one emission the move touched, `examples-calculator-whole.c`, re-blessed: 12 `#line`s, 5 lines up each, no other byte. A push carrying it publishes the site.
