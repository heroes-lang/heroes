# Defect 148 closed: a claim that rests on a jump already refused is the jump's shadow, and is not told

- [x] **148 — `break` or `continue` as a constant's body costs two messages at one place** | `constant M: i64` over `break`: `jump_outside_loop` and `no_value` (*constant `M` produces no value — its body jumps*), both at 2:5, for one mistake; the same with `continue` | `selfhost/check/decls.hero` (the constant's message, defect 139's last repair) · the checker's outcome, which does not carry the jump's kind · **closed 2026-10-02**

    **Origin:** lane flow's first pass for its batch, 2026-10-01
    (`scratchpad/lane-flow/k1/k10-break.hero`, `k11-continue.hero`), left
    open in defect 139's closed record; reproduced by the coordinator at
    00:35 on 2026-10-02 on the trunk at `b9fdb0a3`.

    **Why it is a defect.** design.md §4.17: one mistake costs one message;
    both sentences are true, and the second is debris of the first.

    **2026-10-02, lane checker, a claim that rests on a jump already refused
    is the jump's shadow, and is not told** (the item, and beside it *every
    branch jumps* where a value `if` or `match`, in a constant or a function,
    left by such a jump): repaired at `6113bcab`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane checker, `6113bcab`. **The class**: `constant M:
    i64` over `break` or `continue` cost `jump_outside_loop` and the
    constant's *its body jumps before it gives one* at one place, one mistake
    told twice; and the lane's first pass found it wider, an `if` or a `match`
    used as a value, synthesised or checked, in a constant or a function,
    every branch of which left by such a jump, or by one beside a `return`,
    told *every branch jumps* beside the jumps' own messages. The checker's
    outcome said only whether control left. Now how it leaves is data with
    three answers (`join.Leaves`: falls, jumps, refused): whatever asks
    whether control goes on reads a refused jump as leaving, as before, and a
    claim to the author speaks only where no refused jump made the construct
    leave. A block leaves the way its first leaving statement does, so
    `return 5` then `break` is told twice, two mistakes. The jump's message
    moved out of `statement`'s frame, and the deepest nesting `check` survives
    rose (`if` 195 to 204, `match` 163 to 170, value `if` 123 to 126).

    **Case**: `check/fixedbugs-148-a-claim-that-rests-on-a-jump-already-refused`,
    23 messages where the trunk's compiler printed 35. **Left, a reading the
    lane chose and named**: in a constant, `if true` / `break` / `else` /
    `return 1` then `5` no longer gets the constant's message, one branch
    having left by a refused jump (`scratchpad/lane-checker/p2k/m07_const_stmt_if_refused_then_value.hero`).

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
