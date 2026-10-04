---
kind: defect
area: print
milestone: none
filed: 2026-09-25
commit: 29ed560108e1511efcf304e7432d30e911279818
github: none
---

# Defect 096 closed: a remark inside a group keeps the blank line under it, and `fmt` formats the file

2026-09-27, M-agreed-retention step 17, in lane g (`832478b3` to `574711c3`),
merged `316d974f`. Found by the skeptic seat over defect 095's repair. Defects
096, 099, 100 and 101 were repaired in one lane because they are one printer's
walk over one comment stream; the rounds, the guard and the generators that
judged all four are
`docs/records/done/2026-09-27-1154-defect-101-closed-fmt-moves-no-comment-and-refuses-output-that-would.md`.

- [x] **096 — inside an `extern` group, a remark followed by a blank line before the next member makes `fmt` refuse the file** | the group printer never emits the blank line for a continuing member, so the remark becomes the next member's doc, the self-check sees a different tree and `fmt` exits 2 on a program `check` accepts | `selfhost/print/fmt.hero` (the group's member walk, the `continues` branch) · **closed 2026-09-27**

    **Origin:** the skeptic seat over defect 095's repair, 2026-09-25, attacking
    the repair at the shapes beside it; measured on the seed compiler of
    `0a8fd346` and on the repaired one alike, so it is older than both.

    **The reproducer.** A group whose first member is followed by a remark, a
    blank line, and `record Ob tag ob`:

        extern "x.h"
            function ob_put(o: Ob consumes)
            # a remark about what follows

            record Ob tag ob

    `check` exit 0; `fmt` exit 2, *`fmt` changed the TREE … a DIFFERENT
    PROGRAM*, and the file is not touched. With no line-broken signature
    anywhere, so it is not 095's shape: the blank line is what is lost.

    **Why it is a defect.** A correct program is refused by a tool that must
    be idempotent on every program the parser accepts (design.md §4.15); the
    exit-2 guard stops the silent form, which would attach the remark to the
    record as its doc. The repair keeps the blank line between a remark and
    the member it does not document, inside a group as outside one.

## The repair

The comment stream moved out of `print/fmt.hero` into `print/page.hero`, and
the blank line is its decision: `page.gap` prints a blank line where the
source has one, inside a group as outside it. When two `extern` heads with
the same header merge into one group, the blank line under the dropped second
head belongs to that head, so it is never printed between a doc and the
member it documents. A remark, a doc and such a head, together, merge with the
remark still a remark. The first form of this repair lost a doc above the
second of two same-head groups, `fmt` 0 on the trunk; `groupremark096/samehead.hero`
pins that shape, which defect 100's record names.

## The measurements

`fmt` on the item's reproducer, the trunk's compiler at `f37b01b3` against the
lane merged with it, each built from its own seed, 2026-09-27:

| program | before | after |
|---|---|---|
| the reproducer above | `check` 0, `fmt` exit **2**, *changed the TREE* | `fmt` 0, unchanged, a fixpoint |

Four fixtures pin it, `tests/golden/surface-fixtures/groupremark096/`:
`beside`, `filelevel`, `middle` and `samehead`, each a `fmt` row of
`tests/harness/suite_surface.hero`.
