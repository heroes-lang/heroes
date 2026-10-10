# program

```
constant ALL_ONES: u64
    0xffff_ffff_ffff_ffff

constant LOW_BYTE: u8
    0xff

function main()
    print(ALL_ONES)
    print(LOW_BYTE)
```

Expected output:

```
18446744073709551615
255
```

# choice_points

1. **How to write the all-ones value.** The spec says bitwise operators (`~`, `|`, `<<`, ...) work on `i64` only (section 7), so `~0` cannot produce a `u64`: as a `u64` constant body it would be a type error (an `i64` expression where `u64` is required), and even typed as `i64` it would be `-1`, not a `u64`. Computing it as `2^64 - 1` by arithmetic would overflow and abort (or fail at compile time), because overflow aborts at every width and `2^64` does not fit a `u64`. So I wrote it as a literal, which takes the type its context asks for (section 2), here `u64` from the constant's declared type. The literal fits a `u64`, as section 2 requires.
2. **Base of the literal.** I chose hex (`0xffff_ffff_ffff_ffff`, `0xff`) because it shows that every bit is set. Decimal `18446744073709551615` and `255`, or binary `0b1111_1111`, would produce identical values and output. Octal `0o1777777777777777777777` would too, but it is harder to check.
3. **Digit separators.** `_` is allowed between any two digits. I grouped the u64 literal in 16-bit groups for readability. Without underscores the value is the same.
4. **Using a conversion instead of a literal.** `to_u64` and `to_u8` return a `T?` (section 11), and a constant body "computes over literals and other constants", so a call such as `to_u64(...).must()` is probably not allowed in a constant body and would at least add an abort path. I rejected it.
5. **How to print.** `print` writes its arguments with no separator and one trailing newline. I used two `print` calls, one value per line. A single `print(ALL_ONES, LOW_BYTE)` would print `18446744073709551615255`, run together and ambiguous. Labels such as `print("ALL_ONES = ", ALL_ONES)` would also be valid but add text the brief did not ask for. An `f"..."` string would work as well and give the same text as `to_str`.
6. **Declaration order and comments.** Order does not matter (section 4). I put the constants first and wrote no doc comments; adding `#` comments above them would document them and change nothing in the output.

# confidence

I believe it builds and prints `18446744073709551615` then `255`, each on its own line. The program uses only constructs the spec shows directly: `constant NAME: Type` with an indented body whose value is its last expression, `function main()` with no parameters and no result, and `print` of numbers, which it accepts.

What I am least sure of: section 2 says "One `i64` is `0x1f` ..." when describing literal syntax, which could be read as meaning that the lexer always builds an `i64`-range integer, so a value above `i64` max (`0xffff_ffff_ffff_ffff`) might be refused before typing. The rest of section 2 (a literal takes the type its context asks for; "a literal must fit its type") points to it being accepted as a `u64`, and the spec offers no other way to write a `u64` with its top bit set, since bitwise operators are `i64` only and arithmetic overflow aborts. A smaller doubt is whether `print` renders a `u64` above `i64` max as unsigned decimal; the spec says it prints a number, and I assume unsigned decimal. I could not run the compiler to check.

# context

Only `brief.md` and `spec.md` from this directory informed the program. The harness also supplied its usual system context (environment details and the user's account email), which I did not use. Nothing else about Heroes reached my context, and I read no files outside this directory.
