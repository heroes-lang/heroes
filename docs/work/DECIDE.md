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

- [ ] **panel 148** | the surface of the mark panel 147 admitted: option C refused on twelve headers of nineteen, and the mark that lands is written and names what ends the life | `docs/panel/148-the-mark-is-written-and-never-inferred-and-it-names-what-ends-the-life.md` · `docs/panel/148-reports/` · design.md Part 6

    **Origin:** panel 148, 2026-09-14, M-marked-acquisition. Five seats plus the
    completeness critic, convened without asking because `spec/` is a trigger
    and the author had asked for the milestone to be carried to its conclusion.

    **What is put to you** is R1 to R6. The one that decides the code is **R2**:
    the mark is `acquires <releaser>`, written and naming the function that ends
    the handle's life — the shape this repository already ships as
    `owned sqlite3_free`, and the shape GCC ships as
    `__attribute__((malloc (fclose, 1)))`. **Measured at +66 vendored tokens,
    which is four FEWER than the corrected bare word**, so the more robust
    option is also the cheaper one.

    **Why C fell, in one line**: twelve of nineteen real headers declare both a
    releaser for a type and a borrowed hand-back of that same type, and under C
    a correct program is compelled to free what it does not own — reproduced
    twice under AddressSanitizer, `bad-free` inside `sqlite3VdbeDelete` and a
    SEGV inside `curl_slist_free_all`.

    **What a yes does NOT settle, and it is R6**: whether a missed release is a
    loud exit or a compile error. The coordinator's brief settled that by
    accident, writing the abort into both drafts, so no seat ever saw a
    compile-error instrument. It returns to this milestone undecided, and the
    critic named what it must answer first — every draft puts the mark inside an
    `extern` group, while this corpus acquires inside Heroes wrappers one module
    away.

    **The conservative resolution is recorded in the sitting** and you may take
    it instead: the bare `acquires` at +58, with no named releaser and no
    completeness diagnostic (CL-040).

*******************************************************************************
