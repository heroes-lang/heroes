# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model predict what the compiler does with a program and write the program its author meant. Your
verdict is an experiment, not an opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `task.md`: one task.

## Your method

Read the task and do what it asks. Write every file you write or change whole, with its path. For
every place where the specification left you a choice, say the choice you made and what the other
would produce: a program the compiler refuses, a program that stops with an error when run, or a
program that runs and gives a different answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: what the task asks, and every file, whole, each under its path.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `prediction`: what each program prints when built and run, and whether you expect the compiler to
  accept it.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
