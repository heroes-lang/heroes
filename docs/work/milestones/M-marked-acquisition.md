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

- [ ] **M-marked-acquisition** | R6 is DECIDED and the reason is the critic's, not a preference — what is owed now is the return condition somebody can check | `docs/panel/148-the-mark-is-written-and-never-inferred-and-it-names-what-ends-the-life.md` R6 · `docs/measurements/031` § 2 · `spec/heroes-spec.md` § 13

    **Origin:** panel 147 left it open, panel 148's brief closed it by accident,
    and it is decided here on the recommended resolution as CL-002 allows.

    **The decision: a missed release is a LOUD EXIT, and it is not a compile
    error today.** The reason is structural rather than a preference, and panel
    148's completeness critic wrote it: every text on that sitting's table
    amends `Member` and `CParam`, so the mark is **extern-only** — while this
    corpus acquires inside **Heroes wrappers** one module away, `opened` with 7
    call sites and `prepared` with 6, because §4.19 refuses an `extern` call
    across a module boundary. A runtime counter does not care where the value
    goes next. **A compile error would need the obligation to cross an ordinary
    function's signature, and no ordinary `Param` in this language carries a
    mark of any kind** — not `owned`, not `consumes`, not `acquires`.

    **What is decided is the instrument and not the ambition.** The loud exit
    ships and is measured in both directions; what would make the compile error
    reachable is a separate and larger question — a mark on an ordinary
    parameter, which is surface no sitting has seen and which `Params` at
    `spec § 4` would have to admit.

    **The return condition, so this does not sit forever**: a measured program
    in `examples/` that leaks a handle and **continues**, rather than exiting —
    which `docs/measurements/031` § 3 records does not exist today, all 22 paths
    ending in `exit(1)` or `abort`. The day one is written, the loud exit
    reports at a moment nobody is watching and the case for catching it earlier
    is made by a program rather than by an argument.

- [ ] **M-marked-acquisition** | the corpus discards 22 releaser return codes, and the question is now whether that still matters | `docs/measurements/031` § 4 · `examples/ledger/main.hero` · design.md:1869

    **Origin:** step 1 filed it as the ladder's third row; step 6 re-measured it
    against a ladder that has changed underneath it.

    **What was measured, and it stands**: `sqlite3_close` and `sqlite3_finalize`
    answer an `i64`, `curl_easy_cleanup` answers `void` — **two of the three
    shipped releasers report at all** — and the corpus discards the answer at
    **22** sites with `_ =`. design.md:1869 names the shape: *an ignored C
    return code is C's own classic silent bug*.

    **What changed**: the counter landed, and it reports the same class at exit
    134 saying how many, for **every** library rather than for one. So the
    return code is no longer a candidate instrument for the leak — it is
    **subsumed** — and what remains of it is narrower and still real: SQLite's
    `SQLITE_BUSY` is the library's own opinion about a handle, which the counter
    does not model and cannot.

    **The honest verdict is therefore NOT to build a third instrument**, and to
    say what would reverse that: a measured case where `sqlite3_close` reports
    BUSY and the counter does **not** fire — a handle the library considers
    live for a reason the count cannot see. Produce one and this becomes a
    detector rather than a tidiness complaint. Until then the 22 sites are a
    corpus question about error reporting, not a compiler one.

*******************************************************************************
