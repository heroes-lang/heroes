# Panel 181, brief for the llm-ergonomist's second reading

Written 2026-09-28 at 03:38 by the coordinator, after the four seats and the
critic reported and before the synthesis was committed, on the author's
instruction of the same night (meant as: *always choose the most robust and
solid route, even at the cost of the spec's tokens*). The adopted wording is
longer than the cheapest one priced, and a sentence the sitting adopts is read
blind before it enters the spec, as panel 180 did.

Your input is **this file and the candidate spec**,
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/llm-ergonomist-second-reading/heroes-spec-candidate.md`,
which you read in full, **and nothing else**: not the repository's spec, not
design.md, not the other briefs or reports, not the compiler. The candidate is
today's spec with one sentence added at the end of its opening paragraph and
one clause of § 1 changed; you are not told which, so read it as a first-time
reader would.

## The tasks

1. **Predict.** For each fragment below, inside `function main()` with `a: i64
   = 5`, `b: bool = false`, `c: bool = true`, `xs: [i64] = [1, 2]`, `function
   f(n: i64) -> i64`, `function double(n: i64) -> i64`, `function triple(n: i64)
   -> i64` and `variant S` with cases `dot` and `sq` in scope, and every name a
   fragment binds read afterwards, say whether the compiler accepts it, what it
   does if it does, and the words of the candidate spec you relied on.
   Indentation is exact; count the spaces.

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

   8.  z =
       a

   9.  ys = [1, 2,
       3]

   10. g = if c
           double
       else
           triple
       (5)

   11. v = xs
       [0]

   12. if (a > 0 &&
               b)
           print(1)

   13. y = a +  # a note
       1

   14. s: S = .dot
       k = match s
           .dot => a +
           1
           .sq => 0

   15. for x in
       xs
           print(x)
   ```

2. **Write.** (i) An assignment whose right side is a sum of four calls with
   long names that does not fit on one line, and (ii) an `if` whose condition
   is three `&&` clauses that do not fit on one line. Say how you broke each and
   which words told you how.
3. **Judge.** Is any fragment's answer not readable off the candidate? Is the
   added sentence non-local (a rule you cannot apply by looking at the two
   lines around a break)? Does any word in it grant a permission the rest of
   the spec takes back, or the reverse? If you would reword it, give the
   rewording and which answers it changes; keep every answer it does not.

## Prediction

Register one falsifiable prediction the coordinator can score with an
instrument that exists today (for example, how many of the fifteen the
compiler that lands this sitting's resolution answers as you do).

Your report is your final message; the coordinator writes it to
`docs/panel/181-reports/llm-ergonomist-second-reading.md`. English, no em
dashes.
