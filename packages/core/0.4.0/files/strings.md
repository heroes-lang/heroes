# strings: `use <core>/strings`

Text searched, trimmed and split, never cutting a character in two: every position a function here cuts at is a character's first byte, so none aborts on a text a program reads from outside.

## Types

`record Halves` with `before: str` and `after: str`: What `split_once` answers: the text before the separator, and the text after it.

## Functions

- `has_prefix(s: str, prefix: str) -> bool`, called `strings.has_prefix(s: s, prefix: prefix)`: Whether `s` begins with `prefix`; every text begins with "".
- `has_suffix(s: str, suffix: str) -> bool`, called `strings.has_suffix(s: s, suffix: suffix)`: Whether `s` ends with `suffix`; every text ends with "".
- `contains(s: str, substr: str) -> bool`, called `strings.contains(s: s, substr: substr)`: Whether `substr` occurs in `s`; the empty text occurs in every text.
- `trim_space(s: str) -> str`, called `strings.trim_space(s)`: `s` without the white space at either end, white space being Unicode's, as Go's `strings.TrimSpace` reads it: space, tab, line feed and carriage return among others, and the non-breaking space U+00A0.
- `fields(s: str) -> [str]`, called `strings.fields(s)`: The words of `s`: the pieces between runs of white space, none of them empty, so "  a b  " answers ["a", "b"] and a text of white space alone answers [].
- `split_lines(s: str) -> [str]`, called `strings.split_lines(s)`: The lines of `s`, each without its end, "\n" or "\r\n", the last line's end being optional, so "a\nb" and "a\nb\n" both answer ["a", "b"]; "" has no lines, and a "\r" with no "\n" after it ends nothing.
- `split_once(s: str, sep: str) -> Halves?`, called `strings.split_once(s: s, sep: sep)`: The text of `s` before the first `sep` and the text after it, failing `not_found` where `s` holds no `sep`: `split_once(s: "k=v", sep: "=")` answers `Halves(before: "k", after: "v")`.

## Codes

- `not_found`: where `s` holds no `sep` (`split_once`).
