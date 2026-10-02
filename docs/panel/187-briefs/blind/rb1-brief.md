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

What it should do: print 0, 1 and 2, one per line.

`p.hero`:

```
function main()
    for (i = 0; i < 3; i++) {
        print(i)
    }
```

What the compiler printed for it (`heroes check p.hero`):

```
error[unexpected_character]: `;` is not part of the language's syntax
  at p.hero:2:15
    |
  2 |     for (i = 0; i < 3; i++) {
    |               ^

error[unexpected_character]: `;` is not part of the language's syntax
  at p.hero:2:22
    |
  2 |     for (i = 0; i < 3; i++) {
    |                      ^

error[for_missing_in]: `for` iterates — `for x in xs`; found `(` — a loop over a condition is `while`
  at p.hero:2:9
    |
  2 |     for (i = 0; i < 3; i++) {
    |         ^
  fix (guess): use `while`

error[missing_body]: a `for` needs an indented body — one level deeper, exactly 4 spaces (found `{`)
  at p.hero:2:29
    |
  2 |     for (i = 0; i < 3; i++) {
    |                             ^
```
