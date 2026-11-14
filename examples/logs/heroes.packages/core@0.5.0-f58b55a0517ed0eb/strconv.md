# strconv: `use <core>/strconv`

Numbers read from text, where a number too large for its type is a failure the caller answers and never an abort.

## Functions

- `atoi(s: str) -> i64?`, called `strconv.atoi(s)`: The integer `s` writes in decimal, with an optional `+` or `-` before its digits: Go's `strconv.Atoi`.

## Codes

- `does_not_fit`: where the number lies beyond an `i64` (`atoi`).
- `not_a_number`, `strconv.NOT_A_NUMBER`: What `atoi` fails with where a text is not a number at all.
