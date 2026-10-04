---
kind: defect
area: print
milestone: none
filed: 2026-09-25
commit: 29ed560108e1511efcf304e7432d30e911279818
github: none
---

# Defect 099 closed: a comment after a block's last item stays in that block

2026-09-27, M-agreed-retention step 17, in lane g (`832478b3` to `574711c3`),
merged `316d974f`. Found by the parser seat that repaired defect 096. The
rounds, the guard and the generators that judged it with 096, 100 and 101 are
`docs/records/done/2026-09-27-1154-defect-101-closed-fmt-moves-no-comment-and-refuses-output-that-would.md`.

- [x] **099 — a comment after the last item of a block leaves the block when `fmt` runs** | a `#` remark written at the block's indent after the last member of a group, the last statement of a body or the last field of a record is printed by the NEXT declaration's walk, at column 0, with a blank line added under it: exit 0, the tree the same, and the comment now sits at file level above the next function | `selfhost/print/fmt.hero` (`format_file`'s walk, `comments_before` at the next item's indent) · **closed 2026-09-27**

    **Origin:** the parser seat that repaired defect 096, 2026-09-25, measuring
    the shapes beside its reproducer; reproduced by the coordinator the same
    day on the trunk's compiler at `0029567d`, so it is older than that repair.
    Searched `docs/work/`, `docs/records/done/` and `docs/learn/` for "last
    statement", "end of the body", "after the last member", "after the last
    field", "trailing remark": not filed.

    **The reproducer**, three shapes, each `fmt` exit 0 and each output a
    fixpoint:

        function f() -> i64
            x = 1
            return x
            # a remark after the last statement of the body

        function main()
            print(f())

    comes back with the remark at column 0 under `return x`'s block, above
    `function main()`, a blank line under it; the same for a remark after the
    last member of an `extern` group and after the last field of a record.

    **Why it is a defect.** `fmt` moves a comment out of the block the author
    wrote it in, silently: the tree does not change, so the self-check that
    stops defect 095's and 096's shapes cannot see it, and a remark about the
    end of a body now reads as a remark about the function below it. Defect
    095's record calls the silent move the worse form of the two. The repair
    prints a comment indented inside a block, after the block's last item and
    before the dedent, at the block's own indent.

    **And without the blank line it is a refusal, measured 2026-09-25** by the
    skeptic seat over defect 096's repair and re-run by the coordinator on the
    trunk's compiler: the same remark written directly above the next
    declaration, no blank line between (a body's last statement then
    `function main()`, a group's last member, a record's last field inside a
    group), is `check` 0 and `fmt` **2**, *changed the TREE*, because the
    remark printed at the next declaration's column becomes its doc. One root,
    two outcomes: with the blank line the move is silent, without it the
    self-check catches it.

## The repair

It took three rounds, because the first placed the comment by the walk that
happened to print it and the skeptic seat over it found the class still open:
a comment after a block whose last statement ends with the author's `)` alone
on its line still left the block.

- **One rule gives every comment on a line of its own its block**
  (`print/owners.hero`): of the blocks open between the logical line before it
  and the logical line after it, the deepest whose indent is at most the
  comment's column, and never shallower than the next line's. A line inside a
  bracket counts as part of the logical line it continues, so a `)` alone on
  its line ends the statement it closes and not a block.
- **The printer places by that rule** (`page.block_end`), and so does the
  output guard, which is defect 101's record: a printer that moves a comment
  to another block is refused at exit 2 rather than written.
- **A run of comments is placed wholly in its last comment's block**, because
  the rule reads a run from its last line up. An ascending or mixed run
  therefore moves as one; the reason is written in `print/owners.hero` and
  `print/page.hero`, `comments101/ascending.hero` pins it, and panel 179 ruled
  it the canonical form (provisional).

## The measurements

`fmt` on the item's reproducer and on the shape without the blank line, the
trunk's compiler at `f37b01b3` against the lane merged with it, each built
from its own seed, 2026-09-27:

| program | before | after |
|---|---|---|
| the reproducer above, a blank line before `function main()` | exit 0, the remark MOVED to column 0 above `function main()` | exit 0, unchanged, a fixpoint |
| the same with no blank line | `check` 0, `fmt` exit **2**, *changed the TREE* | exit 0, the remark kept inside the body, a blank line printed under it as between any two declarations, a fixpoint |

Two fixtures pin it, `tests/golden/surface-fixtures/comments099/inside.hero`
and `tight.hero`, and the shapes the later rounds found are in
`comments101/`: `closer.hero`, this defect's own class after a `)` alone on
its line, `runs.hero` (the seat's `s8`) and `ascending.hero` (`d02`), each a
`fmt` row of `tests/harness/suite_surface.hero`.
