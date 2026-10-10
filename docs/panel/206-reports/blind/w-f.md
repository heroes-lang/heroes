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

1. **How to write "all bits set".** Section 7 restricts the bitwise operators (`& | ^ ~ << >>`) to `i64` only, so `~0` cannot be typed `u64` or `u8`. I wrote the value as a literal instead. Section 2 says a literal takes the type its context asks for and that "every base writes a value, so a literal must fit its type", so `0xffff_ffff_ffff_ffff` in a `u64` context is the value 2^64 - 1, and `0xff` in a `u8` context is 255. Alternatives:
   - `~0` in the `u64` constant: compile error, since `~` takes `i64` only (and in `i64` it would be -1, a different type).
   - `(~0).to_u64().must()` or similar: `~0` is `i64` -1, which does not fit `u64`, so `to_u64` fails and `.must()` aborts; it also is not a body over "literals and other constants" in the plain sense.
   - `-1` typed `u64`: compile error, the literal does not fit the type (no two's complement reading).
   - Decimal `18446744073709551615` and `255`: equivalent and would build the same; I chose hex because it shows the bit pattern directly.
2. **Digit separators.** I wrote `_` between groups of four hex digits, which section 2 permits "between any two digits". Writing `0xffffffffffffffff` without them is the same value. An `_` directly after `0x` was avoided, since that is not between two digits and would likely be an error.
3. **Constant body form.** Section 4 shows a constant's value as an indented block whose last expression is the value; I followed that example exactly.
4. **How to print.** `print` writes its arguments with no separator and one trailing newline. I used two `print` calls, one per line, printing only the numbers. Alternatives: `print(ALL_ONES, " ", LOW_BYTE)` would give `18446744073709551615 255` on one line; `print(ALL_ONES, LOW_BYTE)` would run them together as `18446744073709551615255`, which is ambiguous; adding labels such as `print("ALL_ONES = ", ALL_ONES)` would add text the brief did not ask for.
5. **Order of declarations.** Section 4 says order does not matter outside FFI groups; I put the constants before `main` for readability.

# confidence

I believe it builds and prints `18446744073709551615` then `255`. Each constant is used in `main`, so no unused-binding error applies; `main` takes nothing and returns nothing; indentation is 4 spaces; no tabs or non-ASCII.

What I am least sure of: whether the compiler types a hex literal by its context before checking its range (section 2 says it does, but the line "One `i64` is `0x1f` ..." describes the default type, and an implementation that range-checked against `i64` first would reject `0xffff_ffff_ffff_ffff`). If that were the case, the decimal form would have the same problem, so no literal spelling would avoid it. A smaller doubt is whether the block body of a `constant` passes its declared type down as the literal's context; the spec's `MAX_DEPTH` example suggests it does.

# context

Only `brief.md` and `spec.md` from this directory were read. Beyond those, my context held the harness's system instructions and an automatically attached note with the user's email address, which I did not use. Nothing else was read and nothing was run.
