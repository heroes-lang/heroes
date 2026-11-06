# strings: `use core/strings`

Text, read and taken apart: what the programs of `examples/` wrote by hand before this package existed, each function admitted by a count of those copies (the admission notes at the end of this file).

## Types

`record Halves` with `before: str` and `after: str`: What `split_once` answers: the text before the separator, and the text after it.

## Functions

- `has_prefix(s: str, prefix: str) -> bool`, called `strings.has_prefix(s: s, prefix: prefix)`: Whether `s` begins with `prefix`.
- `has_suffix(s: str, suffix: str) -> bool`, called `strings.has_suffix(s: s, suffix: suffix)`: Whether `s` ends with `suffix`.
- `contains(s: str, substr: str) -> bool`, called `strings.contains(s: s, substr: substr)`: Whether `substr` occurs in `s`.
- `trim_space(s: str) -> str`, called `strings.trim_space(s)`: `s` without the white space at either end.
- `fields(s: str) -> [str]`, called `strings.fields(s)`: The words of `s`: the pieces between runs of white space, none of them empty, so "  a b  " answers ["a", "b"] and a text of white space alone answers [].
- `split_lines(s: str) -> [str]`, called `strings.split_lines(s)`: The lines of `s`, each without its end.
- `split_once(s: str, sep: str) -> Halves?`, called `strings.split_once(s: s, sep: sep)`: The text of `s` before the first `sep` and the text after it, failing `not_found` where `s` holds no `sep`: `split_once(s: "k=v", sep: "=")` answers `Halves(before: "k", after: "v")`.

## Codes

- `not_found`: where `s` holds no `sep` (`split_once`).
