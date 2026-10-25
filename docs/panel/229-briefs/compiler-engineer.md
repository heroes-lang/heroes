# Panel 229, the compiler-engineer's brief

Read `00-shared.md` beside this file first, then the critic's first pass,
`../229-reports/critic-briefs.md`; every fact either states carries its
command, and a fact you find false is a finding for your report. Repaired
after that first pass, like the shared brief.

Your seat judges cost, the soundness of the instrument and what each route
does to the probe's power to find a formatter defect. You build and you
measure, in your own copy of `01c5efd75`.

## What to build and run

1. **Route (E), as far as it goes.** Starting from `01c5efd75` (which already
   holds `written_pairs`' repair), make `heroes fmt` near-linear on a comment
   inside a 2,000-long chain written as one statement: the `bare.needs_group`
   / `part_needs` recursion (one level per index, each level asking
   `elements.extent`, `around.untaken`, and at its base rendering the whole
   subtree), `brackets.spread`, `breaks.loose`, `parens.render`,
   `breaks.plan`, `aligned.aligned`, profiling after each step with `sample`.
   **Every step must leave every output the same bytes**: format every tracked
   `.hero` file with the compiler of `01c5efd75` and with yours and compare
   (`git ls-files '*.hero'`, `shasum`), and run `probe`, `surface` and the
   compiler's own tests in your copy. Report, per step, the lines of
   `selfhost/print/` it touches, `heroes fmt` on 259's line-18 variant with a
   comment after the 1st, 200th, 1,000th and 1,990th `[` (the critic's builders
   are in `229-critic/m/`), and 20 judged variants of 259 at `--stride 4202`.
   Say where you stopped and why: a step whose seam you could not find is a
   finding, not a failure.
2. **Route (F)**: the wall time of the run the rule owes on this Mac's 8
   cores, split per seed or per stride offset across processes, measured on a
   sample and multiplied with the multiplication said; and whether `heroes
   probe` needs anything new for it (an offset beside `--stride`, a root split
   by the caller) or nothing.
3. **Routes (A), (B) and (C)**, priced against (E) and (F): the bound or the
   class rule you would write, its variant count over the tree and over 259,
   264 and 169-chains, and what it gives up, measured.
4. **The class the probe must not lose**: defects 503 (issue 1849) and 506
   (issue 1852) and panel 179's 17 (all `bracket`, T/O/B): would each route
   still generate the variant that found each? Where a route drops variants,
   run the cases on the compiler of the day the defect was found
   (`git worktree add --detach <commit> <dir inside your directory>`, a seed
   build) rather than argue it.
5. **Which item of panel 179 each route changes**, its file's lines 128 to
   172, or none; a route that changes none is a defect's repair and the
   sitting says so.

## What your report states

The route or combination you would adopt and why, with its numbers (CPU hours
and wall hours of the owed run, each measured or multiplied as said); the
routes you refuse with the measurement that refuses them (a veto is a refusal
on soundness, not a price); the lines of `selfhost/` each touches; what each
gives up, measured; and your predictions, each with the command that would
falsify it. Your prototype's diff goes in your report (or a patch file beside
it in your directory, named in the report), so the landing can start from it.
