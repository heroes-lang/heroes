# Panel 209, the ffi-pragmatist's brief

Read `00-shared.md` first, then this. Your folder is
`.claude/worktrees/scratch-b15/209-ffi-pragmatist/`, a detached worktree of
the trunk at `87794631`; run no git in it. Your report is
`docs/panel/209-reports/ffi-pragmatist.md`, written as you go.

## Your question

Under R1 a mutable cell is declared `name @= value`, its type inferred from
the value as an immutable's is, the annotation kept where the value cannot
say it. **Write and compile the binding C a real program needs under that
form, and say what the C boundary loses or gains.** Everything in this
language reaches C (design.md §1.11), so the cells that matter are the ones
holding what C hands back.

## What the tree holds, by command

- `grep -rhoE ': (ptr|cstr) @ ' examples` reads **6** cells typed `ptr` or
  `cstr` in `examples/`; `grep -rlE '^extern' examples` reads **21** files
  with an extern group. Spec § 13 (`spec/heroes-spec.md:410`) writes `x: cstr
  @ s.lease()` and `end_lease(@x)`; the sqlite example is the first fenced
  block after `## 13. FFI`, and `tests/harness/suite_special.hero` runs it.
- The `@` argument (`f(@x)`, spec § 9) is untouched by R1: 500 sites in
  `examples/`, 4 in the spec (`grep -rhoE '(\(|: |, )@[a-z_]'`, the critic's
  pattern, which counts `, @x` too). Under R1 `bump(@=n)` lexes `@=` whole:
  say what a binding's caller would be told.
- The marks a group carries, `lent`, `owned`, `acquires`, `consumes`,
  `transfers`, `retains`, `counted_by` (`record Param`,
  `selfhost/ast.hero:361`), are declaration-side and name no cell.
- A cell re-bound only through an `@` argument (`db: Db @ nullptr` then
  `open(@db)`, `tests/harness/suite_special.hero:419`) is the shape R1b must
  not refuse: count such cells in `examples/`' bindings.

## Measurements owed

1. **Three programs, built and run** in your copy with the trunk's grammar
   (R1 does not parse there: write each twice, today's form compiled, R1's
   form as the text you would submit, and say per line what the inferred type
   would be and whether it is the type the annotation states today): a handle
   acquired from C and freed (`db @= sqlite3_open(...)`'s shape, under the
   group's `acquires`); a lease (`x @= s.lease()`, then `end_lease(@x)`); a
   buffer a C call fills through an `@` argument (`buf @= [0u8] * n`'s shape,
   or whatever the spec allows). For each: does the inferred type equal
   today's annotation, and where it is a `T?`, a `u8` or a width C fixed, does
   the reader lose anything by not seeing it on the line?
2. **The widths**: a cell born from a literal takes `i64` unless the context
   asks otherwise (spec § 2, `:43` `b: u8 @ 255`). Under R1 `b @= 255` is an
   `i64` and a later `b @ some_u8` is a type error at the mutation. Count in
   `examples/` the `@` declarations of a width other than `i64` whose first
   value is a literal (`grep -rhE ': (u8|u16|u32|i8|i16|i32|f32|f64) @ [0-9"-]'`),
   and say where the error lands and what the fix says.
3. **The annotation that stays**: a cell born `fail(...)` or `[]` keeps its
   type under R1 (the shared brief's 1551 of 5926). Compile one binding that
   accumulates C results into such a cell and say whether the kept
   annotation reads worse beside `@=` than beside `@`.
4. **The group's grammar**: `@=` and `=@` as byte pairs inside an `extern`
   group's lines and inside a C header the compiler reads (`selfhost/emit/ffi*`,
   `header_reach.hero`): any place the lexer reads a group where `@` and `=`
   can meet.

Verdict per route, a falsifiable prediction with the instrument that scores
it, and the condition. Veto on ABI breakage or where a route makes a wrong
binding compile that today's form refuses.
