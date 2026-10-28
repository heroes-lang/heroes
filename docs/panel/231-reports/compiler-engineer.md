# Panel 231, the compiler-engineer's report

Seat: compiler-engineer (the ceiling: design.md § 1.1, § 1.7, Part 5). Worktree
`.claude/worktrees/231-compiler-engineer`, detached at `bd0c4477b`, its `heroes-next` printing
`sources 5e0ca4c634086a7c`. Scratch `.claude/worktrees/231-compiler-engineer-scratch/`.
Written as it goes, 2026-10-28, and finished the same day.

## Summary

- **No veto**: nothing enters `selfhost/` or design.md Part 5's core; the whole is about 3,100 to
  3,700 harness lines, 1,333 of them built here and measured (Q4).
- **Verdicts**: P1 approve; P2 approve with three amendments; P3 approve, scoped; **P4 object**
  (depth in the net); **P5 object** (oracle 1's and oracle 3's wording); **P6 object** (`sites.hero`
  is unreachable from the harness and a text reducer loses on 36 real failures); P7 approve with
  N = 128; P8 approve as re-homed.
- **The prototype's numbers**: about 240 programs made a second, 3.6 judged a second four wide; 300
  of 300 pass oracles 1 to 3 at `-O0` and `-O2` (95 declared aborts matched); 30 of 30 pass the
  `run` form's five legs; 40 of 40 pass `--sanitize -O0`.
- **Q1, measured**: a planted `>>` miscompilation is invisible to oracle 2 in 300 of 300 programs
  when the generator is built by the planted compiler, and caught in 36 of 300 when it is built by
  the trunk's; the model's contract test catches it too.
- **Programs that failed an oracle**: with the trunk's compiler, **none of the 350 generated** (seeds 1 to 10, 1000 to 1299 and 5000 to
  5039; the first ten judged to the terminal, the rest in `g300.verdicts` and `seq40.verdicts`).
  Under the planted compiler B, 36 (`gA300.byB.verdicts`), all reduced (`red/<seed>/`), the
  smallest `b3 = 2 < -6 >> 3`; that failure is the plant's, not the trunk's. Outside the generator,
  **valid programs on which `heroes build` exits 134** (oracle 1 as P5 words it): a 10,000-term sum,
  a 10,000-deep right-nested sum, 20,000 parentheses, 20,000 unary minuses, 5,000 nested calls
  (`depth/chain10000.hero`, `rnest10000.hero`, `paren20000.hero`, `neg20000.hero`,
  `call5000.hero`), which panel 184's R6 calls the accepted abort past a floor that the spec has
  not yet stated (Q5).
- Every path above is under `.claude/worktrees/231-compiler-engineer-scratch/`; the prototype is
  under `.claude/worktrees/231-compiler-engineer/tests/harness/` (`gen_*.hero`, `generate.hero`).

## Findings, question by question

### Q2. The declared arm: what aborts, what prints, decidable before the operation or not

Measured: 28 probe programs in `.claude/worktrees/231-compiler-engineer-scratch/aborts/` (six are
the coordinator's, copied), each `heroes-next build <f>.hero -o <f>.bin` and the binary run under
`timeout 20`; operands passed through a parameter so no checker fold can see them, and three
(`litdiv`, `litover`, `overflow`) with the operands as `=` names of literals in `main`.

| class | probe | first stderr line, exit 134 unless said | decidable before the op |
|---|---|---|---|
| `+ - *` overflow, i64 | `overflow`, `litover` | `panic: integer overflow` | yes: compute in a wider domain, compare to the width's bounds |
| same at i8, u8, i32, u64 | `i8add`, `u8sub`, `i32mul`, `u64neg` | `panic: integer overflow` | yes |
| unary `-` of the minimum | `negmin` | `panic: integer overflow` | yes |
| `/` and `%` by zero | `div0`, `mod0`, `litdiv` | `panic: division by zero` | yes |
| `/` min by -1, i64 and i8 | `divmin`, `i8div` | `panic: integer overflow` | yes |
| `%` min by -1, **i64** | `modmin` | ``panic: `%` by -1 at the smallest i64: the remainder is 0, but C reaches it through a quotient that has no int64`` | yes |
| `%` min by -1, **i8** (and every narrower width) | `i8mod` | ``panic: `%` by -1 at the smallest i8: the remainder is 0, but the quotient it is the rest of has no i8`` | yes |
| shift count outside 0..63, `<<` and `>>` | `shl64`, `shrneg` | `panic: shift count outside 0..63` | yes |
| **`<<` past the sign bit** | `shlover` (`3 << 62`), `shlneg` (`-1 << 63`) | **exit 0**, prints `-4611686018427387904` and `-9223372036854775808`, the same under `--sanitize` and `--sanitize -O0` | not an abort: design.md §4.14 (`docs/design.md:2071`) defines `1 << 63` as `INT64_MIN`, the shift on the unsigned pattern (the emitted C, `t3 = (int64_t)((uint64_t)t1 << (uint64_t)t2)`) |
| array index | `index` | `panic: array index out of range` | yes |
| `str` index | `strindex` | `panic: string index out of range` | yes |
| array slice | `arrslice` | `panic: array slice out of range` | yes |
| `str` slice out of range | `sliceout` | `panic: string slice out of range` | yes |
| `str` slice through a character | `slice` | `panic: string slice splits a character` | yes, given the string's bytes |
| `.must()` on an error | `must`, `toi8` | `panic: .must() on an error: <code>: <msg>`; `to_i8` of 300 gives `does_not_fit: the value is outside the target's range` | yes |
| `assert` on two records | `assert` | `assert failed: p == q`, its place, `left:` and `right:` rendered | yes |
| recursion too deep, with a base path | `deepbase` (`down(100000000)`) | `panic: stack exhausted in deepbase.down`, **no `at` line** | **no**: the depth that exhausts is the stack the runtime gives, not a property of the program |
| recursion with no base path | `deep` | **refused at build**, `error[endless_recursion]`, exit 1 | n/a: not a run-time abort |

Where each text is made, by `grep -rn 'hero_panic[a-z_]*("' runtime/parts/*.c` and
`selfhost/emit/*.hero`: overflow `runtime/parts/panic.c:44` and `place.c:95`; the indices and slices
`array.c:176`, `array.c:346`, `str.c:306`, `str.c:334`, `str.c:336`, `place.c:216`, `place.c:224`;
`.must()` `failure.c:267` and `place.c:234`; **the two `%` messages are not in `runtime/` at all, they
are string literals the emitter writes** (`selfhost/emit/division.hero:58` and `:62`), and so is the
shift message and the narrow `/` guard; stack exhaustion is `runtime/parts/stack.c`.

**What the `.expected` should hold.** `tests/harness/expectation.hero` reads `!panic: <message>`;
the generator's declared arm writes the stdout printed before the abort, then `!panic: ` and the
first line's text after `panic: `. **The texts are copied from a run, never from the source**,
since the `%` pair and the shift text live in an emitter's string literal a `grep` of `runtime/`
misses (I missed them on the first grep). **Recursion depth stays out of the declared arm**: no
`.expected` can name the depth at which a 536,576-byte thread (`runtime/parts/spawn.c:243-244`) and
the main thread's stack run out, and the abort carries no `at` line to pin. The generator keeps
recursion in the clean arm, its depth bounded far under the stack (a measured bound: `deepbase`
ran out somewhere below 10^8 levels; the safe ceiling is a number to measure per platform, unrun).

**A finding for the generator's own model**: the spec says *Overflow aborts at every width* (spec § 7)
and does not say `<<` wraps; a generator written from the spec alone would compute `3 << 62` as an
abort and oracle 2 would report a false defect. The semantics the generator models are design.md
§4.14's, and its arithmetic model needs a test per operator against a run, the probes above.

### Q3. The core, built: a prototype generator and 300 programs judged

**What was built**, in my worktree only, under `tests/harness/` (`gen_rng`, `gen_tree`, `gen_eval`,
`gen_text`, `gen_make`, `generate`), every module through `heroes-next fmt --in-place` and the
write hook's `check`, with its own `test` blocks (6 tests, all passing). It covers the brief's list:
`i64` and `bool` expressions over all ten integer operators and the six comparisons, `&&` `||` `!`
`-` `~`; `if`/`else` writing a cell; `match` on a three-case variant with payloads, the binder `_`
where the arm reads nothing; `for` over `range` accumulating into a cell; a record with two `i64`
and a `bool`; functions of one or two parameters called with named arguments; array literals and
in-range indices. Policies: six operator contexts per region, a constants mix drawn per program
(small, edges of `i64`, runs of ones, all 64 bits), a reused-subexpression buffer at `main`'s top
level, depth 1 to 4 and leaf share 20 to 70 per cent drawn per program. Values are tracked **over
samples** (one at `main`'s top, three per function body, one per loop turn); an unsafe operator is
repaired YARPGen's way, `^` last. The declared arm plants one `print` of an aborting operation
(`/` or `%` by 0, overflow, shift 64 to 71, index past the end, smallest `i64` `/` or `%` by -1,
`-` of the smallest), and the evaluator's run of `main` writes the `!panic:` it owes.

**Its size, in `layout`'s own unit** (`code_lines`, `tests/harness/suite_layout.hero:1007`:
non-blank lines outside `test` blocks, comments counted): `gen_rng` 73, `gen_tree` 85, `gen_eval` 238,
`gen_text` 184, `gen_make` **410**, `generate` 52; **1,042 in all**, 1,339 by `wc -l`. `gen_make` is
past 300 and splits on a seam it already has (statements apart from expressions).

**The numbers** (commands in `.claude/worktrees/231-compiler-engineer-scratch/`; the machine shared with
two other seats, load 3.0 at the start, so every duration is an indication):

- making: `/usr/bin/time -p ./generate.bin make --seed 1000 --count 300 -o g300 --declared 30`,
  `real 1.26 user 0.45 sys 0.12`: **about 240 programs a second**; 300 programs, 15,894 lines,
  564,701 bytes, 53 lines each on average.
- judging: `bin/judge1.sh` per program (build at `-O0` and run the binary; `heroes run` at `-O2`;
  `heroes fmt`), `xargs -P 4`, `real 82.59 user 111.91 sys 59.65`: **about 3.6 programs a second
  four wide, 0.57 s of CPU each**. The judge, not the generator, is the whole cost: 1.26 s against
  82.59 s.
- verdicts (`awk '{print $2}' g300.verdicts | sort | uniq -c`): **205 PASS and 95 PASS-declared, 300
  of 300**: no exit outside 0 and 1, no `internal error`, no `hero_unreachable`, no refusal of a
  generated program, every answer the computed one at `-O0` and at `-O2`, every declared abort's
  stdout and `panic:` line the computed ones.
- oracle 3: **no program's formatted form fails**: in the 159 that `fmt` re-prints differently, the
  output is a fixed point (`fmt` of it gives it back), builds, and prints the same bytes at the same
  exit; and all 159 differ only where `fmt` breaks a bracket past 120 columns
  (`tr -d ' \n,'` of both sides equal, 159 of 159).

**Any program that failed an oracle: none in 300** (and none in the first ten). What the prototype
DID find is three facts about the canonical form the generator must copy and had not: `fmt` prints
`-(-8)` as `--8` and `~(-5)` as `~-5` (it reads back as the same tree: `print(--8, " ", --a)` prints
`8 8`), arms are padded so `=>` stands in one column, and a bracket longer than 120 columns is broken
one element per line, an array's without commas. Each is a premise of the generator about the
formatter, which is what oracle 3 judges; the first is also a reading question for another seat
(a C reader takes `--a` for a decrement), not mine.

### Q1. The value as it is built, and who computes it: measured with a planted miscompilation

**The plant.** In my worktree, `selfhost/emit/operator.hero`'s `>>` (an explicit arithmetic shift,
`l < 0 ? ~(int64_t)((~(uint64_t)l) >> r) : ...`) replaced by the logical shift a simplification
would write, `(int64_t)((uint64_t)l >> (uint64_t)r)`; compiler B built from it by `heroes-next`
(`heroes-next build selfhost/main.hero -o heroes-planted-shr`, `real 38.88`), and the plant
reverted at once (`git checkout -- selfhost/emit/operator.hero`). B is a correct program (A built
it) that emits wrong C: `-8 >> 1` prints `9223372036854775804` under B and `-4` under A. The same
seeds, 1000 to 1299, 30 per cent declared:

| generator built by | programs judged by | oracle 2 failures |
|---|---|---|
| A (`heroes-next`, the trunk's) | A | 0 of 300 (Q3) |
| **B (planted)** | **B** | **0 of 300: blind** |
| A | B | **36 of 300** (12 per cent), every one at `-O0` |

So **the correlation is total for a miscompilation that is the same in every context**: the
generator's `arith(">>", ...)` and the program's `>>` go through one emitter rule, and the expected
answer and the printed one move together. **The three configurations do not break it**: in the
blind row every program matched at `-O0` and at `-O2`, since the planted C is defined and clang has
nothing to disagree about. The plant moved 36 of 300 expectations and 20 of 300 texts (the repair
decisions read values), measured by `cmp` of the two generators' outputs seed by seed.

**What breaks it, each measured or marked unrun:**

1. **Build the generator with a different compiler than the one judged** (measured: the third row).
   Zero lines: the hunt's `--compiler` names the judged compiler and the generator is built by the
   `heroes` that runs it, so the round's base (the trunk's stage 2) builds the generator and the
   round's compiler is judged. It separates every miscompilation a round INTRODUCES, which is what a
   batch gate asks; it is blind to one both compilers share.
2. **The model's own `test` blocks against constants derived from the contract** (measured: a test
   of eleven asserts, signs and edges of every operator, added to `gen_eval`; under A `3 tests, all
   passed`, under B `assert failed: arith(">>", a: -8, b: 1).must() == -4`, `left:
   9223372036854775804`). Run by the net's own tests (`heroes test tests/harness/main.hero`), which a
   round runs with its own compiler, it is the one place the model meets an answer it did not
   compute. **Its limit, stated**: it catches what its constants enumerate, and I wrote this one
   knowing the plant; YARPGen's answer is an exhaustive grid (056 § 1.2, *exhaustive testing of
   4-bit values*), which here means every operator over every pair in -8..7 against a reference that
   computes through OTHER operators (`>>` as a floor division, `%` as `a - a / b * b`), so one
   operator's miscompilation cannot hide in its own reference. Unrun; about 40 lines.
3. **Compute narrow widths in `i64`, never in their own type** (an inference, unrun: the prototype
   generates `i64` only). Defect 567, *an integer division narrower than `i64` overflowing in
   silence* (056 § 2.3), is the shape: a model computing `i8` `/` with Heroes' `i8` `/` under that
   compiler writes the wrapped -128 into the `.expected` and the program prints it; computed in
   `i64` and range-checked, the model expects the abort and oracle 2 sees the defect. `u64` past
   `i64`'s range needs two halves, about 30 lines. For `i64` itself there is no wider width.
4. **A C helper** (`__int128`, `__builtin_*_overflow`) through an `extern` group: independent of the
   emitter entirely, about 40 lines of C. It would be the first C file in `tests/harness/` (`ls
   tests/harness | grep -v '\.hero$'` prints nothing; the harness binds only `hero_os.h` and
   `math.h`), and whether every CI leg's clang takes `__int128` on Windows is unrun. Not recommended
   while 1 and 2 cost nothing and close the same class.

**Recommended**: 1 and 2 at once, 3 when a width other than `i64` enters the generator. The risk in
practice is narrower than the plant suggests, since the compiler compiles itself and a rule wrong in
every context would have to survive the fixpoint over the compiler's own `i64` arithmetic; the
correlated class is a rule wrong on operands the compiler itself never uses (negative shifts, narrow
widths, the edges), which is exactly where the generator aims.

### Q5. Depth: where the dial meets a stack, and what oracle 1 must read

**Probes**, valid programs each, `heroes-next build` under `timeout 300`, the binary run under
`timeout 20` (`.claude/worktrees/231-compiler-engineer-scratch/depth/`):

| shape | builds and runs | `heroes build` exits 134 |
|---|---|---|
| `y = x + x + ... + x`, a left chain | 1,000, 2,000, 3,000, 4,000, 6,000, 8,000 | 10,000 (`stack exhausted in flatten.expr`), 100,000 (`walk.expr`) |
| `(x + (x + (...)))`, right-nested | 1,000 | 10,000 (`flatten.expr`), 100,000 (`grammar_expr.atom_kind`, *inside the recursion of grammar_expr.binary, ...*) |
| `((((x))))` | 1,000, 5,000 | 20,000 (`grammar_expr.primary`) |
| `----x` | 1,000, 5,000 | 20,000 (`walk.expr`) |
| `g(g(...g(0)...))` | 1,000, 2,000, 3,000 | 5,000 (`lower.shape`, *inside the recursion of walk.check, walk.dispatch_call, ...*) |
| `if` nested in `if` | 100, 300, 1,000 | not reached |

**What oracle 1 must read, which P5 does not say.** Panel 184 (`docs/panel/184-*.md`, ratified
2026-10-01) ruled depth *the compiler's to hold*: R5 runs every pass on a thread whose stack the
compiler chooses (landed, issue 1521); R6, *the spec states a floor, not a ceiling ... past the
floor, today's abort stands*; R7 tells a clang that dies on a deep correct program **at exit 2**,
in the compiler's words with the program's deepest chain (issue 1522, defect 170, issue 1463).
So **P5's oracle 1, "no exit outside 0 and 1", misreads two designed answers**: exit 2 for clang's
death, and exit 134 past the floor. And **R6's floor is not in the spec today** (`grep -n -i
'depth\|deep\|nest' spec/heroes-spec.md` prints only `MAX_DEPTH`, a nested bracket, a nested
argument and *Recursion too deep aborts*), so no instrument can tell an exit 134 below the floor (a
defect, by R6's promise) from one above it (the accepted limit).

**Oracle 1, read so it cannot lie** (my recommendation): (a) exit 0 or 1 with the program valid:
judged by oracles 2 to 4; (b) exit 1 on a generated program: a refusal of a correct one, shape E,
filed; (c) exit 2 whose text is the compiler's report of clang's death: clang's limit, recorded with
the depth, filed as the emitter's shape where it is new (056 § 4); (d) exit 134 `stack exhausted in
<a compiler module>`: a defect below the floor, the limit above it, **which needs the floor stated**,
a number the panel measured per platform; (e) `internal error`, `hero_unreachable`, a signal, any
other exit, a timeout: a defect. The compiler names its own frames (`flatten.expr`, `lower.shape`),
and `--version`'s module names tell them from clang's, whose report is the exit-2 text.

**The dial.** In the net: depth at most 64 for an expression and 16 for statements, every program
under 120 columns a line (Q3: the wrap rule), so a sample costs what a hand-written case costs and
never meets a floor no document states. In the hunt: up to the floor and past it on purpose, at
1,000, 2,000 and 3,000, where today's weakest shape (`g(g(...))`) holds 3,000 and fails at 5,000 on
this Mac; Linux and Windows unrun. Clang's measured limits (056 § 2.3: 875, 32, 3,000 to 5,000,
10,000) are the hunt's, never the net's: each is an exit 2 by design or a defect already filed.

### Q6. The reducer: over the generator's tree, measured against a text reducer on 36 real failures

**The failures**: the 36 programs of seeds 1000 to 1299 that compiler B fails (Q1's third row).

**The tree reducer**, built (`tests/harness/gen_reduce.hero`, 247 code lines by `code_lines`'s rule,
316 by `wc -l`, plus a `reduce` word of 36 lines in `generate.hero`): it regenerates the program from its seed,
then drops statements (every later read of what a statement bound first replaced by its value, from
the evaluator) and replaces every top-level expression by a literal of its value, outermost first,
keeping a candidate while B still prints something other than what the evaluator computes for that
candidate; a tidy pass drops what nothing reads (spec § 5's unused binding) and the functions
nothing calls. **Over the 36** (`reduce36-all.txt`): **median 1,448.5 bytes to 215.5** (min 108, max
675; 5 to 19 lines, median 9), median 24 candidates judged, at most 36, **838 in all, none
refused**. The smallest witness, from 2,511 bytes:

    function main()
        b3 = 2 < -6 >> 3
        print(b3)

`false` under the trunk's compiler, `true` under B. One run, `real 7.63` for 15 candidates.

**Two defects of my reducer, found by running it, each a premise P6 should carry**: it once dropped
`main`'s last statement, which the grammar refuses (a block is never empty; caught as `refused 1`
and guarded); and it looped for ever on three programs, the smallest `i64` being printed as
`-9223372036854775807 - 1`, a `-` node, which "becomes its literal" as the same bytes and still
fails; the guard is the one every reducer owes, a candidate printed as the current best is no
progress. After both: 36 of 36 end, 0 refused.

**The text reducer**, from `sites.hero`'s primitives as far as a harness program can reach them:
`sites.hero` itself **cannot be used from `tests/harness/`** (measured: `use mutate/sites` reads
`tests/harness/mutate/sites.hero`, `error[unknown_module]`; `use ../../selfhost/mutate/sites`,
`error[module_path_climbs]`; a symbolic link reaches the file and then `use ast`, `use source` fail,
since `sites.hero`'s closure is 7 modules and 2,149 lines of the compiler's own tree, `ast`, `token`,
`source`, `bytes`, `char_column`, `module/names` and itself), so the text edit left is a statement's
lines deleted. Two measurements on the same 36:

- **validity** (`bin/statement_deletions.py`): of 892 single-statement deletions, **296 still check
  (33 per cent)**; refused `unknown_name` 399, `unused_binding` 103, `never_rebound` 49,
  `expected_expression` 45. Heroes refuses an unused binding and an unread cell, so most text
  deletions are not programs.
- **reduction** (`bin/text_reduce.py`, greedy, the failure test the only one a text edit leaves: the
  trunk's compiler and B disagree, since the edited program's answer is unknown): **median 1,451.5
  bytes to 1,338** (min 507, max 11,319), for 3,431 checks and 644 builds.

**Verdict on the question**: the tree reducer is sounder (every candidate valid, 0 of 838 refused,
its answer known, so a wrong-code failure is reducible with ONE compiler) and cheaper (838 builds
against 644 builds and 3,431 checks, for a result 6.2 times smaller at the median). A text reducer
needs a second compiler to judge wrong code at all, and the compiler's parser to edit below a
statement, which lives in `selfhost/` where a harness program cannot reach it. **P6's sentence
"from `selfhost/mutate/sites.hero`'s primitives" should be replaced** by *over the generator's own
tree*; its two edits are the ones P6 already names, applied to the tree.

### Q7. The net: the sample's price, its suite, and where the witnesses go

**What a generated case costs**, measured in the `run` form itself (30 generated programs copied
into my worktree's `tests/golden/run/` as `gen-1000` to `gen-1029`, 11 of them declared; the harness
built once, `real 15.68`; removed after):

| set, judged by the `run` form's five configurations, sequential | cases | result | `real` | per case, wall | per case, CPU |
|---|---|---|---|---|---|
| `gen-` (generated) | 30 | 30 passed, 0 failed | 61.36 | 2.05 s | 0.97 s |
| `abort-` (existing) | 15 | 15 passed | 31.49 | 2.10 s | 0.96 s |
| `fixedbugs-0` (existing) | 20 | 20 passed | 77.85 | 3.89 s | 1.85 s |

So **a generated program costs what a small hand-written case costs**, and passes every leg the
`run` form has, Guard Malloc and both sanitised builds included. With the four checks I recommend
for a sample (build at `-O0` and run, `run -O2`, `run --sanitize -O0`, `fmt`), 40 programs
sequentially: `real 66.73 user 22.79 sys 10.69`, **1.67 s of wall each** (load 3.75, an
indication), 40 of 40 passing.

**Where the witnesses go: `tests/golden/run/`, as ordinary cases**, `fixedbugs-<defect>-<slug>`
with the seed and the generator's commit in their header comment. A reduced witness is 5 to 19
lines (Q6) and is exactly the `run` form's shape, a program and its `.expected`, so it gains the
five configurations, `emission`, `determinism`, `lines` and `warnings` for nothing; a form of its
own would copy `suite_run.hero`'s 60-line `one_case` and its five legs. Its price is one blessed
emission each: `tests/emission/` holds 592 `run-` emissions, median 17,149 bytes, 30,429,375 in
all, and a reduced witness emits 10,086 to 16,180 bytes (`heroes-next build red/<s>/r<s>.hero
--emit-c | wc -c`, three witnesses).

**Where the sample goes: a suite of its own, regenerated and never committed.** Unreduced, a
generated program emits 27,364 bytes (seed 1000), so 256 of them under `tests/golden/run/` would
carry about 7 MB of blessed C re-blessed at every emitter change (an inference from that one size).
Regenerated from seeds 1 to N into the suite's scratch and judged there, it carries none. **Its name:
`generated`**. **Its size**: it runs beside `run` in gate B's four slots, `run` first as the critical
path (`.claude/rules/verification.md` § The optimistic chain), so it must end before `run` does;
`run` holds 599 cases today (`ls tests/golden/run/*.hero | wc -l`, trunk) at the 2.05 to 3.89 s
measured per case, about 20 to 39 minutes sequential (an inference: the whole `run` was not timed
today, and gate B read 18:39 split and 19:56 whole on 2026-10-09 with fewer cases). **N = 128
recommended**, about 3.6 minutes at 1.67 s each (inference from the measured rate), N = 256 at
about 7.1 if the first batch gates show room. It is deterministic under its seeds, so it goes red
when the compiler or the generator changes, never at random; and it is built by the compiler it
judges (`./heroes run tests/harness/main.hero -- ./heroes`), so Q1's blindness holds in it and
the model's contract tests, run by the net's own tests, are its guard.

### Q8. Coverage (task 2214): buildable here, cheap, and a weak steering signal for wrong code

**Buildable, measured**: `heroes-next build selfhost/main.hero --emit-c` (28,481,294 bytes of C,
`real 47.57`), then `clang -O0 -fprofile-instr-generate -fcoverage-mapping -I runtime heroes.c
runtime/runtime.c` (`real 14.70`); the instrumented compiler prints `sources 5e0ca4c634086a7c`.
`llvm-profdata` and `llvm-cov` are Xcode's (`xcrun --find`). Coverage maps to the one C file, not
to `.hero` modules (`grep -c '\.hero'` of the report: 0), but every C function carries its module
(`h_parseoperatorarm_to_the_arrow` is `parse/operator_arm`'s), which is enough to read it per
directory.

**One run over `tests/golden/check/`**: 709 cases, one profile each (2,286,304 bytes, 1.5 GB in
all), `real 7.87` sequential; merged in `real 0.74`; each case's executed counters extracted in
`real 23.77`. Together they reach 28.74 per cent of the compiler's regions; by directory, `parse`
84.2, `resolve` 75.2, top-level modules 71.4, `check` 66.1, `module` 37.0, `cli` 1.0, `emit`, `ir`,
`print`, `mutate`, `probe`, `measure` 0 (`check` stops before them). **Task 2214's question, by
executed counters**: 373 cases cover a counter no other case covers and **336 cover none**, 257 of
those named `fixedbugs-`; a greedy cover keeps all 51,912 counters with 425 cases
(`.claude/worktrees/231-compiler-engineer-scratch/cov/no-unique.txt` lists the 336). **This does not
license a deletion**: a `check` case pins a diagnostic's code, place and text in its `.expected` and
`#~` marks, which no counter sees (`.claude/rules/diagnostics-and-goldens.md`, *a fixed defect gets a
case named after it*). The whole instrument costs about two minutes of machine (emit, compile, run,
extract), so it is a measurement to run per milestone, not a suite.

**As a steering signal for the generator, measured weak for wrong code**: through `--emit-c`, the
599 `run` goldens reach 56,943 counters (`real 217.96`), the 300 generated programs 26,751 (`real
46.49`); **63 counters and no function are the generator's alone** (49 in `check`, 6 `ir`, 4 `emit`,
4 top-level). Yet the generator found the planted `>>` in 36 of 300 programs; the `run` goldens see
it too (`bitwise` and `fixedbugs-577-a-constant-whose-every-step-fits-runs` pass at `-O0` under the
trunk's compiler and fail under B, by my crude `-O0` judge, `bin/lite.sh`, which passes 473 of 599
under the trunk's compiler). A wrong-code defect lives in VALUES on code the net already reaches,
so per-program coverage feedback would not have steered toward it. **Recommended use**: the 30,255
counters the goldens reach and the generator does not name the forms it should learn next
(strings, maps, generics, `T?`, floats, the C boundary); per-program, AFL-style steering is the
hunt's to try later and not this milestone's.

### P8. The three tasks

- **972** (`--sanitize` at `-O2` deletes the leaks LeakSanitizer exists to find): **unrun here by
  necessity**: on this Mac a plain C leak of 999 blocks under `-fsanitize=address` and
  `ASAN_OPTIONS=detect_leaks=1` stops at exit 134, *not supported on this platform*, at `-O0` and
  `-O2` (`probe/leak.c`, arm64). The `run` form already builds `--sanitize -O0` (defect 390, its
  header); the corpus suite does not (`["-O0", "-O2", "--sanitize"]`,
  `tests/harness/suite_corpus.hero:427-428`). A Heroes-owned leak is the runtime's
  `hero_runtime_check_leaks()` in every `main`, which oracle 2's *silent stderr* already reads on
  every platform (205 clean programs passed it, Q3). **Fold**: oracle 5 for a generated `extern`
  program is `--sanitize -O0` on the Linux leg; **re-home** the corpus half (a fourth configuration
  there) to the corpus suite, outside this milestone.
- **1234** (nothing compiles or links what `--emit-c` wrote): **re-measured, and its premise no
  longer holds**: `clang -fsyntax-only -w -I runtime -I tests/golden/{run,emit,ir,fixedbugs}` over
  `tests/emission/*.c`: **679 of 693 pass**, `real 4.67` four wide; the 14 others all stop at a
  header `file not found` (curl, raylib, SDL, OpenSSL's SHA-256, libuv, and case headers outside
  those paths), unjudged rather than refused. The 2026-09-16 figure, 28 of 240, is history. **Re-home
  to `emission`**: add the predicate beside the bytes (the rule *an instrument watches the world*
  asks exactly this), with the packages' include paths, about five seconds. For generated programs,
  oracle 4 is the `build` the judge already runs, 300 of 300.
- **2214** (which `check` cases add nothing): **measured** (Q8): 336 of 709 by executed counters.
  **Fold** the instrument into this milestone as a per-step measurement (it costs two minutes and
  needs no suite), and **close the question without deleting a case**, since a case's worth is the
  text it pins and coverage cannot read text.

### Q4. Where it lives and what it costs

**Not in `selfhost/`, and the reason is the ceiling, not taste.** Everything under `selfhost/` is
the compiler: 689 modules, 179,309 lines (`find selfhost -name '*.hero' | xargs cat | wc -l`),
`selfhost/mutate/` among them, 42 modules and 10,522 lines behind the `heroes mutate` verb. A
generator there grows the compiler every user downloads and a verb's surface (CLAUDE.md § 10,
`.claude/rules/cli-surface.md`'s stopping rule, § 4's trigger on the tool surface). In
`tests/harness/` it adds **0 lines to the compiler** and proposes no verb: the harness is 103
modules and 59,390 lines (`cat tests/harness/*.hero | wc -l`), a program the net already is.

**A harness module cannot `use` a module of `selfhost/` at all** (measured, Q6): `use mutate/sites`
reads `tests/harness/mutate/sites.hero` (`unknown_module`), `use ../../selfhost/...` is
`module_path_climbs`, and a symbolic link fails one level down, `use ast` resolving from the root's
directory. So the generator reuses nothing of `selfhost/` by `use`, and that is a property to keep:
its printer and its evaluator are a SECOND implementation of the language's surface and semantics,
which is what lets oracle 2 and oracle 3 compare two things rather than one thing with itself.

**What it reuses, all inside the harness**: `shell.run_bounded` and `run_self_bounded`
(`tests/harness/shell.hero:186`, `:221`), which bound a child in seconds and bytes as
`mutate/child.hero` does; threads through `hero_os.h`, as `suite_cache.hero` already binds
`hero_thread_spawn`; `expectation.hero` for the `.expected` form; `strings.to_number`. Nothing of
`mutate/child.hero`, `mutate/workers.hero` or `cli/process.hero` is needed.

**Its cost, measured for what was built and estimated for the rest** (`code_lines`'s unit,
`tests/harness/suite_layout.hero:1007`):

| part | lines | status |
|---|---|---|
| `gen_rng` 73, `gen_tree` 85, `gen_eval` 246, `gen_text` 184, `gen_make` 410, `gen_reduce` 247, `generate` 88 | **1,333** (1,722 by `wc -l`) | built, Q3 and Q6 |
| `judge` word in Heroes: the four checks, oracle 1's five readings (Q5), `fmt` fixed point and meaning | about 200 | estimate, from `suite_run.hero`'s `one_case` and its two sanitised helpers |
| `hunt` word, workers on threads, failures only written | about 120 | estimate |
| `suite_generated.hero` and its line in `main.hero` | about 80 | estimate |
| widths `i8` to `u64`, computed in `i64` and two halves | about 150 | estimate |
| the contract grid of every operator over -8..7 | about 40 | estimate |
| `fmt`'s wrap rule in the printer | about 60 | estimate |
| strings and slices, maps, `T?`, floats, generics across `use` lines | 600 to 900 | estimate |
| the C boundary dial: a header generator, the `extern` group, its C half | 500 to 800 | estimate, unrun by this seat |
| **total** | **about 3,100 to 3,700** | |

That is the Pascal-P4 scale this seat measures against (about 4,000 lines), and it lands in the
harness, beside the instrument it serves. **No module need pass 300**: `gen_make`'s 410 splits on the
seam it has (statements apart from expressions). The 300 is a convention there, not an instrument:
`suite_layout.hero` excludes `tests/harness/` by its own header (*it is the net, not the compiler*)
and **33 harness modules pass 300** in its unit today (`suite_records.hero` 4,415 at the top).

**The cost nobody listed: a new surface form now lands in one more tool.**
`.claude/rules/diagnostics-and-goldens.md` § *A new surface form lands in every tool that reads the
language* lists the formatter, the dump printers, `mutate`, `probe`, the fixes, `measure` and the
two colourers; a generator that writes and evaluates the language is one more copy that, not
taught a form, never writes it, so the form goes unshot in silence. It belongs on that list, with
its test: the form generated, judged, and its answer checked.

### Corrections to the shared brief, each measured

1. *A module's ceiling is 300 code lines* (`suite_layout.hero:45-46`): that instrument excludes
   `tests/harness/`; the harness is held to the number by convention only, and 33 modules pass it.
2. *design.md says nothing of a generator ... one line, 809, a fuzz-tested C ABI, among the things a
   compiler of this size could hold*: line 809 is QBE's description in § 3.2 (*~12k lines of C,
   textual SSA IL ..., a fuzz-tested C ABI*), a property of QBE, not a thing this compiler could
   hold.
3. P5's oracle 1, *no exit outside 0 and 1*: panel 184's R7 makes exit 2 the designed answer to a
   clang that dies on a deep correct program, and its R6 keeps today's exit 134 past a floor the
   spec has not stated (Q5).
4. Task 1234's premise (28 of 240 emissions failing `-fsyntax-only`): today 0 of the 679 whose
   headers are found (P8).
5. The spec's *Overflow aborts at every width* does not cover `<<`, which wraps by design.md §4.14
   (`3 << 62` prints `-4611686018427387904` at exit 0, under `--sanitize` too): the generator's
   semantics come from design.md, not from the spec alone (Q2).

## Verdicts, per proposal line

### P1. Where it lives
- `verdict`: **approve**
- `section`: design.md § 1.1 (simplicity sets the ceiling) and § 1.7 (*anything in the core must be
  implemented in the type checker and the lowering and the backend*): nothing here enters either.
- `implementation_cost`: 0 lines in `selfhost/`; about 3,100 to 3,700 in `tests/harness/` (Q4),
  1,333 of them built and measured.
- `needed_for_self_hosting`: no.
- `argument`: A harness program adds no line to the compiler and no verb; it cannot `use` the
  compiler's modules (measured), which keeps its printer and evaluator independent of what they
  judge. The four words fit, with one amendment from Q1: `hunt --compiler` names the judged
  compiler and the generator is built by the `heroes` that runs it, so a round's base builds the
  generator and the round's compiler is judged, which separates every miscompilation the round
  introduces (measured: 36 of 300 caught that way, 0 of 300 the other).
- `prediction`: at task 934's close, `git diff --stat <milestone base> -- selfhost/` shows only
  defect repairs, and the core (the forms built here plus the widths, the contract grid, the wrap
  rule, `judge`, `reduce`, `hunt` and the `generated` suite, about 1,983 lines by Q4's table) holds
  at most 2,500 code lines in `code_lines`'s unit in at most 10 modules, none past 300.
- `condition`: a measured need to reach the compiler's parser from the generator (for instance a
  reducer that must edit below a statement on a program the generator did not write); then the
  piece needing it, not the generator, moves.

### P2. From the type, the value as it is built; two arms
- `verdict`: **approve, with three amendments**
- `section`: design.md § 1.12 (*check rather than assume*), § 4.14 (the shift rules,
  `docs/design.md:2071`).
- `implementation_cost`: `gen_eval` 246 and `gen_make` 410 built; plus about 40 (the contract
  grid) and 150 (widths).
- `needed_for_self_hosting`: no.
- `argument`: Built and run: 300 of 300 programs valid and answered right at `-O0` and `-O2`, 95
  declared aborts matched to their message. But the value is computed by the compiler under test,
  and a planted `>>` miscompilation left oracle 2 blind in 300 of 300 when the generator was built
  by the planted compiler. Amendments: (1) contract tests of every operator, run by the net's own
  tests; (2) narrow widths computed in `i64`, never in their own type (defect 567's shape); (3)
  recursion depth stays out of the declared arm, its abort carrying no `at` line and its depth
  being the stack's (Q2).
- `prediction`: at task 931's close, a plant in any one of the ten integer operators' emission
  rules, built into a compiler B, is reported by the contract tests under B or by oracle 2 with the
  generator built by the base, for at least 9 of the 10.
- `condition`: a measured miscompilation the contract grid and the base-built generator both miss
  would move me to the C-helper model (Q1, route 4).

### P3. Policies
- `verdict`: **approve, scoped**
- `section`: design.md § 1.1: a parameter is code the harness keeps; the document does not cover
  generator policy, said so.
- `implementation_cost`: built inside `gen_rng` and `gen_make` (six contexts, a constants mix, a
  reuse buffer, depth and leaf share per program), about 60 of their lines.
- `needed_for_self_hosting`: no.
- `argument`: The four policies cost little and work: edge constants and contexts produced the
  operands that exposed the plant. But coverage measures where the gap is: the 300 programs reach
  26,751 counters, the `run` goldens 56,943, and only 63 are the generator's alone. The gap is
  FORMS (strings, maps, generics, `T?`, the C boundary), not policies; YARPGen's 23 parameters
  should wait until the forms are in.
- `prediction`: after the C boundary arm and strings land, the generator's counters not reached by
  the `run` goldens (measured as in Q8) exceed 1,000, against 63 today.
- `condition`: a defect found only by a policy (reproduced with the policy and lost without it)
  before the forms land would justify more parameters first.

### P4. Two dials
- `verdict`: **object** (depth in the net); approve depth in the hunt and the C boundary dial.
- `section`: design.md § 1.12; panel 184's R6 and R7 (the floor, exit 2), which the document's
  own text does not yet carry.
- `implementation_cost`: depth is a parameter of `gen_make` (none extra); the C boundary dial 500 to
  800 lines (estimate, unrun by this seat).
- `needed_for_self_hosting`: no.
- `argument`: Measured on valid programs: `heroes build` exits 134 at 10,000 terms of a sum, at
  5,000 nested calls, at 20,000 parentheses. Panel 184 says an abort past the floor stands and a
  clang death is exit 2, but the floor is not in the spec, so a net program *hundreds deep* rests
  on a number nobody wrote. In the net: expressions 64 deep, lines under 120 columns. In the hunt:
  past the floor on purpose, oracle 1 reading the five answers of Q5.
- `prediction`: the first hunt whose depth dial reaches 5,000 nested calls reports `stack exhausted
  in lower.shape` (exit 134) on this Mac within its first 100 deep programs, until R6's floor is
  measured and stated.
- `condition`: R6's floor written into the spec with its per-platform N; then the net's depth may
  go to N.

### P5. The five oracles
- `verdict`: **object** (to oracle 1's and oracle 3's wording; the five themselves stand)
- `section`: design.md § 1.12; panel 184's R6 and R7; design.md § 4.15 (the canonical form).
- `implementation_cost`: the `judge` word, about 200 lines (Q4); oracle 3 as I recommend needs no
  copy of `fmt`'s wrap rule, as worded it needs about 60.
- `needed_for_self_hosting`: no.
- `argument`: Oracle 1 as written misreads two designed answers (exit 2, exit 134 past the floor)
  and needs the five readings of Q5. Oracle 3 *byte for byte* tests the generator's copy of the
  formatter as much as the formatter: 159 of 300 differ only by `fmt`'s 120-column wrap. Read it as
  the property defects 003 and 004 broke: `fmt`'s output is a fixed point and builds to the same
  answer (300 of 300), bytes where every line fits. Oracles 2, 4 and 5 hold as written; 4 is the
  judge's own `build`, 5 the runtime's leak report plus `--sanitize -O0` on Linux for `extern`.
- `prediction`: with oracle 1 as written, the first hunt reaching clang's measured limits (056
  § 2.3) flags at least one exit 2 that panel 184's R7 designed.
- `condition`: the author's ruling that exit 2 on a correct program is itself a defect would make
  oracle 1's wording right as it stands.

### P6. The reducer
- `verdict`: **object** to *from `selfhost/mutate/sites.hero`'s primitives*; approve the edits.
- `section`: design.md § 1.1; § 1.12.
- `implementation_cost`: `gen_reduce` 247 lines and a 36-line `reduce` word, built (Q6).
- `needed_for_self_hosting`: no.
- `argument`: `sites.hero` is unreachable from the harness (measured) and its closure is 2,149
  lines of the compiler's tree. A text reducer met three walls on 36 real failures: 596 of 892
  statement deletions do not check (unused bindings refused), the answer of an edited program is
  unknown, so wrong code needs a second compiler, and it ended at a median 1,338 bytes. Over the
  generator's tree the same two edits P6 names ended at a median 215.5 bytes, 0 of 838 candidates
  refused, one compiler. Keep P6's edits; apply them to the tree.
- `prediction`: at task 934's close, at least 90 per cent of reduced witnesses are under 20 lines
  and no reduction judges a refused candidate (today 36 of 36 under 20 lines, 0 of 838 refused).
- `condition`: a failure the tree cannot express (a program the generator did not write, a
  hand-written report) would need a text reducer, and only then the parser's reach.

### P7. The net
- `verdict`: **approve, with the sizes measured**
- `section`: design.md § 1.12; `.claude/rules/verification.md` § The optimistic chain (the critical
  path).
- `implementation_cost`: `suite_generated.hero` about 80 lines; each witness one `.hero`, one
  `.expected` and one blessed emission of 10 to 16 KB.
- `needed_for_self_hosting`: no.
- `argument`: Witnesses are ordinary `tests/golden/run/fixedbugs-<n>-<slug>` cases: they are that
  form's shape and gain its five legs free. The sample is a suite of its own, `generated`,
  regenerated from seeds 1 to 128, never committed (256 committed would bless about 7 MB of C),
  running beside `run`, the critical path; at the measured 1.67 s per program it takes about 3.6
  minutes. It is built by the compiler it judges, so the contract tests are its guard against Q1.
- `prediction`: at the first batch gate after it lands, `generated` at N = 128 ends before `run`
  does in gate B and adds no minute to gate B's wall.
- `condition`: a gate whose wall moves by more than a minute because of it would halve N.

### P8. The three tasks
- `verdict`: **approve as re-homed** (972 folded, 1234 re-homed to `emission`, 2214 measured and
  closed without deletions)
- `section`: design.md § 1.12; `.claude/rules/diagnostics-and-goldens.md` § *An instrument watches
  the world* (the predicate beside the bytes).
- `implementation_cost`: 972, a line in the corpus suite's configurations; 1234, a predicate in
  `suite_emission.hero`, about 30 lines; 2214, no code (a two-minute measurement).
- `needed_for_self_hosting`: no.
- `argument`: 972 cannot be measured on this Mac (LeakSanitizer unsupported on Darwin arm64,
  measured); it belongs to oracle 5 on the Linux leg for `extern` programs and to the corpus
  suite's missing `--sanitize -O0`. 1234's premise is re-measured: 679 of 693 blessed emissions
  pass `-fsyntax-only` and 14 lack a package's header. 2214 is answered: 336 of 709 `check` cases add
  no counter, which licenses no deletion, since a case's worth is the text it pins.
- `prediction`: when `emission` gains `-fsyntax-only` with the packages' include paths, it reads 0
  refusals on this Mac's trunk at that batch's gate.
- `condition`: a refusal among the 14 once their headers are found would move 1234 back into
  this milestone.

**No veto.** Nothing proposed adds a core construct or a line to `selfhost/` (design.md Part 5's
seven constructs are untouched), and the whole stays near the Pascal-P4 scale in the harness.

