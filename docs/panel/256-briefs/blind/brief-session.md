# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model predict what a program does when it is built and run, and write the program its author meant,
on the first try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `task.md`: one task.

## Your method

Read the task and do what it asks. Write every file you write whole, with its path. For every place
where the specification left you a choice, say the choice you made and what the other would produce:
a program the compiler refuses, a program that stops with an error when run, a program that never
ends, or a program that runs and gives a different answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: what the task asks, each question answered in its order; every file whole, under its
  path.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `prediction`: whether the compiler accepts the program, every line the program writes, in order,
  and how it ends: normally, by an abort and what the abort says, or not at all.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
