# Your brief

You judge one thing: whether the specification of a small programming
language lets a language model write a working program at its first try.
Your input is the files of this directory and nothing else: `spec.md`, the
specification (read it in full first), and the task below. Read no file
outside this directory and use no tool but reading and writing files here.

Method: write the code, do not opine. In ONE turn, as you would submit it,
write the program, so that it compiles under the specification and does what
the task asks, to `c.hero` in this directory. You cannot run the compiler;
you have the specification. Do not ask a question: one turn is the
experiment.

Then write `report.md` with exactly these headings: `experiment` (the
program, as in `c.hero`), `reading` (which sentences or examples of the
specification decided each line), `confidence` (whether you expect `c.hero`
to compile and do what the task asks at once, and why), `choice_points`
(every place where the specification left you a choice, the choice you made,
and what the other would produce), `prediction` (a falsifiable rate, over 100
models given this directory, of programs that compile and do what the task
asks at the first try), and `context` (whether anything other than this
directory's files reached your context, and what). English, no em dashes.

## The task

Write a program that prints the square root of 2 using the C standard
library's `sqrt`.
