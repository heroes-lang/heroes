# Defect 126 closed: a match arm split after its sign is refused once, with fixes that parse

2026-09-28, M-agreed-retention step 30, in lane 123 (`d5e088d9`, `492a7ee4`,
`1e2c3506`, `17e3c5a8`, `5c0a9aac` and `c6d62c7e`, the trunk merged into it at
`6a84ce36`), merged `eed23b19`. The six defects were found at 181's landing and
beside each other's repairs; the two found after the author's waiver of
16:40, 129 and 130, are carried and not repaired here.

- [x] **126 — panel 181's join is certain on an arm whose `-` is split from its operand by a line end or a comment, and writes a program that does not parse** | `match n` with `0 => 5` over a line holding `-` alone over `1 => 10` costs three diagnostics, and `check --apply` writes `0 => 5 - 1 => 10`, refused with `expected_end_of_line`; with `-  # the sign` the join keeps the comment inside the line; on the first arm the certain join writes `- 1 => 10`, which defect 123's repair refuses; and the parenthesised `guess` would wrap a pattern's value, and a pattern takes no parentheses | `selfhost/open_line.hero` (`goes_on_at_head`, `refuse_the_end`) · `selfhost/parse/wrap_break.hero` (`spanning`) · **closed 2026-09-28**

    **Origin:** lane 123's agent, 2026-09-28, attacking the separators beside
    defect 123 (a line end or a comment where 123 has a space; reproducers
    `shapes/s07` to `s10` and `applied/` in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `3bde187f` the same morning
    (`arm_split.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect126/`).

    **Why it is a defect.** Panel 181's item 3: a `certain` fix that fails to
    compile is a defect of the landing, and `heroes check --apply` would write
    it into a file.

## The repair

`open_line.goes_on_at_head` let an arm's `-` through as its pattern's sign
only when the arm's `=>` stood on the `-`'s own line, so a `-` that a line end
set apart from its literal was read as a subtraction going on with the arm
above, joined to it, and then refused again at its own line end.
`next_line.arrow_below` finds the arm's `=>` on the next line with words, past
blank and comment lines, and the break costs one diagnostic, the line that
cannot end with `-`; `wrap_break.in_a_pattern` offers no parentheses around
an expression inside a pattern. The join writes `-1` and is `certain` where
the literal's line does not stand alone, panel 181 item 3's rule unchanged;
with a comment after the `-` no fix is offered, since a join would carry the
comment into the arm. The statement shapes beside it are defect 129, carried,
and print as on the trunk (`1e2c3506`).

## The gate

Each commit in the lane was gated, one suite at a time, by the map's suites for
its change, the compiler built from the regenerated seed and the fixpoint by
`cmp`, with the compiler's own tests and the net's own; each commit's body
lists them. A `check` census over the tree's `.hero` files, the trunk's
compiler against each commit's, moved one file in the six commits: 127's own
new `run` golden, 1 to 0. The merge of the trunk at `0338b598` (`6a84ce36`) had
one conflict, `seed/heroes.c`, regenerated and not resolved by hand, and moved
one blessed emission, 127's new file, by 36 lines each way, every one a
` = {0}` dropped on a temporary, 23 `HeroStr` and 13 `HeroArrayHeader *`: panel
182's rule arriving with the trunk, the file's 12 named slots keeping theirs
(counted by the coordinator on the merge). After it: the fixpoint by `cmp`, the
compiler's 828 tests, the net's own 179, and the whole net, 3,144 passed and 0
failed.

Linux arm64 on the merged tree (`6a84ce36`, the `heroes-linux-arm64` image):
the compiler's 828 tests and surface 334, canonical 2, annotations 216, check 167, fixes 32, layout 4, order 3, lines 208, run 207, emission 626, determinism 237, wholes 303, descriptors 303, cache 6, grammar 9, spec 20, probe 24, corpus 53 and warnings 266, each 0 failed. The Windows box on the same
tree, one archive whose sha256 matched on both sides (`190dffa99b896aac`), seed
`0bef1dfe91c51dcd`: the compiler's 828 tests and surface 327, canonical 2, annotations 216, check 167, fixes 32, layout 4, order 3, lines 206, run 205, emission 604, determinism 235, wholes 303, descriptors 303, cache 6, grammar 9, spec 20, probe 24, corpus 50 and warnings 261, each 0 failed.
