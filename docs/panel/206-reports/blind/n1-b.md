# Report

## p1

`y: u8 = 200 + 100`

- Build result: builds. Each literal takes `u8` from the annotation, and `200` and `100` each fit `u8`. No static rule refuses the sum.
- Exit code: nonzero. The spec does not say which value an abort uses.
- Output: nothing on stdout. The program aborts while computing `200 + 100` in `u8`, because 300 exceeds 255. The abort message says why (overflow). `print(y)` is never reached.
- Spec sentences:
  - "A literal takes the type its context asks for ... `b: u8 @ 255`, and `b + 1` is a `u8`"
  - "Every base writes a value, so a literal must fit its type." (200 and 100 fit)
  - "Overflow aborts at every width."
  - "An abort ends the program at once, saying why"

## p2

`s = repeat("-", 0 - 1)`

- Build result: builds. `0 - 1` has no context type, so it is `i64`, and its value is -1 with no overflow. `repeat(s, n)` takes a `str` and a count of different types, so named arguments are not required.
- Exit code: nonzero (abort). The exact value is unspecified.
- Output: nothing on stdout, plus an abort message. The spec does not define `repeat` with a negative count. I chose an abort, since the language aborts on other invalid arguments (out-of-bounds index, shift count outside 0..63). `print(s)` is never reached. See choice_points.
- Spec sentences:
  - "otherwise `i64`" (the literal type with no context)
  - "Built-ins: ... `repeat(s, n)`"
  - "`join` and `repeat` build in one pass."
  - "When two parameters in a signature share a type, named arguments are mandatory" (they do not share one here)

## p3

`constant BIG: u8` with body `200 + 100`

- Build result: fails with a compile error. The constant's body is computed from literals by the compiler, and in `u8` that computation overflows (300 > 255). An overflow during evaluation is an abort, and an abort during compile-time evaluation can only show up as a build failure. No binary is produced.
- Exit code: none (no binary). `heroes build` itself exits nonzero.
- Output: none from the program. The compiler reports the overflow in `BIG`.
- Spec sentences:
  - "Constants use SCREAMING_CASE, and a written body computes over literals and other constants."
  - "A literal takes the type its context asks for" (`u8`, from `BIG: u8`)
  - "Overflow aborts at every width."

## p4

`x: u8 = 255 + 1 - 1`

- Build result: builds. `255`, `1` and `1` are each `u8` and each fits.
- Exit code: nonzero (abort). The value is unspecified.
- Output: nothing on stdout, plus an overflow abort message. `+` and `-` share one precedence level and associate left (`Sum = Product { ( "+" | "-" ) Product }`), so `255 + 1` is computed first. That overflows `u8` before `- 1` can bring the result back to 255. `print(x)` is never reached.
- Spec sentences:
  - "Sum = Product { ( "+" | "-" ) Product } ."
  - "A literal takes the type its context asks for"
  - "Overflow aborts at every width."

## p5

`x: u8 = 2 - 3 + 5`

- Build result: builds. All three literals are `u8` and each fits.
- Exit code: nonzero (abort). The value is unspecified.
- Output: nothing on stdout, plus an overflow abort message. `2 - 3` is evaluated first (left association) and drops below 0, which is outside `u8`. The final value 4 would fit, but the program never gets there. `print(x)` is never reached.
- Spec sentences:
  - "Sum = Product { ( "+" | "-" ) Product } ."
  - "Overflow aborts at every width."
  - "A literal takes the type its context asks for"

## p6

`y: u8 = 300 - 100`

- Build result: fails with a compile error. The literal `300` takes type `u8` from the context and does not fit in it (maximum 255). This is a static rule about the literal itself, regardless of the fact that `300 - 100 = 200` would fit. No binary is produced.
- Exit code: none (no binary). `heroes build` exits nonzero.
- Output: none from the program. The compiler reports that `300` does not fit `u8`.
- Spec sentences:
  - "A literal takes the type its context asks for ... otherwise `i64`."
  - "Every base writes a value, so a literal must fit its type."
  - "No implicit conversions, widths included"

## choice_points

1. **Overflow among literals in a function body (p1, p4, p5): runtime abort or compile error?** The spec says overflow "aborts" and never says the compiler folds constant expressions in function bodies. I chose: the program builds and aborts at run time. The other reading has the compiler fold the literal arithmetic and refuse it, which makes all three a build failure with no binary.
2. **Overflow in a constant's body (p3): compile error or runtime abort?** I chose a compile error, because a constant's body is computed over literals and constants, which implies compile-time evaluation. The other reading has the program build and abort when `BIG` is computed or first read, giving a nonzero exit and no stdout output.
3. **Abort exit code (p1, p2, p4, p5).** The spec says only that an abort ends the program "saying why". I report a nonzero code without a specific number. An implementation might use 1, 134 (SIGABRT), or something else. I assumed the message goes to stderr, so stdout stays empty.
4. **`repeat` with a negative count (p2).** This is unspecified. I chose an abort. The alternative is to treat it as zero repetitions, giving `s = ""`: the program prints one empty line (`"\n"`) and exits 0. If `repeat`'s count were an unsigned type, `0 - 1` would itself overflow and abort, which matches my answer. If it were a fallible result, `s` would be a `str?` and `print(s)` would be a compile error, because `print` takes only numbers, `str` and `bool`.
5. **Types inside `300 - 100` (p6).** I read section 2 as giving each literal the context type, so `300` is a `u8` and does not fit. The alternative is to compute the literal expression as an untyped or `i64` constant (200) and then check that it fits. That would build and print `200` with exit 0, but the spec's `b + 1` example and "No implicit conversions, widths included" argue against it.

## confidence

- p6 (compile error): high.
- p1, p4, p5 (build, then abort on overflow): medium-high on the abort itself. Medium on runtime abort versus compile-time rejection. Low on the exact exit code.
- p3 (compile error): medium.
- p2 (abort): low to medium, because `repeat`'s behaviour with a negative count is unspecified.

## context

Only this directory's files reached my context: `brief.md`, `spec.md` and `p1.hero` to `p6.hero`. The harness also attached the user's account email address and environment metadata (working directory, platform, date). None of that bears on the answers. I read no other files and ran nothing.
