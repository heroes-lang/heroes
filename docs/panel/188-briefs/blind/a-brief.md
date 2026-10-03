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

`heroes check p.hero` printed nothing and exited 0. `heroes build p.hero -o p` exited 2 and printed:

```
internal error: compiling the generated C failed:
build/tu-4ac7f14704f923ea/p.c:5:19: warning: extra tokens at end of #include directive [-Wextra-tokens]
    5 | #include <<math.h>>
      |                   ^
      |                   //
build/tu-4ac7f14704f923ea/p.c:5:10: error: '<math.h' file not found, did you mean 'math.h'?
    5 | #include <<math.h>>
      |          ^~~~~~~~~
      |          <math.h>
1 warning and 1 error generated.

the generated C is at build/tu-4ac7f14704f923ea/p.c
error: clang refused the generated C
```
