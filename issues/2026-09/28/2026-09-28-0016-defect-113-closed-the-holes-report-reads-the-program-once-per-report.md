# Defect 113 closed: the holes report reads the program once per report, not once per hole

2026-09-27, M-agreed-retention step 25, in lane C (`ac3dfaf6`), merged
`ab9f2758`, with defect 111. Found by lane 105's agent; repaired by the
coordinator in lane C after the lane's agent stopped at the account's
weekly limit.

- [x] **113 — the holes report reads every local and every top-level name of the program once per hole** | `check/holes.in_scope` and `nearby` walk all locals and all names for each `???`, so `check` on 200, 400 and 800 holes reads 0.13, 0.33 and 1.01 s user; what the report prints per hole is capped (design.md §4.16), what it reads is not | `selfhost/check/holes.hero:84,158` (`in_scope`, `nearby`) · **closed 2026-09-27**

    **Origin:** lane 105's agent, 2026-09-27 (0.05, 0.12, 0.31, 0.91 s at
    100 to 800 holes); re-run by the coordinator on the trunk's compiler at
    `2b1a1f24`, the numbers above at a load near 4.

    **Why it is a defect.** A report whose work grows with holes times names
    is the shape 105 closed for printed artifacts, left in a reader.

## The repair

`holes.gather` reads once per report what every hole read
(`holes.Gathered`): each declaration's locals, by index into the resolver's
locals and in their order; the functions a hole may be offered, grouped by
result type, each group in ascending (module, name) with its parameters
spelled once; and each declaration's first byte, so the declaration a hole
is in is found by halving, where the starts ascend, and by the old walk
where they do not. `in_scope` reads the owner's locals, `nearby` the group
of the expected type, taking the first five reachable as the walk did,
`owner_of` the halving. Every row of every report is the same, in the same
order.

## The measurements

`heroes check` on generated programs of 100, 200, 400 and 800 holes, `user`
seconds, the base compiler at `f39836a6` against the lane's: 0.05, 0.12,
0.31, 0.91 before, 0.04, 0.08, 0.15, 0.30 after, x2.0, x1.9, x2.0 per
doubling where it was x2.4, x2.6, x2.9. Identity: every file of the tree
with a hole and the four ladder inputs, 15 files, stdout, stderr and exit
the same; and the 1227 comparisons of defect 111's record, which include
the check goldens with holes.

## The gate

Defect 111's record.
