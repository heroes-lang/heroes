---
kind: learn
area: none
milestone: none
filed: 2026-08-03
commit: none
github: none
---

- [ ] **panel 021** | Reference counting leaks cycles, and Nim needed a whole second collector (ORC) for exactly this. Safe for `str`; false the first time a refcounted type can cycle, which is M-value-aggregates's recursive `variant`

    **Where to look:** docs/panel/021 § Watch list · examples/gallery/11-trees.hero
    **Why it matters:** the milestone that makes it false is the next one
