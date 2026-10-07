---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 5a019922a881fa6b569fc5504117cb82de7eeff9
github: none
---

- [ ] **294 — an excerpt prints its line whole, 32 KB under each message for a line of 4,000 refused characters** | a line of 4,000 refused characters is printed whole under each message, eight of them since defect 282, about 32 KB each (lane b9-notext, 2026-10-04) | `selfhost/diag_render.hero` (the excerpt) · **class: improvement**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 5).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a message larger than what it names; no program refused or wrong.

    Repaired at `5a019922`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A line, or the stretch of it before the span, wider than 120 columns is cut to a window of 120 around the span, `…` on each side the line goes on, the caret under it (`selfhost/excerpt_window.hero`); a line within 120 is printed as before, no golden of `full` or `unsupported` moving: 4,000 alternating control characters 257,579 bytes to 2,483, 4,000 trailing tabs 8,477,148 to 951,888.
