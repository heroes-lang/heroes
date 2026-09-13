# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 5**

- [ ] **panel 136** | Part 7 item 6, doctests: ratify DEFERRED AGAIN with the expired clock struck, the reason replaced by §1.7's zero subtraction and the corpus that refutes the wording, the three ancestors corrected, and the documented test block named as the form that returns at M-doc-generator — or rule ENTERS or REFUSED with what each compels | `docs/panel/136-the-item-named-three-ancestors-and-the-two-it-needed-were-elsewhere.md` · design.md §4.18, Part 7 item 6

    **Origin:** 2026-09-13, M-deferral-ledger step 2. **The default the compiler
    runs on while this is open**: no doctests, and design.md carrying the dated
    deferral with its corrections beneath both places the item lives. Five seats,
    no veto on the form and one on a route: all five arrive at wait by four
    different routes, and the two that wrote `adopt-with-condition` meant the
    condition — the reader's seat says *"unconditioned, I refuse the text"*, and
    the historian's condition is a different feature rather than a schedule.
    **The measurement that decided it was found three times independently**: the
    tree's one comment fence holds an `extern` group, so the proposal stops the
    compiler compiling itself on the day it lands. Conservative was recorded
    rather than adopted: the same deferral with the expired clock left in place
    and only a date added. **Recommendation: ratify as adopted.** The expired
    condition is the exact shape this ledger exists to remove, and the sitting
    replaces it with four conditions an instrument in this tree can check.

- [ ] **panel 137** | Part 7 item 8, traits: ratify the three verdicts — traits REFUSED to Part 6 with their falsifier, a structural iteration rule DEFERRED because it contradicts §4.8, and `sort_by` scheduled to the library instead — plus wart 11 rewritten because its stated cause is false | `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md` · design.md Part 6, Part 7 item 8, Part 8 wart 11

    **Origin:** 2026-09-13, M-deferral-ledger step 3. **The default the compiler
    runs on while this is open**: no traits, no extensible `for`, the four
    hand-written sorts standing, and design.md carrying all three verdicts.
    **On route A there was no disagreement at all** — five seats, five noes, for
    five different reasons, one of them a veto from the seat whose mandate is
    cost. **On route B the seats disagreed about what kind of fault it has**, and
    the sitting took the compiler seat's ground over the reader's: it contradicts
    §4.8, an existing written rule, which is a different act from having a
    silence in it. **What the sitting adopts is not a language change at all**:
    `sort_by` joins the six generic higher-order functions the library already
    ships, scheduled at M-generics-library with the engineer's prediction as its
    acceptance test. Conservative was recorded rather than adopted: defer route A
    too, rather than refusing it. **Recommendation: ratify as adopted.** A Part 6
    row carries a falsifier a reader can check; a fifth deferral would carry a
    promise, and this ledger exists to end promises.

- [ ] **panel 138** | Part 7 item 9, variant constructors as values: ratify REFUSED to Part 6 with its falsifier, the row's own text struck because every testable clause of it is false, and `Expr::product` on the existing `::` operator named as the form that returns — or rule ENTERS or DEFERRED with what each compels | `docs/panel/138-the-item-named-a-parser-that-never-existed-and-the-route-was-already-in-the-grammar.md` · design.md Part 6, Part 7 item 9

    **Origin:** 2026-09-13, M-deferral-ledger step 4. **The default the compiler
    runs on while this is open**: no constructor values, the named wrapper and the
    exhaustive-`match` dispatch both legal, and design.md carrying the refusal.
    **Two vetoes engaged and no seat in favour**: the compiler seat on §1.7, since
    a case lowers to a shape and has no C function whose address could be taken,
    and the thread guard is indexed by declaration; the reader on locality, since
    the spelling contradicts a sentence already in § 9 and makes a stored case
    compare as an address. **Every testable clause of the row is false**, which is
    the third ledger item in a row to fail that way, and the sitting strikes the
    text as well as ruling on the feature. Conservative was recorded rather than
    adopted: defer with the `::` route as the return condition. **Recommendation:
    ratify as adopted.** A Part 6 row carries a falsifier a reader can check.

- [ ] **panel 139** | defect 030's repair: ratify the sentence, its descendants in the compiler, and the marking of the shipped counterexample — **and authorise the ONE network call the landing needs**, `heroes measure spec/heroes-spec.md --refresh`, which is the only command in the compiler that reaches out | `docs/panel/139-the-sentence-was-false-and-so-were-four-of-its-neighbours.md` · `spec/heroes-spec.md` § 3 · `docs/work/DEFECTS.md` 030

    **Origin:** 2026-09-13, convened on a defect rather than a proposal. **The
    default the compiler runs on while this is open**: § 3 goes on saying *"No
    aliasing exists anywhere"*, which is false six ways, and the compiler goes on
    printing two diagnostics that quote it. **Why this one is blocked where the
    others were not**: amending the specification trips the staleness detector —
    measured in a copy — and `measure` gives **no verdict against a stale count**,
    so the landing needs `--refresh`. That command sends the document to an
    external service; the document is already public, and the hard stop still asks
    every time. **Everything that does not touch the specification has landed**:
    defect 031 filed, design.md's borrow-checker row corrected on §12's own rule,
    and the sitting recorded. **The wording adopted** merges the two seats that
    each covered what the other missed, and adds *"or free"* after defect 031 was
    measured: *"No aliasing exists among the values this language owns. A `ptr` is
    a copied ADDRESS, wherever it sits: two copies reach one foreign thing, so a
    function taking one without `@` may still change, or free, what C holds. A
    `cstr` copies an address too, and only a group's `record` may hold one."*
    **+73 vendored**, payable by § 4's kinds sentence at −54. **Recommendation:
    ratify, and authorise the refresh.** A reader of the specification is being
    told something untrue about what a copy is, and nothing else on any list is
    costing more than that.

- [ ] **panel 140** | Part 7 item 11, a `raw` module: ratify REFUSED to Part 6 with its falsifier, **Part 9 corrected beneath itself** because its thesis is contradicted by §4.10 in the same document, and the per-declaration confinement the compiler already performs named as the form that returns at M-handle-verdict | `docs/panel/140-the-document-had-already-ruled-against-part-9-and-nobody-told-part-9.md` · design.md Part 6, Part 7 item 11, Part 9

    **Origin:** 2026-09-13, M-deferral-ledger step 5. **The default the compiler
    runs on while this is open**: no `raw` module, `ptr` a first-class type in
    every file, and design.md carrying both the refusal and Part 9's correction.
    **Three vetoes and no seat in favour.** The measurement that ended it is a
    sentence in the same document: §4.10 says *"§4.19's `ptr`/`cstr` sits outside
    the guarantee"* and Part 9 says everything breaking the guarantees goes in
    `raw`, pointers first. **Both of the item's stated preconditions landed
    2026-08-12 and woke nobody**, the fourth such item in this ledger. The
    reader's seat found the sharper objection — `raw.load_u8` would **remove** a
    guarantee, since the same read through `[u8]` aborts — and the critic found
    that the language already implements the principle better than the module
    would, by declaration rather than by import. Conservative was recorded rather
    than adopted: defer with the builtin route as the return condition.
    **Recommendation: ratify as adopted**, and note that the Part 9 correction is
    free, design.md carrying no token budget.

*******************************************************************************
