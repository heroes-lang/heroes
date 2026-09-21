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
**OPEN: 3**

- [ ] **panel 171** | the word is `lent`, in its own slot beside `counted_by`; a lend reaches only a `lent` parameter of an `extern` function; the spec sentence is the warden's merged draft at +13 real and promises nothing about a pointer C hands back; the landing is two commits with 27 marks on 19 functions across three trees — ratify the word, the rule and the landing, or take the conservative sentence the sitting recorded | `docs/panel/171-lent-and-the-pointer-c-hands-back.md`

    **Origin:** panel 171, 2026-09-20, the full panel, on the word and the exact
    rule under the default the author flipped that evening. **The direction is
    not in this item**: it is the author's ruling, recorded in
    `docs/records/log/2026-09-20-1930-heroes-must-be-robust-and-the-default-flips.md`.
    **The default while this is open** is the adopted resolution, and work
    proceeds on it: commit A parses `lent` with the rule unchanged; commit B
    flips the rule, marks 27 parameters, lands the sentence and the fence, and
    moves 35 goldens.

    **What conservative would have been**, recorded so it can be taken: the same
    word and rule with the llm-ergonomist's longer sentence, +42 real, which
    says *takes a lease or a pointer C owns*. It is clearer to a reader and it is
    **false of the pointer C hands back**, which the completeness critic measured
    on five programs. The sitting took the shorter sentence because it is the one
    that is true.

- [ ] **panel 172** | the repair of defect 070 is a run-time report and not a word: the runtime names the live lease when a C function frees it (fifty lines on the handler `stack.c` already ships, 0 to 250 bytes on the filed reproducer, re-run by the coordinator), `consumes` stays refused on a `cstr` or `ptr` because it closes one of seven shapes and three have no parameter to mark, § 13 writes the limit with its falsifier, and the mechanism goes to a soundness-lane sitting before it lands — ratify the direction, or take the conservative option the sitting recorded | `docs/panel/172-the-report-comes-from-the-crash-and-not-from-the-declaration.md`

    **Origin:** panel 172, 2026-09-21, the full panel with the completeness
    critic before the synthesis, convened on defect 070 without asking because
    the milestone's one ask was spent at panel 167. **The default while this is
    open** is the adopted resolution, and work proceeds on it: panel 173 in the
    soundness lane on the fifty lines, then the landing, then the defect closes.

    **What the brief proposed and the sitting refused on measurement**: route A,
    `consumes` gaining a meaning on a pointer parameter. The compiler-engineer
    built it (96 lines, every suite green) and measured that the filed
    reproducer, whose callee carries no word, is `check` 0 before and after; the
    spec-warden found the form inside design.md Part 6 since panel 150; the
    ffi-pragmatist measured that a wrong ownership word has no judge where a
    wrong type has a header; the critic found three shapes with no parameter to
    mark at all.

    **What conservative would have been**, recorded so it can be taken: route C
    alone, the limit written into § 13 and defect 070 closed as a documented
    limit with measurement 037's disposer route as the correct spelling. Zero
    lines of C, and the filed reproducer still dies at 133 saying nothing. The
    sitting took the runtime report because it is measured, reaches the shapes
    no sentence reaches, and turns a silence into a report.

- [ ] **panel 173** | the soundness lane on panel 172's mechanism: both seats VETO the sentence, which asserts that a C function freed the lease and is measured FALSE on six of nine paths (a C library's own `abort`, its failed `assert`, its double free), with `siginfo_t` unable to tell them apart on either platform; the repair is to say what the runtime observed, at zero net lines; the funnel, the sanitizer yield, the chaining and the frame-walk name are endorsed twice; and the goldens panel 172 promised cannot enter `tests/golden/run/` until the harness admits a case that is supposed to provoke the sanitiser — ratify, or take the conservative option the sitting recorded | `docs/panel/173-the-runtime-may-say-what-it-saw-and-not-why.md`

    **Origin:** panel 173, 2026-09-21, the soundness lane (`compiler-engineer`
    and `ffi-pragmatist`), convened by panel 172's own resolution item 1 before
    the mechanism lands. **The default while this is open** is the adopted
    resolution, and work proceeds on it: the landing commit, then defect 070
    closes.

    **What the sitting measured that neither seat could see alone**: the ffi
    seat judged the fifty-line prototype and found it destroys a C library's
    SIGABRT disposition, 10/10 on two platforms; the engineer's build already
    chained to it and marked that branch unrun. The coordinator ran the ffi
    seat's own program against the engineer's build: the library's handler
    returns, 77 ten times of ten, with the report still printing.

    **What conservative would have been**, recorded so it can be taken: report
    only on SIGTRAP, where the assertion is measured sound on Darwin. Refused
    because it drops 24% of the true cases there (152 of 200 at 133, 48 at 134)
    and all of them on Linux, where the allocator raises SIGABRT alone: it
    trades a false sentence for a missing one, and the missing one is the
    defect.

*******************************************************************************
