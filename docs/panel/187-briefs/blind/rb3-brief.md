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
compiler; you have its message for the program as written, and the
specification. Do not ask for a second message: one turn is the experiment.

Then write `report.md` with exactly these headings: `experiment` (the
corrected program, as in `c.hero`), `reading` (which message led you to
which edit), `confidence` (whether you expect `c.hero` to compile at once,
and why), `argument` (at most 120 words: did the message make a one-turn
repair likely, and where did it not), `prediction` (a falsifiable rate, over
100 models given this directory, of one-turn repairs of this program), and
`context` (whether anything other than this directory's files reached your
context, and what). English, no em dashes.

## The program

What it should do: print 3, the sum of a point's two coordinates.

`p.hero`:

```
record Point
    x: i64
    y: i64
    function sum() -> i64

function main()
    p = Point(x: 1, y: 2)
    print(p.x + p.y)
```

What the compiler printed for it (`heroes check p.hero`):

```
error[expected_field]: expected a field name, found `function` — a `record` holds only its fields, one per line, `name: type`, and a `function` in it is declared at the top level of the file, taking the record as a parameter
  at p.hero:4:5
    |
  4 |     function sum() -> i64
    |     ^^^^^^^^

error[missing_body]: a `function` needs an indented body — one level deeper, exactly 4 spaces (found the end of the block)
  at p.hero:6:1
    |
  6 | function main()
    | ^
```
