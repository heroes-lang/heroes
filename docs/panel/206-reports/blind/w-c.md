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

1. **How to write "all bits set".** Section 7 says the bitwise operators `& | ^ ~ << >>` work on `i64` only. So `~0` cannot be a `u64` or a `u8`. In a `u64` context it is a type error. Written as an `i64` it is `-1`, and turning that into a `u64` with `to_u64(...)` gives a `T?` that fails because -1 does not fit. A constant body also cannot use `.must()` on a call in any clear way. Arithmetic such as `0 - 1` aborts on overflow at every width (section 7). The only route the spec clearly allows is a literal that holds the full value. Section 2 says a literal takes the type its context asks for, every base writes a value, and the literal must fit its type. `0xffff_ffff_ffff_ffff` is 2^64 - 1, which fits `u64`. `0xff` is 255, which fits `u8`.
2. **Base of the literal.** I used hex because it shows the bit pattern directly. Decimal `18446744073709551615` and `255`, or binary `0b1111_1111` (and a 64-digit binary literal), would give the same values and the same output. Writing one digit too many (for example `0x1_ffff_ffff_ffff_ffff`, or `0x1ff` for the `u8`) would not fit and is a compile error.
3. **Digit separators.** Section 2 allows `_` between any two digits. I grouped by 4 hex digits so the 16 `f`s are easy to count. Leaving them out gives the same value.
4. **Printing.** I used two `print` calls, so each value goes on its own line. One call, `print(ALL_ONES, " ", LOW_BYTE)`, would put both on one line (print adds no separator, so I would have to add the space myself). An `f` string, `print(f"{ALL_ONES} {LOW_BYTE}")`, would do the same through `to_str`. The values themselves print in decimal either way. The spec has no hex formatting.
5. **Constant body form.** I followed the section 4 example: the type on the declaration line and the value on an indented line below it. The grammar (`"constant" ident ":" Type Block`) gives no other form.
6. **No documentation comments.** Section 1 allows `#` comments above declarations. They would change nothing.

# confidence

I think it is likely, but not certain, that the program builds and prints `18446744073709551615` and then `255` on separate lines. The part I am least sure of is whether an unsuffixed literal larger than `i64`'s maximum (`0xffff_ffff_ffff_ffff`) is accepted in a `u64` context. The sentence "One `i64` is `0x1f` ..." could be read as saying the lexer builds every integer literal as an `i64` first. But the same section says a literal takes the type its context asks for and "must fit its type", and that only makes sense if `u64` values above `i64`'s range can be written. If the compiler rejected it, I see no other way the spec offers to build this `u64` constant. Using a constant inside `print` in `main`, and `print` accepting `u64` and `u8` ("a number"), both seem clear. I could not run the compiler, so none of this is tested.

# context

Only `brief.md` and `spec.md` from this directory reached my context, plus the standard harness and system information: the environment description, the working directory path, and an attached account email that I did not use. I read no other files and used no tools except reading and writing files here.
