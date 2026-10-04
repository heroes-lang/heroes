# Defect 112 closed: the emitter and the lowering read each function once, and emission is linear in the size of a function

2026-09-28, M-agreed-retention step 26, in lane D (`c3dc424b`), merged
`dfcac362`. Found by lane 105's agent on its ladders; repaired by the
coordinator, the agents being at the account's weekly limit.

- [x] **112 — emission is quadratic in the size of a function** | `emit/unread.assigned` scans every instruction of a function for each temporary it declares, so `build --emit-c` less `check` reads about 3.5, 7.7 and 19.7 s at 250, 500 and 1000 units of a generated program whose `main` makes one call per unit; the lowering's `ir/flatten.call` and `ir/owned_release.library_validated` lead the rest | `selfhost/emit/unread.hero:84` (`assigned`) · **closed 2026-09-28**

    **Origin:** lane 105's agent, 2026-09-27 (2,196 of 6,633 samples of
    `build --emit-c` at 1000 units in `assigned`); the ladder re-run by the
    coordinator on the trunk's compiler at `2b1a1f24`, `build --emit-c` 4.01,
    9.12 and 24.02 s user at 250, 500 and 1000 units against `check` 0.56,
    1.45 and 4.31, at a load near 4.

    **Why it is a defect.** The same shape as 111 in the back end: one pass
    per temporary where one pass per function answers every temporary.

## The repair

The profile found four causes, and the entry named two of them.

- **The unread-value analysis reads a function once**
  (`selfhost/emit/unread.hero`). `assigned`, `discarded` and `slot_is_read`
  each walked every live instruction for every temporary or slot the
  prologue declares, and `slot_is_read` called `discarded` at every load.
  `unread.defs` finds each temporary's first live writer in one pass,
  `unread.slots_read` answers every slot in one pass, and
  `emit/body.prologue` reads both once per function.
- **The library's `validated` is found once per program**
  (`selfhost/ir/owned_release.hero`, `selfhost/ir/lower.hero`). The release
  lowering asked for it at every call by walking every declaration;
  `build.Lowering.validated` holds it, set once by `ir/lower.lower`.
- **The builder appends in place** (`selfhost/ir/build.hero`). A push
  through a field place copies (design.md Part 8, wart 8), so a function's
  values, the program's functions and its strings are lent to helpers that
  take the list by `@`; and `push_inst`, whose place has an index in it,
  hoists the block's list, empties the field, pushes on the local and
  stores it back, because a lend of an indexed place copies the list into a
  temporary the callee shares, read in the emitted C.
- **A string is interned by a map** (`selfhost/ir/interning.hero`). `intern`
  looked a literal up by walking every string interned so far, on the
  premise that a file has few distinct literals, which the compiler's own
  source disproves; a map from text to id answers it, the ids still given
  in order of first appearance, which a test pins.

`build.hero` stays under design.md §11's 300 lines by the repair's own
seams: the program-wide interning is `ir/interning.hero`, and a
terminator's successors are `ir/flow.hero`, which the builder and the
verifier both read.

## The measurements

`user` seconds of `build --emit-c` on the generated programs lane 105 made
(one record, one variant and one function per unit, and a `main` that makes
one call per unit), the trunk's compiler before the merge (`f159c023`)
against the one after (`dfcac362`), each built from its seed with the plain
line of CLAUDE.md § Commands, alternated, two rounds, on 2026-09-28 at 01:18
with nothing else running on the machine (load 1.7 to 2.1, the Docker VM
idle; `real` within 0.25 s of `user` plus `sys` in every run, every exit 0):

| units | before, rounds 1 and 2 | after, rounds 1 and 2 |
|---|---|---|
| 250 | 2.54, 2.56 | 1.97, 1.98 |
| 500 | 6.91, 5.98 | 3.94, 3.93 |
| 1000 | 17.85, 15.90 | 8.33, 8.30 |
| 2000 | 52.47, 48.22 | 19.07, 19.06 |

Per doubling the time grew x2.72, x2.58, x2.94 and x2.34, x2.66, x3.03
before, and x2.00, x2.11, x2.29 and x1.98, x2.11, x2.30 after; at 2000 units
the emission is 2.6 times faster. What the top doubling still carries above
x2 is not one function: a `sample` of the new compiler's 2000-unit emission,
taken for this record, has about 10,400 samples and none of the compiler's
own functions near a tenth of them; the runtime's copies and zeroing lead
(`memmove` 1,147, `memset` 1,079), spread over their callers, the largest
share of the zeroing (174) under `emit/inst.emit`, whose temporaries are
zeroed at every call: defect 114's shape, in the emitter itself. The lane's own
ladder, taken with other work running at a load between 2 and 4 and so
superseded by this one, read 3.60, 7.82, 19.69, 56.35 against 2.45, 4.93,
10.06, 22.22.

**Identity.** `build --emit-c` over every run, emit and ir golden and every
example, 294 files, the trunk's compiler against the lane's: stdout, stderr
and exit the same on all, and the compiler's own emission of one source the
same from both.
The four emissions of the ladder above, 2.9 to 23.9 MB of C, are the same
bytes from the two compilers (`cmp`).

## The gate

In the lane, the lane alone on the machine except where said: the seed
regenerated, fixpoint by `cmp` (`783f314336dc944d`); the compiler's 788
tests, the net's own 172; layout 4, order 3, records 24, canonical 2, check
153, emission 636, determinism 240, corpus 55, warnings 271, ir 24, emit 8,
each 0 failed; run 210 and 0 when run alone, after a first reading of 126
and 84 while an earlier gate of the same lane deleted `build/` under it,
which is a reading of the harness and not of the compiler and is recorded
so that nobody finds it later and wonders. On the trunk after the merge:
the compiler's 788 tests, the net's own 172, records 24, each 0 failed.
Linux arm64 on the merged trunk (`dfcac362`, the tree copied into the
`heroes-linux-arm64` image): the compiler's 788 tests and surface 315,
canonical 2, annotations 199, check 153, fixes 26, layout 4, order 3, lines
207, run 206, emission 624, grammar 9, spec 20, each 0 failed. The Windows
box on the same tree, sent as one archive whose sha256 matched on both sides
(`059241237779bb81`) and whose seed read `783f314336dc944d` there: the
compiler's 788 tests and surface 308, canonical 2, annotations 199, check
153, fixes 26, layout 4, order 3, lines 205, run 204, emission 602, grammar
9, spec 20, each 0 failed.
