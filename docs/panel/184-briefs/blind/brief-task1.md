# Your brief

You judge one thing: whether a rule of a programming language makes it more or
less likely that a language model produces a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification of a small programming language (read it in full first),
`task1.hero`, a program in that language, and `o1-check.txt`, what one
compiler for this language printed on `task1.hero` (its last line is the
compiler's exit code). Read no file outside this directory and use no tool but
reading and writing files here.

In `spec.md`, one sentence of section 2 is replaced by the marker `[RULE 1]`.
Three candidate sentences for it, in no particular order:

- **K**: A literal without the `f` holding a `{` whose text up to the `}` that
  closes it would be a hole naming only what is bound where the literal stands
  is a compile error; such braces, meant as text, are written `{{` and `}}` in
  an `f` literal.
- **L**: A literal without the `f` is unchanged.
- **M**: A literal without the `f` holding a `{` whose text up to the `}` that
  closes it would be a hole is a compile error; such braces, meant as text, are
  written `{{` and `}}` in an `f` literal.

Method: write the code, do not opine. **Do steps 1 and 2 and write them into
`report.md` before you open `o1-check.txt`.**

1. **Write**, three times, once under each of K, L and M: a program that, for
   the scores `[12, 7, 30]`, prints one line per score, `score 1 of 3: 12` and
   so on, then `best: 30 (score 3)`, then a line holding the text `{best}`
   followed by ` = ` and the best score, `{best} = 30`. List every place
   where the specification left the author a choice, the choice you made, and
   what the other choice would produce under that variant: a compile error, or a program that compiles and prints something
   else.
2. **Read `task1.hero`**: it holds mistakes. Say which, and for each variant
   what the compiler must do with each mistake by the variant's words.
3. **Then open `o1-check.txt`**, and write the program you would submit after
   reading that output alone, as a model that follows the message would. Run it
   in your head under the specification: what does it print?
4. **Compare** the three variants: under which is a correct program most
   likely in one turn, under which is a silently wrong one most likely, and
   does any variant make a program's meaning depend on something other than
   the line it is on and its enclosing function's signature?

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (for each of K, L and M: approve, object or veto; a veto is for a
rule whose meaning cannot be told from the line and its enclosing signature),
`experiment` (the code you wrote), `choice_points` (every place the specification
left a choice, and what the wrong choice produces), `argument` (at most 120 words), `prediction` (a
falsifiable rate of silent wrong programs or one-turn repairs under each
variant), `condition` (what result would change your verdict), and `context`
(whether anything other than this directory's files reached your context, and
what). English, no em dashes.
