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
**OPEN: 1**

- [ ] **panel 121** | ratify the shape of string interpolation: the gate, the spelling, the ceiling at 4253 rather than the 4224 already confirmed, and a veto the author's own decision overrides | `docs/panel/121-the-brace-was-already-taken.md` § Resolution

    **Origin:** the sitting of 2026-09-08, M-interpolation-verdict, five seats.
    The author had already decided the form ENTERS and confirmed a ceiling of
    4224; the sitting was asked the shape and returned three things that change
    what was confirmed.

    **The recommendation is R1 to R10 as adopted**, and three items in it are
    the author's to overturn rather than mine:

    **(i) R7 IS VOID AS ARITHMETIC, and so is everything that argued about it.**
    Superseded 2026-09-09 by `docs/measurements/023-the-instrument-was-not-the-readers.md`:
    the binding instrument was `cl100k_base`, which is OpenAI's, and it
    undercounts this document by **27.5%**. The spec is **5094** real tokens,
    not 3995, so it was **998 over** the hard 4096 rather than 40 under it, and
    the whole ladder of numbers this item was written to settle — the 4224 the
    author confirmed, the 4237 recommended, R7's 4253 — is arithmetic on the
    wrong scale. **The ceiling is 6144 tokens** by author decision 2026-09-09.
    What survives is the RANKING, re-measured on the real instrument: the
    adopted `f"…"` clause is **+151** and the `\{e}` spelling **+126**, so the
    cheaper one is still cheaper, by 25 tokens rather than 19, and at 6144 it
    costs 12.7% of the free budget instead of being unaffordable.

    **(ii) The cheaper spelling is live, not rejected.** `\(e)` costs **-24**
    tokens against the adopted `f"…"` (+94 against +118, both deciding every
    open case), touches **zero** existing literals because `\(` is
    `error[unknown_escape]` today with 0 occurrences, and lands in
    `selfhost/escape.hero`, which has room. The compiler-engineer prefers it. It
    is not adopted because `docs/panel/008-escape-sequences.md` R3 rules that a
    new escape reconvenes that panel, so taking it costs a sitting rather than
    tokens.

    **(iii) Principle 0's burden is unmet and the resolution says so.** The
    spec-warden vetoed on it: no reader has been put in front of either form,
    and metric 2, which would settle it, has never run. Ratifying R9 is
    ratifying an override of that veto, which is the author's to make. What
    would have met the burden is written in the sitting so a later one can
    collect it.

    **Why it matters:** the form is being funded by a ceiling raise, and the
    number that was confirmed is the one number in this decision that a
    measurement has since moved.

*******************************************************************************
