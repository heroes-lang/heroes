# Your brief

You judge one thing: whether a compiler's messages let a language model repair
a program in one turn. Your input is the files of this directory and nothing
else: `spec.md`, the specification of a small programming language (read it in
full first), and five short programs below, each with what it should do and
what the compiler printed for it. Read no file outside this directory and use
no tool but reading and writing files here.

Method: write the code, do not opine. For each program, in ONE turn, as you
would submit it: write the corrected program, so that it compiles under the
specification and does what it should, to `c1.hero` ... `c5.hero` in this
directory. You cannot run the compiler; you have its message for the program
as written, and the specification. Do not ask for a second message: one turn
is the experiment.

Then write `report.md` with exactly these headings: `experiment` (each
corrected program, as in its file), `reading` (for each program: which
message led you to which edit, and every mistake you found that the messages
did not name), `confidence` (for each program: whether you expect it to
compile at once, and why), `argument` (at most 120 words: did the messages
make a one-turn repair likely, and where did they not), `prediction` (a
falsifiable rate, over 100 models, of one-turn repairs for each program), and
`context` (whether anything other than this directory's files reached your
context, and what). English, no em dashes.

## Program 1

What it should do: print 0, 1 and 2, one per line.

`p1.hero`:

```
function main()
    for (i = 0; i < 3; i++) {
        print(i)
    }
```

What the compiler printed for it (`heroes check p1.hero`):

```
error[unexpected_character]: `;` is not part of the language's syntax
  at p1.hero:2:15
    |
  2 |     for (i = 0; i < 3; i++) {
    |               ^

error[unexpected_character]: `;` is not part of the language's syntax
  at p1.hero:2:22
    |
  2 |     for (i = 0; i < 3; i++) {
    |                      ^

error[for_missing_in]: `for` iterates — `for x in xs`; found `(` — a loop over a condition is `while`
  at p1.hero:2:9
    |
  2 |     for (i = 0; i < 3; i++) {
    |         ^
  fix (guess): use `while`

error[missing_body]: a `for` needs an indented body — one level deeper, exactly 4 spaces (found `{`)
  at p1.hero:2:29
    |
  2 |     for (i = 0; i < 3; i++) {
    |                             ^
```

## Program 2

What it should do: print 7, the value seven returns.

`p2.hero`:

```
function seven(): i64 )
    return 7

function main()
    print(seven())
```

What the compiler printed for it (`heroes check p2.hero`):

```
error[expected_end_of_line]: expected the end of the line, found `:` — a `function`'s body goes on the lines below its head, one level deeper
  at p2.hero:1:17
    |
  1 | function seven(): i64 )
    |                 ^
```

## Program 3

What it should do: print 3, the sum of a point's two coordinates.

`p3.hero`:

```
record Point )
    x: i64
    y: i64

function main()
    p = Point(x: 1, y: 2)
    print(p.x + p.y)
```

What the compiler printed for it (`heroes check p3.hero`):

```
error[expected_end_of_line]: expected the end of the line after `record Point`, found `)` — its fields go on the lines below it, one level deeper
  at p3.hero:1:14
    |
  1 | record Point )
    |              ^
```

## Program 4

What it should do: print 2, the length of the list.

`p4.hero`:

```
function main()
    x = [1, 2
    print(x.len()) )
```

What the compiler printed for it (`heroes check p4.hero`):

```
error[expected_separator]: expected `,` or a new line between one element and the next, found `)`
  at p4.hero:3:20
    |
  3 |     print(x.len()) )
    |                    ^
```

## Program 5

What it should do: print 3.

`p5.hero`:

```
function main()
    total = 1 + 2
    print(totl)
```

What the compiler printed for it (`heroes check p5.hero`):

```
error[unknown_name]: nothing named `totl` is in scope — did you mean `total`?
  at p5.hero:3:11
    |
  3 |     print(totl)
    |           ^^^^
  fix (certain): rename to `total`
```
