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
**OPEN: 5**

- [ ] **129 — a line holding `-` alone draws two certain joins, and applied together they write a program `check` refuses** | in `function main()`, `x = a` over a line holding `-` alone over `print(x)` costs two `continuation_outside_brackets`, and `check --apply` writes `x = a -` over `print(x)`, which `check` refuses again; in a `match`, `0 => 5` over `-` over `"=>"` has both joins applied, `0 => 5 - "=>"`, which is `bad_operand` | `selfhost/open_line.hero` (`goes_on_at_head`, `refuse_the_end`)

    **Origin:** lane 123's agent, 2026-09-28, attacking the positions beside
    defect 126 (a statement where 126 has an arm, and a string where it has a
    pattern; reproducers `shapes/x04_stmt_minus_alone.hero` and
    `shapes/x07_arrow_in_string.hero`, applied in `shapes/apbase/`, in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `6b81e993` at 18:14
    (`stmt_minus_alone.hero`, `x07_arrow_in_string.hero` and their applied
    files in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect129/`).

    **Why it is a defect.** Panel 181's item 3: a `certain` fix that fails to
    compile is a defect of the landing. Each join is certain alone; the line
    holding `-` alone draws one at its head and one at its end, and nothing
    judges the two together. Defect 126 is the same shape on an arm; this is
    it at the statement and before a value that is not a pattern.

    **Carried to the next milestone**, by the author's instruction of
    2026-09-28 16:40 (`docs/records/log/2026-09-28-1640-m-agreed-retention-closes-over-the-defects-found-after-its-last-eight.md`):
    found after the eight M-agreed-retention repairs before its tag, and
    not repaired in it.

    **Widened 2026-09-28 at 23:00 by the coordinator, on the trunk's compiler
    at `79aeeffa`, to two classes, neither of them the `-`'s.** The
    directories cited above were gone by 22:55, removed by nobody in this
    conversation; `stmt_minus_alone.hero` and `x07_arrow_in_string.hero` were
    copied at 22:47 and the shapes below written beside them
    (`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d129/`, 2026-09-28).
    (A) **The joins of one statement broken at two or more line ends are
    judged one at a time**, so a `certain` one applied without the others
    writes a line the same code refuses: `+`, `&&` and `+ b *` alone on the
    line do what `-` does (`p1`, `p4`, `p3`), and so does a statement with no
    line holding an operator alone, `x = y +` over `z +` over `print(x)`,
    joined to `x = y + z +` (`q12`), `x = y` over `+ z +` (`q14`), and a
    chain whose second break carries a comment and so no fix (`q15`); where
    every join is `certain`, `x = a` over `-` over `b`, the three-line join
    is right and compiles (`x05`, `p2`, `q13`). (B) **A `certain` join writes
    an operator against a literal it can take under no type**: `x07`'s
    `5 - "=>"`, `x = a` over `- "s"` (`q10`), `x = a &&` over `1` (`q11`), and,
    beside defect 126, the sign join `-"a" => 10` on a `str` match (`p5`);
    `-'a'` on an `i64` match compiles (`p6`), since a character is an integer.

    **Corrected 2026-09-28 at 23:17**: the directories were removed by the
    author, who took them for old lanes, and restored from the trash the
    same evening, so the paths above resolve again; the copies in the
    scratchpad are what the lanes read.

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

*******************************************************************************
