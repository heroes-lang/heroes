# 065 — M-buildable-structs: the structs a program can build, and the value C computes for a header's constant

## Goal

The milestone's page, opened at row 66 on 2026-10-14: a group record a program can build, `rest:
zero` and the sentences § 13 owes it (panel 194's R1, landed 2026-10-06, its walk through every
tool that re-prints a program finished on 2026-10-14), then what C reads of a record Heroes copies
(panel 215, ratified 2026-10-16: padding zeroed where C receives it, a record holding a lock
refused as a value). Its items were issues 1332 to 1337 and the sitting on a read-only lend
(2098). What it became on the night of 2026-10-16, under the author's goal as they left (*close
the two open milestones and the blocking defects*), is the question the defects kept asking from
the other side of the boundary: what value does C compute for a header's constant, and how does
the compiler know it is one C defines.

## What surprised

- **A value C leaves undefined is told only by running the code that computes it.** Every route of
  panel 216 that did not run the program's own unit was measured wrong in one direction or the
  other over about 42,000 bound real macros on three clangs; the one that runs it under four
  checks in trap mode, linked with the runtime alone, was right on all of them.
- **There are two evaluators, and a probe must ask each in its own way.** A macro's value is
  computed by the program at its own level, so `__builtin_constant_p` answers differently at
  `-O0` and `-O2` and the program really takes the undefined branch at `-O2`; an enumerator's or a
  const object's is computed by clang's constant evaluator, which answers as `-O0` does. The
  critic of panel 218 settled the seats' conflict by running one seat's own prototype at the other
  level.
- **The compiler's own new code had holes only an attack on the code as built could find.** The
  probe of panel 216 counted a probe that did not link as judged, expanded a self-referential
  macro twice and never asked a record; C2's text scan let a `;` inside a literal through; the walk
  missed clang's `array_filler` and an enumeration with no name. Each was found by a seat or a
  critic running the lane's own compiler, and none by a suite.
- **A record can be asked whether it is constant after all.** Panel 063 had written the question
  off because `__builtin_constant_p` answers 0 for every struct; C's own rule for an object of
  static storage asks it, and raylib's 26 `CLITERAL(Color)` macros, the feared cost, build
  unchanged.
- **One clang version can move a platform's verdict.** The Windows box's clang 23 adds `-fwrapv`
  for the MSVC target, where clang 21 and 22 add none, so a shift into the sign bit was defined
  there and nowhere else; the probe now asks the C standard's meaning everywhere (panel 218's R3,
  ratified).
- **Half the seed was restores of a line already anchored.** s4 joins them and the line table
  stays identical at every address: 51,514,006 bytes to 26,229,018.

## What broke and why

- **A record stored whole reached C with stack bytes in its padding** (2112), the x86-64 ABI
  returning a record with none; the store now writes the padding zero again.
- **clang 23 warned of a null pointer inside `__builtin_classify_type`** in every padding table
  (2119), the argument never evaluated; the table now asks through a pointer a declared and never
  defined function returns.
- **A clang excerpt is a drawing of the line, absent past 4,096 bytes** (2117), and four readers
  took a name from it; they now read the bytes of the line at the place a second clang run names.
- **A record constant could read differently twice, or hold an address into a dead frame** (2116,
  2121); its value is a static object now.
- **The coordinator's own repair stopped the unit's compile**: making the undefined-value warnings
  errors in a record's accessor hid ten of a case's eleven refusals until the eleventh was fixed,
  the shape of defect 2120; caught before landing by the case itself, and replaced by telling the
  warning beside clang's refusal.
- **`refusing` read every verdict as refusing**, the undefined canary trapping on every probe, so
  a warm build preprocessed each asked unit again; found by a unit test written for another
  purpose.
- **s4's joined prologue moved a row the `units` suite read**, found by the batch's full net, lane
  s4 not having run that suite.

## What landed, and what carried forward

**The milestone's items, closed on GitHub**: 1332 to 1337, the read-only lend's sitting (2098,
panel 217), measurement 046's five shapes (2100, `cbf724983`), panel 215 (2070, ratified) and its
landing (1334, 1335). **Panels 216 and 218 ratified by the author on 2026-10-17** (2110, 2115);
220, 221 and 222 provisional (2122, 2123, 2124), 217 provisional (2105). **The defects repaired in
this batch**: 2075 (`760a9035b`), 2104 (`760a9035b`), 2108 (`df8b66718`), 2109 (`ca26867d5`),
2112 (`d459e8050`), 2114 (`760a9035b`), 2116 and 2121 (`7fefaead8`), 2117 (`c6e154092`), 2119
(`bc978b09d`); 2111 and 2113 before it (`0e8f01fcb`, `613fdcbdb`), 2101 to 2106 in the batch
before. Filed and left by their class: 2120 (`adjacent`, clang 23's region on Windows tells two
shift shapes only after the unit's other refusals).

**Predictions scored at the landing**: panel 220's compiler-engineer, *C2 at 880 to 920 lines by
`layout`*: the four new modules read 726 in that unit and the growth of `value_told` and
`value_probe` 135 more, with `value_run`'s new share not separated from what it moved, so about
900, **held** by this count; *0 dumps on the compiler's own cold build* (panel 218), withdrawn by
the seat itself under reshape A, which predicts 4, unrun at the close. Panel 221's
compiler-engineer, *the repair at most +60/-30 lines in `selfhost/emit/`*: **missed, high**,
+128/-40 by `git diff --shortstat e92f33a2f 7fefaead8 -- selfhost/emit`, the coordinator's repair
of the ffi-pragmatist's condition (`emit/undefined_words.hero`, the warning read beside clang's
refusal) the overrun. Panels 216's, 218's and 221's predictions over the seats' populations (the
bound real macros, the 33,043 members, raylib's and the systems' initialisers on every CI leg)
rest on the seats' scripts and the CI's legs, **unrun at the close**, owed to the CI of
`f94a8cddb` and a run of the seats' scripts against the landed compiler.

**The gate, 2026-10-17, on `03b6cd829`**, the compiler clang built from the seed
regenerated over the batch, its fixpoint verified over two generations by `cmp` (`seed/heroes.c`
263,233 lines, 26,229,018 bytes): the compiler's own tests 1713 and the net's own 407; the whole
net 30 suites, 0 failed but `units`, repaired at `a02b616f2`: run 534, check 699, emission 1299,
warnings 613, annotations 1166, surface 425, fixes 999, unsupported 344, ir 29, emit 13,
permissive 22, full 39, records 26, spec 31, canonical 2, corpus 70, descriptors 630, determinism
574, grammar 9, lines 535, lldb 14, order 3, probe 27, runtime 8, special 10, unseen 3, wholes
630, cache 7, layout 6 (its module floor raised from 527 to 665), units 3 alone after the repair.
Linux arm64 read `emission` 1277 and 0 and its bless moved no file. The Windows box, on the lane
before the merge of lane-s4, read `run` 511 and 0 and C2's diagnostics identical to this Mac's.
Pushed at `f94a8cddb`; its CI's four legs were running at this close.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler 665 modules and
173,240 lines of Heroes, the seed 263,233 lines of C, 26,229,018 bytes, 56,166 `#line`s; the spec
7703 vendored and 10,108 real against 16384; the contract 8167 real, unchanged; sittings 220,
journals 66 numbered files, milestone pages 60, measurements 47, beats 133, questions in
`docs/learn/` 475 of which 450 open.

**Carried**: the author's readings of panels 217, 220, 221 and 222 (2105, 2122, 2123, 2124); the
census of `check` over the tracked `.hero` files, the batch's side, and panel 187's R2, owed
after the push by the optimistic chain; the formatter's probe M-inferred-cell's changes still owe;
M-bounded-seed, open, its part 2 (2087) to build under panel 222.

**The chain entry**: row 66, `m-buildable-structs`, closed 2026-10-17, this journal.
