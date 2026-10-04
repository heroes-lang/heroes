---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **343 — three sites still push a diagnostic through a copy of the whole list, so N line ends or N bare function types cost the square of N** | `c.diagnostics @ c.diagnostics.push(d)` in `line_end.refused`, `type.bare_function` and `type.named_parameter`: N line ends before an operator went 9.0 times the instructions and N bare function types 5.5 times for four times the input (lane b11-parse, 2026-10-05, the lane's report) | `selfhost/parse/line_end.hero` and `selfhost/parse/type.hero` (the three pushes) · `cursor.push_diagnostic`, which grows in place · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 267's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a broken program whose report costs the square of its mistakes; the messages are right.
