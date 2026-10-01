# Defect 139 closed: a match statement leaves its block only when every arm does, and a branch that ended on a statement is told from one that jumped

- [x] **139 — a block holding a `match` statement counts as leaving whatever its arms do, so `check` passes a function with no `return` and `build` fails** | `function f(c: Color) -> i64` whose body is a `match` with printing arms, then `print(3)`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed ... non-void function should return a value*; the same with the `match` inside one branch of an `if` | `selfhost/check/walk.hero:898-904` (no arm with a value read as every arm jumping) · `arms` at `:1041-1050` · `check/join.hero`'s `Branch` · **closed 2026-10-01**

    **Origin:** panel 184's compiler-engineer and lane 135b's batch gate,
    apart, 2026-09-30; widened by the sitting's critic to a `match` inside a
    branch (`docs/panel/184-reports/completeness-critic.md` B1); reproduced by
    the coordinator at 21:21 on the trunk's compiler at `a294a6ff`
    (`scratchpad/p184/matchleave/match-then-falls.hero`, `match-prints.hero`);
    again at 00:17 on 2026-10-01 on the integrated trunk at `3cc3b553`, both
    `check` 0 and `build` 2 on the same clang message.
    A `match` in a VALUE block is not affected (the critic's case c).

    **Why it is a defect.** A program `missing_return` exists to refuse is
    accepted, and the compiler then fails on its own emitted C with exit 2,
    which says the tool is wrong. Panel 184's R4 reads the same predicate and
    waits on this repair.

    **2026-10-01, lane flow, the value half: an arm that ends on a statement,
    in a `match` used as a value, is told, and *every branch jumps* is said
    only where every branch did** (beside the item: `.b => assert false` or
    `.b => n @ 5` there checked at 0 and read the binding uninitialised in
    the C): repaired at `201b99af`, gated by its cases and the compiler's own
    tests; the net is owed at the batch's close. The statement half, the item
    as filed, waits on leave to edit lane 135c's
    `tests/golden/check/fixedbugs-135-a-dropped-value-that-ends-its-function.hero`;
    it repairs too a VALUE block holding a `match` statement, which the first
    pass found affected, against the line above (`x = if` refused with *every
    branch jumps*, or checked clean and built over an uninitialised read).

    **2026-10-01, lane flow, the statement half: a `match` statement leaves
    its block only when every arm does, and the checked `if` asks it too**
    (the item as filed; by the coordinator's leave of 12:00 for lane 135c's
    golden, which gains its `missing_return` under a dated correction):
    repaired at `3a25b640`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-01, lane flow, the class at a constant: a body that ends on a
    statement is told once, and one that jumps is named**
    (`selfhost/check/decls.hero:54`, by the coordinator's leave of 12:00: an
    `assert`, a loop or a `for` last was told twice, the second time *every
    branch jumps*): repaired at `a26448c0`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close. A `break` or
    `continue` there still costs `jump_outside_loop` and the constant's own
    message, both true.

    **The repair**, lane flow, in three commits. **The class**: a join's
    branch carried leaving as an absent value, which a branch that merely
    ended on a statement has too (panel 017 R1's condition, leaving as data,
    had lapsed). So the statement `match` asked whether an arm PRODUCED a
    value, which no arm of a statement does, and read *none did* as *every arm
    jumps*: a function whose body held a `match` statement checked at 0 with
    no `return` and stopped in clang, exit 2, the item as filed, in a branch
    of an `if` too (panel 184's critic, B1). The lane's first pass found the
    class wider than the entry, 76 programs each checked, built and run: in a
    VALUE `match`, a one-statement arm that ends on a statement (`.b =>
    assert false`, `.b => n @ 5`) checked clean and built C that read an
    uninitialised variable, and a value block holding a `match` statement was
    refused with a false *every branch jumps*, or checked clean over the same
    read.

    `201b99af`, the value half: `join.Branch` gains `jumps`, such an arm is
    told `no_value` as a block's last line already is, and *every branch
    jumps* is said only where every branch did. `3a25b640`, the statement
    half: the statement `match` reads `join.every_branch_jumps`, and the
    checked `if` (`x: i64 = if ...`), which never asked, refuses one whose
    every branch returns, as its synthesised twin does; the two cannot land
    apart, measured (the checked `if` alone refused a legal program).
    `a26448c0`, the same class at a `constant` (`selfhost/check/decls.hero:54`):
    a body that ends on a statement was told twice at one place, the second a
    false *every branch jumps*; now only a body that jumps is told there, by a
    message naming the constant. The nesting `check` survives, trunk then
    batch: `if` 192 and 195, `match` 161 and 163, value `if` 123 and 123,
    calls 95 and 96.

    **Cases**: `check/fixedbugs-139-a-value-arm-that-ends-on-a-statement`
    (six messages), `-a-match-statement-leaves-only-when-every-arm-does` (18),
    `-a-constant-whose-body-ends-on-a-statement` (eight constants, one message
    each where the trunk printed 14); `run/fixedbugs-139-value-arms-that-jump-or-give-a-value`
    (21 lines computed by hand), `-match-statements-that-leave` (36; the trunk
    refuses it), `-constants-that-give-a-value` (5, 8, 7; the trunk refuses
    it), each with its blessed C; and `fixedbugs-135-a-dropped-value-that-ends-its-function`,
    written in lane 135b to document this very defect, gains its
    `missing_return` under a dated correction.

    **Left open, beside it, and filed where they belong**: a `break` or
    `continue` as a constant's body keeps `jump_outside_loop` beside the
    constant's message, both true, one mistake two messages; and
    `branch_without_value` points at a multi-line block's first line, not its
    last (`scratchpad/lane-flow/k1/`), lane flow's next batch. A value block
    whose last statement leaves on every path is refused at that statement,
    which is panel 017 R1's reading and a question, in `docs/work/DECIDE.md`.

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
