# strconv: `use core/strconv`

Numbers read from text: the loop over the digits the programs of `examples/` wrote by hand before this package existed, admitted by a count of those copies (the admission notes at the end of this file).

## Functions

- `atoi(s: str) -> i64?`, called `strconv.atoi(s)`: The integer `s` writes in decimal, with an optional `+` or `-` before its digits: Go's `strconv.Atoi`.

## Codes

- `does_not_fit`: where the number lies beyond an `i64` (`atoi`).
- `not_a_number`, `strconv.NOT_A_NUMBER`: What `atoi` fails with where a text is not a number at all.
