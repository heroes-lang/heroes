# Defect 111 closed: the checker reads each declaration once, and the compiler checks itself six times faster

2026-09-27, M-agreed-retention step 25, in lane C (`ac3dfaf6`), merged
`ab9f2758`. Found by lane 105's agent on its ladders; repaired by lane C's
agent up to the freer table and the rows, and finished by the coordinator
when the agent stopped at the account's weekly limit.

- [x] **111 — `heroes check` is quadratic in a program's calls and declarations, and one scan is most of the compiler checking itself** | `check/freer.marked_as_freer` reads every declaration and every parameter of the program at every call to a user function, asking whether one names it as a freer, and `resolved.declare_top` copies a module's whole map of names at every declaration: `check` on generated programs of 250, 500 and 1000 units reads 0.56, 1.45 and 4.31 s user, and on `selfhost/main.hero` the scan dominates the profile | `selfhost/check/freer.hero:40` (`marked_as_freer`) · `selfhost/resolved.hero:302` (`declare_top`) · **closed 2026-09-27**

    **Origin:** lane 105's agent, 2026-09-27, on its ladders (0.53, 1.35,
    3.92, 12.73 s at 250 to 2000 units, and 13,715 of 15,411 samples of
    `check selfhost/main.hero` in the scan); re-run by the coordinator on the
    trunk's compiler at `2b1a1f24`, the ladder above at a load near 4 and a
    `sample` of `check selfhost/main.hero` with the scan on the stack in most
    of its 10,208 samples.

    **Why it is a defect.** Defect 103's reasoning: a pass read per item where
    it is read once, in the compiler's own code, and the cost every `check`
    and every build pays grows with the square of the program.

## The repair

- **The freer marks are a table built once** (`check/freer.marks`), in one
  pass over the declarations before any body is checked, kept on
  `state.Checked.freers`; the checker's call walk and the three extern probes
  that asked `marked_as_freer` of every declaration ask the table. The first
  mark of a name is the one kept, the result's before a parameter's, the
  answer the walk gave; a test pins that order.
- **The name tables are rows** (`resolved.NameRows`, `UseRows`): an outer
  map from a module or a file to a row index, and an array of rows. A map of
  maps reached a new name only through a read that shared the inner map, so
  every declaration copied its module's whole table of names; a row is
  stored into in place, and a row is made by `name_row` or `use_row` alone.
  Every reader goes through `names_of` and `bindings_of`; the two readers
  outside `resolved.hero`, the holes report and the scopes dump, were found
  by the type checker when the field's type changed.

## The measurements

`user` seconds, the lane's base compiler at `f39836a6` against the lane's,
alternated, the machine at a load near 2:

| program | before | after |
|---|---|---|
| a generated program of 250 units | 0.52 | 0.38 |
| 500 units | 1.36 | 0.77 |
| 1000 units | 3.96 | 1.63 |
| 2000 units | 12.88 | 3.61 |
| `heroes check selfhost/main.hero`, twice | 26.23, 26.24 | 4.07, 4.07 |

Per doubling the ladder grew x2.6, x2.9 and x3.3 before and x2.0, x2.1 and
x2.2 after; `real` was within 0.06 s of `user` in every run. A `sample` of
the new `check selfhost/main.hero` spreads over lex, parse, resolve and
check with no function holding a tenth of the samples a one-pass structure
would remove; what is left and not repaired, a constant rather than a
growth, is `inventory.table()` building the 39 built-ins' descriptors at
every call to read one name, 12% of the samples with its copies and drops.

**Identity**: `check`, `check --json` and `check --dump-scopes`, the base
compiler against the lane's, over the 153 check goldens, the 206 run
goldens and the examples' 50 programs: 409 files by three forms, 1227 of
1227 the same bytes on stdout and stderr and the same exit.

## The gate

In the lane: the seed regenerated, fixpoint by `cmp` (`c57ec383d3a48825`);
the compiler's 762 tests, the net's own 171. On the trunk after the merge:
the seed emitted again by the trunk's compiler, fixpoint (`8541391522618b4d`),
the compiler's 786 tests, the net's own 172; the full net, with every lane merged and the split below, **2754 passed and 0 failed** (`d5d78666`, 1271.47 s real, 746.20 user). Lane C's repair had taken three files past design.md §11's ~300 lines, and the combined trunk's `layout` refused it on Linux arm64 and the Windows box (the coordinator had not run `layout` on the lane): split along the repair's own seams in `97247b0f`, `name_rows.hero`, `check/freer_marks.hero` and `check/holes_gather.hero`, the three at 290, 280 and 234 code lines; and `order` asked an ORDER mark on `resolved.modules_declaring` and the walks floor moved 22 to 21, the repair having replaced two walks by one. Linux arm64 on that trunk: the compiler's 787 tests and surface 315, canonical 2, annotations 199, check 153, fixes 26, layout 4, order 3, lines 207, run 206, grammar 9, spec 20, each 0 failed; the Windows box: the compiler's 787 tests and surface 308, canonical 2, annotations 199,
check 153, fixes 26, layout 4, order 3, lines 205, run 204, grammar 9 and spec
20, each 0 failed.
