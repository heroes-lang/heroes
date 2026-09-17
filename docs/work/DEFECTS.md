# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **050 — a test of a doubly-fallible value asks the outer layer and reads as asking the inner one** | `m["b"].is_err()` on a `{str: i64?}` and `find(xs, …).is_err()` on a `[i64?]` are `check` 0 and RUN, and both answer *was the key there* where the line reads *did the stored value fail* | `docs/panel/158-the-sitting-produced-a-resolution-and-left-it-off-its-own-ballot.md` R4 · `selfhost/check/ops.hero`

    **Origin:** panel 158's completeness critic, which ran it while auditing the
    sitting's framing; re-run by the coordinator at the synthesis. Neither route
    needs a generic and neither needs a map: `find` over `[i64?]` reaches it with
    built-ins § 11 hands out thirteen lines apart.

    **The reproducer, measured 2026-09-16**, `check` 0, built, and it prints both
    lines:

    ```
    m: {str: i64?} @ {}
    m["a"] @ fail(code: "parse", msg: "not a number")

    if m["b"].is_err()
        print("this reads as: the stored value failed")
    ```

    `m["b"]` is an `i64??`. `.is_err()` peels the OUTER level and answers *the key
    was not there*, which is true — and the program was written to ask the other
    question.

    **Why it is a defect and not the doubly-fallible question.** Panel 158 framed
    its sitting as *consistency, not safety*, and the ffi seat measured that
    honestly: nothing leaks, nothing corrupts, nothing answers wrongly at the C
    boundary. **This is the third box the dichotomy has no name for** — a
    plausible mistake that compiles, runs, and gives a confident wrong answer to
    the question the author asked. That is design.md §1.1's own subject rather
    than §1.12's, and CLAUDE.md's opening sentence — every plausible LLM mistake
    is a compile error — is what it falsifies.

    **What is owed.** A refusal, or a diagnostic, at the point a `.is_err()`,
    `.must()` or `.default(v)` is applied to a value whose payload is itself
    fallible. The critic measured that of the four repairs panel 158 weighed,
    **only option 1 would have closed this**, and option 1 protects zero programs
    — so the repair is its own, not a by-product. Note that `.default(0)` on the
    same shape IS loud today, exit 1 with two diagnostics, so the three operations
    do not agree with each other and that disagreement is the place to start.

    **RESOLVED 2026-09-17 by panel 160, adopted and not yet built: option E.**
    `.is_err()` is refused where the payload is itself fallible; `.must()`,
    `.default(v)`, `?` and `match` are untouched, because each of those three
    hands back a value that still carries the second level and `.is_err()`
    hands back a `bool`. Prototyped by two seats independently at **+58 lines**,
    all in the checker: a new module of about 40 lines holding the `is_nested`
    question, a `nested_read` constructor in `selfhost/flow_errors.hero`, and
    one arm rewritten in `selfhost/check/builtins.hero` — whose `DECIDED`
    ceiling moves by 1.
    **0 bootstrap sites, 0 corpus files, `check` time flat.**

    **The message must name BOTH repairs** — `match` for the outer question,
    `.must()` then `.is_err()` for the inner — or the ergonomist's measurement
    says at least one repair in five lands `.must().is_err()`, which asks the
    inner question and aborts on an absent key. A refusal that relocates the
    silent error is not a repair. No `Fix`: both repairs change what the program
    means, and `missing_return` in the same file is the precedent for a note
    alone.

    The specification's half is **E3**, a clause in § 6's `.is_err()` row at
    **+17 vendored / +21 real**, paid by two measured duplicates: § 10's
    *and `for k in sort(keys(m))` walks them in order* (−18/−21) and § 8's
    *`break` and `continue` exist.* (−10/−12). The package lands at 5993 / 7986,
    so the document shrinks while gaining a refusal.

- [ ] **054 — the specification states a balance the runtime stopped keeping** | § 13 says a handle consumed twice hides one never consumed; the counter became a set on 2026-09-15 and the program aborts naming the stray FIRST | `spec/heroes-spec.md:378-379` · `runtime/parts/alloc.c` · `tests/golden/run/abort-handle-given-back-twice.expected`

    **Origin:** 2026-09-17, panel 160's ffi-pragmatist, measured on real SQLite
    while answering a question about something else. The tenth correction to a
    coordinator's briefs across six sittings, and the first that is a sentence in
    the specification rather than a number in a brief.

    **The false sentence**, verbatim: *"The owing is counted, so a handle
    consumed twice hides one never consumed."* It entered on 2026-09-14. On
    2026-09-15 commit `2e7d221c` — *"the counter became a set"*, its own
    subject — replaced the count with a set of live addresses in
    `runtime/parts/alloc.c`, and the specification was not touched.

    **Measured (the seat's P3b, two platforms):** two successful opens, `dbs[0]`
    closed TWICE through two copies, `dbs[1]` never closed. Exit **134**, and the
    message is the SET's — *1 C handle(s) given back that were never taken … the
    set of live handles did not hold that address* — with the stray reported
    BEFORE the leak. `tests/golden/run/abort-handle-given-back-twice.expected`
    asserts exactly that, so the instrument already knows what the document does
    not.

    **What is owed.** The sentence, corrected to what the set does. The seat
    priced the merge that replaces it at **−11 vendored**; the real number is
    UNRUN and the rule of 2026-09-16 says a vendored delta is not a price. That
    removal would then be available to pay for a later addition.

*******************************************************************************
