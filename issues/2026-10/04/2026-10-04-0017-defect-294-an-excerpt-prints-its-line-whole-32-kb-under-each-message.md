---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **294 — an excerpt prints its line whole, 32 KB under each message for a line of 4,000 refused characters** | a line of 4,000 refused characters is printed whole under each message, eight of them since defect 282, about 32 KB each (lane b9-notext, 2026-10-04) | `selfhost/diag_render.hero` (the excerpt) · **class: improvement**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 5).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a message larger than what it names; no program refused or wrong.
