# Panel 190, the shared brief: every return sweeps every owned slot, and one exit per function

Written 2026-10-04 from 07:40 by the coordinator on the trunk at `703af779`,
frozen from the briefs to the synthesis for every seat's purpose: each seat
works from `git archive 703af779`, never from a working tree. Convened by the
author's answer *4a* of 2026-10-04
(`docs/records/log/2026-10-04-0150-the-author-answers-1a-2a-3a-4a-5a.md`):
a sitting in the **soundness lane**, the compiler-engineer and the
ffi-pragmatist with the completeness critic, no paid run, after panel 189,
whose synthesis was written at 04:06. Every number below names the command
that produced it; what was not run says so.

## Why this sitting

Defect 231 (`adjacent`, `docs/work/defects/231-every-return-sweeps-every-owned-slot-so-a-function-of-many.md`):
a function of many returns and many owned slots grows its IR and its C as
their product, because every exit edge releases every refcounted slot. That
is panel 021's design, point 2 and its R3 (*cleanup is a walk over a fixed
table, not a liveness analysis*: every slot zero-initialised in the
prologue, every exit edge decrefs every slot unconditionally, a decref of a
null string a no-op), kept by panel 106. Its cost was named then as
over-retention; its size is this defect.

**Measured by the coordinator from 07:29 to 07:41 (the files' times) on the trunk's compiler at
`703af779`** (built from the seed, sha256 beginning `2d55c5ff8309b812`;
`heroes build <shape> --emit-c`, then `wc -l` and `grep -o 'hero_str_decref('
| wc -l`), over batch 8's emit lane's shapes
`<scratchpad>/batch8/emit/shapes/slots-returns-N.hero` (a function of N
strings, each followed by an `if` that returns it):

| N | returns | lines of C | `hero_str_decref` calls in the C |
|---|---|---|---|
| 100 | 101 | 97,108 | 30,603 |
| 200 | 201 | 374,008 | 121,203 |
| 400 | 401 | 1,467,808 | 482,403 |
| 800 | 801 | 5,815,408 (written at 07:41:34; no duration is a measurement beside four lanes) | 1,924,803 |

Each doubling multiplies both by 3.85 to 3.99, the 800 row being the emit lane's own count of 2026-10-03 to the line. **Carried, not re-run**: lane
irverify's *`slots-returns-400` builds in 64.57 s after defect 218's repair,
and at 800 clang does not finish in 300 s* (2026-10-03, the defect's item);
batch 8's emit lane's *481,201 `decref_slot` for 402 returns and 1,200 slots
at 400*, counted in the IR, and its proposal, *one shared exit per function
(the sweep unconditional as 021 R3 asks, once): about 1,600 instructions
instead of 481,201 at 400*, **derived, not run**.

## The proposal to judge

**One exit block per function**: every `return` stores its value (increfed
first, rule 4 of `selfhost/ir/own.hero`) and jumps to the function's one exit
block, which copies out the `@` parameters and decrefs every refcounted slot
once, unconditionally, as panel 021 asks, then returns. The table walk, the
zero-initialisation and the unconditional sweep stay; only the number of
times the sweep is written changes, from one per exit edge to one per
function.

## The questions

**Q1. Is it sound?** Every path's slots at its return are released exactly
once, and nothing a path did not own is released: the zero-initialised
slots of a path that never reached them are null, and a decref of null is a
no-op (021's own argument). What breaks: a slot reused across paths with a
value a later path did not write (panel 106's frame); an `@` parameter's
copy-out on an early return; a returned value that is itself a slot's (rule
4); a `.must()`, `?`, `&&` or if-as-value opening a block mid-expression
(rule 5's history). Build it and run the cases that would break it.

**Q2. The routes, even to be refused**: the shared exit; a sweep pruned by
liveness (021 refused a liveness pass, and why); a C-only cleanup label
(`.claude/rules/generated-c.md`'s *one `goto` and label per basic block*);
leaving the product and bounding the inputs; any other a seat finds. Each
with its cost in lines (`selfhost/ir/own.hero` is 534 lines, its ceiling
`tests/harness/suite_layout.hero`'s), what it changes in the emitted C for
every program (`tests/emission/`'s blessed files: how many move, and is each
move only the exit's shape), and what would make it wrong.

**Q3. What the instruments say.** The IR verifier (defect 218's linear
reading), `wholes` (a write into a part of a temporary that nothing wrote
whole on that path), `descriptors`, `determinism`, `lines` (every `#line`
restore), ASan and the leak gate, on every platform the project runs.

**Q4. Its size at the shapes above**, and on the compiler's own emission
(the seed's lines), measured on the built route; whether clang finishes
`slots-returns-800` on this Mac, in the Linux arm64 image under clang 22 and
18, and on the Windows box's clang 23, and what the shape costs there.

## Seats convened

`compiler-engineer` (builds the route in its copy, runs Q1's cases, the
compiler's own tests and the suites Q3 names), `ffi-pragmatist` (the emitted
C on the platforms, ASan and LeakSanitizer where they exist, the `@` and
handle paths at an exit), and the completeness critic over these briefs
first and the reports after.

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/190-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 703af779 | tar -x -C <your
copy>`; build your compiler inside it from the seed, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes` (the seed's sha256 begins
`2d55c5ff8309b812`: check yours), and rebuild after an edit with `./heroes
build selfhost/main.hero -o heroes`. The shapes are
`<scratchpad>/batch8/emit/shapes/` (read only; copy what you need). Never
build, run or read inside another seat's copy or a lane's worktree, and in
the trunk write nothing but your own report. **Never `rm` anything**, `-f` or
`-rf`: use a new folder name instead. **No paid run.** Four batch lanes share
this Mac: at most two processes at once, no timing (a duration taken beside
four lanes is no measurement; say so where one is owed). Docker: one
container at a time, `docker ps -q` empty first. The Windows box: `ssh win`,
one folder of your own under `/c/w/`, files sent in 1 MB parts as
`<scratchpad>/platforms/windows2-u.sh` does (read it, never run it). **Every
time you write is read from `date` at that moment, in its own command before
the text that holds it.** Your report is `docs/panel/190-reports/<seat>.md`
in the TRUNK, written as you go: a verdict per question, what you built and
ran, your cost, a falsifiable prediction, the condition that would change
your verdict. English, no em dashes.
