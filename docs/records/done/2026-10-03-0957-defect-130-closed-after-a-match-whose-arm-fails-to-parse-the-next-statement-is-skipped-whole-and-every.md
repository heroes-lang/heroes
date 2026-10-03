# Defect 130 closed: after a `match` whose arm fails to parse, the next statement is skipped whole, and every mistake in it goes unreported

- [x] **130 — after a `match` whose arm fails to parse, the next statement is skipped whole, and every mistake in it goes unreported** | in `function main()`, three bound matches `a = match n`, `b = match n`, `c = match n`, each with the arms `+ => 10` and `_ => 20`, report lines 4 and 10 and never line 7; two statement matches `match n` over `+ => print(1)` report only the first; and a plain statement after one such match, `y = 3 )` over `z = 4 )`, reports the second line and not the first, where the same two lines after no `match` report both | `selfhost/grammar_expr.hero` (`match_expr`) · the enclosing statement's recovery · **class: systemic** · **closed 2026-10-03**

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

    **2026-10-03, lane rec187, panel 187's R1: the shapes beside the item
    close into items and repairs** (the compiler engineer's § 1, each item
    carrying its pin under `tests/golden/check/panel-187-*`): `g4/ti` into
    defect 199, `g2/r12` into 200, `g1/a11` into 195; `hc/h09` and `g1/b07`
    are one message per mistake (class (f), the sitting's F3); `g2/r09` is
    repaired by the sitting's R5 and row 130-34a by its R6, each recorded
    below as it lands. The item stays `- [ ]` here; the round's gate closes
    it.

    **2026-10-03, lane rec187, panel 187's R5: a body told missing names its
    declaration** (`g2/r09`; beside it with its cause, a constant's and a
    test's, and a head whose signature or type failed after its name):
    repaired at `eddb0a7c`, gated by its own cases; the rest is owed at the
    round's gate. Beside it with another cause, a body written at its head's
    margin is defect 202.

    **2026-10-03, lane rec187, panel 187's R6: row 130-34a** filed as defect
    203 and repaired at `97008231`, gated by its own cases; the rest is owed
    at the round's gate.

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

    **Closed 2026-10-03** by panel 187's R1, ratified that morning (the author's answer *1a*): each of the item's rows is repaired (V1, `29425af6`; V5, `eddb0a7c`; row 130-34a's message, R6, `97008231`), filed `adjacent` apart one item per cause (193 to 202, 204, 205), or pinned as a known cost in the ten `panel-187-*` goldens (`3bf8e348`), as lane rec187's dated lines above record; the landing gated at the round of 2026-10-03's gate `b43d224d` (the seed's fixpoint by `cmp`, the compiler's own tests 1,079, the net's own 200, the full net 4,901 passed and 0 failed over 26 suites), and the recovery instrument's reading of that round's compiler against the trunk's over the frozen plan showed no mutant worse for the recovery (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03).
