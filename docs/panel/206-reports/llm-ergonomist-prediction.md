# Panel 206, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator at 09:23 on 2026-10-10 (`date`), before any of the
sessions starts. Folders `<scratchpad>/readings-206/<label>`, outside the
repository and outside any git tree (the author's exception of 2026-10-09;
the budget 3.69 USD within the night's 30). The spec in every folder is the
spec-warden's base (the frozen spec with panels 204 and 205's sentences,
`206-spec-warden/drafts/base.md`); the arm `n2` adds F7e to § 7: *An abort
that literals and written constants alone compute is a compile error.* The
labels' mapping, never in a folder:

- **n1-a, n1-b** (base) and **n2-a, n2-b** (base and F7e): predict six
  programs: `p1` `y: u8 = 200 + 100`, `p2` `repeat("-", 0 - 1)`, `p3` a
  constant `BIG: u8` of `200 + 100`, `p4` `x: u8 = 255 + 1 - 1`, `p5` `x: u8
  = 2 - 3 + 5`, `p6` `y: u8 = 300 - 100`. The round: `p1` to `p5` build and
  abort 134, `p6` refused `int_out_of_range`.
- **w-a to w-f** (base): write a constant `ALL_ONES: u64` with every bit set
  and `LOW_BYTE: u8` with its 8 bits set (the spec-warden's P3: is literal
  arithmetic that cannot fit, `0 - 1` at an unsigned width, a plausible
  first try?).

**What I expect**: n1, both readers predict `p1` aborting (§ 7) and at least
one predicts `p5` printing 4 (the final value); n2, both predict `p1` to `p5`
refused; w, at least 1 of 6 writes `0 - 1` or another literal arithmetic that
cannot fit (the spec-warden's P3, *at least 1 of 4*), the rest the decimal or
hex literal. **What it would falsify**: in w, 0 of 6 writing such
arithmetic, the class then measured implausible at this size.
