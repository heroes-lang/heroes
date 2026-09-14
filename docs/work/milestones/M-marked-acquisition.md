# M-marked-acquisition — the mark goes where the obligation is created

**Scheduled 2026-09-14 by panel 147 R4**, at M-cleanup-verdict's close, and it
is the form that milestone's verdict admitted. The sitting refused a releaser
keyed on the handle TYPE and named the axis instead of the spelling: **the
obligation is created by a CALL, so the mark goes on the acquiring call, and the
compiler never picks the release call.**

**What it delivers**: a compiler rule by which a C handle acquired in a scope
and not released on some path out of it stops being silent. Whether *stops being
silent* means a compile error or a loud exit is the first thing this milestone
measures, not something it inherits.

**Why the sitting would not choose the instrument here.** Two are on the ladder
and both have shipped relatives in this tree, so the choice is a price and not
an argument — and panel 147 had no price, because Route A did not even parse
(`error[expected_extern_signature]`), so nothing at that sitting was built.

- **A `consumes`-shaped mark on the acquiring call.** Nearest shipped
  neighbour: `selfhost/check/consuming.hero`, **114 code lines**, landed
  2026-09-13 at panel 145. It already expresses *this call ends that value's
  life*; the mirror is *this call begins one*.
- **Escape refusal.** Its machinery ships **twice already** — `cstr_escapes`
  (`selfhost/check/lending.hero:242`) and `lease_escapes`
  (`selfhost/check/leasing.hero:109`) — and panel 147's completeness critic
  found that **panel 122 refused an INFERRED release and not escape refusal**,
  so the precedent usually cited against it is for another form. Five shipped
  relatives measure **93 to 241 code lines** (`ffi_sweep` 93, `consuming` 114,
  `leasing` 195, `freer` 221, `lending` 241), two of them under the
  compiler-engineer's own 120-line bar. **Blast radius on this tree: five
  sites** — three handle-typed fields outside an `extern` group and two
  wrapper-returning functions.

**What this milestone may not do.** Re-open panel 147's R1: a releaser keyed on
the type is refused on its axis, it now has a design.md Part 6 row with a
falsifier, and no spelling repairs it. Adopt a scope-bound statement without
answering the `@`-cell defect two seats found from opposite ends — the deferred
call's arguments are read where the statement is written, so the natural program
registers the release of a value the acquiring call has not yet filled, and it
compiles.

**The urgency it inherits is smaller than the sitting was told, and that is
written here rather than discovered later.** `docs/measurements/030`'s
correction of 2026-09-14: live exposure is **22 paths in one file**, not 23 in
two, and **all 22 end in `exit(1)` or `abort`** — `main` closes and exits on
`is_err()`, the six test blocks use `.must()`. **No shipped program leaks a
handle and then goes on running.** The class is real, the reduction proves the
mechanism, and no program in this corpus has yet been written that handles an
error instead of leaving. A milestone that opens by re-measuring that will know
whether its own warrant has grown.

*******************************************************************************
**OPEN: 3**

- [ ] **M-marked-acquisition** | neither thing that COLOURS a program knows any of the eight contextual marks, and no check compares their word lists | `editors/vscode/syntaxes/heroes.tmLanguage.json` · `site/src/lib/highlight.ts` · `.claude/rules/diagnostics-and-goldens.md` § A new surface form

    **Origin:** M-marked-acquisition step 4, 2026-09-14, found while walking
    CL-036's list for `acquires`.

    **Measured, not assumed**: the site derives its keyword set from
    `selfhost/keywords.hero`'s own `keyword()`, so it follows the compiler for
    KEYWORDS automatically — and the eight contextual marks are deliberately not
    keywords, so neither highlighter colours `owned`, `consumes`, `acquires`,
    `tag`, `partial`, `link`, `package` or `as`. That is the state as it was
    before this milestone, not a regression it introduced.

    **Why it is filed as a class rather than patched for one word**: adding
    `acquires` to two files and leaving seven uncovered would make the gap
    harder to see, not easier. `.claude/rules/diagnostics-and-goldens.md` already
    says what is owed and where it lives — *a check that compares each
    highlighter's word list against `selfhost/keywords.hero`'s keywords and
    `selfhost/inventory.hero`'s built-ins has somewhere to live*, namely beside
    `suite_spec.hero`'s reading of `spec/reserved-words.md`. **Build the check,
    then the colouring follows from it.**

- [ ] **M-marked-acquisition** | decide whether a missed release is a compile error or a loud exit, and say what the checker can actually see | `selfhost/check/leasing.hero:29` · `selfhost/check/consuming.hero:22` · `runtime/parts/alloc.c:126-137`

    **Origin:** panel 147, 2026-09-14, out of the three fourth-routes its seats
    proposed independently.

    **Both halves are already built here, for other obligations.** The loud
    exit ships: `runtime/parts/alloc.c` carries the counter whose panic
    *accuses the program rather than this compiler*, and a lease nobody ends
    aborts at `main`'s return saying how many. The compile error ships twice, as
    the two escape refusals above.

    **What the sitting found and this item must not forget**: the runtime
    counter as the compiler-engineer drafted it **carries the same defect as
    its own veto** — keyed on the type, a borrowed handle from
    `sqlite3_db_handle` increments and never decrements. Any instrument here
    inherits the axis: **it must be told which call acquires.** And both
    `check/leasing.hero:29` and `check/consuming.hero:22` state in terms that
    the checker has **no flow analysis**, so a rule that needs one is a
    different and larger milestone.

- [ ] **M-marked-acquisition** | the corpus throws away the one answer it already has: 19 of 21 release sites discard the releaser's return code | `docs/measurements/031-the-census-counted-from-the-tree-and-it-has-two-levels.md` § 4 · design.md:1869 · `examples/ledger/main.hero`

    **Origin:** found at this milestone's own step 1, 2026-09-14, while
    re-counting the census from the tree.

    **`sqlite3_close` answers `SQLITE_BUSY` exactly when a statement was left
    open.** That is this milestone's entire subject, already computed by the
    library, already across the boundary — and the corpus discards it at **21
    of 21** sites, 19 as `_ = <release>(…)` and 2 as a wrapper's `return` to a
    caller that then discards it. design.md:1869 already names the shape: *an
    ignored C return code is C's own classic silent bug*.

    **Price it as a THIRD row on the ladder and not as the answer**, and say its
    width honestly: `free` and `curl_easy_cleanup` return `void`, so it detects
    nothing for them, and it reports at close time rather than at the leak. What
    it costs is near zero and what it buys is real on one library; both numbers
    belong beside the other two instruments rather than instead of them.

*******************************************************************************
