# program

```
# Every bit of a u64 set.
constant ALL_ONES: u64
    0xffff_ffff_ffff_ffff

# Every bit of a u8 set.
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

1. **How to write the all-ones value.** Section 2 says a literal takes the type its context asks for, every base writes a value, and the literal must fit its type. The constant's declared type `u64` is the context, so `0xffff_ffff_ffff_ffff` is a `u64` literal of value 2^64 - 1, which fits. I chose hex because it shows the bit pattern directly.
   - Decimal `18446744073709551615` or binary `0b` followed by 64 ones would give the same value. They are just harder to read.
   - `~0` would not work. Section 7 limits bitwise operators to `i64`, so `~0` is either refused in a `u64` context or gives the `i64` value -1, which does not match the declared type. Either way it is a compile error.
   - `-1` would not work either. It does not fit `u64` (and there are no implicit conversions), so it is a compile error.
2. **How to write the low byte.** I wrote `0xff` with `u8` as its context. `255` or `0b1111_1111` would mean the same thing. Deriving it from `ALL_ONES` (for example `ALL_ONES & 0xff`) would be a compile error, because `&` works on `i64` only. A conversion such as `to_u8` would give a `u8?`, which does not match the declared type `u8`.
3. **Digit separators.** I grouped the hex digits with `_` every four digits. Section 2 allows `_` between any two digits. Leaving them out gives the same value.
4. **Constant body form.** Section 4's example shows a body that is a single indented expression, and I followed that form. Its value is the body's last expression, as with any block.
5. **Printing.** I used two `print` calls, one value per line. A single `print(ALL_ONES, LOW_BYTE)` would print `18446744073709551615255` on one line with no separator (section 11), which is ambiguous. I could also have printed labels with an f-string such as `f"ALL_ONES = {ALL_ONES}"`. The brief only asks for the two values, so I kept it plain.
6. **Doc comments.** I added a `#` comment above each constant. Section 1 says such comments are documentation and have no effect on the program, so leaving them out changes nothing.

# confidence

I am fairly confident it builds and prints `18446744073709551615` and then `255`, because every construct used appears directly in the spec. These are the points I am least sure of:
- The spec never says outright that a constant's declared type is the context for its body's literal. I inferred it from section 2 ("a literal takes the type its context asks for", with the example `b: u8 @ 255`). If the literal were read as `i64` first, `0xffff_ffff_ffff_ffff` would not fit and the build would fail.
- The spec does not say how `print` formats a `u64`. I assumed plain decimal, as for any number.

# context

Nothing other than this directory's files reached my context: I read `brief.md` and `spec.md`. The only other things were the harness's own environment information (working directory, platform, date, and an account email address attached automatically), none of which concerns the language or the task.
