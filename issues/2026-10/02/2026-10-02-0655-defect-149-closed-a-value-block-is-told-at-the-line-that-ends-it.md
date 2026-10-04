# Defect 149 closed: a value block is told at the line that ends it, and a constant at the statement it leaves by

- [x] **149 — a constant's body that ends on a statement is told at the block's first line, not at the line that gives no value** | `constant M: i64` over `x: i64 @ 1`, `x @ x + 1`, `assert x == 2`: `no_value`, *this branch ends on a statement*, at 2:5, where the line that ends the body without a value is line 4 | `branch_without_value` (`selfhost/check/walk.hero`, `check/join.hero`), pinned by `tests/golden/check/fixedbugs-139-a-constant-whose-body-ends-on-a-statement` · **closed 2026-10-02**

    **Origin:** lane flow's first pass, 2026-10-01
    (`scratchpad/lane-flow/k1/k21-two-statements-then-assert.hero`), left
    open in defect 139's closed record; reproduced by the coordinator at
    00:35 on 2026-10-02 on `b9fdb0a3`. The same caret is the rule's for any
    multi-line value block, by the lane's reading, unrun beyond the constant.

    **Why it is a defect.** design.md §4.17: a diagnostic stands at the
    mistake; the author is sent to a line that is right.

    **2026-10-02, lane checker, a value block is told at the line that ends
    it, and a constant at the statement it leaves by** (every value block of
    more than one line, and beside it the constant's own message at its
    body's first line; `fixedbugs-139-a-constant-whose-body-ends-on-a-statement`
    moves under a dated correction): repaired at `f5e239bd`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane checker, `f5e239bd`. **The class**: *this branch
    ends on a statement* took the block's span, which begins at its first
    statement, so every value block of more than one line (a constant's body,
    a branch of a value `if` and its `else`, the checked `if`, a `match` arm's
    block in both forms, a nested `if`, `return if`) sent the author to a line
    that is right; and the constant's own *its body jumps before it gives
    one* stood at the body's first line too. Now the first stands at the
    block's last statement (`walk.ended_on_a_statement`, outside `block`'s
    frame), and the second at the first statement through which the body left
    (`decls.constant_body`); a route that carried the statement through
    `block` on the checker cost one to four levels of nesting and was
    dropped.

    **Cases**: `check/fixedbugs-149-*`, 13 and 3 messages, every multi-line
    one at a line the trunk's compiler did not name; defect 139's constant
    case moves its multi-line message from 29:5 to 31:5, the `assert`, under
    a dated correction below its program.

    **The gates.** Each repair by its cases and the compiler's own tests, and
    the census against the compiler before it, only the repair's own cases
    moving. **The batch gate**, lane checker's closing commit `a09d9bf1` (with
    C3 of defect 135, which stays open), the trunk `442c822d` merged at
    `571c113a`: the seed regenerated once, 35,263,204 bytes, SHA-256
    beginning `470a76b52e7b97be`, its fixpoint by `cmp`; the compiler's own
    tests 1,002 and the net's own 184, all passed; the full net, 25 suites four
    at a time and `cache` alone, 4,291 passed and 0 failed, none red, no floor
    asked; `tests/emission` unchanged; the census over 1,630 files, both
    arms, 7 and 5 moved, every one the batch's own or defect 139's case, and
    `docs/panel/184-briefs/blind/task3b.hero` from exit 134 to 0, C3's repair
    having raised the deepest call nesting `check` survives from 96 to 101.
    **Linux x86-64**, the container `lane-checker-x86` on the closing tree,
    read from `docker logs`: 1,002 tests, all passed, and the 19 suites 4,144
    passed and 0 failed. The trunk fast-forwarded to `a09d9bf1` at 06:50 on
    2026-10-02. **Owed before the push**: Linux arm64 and the Windows box.
