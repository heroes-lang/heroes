- [ ] **280 — the defect checks read a number of three digits, so the thousandth defect has no name they accept** | `is_defect_name` asks three digits and then `-` (`tests/harness/suite_records.hero:4785`), and `defect_number` reads three digits off an item's line (`:2178`), so `1000-<slug>.md` would be refused by `records/defects` at the first filing past 999; the highest number issued is 277 (read by the coordinator, 2026-10-04) | `tests/harness/suite_records.hero` (`is_defect_name`, `defect_number`, the test at `:5622`) · `docs/work/DEFECTS.md`'s *three digits* · **class: improvement**

    **Origin:** lane b8-defects, 2026-10-04 (its reply's *found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a premise about the world, written loud (the check refuses the name rather than misreading it), 722 numbers away; no program moves.

    Repaired at `6b361fc3`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.
