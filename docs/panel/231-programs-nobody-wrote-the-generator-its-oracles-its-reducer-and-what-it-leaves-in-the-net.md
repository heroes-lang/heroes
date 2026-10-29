# Panel 231 — programs nobody wrote: the generator, its oracles, its reducer and what it leaves in the net

**Convened 2026-10-28** in M-generated-programs (row 73), on tasks 931, 934, 972, 1234 and 2214,
after the catalogue of task 929 (measurement 056). **The soundness lane**, the compiler-engineer and
the ffi-pragmatist, by the author's yes of that day, asked once for the milestone (CLAUDE.md § 4):
the proposal is a program of the harness and changes no surface, no diagnostic and no spec token.
**One sitting for the whole milestone, and one pass of the completeness critic after the seats**, by
the author's instruction of the same day, meant as *optimise optimistically: aggregate defects,
aggregate panels, above all the very long operations; take them for granted, do one*. What the lane
gave up: no spec-warden priced a sentence and no blind seat read one, so everything below that would
move the specification is queued for a full sitting rather than adopted (§ What waits for a full
sitting). The briefs are `docs/panel/231-briefs/`, the reports `docs/panel/231-reports/`
(`compiler-engineer.md`, `ffi-pragmatist.md`, `critic.md`), the evidence `231-reports/evidence.tar.xz`.

## The proposal, as the seats received it

`docs/panel/231-briefs/00-shared.md` § The proposal, P1 to P8: a program of the harness,
`tests/harness/generate.hero` with the words `make`, `judge`, `reduce` and `hunt` (P1); generation
from the type with the value computed as the program is built, a clean arm and a declared arm whose
answer is the abort (P2); YARPGen's policies (P3); two dials, depth and the C boundary with a header
generated beside the program (P4); five oracles, the compiler does not fall over, the computed
answer in the net's configurations, `fmt` byte for byte, what `check` accepts `build` compiles, no
leak (P5); a reducer from `selfhost/mutate/sites.hero`'s primitives (P6); reduced witnesses and a
fixed sample in the net, the hunt outside it (P7); tasks 972, 1234 and 2214 settled, folded or
re-homed (P8).

## What was measured

Every number below is a seat's or the critic's, with its command in the report cited.

- **The prototype** (the compiler-engineer, Q3; reproduced by the critic, finding 1): seven modules
  under `tests/harness/`, 1,333 code lines; about 240 programs made a second and 3.6 judged a second
  four wide; **300 of 300 programs pass** oracles 1 to 3 at `-O0` and `-O2`, 95 of them declared
  aborts matched to their message; 30 of 30 pass the `run` form's five configurations in
  `tests/golden/run/`, at 2.05 s of wall a case against 2.10 s for the hand-written `abort-` cases.
- **The correlation the brief feared is total** (the compiler-engineer, Q1): a `>>` miscompilation
  planted in a copy of the compiler, a generator built by that compiler and judged by it, **0 of 300**
  failures; the trunk's generator judged by it, **36 of 300**, all at `-O0`; the three configurations
  agree with each other in the blind case. The contract test of `>>` catches it; that test was
  written knowing the plant.
- **A route nobody listed** (the critic, finding 1): stage 1, the compiler clang builds from
  `seed/heroes.c`, builds the generator byte for byte as stage 2 does on the trunk, and sees the
  plant's 36. It is in every gate already (`heroes-from-seed`, `.github/workflows/ci.yml:566`, the
  `test` job that runs the net at `:799`), and **the whole harness builds with it**, 17.09 s
  (`./heroes build tests/harness/main.hero`, the coordinator, 2026-10-28).
- **What the generator adds to the net was not measured by the seats** (the critic, finding 12):
  the `run` goldens see the seat's plant too (`bitwise`, `fixedbugs-577-...`). A narrower plant, `>>`
  arithmetic only for a count below 32: the 599 `run` goldens see it in **1**, the prototype in **0 of
  300**, and the prototype with shift counts drawn over 0..63 (five lines) in **61 of 300**, the
  trunk passing all 300. Its 300 programs held 182 literal shift counts in 0..31 and **none in
  32..63**.
- **The declared arm** (Q2): 29 programs, one per abort, each text copied from a run; `3 << 62`
  wraps at exit 0 by design.md § 4.14 (`docs/design.md:2071`) where spec § 7's *Overflow aborts at
  every width* reads otherwise; recursion with no base path is refused at build
  (`endless_recursion`), and recursion too deep aborts with no `at` line and at a depth the stack
  decides.
- **The C boundary** (the ffi-pragmatist, Q1 to Q3): six hand-written bindings in the generator's
  shape, a struct by value with a nested struct and an `i32[3]`, out-parameters and `counted_by`,
  three callbacks, `cstr lent` in and an `owned` result, a handle with `acquires` and `consumes`, a
  union with `partial` and `rest: zero`, **every one printing its computed answer in all five
  configurations**, one module and two. **Undefined behaviour in a header voids the oracle**: an
  `i32` printed `6157951600` under `--sanitize`; an uninitialised heap read is silent under every
  sanitiser and shows only as legs disagreeing, and two malloc fills (0 and 190) expose it; `a * b +
  c` in one C expression is fused on arm64 and not on x86-64 (read from the instructions, unrun on
  x86-64). `heroes check` never reads a group's header (six reruns, the critic's finding 3), as
  `docs/roadmap/milestones/M-check-completeness.md:18` and `:50` already recorded.
- **Depth** (Q5): `heroes build` exits 134 on valid programs at a 10,000-term sum, 20,000
  parentheses and 5,000 nested calls (`stack exhausted in flatten.expr`, `lower.shape`); panel 184's
  R6 floor is in no sentence of the spec, and **N = 2,000 holds on all four CI legs** for brackets,
  chains and one-operand runs: `run/fixedbugs-169-*-2000-*` pass in run 40474270628 on Darwin arm64,
  Linux arm64, Linux x86-64 and Windows x86-64, which skips 26 cases, none of them (`gh run view
  40474270628 --log`, the coordinator); nested calls at 2,000 have no committed case.
- **The reducer** (Q6): over the generator's tree, the 36 failures went from a median 1,448.5 bytes
  to 215.5, 838 candidates judged and none refused, one compiler; a text reducer reached a median
  1,338 and needed two compilers, 596 of its 892 single-statement deletions not checking;
  `selfhost/mutate/sites.hero` cannot be used from `tests/harness/` at all (`unknown_module`,
  `module_path_climbs`).
- **Leaks** (Q4, Q6, the critic's findings 4 and 9): task 972's elision holds on this Mac, read from
  the binary; **`leaks --atExit` on the `-O0` binary sees a C leak here**, 200 leaks and 6,400 bytes
  at `-O0`, 0 at `-O2`, one 21-byte leak, a `-> cstr` without `owned`; 0 leaks over 61 committed
  `run` cases and seven generated programs. `.claude/rules/c-boundary.md:179-180` and
  `.claude/rules/platforms.md:120-121` say the opposite of this Mac.
- **Task 1234** (Q5, the critic's finding 7): 692 of 705 blessed emissions pass `-fsyntax-only`, 703
  with pkg-config's flags, the 2 left needing a Linux header; 685 link, the rest IR goldens with no
  `main`, the Linux pair and **`fixedbugs/ffi-missing-link`, which passes `-fsyntax-only` and fails
  at the link**, the task's one live witness; the two routes agree on all 590 run programs.
- **Task 2214** (Q8, the critic's finding 10): the compiler instrumented with clang's coverage over
  its emitted C, 709 `check` cases run in 7.87 s; 373 cover a counter no other covers and 336 cover
  none, 257 of them `fixedbugs-`; of those 336, 2 pin a diagnostic code no other case pins.
- **Coverage as a compass** (Q8): the generator reaches 26,751 counters and the `run` goldens 56,943,
  63 the generator's alone, yet the generator found the plant, which lives in values on code the net
  already reaches.

## The verdict table

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **no veto**: approve P1, P2 with three amendments, P3 scoped, P7, P8 re-homed; object P4 (depth in the net), P5 (oracles 1 and 3 as worded), P6 (*from `sites.hero`*) | design.md § 1.1, § 1.7, § 1.12, § 4.14, § 4.15 | 0 lines in `selfhost/`; about 3,100 to 3,700 in `tests/harness/`, 1,333 built | the core in at most 2,500 code lines and 10 modules; 9 of 10 operator plants caught; witnesses under 20 lines; `generated` at N = 128 adds no minute to gate B | a need to reach the compiler's parser; a miscompilation both the grid and a base-built generator miss |
| **ffi-pragmatist** | **object, no veto**: approve Q1, Q3, Q5 (re-home 1234), Q6; object Q2 (*UBSan is the check*), Q4 (972 *settled by defect 390*) | design.md § 1.11, § 1.12, § 4.19 | the C arm 500 to 800 lines (the engineer's estimate) | the first 1,000 C-boundary programs that build produce an artifact that links and agrees with `build`; `ffi-missing-link` stays the only shape where the routes disagree | a header fault the C driver passes and the program shows |
| **critic** | (no verdict) 18 findings: the stage-1 route, plant B2 and operand domains, the reducer's symptom, the declared arm's place, the CI's Windows leg, a committed digest of the sample's answers | — | — | — | — |

## Disagreements, stated plainly

- **Oracle 4 at the C boundary.** The compiler-engineer: *oracles 2, 4 and 5 hold as written*. The
  ffi-pragmatist: oracle 4 is false by design there, since `check` never reads the header.
  Checkable, and the ffi side holds (six reruns; M-check-completeness's own record). The engineer's
  prototype writes no `extern`, so its *holds* is true off the boundary only.
- **Task 972 on this Mac.** The compiler-engineer: *cannot be measured on this Mac*. The
  ffi-pragmatist measured it, by `otool` and by `leaks --atExit`. The engineer measured ASan's leak
  detector, not the Mac; the ffi side holds.
- **Task 1234's re-home.** The engineer: `-fsyntax-only` in `emission` closes it. The ffi-pragmatist:
  that and the link with both routes in the generator's oracle 4. The one live witness passes
  `-fsyntax-only` and fails at the link, so the engineer's re-home closes the task with its witness
  unseen; the ffi side holds.
- **Task 2214.** The engineer closes it on *coverage cannot read text*; the critic: the task asks
  for exactly that text to be measured, and the code-level cut is one command. The critic holds.
- **The net's generator.** The engineer leaves the `generated` suite blind to Q1's class, guarded by
  the contract tests alone; the critic found stage 1 in every gate. Measured, the critic holds.

## The resolution adopted, provisionally

*Provisional — author ratification pending*, the decision issue named at the foot. The most robust
and complete reading of each line, never the cheapest; what conservative would have been follows.

**R1. Where it lives and what it is.** A program of the harness, `tests/harness/generate.hero` and
its modules `gen_*.hero`, run as the net is, proposing no verb (CLAUDE.md § 10), adding no line to
`selfhost/`, and **using no module of `selfhost/`**, measured impossible and kept as a property: its
printer and its evaluator are a second implementation of the surface and the semantics, which is
what lets oracles 2 and 3 compare two things rather than one thing with itself. Words: `make`,
`judge`, `reduce`, `hunt`. **The judge is Heroes** (the prototype's was a shell script in a seat's
scratch). **The generator joins the tools a new surface form must reach**
(`.claude/rules/diagnostics-and-goldens.md` § A new surface form lands in every tool), with its test:
the form generated, judged, and its answer checked.

**R2. The value as it is built, and two arms, made independent of the compiler judged.**
- **The net's harness is built by stage 1 and judges stage 2**: the CI's net step runs
  `./heroes-from-seed run tests/harness/main.hero -- ./heroes` (`ci.yml:799`, one word), and a gate
  here runs `./heroes run tests/harness/main.hero -- ./heroes-next` with `./heroes` the seed's. A
  miscompilation introduced since the seed was last refreshed is then computed by a generator that
  does not share it, in the net, at no cost; its price is that the harness may use only forms the
  seed knows, the rule `selfhost/` already lives under.
- **The hunt is built by a round's base and judges the round's compiler** (`--compiler` names the
  judged one), Q1's measured route.
- **Every operator against the contract**: a grid of every integer operator over every pair in -8
  to 7 against references computed through other operators (`>>` as a floor division, `%` as `a - a
  / b * b`), in `gen_eval`'s tests, which the net's own tests run with the round's compiler.
- **Narrow widths are computed in `i64`** and range-checked, `u64` past `i64` in two halves, never
  in their own type (defect 567's shape).
- **The semantics are design.md's**, `<<` wrapping (§ 4.14), and every abort text in the declared
  arm is copied from a run, never from `runtime/` (the `%` and shift texts are the emitter's).
- **Recursion depth stays out of the declared arm**; recursion in the clean arm has a base path and a
  depth far under the floor.

**R3. Operand domains, forms, and a yield measured against the net.** **Each operator draws its
operands from its own domain** (the critic's plant B2): a shift's count over 0..63, a divisor near
0 and -1, an index near its array's ends, a width's operands near its own edges, beside the shared
constants mix. **The milestone's yield is measured as a plant set**: one planted miscompilation per
emitter rule and operand class, each built into a compiler of its own, each scored against the
`run` form and against the generator built by stage 1, so a prediction about what the generator
finds is judged against what the net already found, and N is sized by the generator's margin over
it. The four policies built (operator contexts, the constants mix, the subexpression buffer, depth
and leaf share per program) stand; YARPGen's further parameters wait until the forms are in: strings and slices, arrays, maps, `T?`, floats, generics across `use` lines,
widths, the C boundary. **The coverage instrument of Q8 is the compass**: run once per step, the
counters the `run` goldens reach and the generator does not name the forms to teach it next.

**R4. The two dials.**
- **Depth.** In the net, at most 64 for an expression and 16 for statements, and every line under
  120 columns; in the hunt, up to and past 2,000 on purpose. **Panel 184's R6, ratified and never
  landed, is this milestone's to finish**: N = 2,000 is measured on the four CI legs for brackets,
  chains and one-operand runs; the generator's depth dial commits the shapes it lacks (nested calls,
  nested `if`, nested records and variants) at 2,000 as `run` cases, the CI judges them on four
  legs, and R6's sentence lands as R6 and R8 wrote it, priced by `heroes measure --refresh` in the
  landing's tree (panel 184 is the ratification; the sentence's words are R6's own).
- **The C boundary**, the first arm after the core, since 26 of the 38 falls and 19 of the 30 silent
  defects of the catalogue are there (056 § 2.3). The generator writes an `extern` group and a
  header together, and **proves the header before the compiler sees it**: a C driver calling each
  function at the chosen operands, built `clang -std=gnu11 -fsigned-char -fsanitize=undefined
  -fno-sanitize-recover=all`, its output the `.expected`'s C half, so a header's fault is never
  charged to the compiler. The header's C keeps to the ffi-pragmatist's list (Q2): fixed widths,
  `size_t`, `double`, signed `char`; unsigned arithmetic modulo 2^N, never on `uint8_t` or `uint16_t`
  without widening first; signed arithmetic only where the tracked value is in range; shifts within
  the width and on unsigned; narrowing only in range; **one float operation per statement**; no
  `<math.h>` but `sqrt`; every byte written before it is read. And the generator holds the refusals
  the hand writing met: labels on two parameters of one type, a character above ASCII written as
  itself, an `extern` never called across a module (a Heroes wrapper beside it), a handle ended
  through a wrapper taken `@a` and passed with no `@`, no C name that is a reserved word, **and no
  `==` on a handle's copy made before it was given back written as an abort** until that is ruled
  (§ What waits for a full sitting).

**R5. The five oracles, worded so they cannot lie.**
1. **The compiler's answer is read in five**: exit 0, judged by the oracles after it; exit 1 on a
   generated program, a correct program refused, a defect (and `check` asked, to name the stage);
   exit 2 saying *clang died on the C*, panel 184's R7, recorded and not a defect by itself; exit 134
   saying *stack exhausted in*, R6's limit, a defect below the floor once the floor is stated;
   anything else, an `internal error`, a `hero_unreachable`, a signal or a timeout, a defect.
2. **The computed answer, in three built configurations**, `-O0`, `-O2` and `--sanitize -O0`, each
   built with `build -o` and its binary run under a time bound and a byte ceiling (the ceiling on the
   program and never on the compiler: under `run` it held the compiler's instrumented runtime too, 40
   of 40 programs read as *the runtime did not compile*, the coordinator, 2026-10-28); a sanitiser's
   word is a defect; **a program with an `extern` runs its sanitised leg twice, under the malloc
   fills 0 and 190**, and the two must agree. **A declared abort is held to its place as well as its
   message** (the critic, finding 14): the judge compares the report's `at <file>:<line>:<col>` with
   the planted operation's, which the generator knows; and a committed declared witness pins its
   place too, the run form's expectation gaining one optional line, `!at: <file>:<line>:<col>`,
   before its ending, landed with the first such witness. **Printing is bounded by construction**:
   a generated program prints only at `main`'s top level, never in a loop, a function or an arm, so
   its output is bounded by its own text whatever a compiler does to its loops, and the time bound
   alone holds it on Windows, where no byte ceiling exists (the critic, finding 15).
3. **`fmt`'s output is a fixed point and means what the program meant**; bytes equal where the
   generator wrote the canonical form, and the generator learns the canonical form (the 120-column
   wrap, arms padded to one column of `=>`, `--8` for `-(-8)`) rather than the oracle loosening.
4. **Off the C boundary, what `check` accepts `build` compiles** (the judge's own build). **At the
   boundary, M-check-completeness's carve-out** (`docs/roadmap/milestones/M-check-completeness.md:18`,
   `:50`): what `build` builds, the artifact `--emit-c` wrote compiles, links and runs to the same
   exit, stdout and stderr.
5. **No leak.** On every leg and in every configuration, the runtime's count is zero at exit: no
   value, handle, lease or lent region the program held is left. A byte C allocated and the program
   lost is seen on Linux by LeakSanitizer under `--sanitize -O0`, on Darwin by `leaks --atExit` on
   the `-O0` binary, at `-O2` only where clang kept the allocation, and on Windows by nothing; a
   witness names the leg that judges it.

**R6. The reducer works over the generator's tree**, the two edits P6 names applied to it (drop a
statement, every later read of what it bound replaced by its value; replace an expression by a
literal of its value), with the two guards running it found owed: a block is never left empty, and
a candidate printed as the current best is no progress. **It keeps a candidate while the SAME symptom
stays** (the critic, finding 13): for oracle 1, the exit and the text that decided the reading; for
oracle 2, the configuration, the first differing output line and the operator at the root of the
expression that printed it. A reduction under a compiler holding two plants is run before the first
witness is committed, so a slide from one defect to another is measured rather than assumed absent.
A program the generator did not write is reduced over the tokens `heroes lex --dump-tokens --json`
prints, the compiler's own lexer reached as a child (the critic, finding 17), never by reaching into
`selfhost/`.

**R7. What enters the net.**
- **Reduced witnesses are ordinary `tests/golden/run/` cases**, `fixedbugs-<defect>-<slug>`, their
  seed and the generator's commit in the header comment, judged by that form's five configurations
  and blessed in `emission` as every case is.
- **The sample is a suite of its own, `generated`**: the contract grid, then seeds 1 to 128
  regenerated into its scratch and judged, beside `run` in gate B; deterministic under its seeds, so
  it goes red when the compiler or the generator changes and never at random. **Built and run**: 128
  passed and 0 failed, the harness built by stage 1 judging stage 2 (`real 275.83`, `user 111.05`,
  `sys 36.98`, beside the critic, so an indication). **Its answers are committed, its programs not**
  (the critic, finding 18): one file of the sample's expected outputs, a few kilobytes and no
  emission to bless, against which every leg holds both the generator's answer and the program's,
  so the four legs are a differential for the generator's own arithmetic and the correlation of
  finding 1 is broken on every leg at once; it is re-made by a word of `generate.hero` whose output
  a person reads before it is committed, never by a regenerator in a suite. **Its cost on the CI's
  Windows leg is the first batch's measurement** (the critic, finding 16: the net there ran 121 of
  that job's 135 minutes against a ceiling of 240): N halves if that leg passes 200 minutes. N = 256
  if the first batch gates show room.
- **The hunt stays outside the net**, on tags and by hand, writing only what failed.

**R8. The three tasks, each with a home in this milestone.**
- **972**: settled for the `run` form by defect 390 on Linux (an inference until a Linux leg reads a
  leak, the ffi-pragmatist's own mark); **the corpus gains its fourth configuration, `--sanitize
  -O0`**, for the 20 example directories that declare an `extern`, in this milestone, as 972's
  comment of 2026-10-25 kept it here for; oracle 5's sentence above is its generator half; the two
  rule files' sentences are corrected in this milestone, a record and not a language change; the
  first verdict on Linux is the CI's after the push (§ The optimistic chain).
- **1234, two halves apart**: `-fsyntax-only` over every blessed emission joins `emission`, with the
  packages' include paths and the platform's skip (about 7 s four wide); the link and the two routes
  join the generator's oracle 4 at the boundary. 1234 closes when both have landed.
- **2214**: a `docs/measurements/` file, the criterion written before any case is touched: a
  `fixedbugs-` case keeps its place by rule (`.claude/rules/diagnostics-and-goldens.md`, a fixed
  defect keeps its case: 257 of the 336); the other 79 are judged by counters, codes and message
  templates together, each proposed removal reviewed by an attempt to refute it; no case is removed
  in this milestone without that file.

## What waits for a full sitting

Three questions about what the specification says, which this lane cannot price or read blind, put
together so that one sitting answers all three (the author's instruction: aggregate panels), each
with the default the compiler runs on today:

1. **R6's floor sentence** is the landing of a ratified ruling (panel 184), priced in its landing's
   tree; it is listed here only because its words enter § 9 and the warden prices them there.
2. **`<<` past the sign bit wraps** (design.md § 4.14) while spec § 7 says *Overflow aborts at every
   width*, a reader taking `3 << 62` for an abort (the compiler-engineer's correction 5). Default:
   the wrap, design.md's.
3. **`==` on a handle's copy made before it was given back** answers by the address at exit 0
   (`hero_handle_eq`, `runtime/parts/alloc.c:903`; the golden
   `run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle` pins `null: false`),
   while spec § 13 says *== on it aborts, and so does a copy made before*, written in the commit that
   blessed that golden (`c8b1bc878`, the critic's finding 2). Default: no abort, design.md Part 8
   wart 20's reading (*a copy made before the call is refused at its first crossing*).

## What the conservative resolution would have been, so the author can choose it

P1 to P8 as drafted: the generator built by the compiler it judges in the net, guarded by contract
tests alone; oracle 1 as *no exit outside 0 and 1* and oracle 3 as bytes; oracle 4 one sentence for
every program; a text reducer from `sites.hero`; 972 closed on defect 390, 1234 closed on
`-fsyntax-only`, 2214 closed on *coverage cannot read text*; R6 left unwritten with no owner. Each of
those is measured wrong or blind above.

## Predictions to score

| seat | prediction | scored |
|---|---|---|
| compiler-engineer | the core (the forms built, widths, grid, wrap rule, `judge`, `reduce`, `hunt`, `generated`) holds at most 2,500 code lines in at most 10 modules, none past 300; `git diff --stat` of `selfhost/` over the milestone shows only defect repairs | task 934's close |
| compiler-engineer | a plant in any one of the ten integer operators' emission rules, built into a compiler B, is reported by the contract grid under B or by oracle 2 with the generator built by the base, for at least 9 of the 10 | task 931's close |
| compiler-engineer | after the C boundary arm and strings land, the generator's counters not reached by the `run` goldens exceed 1,000, against 63 | that landing |
| compiler-engineer | the first hunt whose depth dial reaches 5,000 nested calls reports `stack exhausted in lower.shape` on this Mac within its first 100 deep programs | the first deep hunt |
| compiler-engineer | with oracle 1 as written, the first hunt reaching clang's limits flags at least one exit 2 panel 184's R7 designed | **void**: R5 rewords oracle 1 |
| compiler-engineer | at least 90 per cent of reduced witnesses are under 20 lines, and no reduction judges a refused candidate | task 934's close |
| compiler-engineer | `generated` at N = 128 ends before `run` does in gate B and adds no minute to its wall | the first batch gate after it lands |
| compiler-engineer | `emission` with `-fsyntax-only` and the packages' paths reads 0 refusals on this Mac's trunk | that landing |
| ffi-pragmatist | the first 1,000 generated C-boundary programs whose `build` exits 0 produce an artifact that links and agrees with `build`; `ffi-missing-link` stays the only shape where the routes disagree | the C arm's first 1,000 |

## Scored at M-generated-programs's close, measured 2026-10-28 and written 2026-10-29

Each row of the table above, scored by the command that settles it, run that day on the lane at
`dbae1e642` or after it as each line says, by the coordinator.

- **The core's size: missed.** The generator's modules, `tests/harness/gen_*.hero` with
  `generate.hero` and `suite_generated.hero`, hold **3,704** code lines in **26** modules, none past
  300 (counted as `suite_layout` counts, a module's code without its comments, its `test` blocks or
  its blank lines), against 2,500 in 10; the milestone built the page's two hard shapes as eight
  families of their own (decision 2248), which the prediction did not price. Its second half holds:
  `git diff --stat 8830e6621 dbae1e642 -- selfhost/` is one file, `selfhost/measure/pinned.hero`,
  the specification's pins, and no defect needed a repair in `selfhost/`.
- **Nine of ten operators: hit, on the nine planted.** Measurement 058 planted nine of the ten
  integer operators' rules, `+ - * / % >> & ^ <<`, and the generator's sample of 128, built by the
  base, saw all nine (E1 to E9, 9 to 28 programs each); `|` was not planted, so the tenth is unscored.
- **The generator's counters past the `run` goldens: missed, 362 against 1,000.** The seat's own instrument re-run as the seat ran it (its instrumented compiler, `sources 5e0ca4c634086a7c`, functionally today's, `selfhost/` having moved since only in the specification's pins; one profile a program through `--emit-c`; its `counters.py`): the sample of 128 with every family reaches **38,857** counters, the `run` goldens' set of the sitting **56,943**, and **362** are the generator's alone, where the sitting's 300 programs reached 26,751 and **63**, a number the same commands give again from the sitting's own set. The C families and the depth shapes grew it nearly six times, and the prediction's thousand stays unreached.
- **5,000 nested calls: hit on the limit, missed on its place.** The hunt's depth dial, `generate deep` (panel 231's R4, *in the hunt, up to and past 2,000 on purpose*, built at this close), wrote `g(g(...g(0)...))` 5,000 deep, and its first program's build ended `panic: stack exhausted in walk.check, inside the recursion of walk.check, walk.dispatch_call, walk.named_call and walk.user_call`: the designed limit of panel 184's R6, past the floor, in the checker's walk rather than in `lower.shape`, the stage after it. The same dial at 5,000 built and ran a chain of records and one of variants, and `if` and `while` blocks at 2,500, each printing its answer.
- **Oracle 1's exit 2: void**, as the table says.
- **Reduced witnesses under 20 lines: hit.** The 68 reductions of step 3's experiment under two
  plants (`.claude/worktrees/lane-generated-programs-scratch/slide/` on the coordinator's Mac): 67 of
  68 under 20 lines counting every line, 68 of 68 counting the lines that are not blank, from 4 to 22,
  median 8; and **0** refused candidates judged in all 68 (`refused 0` in every `reduce.log`).
- **`generated` ends before `run` in gate B: hit.** At the close's gate, six suites wide beside two hunts, `run` took from 20:28:15 to 21:07:04 and the eleven parts `generated` runs in had all ended by 20:34:13; alone on a still machine, before the C families, `generated` read `real 326.26`, `user 116.23`, `sys 49.27` (2026-10-28), so it adds no minute to a wall `run` sets. On the Windows box, two sessions wide, its parts took 7 min 17 s of wall and about 12 min 40 s in all (23:31:34 to 23:38:51), far under R7's 200.
- **`emission` with `-fsyntax-only` reads 0 refusals: hit**, at step 4 (`a21ef026e`): 2,118 and 0
  on this Mac, the two Linux-only cases skipped for `sys/prctl.h`; and at this close's gate,
  2,118 and 0 again, the same skips.
- **The first 1,000 C-boundary programs whose `build` exits 0: hit.** The close's hunt judged 3,200 seeds from 50,001 (eight processes of 400, two of them run again after the generator's own defect of step 17), 1,157 of them binding C, every one through oracle 4 at the boundary, and 0 failed: no artifact that does not link or disagrees with `build`, so `ffi-missing-link` stays the one shape where the routes disagree.

## What the seats could not run

Linux (the Docker daemon down; no seat started it on a shared machine): LeakSanitizer on 972's shape,
the contraction on x86-64, the leak legs' sentences; the Windows box; `leaks` on the CI's `macos-26`
runner; the whole `run` form timed on a still machine. Each is written where it is owed.

## Found beside the sitting

- **`consumed_borrowed_handle`'s note** asks for `@` *at the call site* while pointing at a call where
  `@` is `marker_mismatch` (`selfhost/check/consuming.hero:154`; the ffi-pragmatist, reproduced by the
  critic, finding 5): filed `adjacent`, defect 2242 (https://github.com/heroes-lang/heroes/issues/2242).
- **Heroes' own `a * b + c` is unfused** on every leg because the emitter writes three-address C,
  a property no rule states (the ffi-pragmatist, Q2): pinned by a `run` case when floats enter the
  generator (R3), so a change of the IR's shape that fused it would be seen.
- **The milestone page's oracle 1 says *no 134*** (`docs/roadmap/milestones/M-generated-programs.md`,
  the oracles paragraph of 2026-09-06), which panel 184's R6 overtook on 2026-10-01: corrected
  beneath it with this sitting's reading.
- **`__int128` and `__builtin_mul_overflow` compile for the CI's four triples** (the critic, finding
  17), so route 4 of Q1, a C helper, stays open if the grid and stage 1 are ever measured short;
  128-bit division and a run on the box are unrun.
- **The shared brief's corrections**, five, each measured by a seat: the 300-line ceiling is a
  convention in the harness, which `suite_layout` excludes (33 harness modules pass it);
  `docs/design.md:809` is QBE's description, not this compiler's; oracle 1's wording; 1234's
  premise; the spec's silence on `<<`.

## Process, recorded against this sitting

The critic ran 18 findings, the last seven after it read the compiler-engineer's finished report.
The briefs went out at `bd0c4477b`, each seat in its own detached worktree with its compiler built
there (`sources 5e0ca4c634086a7c`). **The critic was launched before the compiler-engineer had
finished**, on its finished sections, and read the report again before its last findings (the
author's instruction to optimise). **The Mac slept from 13:05 to 13:51**, its lid closed by the
author, so the compiler-engineer's reductions read 58 minutes of wall for about 13 of work; nothing
was lost. **The ffi-pragmatist wrote one file outside the repository**, `/tmp/x`, and removed it in
its next command (its report says so). The seats' prototype was carried into the milestone's lane
while the critic worked, on a tree the sitting does not read.

## Added after the critic's last pass, 2026-10-28

**The critic's report was committed at `5cd38dbb9` while it was still being written**, its header
saying *a section marked in progress is unfinished*: that commit holds eighteen findings, two of
them drafts. Its final version, committed beneath this section, holds **nineteen**, and two of its
facts move the sitting's reading without moving a resolution:

- **Gate A already sees both plants** (finding 12, final): the compiler's own tests, built by the
  planted compiler, read `1763 tests, 1 failed` under B and under B2 alike, at
  `check/const_values.hero:369` (*`shown(bitwise(.shr, x: lowest, y: lit(negative: false,
  magnitude: 63)).value) == "-1"`*), the checker's constant evaluator being a second Heroes model of
  the operators with its own contract test; **the fixpoint does not** (B's and its rebuilt
  compiler's emissions of the planted source are the same bytes). So the plant set of R3 is scored
  against gate A, the `run` form and the generator built by stage 1 together, and the generator's
  yield is its margin over the first two, which neither sitting nor seat has yet measured on a plant
  both of them miss.
- **The verdict table's critic row said *18 findings***: it is 19; the nineteenth corrects two
  numbers of the compiler-engineer's, neither of which sizes anything.

## Author's verdict

*Pending*: https://github.com/heroes-lang/heroes/issues/2243.

**Ratified 2026-10-28, by the author's reading** (the author's *3a*, given that evening to the
decisions put at M-generated-programs' close): the resolution as it landed, R1 to R8, with the three
decisions the coordinator took under the batches' standing default,
https://github.com/heroes-lang/heroes/issues/2246 (the depth shapes the `generated` suite writes
rather than commits), https://github.com/heroes-lang/heroes/issues/2247 (the two malloc fills asked
of every program, not of a program with an `extern` alone) and
https://github.com/heroes-lang/heroes/issues/2248 (the milestone page's two hard shapes built in
this milestone). Owed after the push: R7's reading of `generated` on the CI's Windows leg, the
sample halved where that leg passes 200 minutes.

**Corrected 2026-10-29**: the folder the scoring's reductions bullet names,
`.claude/worktrees/lane-generated-programs-scratch/slide/` on the coordinator's Mac, left it that
day by the author's instruction; its 549 text files are `231-reports/scores-evidence.tar.xz`
(`231-reports/evidence.md`, § Added 2026-10-29).
