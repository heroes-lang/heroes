# paramcomment095 — a comment inside a parameter list stays inside it

Defect 095 (the landing review of M-agreed-retention step 11, its surface
finder, 2026-09-24). `following.hero` writes a group member one parameter
per line with a comment between two of them and a member after it: `check`
was exit 0 and `fmt` exit 2 with *changed the TREE*, because the one-line
signature printer had no place for the comment, expelled it after the
declaration, and `ob_put` took it as documentation. `last.hero` is the same
member with nothing following: the comment left the group as a file-level
remark and `fmt` exited 0 having moved it, the silent shape. `heroes.hero`
is the shapes beside those two, measured the day of the repair: a Heroes
function outside any group with a comment after the `(`, trailing on a
parameter, before the `)`, trailing on the closing line, and alone in an
empty list, every one of which was moved into the body at exit 0.

The repair: the tree records where a signature stops
(`ast.FunctionDecl.signature_end`), and a signature whose list holds a
comment prints one parameter per line, the comment on its own line where
it was. Every list without a comment keeps the one-line shape.

All three files are canonical. `tests/harness/suite_surface.hero` runs
`fmt` on each and expects exit 0 with the file's own text on stdout, which is
the self-check's three properties passing: the output parses, is a fixpoint,
and holds the same tree.
