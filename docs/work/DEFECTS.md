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
**OPEN: 13**

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

    **2026-10-01, lane recovery-b5, a body's braces are laid out by the lexer
    as their fix leaves them, so the lines in them are judged as without the
    braces** (ruling 1, the cluster's C2): repaired at `d422815d`, gated by
    its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **2026-10-01, lane recovery-b5, a head with no name before its braces is
    told once, and the report says where its block goes** (ruling 2, the
    cluster's C5): repaired at `d6af2934`, gated by its cases and the
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

    **Next, by the author's choice of 2026-10-01 on panel 183's R3**
    (`docs/records/done/2026-10-01-2224-panel-183-r3-the-openers-message-reworded-and-a-narrower-rule-prototyped.md`):
    the opener's message reworded to name the line and the word where the
    bracket's reach ended, a sentence true in Task 2 and in the known-cost
    shapes alike (`tests/golden/check/panel-183-a-statement-inside-a-bracket-closed-below-is-the-rules-known-cost.hero`,
    11 messages since C3); and a narrower R3 prototyped in this cluster's
    lane and measured on those shapes, Task 2 and the instrument before any
    sitting sees it.

    **2026-10-02, lane recovery-b6, an opener the rule ended is named still
    open at the line and the word that ended its reach** (panel 183 R3,
    route (b)): repaired at `8c907fa5`, gated by its cases and the compiler's
    own tests; the net is owed at the batch's close.

    **2026-10-02, lane recovery-b6, a comment on its own line hides no kept
    line end, so a name below it still ends a paren's reach** (a shape beside
    R3's rewording, found in the lane's first pass): repaired at `d99b12f8`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **2026-10-02, lane recovery-b6, an orphan block below a failed line that
    can open none is named once, and read** (the cluster's C4): repaired at
    `e85385a9`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-10-02, lane recovery-b6, a block under a head nothing reads is
    read as the kind the head's own words name** (130's group (c)): repaired
    at `dc83367b`, gated by its cases and the compiler's own tests; the net
    is owed at the batch's close.

    **2026-10-02, lane recovery-b6, another language's comment, `//` or `/*
    */` on one line, is one message and a comment** (130's group (e)):
    repaired at `6d49b827`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **Batch gate, lane recovery-b6, 2026-10-02 10:25** (the trunk `8b98bcc7`
    merged at `8a67e4b8`): the seed regenerated once, the fixpoint by `cmp`;
    the compiler's own tests 1,006 and the net's own 184, all passed;
    `tests/emission` unmoved, `emission` 654 and 0; the full net 4,318 passed
    and 0 failed over 26 suites, none re-run, no floor raised; the census over
    1,642 files in both arms, against the trunk's compiler, moving 70 outputs
    in 35 files and no exit, the batch's own 25 cases and ten of panel 183's
    probes whose openers take R3's words; the recovery instrument against the
    lane's base: ONE 12,745 to 12,838, EXTRA 532 to 439 (`line-comment` 93 to
    0), HIDDEN pairs 12 to 4, APPLY-NEW and APPLY-OTHER 0, no class risen;
    Linux x86-64 on the gate tree, 1,006 tests and 19 suites, 4,171 passed and
    0 failed. The item stays open for the shapes lane recovery-b6's report
    names.

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

    **2026-10-02, lane recovery-b7, a line that holds an arm's `=>` below an
    arm or its `match` is an arm, its leading operator told once and the arm
    read past it, and an alternation broken at a match's margin is read as
    one arm** (the audit's 132-06, lane X2's `b13`; beside it `| 1 => 10`,
    `* 2 => 3`, `? =>` at the margin and below a whole arm, and `0` over
    `| 1 => 10` at the margin): repaired at `4c893c14`, gated by its cases
    and the compiler's own tests; the net is owed at the batch's close.

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

    **2026-10-02, lane recovery-b7, a declaration whose head failed, or holds
    a word the lexer refused, is told the body or the members it lacks in the
    same run** (the audit's 133-a-fswap2, 133-m-const_alone, 133-m-fn_alone;
    beside them `$`, `0X` or `let` after a head, `record P fn`, `extern "m.h"
    fn`, `if true fn` over its margin, and `function`, `function f`,
    `constant MAX`, `test`, `record`, `variant` that failed over nothing):
    repaired at `97bbdc68`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

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

    **2026-10-01, lane 135c, a token moved up to the line above is certain
    only where going on is its one reading** (the audit's P6, a `(` or `[`
    below a value in a group or an index; and lane recovery-b4's, a line that
    can stand on its own inside a bracket no closer closes): repaired at
    `19fdef87`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close. P7 no longer breaches on the base of this
    lane, `9d1c209d`: defect 137's rounds write its nested types right (the
    audit's three probes and eighteen beside them).

    **2026-10-01, lane 135c, removing a `.must()` or `.default()` that
    unwraps nothing keeps the call's own receiver, and is certain only where
    it drops nothing that runs** (the audit's C9, the receiver read from the
    call's tokens; beside it, a `.default(v)` whose `v` is not a literal):
    repaired at `b788f716`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-01, lane 135c, *use `while`* for a `for` is certain only where
    its condition is a `bool` by its shape** (the audit's P4, and beside it
    `for 3`, `for ("abc")`, `for {"a": 1}`): repaired at `aa22c907`, gated
    by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **Batch gate, lane 135c, 2026-10-01 21:14** (`8025ad15`, P6, C9 and P4;
    P7 found closed by defect 137's rounds; defect 141 closed by the same
    gate): the seed regenerated once, the fixpoint by `cmp`; the compiler's
    own tests 973 and the net's own 184, all passed; the full net 4,207
    passed and 0 failed; the census over 1,509 files, every move the batch's
    own; the certain-fix audit's 168 probes, certain fixes 124 to 118 and
    applied texts carrying a new code 11 to 4; Linux x86-64 18 of 19 suites at
    0 failed, `probe` on the emulation's timeout and 24 and 0 on the trunk
    that carries the batch; Windows 973 tests and 19 suites at 0 failed. The
    item stays open for P5, L2, L3, L4, L5 and C3, each measured wider than
    the audit found (`scratchpad/lane-135c/next/first-pass.md`), P5 waiting
    on panel 185's ruling on design.md §4.15's premise.

    **2026-10-02, lane literals, an escape that names a character by its
    name, its code or its letter, or one written bare, takes no certain
    backslash** (the audit's L4, `\N{…}`, `\x{…}`, `\u41`, `\cA`, `\%`; and
    beside them `\o{…}`, `\U41`, `\C-a`, `\M-A`, `\E`, `\#`, `\@`, `\8`):
    repaired at `1d134613`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane literals, an uppercase base prefix is lowered with
    certainty only where a digit follows it or its digits already faulted**
    (the audit's L5, `0X`, `0B`, `0O` with no digit after them): repaired at
    `065b0fef`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-10-02, lane literals, the carriage return that begins a CRLF line
    end ends a literal, and is never written into it** (the audit's L2, and
    beside it a backslash before that line end): repaired at `f6fd5e48`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close. Left open: an `f"…"` literal open at a CRLF end with no
    backslash before it, whose loop is `lex_interp.hero:67`.

    **2026-10-02, lane literals, `'\'` is a backslash or an apostrophe, both
    guesses, and a later quote on its line leaves it whole** (the audit's
    L3; beside it `c == '\' || c == '"'` read as one wider literal):
    repaired at `8012b34c`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close, and so is the deletion of
    `fixedbugs-131-a-literal-that-never-closed-takes-its-line.applied`,
    which pinned the old certain fix.

    **2026-10-02, lane checker, a label that names nothing is renamed by its
    position only where the argument can stand there alone** (C3, and beside
    it two labels naming nothing in each other's places, which applied into
    a program that checks clean): repaired at `1ef5c8ca`, gated by its cases
    and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-02, lane recovery-b7, a result written after `:` or `=>` is a
    certain `->` only where a body follows the head or none is owed** (the
    audit's P8, `function stub(): pass` and `=> value` over no body; beside
    them a stub whose word is a type, `(): i64` over nothing; certain still
    over an indented, tabbed or braced body and in an `extern` group):
    repaired at `2e0c53f5`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane recovery-b7, `int` inside an `extern` group is C's,
    and its certain swap writes `i32`** (the audit's L6, `t_int_extern_param`
    and `t_int_extern_field`; beside them a result, a callback's parameter and
    a field two levels under the head; outside a group `int` is `i64` still):
    repaired at `c61a1d04`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane recovery-b7, an `f"…"` literal left open at a CRLF
    line end is told unterminated once, the line end never written into it**
    (the site the L2 line above left open, `lex_interp.hero`'s `scan_piece`;
    lane literals' probes `r2g` and `s2i`, and beside them a later piece, a
    doubled brace and a call): repaired at `664b283b`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

- [ ] **143 — a function-like macro cannot be bound** | `extern "sys/wait.h"` with `function WEXITSTATUS(status: i32) -> i64`: `build` exit 1, *`sys/wait.h` declares no `WEXITSTATUS` — clang read the header and could not find it*, though the header defines it as a macro; design.md §1.11 says *Macros, `inline` functions and `#define` constants are now reachable directly* | the `extern` probe's parenthesized call (panel 092's `(fn)(...)`), which no function-like macro expands · `selfhost/emit/` (the probe)

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (a header of its own
    and `sys/wait.h:144-146`); reproduced by the coordinator at 20:44 on the
    trunk's compiler at `a294a6ff` (`scratchpad/p184/ffi-side/macro.hero`);
    again at 00:17 on 2026-10-01 on `3cc3b553`, exit 1 and `ffi_unknown_name`.

    **Why it is a defect.** A program calling `waitpid` needs `WEXITSTATUS`,
    and the design says it is reachable; the FFI is to be complete (CL-028).

    **Widened 2026-10-02** by panel 185's ffi-pragmatist: glibc's `FD_ZERO`,
    a statement macro (`do { ... } while (0)`), takes the same false *declares
    no* (`scratchpad/185-ffi-pragmatist/p185/`); on macOS `WEXITSTATUS`,
    `htonl`, `FD_ISSET` and `WIFEXITED` are macro-only names alike. Panel 185
    sat on the route (its Q1); the seats measured that a call-form probe holds
    none of a macro's parameters, a `u8` declaration reading out of bounds.

- [ ] **147 — spec § 8's `Inline` production refuses one-statement arms the design allows and the compiler builds** | `Inline = ( Expression | "return" [ Expression ] | "break" | "continue" | "assert" Expression ) NEWLINE` leaves out a mutation (`.blue => n @ 5`) and a `while` (`.red => while n < 3` over its body), and both check, build and run on the trunk, where design.md §4.7 says *an arm's body is one statement, inline, or an indented block* (panel 014) | `spec/heroes-spec.md:241-243` · design.md §4.7 (`:1230-1235`) · `selfhost/parse/arm_line.hero`

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

- [ ] **150 — a correct program that reads a C union naming two members gets clang's warning on the author's line** | `extern "w.h"` with `record W` (two members over a union) and `function make_w() -> W`, `print(w.i)`: `build` exit 0, the program prints 7, and the build prints *warning: excess elements in union initializer* at `read.hero:2:81` | the completeness probe's `{0,0}` for a union record (`selfhost/emit/`, beside defect 140's `{}`)

    **Origin:** lane emit, 2026-10-01 (`scratchpad/lane-emit/pass1/union2/read.hero`
    and its `w.h`), left open in defect 140's closed record; reproduced by
    the coordinator at 00:35 on 2026-10-02 on `b9fdb0a3`.

    **Why it is a defect.** The emitted C is the compiler's, and a warning
    about it reaches the author at a line they wrote correctly
    (`.claude/rules/generated-c.md`; design.md §4.17).

- [ ] **151 — a C struct holding an anonymous union is bound as separate fields, so a value built from Heroes reads back wrong** | `extern "u.h"` over `typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;` with `record SA` naming `kind`, `i`, `f`, `x`: `s = SA(kind: 1, i: 7, f: 0.5, x: 3)` builds at exit 0 and `print(s.i)` prints 1056964608, the bits of 0.5, where 7 was written; `==` on it is accepted and prints `true` (u18); a field left out of such a struct is not reported, the program printing 12 (u07); and a record naming `a` and `c` of a struct `{a, b, c}` is told *does not name `c`* (u19) | `selfhost/emit/` (the record's layout check, `ffi_union_field`), panels 060 to 077's union rule

    **Origin:** lane literals' first pass for defect 150, 2026-10-02
    (`scratchpad/lane-literals/pass1/U150/`, `u.h`, `u17_anon_constructed.hero`,
    `u18_anon_compared.hero`, `u07_anon_omits_x.hero`,
    `u19_struct_omits_middle.hero`); reproduced by the coordinator at 02:42 on
    2026-10-02 on the trunk at `03e70520`.

    **Why it is a defect.** A program that checks and builds computes a value
    nobody wrote, at the C boundary (design.md §1.12, robustness, and §4.19).
    The lane's route, the header's layout read from clang, changes the union
    rule of panels 060 to 077, so the repair is a sitting's (panel 186).

- [ ] **152 — a C object declared as an `extern` `function` stops `build` with an internal error** | `extern "errno.h"` with `function errno() -> i32`: `build` exit 2, *internal error: compiling the generated C failed: ... called object type 'int' is not a function or function pointer* at the result probe; the same for `stdin` and `optarg`, with or without parameters, on macOS and Linux (the seat's), where the same names declared `constant` get a clean exit 1 | `selfhost/emit/extern_probe.hero` (the result probe) · the `extern` member's kind check

    **Origin:** panel 185's ffi-pragmatist, 2026-10-02
    (`scratchpad/185-ffi-pragmatist/p185/`); reproduced by the critic and by the
    coordinator at 02:42 on `03e70520` (`scratchpad/file-0250/errno.hero`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a program
    the author can be told about (`.claude/rules/c-boundary.md`).

- [ ] **153 — `declaration_in_arm` says `_` would be bound, which spec § 5 says it never is** | `.blue => _ = 0` on an arm's line: *an arm's body may not declare a name — `_` would be bound where nothing can read it*; spec § 5: `_` *binds nothing*, and § 8 names `_ = 0` as the arm that does nothing | `selfhost/parse/arm_body.hero:25-38` (`declared_name` does not ask the name)

    **Origin:** panel 185's critic, compiler-engineer and spec-warden,
    2026-10-02 (`docs/panel/185-briefs/probes/q2/s06-discard.hero`); reproduced by
    the coordinator at 02:42 on `03e70520`. Whether `_ = e` stands on an arm's
    line is panel 185's Q2; the message is false whichever way it rules.

    **Why it is a defect.** design.md §4.17: a diagnostic is true; this one
    contradicts the spec it cites.

- [ ] **154 — a hexadecimal number with a fraction is told as a field access** | `x = 0x1.5`: `error[expected_field_name]: expected a field or function name after `.`, found a number (`5`)`, a member-access message for a literal | `selfhost/number.hero` · `selfhost/grammar_expr.hero` (the `.` after a literal)

    **Origin:** lane literals' first pass, 2026-10-02
    (`scratchpad/lane-literals/pass1/L5/t5_hex_fraction_lower.hero`); reproduced by
    the coordinator at 02:42 on `03e70520`.

    **Why it is a defect.** design.md §4.17: the message names a mistake the
    author did not make; the number's own form is the mistake.

- [ ] **155 — the public CI is red on `03e70520`: defect 140's variants case passes a C compiler's own limit** | run 36939966148: Darwin arm64's clang (Apple clang 21.0.0, Xcode 26.6) crashes, *Illegal instruction: 4*, on `run/fixedbugs-140-variants-a-thousand-deep-build`, red in `run`, `determinism` and `emission`; Linux x86-64 and arm64 time out on it at `-O2` (exit 124); Linux arm64 also times out on `probe/selfhost, multi`; Windows green | `tests/golden/run/fixedbugs-140-variants-a-thousand-deep-build.hero` and its trace until 2026-10-02, `tests/golden/run/fixedbugs-140-variants-through-arrays-a-thousand-deep-build.hero` since · `tests/harness/suite_run.hero:151`

    **Origin:** the author, 2026-10-02 at 02:31 (*la ci è rotta*), on the push
    of 01:17; read by the coordinator from `gh run view 36939966148
    --log-failed`. This Mac and the Linux containers passed the same case at
    every gate of 2026-10-01.

    **Why it is a defect.** The trunk's own instrument is red on every push
    until the case witnesses defect 140 without depending on a C compiler's
    recursion limit or speed; lane ci140 is on it.

    **2026-10-02, lane ci140, the variant chain is built through arrays,
    where no C type nests, and kept by value in three `check/` cases, where
    no C compiler reads it**: repaired at `0aa055a9`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

*******************************************************************************
