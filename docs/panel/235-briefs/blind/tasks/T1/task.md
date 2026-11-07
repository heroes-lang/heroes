# The task

Write a Heroes program that prints, for each of the numbers 4, 1987 and 2026,
in that order, one line holding the number, ` = ` and its Roman numeral, as
the expected output shows. Print nothing else.

The numerals come from a package of the Heroes index:

- its name in the index: `roman`
- its version: `2.1.0`
- its fingerprint: `sha256-79151d65cd1d4c2caf5095b19dd286eded992640e674dc3e4ca71650b287fbe9`
- its module `numeral` declares `function to_roman(n: i64) -> str?`: the
  Roman numeral of `n`, failing `out_of_range` where `n` is below 1 or above
  3999.

The program uses that package.

## Expected output

```
4 = IV
1987 = MCMLXXXVII
2026 = MMXXVI
```
