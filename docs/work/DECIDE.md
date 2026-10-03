# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/records/done/`, the record. Rank by
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

- [ ] **panel 188** | ratify, amend or overturn R1 to R12 (a group head's string is its value; what its tool cannot carry as one name refused at `check` in all three strings; a `\` in a header refused as robustness and C's undefined `'` `"` `//` `/*` as the thesis; control and invisible characters refused; a package names one package; the compiler asks again before any tool reads a name; no spec sentence) | `docs/panel/188-a-group-heads-string-is-its-value-and-what-its-tool-cannot-carry-is-refused-on-its-line.md`

    **Origin:** panel 188's synthesis, 2026-10-03 at 21:40, on the trunk
    frozen at `826ddc2f`. Until it is answered the batch that lands R1 to R9
    proceeds on the provisional resolution, defect 216 being `blocking` and
    never deferred (CLAUDE.md § 4, *the panel never blocks*); an overturn is
    follow-up work on that batch's tree.

    **Recommendation: ratify R1 to R12**, on what was built and run: stage E
    in the compiler-engineer's copy, the trunk's exits at 2 or 134 from 9 to
    0 on 98 cases on this Mac and from 14 to 1 on 154 in the Linux arm64
    image (the one left, a private-use code point, is the reader filed with
    the batch); the compiler's own tests 1,109, `check` 450, `run` 261,
    `corpus` 55, `unsupported` 131; the census 0 of 1,073 moved; stage D
    re-run by the critic from its patch alone, the guard silent over 461
    tracked programs; 0 of 103,736 real header names refused; 0 of 13 fresh
    sessions bracketing the name. **Two places the synthesis parts from a
    seat, each yours to turn**: R4 is taken over the spec-warden's objection
    on robustness, and its conservative form drops R4 (`a'b.h`, `d//b.h`,
    `e/*b.h` keep building, and a missing header named with `'` exits 2
    again unless its reader is repaired first); R10 takes no spec sentence on
    P3's measurement, and the spec-warden's a6+r1 (-12 and -13 vendored) is
    the alternative.

*******************************************************************************
