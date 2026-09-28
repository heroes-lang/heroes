# Panel 181, brief for the llm-ergonomist

Your input is **this file and `spec/heroes-spec.md`, nothing else**: read the
spec at `/Users/joseph/Temp/heroes/heroes-lang/spec/heroes-spec.md` in full,
and no other file of the repository (not design.md, not the other briefs, not
the compiler, not the panel records). Your seat judges the objective, what a
model writing Heroes from the spec predicts and writes, and has a veto on a
non-local construct.

Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/llm-ergonomist/`.

## The question

The sitting decides what happens when a line OUTSIDE brackets ends with a
token that cannot end a statement, such as a binary operator or a `.`, and
what the spec says about it. Three variants of the spec, with no labels that
say which is today's:

- **Variant X.** The spec exactly as it is. Nothing is added.
- **Variant Y.** The spec with this sentence added after the paragraph that
  ends at line 17: *Outside brackets every line ends its statement: a line
  that ends with an operator, `.`, `,`, `:`, `=`, `@` or `->` is an error
  unless a block opens below it, so a long expression is broken inside
  parentheses.*
- **Variant Z.** The spec with this sentence added at the same place:
  *Outside brackets a line that ends with a binary operator or `.` goes on to
  the next line, which is indented one level deeper than the line it
  continues and opens no block.*

## The tasks

1. **Predict.** For each of the ten fragments below, inside `function main()`
   with `a: i64 = 5`, `b: bool = false`, `xs: [i64] = [1, 2]` and `function
   f(n: i64) -> i64` in scope, say whether the compiler accepts it under X,
   under Y and under Z (30 answers), what it does if it accepts, and for each
   answer the words of the spec you relied on. Indentation is significant and
   written exactly; `·` is not used, count the spaces.

   ```
   1.  y = a +
       1

   2.  y = a +
           1

   3.  y = (a +
       1)

   4.  if a > 0 &&
       b
           print(1)

   5.  n = xs.
       len()

   6.  y = a
       + 1

   7.  total = a * 3
       - 2

   8.  y = f(n: a) *
           2

   9.  ys = [1, 2,
       3]

   10. z =
       a
   ```

2. **Write.** Under each variant, write (i) an assignment whose right side is a
   sum of four calls with long names that does not fit on one line, and (ii) an
   `if` whose condition is three `&&` clauses that do not fit on one line.
   Say how you broke each and why.
3. **Judge**: under which variant a reader predicts and writes correctly with
   the least to hold; whether any variant leaves a program that reads as two
   statements and runs as one, or the reverse; whether any is non-local (a
   rule the reader cannot apply by looking at the two lines around the break);
   and whether Variant X, read carefully, already answers fragments 1, 2 and
   10, and with which words.

The coordinator holds what the compiler does on each fragment today and
scores your predictions after you report; do not look for it. If a fragment's
answer cannot be read off a variant at all, say so rather than guess.

## Prediction

Register one falsifiable prediction the coordinator can score with an
instrument that exists today.

Write your report to `<your directory>/report.md`, and copy it to
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-reports/llm-ergonomist.md`
(the only file you write in the trunk). English, no em dashes.
