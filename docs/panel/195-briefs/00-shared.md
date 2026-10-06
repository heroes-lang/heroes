# Panel 195, shared brief: a constant read without building it again (defect 382), the soundness lane

Convened 2026-10-06 by the coordinator on lane b12-ir12's recommendation, under
the author's go for this batch's sittings and their goal of that day, every
defect but the improvements closed within a day
(`issues/2026-10/06/2026-10-06-1144-the-author-s-goal-every-defect-but-the-improvements-closed-within.md`).
**The soundness lane** (`.claude/skills/panel/SKILL.md`): the question changes
no surface, no diagnostic and no spec token, only what a constant's read
emits and what the runtime does with it; two seats, compiler-engineer and
ffi-pragmatist, and the completeness critic before and after. **This brief was
repaired after the critic's first pass** (`<scratchpad>/p195/reports/completeness-critic-briefs.md`,
last written 12:20:30): two framing facts were false and are corrected where
they stand, and its other findings are § What the first pass found besides. **Every number
below is a command's output named beside it, run by the coordinator on
2026-10-06 between 11:44 and 11:48 by the clock read before and after; a number from lane ir12's report is marked
CARRIED.**

## The defect

**382** (`adjacent`; `ls issues/*/*/*-defect-382-*`): a `constant` of an array
or map type is lowered as a function of no arguments (`selfhost/ir/lower.hero:77`)
and every read is a call that builds it again. Lane ir12 landed a first route at
`f7576a01` (branch `lane-b12-ir12`): every array literal is now one
`hero_array_new` sized to it and one `hero_array_push_owned` per element, where
it was a copying push per element, n+1 blocks and n(n+1)/2 copies. The item
stays open because a read still builds the constant.

**Measured** (`/usr/bin/time -l`, instructions retired; `<scratchpad>/p195/k.hero`:
a `constant K: [i64]` of 32 elements read `K[at % 32]` a million times, and
`local.hero`, the same loop reading a name bound once to `K`), on a compiler lane ir12 built at 11:07 from its working tree before it
committed that change as `f7576a01` at 11:33:57 (the critic reproduced the
numbers on a compiler built from the commit: 4,599,763,643 and 116,461,213):
- `k`: **4,597,018,435**; `local`: **117,277,639**, so a read still costs about **39 times** a name's at `-O0`, `heroes build`'s
  default; at `-O2` 2,579,516,710 against 38,388,381, **67 times** (the critic). Before `f7576a01` the same `k` read
  56,768,740,722 on the trunk's compiler (measured by the coordinator 09:46).

## The routes lane ir12 built and measured (CARRIED, its report, 2026-10-06)

Hand-written C of the reproducer, instructions at `-O0` / `-O2`:

| route | -O0 | -O2 |
|---|---|---|
| before | 56,853,797,202 | 32,514,871,487 |
| landed, one block and in-place pushes (`f7576a01`) | 4,601,618,060 | 2,578,286,060 |
| a static C table, then in-place pushes | 4,832,551,658 | 2,611,814,164 |
| a static C table and one `memcpy` (a new runtime function) | 1,734,408,333 | 930,658,924 |
| one copy per thread (`_Thread_local` and an incref per read) | 195,594,982 | 63,542,205 |
| **a static block, its count -1, never written (ir12 recommends)** | 142,668,198 | 52,641,845 |
| the constant hoisted into a name (the floor) | 117,769,340 | 38,608,624 |

ir12's recommended C: `static const struct { HeroArrayHeader h; int64_t e[32]; }
K_static = {{-1, 32, 32, &hero_desc_int}, {...}};` and `h_k_K` returning
`(HeroArrayHeader *)&K_static.h`, with one line added to each of
`hero_array_incref` and `hero_array_decref`:
`if (atomic_load_explicit(&a->refcount, memory_order_relaxed) < 0) return;`.
For a `[str]` constant read a million times: 5,810,514,277 before against
170,140,626 (CARRIED). The per-thread route needs a registry to release each
thread's copy at its exit and before the leak check (ir12).

## What the runtime does today, read by the coordinator

- **Strings already have it**: `HERO_STR_STATIC` (`runtime/heroes_runtime.h:198`)
  lays out a `static const` header with count `-1`; `hero_str_incref` and
  `hero_str_decref` (`runtime/parts/str.c:130-148`) return on a negative count,
  read relaxed, and the header's comment demands a lock-free 64-bit atomic
  because the block lives in read-only memory.
- **Arrays read a negative count today as doomed**: `hero_array_decref`
  (`runtime/parts/array.c:78-91`) subtracts first and treats any count at or
  below 1 before the subtraction as a block to release, *negative counts
  included* (its comment); a doomed block is threaded onto `drop.c`'s list
  **through its own refcount field** (`runtime/parts/drop.c:84`, `memcpy(&a->refcount,
  &next, ...)`). **Corrected on the critic's first pass**: on this Mac a
  `static const` block lands in `__DATA_CONST,__const`, and today's `incref`
  and `decref` both die with SIGBUS at their first write to it (exit 138,
  stderr empty); nothing is freed (Linux and Windows unrun). So the route
  needs the guard. **And the trade is not the one this brief first named**: a
  release too many on a heap block does not drive its count negative, it
  frees the block at count 1 and the next release reads freed memory (32
  elements end in malloc's SIGTRAP, exit 133; 4 run silently to exit 0;
  defect 387, open and blocking, reaches it today); the guard changes neither.
  **The trade is that a static block is never freed**, so a release too many
  of a constant's value becomes invisible even to ASan, as it already is for a
  `str` literal. That is the first thing this sitting rules.
- `cow.c` copies a block whose count is not 1 before a write, so a static block
  is copied on its first write (CARRIED: ir12 ran copy-on-write of the shared
  block clean under ASan and UBSan).
- **The runtime's ABI** is 27 (`grep -m1 'define HERO_RUNTIME_ABI' runtime/heroes_runtime.h`),
  and **the stamp is not extended to cover behaviour** (`.claude/rules/generated-c.md`,
  CL-007; `drop.c:19-26` applies it): emitted C that needs the guard, linked
  with a runtime without it, is the silent SIGBUS above, and what makes that
  loud is this sitting's question, not an assumption;
  emitted C that hands the runtime a static array block needs a runtime that
  skips it, and the guard headers of defect 361 (`runtime/heroes_guard_open.h`,
  `selfhost/emit/guarded_names.hero`) push and pop every word the runtime's
  headers write.

## The constants that exist

66 constants of an array or map type in `selfhost/` and `tests/harness/`
(`grep -rE '^constant [A-Z_0-9]+: (\[|\{)' selfhost tests/harness | wc -l`), all
with literal-only bodies, 61 `[str]` and 5 `[i64]`, none a map (the critic re-ran
it); read inside a loop by text, 19 counting the name inside string literals
and **8** without them, `clang_told.hero`'s `LEVELS` the only one in `selfhost/`
(the critic's `tools-critic/loops.py`). The specification promises
no cost for a constant (spec § 4: *a written body computes over literals and
other constants*), and a body may hold other constants.

## The question

**How does a read of a constant stop building it again, soundly, on the three
platforms and in a program whose callbacks run on another thread?** Routes on
the table: the six above; a static block for arrays of scalars and of `str`
only, the rest staying on the landed route; the lowering hoisting a constant's read out of a loop; **nothing more**,
`f7576a01` as the repair; and three the critic named, unbuilt: **`K[i]`,
`K.len()` and `for x in K` lowered straight onto a static C array of elements**
(no runtime change, no guard, no ABI question; six of the eight loop sites are
element reads or loop headers); **one process-wide copy built on the first
read**, published by an atomic compare-and-swap in writable memory (serves
every shape, keeps an abort at the read, still needs the guard and a release
at exit); **a `static` rather than `static const` block**, so a missed guard
is a count moved rather than a SIGBUS. What must hold:
no write to read-only memory, no count a thread can race on, a release too many still loud somewhere, the ABI stamp
honest, and every shape a constant may take served or refused by a reason:
records, nested arrays, variants, a map (a computed value, `BASE + 2`, is
legal), an `f"..."` element, an index that aborts (it aborts at the read
today, exit 134), and **a constant of functions**, `[(function(i64) -> i64)]`,
whose callbacks get their thread guard only through the `funcref` in the
constant's IR (`selfhost/emit/callback_guard.hero`), which a static table
writing `&h_twice` would drop. Evaluating a body at compile time would add a
diagnostic and leave this lane.

## What the first pass found besides

- **Threads**: a callback from a C library's own thread panics at its entry
  (`runtime/parts/thread.c:87-99`), so it never reads a constant; the
  concurrent read a program can reach is through `hero_thread_spawn`, and the
  critic ran two spawned threads and `main` reading `K` at exit 0, also under
  `--sanitize`. Test with spawn.
- **Defect 361's guard list**: a struct the emitter writes names its members;
  `h` is in `guarded_names.NAMES`, an element member `e` is not.
- **Which compiler measures**: the seed predates `f7576a01` (a compiler built
  from it alone retires 56,873,259,045 on `k`), so build the branch's
  compiler, the second build step below, before measuring anything.

## How the sitting runs

Each seat works in its own copy, `<scratchpad>/195-<seat>/`, made by
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 96f3a588 | tar -x` (lane
ir12's branch head: `f7576a01` and the round's newest records, defect 387's
issue among them), with `<scratchpad>/round-b12/run-4254/seed-new.c`
copied over `seed/heroes.c`; build `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, then `./heroes build selfhost/main.hero -o
heroes` for this branch's compiler. Never the repository, never the other
seat's copy. No timing; instruction counts only. No paid run. Your report goes
to `<scratchpad>/p195/reports/<seat>.md`, written as you go.
