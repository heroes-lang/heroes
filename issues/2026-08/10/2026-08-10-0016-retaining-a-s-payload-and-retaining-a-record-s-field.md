---
kind: decision
area: none
milestone: none
filed: 2026-08-10
commit: 0281a179946792dc4113a61d46140b6e599c340c
github: none
---

2026-08-10 | Retaining a `T?`'s payload and retaining a record's field are **one question with one answer** (`perfn::reference_line`), and having had two is what produced a wrong argument count and then a discarded `const`: the second implementation reached for the descriptor's `copy(dst, src)`, which has no retain-in-place form. Neither diagnostic was the defect; the defect was the duplication, which `ir/layout.rs` had already priced | a repair that removes an answer rather than adding a cast | §4.20 | — |
