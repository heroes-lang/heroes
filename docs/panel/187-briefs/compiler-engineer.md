# Panel 187, the compiler-engineer's brief

Repaired 2026-10-02 after the completeness critic's first pass; the text it
read is `compiler-engineer-before-the-critic.md`.

Read `00-shared.md` and `00-facts.md` in this directory first, whole;
`00-shared.md` § Measured on the sitting's head wins where the two differ.

## Your input
The live compiler in your own copy `<scratchpad>/187-compiler-engineer/`
(`git archive 07ccb72a | tar -x`, its compiler from the seed, whose sha256
prefix `00-shared.md` § Measured on the sitting's head gives; check yours).
The recovery lives in the parser and the lexer: `selfhost/parse/`
(`opening.hero`'s `drop_line`, `apart.hero`, `statement_end.hero`,
`headless.hero`, `top_level.hero`, `swallowed.hero` and their neighbours),
`selfhost/lexer.hero`, `selfhost/open_line.hero`, `selfhost/line_above.hero`,
`selfhost/bracket_reach.hero`; count each in
`tests/harness/suite_layout.hero`'s unit, not `wc -l`. The audit cases are
`<scratchpad>/audit-130-133/cases/` (copy them); the critic's re-run script
over 147 files and its probes are `<scratchpad>/187-critic/rerun-critic.py`
and its inputs (copy them), and its experiment on `drop_line` is
`<scratchpad>/187-critic-p166/` (read it; do not build there).

## Your task
1. For each class of open row (`00-facts.md` § 3: the eight (a), the five
   (b), and 166's shapes as § Measured on the sitting's head reads them),
   name the code path that produces it and whether one cause covers several
   rows. Measure, do not read: run each row's case on your compiler, and
   instrument a copy where you must.
2. **Q3's local route**, measured by the critic on `62d65e48`: one condition
   of `drop_line` made never true (`parse/opening.hero` line 310 there).
   Confirm or refute it on `07ccb72a`: the 147 files, the compiler's own
   tests, and **the full net in your copy** (`./heroes run
   tests/harness/main.hero -- ./heroes`, about 20 minutes; at most three
   processes, so run it alone), and say what the condition was for, by its
   own comment and by the golden `fixedbugs-131-a-body-in-a-tab-margin.hero`.
3. **Build** the route you would adopt for Q1 (and Q3 if your route is
   (1d)): if (1b), the suite that holds the instrument's totals, answering
   `00-shared.md` § The recovery instrument's four facts (where it lives in
   the tree, which corpus, its cost, its baseline); the instrument is
   `<scratchpad>/instrument/`, ask the coordinator to run it on your
   compiler with its size rather than running it yourself; if (1d), the
   structural change in your copy, with every audit row and the compiler's
   own tests run on it; if (1f) or (1g), the goldens or the loop, built. A
   route that does not build is not adopted.
4. **`g2/r09`** (Q4): which code path writes its second message, what
   location it has and why, and whether a message naming the function whose
   body is missing, or none at all, is within reach.
5. Its cost, in lines per file and in a build's time if it adds work per
   parse (measured `user` time, said to be on a busy machine).
6. A verdict per route of Q1 to Q4, approve, object or veto, with what you
   built or measured for each.

Write `docs/panel/187-reports/compiler-engineer.md` in the trunk as you go.
