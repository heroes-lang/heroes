---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **311 — a `fn` among a record's fields is told once since defect 200's repair, and the message no longer says to take the record as a parameter** | `fn` written among a record's fields, the record twin of defect 200: two messages before, one since `7831aec7`, and that one has lost *taking the record as a parameter*, the route the old second message gave (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/member_lines.hero` · defect 200's repair, `7831aec7` · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`); the lane's own repair's, reported.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message that lost a route it carried; batch 9's own change, read at its gate.
