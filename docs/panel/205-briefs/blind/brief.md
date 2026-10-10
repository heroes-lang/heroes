# Your brief

You write a program in a small programming language. Your input is the
files of this directory and nothing else: `spec.md`, the language's
specification (read it in full first). Read no file outside this directory
and use no tool but reading and writing files here; you cannot run anything.

**The task**: write `main.hero`, a whole program for Linux with glibc, that
binds `sched_getcpu` from the system's `sched.h`, calls it once, and prints
`true` if it returned a CPU number (zero or more) and `false` if it did not.
It must build with `heroes build main.hero -o main` on an ordinary Linux
machine. Any other file the program needs, write it here too and say so.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (every file you wrote, whole), `choice_points` (every place the
specification left you a choice, the choice you made, and what the other
choice would produce), `confidence` (whether you believe it builds and does
what is asked, and what you are least sure of), and `context` (whether
anything other than this directory's files reached your context, and what).
English, no em dashes.
