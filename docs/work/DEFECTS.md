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
**OPEN: 14**

- [ ] **130 — after a `match` whose arm fails to parse, the next statement is skipped whole, and every mistake in it goes unreported** | in `function main()`, three bound matches `a = match n`, `b = match n`, `c = match n`, each with the arms `+ => 10` and `_ => 20`, report lines 4 and 10 and never line 7; two statement matches `match n` over `+ => print(1)` report only the first; and a plain statement after one such match, `y = 3 )` over `z = 4 )`, reports the second line and not the first, where the same two lines after no `match` report both | `selfhost/grammar_expr.hero` (`match_expr`) · the enclosing statement's recovery

    **Origin:** lane 123's agent, 2026-09-28, beside defect 124, on the
    trunk's compiler at `aee8b01e` (reproducers `shapes/y01_three_bad.hero`
    and `shapes/y02_stmt_bad.hero` in `/Users/joseph/Temp/heroes-lane-123-scratch/`);
    reproduced by the coordinator on the trunk's compiler at `d0f24496` at
    18:47, who widened it from a second `match` to any statement with
    `z2_two_plain_errors.hero`, beside the control `z4_no_match.hero`
    (`/Users/joseph/Temp/heroes-recovery-2026-09-26/defect130/`, 2026-09-28).
    The cause is unrun and inferred by the finder from the reading:
    `match_expr` skips the failed arm's line, and the enclosing statement's
    own recovery then drops one more line.

    **Why it is a defect.** The program is refused, so nothing runs wrong,
    but a mistake is present and unreported: a silence where a message is
    owed. design.md §4.17 asks that a model fix a program in one turn; a
    mistake hidden behind another costs a second one, and a third where two
    are hidden in a row.

    **Carried to the next milestone**, by the author's instruction of
    2026-09-28 16:40 (`docs/records/log/2026-09-28-1640-m-agreed-retention-closes-over-the-defects-found-after-its-last-eight.md`):
    found after the eight M-agreed-retention repairs before its tag, and
    not repaired in it.

    **Widened 2026-09-28 at 23:00 by the coordinator, on the trunk's compiler
    at `79aeeffa`: the class is every statement that ends with its own block,
    and the cause is now read, not inferred.** The reproducers were rebuilt
    from this entry's text, the directory above being gone by 22:55
    (`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d130/`, 2026-09-28),
    and they print what the entry says. `grammar_expr.finish` skips the rest
    of the line of any statement that reported a diagnostic, and a statement
    whose last part is a block has already consumed its own dedent, so the
    line it skips is the next statement's, and `balanced_block` then takes
    that statement's block. The skip is keyed on a diagnostic anywhere inside
    the statement, so the arm is one witness of many: an error in the body of
    a `for` (`w1`), an `if` (`w2`), a `while` (`w3`), an `else` (`w4`), a
    bound `if` (`w5`) or an arm's block (`w7`) hides the statement after it,
    and two nested blocks hide two (`w6`, lines 6 and 7). A declaration's
    body does not (`w9`): `cursor.recover_to_next_decl` asks whether the
    cursor already stands at a fresh line, which `finish` never asks.

    **Corrected 2026-09-28 at 23:17**: the directories were removed by the
    author, who took them for old lanes, and restored from the trash the
    same evening, so the paths above resolve again; the copies in the
    scratchpad are what the lanes read.

    **Widened 2026-09-29 by lane 130's agent, reported beside its repair and
    assigned here by the coordinator**: a statement that failed drops the
    over-indented block hanging below it silently, the mistakes inside it
    too, where after a statement that did not fail the same block is
    `unexpected_block` (`o01_failed_then_orphan.hero` against
    `o02`, in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-130/shapes/`, 2026-09-29).

    **Widened again 2026-09-29 by lane 130's agent at its second round, and
    assigned by the coordinator**: under an arm written inline, `0 => print(1)`
    over a deeper `print(99)` compiled and ran and printed 1, the deeper line
    thrown away at exit 0, a wrong answer and not only a silence (repaired
    in lane 130, `47849cc9`); and a misspelled keyword at a top-level head,
    `recrod Point` over its fields, drops its whole block silently (given to
    lane 133, whose files it is in; reproducers in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-130/`, 2026-09-29).

    **Widened 2026-09-29 by lane 133's agent**: on the compilers of
    `a6eab736` and `7c0cb024` the parser HUNG, forever, on a line headed by a
    refused word after an `if`, `while` or `for` block (`elif`, or `let`
    after an `if`); lane 130's merge `0ba7b084` ended it, measured at exit 1,
    and a case per shape is owed (`hang/o.hero`, `y3`, `y4`, `y8`, `r`, `s`);
    and a bodiless or empty declaration above a line opened by a guidance
    word (`class`, `let`, `€`) is never told, before or after `--apply`
    (`next/fbody_class.hero`); both in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29.

    **Widened 2026-09-29 by lanes 132 and 133 at their last rounds**: the
    fields and cases inside a record's or variant's braces are skipped, so
    their own mistakes wait for the braces to go; a nested `if x == 1 {`
    inside a braced body loses its first line (`r2/brace/nested.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29); and
    `for` over a same-margin `print(1) )` hides the `)`, where `if` alone over
    the same line does not name its missing body (lane 132's report).

    **Widened 2026-09-29 by lane X2's agent**: `extern` alone over
    `"m.h" )` never reports the `)` in the normal arm, the second reading
    holding it (`s4/d13` in `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-x2/`, 2026-09-29).

    **Widened 2026-09-29 by lanes X1 and Y at their reports**: a braced record,
    variant or `extern` group still goes whole, so a mistake inside is never
    reported (`f1*`); a block under a refused line that cannot open one
    (`let x = 5`, `import geom`) is read silently where it is a mistake of its
    own (`f7*`); a block of declarations under a refused head goes whole
    (`f9*`), all in `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-x1/findings/`, 2026-09-29; a
    missing body is not told when the line below fails the joined head, `if`
    over `print(1 +)` (`i3d/c1.hero`, `c2`, `c3`, `i3b/a13.hero`); and an
    unexpected block is dropped whole with its mistakes unsaid, `x = 1` over a
    deeper `print(1) )`, where with `x = 0X10 +` above it the certain fix is
    not a subset (`i4/e8.hero`, `e12.hero`), both in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-y/`, 2026-09-29.

    **Batch gate 2026-09-30 00:07** (`53a5e0a9`, lanes X3 and X4 beside the trunk's
    129 to 133, X1, X2 and Y): the full net, every suite at 0 failed. The shapes
    still open are the next batch lane's items, found beside the repairs, and
    this item stays open until they are repaired and a batch gate reads them.

    **2026-09-30, lane recovery-b1, a member's line ends after its member**
    (lane X4's finding n1, `dark grey` compiled as two cases): repaired at
    `f17bbe90`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane recovery-b1, a case's fields in braces are read as its
    fields** (lane X4's finding n2): repaired at `5cdc7288`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane recovery-b1, a refused literal is read as the literal it
    stands for at any depth** (lane X4's finding n4): repaired at `f4e991da`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **2026-09-30, lane recovery-b1, an orphan block that goes on with the line
    above is named once and read** (lane X4's report, the word-then-value
    rule): repaired at `2fd79070`, gated by its cases and the compiler's own
    tests; the net is owed at the batch's close.

    **Batch gate 2026-09-30 01:48** (`1fc77e31`, lane recovery-b1, five
    repairs): the full net, 3852 passed and 0 failed over 26 suites, the
    compiler's own 903 and the net's own 184; then the compiler's 903 and 19
    suites at 0 failed on Linux x86-64, Linux arm64 and the Windows box, whose
    `run` read 160 and 45 with its C: drive at 99% and 205 and 0 re-run alone
    with 16 GB freed. The shapes still open are lane recovery-b2's items.

    **2026-09-30, lane recovery-b2, a declaration opening its line at column 0
    names the brackets a mistake left open** (lane recovery-b1's report, U2):
    repaired at `41807577`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-09-30, lane recovery-b2, an orphan under a whole arm that holds its
    own mistake** (lane X4's n3, measured closed by lane recovery-b1): read
    closed on this tree, and its case added at `04b51f08`; lane X3's 6a reads
    closed and had its case. The net is owed at the batch's close.

    **Batch gate 2026-09-30 04:45** (`e5cc73eb`, lane recovery-b2, five
    repairs and a verification case): the full net, 3873 passed and 0 failed
    over 26 suites, the compiler's own 911 and the net's own 184, `corpus` red
    once in the parallel pass on defect 134's false `cannot publish` and 55
    and 0 alone; the census of 1358 files moved the six new cases and nothing
    else. The shapes still open are the next batch's items.

    Linux x86-64 at `e5cc73eb` (the `heroes-linux` image), measured
    2026-09-30 by 06:21: the compiler's 911 tests and 19 suites, each 0
    failed.

    **2026-09-30, lane recovery-b3, past a closer of another kind the parser
    resumes where the lexer closed the bracket, and an extern group whose
    header failed has its signatures read** (the recovery instrument's
    `wrong-closer`, `single-quotes` and `string-open` as first mistakes):
    repaired at `52b2d378`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **Batch gate 2026-09-30 09:38** (`77b8ca98`, the merge gate of lane
    recovery-b3's batch, closed at `46846f97`, beside defect 134): the full
    net 3907 passed and 0 failed with no suite re-run, the census moving the
    lane's ten new cases and nothing else, and the recovery instrument over
    its 13,594 planted mistakes reading ONE 11,933, EXTRA 1,326, APPLY-NEW 0,
    APPLY-OTHER 0 and 17 hidden second mistakes where the baseline read 153;
    then Linux x86-64, 930 tests and 19 suites at 0 failed by 11:40, and the
    Windows box at `6d781be1`, the same code, 930 and 19 at 0 failed by 11:42.
    Before the push, Linux arm64 at `ec1fd0a9`, the same code again, 930 and
    19 at 0 failed by 14:46.

    **2026-09-30, lane recovery-b4, the reach of a bracket left open ends at
    a line no bracket can hold, at its statement's margin** (panel 183 R1,
    ratified): repaired at `e5076b57`, gated by its cases and the compiler's
    own tests; the net is owed at the batch's close.

    **2026-09-30, lane recovery-b4, inside a `(` left open, a line that opens
    with a name after a kept line end ends the reach** (panel 183 R2,
    ratified): repaired at `ab36aa61`, gated by its cases and the compiler's
    own tests; the net is owed at the batch's close.

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

- [ ] **131 — a block head whose line failed reports its missing body as a second mistake** | `if n > )` over an indented `print(1)` costs `expected_expression` and then `missing_body` at the same column; so do `else if`, `while` and `for` heads (`missing_body`) and `match )` (`missing_match_arms`); and `if n >` over a deeper line, joined by `continuation_outside_brackets`, is followed by `missing_body` at the next statement | `selfhost/grammar_expr.hero` (the body checks after a block head, `match_expr`'s arms check) · `cursor.at_reported_error`

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

- [ ] **132 — a join into a line that holds a `match` arm's `=>` costs the break a second diagnostic** | inside `match n`, `0 => 5 -` over `1 => 10`, and `0 => 5` over `| 1 => 10`, each draw `continuation_outside_brackets` at the break and then the parser's `expected_end_of_line` at the next arm's `=>`, since the joined line, `0 => 5 - 1 => 10` or `0 => 5 | 1 => 10`, holds two arms | `selfhost/open_line.hero` (the join handed to the parser) · `selfhost/grammar_expr.hero` (`finish`, `match_expr`)

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

- [ ] **133 — after a declaration head the lexer refuses as a foreign word with a certain swap, the declaration's own mistakes are reported only once the swap is applied** | `const MAX = 5` costs `reserved_word` alone, and `check --apply` writes `constant MAX = 5`, which then costs `expected_constant_type`; `const MAX: i64 = 5` then costs `missing_body`, `fn main() {` then `missing_body`, `struct Point {` then `empty_record`: two turns for one habit | `selfhost/scan.hero` (the foreign word emitted as an `error` token) · `selfhost/keywords.hero` (`foreign_word`) · `selfhost/parse/decl.hero` · `cursor.recover_to_next_decl`

    **Origin:** lane 129's agent, 2026-09-29, reading `check/reserved-words`'s
    applied text while building the `fixes` suite's judgement of cases with no
    `.fixed` (`85066233`, reported and not changed); widened by the
    coordinator on the trunk's compiler at `ee1b648b` to every declaration
    keyword a swap names (`const.hero`, `const_typed.hero`, `fn_brace.hero`,
    `struct_brace.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d133/`, 2026-09-29).
    Beside it and not of it: every foreign word the LEXER refuses is reported
    in the first run (`fn add(a: int, b: int) -> int` costs four, clean after
    `--apply`), and `let`, whose repair is guidance, has no certain swap.
    The cause is read and unrun: the lexer emits the word as an `error` token
    (`state.error_token`), and at a top-level line's head the parser's
    recovery drops the whole declaration, its body included.

    **Why it is a defect.** The same as 130's: a mistake present and
    unreported is a silence where a message is owed, and design.md §4.17 asks
    that a model fix a program in one turn. `const MAX = 5`, `fn main() {`
    and `struct Point {` are the habits of the three languages a model has
    read most.

    **Widened 2026-09-29 by lane 133's agent, after its repair landed at
    `6f3d31d6`**: (i) a certain swap that is wrong where it stands, which
    `--apply` writes into a program still refused: `function fn(` to
    `function function(`, `record struct`, `const x = 5` in a body to
    `constant x = 5`, `fn` or `struct` nested in a body, `enum` in a group, a
    same-line `record P fn`, and `include "stdio.h"` to `use`, whose message
    then points at `stdio.h.hero` (defect 015's class of the byte after a
    word, one position further); (ii) `elif` is not read as `else if` (it
    costs `reserved_word`, `unexpected_block` and `expected_expression`) and
    `switch` not as `match`; (iii) `fn(i64) -> i64` without its parentheses
    is silent until the swap; reproducers `shapes/`, `more/`, `a/*swap*`,
    `final/elif_ok.hero`, `shapes/elif_brace.hero`,
    `shapes/switch_expr.hero`, `more/fntype_noparens.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29.

    **Batch gate 2026-09-30 00:07** (`53a5e0a9`, lanes X3 and X4 beside the trunk's
    129 to 133, X1, X2 and Y): the full net, every suite at 0 failed. The shapes
    still open are the next batch lane's items, found beside the repairs, and
    this item stays open until they are repaired and a batch gate reads them.

- [ ] **135 — a `certain` fix chosen from one reading, where another is as likely, writes a program that means something else or is refused anew** | `print("\(n)")` costs `unknown_escape` with the certain fix `\\(`, which checks clean and prints the hole's text where Swift's author meant its value; `True` costs `unknown_name` with the certain rename to the one in-scope name within two edits, `run` or `Value`, which then costs `type_mismatch`; a `,` left out before a mutable argument, `hints: nullptr @res`, costs `misplaced_mutable_marker` with the certain `: ` that writes `nullptr : @res`, refused anew | `selfhost/literals.hero:60` (the escape's fix) · `selfhost/resolve/errors.hero` (`suggest`, `nearest`) · `selfhost/grammar_expr.hero` (`misplaced_mutable_marker`)

    **Origin:** the coordinator's measurement lane, 2026-09-30, the parser
    recovery instrument over 641 programs at `c85bccb8`
    (`scratchpad/instrument/baseline-c85bccb8/`): of 2,630 texts `--apply`
    changed, 17 checked clean and meant something else and 23 carried a code
    the first run did not report. The first three shapes below reproduced by
    the coordinator at 03:48 on the trunk's compiler; the fourth is read from
    the instrument's record and not re-run.

    **Measured.** `interp-swift`: 17 of 25 mutants apply into a program that
    checks clean and prints `\(` and the hole's text, and 6 into one refused
    anew. `python-bool`: 13 of 152 renames of `True` or `False` pick a record, a
    variant or a function (`did you mean run?`, `fix (certain): rename to
    run`, then `expected bool, found (function(str) -> ())`). `missing-comma`:
    3 of 153. And one rename of a one-letter name to the only other one-letter
    name in scope (`p` to `s`), where a line over-indented had taken `p` out of
    scope, which then costs `not_mutable`.

    **The cause, read and not yet proved by a repair.** Each fix encodes one
    reading of the mistake where another is as likely, so it is not certain in
    the sense `.claude/rules/diagnostics-and-goldens.md` gives the word, a fix
    that repairs the defect the diagnostic names, compiling not being the bar.
    `\(` followed by a bracketed expression reads as Swift's interpolation as
    much as a literal backslash; `nearest` counts any one candidate within two
    edits (one below four letters) as certain, and the literals `true` and
    `false` are not among its candidates, so `True`, two edits from `run`,
    renames to it; a name and `@` with no `,` between them
    reads as a missing `,` as much as a misplaced `@`.

    **Why it is a defect.** `check --apply` machine-applies a certain fix, so a
    wrong one is written into the author's program without a reader, and in
    the first shape the result compiles and prints the wrong thing: defect
    120's class (a certain `_ = ` that compiled and printed 100 where 93 was
    meant), and defect 015's (a context-blind certain replacement).

    **Unrun, questions rather than premises**: every other `certain` fix in
    `selfhost/` measured by the same instrument's `--apply` pass beyond these
    96 operators; whether a rename's certainty can ask the candidate's kind
    (a value where a value stands, a type where a type does) at the resolver,
    which does not know types.

    **2026-09-30, lane recovery-b3, an escape the language does not have
    keeps its certain `\\` only where it reads as nothing but a backslash**
    (the instrument's `\(`, and beside it an escape by a character's code, one
    written bare, one in a character literal): repaired at `455940f5`, gated
    by its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **2026-09-30, lane recovery-b3, a rename is certain only where the name's
    own letters allow the one candidate, and `True` is `true`** (the
    instrument's `python-bool`, and `p` renamed to `s`): repaired at
    `4e5ef22e`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane recovery-b3, a value then `@` in an argument list reads
    as a named mutable argument only where the value is a bare name, and as a
    `,` left out always** (the instrument's `missing-comma`): repaired at
    `83e77694`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane recovery-b3, a record's or a variant's type parameters
    end its head's line, told once at the `<`, and its members are read**
    (lane recovery-b2's report, its first shape): repaired at `042a14a4`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **Widened 2026-09-30 by the coordinator's certain-fix audit**, a measuring
    lane at `6d781be1` that enumerated every site able to write a `certain`
    fix (the 31 lines building one with `.certain`, the 78 `Fix(`
    constructions and the helpers traced to their callers, and the 12 swap
    rows of `keywords.hero`'s table: 42 sites) and probed each with a second
    reading, about 165 probes (`scratchpad/certain-audit/`; each breach's
    reproducer, what `--apply` wrote and the program meant, under
    `findings/<site>/`). **16 of the 42 breach the rule**, 11 of them writing a
    program that checks clean and means something else. The discard's `_ = `
    (`discard_errors.hero:79`) is certain on `push(xs, 4)`, `b.items.push(4)`,
    `grid[0].push(1)`, `sort(xs)` and a bare `x == 5`, since
    `returns_its_receiver` reads only the dotted form with a bare receiver,
    and in a `test` block it turns a comparison that lost its `assert` into a
    passing test (reproduced by the coordinator at 10:53 on the trunk's
    compiler: `add(a: 1, b: 2) == 3` over an `add` that returns 4 draws
    `discarded_value`, `--apply` writes `_ = add(a: 1, b: 2) == 3`, and
    `heroes test` reads `1 test, all passed`, where the `assert` meant reads
    `1 failed`). The positional labels of `needs_label` and `missing_label`
    (`data_errors.hero:193`, `:204`) certify the order written: Go's
    `copy(dst, src)` order becomes `blit(from: screen, to: sprite)`, and
    `Size(640, 480)` over fields `height, width` prints 480 where 640 is
    meant. The rename's one slip between two meaningful names
    (`resolve/errors.hero:187`: `printf` to `print`, `origin_y` to `origin_x`,
    `line2` to `line1`), and the module's near name with no such guard at all
    (`module/diagnostics.hero:85`: `calc.sub` to `calc.sum`, `calc.median` to
    `calc.mean`). `::` for a dot on a record's name when a value of that name
    is in scope (`check/access.hero:101`). Python's `\N{...}` and an
    apostrophe meant as `'\''` (`escape_report.hero:43`, `literals.hero:204`).
    Refused anew: the `@` of `marker_mismatch` on a place that cannot take it
    (`data_errors.hero:219`), `while` for a `for` over an array or a string
    (`parse/loop_habit.hero:52`), the arm sign on a string or a character
    (`parse/arm_line.hero:50`), the token moved up in `(a` over `(b))`
    (`parse/line_end.hero:221`), the nested bare function type
    (`parse/type.hero:255`), `.must()` and `.default()`'s text found by the
    last `.default(` in the whole call (`check/builtins.hero:413`), `0X` with
    no digits (`number.hero:80`), and `int` inside an `extern` group made
    `i64` where C's `int` is the spec's `i32` (`scan.hero:152`'s row). The
    three sites the audit calls a wrong reading still refused
    (`literals.hero:89`, `data_errors.hero:89`, `parse/signature.hero:116`)
    and the one it found unreachable (`data_errors.hero:177`) are in its
    table. What overlapping certain fixes do to `--apply` itself is not this
    defect's: it is 137.

    **Batch gate 2026-09-30 09:38** (`77b8ca98`, the merge gate of lane
    recovery-b3's batch, closed at `46846f97`, beside defect 134): the full
    net 3907 passed and 0 failed with no suite re-run, the census moving the
    lane's ten new cases and nothing else, and the recovery instrument over
    its 13,594 planted mistakes reading ONE 11,933, EXTRA 1,326, APPLY-NEW 0,
    APPLY-OTHER 0 and 17 hidden second mistakes where the baseline read 153;
    then Linux x86-64, 930 tests and 19 suites at 0 failed by 11:40, and the
    Windows box at `6d781be1`, the same code, 930 and 19 at 0 failed by 11:42.
    Before the push, Linux arm64 at `ec1fd0a9`, the same code again, 930 and
    19 at 0 failed by 14:46.

    **2026-09-30, lane 135b, a dropped value takes a certain `_ = ` only where
    the discard is its line's one reading** (the audit's C2: a place of the
    value's own type in any spelling, a `bool`, a line in a `test`, the end of
    a function that returns a value, an arm's inline body): repaired at
    `8c0f80e7`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane 135b, a missing label is certain only where the
    argument can stand nowhere else** (the audit's C6 and C7: one argument
    alone, a type only one position takes, a marker only one carries; a
    label goes before an argument's `@`): repaired at `5ea6b86f`, gated by
    its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **2026-09-30, lane 135b, a rename is certain only for a slip inside a
    word of the name, and a module's near name asks the same** (the audit's
    R2 and R3: both ends kept, no digit, a part of three letters or more):
    repaired at `6eda1b3a`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-09-30, lane 135b, `::` for a dot on a record's name is certain
    only where the position asks for a field's name** (the audit's C10: a
    `str` asked for and no value spelled like the record bound in the
    declaration): repaired at `045e6519`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane 135b, the `@` a marked parameter asks is certain only
    before a place that can take it** (the audit's C8: a place rooted in a
    cell or an `@` parameter; elsewhere no `@`, and a note): repaired at
    `8ee4efeb`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **Batch gate, lane 135b, 2026-09-30 21:32** (`f3942bac`, the trunk
    `cf943ddf` merged at `827dcd92`): the seed regenerated once, the fixpoint
    by `cmp`; the compiler's own tests 939 and the net's own 184, all passed;
    the full net 3,952 passed and 0 failed; the census over 1,411 files, no
    exit moved, 8 texts reworded by the batch; the certain-fix audit's probes,
    167 certain fixes in 136 probes before and 123 in 105 after, none into a
    new refusal; Linux x86-64 on `f3942bac`, 939 tests and 19 suites at 0
    failed. **Integrated** at `3cc3b553` (defect 136's record carries the
    integration). The item stays open for the next batch's sites: P4 to P7,
    C9's span, L3 to L5, and L6.

- [ ] **138 — a program that declares a C function the Heroes library also binds, with other marks, stops `check` with an internal error** | a five-line file whose `extern "hero_os.h"` group declares `function hero_file_read(path: cstr, @status: i64) -> str`, where the library writes `path: cstr lent`: `heroes check` prints `internal error: a diagnostic landed inside the Heroes library, at its line 112: [contract_differs] ...` and exits 2, with or without `--permissive`, where a `contract_differs` at the author's line 2 and exit 1 are owed | `selfhost/check/contracts.hero:256` (`differs`, which puts the message at `at`) and its caller's choice of `at` · `selfhost/cli/check.hero:73-82` and `selfhost/cli/compile.hero:96` (the guard that turns a diagnostic inside the library into the internal error)

    **Origin:** the coordinator, 2026-09-30 at 16:22, on the trunk's compiler
    at `a294a6ff`, following up what it had noted and not filed the same
    morning: `check --permissive` over
    `archive/bootstrap-rs/heroes/src/library/source.hero`, an old copy of the
    library, exits 2 with the same internal error. Reproducers in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/p184/libclash/`,
    each `check`ed with and without `--permissive`: `clash-plain.hero` (the
    shape above), `clash-otherheader.hero` (the same declaration under
    `extern "stdio.h"`, the same internal error), `n3-mark-added.hero`
    (`hero_str_try_from_cstr(p: cstr lent, ...)` where the library writes no
    mark: the same, at the library's line 123), `n6-called.hero` (the shape
    above, called from `main`: the same). The shapes beside that do NOT stop
    it, exit 0: the same declaration with the library's own marks
    (`clash-same.hero`, `hero_exit(code: i64)`), a constant at another type
    (`constant HERO_OS_OK: i32`), another result type
    (`hero_args_count() -> i32`), another parameter count, and a plain
    function named `hero_exit`; whether those three `extern` ones are
    clang's to refuse at `build`, as a binding's types are, is a question and
    unrun here.

    **Why it is a defect.** The program is the author's and the mistake is in
    the author's line, yet the message names a line of a file the author
    cannot open and the exit says the tool could not run
    (`.claude/rules/cli-surface.md`: exit 1 is *the input has diagnostics*).
    The guard's own comment says why it exists: *a diagnostic pointing into
    the library is the COMPILER being wrong, not the program* (panel 028
    R5). It fires because `contract_differs` compares two declarations of
    one C symbol and, when one of them is the library's, may be put at the
    library's. Read and unrun: which of the two becomes `at` is its caller's
    choice in `selfhost/check/contracts.hero`, and a declaration of the
    author's is always the one to name.

- [ ] **139 — a block holding a `match` statement counts as leaving whatever its arms do, so `check` passes a function with no `return` and `build` fails** | `function f(c: Color) -> i64` whose body is a `match` with printing arms, then `print(3)`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed ... non-void function should return a value*; the same with the `match` inside one branch of an `if` | `selfhost/check/walk.hero:898-904` (no arm with a value read as every arm jumping) · `arms` at `:1041-1050` · `check/join.hero`'s `Branch`

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

- [ ] **140 — records nested by value a thousand deep abort `build`** | 1,000 flat declarations, `record R<i>` holding `R<i-1>`, reached by `xs: [R999] = []`: `check` exit 0, `build` exit 134, `panic: stack exhausted in emitsynth.collect`; at 10,000 under a larger stack clang itself crashes on the C | `selfhost/emit/` (`emitsynth.collect`) · `selfhost/check/decls.hero` (`.record_decl`, where a bound would stand)

    **Origin:** panel 184's compiler-engineer (`184-reports/compiler-engineer.md`
    § A fourth kind of deep), 2026-09-30; reproduced by the coordinator at
    00:10 on 2026-10-01 on the integrated trunk at `3cc3b553`
    (`scratchpad/p184/newdef/deep1000.hero`; `deep700.hero` builds). No line of
    the source nests anything, so no counter of openers sees it.

    **Why it is a defect.** A program `check` accepts aborts `build`, outside
    the exit contract; panel 184's R7 files it apart from the source's depth.

- [ ] **141 — `check --apply` writes bytes no fix proposed: the newline the loader adds to a root file's open last line** | `fn main()` over `    print(1)` with no final newline, 22 bytes: `check --apply` prints 29 where its one certain fix makes 28, the last byte a `\n`; `--apply --in-place` writes it into the author's file | `source.from_files` · `source_extent.user_text` · `selfhost/cli/check.hero` (what `--apply` prints)

    **Origin:** lane 136 at its close, 2026-09-30, on the trunk's compiler as
    on its own (`scratchpad/lane-136/r2/nl/`); reproduced by the coordinator
    at 00:05 on 2026-10-01 on `3cc3b553` (`scratchpad/p184/newdef/certain.hero`,
    `od -c` of the output). Beside it and to be judged with it: a refused
    file with no fix prints its text with the added newline, and a clean file
    prints nothing.

    **Why it is a defect.** What `--apply` writes must be exactly the fixes
    it applied, the rule defect 137's repair holds; here a byte no fix
    proposed reaches the author's file.

- [ ] **142 — `missing_return`'s note sends the author to a form the checker refuses** | the note says *every path must end in a `return`, or the last statement must be the value*; a function whose last line is the bare value then costs `missing_return` again and `discarded_value` | `selfhost/flow_errors.hero:157-163`

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

- [ ] **143 — a function-like macro cannot be bound** | `extern "sys/wait.h"` with `function WEXITSTATUS(status: i32) -> i64`: `build` exit 1, *`sys/wait.h` declares no `WEXITSTATUS` — clang read the header and could not find it*, though the header defines it as a macro; design.md §1.11 says *Macros, `inline` functions and `#define` constants are now reachable directly* | the `extern` probe's parenthesized call (panel 092's `(fn)(...)`), which no function-like macro expands · `selfhost/emit/` (the probe)

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (a header of its own
    and `sys/wait.h:144-146`); reproduced by the coordinator at 20:44 on the
    trunk's compiler at `a294a6ff` (`scratchpad/p184/ffi-side/macro.hero`);
    again at 00:17 on 2026-10-01 on `3cc3b553`, exit 1 and `ffi_unknown_name`.

    **Why it is a defect.** A program calling `waitpid` needs `WEXITSTATUS`,
    and the design says it is reachable; the FFI is to be complete (CL-028).

- [ ] **144 — a result wider than C's is refused** | `extern "arpa/inet.h"` with `function htonl(x: u32) -> u64`: `build` exit 1, `ffi_return_type`, *`htonl` does not return `u64`*, where spec § 13 says *a result may be wider than C's* | `selfhost/emit/extern_assert.hero:50-79` (*wider* read for `i64` and `f64` only, the seat's reading)

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (`dbus_bool_t`, a
    `uint32_t`); reproduced by the coordinator at 20:44 on `a294a6ff`
    (`scratchpad/p184/ffi-side/wider.hero`); again at 00:17 on 2026-10-01 on
    `3cc3b553`, exit 1 and `ffi_return_type`.

    **Why it is a defect.** Spec beats compiler (CLAUDE.md § 12).

- [ ] **145 — `ffi_unknown_tag`'s note offers two repairs for a typedef of an anonymous struct, and neither is right: the handle repair builds and aborts at run** | a binding written `record Regex tag regex_t partial` for `typedef struct { ... } regex_t;` is refused with the note's two repairs, a misspelled tag or a handle; the handle checks, builds and aborts at run, *panic: a null pointer was read through, at offset 0x8*; the spelling that works, a record named `regex_t` with no `tag`, is not offered (`div_t`, `ldiv_t`, `lldiv_t` have the same shape) | `selfhost/emit/ctype.hero:149` · the note of `ffi_unknown_tag`

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (its
    `work/q1/regex/`, copied to `scratchpad/p184/ffi-side/regex-from-seat/`);
    the handle's abort reproduced by the coordinator at 20:44 on `a294a6ff`;
    both again at 00:17 on 2026-10-01 on `3cc3b553`, the handle `check` 0,
    `build` 0 and its run 134 (`ffi-side/handle.hero`), the note with one
    field under the record (`ffi-side/tag1.hero`; with none, `empty_record`
    speaks first).

    **Why it is a defect.** A §4.17 defect in a note: the repair it offers
    leads to a run-time abort, and the right one is left out.

- [ ] **146 — the parser is quadratic on declaration heads written with a foreign word** | 1,500 `fn f<i>() {` heads parse in 0.80 s and 3,000 in 3.18 s, where 3,000 clean `function` heads take 0.06 s; lane 136 measured 6,000 at 28.5 s | the recovery after a foreign-word head (`selfhost/scan.hero`'s error token, `cursor.recover_to_next_decl`)

    **Origin:** lane 136 at its close, 2026-09-30
    (`scratchpad/lane-136/r2/big/`); timed by the coordinator at 00:09 on
    2026-10-01 on `3cc3b553`, load 1.6 (`scratchpad/p184/newdef/fn1500.hero`,
    `fn3000.hero`, `ok3000.hero`).

    **Why it is a defect.** A file of Rust habits a model writes whole costs
    time that grows with its square (twice the heads, 4.0 times the time,
    measured); at 30,000 heads that square gives about five minutes, an
    inference, unrun.

*******************************************************************************
