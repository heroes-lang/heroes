# M-deferral-ledger — every Part 7 item gets a date


**Scheduled by author instruction 2026-09-03.** Part 7's preamble defers its
items *"until the Principle 0 closure list compiles itself"*; that was
2026-08-18, and on 2026-09-03 the items with no milestone and no verdict since
were **seven**: 5 `alias` (`design.md:2457`), 6 doctests (`:2467`), 8 traits
(`:2469`), 9 variant constructors as values (`:2472`), 11 the `raw` module
(`:2502`, Part 9), 14 declaration visibility (`:2562`, costed at panel 033 and
left to *"a count"*), 16 symmetric variant syntax (`:2585`, *"v2"*). Two more sat
on the panel watch list with no home at all until it was retired on 2026-09-04,
and this milestone is the home they got: raw string literals (Part 8 wart 15,
`design.md:2673` — panel 008's own implementation found that `"C:\temp"` cannot be
made loud, because `\t` is legal) and printing without a
trailing newline (wart 16). Items 1, 7 and 12 have their own rows above; item
10's C-width vocabulary and conditional compilation are questions (v) and (vi)
of M-core-packages' opening sitting and stay there; item 13 is
M-isolated-threads; item 15 is M-qbe-backend.

**What it delivers is a ledger**: each item, in Part 7's order, receives one of
three verdicts with a date — it enters (its own milestone, since a form lands in
every tool that reads the language, CLAUDE.md §9), it is refused (a Part 6 row
with its falsifier, CLAUDE.md §12), or it is deferred **again, with a return
condition** stated as a falsifiable claim, the way item 10's row already does.
A promise without a date is the one shape this milestone exists to end.

**The steps are the sittings**, one per item, full five seats where the item has
surface and the soundness lane where it has none. Nothing lands in the compiler
here except what a sitting adopts; the ledger itself is Part 7's own text,
amended by each verdict's commit. Why before the packages: the same reason as
M-closures-verdict — `alias` (`Handler = alias (function(Request) -> Response)`),
`private` (which functions are a package's API) and symmetric variants (the one
silence Part 11's first-try measurement is told to expect) all change how a
package is written.

*******************************************************************************
**OPEN: 2**

- [ ] **M-deferral-ledger** | the list the ledger opens with | `design.md` Part 7, Part 8 warts 15–16, Part 9 · `docs/panel/008-escape-sequences.md`

    **Origin:** author instruction 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **Seven Part 7 items and two watch-list entries have no milestone and no
    verdict since the fixpoint made Part 7 admissible (2026-08-18).** Measured
    2026-09-03 against design.md: item 5 `alias` (`:2457`), 6 doctests
    (`:2467`), 8 traits (`:2469`), 9 variant constructors as values (`:2472`),
    11 `raw` (`:2502`, Part 9), 14 visibility (`:2562`), 16 symmetric variant
    syntax (`:2585`); raw string literals (Part 8 wart 15, `design.md:2673`;
    panel 008's finding, on the panel watch list until it was retired
    2026-09-04) and printing without a trailing newline (wart 16, `:2668`).

    Each gets one sitting, in this order, and one of three dated verdicts:
    enters, refused with its falsifier (CLAUDE.md §12), or deferred with a
    return condition written as a falsifiable claim. Not on this list, because
    they have homes: items 1 and 12 (**M-closures-verdict, both refused
    2026-09-08** — 1 to Part 6, 12 to the unplaced paragraph, so neither is on
    Part 7 for this ledger to date), 7
    (M-interpolation-verdict), 10 and conditional compilation
    (M-core-packages' sitting, questions v and vi), 13 (M-isolated-threads), 15
    (M-qbe-backend).

    **Why it matters:** a deferral with no date is a promise, and Part 7's
    preamble says it is not one.

    **Re-verified 2026-09-10: STILL OPEN, and every line number in the list has
    moved.** design.md is 3612 lines now, so Part 7's items read: 5 `alias`
    **:2649**, 6 doctests **:2659**, 8 traits **:2672**, 9 variant constructors
    **:2675**, 11 the `raw` module **:2705**, 14 visibility **:2790**, 16 symmetric
    variants **:2813**; wart 15 is **:2893** and wart 16 **:2896**. None has gained a
    verdict or a milestone, so the ledger's nine are unchanged in substance. **One
    body sentence is behind the tree**: it lists item 7 as homed at
    M-interpolation-verdict, and `design.md:2660` now reads *"ENTERS, ruled
    2026-09-09"* with `m-interpolated-strings` a placed tag.

- [ ] **M-deferral-ledger** | the ledger widens from Part 7 to Part 8's warts, and coverage gets the answer it already has | `design.md` Part 8 warts 5, 8, 11 · `docs/panel/034` · `selfhost/mutate/`

    **Origin:** author decision 2026-09-10, § What production-ready means.

    **Why the same milestone and not another**: the ledger's verdict vocabulary
    already fits — enters, refused with a falsifier, or deferred with a return
    condition — and its own list already reaches into Part 8 for warts **15** and
    **16**. Three more warts have no owner at all: **5**, errors are codes and
    strings rather than types; **8**, `+` on `str` is quadratic and so is the
    `[str]` built to avoid it; **11**, no user-extensible iteration, which is
    Part 7 item 8's shadow. **Wart 5 was already ruled to STAY a wart** (panel 034,
    *"the answer is a `constant`, not a feature"*), which is the ledger's third
    verdict and shows the widening costs no new machinery.

    **And coverage, which a production reader will ask for.** Line coverage does
    not exist and `heroes mutate` is a stronger instrument than it — a mutant the
    compiler fails to catch is a hole in the language, where a covered line is only
    a line that ran. **The answer belongs in Part 6's shape**, a refusal naming the
    program that would make it wrong, rather than in a silence a team reads as an
    omission. Its falsifier is available: a defect class `mutate` cannot reach
    because no operator produces it, which a covered-line report would have
    named.

*******************************************************************************
