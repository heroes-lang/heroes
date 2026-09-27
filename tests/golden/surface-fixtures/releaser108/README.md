# releaser108: a comment inside a releaser set is part of no name

Defect 108 (panel 180's completeness critic, 2026-09-27): a comment inside an
`extern` member's releaser set became part of the next releaser's name.
The parser hands a set on as one span, and `selfhost/handles.hero`'s
`releasers` read that span's text dropping only the blanks, so
`acquires h_close |  # either one` over `h_close_v2` named
`#eitheroneh_close_v2`. The checker refused it with `unread_releaser`, and
the same reading fed the emitter's set, the contract two modules'
declarations of one C function are compared by (`check/contracts.hero`),
and `fmt`'s spelling of the set.

`releaser_spans` is the one scanner now, and a comment, which runs to its
line's end and may hold a `|`, is part of no name; `releasers` reads the
names off the spans, so a name and its caret cannot disagree. `fmt` prints
a set with a comment inside through the same break its types take
(`print/headline.hero`), so the comment stays after the `|` it follows, and
the output guard compares each comment inside a set by the word whose set
holds it and the set's tokens before it (`print/insets.hero`): the guard
leaves every mark out of the tokens it compares, since `fmt` reorders the
words, and a comment moved past a set's last name had nothing to cross.

`sets.hero` holds the places a comment can stand in a set, on each word
that takes one: after a `|` at a line's end (`acquires`), on a line of its
own and after a second `|` with a `|` inside the comment (`acquires`), two
comments (`transfers`), and after the set's last name before the list's `)`
(`retains`). A line may end inside a set only after a `|`: after the word,
or before a `|`, the grammar refuses the break with or without a comment,
and a result's set has no bracket to break in. `rel.h` gives the program the
calls it makes, so the rows can run it: `check` accepts it where the trunk's
compiler at `f39836a6` refused it, `run` ends each handle with a name its set
gave past a comment, and `fmt` keeps every comment where it is written,
where the trunk spelled each set on one line, the comment inside it
swallowing the rest of the line, and refused its own output at exit 2.

`commented.hero` and `contract.hero` are two modules declaring one C
function, the first with a comment inside the set and the second without:
one contract, which the trunk refused with `contract_differs`, naming
`#eitheroneh_close_v2`. `check` accepts the pair and `run` keeps it.
