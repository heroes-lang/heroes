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
**OPEN: 41**

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

    **2026-10-02, lane recovery-b8, a line that failed past what it declares
    drops only its line, and the block below it is told** (lane recovery-b6's
    `d01_use_junk` and `d04_extern_member_junk`): repaired at `525fe2dd`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **2026-10-02, lane recovery-b8, a function among a record's fields is
    told once and read as the function it is** (lane recovery-b6's
    `e14_record_with_fn` and `f06`): repaired at `26358f9c`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-02, lane recovery-b8, a block comment over several lines is one
    comment, told once** (lane recovery-b6's `lc12_block_multi`): repaired at
    `4ca2c20a`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-10-02, lane recovery-b8, the body a head's open bracket took in is
    read as its body** (the head class panel 183 left to the cluster; the
    audit's rows 130-H-004 and 130-34d): repaired at `276908b3`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-02, lane recovery-b8, a closer one too many past a failed
    statement is told** (the audit's row 130-28): repaired at `ce5caf89`,
    gated by its own cases alone (the author's instruction of that
    afternoon); the rest is owed at the round's gate.

    **2026-10-02, the round's gate of lanes recovery-b8, land186 and h158,
    two of the lane's commits redone**: `ce5caf89`, past a statement that
    ended with its own block, told the next statement's closer a second
    time at its column (the compiler's own tests, 3 failed at the gate and
    at the lane's merge `2dc1a9d7`, 0 at its `276908b3`; the census, 51
    doubled messages over 19 older goldens), redone at `d105a1e5`, the
    search bounded by what the drop of the failed line takes; and
    `525fe2dd` moved the `expected_declaration` arm into
    `parse/top_level.hero` while the site's claims reader looked for it in
    `decl.hero` (the site's build, exit 1), followed at `a388a056`. Each
    gated by its own cases; the round's gate is the commit carrying this
    line.

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

    **2026-10-02, lane land186, a record over a C union is built naming
    exactly one member of each union, and `build` judges what a
    construction leaves out** (panel 186 R7, home (a), read blind by the
    sitting's third reading): at `8977e7c6`, gated by its own cases; the
    rest is owed at the round's gate, and the platform legs before the push.

    **2026-10-02, lane land186, spec § 13 says it, O_cover in place of
    R_build_cover, and design.md §4.19 says where it is judged** (panel 186
    R7): at `fc0e7284`, gated by its own cases; the rest is owed at the
    round's gate, and the platform legs before the push.

    **2026-10-02, lane land186, R7's cases, the SDL3 event among them**:
    at `ec530b54`, gated by its own cases; the rest is owed at the round's
    gate, and the platform legs before the push.

    **2026-10-02, lane land186, a case that held a GNU fact as everyone's**:
    the Windows box's pre-push leg at `b48d02b8` refused
    `run/fixedbugs-151-members-with-no-bytes-left-out`, an empty struct
    having bytes for MSVC's target, which the compiler was right to refuse
    (R4); the case now holds only the zero-length array, no bytes on every
    target measured, and the empty struct is witnessed missing for
    `x86_64-pc-windows-msvc` and complete for `x86_64-linux-gnu` by
    `selfhost/cli/layout.hero`'s test: at `bac43e50`, gated by its own cases;
    the rest is owed at the round's gate, and the platform legs before the
    push.

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
    coordinator on `9faf7462` before 16:49, the widening's commit,
    `docs/panel/186-briefs/probes/coordinator/handle_utag.hero` over
    `tags.h`).

    **2026-10-02, lane land186, a tag naming a union's or an enum's tag told
    its own kind, a handle's and a bare name's included, with what reaches
    it** (the widening and three shapes beside it with its cause): repaired
    at `669fc846`, gated by its own cases; the rest is owed at the round's
    gate, and the platform legs before the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a false note, against
    panel 077's ratified item 4.

- [ ] **158 — a group's header that includes a header this machine lacks stops `build` with an internal error and clang's text** | `extern "outer.h"` over a header holding `#include <no_such_header_here.h>`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: In file included from ...: ./outer.h:2:10: fatal error: 'no_such_header_here.h' file not found*, where a missing header the group names itself is told `ffi_missing_header` at exit 1 | `selfhost/emit/ffi_build.hero` (where `ffi_missing_header` is told) · `tests/golden/run/fixedbugs-143-system-macros-through-functions-of-the-programs-own.hero`, red on the Windows box · **class: blocking**

    **Origin:** the coordinator, 2026-10-02, reading the Windows
    box's pre-push leg on `2bb45a96` (16 of 19 suites green; `run`,
    `emission` and `determinism` each 1 failed, all on lane ffi-macro's new
    run case, whose header includes `sys/wait.h` and `sys/select.h`, absent
    on Windows; the leg's log reads its exit at 15:52), then reproduced on
    this Mac before 15:55, the filing commit's time, on the trunk at
    `4d0f27a1` (`docs/panel/186-briefs/probes/coordinator/nested.hero` and
    `outer.h`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for the
    machine's fact (`.claude/rules/c-boundary.md`), and clang's text reaches
    the author; the run suite skips a case only on `ffi_missing_header`
    (`tests/harness/shell.hero`'s `machine_lacks_the_library`), so the same
    fact also turns a platform's correct skip into a red that would reach
    the CI's Windows leg at the next push.

    **2026-10-02, lane h158, a header the group's own header includes is
    told on the group from clang's line alone, and one reached through other
    headers from the include stack above it**: repaired at `744cdc08` (the
    line) and `7790f2f6` (the stack, the call in `selfhost/emit/ffi.hero`
    handing clang's whole stderr), gated by their cases, the first also by the
    compiler's own tests; the net is owed at the batch's close.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for the machine's own fact; it would turn the CI's Windows leg red.

- [ ] **160 — a package whose `.pc` gives `-F <dir>` is refused, its flag read joined as `-F<dir>`, while the note says `-F` is accepted** | `extern "Fake/fake.h" package "fakefw"` over a `.pc` with `Cflags: -F ${pcfiledir}/../frameworks`: `pkg-config --cflags` prints `-F/<dir>`, and `build` exit 1, `ffi_package`, *answered with `-F/...`, which this compiler does not pass on*, its note listing `-F` among the flags accepted | `selfhost/cli/libraries.hero` (`filter_words`, which takes `-F` only as two words) · **class: blocking**

    **Origin:** lane h158, 2026-10-02, measuring framework headers for
    defect 158 (`scratchpad/lane-h158/shapes/fw/`); reproduced by the
    coordinator before 16:47, the filing commit's time, on the trunk at
    `545e0044`
    (`docs/panel/186-briefs/probes/coordinator/fw/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** A correct package is refused and the message
    contradicts itself (design.md §4.17); macOS frameworks reach a program
    only through `-F`.

    **2026-10-02, lane h158, every accepted flag of a package is read in both
    its spellings, and a refused one refused in either**: repaired at
    `fb33a992`, gated by its case (a unit test in
    `selfhost/cli/libraries.hero` over pkg-config's measured output) and the
    reproducer by hand; the net is owed at the batch's close. The
    reproducer's package now passes the filter and its program still does
    not build: the compile line keeps only a package's `-I` words
    (`selfhost/cli/units.hero`), so `-F` never reaches clang, a second cause
    that lane h158's report gives for filing apart.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, with a false note.

- [ ] **161 — a package's compile flags other than `-I` never reach clang, so a correct package is refused or stops `build` at exit 2** | `extern "valued.h" package "dpkg"` over a `.pc` with `Cflags: -I<dir> -DHERO_PKG_VALUE=7` and a header returning `HERO_PKG_VALUE`: `build` exit 2, *internal error: ... use of undeclared identifier 'HERO_PKG_VALUE'*; a package's `-F` likewise never reaches the compile step, so a framework header it names is told missing | `selfhost/cli/units.hero:139` and `selfhost/cli/pointee.hero:243`, which keep only a package's `-I` · **class: blocking**

    **Origin:** lane h158, 2026-10-02, beside defect 160 (the framework
    reproducer, once its `-F` was no longer refused, still not built;
    `scratchpad/lane-h158/d160/dpkg/`); reproduced by the coordinator at
    16:57 by `date` on the trunk at `0bcd442c`
    (`docs/panel/186-briefs/probes/coordinator/dpkg/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    package the author wrote correctly, and the flags a `.pc` gives are the
    reason `package` exists (design.md §4.19).

    **2026-10-02, lane h158, every compile is handed a package's `-I -D -U
    -F` by one rule, and its cache keyed by them**: repaired at `40bf84f2`
    (`libraries.compile_flags`, called by `selfhost/cli/units.hero` and
    `selfhost/cli/pointee.hero`'s `with_search`), gated by its cases, unit
    tests in the three modules, and the reproducers by hand, `dpkg` printing
    7 and defect 160's framework reproducer building; the net is owed at the
    batch's close.

    **2026-10-02, the round's third gate, lanes land186 and h158 reconciled:
    the layout check's cache keyed by the same words**: lane land186's
    `selfhost/cli/layout.hero` (`f2a08f13`), whose three clang runs
    `pointee.with_search` now hands a package's compile words and the
    probe's flags (161, 163), kept its verdicts under the package's whole
    answer joined by spaces (162's collision too); on one `build/` the
    merged tree replayed a verdict land186's compiler had reached without
    the package's `-D` and built, at exit 0, a record four bytes short of
    the header's struct. Keyed by `libraries.compile_flags` and
    `pointee.probe_flags` at `77cb114a`, with its compiler test; the round's
    gate is the commit carrying this line.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): exit 2, a correct program refused.

- [ ] **162 — a package whose `.pc` gives a path with a space is refused, the compiler naming a fragment of the path as the flag** | a `.pc` with `Cflags: -I"<dir>/inc with space"`: `pkg-config --cflags` prints the path with its spaces escaped by backslashes, and `build` exit 1, `ffi_package`, *the package `spaced` answered with* the fragment `with` and its backslash, *which this compiler does not pass on* | `selfhost/cli/libraries.hero` (the word splitter before `filter_words`) · **class: blocking**

    **Origin:** lane h158, 2026-10-02, beside defect 160
    (`scratchpad/lane-h158/d160/pc/spaced.pc`); reproduced by the
    coordinator at 16:57 by `date` on the trunk at `0bcd442c`
    (`docs/panel/186-briefs/probes/coordinator/spaced/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** A correct package is refused, and the message
    names a piece of a path as a flag (design.md §4.17).

    **2026-10-02, lane h158, pkg-config's answer is read as a shell reads
    it, so a word with a space stays one word**: repaired at `b37bfce1`
    (`selfhost/cli/shell_split.hero`, and the compile caches keyed by each
    word's length, `libraries.key_text`), gated by its cases, unit tests in
    `shell_split` and `libraries`, and the reproducer by hand, `spaced`
    printing 8; the net is owed at the batch's close.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, with a false message.

- [ ] **163 — the pointee check judges a plain `char` without `-fsigned-char`, so on Linux arm64 it refuses the binding the program's own compile agrees with** | `extern "c.h"` over `void fill(char *p)`, bound `function fill(@p: i8)`: on this Mac `build` exit 0, prints `-1`; in the Linux arm64 container on the trunk at `02b29536`, `build` exit 1, `ffi_parameter_type`, *`p` of `fill` is declared `i8`, and the header's `char *` points at a different width or sign*, while `@p: u8` is refused on this Mac and built on arm64; the program itself is compiled with `-fsigned-char` on both (`selfhost/cli/flags.hero:108`, panel 161) | `selfhost/cli/pointee.hero` (its two clang runs, without the compiler's own flags) · panel 161 (*a plain `char` meaning the same thing on four legs*) · **class: blocking**

    **Origin:** lane h158, 2026-10-02, a question raised beside defect 161
    (the pointee probe's compiles do not use the compiler's own flag list);
    measured by the coordinator at 17:33 by `date`, on this Mac and in the
    Linux arm64 container (`docs/panel/186-briefs/probes/coordinator/signchar/`,
    `s8.hero` and `u8.hero` over `c.h`). A result is held by the program's
    own compile and agrees on both (a `char` result bound `-> u64` refused on
    both, 17:34): only the pointee check diverges.

    **Why it is a defect.** A correct binding is refused on one platform and
    accepted on another, against panel 161's ruling that a plain `char` means
    one thing on every leg; no value read through it is wrong, the bytes
    being the same (measured: `u8` on arm64 prints `255`).

    **2026-10-02, lane h158, a probe compiles under the compiler's own flags
    but its diagnostics, so a plain `char` is signed there too**: repaired at
    `0235b942` (`pointee.probe_flags`, put on every probe's line by
    `selfhost/cli/pointee.hero`'s `with_search`), gated by its cases on this
    Mac, a run case printing -1, an unsupported case refusing `@p: u8` and
    two unit tests asking clang for Linux arm64's target; its Linux arm64
    leg, where the defect shows, is owed before the push with the net.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, on Linux arm64.

- [ ] **164 — a record field misspelt as a name the header defines as a macro stops `build` with an internal error and clang's text** | `extern "macro_field.h"` over `#define size 4` and `typedef struct { int32_t len; int32_t cap; } BUF;`, `record BUF` naming `len` and `size`: `build` exit 2, *internal error: compiling the generated C failed: ... error: expected identifier ... note: expanded from macro 'size'*, at the field assertion; the same with `stdin`, which a libc defines as a macro | `selfhost/emit/` (the field assertion, `heroes-ffi-field`, and the unknown-field mapper) · **class: blocking**

    **Origin:** lane land186's second batch, 2026-10-02, beside panel 186's
    macro-reached fields (`scratchpad/lane-land186/pass2/macro_field.hero`,
    `macro_field_stdio.hero`); reproduced by the coordinator at 17:43 by
    `date` on the trunk at `20652888`
    (`docs/panel/186-briefs/probes/coordinator/macrofield/`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    misspelling the author can be told about, and clang's text reaches the
    author (`.claude/rules/c-boundary.md`).

    **2026-10-02, lane land186, a test the repair left stale**: `25b96332`,
    gated by its own cases, left one of the compiler's own tests stale,
    `emit/layout_check.hero`'s count of the questions a misspelt field puts
    under the preprocessor, which the member probe made five where it asked
    four (1027 passed and 1 failed on that commit's sources, with its
    compiler); fixed at `ba162242`, 1027 and 0, the rest owed at the round's
    gate.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): exit 2. Repaired in lane land186 at `25b96332`,
    gated by its own cases; the rest is owed at the round's gate.

- [ ] **166 — a `-` alone above a name or a float pattern is told `expected_pattern` twice at the same place** | `k = match n` over a line holding only `-` and then `x => "one"` (or `1.5 => "one"`): `continuation_outside_brackets`, then `expected_pattern` twice at the name or the number, the same text at the same column | the pattern's refusal after the join of a `-` line (the same on the compiler before lane h158) · **class: adjacent**

    **Origin:** lane h158's first pass for defect 159, 2026-10-02
    (`scratchpad/lane-h158/d159/s18_name_below.hero`, `s19_float_below.hero`);
    reproduced by the coordinator at 18:37 by `date` on the trunk at
    `4b44f684` (`docs/panel/186-briefs/probes/coordinator/adjacent/`).

    **Why it is a defect.** One mistake told twice at one place (design.md
    §4.17).

    **2026-10-02, lane h158, where the deletion is the one repair the `-` is
    left out of the stream, and the name or the float is told once**:
    repaired at `8cb4ba6c` with defect 165, gated by its case
    `fixedbugs-166-a-minus-alone-above-a-name-or-a-float-is-told-once` and
    the compiler's own tests; the net is owed at the batch's close. The
    second message was a failed arm's recovery, `opening.drop_line`, which
    stops at a line of the text inside a line the lexer joined and reads
    the part below the break again as an arm: `1 |` over `x => "one"` still
    meets it, before this repair and after it, and is reported apart.

    **Its cause found 2026-10-02** by lane h158 (its repair of the `-` shapes,
    `8cb4ba6c`, keeps them away from the cause, not repaired):
    `parse/opening.hero`'s `drop_line` stops a failed arm's recovery at the
    first word of a line of the text even inside a line the lexer joined
    (the stop defect 131 added at `9811d4cd`, for tab margins), so the part
    below the break is read again as an arm and told a second time. Shapes
    that still read two `expected_pattern` at one place, before and after
    `8cb4ba6c`: `1 |` over `x => "one"`, `1 |` over `1.5 => "one"`, `"a" |`
    over `x => "one"`, `1 | -` over `x => "one"`
    (`scratchpad/lane-h158/d166/`). The cause is the recovery cluster's file,
    so it goes with 130 and 131 to the sitting on what a finished recovery
    is.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a second message for one mistake.

- [ ] **167 — the pointee check reads a header without the build's own `-O2` or `--sanitize`, so a parameter the header types otherwise under them is accepted and the program writes past its `i32`** | `opt.h` declares `fill(int32_t *p)`, and `fill(int64_t *p)` under `#ifdef __OPTIMIZE__`, bound `function fill(@p: i32)`, `a: i32 @ 0`, `fill(@a)`, `print(a)`: `build -O0` prints `7`; `build -O2` exit 0, prints `0`; `build -O2 --sanitize` stops in *AddressSanitizer: stack-buffer-overflow ... WRITE of size 8* on the 4-byte local, exit 134; a header keyed on `__has_feature(address_sanitizer)` does the same under `--sanitize` alone | `selfhost/cli/pointee.hero` (its dump and its check, about `:153-174` and `:197-222` at `6bec7c8c`, `probe_flags()` at `:279-286`), run without the level and the sanitizer `selfhost/cli/units.hero` and `selfhost/cli/flags.hero:201` give the program's own compile · defect 163's class · **class: blocking**

    **Origin:** lane h158 at defect 163, 2026-10-02, a question left unmeasured (*no probe compiles under them*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/optimize-macro/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), defect 163's repair `0235b942` included. A result type is held by the program's own compile (a header that changes a return type under `-O2` is refused, `ffi_return_type`), so only the pointee check diverges, as in 163. Unmeasured: whether a real header changes a pointee's type under these macros, and whether the layout check's probe, beside it, is judged without them too.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a memory fault.

- [ ] **168 — the layout check's cache key does not name the header it read, so a program beside a same-named header replays another directory's verdict: a binding its own header refutes builds and prints a wrong value** | `a/x.h` holds `typedef struct { int32_t x; } E;` and `b/x.h` `typedef struct { int32_t x; int32_t y; } E;`, both bound `record E` naming `x` alone, built from one working directory: alone, `b/p.hero` is refused, `ffi_incomplete_record`; after `a/p.hero` in the same cache it builds at exit 0, and with `gety(e: E) -> i32` reading C's `e.y` its run prints `true`, `2`, `3`, `p == q` true for two values whose `y` C reads as 2 and 3; in the other order the correct `a/p.hero` stops at exit 2, *internal error: checking the header's layout of the group records failed ... no member named 'y' in 'E'* | `selfhost/cli/layout.hero:97-99` (the key: the screen unit's text, `#include <x.h>` and one function per record, no source path) · `selfhost/emit/layout_screen.hero:35`, `:51-59` · the pointee check's key names the declaring file (`selfhost/cli/pointee.hero:111-113`) and does not replay · **class: blocking**

    **Origin:** lane round1002c's gate, 2026-10-02, a question left unrun (*neither the pointee key nor the layout key names `source_dir`*); measured by the coordinator's agent on `6bec7c8c` (2026-10-02, `scratchpad/file-queue/source-dir-key/`), every experiment from one working directory, each with a fresh-cache control. The width shape (`int32_t v` against `int64_t v`) replays too, and the program's own compile still refuses it (`ffi_field_type`): a member left out has the layout check as its only judge. Unmeasured beside it: the standard library's own pointee asks key alike in both directories, and a header found through `CPATH` or another environment variable is named by no key.

    **2026-10-02, lane cb4, a probe's kept verdict keyed by every word its
    lines compile under and by what its headers resolve to, a header that
    appears earlier in the search and the library's own asks included** (the
    two shapes beside it with its cause, S4 and S6 of the lane's first pass,
    2026-10-02, `scratchpad/lane-cb4/first-pass.md`): repaired at `cf949d33`,
    gated by its own cases; the rest is owed at the round's gate, and the
    platform legs before the push.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, and an exit 2 on a correct program.

- [ ] **169 — `check` aborts on a correct program of 300 additions** | `total = 1 + 2 + ... + 300` over `print(total)`: `check` exit 134, *panic: stack exhausted in checkwalk.synth* | `selfhost/check/walk.hero:101` (`synth`) · panel 184's R5 (*the compiler runs its passes on a thread whose stack it chooses*) and R6 (*a floor, not a ceiling*), ratified 2026-10-01, not landed (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md:197`, `:204-206`) · **class: blocking**

    **Origin:** panel 184's blind task 3 (`docs/panel/184-briefs/blind/task3a.hero`), aborting in every `check` census since (lane round1002b's; lane round1002c's, *both arms abort on the same file*); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/check-abort-task3a/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The sitting's prediction is that *under R5 every shape of the shared brief's table checks at 2,000 on this Mac* (`:238-239`), and R6's N is to be measured before its sentence is written (`:270-271`). No list held the landing until this item.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a crash, and a correct program refused.

- [ ] **170 — a correct program whose types nest deeper than the C compiler survives stops `build` at exit 2 with clang's crash text** | 3,000 variants nested by value, `variant R<i>` whose case `a` holds `R<i-1>`, `xs: [R2999] = []`, `print(xs == xs)`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: clang: error: unable to execute command: Illegal instruction: 4* (Apple clang 21.0.0); the CI's Apple clang died at 1,000 (defect 155) | the build's clang call (`selfhost/cli/units.hero:96`, `tu_object`) · defect 140's closed record (*facts about the C compiler, not refusals of this one*) · panel 184's R6 · **class: blocking**

    **Origin:** lane ci140's report, 2026-10-02 (*a C compiler's own depth limit reaching the author as an internal error*); defect 155 holds only the CI's red; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/clang-depth/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). Each crash leaves a 7.7 MB preprocessed file and a script in the system's temporary folder and a report under `~/Library/Logs/DiagnosticReports/`. Defect 140's record says a refusal at a depth would be a new diagnostic class, and panel 184's R6 says no source is refused for its depth, so the route may be a sitting's.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

- [ ] **171 — a C function-pointer object declared as an `extern` `function` builds, and called while null it panics naming a callback the program never passed** | `own.h`'s `static int32_t (*hook)(int32_t) = 0;` bound `function hook(x: i32) -> i32`, `print(hook(x: 1))`: `build` exit 0; the run prints *panic: a null function pointer was called — a `ptr` holding `nullptr` reached C where C calls it back*, exit 134 | `runtime/parts/stack.c:515` (and `:738`) · the `extern` member's kind check (defect 152's `2d5240a0`, which lets a function-pointer object through) · **class: blocking**

    **Origin:** lane ffi-macro's first pass for defect 152, 2026-10-02 (`scratchpad/lane-ffi-macro/p1/obj/e23-fnptr-object-called.hero`, 2026-10-02), queued as *a message question and a declaration question, likely a sitting's (panel 038's refusal for `constant`)*; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/null-fnptr-object/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). No memory is corrupted: the guard holds.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a false message. Refusing the declaration or making the message true is the lane's question.

- [ ] **172 — in an `extern` group, `int` is refused with the `certain` fix `i32`, which `check` cannot know: where the header's type is 64 bits, the applied binding is refused anew by `build`** | `docs/panel/176-briefs/xfer_cj.hero`'s `-> i64`, over `cj.h`'s `int64_t cJSON_AddItemToObject(...)`, written `-> int`: `check` exit 1, `reserved_word`, *in an `extern` group a type is the header's own width and sign, and C's `int` is `i32`*, fix (certain) *replace `int` with `i32`*; `check --apply` writes `-> i32`, `check` exit 0, `build` exit 1, `ffi_return_type` | the `reserved_word` fix for `int` in an `extern` group (defect 135's L6, `c61a1d04`) · `.claude/rules/diagnostics-and-goldens.md` § Errors are a deliverable (*a fix that leaves the defect standing is a `guess`*) · **class: blocking**

    **Origin:** panel 187's completeness critic, 2026-10-02, on the recovery instrument's APPLY-OTHER 20 and APPLY-NEW 19 on lane recovery-b8's runs, 0 and 0 on lane recovery-b6's gate, every one operator `int`, and the same 20 and 19 on the trunk's compiler at `6bec7c8c`, the instrument's run of 2026-10-02 from 21:55 to 22:08 by `date` (`scratchpad/inst-187/round3/`, 2026-10-02); built by the coordinator on `62d65e48` (2026-10-02, `scratchpad/apply-int/case/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). In each of the instrument's cases the original said `i64`, so the header's type is 64 bits there. `check` does not read the header, so it cannot tell C's `int` copied from it (`i32` right) from `int` meaning an integer (`i64` right); `build` reads it, and the binding never runs with the wrong width.

    **Why it is a defect.** `check --apply` applies a `certain` fix without asking, and this one writes a binding that says something else than the header in every case the instrument planted.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a `certain` fix that writes a program meaning something else; `build` refuses that program, so no wrong value runs.

- [ ] **173 — an `f` literal writes `}}` as two braces and accepts a lone `}`, where panel 184's R1, ratified, makes `}}` one brace and a lone `}` an error** | `print(f"{{a}}")` prints `{a}}` and `print(f"a}b")` prints `a}b`, both at exit 0; under R1 the first prints `{a}` and the second is refused | `selfhost/lex_interp.hero` · panel 184's R1 (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md:149-150`, ratified at `:248`, *R1 and R2 land as ratified* at `:284`), R8 putting its sentence in spec § 2 (`:223-224`) · spec `:49-52` (*`{{` writes one brace*, no `}}`, no lone `}`) · **class: blocking**

    **Origin:** panel 184, ratified 2026-10-01 at 22:24 (`docs/records/done/2026-10-01-2224-panel-184-ratified-a-brace-both-ways-a-statement-after-a-jump-refused-and-a-floor-for-depth.md`); its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). No list held the landing: the sitting's item was ticked at its ratification. The spec does not say it either; the landing writes both.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the ratified language, a wrong value (`{a}}`) and a wrong program accepted (`a}b`).

- [ ] **174 — a statement after `return`, `break` or `continue` in the same block compiles, where panel 184's R4, ratified, makes it a compile error** | `function f() -> i64` over `return 1` and then `print(2)`, `main` printing `f()`: `run` exit 0, prints `1` | the checker's walk of a block · panel 184's R4 (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md:181-182`; *the spec states the return rule for the first time*, `:186-190`; *R4 lands as it stands*, `:276-277`) · spec § 8 `:229-230`, its only sentence on a jump · **class: blocking**

    **Origin:** panel 184, ratified 2026-10-01 at 22:24; its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/after_return.hero`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the ratified language, a wrong program accepted.

- [ ] **175 — a value block whose last statement is an `if` or a `match` whose every branch leaves is refused `no_value`, where panel 185's R4, ratified, makes the block leave and its arm a jumping arm** | lane flow's `a55-value-arm-block-inner-returns.hero`: `check` exit 1, `no_value` at 4:13, *this `match` produces no value — every branch jumps, so there is nothing to bind*; under R4 it checks clean (its source's own expected output, `1`, `2`, `20`, unrun) | spec § 8 `:229-230` (*A jump (`return`, `break`, `continue`) is a valid arm body*) · panel 185's R4 (`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md:169-170`, ratified at `:241-245`; its sentence goes in § 8, `:171-175`) · **class: blocking**

    **Origin:** panel 185, ratified 2026-10-02 at 03:10 (`docs/records/done/2026-10-02-0310-panel-185-ratified-a-macro-named-as-a-macro-an-arm-of-one-statement-a-leaving-block-a-sign-with-two-readings.md`); its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The sitting predicts *c1, c2, c3, c6 and a55 to b8 (b4, b7 excepted) build and run* (`:231`).

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the ratified language, a correct program refused.

- [ ] **176 — a plain literal whose brace hole names bindings all bound where it stands prints the braces, where panel 185's R7, decided by the author, refuses it with the `f` as its fix** | `x = 1` over `print("{x}")` and `print(x)`: `run` exit 0, prints `{x}` and `1`; under R7 `check` refuses the literal | spec `:52` (*A literal without the `f` is unchanged*), the opposite of the landing · panel 185's R7 (`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md:201-203`; decided 2026-10-02, `:264-268`, *route (5b) lands after panel 184's R1*), so after defect 173 · **class: blocking**

    **Origin:** panel 185's R7, decided by the author 2026-10-02 at 07:05 (`docs/records/done/2026-10-02-0705-panel-185-r7-decided-the-forgotten-f-refused-where-the-braces-hold-names-all-bound.md`); its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/forgot_f.hero`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the decided language, a wrong program accepted.

- [ ] **177 — a `match` whose arms fall inside a bracket left open has each arm told again after the bracket's own message** | `return match scores[name` over `.ok v  => v.to_str()` and `.err e => e.code`: `unclosed_bracket` at the `[`, then `line_end_before_continuation` at each arm, three messages for one missing `]`; `x = match (n` over `.ok v => 1` the same; `y = match n` below `x = [n, 1` gets `expected_end_of_line` at each arm's `=>` | the reach of a bracket left open (panel 183's R1 and R2) over a `match`'s arms · `selfhost/parse/line_end.hero:242` · **class: adjacent**

    **Origin:** lane 135c's report and lane recovery-b4's (*one extra message per arm*), queued under recovery-b5, 2026-10-02 (`scratchpad/lane-135c/shapes/P6/`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/open-bracket-arms/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The certain fix lane recovery-b4 saw inside a `[` left open is a guess today, and `check --apply` leaves the text as it is.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake; no wrong value, no false message, no certain fix. The recovery cluster's, beside 130 and 131.

- [ ] **178 — a separator habit on every line of a block is told once per line: a `,` after each statement or each arm, a `;` after each field** | `x = 1,`, `y = 2,`, `print(x + y),` in a function: three `expected_end_of_line`; arms `0 => 1,`, `1 => 2,`, `_ => 3,`: three; `record P` over `x: i64;`, `y: i64;`, `z: i64;`: three `unexpected_character`; a `,` after every member of a declaration is one message since `68e46a13` | `selfhost/parse/member_lines.hero` (the members' rule, `68e46a13`) · the line end of a statement and of an arm · the lexer's `;` · **class: adjacent**

    **Origin:** lane recovery-b4's report (*the per-line `,` and `;` habits, one message per run with a certain deletion*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/habit-every-line/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **179 — `@return 1` costs two `expected_expression`, one at the `@` and one at `return`** | `function f() -> i64` over `@return 1`: `check` exit 1, *expected an expression, found `@`* at 2:5 and *expected an expression, found `return`* at 2:6, neither naming the sigil | `selfhost/parse/at_prefix.hero` (a sigil before a name is one message since `4441148b`; before a keyword it is not) · **class: adjacent**

    **Origin:** lane recovery-b4's report (*`@return 1`: two messages*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/at-return/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **180 — a range written `1..2` is told twice as a field access** | `x = 1..2`: `expected_field_name` at 3:11, *found `.`*, and again at 3:12, *found a number (`2`)*; `x = 0x1..5` the same at 3:13 and 3:14 | `selfhost/grammar_expr.hero:343` · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e20.hero`, 2026-10-02), queued as *a range habit, recovery's file*; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/range-dots-twice/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The recovery instrument counts its `range-dots` operator at 12 EXTRA.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **181 — a decimal number with a second point, `1.5.2`, is told as a field access** | `x = 1.5.2`: `expected_field_name` at 3:13, *expected a field or function name after `.`, found a number (`2`)* | `selfhost/grammar_expr.hero:343` · `selfhost/number.hero` · defect 154's closed record (*Left, queued*) · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e08.hero`, 2026-10-02), named *Left, queued* in `docs/records/done/2026-10-02-1415-defect-154-closed-a-based-literal-with-a-fraction-is-told-as-the-number-it-is.md` and on no open list; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/float-second-point/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). Lane arm: no existing code covers it, so a new one is a sitting's.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be. The recovery cluster's.

- [ ] **182 — an operator alone on a line deeper than a `match`'s arms costs `continuation_outside_brackets` and `unexpected_block`** | `k = match n` over `1 => "one"`, then a line holding only `+` (or `-`) one level deeper, then `_ => "many"`: two messages on line 5; the `certain` deletion of the operator, applied, checks clean | `selfhost/open_line.hero` · `selfhost/sign_above.hero` (the deletion, defect 165's `8cb4ba6c`) · the orphan block's `unexpected_block` · **class: adjacent**

    **Origin:** lane h158's pass for defects 165 and 166, 2026-10-02 (`scratchpad/lane-h158/d166/q1_plus_deeper.hero`, `q2_minus_deeper_wild.hero`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/operator-deeper-line/`), where the one fix was a guess, and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where it is the `certain` deletion 165's repair brought; the second message stands.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **183 — a `.pc` whose `prefix` holds an unescaped space is refused naming `space/include` as the flag, and not the package file's line that splits it** | `prefix=/opt/with space`, `Cflags: -I${prefix}/include`: `pkg-config --cflags s7` prints `-I/opt/with space/include`; `build` exit 1, `ffi_package`, *the package `s7` answered with `space/include`, which this compiler does not pass on* | `selfhost/cli/shell_split.hero` (the word splitter, defect 162's `b37bfce1`) before `filter_words` (`selfhost/cli/libraries.hero:83`) · **class: adjacent**

    **Origin:** lane h158 beside defect 162, 2026-10-02 (`scratchpad/lane-h158/d162/pc/s7.pc`, 2026-10-02, *true, could say more*); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/pc-prefix-space/`, run with `PKG_CONFIG_PATH` naming that folder) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where only the note's list of accepted flags is worded otherwise.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

- [ ] **184 — `check` takes time quadratic in the `for` heads it refuses: 3,000 `for n > 0` lines cost 16 s** | a function of N loops `for n > 0` over `n @ n - 1`, `check --brief`: 0.44 s of user time at 500, 1.74 at 1,000, 3.87 at 1,500, 15.92 at 3,000, `real` within 0.16 s of `user`; 3,000 `while n > 0,` cost 0.15 s and 3,000 `n @ 0X1` 0.06 s | `selfhost/grammar_expr.hero:899` (`for_stmt`) · `selfhost/parse/loop_habit.hero` (`refuse`), the cause unrun · **class: adjacent**

    **Origin:** the coordinator's file-queue agent, 2026-10-02, measuring lane arm's item on field-place pushes, on `62d65e48` (2026-10-02, `scratchpad/file-queue/for-habit-quadratic/`), the machine at load 2 to 5; re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), the times above being that run's, on a busy machine and in the same order as the first. Two candidates were measured out over the same 3,000: `loop_habit.hero:62`'s append made in place, and `for_stmt`'s trial parse removed.

    **Why it is a defect.** Defect 146's class: a file of N mistakes costs N² work.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): real, found beside the work, no wrong value and no crash.

- [ ] **185 — the emitted C's line restores after a fixed-array field's assertion name a line one too low per such field** | `heroes build tests/golden/run/ffi-a-char-field-becomes-text.hero --emit-c`: line 20 is `#line 19 "ffiacharfieldbecomestext.c"`, so line 21 is reported as 19, and every later restore is 2 short; over `tests/emission`, 261 of 36,781 restores in 19 files are 1 to 10 short, each file's shortfall equal to its number of two-assertion lines | `selfhost/emit/extern_field.hero:158`, `:161` (a `"\n             _Static_assert(` the printer does not count) · `.claude/rules/generated-c.md:27-29` · **class: adjacent**

    **Origin:** lane round1002b at its gate, 2026-10-02 (*not chased*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/line-restore/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a place less exact than it could be in every clang note, sanitizer frame or debugger line past such a field; no value moves.

- [ ] **186 — `ffi_return_type` says a function does not return the program's type and never names the type the header gives** | defect 172's applied program: *`cJSON_AddItemToObject` does not return `i32` — that is what `cj.h` says, and clang read it*, its note *correct the result type, or name the header that declares this*; neither `int64_t` nor `i64` appears | `selfhost/emit/ffi_declared.hero:60` · **class: adjacent**

    **Origin:** the coordinator, 2026-10-02, building defect 172's applied program on `62d65e48` (2026-10-02, `scratchpad/apply-int/case/`), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The pointee check's message beside it names the header's type and offers it (`ffi_parameter_type`, *the header's `int64_t *` points at a different width*, fix *declare `p` as `@p: i64`*).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be: it carries the header's name and not the type that would fix the program (design.md §4.17).

- [ ] **187 — four parser diagnostics are still appended through a field place, against defect 146's rule and `cursor.hero`'s own comment** | `git grep -n 'c.diagnostics @ c.diagnostics.push' -- selfhost/parse/` prints `loop_habit.hero:62`, `line_end.hero:251`, `type.hero:257`, `type.hero:320`, while `selfhost/cursor.hero:272` says *Every parser module appends through this* | the four lines · `cursor.push_diagnostic` (`selfhost/cursor.hero:273`) · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/field-place-push/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), which finds three more modules appending the same way, their cost unmeasured. No cost is measured for any: at `loop_habit.hero:62` the append is not what makes defect 184 slow.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form nobody needs to be right.

- [ ] **188 — two texts still state the arm rule panel 185's R3 replaced** | `selfhost/check/lending.hero:108-109` quotes the spec as *An arm that does nothing is a block holding `_ = 0`*, where the spec now reads *An arm that does nothing holds `_ = 0`*; `tests/golden/check/fixedbugs-135-a-discard-that-is-the-line-s-one-reading.hero:8` reasons *`_ = ` on the arm's own line is `declaration_in_arm`*, false since R3 | the two lines · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/stale-arm-texts/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). design.md §4.7 owes nothing: R3 brought the spec to it.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): no program moves.

- [ ] **189 — `typeorder.visit`'s walk without recursion has no witness** | the record chains the run cases emit are 1,000 deep (`run/fixedbugs-140-records-` and `-extern-records-a-thousand-deep-build`), and at 1,000 and 5,000 lane ci140's mutant, `visit` restored to its recursion, emits the trunk's bytes (666,776 and 3,358,806, `cmp` equal) | `selfhost/emit/typeorder.hero:130` and its one test at `:206` · **class: improvement**

    **Origin:** lane ci140's report, 2026-10-02 (`scratchpad/lane-ci140/mut/emit-typeorder/heroes-mut`, 2026-10-02, *the mutant that no case catches*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/typeorder-witness/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). At 20,000 deep the mutant reached clang, which died (defect 170), so a unit test is the witness the lane names.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): coverage.

- [ ] **190 — `records/lists` reads an item's class token and not the rest of its first line, so an item that lost its title and its *where* field reads as whole** | item 130's first line as `8349d264` left it, with no `**` closing its title and no ` | ` at all, before its class token: `records` 24 passed, 0 failed, after that commit and after every one until the item was restored at `50644159` | `tests/harness/suite_records.hero` (`list_offences`; the class rule, about `:4448-4610`) · **class: improvement**

    **Origin:** the coordinator, 2026-10-02, on the damage panel 187's completeness critic found (`docs/records/log/2026-10-02-2141-item-130-cut-in-8349d264-and-restored-what-cut-it-is-unknown-the-commit-did-not-read-its-diff.md`). It would have caught the cut in the item's line, not the one in its body.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument; nothing a program does moves.

- [ ] **191 — a record named after a typedef of a struct the header keeps opaque, with fields, stops `build` at exit 2 under clang 18, whose refusal of a member read through it does not name the typedef** | `tests/golden/unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, `ffi_unknown_tag` expected: on the public CI's Linux x86-64 leg (Ubuntu clang 18.1.3), `internal error: compiling the generated C failed`, clang's *incomplete definition of type 'struct opaque_s'* three times at the field assertions, exit 2; Apple clang 21 on this Mac words it *'opaque_t' (aka 'struct opaque_s')* and the case passes | `selfhost/emit/ffi_incomplete.hero` (`incomplete_typedef`, which finds the record by the typedef's name in clang's words) · `f2a08f13` (panel 186's layout route, which removed the unit's positional completeness probe) · **class: blocking**

    **Origin:** the public CI on `07ccb72a`, run 37065944766, read by the coordinator (2026-10-02, `scratchpad/ci-x86-07ccb72a.log`, lines 1726 to 1763); reproduced by the coordinator under clang 18 in the arm64 container (Debian clang 18.1.8): `unsupported fixedbugs-145` 8 passed and 1 failed on `07ccb72a` and on `b48d02b8`, 9 and 0 on `8b98bcc7` (the last green CI) and on `415c0a14`, so the round merged at `b48d02b8` brought it; this Mac's clang, Debian clang 22.1.8 and Windows' clang 23.1.1 pass it. Reproduced by lane cb4 on `c2b3f3a1` under Debian clang 18.1.8, the same 8 and 1, and measured on both clangs beside it (2026-10-03, `scratchpad/lane-cb4/d191/words/`): the member read is the one refusal worded apart, a variable of the type, `sizeof` of it and a typedef of `void` are worded alike on both, and every position is the same. The positional probe `f2a08f13` removed was a variable of the record's type, which is why the case passed under clang 18 before it.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a red CI.

*******************************************************************************
