# Panel 231, the completeness critic's report

Seat: completeness critic, one pass after the seats (00-shared.md, first paragraph). No verdict:
what is MISSING, every finding with the command that settles it or written as a question.
Worktree `.claude/worktrees/231-critic`, detached at `bd0c4477b`, its `heroes-next` printing
`sources 5e0ca4c634086a7c`. Scratch `.claude/worktrees/231-critic-scratch/`. Written as it goes,
2026-10-28; a section marked *in progress* is unfinished.

## Findings (in progress)

### 1. Q1 reproduced, and a route nobody listed: the net's generator built by stage 1, the seed's compiler

**Fact.** The compiler-engineer's experiment holds as written. In my worktree, with the seat's
prototype copied in (`tests/harness/gen_*.hero`, `generate.hero`) and its planted compiler copied
out (`heroes-planted-shr`, `--version` `sources 44b1e1fd6c5780ff`), the generator built three ways
and run `make --seed 1000 --count 300 -o out-<g> --declared 30`:

- built by `./heroes` (stage 1, clang over `seed/heroes.c`, `sources 638149f67ad4d782`) and by
  `./heroes-next` (stage 2): `diff -rq out-stage1 out-stage2 | wc -l` prints **0**; the stage-2
  output equals the seat's `gA300` file for file (0 `.hero` differ);
- built by B: `diff -rq out-stage2 out-B` names **36** `.expected` and **20** `.hero` files, the
  seat's 36 and 20;
- judged by B with the seat's `judge1.sh` (copied), `xargs -P 4`: B's own output **205 PASS, 95
  PASS-declared, 0 failures**; stage 1's output **36 `ORACLE2-O0`**, 179 PASS, 85 PASS-declared.

**What is missing.** Route 1 of Q1 (*build the generator with another compiler than the one
judged*) is placed in the hunt alone, and Q7 concedes that the net's `generated` suite is blind
(*built by the compiler it judges, so Q1's blindness holds in it*). But every gate already holds a
second compiler that differs from the judged one wherever the emitter moved since the last seed
refresh: **stage 1**, built by clang from `seed/heroes.c`. CI names it `heroes-from-seed`
(`.github/workflows/ci.yml:566`, `:579` builds stage 2 from it), and the net is run as
`./heroes run tests/harness/main.hero -- ./heroes` with `./heroes` being stage 2 (`ci.yml:799`).
Stage 1 builds the seat's generator (above) and its output is byte-identical to stage 2's. So
`./heroes-from-seed run tests/harness/main.hero -- ./heroes` (or a `--generator-compiler` word of the
`generated` suite) breaks Q1's correlation in the net at zero cost, for every miscompilation
introduced since `seed/heroes.c` was last refreshed (`80a888f39`, 2026-10-26, by `git log -1 --
seed/heroes.c`). Its price, unmeasured: the harness may then use only the forms the seed knows,
which is the rule `selfhost/` already lives under (`.claude/rules/verification.md` § The batch:
*a new form enters `selfhost/` only after the refresh whose seed knows it*).

**What the sitting should do.** Put the stage-1 route beside the contract grid as P7's guard, or say
why not; P2's prediction (*9 of 10 plants caught*) should name which compiler builds the generator
in the net, since with stage 2 building it the net's share of that prediction is 0.

### 2. `==` on a handle's earlier copy: reproduced, and a committed golden already pins it at no abort

**Fact.** The ffi-pragmatist's `q3/d_eq.hero`, `d_eq_null.hero` and `d_eq_cross.hero`, copied with
`gen5.h` and built by my `heroes-next`: `d_eq` prints `0` / `true`, exit 0; `d_eq_null` `0` /
`false` / its sentence, exit 0; `d_eq_cross` aborts at `g5_total(kept)`, 134. Nothing between
`g5_close` and the comparison opens a handle (read: the three programs), so spec § 13's exception
(*unless C has since handed its address out again or a million other lives have ended*,
`spec/heroes-spec.md:439`) does not apply. `hero_handle_eq` (`runtime/parts/alloc.c:903`) asks only
the dead page.

**What the seat's search missed.** Its negative, *the `limit-*` cases pin `==` on a stale copy only
after C handed the address out again*, is true of `limit-*` and leaves out
`tests/golden/run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle.hero`,
which is `d_eq_null` exactly: `kept = a`, `release(o: a)`, then `print("null: ", kept == nullptr)`,
its `.expected` pinning **`null: false`** and the abort only at the next crossing into C. And `git
log -S'and so does a copy made before' -- spec/heroes-spec.md` names **`c8b1bc878`** (2026-09-25,
M-agreed-retention step 14), which is also the commit that rewrote that golden's ending to the
crossing's abort and **kept** its `null: false` line, written the day before at `c981be56e` (`git
log -p -- <the case>.expected`). So the spec's sentence was written in the commit that re-blessed a
`==` on a pre-end copy at no abort, which reads as *so does a copy made before* meant of *handing
it to C again* (an inference from the commit; the writer's intent is unasked), which is
design.md Part 8 wart 20's reading (`docs/design.md:4226-4235`, *a copy made before the call is
refused at its first crossing*). The runtime is not a regression and the net is green on it.

**What the sitting should do.** Name it as what it is: a spec sentence one reading of which
promises an abort the net pins as absent. Changing either side is outside this lane (the spec is a
§ 4 panel path; the runtime's `hero_handle_eq` asking the remembered set would move this golden).
For the generator it is a premise: **the C arm's declared arm must not write `==` on a pre-end copy
as an abort** until it is ruled, or oracle 2 files the spec's ambiguity as the compiler's defect.
Filing it is the coordinator's; its class (`blocking` as *a false message*, or not) depends on
which reading the author takes.

### 3. Oracle 4 at the C boundary: the seat is right, and the record already said so in 2026-09

**Fact.** Rerun of the ffi-pragmatist's six `q3/r_*.hero` with my `heroes-next`: `r_nohdr` check 0
/ build 1 `ffi_missing_header`; `r_noname` 0 / 1 `ffi_unknown_name`; `r_width` 0 / 1
`ffi_parameter_type`; `r_lend`, `r_mapkey`, `r_partial` 1 / 1. **The record states it, which
neither seat cites**: `docs/roadmap/milestones/M-check-completeness.md:18`, *a module that is nothing
but an `extern` group is `heroes check` exit 0 with zero output, because `check` never runs clang —
that fourth one is M-package-manager's*, and `:50`, *Running clang inside `check` for the fourth face
is the same trade at a larger price* (refused, panel 082 R3). The milestone page of this row cites
oracle 4 as *M-check-completeness's promise*, and that promise carries this carve-out.

**Contradiction between the seats**: the compiler-engineer's P5 says *Oracles 2, 4 and 5 hold as
written*; the ffi-pragmatist says oracle 4 is false by design at the C boundary. Checkable, and the
ffi side holds (the six reruns above). The engineer's prototype writes no `extern`, so its *holds* is
true off the boundary only.

**What the sitting should do.** Word oracle 4 with the carve-out the record already has, as the
ffi-pragmatist drafts it, and cite M-check-completeness's two lines so the oracle does not re-derive
a refusal that was already priced.

### 4. `leaks --atExit` sees a C leak on this Mac: reproduced; the compiler-engineer's *972 cannot be measured on this Mac* is false

**Fact.** `q4/tested.hero` and `leak.h` copied; `heroes-next build tested.hero -o t0` and `-O2 -o
t2`; `leaks --atExit -- ./t0`: *Process 12130: 200 leaks for 6400 total leaked bytes*, exit 1;
`./t2`: *0 leaks for 0 total leaked bytes*, exit 0. `noowned.hero` (a `-> cstr` without `owned
free`) at `-O2`: *1 leak for 16 total leaked bytes*, the program itself exit 0. The two rule files
say otherwise in so many words: `.claude/rules/c-boundary.md:179-180` and
`.claude/rules/platforms.md:120-121`, *LeakSanitizer exists on that leg and on no other, so a leak in
a C binding is invisible on this Mac in all three configurations*: true of the net's legs, false of
the Mac.

**Contradiction between the seats**: the compiler-engineer's P8 says 972 is *unrun here by necessity*
and *cannot be measured on this Mac (LeakSanitizer unsupported on Darwin arm64, measured)*; the
ffi-pragmatist measured it on this Mac, read from the binary (`otool`) and by `leaks`. Checkable,
and the ffi side holds (above). The engineer measured ASan's leak detector, not the Mac.

**What the sitting should do.** Take the ffi-pragmatist's oracle 5 sentence (`leaks --atExit` on the
`-O0` binary on Darwin), and file the two rule files' correction as a record, not a language change.
Unrun by anybody: `leaks` on the CI's `macos-26` runner, which oracle 5's Darwin half rests on.

### 5. `consumed_borrowed_handle`'s note: reproduced as the seat describes

**Fact.** A wrapper `function done(a: G5) -> i32` returning `g5_close(a)` is
`error[consumed_borrowed_handle]` at `w1.hero:7:21`, the caret under the `a` of `g5_close(a)`, and
the note says *Mark the parameter `@a` and the call site `@a` too*
(`selfhost/check/consuming.hero:154`). Written at the call the caret points at, `function done(@a:
G5)` with `g5_close(@a)`, it is `error[marker_mismatch]: g5_close does not change this argument`
(`w2.hero:7:22`); `g5_close(a)` with `done(@a)` at the caller checks clean (`w3.hero`). `grep -rl
"and the call site" tests/golden` prints nothing, so no golden pins the note's words and its repair
moves no `.expected`.

**What the sitting should do.** Nothing in its resolution; it is the coordinator's `adjacent` filing,
as the seat says. It matters to the generator only as a premise the C arm's wrappers must follow
(take the handle `@a`, call with no `@`), which the seat measured.

### 6. Panel 184's floor: absent from the spec (true), present in the tree, and deliberately unwritten

**Fact.** `grep -n -i 'depth\|deep\|nest\|floor\|levels' spec/heroes-spec.md` prints five lines,
none a floor: the compiler-engineer's negative holds. But its consequence, *no instrument can tell an
exit 134 below the floor from one above it*, overlooks two things in the record:

- **2,000 is pinned as R6's floor by committed cases**: `grep -rn -i "2,000" tests/golden
  --include='*.hero' | grep -i -E "floor|R6|184"` names
  `run/fixedbugs-169-brackets-2000-deep-build-and-run.hero`, `run/fixedbugs-169-chains-2000-long-...`,
  `run/fixedbugs-169-one-operand-runs-2000-long-...`, `check/fixedbugs-259-...`, `check/fixedbugs-264-...`
  and `full/fixedbugs-489-...`, each saying *the floor panel 184's R6 measured*;
- **the sentence was left unwritten on purpose**: issue 1521 (*Panel 184's R5 landed*), *What R6 can
  say, and what it cannot yet*: *N = 2,000 holds on this Mac and on Linux arm64 ... the first abort on
  both is between 3,000 and 3,500 ... The Windows box is owed (offline on the day), so the sentence is
  not written in the spec*. A search for an issue that carries R6's sentence as owed (`gh search
  issues` for *R6 floor*, *depth floor spec*, *nested up to N compiles*, *Panel 184*) finds none
  open: R1, R2, R4, R5 and R7 have *landed* issues, R6 has none. A question, not a premise: is R6 owed
  by an issue under another wording?

**What the sitting should do.** Oracle 1's reading (d) can take N = 2,000 from the goldens today for
this Mac and Linux, and P4's net dial (64 and 16) is far under it, so the engineer's objection to
P4 needs no spec sentence to be met in the net. Whether R6's sentence (and its Windows N) is owed
work of this milestone, or of nobody, is a question the sitting should put, since issue 1521 left it
with no owner.

### 7. Task 1234: the engineer's re-home drops the half the task was filed for, and the one live witness passes `-fsyntax-only`

**Fact.** `heroes-next build tests/golden/fixedbugs/ffi-missing-link.hero --emit-c` exits 0;
`build` exits 1, `error[ffi_missing_link]`. The C it wrote: `clang -std=gnu11 -fsigned-char -I runtime
-I tests/golden/fixedbugs -fsyntax-only` **exit 0**; compiled with `runtime/runtime.c` and linked,
*Undefined symbols for architecture arm64*, exit 1. Task 1234's own body (`gh issue view 1234`):
*the same program built both ways must give the same exit code and the same stdout, and it must
**LINK** and not merely compile. Each half of that sentence was paid for*. Its 2026-10-25 comment
adds that the artifact's bytes are not what the rounds compiled
(`selfhost/cli/artifact.hero:104-116`, *no clang run of this build read these bytes*), so
`-fsyntax-only` over them is not redundant with `--emit-c`'s own rounds.

**Contradiction between the seats**: the compiler-engineer's P8 re-homes 1234 to `emission` as a
`-fsyntax-only` predicate (*about 30 lines*) and makes only *a refusal among the 14* bring it back;
the ffi-pragmatist puts `-fsyntax-only` in `emission` **and** the link and the two-route run in the
generator's oracle 4 and P7's sample. Checkable, and the run above settles it: the engineer's
re-home closes 1234 with its one known witness unseen. (The two seats' counts, 679 of 693 and 692
of 705, differ by `examples-curl-main.c` and by the 12 `tests/golden/emit/*.expected` the engineer
did not compile; the engineer's include path, `-I tests/golden/{run,emit,ir,fixedbugs}`, has no
`examples/<dir>`; both find no failure of the emission's own.)

**What the sitting should do.** Rule 1234's two halves apart: the syntax half to `emission`, the
link and both-routes half to a named home (the generator's oracle 4 as the ffi-pragmatist has it, or
a leg of `emission` priced at its 72.57 s four wide), and close 1234 only when both have one.

### 8. Task 972: the corpus half was kept on this milestone on purpose; neither seat answers why, and its deciding measurement is unrun

**Fact.** Task 972's comment of 2026-10-25 (`gh issue view 972`) ends: *It is left to this milestone
rather than repaired now because its first verdict is a Linux leg's after the push, and a red leg
there is a leak defect this milestone's oracle 5 exists to find. Still on M-generated-programs.*
The compiler-engineer proposes to *re-home the corpus half ... to the corpus suite, outside this
milestone*, without meeting that reason; the ffi-pragmatist recommends the corpus's `--sanitize -O0`
arm (*owed for the corpus*), milestone unnamed. And the measurement 972 turns on, whether clang at
`-O2` deletes on **Linux** the leak LeakSanitizer would report, is unrun by both seats (`docker
info`, *Cannot connect to the Docker daemon*, the ffi seat; the engineer did not try) and by the
2026-10-25 comment (*a question for the Linux container or the CI, unrun today*).

**What the sitting should do.** Say which milestone owns the corpus's fourth configuration with the
comment's reason answered, and ask the coordinator for the one Linux arm64 container run that
settles 972's premise (the ffi seat's `q4/tested.hero` under `--sanitize` and `--sanitize -O0`);
that is not a paid run. Until it runs, *settled by defect 390 for `run` on Linux* is an inference,
as the ffi seat marks it.

### 9. `leaks --atExit` over 61 committed `run` cases: no false positive, which oracle 5's Darwin half needs

**Fact.** Seventy-two `tests/golden/run/` cases (every one in twelve of the alphabetical list, the
`abort`, `panic`, `dead-`, `limit-`, `leak`, `exit-`, `stack` and `deep` names left out, plus all
19 `ffi-*`), each built at `-O0` by my `heroes-next` and run from the worktree root under `timeout 60
leaks --atExit -- <binary> < /dev/null`, four wide (`.claude/worktrees/231-critic-scratch/lk/`,
`one.sh`, `result.txt`): **61 report `0 leaks`, exit 0**; 10 skipped for an `!panic`/`!exit`
ending; 1 does not build here (`fixedbugs-437-...`, `sys/prctl.h`, `ffi_missing_header`). With the
ffi seat's 0 on its seven generated programs, `leaks` has not yet named a leak that is not one.

**What the sitting should do.** Nothing more is needed for the oracle's sentence on this Mac. The
two premises still unrun are the CI's (`leaks` on `macos-26`, where the runner's permissions may
differ) and the cost over a whole sample (about one second each here, an indication).

### 10. Task 2214: the engineer answers a coverage question, not the one 2214 asks; a cheap half of the asked criterion, measured

**Fact.** Task 2214 (`gh issue view 2214`) asks for *a case counted as load-bearing where some
regression would fail it and no other case of its kind, the criterion and the review that tries to
refute each removal written before any case is removed; a measurement* under `docs/measurements/`,
after Goose's audit (*coverage counted only toward fixtures "that would notice the same
regression"*). The compiler-engineer measures **unique executed counters** (336 of 709 cases cover
none; reproduced from its `cov/no-unique.txt`, 336 lines, 257 `fixedbugs-`) and closes the question
*without deleting a case, since a case's worth is the text it pins*. That text argument is exactly
what 2214's criterion asks to be measured rather than assumed. A first, cheap cut of it, by
diagnostic code: over `tests/golden/check/*.expected`, `grep -oE '(error|warning|note)\[[a-z_]+\]'`
per case, **59 codes are pinned by one case only**, and of the 336 no-unique-counter cases **2 pin
such a code** (`ffi-fixed-array`, `fixedbugs-173-a-closing-brace-alone-is-refused`); the other 334
pin only codes some other case pins too (`.claude/worktrees/231-critic-scratch/codes.txt`,
`nu_lonely.txt`). Whether their message templates are pinned elsewhere is unmeasured.

**What the sitting should do.** Not close 2214 on *coverage cannot read text*: its criterion is a
measurement of text and counters together, and the code-level cut above is one command. The rule
the engineer cites (a fixed defect keeps its case, `.claude/rules/diagnostics-and-goldens.md`) does
answer the 257 `fixedbugs-` cases, and should be the stated reason for them, leaving 79 to the
criterion 2214 names, with its deliverable (a `docs/measurements/` file) as the closing condition.

### 11. Smaller corrections to the seats, each read

- **The compiler-engineer's Q2**, *no `.expected` can name the depth at which a 536,576-byte thread
  (`runtime/parts/spawn.c:243-244`) ... run[s] out*: those lines describe the defect panel 115
  demonstrated, a thread handed the platform default when `pthread_attr_setstacksize` was refused
  and its answer trusted (*blaming the author's recursion for a thread that got 536,576 bytes*); the
  runtime asks for the floor or a size its starter names and aborts if the OS gave less
  (`spawn.c:250-266`). The conclusion (recursion depth out of the declared arm) stands; the number
  is not the stack a program's thread gets.
- **The compiler-engineer's Q3 oracle 3**, *all 159 differ only where `fmt` breaks a bracket past
  120 columns*: the command given, `tr -d ' \n,'` of both sides equal, 159 of 159 (reproduced), also
  hides padding before `=>`, which the same section names as a second difference. Read in the
  seat's `g300/`: every one of the 159 changes its line count (each holds a wrap), and 22 of them
  have a changed line holding `=>`; `diff g1003.hero g1003.fmt` shows line 25, `.c1     => ...`
  printed `.c1 => ...`, a padding difference beside the wraps. The count holds; the *only* does not,
  and oracle 3 as the engineer rewords it (fixed point, same answer) still passes these.
- **The shared brief's facts** re-run at `bd0c4477b` and holding: no `gen|fuzz|reduce` file, 103
  harness modules, 180 `!panic` and 10 `!exit` expectations, 254 `.h` and 19 `ffi-*.h`,
  `suite_corpus.hero:428`, `suite_emission.hero:82,87` comments only, the five `wc -l` (149, 104,
  178, 507, 2,408), 58 `hero_` functions, 84 `hero_unreachable(` calls, `CEILING` 300, the
  montecarlo lines 36, 41, 85, 88, design.md's § 1.11, § 1.12 and § 4.19 at 482, 590, 2522. **The one
  that does not hold** is the engineer's correction 2, reproduced: `docs/design.md:809` is QBE's
  description (*~12k lines of C ... a fuzz-tested C ABI*), not something this compiler could hold.
- **The compiler-engineer's sizes**, re-run with `code_lines`'s rule in `awk`: the seven prototype
  modules 73, 85, 246, 184, 410, 247, 88, total 1,333, 1,722 by `wc -l`; 33 tracked harness modules
  past 300, `suite_records.hero` 4,415; `selfhost/` 689 modules and 179,309 lines, `mutate/` 42 and
  10,522; the harness 59,390 tracked lines. All hold.

### 12. The sitting measured no plant the net misses; one narrower plant, measured: the prototype is blind to it and a five-line policy change sees it in 61 of 300

**Fact, the seat's plant.** The `run` goldens already see the compiler-engineer's plant B (its Q8
says so): built by B, `tests/golden/run/bitwise.hero` and `fixedbugs-577-a-constant-whose-every-step-fits-runs.hero`
print other than their `.expected`, and match under my `heroes-next`. So Q1's experiment measures
the correlation, not what the generator adds to the net; no seat measured a miscompilation the
hand-written net misses and the generator finds, which is the milestone's yield.

**Fact, a narrower plant, B2.** In my worktree, `selfhost/emit/operator.hero:184`'s `>>` made
arithmetic only for a count below 32 (`(l < 0 && r < 32) ? ... : logical`), compiler B2 built by
`heroes-next` (`real 37.70`, `sources 0da830a9742159a3`), the plant reverted at once (`git checkout
--`, `git status` clean but for untracked files). `-8 >> 40` prints `16777215` under B2, `-1`
under the trunk. Then:

| judged under B2 | moved | how measured |
|---|---|---|
| the 599 `run` goldens, built at `-O0` and run (the seat's `lite.sh`, copied) | **1** (`fixedbugs-577-...`) | each case's stdout under B2 against under the trunk, `cmp`, 599 pairs |
| the seat's generator, seeds 1000 to 1299, its own output | **0 of 300** | `judge1.sh`, the same as finding 1 |
| the same generator with shift counts drawn in 0..63 (5 lines in `int_expr`, `.claude/worktrees/231-critic-scratch/gen_make.shift-counts.hero`) | **61 of 300** `ORACLE2-O0`; under the trunk 300 of 300 pass | the same |

**Why the prototype is blind**: its literals come from `constant_of` (`gen_rng.hero:62`), small
ones in -16..16, edges near `±2^k` and the extremes, and a shift whose count falls outside 0..63 is
repaired by changing its operator, so no shift count is drawn for a shift. Counted in its 300
programs: `grep -oE '>> [0-9]+'` gives 182 literal counts in 0..31, 6 in 64 and up (the declared
arm), **0 in 32..63**, and 311 `>>` whose count is an expression.

**What the sitting should do.** Two things, neither in the briefs. First, P3 needs **operand domains
per operator** (a shift's count drawn over 0..63, a divisor near -1 and 0), which YARPGen's
policies have and the prototype's constants mix does not; this one is measured. Second, the
milestone's yield should be measured the way this finding did it, as a **plant set**: one plant per
emitter rule and operand class, each scored against the `run` form and against the generator built
by stage 1, so P2's prediction (*9 of 10 plants*) and P3's are judged against the net rather than
against nothing. A question, not a premise: how many plants does the net already see, and is the
generator's margin over it the number that should size N?

### 13. The reducer's symptom: *any disagreement*, where P6 says *the symptom*; slippage is unmeasured

**Fact.** The compiler-engineer's tree reducer keeps a candidate *while B still prints something
other than what the evaluator computes for that candidate* (its Q6). P6 says *keep the edit while
the symptom stays*. With one plant per compiler the two are the same; with a compiler holding two
wrong-code defects, a reduction that keeps *any* disagreement can slide from the defect found to the
other (C-Reduce's interestingness test exists to pin the symptom). Every one of the 36 reductions ran
under a single plant (`reduce36-all.txt`, 36 lines, 838 candidates, 0 refused, reproduced by `awk`),
so slippage is unmeasured, not absent. B and B2 together (finding 12) are a compiler with two
plants on which it can be measured.

**What the sitting should do.** Say what *the symptom* is for each oracle (for oracle 2, perhaps the
first differing output line and the operator kind at its root; for oracle 1, the exit and the
`stack exhausted in`/`internal error` text) and ask for one reduction under a two-plant compiler
before P6 is ruled.

### 14. A declared abort is matched by its message, never by its place, and the run form can pin only one of the two

**Fact.** The run form's `!panic:` is one line, and the check is `strings.contains(text: err,
needle: p.message)` with any non-zero exit (`tests/harness/expectation.hero:159-166`). An abort's
place is its second stderr line (`  at index.hero:4:11, in index.main`, 00-shared § Facts), which
M-panic-location made exact. So a committed declared-arm witness passes when **another** operation
of the same class aborts first, as long as the stdout before it matches; the compiler-engineer's own
judge compares the first stderr line, which has the same blind spot. The generator knows the
planted operation's line and column.

**What the sitting should do.** A question it did not ask: should a declared-arm witness pin its
place, and if so, does the run form need a second needle (a form change, with its cost), or does the
generator's own judge carry the place while the committed witness carries the message?

### 15. The judge, as prototyped, breaks the rule for a run that may not end

**Fact.** `.claude/rules/verification.md` § A run that may not end: *built with `heroes build` and its
BINARY run under `timeout`, never `timeout heroes run`*, its output *to `/dev/null`, or through `head
-c <bytes>` into a file*, *until defect 425 is repaired and after*. The compiler-engineer's
`judge1.sh` runs its `-O2` leg as `timeout 120 "$H" run "$src" > "$base.o2.out"` and every leg's
output into a file with no byte bound. A generated program ends by construction only under a correct
compiler, and the hunt runs it under one that may not be. `shell.run_bounded` (`tests/harness/shell.hero:185`)
holds the bytes, but refuses a byte cap where the platform has none, Windows (`can_limit_writes`,
its own comment).

**What the sitting should do.** Write the rule into P1's `judge` and `hunt` (build at each level, run
the binary bounded in seconds and bytes), and say what the `generated` suite does on the Windows
leg, where no byte cap exists.

### 16. P7's N is priced on this Mac's gate B; the CI runs the net in one process, and its slowest leg is at 135 of 240 minutes

**Fact.** `gh run view 40474270628 --json jobs` (the trunk's head before this milestone, green on
four legs): the step *The net, in Heroes* ran 08:44:52 to 10:45:39 on Windows x86-64 (121 minutes),
08:42:50 to 10:17:02 on Darwin arm64 (94), 08:44:12 to 09:55:47 on Linux arm64 (72), 08:45:10 to
10:00:18 on Linux x86-64 (75); the Windows job 08:34:27 to 10:49:10, against the job's
`timeout-minutes: 240` (`.github/workflows/ci.yml:193`, *the slowest, Windows, read under two hours on
2026-10-25*). The net there is one `./heroes run tests/harness/main.hero -- ./heroes` (`ci.yml:799`),
suites one after another. The compiler-engineer's N = 128 is priced at 1.67 s a program on this Mac,
beside `run` in gate B's four slots, and its prediction is about gate B alone.

**What the sitting should do.** Price N on the CI's Windows leg too, or state the inference (a
program's cost there is unmeasured), since a sequential net adds the whole sample to a leg already
past half its ceiling.

### 17. Smaller items, each with its command

- **The parser is reachable from the harness as a child**, against the compiler-engineer's *the
  compiler's parser ... lives in `selfhost/` where a harness program cannot reach it* (Q6): `use`
  cannot (reproduced, `unknown_module` and `module_path_climbs`; spec § 1, *Every `use` starts at the
  directory of the file you compile*), but `heroes lex <f> --dump-tokens --json` prints every token
  with its line and column (`{"kind": "kw_function", "text": "function", "line": 1, "col": 1}`), and
  `heroes parse <f> --dump-ast` the tree, fully bracketed and without places (`bind b3 = (2 < ((-6) >>
  3))`). A token-span reducer for a program the generator did not write (P6's condition) needs no
  move of anything; unbuilt.
- **The milestone page says *no 134* too**: `docs/roadmap/milestones/M-generated-programs.md`, the
  oracles paragraph of 2026-09-06, *(1) the compiler does not fall over — no 139, no 134*, which panel
  184's R6 (2026-10-01, *past the floor, today's abort stands*) overtook. Reproduced the
  compiler-engineer's `call5000.hero`: `heroes-next build` exits 134, *stack exhausted in lower.shape,
  inside the recursion of walk.check, walk.dispatch_call, walk.named_call and walk.user_call*. The
  page needs its correction beneath, whatever oracle 1's new wording is.
- **`__int128` on the CI's four triples** (the engineer's route 4, *whether every CI leg's clang takes
  `__int128` on Windows is unrun*): `clang -target <t> -std=c11 -fsyntax-only` of a `__int128`
  multiply and a `__builtin_mul_overflow` exits 0 for `x86_64-pc-windows-msvc`,
  `x86_64-unknown-linux-gnu`, `aarch64-unknown-linux-gnu`, `arm64-apple-macos`, and the Windows
  object has no undefined symbol (`nm`). Division in 128 bits, which would want compiler-rt's
  `__divti3`, and a run on the box are unrun.
- **The uninitialised heap read of the ffi seat's Q2 table**, reproduced (`q2/heap.hero`, `more.h`):
  `5` at `-O0`, `-1` at `-O2`, `5` and `-4702111234474983741` under `--sanitize -O0` with
  `malloc_fill_byte=0` and `=190`.

### 18. A route nobody listed for the sample: commit the answers, not the programs

**An inference, unrun.** P7's sample is regenerated on each leg and judged there, so a miscompilation
that is the same in the generator and in the program **on one platform** is invisible on that leg
(finding 1's correlation, one leg at a time), and each leg's answers are never compared with
another's. A committed digest of the sample's `.expected` files (one line per seed, a few kilobytes,
no emission to bless), computed where it was last blessed, would make the four legs a differential
for the generator's own arithmetic and its programs', and would break finding 1's correlation on
every leg at once, at the price of re-blessing the digest when the generator changes. Neither seat
weighed it against regeneration; the engineer's *never committed* rests on 7 MB of blessed C, which
a digest does not carry.

