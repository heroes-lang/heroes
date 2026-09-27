# Panel 179, brief for the compiler-engineer

Read `00-shared.md` first. Your seat judges implementation cost and
core-versus-sugar (design.md §1.1, §1.7, Part 5), and you have veto power.

## What you decide

1. **The shape under the stopping rule.** Is the probe a flag on `fmt` (it
   answers *how the formatter behaves on this input's neighbourhood*) or a
   subcommand (its artifact is a report over generated programs, a different
   class)? Argue from `.claude/rules/cli-surface.md`'s own definitions, and say
   what the help text would print for each.
2. **Where it lives and what it costs.** The probe needs a generator (tokens of
   the input, positions, the deformations), a runner (format each variant in
   process, not by spawning `heroes fmt` 100,000 times) and a judge (the four
   checks `run_fmt` already makes at `83ac68c1`, plus the owner-block and
   neighbour-token comparison `print/anchors.hero` already computes). Name the
   modules it would be, each under the 300-line ceiling, and what they reuse
   from `print/owners.hero`, `print/anchors.hero` and
   `cli/syntax_cmds.hero`. Read the recovered Python generators: they are the
   specification of the deformations and the independent judge.
3. **Prototype far enough that the number is real.** In your own copy of
   `83ac68c1`, write the generator's first cut in Heroes (the comment-at-every-
   position deformation and the bracket-break deformation are enough), run it
   on ONE seed file (`selfhost/check/walk.hero`), and report: how many variants
   it produces, how many parse, how many the formatter refuses or moves, and
   how the count compares with the Python generator run on the same file
   (`python3 skeptic-g4__tokgen.py`-style runs need the lane's `heroes lex
   --dump-tokens --json`; read `rd.py` for how they lex). Rebuilding the
   compiler from `selfhost/` costs about a minute from an `-O2` seed (panel
   177's measurement, carried); you can afford it.
4. **The cost of running it.** In process, one variant costs one format plus
   one parse plus one anchor comparison. Estimate the probe's time on one 300-
   line file from counts you measure (variants per file, and the work per
   variant as instructions or as lines formatted), not from a clock, unless the
   machine reads a load under 2 (`uptime`). Say whether the whole-tree run
   belongs in the net or in a nightly, and why.

## Prediction

Register one falsifiable prediction with an instrument that exists today, for
example: the Heroes generator produces within N% of the Python generator's
variant count on `walk.hero`; or the in-process probe over the 38 fixtures of
`tests/golden/surface-fixtures/comments101/` runs under M seconds on a quiet
machine. Say when it is scored.

## Your directory

`<scratchpad>/179-compiler-engineer/`, copied from
`git -C /Users/joseph/Temp/heroes-lane-gm archive 83ac68c1`. Build your own
compiler from the seed there. Write your report to
`<scratchpad>/179-compiler-engineer/report.md` and a copy to
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-179/compiler-engineer.md`.
Cite files and line counts you measured. English, no em dashes.
