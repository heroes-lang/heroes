# groupremark096: the blank line under a remark survives inside a group

Defect 096 (the skeptic seat over defect 095's repair, 2026-09-25, attacking
it at the shapes beside it). `middle.hero` writes an `extern` group whose
second member is followed by a remark, a blank line, and `record Pool tag
pool`: `check` was exit 0 and `fmt` exit 2 with *changed the TREE*, because
the group's member walk printed the remark at the member's indent and never
emitted the blank line under it, so the re-parse read the remark as `Pool`'s
documentation. The blank line between a comment and the declaration under it
is design.md §4.1's discriminator, the one blank line in the language that
carries meaning: directly above, the comment is documentation; a blank line
between, it is a remark. The parser reads it (`selfhost/cursor.hero`'s
`take_docs`), `outline` and `???` print it, so deleting it is a different
program.

`beside.hero` is the shapes beside that one, measured the same day on the
seed: a remark and a blank line before the FIRST member (the blank was deleted
at exit 0 with the same tree, since no comment between the head and its first
member is ever a doc: `selfhost/parse/group.hero` gives the first member the
doc read above the `extern` keyword and never calls `take_docs` for it; the
blank is kept now all the same), a doc directly above a member (untouched,
and it must stay a doc), a blank line, a remark and a blank line before a
middle member (exit 2), and two remarks with blank lines between and under
them before the LAST member (exit 2). `filelevel.hero` is the same shapes
outside any group, which the repair must not change: every one of them was
already kept, and the file pins that it still is.

The repair, landed with defects 099 and 100 in `selfhost/print/page.hero`:
`comments_above` prints the comments above a member and, when there were
any before its doc, the blank line the author left under the last of them.
The group's member walk calls it, and so does the flush between a head and
its first member, which has no doc to protect; the file-level walk keeps its
own rule, one blank line between top-level declarations. A blank line
between two members with no comment above is layout, not content, and is
still joined: the members of a group stay a run.

A blank line is one the SOURCE has (`page.hero`'s `blank_between`), never a
gap a line `fmt` dropped left behind. That is the regression the skeptic seat
found in this repair's first form, which counted lines: two groups with the
same head one under the other and a doc directly above the second head. `fmt`
merges the two and drops the second head, the dropped line counted as a
blank one, and a blank line went between the doc and its member, which made
it a remark: exit 2, where the trunk printed the doc above the member at exit
0. `samehead.hero` pins that shape and two beside it, a doc above a third
head with no blank line anywhere and a doc of two lines above a fourth. It is
not canonical: `fmt` prints it merged, each doc directly above its member.

Three shapes of the same bookkeeping are repaired with it, in `fmt.hero`'s
own tests rather than here because they are canonicalisations and not
fixpoints. A signature written across lines with no comment inside joins onto
one line, and a doc directly under it gained a blank line ABOVE it at exit 0
(the rule was keyed on the name's line rather than the `)`'s), and so did a
comment on the first line of the body under a Heroes function's joined
signature (the skeptic's `p05f`). And a comment on the first line of an
`else` block gained one the same way, the `else` line counting as a gap.

The two shapes this repair measured and first left as they were, a remark
after the last member of a group and after the last statement of a body or
the last field of a record, are defect 099's, and are repaired with it: see
`../comments099/`.

`middle.hero`, `beside.hero` and `filelevel.hero` are canonical, and
`tests/harness/suite_surface.hero` runs `fmt` on each and expects exit 0 with
the file's own text on stdout, which is the self-check's three properties
passing: the output parses, is a fixpoint, and holds the same tree. For
`samehead.hero` it expects the merged text.
