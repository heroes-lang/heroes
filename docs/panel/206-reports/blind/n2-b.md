# p1

Build result: compile error, no binary. `y: u8 = 200 + 100` gives both literals the type `u8` from the annotation, so the sum is a `u8` addition. 200 + 100 = 300, which is more than the `u8` maximum of 255, so it overflows. That overflow is computed from literals alone, so the compiler reports it as an error instead of building a binary that would abort.

Exit code: `heroes build` exits nonzero. `./p1` is never produced, so it has no exit code of its own.

Output: a compiler diagnostic about `u8` overflow in `200 + 100` on line 2. Nothing is printed by the program.

Sentences relied on:
- "A literal takes the type its context asks for ... `b: u8 @ 255`, and `b + 1` is a `u8` ... otherwise `i64`." (section 2)
- "Overflow aborts at every width." (section 7)
- "An abort that literals and written constants alone compute is a compile error." (section 7)

# p2

Build result: builds (by the choice described in choice_points). `0 - 1` is an `i64` computation equal to -1 and does not overflow, so no compile-time abort comes from the arithmetic. `repeat("-", -1)` type-checks if `repeat`'s count is an `i64`, and `s` is a `str` that `print` accepts and that is read, so it is used.

Exit code: nonzero (abort). The specification does not give the numeric value of an abort's exit code.

Output: nothing on stdout. The program aborts in `repeat` because the count is negative, and the abort message saying why goes to the diagnostic stream. The `print` line never runs.

Sentences relied on:
- "A literal takes the type its context asks for ... otherwise `i64`." (section 2)
- "`join` and `repeat` build in one pass." (section 10). This is the only other mention of `repeat`, and it says nothing about negative counts.
- "An abort ends the program at once, saying why; no `T?` carries one." (section 6)
- "`print` writes its values with no separator and exactly one trailing newline" (section 11), which matters only for the alternatives below.

# p3

Build result: compile error, no binary. The constant `BIG: u8` has the body `200 + 100`, and the declared type gives the literals type `u8`. The result, 300, overflows `u8`. That abort is computed from literals alone, so it is a compile error.

Exit code: `heroes build` exits nonzero, and no `./p3` exists.

Output: a compiler diagnostic about `u8` overflow in the body of constant `BIG` (line 2). The program prints nothing.

Sentences relied on:
- "a written body computes over literals and other constants." (section 4)
- "A literal takes the type its context asks for" (section 2)
- "Overflow aborts at every width. ... An abort that literals and written constants alone compute is a compile error." (section 7)

# p4

Build result: compile error, no binary. `+` and `-` share one precedence level and associate left (`Sum = Product { ( "+" | "-" ) Product }`), so `255 + 1 - 1` is `(255 + 1) - 1`. With every literal typed `u8`, `255 + 1` overflows before the `- 1` is reached. That overflow is computed from literals alone, so it is a compile error, even though the final value would be 255.

Exit code: `heroes build` exits nonzero, and no `./p4` exists.

Output: a compiler diagnostic about `u8` overflow in `255 + 1` on line 2.

Sentences relied on:
- The grammar rule `Sum = Product { ( "+" | "-" ) Product } .` and the precedence list in section 7.
- "A literal takes the type its context asks for" (section 2)
- "Overflow aborts at every width. ... An abort that literals and written constants alone compute is a compile error." (section 7)

# p5

Build result: compile error, no binary. `2 - 3 + 5` parses as `(2 - 3) + 5`. In `u8`, `2 - 3` would be -1, which underflows the unsigned type. Overflow covers going below the minimum too, so this abort is computed from literals alone and is a compile error, even though the final value would be 4.

Exit code: `heroes build` exits nonzero, and no `./p5` exists.

Output: a compiler diagnostic about `u8` overflow in `2 - 3` on line 2.

Sentences relied on: the same as for p4, namely left association from the `Sum` rule, literals typed `u8` from context (section 2), and "Overflow aborts at every width" together with "An abort that literals and written constants alone compute is a compile error" (section 7).

# p6

Build result: compile error, no binary. The context asks for `u8`, so the literal `300` is a `u8` literal, and 300 does not fit `u8` (0 to 255). The rule that a literal must fit its type rejects it before any arithmetic happens, even though `300 - 100` would be 200.

Exit code: `heroes build` exits nonzero, and no `./p6` exists.

Output: a compiler diagnostic that the literal `300` does not fit `u8` (line 2).

Sentences relied on:
- "A literal takes the type its context asks for" (section 2)
- "Every base writes a value, so a literal must fit its type." (section 2)

# choice_points

1. **p2, what `repeat` does with a negative count.** The specification gives no signature for `repeat` and does not say what a negative `n` does. I chose a runtime abort: the build succeeds, the program prints nothing to stdout, and it exits nonzero. The other choices:
   - `repeat` returns `""` for a count of 0 or less. Then `./p2` prints one empty line (just `\n`) and exits 0.
   - The compiler treats `repeat("-", 0 - 1)` as an abort "that literals and written constants alone compute". Then the build fails with a compile error and there is no binary. I did not choose this because that sentence sits in the operators section and reads as being about operator arithmetic, not built-in calls.
   - `repeat`'s count is unsigned (for example `u64`). Then `0 - 1` would be typed `u64` from context and underflow at compile time, which is a compile error.

2. **p2, the abort's exit code.** The specification says only that an abort ends the program "saying why". I report it as nonzero with an unspecified value.

3. **p4 and p5, whether compile-time evaluation checks each step or only the final result.** I chose step by step, in the literals' context type (`u8`), as runtime would do it, because the literals are `u8` and "Overflow aborts at every width". The other choice is to fold with unbounded precision and check only the final result. Then p4 builds and prints `255`, and p5 builds and prints `4`, both with exit code 0.

4. **p6, whether a literal is checked on its own or only the folded result is.** I chose to check each literal on its own, because "a literal must fit its type". The other choice is to check only the folded result. Then p6 builds and prints `200` with exit code 0.

5. **p1 and p3, whether the context type reaches the literals.** I chose yes, as section 2's example says (`b: u8 @ 255`). If the sum were computed as `i64` instead, p1 would fail anyway, either because no implicit conversion turns `i64` into `u8` or because 300 does not fit. So every reading gives a compile error, and I consider these two cases firm.

# confidence

- p1, p3 and p6: high. The specification directly states overflow, the literal-fit rule and compile-time aborts.
- p4 and p5: medium to high. They depend on step-by-step evaluation in `u8` (choice point 3), which is the most literal reading of the text.
- p2: low to medium. The behaviour of `repeat` with a negative count is not specified. That the build passes the arithmetic is firm, but whether the program aborts, prints an empty line, or is rejected at compile time is a judgement call.

# context

Only the files in this directory reached my context: `brief.md`, `spec.md` and `p1.hero` to `p6.hero`. I also received the harness's system context: environment details, and an automatically attached user email address that I did not use. I read no other files and ran nothing.
