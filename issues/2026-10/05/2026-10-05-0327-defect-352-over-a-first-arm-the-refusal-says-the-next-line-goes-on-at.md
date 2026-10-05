---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 7c6f214498449954c2aa328dd5f476ad9ead6a1a
github: none
---

- [ ] **352 — over a first arm, the refusal says the next line goes on *at the same margin*, naming the level after the jump cap rather than the margin as written** | a continuation refused over a `match`'s first arm: the message's *at the same margin* names the level the parser capped the jump at, not the margin the line was written at; the base says the same (lane b11-parse, 2026-10-05, the lane's report) | the parser's continuation message · defect 268 · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 268's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `7c6f2144`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
