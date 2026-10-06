# Your brief

You write one program in a small programming language and say where its
specification left you a choice. Your input is the files of this directory and
nothing else: `spec.md`, the language's specification (read it in full first),
and `headers.txt`, the C definitions the program uses. Read no file outside
this directory and use no tool but reading and writing files here.

**The task**: a program that opens a Unix stream socket and connects it to the path `/tmp/demo.sock`, printing what `socket` and `connect_un` return; say how the path's bytes reach `sun_path` and what `sun_path` holds after the path.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the whole program, as you would submit it in one turn),
`choice_points` (every place the specification left you a choice, the choice
you made, and what the other choice would produce: a compile error, or a
program that compiles and does something else), `confidence` (whether you
believe the program compiles and does what the task asks, and what you are
least sure of), and `context` (whether anything other than this directory's
files reached your context, and what). English, no em dashes.
