---
kind: defect
area: compiler
milestone: none
filed: 2026-10-02
commit: 435201935c2adf4afdd5610b10335b213e8c1239
github: none
---

# Defect 154 closed: a based literal with a fraction is told as the number it is

- [x] **154 — a hexadecimal number with a fraction is told as a field access** | `x = 0x1.5`: `error[expected_field_name]: expected a field or function name after `.`, found a number (`5`)`, a member-access message for a literal | `selfhost/number.hero` · `selfhost/grammar_expr.hero` (the `.` after a literal) · **closed 2026-10-02**

    **Origin:** lane literals' first pass, 2026-10-02
    (`scratchpad/lane-literals/pass1/L5/t5_hex_fraction_lower.hero`); reproduced by
    the coordinator at 02:42 on `03e70520`.

    **Why it is a defect.** design.md §4.17: the message names a mistake the
    author did not make; the number's own form is the mistake.

    **2026-10-02, lane arm: a `.` and a decimal digit after a based literal's
    digits are its fraction, told once as `digit_not_in_base`, the literal
    one token, with no fix** (beside it `0x.5`, `0x1.5e3`, `0x1.5.2`, C's
    `0x1.8p-3`, `0xG.5`, an uppercase prefix before one): repaired at
    `4574e5d3`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close. Left: `1.5.2`, a decimal float with a second
    point, still told as a field access, since no code's documented meaning
    covers it (lane arm's report).

    **The repair**, lane arm, `4574e5d3`. After a based literal's digits, a
    `.` followed by a decimal digit is read as part of the literal: one error
    token and one `digit_not_in_base` diagnostic with no fix, `0x1.5` having
    three readings; number.hero's third rule is that code's documented
    meaning (a character that cannot continue a based literal ends it with a
    diagnostic). A letter after the point is still a field or a call
    (`0xa.b`). **Case**: `check/fixedbugs-154-a-based-literal-with-a-fraction`.
    **Left, queued**: `1.5.2`, a decimal float with a second point, still told
    as a field access, no existing code covering it.

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
