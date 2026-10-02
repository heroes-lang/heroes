# Panel 187, the compiler-engineer's brief

Read `00-shared.md` and `00-facts.md` in this directory first, whole.

## Your input
The live compiler in your own copy `<scratchpad>/187-compiler-engineer/`
(`git archive <head> | tar -x`, its compiler from the seed). The recovery
lives in the parser and the lexer: `selfhost/parse/` (`opening.hero`'s
`drop_line`, `apart.hero`, `statement_end.hero`, `headless.hero`,
`top_level.hero`, `swallowed.hero` and their neighbours), `selfhost/lexer.hero`,
`open_line.hero`, `line_above.hero`, `bracket_reach.hero`; count each in
`tests/harness/suite_layout.hero`'s unit, not `wc -l`. The audit cases are
`<scratchpad>/audit-130-133/cases/` (copy them).

## Your task
1. For each class of open row (`00-facts.md` § 3: the eight (a), the five
   (b), and 166's four shapes), name the code path that produces it and
   whether one cause covers several rows. Measure, do not read: run each
   row's case on your compiler, and instrument a copy where you must.
2. **Build** the route you would adopt for Q1 (and Q3 if your route is
   (1d)): if (1b), the suite that holds the instrument's totals (what it
   runs, how long, on which corpus; the instrument is
   `<scratchpad>/instrument/`, ask the coordinator to run it on your
   compiler with its size rather than running it yourself); if (1d), the
   structural change in your copy, with every audit row and the compiler's
   own tests run on it. A route that does not build is not adopted.
3. Its cost, in lines per file and in a build's time if it adds work per
   parse (measured `user` time, said to be on a busy machine).
4. A verdict per route of Q1 to Q4, approve, object or veto, with what you
   built or measured for each.

Write `docs/panel/187-reports/compiler-engineer.md` in the trunk as you go.
