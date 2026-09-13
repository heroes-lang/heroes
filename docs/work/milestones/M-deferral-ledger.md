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

    **Step 1, 2026-09-13: item 5 sat (panel 135,
    `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md`)
    — DEFERRED AGAIN with a return condition, its three false claims struck
    beneath the original, and its spelling corrected to `alias Env = …`**; the
    reasoning paragraph at the top of this file still writes `Handler = alias …`,
    which was the item's own spelling until that day and is left as written. Line
    numbers re-measured this session, before the amendment landed: 5 `alias`
    **:2705**, 6 doctests **:2715**, 8 traits **:2728**, 9 variant constructors
    **:2731**, 11 the `raw` module **:2761**, 14 visibility **:2846**, 16 symmetric
    variants **:2869**; wart 15 **:2965**, wart 16 **:2968** — every one moved again
    since 2026-09-10, and item 5's amendment moves every number after it once
    more, which is why the list keys by item and not by line. Found beside the
    sitting and filed: **defect 029**, a swapped opaque handle (`sqlite3_step(db)`)
    that builds at zero diagnostics and exits 139 — the *distinct types* door item
    5 keeps open, owed a sitting of its own and not this milestone's to land.
    Eight of the ledger's nine remain; item 6, doctests, is next.

    **Step 2, 2026-09-13: item 6 sat (panel 136,
    `docs/panel/136-the-item-named-three-ancestors-and-the-two-it-needed-were-elsewhere.md`)
    — DEFERRED AGAIN, and the first of the ledger's items whose stated condition
    had already EXPIRED**: *"once the `test` mechanism is proven"* against 641
    `test` blocks in `selfhost/`, 545 in `examples/` and the compiler's own tests
    being one of the three suites. The clock is struck rather than re-wound, and
    that is the shape this milestone was scheduled to find: **a condition that
    expires without waking anybody is the promise without a date**, and the ledger
    should expect more of them. The reason is replaced by §1.7's zero subtraction
    and by the corpus, and the item's three named ancestors are corrected — Go and
    D, unnamed in it, obtain the benefit with no second test mechanism, so **the
    form that returns is the documented test block and its home is
    M-doc-generator**. Found beside the sitting and filed at the milestone that
    owns it: a failing `assert` names no file and no line, a fifth surface
    M-panic-location's own census did not name. Seven of the ledger's nine remain;
    item 8, traits, is next.

    **Step 3, 2026-09-13: item 8 sat WITH Part 8 wart 11 (panel 137,
    `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md`)
    — traits REFUSED to Part 6, a structural `for` DEFERRED, wart 11 REWRITTEN,
    and what was adopted is not a language change at all.** Two items in one
    sitting because the wart's own text made them one question, and ruling either
    alone would have left the other standing as a promise. **The pattern the
    ledger is now finding twice over**: an item's stated reason is measured false
    before its verdict is written — item 6's clock had expired, and item 8's two
    clauses are both wrong, *"instance resolution is expensive"* merging three
    costs that are not one and *"no traits means no extensible iteration"*
    falsified six times. **The third route the sitting adopted came from the
    corpus rather than from either proposal**: `sort_by` is a library function,
    filed at M-core-packages, and the language needs nothing. Two things found
    beside it and filed where they belong: **defect 030**, the specification's
    promise about copies being false through a `ptr` field, measured on a shipped
    example; and the handle a `break` leaks, filed at M-cleanup-verdict, whose
    sitting owns release on every path. Six of the ledger's nine remain; item 9,
    variant constructors as values, is next.

    **Step 4, 2026-09-13: item 9 sat (panel 138,
    `docs/panel/138-the-item-named-a-parser-that-never-existed-and-the-route-was-already-in-the-grammar.md`)
    — REFUSED to Part 6, and the FIRST sitting of this ledger to run a
    completeness critic over the five seats.** The pattern is now three for three:
    an item's stated reason is measured false before its verdict is written. Here
    **every testable clause** of the row was — it names a parser this repository
    has never had, the pair is one example program written twice, the names that
    differ are three or four rather than two, and the unification it promises would
    not compile even with the feature granted, because a function type cannot carry
    `@`. **The critic earned its place on its first run**: it found `Expr::product`
    on the existing `::` operator, a route no seat had named and which closes every
    silence by construction; it settled an engineer-against-warden corpus count by
    reading the disputed file; and it caught that all five seats had judged the
    feature while none had judged the row. **A method the ledger now carries
    forward**: strike the row's false text under it with the date, as panel 137 did
    for traits, rather than ruling only on the form. Five of the ledger's nine
    remain; item 11, the `raw` module, is next.

    **Step 5, 2026-09-13: item 11 sat (panel 140,
    `docs/panel/140-the-document-had-already-ruled-against-part-9-and-nobody-told-part-9.md`)
    — REFUSED to Part 6, and Part 9 itself corrected beneath, because the
    correction was owed at the Part rather than at the item.** Four for four now:
    every item this ledger has opened had a stated reason that was false or
    expired before its verdict was written. This one is the sharpest of the four —
    **the document had already ruled against Part 9's premise, in the same
    document**, §4.10 naming `ptr` and `cstr` as sitting outside the guarantee
    while Part 9 puts pointers first among what `raw` would bound. And its two
    preconditions landed on 2026-08-12 and woke nobody, the second expired clock
    of this milestone. **A finding about the ledger's own method**: the critic
    noted that two of the coordinator's four framing facts were unverifiable from
    the sitting's artifacts, because the briefs go out as prompts and are never
    written where a later reader can check what the seats were told. Four of the
    ledger's nine remain; item 14, declaration visibility, is next.

    **Step 6, 2026-09-13: item 14 sat (panel 141,
    `docs/panel/141-the-premise-expired-the-ancestry-was-for-another-form-and-the-count-cannot-decide.md`)
    — REFUSED to Part 6, and the milestone's thesis arrives in its purest form.**
    This item did not merely carry a stale reason: it carried **an instruction to
    wait for a measurement that cannot decide**. *"Direction unresolved and left to
    a count"*, and the count now reads 49.5%, 50% and 50% by three methods, against
    a 42% that was another method on another tree in another language. **A ratio
    hovering at half is not a tie-breaker**, and the sitting closed the question by
    measuring that rather than passing it on — which is the one move a ledger of
    deferrals has to be able to make. Its hinge had also expired two weeks after
    being written (M-separate-compilation, 2026-08-26), and its ancestry turned out
    to be for a form none of the six cited languages uses. **Two corrections to the
    coordinator are on the record**: a brief asserted a sentence the specification
    does not contain, and a brief's line citation was invalidated by this same
    session's own edits to the document above it — CL-037's shape, with this
    sitting as its example. Three of the ledger's nine remain; item 16, symmetric
    variant syntax, is next.

    **Step 7, 2026-09-13: item 16 sat (panel 142,
    `docs/panel/142-there-is-no-asymmetry-to-restore-because-there-is-no-destructuring-anywhere.md`)
    — REFUSED to Part 6, and with it the LAST Part 7 item this ledger owed a
    verdict.** Seven Part 7 items entered this milestone and seven now carry dated
    verdicts: 5 `alias` deferred with a condition, 6 doctests deferred with an
    expired clock struck, and 8, 9, 11, 14 and 16 refused to Part 6 with
    falsifiers. **Five of the seven carried a stated reason that was false or
    expired**, and item 16 is the sharpest: its premise was false **one level up**,
    since a record pattern is refused by name and nothing in the language
    destructures at all, so the symmetric proposal would have created the asymmetry
    it claimed to remove. **"v2" is struck as a verdict this ledger can carry**,
    which is the milestone's own thesis applied to the last word standing.
    **A third correction to the coordinator is on the record**: a construction
    count of 1457 was a regex counting every dotted call with a labelled first
    argument, and the true figure from the compiler's own AST is about 626 — the
    match-arm count of 994 is exact. What remains of the ledger is Part 8: warts
    15 and 16, then the widening item's warts 5, 8 and 11 with coverage.

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
