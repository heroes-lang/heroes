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
**OPEN: 2**

- [ ] **panel 122** | ratify the lend rule: positional and never typed, the extern discriminator dropped, two clauses measured free, and defect 022 split because no position rule closes the half where C keeps the pointer | `docs/panel/122-the-lend-was-two-defects.md` § Resolution

    **Origin:** the sitting of 2026-09-09, M-cstr-lifetime, five seats,
    convened by the author in one word after the coordinator put a fork to
    them that rested on its own bad measurement.

    **What is being ratified is not the ballot.** Three candidates went out and
    the resolution is a fourth: every clause a rule on the POSITION of the
    `.cstr()` expression and never on the type `cstr`, which is what makes it
    compatible with `owned`, with a C-returned `cstr`, with a `cstr` in an `@`
    cell and with a binding — all of which ship and run, and all of which a
    type-shaped rule refused. Two seats vetoed the type reading from opposite
    directions and both vetoes are honoured rather than overridden.

    **Two clauses the ballot lacked, both measured free.** A function outside an
    `extern` group may not answer `cstr` (every `-> cstr` in the tree is already
    inside a group), which closes a two-hop laundering that a position rule
    alone blesses; and a `cstr` may not be a record field outside a group (no
    such field exists in the tree), which requires rewriting
    `selfhost/emit/gate.hero:172`'s note in the same commit, because that note
    currently prescribes the shape the clause refuses.

    **R5 IS THE ONE THAT NEEDS THE AUTHOR'S EYE.** Defect 022 splits. What lands
    here closes the lend that escapes its frame; the lend the **C side retains
    past the call** is a different class, its discriminator is an argument of
    the C function rather than a Heroes position, and no position rule can see
    it. Both compiling seats built it and it is accepted at exit 0 with a wrong
    answer today. It is re-filed as defect 024 against §4.19's reserved borrow
    keyword — case 2 of the three that paragraph reserves, of which `owned` is
    case 1. Ratifying R5 is agreeing that the milestone closes over a defect
    that is still open, which is the honest reading and not the tidy one.

    **Why it matters:** the milestone was opened first and alone because a
    silent wrong answer outranks everything else in § Precedence; the sitting
    found that the shape it was opened for was half a class.

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

    **(ii) SETTLED 2026-09-09 by author decision: the `f` prefix, because it is
    the standard one** (*"preferisco l'f-string alla Python, è più standard"*).
    R3 stands as the sitting adopted it, and the two rival spellings that day
    produced are closed: the compiler-engineer's `\(e)`, and the author's own
    `\{e}`, which the coordinator had recommended at **-25** real tokens.
    Neither is taken, so `docs/panel/008-escape-sequences.md` R3 is not
    triggered and no second sitting is owed, which is worth more than the
    tokens. Re-measured on the real instrument (023), the adopted clause is
    **+151** against `\{e}`'s **+126**, and against the 6144 ceiling's 989 free
    that gap is 2.5% of the budget, where on 2026-09-08's false arithmetic it
    had been the whole argument. The deciding scale was familiarity, which is
    the one the thesis rests on, rather than the one the budget measures.

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
