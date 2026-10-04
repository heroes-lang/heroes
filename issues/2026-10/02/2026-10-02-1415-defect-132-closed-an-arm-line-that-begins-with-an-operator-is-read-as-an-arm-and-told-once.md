---
kind: defect
area: compiler
milestone: none
filed: 2026-09-29
commit: 435201935c2adf4afdd5610b10335b213e8c1239
github: none
---

# Defect 132 closed: an arm line that begins with an operator is read as an arm, its operator told once, and never joined to the line above

- [x] **132 — a join into a line that holds a `match` arm's `=>` costs the break a second diagnostic** | inside `match n`, `0 => 5 -` over `1 => 10`, and `0 => 5` over `| 1 => 10`, each draw `continuation_outside_brackets` at the break and then the parser's `expected_end_of_line` at the next arm's `=>`, since the joined line, `0 => 5 - 1 => 10` or `0 => 5 | 1 => 10`, holds two arms | `selfhost/open_line.hero` (the join handed to the parser) · `selfhost/grammar_expr.hero` (`finish`, `match_expr`) · **closed 2026-10-02**

    **Origin:** lane 129's agent, 2026-09-29, beside defect 129 (its finding 2,
    `cascade_next_arms_pattern.hero`, copied to
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d132/`, 2026-09-29);
    reproduced by the coordinator on the trunk's compiler at `7d3cd33b`: two
    breaks, four diagnostics, and on the trunk both joins `certain`, writing
    programs that do not parse (defect 129's class, which lane 129 repairs; the
    second diagnostic stays after that repair, by the lane's own report).

    **The shape, whole, since the file above lives in a session's scratchpad:**
    a function `return match n` whose arms are `0 => 5 -` / `1 => 10` /
    `_ => 20`, and one whose arms are `0 => 5` / `| 1 => 10` / `_ => 20`.

    **Why it is a defect.** Panel 181 item 2: one diagnostic per break, the
    parser handed the line the author broke. Here the line it is handed holds
    the next arm's `=>`, which no join can make part of an arm's body, so the
    parser's refusal is the break's debris and not a second mistake.

    **Widened 2026-09-29 by lane 130's agent, assigned here by the
    coordinator**: an arm with nothing after its `=>`, over the next arm at
    the same margin, is joined by the lexer and the parser then refuses the
    joined `=>`, two messages for one mistake (`o05_empty_arm.hero` and
    `s12`, in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-130/shapes/`, 2026-09-29).

    **Widened again 2026-09-29 by lane 130's agent at its second round**: the
    lexer joins a line into a block head and the parser reports the join's
    debris, `for` alone over a deeper `print(1)` costing
    `continuation_outside_brackets` and `for_missing_in` (`bf.hero`),
    `record`, `variant` and `constant` alone at a line's end (`shapes2/f18`,
    `f19`, `f14`), and `function` alone over a tab-indented body
    (`shapes3/t19`), all in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-130/`, 2026-09-29.

    **Widened 2026-09-29 by lane 133's agent**: the lexer does not take a
    refused word as the start of a fresh line, so such a line joins an open
    line above it and the same mistake takes another code once the swap is
    applied (`next/ctype_then_const.hero`, `field_then_struct.hero`,
    `use_then_import.hero`, `more/open_line_fn2.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29).

    **Widened 2026-09-29 by lane 133 at its last round**: `include <stdio.h>`
    leaves its line open at the `>`, so the next line costs
    `continuation_outside_brackets` beside `reserved_word` (the case it was
    cut from is `fixedbugs-133-a-swap-certain-only-where-its-word-is-right`).

    **Widened 2026-09-29 by lane X2's agent**: a join into a `match` whose arm
    has a bad pattern costs `missing_match_arms` and `expected_pattern` at
    one token in the normal arm (`s2/b13` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-x2/`, 2026-09-29).

    **Batch gate 2026-09-30 00:07** (`53a5e0a9`, lanes X3 and X4 beside the trunk's
    129 to 133, X1, X2 and Y): the full net, every suite at 0 failed. The shapes
    still open are the next batch lane's items, found beside the repairs, and
    this item stays open until they are repaired and a batch gate reads them.

    **2026-09-30, lane recovery-b4, a `\` at a line's end is one message, and
    the line it asks for is joined and read** (the recovery instrument's
    `backslash`): repaired at `81348c99`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane recovery-b4, a `,` after every member is one message
    for its declaration, and the line ends at it** (the recovery instrument's
    `field-commas`): repaired at `68e46a13`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **Batch gate, lane recovery-b4, 2026-09-30 20:12** (`84430015`, the trunk
    `cf943ddf` merged at `acd80b8b`): the seed regenerated once, the fixpoint
    by `cmp`; the compiler's own tests 934 and the net's own 184, all passed;
    the full net 3,970 passed and 0 failed over 26 suites (`probe` red once on
    a timeout, 24 and 0 alone); the census over 1,412 files moving only files
    added since `77b8ca98`; the recovery instrument against batch 3: ONE
    11,955 to 12,448, EXTRA 1,336 to 861, ELSEWHERE 32 to 14, APPLY-NEW and
    APPLY-OTHER 0, no class risen for any operator; Linux x86-64 on
    `84430015`, 934 tests and 19 suites at 0 failed. **Integrated** with lanes
    136 and 135b at `3cc3b553`, the trunk fast-forwarded to it at 00:03 on
    2026-10-01 (the full net 4,023 and 0, Linux x86-64 3,899 and 0; defect
    136's record carries the integration). The item stays open for the shapes
    the audit of 2026-09-30 found open (`scratchpad/audit-130-133/`).

    **2026-10-02, lane recovery-b7, a line that holds an arm's `=>` below an
    arm or its `match` is an arm, its leading operator told once and the arm
    read past it, and an alternation broken at a match's margin is read as
    one arm** (the audit's 132-06, lane X2's `b13`; beside it `| 1 => 10`,
    `* 2 => 3`, `? =>` at the margin and below a whole arm, and `0` over
    `| 1 => 10` at the margin): repaired at `4c893c14`, gated by its cases
    and the compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane recovery-b7, `4c893c14`. **The class**, wider than
    the audit's `b13`: an arm line that begins with an operator (`+ =>`, `| 1
    =>`, `* 2 =>`, `? =>`, `::x =>`) under an arm or its `match` was joined to
    the line above, which cost an extra `expected_pattern` at the `=>`, hid
    the margin message and hid the arm body's own mistake; beside it, an
    alternation broken across lines at a match's margin cost a third
    message. Now such a line is never joined (`line_above.opens_an_arm`), its
    operator is told once and the arm is read past it
    (`parse/operator_arm.hero`), and at a match's margin a line holding only
    patterns over an arm is read as an arm (`parse/margin_arms.hero`).
    **Cases**: `check/fixedbugs-132-an-arm-that-begins-with-an-operator` and
    `-an-alternation-broken-at-the-margin-of-its-match`, and two earlier ones
    moved with a dated note each. The audit's 87 rows of 132 and 133 re-run,
    base against lane: only the intended rows moved. **Ruled by the
    coordinator**: `132-06` (`b13`) now costs `missing_match_arms` and
    `expected_pattern` at one `+`, two mistakes each told once with no debris,
    as `b17` already is on the trunk; read as closed.

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
