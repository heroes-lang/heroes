# Your brief

You write a program in a small programming language. Your input is the files
of this directory and nothing else: `spec.md`, the language's specification
(read it in full first). Read no file outside this directory and use no tool
but reading and writing files here; you cannot run anything.

**The task**: write a complete program with two functions. `longest(words:
[str]) -> str` returns the longest string of a non-empty array, the first one
where two are equally long. `main` prints `longest(["ab", "abcd", "xyz"])`.
Write it so that the language's checker accepts it on the first try, as the
specification describes the checker.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the program you would submit, whole, in one fenced block),
`sentences` (every sentence of the specification your program rests on,
quoted, and for each the line of your program it governs; in particular the
sentences that say how a value that changes during a loop is declared and
how it is changed), `choice_points` (every place the specification left you a
choice, the choice you made, and what the other choice would produce),
`confidence` (how sure you are that the checker accepts the program, and of
which line least), and `context` (whether anything other than this
directory's files reached your context, and what). English, no em dashes.
