- [ ] **M-agreed-retention mutation drill** | `records/tagged` excuses a milestone tag placed over a CARRIED defect only where that defect's own item, in `docs/work/DEFECTS.md` as it was at the tag, says `**Carried to the next milestone**` and names the log entry that carried it. In `tag_is_carried`, replace `body = item_region(text: text, number: n)` with `body = text`. **Before running: which of the test's five assertions goes red, and what could a tag then be excused over?** | `tests/harness/suite_records.hero` (`tag_is_carried`, `item_region`, the test *a carried defect is excused at its tag only where its own item says so*)

    **Where to look after answering:** measured 2026-09-28, in a throwaway
    clone: the fifth goes red and no other, 179 tests and 1 failed. That
    assertion puts the marker under defect 130 and asks about 129; reading
    the whole list, the words anywhere in it excuse every defect in it, so
    one carried item would shelter all the others. The same clone, with a
    real tag over 129 alone, read `records` 24 and 0, and with the marker
    removed from 129's body `records/tagged` failed naming it.

    **The question to carry away.** `.claude/rules/diagnostics-and-goldens.md`
    § An instrument watches the world asks *what would still pass if the thing
    under test were wrong?* Say which clause of `tag_is_carried` answers that
    question for a waiver, and why the row's defect numbers alone would not.
