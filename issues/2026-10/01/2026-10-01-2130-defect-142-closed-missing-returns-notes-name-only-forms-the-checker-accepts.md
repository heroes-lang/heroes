# Defect 142 closed: missing_return's notes name only forms the checker accepts

- [x] **142 — `missing_return`'s note sends the author to a form the checker refuses** | the note says *every path must end in a `return`, or the last statement must be the value*; a function whose last line is the bare value then costs `missing_return` again and `discarded_value` | `selfhost/flow_errors.hero:157-163` · **closed 2026-10-01**

    **Origin:** lane 135b's final report, 2026-09-30; reproduced by the
    coordinator at 00:11 on 2026-10-01 on `3cc3b553`
    (`scratchpad/p184/newdef/mr.hero`, `mr-note-followed.hero`).

    **Why it is a defect.** design.md §4.17: a diagnostic carries what is
    needed to fix the program; this one's advice, followed, is refused.

    **2026-10-01, lane flow, the notes name only forms the checker accepts**
    (every path ends in a `return` with a value, past an `if` with no `else`
    and past any loop; a value at the end of a path is not returned, so
    `return` goes before it; every repair named builds and runs in
    `tests/golden/run/fixedbugs-142-every-repair-the-note-names.hero`):
    repaired at `8d5cedd7`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close. The `match` arm's clause waits with
    defect 139's statement half, which makes it true.

    **The repair**, lane flow, `8d5cedd7`, its `match` clause in `3a25b640`.
    **The class**: the note said *every path must end in a `return`, or the
    last statement must be the value*; the second half, a body's last line as
    its value, is a design the checker refuses (a body runs for effect, panel
    020), and followed it cost `missing_return` again and `discarded_value`.
    Now two notes: every path ends in a `return` with a value, and a path runs
    on past an `if` with no `else` and past any loop, `while true` included;
    a value at the end of a path is not returned, so `return` goes before it.
    The `match` arm's clause landed with defect 139's statement half, which
    made it true; panel 184's R4, if ratified, would amend the loop clause.
    The text is pinned by `selfhost/flow_errors.hero`'s test.

    **Cases**: `run/fixedbugs-142-every-repair-the-note-names` (each repair
    the notes name, in the shape it repairs, computed by hand, legal on the
    trunk too) and `check/fixedbugs-142-a-loop-runs-on-to-the-end`; both
    reproducers, the notes followed by hand, build and run.

    **The gates.** Each repair by its cases and the compiler's own tests (961)
    and the census of the tracked files against the compiler before it, no
    exit moving but the permissive arm's `declaration-in-arm`, 0 to 1 (an arm
    binding a name has no value). **The batch gate**, lane flow's closing
    commit `32e9dd18`, the trunk `6df3d121` merged once at `353ffddb`: the
    seed regenerated, 34,406,367 bytes, SHA-256 beginning
    `89913cebe03b7e60`, its fixpoint by `cmp`; the compiler's own tests 968
    and the net's own 184, all passed; `tests/emission`, the four new run
    cases blessed and nothing else moved; the full net, 25 suites four at a
    time and `cache` alone, 4,166 passed and 0 failed (`surface` 335 and 1 and
    `probe` 18 and 2 in the parallel pass on the harness's 120 s timeout at
    load 36, 336 and 0 and 24 and 0 alone); the census over 1,492 files,
    7 and 8 moved, every one the batch's own. The trunk fast-forwarded to
    `32e9dd18` at 19:54 on 2026-10-01. **Linux x86-64** on `32e9dd18`: the
    compiler's own tests 968, all passed, and 18 of the 19 suites 0 failed;
    `probe` read 21 and 1, its `selfhost, multi` on the 120 s timeout, and
    stayed red alone at 20:51. Measured, not inferred: that one program takes
    the same time on both trees, user 57.00 against 56.83 s on this Mac and
    86.64 against 86.27 s in the container, so the batch did not slow it; the
    container emulates amd64 on this arm64 host, under the host's load. Alone
    on a quiet machine at 21:13, `probe` read 24 and 0: the 19 suites 4,019
    passed and 0 failed. **Owed before the push**: Linux arm64 and the
    Windows box, on the trunk.
