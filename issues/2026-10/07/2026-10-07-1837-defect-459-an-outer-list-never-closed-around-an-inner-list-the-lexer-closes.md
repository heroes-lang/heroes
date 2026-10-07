---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **459 — an outer list never closed around an inner list the lexer closes elsewhere is told twice for one `]]` edit** | the never-closed outer `[` keeps its own report beside the binding's message naming the inner `[`, two messages for one edit, deliberate under defect 307's rule; merging them needs the outer opener's report carried to the binding's message across reports being cut (lane b14-parse, 313's six shapes on the base, none spurious) | `selfhost/parse/unclosed.hero`, `selfhost/closers.hero` · defects 313 and 307 · **class: adjacent**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, true each.
