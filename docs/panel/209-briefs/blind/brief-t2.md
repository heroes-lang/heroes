# Your brief

You write a program in a small programming language. Your input is the files
of this directory and nothing else: `spec.md`, the language's specification
(read it in full first). Read no file outside this directory and use no tool
but reading and writing files here; you cannot run anything.

**The task**: write a complete program with two functions. `tally(words:
[str]) -> {str: i64}` returns how many times each word of the array appears.
`main` binds `t = tally(["a", "b", "a"])` and prints `t["a"].default(0)`.
Write it so that the language's checker accepts it on the first try, as the
specification describes the checker.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the program you would submit, whole, in one fenced block),
`sentences` (every sentence of the specification your program rests on,
quoted, and for each the line of your program it governs; in particular the
sentences that say how an empty container is declared and how a value that
changes is declared and changed), `choice_points` (every place the
specification left you a choice, the choice you made, and what the other
choice would produce), `confidence` (how sure you are that the checker
accepts the program, and of which line least), and `context` (whether
anything other than this directory's files reached your context, and what).
English, no em dashes.
