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
inside them (`print/between.hero` finds them in the source). Parentheses
around no comment are still dropped where they bind nothing.

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

Beside the printer's own check, the output guard now asks the two files
where each comment sits among the code (`selfhost/print/anchors.hero`): how
many of the tree's positions come before it, how many expressions end
before it, and whether it trails code. A comment any printer moved changes
its anchor, and `fmt` refuses: "`fmt` would move the comment on line N".

Every output is pinned in `tests/harness/suite_surface.hero`.
