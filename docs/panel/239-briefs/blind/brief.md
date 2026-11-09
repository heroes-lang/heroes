# Your brief

You judge one thing: whether a compiler's message, read with no other file but the language's
specification and the program, leads a language model to the repair its author meant, on the first
try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `task.md`: one task, a program of several files given in it, and what the compiler said of it.

## Your method

Read the task. Write the repair: every file you change, whole, and which files you leave. For every
place where the specification or the message left you a choice, say the choice you made and what
the other would produce: a program the compiler refuses, a program that stops with an error when
run, or a program that runs and gives a different answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `repair`: the files you change, whole.
- `choice_points`: every place the specification or the message left you a choice, the choice you
  made, and what the other would produce.
- `prediction`: what your repaired program prints when built and run, and whether you expect the
  compiler to accept it.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
