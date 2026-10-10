# Your brief

You read a program in a small programming language and say whether its
checker accepts it. Your input is the files of this directory and nothing
else: `spec.md`, the language's specification (read it in full first), and
`main.hero`, the program. Read no file outside this directory and use no tool
but reading and writing files here; you cannot run anything.

**The task**: would the language's checker accept `main.hero` as written? If
it would, say what the program prints. If it would not, say which lines it
refuses and why, and give the smallest change that makes the program
accepted while it still does what it evidently means. Then, for every line of
`main.hero` that holds `=` or `@`, say whether that line brings a new name
into existence or changes a name that already exists, and which words of the
specification let you tell the two apart on that line alone.

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (accepted, or refused and on which lines), `lines` (one row per line
holding `=` or `@`: its number, *new name* or *existing name*, and the words
of the specification you relied on), `sentences` (every sentence of the
specification your verdict rests on, quoted), `program` (the program you
would submit, whole), `choice_points` (every place the specification left you
a choice, the choice you made, and what the other choice would produce),
`confidence` (how sure you are, and of what least), and `context` (whether
anything other than this directory's files reached your context, and what).
English, no em dashes.
