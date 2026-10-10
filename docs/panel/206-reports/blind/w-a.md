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

1. **How to write all ones.** Section 7 says the bitwise operators `& | ^ ~ << >>` work on `i64` only, so `~0` cannot produce a `u64` or a `u8`: it would be an `i64` with value -1, and putting it in a `u64` constant is a type error (section 3: no implicit conversions). A shift such as `(1 << 64) - 1` is also out, because the shift count must be 0..63 and the result is still `i64`. So I wrote the value as a literal. Section 2 says a literal takes the type its context asks for (here the declared constant type) and "every base writes a value, so a literal must fit its type". `0xffff_ffff_ffff_ffff` is 2^64 - 1, which fits `u64` exactly, and `0xff` is 255, which fits `u8`.
2. **Literal base.** I chose hex because it shows the bit pattern directly. Decimal `18446744073709551615` and `255`, or binary `0b1111_1111`, would give the same value and the same output. A `0o` octal form would too, but is harder to read.
3. **Digit separators.** I grouped the hex digits by 4 with `_`, which section 2 allows between any two digits. Leaving them out gives the same value.
4. **Conversion instead of a literal.** Something like `to_u64(-1)` is not an option: it returns `u64?` and would fail at runtime because -1 does not fit, and section 4 says a constant body computes over literals and other constants, so a function call there is doubtful anyway. Writing an `i64` literal like `0xffff_ffff_ffff_ffff` without the `u64` context would be a compile error, because it does not fit `i64`.
5. **Output format.** I used two `print` calls, which puts each value on its own line. One call, `print(ALL_ONES, " ", LOW_BYTE)`, would print `18446744073709551615 255` on one line. `print(ALL_ONES, LOW_BYTE)` with no separator would print `18446744073709551615255`, which runs the two numbers together, so I did not use it.
6. **Comments.** I left out doc comments. Adding `#` lines above the declarations would not change the build or the output.

# confidence

High. I believe it builds with `heroes build main.hero -o main` and prints `18446744073709551615` and then `255`. Both constants are SCREAMING_CASE, both are read in `main` so neither is unused, `main` takes nothing and returns nothing, and `print` accepts any number type.

What I am least sure of: whether a constant body that is just a literal gets the declared type as its context. Section 2 shows typed context for a binding (`b: u8 @ 255`), and the constant example in section 4 (`constant MAX_DEPTH: i64` with body `64`) suggests a constant works the same way, but the spec never says so outright for a non-`i64` constant. If the literal were typed `i64` first, `0xffff_ffff_ffff_ffff` would not fit and the build would fail. I also cannot run the compiler to check.

# context

Only this directory's files reached my context: `brief.md` and `spec.md`. I also had the system prompt and environment details from the harness (working directory, platform, date, and the user's account email, which I did not use). I read no other files and ran nothing.
