---
kind: learn
area: none
milestone: M-struct-passing
filed: 2026-08-03
commit: none
github: none
---

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, fallibles lowered | **`f()?` must not call f twice — find the guarantee.** Where does the subject go before the tag is read, and what would `hold` NOT existing break in `next_token()?`

    **Where to look:** ir_lower.hero::hold
    **Why it matters:** one synthetic slot is the difference between reading a value twice and running its side effects twice
