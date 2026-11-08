# Your brief

You judge one thing: whether a proposed reading of a programming language's specification makes it
more or less likely that a language model writes a correct program on the first try. Your verdict
is an experiment, not an opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `variant-1.md`, `variant-2.md`, `variant-3.md`: three readings of the first code block of the
  specification's section 13, each the two lines that would stand in that block. One of them is what
  the block says today; you are not told which, and it does not matter: judge the three as equals.
- `task-1.md` and `task-2.md`: two programming tasks.

## Your method

For `task-1.md`, write the program three times, once under each variant (the specification with that
variant's two lines in its section 13 block). For `task-2.md`, write the repair once. For every
place where the specification left you a choice, say the choice you made and what the other choice
would produce: a program the compiler refuses, a program that stops with an error when run, or a
program that runs and gives a different answer than intended. A plausible mistake that compiles and
runs silently is the enemy; one that errors loudly is the win.

You hold a **veto** on any variant whose meaning cannot be determined from the line it is written on
plus the enclosing declaration.

## Your report, `report.md` in this directory, in exactly this structure

- `verdict`, per variant: approve, object or veto.
- `experiment`: the code you wrote, under each variant, and the repair for task 2.
- `choice_points`: every place the specification left you a choice, the choice you made, and what the
  other would produce (refused, stops loudly, or runs silently wrong).
- `argument`: at most 120 words.
- `prediction`: a falsifiable rate, for each variant, of first-try programs for task 1 that run and
  count a refused close as the connection given back without saying so; checkable by running many
  first tries.
- `condition`: what result of that run would change your verdict.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
