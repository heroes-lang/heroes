# program

```
# All 64 bits set.
constant ALL_ONES: u64
    0xffff_ffff_ffff_ffff

# All 8 bits set.
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

1. **How to write the all-ones value.** The spec says bitwise operators (`~`, `|`, `<<` and so on) work on `i64` only (section 7), so `~0` cannot produce a `u64` or a `u8`; in a `u64` constant it would be a type error. `to_u64(-1)` gives a `u64?` and would fail anyway, since -1 does not fit. Overflow aborts, so arithmetic tricks are out too. I chose a plain literal, which takes its type from the context (section 2: "A literal takes the type its context asks for", "Every base writes a value, so a literal must fit its type"). `0xffff_ffff_ffff_ffff` is 2^64 - 1 and fits `u64`; `0xff` is 255 and fits `u8`.
2. **Base of the literal.** I chose hex with `_` separators because it shows the bit pattern directly. The decimal `18446744073709551615` and `255`, or binary `0b1111_1111`, would produce the identical values and output; only readability differs.
3. **Body of a constant.** The grammar makes a constant's body a `Block`, and the example in section 4 shows a bare literal on the indented line. I followed that example exactly. Writing `return 0xff...` instead is not shown for constants and might be rejected.
4. **How to print.** `print` takes numbers directly and adds one newline, so I used two `print` calls, one value per line. One call `print(ALL_ONES, LOW_BYTE)` would print `18446744073709551615255` with no separator, which is hard to read. Adding labels (for example `print("ALL_ONES = ", ALL_ONES)`) would also work but adds text the task did not ask for. A `print(...)` returns `()`, so it stands alone as a statement and needs no `_ =`.
5. **Doc comments.** I put a `#` comment above each constant; omitting them changes nothing in the build or output.

# confidence

I believe it builds and prints `18446744073709551615` then `255`. What I am least sure of: (a) whether the compiler accepts a hex literal whose value exceeds `i64` range when the context is `u64` (the spec says every base writes a value and a literal takes its context type, which supports it, but the default literal type is `i64`); if it were refused, the decimal form would face the same question, so a fallback would be hard to find within the spec. (b) That `print` renders a `u64` as an unsigned decimal; the spec says it takes "a number", and I assume an unsigned value prints unsigned.

# context

Only `brief.md` and `spec.md` from this directory were read. Beyond them, my context held the harness system prompt and an automatically attached note with the user's account email, which I did not use. No other files or external sources were consulted.
