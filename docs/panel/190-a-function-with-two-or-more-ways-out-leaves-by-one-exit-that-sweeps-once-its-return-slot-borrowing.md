# Panel 190: a function with two or more ways out leaves by one exit that sweeps once, its return slot borrowing

2026-10-04, written from 13:57 by the clock (`date`). The **soundness lane**:
`compiler-engineer` and `ffi-pragmatist`, with the completeness critic over the
briefs first and over the reports after. Convened by the author's answer *4a*
of 2026-10-04 (`docs/records/log/2026-10-04-0150-the-author-answers-1a-2a-3a-4a-5a.md`)
for defect 231. **No paid run.**

**The tree.** The seats worked from `git archive 703af779`, each in a copy of
its own under the scratchpad. The trunk moved once during the sitting, at
`f6a3122e` (12:36, the author's answers *1a* to *6b*: records and one rule
file), which no seat's measurement reads. The briefs are
`docs/panel/190-briefs/`, repaired from 08:07 after the critic's first pass
before any seat was launched; each keeps the text the critic read as
`<name>-before-the-critic.md`. The reports are `docs/panel/190-reports/`.

**Interruptions.** Both seats stopped at the account's session limit at about
10:27, and resumed at 12:27 and 12:28 from their own records. The machine did
not sleep during the sitting: the kernel's last sleep is 2026-10-01 at 16:33,
`sysctl kern.sleeptime`, read at 13:57.

**Faults of process, each the seat's own record:**
- **The compiler-engineer** ran `rm -f` at about 08:30 on its own copy of one
  of the critic's files, the original standing. It also edited four cells of
  its report in place (an em dash to `n/a`) with a dated note.
- **The ffi-pragmatist** left two check files in `/tmp`.

**Routes moved between seats.** The coordinator carried the compiler-engineer's
routes A and G (`<scratchpad>/190-shared/routes/`, exported at 09:43) and
route A-star (built by the coordinator from the engineer's diff, 13:02 to
13:08, its fixpoint by `cmp`) to the ffi-pragmatist. No seat read inside
another's copy.

## The proposal

Defect 231 (`adjacent` until this sitting): **every block that returns
releases every refcounted slot**, design.md Part 5's rule (`:2752` to
`:2758`), so a function of many returns and many slots grows its IR and its C
as their product. Measured on the trunk: 481,201 `decref_slot` in the IR at
N = 400; 5,815,408 lines of C at 800, each doubling ×3.85 to ×3.99.

**The proposal** (batch 8's emit lane): one exit block per function, every
`return` storing its value and jumping there, the copy-out and the sweep done
once.

**The routes to be priced, none presumed**:
- the shared exit, with or without a threshold;
- one sweep instruction per exit edge;
- a cleanup ladder;
- fewer slots;
- a sweep pruned by liveness;
- a C-only cleanup label;
- the product left as it is.

**Four questions**:
- **Q1**: soundness, on every shape of return;
- **Q2**: cost;
- **Q3**: the instruments;
- **Q4**: size, and the 800 shape at `-O2` and `--sanitize`.

## The verdict table

| seat | verdict | cost | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | **Q1 approve** A, A-borrow, A-star, A-star-2 and G (the per-block checks read one returning block unchanged; fifteen case programs byte-identical to the trunk at `-O0`, `-O2` and under ASan/UBSan); **Q2 object to route A** (no veto) and **adopt A-star**; Q3 approve; Q4 approve A-star's sizes as the repair of size only | A +52 code lines; **A-star +131** (`ir/exits.hero` 119), plus about 20 for a slot kind and the verifier at landing; G +119 | at A-star's landing `run` green, the fifteen cases identical; `emission` moves 86, `ir` 4, `emit` 2; the seed between 1,150,000 and 1,175,000 lines; ceilings at least 305 / 790 / 160 | a case diverging from the trunk; the ffi-pragmatist's Valgrind count moving under an A-family compiler; route A within 1% of the trunk's ceilings |
| ffi-pragmatist | **Q1 approve** the routes that release at the exit (A, G, A-star, the hand shapes); **veto** coalescing synthetic slots and releasing at last use; **Q2 approve with the price named** (moved from *object* by the Windows box); Q3 object to ASan or the leak gate as settling a route that moves a release; Q4 the premise *clang cannot build the 800 shape* holds on the box only, where every exit route removes the failure | clang's peak footprint at `-O2 -g` on the 800 shape a fifth to a third above the trunk's on Apple clang 21, every exit route alike; 12 to 13% below on the compiler's own seed (its hand shape) | **scored**: Q1 holds on A, G and A-star (`sqlite_lend.hero` prints the trunk's `21 -1 0 0 9010`, Valgrind 2 invalid reads in `libsqlite3.so.0.8` as the trunk, under clang 22.1.8 and 18.1.8); Q2 holds; Q4 half falsified, half right in substance | the veto lifts only behind panel 106's whole-function gate; the approval falls if a route puts any call but the runtime's between a return site and its sweep |

## What the sitting measured

**At the C boundary** (the ffi-pragmatist, sections 1 to 5 and 20 to 26):
- **Every route that releases at the exit releases where the trunk does.**
  This holds for the hand shared exit, the hand ladder and the block shape, and
  for the built routes A, G and A-star. On a C library that keeps the pointer
  it is handed, and on SQLite with a null destructor under a false `lent`,
  every column is the trunk's: plain, ASan, ASan's fill byte, Guard Malloc,
  `MALLOC_PERTURB_` and Valgrind. The platforms are this Mac, Linux arm64 under
  two clangs, and (the hand shapes and the keeper only) the Windows box.
- **Coalescing two synthetic slots and releasing at last use free a string
  the library still reads**: `10` for `9010` and `0 -1 0 0 0` for
  `21 -1 0 21 9010`, at exit 0, **ASan silent on all three platforms**. Only
  Guard Malloc, Valgrind or the fill byte see it, and none of them is in the
  net.

**Size and the compiler's own work** (the compiler-engineer, sections 3, 5, 8
and 10; the critic):

| | trunk | A | A-star | G |
|---|---|---|---|---|
| `slots-returns-400`, lines of C | 1,467,808 | 30,631 | 29,418 | 28,212 |
| `decref_slot` in the IR at 400 | 481,201 | 1,202 | 1,201 | 481,201 |
| the compiler's own memory on the 800 shape, `--dump-ir` (the critic) | 450,577,272 B | | **16,449,920 B** | 449,840,016 B |
| the seed, lines | 1,358,630 | 1,197,652 | **1,160,979** | 1,148,344 |
| clang `-O2 -g` on the seed, peak footprint (the critic) | 3,534,818,232 B | | 2,765,408,752 (−21.8%) | 2,765,277,680 (−21.8%) |
| the compiler's own tests | 1,158 passed | 7 failed (shape assertions) | 1 failed (one) | all passed |
| `ir` / `emit` / blessed emissions moved | | 23 / 7 / 359 | 4 / 2 / 86 | 0 / 1 / 68 |

**Depth** (panel 106's instrument; the interpreter's deepest nesting):
- under `-O0`, `-O2` and `--sanitize`, the trunk reads 315 / 836 / 166 and
  route A 295 / 703 / 158;
- **A-star reads 313 / 807 / 171** by the compiler-engineer's per-module
  build, and 313 / 795 / 171 by the ffi-pragmatist's fused file, against the
  same file's 829. So **−3.5% to −4.1% at `-O2`**, the two figures by two
  build methods (the critic's finding 2);
- G costs none.

Every exhaustion on every route is the stack guard's named abort, so §1.12
holds.

**Clang's memory on the extreme shape**:
- **On Apple clang 21**, every exit route needs a fifth to a third more peak
  footprint at `-O2 -g` on the 800 shape than the trunk. The two seats' trunk
  readings differ by 8%, more than the routes differ from each other, so this
  instrument cannot order A, G and A-star (the critic's finding 1).
- **On the Windows box** (clang 23.1.1, a Hyper-V machine whose memory grew
  from 1.2 to 8.6 GB during the run), the trunk's `heroes.exe build
  slots-returns-800.hero -O2` **ended at exit 2, *"clang died on the C"***.
  Windows' log reads clang at 9,540,055,040 bytes of virtual memory beside the
  compiler's 2,179,330,048. **Every exit route's 800 C built and ran there**,
  clang alone. The critic's finding 7: the comparison is of a whole build
  against clang alone, and the fair pair is owed.
- **A route nobody listed** (the critic, finding 5): `-gline-tables-only` for
  `-g` lowers the trunk's 400 C by 42.7% and puts A-star's below the trunk's.

## Disagreements, unsmoothed

1. **A-star or G.** The compiler-engineer recommends A-star, and records G as
   the conservative alternative. The ffi-pragmatist: *from the C alone, G is
   the cheaper route, while A-star's gains in the IR and the verifier are the
   compiler-engineer's to weigh*.
   - **A-star's price**: 3.5 to 4.1% of `-O2` depth, plus a new pass, a slot
     kind and verifier lines.
   - **G's price**: the IR and the compiler's own memory stay quadratic (450 MB
     at 800, the critic), plus two rule amendments: `.claude/rules/generated-c.md`'s
     one label per block, and panel 021's R1 and R8, the dump no longer
     showing the C's control flow.
   - **What separates them** is the compiler's own work on such a function,
     which A-star makes linear and G does not.
2. **What clang's memory says.** On Apple clang, every exit route costs
   memory on the extreme shape and saves it on the compiler's seed. On the
   box, the trunk does not build and the routes do. Which of the clangs, the
   targets (DWARF or CodeView) or the instruments reverses the order is unrun.
3. **What the critic found missing, and where each goes.**
   - **A-star's fallback**: the prototype leaves unmerged a function whose
     returns do not match its shape, and no function anywhere takes it. R3
     answers it.
   - **`check_return_type`**: it checks nothing on a merged function. R4
     answers it.
   - **The block shape and a slot kind**: neither was built. A-star with its
     copy-outs at each site was not built either. Each is unrun, and recorded.
   - **The landing predictions**: they are pinned to counts batch 9 moves. R12
     answers it.

## The resolution: `provisional — author ratification pending`

**R1. Route A-star.** A function with two or more ways out keeps its
returning blocks up to their copy-outs, each storing its value into the
function's return slot and jumping to one exit block. That block performs the
`@` copy-outs, loads the return slot, retains it by rule 4 and sweeps every
slot. A function with one way out is left exactly as lowering makes it (its IR
byte-identical to the trunk's, `cmp`). The merge is a pass of its own after
lowering, `selfhost/ir/exits.hero`; lowering, flattening, the emitter and the
verifier's per-block rules read its output unchanged.

**R2. The return slot borrows.**
- A store into it is a plain copy, and the sweep leaves it out.
- The exit's rule-4 retain gives the caller its reference before the sweep
  releases the value's real owner.
- The landing replaces the prototype's name test (`$ret0`) with a slot kind,
  taken by every `SlotKind` match.
- The verifier gains a check that a store into the return slot never takes an
  owning temporary.
- **Nothing but copy-outs stands between a return site's store and the exit's
  retain.** The compiler-engineer names a future instruction placed there as
  the one thing that would make the borrow wrong, so the verifier refuses it.

**R3. The merge is total or loud.** A function with two or more ways out
whose returning blocks do not have the shape the pass merges is a refusal of
the verifier, never a function silently left with the product. No program the
critic counted takes that path: 0 of the 390 corpus programs' functions, and 0
of the compiler's own (`<scratchpad>/190-critic/pass2/`). A refusal there is a
compiler bug named, which is the loud direction
(`.claude/rules/module-shape.md`).

**R4. The return-type check is restored over the exit.** `check_return_type`
reads the exit block's return on a merged function, where the prototype left
it checking nothing.

**R5. Coalescing synthetic slots and a sweep pruned by liveness are refused**,
on panel 106's ground, reaffirmed with the ffi-pragmatist's measured fault: a
C library reading a string freed early, at exit 0, with ASan silent on three
platforms. Neither returns without panel 106's whole-function gate and a run
golden per FFI shape.

**R6. design.md Part 5's sentence**, the compiler-engineer's wording (its
section 7):

> *Every way out of a function that has two or more, a `return`, the early
> return a `?` lowers to and the edge that falls off the end, stores what it
> returns into the function's return slot and jumps to the function's one exit
> block, which performs the `@` copy-out, retains what it returns and releases
> every local and synthetic slot. The return slot only borrows, so a store into
> it retains nothing and the exit does not release it. A function with one way
> out does all of this in the block that returns.*

**R7. The prices, named.**
- **Depth**: 3.5 to 4.1% of `-O2` depth on the interpreter, each overflow
  still the guard's named abort.
- **Clang's memory on the extreme shape**: a fifth to a third more peak
  footprint at `-O2 -g` on Apple clang, on a function of hundreds of returns
  over thousands of slots. Against it, on the compiler's own seed: 21.8% less
  at `-O2` and 14.5% fewer lines.
- **The landing's edits**: the four `ir` goldens edited by hand (109 lines
  out, 111 in), the two `emit` goldens, and the moved emissions, each read as
  the exit's shape alone. Also one own test's assertion.

**R8. The conservative alternative, recorded for the author: route G.** It
has no depth cost and its C is linear. Its IR and the compiler's own memory
stay quadratic, and it needs the two rule amendments of disagreement 1.

**R9. Defect 231 becomes `blocking`.** On the Windows box the trunk does not
build a correct program: the 800 shape exits 2, clang dead of memory, and
every exit route's C builds there. That is *a correct program refused* (§
Bounded discovery), the critic's question. So 231 is never deferred: it lands
in the next batch, on this provisional resolution, as defect 227 landed on
panel 189's.

**R10. What the landing owes before 231 closes.**
- **The golden cases the repair is pinned by**:
  - an `ir` golden of a merged function with an `@` copy-out and a `?`;
  - a `run` golden of a function of many returns, each return taken.
- **The fair Windows pair** (the critic's finding 7): A-star's own `heroes.exe
  build` of the 800 shape on the box, and clang alone on the trunk's 800 C
  there.
- **The compiler's own timing**, before and after, on a still machine.
- **The platform legs**, since the runtime's ABI does not move but every
  function's C does.

**R11. Filed and not filed, from beside the sitting.**
- **Filed**:
  - **314** (`improvement`): on the Windows box a doubled release runs on in
    silence without ASan;
  - **321** (`improvement`): no instrument in the net sees a C library read
    freed memory;
  - **322** (`improvement`): `-gline-tables-only` for `-g`, a question about
    the flag list for a sitting of its own.
- **Not filed, with its reason**: the checker accepts `str` and `@ str` as
  extern parameters, which spec § 13's field list does not name. design.md
  `:2626` to `:2637` (panel 021) makes the by-value `str` the FFI's guard,
  the compiler's own runtime group uses it four times, and a real header's
  `const char *` against a `str` parameter is refused at build,
  `error[ffi_parameter_type]` with the fix *declare it `cstr`*. The
  coordinator ran this on batch 9's round compiler at 09:44
  (`<scratchpad>/p190-beside/str-param/`).

**R12. The predictions are read against the landing's base.** The
compiler-engineer's counts (268 `run` cases, 86 of 359 emissions, 4 of 23 `ir`
goldens) are 703af779's. Batch 9 moved the base, so the landing reads its moves
against the trunk it lands on, each move read as the exit's shape or named.

## Predictions to score

- **The compiler-engineer's, at the landing**:
  - `run` green, and the fifteen Q1 cases identical to the trunk's;
  - the seed between 1,150,000 and 1,175,000 lines;
  - the interpreter's ceilings at least 305 / 790 / 160 by its per-module
    build, the build method named (the critic's finding 2);
  - `--dump-ir` of the 800 shape under a tenth of the trunk's user time.
- **The ffi-pragmatist's**, carried to the landing: Valgrind's count on
  `sqlite_lend.hero` stays the trunk's 2 under the landed compiler, on Linux
  arm64.
- **Unrun, the box**: A-star's own 800 C, and its whole `heroes.exe build`, at
  `-O2` there.

## The critic's passes

**First, over the briefs** (`completeness-critic-briefs.md`): the briefs'
framing facts re-run in its own copy. Among the repairs, the history of the
cleanup-label chain was added: promised from `8cacab7a`, never built, replaced
by panel 182.

**Second, over the reports** (`completeness-critic.md`): eleven findings, each
with the command that settles it. This synthesis answers them so:
- **1** (the 8% drift; the routes not ordered by memory): disagreement 2 and R7;
- **2** (two build methods for the ceilings): R7 and § Predictions;
- **3** (the hand shared exit is not the built G): § What the sitting
  measured, which names built routes apart from hand shapes;
- **4** (the seed's clang memory, measured: −21.8% for A-star and G): § What
  the sitting measured and R7;
- **5** (`-gline-tables-only`): R11, filed as 322;
- **6** (the compiler's own memory): § What the sitting measured;
- **7** (the Windows pair): R10;
- **8** (the block shape, A-star's copy-outs at each site, the slot kind's
  room): recorded unbuilt, the slot kind's room left to the landing's
  `layout`;
- **9** (the fallback): R3;
- **10** (four findings in no file): R4 and R11;
- **11** (counts pinned to a moved base): R12.

Its four questions: *231's class* is R9. *The golden case* is R10. *Which
compile pays*: a program the author builds per module at the level asked,
which no seat measured at `-O2` per module, so it is unrun. *The trade between
A-star and G* is disagreement 1 and R8.

## Author's verdict

**RATIFIED 2026-10-04**, on the author's answer to the recommendation put to
them that afternoon, in their words *"1a 2a 3a 4a 5a"*, of which *1a* is this
sitting's. **Recorded as a reading**, CLAUDE.md § 4's default; not `by
delegation`.

**What the yes settles**:
- R1 to R12 as the resolution above states them, route A-star over the
  conservative G, and 231's class;
- **R3 as lane b10-ir landed it** (`6e616898`): total or loud, save the one
  shape the verifier exempts by name. That shape is a program holding a hole
  `???`, whose function with a result keeps one valueless returning block,
  pinned by `tests/golden/ir/fixedbugs-231-a-hole-leaves-its-function-unmerged`.

The landing is batch 10's lane b10-ir, `075b425d` to `052f639a`.

**What it does not settle**: what only the landing's gate and legs measure,
R10's pair on the box, the timing, and the platform legs.
