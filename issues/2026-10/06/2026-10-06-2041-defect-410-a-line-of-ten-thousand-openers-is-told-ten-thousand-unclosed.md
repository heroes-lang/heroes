---
kind: defect
area: parse
milestone: none
filed: 2026-10-06
commit: 5d5a2943aeda36ff8cbf4566e44f21e7f9fcfc78
github: none
---

- [ ] **410 — a line of ten thousand openers is told ten thousand `unclosed_bracket`** | a line of 10,000 opening brackets gets one `unclosed_bracket` per bracket, on the base and on lane front alike | `selfhost/parse/`, the pairing of brackets · **class: improvement**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a pathological input, no program a writer means; at most an improvement.

    Repaired at `5d5a2943` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reproduced on `dad2da47`: 10,000 reports, the full form 152,573,925 bytes. The openers of one line the lexer named open at one place are one report at the first now, saying how many more stand after it on its line, in both arms (`unclosed_runs`, after `parse/unclosed.withdrawn`): 10,000 reports to 1, the full form to 10,290 bytes; eight `check` goldens moved, 11 runs of two reports to one each. Openers on lines of their own, and openers with another place the lexer named openers at between them, keep a report each.
