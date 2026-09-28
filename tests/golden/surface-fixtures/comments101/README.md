# comments101: a comment stays on the line it trails and in the bracket it is in

Defect 101 (the skeptic seat over the repair of defects 096, 099 and 100,
2026-09-25), with the regression that repair made and the `else` beside it.
`heroes fmt` moved a comment away from the code it was written beside, at
exit 0 with the tree unchanged, so no guard saw it; or it refused at exit 2
a program `check` accepts. Every file here is measured on the trunk's
compiler at `3b40c60d` and on the lane's first repair, the same day.

`blankunderhead.hero` is the regression: a doc directly above a second
`extern` head with the same header, and a blank line under that head. `fmt`
merges the two groups and drops the second head, and the first repair
printed the blank line under it between the doc and its member, where the
re-parse no longer reads it as the doc: exit 2, where the trunk printed it at
exit 0. A doc of two lines above a third head with two blank lines under it
is the same; a remark under a fourth head with a blank line between them was
refused by the trunk at exit 2 and printed with that blank line above it by
the first repair. `fmt` prints the file merged, each doc directly above its
member: a blank line under a head the merge drops belongs to that head
(`selfhost/print/page.hero`'s `gap`).

`remarkdochead.hero` is the same family and older: a remark after a group's
last member, then such a doc and such a head. Merged, the remark and the doc
stand at the members' column as one run, which the re-parse reads as the
doc of both, so the remark takes the blank line that keeps it a remark and
the doc gains none. The trunk and the first repair both refused it at exit
2; the same with the remark at column 2 is the shape beside it.

`openers.hero` is the defect's first shape: a comment trailing a line that
opens a block (`if`, `else if`, `while`, `for`, `match`, a block arm, a
group's head, an `if` or a `match` that is a value) was printed as the
block's first line at exit 0. On a variant's head and on `else` the trunk
refused at exit 2, and the first repair moved the comment under the line at
exit 0, which is the silent form. Each stays on the line it trails
(`page.opened`). The file is canonical.

`trailinghead.hero` is the shape beside it at a group's head: a comment
trailing a second head with the same header, whose line a merge drops. The
trunk refused at exit 2 and the first repair printed the comment as a remark
under the first group's member; `fmt` keeps the second head now
(`page.keeps_head`), as it keeps one whose merge would move a doc. The file
is canonical.

`brackets.hero` is the second shape: a comment inside a bracket. `fmt`
printed the elements one per line and left the comment to the next walk,
which at a body's end printed it at column 0 above the next declaration. It
stays in its bracket now, on its element's line: after the last element of a
list, after a call's last argument, inside a call that is an argument,
inside an operand, inside a receiver, after a map's entry, above its last
one, and on the opening and closing lines (`selfhost/print/brackets.hero`).
The output drops the lists' commas, which §4.9 asks of a list written down
the page.

`arms.hero` is the third shape: a comment between two `match` arms, and one
at the end of a block arm, gained a blank line under it the source does not
have. The arm walk counted lines, and a comment's line is not a blank one;
it asks the source now (`page.blank_between`). The file is canonical.

The files below are the defect's last round, 2026-09-26, and measure what
the first repair did with each: it refused at exit 2 a comment it could not
keep (`page.Fmt.moved`), and the coordinator's rule stands that a formatter
refusing a correct program still cannot format it. Each is kept now.

`signature.hero` is the one silent move the first repair left: a comment on
the `(` line of a signature written across lines, after a parameter there.
`fmt` joined the signature and the comment went to the joined line's end,
at exit 0, the same bytes as the trunk, and the printer's check could not
see it. The signature keeps one parameter per line now whenever a comment
sits from its `(` line to its `)` line (`page.comment_inside`), and a
comment trailing a line two parameters share goes after the second.
Corrected 2026-09-26: of a list written across lines; one written on one
line stays on one (`results.hero` below), and the rule's home is
`selfhost/print/headline.hero`, which took the signature out of
`print/fmt.hero` that day.

`conditions.hero` replaces the first repair's `refused.hero`: a comment
inside the value of a line that opens a block, in an `if`, an `else if`, a
`while`, a `for` and a `match`, on the opening and closing lines, and in a
condition written inside parentheses. The value is opened where the
comment is (`print/fmt.hero`'s `block_head`).

`gaps.hero`: a comment between an argument's name, a map's key or a
mutable argument's `@` and the value. The name keeps its line and its
comment, and the value goes on the next.

`parens.hero`: a comment inside parentheses, which the tree does not keep.
§4.15 breaks a long expression inside parentheses, so `fmt` keeps them
where a comment needs them, breaking a chain after its operators, one
operand per line, and keeps the author's own where a comment sits just
inside them (found in the source's tokens: `print/owners.hero`'s `levels`,
which replaced a byte scanner, `print/between.hero`, on 2026-09-26, see
`holes.hero`; `levels` is `print/groups.hero`'s since the fourth round).
Parentheses around no comment are still dropped where they bind nothing.

`parts.hero`: a comment in the part of a value before its own bracket, a
key, a receiver, a callee, a base, and the prefix of a control form. Each
part holding a comment is opened and the rest goes on its last line.

`nested.hero`: several comments in nested brackets, on every opener and
closer. The file is canonical.

`lastline.hero` and `lastbracket.hero` end with no newline, on a trailing
comment and inside a bracket; `fmt` ends each with one and keeps the
comment on its line.

A CRLF file is not a fixture here: `.gitattributes` normalises every
checked-in file to LF, so a CRLF fixture would test LF. The CRLF shapes are
`selfhost/print/fmt.hero`'s test on this round's four refusals and
`selfhost/print/page.hero`'s on blank lines, and every probe of the round
was run again with CRLF line ends.

Beside the printer's own check, the output guard asks the two files where
each comment sits among the code (`selfhost/print/anchors.hero`), and `fmt`
refuses where they disagree: "`fmt` would move the comment on line N".
Corrected 2026-09-26: the first form of that check counted the tree's
positions before each comment, and the skeptic seat over 832478b3 showed
six moves it answered with nothing, among them an inner block's last
comment moved out to the outer block and a comment trailing `else` moved to
the line above. It compares each comment's owner block, by the rule
`print/owners.hero` places it with, and the tokens `fmt` keeps on either
side of it; the six moves are its unit tests.

The files below are the defect's third round, 2026-09-26, the skeptic
seat's findings over 832478b3, each refused at exit 2 or moved at exit 0 by
that commit, and each kept at exit 0 now.

`closer.hero` is defect 099's own class: a comment after a block's last
statement when the statement ends with the author's `)` alone on its line.
The `)` line was not counted as the statement's, so code seemed to stand
between it and the comment, and the comment left the block at exit 0: after
a body (`m1`), an `if` block (`s2`), a match's last arm (`s5`), a loop. A
line inside a bracket is part of the logical line it continues now
(`page.ended`, `owners.end_of`), and never a code line that closes a block.

`runs.hero` is `s8`: a column-0 comment above an `if` block's last comment
pulled that comment out with it. A run of comments is read from its last
one up, and no comment belongs to a shallower block than one after it.

`unitresult.hero` is a regression the second round made (832478b3; trunk
exit 0, lane exit 2): a comment trailing `-> ()`, which `fmt` drops and the
old guard counted.

`closeparen.hero`: a comment trailing the author's `)` line, which `fmt`
drops where the parentheses bind nothing (`p4`, a `while`'s, `k1`, `k2`).
The statement's or the element's last line is the `)`'s now, and the
comment trails what is printed.

`types.hero`, `indexes.hero`: a comment inside a type (`t1`, `t2`, `t7`) or
inside index brackets (`r1`, `r4`, `r5`, `r6`, `t4`), which `fmt` prints on
one line. The line breaks after the token the comment follows now, the rest
going on four columns in (`brackets.lead`; corrected 2026-09-26: it is
`selfhost/print/breaks.hero`'s `lead`).

`holes.hero`: a `)` or `(` inside an interpolation's hole, inside the
author's parentheses with a comment (`h4/f1`, `f3`), which the byte scanner
took for the group's.

The CRLF and no-final-newline forms of every shape of this round are the
skeptic seat's generators, rerun, and `selfhost/print/owners.hero`'s tests.

The files below are the defect's fourth round, 2026-09-26, the second
skeptic seat's findings over e0e08d18, each refused at exit 2, moved at exit
0 or printed a column too far by that commit, and each kept at exit 0 now.
The seat's bracket-break generator, a comment after every token inside a
bracket, trailing it, on a line of its own, or both, reads 0 refused of the
5,223 of its 7,464 variants that parse.

`unitinside.hero` (`k01` to `k07`): a comment inside a `-> ()` the author
wrote. The guard read the `()` around the comment as kept tokens, its
streams parted, and it compared the rest by text alone, so the comment moved
into the parameter list at exit 0; with a parameter it was refused. `fmt`
keeps the `()` where a comment is inside it, the one place that comment
has, and the guard reads `-> ()` over the code tokens and refuses every
comment past a point where the streams part.

`dots.hero`: a comment after a `.`, of a field, a method or a variant case,
and a name with a `(` on a list's next line, which the lexer's terminator
makes a new element and no call. The one-line text breaks after the token
the comment follows; a bracket opened after that break opens four columns
in.

`operands.hero`: a comment just inside the author's parentheses around an
operand, `x = (  # c` over `a + b) * 2`. The tree's span of the value starts
at `a`, so the comment was outside it; a value's extent takes the
parentheses at its edges now (`selfhost/print/groups.hero`'s `extent`), and
the group is opened.

`innermost.hero`: a comment after the outer of two parentheses around one
value, inside index brackets. The one-line text's parentheses were matched
one by one and the comment moved inside the inner pair; a printed pair is
the written pair around the same tokens now, the innermost of several
(`breaks.aligned`).

`leads.hero`: a comment after a parameter's `@`, and between an argument's
name and its `@`. A parameter starts at its `@`, and a lead breaks after the
token its comment follows.

`results.hero`: a comment inside the result type of a signature with no
parameters, which opened the list and moved into it. A list on one line
stays on one.

`commas.hero`: a comma between two comments, one trailing the value before
it and one trailing the comma, keeps a line of its own
(`owners.lone_comma`).

`pieces.hero` (`i01`, `i02`): a line broken after two comments' tokens; the
piece between the breaks kept the space before it, a column too far.

`ascending.hero` (`d02`): a run of comments whose columns climb, placed in
the block of its last comment. A stated canonical form and not an accident:
`selfhost/print/owners.hero` writes the rule and its reason.

The files below are the defect's fifth round, 2026-09-26, the third skeptic
seat's findings over 83ac68c1: no comment moved at exit 0 there, and each
of these was refused at exit 2 or printed where it read wrong. Each is kept
at exit 0 now.

`indexparens.hero` (`min1`, 571 of the 575 refusals of the seat's
parenthesis generator): a comment inside the author's parentheses inside
index brackets. `fmt` dropped the parentheses and broke the line after the
value, where the parser skips a line's end before a group's `)` and not
before an index's `]`, so the output did not parse. The one-line text keeps
the author's parentheses around a comment it breaks at, and every pair of
theirs inside such a pair around the same value (`breaks.restored`).

`twocomments.hero` (`nf1`): two comments around the outer parenthesis in
index brackets; the second went to a line of its own and the second pass
read it as one, no fixpoint. With the parentheses kept each comment trails
its token. `innermost.hero` above prints its outer pair too now.

`doubleparens.hero` (`dp1`): a comment just inside the outer of two pairs
around one value. `fmt` kept the outer pair and dropped the inner, and the
guard, which counts the innermost of several pairs, refused. Every pair of
the author's inside a pair kept for a comment is kept too (`Place.all`).

`parenplace.hero` (`pl1`, and 43 variants in `examples/logs` and
`examples/ledger`): a comment inside the parentheses around a mutated place.
The place begins and ends at its parentheses now (`groups.whole`).

`qualified.hero` (`q2`, which the trunk refuses too), with `money.hero`: a
comment after the `.` of a type named from another module. The type's text
was its source's, the comment in it, so `fmt` printed the comment twice and
`check` looked for a type called by the comment's words. A named type is
read as its two names and the dot (`selfhost/type_text.hero`'s `named_text`).

`continuations.hero` (`m3`, `sh3`): the lines of a block's head `fmt`
breaks to keep a comment. A continuation at the body's column read as the
body, and one starting with `)` at the head's own column read as a
statement the block hung under. Every line of a head after its first goes
eight columns further in than elsewhere, four past the body
(`selfhost/print/margins.hero`'s `deepened`): the four-columns-in of
`types.hero`, `results.hero` and the rest above holds outside a head, and a
signature's parameters, `signature.hero`'s and the defect-095 files', stand
twelve in, their `)` eight.

`insidegroups.hero`: the shapes panel 179's compiler-engineer found over
83ac68c1 with the seats' bracket-break generator ported to Heroes, on
`parens.hero` and `innermost.hero`. A unary operator's comment inside the
author's parentheses kept for a comment gained a group of `fmt`'s own
inside them, which the second pass read as the author's (no fixpoint); the
line breaks inside the kept pair now. A comment after an argument's `(` was
read as the argument's lead and moved before the `(`; the lead ends at the
author's parentheses around the value. The third shape, a comment before
the outer `)` of two pairs in an index, this round's first repair already
kept. `qualified.hero` gains the seat's `walk.hero` shapes: a comment on a
line of its own after the dot, one trailing the dot with another under it,
and one after the dot inside an array parameter's type.

A comma after a comment now keeps a line of its own whether or not a
comment trails it (`commas.hero`, `owners.lone_comma`): the seat's second
reader counts a comma as a token both files have, and a comma moved up to
its value crossed the comment.

`nextvalue.hero` (the continuation-column generator over `qualified.hero`,
2026-09-27, and the shapes beside it): such a comma's line took any comment
further along it, one inside the value after the comma, and printed the
value without it, refused at exit 2, or, with the comment at the value's
end, no fixpoint. The comma's line takes a comment only when it is the
comma's next token (`margins.comma_line`).

`commacomment.hero` (`heroes probe`'s first run over the fixtures, panel
179, 2026-09-28): a comma the author wrote at the start of a line of its
own with a comment after it, and nothing between the value and the comma.
The probe's bracket break after the `1` of `brackets.hero`'s `[1,  #
inside a receiver` makes one, and so did the same break in
`conditions.hero` and `parts.hero`. The comma went up to its value, or in a
list or a map down the page was dropped, and the comment after it went on a
line of its own inside the bracket, which the guard refused at exit 2 in a
list, a map, a call, a construction, a method call and a signature. Such a
comma keeps its line now, as one after a comment does
(`margins.commented_comma`).

Every output is pinned in `tests/harness/suite_surface.hero`.
