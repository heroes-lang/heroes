# Report

All six programs are about how integer literals get their type and what happens when arithmetic on them overflows. Rules used throughout:

- Section 2: "A literal takes the type its context asks for ... `b: u8 @ 255`, and `b + 1` is a `u8` ... otherwise `i64`." (The ellipses replace dashes in the original.)
- Section 2: "Every base writes a value, so a literal must fit its type."
- Section 7: "Overflow aborts at every width."
- Section 6: "An abort ends the program at once, saying why; no `T?` carries one."
- Section 7 grammar: `Sum = Product { ( "+" | "-" ) Product } .`, so `+` and `-` group from the left.

The specification gives no number for an abort's exit code. Where I say "abort", the exit code is some nonzero value and the specification does not fix which one. The abort message ("saying why") goes to the diagnostic stream, and nothing reaches standard output.

## p1

```
y: u8 = 200 + 100
print(y)
```

- **Build result**: builds. The binding asks for `u8`, so `200` and `100` are each `u8` literals, and both fit (0..255). The form `ident ":" Type "=" Expression` is a valid Binding, and `y` is read by `print`, so it counts as used.
- **Run**: `200 + 100` in `u8` is 300, which is more than 255. This overflows, so the program aborts before `print` runs.
- **Exit code**: nonzero, from the abort. The number is not specified.
- **Output**: nothing on stdout. An abort message names the overflow.
- **Sentences relied on**: "A literal takes the type its context asks for ... `b + 1` is a `u8`"; "a literal must fit its type"; "Overflow aborts at every width."; "An abort ends the program at once, saying why".

## p2

```
s = repeat("-", 0 - 1)
print(s)
```

- **Build result**: builds. The specification gives no signature for `repeat`. I take its count as `i64`, which is the default literal type and the type of `range`'s bounds and `exit`'s code. Then `0 - 1` is the `i64` value -1, with no overflow. The two parameters (`str`, `i64`) have different types, so no argument names are needed.
- **Run**: `repeat` gets a negative count. The specification does not say what that does. I chose an abort, to match how the language treats other out-of-range operands ("shift count 0..63 or it aborts", "An out-of-bounds index or slice aborts").
- **Exit code**: nonzero, from the abort. The number is not specified.
- **Output**: nothing on stdout. An abort message about the negative count.
- **Sentences relied on**: "`join` and `repeat` build in one pass" (the only mention of `repeat`); "otherwise `i64`"; "shift count 0..63 or it aborts"; "An out-of-bounds index or slice aborts". This answer is a choice, not something the specification states (see choice_points).

## p3

```
constant BIG: u8
    200 + 100
```

- **Build result**: fails with a compile error. A constant's "written body computes over literals and other constants", so the compiler works out its value when it builds. Here the body is `u8` arithmetic: both literals fit, but the sum 300 overflows `u8`. There is no running program that could abort at that point, so the overflow is reported as a compile error and no binary is made.
- **Exit code / output**: not applicable, because no binary exists. The compiler reports the overflow in `BIG`.
- **Sentences relied on**: "Constants use SCREAMING_CASE, and a written body computes over literals and other constants."; "A literal takes the type its context asks for"; "Overflow aborts at every width."

## p4

```
x: u8 = 255 + 1 - 1
```

- **Build result**: builds. `255`, `1` and `1` are all `u8` literals, and each one fits.
- **Run**: the expression groups from the left as `(255 + 1) - 1`. `255 + 1` overflows `u8` before the `- 1` can bring it back to 255, so the program aborts.
- **Exit code**: nonzero, from the abort. The number is not specified.
- **Output**: nothing on stdout. An abort message names the overflow.
- **Sentences relied on**: the `Sum` production (left grouping); "A literal takes the type its context asks for"; "Overflow aborts at every width."

## p5

```
x: u8 = 2 - 3 + 5
```

- **Build result**: builds. All the literals are `u8` and they fit.
- **Run**: `(2 - 3) + 5`. `2 - 3` goes below 0 in an unsigned 8-bit type, and that is an overflow, so the program aborts. The mathematical result, 4, would fit, but each step is checked on its own.
- **Exit code**: nonzero, from the abort. The number is not specified.
- **Output**: nothing on stdout. An abort message names the overflow.
- **Sentences relied on**: the `Sum` production; "A literal takes the type its context asks for"; "Overflow aborts at every width."

## p6

```
y: u8 = 300 - 100
```

- **Build result**: fails with a compile error. The context asks for `u8`, so `300` is a `u8` literal, and 300 does not fit in `u8`. The result, 200, would fit, but that does not help: the literal itself must fit its type, and no implicit conversion could let `300` be an `i64` that is narrowed afterwards.
- **Exit code / output**: not applicable, because no binary exists. The compiler reports that `300` does not fit `u8`.
- **Sentences relied on**: "A literal takes the type its context asks for"; "Every base writes a value, so a literal must fit its type."; "No implicit conversions, widths included".

## choice_points

1. **When overflow in a function body is detected (p1, p4, p5).** "Overflow aborts at every width" describes a run-time abort, and the specification never says the compiler folds constant expressions inside functions. So I chose: the program builds, then aborts at run time. The other reading is that the compiler folds literal-only arithmetic and rejects it. Under that reading p1, p4 and p5 would each be a compile error with no binary.
2. **When overflow in a constant body is detected (p3).** I chose a compile error, because a constant's body is computed from literals and constants alone, which means at build time. The other reading is that the constant is computed when the program starts, or where it is used, and aborts then. That would build a binary that aborts with a nonzero code and prints nothing.
3. **`repeat` with a negative count (p2).** The type of `repeat`'s count and what a negative count does are not specified. I chose an `i64` count and a run-time abort. The alternatives:
   - A negative count gives an empty string: the program prints one empty line (just `\n`) and exits 0.
   - The count is unsigned (for example `u64`): then `0 - 1` overflows that unsigned type at run time and aborts anyway (or is a compile error under the folding reading in point 1).
4. **Abort exit code (p1, p2, p4, p5).** The specification gives no number. I report "nonzero, not specified".
5. **p6 is not really a choice**: a literal must fit the type the context gives it, and the context gives `u8`. I note only that a reader who typed the whole expression as `i64` and then narrowed it would get 200. The rule against implicit conversions rules that reading out.

## confidence

- **p6**: high. The specification says this outright.
- **p4 and p5**: high that they do not print 255 or 4. Moderately high that this happens as a run-time abort rather than a compile error (choice point 1).
- **p1**: same as p4 and p5.
- **p3**: moderate. A compile error is the natural reading, but the specification does not say it.
- **p2**: low to moderate. Both the type and the behaviour of `repeat`'s count are unspecified.
- **Exit codes for aborts**: the specification does not give the exact number, so I cannot know it.

## context

Nothing outside this directory's files reached my context except the harness's own system information: environment details, and an automatically attached note giving the user's account email address. I did not use either. I read only `brief.md`, `spec.md` and `p1.hero` to `p6.hero`, and wrote only `report.md`.
