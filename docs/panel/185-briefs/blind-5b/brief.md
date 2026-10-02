# Your brief

You judge one thing: whether a rule of a programming language makes it more or
less likely that a language model produces a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification of a small programming language (read it in full first),
`task1.hero` and `task2.hero`, two programs in that language, and
`o1-check.txt`, what one compiler for this language printed on `task1.hero`
(its last line is the compiler's exit code). Read no file outside this
directory and use no tool but reading and writing files here.

In `spec.md`, one sentence of section 2 is replaced by the marker `[RULE 1]`.
Three candidate sentences for it, in no particular order:

- **L**: A literal without the `f` is unchanged.
- **K**: A literal without the `f` may not hold a bound name in braces: braces
  meant as text are `{{` and `}}` in an `f` literal.
- **M**: A literal without the `f` holding a `{` whose text up to the `}` that
  closes it would be a hole is a compile error; such braces, meant as text, are
  written `{{` and `}}` in an `f` literal.

Method: write the code, do not opine. **Do steps 1 and 2 and write them into
`report.md` before you open `o1-check.txt`.**

1. **Write**, three times, once under each of L, K and M: a program that, for
   the scores `[12, 7, 30]`, prints one line per score, `score 1 of 3: 12` and
   so on, then `best: 30 (score 3)`, then a line holding the text `{best}`
   followed by ` = ` and the best score, `{best} = 30`, then the text
   `{"id": 7}` exactly. List every place where the specification left the
   author a choice, the choice you made, and what the other choice would
   produce under that candidate: a compile error, or a program that compiles
   and prints something else.
2. **Read `task1.hero` and `task2.hero`**: they hold mistakes. Say which, and
   for each candidate what the compiler must do with each line a candidate
   treats differently, and what each program prints where it compiles.
3. **Then open `o1-check.txt`**, and write the program you would submit after
   reading that output alone, as a model that follows the message would. Run it
   in your head under the specification: what does it print?
4. **Compare** the three candidates: under which is a correct program most
   likely in one turn, under which is a silently wrong one most likely, and
   does any candidate make a program's meaning, or whether it compiles, depend
   on something other than the line it is on and its enclosing function's
   signature? Say which of the two it is, meaning or compiling, for each
   candidate you judge that way.

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (for each of L, K and M: approve, object or veto; a veto is for a
rule whose meaning cannot be told from the line and its enclosing signature),
`experiment` (the code you wrote), `choice_points` (every place the
specification left a choice, and what the wrong choice produces), `argument`
(at most 120 words), `prediction` (a falsifiable rate of silent wrong programs
or one-turn repairs under each candidate), `condition` (what result would
change your verdict), and `context` (whether anything other than this
directory's files reached your context, and what). English, no em dashes.
