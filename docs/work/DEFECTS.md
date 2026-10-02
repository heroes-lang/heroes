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
**OPEN: 12**

- [ ] **130 — after a `match` whose arm fails to parse, the next statement is skipped whole, and every mistake in it goes unreported** | in `function main()`, three bound matches `a = match n`, `b = match n`, `c = match n`, each with the arms `+ => 10` and `_ => 20`, report lines 4 and 10 and never line 7; two statement matches `match n` over `+ => print(1)` report only the first; and a plain statement after one such match, `y = 3 )` over `z = 4 )`, reports the second line and not the first, where the same two lines after no `match` report both | `selfhost/grammar_expr.hero` (`match_expr`) · the enclosing statement's recovery · **class: systemic**

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

    **Class: systemic**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): six batches on the
    trunk's record (recovery-b1 to b6) and a seventh in lane recovery-b8, which
    leaves one row, `130-34a`, that needs a ruling panel 183's reach rule does
    not reach (inside an open `[` a name-led line is an element, so the `)`
    pairs with the `[`); to a sitting with 131 on what a finished recovery is.
    Seen beside it by lane recovery-b8, for that sitting and not repaired: a
    function among a variant's cases dropped silently (`g2/r12`), a bodiless
    method in a record costing two messages (`g2/r09`), `return match f(n` over
    its arms swallowing them (`hc/h09`), `if f(1 +) )` hiding its stray `)`
    (`g4/ti`), an extern member whose `(` is left open over a body (`g1/b07`), a
    function nested in an orphan block costing two messages (`g1/a11`), under
    `scratchpad/lane-recovery-b8/p1/`.

- [ ] **131 — a block head whose line failed reports its missing body as a second mistake** | `if n > )` over an indented `print(1)` costs `expected_expression` and then `missing_body` at the same column; so do `else if`, `while` and `for` heads (`missing_body`) and `match )` (`missing_match_arms`); and `if n >` over a deeper line, joined by `continuation_outside_brackets`, is followed by `missing_body` at the next statement | `selfhost/grammar_expr.hero` (the body checks after a block head, `match_expr`'s arms check) · `cursor.at_reported_error` · **class: systemic**

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

    **Class: systemic**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): four batches
    (recovery-b1, b2, b4, b5) and 15 rows open on `2bb45a96` by lane
    recovery-b8's count; to the same sitting as 130.

- [ ] **143 — a function-like macro cannot be bound** | `extern "sys/wait.h"` with `function WEXITSTATUS(status: i32) -> i64`: `build` exit 1, *`sys/wait.h` declares no `WEXITSTATUS` — clang read the header and could not find it*, though the header defines it as a macro; design.md §1.11 says *Macros, `inline` functions and `#define` constants are now reachable directly* | the `extern` probe's parenthesized call (panel 092's `(fn)(...)`), which no function-like macro expands · `selfhost/emit/` (the probe) · **class: blocking**

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

    **2026-10-02, lane ffi-macro, panel 185 R1, a macro-only name is
    `ffi_macro_name`, its note drafting a function of the program's own with
    a placeholder for every C type**: repaired at `357589d6`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close,
    and so are Linux x86-64 (the gate's container), Linux arm64 and the
    Windows box, R1 being at the C boundary.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a false message
    (*declares no*) on a header that defines the name; repaired and gated,
    closes after the push's platform legs.

- [ ] **150 — a correct program that reads a C union naming two members gets clang's warning on the author's line** | `extern "w.h"` with `record W` (two members over a union) and `function make_w() -> W`, `print(w.i)`: `build` exit 0, the program prints 7, and the build prints *warning: excess elements in union initializer* at `read.hero:2:81` | the completeness probe's `{0,0}` for a union record (`selfhost/emit/`, beside defect 140's `{}`) · **class: blocking**

    **Origin:** lane emit, 2026-10-01 (`scratchpad/lane-emit/pass1/union2/read.hero`
    and its `w.h`), left open in defect 140's closed record; reproduced by
    the coordinator at 00:35 on 2026-10-02 on `b9fdb0a3`.

    **Why it is a defect.** The emitted C is the compiler's, and a warning
    about it reaches the author at a line they wrote correctly
    (`.claude/rules/generated-c.md`; design.md §4.17).

    **2026-10-02, lane land186, completeness asked of the header's layout
    and the positional probe gone** (panel 186 R4): repaired at `f2a08f13`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close, and the platform legs before the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a clang warning on a
    correct program reaches the author's line.

- [ ] **151 — a C struct holding an anonymous union is bound as separate fields, so a value built from Heroes reads back wrong** | `extern "u.h"` over `typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;` with `record SA` naming `kind`, `i`, `f`, `x`: `s = SA(kind: 1, i: 7, f: 0.5, x: 3)` builds at exit 0 and `print(s.i)` prints 1056964608, the bits of 0.5, where 7 was written; `==` on it is accepted and prints `true` (u18); a field left out of such a struct is not reported, the program printing 12 (u07); and a record naming `a` and `c` of a struct `{a, b, c}` is told *does not name `c`* (u19) | `selfhost/emit/` (the record's layout check, `ffi_union_field`), panels 060 to 077's union rule · **class: blocking**

    **Origin:** lane literals' first pass for defect 150, 2026-10-02
    (`scratchpad/lane-literals/pass1/U150/`, `u.h`, `u17_anon_constructed.hero`,
    `u18_anon_compared.hero`, `u07_anon_omits_x.hero`,
    `u19_struct_omits_middle.hero`); reproduced by the coordinator at 02:42 on
    2026-10-02 on the trunk at `03e70520`.

    **Why it is a defect.** A program that checks and builds computes a value
    nobody wrote, at the C boundary (design.md §1.12, robustness, and §4.19).
    The lane's route, the header's layout read from clang, changes the union
    rule of panels 060 to 077, so the repair is a sitting's (panel 186).

    **Widened 2026-10-02** by panel 186's completeness critic: ONE declared
    field inside an anonymous union is enough for a wrong answer.
    `one_arm.h`'s `typedef struct { int32_t kind; union { int8_t b; int64_t
    q; }; } SB;` with `record SB` naming `kind` and `b`, `print(make_a() ==
    make_b())` over two values whose `q` differs: `build` exit 0, prints
    `true` (reproduced by the coordinator at 11:02 on `ae08ed93`), where the
    same union bound alone by one member is `ffi_union_field`
    (`docs/panel/186-briefs/probes/critic/one_arm.hero`).

    **2026-10-02, lane land186, the layout read from clang: a field left out
    named by it, two fields of one union not built, a misspelling told
    first** (panel 186 R1, R2, R4): repaired at `f2a08f13`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's
    close, and the platform legs before the push.

    **2026-10-02, lane land186, one comparison rule: a field in a union is
    compared only as an integer, a pointer or an array of them as wide as
    the union** (panel 186 R3, the widening's `SB` among its cases):
    repaired at `75ff04b9`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close, and the platform legs before the
    push.

    **2026-10-02, lane land186, spec § 13 says it, and *as wide as the
    union* is the width clang gives the union's own block** (panel 186 R6):
    at `b7c510c5`, gated by its own cases; the rest is owed at the round's
    gate, and the platform legs before the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a wrong value at the C
    boundary, built from Heroes.

- [ ] **152 — a C object declared as an `extern` `function` stops `build` with an internal error** | `extern "errno.h"` with `function errno() -> i32`: `build` exit 2, *internal error: compiling the generated C failed: ... called object type 'int' is not a function or function pointer* at the result probe; the same for `stdin` and `optarg`, with or without parameters, on macOS and Linux (the seat's), where the same names declared `constant` get a clean exit 1 | `selfhost/emit/extern_probe.hero` (the result probe) · the `extern` member's kind check · **class: blocking**

    **Origin:** panel 185's ffi-pragmatist, 2026-10-02
    (`scratchpad/185-ffi-pragmatist/p185/`); reproduced by the critic and by the
    coordinator at 02:42 on `03e70520` (`scratchpad/file-0250/errno.hero`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a program
    the author can be told about (`.claude/rules/c-boundary.md`).

    **2026-10-02, lane ffi-macro, a name the header has as an object, a
    value or a type is told on its declaration, and so is a field the header
    lacks whatever the header calls its type** (the second found in the
    lane's first pass, at the field assertion): repaired at `2d5240a0` and
    `6559facf`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 where the
    author can be told; repaired and gated, closes after the push's platform
    legs.

- [ ] **155 — the public CI is red on `03e70520`: defect 140's variants case passes a C compiler's own limit** | run 36939966148: Darwin arm64's clang (Apple clang 21.0.0, Xcode 26.6) crashes, *Illegal instruction: 4*, on `run/fixedbugs-140-variants-a-thousand-deep-build`, red in `run`, `determinism` and `emission`; Linux x86-64 and arm64 time out on it at `-O2` (exit 124); Linux arm64 also times out on `probe/selfhost, multi`; Windows green | `tests/golden/run/fixedbugs-140-variants-a-thousand-deep-build.hero` and its trace until 2026-10-02, `tests/golden/run/fixedbugs-140-variants-through-arrays-a-thousand-deep-build.hero` since · `tests/harness/suite_run.hero:151` · **class: blocking**

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

    **2026-10-02, lane ci-probe, the second half: a probe row over a tree
    probes its files one at a time, and the files together must read what
    the probe reads of the whole root**: repaired at `efe2fea2`, gated by
    the net's own tests and `probe` whole; the net is owed at the batch's
    close, and Linux arm64 at the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): the public CI is red;
    both halves repaired, closes on a green CI run.

- [ ] **156 — a C struct with a bit-field member stops `build` with an internal error and clang's text** | `extern "bf.h"` over `typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;` with `record BF` naming `kind: i32`, `flag: u32`, `rest: u32`, reading `make_bf().kind`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: ... invalid application of 'sizeof' to bit-field* at the field assertions and *address of bit-field requested* in the generated hash | `selfhost/emit/` (the field assertion, `heroes-ffi-field`, and the record's descriptor) · panel 073's item 3 (bit-fields filtered before the assertion) · **class: blocking**

    **Origin:** panel 186's completeness critic, 2026-10-02, in its first
    pass over the briefs (`docs/panel/186-briefs/probes/critic/bf.h`, `bf.hero`);
    reproduced by the coordinator at 11:02 on `ae08ed93`.

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    binding the author can be told about, and clang's text reaches the
    author (`.claude/rules/c-boundary.md`). Panel 073 resolved that a
    bit-field is filtered before its assertion; what such a field binds to,
    if anything, is panel 186's question beside defect 151.

    **2026-10-02, lane land186, a bit-field refused on its own line at
    build, and one left out sent to `partial`** (panel 186 R5): repaired at
    `afd07f00`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close, and the platform legs before the push.

    **2026-10-02, lane land186, spec § 13's *A bit-field is none of these:
    leave it to `partial`.*** (panel 186 R6): at `b7c510c5`, gated by its
    own cases; the rest is owed at the round's gate.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for a binding the author can be told about.

- [ ] **157 — `ffi_tag_is_a_union` says a union has no `record` spelling, which a record binding a union falsifies** | `extern "tu.h"` over `union utag { int32_t i; float f; };` with `record UI tag utag` naming `i`: `build` exit 1, `ffi_tag_is_a_union`, its note *a group's `record` is the header's STRUCT (§4.19). A union has no `record` spelling in this language: reach it as a `ptr` and read it through C functions, or bind the one member you need as its own type*, while `record W` over the typedef'd union `W` builds and prints `7` | `selfhost/emit/ffi_tag.hero:200-218` (landed in `8fdd2a3c`) · panel 077's ratified item 4 (*spell `union T` where the header says union*) · panel 073's *one record per arm by `tag`* · **class: blocking**

    **Origin:** panel 186's ffi-pragmatist and compiler-engineer (both
    measured the refusal, on SDL3's `SDL_Event` and on a minimal `union
    utag`) and its completeness critic (who read the note against
    `read.hero` and panel 077, and found no golden holding the code),
    2026-10-02; reproduced by the coordinator at 14:20 on the trunk at
    `43520193` (`docs/panel/186-briefs/probes/coordinator/tu1.hero` and
    `read.hero`).

    **Why it is a defect.** design.md §4.17: a diagnostic is true, and its
    note sends the author to a `ptr` and C functions for a union the
    language binds; the refusal itself, of a TAGGED union where a typedef'd
    one binds, is the question panel 186's R9 leaves open with it.

    **2026-10-02, lane land186, the note says what is true, its two routes
    built and run**: repaired at `24d3d23d`, gated by its own cases; the
    rest is owed at the round's gate, and the platform legs before the push.

    **Widened 2026-10-02** by lane land186's first pass, the same cause (a
    union reached by `tag`): a handle, `record UH tag utag` with no fields
    over `union utag { int32_t i; float f; };`, is told `ffi_unknown_name`,
    *`tags.h` declares no `utag`*, which is false (reproduced by the
    coordinator at 17:40 on `9faf7462`,
    `docs/panel/186-briefs/probes/coordinator/handle_utag.hero` over
    `tags.h`).

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a false note, against
    panel 077's ratified item 4.

- [ ] **158 — a group's header that includes a header this machine lacks stops `build` with an internal error and clang's text** | `extern "outer.h"` over a header holding `#include <no_such_header_here.h>`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: In file included from ...: ./outer.h:2:10: fatal error: 'no_such_header_here.h' file not found*, where a missing header the group names itself is told `ffi_missing_header` at exit 1 | `selfhost/emit/ffi_build.hero` (where `ffi_missing_header` is told) · `tests/golden/run/fixedbugs-143-system-macros-through-functions-of-the-programs-own.hero`, red on the Windows box · **class: blocking**

    **Origin:** the coordinator, 2026-10-02 at 15:55, reading the Windows
    box's pre-push leg on `2bb45a96` (16 of 19 suites green; `run`,
    `emission` and `determinism` each 1 failed, all on lane ffi-macro's new
    run case, whose header includes `sys/wait.h` and `sys/select.h`, absent
    on Windows), then reproduced on this Mac at 15:57 on the trunk at
    `4d0f27a1` (`docs/panel/186-briefs/probes/coordinator/nested.hero` and
    `outer.h`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for the
    machine's fact (`.claude/rules/c-boundary.md`), and clang's text reaches
    the author; the run suite skips a case only on `ffi_missing_header`
    (`tests/harness/shell.hero`'s `machine_lacks_the_library`), so the same
    fact also turns a platform's correct skip into a red that would reach
    the CI's Windows leg at the next push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for the machine's own fact; it would turn the CI's Windows leg red.

- [ ] **159 — a `-` alone on the line above an arm's pattern is joined to it with certainty, though deleting it is as likely a reading** | `k = match n` over a line holding only `-` and then `1 => "one"`, `_ => "many"`, with `n = 1`: `continuation_outside_brackets` with the certain fix *write the statement on one line*, and `check --apply` writes `-1 => "one"`, which checks clean and prints `many`; deleting the `-` gives `1 => "one"` and prints `one` | the lexer's join of a line ending in an operator (`continuation_outside_brackets`'s certain fix) · design.md §4.8 (*two readings make two guesses*) · defect 135's class, closed 2026-10-02 · **class: blocking**

    **Origin:** lane arm's first pass, 2026-10-02
    (`scratchpad/lane-arm/pass1/r6/u14_lexer_split_int.hero`), raised again
    by lane recovery-b8 as a question; reproduced by the coordinator at
    17:00 on the trunk at `6c4da49b`
    (`docs/panel/186-briefs/probes/coordinator/u14_minus_above_an_arm.hero.txt`,
    kept as text since it does not parse, which is the defect).

    **Why it is a defect.** A `certain` fix is machine-applicable
    (`.claude/rules/diagnostics-and-goldens.md`), and this one writes a
    program that checks clean and means something else where another
    reading is as likely; panel 185's R6 made the same two readings two
    guesses for a spaced `-` on the arm's own line.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a `certain` fix that
    writes a program meaning something else.

- [ ] **160 — a package whose `.pc` gives `-F <dir>` is refused, its flag read joined as `-F<dir>`, while the note says `-F` is accepted** | `extern "Fake/fake.h" package "fakefw"` over a `.pc` with `Cflags: -F ${pcfiledir}/../frameworks`: `pkg-config --cflags` prints `-F/<dir>`, and `build` exit 1, `ffi_package`, *answered with `-F/...`, which this compiler does not pass on*, its note listing `-F` among the flags accepted | `selfhost/cli/libraries.hero` (`filter_words`, which takes `-F` only as two words) · **class: blocking**

    **Origin:** lane h158, 2026-10-02, measuring framework headers for
    defect 158 (`scratchpad/lane-h158/shapes/fw/`); reproduced by the
    coordinator at 17:25 on the trunk at `545e0044`
    (`docs/panel/186-briefs/probes/coordinator/fw/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** A correct package is refused and the message
    contradicts itself (design.md §4.17); macOS frameworks reach a program
    only through `-F`.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, with a false note.

*******************************************************************************
