# Your brief

You judge one thing: whether a rule of a programming language makes it more or
less likely that a language model produces a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification of a small programming language (read it in full first),
`task2.hero`, a program in that language, and two outputs: `o2-check.txt`,
what one compiler for this language printed on `task2.hero` (its last line is
the compiler's exit code), and `o2-run.txt`, what happened when the program it
gave was run. Read no file outside this directory and use no tool but reading
and writing files here.

In `spec.md`, section 8 holds the marker `[RULE 2]` where a sentence may stand.
Three candidates for it, in no particular order:

- **P**: A statement after a `return`, `break` or `continue` in the same block
  is a compile error.
- **Q**: nothing: the marker is removed and no sentence stands there.
- **R**: A statement after one that always leaves its block (a `return`,
  `break` or `continue`, a call to `exit`, an `assert false`, a `while true`
  with no `break`, an `if` whose every branch leaves, or a `match` whose every
  arm leaves) is a compile error, and a function none of whose paths reaches
  its end needs no `return` there.

Method: write the code, do not opine. **Do steps 1 and 2 and write them into
`report.md` before you open `o2-check.txt` or `o2-run.txt`.**

1. **Write**, three times, once under each of P, Q and R: a function that
   returns the index of the first element of an array of `i64` greater than a
   limit, or `-1`, with a `while` loop and an early `return`; a function that
   returns `1` for a positive argument and otherwise ends the program with
   `exit`; and a `main` that prints the first function's result for `[3, 9, 1,
   12]` and the limit `8`, then walks the same array with a `for` loop, skips
   the elements under 5 with `continue`, and prints the others. List every
   place where the specification left the author a choice, the choice you made,
   and what the other choice would produce under that variant: a compile error, or a program that compiles and does
   something else.
2. **Read `task2.hero`**: it holds mistakes. Say which, and for each variant
   what the compiler must do with each mistake by the variant's words.
3. **Then open `o2-check.txt` and `o2-run.txt`**, and say what a model that
   reads only those two does next.
4. **Compare** the three variants: under which is a correct program most
   likely in one turn, under which is a silently wrong one most likely, and
   does any variant forbid a program you would want to write?

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (for each of P, Q and R: approve, object or veto; a veto is for a
rule whose meaning cannot be told from the line and its enclosing signature),
`experiment` (the code you wrote), `choice_points` (every place the specification
left a choice, and what the wrong choice produces), `argument` (at most 120 words), `prediction` (a
falsifiable rate of silent wrong programs or one-turn repairs under each
variant), `condition` (what result would change your verdict), and `context`
(whether anything other than this directory's files reached your context, and
what). English, no em dashes.
