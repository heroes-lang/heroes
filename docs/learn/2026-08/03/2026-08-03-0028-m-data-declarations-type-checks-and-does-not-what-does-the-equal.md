---
kind: learn
area: none
milestone: none
filed: 2026-08-03
commit: none
github: none
---

- [ ] **M-data-declarations** | `t == .plus` type-checks and `t = .plus` does not. What does the equality operator hand to the case, and why does every *other* binary operator do the same thing?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/exprs.rs (the Binary branch, `contextual`)
    **Why it matters:** one rule where two would have been the obvious shape
