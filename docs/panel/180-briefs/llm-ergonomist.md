# Panel 180, brief for the llm-ergonomist

Your input is **this file and `spec/heroes-spec.md`, nothing else**: read the
spec at `/Users/joseph/Temp/heroes/heroes-lang/spec/heroes-spec.md` in full,
and no other file of the repository (not design.md, not the other briefs, not
the compiler). Your seat judges the objective, what a model writing Heroes
from the spec predicts and writes, and has a veto on a non-local construct.
Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/llm-ergonomist/`.

## The question

The spec's opening paragraph ends with a sentence about line breaks inside
brackets (lines 11-12). The sitting chooses what that sentence says. Three
candidates, in no order and with no labels that say which is today's:

- **Sentence P.** *Inside `(` `[` `{` a NEWLINE never ends a statement: where
  it separates, a production writes it; elsewhere it may fall between any two
  tokens.*
- **Sentence Q.** *Inside `(` `[` `{` a NEWLINE never ends a statement. A line
  there may end after an operator, `.`, `,`, `:` or an opening bracket and go
  on below; a line that ends with a name, a literal, `?`, `???` or a closing
  bracket carries a NEWLINE, which stands only before a closing bracket other
  than an index's `]`, before a `,` between a call's arguments, or where a
  production writes it.*
- **Sentence R.** *Inside `(` `[` `{` a NEWLINE never ends a statement. A line
  there may end after an operator, `.`, `,`, `:` or an opening bracket and go
  on below; a line that ends with a name, a literal, `?`, `???` or a closing
  bracket carries a NEWLINE, which stands only before a closing bracket or
  where a production writes it.*

## The tasks

Read the spec with each sentence in place of lines 11-12 in turn.

1. **Predict.** For each of the ten fragments below, inside `function main()`
   with `xs = [1, 2]` and `function f(a: i64, b: i64) -> i64` in scope, say
   whether the parser accepts it under P, under Q and under R (30 answers), and
   for each answer the words of the spec you relied on.

   ```
   1. print(f(a: 1, b: 2
      ))
   2. x = (1
          + 2)
   3. x = (1 +
          2)
   4. print(xs[0
      ])
   5. print(f(a: 1
          , b: 2))
   6. ys = [1
          , 2]
   7. n = (xs
          .len())
   8. n = (xs.
          len())
   9. m = {"a"
          : 1}
   10. ys = [
           1
           2
       ]
   ```

2. **Write.** Under each sentence, write a call to a function of four named
   arguments with long names that does not fit on one line, broken across
   lines the way the sentence permits, and a boolean condition of three
   clauses inside parentheses broken across two lines. Say which break you
   chose and why.
3. **Judge**: which sentence lets a reader predict and write correctly with
   the least to hold, whether any of them is non-local (a rule the reader
   cannot apply by looking at the two lines around the break), and what a
   shorter sentence with the same predictions would be, if one exists.

The coordinator holds what the compiler does on each fragment and scores your
predictions after you report; do not look for it. If you believe a fragment's
answer cannot be read off a sentence at all, say so rather than guess.

## Prediction

Register one falsifiable prediction the coordinator can score with an
instrument that exists today (for example: how many of the ten a fresh reader
predicts correctly under the adopted sentence).

Write your report to `<your directory>/report.md`. English, no em dashes.
