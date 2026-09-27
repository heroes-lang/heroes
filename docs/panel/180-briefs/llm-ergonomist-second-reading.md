# Panel 180, the llm-ergonomist's second reading

Your input is **this file and `spec/heroes-spec.md`, nothing else**: read the
spec at `/Users/joseph/Temp/heroes/heroes-lang/spec/heroes-spec.md` in full,
then replace its lines 11-12 (the sentence beginning *Inside `(` `[` `{` a
NEWLINE never ends a statement*) with the sentence below, and read no other
file of the repository.

The sitting converged on a rule and this is the sentence proposed for it:

> Inside `(` `[` `{` a NEWLINE never ends a statement. A line there that ends
> with a word other than `function` or `fail`, a literal, `?`, `???` or a
> closing bracket carries one, which stands only before a closing bracket or a
> `,`, or where a production writes it, and a line ending otherwise goes on
> below, at any column. Where a NEWLINE separates without a `,`, the next line
> may not begin with a `-` set apart from its operand.

## Tasks

1. **Predict.** Under that sentence, for each fragment below (inside
   `function main()`, with `a = 5`, `b = 3`, `xs = [1, 2]` and
   `function f(a: i64, b: i64) -> i64` in scope), say whether the parser
   accepts it, and if it accepts a list, how many elements it has; for each,
   the words you relied on.

   ```
   1. print(f(a: 1
          , b: 2))
   2. ys = [1
          , 2]
   3. print(xs[0
      ])
   4. zs = [a
          - b]
   5. zs = [a
          -b]
   6. zs = [a -
          b]
   7. n = (a
          + b)
   8. deltas = [
          1
          -1
      ]
   9. zs = [a,
          - b]
   10. m = {1: 10
           - 2: 20}
   11. extern "x.h"
           function g(p: ptr counted_by
               n, n: u64)
   12. t: (function(i64) ->
           i64) = ...
   ```
   (For 11 and 12 say only whether the line break itself is accepted.)
2. **Write** a list of four long arithmetic expressions, one per line, where
   the second is a subtraction too long for one line, the way the sentence
   permits.
3. **Judge**: what a reader must hold, whether anything is non-local, whether
   a word in the sentence is one a reader will misread (for example *word*,
   *carries*, *set apart*), and a shorter or clearer sentence with the same
   answers if one exists.

The coordinator holds what the prototype compiler does on each fragment and
scores your answers after you report; do not look for it.

## Prediction

Register one prediction the coordinator can score today.

Write your report to
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/llm-ergonomist-2/report.md`
if you can; if the write is refused, your final message is the report.
English, no em dashes.
