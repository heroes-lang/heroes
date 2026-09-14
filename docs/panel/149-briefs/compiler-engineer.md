# Panel 149 — brief for the compiler-engineer

Read `docs/panel/149-briefs/00-shared.md` first. It carries every measurement
already taken; do not repeat one, extend it.

## Your seat

design.md §1.1, §1.7, Part 5. You judge implementation cost and core-versus-sugar,
and you have a veto on soundness.

## What to read

- `selfhost/check/acquiring.hero` — 145 lines, the whole rule. `handle_behind`
  at the top is what does not look inside a record.
- `selfhost/check/ffi.hero` — `crosses_the_boundary:76`, `ffi_field:188`,
  `fixed_element_fine:225`, `header_owned:232`. These bound the shape space.
- `selfhost/check/table.hero` — how a type id indexes the interned table, and
  `poisoned`'s termination argument around lines 278-285.
- `selfhost/handles.hero` — `is_handle`, and the one-tag-one-type identity that
  panel 148 keyed the rule on.
- `selfhost/checker.hero:40-90` — the pass order.
- `selfhost/check/sized.hero` — what `order_and_cycles` computes, and whether the
  order it produces could be reused to make a reachability walk terminate
  without a visited set of its own.
- `runtime/parts/alloc.c:99-210, 261-268` — the counter and its exit message.
- wherever `acquires` becomes an emitted `hero_handle_acquired()` call. Find it
  and cite the file and line; the shared brief measured that a record result
  with a mark produces a correct count, and you should say from the code WHY,
  and whether it produces exactly ONE increment regardless of how many handles
  the type reaches.

## The questions that are yours

1. **Price R1 in lines**, against the live compiler. A `handles_reachable(ty)`
   walking group-record fields and fixed-array elements with a visited set —
   how many lines, and where does it belong? `handles.hero` is where a question
   about a handle is asked once, by its own module doc; `acquiring.hero` is where
   this rule lives; `check/ffi.hero` already owns the reachability vocabulary.
   Name the home and say why the other two are wrong.
2. **The termination argument, stated so a later reader can check it.** The walk
   is over DECLARATIONS, not over the interned type table, and the shared brief
   measured that `acquiring` runs on a program `sized` already refused as
   cyclic. Is a visited set sufficient, or is there a second cycle shape it
   misses? Write the adversarial program and run it.
3. **R3, and this is the one I expect to be hardest.** The counter is one global
   integer. If a returned type reaches N handles, a correct count needs N
   increments. N is a compile-time constant from the type. So: is *refuse a
   returned type reaching more than one handle type* a narrowing that buys
   soundness, or a premise about the world that expires when somebody binds
   `getaddrinfo`? Price the alternative — the mark repeating, once per reachable
   handle type — in parser, AST, checker and emitter lines.
4. **A fixed array of four handles, and whether the count can ever be right.**
   `Four` holds `a: Slot[4]`. C fills all four in the shared brief's header.
   A C function that fills two and leaves two null would make four increments
   wrong. Does that make the fixed-array route a refusal rather than a count?
   Measure what the emitter would have to know.
5. **What breaks.** Run the rule's own tests and the compiler's own tests before
   and after your prototype: `./heroes test selfhost/main.hero`. Say which of
   the three shipped bindings under `examples/` would newly need a mark — read
   them, do not guess.

## Predict something falsifiable

One prediction, with the milestone at which it becomes checkable and the command
that would settle it.
