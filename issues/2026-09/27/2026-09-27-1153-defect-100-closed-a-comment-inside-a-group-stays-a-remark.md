---
kind: defect
area: print
milestone: none
filed: 2026-09-25
commit: 29ed560108e1511efcf304e7432d30e911279818
github: none
---

# Defect 100 closed: a comment inside a group that is not a member's doc stays a remark when `fmt` runs

2026-09-27, M-agreed-retention step 17, in lane g (`832478b3` to `574711c3`),
merged `316d974f`. Found by the skeptic seat over defect 096's repair. The
rounds, the guard and the generators that judged it with 096, 099 and 101 are
`docs/records/done/2026-09-27-1154-defect-101-closed-fmt-moves-no-comment-and-refuses-output-that-would.md`.

- [x] **100 — inside an `extern` group, a comment at another column than the member below it, or directly under a second head of the same group, becomes that member's doc when `fmt` runs** | `fmt` re-indents a column-0 comment between two members to the members' indent, and merges two `extern` heads with the same header into one group while keeping a remark that sat directly under the second head, so in both the comment ends up at the member's column, directly above it, and the re-parse takes it as the member's doc: `check` 0, `fmt` exit 2 | `selfhost/print/fmt.hero` (the group walk, `continues`, `extern_head_once`) · **closed 2026-09-27**

    **Origin:** the skeptic seat over defect 096's repair, 2026-09-25, beside
    its reproducer; re-run by the coordinator the same day on the trunk's
    compiler at `5c3a6e39`, so older than that repair.

    **The reproducers.** A group `extern "x.h"` with `record Ob tag ob`, then
    a comment at column 0, then `    record Pool tag pool`: `fmt` 2. And two
    groups `extern "x.h"` one under the other, the second holding
    `    # remark under a second head` above `    record Pool tag pool`:
    `fmt` 2. `take_docs` in the parser takes a comment as a doc only at the
    member's own column and directly above it, which the source did not have
    and the output does.

    **Why it is a defect.** A correct program is refused by a tool that must
    be tree-preserving on every program the parser accepts (design.md §4.15).
    Beside it, the skeptic found one shape the repair of 096 in its first form
    broke: a doc above the second of two same-head groups, `fmt` 0 on the
    trunk, lost at the merge in the repair's first form; the repair of this
    defect and of 096 land together.

## The repair

- **A comment the parser did not take as a doc stays one that it will not
  take** (`page.comments_above`). Where `fmt` prints it at the member's
  column, it prints the blank line under it that keeps it a remark, so the
  re-parse reads the same tree.
- **A second head whose merge would move a comment is kept**
  (`page.keeps_head`), and a second head whose merge moves nothing is merged
  as before. The comment guard leaves a dropped head's lines out of the
  input's kept tokens, so a merge alone is never a move.
- **The doc above the second of two same-head groups**, which the first form
  of 096's repair lost, is kept with the member it documents:
  `groupremark096/samehead.hero` pins it and two shapes beside it.

A doc written directly above a second head moves into the merged group with
the head's member, which is where the re-parse finds it. The two independent
readers in defect 101's record flag that as a move, and it is one of the three
documented classes they report, never a refusal.

## The measurements

`fmt` on the item's two reproducers, the trunk's compiler at `f37b01b3`
against the lane merged with it, each built from its own seed, 2026-09-27:

| program | before | after |
|---|---|---|
| a column-0 comment between two members | `check` 0, `fmt` exit **2**, *changed the TREE* | exit 0, the comment printed at the members' indent with a blank line under it, a remark in the tree as before, a fixpoint |
| a remark directly under a second same-head head | `check` 0, `fmt` exit **2** | exit 0, the heads merged, the remark kept a remark by a blank line under it, a fixpoint |

Two fixtures pin it, `tests/golden/surface-fixtures/comments100/members.hero`
and `kepthead.hero`, and two more shapes are in `comments101/`
(`blankunderhead.hero`, `remarkdochead.hero`), each a `fmt` row of
`tests/harness/suite_surface.hero`.
