---
kind: defect
area: print
milestone: none
filed: 2026-09-24
commit: f1e2132f06d03c40b808bf2fe98cc50799ad1f4f
github: none
---

# Defect 095 closed: a comment inside a parameter list stays where the author put it

2026-09-25, M-agreed-retention step 13, in lane `42b76e38`, merged `fd5c7a14`.
Found by the surface finder of step 11's landing review; measured older than
the lane by the parser seat that repaired the landing.

- [x] **095 — `fmt` refuses a group member whose parameters are written one per line with a comment between two of them** | the signature printer joins the parameters onto one line and has no place for the comment, which is expelled after the declaration and becomes the next member's doc, so the self-check sees a different tree and `fmt` exits 2 on a program `check` accepts | `selfhost/print/fmt.hero` (`signature`) · `selfhost/parse/members.hero` · **closed 2026-09-25**

    **Origin:** the landing review of M-agreed-retention step 11, its surface
    finder, 2026-09-24; reproduced by the parser seat against the seed compiler
    of the same day (`fmt` exit 2 before the lane and after it), so it predates
    the landing.

    **The reproducer.** An `extern` member written

        function ob_get(
            o: Ob,
            # a comment between
            n: i64
        ) -> Ob retains ob_put

    followed by another member: `check` exit 0, `fmt` exit 2 with *`fmt`
    changed the TREE of … the output parses and is a fixpoint, and it is a
    DIFFERENT PROGRAM*, and the file is not touched. With no member following,
    the comment lands after the group as a file-level comment and `fmt` exits 0
    having moved it. Measured with `parse --dump-ast`: the one line that
    differs is `doc # a comment between`, attached to the following member.

    **Why it is a defect.** A correct program is refused by a tool that must
    be idempotent on every program the parser accepts (design.md §4.15, CLAUDE.md
    §9); the exit-2 guard is what stops the silent form, which is worse. The
    repair is either a place for a comment inside a printed parameter list
    (the printer keeps the author's line breaks when a comment sits between
    parameters) or a refusal at parse of a comment inside a signature, and
    the choice is a formatter-surface question for the sitting that owns
    `fmt`'s canonical form.

## The repair

The tree did not carry where a signature stops, so the printer could not tell
a comment inside the parentheses from the body's first: `FunctionDecl` gains
`signature_end`, set in `parse/tails.hero` after the result and its marks.
`print/signature.hero` is the one home of a signature's words for `fmt` and
`dump`. `fmt` prints a signature one parameter per line, each with its
comments above it and its trailing comment beside it, exactly when a comment
sits inside the parentheses on a line after the name's, and one line
otherwise, so every list without a comment keeps the canonical shape:
`fmt --in-place` over a copy of `selfhost/` changes nothing, and
`parse --dump-ast` is identical to the seed's over 568 files. A skeptic seat
attacked it at sixteen shapes beside the reproducer, every one a fixpoint with
the tree preserved. One placement is pre-existing and stays: a trailing
comment on the `(` line with nothing else inside the list trails the joined
signature, the bytes the seed writes.

## The measurements

| program | before (trunk `0a8fd346`) | after |
|---|---|---|
| `surface-fixtures/paramcomment095/following.hero` (a member follows) | `fmt` exit **2**, *changed the TREE* | 0, unchanged, a fixpoint |
| `surface-fixtures/paramcomment095/last.hero` (none follows) | exit 0, the comment MOVED after the group | 0, unchanged |
| `surface-fixtures/paramcomment095/heroes.hero` (a Heroes function) | exit 0, the comment moved INTO THE BODY | 0, unchanged |

Lane gate: the compiler's 702 tests; layout, canonical, surface 117, check
146, run 165, fixes 25, corpus 55, emission 546, annotations 186, records 24,
the net's own 167; seed regenerated and the fixpoint held. No slowdown on
`check selfhost/main.hero` (26.3 s against 27.0 s before, two rounds).
