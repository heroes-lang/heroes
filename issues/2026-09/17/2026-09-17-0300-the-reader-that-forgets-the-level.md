- [x] **050 — a test of a doubly-fallible value asks the outer layer and reads as asking the inner one** | `m["b"].is_err()` on a `{str: i64?}` and `find(xs, …).is_err()` on a `[i64?]` are `check` 0 and RUN, and both answer *was the key there* where the line reads *did the stored value fail* | `docs/panel/158-the-sitting-produced-a-resolution-and-left-it-off-its-own-ballot.md` R4 · `selfhost/check/ops.hero`

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

    **CLOSED 2026-09-17, M-check-completeness step 17.** Option E is built:
    `selfhost/check/nested.hero` at 40 lines, asked from the one arm in
    `selfhost/check/builtins.hero` whose `DECIDED` ceiling moves 377 to 378 —
    the number the sitting's compiler-engineer named before building anything.
    The message carries BOTH repairs, which was the resolution's own condition.
    `check` 125 to **126**, `annotations` 162 to **163**, the full net **1864
    passed, 0 failed** on a compiler built from the regenerated seed.

    The document's half landed with it: § 6's `.is_err()` row gains the clause
    at +17 vendored, paid by two true duplicates at −18 and −10, so the package
    is **−11 vendored and −12 real** and the document SHRINKS while gaining a
    refusal — the first row in the ledger to do so. **The spec-warden's
    prediction is scored and exact on both instruments**: 7986 real and 5993
    cl100k, the first prediction there to hit a real count to the token.
