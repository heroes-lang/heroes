# Panel 182, shared brief: what the emitted C zeroes at a function's entry, and why

Written 2026-09-28 between 01:29 and 01:40 by the coordinator, for every seat.
Every number and path below names the command that produced it, run while this
brief was written, on the trunk's seed at `dfcac362` and its compiler built
from that seed with CLAUDE.md § Commands' first line. The tree the seats copy
is named in each seat's brief with the `git log -1` read before the briefs
went out.

**The lane is the soundness lane** (`.claude/skills/panel/SKILL.md` § Two
lanes): the question changes no surface, no diagnostic and no spec token, only
what the emitter and the lowering produce, so the two seats that compile sit,
the compiler-engineer and the ffi-pragmatist, with the completeness critic
after them. What the lane gives up is a reader's view of the change, which
has no reader-facing half unless a seat finds one; if a seat does, it says so
and the sitting widens.

## The sitting and why

**Defect 114** (`docs/work/DEFECTS.md`, the item whose first field is `114`),
found by lane 105's agent on 2026-09-27: the emitted C zeroes temporaries at
a function's entry, so a lookup that returns early pays for every arm. The
defect's body carries a note of 2026-09-28 by the coordinator: *the zeroing is
not waste, it is the release's precondition*, because every arm of a `match`
writes into an owned variable of its own and the function's exit releases all
of them, the arms not taken included. **Measuring for this brief found that
the note is true of one part and not of the rest**, which is why the sitting
has two questions and not one.

## The emitted function, read

`h_keywords_keyword` in `seed/heroes.c` (the lookup `selfhost/keywords.hero:148-170`
compiles to), extracted with a brace-matching script from its definition line:

- **941 lines, 139 `= {0}`, 21 calls to `hero_str_eq`, 23 owned variables**
  (`h3_own3` to `h25_own25`, counted as distinct `h<N>_own<N>` names), and 182
  declaration lines before the first `goto`.
- **The owned variables.** Each arm stores its value into its own:
  `t152 = h23_own23; h23_own23 = t106; h_0opt_316a1e6d_release(&t152);`, then
  copies it into the result: `t153 = h2_r0; h_0opt_316a1e6d_retain(&t106);
  h2_r0 = t106; h_0opt_316a1e6d_release(&t153); goto bb1;`. The exit block
  `bb1` retains the result into a return temporary and then releases `h1_s0`,
  `h2_r0` and **all 23 owned variables**, `h3_own3` to `h25_own25`. An arm not
  taken leaves its owned variable as it was declared, so without `= {0}` the
  exit would release an uninitialised aggregate. For these 23, the note holds.
- **The temporaries.** The other declarations are values (`t1` to `t156`):
  `HeroStr t2 = {0}; HeroStr t3 = {0}; bool t4;` for each comparison, written
  `t2 = h1_s0; t3 = HERO_STR_LIT(...); t4 = hero_str_eq(t2, t3);`. A read of
  the arms shows no release of `t2` or `t3` anywhere in the function; a value
  such as `t152`, which is released, is released only after the instruction
  that assigns it. **Whether every `= {0}` on a value is dead is a claim this
  brief does not make**: it is the compiler-engineer's first task, over every
  function and not this one.

## Where the compiler does it

- `selfhost/emit/body.hero` (333 lines): the slots' declarations, `= {0}` when
  `layout.is_refcounted(c, slot.ty)` (lines 156-157); the values'
  declarations, the same test (lines 204-205), after `unread.assigned` and
  `unread.discarded` have decided which values are declared at all.
- `selfhost/ir/layout.hero:153`, `is_refcounted`.
- `selfhost/emit/unread.hero` (355 lines): `defs` (line 95), which since defect
  112's repair (`c3dc424b`) finds each temporary's first live writer in one
  pass; `assigned` (124); `slots_read` (232).
- `selfhost/ir/own.hero` (354 lines), the ownership pass, whose doc's rule 5
  reads: *an owning temporary is MOVED into a synthetic slot, in the block that
  defines it. Nothing is ever owned by a temporary past its own instruction, so
  nothing is released at a block boundary: ownership lives in slots, and
  cleanup is a walk over a table.* The per-arm owned variables are those
  synthetic slots, and the exit sweep is that walk.
- The verifier checks that a value's definition dominates its uses
  (`h_irvalues_dominance` appears in the profile below; `selfhost/ir/verify.hero`).
- The runtime ABI is 26 (`runtime/heroes_runtime.h:37`).

## How much it costs, measured

- **The whole seed**: `grep -c "= {0};" seed/heroes.c` is **102,990**, of which
  **29,366** are `HeroStr t<N> = {0};` (a temporary of type `str`) and
  **10,902** are `<type> h<N>_own<N> = {0};` (an owned slot), by `grep -cE`
  with those two patterns; the rest are slots and temporaries of other
  refcounted types, not broken down here.
- **The formatter**: `heroes fmt` on a 36,672-line file (lane 105's `w16.hero`,
  sixteen copies of `selfhost/check/walk.hero`) read `user 2.94`, `real 2.98`.
  A `sample` of it, 1,914 samples at the top of the stack: **`memset` 439**
  (`_platform_memset` 363 and its stub 76), attributed to the nearest compiler
  function on the stack as `keywords.declared_word` 44, `keywords.keyword` 37,
  `scan.one_byte` 31, `grammar_expr.binary_op` 31, `print/parens.render` 28,
  `print/bodies.render_expr` 22, `scan.ident` 19, the rest under 13 each.
  Lane 105's measurement, the defect's origin, was 240 of 1,113.
- **The emitter**: a `sample` of `build --emit-c` on a generated program of
  2,000 units (defect 112's ladder), about 10,400 samples: `memset` 1,079,
  the largest share (174) under `emit/inst.emit`.

## What the sitting decides

1. **Which zeroing a release needs.** A value or slot needs `= {0}` if some
   path from the entry reaches a release or a read of it before a write;
   otherwise the zeroing is dead. The route that removes only dead zeroing
   keeps the guarantee it buys and changes no lowering.
2. **Whether the arms need an owned slot each.** Rule 5 of `ir/own.hero` puts
   each arm's owning value in a synthetic slot of its own, which the exit
   sweep releases; a lowering where the arms share one destination, or where
   an arm's slot is released where the arm joins, changes the ownership pass,
   which is the architecture this lane judges.
3. **Robustness first** (CLAUDE.md § Precedence, rank 3): no route may leave
   a path on which the emitted C reads or releases an uninitialised value, and
   the instrument that would catch one (ASan in the `run` suite, clang's
   `-Wsometimes-uninitialized` and `-Wuninitialized`, the IR verifier) is named
   for each route.
4. **What the default build line pays**: the plain `clang` of CLAUDE.md
   § Commands does not optimise; say what `-O2` does with each route's C, so
   the number is not only the unoptimised one.
