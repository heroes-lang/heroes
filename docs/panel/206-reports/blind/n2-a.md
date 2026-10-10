# Report

## p1

- Build result: compile error. `heroes build` reports the error, exits nonzero, and writes no binary, so `./p1` does not exist.
- Exit code: none for `./p1` (it is never produced); the build itself fails.
- Output: none from the program; only the compiler's diagnostic.
- Why: `y: u8 = 200 + 100` gives the literals the type `u8` from the annotation, so `200` and `100` are each `u8` (both fit) and the sum is a `u8` addition. 200 + 100 = 300 exceeds 255, which is an overflow. Overflow is an abort, and since only literals compute it, it becomes a compile error.
- Spec sentences:
  - "A literal takes the type its context asks for: `b: u8 @ 255`, and `b + 1` is a `u8`" (section 2).
  - "Overflow aborts at every width." (section 7)
  - "An abort that literals and written constants alone compute is a compile error." (section 7)

## p2

- Build result: builds (my choice; see choice_points). `0 - 1` is an `i64` subtraction giving -1. There is no overflow, so no compile-time abort comes from the operators.
- Exit code: nonzero (the abort's exit status is not specified; I expect something like 1).
- Output: nothing on stdout. The program aborts inside `repeat("-", -1)` and, as the abort rule says, states why (for example, a negative repeat count) on its diagnostic stream. `print(s)` never runs.
- Why: the specification lists `repeat(s, n)` but gives neither the type of `n` nor what a negative count does. I read `n` as `i64`, the default integer type that `range`, `len` and the bitwise operators all use, so the literals take `i64`. A negative count has no meaningful string, so I take it to abort at run time, because the program's failure modes are aborts and `T?` and `repeat` is not documented as fallible.
- Spec sentences:
  - "A literal takes the type its context asks for ... otherwise `i64`." (section 2)
  - "`join` and `repeat` build in one pass." (section 10) and the built-in list "`join(xs, sep)` · `repeat(s, n)`" (section 11), which is all the specification says about `repeat`.
  - "An abort ends the program at once, saying why; no `T?` carries one." (section 6)

## p3

- Build result: compile error. No binary.
- Exit code: none for `./p3`; the build fails.
- Output: only the compiler's diagnostic.
- Why: the constant `BIG: u8` has the body `200 + 100`. The body's literals take `u8` from the declared type, and 200 + 100 overflows `u8`. Literals alone compute this abort (it is a constant's written body), so it is a compile error.
- Spec sentences:
  - "Constants use SCREAMING_CASE, and a written body computes over literals and other constants." (section 4)
  - "A literal takes the type its context asks for" (section 2).
  - "Overflow aborts at every width. ... An abort that literals and written constants alone compute is a compile error." (section 7)

## p4

- Build result: compile error. No binary.
- Exit code: none for `./p4`; the build fails.
- Output: only the compiler's diagnostic.
- Why: `+` and `-` are one precedence level and the production `Sum = Product { ( "+" | "-" ) Product }` groups them left to right, so the expression is `(255 + 1) - 1`. Every literal is `u8` from the annotation, and `255 + 1` = 256 overflows `u8` before the `- 1` can bring it back. That is a literal-only abort, so it is a compile error. The final value, 255, would fit, but the intermediate does not.
- Spec sentences:
  - "Sum = Product { ( "+" | "-" ) Product } ." and the precedence list "`* / %` → `+ -`" (section 7).
  - "A literal takes the type its context asks for: `b: u8 @ 255`, and `b + 1` is a `u8`" (section 2).
  - "Overflow aborts at every width. ... An abort that literals and written constants alone compute is a compile error." (section 7)

## p5

- Build result: compile error. No binary.
- Exit code: none for `./p5`; the build fails.
- Output: only the compiler's diagnostic.
- Why: the expression is `(2 - 3) + 5`, all in `u8`. `2 - 3` = -1 is below the range of an unsigned type, so it overflows (underflows) even though the final value, 4, would fit. Literals alone compute it, so it is a compile error.
- Spec sentences: the same as p4 ("Sum = ..." left to right; "A literal takes the type its context asks for"; "Overflow aborts at every width."; "An abort that literals and written constants alone compute is a compile error."), plus "`u8` `u16` `u32` `u64` | unsigned integers" (section 3).

## p6

- Build result: compile error. No binary.
- Exit code: none for `./p6`; the build fails.
- Output: only the compiler's diagnostic.
- Why: the literal `300` takes the type `u8` from the annotation, and 300 does not fit in `u8` (maximum 255). A literal must fit its type, so this is an error on the literal itself, before any arithmetic. The result, 200, would fit, but that does not matter.
- Spec sentences:
  - "A literal takes the type its context asks for" (section 2).
  - "Every base writes a value, so a literal must fit its type." (section 2)

## choice_points

1. **p2, the type of `repeat`'s count.** The specification never gives `repeat`'s signature.
   - My choice: `n: i64`, so `0 - 1` is -1 and the build succeeds.
   - Other choice: if `n` were unsigned (for example `u64`), the literals would take that type, `0 - 1` would underflow, and the program would fail to build with a compile error, like p5.
2. **p2, what `repeat` does with a negative count.** The specification does not say.
   - My choice: a run-time abort with a nonzero exit and nothing on stdout.
   - Other choice: `repeat` treats a count of zero or less as zero. Then the program prints one empty line (`print` adds exactly one trailing newline) and exits 0.
   - Third choice: the compiler treats `repeat("-", -1)` as "an abort that literals ... alone compute", because its arguments are all literals, and makes it a compile error. I did not pick this because that rule sits among the operator rules and constant bodies are described as computing over literals and constants, not built-in calls. It is still a defensible reading.
3. **Exit code of an abort.** The specification says an abort ends the program "saying why" but gives no number. I report "nonzero" and do not commit to a value.
4. **p4 and p5, intermediate typing versus folding the whole expression.** I typed every subexpression as `u8`, following "`b + 1` is a `u8`", so intermediate overflow counts.
   - Other choice: a compiler that folds literal arithmetic in unbounded precision and checks only the final value would build p4 and print `255` (exit 0), and build p5 and print `4` (exit 0).
5. **p6, the same alternative.** Folding first and checking only the final result would build p6 and print `200` (exit 0). I rejected this because "a literal must fit its type" applies to the literal `300` itself.

## confidence

- p1, p3: high. These are direct applications of the overflow rule and the literal-only abort rule.
- p6: high. "A literal must fit its type" is explicit.
- p4, p5: medium-high. They depend on intermediate results being typed `u8`, which the spec's `b + 1` example supports.
- p2: low. Both the count type and the behaviour on a negative count are unspecified. The empty-line and compile-error outcomes are real alternatives.

## context

Nothing beyond this directory's files reached my context: `brief.md`, `spec.md` and `p1.hero` to `p6.hero`. The only other context was the harness's own environment notes (working directory, platform, date) and an automatically attached account email, which I did not use. I read no other files and ran nothing.
