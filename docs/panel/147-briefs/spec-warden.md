# Panel 147 — spec-warden

Read `00-shared.md` first. This brief adds your numbers.

You judge the **indicator** (design.md §1.2's cost formula, §1.6's spec budget)
and **Principle 0's burden of proof**. You have a veto on budget breach.

## The numbers, measured 2026-09-14 with `heroes measure`

| | vendored (`cl100k_base`, the `maximum` row) |
|---|---|
| `spec/heroes-spec.md` today | **5863** |
| Route A — handle names its releaser | **5943** (**+80**) |
| Route B — `cleanup` statement | **5971** (**+108**) |
| Route C — refuse to Part 6 | **5863** (**+0**) |

**The real count is 7806** (`claude-opus-5`, the binding number), against a
ceiling of **8192**. Headroom is **386**, but the FFI floor mortgages 60 of it
(panel 030 R3), so **326 is what is actually free**.

**The real count for the two DRAFTS could not be taken, and this is stated
rather than papered over.** `heroes measure <draft> --refresh` is refused by
design: *"`--refresh` refreshes the record for `spec/heroes-spec.md` and
`CLAUDE.md`, and \<draft\> has none — no ceiling judges it and no check keeps it
honest"* (panel 123 R5). So the two route costs above are **vendored** numbers.
The current document's own vendored-to-real ratio is 7806/5863 = **1.331**,
which would put Route A near **+106 real** and Route B near **+144 real** — but
that is an **estimate derived from a ratio**, not a measurement, and your
verdict on the budget limb should say so and be provisional on it.

The drafts are at:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/route-a.md`
and `.../route-b.md`. Read the actual diffs rather than trusting my summary of
what they add — one adds a prose sentence and a grammar production, the other
adds a paragraph and a statement production.

## What only you are asked

1. **Principle 0.** A form enters v1 if the compiler NEEDS it (the §1.0 closure
   list) **or** it provably serves the thesis (a measured design.md Part 11
   effect, or a measured argument). **Which limb does each route stand on, and
   is that limb actually satisfied?** The compiler-need limb has a cheap test
   here: `selfhost/` is 60,312 lines of Heroes by the people who know this
   language best — **count its acquire-and-release pairs**. The census says one.
   Decide what that means for the closure-list limb, and say it plainly.
2. **The payment rule** (§1.6): an addition owes a **named removal** or a
   **registered prediction naming an instrument that exists**. For whichever
   route you find admissible, say which it is and name the removal or the
   instrument. If neither exists, that is a veto and you should cast it.
3. **§1.7's subtraction test**: what does each route REMOVE? Zero is a common
   and damning answer here; say the number.
4. **The refusal is held to a feature's standard** (CLAUDE.md §12): if you land
   on Route C, the Part 6 row you would write must name the **program or
   compiler fact that would make it wrong**. Draft that falsifier — a row
   without one is not admissible, and this project has spent a whole milestone
   (M-deferral-ledger) discovering that nine such reasons had expired.

## A caution this sitting has earned

M-deferral-ledger found that **nine times in a row** an item's stated reason was
measured false or expired before its verdict could be written. The census that
convened THIS sitting falsified two thirds of the premise the milestone was
scheduled on. Check the premises in `00-shared.md` that your verdict rests on,
rather than inheriting them — and if one is wrong, that is the finding.
