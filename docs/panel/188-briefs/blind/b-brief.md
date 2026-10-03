# Your brief

You judge one thing: whether a compiler's message lets a language model
repair a program in one turn. Your input is the files of this directory and
nothing else: `spec.md`, the specification of a small programming language
(read it in full first), and one short program below, with what it should do
and what the compiler printed for it. Read no file outside this directory
and use no tool but reading and writing files here.

Method: write the code, do not opine. In ONE turn, as you would submit it,
write the corrected program, so that it compiles under the specification and
does what it should, to `c.hero` in this directory. You cannot run the
compiler; you have what it printed for the program as written, and the
specification. Do not ask for a second message: one turn is the experiment.

Then write `report.md` with exactly these headings: `experiment` (the
corrected program, as in `c.hero`), `reading` (which part of what the
compiler printed led you to which edit), `confidence` (whether you expect
`c.hero` to compile and do what it should at once, and why), `argument` (at
most 120 words: did what the compiler printed make a one-turn repair likely,
and where did it not), `choice_points` (every place where the specification
left you a choice, the choice you made, and what the other would produce),
`prediction` (a falsifiable rate, over 100 models given this directory, of
one-turn repairs of this program), and `context` (whether anything other
than this directory's files reached your context, and what). English, no em
dashes.

## The program

What it should do: print the square root of 2, using the C library's `sqrt` from the header `math.h`.

`p.hero`:

```
extern "<math.h>"
    function sqrt(x: f64) -> f64

function main()
    print(sqrt(2.0))
```

What the compiler printed for it:

`heroes check p.hero` exited 1 and printed:

```
error[header_name]: `<math.h>` is C's `#include <math.h>` written inside the string, and the compiler writes the `#include <...>` around a header's name itself, where a name cannot hold `>`: the name is `math.h`
  at p.hero:1:8
    |
  1 | extern "<math.h>"
    |        ^^^^^^^^^^
  fix (certain): replace `"<math.h>"` with `"math.h"`
```
