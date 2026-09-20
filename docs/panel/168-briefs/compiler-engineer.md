# Panel 168 — brief for the compiler-engineer

Read `docs/panel/168-briefs/00-shared.md` first. It carries the proposal, the
runtime facts with their line numbers, and three measurements you should re-run
rather than trust.

## Your axis

You judge the **ceiling and the mechanism**: design.md §1.1, §1.7, Part 5. What
does route A cost in compiler lines under each layout, and does the trailing
header need machinery the compiler does not have?

## Pointers into the live compiler

`selfhost/` is Heroes and it is what ships. `runtime/` is the C. Never
`archive/bootstrap-rs/`.

- `runtime/parts/str.c:419-470` — `hero_str_held` and `hero_held_release`, the
  two functions the proposal is about. Read the comment at `:430-450` before
  the code: it states why the release takes the CELL and not the pointer, and
  that reasoning is an input to your verdict.
- `runtime/parts/alloc.c:349-358` — `hero_alloc_held` and `hero_release_held`,
  which keep `hero_live_held`. Ask what happens to that counter when C frees the
  block instead of the program, under each layout.
- `selfhost/check/leasing.hero` — 259 lines by `wc -l`. `is_lease_cell` at
  `:62-67` is the walk panel 167's note says the emitter could copy. Judge
  whether it can: it reads `r.locals[local].value`, the resolver's record of a
  declaration's initialiser, which is an AST-level fact.
- `selfhost/emit/builtins.hero:138-143` — where `lease` becomes `hero_str_held`
  and `end_lease` becomes `hero_held_release`. One line each.
- `selfhost/check/lending.hero` 386, `check/lend_extent.hero` 304,
  `check/lend_types.hero` 138, `check/lend_decls.hero` 130,
  `emit/ffi_lend.hero` 169 — all by `wc -l` today.

## The ratchets, which are yours to re-measure

Panel 167's verdict rested on three files sitting one or two lines under their
`tests/harness/suite_layout.hero` ceiling. **Do not carry those numbers from
that sitting** — it is a day old and route C landed in between. Re-measure with
the instrument that judges them, which is `suite_layout`'s own unit and not
`wc -l`; `.claude/rules/module-shape.md` records that 71 of 189 files pass 300
by `wc -l` while the suite is green.

Then answer: does the proposal touch any ratcheted file, and does the trailing
header's alternative — a pointer-keyed side table in the runtime, or a new IR
operand — touch one?

## The questions, in the order they decide the verdict

1. **Re-run measurement 1.** Build a lease program, `--dump-ir` and `--emit-c`
   it, and say in your own words what the release instruction carries. Then put
   a loop between the lease and the release and say which basic blocks they land
   in.
2. **Is there a route nobody listed?** Panel 167 recorded two candidates for
   making a trailing header findable and both answer a question that turns out
   not to be the one that fails. One the coordinator thought of and did not
   pursue: a hidden sibling local emitted beside the cell, `h1_c__len`, written
   at the lease and read at the release, which needs no flow analysis because
   both sites name the same slot — but needs *which slots are lease cells* to
   reach the emitter. Price it, and say whether it is worth having for any
   reason **other** than the give-away case, which measurement 2 shows it does
   not fix.
3. **Is route A's field lease sound on the leading header?** A field lease
   copies a fixed-array field's bytes out of a record into a block the program
   owns. Walk the four things that could need the allocation base and say
   whether any of them does.
4. **What does the proposal cost in compiler lines**, against the trailing
   header plus whichever candidate you judge least bad?
5. **Register a falsifiable prediction** with an instrument that exists today.

## Working rules

- `cp -r` the tree to your scratchpad, then `rm -rf target build` in the copy.
- A compiler in 3.80 s:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Rebuilding from
  `selfhost/` is roughly twenty minutes and will kill you on the watchdog.
- Write your report to `docs/panel/168-reports/compiler-engineer.md`.
- You have a veto on soundness. A veto is a refusal, not a price.
