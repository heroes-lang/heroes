# Panel 190, the shared brief: every return sweeps every owned slot, and the routes that write the sweep once

Written 2026-10-04 from 07:40 by the coordinator on the trunk at `703af779`,
**repaired from 08:07 after the completeness critic's first pass**
(`docs/panel/190-reports/completeness-critic-briefs.md`; the text it read is
`00-shared-before-the-critic.md`). Every seat works from `git archive
703af779`, never from a working tree. Convened by the author's answer *4a* of
2026-10-04 (`docs/records/log/2026-10-04-0150-the-author-answers-1a-2a-3a-4a-5a.md`):
a sitting in the **soundness lane**, the compiler-engineer and the
ffi-pragmatist with the completeness critic, no paid run, after panel 189,
whose synthesis was written at 04:06. Every number below names the command
that produced it; what was not run says so.

## Why this sitting

Defect 231 (`adjacent`, `docs/work/defects/231-every-return-sweeps-every-owned-slot-so-a-function-of-many.md`):
a function of many returns and many owned slots grows its IR and its C as
their product, because every block that returns releases every refcounted
slot. **The rule it follows is design.md Part 5's** (`docs/design/design.md:2752`
to `:2758`, panel 182's wording): *every block that returns, a `return` and
the early return a `?` lowers to, performs the `@` copy-out, retains what it
returns, and releases every local and synthetic slot*. Its roots: panel
021's point 2 and R3 (*cleanup is a walk over a fixed table, not a liveness
analysis*: every refcounted slot zero-initialised in the prologue, every exit
edge decrefs every slot unconditionally, a decref of a null string a no-op),
and panel 106 (which refused slot sharing). **A history no seat should
miss**: the sentence panel 182 replaced, from `8cacab7a` on 2026-08-03,
promised *a cleanup-label chain per function*, which was never built (`git
log -S 'cleanup-label chain per function'`, the critic); panel 182 deferred
coalescing synthetic slots. Rule 5 of `selfhost/ir/own.hero` (an owning
temporary moved into a synthetic slot) is what multiplies the slots: at
N = 400, 800 of the 1,200 are synthetic (the critic).

**Measured by the coordinator from 07:29 to 07:41 (the files' times) on the
trunk's compiler at `703af779`** (built from the seed, sha256 beginning
`2d55c5ff8309b812`; `heroes build <shape> --emit-c`, then `wc -l` and `grep -o
'hero_str_decref(' | wc -l`), and **re-emitted byte for byte by the critic**
at 100, 200 and 400, over batch 8's emit lane's shapes
`<scratchpad>/batch8/emit/shapes/slots-returns-N.hero` (a function of N
strings, each followed by an `if` that returns it):

| N | returns | lines of C | `hero_str_decref` calls in the C |
|---|---|---|---|
| 100 | 101 | 97,108 | 30,603 |
| 200 | 201 | 374,008 | 121,203 |
| 400 | 401 | 1,467,808 | 482,403 |
| 800 | 801 | 5,815,408 (written at 07:41:34) | 1,924,803 |

Each doubling multiplies both by 3.85 to 3.99; the 800 row equals the emit
lane's own file, written 2026-10-04 at 00:04. The IR's count at 400 is
481,201 `decref_slot` (`--dump-ir`, then `grep -c`, run by the critic).
**What clang does with it at `-O0` is already answered**: `heroes build
--emit-c` compiles every unit before it writes the C (`selfhost/cli/produce.hero:243`
to `:272`, at `-O0` by `verbs.hero:31`), and the trunk's cache holds the
800 shape's object, 49,024,896 bytes, written at 07:41 (the critic). At
`-O2` and under `--sanitize` it is unrun. **Carried, not re-run**: lane
irverify's *`slots-returns-400` builds in 64.57 s, and at 800 clang does not
finish in 300 s* (2026-10-03), at an optimisation level it does not name.

## The proposal, and the routes beside it

**The proposal** (batch 8's emit lane): **one exit block per function**;
every `return` stores its value and jumps there, and the exit performs the
copy-out and the unconditional sweep once. Its lane's figure, *about 1,600
instructions instead of 481,201 at 400*, is **derived, not run**, and leaves
out the store of the returned value at each return (the critic's arithmetic,
unrun, gives about 2,000 to 2,800).

**Every route the sitting judges, each to be priced and none presumed**: the
shared exit; one sweep instruction per exit edge (the IR carries a sweep as
one instruction the emitter expands once per function); a cleanup ladder in
initialisation order (the chain design.md promised before panel 182, never
built); fewer slots (coalescing the synthetic slots, which panel 182
deferred and panel 106's refusal of slot sharing may bar); the shared exit
only above a threshold of returns times slots; a sweep pruned by liveness
(which panel 021 refused, and why); a C-only cleanup label
(`.claude/rules/generated-c.md:33`'s *one `goto` and label per basic
block*); and leaving the product as it is. Bounding the inputs by refusing a
large function would be a diagnostic, which takes the question out of the
soundness lane: name its price only.

## The questions

**Q1. Is each route sound, and by what argument?** The question, not a
premise: show, for each route you build or price, that every path's slots
at its exit are released exactly once and nothing a path did not own is
released, on every shape a path can take: an early return of a slot's value;
an `@` parameter written then returned early (copy-out, which lowering
writes from four call sites, `selfhost/ir/emissions.hero:38` to `:43`, and
the verifier's `check_copy_out`, `selfhost/ir/verify.hero:215`); a return in
a loop, a `match` arm, an `if` used as a value; a `.must()` and a `?` on a
return path (a chain of twenty `?` holds 2,610 `decref_slot`, the critic's
`--dump-ir`); a slot stored on one path and not on another; `@` parameters
times returns (20 by 21 gives 420 `copyout`, the critic); a counted return
type that is not `str`. Say what breaks and on which route.

**Q2. The cost of each route**: lines, against the files' ceilings in the
instrument's own unit (`selfhost/ir/own.hero` reads **288 of 300**, 12 lines
of room; `selfhost/ir/flatten.hero`, where a `return` and a `?` become
terminators at `:136` to `:148` and `:470` to `:480`, reads **1,150 of its
`DECIDED` 1,150**, none; both settled by the critic with `layout`); what each
changes in every program's emitted C (`tests/emission/`'s blessed files:
how many move, and is each move only the exit's shape); design.md Part 5's
sentence, which any route but the last rewrites (a panel's text: say the
sentence your route would need).

**Q3. What the instruments say**, each run whole with the harness's own
command: `ir` (23 goldens, 15 holding `decref_slot`; `UPDATE_GOLDEN` is
forbidden there, so a route that moves them names each edit), `emit`,
`emission`, `determinism`, `wholes`, `descriptors`, `lines`, `warnings` (the
flag list carries `-Werror=conditional-uninitialized`), `layout`, the
compiler's own tests, ASan and the leak gate, the IR verifier (defect 218's
linear reading); and the stack frame of the shapes (panel 106's subject).
The compiler's own timing is owed and cannot be taken beside the lanes:
name it as owed.

**Q4. Its size**, measured on what each built route emits, at the shapes
above and on the compiler's own emission (the seed's lines); and, on each
platform you reach, whether the 800 shape builds at `-O2` and under
`--sanitize` within a bound you name (a finish or not, never a duration
beside the lanes).

## Seats convened

`compiler-engineer` (builds the shared exit and at least one other route far
enough to compare, prices the rest, runs Q1's cases and Q3's instruments),
`ffi-pragmatist` (the emitted C on the platforms: the shapes at `-O2` and
under the sanitizers, and what an exit's order means for memory a C library
still reads, panel 106's carried warning that ASan does not see a library
reading freed memory), and the completeness critic, whose first pass this
text answers, over the reports after.

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/190-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 703af779 | tar -x -C <your
copy>`; build your compiler inside it from the seed, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes` (the seed's sha256 begins
`2d55c5ff8309b812`: check yours), and rebuild after an edit with `./heroes
build selfhost/main.hero -o heroes`. The shapes are
`<scratchpad>/batch8/emit/shapes/` (read only; copy what you need); the
critic's beside-shapes are in `<scratchpad>/190-critic/beside/` (read only).
Never build, run or read inside another seat's copy or a lane's worktree,
and in the trunk write nothing but your own report. **Never `rm` anything**,
`-f` or `-rf`: use a new folder name instead. **No paid run.** Batch lanes
share this Mac: at most two processes at once, no timing. Docker: one
container at a time, `docker ps -q` empty first. The Windows box: `ssh win`,
one folder of your own under `/c/w/`, files sent in 1 MB parts as
`<scratchpad>/platforms/windows2-u.sh` does (read it, never run it). **Every
time you write is read from `date` at that moment, in its own command before
the text that holds it.** Your report is `docs/panel/190-reports/<seat>.md`
in the TRUNK, written as you go: a verdict per question, what you built and
ran, your cost, a falsifiable prediction, the condition that would change
your verdict. English, no em dashes.
