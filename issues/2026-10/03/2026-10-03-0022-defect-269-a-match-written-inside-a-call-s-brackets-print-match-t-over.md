---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **269 — a `match` written inside a call's brackets, `print(match t` over `.a => 1)`, is told twice, the first saying its arms are not indented below it** | `function f(t: i64)` over `print(match t` and `.a => 1)`: `check` exit 1, `missing_match_arms` at 3:9, *a `match` needs its arms indented one level below it, found `.`*, then `expected_args_close` at 3:12, *found `=>`*, on the trunk's compiler and on batch 8's round compiler at `1eb854c3` alike (2026-10-04, `<scratchpad>/batch8/recovery/shapes/s177_closed_inside.hero`); panel 180 records that `if` and `match` never stand in brackets, and a `grep` of `docs/panel/180-*` for this shape finds no ruling on its messages | `selfhost/parse/` (a `match` opened inside brackets) · panel 180 · **class: adjacent**

    **Origin:** batch 8's recovery lane, 2026-10-03, as a question (its report's *Found beside*: panel 180's shape 74, *sanctioned or not*); run on both compilers by the coordinator, 2026-10-04.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, and a first that names the arms' margin to an author who indented them, where the mistake is the bracket.
