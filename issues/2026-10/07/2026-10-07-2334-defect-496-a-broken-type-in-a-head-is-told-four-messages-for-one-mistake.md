---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: be205ad0ae9e8a1172891ee2378e3626298b3929
github: none
---

- [ ] **496 — a broken type in a head is told four messages for one mistake** | defect 456's shapes: the broken-type head is told 4 messages; with two such types, the second one's `expected_function_type` is told twice at one place (8:9); the 456 cases pin it as it stands (lane b14-text) | `selfhost/parse/brace_habit.hero` and the head's recovery · defect 456 · **class: adjacent**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told four times.

    Repaired at `be205ad0`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The line below a parameter's type broken after its `(` is the head's own and no spilled body: a spill's refused word stands in no bracket opened after the head's list, and its closer ends its line (`spill_reading.hero`); the broken-type head told 4 messages to 1, two such types 6 to 2 (the second's twice at 8:9 gone), defect 456's four cases moved and read, the new case 13 to 4.
