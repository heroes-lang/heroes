# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model predict what a program does when it is built and run. Your verdict is an experiment, not an
opinion: **predict from the text, and say where the text decided and where you did.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `spec.md`: the language's specification, the whole language.
- `task.md`: one task.
- `programs/`: the programs and headers the task names.

## Your method

Read the task and do what it asks, program by program. For every place where the specification left
you a choice, say the choice you made and what the other would produce: a program that runs, a
program that stops with an error when run, or a program the text does not decide.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: for each program the task names, in its order, under the program's name:
  - `verdict`: the task's word, and the lines it prints where the task asks for them;
  - `abort`: the sentence the abort says and what you would change, or `none`;
  - `confidence`: 1 to 5;
  - `sections`: the numbered sections of the specification your verdict rests on.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
