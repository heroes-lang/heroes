# Your brief

You judge one thing: whether a compiler's message lets a language model
repair a program in one turn. Your input is the files of this directory and
nothing else: `spec.md`, the specification of a small programming language
(read it in full first), `p.hero`, a short program (read it), and below, what
the program should do and what the compiler printed for it. Read no file
outside this directory and use no tool but reading and writing files here.

Method: write the code, do not opine. In ONE turn, as you would submit it,
write the corrected program, so that it compiles under the specification and
does what it should, to `c.hero` in this directory. You cannot run the
compiler; you have what it printed for the program as written, and the
specification. Do not ask for a second message: one turn is the experiment.

Then write `report.md` with exactly these headings: `experiment` (the
corrected program, as in `c.hero`), `cause` (in one sentence, what you
believe was wrong with `p.hero`), `reading` (which part of what the compiler
printed led you to which edit), `confidence` (whether you expect `c.hero` to
compile and do what it should at once, and why), `argument` (at most 120
words: did what the compiler printed make a one-turn repair likely, and where
did it not), `choice_points` (every place where the specification left you a
choice, the choice you made, and what the other would produce), `prediction`
(a falsifiable rate, over 100 models given this directory, of one-turn
repairs of this program), and `context` (whether anything other than this
directory's files reached your context, and what). English, no em dashes.

## The program

`p.hero`, in this directory. What it should do: print the word `café`.

What the compiler printed for it:

`heroes check p.hero` exited 2 and printed:

```
error: cannot read `p.hero`
```

`heroes build p.hero -o p` exited 2 and printed the same line.
