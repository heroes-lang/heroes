# Defect 147 closed: an arm's line holds one statement that binds no name, and the spec says so

- [x] **147 — spec § 8's `Inline` production refuses one-statement arms the design allows and the compiler builds** | `Inline = ( Expression | "return" [ Expression ] | "break" | "continue" | "assert" Expression ) NEWLINE` leaves out a mutation (`.blue => n @ 5`) and a `while` (`.red => while n < 3` over its body), and both check, build and run on the trunk, where design.md §4.7 says *an arm's body is one statement, inline, or an indented block* (panel 014) | `spec/heroes-spec.md:241-243` · design.md §4.7 (`:1230-1235`) · `selfhost/parse/arm_line.hero` · **closed 2026-10-02**

    **Origin:** lane flow's first pass, 2026-10-01
    (`scratchpad/lane-flow/p1/a54-statement-arm-mutation.hero`,
    `a76-inline-while-arm.hero`); built and run by the coordinator at 12:35
    on 2026-10-01 on the trunk at `e252fda4` (`scratchpad/p185/inline/`),
    printing 5 and 3.

    **Why it is a defect.** The spec is the language as a reader gets it,
    and here it refuses programs the language accepts: a reader of one is
    told by the spec that it cannot compile, and a model writing from the
    spec never learns the form. Which side moves, the production or the
    parser, is a change to the language, so the repair is a sitting's, with
    defect 143 and the value-block question in `docs/work/DECIDE.md`.

    **2026-10-02, lane arm, panel 185 R3: an arm takes § 5's `Simple`, every
    statement but a declaration or an `=` of a name other than `_`, and
    `Inline` is deleted**: repaired at `1ea85b01`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane arm, `1ea85b01`, panel 185's R3 as ratified on
    2026-10-02. **The class**: spec § 8's `Inline` production refused
    one-statement arms the design allows and the compiler builds (a mutation,
    a `while`). Now `Inline` is deleted: § 5 defines `Statement` as a
    `Binding` or a `Simple` statement and `Arm` takes `Simple`, and § 8's
    prose says an arm's line holds one statement that is not a declaration or
    an `=` of a name other than `_`; the wording is the landing's, as R3
    provides, without the priced *`_ = 0` included*. **Price**: vendored 6716
    to 6705 and 6838 to 6824, real 9060 to 9040 on claude-opus-5 (one
    `--refresh`), the ledger's row and the pins moved with it. **Case**:
    `run/fixedbugs-147-one-statement-on-an-arm-s-line`, with its emission;
    `spec`, `special` and `grammar` green.

    **The gates.** Each repair by its cases and the compiler's own tests in
    its own lane, and the census against the compiler before it. **The
    round's one gate**, the first under the author's instruction of
    2026-10-02 (one gate per round, on this Mac): lanes ffi-macro, ci-probe,
    arm and recovery-b7 merged into `lane-round1002` (`c0f0f627`
    fast-forwarded, then `e8e4eaf6`, `76777908`, `ccba1df5`, one conflict, in
    item 135's body, every line of both lanes kept), closed at `e2d59fdb`:
    the seed regenerated once, 36,141,732 bytes, SHA-256 beginning
    `e6d520df7426008b`, its fixpoint by `cmp`; the compiler's own tests 1,018
    and the net's own 187, all passed; the full net, 25 suites three at a time
    and `cache` alone, 4,449 passed and 0 failed, one floor raised as the
    suite asked (`annotations`, 2,148 to 2,779), nothing blessed; the census
    of `check --brief` over 1,720 files, two exits moved, lane arm's two
    predicted ones, every moved output attributed to one lane by re-running
    the census with each lane's own compiler; the `--emit-c` census, 0 bytes
    of C moved; the site's build green. The trunk took the round at
    `415c0a14` on 2026-10-02, with only panel 186's sitting between, and
    `records` read 24 passed, 0 failed after it. **Owed before the push**:
    Linux arm64 and the Windows box, by the same instruction.
