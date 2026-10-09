# Your brief

You read a program in a small programming language and say what its tools do
with it. Your input is the files of this directory and nothing else:
`spec.md`, the language's specification (read it in full first), the
program's `.hero` files and the C headers they name. Read no file outside
this directory and use no tool but reading and writing files here; you cannot
run anything.

**The task**: say, for each of `heroes check main.hero`, `heroes build
main.hero -o main` followed by `./main`, and `heroes test main.hero`, the
exit code and everything it prints, and why. Be concrete about the values.

Write `report.md` here as you go, with exactly these headings: `check`,
`build_and_run`, `test` (each: exit code, output, the sentences of the
specification or the C facts your answer rests on), `choice_points` (where
the specification left you a choice, the choice you made, and what the other
would produce), `confidence`, and `context` (whether anything other than
this directory's files reached your context, and what). English, no em dashes.
