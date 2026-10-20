# Panel 226, the compiler-engineer's brief

Read `00-shared.md` first. Your copy: `<scratchpad>/226-compiler-engineer/`, `git archive` of
`a0a8d1249`; build your own compiler there (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, about 8 s; `./heroes build selfhost/main.hero -o heroes-next`, about a minute, is stage 2).
Never the trunk's `./heroes`, never another seat's directory. Write your report as you go to
`docs/panel/226-reports/compiler-engineer.md` **in the repository's lane**
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/`, not in your copy. No paid run, no
credential printed.

Your questions, in order, each answered by a command:

1. **R1, `build --permissive`: what does it cost, and where does each line land?** Today the flag
   is `check_flags`' alone (`selfhost/cli/table.hero:90-97`); the control parse exists
   (`parsed.control`, `selfhost/cli/check.hero:40-41`, `selfhost/parse.hero:32-39`); `kept_of`
   filters by `diag.is_thesis_rule` (`check.hero:281-291`). Read how `build` reaches the checker
   (`selfhost/main.hero`'s `compiling`, lines 267 to 288, and `selfhost/cli/rounds.hero`) and
   whether the checker's refusal paths leave a tree the lowering can take. **For each of the 27
   codes** (`selfhost/diag.hero:192-221`), write one row: the code, the site that emits it, whether
   the program is otherwise lowerable when the diagnostic is dropped, the runtime meaning the brief
   proposes (00-shared R1) or the one you would pin, and the lines it costs and in which module.
   Three codes have no obvious meaning (`marker_mismatch`, `ufcs_on_mutable`,
   `declaration_in_arm`): propose a sentence for each or say it cannot be given one and why.
2. **Prototype far enough that one number is real**: in your copy, make `build --permissive` accept
   `tests/golden/permissive/the-control-arm-reads-a-match-missing-a-case.hero` (a `match` missing a
   case) and run it; does it abort at the unnamed case (`hero_unreachable`,
   `runtime/parts/panic.c:39-47`, status 134 with `panic:` on stderr, and its message *this is a
   compiler bug, please report it*, which a permissive abort must not say: propose the message)?
   Report the diff's size (`diff -ru` against a second `git archive` of the tree, or `wc -l` of what
   you touched; your copy has no `.git`) and the compiler's own tests before and after
   (`./heroes-next test selfhost/main.hero`, count line). Judge with stage 2, `heroes-next`, never
   the seed-built binary: their `sources` keys differ.
3. **The seed**: does the flag enter `selfhost/` in a way that forces a seed refresh (a new form the
   seed must know, `.claude/rules/verification.md` § The batch), or does the two-stage build stay
   green? Run it: stage 2 from your edited `selfhost/` with your seed-built compiler.
4. **The grader's spawns**: `heroes check --json` writes schema 2 to stderr
   (`selfhost/cli/check_json.hero:1-17`): is the `code` list enough to bucket a refusal, and does a
   `???` program exit 0 with no binary (`check.hero`'s test *holes stay exit 0*)? `heroes test` has no
   per-test time limit (`selfhost/cli/verbs.hero:112-172`): confirm by reading, and say whether the
   harness's `hero_run_limit` on the parent reaches the test binary's children
   (`runtime/parts/run.c:238-296`).
5. **The harness side**: `shell.run_bounded(scratch, program, words, seconds, bytes, stdin_path)`,
   the coordinator's draft in a copy of its own (not in your tree): `spawned` takes the seconds,
   `run`, `run_fed`, `run_limited` and `attempted` pass `WATCHDOG_SECONDS`, a bound of zero is
   refused, and a test runs `/bin/sh -c 'sleep 30'` under one second and asserts 124. Judge it
   against `spawned` (`tests/harness/shell.hero:239-262`, the constant at 751): what it costs and what
   it misses. And the second root in `tests/harness/main.hero` (`ISSUE_PROGRAM`, line
   645, the test at 777 to 805): what `PROGRAMS: [str]` costs and whether one test per root is the
   shape.
6. **Principle 0 and the ceiling**: is `build --permissive` core or a flag the frontend erases? Does
   it add a construct the checker, the lowering and the backend must all handle, or a filter and a
   default arm? Your veto stands on a core construct or the ceiling, nothing else.

Output the structure your definition asks for, the 27-row table under `implementation_cost`, and a
prediction tied to a line count or to the first held-out run.
