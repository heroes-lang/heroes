---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **351 — two stray `-` lines in a row under an integer arm are still told three messages** | a `match` whose integer arm is followed by two lines holding a `-` alone, each deeper: `check` tells three messages for them where defect 268's repair tells one for a single such line (lane b11-parse, 2026-10-05, the lane's report) | `selfhost/sign_above.hero` · defect 268 · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 268's repair (its final report, *Found beside*), a shape beside the item's own.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second and third message for what may be one mistake; the lane reads it apart from 268's cause.
