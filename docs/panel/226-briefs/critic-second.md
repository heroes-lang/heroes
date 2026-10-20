# Panel 226, the completeness critic's second pass: what the five reports leave out

You read the five reports and the six briefs of sitting 226 and name **what is missing**. You are
not a sixth judge and give no verdict (`/panel` § 3b). Your copy of the tree is the one your first
pass made, `<scratchpad>/226-critic/`, with its stage 2 compiler `heroes-next`; build nothing in
another seat's folder, print no credential, start no paid run (the brief of your first pass says
how and why). Write your report **as you go** into `docs/panel/226-reports/critic.md` in the lane
(`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis`), so a stall leaves what you had;
every number you write is one a command of yours produced, with the command beside it, or it is
marked *unrun* in your own words (CLAUDE.md § RUN IT).

## What you read

- `docs/panel/226-briefs/00-shared.md` (the proposal and the thirteen questions R1 to R13), the
  five seats' briefs beside it, and your own first pass, `226-reports/critic-briefs.md`.
- `docs/panel/226-reports/compiler-engineer.md` (object, no veto; 27 witnesses, the prototype of
  `build --permissive`), `ffi-pragmatist.md`, `spec-warden.md` (approve, conditions), `historian.md`
  (approve, advisory, four conditions), `llm-ergonomist.md` (approve, blind; three programs, 25
  choice points).
- The coordinator's draft resolution, which you are the last reader of before it is written into
  the tree: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis-drafts/coordinator-work/226-metrics-2-and-4-measured-by-one-instrument-three-arms-forked-from-one-generation-the-control-arm-built-and-run-and-the-held-out-tasks-sealed-in-the-tree.md`
  (its § Resolution, R1 to R13, and § What was measured, the coordinator's own numbers). Its two
  gaps marked `<<…>>` are what your pass and the ffi-pragmatist's report fill.

## The four questions you answer, each with a command or an *unrun*

1. **A route nobody listed.** For each of R1 (the control arm's 27 meanings: the compiler-engineer's
   condition (a), the complete table, is adopted; (b), the third category, is the conservative
   route), R2 (the primary and its test), R9 (route A against B) and R10 (the opening trigger and
   the CI secret): what third route exists that no seat and no brief named, and what would have to
   be true for it to be better? Name it or say *none found, searched for <what>*.
2. **A claim asserted and not measured.** Read every number in the five reports and in the draft's
   § What was measured; for each one with no command beside it, say so by file and line. In
   particular: the compiler-engineer's reading of the signal forwarding into a test binary (its own
   *unrun*), the historian's `verified in part` lines, the llm-ergonomist's predicted percentages
   (predictions, not measurements, which is their role), the draft's *445 KB*, *4 canaries*,
   *exit 153*, *Invalid argument*, *39 tests*, *92 tests*.
3. **A contradiction between seats, with which is checkable.** At least these three: the
   compiler-engineer says `bound_unit` costs *unrun* lines through `mono` and the draft adopts it
   with its cost *measured in the landing step*; the historian says both providers' consumer terms
   forbid automated access outside the API, and the ffi-pragmatist's `claude -p` probe ran on this
   Mac (under which authentication, and does its report say?); the spec-warden says the five live
   ids count the spec identically and the brief said 10,294 on four of them against 10,292 on
   `claude-opus-5`. For each, which is checkable by a command in your copy, and run it where it is.
4. **The question the sitting should have asked and did not.** The one you would put to a sixth
   seat. Candidates you weigh and may reject: whether the trial's judge-only control (R1's interim)
   can leak into the held-out run's pre-registration; whether a model's program can detect the
   sandbox (`sandbox-exec` leaves a trace a program can read?) and behave differently; whether the
   per-task canary line in `task.md` changes what the first-try rate measures (a line of noise in
   every prompt); whether `green@3`'s denominator should drop the samples a transport lost
   (`api_refusal`, `transport_error`) or count them as not green; whether the `brief` arm's
   feedback on a program that compiles and fails the example is the same text as the strict arm's
   (it is, by R5: say whether that makes the two arms differ at turn 1 alone).

## What you do not do

You do not rewrite the resolution; you name what it misses and the coordinator decides. You do
not re-run the seats' measurements except where question 3 asks which of two is right. You run no
model and no `claude -p`. Your report's last section is a list, one line each, of the repairs you
ask of the draft, by its section, so the coordinator can take them one by one and say in the
synthesis which were taken.
