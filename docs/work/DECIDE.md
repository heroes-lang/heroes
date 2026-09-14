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

- [ ] **panel 140** | where a sitting's briefs live, since the ratified home and the implemented one are different places and both vanish | `/panel` step 2 · `docs/records/done/2026-09-13-1104-decided-2026-09-13-author-every-brief-is-written-to-disk-before-a-seat-starts.md` · `docs/panel/147-briefs/`

    **Origin:** found at panel 147, 2026-09-14, while performing the rule.

    **The decision of 2026-09-13 recommended `docs/panel/NNN-briefs/<seat>.md`
    and was ratified *as adopted, nothing changed*** — and what had been adopted
    in `/panel` step 2 is the **session scratchpad**, `panel-NNN/briefs/`. Those
    are two different places, and the second does not do the job the first was
    asked for: a scratchpad is session-specific and goes away with the session,
    so *"the briefs are prompts; they vanish when the workflow ends"* — panel
    140's critic, the sentence the whole decision exists to answer — is still
    true of panels 145 and 146. `find docs/panel -maxdepth 1 -type d` returns
    one directory and it is from 2026-08.

    **The default while this is open**: a sitting's briefs survive only if its
    coordinator happens to copy them, which is the shape of a rule with no
    executor (CLAUDE.md § 3, CL-044).

    **Recommendation, 2026-09-14: `docs/panel/NNN-briefs/` per sitting, committed with it.**
    Panel 147 did it and the cost is one `cp` and 2396 lines for twelve files.
    It makes the panel's central claim — that judges are differentiated **by
    input** — checkable by a reader who was not there, which is the only reason
    the five-seat design carries more information than one opinion. **This is
    the author's to settle and not the assistant's**: CLAUDE.md § 4 says the
    skills are amended by author instruction and no panel.

- [ ] **panel 147** | the ruling on a scope-bound release for a C handle: Route A refused on its axis, Route B not as drafted, Route C not taken, and a form enters marked on the CALL with its own milestone | `docs/panel/147-the-obligation-is-created-by-a-call-and-not-by-a-type.md` · `docs/measurements/030-three-release-obligations-and-only-one-of-them-is-silent.md` · design.md Part 6 borrow-checker row

    **Origin:** panel 147, 2026-09-14, M-cleanup-verdict. Full five seats plus
    the completeness critic, on the author's authorisation of the narrowed
    question.

    **What is put to you.** The sitting adopted R1-R6 provisionally. The one
    that decides the next milestone is **R4**: a form enters, and its axis is
    that the obligation is marked where it is CREATED — on the acquiring call —
    and never on the type. The compiler never picks the release call. Two
    instruments are on the ladder and the successor milestone prices them
    against each other rather than the sitting choosing here: a
    `consumes`-shaped mark on the acquiring call (nearest shipped neighbour
    `check/consuming.hero`, 114 code lines) and escape refusal (`cstr_escapes`
    and `lease_escapes` already ship; five shipped relatives measure 93 to 241
    code lines; blast radius five sites).

    **The conservative resolution you may take instead is written into the
    sitting at R5**, as CL-040 requires: Route C, refuse, with a falsifier
    redrafted on the corrected census. It was not adopted because § 4 takes
    robust where the two disagree.

    **Why Route A fell, in one sentence, because it is the finding worth
    keeping:** the same handle type is handed back owned by one C function and
    borrowed by another — `sqlite3_db_handle` compiled at the sitting,
    `borrowed == owned: true` — so a releaser keyed on the TYPE makes the
    compiler close a connection it was lent, at exit 0 with the sanitizer
    silent. CLAUDE.md §11's *a narrowing asks the value, never the world* had
    already settled it; three seats reached it the expensive way.

*******************************************************************************
