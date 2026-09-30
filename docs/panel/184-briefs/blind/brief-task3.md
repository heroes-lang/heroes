# Your brief

You judge one thing: whether a rule of a programming language makes it more or
less likely that a language model produces a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification of a small programming language (read it in full first), two
programs in that language, `task3a.hero` and `task3b.hero`, and what one
compiler for this language printed on them: `o3a-check.txt` (checking
`task3a.hero`), `o3b-check.txt` (checking `task3b.hero`) and `o3b-build.txt`
(building `task3b.hero`); the last line of each is the compiler's exit code,
and the compiler's documented exit codes are 0 clean, 1 the input has
diagnostics, 2 the tool could not run. Read no file outside this directory and
use no tool but reading and writing files here.

In `spec.md`, section 9 holds the marker `[RULE 3]` where a sentence may stand.
Three candidates for it, in no particular order:

- **X**: nothing: the marker is removed and no sentence stands there.
- **Y**: A source nested more than 256 deep, brackets, blocks and holes counted
  together, is a compile error at the opener that passes the limit; a chain,
  `a + b + c`, `- - x` or `v.f().g()`, is not nesting and compiles at any
  length. (The number is the rule's example; the rule would fix one number for
  every platform.)
- **Z**: A source nested deeper than the compiler's own stack holds aborts the
  compiler, and that depth is not the same on every platform.

Method: write the code, do not opine. **Do steps 1 and 2 and write them into
`report.md` before you open any of the three outputs.**

1. **Write**, three times, once under each of X, Y and Z: a program that
   prints the sum of the integers 1 to 300, written the way a code generator
   that emits one expression per table would write it (every term in one
   expression), and a second program that prints the result of applying a
   one-line function `step(v: i64) -> i64` 100 times to 0, written as one
   nested call. List every place where the specification left the author a
   choice, the choice you made, and what the other choice would produce under
   that variant.
2. **Read the two programs**, and for each variant say what the compiler must
   do with each by the variant's words.
3. **Then open the three outputs**, and say what a model that reads only each
   output does next: which line it edits, and whether it can tell that the
   program is legal.
4. **Compare** the three variants: under which is a correct program most
   likely in one turn, under which does a program's acceptance depend on
   something the author cannot see, and does Y's number forbid a program you
   would want to write?

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (for each of X, Y and Z: approve, object or veto; a veto is for a
rule whose meaning cannot be told from the line and its enclosing signature),
`experiment` (the code you wrote), `choice_points` (every place the specification
left a choice, and what the wrong choice produces), `argument` (at most 120 words), `prediction` (a
falsifiable rate of silent wrong programs or one-turn repairs under each
variant), `condition` (what result would change your verdict), and `context`
(whether anything other than this directory's files reached your context, and
what). English, no em dashes.
