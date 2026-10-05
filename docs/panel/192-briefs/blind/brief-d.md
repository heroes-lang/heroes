# Your brief

You judge one thing: whether a small programming language's specification
lets a language model write a correct program in one turn. Your input is the
files of this directory and nothing else: `spec.md`, the specification (read
it in full first), and below, what the program should do. Read no file
outside this directory and use no tool but reading and writing files here.

Method: write the code, do not opine. In ONE turn, as you would submit it,
write the program, so that it compiles under the specification and does what
it should, to `c.hero` in this directory. You cannot run the compiler; you
have the specification. Do not ask for a second message: one turn is the
experiment.

Then write `report.md` with exactly these headings:
- `experiment`: the program, as in `c.hero`;
- `reading`: which sentences of the specification led you to which part
  of the program;
- `confidence`: whether you expect `c.hero` to compile and do what it
  should at once, and why;
- `argument`: at most 120 words, did the specification make a one-turn
  program likely, and where did it not;
- `choice_points`: every place where the specification left you a choice,
  the choice you made, and what the other would produce;
- `prediction`: a falsifiable rate, over 100 models given this directory,
  of one-turn programs that compile and do what they should;
- `context`: whether anything other than this directory's files reached
  your context, and what.

English, no em dashes.

## The program

What it should do: print the word `café`, then a zero-width space, then `!`,
and end the line, and nothing else. The `é` is the one character U+00E9, not
an `e` followed by a combining accent; the zero-width space is the character
U+200B, and it must be in the output although a terminal draws it as nothing.
