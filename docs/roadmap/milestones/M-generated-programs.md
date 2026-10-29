# M-generated-programs — programs nobody wrote


**Scheduled by author instruction 2026-09-06** — § Who scheduled what carries the
instruction, the four choices the author made and the measurements that placed the
row. The warrant is **§1.12**: a Heroes program must not segfault, and the
compiler is a Heroes program. Defect 007 is that sentence failing on a **valid**
file, `heroes check` at exit **139** with nothing on either stream.

**What it delivers.** A generator, written in Heroes, that composes programs
**valid by construction** and **knows what each one must print before the compiler
is asked**; five oracles over every program it writes; a reducer that takes a
failure down to something a person can read; the defects that come out, repaired
at the class rather than at the witness; and a committed corpus of the reduced
witnesses in the net, which is what keeps them shut.

**Its first step is a catalogue, and it is enumerated from the world** (CLAUDE.md
§1, which says a list names where it came from). Csmith and YARPGen state in their
own papers which classes of bug they found — YARPGen's count is **more than 220**
in GCC, LLVM and the Intel compiler — a survey of compiler fuzzing exists, Zig
carries an issue titled *Compiler crashes found with fuzzing*, Go keeps
`test/fixedbugs` and Nim its `tests/`. The nearest corpus of all is this
repository: **fourteen** defect entries in `docs/work/DONE.md` and **76**
regression cases named after one — 49 with the `fixedbugs-` prefix under `check/`,
`run/` and `unsupported/`, 27 in `tests/golden/fixedbugs/`. The catalogue is a
`docs/measurements/` file naming each shape with its source and marking what was
read and what was not.

**The two moves that make it more than a test of the lexer**, both borrowed, both
named here so the milestone does not re-derive them. Generation goes **from the
type, never from the text**: it starts at *an expression of type `i64` is needed*
and descends, choosing at each node among the forms that type admits, so the
program type-checks by construction and a refusal from `heroes check` is itself a
defect. And **the answer is computed while the program is built**: every node
carries its value, so the generator writes the program and its `main.expected`
together, with no second compiler standing in as judge. That is Csmith's checksum
trick, and it is the only thing that makes the silent class — exit 0, wrong number
— visible at all.

**Heroes has no undefined behaviour, and that changes the generator's job.** An
overflow, a division by zero and an index out of range all abort by design, so
there is nothing to steer around the way YARPGen must for C; there is a choice to
declare instead. The clean arm picks values that make the operation safe by
construction. A smaller, declared arm expects the **abort and its message**, which
is how the guard rails get checked rather than assumed.

**The five oracles**, all of them, by author decision 2026-09-06 against the two
narrower options offered: (1) the compiler does not fall over — no 139, no 134, no
`internal error`, no silent exit, and every `hero_unreachable` reached is a defect,
of which `selfhost/` holds **63**; (2) the answer is the computed one, in the three
configurations the net already runs — `-O0`, `-O2`, `--sanitize`
(`tests/harness/suite_corpus.hero::configurations()`) — whose disagreement is the
only differential arm there is while there is one backend; (3) `heroes fmt`
re-prints it byte for byte, the class that produced defects 003 and 004, one of
which rewrote a compiler source into a different program at exit 0; (4) what
`heroes check` accepts, `heroes build` compiles, which is M-check-completeness's
promise put under a volume nobody writes by hand; (5) no leak, with the Linux leg
under `--sanitize` for whatever declares an `extern` (CLAUDE.md § Commands, where
LeakSanitizer is the reason that leg exists).

**The two hard shapes are the author's own, and the record agrees with them.**
The C boundary — generated `extern` groups against a header generated with them:
structs by value, `@` out-parameters, callbacks, `cstr`, `owned` — is where four
of the fourteen entries are (010, 013 and both 014s). Depth — records inside
variants inside maps, generics
instantiated across `use` lines, expressions hundreds of levels deep — is
defect 007.

**Where it lives, and what it must never become.** A Heroes program under
`tests/harness/`, run by `heroes run` as the net is, so §10's stopping rule is
untouched and no verb is proposed. The long hunt stays **outside** the net: a
suite that generates at random goes red at random, and this project has already
paid for an instrument nobody trusts. What enters the net is the committed,
reduced corpus, deterministic under a seed. The reducer starts from
`selfhost/mutate/sites.hero`, split out as *the primitives every mutation operator
is built out of: a text edit, a span, and the four questions about a tree node*.
A seed is an integer, and `examples/montecarlo/main.hero` already carries a
deterministic stream with a test saying why — so a defect is reported by its
number and anybody can reproduce it.

**Why here.** After the four verdict milestones, because a generator has to know
the final surface; after M-check-completeness, whose promise is oracle 4; after
M-panic-location, because a crash that names its file, its line and its function
is the difference between a triage of minutes and one of hours. Before the books,
the channels and the gate, so that what gets written about and shipped is a
compiler that has been shot at. **The sixth oracle is dated rather than
promised**: when M-qbe-backend exists, the same generated program through two
backends is differential testing in the full sense, and that is the one thing this
milestone deliberately leaves to a later one.

**Corrected 2026-10-14.** `docs/work/DONE.md`, which the paragraphs above cite,
was deleted on 2026-10-05; its entries read at `c911dd61`, the last commit
that held the maps.

**A comparison read on 2026-10-25, and one question it leaves for this row.**
Goose, another language whose compiler a model writes
(github.com/aardappel/goose), checks one of its compiler's passes with
programs it generates and needs no answer for: its implementation notes § 5.11
say a run of random programs "through the JIT with the pass on and off" found
the pass's "former misreading of an inlined body with several returns ...
within a few hundred programs". The same shape here is a second oracle beside
the known answer this row plans: one generated program built at `-O0`, at
`-O2` and under `--sanitize` must print the same thing, which needs no answer
at all and names a class the sanitizers do not, an optimiser taking an
assumption the emitted C makes.

**Opened 2026-10-28, at row 73, on the trunk**, by `/step` with no step named,
the ROADMAP's next row; in no lane yet, since the opening writes records alone
and the four other sessions on this Mac were offline. What the opening
measured, with the trunk's stage 2 (`sources 5e0ca4c634086a7c`), so that the
steps start from it:

- **Seven tasks open** on the GitHub milestone (`open --milestone
  M-generated-programs`) and no feature: 929, the catalogue, the first step;
  931, the generator computing the answer as it builds; 934, the reducer and
  the committed witnesses; 972, `--sanitize` at `-O2`, where clang deletes the
  leaks LeakSanitizer exists to find; 1234, nothing in the tree compiles or
  links what `--emit-c` wrote; 2214, which `check` cases add nothing, a
  coverage measurement; 2236, what M-panic-location's push left to read, read
  first and closed by this opening.
- **What M-panic-location's push left, read** (task 2236). The CI's run
  40401936020 on `e4ad8e14c`: Linux x86-64, Linux arm64 and Darwin arm64
  green, Windows x86-64 red at *Build the compiler from the seed*, defect 2239,
  repaired at `c3996424f` and closed at the tag; the run after it, 40456967468
  on `e9857f5ea`, red on both Linux legs at `emission: 1399 passed, 2 failed`,
  defect 2240, the two emissions this Mac skips blessed at `24f33f34a` and
  closed; the run on `88af6ca60` cancelled by the push seven minutes behind
  it; and run 40474270628 on `f867b7552`, the trunk's head, green on all four
  legs, CodeQL and the site's deploy beside it. Panel 187's R2, not run since
  M-readonly-lend's push on 2026-10-18 (journal 068 declaring it not owed,
  069 and 070 silent), run over everything since:
  moved 0 and findings 0 in both arms, 14,209 singles the same in the normal
  and the control arm, 15,737 pairs with the same told status, each arm 27 to
  28 minutes of wall six workers wide beside the other (`real` 1,637 and
  1,658 s, `user` 4,822 and 4,852 s). Three programs of the frozen corpus,
  unmutated, are told one message by today's compiler and none by the base,
  the language having moved under the freeze, and are subtracted as the
  instrument subtracts a program's own: `examples/interpreter/main.hero:63`,
  `type_mismatch` in both arms, `exit(code:)` taking a `u8` since panel 230's
  R4, the live example converting and the frozen copy not;
  `selfhost/cli/io.hero:123`, `wrong_label` in the normal arm alone,
  `repeat(count: 512)` where a built-in's unnamed position takes no label
  since defect 2184 (`be9009cac`, M-thesis-harness step 12, the round journal
  069 did not read); and the two ledger programs told `never_rebound` at
  `examples/ledger/db/sqlite.hero:322` by both compilers, panel 210's rule, as
  on 2026-10-18. Today's records (2026-10-28) stay in
  `.claude/worktrees/r2-runs/today` on this Mac, the base the next reading can
  reuse. The author's counts W4 and W5 of panel 230 are a decision issue, https://github.com/heroes-lang/heroes/issues/2241, with the
  two commands that print each number alone.
- **The nearest corpus, re-measured**, and task 929's figures of 2026-09-06
  and 2026-09-10 are low by an order of magnitude: 1,341 `fixedbugs-*.hero`
  cases under `tests/golden/` (`find tests/golden -name 'fixedbugs-*.hero'`),
  541 in `check/`, 396 in `run/`, 329 in `unsupported/`, 39 in `full/`, 19 in
  `permissive/`, 5 in `ir/`, 5 in `emit/`, 3 in `lldb/`, 3 in `fixedbugs/`
  and 1 in `control/`, with `tests/golden/fixedbugs/` holding 40 `.hero` files
  in all; 649 distinct `defect NNN` citations in `selfhost/`, `runtime/` and
  `tests/`; and on GitHub, where the defects live since 2026-10-12, 789
  defects, 785 closed and 4 open (2 `adjacent`, 2 `improvement`, no
  `blocking`, no `systemic`), of 2,240 issues (`gh api search/issues`, read
  this morning).
- **The places the compiler declares impossible, counted by `grep -rn` over
  `selfhost/**/*.hero`, lines rather than sites**: `hero_unreachable` 94,
  `unreachable` 207 (the 94 among them), `internal error` 94, `impossible` 11,
  `cannot happen` 5; task 929's re-verification of 2026-09-10 read 119, 27, 9
  and 4, and its warning stands: the catalogue's first act is to define what
  it counts before it counts it.
- **The premises the row rests on, re-read in the tree**: the net's three
  configurations are still `["-O0", "-O2", "--sanitize"]`
  (`tests/harness/suite_corpus.hero`, `configurations()`), and no fourth
  `-O0 --sanitize` arm exists (task 972); the seeded stream is still
  `examples/montecarlo/main.hero`'s `inside_for(seed:)`;
  `selfhost/mutate/sites.hero` is 149 lines, as on 2026-09-10, with
  41 modules beside it, the recovery arm's workers and its child
  bounded in time and bytes among them, parts a generator's runner and a
  reducer can reuse; the net selects 26 suites by name and six golden forms,
  none generative, and `tests/harness/` holds 103 modules and no
  generator.
- **What this opening did not do, and why**: no lane and no panel. The
  catalogue is a measurement under `docs/measurements/`, whose next number is
  056, and touches no panel path; the generator and the reducer are a Heroes
  program under `tests/harness/`, which proposes no verb, so § 4's gate is
  asked once, at step 1's end with the catalogue in hand, the soundness lane
  recommended, and the lane opens with the first code.

**Corrected 2026-10-28, by panel 231** (the completeness critic's finding 17). The oracles paragraph
above says *(1) the compiler does not fall over — no 139, no 134*; panel 184's R6 and R7, ratified
on 2026-10-01, made two exits designed answers on a correct program: exit 2 where clang dies on
its C, told in the compiler's words, and exit 134 with `stack exhausted in` past the depth floor,
which this milestone finishes stating. Oracle 1 is read in five since the sitting
(`docs/panel/231-programs-nobody-wrote-the-generator-its-oracles-its-reducer-and-what-it-leaves-in-the-net.md`,
R5), and the reducer works over the generator's own tree rather than from
`selfhost/mutate/sites.hero`, which no program of the harness can `use` (R6).

**Ratified 2026-10-29** by the author's reading, panel 231 as it landed
(https://github.com/heroes-lang/heroes/issues/2243). **Closed the same morning**,
[072](../../records/journal/072-generated-programs.md): the full net 10,254 and 0, pushed at
`cb8a6b3d9`; decision 2241 answered by the author's counts of the sealed run, W4 0 of 480 and W5 3
of 480 (https://github.com/heroes-lang/heroes/issues/2241). **Tagged that afternoon**,
`m-generated-programs` on `de2353e17`, once defects 2230 and 2238, `adjacent` at their second tag,
were repaired at steps 21 and 22 and the tag's gate read the full net 10,217 and 0, pushed at
`b0bfd2690`. What it carried forward is https://github.com/heroes-lang/heroes/issues/2245 and
https://github.com/heroes-lang/heroes/issues/2250, on row 74, and the `adjacent`
https://github.com/heroes-lang/heroes/issues/2242.
