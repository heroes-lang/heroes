---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 7419b76fe2817b751d271487b47b18b1e1df3cf2
github: none
---

- [ ] **267 — a clean record of N fields costs the square of N, its fields pushed through a growing array** | 2,000 to 8,000 fields: 13.5 times, 30.2 billion instructions at 8,000 (batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/recovery/r178/clean_fields_*`), defect 146's pattern | `selfhost/parse/member_lines.hero:82` (`fields.push`) · **class: adjacent**

    **Origin:** batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): the lane's reading: a correct program's cost, the pattern a closed defect already named.

    Repaired at `7b63179e` (the parser's half: `member_lines`' pushes, a `.must()` inside the push and two places no push grows in place) and `7419b76f` (the resolver's half: `resolve/state.keyed_add`, a map of maps whose read shared each module's set), 2026-10-04 (lane b11-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-05, the coordinator**: the resolver's half, Repaired at `7419b76f`, is the commit that completes the item, so the card names it; the parser's half at `7b63179e` stands above.
