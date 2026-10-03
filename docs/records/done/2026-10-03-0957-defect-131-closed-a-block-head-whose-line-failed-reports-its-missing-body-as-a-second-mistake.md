# Defect 131 closed: a block head whose line failed reports its missing body as a second mistake

- [x] **131 — a block head whose line failed reports its missing body as a second mistake** | `if n > )` over an indented `print(1)` costs `expected_expression` and then `missing_body` at the same column; so do `else if`, `while` and `for` heads (`missing_body`) and `match )` (`missing_match_arms`); and `if n >` over a deeper line, joined by `continuation_outside_brackets`, is followed by `missing_body` at the next statement | `selfhost/grammar_expr.hero` (the body checks after a block head, `match_expr`'s arms check) · `cursor.at_reported_error` · **class: systemic** · **closed 2026-10-03**

    **Origin:** the coordinator, 2026-09-28 at 22:58, attacking the head of
    the blocks beside defect 130 on the trunk's compiler at `79aeeffa`
    (`w8_cond_error.hero` and `v1` to `v6` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d130/`, 2026-09-28).
    The cause is read and unrun: the guard before `missing_body` and
    `missing_match_arms` is `cursor.at_reported_error`, which asks whether the
    current TOKEN is an `error` token, not whether the head already reported;
    the failed head leaves the cursor on the `)` it could not read.
    `if (n > )` costs one diagnostic (`v6`), because the parenthesis
    recovery consumes the line.

    **Why it is a defect.** design.md §4.17: one mistake, one message. The
    second names a body that is there, one level down, as missing, so a
    model that reads it as a second mistake repairs a block that was right.

    **Widened 2026-09-29 by lane 130's agent, reported beside its repair and
    assigned here by the coordinator**: a `for` with nothing after it costs
    `for_missing_in` and `expected_expression` at the same column
    (`o03_bare_for.hero`), and a body indented with a tab costs
    `tab_in_indentation` and then `missing_body` on the same line, for a
    body that is there (`l06_tab_body.hero`), both in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-130/shapes/`, 2026-09-29.

    **Widened again 2026-09-29 by lane 130's agent at its second round**:
    `record Point )` and `variant Token )` over their bodies are named
    `empty_record` and `empty_variant`, which says the fields are missing
    while they are there (`shapes3/f37`, `f38` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-130/`, 2026-09-29); given to lane 133.

    **Widened again 2026-09-29 by the `fixes` suite lane 129 landed, at the
    coordinator's gate of lane 130's second round (`3ecb7ac9`)**: `for >`
    gets `for_missing_in` with the `certain` fix *use `while`*, and
    `--apply` writes `while >`, which is no loop and costs
    `expected_expression` twice on its one line. Measured on the compiler
    of `a6eab736` as well, so it was there before tonight; a `certain` fix
    that does not repair, and one mistake with two messages. Sent back to
    lane 130.

    **Widened 2026-09-29 by lane 133's agent**: `test`, `function` and
    `constant` heads with junk over a body that is there still name it
    missing, `missing_body` found `)` (`a/fjunk.hero`, `cjunk`, `tjunk`);
    `extern "m.h" )` costs two messages; `fn main() {` costs
    `expected_declaration` again at its `}`; and `empty_variant`'s caret
    stands on the next line's first token where `empty_record` points at its
    own head; all in `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29.

    **Widened 2026-09-29 by lanes 132 and 133 at their last rounds**: a block
    under a line headed by a refused word is named `unexpected_block`
    beside the word's own report, for a block that is its body
    (`stray_elif.hero`, `const_nested_deeper.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-132/findings/`, 2026-09-29; `fn` or `struct`
    nested in a body, `r2/matrix/fn__body_decl.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29); `n: 2 @ 3` costs
    two messages at one token; an arm at its `match`'s own margin costs
    `missing_match_arms` and `expected_end_of_line`; `0` over `=> 5` costs
    two; an opening brace on the line below its head, Allman's style, costs
    three (`r2/brace/allman.hero`); and `parse/records.hero` quotes a head
    across a joined line break in the control arm
    (`record_head_quoted_across_a_join.hero`), all reproduced in the two
    lanes' scratch directories named here.

    **Widened 2026-09-29 by lane X2's agent** (reproducers in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-x2/`, 2026-09-29): a `:` in mid-line,
    `if n > 0: print(1)` or `record P: i64`, is taken for the Python habit
    and its `certain` fix writes a program still refused, the line costing
    two messages (`s6/f01`, `f02`, `f03`); `n: {2: i64}` costs
    `expected_type` twice at one token (`s1/a09`); one missing `]` costs
    `unclosed_bracket` and `expected_array_close` (`s1/a21`); an arm at a
    shallower margin after a dedent, and a `=>` line past the arms' dedent,
    cost two (`s2/b09`, `s3/c20`); junk before a broken arm's line end still
    reports `expected_pattern` at the `=>` (`s3/c19`); and a `=>` alone at a
    line's end over the next arm costs two (`s3/c14`, `c21`).

    **Widened 2026-09-29 by lanes X1 and Y at their reports**: `match x {` arms
    and `0 => {` bodies in braces are misread (`f2*`); mixed style, braces
    outside and indentation inside, names each inner body missing (`f3*`); in
    `} x` the `x` goes unsaid (`f4*`); `do {` ... `} while (c)` costs a second
    `missing_body` (`f5*`); `for`'s certain *use `while`* before `(` or `{`
    writes a program with new parse errors (`f6*`); `let x = if c` over a
    body, then `else`, refuses the `else` (`f8*`); an Allman record's caret
    points at the next line's `{` (`f12*`), all in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-x1/findings/`, 2026-09-29; and `k = match n` over a
    same-margin `0 => 1` costs `expected_end_of_line` at the `=>`
    (`i3/match_n_arm_same.hero`, `i3c/b8.hero` in `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-y/`, 2026-09-29).

    **Batch gate 2026-09-30 00:07** (`53a5e0a9`, lanes X3 and X4 beside the trunk's
    129 to 133, X1, X2 and Y): the full net, every suite at 0 failed. The shapes
    still open are the next batch lane's items, found beside the repairs, and
    this item stays open until they are repaired and a batch gate reads them.

    **2026-09-30, lane recovery-b1, a result type written after `:` or `=>`
    is told as that habit, with the certain fix that writes `->`** (lane X3's
    report): repaired at `cda5ee4c`, gated by its cases and the compiler's
    own tests; the net is owed at the batch's close.

    **Batch gate 2026-09-30 01:48** (`1fc77e31`, lane recovery-b1, five
    repairs): the full net, 3852 passed and 0 failed over 26 suites, the
    compiler's own 903 and the net's own 184; then the compiler's 903 and 19
    suites at 0 failed on Linux x86-64, Linux arm64 and the Windows box, whose
    `run` read 160 and 45 with its C: drive at 99% and 205 and 0 re-run alone
    with 16 GB freed. The shapes still open are lane recovery-b2's items.

    **2026-09-30, lane recovery-b2, a group, a call or an index whose opener
    the lexer named never closed ends at its line and names no closer**
    (lane recovery-b1's report, U4): repaired at `de991c25`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane recovery-b2, braces the lexer named never closed end
    where it named them, and a brace habit's `{` is told once** (lane
    recovery-b1's report, U1): repaired at `48a433f4`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane recovery-b2, a literal that never closed takes its
    line's closers, and `'\'` is fixed without the rest of its line** (lane
    recovery-b1's report, U3): repaired at `6795bedd`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane recovery-b2, a head that left a bracket open is told
    once, and the lines it swallowed are no body to be named missing** (lane
    recovery-b1's report, U5): repaired at `ac62cde9`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **Batch gate 2026-09-30 04:45** (`e5cc73eb`, lane recovery-b2, five
    repairs and a verification case): the full net, 3873 passed and 0 failed
    over 26 suites, the compiler's own 911 and the net's own 184, `corpus` red
    once in the parallel pass on defect 134's false `cannot publish` and 55
    and 0 alone; the census of 1358 files moved the six new cases and nothing
    else. The shapes still open are the next batch's items.

    Linux x86-64 at `e5cc73eb` (the `heroes-linux` image), measured
    2026-09-30 by 06:21: the compiler's 911 tests and 19 suites, each 0
    failed.

    **Widened 2026-09-30 by panel 183's completeness critic, a regression of
    batch 3's**: `52b2d378`, which removed `cursor.recover_past_closer`,
    raises the diagnostics on 42 of 3,000 single missing closers from 101 to
    253 and `missing_body` from 14 to 142, on bodies that exist (the critic
    built `52b2d378` and its parent over the same 3,000; one example read,
    the other 41 counted; `docs/panel/183-reports/completeness-critic.md`
    § 1). This item's own class; lane recovery-b4's item C4r.

    **2026-09-30, lane recovery-b4, a sigil before a name is one message at
    its `@`, and the line is read as the statement it is with the sigil gone**
    (the recovery instrument's `at-prefix`): repaired at `4441148b`, gated by
    its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **2026-09-30, lane recovery-b4, a head inside a bracket left open above
    it names no missing body, and an `else` there is its `if`'s** (panel
    183's critic, the regression of `52b2d378`): repaired at `3a93d155`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

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

    **2026-10-01, lane recovery-b5, an indentation habit is one message, and
    its lines are laid out as its fix writes them** (ruling 5, the cluster's
    C1): repaired at `4f0db097`, gated by its cases and the compiler's own
    tests; the net is owed at the batch's close.

    **2026-10-01, lane recovery-b5, one closer or one quote left out costs one
    message, and the opener a missing closer was for is the one named** (the
    cluster's C3): repaired at `df2e13ca`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **Batch gate, lane recovery-b5, 2026-10-01 23:00** (`a8b04ea8`, C1, C2, C3
    and C5 with `538d70ac`; defect 146 closed by the same gate): the seed
    regenerated once, the fixpoint by `cmp`; the compiler's own tests 993 and
    the net's own 184, all passed; the full net 4,240 passed and 0 failed;
    the census over 1,530 files, every move the batch's own; the recovery
    instrument EXTRA 850 to 532 (`indent-2` and `indent-tab` 220 to 0,
    `bracket-open` 44 to 7, `string-open` 28 to 2), ONE 12,427 to 12,745,
    HIDDEN pairs 16 to 12, no class risen; Linux x86-64, 993 tests and 19
    suites at 0 failed. The trunk fast-forwarded to it at 00:09 on
    2026-10-02. The item stays open for the shapes `scratchpad/lane-recovery-b5-items.md`
    § Queued and `scratchpad/close/next-batches-1001.md` name.

    **2026-10-03, lane rec187, panel 187's R1: the fifteen rows close into
    six items and a pin** (the compiler engineer's § 1, each item carrying
    its pin under `tests/golden/check/panel-187-*`): 131-33a, 54a and 54b
    into defect 194; 131-55a and 55b into 195; 131-56a and 56b into 196;
    131-41a into 197; 131-16a, 16b, 53a and 53b into 198; 131-32 into 201.
    131-52a is three messages for three edits by ruling 1, pinned by
    `panel-187-a-variants-cases-in-braces-cost-three-edits`; 131-22 is two
    messages for two mistakes in the normal arm, its control-arm message,
    which quotes `record x` across the break, unpinnable while no golden
    form runs `--permissive` (the sitting's R3). The item stays `- [ ]` here;
    the round's gate closes it.

    **Class: systemic**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): four batches
    (recovery-b1, b2, b4, b5) and 15 rows open on `2bb45a96` by lane
    recovery-b8's count; to the same sitting as 130.

    **Closed 2026-10-03** by panel 187's R1, ratified that morning (the author's answer *1a*): its fifteen rows each repaired, filed `adjacent` apart one item per cause (194 to 201), or pinned as a known cost in the ten `panel-187-*` goldens (`3bf8e348`), as lane rec187's dated line above records; the landing gated at the round of 2026-10-03's gate `b43d224d` (the seed's fixpoint by `cmp`, the compiler's own tests 1,079, the net's own 200, the full net 4,901 passed and 0 failed over 26 suites), and the recovery instrument's reading of that round's compiler against the trunk's over the frozen plan showed no mutant worse for the recovery (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03).
