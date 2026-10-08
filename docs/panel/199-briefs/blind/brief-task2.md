# Your brief

You write the next version of one program in a small programming language and
say where the specification, or the output beside the program, left you a
choice. Your input
is the files of this directory and nothing else: `spec.md`, the language's
specification (read it in full first); `p2.hero`, a program in that language; and
`output.txt`, what was printed when each command shown in it was run on that
program: a line beginning `$ ` is the command, and `(exit N)` is its exit
code. The compiler's own exit codes are 0 clean, 1 the input has diagnostics,
2 the tool could not run. Read no file outside this directory and use no tool
but reading and writing files here.

**What the program is for**: `fact(n)` gives the factorial of `n`, and `main` prints the factorial of 5.

Method: write the code, do not opine. Read `spec.md`, then `p2.hero`, then
`output.txt`, and write the program you would submit next, in one turn, so
that it does what it is for. You may change any line, keep any, or keep the
program as it is.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the whole program you would submit next), `choice_points` (every
place where the specification or the output left you a choice, the choice you
made, and what the other choice would produce: a compile error, or a program
that compiles and does something else), `confidence` (whether you believe the
program compiles and does what it is for; which part of `output.txt`, if any,
told you what to change; and what you are least sure of), and `context`
(whether anything other than this directory's files reached your context, and
what). English, no em dashes.
