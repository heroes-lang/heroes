# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model predict what a program does when it is built and run, and write the program its author meant,
on the first try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `spec.md`: the language's specification, the whole language.
- `task.md`: one task.

## Your method

Read the task and do what it asks. Write every file you write whole, with its path. For every place
where the specification left you a choice, say the choice you made and what the other would produce:
a program the compiler refuses, a program that stops with an error when run, a program that never
ends, or a program that runs and gives a different answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: what the task asks, in this order and under these names:
  - `main.hero`: the program, whole, in one fenced code block, and no other code block in `answer`;
  - `prints`: every line you expect the program to print, in order;
  - `compiles`: *yes* or *no*, and if no, which line and why;
  - `parameters`: for each function the program binds, each of its parameters and why it is written
    so.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
