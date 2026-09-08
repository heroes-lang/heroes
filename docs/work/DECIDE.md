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

- [ ] **panel 119** | ratify or overturn the verdict on Part 7 items 1 and 12: closures REFUSED with a Part 6 row, inline blocks EXAMINED AND UNPLACED, and three design.md sentences corrected | `docs/panel/119-the-warning-that-does-not-fit.md`

    **Origin:** the sitting of 2026-09-08, M-closures-verdict step 1 — full panel, five seats, provisional pending this answer.
    **The default the compiler is running on** is the resolution as adopted:
    R1 refuses closures with capture on cost alone, R2 gives the row a falsifier
    that deliberately does NOT rest on capture being hazardous, R4 names the
    capture-free narrowing as the form that returns on a measured Part 11
    effect, R5 puts inline blocks in `comptime`'s *unplaced* shape with three
    joint return conditions, and R6 corrects *"~150 lines"*, *"~60 spec
    tokens"* and *"no refcount interaction"* in design.md.
    **The measurements that decided it**, all 2026-09-08: 282 match arms in 74
    of 190 modules and five DECIDED ceilings breached; **+118** spec tokens
    against **71** free, with six removals priced and none admissible; four
    compiled corruption classes, one of them clang-clean under all fourteen
    flags and **exit 139** at run time; and the subtraction test worth **21
    declarations, all in example programs, none in the compiler**.
    **What conservative would have been** is in the file's own section: option
    F for both, a dated deferral, cheaper by two documents and objected to by
    two seats — the warden because a dated condition is the mechanism §1.6 names
    as the one the record shows failing, the ffi seat because it leaves three
    pieces of prose reading as true when they are not.
    **Recommendation: ratify.** Three seats vetoed the feature and the two that
    compile both approved the refusal; the historian found the refusal is a
    mainstream position under this language's exact constraints, four languages
    in writing and one of them self-hosting since 1988. The one thing the author
    may want to overturn is R5: putting inline blocks *unplaced* rather than
    deferring them costs a design.md paragraph that a deferral would not.

- [ ] **panel 118** | ratify or overturn the resolution on `_ =` and a fallible: three positions closed, no `certain` fix, and the deliberate drop paid for by a narrow emitter relaxation rather than a new built-in | `docs/panel/118-the-missing-word-was-a-dead-variable.md`

    **Origin:** the sitting of 2026-09-08, `docs/panel/118-the-missing-word-was-a-dead-variable.md` — full panel, five seats, provisional pending this answer.
    **The default the compiler is running on** is the resolution as adopted: R1
    closes `_ = e`, a `_` parameter and a type parameter; R3 ships three `guess`
    fixes and no `certain` one; R5 relaxes `emit/unread.hero` for `eq`, `ne` and
    the three bitwise ops on scalar operands so `_ = f().is_err()` stops emitting
    a C warning; R6 spends 49 spec tokens naming all three positions where 15
    would have named one.
    **What conservative would have been** is in the file's own section: the
    clause at +15 covering the statement only, `match` with block arms at five
    lines a site and no emitter change, and the `ignore` built-in, which two
    seats wanted and which is blocked by `selfhost/check/builtins.hero` sitting
    at 374 of a DECIDED 374 rather than by any seat's judgement.
    **Recommendation: ratify.** The robust and the cheap disagree here and the
    author's standing instruction of 2026-09-08 is to prefer robustness; the two
    extra positions cost zero migration sites, and the emitter relaxation costs
    ~14 lines in the one nearby file with headroom and moves no golden's bytes.

- [ ] **panel 118 finding** | may a `certain` fix preserve meaning while preserving the bug? design.md answers the inverse case only | `.claude/rules/diagnostics-and-goldens.md:18` · `docs/panel/071` · `selfhost/value_errors.hero:121-145`

    **Origin:** the compiler seat, panel 118, 2026-09-08, which ruled against it on §1.2 and named the gap rather than inventing a rationale for it.
    `.is_err()` is meaning-preserving and preserves the swallow, so certifying it
    would make `heroes check --apply` automate defeating the rule the same
    milestone lands. The rule and panel 071 both address a fix that *changes*
    meaning; this is the inverse and is unwritten.
    **Recommendation: write the rule as "a `certain` fix repairs the defect the
    diagnostic names, and a fix that leaves it standing is a `guess`."** It costs
    one sentence in the rules file, no spec token, and it is what R3 already
    assumes.

- [ ] **panel 118 finding** | is a runtime `hero_*_require` null rejection a program abort or a compiler tripwire? R5's scope depends on the answer | `runtime/parts/array.c:238-239` · `selfhost/emit/structural.hero:53-56` · design.md Part 5

    **Origin:** the compiler seat, panel 118, 2026-09-08 — Part 5 calls `ptr == NULL` "the one non-value that every runtime entry point rejects" without saying whose bug it catches.
    R5 excludes aggregate operands from the emitter relaxation on the assumption
    that those rejections are checks worth keeping. If they are compiler-internal
    tripwires the exclusion can be dropped, the relaxation becomes a pure
    function of `ir.OpKind`, and it costs two lines less.
    **Recommendation: keep them.** A tripwire that fires only when the checker
    has already failed is worth its cost, and `structural.hero:53-56`'s own
    comment gives the reason: "'should never' is exactly the premise that goes in
    the loud direction."

- [ ] **panel 118 finding** | should the spec exhibit one diagnostic, and should `.must()`'s abort say it ends the program? | `spec/heroes-spec.md:154` · `docs/measurements/010-spec-budget-ledger.md`

    **Origin:** the llm-ergonomist, panel 118, 2026-09-08, reported as an appetite rather than a request, from a seat that reads the spec and nothing else.
    Two halves, decided together because they are paid from one budget. A
    language whose thesis is that every plausible mistake is a compile error
    shows its reader **zero** compile errors, so a model cannot predict what it
    is about to be told. And `.must()` is described in three words, "extract or
    abort", with no consequence attached — which is what made it that seat's
    attractive repair in the one situation where it was catastrophic.
    **Recommendation: buy the `.must()` half, defer the exhibit.** The
    consequence clause is a few tokens and it steers a writer away from turning
    an ordinary failure into an exit 134; a worked diagnostic is dearer than the
    remaining headroom after M-discard-refusal and wants its own payment.

*******************************************************************************
