# Panel 190, the completeness critic's second pass: the reports

Begun 2026-10-04 at 13:28:43 (read from `date`), by the completeness critic,
who gives no verdict. The first pass, over the briefs, is
`docs/panel/190-reports/completeness-critic-briefs.md`.

Read whole, as they stood at 13:24: `docs/panel/190-reports/compiler-engineer.md`
(869 lines, final at 13:01) and `docs/panel/190-reports/ffi-pragmatist.md`
(1,335 lines, final at about 13:23), and the repaired briefs
(`docs/panel/190-briefs/00-shared.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`). Where a seat corrected itself underneath, the
correction is read as the claim. The neutral folder
`<scratchpad>/190-shared/routes/{A,G,As}/` was read (its README files, the
build log of A-star, the three sha256 files); I run its binaries only from my
own copy. Nothing inside a seat's own copy was read or run.

My copy: `<scratchpad>/190-critic/` (the first pass's `git archive 703af779`
extract and its compiler built from the seed), and this pass's work goes in
its new subfolder `pass2/`. No paid run, nothing removed, the Windows box not
used, and Docker only after `docker ps -q` reads empty.

Written as I go; each finding names the command that settles it, run here or
named as unrun.

## 1. Two claims checked that hold

Written 13:31:56 (`date`).

- **"Two atomic refcount operations per call" (compiler-engineer, sections 6
  and 11) is literal.** The count is `typedef _Atomic int64_t HeroRefcount`
  (`runtime/heroes_runtime.h:125`), moved by `atomic_fetch_add_explicit` and
  `atomic_fetch_sub_explicit` (`runtime/parts/str.c:105-123`), read in my
  copy. The cost itself stays counted in the IR and untimed, as the seat says.
- **A-star merges every function the tree has that returns from two places;
  its fallback is taken by no program here.** A-star's pass merges a function
  only if every returning block ends in exactly its copy-outs (compiler-engineer
  section 8), and leaves any other "as lowering made it". Neither seat counted
  how often that happens. Counted with the exported A-star compiler
  (`<scratchpad>/190-shared/routes/As/heroes`, sha256 `d266f065f6054638`, run
  from my copy):
  - the compiler: in A-star's own seed (`As/s2.c`), 82 C functions still
    return from two or more places, **none** carrying a `#line` to a `.hero`
    file, so all 82 are emitter-written helpers; the trunk's seed has 1,595,
    of which 1,510 carry one (`pass2/returns_per_fn.py`, the C functions and
    their `return` statements);
  - the corpus: `--dump-ir` of 390 programs (`tests/golden/{emit,ir,run,fixedbugs}`
    and `examples/*/main.hero`, every one exit 0), every top-level unit
    counted apart (`pass2/ir_multi2.py`): 980 functions, 305 tests, 83
    constants; **0** with two or more `return` terminators under A-star,
    **159** functions under the trunk.

  So the fallback is a branch of a new pass that no program in the tree
  exercises. **The question nobody asked**: which shape takes it, and should
  such a shape be the verifier's refusal rather than a silent return of the
  product for that one function? Unrun; settled by writing the shape or by
  showing none exists.

## 2. Contradiction: clang's `-O2` memory on the 800 shape, and what the instrument can tell apart

Written 13:38:13 (`date`).

**The two seats disagree on the trunk's own reading of the same C.** The
compiler-engineer read the trunk's 800 C at **6,369,597,560** bytes of peak
footprint (its section 10), the ffi-pragmatist at **5,896,444,312** and,
four hours later, **5,804,874,136** (its sections 18 and 26), by one method
(clang alone, `-fintegrated-cc1`, `flags()` at `-O2`, `/usr/bin/time -l`'s
memory lines). The ffi-pragmatist names a suspect, unrun: the
compiler-engineer compiled the coordinator's `sr800.c`, whose `#line`
directives carry long absolute paths (450 MB against 224 MB).

**Run here at N = 400, where I hold the same C with both paths** (my
`out190/sr400.c`, 54,538,445 bytes, and `<scratchpad>/190-facts/sr400.c`,
112,564,154 bytes, `cmp` identical once the path is normalised), by the same
method (`pass2/fp.sh`: the sixteen words of `flags()` read from
`selfhost/cli/flags.hero:91-109`, `-O2`, `-I runtime`, one in-process
compile, bounded at 1,800 s by `perl -e alarm`, only the two memory lines
kept), one after the other:

| the trunk's 400 C | peak memory footprint | maximum resident set size |
|---|---|---|
| short path | 1,695,369,184 | 2,270,199,808 |
| long path (the coordinator's) | **1,669,548,024** (1.5% lower) | 2,312,011,776 |

So **path length does not raise the footprint at 400**, and the suspect does
not explain the half gigabyte at 800 (unrun at 800: an 800 compile needs
about 8 GB, which I did not take beside the lanes and the coordinator's
container). What it does tell:

- **The instrument's spread on one C is about 8% at 800** (6.37 against 5.80
  to 5.90 GB, the same lines modulo paths) and **2 to 5% at 400** (mine
  1.670 and 1.695 GB, the ffi-pragmatist's 1.617).
- **The three built routes are inside that spread of one another**: A, A-star
  and G read 7.85, 7.59 and 7.89 GB by the compiler-engineer, 7.87, 7.91 and
  7.56 GB by the ffi-pragmatist, the order flipping between the two runs.
  So the compiler-engineer's *"A-star least"* (its section 10) is not
  supported by its data; the ffi-pragmatist's *"not distinguishable from each
  other by this instrument"* (its section 26) is the reading the data allows.
- **Every route's excess over the trunk is real** (each route above every
  trunk reading by more than the spread), and its size is *"a fifth to a
  third"* only as a range: +19 to +24% against the higher trunk reading,
  +28 to +36% against the lower.

## 3. Contradiction: the interpreter's `-O2` ceilings differ by seat, and the method is why (an inference)

| `-O2` nesting ceiling | trunk | A | A-star | G |
|---|---|---|---|---|
| compiler-engineer (sections 6, 8) | 836 | 703 (−15.9%) | 807 (−3.5%) | 841 (+0.6%) |
| ffi-pragmatist (sections 14, 21, 26) | 829 | 694 (−16.3%) | 795 (−4.1%) | 829 (0%) |

The `-O0` and `--sanitize` rows agree cell for cell; at `-O2` every absolute
differs by 1 to 1.5% while the deltas agree. The two methods differ: the
compiler-engineer built each binary with the route's own `heroes build -O2`,
which compiles **one translation unit per module**
(`selfhost/cli/assemble.hero:158-218`, `per_module`); the ffi-pragmatist
compiled the **fused `--emit-c` artifact** with `cc.sh`
(`selfhost/cli/artifact.hero:94-100`: the artifact is the fused text). One
unit or eleven changes what clang may inline at `-O2`, and so the frames
that set the depth. **That is my reading, unrun**; it is settled by
`clang -fstack-usage -O2` over `examples/interpreter/syn/expr.hero`'s unit in
`build/tu-*/` after a `heroes build -O2`, against the same functions in the
fused artifact.

**Why it matters for a prediction**: the compiler-engineer predicts A-star's
landed ceilings at **at least 305 / 790 / 160**. By the fused method A-star
reads 795 at `-O2`, five levels above the bound; by `heroes build`, 807. The
prediction should name its build method, or it can be scored either way.

**And a statistic that hides the cost**: the compiler-engineer's Q2 argument
says A-star keeps *"frames within 0.31%"* (the `-O0` sum over every
function); the ffi-pragmatist's `-O2` sum even falls (89,008 to 87,040),
while the recursive parser's own frames grow (`h_synexpr_compared` 1,952 to
2,192, +12%), which is what costs the 3.5 to 4.1% of depth. The sum is not
the instrument for depth; the ceiling is, and both seats measured it.

## 4. A claim of the ffi-pragmatist's that the built route G falsifies

Its section 1 says **"The C-only cleanup label is this same C"** as its hand
shape, *"so every C measurement below is both routes'"*, and sections 1 to
19 label rows *"shared exit (= C-only label)"*. The route G that was built
keeps the copy-outs and the rule-4 retain in each returning block and shares
only the sweep (ffi-pragmatist section 20, compiler-engineer section 5);
the hand shape moves all three behind the label. Their figures, from the
two reports:

| | hand shape | built G |
|---|---|---|
| the seed, lines | 1,089,637 (ffi section 10) | 1,148,344 (both seats) |
| `slots-returns-800`, lines | 50,614 (ffi section 7) | 56,212 |
| interpreter `-O2` ceiling | 805 (ffi section 14) | 829 (ffi section 21) |
| clang footprint at 800 | +34.4% (ffi section 18) | +28.2% (ffi section 21) |

So every row labelled *"= C-only label"* measured the hand shape alone,
which is none of the built routes (closest to A-borrow; its script rewrote
2,777 of the seed's functions, ffi-pragmatist section 10). The boundary
items were re-run on G's own C (ffi sections 20, 21: every cell the trunk's),
so its Q1 verdict on G stands on G's C. **But the hand shape's seed figures
(19.8% fewer lines, 12 to 13% less clang memory, +0.36% `-O0` frames, the
fixpoint, 1,158 tests passing) belong to no built route**, and the label
stands uncorrected in the report.

## 5. Measured here: what each route does to clang on the compiler's own C

Written 13:44:44 (`date`).

**Both seats lean on a figure no instrument they trusted produced.** The
ffi-pragmatist's *"12 to 13% less on the compiler's own seed"* (its section
10, its Q2 verdict and section 27) was read with its three-second RSS poll,
which its own section 15 then rules out for differences of tens of percent
(*"not settled by this instrument"*), and on its hand shape, which is no
built route (section 4 above); section 18's footprint instrument never
re-measured the seed. The compiler-engineer's Q4 `condition` rests on it
(*"clang's memory on the compiler's own seed measuring higher under A-star
than under the trunk"*).

**Measured with the footprint instrument** (`pass2/fp.sh`, as in section 2;
the trunk's `seed/heroes.c` of my copy; A-star's own seed
`<scratchpad>/190-shared/routes/As/s2.c`, 1,160,979 lines, the coordinator's
fixpoint; route G's own seed emitted here by G's exported compiler
(`8538d840fe88716a`) from a fresh extract with G's `selfhost.diff` applied,
`pass2/treeG/`, 1,148,344 lines, the seats' figure; all three carry the same
relative paths). One compile at a time, each exit 0:

| the seed, clang `-O2`, `flags()` | peak memory footprint | against the trunk | maximum resident set size |
|---|---|---|---|
| trunk | 3,534,818,232 | | 3,541,991,424 |
| A-star | 2,765,408,752 | **−21.8%** | 3,168,862,208 |
| G | 2,765,277,680 | **−21.8%** | 3,158,147,072 |

And the line every new platform runs first, `clang -I runtime
seed/heroes.c runtime/runtime.c` (CLAUDE.md § Commands: no flags, so `-O0`
and no `-g`), its compile step alone (`pass2/fp0.sh`): trunk 494,600,960,
A-star 452,084,432 (−8.6%).

So **the compiler-engineer's Q4 condition is not met**: on the compiler's own
C, A-star lowers clang's `-O2` memory by a fifth, and G by the same fifth;
the instrument does not tell the two apart (131,072 bytes between them). The
seats' *"12 to 13%"* should be replaced by this, with its method. Route A's
seed is unmeasured here.

## 6. A route nobody listed: line tables without variable locations

The ffi-pragmatist found, as an experiment, that most of clang's `-O2` memory
on an exit-shaped function is the debug information `-g` asks for (its
sections 9, 10, 15), and measured `-gline-tables-only` only with the RSS poll
it later disqualified, which even read the trunk HIGHER with less debug
information. Neither seat turned it into a route. **Measured here with the
footprint instrument at 400** (`pass2/fpx.sh`, the sixteen words of `flags()`
with `-g` replaced by `-gline-tables-only`, `pass2/flags-glt.txt`; A-star's
400 C emitted here by its exported compiler, 29,418 lines and 2,403 releases,
the compiler-engineer's figures):

| clang `-O2`, N = 400 | `-g` (the compiler's words) | `-gline-tables-only` |
|---|---|---|
| trunk's C | 1,695,369,184 | **970,589,672** (−42.7%) |
| A-star's C | 1,985,595,312 (+17.1% on the trunk) | **913,409,056** (−5.9% on the trunk's, −54.0% on its own `-g`) |

At 400, with line tables only, **the regression every exit route pays
disappears** (A-star's C needs less than the trunk's), and the trunk itself
needs 43% less. At 800 unrun (about 8 GB a compile).

**Its price, read rather than run**: `flags.hero:17` gives `-g`'s only reason
as *"design.md §2 promises lldb steps .hero lines"*, which line tables keep.
What goes is C-level variable inspection, which design.md Part 2
(`docs/design/design.md:630-632`) describes rather than promises (*"No typed
variable inspection in v1 ... `p x` shows a mangled C temporary"*). It is a
change to the flag list (`selfhost/cli/flags.hero`, `.claude/rules/generated-c.md`
§ Flags) touching every program's debug information, so it is its own
question, not this sitting's resolution; the sitting should know it exists
before it names a third more clang memory as a route's price.

## 7. The compiler's own memory on the 800 shape: G keeps the product, A-star removes it

Written 13:46:44 (`date`). The compiler-engineer argues that route G *"leaves
the IR's product, the half of defect 231 that is the compiler's own time and
memory"* (its sections 5 and 7), measured in instruction counts, never in
memory or time. Memory can be read beside the lanes: `/usr/bin/time -l
<compiler> build shapes190/slots-returns-800.hero --dump-ir > /dev/null` (no
clang runs), bounded at 1,800 s, only the memory lines kept, each from my
copy, each exit 0:

| compiler | peak memory footprint | maximum resident set size |
|---|---|---|
| trunk (my copy's, from the seed) | 450,577,272 | 476,839,936 |
| route G (`8538d840fe88716a`) | 449,840,016 | 476,184,576 |
| route A-star (`d266f065f6054638`) | **16,449,920** (27 times less) | 20,938,752 |

So that half of the argument is now a measurement. It also bounds the
compiler's share of the Windows failure (next section): its IR passes hold
under half a gigabyte at 800, so most of the 2,179,330,048 bytes Windows
logged for `heroes.exe` belong to the build's later stages, the emitted C
above all; that last step is my reading, unrun (a `--emit-c` build's own
footprint was not taken, since it runs clang on the 800 C).

## 8. The Windows box: a verdict moved on two runs that are not alike

The ffi-pragmatist's Q2 moves from *object* to *approve* and its Q4 turns,
*"The box is why"* (its section 27): there *"the trunk's 800 shape does not
build at `-O2` and every exit route's does"*. Read in its section 24:

- the trunk's run was **`heroes.exe build slots-returns-800.hero -O2`**, the
  compiler and clang together (Windows' log: clang 9,540,055,040 bytes of
  virtual memory beside `heroes.exe`'s 2,179,330,048);
- each exit route's run was **`cc.sh -O2` on that route's 800 C**, clang
  alone, no compiler in memory.

Its own section 9 says the same mix on Linux *"compare[s] only loosely"*; on
the box, where the verdict moved, it is not said. **Two runs make it alike,
neither run** (the box is batch 9's until about 14:30): `cc.sh -O2` on the
trunk's own 800 C on the box (the routes' method), and one route's own
`heroes.exe build slots-returns-800.hero -O2` there, its compiler built on the
box from its own seed as was done on Linux (the trunk's method). Each is one
run of a machine whose memory grew from 1.2 to 8.6 GB during the sitting
(Hyper-V, its section 24), so whether the trunk's failure repeats is unrun
too. Section 7's reading suggests the direction will hold; it is not the run.

**The question it raises and nobody asked**: on that box a correct program
ends at exit 2 (*"clang died on the C instead of judging it"*). Defect 231
is filed `adjacent` (*"an 800-return function is no shape of panel 184's
R6, so no floor it sets is broken"*, its class line), and
`.claude/rules/verification.md` § Bounded discovery classes *"an exit 2 where
the author can be told"* and *"a correct program refused"* as `blocking`. Does
the box move the defect's class? A question for the coordinator, settled by
the like-for-like runs above.

## 9. Routes the sitting measured by hand and nobody priced in the compiler

- **The block** (every counted slot of a function in one array, the sweep one
  loop): the ffi-pragmatist's hand shape needs **80 to 85% less** clang
  footprint at `-O2` with `-g` (its section 18: 317,309,528 and 886,686,776
  bytes at 400 and 800), the only exit shape cheap inside clang under the
  compiler's own flags, and it found the block's boundary price (its section
  12: ASan stops naming a C overrun of an `@ str` argument and the leak gate
  blames the compiler). The compiler-engineer's row B prices *"G's C
  exactly"*, not the block. Unpriced: the emitter's change (every counted
  slot spelt as an element), its frames, and its run time, since a loop over
  the array with a variable index should keep clang from splitting it into
  registers at `-O2` (my reading, unrun). Unlisted: the block without any
  slot whose address reaches C, which would remove section 12's hazard.
- **A-star with the copy-outs left at each site.** The interpreter's six
  recursive parser functions each take two `@` records and return `i64?`
  (`examples/interpreter/syn/expr.hero:36-186`). A-star moves their copy-outs
  and rule 4's retain behind the join; A-star-2 moved only the retain back
  and recovered 12 of 29 `-O2` levels (compiler-engineer section 9); route G,
  which keeps both at the sites and joins in C, loses none. So the
  copy-outs' place is the untested half of the cause the compiler-engineer
  now calls unrun. Its price is known: left at the sites, the copy-out
  product stays (420 `copyout` at 20 by 21). Unbuilt; settled by building it
  and running `ceiling.py`'s search at `-O2`.
- **Line tables without variable locations** (section 6).

## 10. The landing A-star needs, and the room it lands in

The compiler-engineer prices A-star's landing at *"about 20 more"* lines
beyond the prototype (a slot kind, about 5 verifier lines, about 15 to keep
`check_return_type` checking) and concludes *"no ceiling is breached"*. The
slot kind's match sites, counted here (`grep -rn synthetic_slot selfhost/`
without constructions and comments, `pass2/slotkind-sites.txt`): **18 lines
in 15 files**, its count. Their room by my replication of `code_lines` (the
instrument settled two files in the first pass; these are unrun by it):

| file | room |
|---|---|
| `selfhost/emit/extern_probe.hero` (2 sites) | **3** (297 of 300) |
| `selfhost/ir/print.hero` | **3** (467 of its `DECIDED` 470) |
| `selfhost/ir/own.hero` | 4 after the prototype's 8 (296 of 300) |
| `selfhost/ir/build.hero` | 9 |
| `selfhost/ir/released.hero` | 14 after the prototype's 1 |
| `selfhost/emit/ops.hero` | 16 |
| `selfhost/ir/verify.hero` | 24, of which the two repairs take about 20 |

An arm added to an existing `|` group may cost no line; one that wraps costs
one. So *"no ceiling is breached"* is a prediction about an unbuilt landing
whose margins are three and four lines in four files, settled only by
building the slot kind and running `layout` on that tree. The report's own
sizes do not add up either: its section 7 prices the slot kind alone at
*"about 30"* lines, its Q2 the whole landing at *"about 20 more"*.

## 11. Found beside the sitting and routed nowhere

Written 13:47:46 (`date`). Each is real, measured or read by a seat, and in
no file a later session would open: the trunk's `docs/work/defects/` gained
285 to 295 since `703af779` (`git diff --name-status 703af779 HEAD --
docs/work/defects/`), all other lanes', and `grep -l -i` over the defect files
for these shapes finds none.

- **A `str` crosses into C as an extern parameter.** `heroes check` accepts
  `take(s: str)` and `take(@s: str)` in an extern group, and the second hands
  C the address of a counted slot (`t2 = take(&h0_v);`), while spec § 13's
  list of what a parameter may be names no `str` (ffi-pragmatist section 12,
  which calls it the spec-warden's and found no ruling).
- **On Windows a doubled release passes in silence without ASan**: exit 0,
  the right output, at `-O0` and `-O2`, where Darwin and Linux stop it with
  the runtime's mark panic (ffi-pragmatist section 5; its reading of
  `runtime/parts/str.c:76-86` for why is unrun as a cause). Every route's
  correctness argument leans on that mark.
- **A library reading freed memory is invisible to the whole net**, today, on
  the trunk: ASan reports nothing on every platform, glibc's plain run is
  right by accident, and only Guard Malloc, Valgrind or ASan's
  `free_fill_byte` see it, none of them in the net (ffi-pragmatist sections
  2, 3 and its Q3).
- **`check_return_type` stops checking on a merged function**
  (compiler-engineer section 4): the returned value is always the exit's load
  of the return slot, so the stores at the return sites are checked by
  nothing. Its repair (about fifteen lines) is unbuilt and has to ride
  A-star's landing.

## 12. Every prediction, as written and as scored by its seat

**compiler-engineer** (all four checkable only at A-star's landing):

- Q1: *`run` reads 268 passed and 0 failed*, and its fifteen programs stay
  identical to the trunk's. Q2: *`emission` moves exactly 86 of 359, `ir` 4 of
  23, `emit` 2 of 7, one test assertion, the seed between 1,150,000 and
  1,175,000 lines, the ceilings at least 305 / 790 / 160*. **Pinned to exact
  counts of a tree that will move**: batch 9 lands first, and any golden it
  adds moves 268, 359, 23 or 7 for reasons that are not the route's. Stated
  as moves against the trunk of the landing's day, they stay scorable. The
  ceilings need their build method (section 3).
- Q3: *`--dump-ir` of the 800 shape by A-star's compiler uses under a tenth
  of the trunk's user time*: owed, untimed. Its memory, which is not a
  duration, is section 7: 16,449,920 against 450,577,272 bytes.
- Q4: *`--emit-c` of `slots-returns-N` within 74N + 300 lines*: the prototype
  reads 7,518 to 58,618 against bounds of 7,700 to 59,500.
- Its Q4 `condition` (*the seed's clang memory higher under A-star than under
  the trunk*) is scored here: **not met**, 21.8% lower (section 5).

**ffi-pragmatist**:

- Q1 (*`sqlite_lend.hero` built by route A prints `21 -1 0 0 9010` here and
  Valgrind counts 2 on Linux arm64*): scored by its seat, **holds** on A, G
  and A-star (its sections 20, 21, 25, 26). Its second half, about a
  compiler that coalesces slots, cannot be scored: none was built.
- Q2 (*route A's own 800 C above the trunk's 5,896,444,312 bytes*):
  **holds** on both seats' runs (7.85 and 7.87 GB); the margin depends on
  which trunk reading it is set against (section 2).
- Q3 (*a coalescing compiler passes `run` whole*): unscorable, none built.
- Q4 (the box): its seat scores the second half **falsified** and the first
  **wrong in its letter, right in substance**; the substance rests on the
  runs section 8 calls unalike.

## 13. Questions the sitting should have asked and did not

1. **Does the Windows result move defect 231 from `adjacent` to `blocking`?**
   (Section 8.)
2. **What case pins the repair?** Neither seat names the golden or test that
   fails if the product comes back, and `.claude/rules/verification.md`
   § The batch asks *"the golden case per shape"* of every repair. The
   nearest shape is `own.hero`'s own test of 400 chained strings, which
   counts visits, not releases per return; an `ir` golden would be hand-edited
   forever. Unasked, unbuilt.
3. **Which compile is the program that pays?** Both seats measure the fused
   seed at `-O2` with `-g`, a compile nobody runs. The compiler builds itself
   one unit per module at `-O0` with `-g` (`selfhost/cli/verbs.hero:31`,
   `assemble.hero:158-218`), and the seed's own line takes no flags (section
   5 measures that one: −8.6% under A-star). The per-module footprint under
   each route is unmeasured.
4. **Which shape takes A-star's fallback, and should it be a refusal?**
   (Section 1.)
5. **The trade the synthesis has to state in numbers.** A-star: the IR and
   the compiler's own memory linear (27 times less at 800), the seed's clang
   memory a fifth lower, 3.5 to 4.1% of `-O2` depth lost, a slot kind landing
   in margins of three lines. G: no depth lost, the same fifth off the seed's
   clang memory, the compiler still holding 450 MB of IR at 800, two written
   rules amended. The compiler-engineer adopts A-star; the ffi-pragmatist
   writes that *"from the C alone, G is the cheaper route"* and leaves the IR
   to the compiler-engineer. Section 9's unbuilt variant is the run that
   says whether one route can have both.
6. **Is a third more clang memory a route's price, or `-g`'s?** (Section 6.)

## 14. For the coordinator

Written at the end of this pass. What does not hold, or is missing, each with
the command that settles it:

1. The seats disagree on the trunk's own clang footprint at 800 by about 8%;
   path length is ruled out at 400 (`pass2/fp.sh` on the same C with short
   and long paths: 1,695,369,184 and 1,669,548,024). The routes sit within
   that spread of one another, so *"A-star least"* does not hold (section 2).
2. The two seats' `-O2` ceilings differ by method, per-module `heroes build`
   against the fused artifact; my reading, settled by `clang -fstack-usage -O2`
   over the same parser function both ways; the prediction 305 / 790 / 160
   must name its method (section 3).
3. *"The C-only cleanup label is this same C"* (ffi-pragmatist section 1) is
   false of the built G; the hand shape's seed figures belong to no route
   (section 4).
4. *"12 to 13% less on the compiler's own seed"*: an RSS poll its own seat
   ruled out, on no built route. Measured: A-star and G both −21.8% at `-O2`
   (section 5).
5. Line tables without variable locations: a route nobody listed that takes
   the trunk's 400 C 43% lower and puts A-star's below the trunk's (section 6).
6. The compiler's own memory on the 800 shape: trunk 450,577,272, G
   449,840,016, A-star 16,449,920 bytes (section 7).
7. The box: the verdict that moved compared a whole build against clang
   alone; two runs make it alike, both owed to the box after batch 9's leg
   (section 8); and the class question it raises.
8. Unpriced: the block in the compiler, and A-star with its copy-outs at the
   sites (section 9). Unbuilt: the slot kind, landing in margins of three and
   four lines in four files (section 10).
9. Four findings beside the sitting are in no file (section 11).
10. The compiler-engineer's landing predictions are pinned to exact counts of
    a tree batch 9 will move (section 12).
