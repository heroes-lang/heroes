# Panel 178 — compiler-engineer

Read `00-shared.md` first. Your directory is `<scratchpad>/178-compiler-engineer/`,
a `git archive` of HEAD `57679005`; build the compiler there from the seed. You
judge the ceiling (design.md §1.1, §1.7, Part 5) and cost, with veto on
soundness. Write your report to `REPORT.md` in your directory.

## Pointers, read at HEAD while this brief was written

- `selfhost/check/walk.hero:609` `checked_array_literal`, where `:616` raises
  `fixed_array_length` when a literal's count differs from the fixed length.
- `selfhost/data_errors.hero:56` `missing_fields`, the construction's
  every-field rule.
- `selfhost/parse/type.hero:258` `fixed_length`, which reads an integer literal
  as an array length (panel 163's critic: the piece `[x; N]` would reuse).
- `selfhost/emit/storageless.hero`, 148 lines: a `T[N]` has no storage of its
  own in C, so it is rendered where it is USED, and that works as a subscript
  base and as a construction argument (`tests/golden/unsupported/fixed-array-flow.hero`
  lines 23-27 say so, and its four rows are the positions refused).
- `selfhost/emit/gate.hero`, 461 lines by `wc -l`, the guard those rows come
  from.
- `selfhost/emit/inst.hero:133` and `selfhost/emit/container.hero:142`
  `write_element` / `:234`: **defect 091**, the element store of a fixed array
  that is `check` 0 and `hero_unreachable()` at run time (`elem_min.hero` here).
- `selfhost/emit/body.hero:146` and `:194`: a local gets `= {0}` only when it is
  reference-counted; a C record local is declared bare and assigned from a
  compound literal.
- `selfhost/emit/extern_record.hero`, 274 lines: `partial` and the header probe.
- `selfhost/emit/assert_spelling.hero:170` `zero_of`: **not a zero mechanism**.
  `.claude/skills/panel/SKILL.md` lines 91-92: *a C11 type probe inside an
  expression that is never evaluated*. Do not build on it unless you read it
  and find otherwise.
- Layout ceilings, in `tests/harness/suite_layout.hero`'s own unit (not
  `wc -l`): `check/walk.hero` **1870** (`:422`), `emit/gate.hero` **365**
  (`:433`), `emit/inst.hero` **350** (`:436`).

## What to measure

1. **Price R1, R0, R2, A and T** in `selfhost/` lines by `git diff --stat` in
   your copy, and prototype at least R1 and the repair of 091 far enough that
   the numbers are run: a group record built with `rest: zero` emits C that
   zeroes fields AND padding (say how: `= {0}` then stores, `memset`, or a
   compound literal, and whether the choice survives `-O2`); a Heroes record
   refuses it with a diagnostic.
2. **Defect 091's repair as a lowering**: `s.name[i] @ v` into a fixed field,
   index checked like the read at `inst`'s load. Run `elem_min.hero` and
   `sunpath_*_ascii.hero` under it on Darwin and both Linux legs.
3. **T without a call typed by context**: panel 163's veto on a call typed by
   context rests on *C cannot return an array*. Say whether `s.to_fixed()`
   rendered as a construction argument (where `storageless` already works)
   escapes that veto, or needs the same compile-time erasure, and build it if
   it can be built.
4. **Which of the 510 `.hero` files under `tests/golden` and `examples` move**
   (`find tests/golden examples -name '*.hero' | wc -l` read 510 at panel 177;
   count it again) under each prototype, and the `run` and `corpus` suites
   through the net's own binary naming your compiler.
5. **`check` time** of `selfhost/main.hero` before and after, `/usr/bin/time -p`,
   the machine still while the clock runs.

Say in your report which of these you did not do, and why.
