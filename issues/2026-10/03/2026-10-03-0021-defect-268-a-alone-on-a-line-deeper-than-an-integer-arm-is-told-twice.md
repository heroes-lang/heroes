---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **268 — a `-` alone on a line deeper than an integer arm is told twice, the second an `unexpected_block` true only if the `-` joins the next line** | `k = match n` over `2 => "two"`, a line `-` one level deeper, then `1 => "one"` and `_ => "many"`: `check` exit 1, `continuation_outside_brackets` at 5:13, then `unexpected_block` at 5:1 (batch 8's round compiler at `1eb854c3`, 2026-10-04, `<scratchpad>/batch8/recovery/shapes/s182_minus_over_int.hero`) | `selfhost/parse/` (the stray operator's margin, beside defect 182's repair) · **class: adjacent**

    **Origin:** batch 8's recovery lane, 2026-10-03 (its report's *Found beside*); reproduced by the coordinator, 2026-10-04.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, the stray `-`.
