---
kind: feature
area: design
milestone: none
filed: 2026-08-12
commit: d6efbfa26892c77529607439b472c1e7a7aeb1e7
github: none
---

- [x] Covered | **CLOSED — verified 2026-08-12 while splitting the queue.** panel 022 | A new diagnostic class M-value-aggregates walks into: **no infinite-size rule exists**. `record Node { child: Node }` reaches clang as `error: field has incomplete type` — design.md §8's wart 13 happening to a *program* error. CLAUDE.md §4 makes a diagnostic class its own panel trigger | crates/heroes/src/types/decls.rs · docs/panel/022 § Watch list | the class is invisible until aggregates are emitted, which is now
