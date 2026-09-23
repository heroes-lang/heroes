# Panel 175 — compiler-engineer

Read `00-shared.md` first. Your seat judges implementation cost and
core-versus-sugar (design.md §1.1, §1.7, Part 5), and you hold a veto on
soundness. Work in `<scratchpad>/175-compiler-engineer/`, built from the seed;
never `archive/bootstrap-rs/`.

## The pointers

- `selfhost/check/acquiring.hero`, 278 lines by `wc -l`: `bindings_say_which`
  `:146`, `releaser_reads` `:194`, `unread_releaser` `:269`. Its header says
  the checker has no flow analysis.
- `selfhost/check/decls.hero:334`, `one_tag_one_type`, program-wide, and its
  two narrowing paragraphs above it (defect 029's ground, defect 072's
  `tag void`).
- `selfhost/emit/handle_traffic.hero`, 143 lines: `for_call` `:36` (after the
  call; `fd.value.acquires_result` at `:39` is the releaser's span, and the
  `@out` arm at `:64`), `before_call` `:101` (before a consuming call), called
  from `selfhost/emit/ops.hero:178` and `:204`.
- `runtime/parts/alloc.c`, 610 lines: the set from `:372`, `hero_handle_slot`
  `:380`, `hero_handle_grow` `:389`, `hero_handle_acquired` `:402`,
  `hero_handle_consumed` `:422`. `runtime/heroes_runtime.h:205-206` declares the
  two, and `HERO_RUNTIME_ABI` is **22** at `:37`, which moves whenever a
  declaration changes shape.
- `tests/harness/suite_runtime.hero:199-204` lists `hero_handle_set`,
  `hero_handle_lock` and `hero_handle_cap` as shared state by decision; a new
  table beside them is a new entry there (panel 173 R4 is the precedent).
- `runtime/parts/os.c:150-291`, panel 173's crash handler: it speaks only
  `held > 0` (`:249` POSIX, `:207` Windows).
- Four goldens under `tests/golden/run/` name `hero_handle_` in their text.

## What to measure

1. **Price A, B, C and E in code lines**, compiler and runtime separately, and
   say what each does to `HERO_RUNTIME_ABI` and to the emitted C of every
   `tests/golden/emit/` and `tests/golden/ir/` case that carries a mark.
2. **Prototype A far enough that the four reproducers of Question 1 change
   verdict**, and say what they print. The releaser's identity has to reach the
   runtime from `for_call` and the consumer's from `before_call`; say what
   identity is sound across translation units, since separate compilation emits
   one `.c` per module (a string literal's address is not one).
3. **The shapes beside A**, run rather than argued: a handle reached through a
   RECORD (`acquires UnloadFont` on a `Font` reaching two handles, one mark on
   the whole value, panel 149); a handle acquired in one module and released in
   another through a Heroes wrapper; a `borrows` result later given to a
   releaser; and a handle C hands out AGAIN at the same address after it was
   released (the set's comment at `alloc.c:411-414`).
4. **B's reach**: how many of the four reproducers a same-function rule would
   refuse, and what it would take to reach the rest.
5. **E**: whether a line with no lease live can be made true on every path the
   handler reaches — C's own `abort()`, a failed C `assert`, a trap — using only
   what the handler holds. `docs/panel/173-briefs/` carries the programs panel
   173 used (`cabort_lease`, `cassert_lease` among them).
6. **The time.** `selfhost/` declares no handle mark, so the compiler's speed
   is not expected to move; if your route touches the emitter for every call
   rather than only marked ones, time `./heroes check selfhost/main.hero` before
   and after with `/usr/bin/time -p`, three runs each, with nothing else running.

Give a verdict, the cost, one falsifiable prediction with the milestone at which
it is checkable, and the condition under which you would change your mind.
