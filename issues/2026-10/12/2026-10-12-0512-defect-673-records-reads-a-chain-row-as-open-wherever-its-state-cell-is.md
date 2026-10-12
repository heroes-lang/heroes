---
kind: defect
area: harness
milestone: none
filed: 2026-10-12
commit: self
github: https://github.com/heroes-lang/heroes/issues/2020
---

- [x] **673 — `records` reads a chain row as open wherever its state cell is not `done YYYY-MM-DD`, and row 84 said `closed 2026-10-10`** | `tests/harness/suite_records.hero` `chain_states` set a milestone's openness to `!contains(row, "| done ")`, so every state word but `done` read as open, the quiet direction; and `docs/ROADMAP.md` § The chain row 84, M-inferred-cell, read `closed 2026-10-10` since its close at `d32c19a1`, where the other 62 closed rows read `done YYYY-MM-DD`, so `records/homes` read a closed milestone as open and would have passed an open task filed under it (none names it today); `site/src/lib/chain.ts`, the chain's other reader, took the same row for `scheduled` | `tests/harness/suite_records.hero` (`chain_states`, its callers `carded` and `homed`) · `docs/ROADMAP.md` § The chain, row 84 · `site/src/lib/chain.ts` · **class: adjacent**

    **Origin:** found 2026-10-12 by the lane building the transcription program's `prepare` (`lane-issue-push`), whose own chain reader, `tests/harness/issue_plan.hero`'s `chain_of`, read the cell as closed where `records` read it as open; confirmed by the coordinator the same day, reading `chain_states` and the row.

    **Class: adjacent**, 2026-10-12: real and found beside the work, and no wrong verdict stands today, since no open item names M-inferred-cell; it would have become a false pass the day one did, and a false state on the site's chain table already.

## The repair

Row 84's state cell is `done 2026-10-10`, the shape of every other closed row,
and the chain now holds 63 `done`, one `**OPEN**` and 20 `scheduled`, 84 rows
(counted on 2026-10-12). `chain_states` reads a state cell as one of those three
shapes, `done YYYY-MM-DD` closing a row (its day read as a day of the
calendar), `**OPEN**` and `scheduled` leaving it open, the three the table held
and the three `site/src/lib/chain.ts` names; any other is refused by its row,
`records/cards` and `records/homes` failing with the milestone and the word it
holds, so a new word fails loudly instead of reading as open. The case is
`fixedbugs: a chain row whose state is none of the chain's three shapes is
refused by its row, never read as open (defect 673)` in
`tests/harness/suite_records.hero`, over `closed 2026-10-10`, a bare `done`, a
day cut short, a day no calendar has, `OPEN` and `Scheduled` written otherwise,
and an empty cell. `issue_plan.chain_of`, the transcription's reader, which lands in the commit
after this one (M-issue-files step 14), takes the same three shapes and refuses
any other.

**Closed 2026-10-12** in the commit that writes this file.
