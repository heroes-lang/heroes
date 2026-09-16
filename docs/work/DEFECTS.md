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

- [ ] **049 — `--emit-c` writes the pre-probe tag spelling, and the obvious repair makes the artifact platform-dependent** | a program binding `record <Name> tag <name>` gets C that clang refuses — 20 errors on `structtag` — and the probe that fixes it made the SAME program emit two different files on two machines | `selfhost/cli/artifact.hero` · `docs/panel/157-the-artifact-of-a-build-that-never-happened.md` R2 and R4

    **Origin:** the tag half of defect 048, withdrawn 2026-09-16 within the hour
    of landing, by CI. 048's §1.12 half — the pointee check on the `--emit-c`
    path — **stands and is closed**; this is only the spelling.

    **The repair worked and its consequence is the defect.** The advisory probe
    asks clang `-fsyntax-only`, harvests the `struct` request, discards the
    verdict and re-emits: `structtag`'s artifact went from **20 clang errors to
    0**, with `struct probe *` written 21 times. But it made the artifact **a
    function of the local headers**, and `tests/emission/` blesses those bytes.

    **Measured by CI, both legs of one push:**

    | | the blessed line |
    |---|---|
    | macOS | `(struct addrinfo * *)0` — the probe learned |
    | Windows | `(addrinfo * *)0` — no `netdb.h`, so it learned nothing |

    The probe's degradation is correct by design — *on a machine without the
    header it learns nothing and the output is yesterday's* — and that is
    precisely what a byte-for-byte blessing cannot tolerate. **It is defect
    047's own class**, a golden encoding one platform's answer, committed inside
    a repair on the day the rule naming it was written
    (`.claude/rules/diagnostics-and-goldens.md`, CL-076).

    **So the sitting's question is not how to spell a tag.** It is: a blessed
    artifact cannot depend on the machine that produced it, so either
    `--emit-c` stays deterministic and a tagged binding keeps getting C that
    does not compile, or those programs stop being blessed by bytes and are
    judged by a predicate instead. The second is **panel 157's R4**, adopted and
    unbuilt, filed with its price in
    `docs/work/milestones/M-package-manager.md`: build both ways, compare exit
    code and stdout, and LINK.

    **What is owed** is that choice, made at a sitting rather than in the hour
    after a red leg — and the withdrawn probe is thirty lines that can be put
    back in one commit, kept in `selfhost/cli/artifact.hero`'s own comment with
    its date and its measurement.

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

*******************************************************************************
