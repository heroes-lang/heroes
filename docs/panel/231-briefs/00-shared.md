# Panel 231, shared brief: programs nobody wrote

**Convened 2026-10-28** in M-generated-programs (row 73), on tasks 931, 934, 972, 1234 and 2214
(https://github.com/heroes-lang/heroes/issues/931, /934, /972, /1234, /2214), after the catalogue
of task 929, measurement 056
(`docs/measurements/056-the-catalogue-of-shapes-enumerated-from-the-world-and-from-789-defects-of-our-own-the-c-boundary-151-of-581-depth-31-and-the-silent-class-30.md`).
By the author's yes of that day, asked once for the milestone (CLAUDE.md § 4): **the soundness lane**,
the compiler-engineer and the ffi-pragmatist, since the proposal is a program of the harness and
changes no surface, no diagnostic and no spec token. **And by the author's instruction of the same
day**, meant as *optimise optimistically: aggregate defects, aggregate panels, above all the very
long operations; take them for granted, do one*: **one sitting for the whole milestone's design**,
tasks 972, 1234 and 2214 inside it so no second sitting is convened for them, and **one pass of the
completeness critic, after the seats, over the briefs and the reports together**, where `/panel`
§ 3c asks two. What that gives up is said once: a false framing fact in these briefs is found by a
seat paying for it in its own time, or by the critic after the seats, not before them.

**The tree**: the trunk at `bd0c4477b` (step 1, the catalogue; `git status` clean). **Each seat works
in its own detached worktree at that commit**, made by the coordinator, its compiler already built
there in two stages: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (real 9.45 s, 9.41
s, 9.31 s in the three worktrees) and `./heroes build selfhost/main.hero -o heroes-next` (real 95.11
s, 95.12 s, 94.36 s, built side by side, so read as an indication); `heroes-next --version` prints
`sources 5e0ca4c634086a7c` in all three, the trunk's key. Judge with your own `heroes-next`. **Write
your report as you go**, from your first finding, at
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/231-reports/<seat>.md`, and nothing else in the
trunk. **No file outside the repository's root** (CLAUDE.md § Hard stops): your scratch is your
worktree or `.claude/worktrees/231-<seat>-scratch/`. No seat reads, builds or runs in another's
worktree. **No credential printed** (`gh auth status`, never `gh auth token`). **No paid run** of
any kind (no `claude -p`, no API call). **No clock**: a duration you need is written with its
command and marked an indication, since three worktrees build side by side on a machine of eight
cores; the timed run, if the sitting needs one, is the coordinator's.

## The proposal

The coordinator's draft, from measurement 056 § 3 and § 4 and the milestone's page; every line is
the sitting's to rule.

- **P1. Where it lives.** A program of the harness, `tests/harness/generate.hero` and modules beside
  it, run as the net is, `heroes run tests/harness/generate.hero -- <word> ...`, proposing no verb
  (CLAUDE.md § 10). Words: `make --seed N --count K -o <dir>` writes K programs, each with its
  `.expected`; `judge <dir> --compiler <path>` runs the oracles over them; `reduce <case> --compiler
  <path>` takes a failure down; `hunt --from S --count K --compiler <path>` loops the three outside
  the net, writing only what failed.
- **P2. From the type, the value as it is built.** The generator starts at *an expression of type T
  is needed* and descends among the forms T admits, so every program type-checks by construction; each
  node carries its value, so the program and its `.expected` are written together. **Two arms**: the
  clean arm picks operands that make every operation safe; the declared arm keeps one operation that
  aborts and expects the abort, as `tests/golden/run/` expects one (`!panic: <message>`).
- **P3. Policies**, YARPGen's (056 § 1.2): operator contexts over regions of an expression tree,
  constants near each width's edges and with runs of bits, a buffer of reused subexpressions, and
  parameters drawn per program.
- **P4. Two dials.** Depth: records inside variants inside maps, generics instantiated across `use`
  lines, expressions and blocks hundreds deep, recursion with a base path. The C boundary: an
  `extern` group generated with a header beside it, in the shapes spec § 13 admits, its C written so
  that its answers are known too.
- **P5. The five oracles** of the milestone's page: (1) the compiler does not fall over, no exit
  outside 0 and 1, no `internal error`, no `hero_unreachable` reached; (2) the answer is the computed
  one, in the configurations the net runs; (3) `heroes fmt` re-prints the program byte for byte; (4)
  what `heroes check` accepts, `heroes build` compiles and links; (5) no leak.
- **P6. The reducer**, from `selfhost/mutate/sites.hero`'s primitives: remove a declaration, a
  statement, an arm, replace an expression by a literal of its value, keep the edit while the
  symptom stays, until no edit keeps it.
- **P7. The net.** The free-running hunt stays outside it. What enters is deterministic: the reduced
  witnesses, committed under `tests/golden/` as cases of the form that judges them, and a fixed
  sample (seeds 1 to N, N priced by the sitting) regenerated and judged by a suite of its own.
- **P8. And the three tasks the author's instruction brings in**: 972 (`--sanitize` at `-O2` deletes
  the leaks LeakSanitizer exists to find), 1234 (nothing compiles or links what `--emit-c` wrote),
  2214 (which `check` cases add nothing, by coverage): each settled by measurement, folded into P5 or
  P7, or re-homed, with the reason.

## Facts, each with its command

- **The open work of the row** (`heroes run tests/harness/issues.hero -- open --milestone
  M-generated-programs`): 931, 934, 972, 1234, 2214, five tasks; 929 closed at `bd0c4477b`.
- **Nothing generative exists in the harness**: `ls tests/harness/ | grep -i 'gen\|fuzz\|reduce'`
  prints nothing; 103 modules (`ls tests/harness/*.hero | wc -l`).
- **What an abort prints today**, one program per class, each built with the trunk's compiler and its
  binary run (`.claude/worktrees/231-scratch/aborts/`, copy them): index past the end, `panic: array
  index out of range` / `  at index.hero:4:11, in index.main`, exit 134; `i64` max plus one, `panic:
  integer overflow` at `overflow.hero:3:9`, 134; `.must()` on an error, `panic: .must() on an error:
  odd: not even`, 134; `assert p == q` on two records, `assert failed: p == q`, its place, `left:  P(x:
  1, y: 2)`, `right: P(x: 1, y: 3)`, 134; a slice through `è`, `panic: string slice splits a
  character`, 134. **A recursion with no base path is refused at build**, not run: `down(n) -> i64`
  returning `down(n + 1) + 1` stops with the note *a call to `down` gives nothing back until the call
  it makes does* (`deep.hero` there), so a generator's recursion needs a path that returns first.
- **The run form's expectations**: an `.expected` holds the stdout, then optionally `!sanitizer:
  <needle>` and one ending, `!panic: <message>` or `!exit: <code>` (`tests/harness/suite_run.hero:105`);
  180 cases end `!panic` and 10 `!exit` (`grep -l '^!panic' tests/golden/run/*.expected | wc -l`).
  **It runs five configurations** (its header, lines 1 to 37): `-O0` built and run by the harness,
  `-O2` by `run`, `--sanitize`, **`--sanitize -O0` since 2026-10-07 (defect 390)**, and Guard Malloc
  on Darwin. A header beside a case is a `.h` of the same stem: 254 `.h` in `tests/golden/run/`, 19
  named `ffi-*` (`ls`).
- **The corpus suite's three** are still `["-O0", "-O2", "--sanitize"]`
  (`tests/harness/suite_corpus.hero:427-428`), over `examples/`.
- **`emission` compiles none of what it compares**: `grep -n "fsyntax-only\|link"
  tests/harness/suite_emission.hero` prints its comments at :82 and :87 and no call; task 1234's
  premise, measured on 2026-09-16 at 28 of 240 blessed emissions failing `-fsyntax-only`, is not
  re-measured here.
- **Parts that exist** (`wc -l`): `selfhost/mutate/sites.hero` 149, `selfhost/mutate/child.hero` 104
  (a compiler run as a child under the runtime's watchdog, `LIMIT` 30 s, every file it writes held to
  `CAP` 67,108,864 bytes), `selfhost/mutate/workers.hero` 178 (six threads, `JOBS`),
  `selfhost/cli/process.hero` 507, `tests/harness/shell.hero` 2,408; `runtime/hero_os.h` declares 58
  `hero_` functions (`grep -oE "\bhero_[a-z_0-9]+\(" runtime/hero_os.h | sort -u | wc -l`), among them
  `hero_run_go`, `hero_run_limit`, `hero_run_limit_writes`, `hero_thread_spawn`, `hero_thread_join`.
  **The recovery arm ran 1,356 programs, 14,209 singles and 15,737 pairs this morning in 27 to 28
  minutes of wall per compiler, six workers wide** (two compilers side by side; `real` 1,637 and
  1,658 s), which is the one throughput of `check` over many small programs measured on this Mac
  today.
- **A seeded stream**: `examples/montecarlo/main.hero`, `next_of(x)` at :36 and `inside_for(seed:)` at
  :41, with two tests at :85 and :88 saying why one seed must give one stream.
- **A module's ceiling is 300 code lines** (`tests/harness/suite_layout.hero:45-46`, `CEILING`).
- **The compiler's declared-impossible sites**: 84 calls of `hero_unreachable(` in `selfhost/**/*.hero`
  (`grep -rhoE 'hero_unreachable\(' selfhost --include='*.hero' | wc -l`).
- **design.md says nothing of a generator**: `grep -niE "fuzz|random program|generated
  program|csmith" docs/design.md` prints one line, 809, *a fuzz-tested C ABI*, among the things a
  compiler of this size could hold; § 1.12 is the warrant (`docs/design.md:590`), § 1.11 and § 4.19
  the boundary (`:482`, `:2522`).
