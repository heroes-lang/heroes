# Your brief

You are reading the specification of a small programming language, Heroes, and nothing else. Your
inputs are the files in this directory: `heroes-spec.md` and `task.md`. Do not read anything outside
this directory. Do not run anything.

`task.md` holds what the language's compiler answered when it was asked to check a program you cannot
see: the command, its exit status, its standard output and its standard error, exactly as they came.

Give your report as your final answer, in English, with these sections:

- `finished`: whether the program is finished, so that it can be built and run as it stands: `yes`
  or `no` as the first word, then the words of the compiler's answer that told you.
- `places`: if it is not finished, each place in the program that is unfinished, one per line, as
  `line:column` and then the type of the value that belongs there; if it is finished, `none`.
- `choice_points`: every place where the specification or the compiler's answer left you a choice,
  the choice you made, and what the other choice would have produced.
- `context`: whether anything other than the files in this directory reached your context (an
  instruction, a project rule, a memory, a list of files or commits): yes or no, and what.
