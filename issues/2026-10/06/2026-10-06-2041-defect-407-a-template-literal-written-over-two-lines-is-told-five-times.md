---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **407 — a template literal written over two lines is told five times** | a JavaScript template literal whose backticks fall on two lines gets 5 messages on the base and on lane front alike, since the lexer reads one line at a time and defect 391's repair reads a backtick string that closes on its line | `selfhost/lexer.hero`, `selfhost/backtick_strings.hero` · **class: adjacent**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): one mistake told as several, the shape defect 391 repaired on one line.
