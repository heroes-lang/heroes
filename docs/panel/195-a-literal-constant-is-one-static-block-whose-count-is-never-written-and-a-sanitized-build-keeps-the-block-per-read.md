# Panel 195: a literal constant is one static block whose count is never written, and a sanitized build keeps the block per read

**The soundness lane** (`.claude/skills/panel/SKILL.md`): the compiler-engineer
and the ffi-pragmatist, the completeness critic before and after. Convened
2026-10-06 by the coordinator on lane b12-ir12's recommendation, under the
author's goal of that day (`issues/2026-10/06/2026-10-06-1144-the-author-s-goal-every-defect-but-the-improvements-closed-within.md`);
briefs written from 11:44 to 11:48, repaired after the critic's first pass
(closed 12:20:30), the seats from about 12:25 to 13:10, the critic's second
pass closed 13:26:29, this synthesis from 13:28, every time read from `date`.
The briefs are `195-briefs/`, the reports `195-reports/`. The seats worked on
lane b12-ir12's branch head, `96f3a588`, with `f7576a01` in it.

## The proposal, verbatim from the brief

**How does a read of a constant stop building it again, soundly, on the three
platforms and in a program whose callbacks run on another thread?** Defect 382
(`adjacent`): a `constant` of an array or map type is lowered as a function of
no arguments (`selfhost/ir/lower.hero:77`) and every read builds it again.
Lane ir12's first route, `f7576a01`, builds every array literal in one block;
a read of a 32-element constant a million times still retires 4,597,018,435
instructions against 117,277,639 for a name bound once to it, about 39 times
at `-O0` and 67 times at `-O2`.

## The verdict table

| | compiler-engineer | ffi-pragmatist |
|---|---|---|
| **verdict** | **approve** the static block as it built it; object to every other route; no veto | **veto** ir12's static `const` block as written and the static writable block; **approve** the element route and `onceheld`; object to the rest |
| **section** | §1.7, Part 5, §1.12 | §1.12 (its last paragraph: a defensive check that hides a defect is in the wrong place), §1.11, §4.19, defect 361, CL-007 |
| **cost** | new `emit/static_constant.hero` 211 lines; header 684 to 709, `array.c` 341 to 369; ABI 27 to 28; `k` 147.9M / 54.8M instructions at -O0 / -O2, 1.28x / 1.43x the floor; the compiler's own `--emit-c` 0.72% fewer | element route 79.7M / 23.4M (0.68x the floor); `onceheld` 174.7M / 78.5M (1.50x / 2.01x) |
| **prediction** | at the commit closing 382, `k` at -O0 at most 1.5x `local`, and `--emit-c` no more instructions than before | `onceheld` leaves `heroes_runtime.h` byte-identical; a release too many names `heap-use-after-free` under `--sanitize` on all three platforms |
| **condition** | to object if Linux arm64 or Windows writes a static block without a fault, if a runtime path writes a header without testing its count, if a run golden's output or `--sanitize` result moves, or if `--emit-c` rises | the veto drops to *object* if the block goes through a runtime macro whose members are `h` and `b`, its name is in `guarded_names`, and the stamp moves 27 to 28; approve only if a release too many on a constant stays loud |

## What the sitting measured

- **The compiler-engineer built the route** (`HERO_ARRAY_STATIC`, members `h`
  and `b`, four words added to `guarded_names` and both guard files, the
  stamp 27 to 28, `hero_array_incref` and `hero_array_decref` returning on a
  count of exactly -1 and panicking by name on any other count below 1). A
  constant is laid out only where its one block holds literals, array and
  case constructions and the ownership pass's bookkeeping; calls, arithmetic,
  branches, maps, `f"..."` and function values keep the landed route, so a
  function constant keeps its `funcref` and its thread guard. All 23 array
  constants of the compiler are laid out; its own tests 1,288 passed; the
  emitted C byte-identical between stages. `[Record]` cannot be a constant's
  body today (`constant_body`).
- **The critic rebuilt it in its own copy and ran it on four platforms**:
  `k`, `ks`, `spawnk` and a nested variant constant run clean on Darwin, Linux
  arm64, Linux x86-64 and Windows; ASan clean on the three Unix legs; TSan 0
  warnings on Linux arm64 (x86-64 unrun, TSan refuses the emulated layout); the
  old runtime refuses the new C and the new runtime the old, both ways, on all
  four (`'27 == 28'`, `'28 == 27'`). **It meets each of the ffi-pragmatist's
  conditions to lift the veto** (the macro, `h` and `b`, the seat's
  `hostile.h` defining `e`, `b` and `h` builds under it, `NAMES`, the stamp),
  **and fails the last**: a release too many on a constant stays invisible. By
  that seat's own words the route stands at *object*.
- **A release too many, and a release missing, under each route** (the
  critic, the same sequence: a reader releases once too many, a second writes
  `xs[0] @ 99` on its copy, a third reads `K[0]`):

  | route | release too many, plain | under ASan | release missing |
  |---|---|---|---|
  | landed (`f7576a01`) | loud, a misleading message, exit 134 | `heap-use-after-free` | *1 heap blocks still live* |
  | static block | the right value, exit 0 | silent | passes the leak check |
  | `onceheld` | **prints 99 where the constant says 3**, exit 0 | 99, then `heap-use-after-free` | *1 heap blocks still live* |

  **Under `onceheld` one release too many lets a reader write the shared
  constant in place, and every reader on every thread then sees the changed
  value** (Darwin and both Linux legs; on Windows the heap manager reports it
  after the 99 is printed). The static block never changes; it can only be
  silent.
- **The compiler-engineer's louder guard** panics by name only on Darwin at 32
  elements: a freed block's first word is a non-negative pointer, so on Linux
  and below 32 elements it never reaches the guard (the critic). It costs +4.9%
  at -O0 and +5% at -O2 on a constant's block against ir12's plain guard,
  because the branch it treats as cold is the hot one for a constant. Nothing
  gets quieter with it.
- **The element route's gain is mostly an inlined read**: 84 to 86% of its
  0.68x comes from replacing the `hero_array_at` call with an inline index,
  which every array read could have; inlining that call in the floor alone
  takes `local` from 116.5M to 85.5M (the critic). As a new IR operation it
  joins 80 exhaustive matches in 28 files, and `f(K)` still builds
  (compiler-engineer).
- **Refused by measurement**: the static writable block (a missed guard calls
  `free()` on a static pointer: Darwin 134, glibc 134, Windows 0xC0000374);
  one copy per thread (`spawnk` dies *2 heap blocks still live* on Darwin and
  Linux; a registry at thread exit unbuilt); a static table and `memcpy` (a
  new runtime function, an allocation at every read); hoisting a read out of a
  loop (`ir/flatten`'s cycle, `tests/golden/ir/` moved, 8 sites).
- **A probe, not a route**: a per-thread count of references to static blocks
  makes a release too many panic by name on a plain build, at +52% / +82% over
  the static route always on; compiled only into `--sanitize` builds it would
  cost plain builds nothing; unbuilt.
- Instruction counts were taken on Darwin alone; the Docker VM has no
  hardware counter, and none was taken on Windows.

## Disagreements, stated plainly

The two seats disagree on one ruling only, and the critic's runs decide it.
The ffi-pragmatist approved `onceheld` because it keeps a real count, so a
release too many is loud; the critic measured that the same real count is what
lets the release too many hand a reader the shared block to write in place.
The compiler-engineer approved the static block because nothing can write it;
the ffi-pragmatist's last condition is that a release too many on a constant
be loud, and on the static block in a plain build it is not. **What a run
cannot settle is whether a guard that makes the compiler's own mistake harmless
but unnamed is §1.12's defensive check that hides a defect.** This sitting
rules that it is, in a plain build, and answers it where defects are hunted:
the sanitized build.

## The resolution — ratified by delegation (below)

1. **R1, the static block, as the compiler-engineer built it**: a constant
   whose one IR block holds only literals and array and case constructions is
   emitted as one `HERO_ARRAY_STATIC` block, members `h` and `b`, its count -1,
   never written; its read returns the block's address. Every other constant
   keeps `f7576a01`'s route. `hero_array_incref` and `hero_array_decref` return
   on a count of exactly -1 and panic by name on any other count below 1
   (kept for the case it catches, at its measured +5% on a constant's block).
   The four new words enter `selfhost/emit/guarded_names.hero` and both guard
   files; **`HERO_RUNTIME_ABI` moves from 27 to 28**, because emitted C now
   needs what only the new header declares (`hero_thread_guard`'s and defect
   245's precedent; the stamp still does not cover behaviour, CL-007).
2. **R2, a sanitized build keeps the block per read**: under `--sanitize` a
   constant is built at every read, as `f7576a01` builds it, so ASan names a
   release too many (`heap-use-after-free`) and a release missing (*heap
   blocks still live*) of a constant's value; a plain build pays nothing.
   **This is the answer to the ffi-pragmatist's last condition and to §1.12**:
   in the build where memory errors are hunted, nothing the static block makes
   silent is silent. **R2 is a choice between two routes both built, and the
   switch itself is unbuilt**: the landing builds it and runs the critic's
   release-too-many and release-missing sequences under `--sanitize`, each
   naming its error; if it cannot, the landing reports, R1 stands alone, and
   the residual is written into defect 382's closing.
3. **R3, the landing's cases**: the critic's shapes (a negative element, the
   largest `u64`, `[f32]`, `[[str]]` with an empty inner array, a 70,000-element
   constant) and the briefs' (`[str]`, `[[i64]]`, `[Variant]`, empty, a
   constant naming another, a map with a computed value, an `f"..."` element,
   an index that aborts, a function constant, `spawnk`), each a `run` golden
   with its output, plain and under `--sanitize`; Linux arm64 and the Windows
   box before the push, the C boundary's rule. Defect 382 closes on them.
4. **R4, refused**: `onceheld` (a release too many writes the shared constant
   in place, measured on three platforms); the static writable block; one copy
   per thread; a static table with `memcpy`; hoisting. **The element route is
   not adopted**: its gain is an inlined array read that every read can have,
   which is filed as an improvement of its own, `hero_array_at` inlined, with
   the critic's 116.5M to 85.5M.
5. **R5, the seed**: the stamp's move lands in a batch of its own, after batch
   12, so its seed is regenerated over two generations from a trunk at ABI 27
   rather than three.

**What the vetoes compel**: no static block that is not the runtime's macro,
whose members a header can rename, or whose stamp does not move. **What
conservative would have been** (CL-040): `f7576a01` alone, a read of a
constant 39 times a name's at `-O0`, and 382 closed with that cost written down.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the commit closing 382, `k` at -O0 at most 1.5x `local`, and the compiler's `--emit-c` no more instructions than before | the landing |
| ffi-pragmatist | (for `onceheld`, refused) a release too many names `heap-use-after-free` under `--sanitize` on all three platforms | R2's landing, which makes it true of every constant |
| the critic | the louder guard reaches its panic on Darwin at 32 elements only | the landing's `dbl.c` on each leg |

## Author's verdict

**RATIFIED by delegation, 2026-10-06**, on the author's answer at 12:49 by the
clock read then, given before this synthesis was written, meant as: *OK,
ratify* (`issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`,
point 3). The author may overturn it (CLAUDE.md § 4).
