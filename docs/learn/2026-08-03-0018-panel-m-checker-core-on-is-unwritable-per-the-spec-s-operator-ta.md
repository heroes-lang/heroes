- [ ] **panel? M-checker-core** | `+` on `str` is unwritable per the spec's operator table, while §4.20 gives the runtime string concatenation and the appendix writes `"…" + name`. Found by the judge that had only the spec — it restructured a whole program to avoid a construct it could not find

    **Where to look:** docs/panel/015-* § Watch list, design.md §4.20
    **Why it matters:** the same class as the missing `\"` (panel 008): the language is INCOMPLETE without it, and nothing says so
