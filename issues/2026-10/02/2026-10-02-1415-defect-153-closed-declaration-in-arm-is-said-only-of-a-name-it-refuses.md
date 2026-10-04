# Defect 153 closed: declaration_in_arm is said only of a name it refuses, and a discard stands on an arm's line

- [x] **153 — `declaration_in_arm` says `_` would be bound, which spec § 5 says it never is** | `.blue => _ = 0` on an arm's line: *an arm's body may not declare a name — `_` would be bound where nothing can read it*; spec § 5: `_` *binds nothing*, and § 8 names `_ = 0` as the arm that does nothing | `selfhost/parse/arm_body.hero:25-38` (`declared_name` does not ask the name) · **closed 2026-10-02**

    **Origin:** panel 185's critic, compiler-engineer and spec-warden,
    2026-10-02 (`docs/panel/185-briefs/probes/q2/s06-discard.hero`); reproduced by
    the coordinator at 02:42 on `03e70520`. Whether `_ = e` stands on an arm's
    line is panel 185's Q2; the message is false whichever way it rules.

    **Why it is a defect.** design.md §4.17: a diagnostic is true; this one
    contradicts the spec it cites.

    **2026-10-02, lane arm, panel 185 R3: `_` stands on an arm's line, in
    `_ = e`, `_: T = e` and `_: T @ e`, and the message is true of every name
    it still refuses**: repaired at `1ea85b01`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane arm, `1ea85b01`, with defect 147 (panel 185's R3).
    `parse/arm_body.hero` now asks the name being bound: `_ = e`, `_: T = e`
    and `_: T @ e` stand on an arm's line and get the message their block
    form gets, and `declaration_in_arm` is said only of a name it still
    refuses, so its message is true. **Cases**:
    `check/fixedbugs-153-a-name-bound-on-an-arm-s-line` and
    `-a-discard-on-an-arm-s-line-is-told-as-in-a-block`. The panel's own
    reproducer, `docs/panel/185-briefs/probes/q2/s06-discard.hero`, moved
    from exit 1 to 0 in the census, as R3 meant.

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
