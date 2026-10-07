# Your brief

You write one program in a small programming language and say where its
specification left you a choice. Your input is the files of this directory and
nothing else: `spec.md`, the language's specification (read it in full first),
and `headers.txt`, the C definitions the program uses. Read no file outside
this directory and use no tool but reading and writing files here. If your
program needs a C header file of your own, you may write it in this
directory beside the program.

**The task**: a program that prints this machine's host name, obtained through `gethostname` (its header is `unistd.h`; nothing to link).

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the whole program, as you would submit it in one turn),
`files` (every other file you wrote for it, each whole, or *none*),
`choice_points` (every place the specification left you a choice, the choice
you made, and what the other choice would produce: a compile error, or a
program that compiles and does something else), `confidence` (whether you
believe the program compiles and does what the task asks, and what you are
least sure of), and `context` (whether anything other than this directory's
files reached your context, and what). English, no em dashes.
