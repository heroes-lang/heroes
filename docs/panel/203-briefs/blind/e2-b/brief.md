# Your brief

You read a program in a small programming language and say what its checker
does with it. Your input is the files of this directory and nothing else:
`spec.md`, the language's specification (read it in full first), and
`main.hero`, the program. Read no file outside this directory and use no tool
but reading and writing files here; you cannot run anything.

**The task**: `main` holds six lines that each call a generic function: lines
19, 20, 21, 22, 23 (with its `print` on 24) and 25 (with its `print` on 26).
For each of the six, say whether the checker accepts it or refuses it, and
why; for each accepted, the type each type parameter takes and what the line
prints.

Write `report.md` in this directory as you go, with exactly these headings:
`lines` (one entry per line: accepted or refused, the type parameters' types,
what it prints, and the sentences of the specification it rests on, quoted),
`choice_points` (where the specification left you a choice, the choice you
made, and what the other would produce), `confidence`, and `context` (whether
anything other than this directory's files reached your context, and what).
English, no em dashes.
